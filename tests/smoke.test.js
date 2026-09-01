import { afterAll, beforeAll, describe, expect, test } from 'bun:test';
import { spawn } from 'node:child_process';
import { openSync } from 'node:fs';
import { chromium } from 'playwright';

const PORT = 5187;
const BASE = `http://localhost:${PORT}`;
const SEEDED = [
	'Edit specs/tasks.tron and watch this page regenerate',
	'Run `bunx norns trace` to execute the action examples'
];

let server;
let browser;

// `norns dev` regenerates on startup, which can restart Vite right after the
// first successful response — require a few consecutive OKs so tests only
// start against a settled server.
async function waitForServer(url, timeoutMs = 60_000) {
	const deadline = Date.now() + timeoutMs;
	let streak = 0;
	while (Date.now() < deadline) {
		try {
			const res = await fetch(url);
			streak = res.ok ? streak + 1 : 0;
		} catch {
			streak = 0;
		}
		if (streak >= 4) return;
		await new Promise((r) => setTimeout(r, 750));
	}
	throw new Error(`dev server did not become ready at ${url}`);
}

beforeAll(async () => {
	const log = openSync('/tmp/norns-app-smoke-server.log', 'w');
	server = spawn('bun', ['run', 'dev', '--port', String(PORT)], {
		cwd: new URL('..', import.meta.url).pathname,
		stdio: ['ignore', log, log],
		detached: true
	});
	await waitForServer(BASE);
	browser = await chromium.launch();

	// Warm-up: the first browser load on a cold Vite cache triggers dep
	// optimization + a mid-hydration reload. Absorb that here so the actual
	// tests run against a settled module graph.
	const page = await browser.newPage();
	try {
		await page.goto(BASE + '/', { waitUntil: 'load', timeout: 30_000 });
		await page.waitForTimeout(3_000);
		// Also exercise the interactive graph once: the first-ever click on a
		// cold graph can throw ("reading 'call'") and kill reactivity.
		await page.locator('button.data-table-sort').first().click({ timeout: 5_000 });
		await page.waitForTimeout(1_000);
	} catch {}
	await page.close();
}, 120_000);

afterAll(async () => {
	await browser?.close();
	if (server?.pid) {
		try {
			process.kill(-server.pid, 'SIGTERM');
		} catch {}
	}
});

describe('generated pages smoke', () => {
	test('/ renders the Table bound to tasks.Query.open and hydrates cleanly', async () => {
		const page = await browser.newPage();
		const pageErrors = [];
		page.on('pageerror', (e) => pageErrors.push(String(e)));
		try {
			await page.goto(BASE + '/', { waitUntil: 'load' });
			await page.waitForSelector('.table-root tbody tr');
			for (const title of SEEDED) {
				await expect(page.getByText(title).first().isVisible()).resolves.toBe(true);
			}
			expect(await page.locator('tbody tr').count()).toBe(SEEDED.length);
			await page.waitForTimeout(1_000);
			expect(pageErrors).toEqual([]);
		} finally {
			await page.close();
		}
	}, 30_000);

	test('clicking a column header re-sorts the rows', async () => {
		const page = await browser.newPage();
		try {
			const attempt = async () => {
				await page.goto(BASE + '/', { waitUntil: 'load' });
				await page.waitForSelector('.table-root tbody tr');
				const firstCell = () => page.locator('tbody tr').first().textContent();
				const before = await firstCell();
				const sortButton = page
					.locator('button.data-table-sort', { hasText: 'Title' })
					.first();
				await sortButton.click();
				await sortButton.click();
				const deadline = Date.now() + 3_000;
				while (Date.now() < deadline) {
					if ((await firstCell()) !== before) return true;
					await page.waitForTimeout(150);
				}
				return false;
			};
			// A dev-mode cold-graph error can swallow the first click's reactivity;
			// a fresh load recovers, so retry once before failing.
			let sorted = await attempt();
			if (!sorted) sorted = await attempt();
			expect(sorted).toBe(true);
		} finally {
			await page.close();
		}
	}, 60_000);

	test('unknown routes 404 instead of crashing', async () => {
		const res = await fetch(BASE + '/definitely-not-a-page');
		expect(res.status).toBe(404);
	});
});
