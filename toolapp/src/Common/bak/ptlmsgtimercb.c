#include "ptlmsgtimercb.h"

static void SenMsg(int nIdx);

static void SenMsg(int nIdx)
{
	int res;
	int nPidIdx = 0;
	MsgBagInf *pMsgBag = NULL;
	PVPDLIST head = NULL;
	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS) 
	{
		vLogErr("_tGetPidIdx fail!!!!");
		return;
	}
	if (_gstrpMsg[nIdx].nFlag == F_TERMMSG_FLAG)
	{
		head = _gstrpPid[nPidIdx].strpTerm;
	}
	else if(_gstrpMsg[nIdx].nFlag == F_PHYMSG_FLAG)
	{
		head = _gstrpPid[nPidIdx].strpPhy;
	}
	else if(_gstrpMsg[nIdx].nFlag == F_LOGICMSG_FLAG)
	{
		head = _gstrpPid[nPidIdx].strpLogic;
	}
	else
	{
		head = NULL;
		vLogErr("get term/srv send list fail!!!!");
	}
	vLogTrace(EL_INFO,"send [%d] msg!!!!",_gstrpMsg[nIdx].nFlag);
	//snd msg
	head = head->next;
	while(head)
	{
		pMsgBag = vGenMsg(nIdx,head->vpd);
		if (head->vpd.bev != NULL)
		{
			if (pMsgBag == NULL)
			{
				vLogErr("don't msg send!!!");
				break;
			}
			if ((head->vpd.nConn == F_CONN_ERR) 
				|| (head->vpd.nReg == F_REG_ERR))
			{
				if ((head == NULL) || (head ->next == NULL))
				{
					break;
				}
				head = head->next;
				//vLogNM(EL_ERR,"send.err","connect [%d] isn't read yet!!!!",head->vpd.fd);
				continue;
			}
			res = bufferevent_write(head->vpd.bev,
					pMsgBag->msg,
					pMsgBag->len);
			if( res < 0)
			{
				vLogErr("send error!\n");
			}
			vLogNMS(EL_INFO,"send_trace.log","send msg [%s] len [%d]",
					pMsgBag->msg + 4,pMsgBag->len);
		}
		head = head->next;
	}
	//free msg
	nMsgPackFree(pMsgBag);
}

//HEARTBEAT_REQ_MSG
void MsgTimer00300_cb(evutil_socket_t fd, short events, void *arg)
{
	struct timearg *msg_arg = (struct timearg *)arg;
	int nPidIdx = 0;
	int res = 0;
	int i = 3;
	int count = 0;
	MsgBagInf *pMsgBag = nMsgPack(J_HEARTBEAT_REQ_MSG);
	PVPDLIST head[3] = {NULL,NULL,NULL};
	if (_tGetPidIdx(getpid(),&nPidIdx) != SUCCESS) 
	{
		vLogNM(EL_ERR,"send.err","_tGetPidIdx fail!!!!");
		return;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[2] = _gstrpPid[nPidIdx].strpTerm;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[1] = _gstrpPid[nPidIdx].strpPhy;
	}
	if (_gstrpPid[nPidIdx].strpTerm != NULL)
	{
		head[0] = _gstrpPid[nPidIdx].strpLogic;
	}
	while(i)
	{
		if (head[i-1] == NULL)
		{
			continue;
		}
		head[i-1] = head[i-1]->next;
		while(head[i-1])
		{
			if (head[i-1]->vpd.bev != NULL)
			{
				if (pMsgBag == NULL)
				{
					vLogNM(EL_ERR,"send.err","HEARTBEAT msg is not ready,don't msg send!!!");
					break;
				}
				if ((head[i-1]->vpd.nConn == F_CONN_ERR)
					|| (head[i-1]->vpd.nReg == F_REG_ERR))
				{
					if ((head[i-1] == NULL) || (head[i-1] ->next == NULL))
					{
						break;
					}
					//vLogNM(EL_ERR,"send.err","connect [%d] isn't read yet!!!!",head[i-1]->vpd.fd);
					head[i-1] = head[i-1]->next;	
					continue;
				}
				res = bufferevent_write(head[i-1]->vpd.bev,
					pMsgBag->msg,
					pMsgBag->len);
				if( res < 0)
				{
					vLogNM(EL_ERR,"send.err","send error!");
				}
				head[i-1]->vpd.nHeartFlag = F_HEART_ERR;
				head[i-1]->vpd.nSndHeartNum++;
				vLogNMS(EL_INFO,"send_heart.log","send msg [%s] len [%d]",
					pMsgBag->msg + 4,pMsgBag->len);
				count++;
			}
			head[i-1] = head[i-1]->next;
		}
		i--;
	}
	vLogNMS(EL_INFO,"send_heart.log","send msg [%d]",
					count);
	evtimer_add(msg_arg->timer,&_gstrpMsg[0].tv);
}
void MsgTimer00900_cb(evutil_socket_t fd, short events, void *arg)
{
	FN_EXEC
}
void MsgTimer001500_cb(evutil_socket_t fd, short events, void *arg)
{
	FN_EXEC
}
void MsgTimer001600_cb(evutil_socket_t fd, short events, void *arg)
{
	FN_EXEC
}
void MsgTimer001700_cb(evutil_socket_t fd, short events, void *arg)
{
	FN_EXEC
}
void MsgTimer002100_cb(evutil_socket_t fd, short events, void *arg)
{
	FN_EXEC
}

