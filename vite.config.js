import { defineConfig } from 'vite';
import { sveltekit } from '@sveltejs/kit/vite';
import adapter from '@sveltejs/adapter-auto';
import tailwindcss from '@tailwindcss/vite';
import { nornsAutoImport } from '@human-synthesis/norns/auto-import';
import { nornsConfig } from '@human-synthesis/norns/config';
import { nornsPreprocess } from '@human-synthesis/norns/preprocess';
import { nornsCivetPlugin, pugTailwindExtract } from '@human-synthesis/norns/vite';
import { presetUI } from '@human-synthesis/norns-ui/auto-import';

const ui = presetUI();

export default defineConfig({
	plugins: [
		nornsCivetPlugin(),
		// Tailwind v4's content scanner doesn't understand Pug's chained-class
		// shorthand (`.flex.items-center.p-4` reads as one dotted token and
		// gets dropped). This plugin walks `.n` files, extracts class candidates,
		// and writes them to node_modules/.cache/norns/tailwind-pug-classes.html,
		// which app.css references via `@source`.
		pugTailwindExtract(),
		// Vite-plugin half of the auto-importer, for standalone .c modules.
		// `helpers` is omitted so the defaults apply; `exportGlobs` is not
		// enabled, so project code (facades, schemas, services) is always
		// imported explicitly.
		nornsAutoImport({
			components: ui.components
		}),
		tailwindcss(),
		// SvelteKit 3 takes its config here (svelte.config.js is gone).
		// nornsConfig adds .n components, .c/.civet modules and hooks, the
		// $lib alias and the Pug + Civet preprocessor; the preprocessor half of
		// the auto-importer (for .n / .svelte files) is registered here too.
		sveltekit(
			nornsConfig({
				preprocess: [
					...nornsPreprocess(),
					nornsAutoImport({
						componentDirs: ['src/lib/components', 'src/routes'],
						components: ui.components
					})
				],
				adapter: adapter()
			})
		)
	],
	// In workspace mode @human-synthesis/norns is a symlink into the kit fork,
	// which has its own @sveltejs/kit under pnpm. Left external, its server
	// modules would load THAT copy (realpath resolution) and crash with
	// "Could not get the request store" — two kit instances, two
	// AsyncLocalStorage worlds. Bundling norns through Vite dedupes its kit
	// imports onto the app's single copy.
	// Accept reverse-proxied Host headers in dev (norns lint: vite/allowed-hosts).
	server: { allowedHosts: true },
	resolve: { dedupe: ['@sveltejs/kit'] },
	ssr: { noExternal: ['@human-synthesis/norns'] }
});
