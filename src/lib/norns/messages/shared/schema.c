import * as v from 'valibot'

// Input contract for the messages feature. Lives in shared/ so both the
// form action (+page.server.c) and the API endpoint (+server.c) validate
// against the same schema, and a client could import it too.
export sendMessageSchema := v.object
	text: v.pipe v.string(), v.trim(), v.minLength(1, 'message is required'), v.maxLength(280)

export type SendMessageInput = v.InferOutput<typeof sendMessageSchema>
