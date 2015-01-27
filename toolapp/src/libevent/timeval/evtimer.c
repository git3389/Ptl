#include "evtimer.h"
static void evtimer_cb1(evutil_socket_t fd,short evs_flags,void *arg);
/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Mon Dec  8 14:16:10 CST 2014
 *
 *  Purpose:
 *			init timer event
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void evtimer()
{
	struct event_base *base;
	//struct event *ev_timeout;
	struct event ev_timeout;
	struct event ev_timeout1;
	struct timeval sec;      //定时时间 
	struct arg args;
	int evrs;
	//1 初始化event库
	base = event_base_new();
	
	if(base == NULL)
	{
		printf("event_base_naw error!!!\n");
		return ;
	}

	//2 创建事件
	//-1没有文件描述符，EV_PERSIST事件发生并处理后不从事件队列中移除该事件
	//第一种方式	
	//ev_timeout = event_new(base,-1,EV_PERSIST,evtimer_cb,(void*)base);
	//第二种方式
	//使用定义的宏
	//ev_timeout = evtimer_new(base,evtimer_cb,(void *)base);
#if 1
	args.num = 10;
	args.timer = &ev_timeout;
	args.base = base;
	evrs = evtimer_assign(&ev_timeout,base,evtimer_cb,(void*)&args);
	if(evrs == -1)
	{
		event_base_free(base);
		printf("evtimer_assign error!!\n");
		return;
	}

	evrs = evtimer_assign(&ev_timeout1,base,evtimer_cb1,(void*)&ev_timeout1);
	//3 注册事件
	evutil_timerclear(&sec);	
	sec.tv_sec = 2;
	sec.tv_usec = 0;
	evrs = event_add(&ev_timeout,&sec);
	evrs = event_add(&ev_timeout1,&sec);
	if(evrs == -1)
	{
		event_base_free(base);
		printf("evtimer_add error!!\n");
		return;
	}
#endif
	//4 事件循环处理

	evrs = event_base_dispatch(base);	
	if(evrs == -1)
	{
//		event_del(&ev_timeout);
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
 *  Date: Mon Dec  8 14:16:48 CST 2014
 *
 *  Purpose:
 *			timer event callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void evtimer_cb(evutil_socket_t fd,short evs_flags,void *arg)
{
	struct arg *args = (struct arg *)arg;
	static int count = 1;
	struct timeval sec = {2,0};      //定时时间 
	sleep(10);
	if(args->num <=0)
	{
		event_base_loopbreak(args->base);
	}
	args->num--;
	event_add(args->timer,&sec);
}

void evtimer_cb1(evutil_socket_t fd,short evs_flags,void *arg)
{
	struct timeval sec = {1,0};      //定时时间 
	printf("call %s !!\n",__func__);
	event_add(arg,&sec);
}
