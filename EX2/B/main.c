#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int var_global=0;

void* mainOne( void* param )
{
	int var_local=0, i;
	
	for ( i=0; i<10; i++ ) {
		var_global++;
		var_local++;
	}
	
	printf( "ONE\nglobal = %d\nlocal = %d\n", var_global, var_local );

    return NULL;
}

void* mainTwo( void* param )
{
	int var_local=0, i;
	
	for ( i=0; i<10; i++ ) {
		var_global++;
		var_local++;
	}
	
	printf( "TWO\nglobal = %d\nlocal = %d\n", var_global, var_local );

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
    
	printf( "MAIN\nglobal = %d\n", var_global );

    return 0;

}
