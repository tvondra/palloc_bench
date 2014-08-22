MODULE_big = palloc_bench
OBJS = palloc_bench.o

EXTENSION = palloc_bench
DATA = palloc_bench--1.0.0.sql
MODULES = palloc_bench

CFLAGS=`pg_config --includedir-server`

TESTS        = $(wildcard test/sql/*.sql)
REGRESS      = $(patsubst test/sql/%.sql,%,$(TESTS))
REGRESS_OPTS = --inputdir=test

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

palloc_bench.so: palloc_bench.o

palloc_bench.o: palloc_bench.c
