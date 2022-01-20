-- create film table
CREATE TABLE film (
  film_id INTEGER PRIMARY KEY NOT NULL,
  title TEXT NOT NULL,
  release_year INTEGER,
  language_id INTEGER,
  rental_duration INTEGER DEFAULT 3 NOT NULL,
  rental_rate REAL DEFAULT 4.99 NOT NULL,
  length INTEGER,
  rating TEXT CHECK (rating in ('G','PG','PG-13','R','NC-17')),
  FOREIGN KEY (language_id) REFERENCES language (language_id)
);

-- add description column
ALTER TABLE film ADD COLUMN description TEXT;

-- rename column from title to name
ALTER TABLE film RENAME COLUMN title TO name;

-- drop length column
ALTER TABLE film DROP COLUMN length;

-- rename table from film to movie
ALTER TABLE film RENAME TO movie;

-- drop film table
DROP TABLE IF EXISTS film;

-- drop language table preventing constraint failed error being caused by foreign keys in film table
PRAGMA foreign_keys = OFF;
DROP TABLE language;
UPDATE film SET language_id = NULL;
PRAGMA foreign_keys = ON;