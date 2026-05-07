import { json } from '@sveltejs/kit'
import { items, search } from '../data'

export GET = -> json items

export POST = ({ request }) ->
	body = await request.json()
	json search body?.q
