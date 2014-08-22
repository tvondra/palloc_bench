#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

#include "postgres.h"
#include "utils/datum.h"
#include "utils/array.h"
#include "utils/memutils.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define GET_AGG_CONTEXT(fname, fcinfo, aggcontext)  \
    if (! AggCheckCallContext(fcinfo, &aggcontext)) {   \
        elog(ERROR, "%s called in non-aggregate context", fname);  \
    }

/* prototypes */
PG_FUNCTION_INFO_V1(palloc_bench);

// #define TRACKING_FLAG

Datum palloc_bench(PG_FUNCTION_ARGS);

Datum
palloc_bench(PG_FUNCTION_ARGS)
{

	/* info for anyelement */
	int i = 0;
	int32	ncontexts   = PG_GETARG_INT32(0);
	int32	niterations = PG_GETARG_INT32(1);
	int32	allocsize   = PG_GETARG_INT32(2);

	struct timeval start_time, end_time;

	/* memory contexts */
	MemoryContext initctx = CurrentMemoryContext;
	MemoryContext ctx = initctx;

	/* switch to the per-group hash-table memory context */

	for (i = 0; i < ncontexts; i++) {
		
		char name[256];
		sprintf(name, "test context %d", i);

#ifdef TRACKING_FLAG
		ctx = AllocSetContextCreateTracked(ctx,
										   name,
										   ALLOCSET_DEFAULT_MINSIZE,
										   ALLOCSET_DEFAULT_INITSIZE,
										   ALLOCSET_DEFAULT_MAXSIZE,
										   true);
#else
		ctx = AllocSetContextCreate(ctx,
									name,
									ALLOCSET_DEFAULT_MINSIZE,
									ALLOCSET_DEFAULT_INITSIZE,
									ALLOCSET_DEFAULT_MAXSIZE);
#endif
	}

	MemoryContextSwitchTo(ctx);

	gettimeofday(&start_time, NULL);
	
	for (i = 0; i < niterations; i++) {

		char * p = palloc(allocsize);
		
		if (p != NULL)
			pfree(p);

	}

	gettimeofday(&end_time, NULL);
	
	MemoryContextSwitchTo(initctx);

	elog(WARNING, "duration = %.2f ms", (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000.0);

    PG_RETURN_VOID();

}
