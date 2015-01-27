#include "ptlconst.h"
#include "ptlutil.h"


/*====================================================================
 * 函数名   : StrToLower
 * 功能    : 将一个字符串的大写字母转换成小写
 * 算法实现 : 遍历字符串中的每个字符使用内置api tolower函数将大写转换成小
 * 参数说明 : str 要转换的字符串
 * 返回值说明: 无
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0         YLI                         创建
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
 * 函数名   : StrToUpper
 * 功能    :  将一个字符串中的小写转换成大小
 * 算法实现 :
 * 参数说明 : str 转换的字符串
 * 返回值说明:
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0         YLI                           创建
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
 * 函数名   : Trim
 * 功能    : 去除字符串的左右空格
 * 算法实现 :
 * 参数说明 : str  要除空格的字符串
 * 返回值说明:
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25     v1.0        YLI							创建
 * ====================================================================*/
void Trim(char *str)
{
	LTrim(str);
	RTrim(str);
}

/*====================================================================
 * 函数名   : RTrim 
 * 功能    : 去除字符串的右边空格
 * 算法实现 :
 * 参数说明 : str 要去空格的字符串
 * 返回值说明:
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0         YLI                          创建
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
 * 函数名   : LTrim
 * 功能    : 去除字符串的左空 * 算法实现 :
 * 参数说明 : str 要除空格的字符串
 * 返回值说明:
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0        YLI                           创建
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
 * 函数名   : GetCurrentTime
 * 功能    :  获得当前系统时间
 * 算法实现 :
 * 参数说明 : sCurrentTime 获得的时间字符串,格式为19900122000000
 * 返回值说明:
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0           YLI                       创建
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
 * 函数名   : GetLocalTime
 * 功能    :  获得当前系统时间
 * 算法实现 :
 * 参数说明 : sCurrentTime 获得的时间字符串,格式为1990-01-22 00:00:00
 * 返回值说明:
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/25    v1.0           YLI                       创建
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
 * 函数名   : tPflGetString
 * 功能    : 通过关键字在json文体中返回找到的字符串，种幌薰丶值ヒ    
 * 算法实现 :
 * 参数说明 : vspFileName json文件名加完整路径
 * 			  key         查找的关键字
 * 			  vspValue    找到的结果字符串
 *			  nLen        保存结果字符串的缓冲区长度
 * 返回值说明: 成功       0
 * 			   失败       -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30     v1.0        YLI                           创建
 * 2015/1/5       v1.0        YLI                         增加参数nLen,防止获得的
 *                                                        字符串过长，而使缓冲区溢出
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
 * 函数名   : tPflGetInt 
 * 功能    : 通过关键字在json文件中返回一个整数，只限关键字单一
 * 算法实现 :
 * 参数说明 : vspFileName   json文件名
 * 			  key			查找的关键字
 * 			  vspValue      找到的整数
 * 返回值说明: 成功         0
 * 			   失败         -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30     v1.0        YLI                         创建
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
 * 函数名   :tPflGetJsonObj
 * 功能    : 找到第一个与关键字匹配并且值是一个对象
 * 算法实现 :
 * 参数说明 : vspFileName      json文件名
 * 			  key              查找关键字
 * 			  vspValue         返回的json_t对象
 * 返回值说 : 成功             0
 * 			  失败             -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30     v1.0        YLI                         创建
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
 * 函数名    : CovertHourToSec
 * 功能      : 将小时转换成秒
 * 算法实现  : 
 * 参数说明  : hr 要转换的小时数
 * 返回值说明: 成功  返回转换的秒
 *             失败  返回0             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/8       v1.0        YLI                          创建
 * ====================================================================*/
long CovertHourToSec(double hr)
{
	if (hr < 0)
	{
		return 0L;
	}
	return (long)(hr * 60 * 60);
}

