/*====================================================================
 * 模块名  :
 * 文件名  :
 * 相关文件:
 * 实现功能:
 * 作者   : YLI
 * 版权   : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *
 * ====================================================================*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ptlpack.h"
#include "ptlbase.h"
#include "common.h"


/*Program entrance*/
int main(int argc, char** argv)
{
#if 0
	unsigned char i = 0;
	int k = 0;
	for(i=0;i<128;i++)
	{
		printf("%d\n",i&3);
		k += (i&3);
	}
	printf("k=%d\n",k);
	vLogTrace(EL_INFO,"test");
	vLogErr("test\n");
	MsgBagInf* msg;
	msg = nMsgPack("test");
	printf("len:%d\n",msg->len);
	printf("msg:");
	char *c = msg->msg;
	int i = 0;
	for(i = 0;i<msg->len;i++)
	{
		printf("%d\n",(char)*(c+i));
	}
	printf("\n");
	nMsgPackFree(msg);
	//test config
#endif
	//test shm
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
		return -1;
	}
	printf("tcn:%s\n",_gstrpShm->rc.saTCN);
	printf("bmn:%d\n",_gstrpShm->rc.nBaseMaxNum);
	printf("mcun:%d\n",_gstrpShm->rc.nMCUNum);
	getchar();
	_tSemRm();
	_tShmRm();
	return 0;
}


