import { messages } from '$lib/norns/messages/server/public'
import { sendMessageSchema } from '$lib/norns/messages/shared/schema'

// `page` comes from @human-synthesis/norns/server via auto-import (server
// files get the runtime helpers for free); project code is imported
// explicitly above.
export load := page.load
	handler: ({ container }) =>
		items: messages(container).list()

export actions := page.actions
	send:
		input: sendMessageSchema
		run: ({ input, container }) =>
			messages(container).create input
			{ ok: true }
