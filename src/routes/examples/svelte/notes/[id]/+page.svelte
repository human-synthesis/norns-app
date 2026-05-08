<script lang="ts">
	import './../notes.css';
	import type { PageProps } from './$types';

	let { data, form }: PageProps = $props();
</script>

<section class="animate-in space-y-3">
	<a class="text-sm nav-link inline-flex items-center gap-1" href="/examples/svelte/notes">← all notes</a>
	<h1 class="text-3xl font-bold tracking-tight leading-none">{data.note.title}</h1>
	<p class="text-sm text-neutral-500">updated {new Date(data.note.updated_at).toLocaleString()}</p>
</section>

<section class="animate-in-delayed pt-8 space-y-4">
	<form class="note-form" method="POST" action="?/update">
		<input class="note-title-input" name="title" value={data.note.title} required />
		<textarea class="note-body-input" name="body" rows="6">{data.note.body}</textarea>
		<div class="flex items-center gap-3">
			<button class="note-submit" type="submit">Save</button>
			{#if form?.saved}
				<span class="note-saved">saved</span>
			{/if}
			{#if form?.error}
				<span class="text-sm text-red-600">{form.error}</span>
			{/if}
		</div>
	</form>

	<form method="POST" action="?/delete">
		<button class="note-delete" type="submit">Delete note</button>
	</form>
</section>
