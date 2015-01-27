/*====================================================================
 * 模块名  : ptlstatic
 * 文件名  : ptlstatic.h
 * 相关文件:
 * 实现功能: 定义一些常量、静态数据
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26  v1.0        YLI                          创建
 * ====================================================================*/
#ifndef _PTL_STATIC_H
#define _PTL_STATIC_H


//const data
//注册失败的回应码的字符串对应
static struct {int code; const char *msg; } _reg_code_errors[] = {
	{1,"Login ID error"},
	{2,"Repeat login error by terminal ID"},
	{3,"Login device type error"},
	{-1,NULL},
};
//消息对应的生成函数的映射
static struct {const char *eventid; json_t * (*msgcb) (VPD); } _ev_msg_cb_map[] = {
	//term
	{"EV_PFMINFO_MSG",vGenPFMINFOMsg},
	{"EV_SHOULD_CONNSRV_MSG",vGenSHOULDCONNSRVMsg},
	{"EV_CONNSRV_DESC_MSG",vGenCONNSRVDESCMsg},
	{"EV_CONNSRV_CONN_MSG",vGenCONNSRVCONNMsg},
	{"EV_ALARM_MSG",vGenALARMMsg},
	{"EV_CONF_INFO",vGenCONFINFOMsg},
	{"EV_VERSION_MSG",vGenVERSIONMsg},
	{"EV_EXCEPTION_FILE",vGenEXCEPTIONFILEMsg},
	{"EV_BANDWIDTH_MSG",vGenBANDWIDTHMsg},
	{"EV_NETINFO_MSG",vGenNETINFOMsg},
	//srv
	{"EV_PFMINFO_CPU",vGenPFMINFOCPUMsg},
	{"EV_PFMINFO_MEM",vGenPFMINFOMEMMsg},
	{"EV_PFMINFO_NETCARD",vGenPFMINFONETCARDMsg},
	{"EV_PFMINFO_DISK",vGenPFMINFODISKMsg},
	{"EV_SYSTIME_SYNC",vGenSYSTIMESYNCMsg},
	{"EV_COLLECTOR_HEARTBEAT",vGenCOLLECTORHEARTBEATMsg},
	{"EV_DEV_ONLINE",vGenDEVONLINEMsg},
	{"EV_DEV_OFFLINE",vGenDEVOFFLINEMsg},
	{"EV_PAS_INFO",vGenPASINFOMsg},
	{"EV_PAS_P2PCONF_CREATE",vGenPASP2PCONFCREATEMsg},
	{"EV_PAS_P2PCONF_DESTROY",vGenPASP2PCONFDESTROYMsg},
	{"EV_MPCD_INFO",vGenMPCDINFOMsg},
	{"EV_MCU_INFO",vGenMCUINFOMsg},
	{"EV_MCU_CONF_CREATE",vGenMCUCONFCREATEMsg},
	{"EV_MCU_CONF_DESTROY",vGenMCUCONFDESTROYMsg},
	{"EV_MCU_MT_ADD",vGenMCUMTADDMsg},
	{"EV_MCU_MT_DEL",vGenMCUMTDELMsg},
	{"EV_MP_INFO",vGenMPINFOMsg},
	{"EV_PRS_INFO",vGenPRSINFOMsg},
	{"EV_SUS_INFO",vGenSUSINFOMsg},
	{"EV_SUSMGR_INFO",vGenSUSMGRINFOMsg},
	{"EV_APS_INFO",vGenAPSINFOMsg},
	{"EV_LGS_INFO",vGenLGSINFOMsg},
	{"EV_NTS_INFO",vGenNTSINFOMsg},
	{"EV_XMPP_INFO",vGenXMPPINFOMsg},
	{NULL,NULL},
};
//告警码
static int alarm_code[] = {1003,1004,1006,1007,1008,1017,1010,1011,1016,1018,0};
//设备状态定义
static int mt_state[] = {1,2,3,0};




#endif /*_PTL_STATIC_H*/