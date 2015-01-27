#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>

#include "rub.h"
struct tag
{
	struct event_base *base;
	pid_t pid;
	char *str;
};

struct tag *strTag;
static void sig_chld(int sig);
static void sig_int(int sig);

static void sig_act(evutil_socket_t fd, short events, void *arg);
static void sig_act1(evutil_socket_t fd, short events, void *arg);
static void sig_act2(evutil_socket_t fd, short events, void *arg);
static void timerout(evutil_socket_t fd, short events, void *arg);
static void timerout1(evutil_socket_t fd, short events, void *arg);
static void timerout2(evutil_socket_t fd, short events, void *arg);
static void create_base();

 /*Program entrance*/
int main(int argc, char** argv)
{

	key_t ipckey = ftok("/",999);
	int shmid = shmget(ipckey,sizeof(struct tag),IPC_CREAT |0660);
	strTag = (struct tag *)shmat(shmid,NULL,0);
	strTag->str = (char *)malloc(10);
	printf("malloc : %lx\n",(unsigned long)strTag->str);
	pid_t pid = fork();

	if(pid == 0)
	{
		create_base();
	}
	if(pid > 0 )
	{
		signal(SIGCHLD,sig_chld);
		struct event_base *base = event_base_new();
		struct timeval tv = {1,0};
		struct event sig;
		struct event sig1;
//		struct event timeout;
//		evtimer_assign(&timeout,base,timerout,&timeout);
//		evtimer_add(&timeout,&tv);
		evsignal_assign(&sig,base,SIGINT,sig_act,(void *)&sig);
		evsignal_add(&sig,NULL);
		evsignal_assign(&sig1,base,SIGUSR1,sig_act2,(void *)base);
		evsignal_add(&sig1,NULL);
		event_base_dispatch(base);
		event_base_free(base);
		shmdt(strTag);
		shmctl(shmid,IPC_RMID,NULL);
	}
	return 0;
}


static void sig_act(evutil_socket_t fd, short events, void *arg)
{
	
//	printf("sig_act pid:%d subpid:%d sig:%d base:%p\n ",getpid(),strTag->pid,events,strTag->base);
//	printf("kill %d !!!\n",kill(strTag->pid,SIGUSR1));
//	event_base_loopexit(event_get_base(arg),NULL);
//	event_base_loopbreak(strTag->base);
	pid_t pid = waitpid(0,NULL,0);	
	printf("pid %d dead!!\n",pid);
//	printf("str:%p,%s\n",&(strTag->str),strTag->str);
//	free(strTag->str);
	event_base_loopbreak(event_get_base(arg));
	printf("parent dead!!\n");
//	exit(-1);
}
static void sig_act1(evutil_socket_t fd, short events, void *arg)
{ 
//	printf("sig_act1 pid:%d sig:%d base:%p\n",getpid(),events,event_get_base(arg));
//	event_base_loopbreak(event_get_base(arg));
//	printf("str:%p,%s\n",&(strTag->str),strTag->str);
	printf("sig_act1!!\n");
//	exit(-1);
}
static void sig_act2(evutil_socket_t fd, short events, void *arg)
{
	printf("creat a child\n");
	pid_t pid = fork();
	if(pid == 0)
	{
		create_base();
		printf("ppid %d,pid %d exit!!!\n",getppid(),getpid());
		exit(-1);
	}
	else
		return;
	/*
	struct timeval tv = {2,0};
	struct event *timeout;
	timeout = evtimer_new(arg,timerout2,timeout);
	evtimer_add(timeout,&tv);
*/
	
}
static void sig_chld(int sig)
{
	//printf("sig_chld pid:%d\nsig:%d\n",strTag->pid,sig);
	printf("ppid %d pid %d dead!!\n",getpid(),waitpid(0,NULL,0));	
}
static void sig_int(int sig)
{
//	printf("sig_int pid:%d sig:%d\n ",getpid(),sig);
}

static void timerout(evutil_socket_t fd, short events, void *arg)
{
	struct timeval tv = {10,0};
	kill(getppid(),SIGUSR1);
	event_add(arg,&tv);
}

static void timerout1(evutil_socket_t fd, short events, void *arg)
{
	event_base_loopbreak(event_get_base(arg));
	kill(getppid(),SIGUSR1);
}

static void timerout2(evutil_socket_t fd, short events, void *arg)
{
	printf("creat a child\n");
	pid_t pid = fork();
	if(pid == 0)
	{
		printf("%p\n",arg);	
		create_base();
		printf("pid %d exit!!!\n",getpid());
		exit(-1);
	}
	else
	{
	//	event_free(arg);
		printf("%p\n",arg);	
		return;
	}
}
static void create_base()
{
	signal(SIGINT,sig_int);
//	strTag->pid = getpid();
	struct event_base *base = event_base_new();
	struct timeval tv = {5,0};
	struct event sig;
	struct event timeout;
	evtimer_assign(&timeout,base,timerout1,&timeout);
	evtimer_add(&timeout,&tv);
	evsignal_assign(&sig,base,SIGUSR2,sig_act1,&sig);
	evsignal_add(&sig,NULL);
	event_base_dispatch(base);
	printf("child break loopbase!!\n");
	event_base_free(base);
	sleep(5);
}
