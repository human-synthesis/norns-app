<script lang="ts">
	import TodoList from 'TodoList';
</script>

<section class="animate-in space-y-3">
	<a class="text-sm nav-link inline-flex items-center gap-1" href="/examples/svelte">← Svelte examples</a>
	<h1 class="text-4xl font-bold tracking-tight leading-none">
		<span class="gradient-text">Todo</span>
	</h1>
	<p class="text-neutral-600 max-w-prose leading-relaxed">
		Components, props, stores — written in <code>.svelte</code> files (HTML + TypeScript).
	</p>
</section>

<section class="animate-in-delayed pt-8">
	<TodoList />
</section>
