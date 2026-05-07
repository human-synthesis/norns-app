items = [
	{ id: 'm1', name: 'Margherita',       category: 'pizza',   price: 14, tags: ['vegetarian', 'classic'],     description: 'tomato, mozzarella, basil' }
	{ id: 'm2', name: 'Pepperoni',        category: 'pizza',   price: 16, tags: ['classic'],                   description: 'tomato, mozzarella, pepperoni' }
	{ id: 'm3', name: 'Quattro Formaggi', category: 'pizza',   price: 18, tags: ['vegetarian'],                description: 'mozzarella, gorgonzola, parmesan, fontina' }
	{ id: 'm4', name: 'Diavola',          category: 'pizza',   price: 17, tags: ['spicy'],                     description: 'tomato, mozzarella, spicy salami, chili' }
	{ id: 'm5', name: 'Carbonara',        category: 'pasta',   price: 16, tags: ['classic'],                   description: 'eggs, pecorino, guanciale, black pepper' }
	{ id: 'm6', name: 'Pesto Linguine',   category: 'pasta',   price: 15, tags: ['vegetarian'],                description: 'basil pesto, parmesan, pine nuts' }
	{ id: 'm7', name: 'Cacio e Pepe',     category: 'pasta',   price: 14, tags: ['vegetarian', 'classic'],     description: 'pecorino romano, black pepper' }
	{ id: 'm8', name: 'Caprese',          category: 'salad',   price: 12, tags: ['vegetarian', 'gluten-free'], description: 'tomato, mozzarella, basil, olive oil' }
	{ id: 'm9', name: 'Caesar',           category: 'salad',   price: 13, tags: ['classic'],                   description: 'romaine, parmesan, croutons, anchovy' }
	{ id: 'm10', name: 'Tiramisu',        category: 'dessert', price: 9,  tags: ['classic'],                   description: 'mascarpone, espresso, cocoa, ladyfingers' }
	{ id: 'm11', name: 'Panna Cotta',     category: 'dessert', price: 8,  tags: ['vegetarian', 'gluten-free'], description: 'vanilla cream, berry coulis' }
	{ id: 'm12', name: 'Espresso',        category: 'drink',   price: 4,  tags: ['vegan', 'gluten-free'],      description: 'single shot, dark roast' }
]

search = (q) ->
	needle = (q ? '').trim().toLowerCase()
	return items unless needle
	items.filter (item) ->
		hay = "#{item.name} #{item.description} #{item.category} #{item.tags.join ' '}".toLowerCase()
		hay.includes needle

export { items, search }
