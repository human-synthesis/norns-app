import { writable, derived } from 'svelte/store'

todos = writable []

addTodo = (text) ->
	text = text.trim()
	return unless text
	todos.update (list) ->
		[...list, { id: crypto.randomUUID(), text, done: false }]

toggleTodo = (id) ->
	todos.update (list) ->
		list.map (t) ->
			if t.id is id then { ...t, done: not t.done } else t

removeTodo = (id) ->
	todos.update (list) ->
		list.filter (t) -> t.id isnt id

clearCompleted = ->
	todos.update (list) ->
		list.filter (t) -> not t.done

remaining = derived todos, ($todos) ->
	$todos.filter((t) -> not t.done).length

total = derived todos, ($todos) -> $todos.length

export { todos, addTodo, toggleTodo, removeTodo, clearCompleted, remaining, total }
