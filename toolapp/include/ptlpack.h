#ifndef _PTLPACK_H
#define _PTLPACK_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "jansson.h"

#define MSG_MAX_LEN 1024

//��Ϣ���ݶ���
typedef struct tagMsgBagDef
{
	int len;                     //��Ϣ����
	char *msg;     				 //��Ϣ����
}MsgBagDef;

typedef struct tagMsgBagInf
{
	int len;                //������Ϣ���ݵĳ���
	char *msg;          //������Ϣ������
}MsgBagInf;

//pack
extern MsgBagInf* nMsgPack(char *vspMsg);

//unpack
extern int nMsgGetString(const char *vspMsg,const char *key,char *vspValue,size_t len);
extern int nMsgGetInt(const char *vspMsg,const char *key,int *nValue);
extern int nMsgGetReal(const char *vspMsg,const char *key,double *nValue);
extern json_t *nMsgGetObj(const char *vspMsg,const char *key);

//free pack
extern void nMsgPackFree(MsgBagInf *pMsgBag);

#endif
