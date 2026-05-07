<template lang="pug">
	.space-y-6
		.space-y-2
			h1.text-4xl.font-bold Hello, Norns
			p.text-neutral-600 Edit #[code src/routes/+page.svelte] to get started.

		.flex.items-center.gap-3
			button.px-3.py-1.bg-neutral-900.text-white.rounded(onclick!="{increment}") count: {count}
			span.text-sm.text-neutral-500 (CoffeeScript + Pug + UnoCSS)
</template>

<script lang="coffee">
	`
	let count = $state(0)
	let increment = () => count++
	`
</script>
