#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/times.h>

//#define BUSY_WAIT

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

void* mainOne( void* param )
{
	printf( "I'm thread ONE\n" );
#ifdef BUSY_ZWAIT
	busy_wait_delay( 5 );
#else
	sleep(5);
#endif
	printf( "I'm thread ONE done\n" );

	return NULL;
}

void* mainTwo( void* param )
{
	printf( "I'm thread TWO\n" );
#ifdef BUSY_ZWAIT
	busy_wait_delay( 5 );
#else
	sleep(5);
#endif
	printf( "I'm thread TWO done\n" );

	return NULL;
}

int main()
{
	pthread_t threadOne;
	pthread_t threadTwo;

	pthread_create( &threadOne, NULL, mainOne, NULL );
	pthread_create( &threadTwo, NULL, mainTwo, NULL );

	pthread_join( threadOne, NULL );
	pthread_join( threadTwo, NULL );

	return 0;
}
