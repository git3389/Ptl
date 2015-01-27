#include "common.h"
#include "ptlpack.h"
#include "client.h"

//global
struct sockaddr_in server_addr;
int vpdCount;
int baseCount;

//static
static void SndTimer(evutil_socket_t fd,short evs_flags,void *arg);


void sysinit(char *argv[])
{

	int evrs = 0;
	struct event_base *main_base;
	struct event *strSndTimer;
	struct timeval tvSnd = {1,0};
	PtlDef strPtlDef;
	memset(&server_addr,0x00,sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	//2 创建event_base事件集合
	main_base = event_base_new();
	if(main_base == NULL)
	{
		vLogErr("创建main_base集合出错!\n");
		return;
	}
	//1 创建bufferevent
	
	struct bufferevent *bev;
	bev = bufferevent_socket_new(main_base,-1,BEV_OPT_CLOSE_ON_FREE);
	if(bev == NULL)
	{
		perror("bufferevent_socket_new error!!!\n");
		return;
	}
	
	//2 向服务器发起连接
	evrs = bufferevent_socket_connect(bev,(struct sockaddr *)&server_addr,sizeof server_addr);
	if(evrs == -1)
	{
		bufferevent_free(bev);
		vLogErr("connect error!\n");
		return;
	}
	//3 设置回调	
	bufferevent_setcb(bev,read_cb,write_cb,event_cb,(void *)main_base);
	bufferevent_enable(bev,EV_READ|EV_WRITE);

	strPtlDef.bev = bev;
	strPtlDef.sndTimer = strSndTimer;
	strPtlDef.nSndCount = 1000;
	strPtlDef.base = main_base;
	strSndTimer = evtimer_new(main_base,SndTimer,&strPtlDef);
	if(strSndTimer == NULL)
	{
		bufferevent_free(bev);
		vLogErr("sndtimer new error!\n");
		return;
	}
	evtimer_add(strSndTimer,&tvSnd);
	//4 事件处理循环
	evrs = event_base_dispatch(main_base);
	if(evrs == -1)
	{
		event_base_free(main_base);
		vLogErr("事件循环退出异常!\n");
		return;
	}
	vLogErr("所有事件处理完成!\n");
	//5 释放资源
	event_base_free(main_base);
	vLogErr("资源释放完成\n");
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 14:52:42 CST 2014
 *
 *  Purpose:
 *			read callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void read_cb(struct bufferevent *bev, void *ctx)
{
	/*
	struct evbuffer *input = bufferevent_get_input(bev);
	char buf[1024];
	size_t len = 0;
	//为读事件加锁
	
	//从输入缓冲区中读取数据
	memset(buf,0x00,sizeof buf);
	if(evbuffer_remove(input,buf,4) < 0)
	{
		vLogErr("read from input error!\n");
		return;
	}

	len = atoi(buf);

	if(len <= 0)
	{
		vLogErr("read from input len error!\n");
		return;
	}
	memset(buf,0x00,sizeof buf);
	evbuffer_remove(input,buf,len);
	*/
//	vLogErr(EL_DEBUG,"recv msg: [%s]\n",buf);

}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 14:52:47 CST 2014
 *
 *  Purpose:
 *
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void write_cb(struct bufferevent *bev, void *ctx)
{
}


/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 14:52:54 CST 2014
 *
 *  Purpose:
 *
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void event_cb(struct bufferevent *bev,short events, void *ctx)
{
	vLogErr(" the most recent error [%d] [%s]", EVUTIL_SOCKET_ERROR(),
		evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
	vLogErr("fd error is [%d] and error string [%s]",
		evutil_socket_geterror(bufferevent_getfd(bev)),
		evutil_socket_error_to_string(evutil_socket_geterror(bufferevent_getfd(bev))));
	if(events & BEV_EVENT_CONNECTED)
	{
		vLogErr("connected!\n");
	}
	else
	{
		if(events & BEV_EVENT_READING )
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogErr("reading  error!\n");
		}
		if(events & BEV_EVENT_WRITING)
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogErr("writing  error!\n");
		}
		if((events & BEV_EVENT_READING) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogErr("reading timeout error!\n");
		}
		if((events & BEV_EVENT_WRITING) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogErr("writing timeout error!\n");
		}
		if(events & BEV_EVENT_EOF)
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogErr("eof file!\n");
		}
		if(events & BEV_EVENT_ERROR)
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
		}
		if((events & BEV_EVENT_ERROR) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogErr("error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogErr("timeout error!\n");
		}
		bufferevent_free(bev);
	}
}

static void SndTimer(evutil_socket_t fd,short evs_flags,void *arg)
{
	
	PtlDef *pPtlDef = arg;
	struct timeval tvSnd = {1,0};
	struct event *strSndTimer;
	struct event_base *base = pPtlDef->base;

	#if 1
	//pack msg
	MsgBagInf *pMsgBag = NULL;
	pMsgBag = nMsgPack(SEND_MSG);
	if(pMsgBag == NULL)
	{
		vLogErr("pack error!\n");
		return;
	}
	if(pPtlDef->nSndCount <= 0)
	{
		bufferevent_disable(pPtlDef->bev,EV_READ|EV_WRITE);
		bufferevent_free(pPtlDef->bev);
		return;	
	}
	printf("writing ...\n");
	if(bufferevent_write(pPtlDef->bev,
		(void *)(pMsgBag->msg),pMsgBag->len + 4) < 0)
	{
		vLogErr("send error!\n");
	}
	printf("write end.\n");
	pPtlDef->nSndCount--;
	nMsgPackFree(pMsgBag);
	vLogErr("send %d conut!\n",pPtlDef->nSndCount);
	#endif
	strSndTimer = evtimer_new(base,SndTimer,pPtlDef);
	event_add(strSndTimer,&tvSnd);
}
