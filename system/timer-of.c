#include <sys/time.h>
#include <stdlib.h>
#include <place/system/timer-of.h>

static inline long	times_thousand(long i)
{
	return ((i << 10) - (i << 4) - (i << 3));
}

TIMER_OF_CALL long TIMER_OF_TYPE time_now(void)
{
	struct timeval  now;

    gettimeofday(&now, NULL);
    return (times_thousand(now.tv_sec) + (now.tv_usec * 1e-3));
}