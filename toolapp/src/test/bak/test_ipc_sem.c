/*====================================================================
 * ģ����  : test ipc/sem
 * �ļ���  : test_ipc_sem
 * ����ļ�: 
 * ʵ�ֹ���: ���Թ����ڴ����������ź����������
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/31     v1.0        YLI                        ����
 * ====================================================================*/
#include "testunit.h"
#include "ptlbase.h"
#include "common.h"
void test_ipc_sem()
{
	if(_tShmInit() != SUCCESS)
	{
		printf("_tShmInit fail\n");
	}
	_tSemInit();
	getchar();
	if(_tShmDetach() != SUCCESS)
	{
		printf("_tShmDetach fail");
	}
	if(_tShmAtach() != SUCCESS)
	{
		printf("_tShmAtach fail");
		_tShmRm();
		return;
	}
	printf("tcn:%s\n",_gstrpShm->rc.saTCN);
	printf("bmn:%d\n",_gstrpShm->rc.nBaseMaxNum);
	printf("phyn:%d\n",_gstrpShm->rc.nPhyNum);
	printf("logicn:%d\n",_gstrpShm->rc.nLogicNum);
	getchar();
	_tSemRm();
	_tShmRm();
}
