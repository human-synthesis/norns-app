# norns-app

**Starter template for Norns apps.**

A single page that exercises the full Norns runtime — feature-folder modularity, DI, valibot validation, server actions, Pug + Civet — in roughly 60 LOC of code you can read in one sitting and rewrite in two.

## Stack

- [Svelte 5](https://svelte.dev) — components and runes
- [SvelteKit 2](https://kit.svelte.dev) — file-system routing, SSR, endpoints
- [Pug](https://pugjs.org) — templates (in `.n` files)
- [Civet](https://civet.dev) — script language (in `.n` `<script>` blocks and `.c` files)
- [Tailwind CSS v4](https://tailwindcss.com) — styling
- [Vite](https://vitejs.dev) — bundler
- [bun](https://bun.sh) — runtime / package manager
- [`@human-synthesis/norns`](https://github.com/human-synthesis/norns) + [`norns-core`](https://github.com/human-synthesis/norns-core) — the framework
- [`@human-synthesis/norns-ui`](https://github.com/human-synthesis/norns-ui) — UI components
- [valibot](https://valibot.dev) — input validation

## Setup

```sh
bun create human-synthesis/norns-app my-app
cd my-app
bun install
```

## Run

```sh
bun run dev          # dev server at http://localhost:5173
bun run build        # production build
bun run preview      # preview production build
```

## What's in here

```
src/
  hooks.server.c                # boots the norns runtime, eager-loads feature modules
  app.css, app.html             # global styles + html shell
  routes/
    +layout.c, +layout.n        # app shell with the <Header>
    +page.n                     # one-page demo: form + list of messages
    +page.server.c              # load() + send action wired to the messages feature
  lib/
    components/Header.n         # site nav
    norns/
      messages/                 # one feature folder, in-memory store
        server/{module,repo,service,public}.c
```

## The starter feature

`src/lib/norns/messages/` is a complete Norns feature folder, in miniature:

- **`repo.c`** — in-memory `list` / `add` / `clear`. Replace with Drizzle, better-sqlite3, D1, or anything else when you need persistence.
- **`service.c`** — valibot schema + `create` / `list` business logic. Errors flow back through `fail(400, { errors })` and end up rendered by `<Form form={form}> + <Field name="text">`.
- **`module.c`** — DI registrations. The only file `boot()` reaches; everything else stays private to the feature.
- **`public.c`** — `import { messages }` from this is how routes / other features call into the service.

To see how it's wired:

- `src/routes/+page.server.c` calls `messages().create({ text })` and `messages().list()`.
- `src/routes/+page.n` renders `<Form>` from `@human-synthesis/norns-ui`. Field-level errors arrive via `form?.errors` and the Form-context error map; no per-page boilerplate.

## Going further

Delete `src/lib/norns/messages/` and `src/routes/+page.*` and rewrite. Or copy the feature folder for each new domain you add — that's the whole pattern.

Examples + a side-by-side LOC comparison vs vanilla SvelteKit live in [norns-demo](https://github.com/human-synthesis/norns-demo).

## License

MIT © Daniel Teodoroiu / [Human Synthesis](https://humansynthesis.ai). Built on top of [SvelteKit](https://github.com/sveltejs/kit) and [Svelte](https://github.com/sveltejs/svelte) © Svelte Contributors, MIT licensed.
