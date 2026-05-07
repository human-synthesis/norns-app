import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { items, search } from '../data';

export const GET: RequestHandler = () => json(items);

export const POST: RequestHandler = async ({ request }) => {
	const body = await request.json();
	return json(search(body?.q));
};
