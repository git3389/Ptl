/*====================================================================
 * 模块名  : callback
 * 文件名  : ptlmsgtimercb.h
 * 相关文件:
 * 实现功能:  定义所有消息的回调函数 
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/9      v1.0        YLI                          创建
 * ====================================================================*/
#ifndef _PTLMSGTIMERCB_H
#define _PTLMSGTIMERCB_H

#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"
#include "event2/bufferevent.h"
#include "ptl.h"

/*
#define DE_FUN_CB(fn) \
	extern void fn(evutil_socket_t fd, short events, void *arg)

#define BEGIN_FN_DE

	DE_FUN_CB(EV_HEARTBEAT_REQ_CB);
	DE_FUN_CB(EV_PFMINFO_MSG_CB);

#define END_FN_DE



#define BEGIN_FN_CB (fn)
	DE_FUN_CB(fn)\
	{

#define END_FN_CB \
	}

#define CB_FN_DEF(fn) \
	BEGIN_FN_CB(fn) \
	FN_EXEC \
	END_FN_CB \
*/
#define FN_EXEC \
	struct timearg *msg_arg = (struct timearg *)arg;\
	int nIdx = 0;\
	if (_tGetMsgIdx(msg_arg->nMsgType\
		,&nIdx) != SUCCESS)\
	{\
		vLogErr("_tGetMsgIdx fail!!!");\
		return;\
	}\
	SenMsg(nIdx);\
	vLogTrace(EL_INFO,"[%d]'s msg [%d]sended!!!",msg_arg->nFlag,msg_arg->nMsgType);\
	evtimer_add(msg_arg->timer,&_gstrpMsg[nIdx].tv);

void MsgTimer00300_cb(evutil_socket_t fd, short events, void *arg);
void MsgTimer00900_cb(evutil_socket_t fd, short events, void *arg);
void MsgTimer001500_cb(evutil_socket_t fd, short events, void *arg);
void MsgTimer001600_cb(evutil_socket_t fd, short events, void *arg);
void MsgTimer001700_cb(evutil_socket_t fd, short events, void *arg);
void MsgTimer002100_cb(evutil_socket_t fd, short events, void *arg);

#endif /*_PTLMSGTIMERCB_H*/

