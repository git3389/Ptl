#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

#define MAXPRO 65536
#define SLEEPSEC 10
 /*Program entrance*/
int main(int argc, char** argv)
{
	pid_t pid;
	int i = 0;
	int max = MAXPRO;
	if(argc == 2)
	{
		max = atoi(argv[1]);
	}
	for(i=0;i<max;i++)
	{
		pid = fork();
		if(pid == 0)
		{
			printf("child %d created!\n",i);
			sleep(SLEEPSEC);
			exit(0);
		}
		else if(pid < 0)
		{
			printf("create processe error!\n");
			exit(0);
		}

	}
	printf("create %d processed!\n",i);
	for(i=0;i<max;i++)
	{
		wait();
	}
	return 0;
}


