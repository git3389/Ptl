/*====================================================================
 * 模块名  : ptlcm 
 * 文件名  : ptlcm.c
 * 相关文件:
 * 实现功能: 一些全局变量、接口函数的定义 
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/5    v1.0        YLI                          创建
 * ====================================================================*/
#include "ptlbase.h"
#include "common.h"


//global
int ptlerrno;
ShmDef *_gstrpShm;
MsgDef *_gstrpMsg;
PidDef *_gstrpPid;
char _gsaConfig[L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN + 1];
PVPDLIST _gstrpTermList;
PVPDLIST _gstrpPhyList;
PVPDLIST _gstrpLogicList;
//static
static int CheckIP(char *vspIp);
static int CheckPort(int port);
static int LoadMsgDef();
static int CheckMsgAndTimer(char *vspMsg,int *nTimer);
static int CheckMsgExist(char *vspMsg);
static void StopBase();
static void CleanMsg();

/*====================================================================
 * 函数名    : vSysInit
 * 功能      : 初始化环境
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明: 成功  0
 *             失败  -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/5      v1.0        YLI                          创建
 * ====================================================================*/
int vSysInit()
{
	if(vLogInit() != SUCCESS)
	{
		return FAILUER;
	}
	if (_tShmInit() != SUCCESS)
	{
		vLogErr("_tShmInit error!!!!");
		return FAILUER;
	}
	if (_tSemInit() != SUCCESS)
	{
		vLogErr("_tSemInit error!!!");
		return FAILUER;
	}
	if (_tSemP(_gstrpShm->rc.tSemBoot) != SUCCESS)
	{ 
		vLogErr("_tSemP fail!!!");
		return FAILUER;
	}
	if (nLoadParam() != SUCCESS)
	{			
		vLogErr("nLoadParam error!!!");
		return FAILUER;
	}
	if (nPidInit() != SUCCESS)
	{
		vLogErr("nPidInit error!!!");
		return FAILUER;
	}
	if (nMsgInit() != SUCCESS)
	{
		vLogErr("nMsgInit error!!!");
		return FAILUER;
	}
	if (_tSemV(_gstrpShm->rc.tSemBoot) != SUCCESS)
	{
		vLogErr("_tSemV fail!!!");
		return FAILUER;
	}
	return SUCCESS;
}
/*====================================================================
 * 函数名    : nLoadParam
 * 功能      : 解析配置文件中的项将资源加载到内存
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明: 成功  0
 *             失败  -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/5       v1.0        YLI                          创建
 * ====================================================================*/
int nLoadParam()
{
	ASSERT(_gstrpShm != NULL);
	ASSERT(_gstrpMsg != NULL);
	ASSERT(_gstrpPid != NULL);
	char tsaConfig[L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN +1];
	memset(tsaConfig,0x00,sizeof tsaConfig);
	strcpy(tsaConfig,_gstrpShm->rc.saTCN);
	//get ip	
	if(tPflGetString(tsaConfig,K_IP_KEY,_gstrpShm->rc.nSrvIP,
		sizeof(_gstrpShm->rc.nSrvIP)) != SUCCESS)
	{
		vLogErr("IP config error!!!");
		return FAILUER;
	}
	if(CheckIP(_gstrpShm->rc.nSrvIP) != SUCCESS)
	{
		vLogErr("IP 配置得不合法!!!");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"服务器的ip地址: [%s]",_gstrpShm->rc.nSrvIP);

	//get port
	if(tPflGetInt(tsaConfig,K_PORT_KEY,&_gstrpShm->rc.nSrvPort) != SUCCESS)
	{
		vLogErr("Listen port config error!!!!");
		return FAILUER;
	}
	if(CheckPort(_gstrpShm->rc.nSrvPort) != SUCCESS)
	{
		vLogErr("监听端口配置不合法!!!");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"服务器的监听端口: [%d] ",_gstrpShm->rc.nSrvPort);
	//get termnum
	if(tPflGetInt(tsaConfig,K_TERMNUM_KEY,&_gstrpShm->rc.nTermNum) != SUCCESS)
	{
		vLogErr("Termnum config error!!!!");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"虚拟终端数: [%d] ",_gstrpShm->rc.nTermNum);

	//get srv num
	if(tPflGetInt(tsaConfig,K_PHYNUM_KEY,&_gstrpShm->rc.nPhyNum) != SUCCESS)
	{
		vLogErr("Phynum config error,phy server at last one!!!");
		return FAILUER;
	}
	if (_gstrpShm->rc.nPhyNum == 0)
	{
		_gstrpShm->rc.nPhyNum = S_DEFAULT_PHY;
	}
	vLogTrace(EL_INFO,"虚拟物理服务器数: [%d] ",_gstrpShm->rc.nPhyNum);
	if(tPflGetInt(tsaConfig,K_LOGICNUM_KEY,&_gstrpShm->rc.nLogicNum) != SUCCESS)
	{
		vLogErr("Phynum config error!!!!");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"虚拟逻辑服务器数: [%d] ",_gstrpShm->rc.nLogicNum);

	int tTesTime = 0;
	//get TestTime
	if(tPflGetInt(tsaConfig,K_TESTTIME_KEY,&tTesTime) != SUCCESS)
	{
		if(tPflGetDouble(tsaConfig,K_TESTTIME_KEY,&_gstrpShm->rc.nTestTime) != SUCCESS)
		{
			vLogTrace(EL_WARNING,"TestTime config error,time is integer or double !!!!");
			_gstrpShm->rc.nTestTime = S_DEFAULT_TESTTIME; 
		}
	}
	if((_gstrpShm->rc.nTestTime > S_TESTTIME_MAX_SIZE)
		|| (tTesTime > S_TESTTIME_MAX_SIZE))
	{
		vLogErr("TestTime set too long!!!!");
		return FAILUER;
	}
	if (tTesTime != 0)
	{
		_gstrpShm->rc.nTestTime = tTesTime;
	}
	vLogTrace(EL_INFO,"测试时间: [%0.4fh] ",_gstrpShm->rc.nTestTime);
	_gstrpShm->rc.tFirstPid = -1;
	
	//get gen conn timer
	if(tPflGetInt(_gsaConfig,K_GENCONNTIMER_KEY,&_gstrpShm->rc.nGenConnTimer) != SUCCESS)
	{
		vLogErr("gen conn timer config error!!!!");
		_gstrpShm->rc.nGenConnTimer = S_GENCONN_TIME; 
	}
	vLogTrace(EL_INFO,"生成连接的间隔时间: [%ds] ",_gstrpShm->rc.nGenConnTimer);

	//load all msg define
	if (LoadMsgDef() != SUCCESS)
	{
		vLogErr("all Msg define load fail !!!");
		return FAILUER;
	}
	return SUCCESS;
}

/*====================================================================
 * 函数名    :  nPidInit
 * 功能      :  初始化每个base，为每个base中的服务器/终端创建链表
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:  成功   0
 *              失败   -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/6      v1.0        YLI                          创建
 * ====================================================================*/
int nPidInit()
{
	ASSERT(_gstrpShm != NULL);
	ASSERT(_gstrpMsg != NULL);
	ASSERT(_gstrpPid != NULL);

	if (_tSemP(_gstrpShm->rc.tSemPidId) != SUCCESS)
	{
		vLogErr("_tSemP fail!!!");
		return FAILUER;
	}	

	//init every base
	int i;
	for (i = 0; i < _gstrpShm->pidInf.nCount; ++i)
	{
		_gstrpPid[i].nPhyCount = 0;
		_gstrpPid[i].nLogicCount = 0;
		_gstrpPid[i].nTermCount = 0;
		_gstrpPid[i].nConnCount = 0;
		_gstrpPid[i].nIdx = i;
		_gstrpPid[i].ppid = -1;
		_gstrpPid[i].selfPid = -1;
		_gstrpPid[i].strpPhy = NULL;
		_gstrpPid[i].strpLogic = NULL;
		_gstrpPid[i].strpTerm = NULL;
	}
	if (_tSemV(_gstrpShm->rc.tSemPidId) != SUCCESS)
	{
		vLogErr("_tSemV fail!!!");
		return FAILUER;
	}	
	return SUCCESS;
}
/*====================================================================
 * 函数名    : nMsgInit
 * 功能      : 初始化要测试的消息，生成消息的实体
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:  成功    0
 *				失败    -1          
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *  2015/1/6      v1.0        YLI                          创建
 * ====================================================================*/
int nMsgInit()
{
	json_t *jMsgType;
	json_t *jt;
	char *saMsgBuf;
	int nTimer;
	char tsaConfig[L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN +1];
	int i;
	int nIdx = 0;

	//获得配置文件
	memset(tsaConfig,0x00,sizeof tsaConfig);
	strcpy(tsaConfig,_gstrpShm->rc.saTCN);
	//添加心跳保活消息
	HEARTBEAT
	nIdx++;
	//获得消息类型数组
	if ((jMsgType = tPflGetJsonObj(tsaConfig,K_MSGTYPE_KEY)) == NULL)
	{
		vLogErr("MsgType set fail!!!");
		return FAILUER;
	}
	if (json_is_array(jMsgType))
	{
		if (_tSemP(_gstrpShm->rc.tSemMsgId) != SUCCESS)
		{
			vLogErr("_tSemP fail!!!");
			return FAILUER;
		}	
		for (i = 0; i < (int)json_array_size(jMsgType); ++i)
		{
			saMsgBuf = NULL;
			nTimer = 0;
			jt = json_array_get(jMsgType,i);
			if(json_unpack(jt,"{s?s,s?i}",
				K_MSGID_KEY,&saMsgBuf,
				K_TIMER_KEY,&nTimer) == -1)
			{
				vLogErr("a msg laoded fail !!!");
			}
			else
			{
				if (CheckMsgAndTimer(saMsgBuf,&nTimer) == SUCCESS)
				{
					if (strcmp(saMsgBuf,"EV_HEARTBEAT_REQ") == 0)
					{
						_gstrpMsg[0].tv.tv_sec = nTimer;
						continue;
					}
					if(CheckMsgExist(saMsgBuf) == FAILUER)
					{
						continue;
					}
					MSGINIT(saMsgBuf,nIdx)
					nIdx++;
				}			
			}
		}
		_gstrpShm->msgInf.nCount = nIdx;
		vLogTrace(EL_INFO,"Test Msg conut is :[%d]",_gstrpShm->msgInf.nCount);
		vLogTrace(EL_INFO,"To Test msg add callback fun!!!");
		MsgTimerCBInit();
		if (_tSemV(_gstrpShm->rc.tSemMsgId) != SUCCESS)
		{
			vLogErr("_tSemV fail!!!");
			return FAILUER;
		}	
	}
	return SUCCESS;
}
/*====================================================================
 * 函数名    : vSysDone
 * 功能      :  销毁所有的链表，移除信号量和共享内存
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:  成功   0
 *              失败   -1           
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/6      v1.0        YLI                          创建
 * ====================================================================*/
int vSysDone()
{
	ASSERT(_gstrpShm != NULL);
	//pid
	StopBase();
	//remove sem
	if(_tSemRm() != SUCCESS)
	{
		vLogErr("Remove sem error !!!");
	}
	//remove ipc
	if (_tShmRm() != SUCCESS)
	{
		vLogErr("Remove ipc error!!!");
		return FAILUER;
	}
}
/*====================================================================
 * 函数名    : vSubPidRelease
 * 功能      : 子进程的一些资源的释放，如链表、事件、信号
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/15       v1.0        YLI                          创建
 * ====================================================================*/
int vSubPidRelease()
{
	int nPidIdx = 0;
	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS)
	{ 
		vLogErr("Pid [%d]'s idex is not exist !!!",getpid());
		return FAILUER;
	}
	vLogTrace(EL_INFO,"index [%d]:release pid [%d]'s srv/term list...",nPidIdx,getpid());
	//_tPrintList(nPidIdx);
	if (_gstrpPid[nPidIdx].strpPhy != NULL)
	{
		DestroyList(&(_gstrpPid[nPidIdx].strpPhy));
	}
	if (_gstrpPid[nPidIdx].strpLogic != NULL)
	{
		DestroyList(&(_gstrpPid[nPidIdx].strpLogic));
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		DestroyList(&(_gstrpPid[nPidIdx].strpTerm));
	}
	vLogTrace(EL_INFO,"release pid [%d]'s srv/term list successful!!!",getpid());	
	return SUCCESS;
}
/*====================================================================
 * 函数名    : StopBase
 * 功能      : 停止所有的base事件
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12      v1.0        YLI                          创建
 * ====================================================================*/
static void StopBase()
{
	int i = 0;
	for (i = 0; i < _gstrpShm->pidInf.nUseCount; ++i)
	{
		if(0 == waitpid(_gstrpPid[i].selfPid,NULL,WNOHANG))
		{
			if(kill(_gstrpPid[i].selfPid,S_SUBBASEINT_SIG) == 0)
			{
				if(waitpid(_gstrpPid[i].selfPid,NULL,0) == -1)
				{
					vLogErr("pid [%u] exit error [%d]!!!",_gstrpPid[i].selfPid,errno);
					vLogNM(EL_INFO,"list.log","StopBase!!!");
				}
				vLogTrace(EL_INFO,"pid [%u]'s exit!!!",_gstrpPid[i].selfPid);
			}
		}
		else
		{
			vLogTrace(EL_INFO,"pid [%u]'s already break loop and dead!!!",_gstrpPid[i].selfPid);
		}
	}

}
/*====================================================================
 * 函数名    : CleanMsg
 * 功能      : 清理打包好的消息
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
static void CleanMsg()
{
	int i = 0;
	for (i = 0; i < _gstrpShm->msgInf.nCount; ++i)
	{
		if (_gstrpMsg[i].saMsgType == NULL )
		{
			continue;
		}
		vLogTrace(EL_INFO,"Clean msg [%s] pack!!!",_gstrpMsg[i].saMsgType);
		//nMsgPackFree(_gstrpMsg[i].strpMsgBagInf);
	}
}
/*====================================================================
 * 函数名    : CheckIP 
 * 功能      : 检查配置的IP是否合法 
 * 算法实现  : 
 * 参数说明  :  vspIp   要检查的IP字符串
 * 返回值说明:  成功     0
 *              失败     -1       
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/5      v1.0        YLI                          创建
 * ====================================================================*/
static int CheckIP(char *vspIp)
{
	int nConutDot = 0;         //record dot conut
	int i = 0;
	int j = 0;
	char c = '0';
	char buf[20];

	if(vspIp == NULL)
	{
		return FAILUER;
	}

	memset(buf,0x00,sizeof buf);
	Trim(vspIp);
	for(i = 0;i < (int)strlen(vspIp); i++)
	{
		c = *(vspIp + i);
		if(c == '.')
		{
			nConutDot ++;
			if(atoi(buf) > 255 || atoi(buf) < 0)
			{
				vLogErr("IP地址不合法!");
				return FAILUER;
			}
			memset(buf,0x00,sizeof buf);
			j = 0;
			continue;
		}
		if((c >= '0') && (c <= '9'))
		{
			buf[j++] = c;	
		}	
		else
		{
			vLogErr("IP地址中含有非数字的字符!!!");
			return FAILUER;
		}	
	}
	if(atoi(buf) > 255 || atoi(buf) < 0)
	{
		vLogErr("IP地址不合法!");
		return FAILUER;
	}

	if(nConutDot >= 4 )
	{
		vLogErr("IP 地址中的点多了!!!");
		return FAILUER;
	}
	return SUCCESS;
}
/*====================================================================
 * 函数名    : CheckPort 
 * 功能      : 检查端口是否合法
 * 算法实现  : 
 * 参数说明  : port   要检查的端口 
 * 返回值说明:  成功    0
 *              失败   -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/1/5       v1.0        YLI                          创建
 * ====================================================================*/
static int CheckPort(int port)
{
	if((port < 0) || (port > 65536))
	{
		vLogErr("端口数字大太!!!");
		return FAILUER;
	}
	return SUCCESS;
}
/*====================================================================
 * 函数名    : CheckMsgAndTimer
 * 功能      : 检查该消息是否被定义过，消息的定时时间是否合法
 * 算法实现  : 
 * 参数说明  : vspMsg   检查的消息
 *             nTimer   检查的时间，不合法则使用消息的默认的时间
 * 返回值说明:  成功  0
 *              失败  -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
static int CheckMsgAndTimer(char *vspMsg,int *nTimer)
{
	int i;
	for (i = 0; _gstrpShm->rc.strTagMsg[i].MsgId != 0; ++i)
	{
		Trim(vspMsg);
		if (strcmp(vspMsg,_gstrpShm->rc.strTagMsg[i].MsgType) == 0)
		{
			if (*nTimer == 0)
			{
				*nTimer = _gstrpShm->rc.strTagMsg[i].Timer;
			}
			else if ((*nTimer < 0) || (*nTimer > S_TIMER_MAX_SIZE) ||
					(*nTimer > CovertHourToSec(_gstrpShm->rc.nTestTime)))
			{
				vLogTrace(EL_WARNING,"Msg [%s] timer [%d] set illegality will use default timer",vspMsg,*nTimer);
				*nTimer = _gstrpShm->rc.strTagMsg[i].Timer;
			}
			return SUCCESS;
		}
	}
	vLogErr("The Msg [%s] is not defined by the configuration!!!",vspMsg);
	return FAILUER;
}
/*====================================================================
 * 函数名    : CheckMsgExist
 * 功能      : 检测测试消息是否有重复
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明: 不重复  0
 *             重复    -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
static int CheckMsgExist(char *vspMsg)
{
	int i = 0;
	for (i = 0; _gstrpMsg[i].nMsgType != 0; ++i)
	{
		if (strcmp(vspMsg,_gstrpMsg[i].saMsgType) == 0)
		{
			vLogErr("[%s] msg already loading",vspMsg);
			return FAILUER;
		}
	}
	return SUCCESS;
}
/*====================================================================
 * 函数名    : LoadMsgDef
 * 功能      :  加载所有定义好的消息以消息的发送时间到内存
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:  成功    0
 *              失败    -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/6       v1.0        YLI                          创建
 * ====================================================================*/
static int LoadMsgDef()
{
	char saMsgTypeFN[L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN +1];
	json_t *jMsgRoot;
	json_t *jt;
	int idx = 0;                  //msg id
 	char *saMsgBuf = NULL;
	int nTimer = 0;
	int i = 0;
	memset(saMsgTypeFN,0x00,sizeof saMsgTypeFN);
	memset(&_gstrpShm->rc.strTagMsg,0x00,sizeof(struct tagMsg));

	if(tPflGetString(_gsaConfig,K_MTFN_KEY,saMsgTypeFN,sizeof saMsgTypeFN) != SUCCESS)
	{
		vLogErr("msg type path name don't set !!!!");
		return FAILUER;
	}

	//common msg
	jMsgRoot = tPflGetJsonObj(saMsgTypeFN,K_COMMSG_KEY);
	if (jMsgRoot != NULL)
	{
		if (json_is_array(jMsgRoot))
		{
			for (i = 0; i < (int)json_array_size(jMsgRoot); ++i)
			{
				jt = json_array_get(jMsgRoot,i);
				if(json_unpack(jt,"{s?s,s?i}",
					K_MSGID_KEY,&saMsgBuf,
					K_TIMER_KEY,&nTimer) == -1)
				{
					vLogErr("a common msg laoded fail !!!");
					saMsgBuf = NULL;
					nTimer = 0;
				}
				else
				{
					_gstrpShm->rc.strTagMsg[idx].MsgId = GenMsgId(saMsgBuf);
					strcpy(_gstrpShm->rc.strTagMsg[idx].MsgType,saMsgBuf);
					_gstrpShm->rc.strTagMsg[idx].Timer = nTimer;
					_gstrpShm->rc.strTagMsg[idx].nFlag = F_COMMSG_FLAG;
					saMsgBuf = NULL;
					nTimer = 0;
					idx++;
				}
			}
		}
		else
		{
			vLogErr("Common msg set fail !!!");
		}
	}
	else
	{
		vLogErr("Common msg don't set !!!!");
	}
	json_decref(jMsgRoot);
	json_decref(jt);
	//term msg
	jMsgRoot = tPflGetJsonObj(saMsgTypeFN,K_TERMMSG_KEY);
	if (jMsgRoot != NULL)
	{
		if (json_is_array(jMsgRoot))
		{
			for (i = 0; i < (int)json_array_size(jMsgRoot); ++i)
			{
				jt = json_array_get(jMsgRoot,i);
				if(json_unpack(jt,"{s?s,s?i}",
					K_MSGID_KEY,&saMsgBuf,
					K_TIMER_KEY,&nTimer) == -1)
				{
					vLogErr("a common msg laoded fail !!!");
					saMsgBuf = NULL;
					nTimer = 0;
				}
				else
				{
					_gstrpShm->rc.strTagMsg[idx].MsgId = GenMsgId(saMsgBuf);
					strcpy(_gstrpShm->rc.strTagMsg[idx].MsgType,saMsgBuf);
					_gstrpShm->rc.strTagMsg[idx].Timer = nTimer;
					_gstrpShm->rc.strTagMsg[idx].nFlag = F_TERMMSG_FLAG;
					saMsgBuf = NULL;
					nTimer = 0;
					idx++;
				}
			}
		}
		else
		{
			vLogErr("Term msg set fail !!!");
		}
	}
	else
	{
		vLogErr("Term msg don't set !!!!");
	}
	json_decref(jMsgRoot);
	json_decref(jt);
	//phy msg
	jMsgRoot = tPflGetJsonObj(saMsgTypeFN,K_PHYMSG_KEY);
	if (jMsgRoot != NULL)
	{
		if (json_is_array(jMsgRoot))
		{
			for (i = 0; i < (int)json_array_size(jMsgRoot); ++i)
			{
				jt = json_array_get(jMsgRoot,i);
				if(json_unpack(jt,"{s?s,s?i}",
					K_MSGID_KEY,&saMsgBuf,
					K_TIMER_KEY,&nTimer) == -1)
				{
					vLogErr("a common msg laoded fail !!!");
					saMsgBuf = NULL;
					nTimer = 0;
				}
				else
				{
					_gstrpShm->rc.strTagMsg[idx].MsgId = GenMsgId(saMsgBuf);
					strcpy(_gstrpShm->rc.strTagMsg[idx].MsgType,saMsgBuf);
					_gstrpShm->rc.strTagMsg[idx].Timer = nTimer;
					_gstrpShm->rc.strTagMsg[idx].nFlag = F_PHYMSG_FLAG;
					saMsgBuf = NULL;
					nTimer = 0;
					idx++;
				}
			}
		}
		else
		{
			vLogErr("PHY msg set fail !!!");
		}
	}
	else
	{
		vLogErr("PHY msg don't set !!!!");
	}
	json_decref(jMsgRoot);
	json_decref(jt);
	//Logic msg
	jMsgRoot = tPflGetJsonObj(saMsgTypeFN,K_LOGICMSG_KEY);
	if (jMsgRoot != NULL)
	{
		if (json_is_array(jMsgRoot))
		{
			for (i = 0; i < (int)json_array_size(jMsgRoot); ++i)
			{
				jt = json_array_get(jMsgRoot,i);
				if(json_unpack(jt,"{s?s,s?i}",
					K_MSGID_KEY,&saMsgBuf,
					K_TIMER_KEY,&nTimer) == -1)
				{
					vLogErr("a common msg laoded fail !!!");
					saMsgBuf = NULL;
					nTimer = 0;
				}
				else
				{
					_gstrpShm->rc.strTagMsg[idx].MsgId = GenMsgId(saMsgBuf);
					strcpy(_gstrpShm->rc.strTagMsg[idx].MsgType,saMsgBuf);
					_gstrpShm->rc.strTagMsg[idx].Timer = nTimer;
					_gstrpShm->rc.strTagMsg[idx].nFlag = F_LOGICMSG_FLAG;
					saMsgBuf = NULL;
					nTimer = 0;
					idx++;
				}
			}
		}
		else
		{
			vLogErr("Logic msg set fail !!!");
		}
	}
	else
	{
		vLogErr("Logic msg don't set !!!!");
	}
	json_decref(jMsgRoot);
	json_decref(jt);
	return SUCCESS;
}


