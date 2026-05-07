<script lang="ts">
	import { onMount } from 'svelte';
	import './menu.css';
	import type { MenuItem } from './data';

	let items: MenuItem[] = $state([]);
	let query: string = $state('');
	let loading: boolean = $state(false);
	let timer: ReturnType<typeof setTimeout> | null = null;

	const loadAll = async () => {
		loading = true;
		const res = await fetch('/examples/svelte/menu/api');
		items = await res.json();
		loading = false;
	};

	const runSearch = async (q: string) => {
		loading = true;
		const res = await fetch('/examples/svelte/menu/api', {
			method: 'POST',
			headers: { 'content-type': 'application/json' },
			body: JSON.stringify({ q })
		});
		items = await res.json();
		loading = false;
	};

	const handleInput = () => {
		if (timer) clearTimeout(timer);
		timer = setTimeout(() => runSearch(query), 200);
	};

	const handleSubmit = (e: Event) => {
		e.preventDefault();
		if (timer) clearTimeout(timer);
		runSearch(query);
	};

	onMount(loadAll);
</script>

<section class="animate-in space-y-3">
	<a class="text-sm nav-link inline-flex items-center gap-1" href="/examples/svelte">← Svelte examples</a>
	<h1 class="text-4xl font-bold tracking-tight leading-none">
		<span class="gradient-text">Menu</span>
	</h1>
	<p class="text-neutral-600 max-w-prose leading-relaxed">
		Loaded over <code>GET</code> from a SvelteKit endpoint, searched over <code>POST</code>.
	</p>
</section>

<section class="animate-in-delayed pt-8 space-y-4">
	<form class="menu-search" onsubmit={handleSubmit}>
		<input
			class="menu-search-input"
			type="text"
			placeholder="search by name, ingredient, category, or tag…"
			bind:value={query}
			oninput={handleInput}
		/>
		{#if loading}
			<span class="menu-search-status">loading…</span>
		{/if}
	</form>

	{#if items.length === 0}
		<p class="menu-empty">no items match "{query}"</p>
	{:else}
		<table class="menu-table">
			<thead>
				<tr>
					<th>Name</th>
					<th>Description</th>
					<th>Category</th>
					<th>Tags</th>
					<th class="text-right">Price</th>
				</tr>
			</thead>
			<tbody>
				{#each items as item (item.id)}
					<tr class="menu-row">
						<td class="menu-name">{item.name}</td>
						<td class="menu-description">{item.description}</td>
						<td><span class="menu-tag">{item.category}</span></td>
						<td>
							<div class="menu-tag-list">
								{#each item.tags as tag}
									<span class="menu-tag-pill">{tag}</span>
								{/each}
							</div>
						</td>
						<td class="menu-price">${item.price}</td>
					</tr>
				{/each}
			</tbody>
		</table>
	{/if}
</section>
