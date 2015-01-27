/*====================================================================
 * 模块名  : test log
 * 文件名  :
 * 相关文件:
 * 实现功能: 对日志模块的测试
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/20     v1.0        YLI                         创建
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


