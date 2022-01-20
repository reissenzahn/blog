-- create index on column
CREATE INDEX index_name ON table_name(column_name);

-- create index on multiple columns
CREATE INDEX index_name ON table_name(column_name, column_name, ...);

-- create unique index (ensures values in one or more columns are unique)
CREATE UNIQUE INDEX index_name ON table_name(column_name);

-- check if index is being used
EXPLAIN QUERY PLAN
SELECT * FROM table_name WHERE column_name = value;

-- drop index
DROP INDEX IF EXISTS index_name;