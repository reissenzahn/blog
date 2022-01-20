-- select all columns from film table
SELECT * FROM film;

-- select list of columns from film table
SELECT title, description, release_year FROM film;

-- select list of columns with column aliases
SELECT title, description AS desc, release_year AS year FROM film;

-- select expressions from film table
SELECT upper(title), rental_rate / 2 AS sale_rate FROM film;

-- select distinct rows from film table
SELECT DISTINCT * FROM film;

-- select rows ordered by the title column in ascending order
SELECT * FROM film
ORDER BY title ASC;

-- rows with the same value for the first column can be further ordered by the additional columns
SELECT * FROM film
ORDER BY title ASC, length DESC;

-- constrain number of rows returned to 10
SELECT * FROM film
LIMIT 10;

-- constrain rows to 10 starting from offset 5
SELECT * FROM film
LIMIT 10 OFFSET 5;

-- filter results based on rating and rental_duration columns
SELECT * FROM film
WHERE rating = 'PG' AND rental_duration >= 7;