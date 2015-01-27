/*====================================================================
 * ģ����  : ptlpack
 * �ļ���  : ptlpack.c
 * ����ļ�:
 * ʵ�ֹ���: �ṩ��Ϣ�Ĵ�����������ӿڣ��Լ������ͷ�
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/22   v1.0        YLI                          ����
 * ====================================================================*/

#include "ptlpack.h"
#include "common.h"


static MsgBagInf* nMsgBagInit(char *vspMsg);
static json_t *FindObj(json_t *root,const char *vspKey);

/*====================================================================
 * ������    : FindObj
 * ����      : �ݹ����һ��json�����в�������������json����
 * �㷨ʵ��  : 
 * ����˵��  : root   Ҫ���ҵ�json������
 *				vspKey   Ҫ���ҵ�json�ؼ���
 * ����ֵ˵��:  �ɹ�  �ҵ���json����
 *				ʧ��  NULL            
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/19       v1.0        YLI                          ����
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
 * ������    : nMsgPack
 * ����      : ����jsonΪ���ݽṹ�����ݴ���ɰ�Э�鶨��ķ��͵���Ϣ�ṹ���� 
 * �㷨ʵ��  : 
 * ����˵��  : vspMsg Ҫ�������Ϣ
 * ����ֵ˵��: �ɹ�   ����õ���Ϣ�ṹ����
 *			   ʧ��   NULL             
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/22       v1.0        YLI                          ����
 * ====================================================================*/
MsgBagInf *nMsgPack(char *vspMsg)
{
	int msgLen = 0;	
	int nMsgLen = 0;	
	MsgBagInf *pMsgBag = nMsgBagInit(vspMsg); 
	if(pMsgBag == NULL)
	{
		vLogErr("�����ʼ������!\n");
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
 * ������    : nMsgBagInit
 * ����      : ���䱣�������ݵ��ڴ�
 * �㷨ʵ��  : 
 * ����˵��  : 
 * ����ֵ˵��:              
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/22       v1.0        YLI                          ����
 * ====================================================================*/
MsgBagInf* nMsgBagInit(char *vspMsg)
{
	MsgBagInf *strMsgBag;

	strMsgBag = (MsgBagInf *)malloc(sizeof(MsgBagInf));	
	if(strMsgBag == NULL)
	{
		vLogErr("�ڴ����ʧ��!\n");
		return NULL;
	}
 
	strMsgBag->len = 0;
	strMsgBag->msg = (char *)malloc(strlen(vspMsg) + 4 + 1);
	if(strMsgBag->msg == NULL)
	{
		vLogErr("�ڴ����ʧ��!\n");
		return NULL;
	}
	memset(strMsgBag->msg,0x00,strlen(vspMsg) + 4 + 1);
	return strMsgBag;
}

/*====================================================================
 * ������    : nMsgGetString
 * ����      : �����ݰ��и���json�ؼ��֣�����ַ���
 * �㷨ʵ��  : 
 * ����˵��  : vspMsg  ���ݰ�
 *				key    json�ؼ���
 *				vspValue ��õ��ַ���
 *				len      ��õ��ַ�������Ļ�������С
 * ����ֵ˵��:  �ɹ�   0
 *				ʧ��   -1            
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/19       v1.0        YLI                          ����
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
 * ������    : nMsgGetInt
 * ����      : �����ݰ��и���json�ؼ��֣��������
 * �㷨ʵ��  : 
 * ����˵��  : vspMsg  ���ݰ�
 *				key    json�ؼ���
 *				nValue ��õ�����
 * ����ֵ˵��:  �ɹ�   0
 *				ʧ��   -1            
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/19       v1.0        YLI                          ����
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
 * ������    : nMsgGetReal
 * ����      : �����ݰ��и���json�ؼ��֣����ʵ��
 * �㷨ʵ��  : 
 * ����˵��  : vspMsg  ���ݰ�
 *				key    json�ؼ���
 *				nValue ��õ�ʵ��
 * ����ֵ˵��:  �ɹ�   0
 *				ʧ��   -1            
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/19       v1.0        YLI                          ����
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
 * ������    : nMsgGetObj
 * ����      : �����ݰ��и���json�ؼ��֣����json����
 * �㷨ʵ��  : 
 * ����˵��  : vspMsg  ���ݰ�
 *				key    json�ؼ���
 * ����ֵ˵��:  �ɹ�   json����
 *				ʧ��   NULL            
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/19       v1.0        YLI                          ����
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
 * ������    : nMsgPackFree
 * ����      : �ͷ����ð��ڴ�
 * �㷨ʵ��  : 
 * ����˵��  : 
 * ����ֵ˵��:              
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/22       v1.0        YLI                          ����
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
