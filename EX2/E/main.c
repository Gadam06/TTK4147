#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N_THREADS	5
#define N_MUTEXES	5

#define NEXT(i,n)	((i==(n-1))?0:(i+1))

pthread_mutex_t	pMux[N_MUTEXES];
int				pFork[N_MUTEXES];
pthread_mutex_t muxMux;

void* threadFun( void* _pParam )
{	
	int* piT = _pParam;

	while ( 1 ) {
		if ( pFork[*piT] && pFork[NEXT(*piT,N_MUTEXES)] ) {
			pFork[*piT] = 0;
			pFork[NEXT(*piT,N_MUTEXES)] = 0;
			pthread_mutex_lock( &muxMux );
			break;
		}
	}
		pthread_mutex_lock( &pMux[*piT] );
		printf( "%d took its left fork\n", *piT );
		pthread_mutex_lock( &pMux[NEXT(*piT,N_MUTEXES)] );
		printf( "%d took its right fork\n", *piT );
	pthread_mutex_unlock( &muxMux );
	
	usleep( 100000 );
	
	pthread_mutex_unlock( &pMux[NEXT(*piT,N_MUTEXES)] );
	printf( "%d put its right pFork\n", *piT );
	pthread_mutex_unlock( &pMux[*piT] );
	printf( "%d put its left pFork\n", *piT );
		pFork[*piT] = 1;
		pFork[NEXT(*piT,N_MUTEXES)] = 1;
	
    return NULL;
}

void* threadFun2( void* _pParam )
{	
	int* piT = _pParam;

	while(1) {
		pthread_mutex_lock( &muxMux );
			//printf( "thread %d tries...\n", *piT );
			if( !pthread_mutex_trylock( &pMux[*piT] ) ) {
				if( !pthread_mutex_trylock( &pMux[NEXT(*piT,N_MUTEXES)] ) ) {
					printf( "%d took its forks >\n", *piT );
					break;
				} else {
					pthread_mutex_unlock( &pMux[*piT] );
				}
			}
		pthread_mutex_unlock( &muxMux );
	}
	pthread_mutex_unlock( &muxMux );
	
	printf( "%d eats\n", *piT );
	usleep( 1000000 );
	
	pthread_mutex_unlock( &pMux[NEXT(*piT,N_MUTEXES)] );
	pthread_mutex_unlock( &pMux[*piT] );
	printf( "%d put its forks <\n", *piT );
	
    return NULL;
}


int main()
{
	int i;
	int piThread[5];
	pthread_t 		pThread[N_THREADS];
	
	for ( i=0; i<N_MUTEXES; i++ ) {
		pthread_mutex_init( &pMux[i], NULL );
		pFork[i] = 1;
	}
	pthread_mutex_init( &muxMux, NULL );

	for ( i=0; i<N_THREADS; i++ ) {
	piThread[i] = i;
    	pthread_create( &pThread[i], NULL, threadFun2, &piThread[i] );
    }
 
	for ( i=0; i<N_THREADS; i++ )
    	pthread_join( pThread[i], NULL );
    
    return 0;

}
