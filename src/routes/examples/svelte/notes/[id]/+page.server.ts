import { error, fail, redirect } from '@sveltejs/kit';
import type { Actions, PageServerLoad } from './$types';
import { getNote, updateNote, deleteNote } from '../db';

export const load: PageServerLoad = ({ params }) => {
	const id = Number(params.id);
	const note = getNote(id);
	if (!note) throw error(404, 'note not found');
	return { note };
};

export const actions: Actions = {
	update: async ({ request, params }) => {
		const id = Number(params.id);
		const data = await request.formData();
		const title = data.get('title')?.toString().trim();
		const body = data.get('body')?.toString() ?? '';
		if (!title) return fail(400, { error: 'title is required' });
		updateNote(id, title, body);
		return { saved: true };
	},

	delete: ({ params }) => {
		deleteNote(Number(params.id));
		throw redirect(303, '/examples/svelte/notes');
	}
};
