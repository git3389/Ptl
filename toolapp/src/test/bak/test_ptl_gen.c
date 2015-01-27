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
	VPD vpd;
	MsgBagInf *pMsgBag = NULL;
	memset(&vpd,0x00,sizeof vpd);
	//term vpd
	strcpy(vpd.saDevId,"1.1.23");
	strcpy(vpd.saDevType,"server");
	pMsgBag = vGenMsg(2,vpd);
	if (pMsgBag != NULL)
	{
		printf("EV_PFMINFO_MSG msg:%s len[%d]\n",pMsgBag->msg + 4,pMsgBag->len);
	}
	nMsgPackFree(pMsgBag);

	//cpu
	memset(&vpd,0x00,sizeof vpd);
	strcpy(vpd.saDevId,"1.1.23");
	strcpy(vpd.saDevType,"server");
	pMsgBag = vGenMsg(3,vpd);
	if (pMsgBag != NULL)
	{
		printf("EV_PFMINFO_CPU msg:%s len[%d]\n",pMsgBag->msg + 4,pMsgBag->len);
	}
	nMsgPackFree(pMsgBag);

	//mem
	memset(&vpd,0x00,sizeof vpd);
	//term vpd
	strcpy(vpd.saDevId,"1.1.23");
	strcpy(vpd.saDevType,"server");
	pMsgBag = vGenMsg(4,vpd);
	if (pMsgBag != NULL)
	{
		printf("EV_PFMINFO_MEM msg:%s len[%d]\n",pMsgBag->msg + 4,pMsgBag->len);
	}
	nMsgPackFree(pMsgBag);

	vSysDone();
}

