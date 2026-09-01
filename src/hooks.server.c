// Spec-first boot: the generated tree (.norns/generated) owns routes and lib;
// this file only wires runtime services — db, triggers, serializer, auth —
// around it.
import { boot, applyMigrations, betterSqlite, d1 } from '@human-synthesis/norns/server'
import { tronSerializer } from '@human-synthesis/norns-tron/server'
import { env } from '$env/dynamic/private'

import { createAuth } from './auth.c'
import { Task } from '$lib/tasks/schema.c'

// Generated trigger tables — one per module that declares triggers.
// Event triggers feed the in-process bus; schedule ones run on the cron shim
// locally and on Cloudflare cron triggers in production.
triggerFiles := import.meta.glob '/.norns/generated/lib/*/triggers.c', { eager: true }
triggers := Object.values(triggerFiles).map (m) => m.triggers ?? []

// Local dev: SQLite under .norns/ (gitignored), migrations applied on boot,
// sample rows seeded on first run. Production D1 migrates via
// `wrangler d1 migrations apply` instead.
devDb .= undefined
if import.meta.env.DEV
	devDb = await betterSqlite '.norns/dev.db'
	await applyMigrations devDb, 'migrations'
	existing := await devDb.select().from(Task).limit(1)
	if existing.length === 0
		await devDb.insert(Task).values [
			{ id: crypto.randomUUID(), owner: 'dev', title: 'Edit specs/tasks.tron and watch this page regenerate' }
			{ id: crypto.randomUUID(), owner: 'dev', title: 'Run `bunx norns trace` to execute the action examples' }
		]

// On Cloudflare the D1 binding only exists per-request, so the scoped
// container gets its db here rather than at boot.
d1Handle := async ({ event, resolve }) =>
	if event.platform?.env?.DB
		db := await d1 event.platform.env.DB
		event.locals.container.single 'db', => db
	resolve event

// Auth is opt-in: set BETTER_AUTH_SECRET (and run the better-auth CLI
// migration once) to enable it. Without it, dev requests act as the seeded
// `dev` admin so policy-filtered queries stay visible; production without
// auth serves anonymous requests (policies filter everything out).
auth := devDb and env.BETTER_AUTH_SECRET ? createAuth({ db: devDb, env }) : undefined

devUserHandle := async ({ event, resolve }) =>
	event.locals.user ?= { id: 'dev', roles: ['admin'] }
	resolve event

opts := {
	triggers
	serializer: tronSerializer()
	cronShim: import.meta.env.DEV
	extraHandle: import.meta.env.DEV and !auth ? [d1Handle, devUserHandle] : d1Handle
}
if auth then opts.auth = auth

app := await boot opts

if devDb then app.container.single 'db', => devDb

{ handle, handleError } := app
export { handle, handleError }
