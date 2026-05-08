// In-memory store. Replace with Drizzle / better-sqlite3 / D1 when you
// need persistence — only `list` and `add` are called from outside.

export class MessagesRepo
	items: { id: number; text: string; at: number }[] = []

	list()
		[...@items]

	add(text: string)
		item := { id: @items.length + 1, text, at: Date.now() }
		@items.unshift item
		item
