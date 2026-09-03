// Spec-first boot (D45): serializer and dev seed come from the spec's
// `app.settings` (generated into $lib/app/settings.c) — this file keeps only
// the wiring the spec genuinely cannot say: env-specific db + auth handles.
import { boot, applyMigrations, betterSqlite, createLive, d1, seedDev } from '@human-synthesis/norns/server'
import { tronSerializer } from '@human-synthesis/norns-tron/server'
import { env } from '$env/dynamic/private'

import { createAuth } from './auth.c'
import { SETTINGS } from '$lib/app/settings.c'

triggerFiles := import.meta.glob '/.norns/generated/lib/*/triggers.c', { eager: true }
triggers := Object.values(triggerFiles).map (m) => m.triggers ?? []
schemaFiles := import.meta.glob '/.norns/generated/lib/*/schema.c', { eager: true }

// Local dev: SQLite under .norns/ (gitignored), migrations applied on boot,
// spec-declared seed rows on first run. Production D1 migrates via
// `wrangler d1 migrations apply` (generate mirrors migrations/<module>/*.sql
// into the flat dir the generated wrangler.json points at).
devDb .= undefined
if import.meta.env.DEV
	devDb = await betterSqlite '.norns/dev.db'
	await applyMigrations devDb, 'migrations'
	await seedDev devDb, schemaFiles, SETTINGS.seed

// On Cloudflare the D1 and ROOM bindings only exist per-request, so the
// scoped container gets its db — and, when live queries bound a Room, its
// Room-backed live bridge — here rather than at boot. The two have the
// same lifetime; wiring one without the other leaves live silently local.
d1Handle := async ({ event, resolve }) =>
	if event.platform?.env?.DB
		db := await d1 event.platform.env.DB
		event.locals.container.single 'db', => db
	if event.platform?.env?.ROOM
		room := event.platform.env.ROOM
		event.locals.container.single 'live', =>
			createLive { events: event.locals.container.resolve('events'), room }
	resolve event

// Auth is opt-in: set BETTER_AUTH_SECRET (and run the better-auth CLI
// migration once) to enable it. Without it the app is OPEN — every request,
// in dev and in production alike, acts as the `dev` admin, so `$user` writes
// and owner/role policies keep working instead of failing on a missing user
// (D86). Configure auth before exposing real data.
//
// With the secret set: dev builds better-auth over the local SQLite handle;
// on Workers the D1 binding exists only per request, so the instance is
// built once per binding from the request's DB (authHandle accepts a factory).
authCache := new WeakMap()
productionAuth := async (event) =>
	binding := event.platform?.env?.DB
	if !binding then return undefined
	if !authCache.has(binding) then authCache.set(binding, createAuth({ db: await d1(binding), env }))
	authCache.get(binding)

auth := env.BETTER_AUTH_SECRET ? (devDb ? createAuth({ db: devDb, env }) : productionAuth) : undefined

openUserHandle := async ({ event, resolve }) =>
	event.locals.user ?= { id: 'dev', roles: ['admin'] }
	resolve event

opts := {
	triggers
	serializer: SETTINGS.serializer === 'tron' ? tronSerializer() : undefined
	cronShim: import.meta.env.DEV
	extraHandle: auth ? d1Handle : [d1Handle, openUserHandle]
}
if auth then opts.auth = auth

app := await boot opts

if devDb then app.container.single 'db', => devDb

{ handle, handleError } := app
export { handle, handleError }
