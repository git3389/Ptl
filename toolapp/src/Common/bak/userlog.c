/*====================================================================
 * ģ����  : userlog
 * �ļ���  : userlog.c
 * ����ļ�: 
 * ʵ�ֹ���: ��־����
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20    v1.0        YLI           YLI
 * ====================================================================*/

#include "ptlconst.h"
#include "jansson.h"
#include "ptlutil.h"
#include "userlog.h"

//global
static int _gsInitFlag = 0;
static LogCfg _gstrLogCfg;
static char szCfgFile[256];

//static
static void getLevelStr(int level,char *vspLevelStr);
static int GetConfig(LogCfg *vspLogCfg);
static json_t *GetJsonObj(FILE *fp,const char *key);
static void CheckFileStat(char *logName,int logSize);
static int CheckDirectory(const char *dirname);
static void Archive(int sig);
/*====================================================================
 * ������   : _vWriteLog
 * ����     : ����־��Ϣд���ļ���
 * �㷨ʵ�� : ���ļ�����һ����ʽд����־��Ϣ
 * ����˵�� : vspFileName  ������־��Ϣ���ļ���
 * 			  vLine        ��������
 * 			  vspFileName  ������
 * 			  vLevel       ��־����,��EL_��ͷ�ĺ궨��
 * 			  vspLogName   д����־���ļ���
 * 			  vspFmt       ��ʽ
 * ����ֵ˵��: ��
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20    v1.0        YLI            YLI
 * ====================================================================*/
void _vWriteLog(char *vspFileName,int vLine,const char *vspFunName,
		int vLevel,char *vspLogName,char *vspFmt,...)
{
	va_list strAp;
	char vspLevelStr[10];
	char sDateTime[20]={0};
	char logPath[L_LOG_PATH_LEN_MAX];
	FILE *fp = NULL;

	GetCurrentTime(sDateTime);
	getLevelStr(vLevel,vspLevelStr);
	if(strlen(vspLevelStr) == 0)
	{
		printf("error:log level error!\n");
		return;
	}
	if(_gsInitFlag != F_LOG_INIT)
	{
		if(vLogInit() == LOGFAIL)
		{
			printf("error:vlogInit error!!!!\n");
			return;
		}
	}
	if(CheckDirectory(_gstrLogCfg.logDir) == LOGFAIL)
	{
		printf("error:log path error!!!!\n");
		return;
	}	

	if(strlen(_gstrLogCfg.logDir) > L_LOG_PATH_LEN_MAX)
	{
		printf("error:log path name too long!!!!\n");
		return;
	}

	sprintf(logPath,"%s/%s",_gstrLogCfg.logDir,vspLogName);

	CheckFileStat(logPath,_gstrLogCfg.logFileMaxSize);

	fp = fopen(logPath,"a+");
	
	if(fp == NULL)
	{
		printf("error:[%s] [%d] log file open fail!!! [%d]\n",vspFileName,vLine,errno);
		return;
	}

	va_start(strAp,vspFmt);
	if (vLine == 0)
	{
		fprintf(fp,"[%s] ",sDateTime);
	}
	else
	{
		fprintf(fp,"[%s] [%u] [%s] [%d] [%s] %s:",
				sDateTime,getpid(),
				vspFileName,vLine,
				vspFunName,vspLevelStr);
	}
	vfprintf(fp,vspFmt,strAp);
	fprintf(fp,"\n");
	va_end(strAp);

	fflush(fp);
	fclose(fp);
	fp = NULL;
	return;
}

/*====================================================================
 * ������   : vLogInit
 * ����     : ��ʼ����־ģ��   
 * �㷨ʵ�� : �������ļ��л����Ӧ����
 * ����˵�� : ��
 * ����ֵ˵��: �ɹ�  0
 * 			   ʧ��  -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20    v1.0        YLI           YLI
 * ====================================================================*/
int vLogInit()
{
	memset(&_gstrLogCfg,0x00,sizeof(LogCfg));
	_gstrLogCfg.logFileMaxSize = S_LOGFILE_SIZE;
	if(LOGFAIL == GetConfig(&_gstrLogCfg))
	{
		printf("error:Get log config error!!!\n");
		return LOGFAIL;
	}
	return LOGOK;
}

/*====================================================================
 * ������   : getLevelStr
 * ����     : �����־�����Ӧ���ַ���
 * �㷨ʵ�� : ������־���𣬷��ض�Ӧ�ַ���
 * ����˵�� : vLevel     ��־����
 * 			  vspLevelStr  ��־�����Ӧ���ַ���
 * ����ֵ˵��: ��
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20    v1.0          YLI          YLI
 * ====================================================================*/
static void getLevelStr(int vLevel,char *vspLevelStr)
{
	memset(vspLevelStr,0x00,strlen(vspLevelStr));
	switch(vLevel)
	{
	case EL_DEBUG:
		strcpy(vspLevelStr,"debug");
		break;

	case EL_INFO:
		strcpy(vspLevelStr,"info");
		break;

	case EL_WARNING:
		strcpy(vspLevelStr,"warning");
		break;

	case EL_ERR:
		strcpy(vspLevelStr,"error");
		break;

	case EL_FATAL:
		strcpy(vspLevelStr,"fatal");
		break;

	default:
		break;
	}
}

/*====================================================================
 * ������   : GetConfig
 * ����     : ��õ������ļ��ж�����־��ز���   
 * �㷨ʵ�� : ʹ��jasson�������ؼ�ΪLog�����ֵ�����浽��־�ṹ����
 * ����˵�� : vspLogCfg   ָ��һ����־�ṹ��
 * ����ֵ˵��: �ɹ�    0
 * 			   ʧ��    -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20   v1.0         YLI           YLI
 * ====================================================================*/
static int GetConfig(LogCfg *vspLogCfg)
{
	FILE *fp = NULL;
	json_t *obj;
	json_t *logArr;
    const char *key;
	json_t *value;
	char keytmp[100];
	size_t index = 0;
	if(getenv("A_PROFILE_NAME") == NULL)
	{
		printf("error:evn set error!!!\n");
		return LOGFAIL;
	}

	if(strlen(getenv("A_PROFILE_NAME")) >= 256)
	{
		printf("error:config file name too long!!!\n");
		return LOGFAIL;
	}
	memset(szCfgFile,0x00,sizeof szCfgFile);
	strcpy(szCfgFile,getenv("A_PROFILE_NAME"));
	if((fp = fopen(szCfgFile,"r")) == NULL)
	{
		printf("error:config file open error!!!\n");
		return LOGFAIL;
	}

	obj = GetJsonObj(fp,J_LOG_KEY);

	if(obj == NULL)
	{
		printf("error:config file configuration error!!!\n");
		printf("warning:please check \"log\" whether the lower\n");
		return LOGFAIL;
	}

	//�����־����
	logArr = json_object_get(obj,"log");	
	for(index = 0;index < json_array_size(logArr);index++)
	{
		json_object_foreach(json_array_get(logArr,index),key,value)
		{
			strcpy(keytmp,key);
			Trim(keytmp);
			StrToLower(keytmp);
			if(strncmp(keytmp,"logdir",6) == 0)
			{
				strcpy(vspLogCfg->logDir,json_string_value(value));
			}
			else if(strcmp(keytmp,"logsize") == 0)
			{
				vspLogCfg->logFileMaxSize = (int)json_integer_value(value);
			} 
			else if(0 == strcmp(keytmp,"achivesize"))
			{
				vspLogCfg->logAchiveSize = (int)json_integer_value(value);
			}
		}
	}
	if((strlen(vspLogCfg->logDir) == 0)
		|| (vspLogCfg->logFileMaxSize == 0))
	{
		printf("error:log path or max size don't configure!!!\n");
		fclose(fp);
		fp = NULL;
		json_decref(logArr);
		return LOGFAIL;
	}
	_gsInitFlag = F_LOG_INIT;
	fclose(fp);
	fp = NULL;
	json_decref(logArr);
	return LOGOK;
}

/*====================================================================
 * ������   : GetJsonObj
 * ����     : �������ļ���json�ı��л��һ��objͨ��key
 * �㷨ʵ�� : 
 * ����˵�� : fp       �ļ�ָ��
 * 			  key      json�ļ��е�key
 * 			  obj      ��õ�obj
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20    v1.0        YLI             YLI
 * ====================================================================*/
static json_t *GetJsonObj(FILE *fp,const char *key)
{
	json_t *root;
	json_t *app;
	json_error_t error;
	json_t *obj;
	size_t index = 0;
	if(fp == NULL)
	{
		printf("file point is null!!!! \n");
		return NULL;
	}	
	root = json_loadf(fp,JSON_DECODE_ANY,&error);
	if(root == NULL)	
	{
		printf("decoder error: %s\n",error.text);
		return NULL;
	}
	app = json_object_get(root,"app");
	if(app == NULL || !json_is_array(app))
	{
		printf("error:config file configuration error!!!\n");
		printf("warning:please check \"app\" whether the lower\n");
		return NULL;
	}
	void *iter;
	for(index = 0;index < json_array_size(app);index++)
	{
		obj = json_array_get(app,index);
		iter = json_object_iter(obj);
		//ƥ��logkey
		while(iter)
		{
			if(strcmp(key,json_object_iter_key(iter)) == 0)
			{
				return obj;	
			}
		}
	}
	json_decref(root);
	json_decref(app);
	return NULL;	
}

/*====================================================================
 * ������   : CheckFileStat 
 * ����    :  ��鵱ǰ��־�ļ��Ĵ�С�������С���������־�ļ���С��
 *             �򽫵�ǰ��־�ļ����������ݣ����´�һ��־�ļ�������־��¼
 * �㷨ʵ�� :
 * ����˵�� : logName  ��ǰ����־�ļ���
 * 			  logSize  �����־�ļ���С
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/20     v1.0       YLI                          ����
 * ====================================================================*/
static void CheckFileStat(char *logName,int logSize)
{
	char sNewLogName[256]={0};
	char sDateTime[20]={0};
	struct stat	statbuf;
	long lFileLmts;

	memset (&statbuf, 0x00, sizeof(statbuf));
	lFileLmts=LOG_SIZE_UNIT * logSize;
#if 1
	if(0==stat(logName, &statbuf))
	{
		if(statbuf.st_size >= lFileLmts)
		{
			GetCurrentTime(sDateTime);
			sprintf(sNewLogName, "%s.%s", logName, sDateTime);
			rename(logName, sNewLogName);/*�������ļ�*/
		}
	}
#endif
}
/*====================================================================
 * ������    : Archive
 * ����      : ����־�ļ��ﵽһ������ʱ���й鵵����ɾ���Ѿ��鵵�ļ�
 * �㷨ʵ��  : 
 * ����˵��  : 
 * ����ֵ˵��:              
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/23       v1.0        YLI                          ����
 * ====================================================================*/
static void Archive(int sig)
{
	DIR *dir;
	

}
/*====================================================================
 * ������   : CheckDirectory 
 * ����    : ���·���Ƿ�Ϸ���·���������򴴽�
 * �㷨ʵ�� :
 * ����˵�� : dirname  ·���� 
 * ����ֵ˵��: �ɹ�  0
 * 			   ʧ��  -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 *  2014/12/20   v1.0          YLI                        ����
 * ====================================================================*/
static int CheckDirectory(const char *dirname)
{
	char cmd[256];
 	if (access(dirname, 0) != 0)      /*F_OK*/
	{		
		memset(cmd, 0x00, sizeof(cmd));
		sprintf(cmd, "mkdir -p %s", dirname);
		system(cmd);

	}

	if (access(dirname, 0) != 0)
	{
		return LOGFAIL;        
	}
	return LOGOK;
}

int _vAssertFail(char *vspFileName,int vLine,const char *vspFunName)
{
	_vWriteLog(vspFileName,vLine,vspFunName,EL_ERR,"err.log","assert fail!");
	return LOGOK;
}

void _vDebugBreak()
{
	_vWriteLog(__FILE__,__LINE__,__func__,EL_ERR,"err.log","exit!");
	exit(-1);
}
