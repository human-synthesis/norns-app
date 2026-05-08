import { error, fail, redirect } from '@sveltejs/kit'
import { getNote, updateNote, deleteNote } from '../db'

export load = ({ params }) ->
	id = Number params.id
	note = getNote id
	throw error(404, 'note not found') unless note
	{ note }

export actions =
	update: ({ request, params }) ->
		id = Number params.id
		data = await request.formData()
		title = data.get('title')?.toString().trim()
		body = data.get('body')?.toString() ? ''
		return fail 400, { error: 'title is required' } unless title
		updateNote id, title, body
		{ saved: true }

	delete: ({ params }) ->
		deleteNote Number(params.id)
		throw redirect(303, '/examples/norns/notes')
