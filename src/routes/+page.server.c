import { page } from '@human-synthesis/norns/server'
import { messages } from '$lib/norns/messages/server/public'
import { sendMessageSchema } from '$lib/norns/messages/server/service'

export load := page.load
	handler: ({ container }) =>
		items: messages(container).list()

export actions := page.actions
	send:
		input: sendMessageSchema
		run: ({ input, container }) =>
			messages(container).create input
			{ ok: true }
