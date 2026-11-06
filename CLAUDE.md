# norns-app — agent guide

Starter template for Norns apps: SvelteKit + Svelte 5 with Pug + Civet (`.n` components, `.c` modules), the `@human-synthesis/norns` runtime (feature folders, DI container, `page` / `route` wrappers with valibot validation) and `@human-synthesis/norns-ui`.

## Layout

```
src/
  hooks.server.c                     boots the runtime; eager-loads src/lib/norns/*/server/module.c
  routes/+page.n, +page.server.c     the one-page demo (form action + list)
  routes/api/messages/+server.c      the same feature as a JSON / TRON endpoint
  lib/components/*.n                 app components (auto-imported by name)
  lib/norns/<feature>/
    server/module.c                  DI registrations — the only file boot() touches
    server/repo.c, service.c         data access, business logic (private to the feature)
    server/public.c                  the ONLY file routes / other features import
    shared/schema.c                  valibot schemas (importable from client and server)
tests/                               bun tests; tests/civet-loader.js lets bun import .c files
```

Add a feature by copying `src/lib/norns/messages/`. Import project code explicitly (`import { messages } from '$lib/norns/messages/server/public'`); only framework helpers (`page`, `route`, `boot`, `redirect`, `error`, `onMount`, `writable`, …) and components (`Btn`, `Form`, your `src/lib/components/*.n`) are auto-imported.

## Commands

```sh
bun run dev            # http://localhost:5173
bun run lint           # norns lint — Civet / Pug pitfall scan (templates + script blocks)
bun run check          # norns check — preprocess + compile every .n / .c; file:line:column errors
bun run build          # full Vite build
bun test               # tests/*.test.js (feature through the DI container)
bunx norns diag <file>            # JS that Civet emits for a .c / .n script
bunx norns diag --template <f.n>  # Svelte source the compiler sees after Pug / Civet / auto-import
bun run check:svelte   # svelte-check over .js / .ts / .svelte only — never reads .n / .c
```

## Working with norns (Civet + Pug + Svelte 5)

Civet is great for app code but has rough edges on advanced syntax. **When something doesn't parse or compile in Civet, drop to plain `.js` for that file — don't fight the parser.** Libraries and dense generator/stream/type code belong in `.js`; routes, components, and feature code stay in `.c`/`.n`.

### Civet pitfalls — do not write these

| Don't | Do | Why |
|---|---|---|
| `if x isnt y` | `if x !== y` | `isnt` compiles to a bare identifier reference at runtime |
| `async *foo()` as class method | callback `foo(onEvent)` or top-level `async function*` | parser rejects async generators in class shorthand |
| `value := $state ''` then later `value = 'x'` | `value .= $state ''` | `:=` creates `const`; reassigning `$state` needs `let` (which `.=` produces) |
| `raw: unknown` / `raw: any` as a let-with-type | `raw .= null` (no annotation) or `let raw: any = null` | bare `Type` annotations get read as identifier references |
| `loop := …`, or any variable named after a Civet keyword | another name (`onward`, `cycle`) | `loop` starts Civet's infinite-loop statement; the parse fails on the next token |
| `if (a)[k]` / `if (a).b()` / `x = y if (a)[k]` | name it first: `m := a`, then `if m[k]` | a parenthesised `if` condition ends at its `)`, so `[k]` / `.b` become the body (and a postfix `if (…)` turns the left side into a call) |
| a class method or facade key named `type` | `getType` | `type` starts a TypeScript alias declaration in Civet |

### Pug / `.n` pitfalls — do not write these

| Don't | Do | Why |
|---|---|---|
| `{@html foo}` as a top-level Pug line | `\| {@html foo}` (pipe-prefix) | Pug parses leading `{` as a malformed tag |
| `#{expr}` interpolation in template | `{expr}` (Svelte interpolation) | `#{...}` evaluates at preprocess time, with no runtime data in scope; SSR 500s |
| `attr="#{expr}"` attribute interpolation | `attr!="{expr}"` (Svelte) | same reason |
| `+each('row of rows')` | `+each('rows as row')` (Svelte `as` form, optionally `(row.id)` key) | the `of` form is copied verbatim into the block and the Svelte compiler rejects it |
| `#{expr}` or `.a.b` shorthand across a line break | keep one element per line | Pug is line-oriented; parse errors surface on the *next* line |

Template syntax that works: `+if('cond')` / `+elseif('cond')` / `+else`, `+each('items as item (item.id)')`, `+snippet('name', arg)` with `| {@render name(x)}`, `attr!="{expr}"` for Svelte expressions, `.a.b` class shorthand (Tailwind variants and fractions like `.hover:bg-x.gap-2.5` are rewritten for you).

### SvelteKit / norns gotchas

- **Workers forbid randomness and I/O in global scope.** `randomBytes()`, `crypto.getRandomValues`, `fetch` or timers at module top level crash every request under workerd (`Disallowed operation called within global scope`) while working under Bun and Vite. Compute such values lazily inside a handler or function, and smoke-test the production build with `wrangler dev`.
- **norns-ui `Btn` and `Form` do not forward unknown attributes.** `formaction`, `oninput`, `onchange`, `data-*` on them are silently dropped. Use a plain `button.btn.btn-primary(type="submit" formaction="?/publish")`, and put input listeners on a wrapper element inside the form (events bubble).
- **norns-ui `Tabs` renders only the active panel.** Fields on other tabs are not in the DOM, so a form spanning tabs loses them on submit. For tabbed forms, toggle `hidden` on panels you render yourself.
- **`event.locals.container`** is the per-request DI scope set by `contextHandle` in `boot()`. Don't destructure DI directly from event args.
- **Body parsing is content-type-driven.** `+page.server.c` actions get `formData()`; client `fetch` posting JSON must hit a `+server.c` endpoint, not a form action.
- **`svelte-check` never reads `.n` or `.c` files.** `bun run check:svelte` only covers `.js` / `.ts` / `.svelte`; in a workspace checkout it also reports framework-source errors reached through symlinks. It is not the pass signal for Norns code — `norns check` is.
- **Type-check is not feature verification.** Verify through the request path: `curl` against `bun run dev`, hitting POST actions and not just GET pages — that's where Bun-vs-Node and Pug-vs-Svelte issues surface.

### Verification workflow — run before claiming done

1. **`bun run lint`** — must show 0 errors. `--json` for machine-readable output.
2. **`bun run check`** — must exit 0. Pug and Civet errors point at the line you wrote; Svelte errors inside Pug-rendered markup are reported against the preprocessed output (see `norns diag --template`). `--json` available.
3. **`bun test`** — when you touched a feature folder.
4. **`bun run build`** — a few seconds; the final word on whether everything compiles.
5. **`curl` through the dev server** — for any request-path change. Hit the real route, including POST/form actions.

### Things AI tends to get wrong here

- **Don't introduce ORMs.** SQL via `better-sqlite3`, hand-written.
- **Don't add `bcrypt`/`argon2`.** Passwords via `node:crypto.scrypt`. `Bun.password.hash` is undefined when Vite launches under Node.
- **Don't call `new Database(...)` in multiple modules.** A single owner (a feature's `server/module.c`) registers `db` in the DI container; everyone else calls `c.resolve('db')`.
- **In `vite.config.js`, set `server.allowedHosts: true`** when behind any reverse proxy in dev (otherwise Vite blocks with HTML "Blocked request").
- **Container env vars are read at create time.** `podman-compose restart` does NOT pick up `.env` changes — use `up -d --force-recreate <service>`.
