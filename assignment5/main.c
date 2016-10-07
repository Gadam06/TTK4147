
#include "io.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <sys/times.h>
#include <unistd.h>
#include <sched.h>

int set_cpu(int cpu_number){
	//setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	//set cpu to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

/*void busy_wait_delay(int seconds)
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
}*/

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;

	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000) {
		t->tv_nsec = t->tv_nsec-1000000000;t->tv_sec += 1;
	}
}

void* freadA( void* pp )
{
	struct timespec next;

	while ( 1 ) {
		clock_gettime( CLOCK_REALTIME, &next );
		timespec_add_us( &next, 100 );
		//printf( "A\n" );
		pthread_yield();
		if ( !io_read(1) ) {
			io_write( 1, 0 );
			usleep( 5000 );
			io_write( 1, 1 );
		}
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
}

void* freadB( void* pp )
{
	struct timespec next;

	while ( 1 ) {
		clock_gettime( CLOCK_REALTIME, &next );
		timespec_add_us( &next, 100 );
		//printf( "B\n" );
		pthread_yield();
		if ( !io_read(2) ) {
			io_write( 2, 0 );
			usleep( 5000 );
			io_write( 2, 1 );
		}
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
}

void* freadC( void* pp )
{
	struct timespec next;

	while ( 1 ) {
		clock_gettime( CLOCK_REALTIME, &next );
		timespec_add_us( &next, 100 );
		//printf( "C\n" );
		pthread_yield();
		if ( !io_read(3) ) {
			io_write( 3, 0 );
			usleep( 5000 );
			io_write( 3, 1 );
		}
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
}

void *infinite()
{
	set_cpu(1);
	while(1);
}

int main(){
	
	io_init();
	io_write( 1, 1 );
	io_write( 2, 1 );
	io_write( 3, 1 );

	mlockall( MCL_CURRENT | MCL_FUTURE );
	
	pthread_t A_task, B_task, C_task;
	pthread_t taskDisturb[10];
	
	pthread_create( &A_task, NULL, freadA, NULL );
	pthread_create( &B_task, NULL, freadB, NULL );
	pthread_create( &C_task, NULL, freadC, NULL );

	int i;
	for(i=0; i<10; i++){
		pthread_create(&taskDisturb[i],NULL, infinite,0);
	}
	
	pthread_join(A_task,NULL);
	pthread_join(B_task,NULL);
	pthread_join(C_task,NULL);

	printf( "end\n" );

	return 0;
}
