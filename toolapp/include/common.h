#ifndef _COMMON_H
#define _COMMON_H

#include "ptl.h"
#include "ptlstatic.h"

#define DEBUGONLY(f) (f)
#define ASSERT(f) \
	DEBUGONLY(((f) || \
				_vAssertFail(__FILE__,__LINE__,__func__) || (_vDebugBreak(),0)))

#undef EPSILON
#define EPSILON 0.00001
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))

#define HEARTBEAT \
	_gstrpMsg[0].nMsgType = EV_HEARTBEAT_REQ;\
	_gstrpMsg[nIdx].nFlag = F_COMMSG_FLAG;\
	strcpy(_gstrpMsg[0].saMsgType,"EV_HEARTBEAT_REQ");\
	_gstrpMsg[0].tv.tv_sec = S_DEFAULT_HEARTBEAT;\
	_gstrpMsg[0].tv.tv_usec = 0;\
	_gstrpMsg[0].fn = NULL; \
	_gstrpMsg[0].base = NULL; 

#define MSGINIT(mt,nIdx) \
	int i;\
	for (i = 0; _gstrpShm->rc.strTagMsg[i].MsgId != 0; ++i) \
	{\
		if (strcmp(mt,_gstrpShm->rc.strTagMsg[i].MsgType) == 0)\
		{\
			_gstrpMsg[nIdx].nMsgType = _gstrpShm->rc.strTagMsg[i].MsgId;\
			_gstrpMsg[nIdx].nFlag = _gstrpShm->rc.strTagMsg[i].nFlag;\
			strcpy(_gstrpMsg[nIdx].saMsgType,_gstrpShm->rc.strTagMsg[i].MsgType);\
			_gstrpMsg[nIdx].tv.tv_sec = nTimer;\
			_gstrpMsg[nIdx].tv.tv_usec = 0;\
			_gstrpMsg[nIdx].fn = NULL; \
			_gstrpMsg[nIdx].base = NULL; \
		}\
	}


#define VPD_IS_MAX(idx) \
	((_gstrpPid[idx].nTermCount + \
	_gstrpPid[idx].nPhyCount + \
	_gstrpPid[idx].nLogicCount) > _gstrpShm->rc.nBaseMaxNum)

#define LIST_SAVE_LOCK() \
	_tSemP(_gstrpShm->rc.tSemPidId);

#define LIST_SAVE_UNLOCK() \
	_tSemV(_gstrpShm->rc.tSemPidId);
   

//ptlipc.c
extern int _tSemP(int vtSemId);
extern int _tSemV(int vtSemId);
extern int _tShmInit();
extern int _tShmRm();
extern int _tShmAtach();
extern int _tShmDetach();
extern int _tSemInit();
extern int _tSemRm();

//ptllist.c 
extern PVPDLIST CreateNode(VPD vpd); 
extern int InsertNode(VPD vpd);
extern int DelNode(VPD vpd);
extern PVPDLIST GetNode(VPD vpd,int nPre);
extern void DestroyList();



//ptlcm.c
extern int nLoadParam();
extern int nPidInit();
extern int nMsgInit();
extern int GenMsgId(char *vspMsg);
extern void MsgTimerCBInit();

#endif /*_COMMON_H*/
