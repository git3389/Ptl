/*====================================================================
 * ģ����  : test json parse
 * �ļ���  : test_parse_json.c
 * ����ļ�: testunit.h common.h
 * ʵ�ֹ���: ���Բ��ֶ�json�ļ��Ľ�������ģ��
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/30     v1.0       YLI                          ����
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
