# norns-app

**AI-driven software architecture and development framework, based on Svelte — starter template.**

## Stack

- [Svelte 5](https://svelte.dev) — components and runes
- [SvelteKit 2](https://kit.svelte.dev) — file-system routing, SSR, endpoints
- [Pug](https://pugjs.org) — templates (in `.n` files)
- [Civet](https://civet.dev) — script language (in `.n` `<script>` blocks and `.c` files)
- [Tailwind CSS v4](https://tailwindcss.com) — styling
- [Vite](https://vitejs.dev) — bundler
- [bun](https://bun.sh) — runtime / package manager
- [`@human-synthesis/norns`](https://github.com/human-synthesis/norns) + [`norns-core`](https://github.com/human-synthesis/norns-core) — the framework
- [valibot](https://valibot.dev) — input validation
- [better-sqlite3](https://github.com/WiseLibs/better-sqlite3) — local SQLite

## Setup

```sh
bun create human-synthesis/norns-app my-app
cd my-app
bun install
bun run migrate up   # apply migrations to data/notes.db (or whatever DATABASE_URL points to)
```

## Run

```sh
bun run dev          # dev server at http://localhost:5173
bun run build        # production build
bun run preview      # preview production build

bun run migrate up        # apply pending migrations
bun run migrate status    # list applied + pending
bun run migrate create <feature>/<name>   # scaffold a new migration

bun run check:arch   # verify feature-folder boundaries (no internals leaks)
bun run compare      # side-by-side LOC/char comparison: norns vs vanilla SvelteKit
```

## What's in here

```
src/
  hooks.server.c                    # boots the norns runtime
  +layout.c, +layout.n              # app shell
  routes/
    +page.n                         # home
    examples/
      +page.n
      norns/                        # ← Norns version of the examples
        +page.n
        notes/                      # SQLite + form actions + DI + valibot
          +page.n / +page.server.c
          [id]/+page.n, +page.server.c, +error.n
        tic-tac-toe/                # components + Svelte stores + AI heuristic
          +page.n, Game.n, Board.n, Cell.n, store.c, ai.c
      svelte/                       # ← Vanilla SvelteKit version of the same examples
        notes/, tic-tac-toe/
  lib/
    components/Header.n
    norns/                          # framework-managed feature folders
      notes/
        server/{module,repo,service,public}.c
        shared/schema.c
    svelte/                         # vanilla version of the data layer
      notes/{db,repo,service,schema}.ts
migrations/
  notes/                            # SQL migrations, applied via `bun run migrate up`
```

## Two side-by-side examples

The `examples/` folder is intentionally minimal — two apps written twice:

- **Notes** — SQLite, dynamic routes, form actions, valibot validation, error pages, SSR `load`. Norns side uses the runtime's DI container, public.c boundary, and route/page wrappers; vanilla side uses direct module imports and ad-hoc validation.
- **Tic-tac-toe** — multi-component composition, `$props` with defaults, Svelte stores (`writable` / `derived`), `$state`/`$effect` runes, scoped CSS, AI heuristic.

Run `bun run compare` for the line/char delta.

## License

MIT © Daniel Teodoroiu / [Human Synthesis](https://humansynthesis.ai). Built on top of [SvelteKit](https://github.com/sveltejs/kit) and [Svelte](https://github.com/sveltejs/svelte) © Svelte Contributors, MIT licensed.
