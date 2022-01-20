-- comparison operators (=, !=, <, >, <=, >=)
SELECT * FROM customer
WHERE active = 0;

-- logical operators (AND, OR, NOT)
SELECT * FROM customer
WHERE first_name = 'ANDY' OR last_name = 'MARTIN';

-- comparisons involving null (IS NULL, IS NOT NULL)
SELECT * FROM customer
WHERE email IS NULL;

-- set and range inclusion (IN, BETWEEN)
SELECT * FROM customer
WHERE first_name IN ('ANDY', 'SANDRA', 'ERICA');
SELECT * FROM customer
WHERE date(create_date) BETWEEN '2006-02-14' AND '2006-02-15';

-- match case-insensitive pattern (LIKE)
SELECT * FROM customer
WHERE last_name LIKE '_A_T%S';