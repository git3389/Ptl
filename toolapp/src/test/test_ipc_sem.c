/*====================================================================
 * 模块名  : test ipc/sem
 * 文件名  : test_ipc_sem
 * 相关文件: 
 * 实现功能: 测试共享内存分配情况和信号量分配情况
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/31     v1.0        YLI                        创建
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
