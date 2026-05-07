# norns-app

Starter template for [Norns](https://github.com/human-synthesis/norns) apps. Clone, install, run.

## Stack

- [Norns](https://github.com/human-synthesis/norns) — SvelteKit fork
- CoffeeScript (`<script lang="coffee">` + `+page.coffee`)
- Pug (`<template lang="pug">`)
- UnoCSS (`presetUno`, `presetAttributify`, `presetIcons`, `presetTypography`)
- Bun

## Get started

```sh
git clone https://github.com/human-synthesis/norns-app my-app
cd my-app
bun install
bun run dev
```

Then edit `src/routes/+page.svelte` and start building.

## Local dev against unpublished Norns

While Norns isn't on npm yet, `package.json` uses `file:` links to sibling repos:

```
human-synthesis/
├── norns-core/   # Svelte fork
├── norns/        # SvelteKit fork
└── norns-app/    # this template
```

If you've cloned all three side-by-side, `bun install` will resolve them locally. Once published to npm, the `file:` paths will be replaced with semver ranges and only `norns-app` is needed.

## What's where

- `src/routes/+layout.svelte` — top-level layout, imports `virtual:uno.css`
- `src/routes/+page.svelte` — landing page (Coffee + Pug + Uno smoke test)
- `svelte.config.js` — uses `nornsConfig()` from `@human-synthesis/norns`
- `vite.config.js` — wires `nornsCoffeePlugin()` and `nornsUno()`
