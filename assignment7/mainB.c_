
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
#include <native/sem.h>
#include <native/mutex.h>
#include  <rtdk.h>

void print_pri(RT_TASK *task, char *s)
{
	struct rt_task_info temp;
	rt_task_inquire(task, &temp);
	rt_printf("b:%i c:%i ", temp.bprio, temp.cprio);
	rt_printf(s);
}
int rt_task_sleep_ms(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}
void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

int set_cpu(int cpu_number){
	//setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	//set cpu to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

RT_SEM sem, semsync;
//RT_MUTEX mux;
RT_TASK task_1, task_2, task_3, task_sync;

void fun_1( void* pp )
{
	set_cpu(1);

	//rt_sem_p( &semsync, TM_INFINITE );

	rt_sem_p( &sem, TM_INFINITE );
	//rt_mutex_acquire( &mux, TM_INFINITE );
	print_pri( &task_1, "fun_1 takes\n");

	busy_wait_ms( 300 );

	print_pri( &task_1, "fun_1 gives\n");
	rt_sem_v( &sem);
	//rt_mutex_release( &mux );
}

void fun_2( void* pp )
{
	set_cpu(1);

	//rt_sem_p( &semsync, TM_INFINITE );

	rt_task_sleep_ms( 100 );

	print_pri( &task_2, "fun_2 start\n");

	busy_wait_ms( 500 );

	print_pri( &task_2, "fun_2 end\n");
}

void fun_3( void* pp )
{
	set_cpu(1);

	//rt_sem_p( &semsync, TM_INFINITE );

	rt_task_sleep_ms( 200 );

	rt_sem_p( &sem, TM_INFINITE);
	//rt_mutex_acquire( &mux, TM_INFINITE );
	print_pri( &task_3, "fun_3 takes\n");

	busy_wait_ms( 200 );

	print_pri( &task_3, "fun_3 gives\n");
	rt_sem_v( &sem);
	//rt_mutex_release( &mux );
}

void fun_sync( void* pp )
{
	rt_task_sleep( 100 );
	rt_sem_broadcast( &semsync );
}

int main(){

	mlockall( MCL_CURRENT | MCL_FUTURE );

	rt_print_auto_init(1);

	rt_sem_create( &sem, "sem", 1, S_FIFO );
	rt_sem_create( &semsync, "sync", 0, S_FIFO );
	//rt_mutex_create( &mux, "mutex" );

	assert( !rt_task_create( &task_1, "task 1", 0, 50, T_JOINABLE ) );
	assert( !rt_task_create( &task_2, "task 2", 0, 60, T_JOINABLE ) );
	assert( !rt_task_create( &task_3, "task 3", 0, 70, T_JOINABLE ) );
	//assert( !rt_task_create( &task_sync, "task sync", 0, 99, T_JOINABLE ) );

	rt_task_shadow(NULL, "Main", 99, T_JOINABLE);

	assert( !rt_task_start( &task_1, fun_1, NULL ) );
	assert( !rt_task_start( &task_2, fun_2, NULL ) );
	assert( !rt_task_start( &task_3, fun_3, NULL ) );
	//assert( !rt_task_start( &task_sync, fun_sync, NULL ) );
	
	rt_task_join(&task_1);
	rt_task_join(&task_2);
	rt_task_join(&task_3);
	//rt_task_join(&task_sync);

	rt_sem_delete( &sem );
	rt_sem_delete( &semsync );
	//rt_mutex_delete( &mux );

	printf( "end\n" );

	return 0;
}
