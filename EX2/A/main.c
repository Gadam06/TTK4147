#include <stdio.h>
#include <unistd.h>

int var_global=0;

int main()
{
	int var_local=0, i;
	
	vfork();	//switch between fork() and vfork() to see differences
	
	for ( i=0; i<10; i++ ) {
		var_global++;
		var_local++;
	}
	
	printf( "global = %d\nlocal = %d\n", var_global, var_local );

	_exit(2);
}
