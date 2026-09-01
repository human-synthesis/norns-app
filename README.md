# norns-app

Spec-first starter for [Norns](https://github.com/human-synthesis/norns) apps.

You write **specs** (TRON) and, when needed, small **custom bodies** (Civet).
`norns generate` turns the specs into the entire SvelteKit app — schema,
queries, actions, policies, state machines, pages, wrangler config — under the
gitignored `.norns/generated/` tree. Nothing in that tree is edited by hand.

## Layout

```
specs/            canonical app definition (TRON) — app.tron + one file per module
src/              custom code only
  hooks.server.c  runtime wiring: db, triggers, serializer, optional auth
  auth.c          better-auth factory (opt-in via BETTER_AUTH_SECRET)
  tasks/actions/retitle.c   custom body for the `impl: custom` action
  app.css         theme — imported by the generated root layout
migrations/       committed SQL, produced by `norns migrate gen`
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
bunx norns trace         # runs every action example against sandboxed SQLite
```

## The starter spec

`specs/tasks.tron` defines a `Task` entity (status machine `open → done`),
a query, two actions and a page:

- `tasks.Action.complete` — declarative: guard `status == open`, sets status,
  emits `task.completed`.
- `tasks.Action.retitle` — `impl: custom`; its body lives at
  `src/tasks/actions/retitle.c` and is called by the generated shell after
  guards and policies have run.
- The `/` page binds `tasks.Query.open` to the norns-ui `Table` component;
  bindings are validated at generate time against the props contracts
  exported by `@human-synthesis/norns-ui/contracts`.

## Auth (optional)

Set `BETTER_AUTH_SECRET` in `.env` and create better-auth's tables once with
`bunx @better-auth/cli migrate`. Without the secret the app runs open.

## Deploy (Cloudflare)

```sh
bun run build
bunx wrangler d1 migrations apply <db> --remote
bunx wrangler deploy -c .norns/generated/wrangler.json
```

`wrangler.json` is generated from the app spec (D1 binding, crons, R2 when a
`file` field exists). Set `settings.cloudflare.d1_id` in `specs/app.tron`.
