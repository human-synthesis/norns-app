import adapter from '@sveltejs/adapter-auto';
import { nornsPreprocess } from '@human-synthesis/norns-core/preprocess';

export default {
	preprocess: nornsPreprocess(),
	kit: {
		adapter: adapter(),
		moduleExtensions: ['.js', '.ts', '.coffee']
	}
};
