-- start transaction
BEGIN TRANSACTION;

-- create savepoint
SAVEPOINT save;

-- rollback to savepoint
ROLLBACK TO save;

-- remove savepoint
RELEASE save;

-- rollback transaction
ROLLBACK;

-- commit transaction
COMMIT;