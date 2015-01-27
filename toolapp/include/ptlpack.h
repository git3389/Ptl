#ifndef _PTLPACK_H
#define _PTLPACK_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "jansson.h"

#define MSG_MAX_LEN 1024

//消息数据定义
typedef struct tagMsgBagDef
{
	int len;                     //消息长度
	char *msg;     				 //消息内容
}MsgBagDef;

typedef struct tagMsgBagInf
{
	int len;                //发送消息数据的长度
	char *msg;          //发送消息的内容
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
