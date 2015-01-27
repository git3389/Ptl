#include "evbuf.h"

//global

char *msg = "hello!!!!";




/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 13:23:05 CST 2014
 *
 *  Purpose:
 *			bufferevent init
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void evbuf(char *argv[])
{

	int evrs;
	
	//1 为创建的socket邦定一个地址
	
	struct sockaddr_in server_addr;
	memset(&server_addr,0x00,sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	//2 创建event_base事件集合
	struct event_base *base;  
	base = event_base_new();
	if(base == NULL)
	{
		perror("event_new error!!!\n");
		return;
	}	
	
	//3 添加监听事件
	struct evconnlistener *eclistener;
	eclistener = evconnlistener_new_bind(base,listener_cb,NULL,
			LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,-1,(struct sockaddr *)&server_addr,sizeof server_addr);

	if(eclistener == NULL)
	{
		event_base_free(base);
		perror("evconnlistener_new error!\n");
		return;
	}
	//启动一下连接
	evconnlistener_enable(eclistener);

	//5 创建bufferevnet,并向自身发起连接
	struct bufferevent *bev;
	evutil_socket_t fd = evconnlistener_get_fd(eclistener);
	bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev,read_cb,NULL,event_cb,NULL);  //设置回调
	bufferevent_enable(bev,EV_READ|EV_WRITE);
	bufferevent_socket_connect(bev,(struct sockaddr *)&server_addr,sizeof server_addr);
	
	//6 事件处理循环
	evrs = event_base_dispatch(base);
	if(evrs == -1)
	{
		evconnlistener_free(eclistener);
		event_base_free(base);
		perror("event_base_dispatch errror!!!\n");
		return;
	}
	//7 资源释放
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
 *  Date: Tue Dec  9 13:23:39 CST 2014
 *
 *  Purpose:
 *             socket listener callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void listener_cb(struct evconnlistener *listener, evutil_socket_t socket, struct sockaddr *addr, int socklen, void *arg)
{
	printf("%s called!!!\n",__func__);
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base,socket,BEV_OPT_CLOSE_ON_FREE);
	if(bev == NULL)
	{
		printf("%s:bufferevent_socket_new error!!!\n",__func__);
		event_base_loopbreak(base);
	}
	
	bufferevent_setcb(bev,NULL,write_cb,event_cb,NULL);  //设置回调
	bufferevent_enable(bev,EV_READ|EV_WRITE);
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 13:24:42 CST 2014
 *
 *  Purpose:
 *         read callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void read_cb(struct bufferevent *bev, void *ctx)
{
	printf("%s called!!!\n",__func__);
	char buf[1024] = {0};
	bufferevent_read(bev,buf,strlen(msg));
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 13:25:21 CST 2014
 *
 *  Purpose:
 *			write callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void write_cb(struct bufferevent *bev, void *ctx)
{
	printf("%s called!!!\n",__func__);
	//bufferevent_write(bev,msg,strlen(msg));
	//向socket中写
	struct evbuffer *output;
	evutil_socket_t fd = bufferevent_getfd(bev);
	output = bufferevent_get_output(bev);
	evbuffer_add(output,msg,strlen(msg));
	evbuffer_write(output,fd);
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 13:25:56 CST 2014
 *
 *  Purpose:
 *			bufferevent callback funtion
 *
 *  Params:
 *
 *  Return:
 *
 *  ###################################################
 */
void event_cb(struct bufferevent *bev,short events, void *ctx)
{
	printf("%s called!!!\n",__func__);
}
