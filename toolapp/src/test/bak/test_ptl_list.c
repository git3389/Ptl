#include "testunit.h"
#include "common.h"
static void print(PVPDLIST head);

void test_ptl_list(void)
{
	VPD vpd;
	memset(&vpd,0x00,sizeof vpd);
	//create
	printf("create two list\n");
	_gstrpTermList = CreateNode(vpd);
	_gstrpPhyList = CreateNode(vpd);
	printf("insert node\n");	
	//insert node
	int i = 0;
	//term  ten
	for(i = 0;i < 10; i++)
	{
		vpd.fd = i;
		vpd.nFlag = F_TERM_FLAG;
		InsertNode(vpd);
	}
	//mcu  ten
	for(i = 0;i < 10; i++)
	{
		vpd.fd = i;
		vpd.nFlag = F_PHY_FLAG;
		InsertNode(vpd);
	}
		
	//print all
	printf("print\n");
	printf("print term\n");
	print(_gstrpTermList);
	printf("print phy\n");
	print(_gstrpPhyList);

	memset(&vpd,0x00,sizeof vpd);
	//del node
	printf("del node\n");
	//del term
	vpd.fd = 2;
	//don't set flag
    //vpd.nFlag = F_TERM_FLAG;
    vpd.nFlag = F_TERM_FLAG;
	DelNode(vpd);

	//del mcu
	vpd.fd = 2;
	//don't set flag
	//vpd.nFlag = F_TERM_FLAG;
	vpd.nFlag = F_PHY_FLAG;
	DelNode(vpd);
	printf("print\n");
	printf("print term\n");
	print(_gstrpTermList);
	printf("print phy\n");
	print(_gstrpPhyList);

	//get node
	PVPDLIST tmp;
	printf("get node\n");
	memset(&vpd,0x00,sizeof vpd);
	vpd.fd = 3;
	vpd.nFlag = F_TERM_FLAG;
	tmp = GetNode(vpd,F_CURR_FLAG);
	if(tmp != NULL)
		printf("id:%d\tflag:%d\n",tmp->vpd.fd,tmp->vpd.nFlag);
	//destroy
	printf("destroy list\n");
	DestroyList(&_gstrpTermList);
	DestroyList(&_gstrpPhyList);
}

static void print(PVPDLIST head)
{
	head = head->next;
	while(head)	
	{
		printf("id:%4d\t",head->vpd.fd);
		head = head->next;
	}
	printf("\n");
}

