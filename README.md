# norns-app

Spec-first starter for [Norns](https://github.com/human-synthesis/norns) apps.

You write **specs** (TRON) and, when needed, small **custom bodies** (Civet).
`norns generate` turns the specs into the entire SvelteKit app — schema,
queries, actions, policies, state machines, pages, wrangler config — under the
gitignored `.norns/generated/` tree. Nothing in that tree is edited by hand.

## Layout

```
specs/            canonical app definition (TRON) — app.t + one file per module
                  (incl. `settings`: serializer, dev seed rows, shell nav/brand).
                  Starts EMPTY: app.t with no modules; `/` shows a blank-state
                  screen until the first spec Page exists
src/              custom code only
  hooks.server.c  env wiring the spec can't say: db split, D1 handle, opt-in auth
                  (serializer + dev seed come from the spec via $lib/app/settings.c)
  auth.c          better-auth factory (opt-in via BETTER_AUTH_SECRET)
  <module>/…      custom bodies appear here as specs point at them
  app.css         BUILD WIRING ONLY (tailwind entry) — never add styles here;
                  theme = `app.settings.tokens`, everything else = the owning
                  Component/Snippet body's scoped <style>
migrations/       committed SQL, produced by `norns migrate gen` (absent until then)
tests/smoke.test.js   13-line bridge — e2e is DERIVED from the specs (smoke
                  matrix); grow coverage with `pages.<name>.expect`, not test code
.norns/           generated output + dev SQLite (gitignored)
```

## Develop

```sh
bun install
bun run dev            # regenerates from specs/ on change, serves the app
```

`norns dev` watches `specs/`, regenerates on save, and refuses to emit code
that violates the spec (bad addresses, invalid page bindings, unsound
guards). Local requests run against SQLite at `.norns/dev.db`; migrations in
`migrations/` are applied automatically on boot.

After changing entities:

```sh
bunx norns migrate gen   # writes migrations/<module>/*.sql — commit these
bunx norns trace         # runs every example + derived cases (illegal status
                         # transitions, permission/IDOR matrix) against the
                         # generated shells in sandboxed SQLite
bun test                 # the derived smoke matrix: every page + 404 control
```

## Starting from empty

The starter carries no example: `specs/app.t` declares no modules, so the
generated app is a shell with a blank-state `/`. Your first module is one file
— `specs/<module>.t` with entities, queries, actions, policies and pages — plus
its name in `app.t`'s `modules` list; through norns-mcp that is one
`spec_apply`. The first Page replaces the blank screen; the first
`impl: custom` unit gets its body at `src/<module>/<kind>s/<name>.c`.
Bindings are validated at generate time against the props contracts exported
by `@human-synthesis/norns-ui/contracts`.

## Live queries (cross-device sync)

Mark any query `"live": true` and the generator does the rest: a `/_norns/live`
SSE route, `depends` keys in the load, and the subscription injected into the
generated page shell — a task created in one browser appears in another with no
polling and no code in your bodies, because every action's `refresh` list
already publishes through the same bridge. On Cloudflare the `ROOM` Durable
Object binding is per-request exactly like `DB`: `src/hooks.server.c` rebinds
the `live` bridge beside `db`, and the generated `worker.js` (pointed at by
`wrangler.json`'s `main`) exports the `NornsRoom` class the config binds —
SQLite-backed (`new_sqlite_classes`), so it works on the free plan.

## Auth (optional)

Set `BETTER_AUTH_SECRET` in `.env` and create better-auth's tables once with
`bunx @better-auth/cli migrate`. Without the secret the app runs open.

## Deploy (Cloudflare)

```sh
bunx wrangler d1 create <app>-db            # once; put the returned id in the spec (below)
bunx wrangler d1 migrations apply <app>-db --remote
bun run deploy                              # build + wrangler deploy -c .norns/generated/wrangler.json
```

`wrangler.json` is generated from the app spec (D1 binding, crons, R2 when a
`file` field exists). The D1 id lives in the spec at `app.settings.cloudflare.d1_id`
— through the MCP that is one op:
`{op: "set", path: "app.settings.cloudflare.d1_id", value: "<uuid>"}` (the `app.`
prefix is required; `settings.…` alone is not a spec path). Migrations: generate
mirrors `migrations/<module>/*.sql` into `.norns/generated/migrations/` in the flat,
numbered layout wrangler tracks, so `d1 migrations apply` sees them.
