#include <stdlib.h>
#include <stdio.h>

int allocate( int value );

int main()
{
	while( 1 )
		if ( allocate(666) != 0 )
			return -1;
			
	return 0;
}

/**
 * @return 0 on success, -1 otherwise
 */
int allocate( int value )
{
	int* ptr = NULL;
	ptr = malloc( 1000000*1024*sizeof(int) );
	//error handling
	if ( ptr == NULL ) {
		perror( "failed to malloc\n" );
		return -1;
	}
	
	*ptr = value;
	printf( "test of allocated memory: %i\n", *ptr );

	return 0;
}
