
#include "io.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <sys/times.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include  <rtdk.h>

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

void freadA( void* pp )
{
	assert( !rt_task_set_periodic( NULL, TM_NOW, 10000 ) );

	while ( 1 ) {
		if ( !io_read(1) ) {
			io_write( 1, 0 );
			rt_task_sleep( 5 );
			io_write( 1, 1 );
		}
		switch( rt_task_wait_period(NULL) ) {
			case EWOULDBLOCK : assert(0); break;
			case EINTR : assert(0); break;
			case ETIMEDOUT : assert(0); break;
			case EPERM : assert(0); break;
			default : break;
		}
	}
}

void freadB( void* pp )
{
	assert( !rt_task_set_periodic( NULL, TM_NOW, 10000 ) );
	while ( 1 ) {
		if ( !io_read(2) ) {
			io_write( 2, 0 );
			rt_task_sleep( 5 );
			io_write( 2, 1 );
		}
		switch( rt_task_wait_period(NULL) ) {
			case EWOULDBLOCK : assert(0); break;
			case EINTR : assert(0); break;
			case ETIMEDOUT : assert(0); break;
			case EPERM : assert(0); break;
			default : break;
		}
	}
}

void freadC( void* pp )
{
	assert( !rt_task_set_periodic( NULL, TM_NOW, 10000 ) );

	while ( 1 ) {
		if ( !io_read(3) ) {
			io_write( 3, 0 );
			rt_task_sleep( 5 );
			io_write( 3, 1 );
		}
		switch( rt_task_wait_period(NULL) ) {
			case EWOULDBLOCK : assert(0); break;
			case EINTR : assert(0); break;
			case ETIMEDOUT : assert(0); break;
			case EPERM : assert(0); break;
			default : break;
		}
	}
}

void* infinite(void* pp)
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
	
	RT_TASK A_task, B_task, C_task;
	pthread_t taskDisturb[10];

	assert( !rt_task_create( &A_task, "task A", 0, 99, T_CPU(1)|T_JOINABLE ) );
	assert( !rt_task_create( &B_task, "task B", 0, 99, T_CPU(1)|T_JOINABLE ) );
	assert( !rt_task_create( &C_task, "task C", 0, 99, T_CPU(1)|T_JOINABLE ) );

	assert( !rt_task_start( &A_task, freadA, NULL ) );
	assert( !rt_task_start( &B_task, freadB, NULL ) );
	assert( !rt_task_start( &C_task, freadC, NULL ) );

	int i;
	for(i=0; i<10; i++){
		pthread_create(&taskDisturb[i], NULL, infinite, NULL );
	}
	
	rt_task_join(&A_task);
	rt_task_join(&B_task);
	rt_task_join(&C_task);

	printf( "end\n" );

	return 0;
}
