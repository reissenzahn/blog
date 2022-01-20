-- insert row into actor table
INSERT INTO actor (first_name, last_name)
VALUES ('Christian', 'Bale');

-- update existing rows in actor table by actor_id
UPDATE TABLE actor
SET first_name = 'Nicolas', last_name = 'Cage'
WHERE actor_id = 3;

-- if a unique or primary key constraint violation occurs then delete the existing row that caused the violation and insert the new row
INSERT OR REPLACE INTO actor (actor_id, first_name, last_name)
VALUES (3, 'Nicolas', 'Cage');

-- delete rows by actor_id
DELETE FROM actor WHERE actor_id = 3;