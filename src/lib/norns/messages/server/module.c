import type { Container } from '@human-synthesis/norns/server'
import { MessagesRepo } from './repo'
import { MessagesService } from './service'

export default (app: Container) =>
	app.single 'messages.repo', => new MessagesRepo!
	app.single 'messages.service', (c: Container) => new MessagesService c.resolve('messages.repo')
