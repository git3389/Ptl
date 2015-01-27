/*====================================================================
 * 模块名  : test json parse
 * 文件名  : test_parse_json.c
 * 相关文件: testunit.h common.h
 * 实现功能: 测试部分对json文件的解析功能模块
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30     v1.0       YLI                          创建
 * ====================================================================*/
#include "testunit.h"
#include "common.h"

void test_parse_json()
{
	char buf[256];
	int tmp;
	json_t *value;
	if(tPflGetString("test.json","str",buf,256) == 0)
	{
		printf("str:%s\n",buf);
	}
	if(tPflGetInt("test.json","int",&tmp) == 0)
	{
		printf("int:%d\n",tmp);
	}
	if((value = tPflGetJsonObj("test.json","str")) != NULL)
	{
		printf("obj:%s\n",json_string_value(value));
	}
}
