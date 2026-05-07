# norns-app

**AI-driven software architecture and development framework, based on Svelte — starter template.**

## Stack

- [Svelte 5](https://svelte.dev) — components and runes
- [SvelteKit 2](https://kit.svelte.dev) — file-system routing, SSR, endpoints
- [Pug](https://pugjs.org) — templates
- [CoffeeScript 2](https://coffeescript.org) — script
- [Tailwind CSS v4](https://tailwindcss.com) — styling
- [Vite](https://vitejs.dev) — bundler
- [bun](https://bun.sh) — runtime / package manager
- [`@human-synthesis/norns`](https://github.com/human-synthesis/norns) + [`norns-core`](https://github.com/human-synthesis/norns-core) — the framework

## Setup

```sh
bun create human-synthesis/norns-app my-app
cd my-app
bun install
```

## Run

```sh
bun run dev      # dev server at http://localhost:5173
bun run build    # production build
bun run preview  # preview production build
```

## License

MIT © Daniel Teodoroiu / [Human Synthesis](https://humansynthesis.ai). Built on top of [SvelteKit](https://github.com/sveltejs/kit) and [Svelte](https://github.com/sveltejs/svelte) © Svelte Contributors, MIT licensed.
