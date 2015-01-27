/*====================================================================
 * 模块名  : ptlpack
 * 文件名  : ptlpack.c
 * 相关文件:
 * 实现功能: 提供消息的打包与解包操作接口，以及包的释放
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/22   v1.0        YLI                          创建
 * ====================================================================*/

#include "ptlpack.h"
#include "common.h"


static MsgBagInf* nMsgBagInit(char *vspMsg);
static json_t *FindObj(json_t *root,const char *vspKey);

/*====================================================================
 * 函数名    : FindObj
 * 功能      : 递归的在一个json对象中查找满足条件的json对象
 * 算法实现  : 
 * 参数说明  : root   要查找的json根对象
 *				vspKey   要查找的json关键字
 * 返回值说明:  成功  找到的json对象
 *				失败  NULL            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/19       v1.0        YLI                          创建
 * ====================================================================*/
static json_t *FindObj(json_t *root,const char *vspKey)
{
	json_t *value = NULL;
	json_t *obj = NULL;
	const char *key;
	void *iter;
	if (root == NULL)
	{
		return NULL;
	}
	//iterator find json object
	iter = json_object_iter(root);
	while(iter)
	{
		key = json_object_iter_key(iter);
		value = json_object_iter_value(iter);
		//value whether is json object 

		if (strcmp(key,vspKey) == 0)
		{
			return value;
		}
		else
		{
			if (json_is_object(value))
			{
				obj = FindObj(value,vspKey);  //iterator another json object 
				if (obj != NULL)
				{
					return obj;
				}
			}
		}
		
		iter = json_object_iter_next(root,iter);
	}
	return NULL;
}
/*====================================================================
 * 函数名    : nMsgPack
 * 功能      : 将以json为数据结构的数据打包成按协议定义的发送的消息结构数据 
 * 算法实现  : 
 * 参数说明  : vspMsg 要打包的消息
 * 返回值说明: 成功   打包好的消息结构数据
 *			   失败   NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/22       v1.0        YLI                          创建
 * ====================================================================*/
MsgBagInf *nMsgPack(char *vspMsg)
{
	int msgLen = 0;	
	int nMsgLen = 0;	
	MsgBagInf *pMsgBag = nMsgBagInit(vspMsg); 
	if(pMsgBag == NULL)
	{
		vLogErr("打包初始化出错!\n");
		return NULL;
	}

	msgLen = strlen(vspMsg);
	nMsgLen = htonl(msgLen);     //covert to network byte order
	memcpy(pMsgBag->msg,&nMsgLen,sizeof(int));
	memcpy(pMsgBag->msg+4,vspMsg,msgLen);
	pMsgBag->len = msgLen + 4;

	return pMsgBag;
}
/*====================================================================
 * 函数名    : nMsgBagInit
 * 功能      : 分配保存打包数据的内存
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/22       v1.0        YLI                          创建
 * ====================================================================*/
MsgBagInf* nMsgBagInit(char *vspMsg)
{
	MsgBagInf *strMsgBag;

	strMsgBag = (MsgBagInf *)malloc(sizeof(MsgBagInf));	
	if(strMsgBag == NULL)
	{
		vLogErr("内存分配失败!\n");
		return NULL;
	}
 
	strMsgBag->len = 0;
	strMsgBag->msg = (char *)malloc(strlen(vspMsg) + 4 + 1);
	if(strMsgBag->msg == NULL)
	{
		vLogErr("内存分配失败!\n");
		return NULL;
	}
	memset(strMsgBag->msg,0x00,strlen(vspMsg) + 4 + 1);
	return strMsgBag;
}

/*====================================================================
 * 函数名    : nMsgGetString
 * 功能      : 从数据包中根据json关键字，获得字符串
 * 算法实现  : 
 * 参数说明  : vspMsg  数据包
 *				key    json关键字
 *				vspValue 获得的字符串
 *				len      获得的字符串保存的缓冲区大小
 * 返回值说明:  成功   0
 *				失败   -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/19       v1.0        YLI                          创建
 * ====================================================================*/
int nMsgGetString(const char *vspMsg,const char *key,char *vspValue,size_t len)
{
	json_t *value;
	if((value = nMsgGetObj(vspMsg,key)) == NULL)
	{
		vspValue = NULL;
		json_decref(value);
		return FAILUER;
	}
	else
	{
		if(json_is_string(value))
		{
			if(strlen(json_string_value(value)) > len)
			{
				vLogErr("parse json data get the length of string over buffer!!!");
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
 * 函数名    : nMsgGetInt
 * 功能      : 从数据包中根据json关键字，获得整数
 * 算法实现  : 
 * 参数说明  : vspMsg  数据包
 *				key    json关键字
 *				nValue 获得的整数
 * 返回值说明:  成功   0
 *				失败   -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/19       v1.0        YLI                          创建
 * ====================================================================*/
int nMsgGetInt(const char *vspMsg,const char *key,int *nValue)
{
	json_t *value;
	if((value = nMsgGetObj(vspMsg,key)) == NULL)
	{
		*nValue = 0;
		json_decref(value);
		return FAILUER;
	}
	else
	{
		if(json_is_integer(value))
		{
			*nValue = json_integer_value(value);
		}
		else
		{
			*nValue = 0;
			json_decref(value);
			return FAILUER;
		}
	}
	json_decref(value);
	return SUCCESS;
}

/*====================================================================
 * 函数名    : nMsgGetReal
 * 功能      : 从数据包中根据json关键字，获得实数
 * 算法实现  : 
 * 参数说明  : vspMsg  数据包
 *				key    json关键字
 *				nValue 获得的实数
 * 返回值说明:  成功   0
 *				失败   -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/19       v1.0        YLI                          创建
 * ====================================================================*/
int nMsgGetReal(const char *vspMsg,const char *key,double *nValue)
{
	json_t *value;
	if((value = nMsgGetObj(vspMsg,key)) == NULL)
	{
		*nValue = 0;
		json_decref(value);
		return FAILUER;
	}
	else
	{
		if(json_is_real(value))
		{
			*nValue = json_real_value(value);
		}
		else
		{
			*nValue = 0.0;
			json_decref(value);
			return FAILUER;
		}
	}
	json_decref(value);
	return SUCCESS;
}

/*====================================================================
 * 函数名    : nMsgGetObj
 * 功能      : 从数据包中根据json关键字，获得json对象
 * 算法实现  : 
 * 参数说明  : vspMsg  数据包
 *				key    json关键字
 * 返回值说明:  成功   json对象
 *				失败   NULL            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/19       v1.0        YLI                          创建
 * ====================================================================*/
json_t *nMsgGetObj(const char *vspMsg,const char *key)
{
	json_t *obj = NULL;
	json_error_t error;
	obj = json_loads(vspMsg,0,&error);
	if (obj == NULL)
	{
		vLogErr("json pack load error!!!");
		return NULL;
	}
	return FindObj(obj,key);
}

/*====================================================================
 * 函数名    : nMsgPackFree
 * 功能      : 释放无用包内存
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/22       v1.0        YLI                          创建
 * ====================================================================*/
void nMsgPackFree(MsgBagInf *pMsgBag)
{
	if(pMsgBag == NULL)
		return;
	if(pMsgBag->msg == NULL)
	{
		free(pMsgBag);
		return;
	}

	free(pMsgBag->msg);
	free(pMsgBag);
}
