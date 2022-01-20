-- inner join to match addresses to customers
SELECT c.first_name, c.last_name, a.address and filter results by postal code
FROM customer AS c
  INNER JOIN address AS a
  ON c.address_id = a.address_id;
WHERE a.postal_code = 52137;

-- inner join to find all customers who rented a film on 2005-05-31
SELECT c.first_name, c.last_name, time(r.rental_date) AS rental_time
FROM customer AS c
  INNER JOIN rental AS r
  ON c.customer_id = r.customer_id
WHERE date(r.rental_date) = '2005-05-31';

-- inner join to find all of the customers who have rented 40 or more films
SELECT c.first_name, c.last_name, count(*)
FROM customer AS c
  INNER JOIN rental AS r
  ON c.customer_id = r.customer_id
GROUP BY c.first_name, c.last_name
HAVING count(*) >= 40;

-- three table join to obtain city names for addresses of customers
SELECT c.first_name, c.last_name, ct.city
FROM customer AS c
  INNER JOIN address AS a
  ON c.address_id = a.address_id
  INNER JOIN city AS ct
  ON a.city_id = ct.city_id;

-- inner join using sub-query to find all addresses
that are in California and then return information about the customers living there
SELECT c.first_name, c.last_name, ad.address, ad.city
FROM customer AS c
  INNER JOIN (
    SELECT a.address_id, a.address, ct.city
    FROM address AS a
      INNER JOIN city AS ct
      ON a.city_id = ct.city_id
    WHERE a.district = 'California'
  ) AS ad
  ON c.address_id = ad.address_id;

-- join same table multiple times to find all of the films in which either of two specific actors appear
SELECT f.title
FROM film as f
  INNER JOIN film_actor AS fa
  ON f.film_id = fa.film_id
  INNER JOIN actor AS a
  ON fa.actor_id = a.actor_id
  WHERE ((a.first_name = 'CATE' AND a.last_name = 'MCQUEEN')
    OR (a.first_name = 'CUBA' AND a.last_name = 'BIRCH'));

-- join same table multiple times to find all of the films in which two specific actors appear together
SELECT f.title
FROM film AS f
  INNER JOIN film_actor AS fa1
  ON f.film_id = fa1.film_id
  INNER JOIN actor AS a1
  ON fa1.actor_id = a1.actor_id
  INNER JOIN film_actor AS fa2
  ON f.film_id = fa2.film_id
  INNER JOIN actor AS a2
  ON fa2.actor_id = a2.actor_id
  WHERE (a1.first_name = 'CATE' AND a1.last_name = 'MCQUEEN')
    AND (a2.first_name = 'CUBA' AND a2.last_name = 'BIRCH');