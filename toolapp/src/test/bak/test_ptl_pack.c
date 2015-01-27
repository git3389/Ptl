#include "testunit.h"
#include "common.h"
void test_ptl_pack(void)
{
	/*	
	int i = 100;
	int b = htonl(i);
	printf("htonl:%u\n",b);
	printf("%d\n", sizeof b);
	printf("ntohl:%u\n", ntohl(b));
	char str[100];
	memset(str,0x00,sizeof str);
	memcpy(str,&b,sizeof b);
	//sprintf(str,"%4diiikke",b);
	printf("str:%s\n",str);
	int tmp;
	memset(&tmp,0x00,sizeof tmp);
	memcpy(&tmp,str,4);
	printf("tmp:%d\n",tmp);
	printf("htohl:%u\n",ntohl(tmp));
	int j = 0;
	for (j = 0; j < 4; j++)
	{
		printf("%x ", str[j]);
	}	
	printf("\n");*/
	
	MsgBagInf* msg, *msg2;
	msg = nMsgPack("test1333");
	msg2 = nMsgPack("test2");
	printf("len:%d\n",msg->len);
	int len;
	memcpy(&len,msg->msg,4);
	printf("msglen:%d\n",len);
	printf("msglenntohl:%u\n",ntohl(len));

	printf("msg:\n");
	char *c = msg->msg;
	int i = 0;
	for(i = 0;i<msg->len;i++)
	{
		printf("%x\n",(char)*(c+i));
	}
	printf("\n");
	printf("msg1:%s\n",msg->msg+4);
	printf("msg2:%s\n",msg2->msg+4);
	nMsgPackFree(msg);
	nMsgPackFree(msg2);
}

void test_ptl_unpack(void)
{
	char *text = "{\"name\":\"Consloas\",\"format\":{\"type\":\"rect\",\"width\":1024,\"height\":768.7,\"interlace\":false,\"frame\":24}}";
	//char *text = "{\"name\":\"Consloas\",\"type\":\"rect\",\"width\":1024,\"height\":768.3,\"interlace\":false,\"frame\":24}";
	char buf[100];
	int i = 0;
	double d = 0.0; 
	json_t *obj;
	memset(buf,0x00,100);
	//get string
	if(nMsgGetString(text,"type",buf,100) != SUCCESS)
	{
		printf("Get string error\n");
	}
	else
	{
		printf("nMsgGetString: %s\n",buf);
	} 
	//get int
	if(nMsgGetInt(text,"width",&i) != SUCCESS)
	{
		printf("Get integer error\n");
	}
	else
	{
		printf("nMsgGetInt: %d\n",i);
	} 
	//get real
	if(nMsgGetReal(text,"height",&d) != SUCCESS)
	{
		printf("Get real error\n");
	}
	else
	{
		printf("nMsgGetReal: %lf\n",d);
	} 
	
	//get object
	if((obj = nMsgGetObj(text,"format")) == NULL)
	{
		printf("Get object error\n");
	}
	else
	{
		if (json_is_object(obj))
		{
			printf("nMsgGetObj: %s\n",json_dumps(obj,0));
		}
		printf("addr:%p\n",obj);
	} 
	return ;
}
