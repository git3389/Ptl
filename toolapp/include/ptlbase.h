/*====================================================================
 * 模块名  :
 * 文件名  : ptlbase.h
 * 相关文件:
 * 实现功能: 定义一些结构、接口
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/19    v1.0          YLI
 * ====================================================================*/
#ifndef _PTLBASE_H
#define _PTLBASE_H

//include system header files
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//include custom header files
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"
#include "event2/bufferevent.h"
#include "ptlconst.h"
#include "ptlpack.h"

typedef void (* evtimer_cb) (evutil_socket_t, short, void *);

typedef struct 
{
	char saDevPhyId[L_DEVID_LEN+1];   //belonge phy id
}DevInf;

//struct defintion
typedef struct 
{
	char saDevId[L_DEVID_LEN+1];      //devid
	char saDevType[L_DEVTYPE_LEN+1];  //devtype
	DevInf devInf;                    //extend info
	evutil_socket_t fd;               //socket
	int  nHeartNum;                   
	int  nHeartFlag;                   //whether reiceve heart bag
	int  nFlag;                        //flag phy or termianl
	int  nConn;						  //connect staus
	int  nReg;                         //reg staus
	int  nPidIdx;
	int nSndHeartNum;
	MsgBagInf *strpRegBag;
	struct bufferevent *bev;
}VPD;

//vpd link list
typedef struct vpdlist
{
	VPD vpd;
	struct vpdlist *next;
}VPDLIST,*PVPDLIST;

struct timearg
{
	int nMsgType;                     //msg type
	int nFlag;                       //srv or term msg
	struct event *timer;
};

typedef struct
{
	char nSrvIP[15+1];  //connect nms ip
	int nSrvPort;       //nms listen port
	int nPhyNum;        //phy number
	int nLogicNum;      //logic number
	int nTermNum;       //terminal number
	double nTestTime;      //test time
	int nGenConnTimer;
	struct tagMsg strTagMsg[S_MSGTYPE_SIZE];              //all define's msg
	char saTCN[L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN + 1]; //test config name
	pid_t tFirstPid;
	int nBaseMaxNum;   //base max num
	int nBaseCount;    //create base count

	/*sem*/
	int tSemMsgId;
	int tSemPidId;
	int tSemBoot;

}RcDef;

typedef struct
{
	int nMsgType;         		//msg type
	int nFlag;                  //flag srv or termianl msg
	char saMsgType[L_DEVTYPE_LEN];
	//create timer or other event with callback
	union {
		evtimer_cb fn;
	};
	struct timeval tv;        //if timer event the value is timer time,otherwise is 0
	struct timearg arg;
	struct event timer;
	struct event_base *base;
}MsgDef;

typedef struct 
{
	int nPhyCount;           
	int nLogicCount;
	int nTermCount;
	int nConnCount;             //connect sucess count
	int nIdx;
	pid_t selfPid;
	pid_t ppid;
	struct event_base *base;
	VPDLIST *strpPhy;          //pyh srv link list
	VPDLIST *strpLogic;        //logic srv link list
	VPDLIST *strpTerm;        //term link list
}PidDef;

typedef struct
{
	int nCount;
	int nUseCount;
	int nShmId;
}MsgInfDef;

typedef struct 
{
	int nCount;
	int nUseCount;
	int nShmId;
}PidInfDef;

typedef struct
{
	RcDef rc;
	MsgInfDef msgInf;
	PidInfDef pidInf;
}ShmDef;

extern ShmDef *_gstrpShm;
extern MsgDef *_gstrpMsg;
extern PidDef *_gstrpPid;
extern char _gsaConfig[L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN + 1];
extern PVPDLIST _gstrpTermList;
extern PVPDLIST _gstrpPhyList;
extern PVPDLIST _gstrpLogicList;
//typedef

//funtions

#endif /*_PTLBASE_H*/
