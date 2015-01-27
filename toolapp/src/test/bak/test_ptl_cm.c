/*====================================================================
 * ģ����  : test ptl cm
 * �ļ���  : test_ptl_cm.c
 * ����ļ�:
 * ʵ�ֹ���:  ������Դ���غ���
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/15        v1.0        YLI                          ����
 * ====================================================================*/
#include "testunit.h"
#include "common.h"

 void test_load_param(void)
 {
	if(_tShmInit() != SUCCESS)
	{
		return;
	}
	
 	if(nLoadParam() != SUCCESS)
 	{
		_tShmRm();
 		printf("Load parameter fail!!\n");
 		return;
 	}

 	printf("ip   : %s\n",_gstrpShm->rc.nSrvIP);
 	printf("port : %d\n",_gstrpShm->rc.nSrvPort);
 	printf("TermNum : %d\n",_gstrpShm->rc.nTermNum);
 	printf("PhyNum : %d\n",_gstrpShm->rc.nPhyNum);
 	printf("LogicNum : %d\n",_gstrpShm->rc.nLogicNum);
 	printf("TestTime : %d\n",_gstrpShm->rc.nTestTime);
 	int i;
 	for (i = 0; _gstrpShm->rc.strTagMsg[i].MsgId != 0; ++i)
 	{
 		printf("id:%d\nmsgtype:%s\ntimer:%d\nflag:%d\n",
 				_gstrpShm->rc.strTagMsg[i].MsgId,
 				_gstrpShm->rc.strTagMsg[i].MsgType,
 				_gstrpShm->rc.strTagMsg[i].Timer,
 				_gstrpShm->rc.strTagMsg[i].nFlag);
 	}
 	printf("test's msg count [%d]:\n",_gstrpShm->msgInf.nCount);
 	for(i = 0;i < _gstrpShm->msgInf.nCount; i++)
 	{
 		printf("id:%d\nmsgtype:%s\ntimer:%u\nflag:%d\nfn:%x\n",
 				_gstrpMsg[i].nMsgType,
 				 _gstrpMsg[i].saMsgType,
 				 _gstrpMsg[i].tv.tv_sec,
 				 _gstrpMsg[i].nFlag,
 				 _gstrpMsg[i].fn);
 	}
	_tShmRm();
 	
 }
void test_init_done(void)
{
	if(vSysInit() != SUCCESS)
	{
		vSysDone();
		return;
	}
	printf("ip   : %s\n",_gstrpShm->rc.nSrvIP);
 	printf("port : %d\n",_gstrpShm->rc.nSrvPort);
 	printf("TermNum : %d\n",_gstrpShm->rc.nTermNum);
 	printf("PhyNum : %d\n",_gstrpShm->rc.nPhyNum);
 	printf("LogicNum : %d\n",_gstrpShm->rc.nLogicNum);
 	printf("TestTime : %d\n",_gstrpShm->rc.nTestTime);
 	int i;
 	for (i = 0; _gstrpShm->rc.strTagMsg[i].MsgId != 0; ++i)
 	{
 		printf("id:%d\nmsgtype:%s\ntimer:%d\nflag:%d\n",
 				_gstrpShm->rc.strTagMsg[i].MsgId,
 				_gstrpShm->rc.strTagMsg[i].MsgType,
 				_gstrpShm->rc.strTagMsg[i].Timer,
 				_gstrpShm->rc.strTagMsg[i].nFlag);
 	}
 	printf("test's msg count [%d]:\n",_gstrpShm->msgInf.nCount);
 	for(i = 0;i < _gstrpShm->msgInf.nCount; i++)
 	{
 		printf("id:%d\nmsgtype:%s\ntimer:%u\nflag:%d\nfn:%x\n",
 				_gstrpMsg[i].nMsgType,
 				 _gstrpMsg[i].saMsgType,
 				 _gstrpMsg[i].tv.tv_sec,
 				 _gstrpMsg[i].nFlag,
 				 _gstrpMsg[i].fn);
 	}
 	vSysDone();
 }


