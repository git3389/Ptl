/*====================================================================
 * 模块名  : ptllist
 * 文件名  : ptllist.c
 * 相关文件:
 * 实现功能: 链表的相关操作
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/4       v1.0        YLI                          创建
 * ====================================================================*/
#include "common.h"

static PVPDLIST GetHead(VPD vpd);

/*====================================================================
 * 函数名   : CreateNode 
 * 功能    :  创建一个链表的结点,结点代表终端或服务的相关信息
 * 算法实现 :
 * 参数说明 :  vpd   结点的数据部分
 * 返回值说明: 成功  创建成功的结点
 *             失败  NULL
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/4      v1.0         YLI                           创建
 * ====================================================================*/
PVPDLIST CreateNode(VPD vpd)
{
	PVPDLIST node = (PVPDLIST)malloc(sizeof(VPDLIST));
	if(NULL == node)
	{
		vLogErr("Initial create a node error!!");
		return NULL;
	}
	node->vpd = vpd;
	node->next = NULL;
	return node;
}

/*====================================================================
 * 函数名   : InsertNode
 * 功能    :  向终端或服务的链表中插入一个结点
 * 算法实现 : 使用的头插法
 * 参数说明 : vpd   插入的结点信息
 * 返回值说明: 成功    0
 *             失败    -1 
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/14       v1.0        YLI                          创建
 * ====================================================================*/
int InsertNode(VPD vpd)
{
	PVPDLIST newNode = CreateNode(vpd);
	PVPDLIST head = NULL;
	if(newNode == NULL)
	{
		vLogErr("creat vpd error!");
		return FAILUER;
	}
	if((head = GetHead(vpd)) == NULL)
	{
		vLogErr("List head error!");
		return FAILUER;
	}	
	newNode->next = head->next;
	head->next = newNode;
	return SUCCESS;
}

/*====================================================================
 * 函数名   : GetNode
 * 功能    :  获得一个结点在链表中的位置
 * 算法实现 :
 * 参数说明 : fd  要获得的结点
 *            nPre 标识是否是获得该结点前一个结点 0获得当前结点，1获得前一个结点
 * 返回值说明: 成功  该结点对应的地址
 * 			   失败  NULL
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/4      v1.0         YLI                         创建
 * ====================================================================*/
PVPDLIST GetNode(VPD vpd,int nPre)
{
	PVPDLIST head = NULL;
	PVPDLIST pre; 
	if((head = GetHead(vpd)) == NULL)
	{
		vLogErr("List head error!");
		return NULL;
	}	
	pre = head;
	head = head->next;
	while(head)
	{
		if(head->vpd.fd == vpd.fd)
		{
			if(nPre == F_PRE_FLAG)
			{
				return pre;
			}
			else
			{
				return head;
			}
		}
		pre = head;
		head = head->next;
	}	
	pre = NULL;
	return NULL;	
}

/*====================================================================
 * 函数名   : DelNode
 * 功能    :  删除一个结点
 * 算法实现 :
 * 参数说明 : vpd  要删除的结点
 * 返回值说明: 成功  0
 *             失败  -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/4      v1.0         YLI                         创建
 * ====================================================================*/
int DelNode(VPD vpd)
{
	PVPDLIST curr = GetNode(vpd,F_CURR_FLAG);
	PVPDLIST pre = GetNode(vpd,F_PRE_FLAG);
	if(curr == NULL)
	{
		return FAILUER;
	}	
	if(pre == NULL)
	{
		return FAILUER;
	}
	//del
	pre->next = curr->next;
	curr->next = NULL;
	free(curr);
	curr = NULL;
	pre = NULL;
	vLogTrace(EL_INFO,"Del a vpd!");
	return SUCCESS;	
}

/*====================================================================
 * 函数名   : DestroyList
 * 功能    :  销毁一条链表
 * 算法实现 :
 * 参数说明 : head  要销毁链表的头结点
 * 返回值说明: 无
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/4      v1.0        YLI                          创建
 * ====================================================================*/
void DestroyList(PVPDLIST *head)
{
	PVPDLIST strpCurr,strpNext;

	if((*head) == NULL)
	{
		vLogErr("list is null!!!!");
		return;
	}
	strpCurr = (*head)->next;
	while(strpCurr)
	{ 
		strpNext = strpCurr->next;
		free(strpCurr);
		strpCurr = strpNext;	
	}
	(*head)->next = NULL;
	free(*head);
	*head = NULL;	
}

/*====================================================================
 * 函数名   : GetHead
 * 功能    :  获得当前操作链表的头结点
 * 算法实现 :
 * 参数说明 : vpd   一人结点数据部分
 * 返回值说明: 成功   当前头结点
 *             失败    NULL
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/4       v1.0         YLI                        创建
 * ====================================================================*/
static PVPDLIST GetHead(VPD vpd)
{
	PVPDLIST head = NULL;
	if(vpd.nFlag == F_NULL_FLAG || vpd.nFlag == 0)
	{
		vLogErr("the vpd is not exist!");
		return NULL;
	}

	if((vpd.nFlag == F_TERM_FLAG) && (_gstrpTermList == NULL))
	{
		vLogErr("Term list not create!!!");
		return NULL;
	}
		
	if((vpd.nFlag == F_PHY_FLAG) && (_gstrpPhyList == NULL))
	{
		vLogErr("Phy list not create!!!");
		return NULL;
	}
	if((vpd.nFlag == F_LOGIC_FLAG) && (_gstrpLogicList == NULL))
	{
		vLogErr("Logic list not create!!!");
		return NULL;
	}	
	if(vpd.nFlag == F_TERM_FLAG)
	{
		head = _gstrpTermList;
	}
	else if (vpd.nFlag == F_PHY_FLAG)
	{
		head = _gstrpPhyList;
	}
	else
	{
		head = _gstrpLogicList;
	}
	return head;
}

