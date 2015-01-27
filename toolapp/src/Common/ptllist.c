/*====================================================================
 * ģ����  : ptllist
 * �ļ���  : ptllist.c
 * ����ļ�:
 * ʵ�ֹ���: �������ز���
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/4       v1.0        YLI                          ����
 * ====================================================================*/
#include "common.h"

static PVPDLIST GetHead(VPD vpd);

/*====================================================================
 * ������   : CreateNode 
 * ����    :  ����һ������Ľ��,�������ն˻����������Ϣ
 * �㷨ʵ�� :
 * ����˵�� :  vpd   �������ݲ���
 * ����ֵ˵��: �ɹ�  �����ɹ��Ľ��
 *             ʧ��  NULL
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/4      v1.0         YLI                           ����
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
 * ������   : InsertNode
 * ����    :  ���ն˻����������в���һ�����
 * �㷨ʵ�� : ʹ�õ�ͷ�巨
 * ����˵�� : vpd   ����Ľ����Ϣ
 * ����ֵ˵��: �ɹ�    0
 *             ʧ��    -1 
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/14       v1.0        YLI                          ����
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
 * ������   : GetNode
 * ����    :  ���һ������������е�λ��
 * �㷨ʵ�� :
 * ����˵�� : fd  Ҫ��õĽ��
 *            nPre ��ʶ�Ƿ��ǻ�øý��ǰһ����� 0��õ�ǰ��㣬1���ǰһ�����
 * ����ֵ˵��: �ɹ�  �ý���Ӧ�ĵ�ַ
 * 			   ʧ��  NULL
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/4      v1.0         YLI                         ����
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
 * ������   : DelNode
 * ����    :  ɾ��һ�����
 * �㷨ʵ�� :
 * ����˵�� : vpd  Ҫɾ���Ľ��
 * ����ֵ˵��: �ɹ�  0
 *             ʧ��  -1
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/4      v1.0         YLI                         ����
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
 * ������   : DestroyList
 * ����    :  ����һ������
 * �㷨ʵ�� :
 * ����˵�� : head  Ҫ���������ͷ���
 * ����ֵ˵��: ��
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/4      v1.0        YLI                          ����
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
 * ������   : GetHead
 * ����    :  ��õ�ǰ���������ͷ���
 * �㷨ʵ�� :
 * ����˵�� : vpd   һ�˽�����ݲ���
 * ����ֵ˵��: �ɹ�   ��ǰͷ���
 *             ʧ��    NULL
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/4       v1.0         YLI                        ����
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

