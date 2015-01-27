/*====================================================================
 * 模块名  : test util
 * 文件名  : test_util.c
 * 相关文件:
 * 实现功能: 测试一些简单工具
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0        YLI                           创建
 * ====================================================================*/
#include "testunit.h"
#include "common.h"

void test_util()
{
	char tmp[100];
	strcpy(tmp,"strIng");
	printf("test string Lower to Upper\n");
	printf("before:%s\n",tmp);
	StrToUpper(tmp);
	printf("after:%s\n",tmp);
	
	memset(tmp,0x00,sizeof tmp);
	strcpy(tmp,"STRiNG");
	printf("test string Lower to Upper\n");
	printf("before:%s\n",tmp);
	StrToLower(tmp);
	printf("after:%s\n",tmp);

	memset(tmp,0x00,sizeof tmp);
	strcpy(tmp,"STRiNG      ");
	printf("test string remove right space\n");
	printf("before:%s\n",tmp);
	RTrim(tmp);
	printf("after:%s\n",tmp);

	memset(tmp,0x00,sizeof tmp);
	strcpy(tmp,"        STRiNG");
	printf("test string remove left space\n");
	printf("before:%s\n",tmp);
	LTrim(tmp);
	printf("after:%s\n",tmp);

	memset(tmp,0x00,sizeof tmp);
	strcpy(tmp,"        STRiNG            ");
	printf("test string remove left and right space\n");
	printf("before:%s\n",tmp);
	Trim(tmp);
	printf("after:%s\n",tmp);
}
