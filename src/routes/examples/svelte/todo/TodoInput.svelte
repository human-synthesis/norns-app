<script lang="ts">
	import { addTodo } from 'store';

	let value = $state('');

	function handleSubmit(e: Event) {
		e.preventDefault();
		addTodo(value);
		value = '';
	}
</script>

<form class="flex gap-2" onsubmit={handleSubmit}>
	<input
		class="todo-input"
		type="text"
		placeholder="What needs to be done?"
		bind:value
	/>
	<button class="todo-add" type="submit" disabled={!value.trim()}>Add</button>
</form>
