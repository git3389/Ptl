#include "common.h"
#include "ptlpack.h"
#include "server.h"

//global

//static

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 09:48:32 CST 2014
 *
 *  Purpose:
 *         server event init
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void evserver(char *argv[])
{
	int evrs;
	//1 创建socket
#if 0	
	int socket_fd;
	socket_fd = socket(AF_INET,SOCK_STREAM,0);

	if(socket_fd == -1)
	{
		perror("socket create fail!!!!\n");
		return ;
	}
#endif
	//2 为创建的socket邦定一个地址
		
	struct sockaddr_in server_addr;
	memset(&server_addr,0x00,sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
#if 0
	if(bind(socket_fd,
			(struct sockaddr *)&server_addr,sizeof server_addr) == -1)
	{
		close(socket_fd);
		perror("bind error!!!\n");
		return ;
	}
#endif
	//3 创建event_base事件集合
	struct event_base *base;  
	base = event_base_new();
	if(base == NULL)
	{
		vLogErr("event base new error!\n");
		return;
	}	
	
	//4 添加监听事件
	struct evconnlistener *eclistener;
	eclistener = evconnlistener_new_bind(base,listener_cb,NULL,
			LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,-1,(struct sockaddr *)&server_addr,sizeof server_addr);

	if(eclistener == NULL)
	{
		event_base_free(base);
		vLogNM(EL_ERR,"server.err","evconnlistener_new error!\n");
		return;
	}
	
	//5 事件处理循环
	evrs = event_base_dispatch(base);
	if(evrs == -1)
	{
		evconnlistener_free(eclistener);
		event_base_free(base);
		vLogNM(EL_ERR,"server.err","event_base_dispatch errror!!!\n");
		return;
	}
	//6 资源释放
	evconnlistener_free(eclistener);
	event_base_free(base);
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Wed Dec 10 17:26:11 CST 2014
 *
 *  Purpose:
 *			Allocated a new read_buf_q
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
PRBQ read_buf_q_new()
{
	struct read_buf_q rbq;
	rbq.evb = evbuffer_new();
	rbq.fd = 0;
	if(rbq.evb == NULL)
	{
		return NULL;
	}
	return &rbq;
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Wed Dec 10 17:32:57 CST 2014
 *
 *  Purpose:
 *			free a struct read_buf_q
 *			
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void read_buf_q_free(PRBQ pr)
{
	evbuffer_free(pr->evb);
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 09:48:55 CST 2014
 *
 *  Purpose:
 *			listener callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void listener_cb(struct evconnlistener *listener, evutil_socket_t socket, 
		struct sockaddr *addr, int socklen, void *arg)
{
	struct event_base *base = evconnlistener_get_base(listener);
	//处理客户端的请求
	struct bufferevent *bev = bufferevent_socket_new(base,socket,BEV_OPT_CLOSE_ON_FREE);
	if(bev == NULL)
	{
		vLogNM(EL_ERR,"server.err","bufferevent_socket_new error!!!\n");
		return;
	}
	//bufferevent_write(bev,WELCOME,strlen(WELCOME));
	bufferevent_setcb(bev,read_cb,NULL,event_cb,NULL);
	bufferevent_enable(bev,EV_WRITE|EV_READ);
#if 0
	bufferevent_write(bev,"welcome!!!!",11);
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
	evbuffer_add(output,"welcome!!!!",11);
	char buf[1024];
	int len = evbuffer_get_length(input);
	evbuffer_remove(input,buf,len);
	printf("recv:%s\n",buf);

	evbuffer_add(output,buf,len);
#endif
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 17:06:33 CST 2014
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
	printf("%s called !!!\n",__func__);
//	bufferevent_write(bev,"welcome!!!!",11);
}

void read_cb(struct bufferevent *bev, void *ctx)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
	int len = 0;
	int nLen = 0;
//	MsgBagInf *pMsgBag;
	char buf[DATA_MAX];
//	evbuffer_remove(input,buf,inputlen);
	memset(buf,0x00,sizeof buf);	
	if(evbuffer_remove(input,&len,4) < 0)
	{
		vLogNM(EL_ERR,"server.err","read len error!\n");
		return;
	}
	nLen = ntohl(len);
	printf("msg len : [%d] \n",nLen);
	if(nLen <= 0)
	{
		vLogNM(EL_ERR,"server.err","read len error!\n");
		return;
	}
	memset(buf,0x00,sizeof buf);	
	printf("input len: [%d] \n",evbuffer_get_length(input));
	if(evbuffer_remove(input,buf,nLen) < 0)
	{
		vLogNM(EL_ERR,"server.err","read msg error!\n");
		return;
	}
	//vLogErr(EL_DEBUG,"msg : [%x] \n",buf[15]);
	printf("msg : [%s] msg len [%d]\n",buf,strlen(buf));
//	pMsgBag = nMsgPack(buf);	
//	evbuffer_add(output,(void *)pMsgBag->msg,pMsgBag->len + 4);
//	nMsgPackFree(pMsgBag);
}


void event_cb(struct bufferevent *bev,short events, void *ctx)
{
	printf("%s called\n",__func__);
	vLogNM(EL_ERR,"server.err","fd error is [%d] and error string [%s]",
		evutil_socket_geterror(bufferevent_getfd(bev)),
		evutil_socket_error_to_string(evutil_socket_geterror(bufferevent_getfd(bev))));
	if(events & BEV_EVENT_CONNECTED)
	{
		vLogNM(EL_ERR,"server.err","connect ok!!!");
	}
	else
	{
		if(events & BEV_EVENT_READING )
		{
			vLogNM(EL_ERR,"server.err","error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogNM(EL_ERR,"server.err","reading  error!!!!");
		}
		if(events & BEV_EVENT_WRITING)
		{
			vLogNM(EL_ERR,"server.err","error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogNM(EL_ERR,"server.err","writing  error!!!");
		}
		if((events & BEV_EVENT_READING) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogNM(EL_ERR,"server.err","error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogNM(EL_ERR,"server.err","reading timeout error!!!");
		}
		if((events & BEV_EVENT_WRITING) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogNM(EL_ERR,"server.err","error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogNM(EL_ERR,"server.err","writing timeout error!!!");
		}

		if(events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
		{
			vLogNM(EL_ERR,"server.err","error [%d] [%s]!",events,evutil_socket_error_to_string(events));
			vLogNM(EL_INFO,"server.err","disconnect!!!");
			bufferevent_free(bev);
		}
	}
}
#if 0
void *respond(void *arg)
{

}
#endif
