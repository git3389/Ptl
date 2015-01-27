#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "evbuf.h"

 /*Program entrance*/
int main(int argc, char** argv)
{

	if(argc != 3)
	{
		printf("uarg:%s <server IP> <port>\n",argv[0]);
		return -1;
	}	
	evbuf(argv);

	return 0;
}


