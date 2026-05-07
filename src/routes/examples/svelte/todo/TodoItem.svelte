<script lang="ts">
	import { toggleTodo, removeTodo, type Todo } from 'store';

	let { todo }: { todo: Todo } = $props();
</script>

<li class="todo-item {todo.done ? 'done' : ''}">
	<input
		class="todo-checkbox"
		type="checkbox"
		checked={todo.done}
		onchange={() => toggleTodo(todo.id)}
	/>
	<span class="todo-text">{todo.text}</span>
	<button class="todo-remove" onclick={() => removeTodo(todo.id)} aria-label="remove">×</button>
</li>

<style>
	@reference 'tailwindcss';

	.todo-item {
		@apply flex items-center gap-3 px-4 py-3 bg-white border border-neutral-200 rounded-lg shadow-sm;
		@apply transition-all duration-200 ease-out;
	}
	.todo-item:hover {
		@apply border-neutral-300 shadow-md;
	}
	.todo-item.done {
		@apply opacity-60;
	}

	.todo-checkbox {
		@apply w-4 h-4 cursor-pointer accent-violet-600;
	}

	.todo-text {
		@apply flex-1 text-neutral-900 transition-colors;
	}
	.todo-item.done .todo-text {
		@apply line-through text-neutral-400;
	}
</style>
