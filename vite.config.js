import { defineConfig } from 'vite';
import { sveltekit } from '@sveltejs/kit/vite';
import tailwindcss from '@tailwindcss/vite';
import { nornsAutoImport } from '@human-synthesis/norns/auto-import';
import { nornsCivetPlugin } from '@human-synthesis/norns/vite';

export default defineConfig({
	plugins: [
		nornsCivetPlugin(),
		nornsAutoImport({ exportDirs: ['src/lib', 'src/routes'] }),
		tailwindcss(),
		sveltekit()
	]
});
