import adapter from '@sveltejs/adapter-auto';
import { nornsAutoImport } from '@human-synthesis/norns/auto-import';
import { nornsConfig } from '@human-synthesis/norns/config';
import { nornsPreprocess } from '@human-synthesis/norns/preprocess';
import { presetUI } from '@human-synthesis/norns-ui/auto-import';

const ui = presetUI();

export default nornsConfig({
	preprocess: [
		...nornsPreprocess(),
		nornsAutoImport({
			componentDirs: ['src/lib/components', 'src/routes'],
			exportDirs: ['src/lib', 'src/routes'],
			components: ui.components
		})
	],
	kit: {
		adapter: adapter()
	}
});
