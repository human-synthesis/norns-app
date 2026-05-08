import { fail, redirect } from '@sveltejs/kit'
import { listNotes, createNote } from './db'

export load = ->
	notes: listNotes()

export actions =
	create: ({ request }) ->
		data = await request.formData()
		title = data.get('title')?.toString().trim()
		body = data.get('body')?.toString() ? ''
		return fail 400, { error: 'title is required', body } unless title
		id = createNote title, body
		throw redirect 303, "/examples/norns/notes/#{id}"
