import type { Container } from '@human-synthesis/norns/server'
import type { MessagesService, SendMessageInput } from './service'

svc := (c: Container) => c.resolve('messages.service') as MessagesService

export messages := (c: Container) => {
	list: () => svc(c).list()
	create: (input: SendMessageInput) => svc(c).create(input)
}
