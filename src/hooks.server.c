// Spec-first boot (D45): serializer and dev seed come from the spec's
// `app.settings` (generated into $lib/app/settings.c) — this file keeps only
// the wiring the spec genuinely cannot say: env-specific db + auth handles.
import { boot, applyMigrations, betterSqlite, d1, seedDev } from '@human-synthesis/norns/server'
import { tronSerializer } from '@human-synthesis/norns-tron/server'
import { env } from '$env/dynamic/private'

import { createAuth } from './auth.c'
import { SETTINGS } from '$lib/app/settings.c'

triggerFiles := import.meta.glob '/.norns/generated/lib/*/triggers.c', { eager: true }
triggers := Object.values(triggerFiles).map (m) => m.triggers ?? []
schemaFiles := import.meta.glob '/.norns/generated/lib/*/schema.c', { eager: true }

// Local dev: SQLite under .norns/ (gitignored), migrations applied on boot,
// spec-declared seed rows on first run. Production D1 migrates via
// `wrangler d1 migrations apply` instead.
devDb .= undefined
if import.meta.env.DEV
	devDb = await betterSqlite '.norns/dev.db'
	await applyMigrations devDb, 'migrations'
	await seedDev devDb, schemaFiles, SETTINGS.seed

// On Cloudflare the D1 binding only exists per-request, so the scoped
// container gets its db here rather than at boot.
d1Handle := async ({ event, resolve }) =>
	if event.platform?.env?.DB
		db := await d1 event.platform.env.DB
		event.locals.container.single 'db', => db
	resolve event

// Auth is opt-in: set BETTER_AUTH_SECRET (and run the better-auth CLI
// migration once) to enable it. Without it, dev requests act as the seeded
// `dev` admin so policy-filtered queries stay visible.
auth := devDb and env.BETTER_AUTH_SECRET ? createAuth({ db: devDb, env }) : undefined

devUserHandle := async ({ event, resolve }) =>
	event.locals.user ?= { id: 'dev', roles: ['admin'] }
	resolve event

opts := {
	triggers
	serializer: SETTINGS.serializer === 'tron' ? tronSerializer() : undefined
	cronShim: import.meta.env.DEV
	extraHandle: import.meta.env.DEV and !auth ? [d1Handle, devUserHandle] : d1Handle
}
if auth then opts.auth = auth

app := await boot opts

if devDb then app.container.single 'db', => devDb

{ handle, handleError } := app
export { handle, handleError }
