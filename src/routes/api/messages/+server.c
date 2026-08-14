import { route } from '@human-synthesis/norns/server'
import { messages } from '$lib/norns/messages/server/public'
import { sendMessageSchema } from '$lib/norns/messages/server/service'

// GET /api/messages
//
// Responses are content-negotiated by the app-wide serializer (see
// hooks.server.c): `Accept: application/tron` gets TRON, anything else gets
// plain JSON. From the browser, use the client wrapper:
//
//   import { api } from '@human-synthesis/norns-tron/client'
//   { data } := await api.get '/api/messages'
export GET := route
	handler: ({ container }) =>
		data: messages(container).list()

// POST /api/messages — accepts JSON, form, or TRON request bodies; the body
// is validated against the same valibot schema the form action uses.
export POST := route
	input: sendMessageSchema
	handler: ({ input, container }) =>
		messages(container).create input
		{ ok: true }
