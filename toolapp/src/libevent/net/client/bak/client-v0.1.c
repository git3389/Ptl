#include "common.h"
#include "ptlpack.h"
#include "client.h"

//global
struct sockaddr_in server_addr;
struct event_base *base;  
int vpdCount;

//static


void sysinit(char *argv[])
{

	int evrs = 0;
	pthread_t tid;
	struct event timeout;
	struct timeval tv = {2,0};
	vpdCount = THREAD_MAX;
	//1 ���������ַ�ṹ
	memset(&server_addr,0x00,sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	//2 ����event_base�¼�����
	base = event_base_new();
	if(base == NULL)
	{
		vLogErr(EL_ERR,"����base���ϳ���!\n");
		return;
	}
	//����һ����ʱ��
	if(evtimer_assign(&timeout,base,evtimer_cb,&timeout) < 0)
	{
		vLogErr(EL_ERR,"��ʱ���¼�����ʧ��!\n");
		event_base_free(base);
	}
	if(event_add(&timeout,&tv) < 0)
	{
		vLogErr(EL_ERR,"��ʱ������ʧ��!\n");
		event_base_free(base);
	}
#if 0
	//3 ��������
	pthread_attr_t strSubAttr;
	pthread_attr_init(&strSubAttr);
	pthread_attr_setdetachstate(&strSubAttr,PTHREAD_CREATE_DETACHED);
	if(pthread_create(&tid,&strSubAttr,clthread,NULL) != 0)
	{
		vLogErr(EL_ERR,"�����߳�ʧ��!\n");
		event_base_free(base);
		pthread_attr_destroy(&strSubAttr);
		return;
	}
	pthread_attr_destroy(&strSubAttr);
	sleep(1);
	fprintf(fp,"init:\n");
	event_base_dump_events(base,fp);
	fflush(fp);
#endif
	//4 �¼�����ѭ��
	evrs = event_base_dispatch(base);
	if(evrs == -1)
	{
		event_base_free(base);
		vLogErr(EL_ERR,"�¼�ѭ���˳��쳣!\n");
		return;
	}
	vLogErr(EL_INFO,"�����¼��������!\n");
	//5 �ͷ���Դ
	event_base_free(base);
	vLogErr(EL_INFO,"��Դ�ͷ����\n");
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
void evclient()
{
	int evrs;
	pthread_t sndid;
	
	//1 ����bufferevent
	
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	if(bev == NULL)
	{
		perror("bufferevent_socket_new error!!!\n");
		return;
	}
	
	//2 ���������������
	
	evrs = bufferevent_socket_connect(bev,(struct sockaddr *)&server_addr,sizeof server_addr);
	if(evrs == -1)
	{
		bufferevent_free(bev);
		vLogErr(EL_ERR,"connect error!\n");
		return;
	}

	//3 ���ûص�	
	bufferevent_setcb(bev,read_cb,write_cb,event_cb,(void *)base);
	bufferevent_enable(bev,EV_READ|EV_WRITE);

	//4 ����write�߳�	
	pthread_attr_t strSubAttr;
	pthread_attr_init(&strSubAttr);
	pthread_attr_setdetachstate(&strSubAttr,PTHREAD_CREATE_DETACHED);
	if(pthread_create(&sndid,&strSubAttr,send_msg,(void*)bev) != 0)
	{
		vLogErr(EL_ERR,"create send msg thread error!\n");
		bufferevent_free(bev);
		pthread_attr_destroy(&strSubAttr);
		return;
	}
	pthread_attr_destroy(&strSubAttr);
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
	char buf[1024];
	size_t len = 0;
	//Ϊ���¼�����
	
	//�����뻺�����ж�ȡ����
	memset(buf,0x00,sizeof buf);
	if(evbuffer_remove(input,buf,4) < 0)
	{
		vLogErr(EL_ERR,"read from input error!\n");
		return;
	}

	len = atoi(buf);
	vLogErr(EL_DEBUG,"recv len: [%d]\n",len);

	if(len <= 0)
	{
		vLogErr(EL_ERR,"read from input len error!\n");
		return;
	}
	memset(buf,0x00,sizeof buf);
	evbuffer_remove(input,buf,len);
	vLogErr(EL_DEBUG,"recv msg: [%s]\n",buf);

	/*
	printf("> ");
	memset(buf,0x00,sizeof buf);
	gets(buf);
	bufferevent_write(bev,buf,strlen(buf));
	*/
	//�ͷ�
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
	//Ϊ���¼�����
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
	//�ͷ�
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
	if(events & BEV_EVENT_CONNECTED)
	{
		vLogErr(EL_INFO,"connected!\n");
	}
	else
	{
		if(events & BEV_EVENT_READING )
		{
			vLogErr(EL_ERR,"reading error!\n");
		}
		else if(events & BEV_EVENT_WRITING)
		{
			vLogErr(EL_ERR,"writing error!\n");
		}
		else if(events & BEV_EVENT_EOF)
		{
			vLogErr(EL_ERR,"eof file!\n");
		}
		else if(events & BEV_EVENT_TIMEOUT)
		{
			vLogErr(EL_ERR,"timeout!\n");
		}
		else if(events & BEV_EVENT_ERROR)
		{
			vLogErr(EL_ERR,"error!\n");
		}
	}
}

void *send_msg(void *arg)
{
//	sleep(1);
//	printf("%s called\n",__func__); 
	struct bufferevent *sndbev = (struct bufferevent *)arg;
	int num = 0;
	int i = 0;
	MsgBagInf *pMsgBag = NULL;
	pMsgBag = nMsgPack(SEND_MSG);
	if(pMsgBag == NULL)
	{
		vLogErr(EL_ERR,"pack error!\n");
		bufferevent_disable(sndbev,EV_READ|EV_WRITE);
		bufferevent_free(sndbev);
		return;
	}
	while(1)
	{
		if(i>= 10)
		{
			break;
		}
		vLogErr(EL_DEBUG,"snd msg [%s len:%d len1:%d]\n",
						pMsgBag->msg,
						pMsgBag->len,
						strlen(pMsgBag->msg));
		if(bufferevent_write(sndbev,
			(void *)(pMsgBag->msg),pMsgBag->len + 4) < 0)
		{
			vLogErr(EL_ERR,"send error!\n");
		}
		//bufferevent_write(sndbev,SEND_MSG,30);
		i++;
		sleep(1);
//		pthread_mutex_unlock(&thread_mutex);
	}
	//bufferevent_disable(sndbev,EV_READ | EV_WRITE);
//	event_base_loopexit(base,NULL);
	bufferevent_disable(sndbev,EV_READ|EV_WRITE);
	bufferevent_free(sndbev);
	nMsgPackFree(pMsgBag);
	vLogErr(EL_INFO,"request filish\n");
}

void *clthread(void *arg)
{
	int vpdCount = THREAD_MAX;
	while(vpdCount--)
	{
		vLogErr(EL_INFO,"create %d vpd!\n",vpdCount);	
		evclient();
		sleep(1);
	}
	vLogErr(EL_INFO,"create vpd filish!\n");
}

void evtimer_cb(evutil_socket_t fd,short evs_flags,void *arg)
{
	struct event *timeout = (struct event *)arg;
	struct timeval sec = {2,0};
	if(vpdCount)
	{
		//��������ƽ̨�豸
		vLogErr(EL_INFO,"create %d vpd!\n",vpdCount);	
		evclient();
		vpdCount--;
	}
	event_add(timeout,&sec);		
}
