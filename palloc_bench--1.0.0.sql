CREATE OR REPLACE FUNCTION palloc_bench(int, int, int)
    RETURNS void
    AS 'palloc_bench', 'palloc_bench'
    LANGUAGE C IMMUTABLE;