/*====================================================================
 * ģ����  : test util
 * �ļ���  : test_util.c
 * ����ļ�:
 * ʵ�ֹ���: ����һЩ�򵥹���
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0        YLI                           ����
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
