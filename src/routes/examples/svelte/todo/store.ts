import { writable, derived } from 'svelte/store';

export type Todo = { id: string; text: string; done: boolean };

export const todos = writable<Todo[]>([]);

export function addTodo(text: string): void {
	text = text.trim();
	if (!text) return;
	todos.update((list) => [...list, { id: crypto.randomUUID(), text, done: false }]);
}

export function toggleTodo(id: string): void {
	todos.update((list) =>
		list.map((t) => (t.id === id ? { ...t, done: !t.done } : t))
	);
}

export function removeTodo(id: string): void {
	todos.update((list) => list.filter((t) => t.id !== id));
}

export function clearCompleted(): void {
	todos.update((list) => list.filter((t) => !t.done));
}

export const remaining = derived(todos, ($todos) => $todos.filter((t) => !t.done).length);
export const total = derived(todos, ($todos) => $todos.length);
