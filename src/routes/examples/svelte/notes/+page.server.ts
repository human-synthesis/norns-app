import { fail, redirect } from '@sveltejs/kit';
import type { Actions, PageServerLoad } from './$types';
import { listNotes, createNote } from './db';

export const load: PageServerLoad = () => {
	return { notes: listNotes() };
};

export const actions: Actions = {
	create: async ({ request }) => {
		const data = await request.formData();
		const title = data.get('title')?.toString().trim();
		const body = data.get('body')?.toString() ?? '';
		if (!title) return fail(400, { error: 'title is required', body });
		const id = createNote(title, body);
		throw redirect(303, `/examples/svelte/notes/${id}`);
	}
};
