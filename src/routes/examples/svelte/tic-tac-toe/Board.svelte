<script lang="ts">
	import Cell from 'Cell';
	import type { Cell as CellType } from 'store';

	let {
		cells,
		winningLine = [],
		disabled = false,
		onCellClick
	}: {
		cells: CellType[];
		winningLine?: number[];
		disabled?: boolean;
		onCellClick: (idx: number) => void;
	} = $props();
</script>

<div class="tt-board">
	{#each cells as value, idx (idx)}
		<Cell
			{value}
			isWinning={winningLine.includes(idx)}
			disabled={disabled || !!value || winningLine.length > 0}
			onclick={() => onCellClick(idx)}
		/>
	{/each}
</div>
