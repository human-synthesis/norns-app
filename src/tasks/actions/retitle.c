// Custom body for tasks.Action.retitle — the starter's example of `impl: custom`.
// Contract: ({ row, input, container, user }) => result. Guards already ran in the shell.
import { eq } from 'drizzle-orm'

import { Task } from '$lib/tasks/schema.c'

export default async ({ input, container }) => {
	const db = container.resolve('db')
	await db.update(Task).set({ title: input.title }).where(eq(Task.id, input.id))
	return { title: input.title }
}
