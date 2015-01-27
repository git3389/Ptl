#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int global;
 /*Program entrance*/
int main(int argc, char** argv)
{
	int stack;
	int uinit[10];
	static int si = 9;
	char *ptr;
	ptr = (char *)malloc(10);
	printf("global:%lx\n",&global);
	printf("stack:%lx\n",&stack);
	printf("uinit:form %lx to %lx\n",&uinit[0],&uinit[9]);
	printf("static:%lx\n",&si);
	printf("malloc:from %lx to %lx\n",ptr,ptr + 10);
	EV_KEY;
	KEY_0;

	return 0;
}


