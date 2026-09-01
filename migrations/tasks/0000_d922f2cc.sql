CREATE TABLE `tasks_task` (
	`id` text PRIMARY KEY NOT NULL,
	`note` text,
	`owner` text NOT NULL,
	`title` text NOT NULL,
	`status` text DEFAULT 'open' NOT NULL
);
