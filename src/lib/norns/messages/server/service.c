import type { MessagesRepo } from './repo'
import type { SendMessageInput } from '../shared/schema'

export class MessagesService
	repo: MessagesRepo
	constructor(@repo: MessagesRepo)

	list()
		@repo.list()

	create(input: SendMessageInput)
		@repo.add input.text
