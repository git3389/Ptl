#include "ptlconst.h"
#include "ptlutil.h"


/*====================================================================
 * ������   : StrToLower
 * ����    : ��һ���ַ����Ĵ�д��ĸת����Сд
 * �㷨ʵ�� : �����ַ����е�ÿ���ַ�ʹ������api tolower��������дת����С
 * ����˵�� : str Ҫת�����ַ���
 * ����ֵ˵��: ��
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0         YLI                         ����
 * ====================================================================*/
void StrToLower(char *str)
{
	int i = 0;
	for(i=0;i<(int)strlen(str);i++)
	{
		str[i] = tolower(str[i]);
	}
}
/*====================================================================
 * ������   : StrToUpper
 * ����    :  ��һ���ַ����е�Сдת���ɴ�С
 * �㷨ʵ�� :
 * ����˵�� : str ת�����ַ���
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0         YLI                           ����
 * ====================================================================*/
void StrToUpper(char *str)
{
	int i = 0;
	for(i=0;i<(int)strlen(str);i++)
	{
		str[i] = toupper(str[i]);
	}
}

/*====================================================================
 * ������   : Trim
 * ����    : ȥ���ַ��������ҿո�
 * �㷨ʵ�� :
 * ����˵�� : str  Ҫ���ո���ַ���
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25     v1.0        YLI							����
 * ====================================================================*/
void Trim(char *str)
{
	LTrim(str);
	RTrim(str);
}

/*====================================================================
 * ������   : RTrim 
 * ����    : ȥ���ַ������ұ߿ո�
 * �㷨ʵ�� :
 * ����˵�� : str Ҫȥ�ո���ַ���
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0         YLI                          ����
 * ====================================================================*/
void RTrim(char *str)
{
	if(str == NULL)	return;
	int len = strlen(str);
	char *p = str;
	int i = 0;
	for(i=len-1;i>=0;i--)
	{
		//find right the fist of not space
		if(p[i] != ' ')
			break;
	}
	p[i+1] = '\0';
}

/*====================================================================
 * ������   : LTrim
 * ����    : ȥ���ַ�������� * �㷨ʵ�� :
 * ����˵�� : str Ҫ���ո���ַ���
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0        YLI                           ����
 * ====================================================================*/
void LTrim(char *str)
{
	char *p = str;
	while(1)
	{
		if(*p == ' ')
		{
			while(*p)
			{
				*p=*(p+1);
				p++;
			}
		}
		else
		{
			break;
		}
		p=str;
	}	
}
										
/*====================================================================
 * ������   : GetCurrentTime
 * ����    :  ��õ�ǰϵͳʱ��
 * �㷨ʵ�� :
 * ����˵�� : sCurrentTime ��õ�ʱ���ַ���,��ʽΪ19900122000000
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0           YLI                       ����
 * ====================================================================*/
void GetCurrentTime(char *sCurrentTime)
{
   	time_t current;
	struct tm *tmCurrentTime;

	tzset();
	time(&current);
	tmCurrentTime = localtime(&current);
	sprintf(sCurrentTime, "%4d%02d%02d%02d%02d%02d",
			tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
			tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
			tmCurrentTime->tm_min, tmCurrentTime->tm_sec);
}

/*====================================================================
 * ������   : GetLocalTime
 * ����    :  ��õ�ǰϵͳʱ��
 * �㷨ʵ�� :
 * ����˵�� : sCurrentTime ��õ�ʱ���ַ���,��ʽΪ1990-01-22 00:00:00
 * ����ֵ˵��:
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/25    v1.0           YLI                       ����
 * ====================================================================*/
void GetLocalTime(char *sCurrentTime)
{
   	time_t current;
	struct tm *tmCurrentTime;

	tzset();
	time(&current);
	tmCurrentTime = localtime(&current);
	sprintf(sCurrentTime, "%4d-%02d-%02d/%02d:%02d:%02d",
			tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
			tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
			tmCurrentTime->tm_min, tmCurrentTime->tm_sec);
}

/*====================================================================
 * ������   : tPflGetString
 * ����    : ͨ���ؼ�����json�����з����ҵ����ַ������ֻ�޹ؼ��ֵ��    
 * �㷨ʵ�� :
 * ����˵�� : vspFileName json�ļ���������·��
 * 			  key         ���ҵĹؼ���
 * 			  vspValue    �ҵ��Ľ���ַ���
 *			  nLen        �������ַ����Ļ���������
 * ����ֵ˵��: �ɹ�       0
 * 			   ʧ��       -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/30     v1.0        YLI                           ����
 * 2015/1/5       v1.0        YLI                         ���Ӳ���nLen,��ֹ��õ�
 *                                                        �ַ�����������ʹ���������
 * ====================================================================*/
int tPflGetString(char *vspFileName,char *key,char *vspValue,int nLen)
{
	json_t *value;
	if((value = tPflGetJsonObj(vspFileName,key)) == NULL)
	{
		vspValue = NULL;
		json_decref(value);
		return FAILUER;
	}
	else
	{
		if(json_is_string(value))
		{
			if(strlen(json_string_value(value)) > (size_t)nLen)
			{
				vspValue = NULL;
				return FAILUER;
			}
			strcpy(vspValue,json_string_value(value));
		}
		else
		{
			vspValue = NULL;
			json_decref(value);
			return FAILUER;
		}
	}
	json_decref(value);
	return SUCCESS;
}

/*====================================================================
 * ������   : tPflGetInt 
 * ����    : ͨ���ؼ�����json�ļ��з���һ��������ֻ�޹ؼ��ֵ�һ
 * �㷨ʵ�� :
 * ����˵�� : vspFileName   json�ļ���
 * 			  key			���ҵĹؼ���
 * 			  vspValue      �ҵ�������
 * ����ֵ˵��: �ɹ�         0
 * 			   ʧ��         -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/30     v1.0        YLI                         ����
 * ====================================================================*/
int tPflGetInt(char *vspFileName,char *key,int *vspValue)
{
	json_t *value;
	if((value = tPflGetJsonObj(vspFileName,key)) == NULL)
	{
		*vspValue = 0;
		json_decref(value);
		return FAILUER;
	}
	else
	{
		if(json_is_integer(value))
		{
			*vspValue = json_integer_value(value);
		}
		else
		{
			*vspValue = 0;
			json_decref(value);
			return FAILUER;
		}
	}
	json_decref(value);
	return SUCCESS;
}
int tPflGetDouble(char *vspFileName,char *key,double *vspValue)
{
	json_t *value;
	if((value = tPflGetJsonObj(vspFileName,key)) == NULL)
	{
		*vspValue = 0;
		json_decref(value);
		return FAILUER;
	}
	else
	{
		if(json_is_real(value))
		{
			*vspValue = json_real_value(value);
		}
		else
		{
			*vspValue = 0.0;
			json_decref(value);
			return FAILUER;
		}
	}
	json_decref(value);
	return SUCCESS;
}
/*====================================================================
 * ������   :tPflGetJsonObj
 * ����    : �ҵ���һ����ؼ���ƥ�䲢��ֵ��һ������
 * �㷨ʵ�� :
 * ����˵�� : vspFileName      json�ļ���
 * 			  key              ���ҹؼ���
 * 			  vspValue         ���ص�json_t����
 * ����ֵ˵ : �ɹ�             0
 * 			  ʧ��             -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/30     v1.0        YLI                         ����
 * ====================================================================*/
json_t *tPflGetJsonObj(char *vspFileName,char *key)
{
	FILE *fp = NULL;
	json_t *root;
	json_t *value;
	json_error_t error;
	char *keytmp;
	void *iter;
	if((fp = fopen(vspFileName,"r")) == NULL)
		return NULL;
	
	root = json_loadf(fp,JSON_DECODE_ANY,&error);
	if(root == NULL)
	{
		fclose(fp);
		fp = NULL;
		return NULL;
	}
	iter = json_object_iter(root);
	while(iter)
	{
		keytmp = (char *)json_object_iter_key(iter);
		value = json_object_iter_value(iter);
		Trim(keytmp);
		if(strcmp(keytmp,key) == 0)
		{
			fclose(fp);
			fp = NULL;
			return value;
		}
		iter = json_object_iter_next(root,iter);
	}
	fclose(fp);
	fp = NULL;
	json_decref(root);
	return NULL;
}
/*====================================================================
 * ������    : CovertHourToSec
 * ����      : ��Сʱת������
 * �㷨ʵ��  : 
 * ����˵��  : hr Ҫת����Сʱ��
 * ����ֵ˵��: �ɹ�  ����ת������
 *             ʧ��  ����0             
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/8       v1.0        YLI                          ����
 * ====================================================================*/
long CovertHourToSec(double hr)
{
	if (hr < 0)
	{
		return 0L;
	}
	return (long)(hr * 60 * 60);
}

