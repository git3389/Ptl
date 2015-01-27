/*====================================================================
 * 模块名  : ptl
 * 文件名  : ptl.h
 * 相关文件:
 * 实现功能: 提供所有可以外部使用的接口函数
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *    v1.0        YLI                          创建
 * ====================================================================*/

#ifndef __PTL__H
#define __PTL__H

#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"
#include "event2/bufferevent.h"
#include "ptlconst.h"
#include "ptlutil.h"
#include "userlog.h"
#include "ptlbase.h"


//log
#define vLogErr(...) \
	_vWriteLog(__FILE__,__LINE__,__func__,EL_ERR,"err.log",##__VA_ARGS__)

#define vLogTrace(vLevel,...) \
	_vWriteLog(__FILE__,__LINE__,__func__,vLevel,"tarce.log",##__VA_ARGS__)

#define vLogNM(vLevel,nm,...) \
	_vWriteLog(__FILE__,__LINE__,__func__,vLevel,nm,##__VA_ARGS__)

#define vLogNMS(vLevel,nm,...) \
	_vWriteLog(NULL,0,__func__,vLevel,nm,##__VA_ARGS__)

extern int ptlerrno;
//ptlget.c
extern int _tGetMsgId(char *vspMsg,int *nId);
extern int _tGetTestTime(long *nTimer);
extern int _tGetPidIdx(const pid_t pid,int *nIdx);
extern ShmDef *_tGetShmAddr();
extern PidDef *_tGetPidAddr();
extern int _tGetGenTermOrSrvFlag();
extern int _tGetMsgIdx(int nMsgId,int *nIdx);
extern json_t *GetCpuInfo();
extern json_t *GetMemInfo();
void _tPrintList(int nIdx);
extern const char *_get_code_error_to_string(int errcode);


//ptlput.c
extern int _tPutConnCountInc(int nIdx,int nFlag);
extern int _tPutConnCountDec(int nIdx,int nFlag);
extern int _tPutConnCount(int nIdx,int nCount);
extern int _tPutConnToList(int nIdx,VPD vpd);

//ptlcm.c
extern int vSysInit();
extern int vSysDone();
extern int vSubPidRelease();

//ptlgen.c
extern int vGenFirstSubPid();
extern int vGenSubPid();
extern int vGenBase();
extern struct bufferevent * vGenConn(struct event_base *base);
extern MsgBagInf * vGenMsg(int nIdx,VPD vpd);
extern int Regist(VPD *vpd);\
int UnRegist();

//ptlgenmsg.c
//term
extern json_t *vGenRegTermMsg(int nIdx);			/*EV_REG_REQ*/
extern json_t * vGenPFMINFOMsg(VPD vpd);			/*EV_PFMINFO_MSG*/
extern json_t * vGenSHOULDCONNSRVMsg(VPD vpd);		/*EV_SHOULD_CONNSRV_MSG*/
extern json_t * vGenCONNSRVDESCMsg(VPD vpd);		/*EV_CONNSRV_DESC_MSG*/
extern json_t * vGenCONNSRVCONNMsg(VPD vpd);		/*EV_CONNSRV_CONN_MSG*/
extern json_t * vGenALARMMsg(VPD vpd);				/*EV_ALARM_MSG*/
extern json_t * vGenCONFINFOMsg(VPD vpd);			/*EV_CONF_INFO*/
extern json_t * vGenVERSIONMsg(VPD vpd);			/*EV_VERSION_MSG*/
extern json_t * vGenEXCEPTIONFILEMsg(VPD vpd);		/*EV_EXCEPTION_FILE*/
extern json_t * vGenBANDWIDTHMsg(VPD vpd);			/*EV_BANDWIDTH_MSG*/
extern json_t * vGenNETINFOMsg(VPD vpd);			/*EV_NETINFO_MSG*/

//srv
extern json_t *vGenRegSrvMsg(int nFlag,int nIdx);   /*EV_REG_REQ*/
extern json_t * vGenPFMINFOCPUMsg(VPD vpd);			/*EV_PFMINFO_CPU*/
extern json_t * vGenPFMINFOMEMMsg(VPD vpd);			/*EV_PFMINFO_MEM*/
extern json_t * vGenPFMINFONETCARDMsg(VPD vpd);		/*EV_PFMINFO_NETCARD*/
extern json_t * vGenPFMINFODISKMsg(VPD vpd);		/*EV_PFMINFO_DISK*/
extern json_t * vGenSYSTIMESYNCMsg(VPD vpd);		/*EV_SYSTIME_SYNC*/
extern json_t * vGenCOLLECTORHEARTBEATMsg(VPD vpd); /*EV_COLLECTOR_HEARTBEAT*/
extern json_t * vGenDEVONLINEMsg(VPD vpd);			/*EV_DEV_ONLINE*/
extern json_t * vGenDEVOFFLINEMsg(VPD vpd);			/*EV_DEV_OFFLINE*/
extern json_t * vGenPASINFOMsg(VPD vpd);			/*EV_PAS_INFO*/
extern json_t * vGenPASP2PCONFCREATEMsg(VPD vpd);	/*EV_PAS_P2PCONF_CREATE*/
extern json_t * vGenPASP2PCONFDESTROYMsg(VPD vpd);	/*EV_PAS_P2PCONF_DESTROY*/
extern json_t * vGenMPCDINFOMsg(VPD vpd);			/*EV_MPCD_INFO*/
extern json_t * vGenMCUINFOMsg(VPD vpd);			/*EV_MCU_INFO*/
extern json_t * vGenMCUCONFCREATEMsg(VPD vpd);		/*EV_MCU_CONF_CREATE*/
extern json_t * vGenMCUCONFDESTROYMsg(VPD vpd);		/*EV_MCU_CONF_DESTROY*/
extern json_t * vGenMCUMTADDMsg(VPD vpd);			/*EV_MCU_MT_ADD*/
extern json_t * vGenMCUMTDELMsg(VPD vpd);			/*EV_MCU_MT_DEL*/
extern json_t * vGenMPINFOMsg(VPD vpd);				/*EV_MP_INFO*/
extern json_t * vGenPRSINFOMsg(VPD vpd);			/*EV_PRS_INFO*/
extern json_t * vGenSUSINFOMsg(VPD vpd);			/*EV_SUS_INFO*/
extern json_t * vGenSUSMGRINFOMsg(VPD vpd);			/*EV_SUSMGR_INFO*/
extern json_t * vGenAPSINFOMsg(VPD vpd);			/*EV_APS_INFO*/
extern json_t * vGenLGSINFOMsg(VPD vpd);			/*EV_LGS_INFO*/
extern json_t * vGenNTSINFOMsg(VPD vpd);			/*EV_NTS_INFO*/
extern json_t * vGenXMPPINFOMsg(VPD vpd);			/*EV_XMPP_INFO*/


#endif  /*__PTL__H*/

