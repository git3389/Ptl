/*====================================================================
 * 模块名  : ptltypes
 * 文件名  : ptltypes.h
 * 相关文件:
 * 实现功能: 终端、服务器相关的信息类型、设备类型等的宏定义
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/29     v1.0        YLI                           创建
 * ====================================================================*/

#ifndef _PTLTYPES_H
#define _PTLTYPES_H

#define L_DEVID_LEN    30
#define L_DEVTYPE_LEN  50
#define S_MSGTYPE_SIZE  40 
 
//msg flag
#define F_PHYMSG_FLAG     300
#define F_LOGICMSG_FLAG     301
#define F_TERMMSG_FLAG    302
#define F_COMMSG_FLAG  	  303
#define F_NULL_FLAG    -1

//message types id
//terminal
#define EV_REG_REQ  		1    //注册请求
#define EV_LOGOUT_REQ   	2    //注销请求
#define EV_HEARTBEAT_REQ 	3    //保活
#define EV_SHOULD_CONNSRV_MSG 4  //上报目标连接逻辑服务器列表
#define EV_CONNSRV_DESC_MSG   5  //与某些逻辑服务器断开连接上报
#define EV_CONNSRV_CONN_MSG   6  //与某些逻辑服务器成功连接上报
#define EV_ALARM_MSG    	7	 //上报告警信息
#define EV_CONF_INFO 		8    //上报终端的会议信息
#define EV_PFMINFO_MSG 		9    //上报终端性能信息
#define EV_VERSION_MSG		10	 //上报版本信息
#define EV_EXCEPTION_FILE   12   //崩溃重启后上报

//nms
#define EV_REG_ACK			12   //设备注册请求响应ACK消息
#define EV_REG_NACK         13   //设备注册请求响应NACK消息
#define EV_HEARTBEAT_ACK    14   //保活消息ACK消息

//server
#define EV_PFMINFO_CPU      15  //上报设备CPU性能
#define EV_PFMINFO_MEM      16  //上报设备MEM性能
#define EV_PFMINFO_NETCARD  17  //上报设备NETCARD性能
#define EV_PFMINFO_DISK     18  //上报设备DISK性能
#define EV_SYSTIME_SYNC     19  //上报物理服务器和NTP服务器时间同步状态
#define EV_COLLECTOR_HEARTBEAT 20 //collector心跳消息
#define EV_DEV_ONLINE       21 //设备上线
#define EV_DEV_OFFLINE      22  //设备下线
#define EV_PAS_INFO         23  //PAS上报逻辑服务器业务信息
#define EV_PAS_P2PCONF_CREATE 24 //PAS上报创建点对点会议信息
#define EV_PAS_P2PCONF_DESTROY 25 //PAS上报销毁点对点会议信息
#define EV_MPCD_INFO        26  //MPCD报逻辑服务器业务信息
#define EV_MCU_INFO         27  //MCU上报逻辑服务器基本信息
#define EV_MCU_CONF_CREATE  28  //MCU上报多点会议创建会议/结束会议消息
#define EV_MCU_CONF_DESTROY 29  //MCU上报多点会议结会消息
#define EV_MCU_MT_ADD       30  //MCU上报多点会议中终端入会消息
#define EV_MCU_MT_DEL       31  //MCU上报多点会议中终端离会消息
#define EV_MP_INFO			32  //MP上报逻辑服务器业务信息
#define EV_PRS_INFO			33  //PRS上报逻辑服务器业务信息
#define EV_SUS_INFO 		34  //SUS上报逻辑服务器业务信息
#define EV_SUSMGR_INFO		35  //SUSMGR上报逻辑服务器业务信息
#define EV_APS_INFO		    36  //APS上报逻辑服务器业务信息
#define EV_LGS_INFO         37  //LGS上报逻辑服务器业务信息
#define EV_NTS_INFO			38  //NTS上报逻辑服务器业务信息
#define EV_XMPP_INFO		39  //XMPP上报服务器业务信息

//other json
#define J_HEARTBEAT_REQ_MSG "{\"eventid\": \"EV_HEARTBEAT_REQ\"}"
#define J_LOGOUT_REQ        "{\"eventid\": \"EV_LOGOUT_REQ\"}"

//callback define
#define EV_HEARTBEAT_REQ_CB  	MsgTimer00300_cb
#define EV_PFMINFO_MSG_CB    	MsgTimer00900_cb
#define EV_PFMINFO_CPU_CB   	MsgTimer001500_cb 
#define EV_PFMINFO_MEM_CB     	MsgTimer001600_cb
#define EV_PFMINFO_NETCARD_CB  	MsgTimer001700_cb
#define EV_PFMINFO_DISK_CB     	MsgTimer001800_cb 
#define EV_DEV_ONLINE_CB    	MsgTimer002100_cb 

struct  tagMsg
{
	int MsgId;
	char MsgType[L_DEVTYPE_LEN];       //消息字符串
	int Timer;
	int nFlag;                         //消息的种类
};



#endif /*_PTLTYPES_H*/
