import { defineConfig } from 'vite';
import { sveltekit } from '@sveltejs/kit/vite';
import { nornsCoffeePlugin } from '@human-synthesis/norns/vite';
import { nornsUno } from '@human-synthesis/norns-core/uno';

export default defineConfig({
	plugins: [nornsCoffeePlugin(), nornsUno(), sveltekit()]
});
