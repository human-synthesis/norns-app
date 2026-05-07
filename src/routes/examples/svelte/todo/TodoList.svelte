<script lang="ts">
	import './todo.css';
	import TodoInput from 'TodoInput';
	import TodoItem from 'TodoItem';
	import { todos, total, remaining, clearCompleted } from 'store';
</script>

<div class="space-y-4">
	<TodoInput />

	{#if $total > 0}
		<ul class="space-y-2 list-none p-0 m-0">
			{#each $todos as todo (todo.id)}
				<TodoItem {todo} />
			{/each}
		</ul>

		<div class="todo-summary">
			<span>{$remaining} of {$total} remaining</span>
			{#if $total > $remaining}
				<button class="todo-clear" onclick={clearCompleted}>clear completed</button>
			{/if}
		</div>
	{:else}
		<p class="todo-empty">no todos yet — add one above</p>
	{/if}
</div>
