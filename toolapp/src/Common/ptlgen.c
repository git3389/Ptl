/*====================================================================
 * 模块名  : ptlgen
 * 文件名  : ptlgen.c
 * 相关文件:
 * 实现功能: 创建子进程、子base、连接等功能函数
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12   v1.0        YLI                          创建
 * ====================================================================*/
#include "common.h"
#include "ptlmsgtimercb.h"


static void DealRcvMsg(const char *vspMsg,struct bufferevent *bev);
static int MsgTimerInit(struct event_base *base);
static void SubSigInt(int sig);
static int IsGenFulfill();
static PVPDLIST FindVpdfd(evutil_socket_t fd);
//callback
static void GenConnTimerCb(evutil_socket_t fd, short events, void *arg);
static void SubBaseIntCb(int sig);
static void KeepAliveCheckCb(evutil_socket_t fd, short events, void *arg);
static void ConnCountCb(evutil_socket_t fd, short events, void *arg);
static void ReconnCb(evutil_socket_t fd, short events, void *arg);
static void read_cb(struct bufferevent *bev, void *ctx);
static void write_cb(struct bufferevent *bev, void *ctx);
static void event_cb(struct bufferevent *bev,short what, void *ctx);

#define SET_REG_FLAG(fd,flag) \
	PVPDLIST strpVpd = FindVpdfd(fd); \
	if (strpVpd != NULL)\
	{\
		strpVpd->vpd.nReg = flag;\
	}

#define SET_KEEP_NUM(fd,flag)\
	PVPDLIST strpVpd = FindVpdfd(fd); \
	if (strpVpd != NULL)\
	{\
		if (flag == F_KEEP_INIT)\
		{\
			strpVpd->vpd.nHeartNum = S_KEEP_NUM;\
			strpVpd->vpd.nHeartFlag = F_HEART_OK;\
		}\
		else if(flag == F_KEEP_DEC)\
		{\
			strpVpd->vpd.nHeartNum--;\
		}\
	}

#define CHECK_KEEP_NUM(fd) \
	PVPDLIST strpVpd = FindVpdfd(fd); \
	if (strpVpd != NULL)\
	{\
		if (strpVpd->vpd.nHeartNum <= 0)\
		{\
			strpVpd->vpd.nConn = F_CONN_ERR;\
			strpVpd->vpd.nReg = F_REG_ERR;\
			if (strpVpd->vpd.bev != NULL)\
			{\
				bufferevent_free(strpVpd->vpd.bev);\
				return;\
			}\
		}\
	}

#define SESSION_ERR_(fd)\
	if (fd == -1)\
	{\
		return;\
	}\
	PVPDLIST pVpd = FindVpdfd(fd); \
	if (pVpd != NULL)\
	{\
		pVpd->vpd.nConn = F_CONN_ERR;\
		pVpd->vpd.nReg = F_REG_ERR;\
	}
	
#define REG_FAIL_(fd)\
	PVPDLIST pVpd = FindVpdfd(fd); \
	if (pVpd != NULL)\
	{\
		if (pVpd->vpd.bev != NULL)\
		{\
			bufferevent_free(pVpd->vpd.bev);\
		}\
		nMsgPackFree(pVpd->vpd.strpRegBag);\
		DelNode(pVpd->vpd);\
		_tPutConnCountDec(pVpd->vpd.nPidIdx,pVpd->vpd.nFlag);\
	}
	/*
#define LIST_ITER_START() \
	int nPidIdx = 0;\
	int i = 3;\
	PVPDLIST head[3] = {NULL,NULL,NULL};\
	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS)\ 
	{\
		vLogErr("_tGetPidIdx fail!!!!");\
		return;\
	}\
	if (_gstrpPid[nPidIdx].strpTerm != NULL)\
	{\
		head[2] = _gstrpPid[nPidIdx].strpTerm;\
	}\
	if (_gstrpPid[nPidIdx].strpTerm != NULL)\
	{\
		head[1] = _gstrpPid[nPidIdx].strpPhy;\
	}\
	if (_gstrpPid[nPidIdx].strpTerm != NULL)\
	{\
		head[0] = _gstrpPid[nPidIdx].strpLogic;\
	}\
	while(i)\
	{\
		if (head[i-1] == NULL)\
		{\
			i--;\
			continue;\
		}\
		head[i-1] = head[i-1]->next;\
		while(head[i-1])\
		{

#define LIST_ITER_END() \
		head[i-1] = head[i-1]->next;\
		}\
		i--;\
	}*/

/*====================================================================
 * 函数名    : GenConnTimerCb
 * 功能      : 定时产生终端或服务器与系统相连接的回调
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/7       v1.0        YLI                          创建
 * ====================================================================*/
static void GenConnTimerCb(evutil_socket_t fd, short events, void *arg)
{
	struct timeval tv = {0,0};
	struct bufferevent *bev = NULL;
	int nIdx = 0;
	int nFlag = 0;
	VPD strVpd;
	tv.tv_usec = _gstrpShm->rc.nGenConnTimer;
	if (_tGetPidIdx(getpid(),&nIdx) != SUCCESS)
	{
		vLogErr("_tGetPidIdx [%d] error!!!",getpid());
		return;
	}
	if (IsGenFulfill() == SUCCESS)
	{
		vLogTrace(EL_INFO,"All connect genertion fullfill!!");
		return;
	}

	//conn
	bev = vGenConn(_gstrpPid[nIdx].base);
	if (bev == NULL)
	{
		vLogErr("conn fail!!!");
		evtimer_add(arg,&tv);
		return;
	}
	memset(&strVpd,0x00,sizeof(strVpd));

	//get generate term or srv flag
	nFlag = _tGetGenTermOrSrvFlag();
	if (nFlag == F_NULL_FLAG)
	{
		vLogErr("Term or srv  count is max!!!");
		evtimer_add(arg,&tv);
		return;
	}
	
	//modify current vpd status
	strVpd.nFlag = nFlag;
	strVpd.nHeartNum = S_KEEP_NUM;
	strVpd.nHeartFlag = F_HEART_ERR;
	strVpd.nConn = F_CONN_OK;
	strVpd.bev = bev;
	strVpd.fd = bufferevent_getfd(bev);
	strVpd.nPidIdx = nIdx;
	strVpd.nReg = F_REG_ERR;
	//strVpd.nReg = F_REG_OK;
	//reg
	if(Regist(&strVpd) != SUCCESS)
	{
		strVpd.nReg = F_REG_ERR;
	}
	vLogNM(EL_DEBUG,"conn.log","on pid idx [%d] create [%d] conn\n"
		"devid [%s] devtype [%s] fd [%d]",nIdx,
		_gstrpPid[nIdx].nPhyCount +
		 _gstrpPid[nIdx].nLogicCount + 
		_gstrpPid[nIdx].nTermCount,
		strVpd.saDevId,
		strVpd.saDevType,
		bufferevent_getfd(bev));
	//insert list 
	if(_tPutConnToList(nIdx,strVpd) != SUCCESS)
	{
		vLogErr("Create vpd isn't success!!!");
	}
	else
	{
		_tPutConnCountInc(nIdx,nFlag);	
	}
	//_tPrintList(nIdx);
	if (!VPD_IS_MAX(nIdx))
	{
		evtimer_add(arg,&tv);
		return;
	}

	//in base term/srv is max 
	kill(getppid(),S_GENSUBBASE_SIG);
}

/*====================================================================
 * 函数名    : MsgTimerInit
 * 功能      : 产生所有要测试消息的定时器
 * 算法实现  : 
 * 参数说明  : base  产生的定时器要被base事件集合
 * 返回值说明: 成功  0
 *             失败  -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/9       v1.0        YLI                          创建
 * ====================================================================*/
static int MsgTimerInit(struct event_base *base)
{
	int i = 0;
	int eres;
	struct event *ev_keep_check = NULL;  //keep check event
	struct event *ev_reconn = NULL;      //reconn event
	struct event *ev_conn_count = NULL;
	struct timeval tv_keep_ckeck = {0,0};
	struct timeval tv_reconn = {0,0};
	struct timeval tv_conn = {5,0};

	for (i = 0; i < _gstrpShm->msgInf.nCount; ++i)
	{
		vLogTrace(EL_DEBUG,"msg [%d] [%s] cb fun %x",_gstrpMsg[i].nMsgType,_gstrpMsg[i].saMsgType,_gstrpMsg[i].fn);
		if (_gstrpMsg[i].fn == NULL)
		{
			continue;
		}
		_gstrpMsg[i].arg.nMsgType = _gstrpMsg[i].nMsgType;
		_gstrpMsg[i].arg.nFlag = _gstrpMsg[i].nFlag;
		_gstrpMsg[i].arg.timer = &_gstrpMsg[i].timer;
		eres = evtimer_assign(&_gstrpMsg[i].timer,base,_gstrpMsg[i].fn,(void *)&_gstrpMsg[i].arg);
		if (eres == -1)
		{
			vLogErr("Msg [%s] create timer fail!!!",_gstrpMsg[i].saMsgType);
			continue;
		}
		eres = evtimer_add(&_gstrpMsg[i].timer,&_gstrpMsg[i].tv);
		if (eres == -1)
		{
			vLogErr("Msg [%s] event add fail!!!",_gstrpMsg[i].saMsgType);
			continue;
		}
		_gstrpShm->msgInf.nUseCount++;
		vLogTrace(EL_INFO,"Msg [%d] timer success!!!",_gstrpShm->msgInf.nUseCount);
	}
	if (_gstrpShm->msgInf.nUseCount == 0)
	{
		vLogErr("Msg timer event add zero!!!");
		return FAILUER;
	}
	//the timer is check whether reviceve heart bag 
	ev_keep_check = event_new(base,-1,EV_PERSIST,KeepAliveCheckCb,(void *)ev_keep_check);
	if (ev_keep_check == NULL)
	{
		vLogErr("keep alive check timer create failure!!");
		return FAILUER;
	}
	tv_keep_ckeck.tv_sec = S_KEEP_ALIVE_CHECK;
	eres = evtimer_add(ev_keep_check,&tv_keep_ckeck);
	if (eres < 0)
	{
		event_free(ev_keep_check);
		vLogErr("keep alive check timer event add failure!!");
		return FAILUER;
	}
	//the timer is reconnect when connection disconnect with the server
	ev_reconn = event_new(base,-1,EV_PERSIST,ReconnCb,(void *)ev_reconn);
	if (ev_reconn == NULL)
	{
		vLogErr("reconnect timer create failure!!");
		return FAILUER;
	}
	tv_reconn.tv_sec = 5;
	eres = evtimer_add(ev_reconn,&tv_reconn);
	if (eres < 0)
	{
		event_free(ev_reconn);
		vLogErr("reconnect timer event add failure!!");
		return FAILUER;
	}

	ev_conn_count = event_new(base,-1,EV_PERSIST,ConnCountCb,(void *)ev_conn_count);
	if (ev_conn_count == NULL)
	{
		vLogErr("connect statistic count create timer fail!!!");
		return FAILUER;
	}
	eres = evtimer_add(ev_conn_count,&tv_conn);
	if (eres == -1)
	{
		vLogErr("connect statistic count event add fail!!!");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"Msg timer success count : [%d]",_gstrpShm->msgInf.nUseCount);
	return SUCCESS;
}
static void read_cb(struct bufferevent *bev, void *ctx)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	int nLenNL = 0;  //pack len is network byte order
	int nLenHL = 0;  //pack len is host byte order
	char saMsg[L_PACK_BAG_MAX_LEN];

	//read pack len
	if(evbuffer_remove(input,&nLenNL,4) < 0)
	{
		vLogNM(EL_ERR,"read.err","get fd [%d] pack length failure!!",
			bufferevent_getfd(bev));
		return;
	}
	nLenHL = ntohl(nLenNL);
	if(nLenHL <= 0)
	{
		vLogNM(EL_ERR,"read.err","get fd [%d] pack length failure!!",
			bufferevent_getfd(bev));
		return;
	}
	memset(saMsg,0x00,sizeof saMsg);
	//read data	
	if(evbuffer_remove(input,saMsg,nLenHL) < 0)
	{
		vLogNM(EL_ERR,"read.err","get fd [%d] pack data context failure!!",
			bufferevent_getfd(bev));
		return;
	}
	//deal receive msg
	DealRcvMsg(saMsg,bev);
}
static void write_cb(struct bufferevent *bev, void *ctx)
{

}
/*====================================================================
 * 函数名    : event_cb
 * 功能      : 处理终端/服务器与系统断开连接或出现异常时的回调函数
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
static void event_cb(struct bufferevent *bev,short events,void *ctx)
{
	if (bev == NULL)
	{
		return;
	}

	evutil_socket_t fd = bufferevent_getfd(bev);
	PVPDLIST strpTVpd = FindVpdfd(fd);

	if(events & BEV_EVENT_CONNECTED)
	{
		vLogTrace(EL_INFO,"fd [%d] connected!",fd);
	}
	//error
	else
	{
		if (strpTVpd != NULL)
		{
			vLogNMS(EL_INFO,"session.err","eventid [%s] devtype [%s]",
			strpTVpd->vpd.saDevId,strpTVpd->vpd.saDevType);
		}
		if(events & BEV_EVENT_READING )
		{
			vLogNM(EL_ERR,"session.err","fd [%d] reading error!!!",
				fd);
		}
		if(events & BEV_EVENT_WRITING)
		{
			vLogNM(EL_ERR,"session.err","fd [%d] writing error!!!",
				fd);
		}
		if((events & BEV_EVENT_READING) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogNM(EL_ERR,"session.err","fd [%d] reading timeout error!!!",
				fd);
		}
		if((events & BEV_EVENT_WRITING) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogNM(EL_ERR,"session.err","fd [%d] writing timeout error!!!",
				fd);
		}
		if(events & BEV_EVENT_EOF)
		{
			vLogNM(EL_ERR,"session.err","fd [%d] eof file reached!!!"
				,fd);
		}
		if(events & BEV_EVENT_ERROR)
		{
			vLogNM(EL_ERR,"session.err","fd [%d] error!!!",
				fd);
		}
		if((events & BEV_EVENT_ERROR) && (events & BEV_EVENT_TIMEOUT))
		{
			vLogNM(EL_ERR,"session.err","fd [%d] timeout error!!!",
				fd);
		}
		vLogNM(EL_ERR,"session.err","fd [%d] disconnected with the server!!!",
				fd);
		vLogNMS(EL_INFO,"heartnmu.log","devid [%s] SendHeartNUm:[%d]!!",
						strpTVpd->vpd.saDevId,
						strpTVpd->vpd.nSndHeartNum);
		//deal this is connect resouce
		SESSION_ERR_(fd)
	}
}

/*====================================================================
 * 函数名    : DealRcvMsg
 * 功能      : 处理终端/服务器接收到的信息
 * 算法实现  : 
 * 参数说明  : vspMsg 接收到的消息
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/19      v1.0        YLI                          创建
 * ====================================================================*/
static void DealRcvMsg(const char *vspMsg,struct bufferevent *bev)
{
	//static int revHart = 0;
	char buf[L_PACK_BAG_MAX_LEN];
	struct event_base *base = bufferevent_get_base(bev);
	struct timeval strKACTV = {0,0};  //keep alive check time
	struct event *evKACT = NULL; 
	memset(buf,0x00,sizeof buf);
	PVPDLIST strpVpd = FindVpdfd(bufferevent_getfd(bev));
	if (nMsgGetString(vspMsg,"eventid",buf,L_PATH_MAX_LEN) != SUCCESS)
	{
		vLogNM(EL_ERR,"read.err","Get received pack's eventid error!!!");
		return;
	}
	//reg msg
	if (strcmp(buf,"EV_REG_ACK") == 0)
	{
		//reg success
		SET_REG_FLAG(bufferevent_getfd(bev),F_REG_OK)
		vLogNMS(EL_INFO,"reg.log","fd [%d] registration ok!!",bufferevent_getfd(bev));
		return;
	}
	//reg fail
	else if (strcmp(buf,"EV_REG_NACK") == 0)
	{
		int nResCode = 0;
		SET_REG_FLAG(bufferevent_getfd(bev),F_REG_ERR)
		REG_FAIL_(bufferevent_getfd(bev))
		if(nMsgGetInt(vspMsg,"reasoncode",&nResCode) != SUCCESS)
		{
			vLogErr("Get fd [%d] registration failure reason code error!!!",
				bufferevent_getfd(bev));
		}
		vLogNMS(EL_ERR,"reg.err","fd [%d] registration failure reason code is [%d] [%s]",
			bufferevent_getfd(bev),nResCode,
			_get_code_error_to_string(nResCode));
		return;
	}
	//HEARTBEAT
	if (strcmp(buf,"EV_HEARTBEAT_ACK") == 0)
	{
		SET_KEEP_NUM(bufferevent_getfd(bev),F_KEEP_INIT)
		vLogNM(EL_INFO,"rev_heart.log","fd [%d] id [%s] [%s] count ",
			bufferevent_getfd(bev),strpVpd->vpd.saDevId,buf);
	}
}

/*====================================================================
 * 函数名    : FindVpdfd
 * 功能      : 在终端/服务器链表中查找，与当前fd相同的结点
 * 算法实现  : 
 * 参数说明  : fd  要查找的结点的fd
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/20       v1.0        YLI                          创建
 * ====================================================================*/
static PVPDLIST FindVpdfd(evutil_socket_t fd)
{
	VPD tVpd;
	PVPDLIST pVpd = NULL;
	memset(&tVpd,0x00,sizeof(VPD));
	tVpd.nFlag = F_TERM_FLAG;
	tVpd.fd = fd;
	pVpd = GetNode(tVpd,F_CURR_FLAG);
	if (pVpd == NULL)
	{
		memset(&tVpd,0x00,sizeof(VPD));
		tVpd.nFlag = F_PHY_FLAG;
		tVpd.fd = fd;
		pVpd = GetNode(tVpd,F_CURR_FLAG);
		if (pVpd == NULL)
		{
			memset(&tVpd,0x00,sizeof(VPD));
			tVpd.nFlag = F_LOGIC_FLAG;
			tVpd.fd = fd;
			pVpd = GetNode(tVpd,F_CURR_FLAG);
		}
	}
	if (pVpd != NULL)
	{
		vLogNMS(EL_DEBUG,"findvpd.log","[%d] eventid[%s] devtype[%s]",
			pVpd->vpd.nFlag,
			pVpd->vpd.saDevId,
			pVpd->vpd.saDevType);
	}
	
	return pVpd;
}
/*====================================================================
 * 函数名    : SubBaseIntCb
 * 功能      : 处理子进程结束信号回调
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/16       v1.0        YLI                          创建
 * ====================================================================*/
static void SubBaseIntCb(int sig)
{
	vLogTrace(EL_INFO,"sub base break sig recevied!!!!");
	int nPidIdx = 0;
	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS)
	{
		vLogErr(" _tGetPidIdx pid [%d] fail!!",getpid());
		vSubPidRelease();
		exit(-1);
	}
	if(event_base_loopbreak(_gstrpPid[nPidIdx].base) <0)
	{
		vLogErr("sub base break loop fail!!!");
	}
	UnRegist();
	vSubPidRelease();
}
static void SubSigInt(int sig)
{
	vLogTrace(EL_INFO,"pid [%d] int signal!!!",getpid());
}
/*====================================================================
 * 函数名    : KeepAliveCheckCb
 * 功能      : 检测连接是否有心跳包接收的事件回调函数
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/20       v1.0        YLI                          创建
 * ====================================================================*/
static void KeepAliveCheckCb(evutil_socket_t fd, short events, void *arg)
{
	vLogNMS(EL_DEBUG,"keep_tacre.log","check keep");
	int nPidIdx = 0;
	int res = 0;
	int i = 3;
	PVPDLIST head[3] = {NULL,NULL,NULL};
	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS) 
	{
		vLogErr("_tGetPidIdx fail!!!!");
		return;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[2] = _gstrpPid[nPidIdx].strpTerm;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[1] = _gstrpPid[nPidIdx].strpPhy;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[0] = _gstrpPid[nPidIdx].strpLogic;
	}
	while(i)
	{
		if (head[i-1] == NULL)
		{
			i--;
			continue;
		}
		head[i-1] = head[i-1]->next;
		while(head[i-1])
		{
			if ((head[i-1]->vpd.nConn == F_CONN_ERR)
				|| (head[i-1]->vpd.nReg == F_REG_ERR))
			{
				head[i-1] = head[i-1]->next;
				continue;
			}
			//check heart bag send attempts
			if (head[i-1]->vpd.nHeartNum <= 0)
			{
				head[i-1]->vpd.nConn = F_CONN_ERR;
				head[i-1]->vpd.nReg = F_REG_ERR;
				vLogNM(EL_ERR,"keep.err","connect [%d] [%s] disconnect with the server!!",
					head[i-1]->vpd.fd,head[i-1]->vpd.saDevId);
				vLogNMS(EL_INFO,"heartnmu.log","devid [%s] SendHeartNUm:[%d]!!",
						head[i-1]->vpd.saDevId,
						head[i-1]->vpd.nSndHeartNum);
			}
			else
			{	//check whether receive heart bag
				if (head[i-1]->vpd.nHeartFlag == F_HEART_ERR)
				{
					head[i-1]->vpd.nHeartNum--;
					vLogNMS(EL_DEBUG,"keep_tacre.log","[%d] [%s] don't rev hart bag",
						head[i-1]->vpd.fd,head[i-1]->vpd.saDevId);
				}
			}
			head[i-1] = head[i-1]->next;
		}
		i--;
	}
}

/*====================================================================
 * 函数名    : ReconnCb
 * 功能      : 尝试重新连接与服务器断开的那些连接
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/22       v1.0        YLI                          创建
 * ====================================================================*/
static void ReconnCb(evutil_socket_t fd, short events, void *arg)
{
	int nPidIdx = 0;
	int res = 0;
	int i = 3;
	int evrs;
	struct sockaddr_in server_addr;

	PVPDLIST head[3] = {NULL,NULL,NULL};
	memset(&server_addr,0x00,sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_gstrpShm->rc.nSrvPort);
	server_addr.sin_addr.s_addr = inet_addr(_gstrpShm->rc.nSrvIP);

	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS) 
	{
		vLogNM(EL_ERR,"reconn.err","_tGetPidIdx fail!!!!");
		return;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[2] = _gstrpPid[nPidIdx].strpTerm;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[1] = _gstrpPid[nPidIdx].strpPhy;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[0] = _gstrpPid[nPidIdx].strpLogic;
	}
	while(i)
	{
		if (head[i-1] == NULL)
		{
			i--;
			continue;
		}
		head[i-1] = head[i-1]->next;
		while(head[i-1])
		{
			if (head[i-1]->vpd.nConn == F_CONN_ERR)
			{
				if ((head[i-1]->vpd.fd == -1) || 
					(head[i-1]->vpd.saDevId == NULL)
					|| (strlen(head[i-1]->vpd.saDevId) == 0))
				{
					head[i-1] = head[i-1]->next;
					continue;
				}
				if (head[i-1]->vpd.bev == NULL )
				{
					//try create a new connection
					vLogNM(EL_INFO,"reconn.log","To [%s],trying create a new connect ...",
						head[i-1]->vpd.saDevId);
					head[i-1]->vpd.bev = vGenConn(_gstrpPid[head[i-1]->vpd.nPidIdx].base);
					if (head[i-1]->vpd.bev == NULL)
					{
						vLogNM(EL_INFO,"reconn.log","To [%s],trying create new connect failure!!!"
							,head[i-1]->vpd.saDevId);
					}
					else
					{
						vLogNM(EL_INFO,"reconn.log","To [%s] create a new connect !!!",
							head[i-1]->vpd.saDevId);
						//regist agin
						vLogNM(EL_INFO,"reconn.log","register [%s] ....",
							head[i-1]->vpd.saDevId);
						if (bufferevent_write(head[i-1]->vpd.bev,
									head[i-1]->vpd.strpRegBag->msg,
									head[i-1]->vpd.strpRegBag->len) < 0)
						{
							vLogNM(EL_INFO,"reconn.log","register [%s] failure!!!!",
								head[i-1]->vpd.saDevId);
						}
					}
				}
				else
				{
					//try reconnect
					vLogNM(EL_INFO,"reconn.log","To [%s],trying  reconnect ...",
						head[i-1]->vpd.saDevId);
					evrs = bufferevent_socket_connect(head[i-1]->vpd.bev,
							(struct sockaddr *)&server_addr,sizeof server_addr);
					if(evrs == -1)
					{
						vLogNM(EL_INFO,"reconn.log","To [%s],trying  reconnect failure",
						head[i-1]->vpd.saDevId);
					}
					else
					{
						//register agin
						vLogNM(EL_INFO,"reconn.log","The [%s]  establish the connection with server!!!",
						head[i-1]->vpd.saDevId);
						vLogNM(EL_INFO,"reconn.log","register [%s] ....",
							head[i-1]->vpd.saDevId);
						if (bufferevent_write(head[i-1]->vpd.bev,
							head[i-1]->vpd.strpRegBag->msg,
							head[i-1]->vpd.strpRegBag->len) < 0)
						{
							vLogNM(EL_INFO,"reconn.log","register [%s] failure!!!!",
								head[i-1]->vpd.saDevId);
						}

					}
				}
			}
			head[i-1] = head[i-1]->next;
		}
		i--;
	}
}
/*====================================================================
 * 函数名    : ConnCountCb
 * 功能      : 统计所有连接成功的数量
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
static void ConnCountCb(evutil_socket_t fd, short events, void *arg)
{
	struct timeval tv_conn = {5,0};
	int nPidIdx = 0;
	int nSum = 0;
	int i = 3;

	if(_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS)
	{
		vLogErr("_tGetPidIdx failure!!!");
		return;
	}
	PVPDLIST head[3] = {NULL,NULL,NULL};
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[2] = _gstrpPid[nPidIdx].strpTerm;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[1] = _gstrpPid[nPidIdx].strpPhy;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[0] = _gstrpPid[nPidIdx].strpLogic;
	}
	while(i)
	{
		if (head[i-1] == NULL)
		{
			i--;
			continue;
		}
		head[i-1] = head[i-1]->next;
		while(head[i-1])
		{
			if ((head[i-1]->vpd.nConn == F_CONN_OK)
				&& (head[i-1]->vpd.nReg == F_REG_OK))
			{
				nSum++;
			}
			head[i-1] = head[i-1]->next;
		}
		i--;
	}
	_tPutConnCount(nPidIdx,nSum);
}
/*====================================================================
 * 函数名    : IsGenFulfill
 * 功能      : 判断虚拟的终端/服务器是否已经为测试要生成的总数
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:  是  0
 *				否  -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/16       v1.0        YLI                          创建
 * ====================================================================*/
static int IsGenFulfill()
{
	int nTermSumCount = 0;
	int nPhySumCount = 0;
	int nLogicSumCount = 0;
	int nGenSum = 0;
	int i = 0;
	for (i = 0; i < _gstrpShm->pidInf.nUseCount; ++i)
	{
		nTermSumCount += _gstrpPid[i].nTermCount;
		nPhySumCount += _gstrpPid[i].nPhyCount;
		nLogicSumCount += _gstrpPid[i].nLogicCount;
	}
	//get generate connect sum
	nGenSum = _gstrpShm->rc.nTermNum + _gstrpShm->rc.nPhyNum
			+ _gstrpShm->rc.nLogicNum;

	if ((nTermSumCount + nPhySumCount + nLogicSumCount) > (nGenSum -1))
	{
		return SUCCESS;
	}
	return FAILUER;
}

 /*====================================================================
 * 函数名    : vGenBase
 * 功能      : 创建子进程的base以及启动所有的要测试的消息定时器，创建生成终端或
 *             服务器的定时器
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明: 成功  0
 *             失败  -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
int vGenBase()
{
	signal(SIGINT,SubSigInt);  //deal int signal
	signal(SIGTERM,SubSigInt);  //deal kill signal
	signal(S_SUBBASEINT_SIG,SubBaseIntCb);  //deal sub base break signal
	int eres;
	struct event_base *ev_sub_base;
	struct event ev_gen_conn_timer;  //create term or srv timer
	struct event ev_sub_base_int_sig;
	struct timeval tv_gct = {0,0};
	ev_sub_base = event_base_new();
	int nIdx = 0;
	if (ev_sub_base == NULL)
	{
		vLogErr("Gen sub base error!!!");
		return FAILUER;
	}

	VPD vpd;
	memset(&vpd,0x00,sizeof(vpd));
	nIdx = _gstrpShm->pidInf.nUseCount;
	_gstrpPid[nIdx].base = ev_sub_base;
	vpd.nFlag = F_PHY_FLAG;
	_gstrpPid[nIdx].strpPhy = CreateNode(vpd);
	vpd.nFlag = F_LOGIC_FLAG;
	_gstrpPid[nIdx].strpLogic = CreateNode(vpd);
	vpd.nFlag = F_TERM_FLAG;
	_gstrpPid[nIdx].strpTerm = CreateNode(vpd);
	_gstrpShm->pidInf.nUseCount++;

	//create msg timer
	MsgTimerInit(ev_sub_base);

	//create gen conn timer
	eres = evtimer_assign(&ev_gen_conn_timer,ev_sub_base,GenConnTimerCb,&ev_gen_conn_timer);
	if (eres == -1)
	{
		event_base_free(ev_sub_base);
		vLogErr("GenConnTimer fail !!!");
		return FAILUER;
	}
	tv_gct.tv_usec = _gstrpShm->rc.nGenConnTimer;
	eres = evtimer_add(&ev_gen_conn_timer,&tv_gct);
	if (eres == -1)
	{
		event_base_free(ev_sub_base);
		vLogErr("GenConnTimer event add fail!!!");
		return FAILUER;
	}
	
	#if 0
	if(evsignal_assign(&ev_sub_base_int_sig,ev_sub_base,S_SUBBASEINT_SIG,
		SubBaseIntCb,(void *)ev_sub_base) == -1)
	{
		event_base_free(ev_sub_base);
		vLogErr("Allocating sub base int signal fail !!!");
		return FAILUER;
	}
	eres = evsignal_add(&ev_sub_base_int_sig,NULL);
	if (eres == -1)
	{
		event_base_free(ev_sub_base);
		vLogErr("Int sub base sig event add fail!!!");
		return FAILUER;
	}
	#endif

	eres = event_base_dispatch(ev_sub_base);
	if (eres == -1)
	{
		event_base_free(ev_sub_base);
		vLogErr("sub base dispthc fail!!!!");
		return FAILUER;
	}
	event_base_free(ev_sub_base);
	return SUCCESS;
}

/*====================================================================
 * 函数名    : vGenConn
 * 功能      : 产生一个终端或服务器的连接，即bufferevent事件
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明: 成功 返回创建的bufferevent
 *             失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
struct bufferevent *vGenConn(struct event_base *base)
{
	int evrs;
	struct bufferevent *bev;
	struct sockaddr_in server_addr;
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	if(bev == NULL)
	{
		vLogErr("bufferevent_socket_new error!!!");
		return NULL;
	}
	memset(&server_addr,0x00,sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_gstrpShm->rc.nSrvPort);
	server_addr.sin_addr.s_addr = inet_addr(_gstrpShm->rc.nSrvIP);
	evrs = bufferevent_socket_connect(bev,(struct sockaddr *)&server_addr,sizeof server_addr);
	if(evrs == -1)
	{
		vLogNM(EL_ERR,"conn.err","fd [%d] connect error with server!",bufferevent_getfd(bev));
		bufferevent_free(bev);
		return NULL;
	}

	bufferevent_setcb(bev,read_cb,write_cb,event_cb,(void *)base);
	bufferevent_enable(bev,EV_READ|EV_WRITE);
	return bev;
}

/*====================================================================
 * 函数名    : vGenFirstSubPid
 * 功能      : 产生第一个子进程，负责生成第一个event base
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明: 子进程    0
 *             父进程   1
 *             出错     -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
int vGenFirstSubPid()
{
	pid_t tFirstPid = fork();
	if (tFirstPid == 0)  //child
	{
		_gstrpShm->rc.tFirstPid = getpid();
		_gstrpPid[0].ppid = getppid();
		_gstrpPid[0].selfPid = getpid();
		return 0;
	}
	else if(tFirstPid > 0) //father
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
int vGenSubPid()
{
	pid_t tPid = fork();
	if (tPid == 0)  //child
	{
		_gstrpPid[_gstrpShm->pidInf.nUseCount].ppid = getppid();
		_gstrpPid[_gstrpShm->pidInf.nUseCount].selfPid = getpid();
		return 0;
	}
	else if(tPid > 0) //father
	{
		return 1;
	}
	else
	{
		return -1;
	}
}
/*====================================================================
 * 函数名    : GenMsgId
 * 功能      : 给每个消息编号，便于对消息的操作
 * 算法实现  : 
 * 参数说明  : vspMsg 要编号的消息
 * 返回值说明: 返回编好的号            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12      v1.0        YLI                          创建
 * ====================================================================*/
int GenMsgId(char *vspMsg)
{
	if (strcmp(vspMsg,"EV_REG_REQ") == 0) return EV_REG_REQ;
	if (strcmp(vspMsg,"EV_LOGOUT_REQ") == 0) return EV_LOGOUT_REQ;
	if (strcmp(vspMsg,"EV_HEARTBEAT_REQ") == 0) return EV_HEARTBEAT_REQ;
	if (strcmp(vspMsg,"EV_PFMINFO_MSG") == 0) return EV_PFMINFO_MSG;
	if (strcmp(vspMsg,"EV_PFMINFO_CPU") == 0) return EV_PFMINFO_CPU;
	if (strcmp(vspMsg,"EV_PFMINFO_MEM") == 0) return EV_PFMINFO_MEM;
	if (strcmp(vspMsg,"EV_PFMINFO_NETCARD") == 0) return EV_PFMINFO_NETCARD;
	if (strcmp(vspMsg,"EV_PFMINFO_DISK") == 0) return EV_PFMINFO_DISK;
	if (strcmp(vspMsg,"EV_DEV_ONLINE") == 0) return EV_DEV_ONLINE;
}
/*====================================================================
 * 函数名    : MsgTimerCBInit
 * 功能      : 为要测试的消息配置消息回调函数
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/7       v1.0        YLI                          创建
 * ====================================================================*/
void MsgTimerCBInit()
{
	int i = 0;
	for (i = 0; _gstrpMsg[i].nMsgType != 0; ++i)
	{
		switch(_gstrpMsg[i].nMsgType)
		{
			case EV_HEARTBEAT_REQ:
				_gstrpMsg[i].fn = EV_HEARTBEAT_REQ_CB;
				vLogTrace(EL_INFO,"msg [%d] cb is %x",_gstrpMsg[i].nMsgType,_gstrpMsg[i].fn);
				break;
			case EV_PFMINFO_MSG:				
				_gstrpMsg[i].fn = EV_PFMINFO_MSG_CB;
				vLogTrace(EL_INFO,"msg [%d] cb is %x",_gstrpMsg[i].nMsgType,_gstrpMsg[i].fn);
				break;
			case EV_PFMINFO_CPU:
				_gstrpMsg[i].fn = EV_PFMINFO_CPU_CB;
				vLogTrace(EL_INFO,"msg [%d] cb is %x",_gstrpMsg[i].nMsgType,_gstrpMsg[i].fn);
				break;
			case EV_DEV_ONLINE:
				_gstrpMsg[i].fn = EV_DEV_ONLINE_CB;
				vLogTrace(EL_INFO,"msg [%d] cb is %x",_gstrpMsg[i].nMsgType,_gstrpMsg[i].fn);
				break;
		}
	}
}
/*====================================================================
 * 函数名    : Regist
 * 功能      : 注册终端/服务器
 * 算法实现  : 
 * 参数说明  : vpd 终端/服务器注册的参数
 * 返回值说明: 成功  0
 *			   失败  -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/14       v1.0        YLI                          创建
 * ====================================================================*/
int Regist(VPD *vpd)
{
	json_t *reg;
	json_t *value;
	const char *key;

	if (vpd->nConn != F_CONN_OK || vpd->bev == NULL)
	{
		vLogErr("Connection failed,please reconnect!!!");
		return FAILUER;
	}
	if (vpd->nFlag == F_NULL_FLAG || vpd->nFlag == 0)
	{
		vLogErr("vpd flag set fail !!!");
		return FAILUER;
	}
	if (vpd->nPidIdx < 0)
	{
		vLogErr("pid idx don't set!!!");
		return FAILUER;
	}
	//gen reg msg
	if (vpd->nFlag == F_TERM_FLAG)
	{
		reg = vGenRegTermMsg(vpd->nPidIdx);
	}
	else
	{
		reg = vGenRegSrvMsg(vpd->nFlag,vpd->nPidIdx);
	}
	if (reg == NULL)
	{
		vLogErr("reg msg generate failed!!!");
		return FAILUER;
	}
	json_object_foreach(reg,key,value)
	{
		if (strcmp(key,"devid") == 0)
		{
			strcpy(vpd->saDevId,json_string_value(value));
		}
		if (strcmp(key,"devtype") == 0)
		{
			strcpy(vpd->saDevType,json_string_value(value));
		}
	}
	
	//pack reg msg and regist
	vpd->strpRegBag = nMsgPack(json_dumps(reg,0));
	if (vpd->strpRegBag == NULL)
	{
		json_decref(reg);
		vLogErr("Packaging registration message failure!!!");
		return FAILUER;
	}
	if (bufferevent_write(vpd->bev,
		vpd->strpRegBag->msg,
		vpd->strpRegBag->len) < 0)
	{
		json_decref(reg);
		vLogErr("Registration message send failure!!!");
		return FAILUER;
	}
	json_decref(value);
	vLogNM(EL_DEBUG,"reg.log","A term or srv suceessfull registration!!!\nreg msg [%s] msg len [%d] ",
			vpd->strpRegBag->msg + 4,vpd->strpRegBag->len);
	return SUCCESS;
} 
int UnRegist()
{
	int i = 3;
	PVPDLIST head[3] = {NULL,NULL,NULL};
	int nPidIdx = 0;
	MsgBagInf *pMsgBag = nMsgPack(J_LOGOUT_REQ);
	if(_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS)
	{
		vLogErr("_tGetPidIdx failure!!!");
		return FAILUER;
	}
	if (pMsgBag == NULL)
	{
		vLogErr("generate UnRegist msg failure!!");
		return FAILUER;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[2] = _gstrpPid[nPidIdx].strpTerm;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[1] = _gstrpPid[nPidIdx].strpPhy;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[0] = _gstrpPid[nPidIdx].strpLogic;
	}
	while(i)
	{
		if (head[i-1] == NULL)
		{
			i--;
			continue;
		}
		head[i-1] = head[i-1]->next;
		while(head[i-1])
		{
			if (head[i-1]->vpd.bev == NULL)
			{
				head[i-1] = head[i-1]->next;
				continue;
			}
			if ((head[i-1]->vpd.nConn == F_CONN_OK)
				&& (head[i-1]->vpd.nReg == F_REG_OK))
			{
				if (bufferevent_write(head[i-1]->vpd.bev,
					pMsgBag->msg,
					pMsgBag->len) < 0)
				{
					vLogErr("unRegistration message send failure!!!");
					return FAILUER;
				}
				vLogNM(EL_INFO,"unreg.log","devid [%s] UnRegist suceessfull!!!",
					head[i-1]->vpd.saDevId);
			}
			head[i-1] = head[i-1]->next;
		}
		i--;
	}
	return SUCCESS;
}
/*====================================================================
 * 函数名    : vGenMsg
 * 功能      : 生成普通发送的消息
 * 算法实现  : 
 * 参数说明  : nIdx  消息索引
 *				vpd  为哪个终端/服务器生成消息
 * 返回值说明:  成功 生成的消息
 *				失败 NULL           
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/10       v1.0        YLI                          创建
 * ====================================================================*/
MsgBagInf * vGenMsg(int nIdx,VPD vpd)
{
	MsgBagInf *pMsgBag = NULL;
	json_t *j_msg = NULL;
	int i = 0;

	//find msseage callback
	for (i = 0;_ev_msg_cb_map[i].eventid != NULL; ++i)
	{
		if (strcmp(_ev_msg_cb_map[i].eventid,_gstrpMsg[nIdx].saMsgType) == 0)
		{
			j_msg = (json_t *)(* _ev_msg_cb_map[i].msgcb)(vpd);
		}
	}
	if (j_msg != NULL)
	{
		pMsgBag = nMsgPack(json_dumps(j_msg,0));
	}

	if (pMsgBag != NULL)
	{
		return pMsgBag;
	}
	return NULL;
}



