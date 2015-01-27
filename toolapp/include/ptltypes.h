/*====================================================================
 * ģ����  : ptltypes
 * �ļ���  : ptltypes.h
 * ����ļ�:
 * ʵ�ֹ���: �նˡ���������ص���Ϣ���͡��豸���͵ȵĺ궨��
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/29     v1.0        YLI                           ����
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
#define EV_REG_REQ  		1    //ע������
#define EV_LOGOUT_REQ   	2    //ע������
#define EV_HEARTBEAT_REQ 	3    //����
#define EV_SHOULD_CONNSRV_MSG 4  //�ϱ�Ŀ�������߼��������б�
#define EV_CONNSRV_DESC_MSG   5  //��ĳЩ�߼��������Ͽ������ϱ�
#define EV_CONNSRV_CONN_MSG   6  //��ĳЩ�߼��������ɹ������ϱ�
#define EV_ALARM_MSG    	7	 //�ϱ��澯��Ϣ
#define EV_CONF_INFO 		8    //�ϱ��ն˵Ļ�����Ϣ
#define EV_PFMINFO_MSG 		9    //�ϱ��ն�������Ϣ
#define EV_VERSION_MSG		10	 //�ϱ��汾��Ϣ
#define EV_EXCEPTION_FILE   12   //�����������ϱ�

//nms
#define EV_REG_ACK			12   //�豸ע��������ӦACK��Ϣ
#define EV_REG_NACK         13   //�豸ע��������ӦNACK��Ϣ
#define EV_HEARTBEAT_ACK    14   //������ϢACK��Ϣ

//server
#define EV_PFMINFO_CPU      15  //�ϱ��豸CPU����
#define EV_PFMINFO_MEM      16  //�ϱ��豸MEM����
#define EV_PFMINFO_NETCARD  17  //�ϱ��豸NETCARD����
#define EV_PFMINFO_DISK     18  //�ϱ��豸DISK����
#define EV_SYSTIME_SYNC     19  //�ϱ������������NTP������ʱ��ͬ��״̬
#define EV_COLLECTOR_HEARTBEAT 20 //collector������Ϣ
#define EV_DEV_ONLINE       21 //�豸����
#define EV_DEV_OFFLINE      22  //�豸����
#define EV_PAS_INFO         23  //PAS�ϱ��߼�������ҵ����Ϣ
#define EV_PAS_P2PCONF_CREATE 24 //PAS�ϱ�������Ե������Ϣ
#define EV_PAS_P2PCONF_DESTROY 25 //PAS�ϱ����ٵ�Ե������Ϣ
#define EV_MPCD_INFO        26  //MPCD���߼�������ҵ����Ϣ
#define EV_MCU_INFO         27  //MCU�ϱ��߼�������������Ϣ
#define EV_MCU_CONF_CREATE  28  //MCU�ϱ������鴴������/����������Ϣ
#define EV_MCU_CONF_DESTROY 29  //MCU�ϱ�����������Ϣ
#define EV_MCU_MT_ADD       30  //MCU�ϱ����������ն������Ϣ
#define EV_MCU_MT_DEL       31  //MCU�ϱ����������ն������Ϣ
#define EV_MP_INFO			32  //MP�ϱ��߼�������ҵ����Ϣ
#define EV_PRS_INFO			33  //PRS�ϱ��߼�������ҵ����Ϣ
#define EV_SUS_INFO 		34  //SUS�ϱ��߼�������ҵ����Ϣ
#define EV_SUSMGR_INFO		35  //SUSMGR�ϱ��߼�������ҵ����Ϣ
#define EV_APS_INFO		    36  //APS�ϱ��߼�������ҵ����Ϣ
#define EV_LGS_INFO         37  //LGS�ϱ��߼�������ҵ����Ϣ
#define EV_NTS_INFO			38  //NTS�ϱ��߼�������ҵ����Ϣ
#define EV_XMPP_INFO		39  //XMPP�ϱ�������ҵ����Ϣ

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
	char MsgType[L_DEVTYPE_LEN];       //��Ϣ�ַ���
	int Timer;
	int nFlag;                         //��Ϣ������
};



#endif /*_PTLTYPES_H*/
