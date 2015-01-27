/*====================================================================
 * ģ����  : PTLCtl
 * �ļ���  : PTLCtl.c
 * ����ļ�:
 * ʵ�ֹ���: ���ع���,����event_base
 * ����    : YLI
 * ��Ȩ    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2014/12/19    v1.0        YLI           YLI
 * ====================================================================*/
#include "ptl.h"
#include "ptlctl.h"
//global

//static 
static int Process();
static void RootSigIntCb(evutil_socket_t fd, short events, void *arg);
static void RootSigChldCb(int sig);
static void RootSigCb(evutil_socket_t fd, short events, void *arg);
static void RootTestTimerCb(evutil_socket_t fd, short events, void *arg);
static void RootConnTimerCb(evutil_socket_t fd, short events, void *arg);

/*====================================================================
 * ������    : RootSigCb
 * ����      : ���س�����źŴ���ص�����������fork�µ��ӽ���Ϊ�´����µ�
 *				event_base����
 * �㷨ʵ��  : 
 * ����˵��  : 
 * ����ֵ˵��:              
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/7       v1.0        YLI                          ����
 * ====================================================================*/
static void RootSigCb(evutil_socket_t fd, short events, void *arg)
{
	vLogNM(EL_INFO,"base.log","create a base !!!");
	int genRes;
	genRes = vGenSubPid();
	if (genRes == 0)
	{
		vGenBase();
		exit(-1);
	}
	else 
	{
		return;
	}
}
static void RootSigIntCb(evutil_socket_t fd, short events, void *arg)
{
	vLogNM(EL_DEBUG,"list.log","RootSigIntCb term [%d] phy [%d] logic [%d]:",
		_gstrpPid[0].nTermCount,_gstrpPid[0].nPhyCount,_gstrpPid[0].nLogicCount);
	vSysDone();
	exit(-1);
}
/*====================================================================
 * ������    : RootTimerCb
 * ����      : ���س���Ķ�ʱ���ص���������¼����ʱ��
 * �㷨ʵ��  : 
 * ����˵��  : 
 * ����ֵ˵��:              
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/7       v1.0        YLI                          ����
 * ====================================================================*/
static void RootTestTimerCb(evutil_socket_t fd, short events, void *arg)
{
	event_base_loopbreak(event_get_base(arg));
}
static void RootConnTimerCb(evutil_socket_t fd, short events, void *arg)
{
	struct timeval tv_st = {10,0};
	ShmDef *strpShm = NULL;
	PidDef *strpPid = NULL;
	int nSum = 0;
	int i = 0;
	strpShm = _tGetShmAddr();
	strpPid = _tGetPidAddr();
	if ((strpShm == NULL) 
		|| (strpPid == NULL))
	{
		evtimer_add((struct event *)arg,&tv_st);
		return;
	}
	for (i = 0; i < strpShm->pidInf.nUseCount; ++i)
	{		
		nSum += strpPid[i].nConnCount;
	}
	vLogNMS(EL_INFO,"KeepConnCount.log","ConnCount [%d]",nSum);
	evtimer_add((struct event *)arg,&tv_st);
}
static void RootSigChldCb(int sig)
{
	vLogTrace(EL_INFO,"[%d] child exit!!!",waitpid(0,NULL,0));
}
/*====================================================================
 * ������    : _tStartTest
 * ����      :  ���ߵ����
 * �㷨ʵ��  : 
 * ����˵��  : 
 * ����ֵ˵��: �ɹ�   0
 *             ʧ��   -1             
 * ----------------------------------------------------------------------
 * �޸ļ�¼:
 * ��  ��        �汾        �޸���        �߶���        �޸ļ�¼
 * 2015/1/7      v1.0        YLI                          ����
 * ====================================================================*/
int _tStartTest(int argc,char *argv[])
{
	int res;
	//1 ������ʼ��
	//2 ��Դ��ʼ��
	if (vSysInit() != SUCCESS)
	{
		vLogErr("Initial system error!!!");
		vSysDone();
		return FAILUER;
	}
	//3 ���ܵ���
	res = Process();
	//4 �����������
	//5 ��Դ�黹
	if (res == 1 || res == FAILUER)
	{
		if(vSysDone() != SUCCESS)
		{
			vLogErr("rc remove error !!!");
			return FAILUER;
		}
		vLogTrace(EL_INFO,"Test successed!!!");
	}
	return SUCCESS;
}

static int Process()
{
	struct event_base *ev_root_base = NULL;
	struct event ev_root_sig;
	struct event ev_root_int_sig;
	struct event ev_root_kill_sig;
	struct event ev_root_timer;
	struct event ev_status_timer;   //statistc connect count 
	struct timeval tv_tt = {0,0};
	struct timeval tv_st = {10,0};
	int nGenRes = 0;	
	// generated first sub base
	vLogTrace(EL_INFO,"Genertating first sub process ...");
	nGenRes = vGenFirstSubPid();
	if (nGenRes == 0)
	{
		usleep(2000);
		if(vGenBase() == FAILUER)
		{
			vLogErr("vGenBase error!!!");
			return 0;
		}
		return SUCCESS;
	}
	else if (nGenRes > 0)
	{
		signal(SIGCHLD,RootSigChldCb);
		vLogTrace(EL_INFO,"Create root base ....");
		ev_root_base = event_base_new();
		if (ev_root_base == NULL)
		{
			vLogErr("root base create error!!!");
			return FAILUER;
		}
		if (_tGetTestTime(&(tv_tt.tv_sec)) != SUCCESS)
		{
			vLogErr("Get Test time fail!!!");
			return FAILUER;
		}
		//allocated event
		vLogTrace(EL_INFO,"Allocating root timer and signal event ....");
		if(evtimer_assign(&ev_root_timer,ev_root_base,
			RootTestTimerCb,&ev_root_timer) == -1)
		{
			event_base_free(ev_root_base);
			vLogErr("Allocating Test time timer fail !!!");
			return FAILUER;
		} 
		if (evtimer_assign(&ev_status_timer,ev_root_base,
			RootConnTimerCb,&ev_status_timer) == -1)
		{
			event_base_free(ev_root_base);
			vLogErr("Allocating statistc count timer fail !!!");
			return FAILUER;
		}
		if(evsignal_assign(&ev_root_sig,ev_root_base,S_GENSUBBASE_SIG,
			RootSigCb,NULL) == -1)
		{
			event_base_free(ev_root_base);
			ptlerrno = E_SUBSIG;
			vLogErr("Allocating generate sub base signal fail [%d]!!!",ptlerrno);
			return FAILUER;
		}
		if(evsignal_assign(&ev_root_int_sig,ev_root_base,SIGINT,
			RootSigIntCb,NULL) == -1)
		{
			event_base_free(ev_root_base);
			ptlerrno = E_SUBSIG;
			vLogErr("Allocating int <ctrl + c>signal fail [%d]!!!",ptlerrno);
			return FAILUER;
		}
		
		if(evsignal_assign(&ev_root_kill_sig,ev_root_base,SIGTERM,
			RootSigIntCb,NULL) == -1)
		{
			event_base_free(ev_root_base);
			ptlerrno = E_SUBSIG;
			vLogErr("Allocating int <kill command>signal fail [%d]!!!",ptlerrno);
			return FAILUER;
		}
		//add timer and signal event to base event
		vLogTrace(EL_INFO,"Adding timer and signal event ...");
		if (evtimer_add(&ev_root_timer,&tv_tt) == -1)
		{
			event_base_free(ev_root_base);
			vLogErr("Add Test timer event fail !!!");
			return FAILUER;
		}

		if (evtimer_add(&ev_status_timer,&tv_st) == -1)
		{
			event_base_free(ev_root_base);
			vLogErr("Add statistc timer event fail !!!");
			return FAILUER;
		}

		if (evsignal_add(&ev_root_sig,NULL) == -1)
		{
			event_base_free(ev_root_base);
			ptlerrno = E_SUBSIG;
			vLogErr("add generate sub base signal fail [%d]!!!",ptlerrno);
			return FAILUER;
		}
		if (evsignal_add(&ev_root_int_sig,NULL) == -1)
		{
			event_base_free(ev_root_base);
			ptlerrno = E_SUBSIG;
			vLogErr("add int <ctrl+c> signal fail [%d]!!!",ptlerrno);
			return FAILUER;
		}
		if (evsignal_add(&ev_root_kill_sig,NULL) == -1)
		{
			event_base_free(ev_root_base);
			ptlerrno = E_SUBSIG;
			vLogErr("add int <kill command> signal fail [%d]!!!",ptlerrno);
			return FAILUER;
		}
		//root base event loop
		if (event_base_dispatch(ev_root_base) == -1)
		{
			event_base_free(ev_root_base);
			vLogErr("Root base event loop fail !!!");
			return FAILUER;
		}
		event_base_free(ev_root_base);
		return 1;
	}
	else
	{
		event_base_free(ev_root_base);
		vLogErr("Generate First sub process fail !!!");
		return FAILUER;
	}
	return 1;
}

