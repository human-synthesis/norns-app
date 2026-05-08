import Database from 'better-sqlite3';
import { mkdirSync } from 'node:fs';
import { dirname } from 'node:path';

export type Note = {
	id: number;
	title: string;
	body: string;
	created_at: number;
	updated_at: number;
};

const DB_PATH = 'data/notes.db';
mkdirSync(dirname(DB_PATH), { recursive: true });

const db = new Database(DB_PATH);
db.pragma('journal_mode = WAL');
db.exec(`
	CREATE TABLE IF NOT EXISTS notes (
		id         INTEGER PRIMARY KEY AUTOINCREMENT,
		title      TEXT NOT NULL,
		body       TEXT NOT NULL DEFAULT '',
		created_at INTEGER NOT NULL,
		updated_at INTEGER NOT NULL
	)
`);

export function listNotes(): Note[] {
	return db.prepare('SELECT * FROM notes ORDER BY updated_at DESC').all() as Note[];
}

export function getNote(id: number): Note | undefined {
	return db.prepare('SELECT * FROM notes WHERE id = ?').get(id) as Note | undefined;
}

export function createNote(title: string, body: string): number | bigint {
	const now = Date.now();
	const result = db
		.prepare('INSERT INTO notes (title, body, created_at, updated_at) VALUES (?, ?, ?, ?)')
		.run(title, body, now, now);
	return result.lastInsertRowid;
}

export function updateNote(id: number, title: string, body: string): void {
	db.prepare('UPDATE notes SET title=?, body=?, updated_at=? WHERE id=?').run(title, body, Date.now(), id);
}

export function deleteNote(id: number): void {
	db.prepare('DELETE FROM notes WHERE id=?').run(id);
}
