// The starter feature, exercised through the DI container exactly the way
// boot() wires it — no HTTP, no Vite. Copy this shape for each feature you
// add: register the module, resolve the public facade, call it.
import { describe, test, expect } from 'bun:test';
import * as v from 'valibot';
import { createApp } from '@human-synthesis/norns/server';
import register from '../src/lib/norns/messages/server/module.c';
import { messages } from '../src/lib/norns/messages/server/public.c';
import { sendMessageSchema } from '../src/lib/norns/messages/shared/schema.c';

function bootFeature() {
	const app = createApp();
	register(app);
	// Per-request scope, like event.locals.container.
	return app.scope();
}

describe('messages feature', () => {
	test('starts empty', () => {
		const c = bootFeature();
		expect(messages(c).list()).toEqual([]);
	});

	test('create() stores a message and list() returns newest first', () => {
		const c = bootFeature();
		const facade = messages(c);
		facade.create({ text: 'first' });
		facade.create({ text: 'second' });
		const items = facade.list();
		expect(items.map((m) => m.text)).toEqual(['second', 'first']);
		expect(items[0].id).toBe(2);
		expect(typeof items[0].at).toBe('number');
	});

	test('the service is a singleton per scope, a fresh one per app', () => {
		const c = bootFeature();
		messages(c).create({ text: 'x' });
		expect(messages(c).list()).toHaveLength(1);
		expect(messages(bootFeature()).list()).toHaveLength(0);
	});
});

describe('sendMessageSchema', () => {
	test('trims and accepts a normal message', () => {
		const r = v.safeParse(sendMessageSchema, { text: '  hello  ' });
		expect(r.success).toBe(true);
		expect(r.output.text).toBe('hello');
	});

	test('rejects an empty message with the field message', () => {
		const r = v.safeParse(sendMessageSchema, { text: '   ' });
		expect(r.success).toBe(false);
		expect(r.issues[0].message).toBe('message is required');
	});
});
