import adapter from '@sveltejs/adapter-auto';
import { nornsConfig } from '@human-synthesis/norns/config';

export default nornsConfig({
	kit: {
		adapter: adapter()
	}
});
