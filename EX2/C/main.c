#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N_THREADS	5
#define N_SEMS		3

typedef struct {
	int 	iThread;
	sem_t*	pSem;
} param_t;

void* threadFun( void* _pParam )
{	
	param_t* pParam = _pParam;
	int i;

	sem_wait( pParam->pSem );
	printf( "T%d (GET) <\n", pParam->iThread );

	for ( i=0; i<4; i++ ) {
		printf( "T%d (%d/4)\n", pParam->iThread, i+1 );
		usleep( 100000 );
	}
	
	printf( "T%d (give) >\n", pParam->iThread );
	sem_post( pParam->pSem );

    return NULL;
}


int main()
{
	int i;
	pthread_t 	pThread[N_THREADS];
	sem_t		semaphore;
	param_t		pParam[N_THREADS];
	
	sem_init( &semaphore, 0, N_SEMS );

	for ( i=0; i<N_THREADS; i++ ) {
		pParam[i] = (param_t){ .iThread=i, .pSem=&semaphore };
    	pthread_create( &pThread[i], NULL, threadFun, &pParam[i] );
    }

	for ( i=0; i<N_THREADS; i++ )
    	pthread_join( pThread[i], NULL );
    
    return 0;

}
