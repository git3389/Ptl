/*====================================================================
 * ģ����  : userlog
 * �ļ���  : userlog.h
 * ����ļ�:
 * ʵ�ֹ���: ������־ģ����ʹ�õĳ����������ӿڡ���־������������
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20    v1.0          YLI         YLI
 * ====================================================================*/
#ifndef _USERLOG_H
#define _USERLOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>

//log level
#define EL_DEBUG 			0
#define EL_INFO  			1
#define EL_WARNING 			2
#define EL_ERR				3
#define EL_FATAL			4

//const
#define N_INITIALIZE        1
#define LOGOK               0
#define LOGFAIL           -1

//len
#define L_LOG_PATH_LEN_MAX  256

//typedef
typedef struct
{
	char logDir[L_LOG_PATH_LEN_MAX];
	char logFileName[L_FILE_NAME_MAX_LEN];
	int logFileMaxSize;
	int logAchiveSize;
}LogCfg;

extern void _vWriteLog(char *vspFileName,int vLine,const char *vspFunName,int vLevel,char *vspLogName,char *vspFmt,...);
extern int vLogInit();
extern int _vAssertFail(char *vspFileName,int vLine,const char *vspFunName);
extern void _vDebugBreak();

#endif /*_USERLOG_H*/
