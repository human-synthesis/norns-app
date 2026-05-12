import { defineConfig } from 'vite';
import { sveltekit } from '@sveltejs/kit/vite';
import tailwindcss from '@tailwindcss/vite';
import { nornsAutoImport } from '@human-synthesis/norns/auto-import';
import { nornsCivetPlugin, pugTailwindExtract } from '@human-synthesis/norns/vite';
import { presetUI } from '@human-synthesis/norns-ui/auto-import';

const ui = presetUI();

export default defineConfig({
	plugins: [
		nornsCivetPlugin(),
		// Tailwind v4's content scanner doesn't understand Pug's chained-class
		// shorthand (`.flex.items-center.p-4` reads as one dotted token and
		// gets dropped). This plugin walks `.n` files, extracts class candidates,
		// and writes them to `src/.tailwind-pug-classes.html`. app.css already
		// references that file via `@source`. See the README for details.
		pugTailwindExtract(),
		// `helpers` is omitted (defaults preserved). When the UI preset starts
		// shipping helpers (e.g. `toast()` in Phase 3), nornsAutoImport will
		// need a `presets` or `additionalHelpers` option to merge with defaults
		// without replacing them. Until then, `components` is the only preset
		// channel.
		nornsAutoImport({
			exportGlobs: ['src/lib/**/public.c'],
			components: ui.components
		}),
		tailwindcss(),
		sveltekit()
	]
});
