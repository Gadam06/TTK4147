#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define USE_MUTEX	1

int gbRunning = 1;
int gVar1 = 0;
int gVar2 = 0;
pthread_mutex_t gVarMux;

void* threadFun1( void* _pParam )
{	
	while ( gbRunning ) {
#ifdef USE_MUTEX
		pthread_mutex_lock( &gVarMux );
#endif
		gVar1++;
		gVar2 = gVar1;
#ifdef USE_MUTEX
		pthread_mutex_unlock( &gVarMux );
#endif
	}

    return NULL;
}

void* threadFun2( void* _pParam )
{	
	int i;
	
	for ( i=0; i<20; i++ ) {
#ifdef USE_MUTEX
		pthread_mutex_lock( &gVarMux );
#endif
		printf( "var1=%d\tvar2=%d\n", gVar1, gVar2 );
#ifdef USE_MUTEX
		pthread_mutex_unlock( &gVarMux );
#endif
		usleep( 100000 );
	}
	gbRunning = 0;

    return NULL;
}


int main()
{
	pthread_t 	pThread1;
	pthread_t 	pThread2;
	pthread_mutex_init( &gVarMux, NULL );
	
	pthread_create( &pThread1, NULL, threadFun1, NULL );
	pthread_create( &pThread2, NULL, threadFun2, NULL );

	pthread_join( pThread1, NULL );
	pthread_join( pThread2, NULL );
    
    return 0;

}
