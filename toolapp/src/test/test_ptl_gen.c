#include "testunit.h"
#include "common.h"

void test_genbase(void)
{
 	if(vSysInit() != SUCCESS)
	{
		vSysDone();
	}
	vGenBase();
	vSysDone();
}

void test_gen_reg_msg()
{
	if(vSysInit() != SUCCESS)
	{
		vSysDone();
		return;
	}
	printf("term reg msg:%s\n",json_dumps(vGenRegTermMsg(0),0));
	printf("phy reg msg:%s\n",json_dumps(vGenRegSrvMsg(F_PHY_FLAG,0),0));	
	_gstrpPid[0].nPhyCount = 4;
	printf("logic reg msg:%s\n",json_dumps(vGenRegSrvMsg(F_LOGIC_FLAG,0),0));
	vSysDone();
}

void test_gen_msg()
{
	if(vSysInit() != SUCCESS)
	{
		vSysDone();
		return;
	}
	int i = 0;
	VPD vpd;
	MsgBagInf *pMsgBag = NULL;
	for (i = 0; i < 4; ++i)
	{
		memset(&vpd,0x00,sizeof vpd);
		//term vpd
		strcpy(vpd.saDevId,"1.1.23");
		strcpy(vpd.saDevType,"server");
		pMsgBag = vGenMsg(i+1,vpd);
		if (pMsgBag != NULL)
		{
			printf("msg:%s len[%d]\n",pMsgBag->msg + 4,pMsgBag->len);
		}
		nMsgPackFree(pMsgBag);
	}
	vSysDone();
}

void test_gen_msg1(void)
{
	if(vSysInit() != SUCCESS)
	{
		vSysDone();
		return;
	}
	int i = 0;
	VPD vpd;
	json_t *j_msg = NULL;
	memset(&vpd,0x00,sizeof vpd);
	//term vpd
	strcpy(vpd.saDevId,"1.1.23");
	strcpy(vpd.saDevType,"server");
	for (i = 0; _ev_msg_cb_map[i].eventid != NULL; ++i)
	{
		j_msg = (json_t *)(* _ev_msg_cb_map[i].msgcb)(vpd);
		printf("%d: %s\n\n",i,json_dumps(j_msg,0));
	}
	vSysDone();
}
