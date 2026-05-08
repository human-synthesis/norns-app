import * as v from 'valibot'
import type { MessagesRepo } from './repo'

export sendMessageSchema := v.object
	text: v.pipe v.string(), v.trim(), v.minLength(1, 'message is required'), v.maxLength(280)

export type SendMessageInput = v.InferOutput<typeof sendMessageSchema>

export class MessagesService
	repo: MessagesRepo
	constructor(@repo: MessagesRepo)

	list()
		@repo.list()

	create(input: SendMessageInput)
		@repo.add input.text
