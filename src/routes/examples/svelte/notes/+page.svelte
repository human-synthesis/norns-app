<script lang="ts">
	import './notes.css';
	import type { PageProps } from './$types';

	let { data, form }: PageProps = $props();
</script>

<section class="animate-in space-y-3">
	<a class="text-sm nav-link inline-flex items-center gap-1" href="/examples/svelte">← Svelte examples</a>
	<h1 class="text-4xl font-bold tracking-tight leading-none">
		<span class="gradient-text">Notes</span>
	</h1>
	<p class="text-neutral-600 max-w-prose leading-relaxed">
		SQLite-backed CRUD with SvelteKit form <code>actions</code> and SSR <code>load</code> — no client-side fetch.
	</p>
</section>

<section class="animate-in-delayed pt-8 space-y-6">
	<form class="note-form" method="POST" action="?/create">
		<input class="note-title-input" name="title" placeholder="title…" value={form?.title ?? ''} required />
		<textarea class="note-body-input" name="body" placeholder="body (optional)…" rows="3">{form?.body ?? ''}</textarea>
		<button class="note-submit" type="submit">Create note</button>
		{#if form?.error}
			<p class="note-error">{form.error}</p>
		{/if}
	</form>

	{#if data.notes.length === 0}
		<p class="note-empty">no notes yet — create one above</p>
	{:else}
		<ul class="list-none p-0 m-0 space-y-2">
			{#each data.notes as note (note.id)}
				<li class="note-card">
					<a class="note-link" href="/examples/svelte/notes/{note.id}">
						<div class="note-card-title">{note.title}</div>
						{#if note.body}
							<div class="note-card-snippet">{note.body}</div>
						{/if}
						<div class="note-card-meta">updated {new Date(note.updated_at).toLocaleString()}</div>
					</a>
				</li>
			{/each}
		</ul>
	{/if}
</section>
