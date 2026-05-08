import Database from 'better-sqlite3'
import { mkdirSync } from 'node:fs'
import { dirname } from 'node:path'

DB_PATH = 'data/notes.db'
mkdirSync dirname(DB_PATH), recursive: true

db = new Database DB_PATH
db.pragma 'journal_mode = WAL'
db.exec '''
	CREATE TABLE IF NOT EXISTS notes (
		id         INTEGER PRIMARY KEY AUTOINCREMENT,
		title      TEXT NOT NULL,
		body       TEXT NOT NULL DEFAULT '',
		created_at INTEGER NOT NULL,
		updated_at INTEGER NOT NULL
	)
'''

listNotes = ->
	db.prepare('SELECT * FROM notes ORDER BY updated_at DESC').all()

getNote = (id) ->
	db.prepare('SELECT * FROM notes WHERE id = ?').get(id)

createNote = (title, body) ->
	now = Date.now()
	result = db.prepare('INSERT INTO notes (title, body, created_at, updated_at) VALUES (?, ?, ?, ?)').run(title, body, now, now)
	result.lastInsertRowid

updateNote = (id, title, body) ->
	db.prepare('UPDATE notes SET title=?, body=?, updated_at=? WHERE id=?').run(title, body, Date.now(), id)

deleteNote = (id) ->
	db.prepare('DELETE FROM notes WHERE id=?').run(id)

export { listNotes, getNote, createNote, updateNote, deleteNote }
