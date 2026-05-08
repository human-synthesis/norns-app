import { defineConfig } from 'vite';
import { sveltekit } from '@sveltejs/kit/vite';
import tailwindcss from '@tailwindcss/vite';
import { nornsCivetPlugin } from '@human-synthesis/norns/vite';

export default defineConfig({
	plugins: [nornsCivetPlugin(), tailwindcss(), sveltekit()]
});
