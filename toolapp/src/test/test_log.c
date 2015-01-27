/*====================================================================
 * ģ����  : test log
 * �ļ���  :
 * ����ļ�:
 * ʵ�ֹ���: ����־ģ��Ĳ���
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20     v1.0        YLI                         ����
 * ====================================================================*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "testunit.h"
#include "common.h"

void test_log()
{
	
	vLogErr("test log error info!");
	vLogTrace(EL_INFO,"test log info level!");
	vLogTrace(EL_DEBUG,"test log debug level!");
	vLogTrace(EL_WARNING,"test log warning level!");
	vLogTrace(EL_FATAL,"test log fatal level!");
	vLogNMS(EL_DEBUG,"test_log.log","test sample log");
}


