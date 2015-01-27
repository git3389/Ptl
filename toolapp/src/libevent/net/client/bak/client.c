#include "client.h"
//global
ClDef _gstrCl[THREAD_MAX];
pthread_mutex_t thread_mutex;  //线程锁

int conn = 0;
//static


void sysinit(char *argv[])
{
	pthread_attr_t strAttr;
	int i = 0;
	int err = 0;
	pthread_attr_init(&strAttr);
	pthread_attr_setdetachstate(&strAttr,PTHREAD_CREATE_DETACHED);
	for(i = 0;i<THREAD_MAX;i++)
	{
		
		_gstrCl[i].flag = 1;
		err = pthread_create(&_gstrCl[i].pid,&strAttr,clthread,(void*)argv);
		if(err != 0)
		{
			printf("created thread error!\n");
		}
		printf("created %d client %u \n",i,(unsigned int)_gstrCl[i].pid);
//		sleep(2);
	}
	pthread_attr_destroy(&strAttr);
	for(;;)
	{
	}
	
}

/**
 *  ################################################
 *
 *  Fun:
 *
 *  Author:
 *
 *  Date: Tue Dec  9 14:52:01 CST 2014
 *
 *  Purpose:
 *          client event init
 *
 *  Params: 
 *			argv - ip and port
 *  Return:
 *
 *  ###################################################
 */
void evclient(char *argv[],event_base *base)
{
	int evrs;
	pthread_t sndid;
	
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
	
	struct sockaddr_in client_addr;
	memset(&client_addr,0x00,sizeof client_addr);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(atoi(argv[2]));
	client_addr.sin_addr.s_addr = inet_addr(argv[1]);

	//3 创建event_base事件集合
	struct event_base *base;  
	base = event_base_new();
	if(base == NULL)
	{
		perror("event_new error!!!\n");
		return;
	}

	//4 创建bufferevent
	
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	if(bev == NULL)
	{
		perror("bufferevent_socket_new error!!!\n");
		return;
	}
	

	//5 创建读写事件线程
	
	
	//6 向服务器发起连接
	
	evrs = bufferevent_socket_connect(bev,(struct sockaddr *)&client_addr,sizeof client_addr);
	if(evrs == -1)
	{
		bufferevent_free(bev);
		event_base_free(base);
		perror("bufferevent_socket_connect errror!!!\n");
		return;
	}
	conn = 1;
	//设置回调
	
	bufferevent_setcb(bev,read_cb,write_cb,event_cb,(void *)base);
//	bufferevent_setwatermark(bev,EV_READ,1,0);
//	bufferevent_setwatermark(bev,EV_WRITE,0,0);
	bufferevent_enable(bev,EV_READ|EV_WRITE);
	
	pthread_mutex_init(&thread_mutex,NULL);
	pthread_attr_t strSubAttr;
	pthread_attr_init(&strSubAttr);
	pthread_attr_setdetachstate(&strSubAttr,PTHREAD_CREATE_DETACHED);
	if(pthread_create(&sndid,&strSubAttr,send_msg,(void*)bev) != 0)
	{
		printf("send thread create error!\n");
		bufferevent_free(bev);
		event_base_free(base);
		pthread_attr_destroy(&strSubAttr);
		return;
	}
	pthread_attr_destroy(&strSubAttr);
	
	//7 事件处理循环
	evrs = event_base_dispatch(base);
	if(evrs == -1)
	{
		bufferevent_free(bev);
		event_base_free(base);
		perror("event_base_dispatch errror!!!\n");
		return;
	}
	printf("exit!\n");
	//8 释放资源
	bufferevent_free(bev);
	event_base_free(base);
	printf("disconnect!\n");

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
//	pthread_mutex_lock(&thread_mutex);
//	printf("%s called\n",__func__);
	struct evbuffer *input = bufferevent_get_input(bev);
	struct event_base * base = bufferevent_get_base(bev);
	char buf[1024];
	size_t len = 0;
	//为读事件加锁
	
	//从输入缓冲区中读取数据
	memset(buf,0x00,sizeof buf);
	len = evbuffer_get_length(input);
	evbuffer_remove(input,buf,len);
	printf("recv:%s\n",buf);
	if(strncmp(buf,"quit",4) == 0)
	{
//		event_base_loopbreak(base);
		return;
	}
	/*
	printf("> ");
	memset(buf,0x00,sizeof buf);
	gets(buf);
	bufferevent_write(bev,buf,strlen(buf));
	*/
	//释放
//	pthread_mutex_unlock(&thread_mutex);
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
//	printf("%s called\n",__func__);
#if 0
	struct event_base *base = bufferevent_get_base(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
	char buf[1024];
	//为读事件加锁
	bufferevent_lock(bev);
	memset(buf,0x00,sizeof buf);
	printf("> ");
	gets(buf);
	if(strncmp("quit",buf,4) == 0)
	{
		event_base_loopbreak(base);
		return;
	}

	evbuffer_add(output,buf,strlen(buf));
	//释放
	bufferevent_unlock(bev);
#endif
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
//	printf("%s called\n",__func__);
//	struct event_base * base = bufferevent_get_base(bev);
//	if(conn == 0)
//		event_base_loopbreak(base);
}

void *send_msg(void *arg)
{
//	printf("%s called\n",__func__); 
	char buf[1024];
	struct bufferevent *sndbev = (struct bufferevent *)arg;
	struct event_base * base = bufferevent_get_base(sndbev);
	int i = 0;
	while(1)
	{
//		pthread_mutex_lock(&thread_mutex);
//		printf("> ");
		memset(buf,0x00,sizeof buf);
//		gets(buf);
//		if(strncmp("quit",buf,4) == 0)
//		{
//			conn = 0;
//			pthread_mutex_unlock(&thread_mutex);
//			break;
//		}
		if(i>= 10)
		{
			bufferevent_write(sndbev,"quit",strlen("quit"));
			break;
		}
		sprintf(buf,"%u snd data %d",(unsigned int)pthread_self(),i++);
		bufferevent_write(sndbev,buf,strlen(buf));
//		pthread_mutex_unlock(&thread_mutex);
	}
	//bufferevent_disable(sndbev,EV_READ | EV_WRITE);
	event_base_loopexit(base,NULL);
	printf("thread exit!!!!\n");
}
void *clthread(void *arg)
{
	evclient((char **)arg);
	printf("request finish\n");
}
