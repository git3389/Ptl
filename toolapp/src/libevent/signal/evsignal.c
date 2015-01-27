#include "evsignal.h"
#define SIGUSER2 (SIGUSER2 + 1)

void evtimer_cb(evutil_socket_t fd,short evs_flags,void *arg);

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Mon Dec  8 16:42:46 CST 2014
 *
 *  Purpose:
 *         init signal event
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void evsignal()
{
	struct event_base * base;
	struct event ev_signal;
	int evrs;               //执行结果
	struct event timeout;
	struct timeval tv = {2,0};

	//1 创建event_base事件集合
	base = event_base_new();
	if(base == NULL)
	{
		printf("event_base_naw error!!!\n");
		return ;
	}

	//2 创建一个事件
	evrs = evsignal_assign(&ev_signal,base,SIGTERM,evsignal_cb,(void *)&ev_signal);
	if(evrs == -1)
	{
		event_base_free(base);
		printf("evsignal_assign error!!\n");
		return;
	}

	//3 注册事件
	evrs = evsignal_add(&ev_signal,NULL);
	if(evrs == -1)
	{
		event_base_free(base);
		printf("evsignal_add error!!\n");
		return;
	}
	evtimer_assign(&timeout,base,evtimer_cb,&timeout);	
	evtimer_add(&timeout,&tv);
	//4 事件处理循环
	evrs = event_base_dispatch(base);
	if(evrs == -1)
	{
		event_del(&ev_signal);
		event_base_free(base);
		printf("event_dispatch error!!\n");
		return;

	}
	//5 释放资源
	event_base_free(base);
	printf("deal finish!!!\n");

}


/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Mon Dec  8 16:42:51 CST 2014
 *
 *  Purpose:
 *		   signal event callback funtion						
 *		
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */

void evsignal_cb(evutil_socket_t fd,short evs_flags,void *arg)
{
	static int count = 1;

	printf("send SIGUSR2 signal!\n");
	if(count >= 2)
	{
		event_del((struct event *)arg);
		event_base_loopbreak(event_get_base(arg));
	}
	count++;
}
void evtimer_cb(evutil_socket_t fd,short evs_flags,void *arg)
{
	struct timeval tv = {2,0};
//	kill(getpid(),SIGUSR2);
	evtimer_add(arg,&tv);
}
