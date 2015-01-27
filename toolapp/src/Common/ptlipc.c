/*====================================================================
 * 模块名  : ptlipc
 * 文件名  : ptlipc.c
 * 相关文件:
 * 实现功能: 实现分配内存、初始化环境、加载资源的接口函数
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30    V1.0         YLI                          创建
 * ====================================================================*/

#include "common.h"

//ShmDef *_gstrpShm;
//MsgDef *_gstrpMsg;
//PidDef *_gstrpPid;
 
static key_t _gIpcKey; 
static int   _gShmId;
static int _tCmInit();

/*====================================================================
 * 函数名   :_tSemP
 * 功能    : 对信号量进行减1操作，即申请资源
 * 算法实现 :
 * 参数说明 : vtSemId  信号量的值
 * 返回值说明: 成功   0
 * 			   失败   -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30    v1.0         YLI                           创建
 * ====================================================================*/
int _tSemP(int vtSemId)
{
	struct sembuf   strSemBuf;

	strSemBuf.sem_num = 0;    //操作的信号编号
	strSemBuf.sem_op = -1;    //信号量减1
	strSemBuf.sem_flg = SEM_UNDO;
	if (semop(vtSemId, &strSemBuf, 1) < 0) {
		vLogErr("lock sem failure[%d]!", errno);
		return FAILUER;
	}
	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tSemV
 * 功能    : 对信号量进行加1操作，即释放资源
 * 算法实现 :
 * 参数说明 : vtSemId 信号量的值
 * 返回值说明: 成功  0
 *             失败  -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30     v1.0       YLI                            创建
 * ====================================================================*/
int _tSemV(int vtSemId)
{
	struct sembuf   strSemBuf;

	strSemBuf.sem_num = 0; //信号编号
	strSemBuf.sem_op = 1;  //加1
	strSemBuf.sem_flg = SEM_UNDO;
	if (semop(vtSemId, &strSemBuf, 1) < 0) 
	{
		vLogErr("unlock sem failure[%d]!\n", errno);
		return FAILUER;
	}
	return SUCCESS;
}

static int _tCmInit()
{
	int nKeyTmp;
	memset(&_gsaConfig,0x00,sizeof _gsaConfig);
	if(getenv("A_PROFILE_NAME") == NULL)
	{
		vLogErr("Please set environment value A_PROFILE_NAME!");
		return FAILUER;
	}
	if(strlen(getenv("A_PROFILE_NAME")) >= sizeof(_gsaConfig))
	{
		vLogErr("Environment value A_PROFILE_NAME too long!");
		return FAILUER;
	}
	strcpy(_gsaConfig,getenv("A_PROFILE_NAME"));
	
	if(strlen(_gsaConfig) == 0)
	{
		vLogErr("Profile don't find!");
		return FAILUER;
	}
	if(tPflGetInt(_gsaConfig,K_IPC_KEY,&nKeyTmp) != SUCCESS)
	{
		vLogErr("Please config ipc key!!!");
		return FAILUER;
	}
	_gIpcKey = nKeyTmp;
	_gstrpShm = NULL;

	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tShmInit 
 * 功能    : 分配共享内存
 * 算法实现 :
 * 参数说明 :
 * 返回值说明: 成功      0
 *             失败      -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/31     v1.0        YLI                           创建
 * ====================================================================*/
int _tShmInit()
{
	int tMod; 
	int tDiv;
	if(_tCmInit() != SUCCESS)
	{
		vLogErr("_tCmInit failuer !");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"Initial main shm ...");
	if((_gShmId = shmget(_gIpcKey,sizeof(ShmDef),IPC_CREAT|IPC_EXCL|0660)) == -1)
	{
		if(errno == EEXIST)
		{
			vLogErr("Initial shm invoke shmget fail,this shm already exist!");
		}
		else
		{
			vLogErr("Initial shm invoke shmget fail [%d]!",errno);
		}
		return FAILUER;
	}	
	vLogTrace(EL_INFO,"shmat main shm ...");
	if((_gstrpShm = shmat(_gShmId,0,0)) == (void *)-1)
	{
		vLogErr("Initial shm invoke shmat fail [%d]!",errno);
		return FAILUER;
	}

	ASSERT(_gstrpShm != NULL);
	memset(_gstrpShm,0x00,sizeof(ShmDef));
	vLogTrace(EL_INFO,"Initial shm success!");

	vLogTrace(EL_INFO,"Get config value ...");
	if(tPflGetString(_gsaConfig,K_TCN_KEY,_gstrpShm->rc.saTCN,
		L_PATH_MAX_LEN + L_FILE_NAME_MAX_LEN + 1) != SUCCESS)
	{
		vLogErr("Pealse config testconfigname!!!");
		return FAILUER;
	}	
	vLogTrace(EL_INFO,"TestConfigName[%s]!",_gstrpShm->rc.saTCN);
	
	if(tPflGetInt(_gsaConfig,K_BMN_KEY,&_gstrpShm->rc.nBaseMaxNum) != SUCCESS)
	{
		_gstrpShm->rc.nBaseMaxNum = S_BMN_SIZE;
	}
	vLogTrace(EL_INFO,"BaseMaxNum[%d]!",_gstrpShm->rc.nBaseMaxNum);

	if(tPflGetInt(_gstrpShm->rc.saTCN,K_PHYNUM_KEY,&_gstrpShm->rc.nPhyNum) != SUCCESS)
	{
		_gstrpShm->rc.nPhyNum = 0;
	}
	vLogTrace(EL_INFO,"PhyNum[%d]!",_gstrpShm->rc.nPhyNum);

	if(tPflGetInt(_gstrpShm->rc.saTCN,K_TERMNUM_KEY,&_gstrpShm->rc.nTermNum) != SUCCESS)
	{
		_gstrpShm->rc.nTermNum = 0;
	}
	vLogTrace(EL_INFO,"TermNum[%d]!",_gstrpShm->rc.nTermNum);

	if(tPflGetInt(_gstrpShm->rc.saTCN,K_LOGICNUM_KEY,&_gstrpShm->rc.nLogicNum) != SUCCESS)
	{
		_gstrpShm->rc.nLogicNum = 0;
	}
	vLogTrace(EL_INFO,"LogicNum[%d]!",_gstrpShm->rc.nLogicNum);
	if((_gstrpShm->rc.nPhyNum == 0)	
		&& (_gstrpShm->rc.nTermNum == 0)
		&& (_gstrpShm->rc.nLogicNum == 0))
	{
		vLogErr("phy or Term count must config one!");
		return FAILUER;
	}
	//计算base的个数
	tMod = (_gstrpShm->rc.nPhyNum + _gstrpShm->rc.nTermNum + 
		_gstrpShm->rc.nLogicNum) % (_gstrpShm->rc.nBaseMaxNum);
	tDiv = (_gstrpShm->rc.nPhyNum + _gstrpShm->rc.nTermNum + 
		_gstrpShm->rc.nLogicNum) / (_gstrpShm->rc.nBaseMaxNum);
	_gstrpShm->rc.nBaseCount = (tMod)?(tDiv+1):tDiv;
	vLogTrace(EL_INFO,"BaseCount[%d]!",_gstrpShm->rc.nBaseCount);

	vLogTrace(EL_INFO,"Initial msgtype shm....");	
	if((_gstrpShm->msgInf.nShmId = 
		shmget(IPC_PRIVATE,sizeof(MsgDef) * S_MSGTYPE_SIZE,
			IPC_CREAT|0660)) == -1)
	{
		vLogErr("Initial msgtype shm fail [%d]!",errno);
		return FAILUER;
	}
	if((_gstrpMsg = 
		(MsgDef *)shmat(_gstrpShm->msgInf.nShmId,0,0)) == (void *)-1)
	{
		vLogErr("Initial msgtype shm invoke shmat fail [%d]!",errno);
		return FAILUER;
	}
	ASSERT(_gstrpMsg != NULL);

	vLogTrace(EL_INFO,"Initial base shm....");	
	if((_gstrpShm->pidInf.nShmId =
		shmget(IPC_PRIVATE,sizeof(PidDef) * _gstrpShm->rc.nBaseCount,
			IPC_CREAT|0660)) == -1)
	{
		vLogErr("Initial base shm fail!");
		return FAILUER;
	}
	if((_gstrpPid = 
		(PidDef *)shmat(_gstrpShm->pidInf.nShmId,0,0)) == (void *)-1)
	{
		vLogErr("Initial base shm invoke shmat fail [%d]!",errno);
		return FAILUER;
	}
	ASSERT(_gstrpPid != NULL);	
	vLogTrace(EL_INFO,"Initial all shm filish");	

	_gstrpShm->msgInf.nCount = 0;
	_gstrpShm->pidInf.nCount = _gstrpShm->rc.nBaseCount;
	_gstrpShm->pidInf.nUseCount = 0;
	_gstrpShm->msgInf.nUseCount = 0;
	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tShmRm 
 * 功能    : 移除所有的创建的共享内存，将资源返给系统
 * 算法实现 :
 * 参数说明 :
 * 返回值说明: 成功  0
 * 			   失败  -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/31    v1.0          YLI                         创建
 * ====================================================================*/
int _tShmRm()
{
	ASSERT(_gstrpShm != NULL);
	if(shmctl(_gstrpShm->msgInf.nShmId,IPC_RMID,0) == -1)
	{
		vLogErr("Remove msgtype shm invoke shmctl fail [%d]!",errno);
		return FAILUER;
	}

	if(shmctl(_gstrpShm->pidInf.nShmId,IPC_RMID,0) == -1)
	{
		vLogErr("Remove base shm invoke shmctl fail [%d]!",errno);
		return FAILUER;
	}

	if(shmctl(_gShmId,IPC_RMID,0) == -1)
	{
		vLogErr("Remove main shm invoke shmctl fail [%d]!",errno);
		return FAILUER;
	}
	_gstrpShm = NULL;
	vLogTrace(EL_INFO,"remove all shm success!");
	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tShmAtach
 * 功能    : 重新连接分配好的共享内存
 * 算法实现 :
 * 参数说明 :
 * 返回值说明: 成功   0
 * 			   失败   -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *  2014/12/31   v1.0        YLI                           创建
 * ====================================================================*/
int _tShmAtach()
{
	if(_tCmInit() != SUCCESS)
	{
		vLogErr("Initial _tcminit fail!");
		return FAILUER;
	}
	vLogTrace(EL_INFO,"Atach main shm ....");
	if((_gShmId = shmget(_gIpcKey,sizeof(ShmDef),0660)) == -1)
	{
		vLogErr("Atach main shm invoke shmget faile [%d]!",errno);
		return FAILUER;
	}
	if((_gstrpShm = (ShmDef *)shmat(_gShmId,0,0)) == (void *)-1)
	{
		vLogErr("Atach main shm invoke shmat faile [%d]!",errno);
		return FAILUER;
	}
	vLogTrace(EL_INFO,"Atach main shm success!");

	if((_gstrpMsg = (MsgDef *)shmat(_gstrpShm->msgInf.nShmId,0,0)) == (void *)-1)
	{
		vLogErr("Atach msgtype shm invoke shmat faile [%d]!",errno);
		return FAILUER;

	}
	if((_gstrpPid = (PidDef *)shmat(_gstrpShm->pidInf.nShmId,0,0)) == (void *)-1)
	{
		vLogErr("Atach base shm invoke shmat faile [%d]!",errno);
		return FAILUER;
	}

	ASSERT(_gstrpShm != NULL);
	ASSERT(_gstrpMsg != NULL);
	ASSERT(_gstrpPid != NULL);

	vLogTrace(EL_INFO,"Atach all shm success!");
	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tShmDetach
 * 功能    :  断开共享内存间的连接
 * 算法实现 : 
 * 参数说明 :
 * 返回值说明: 成功     0
 * 			   失败     -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/31    v1.0         YLI                          创建
 * ====================================================================*/
int _tShmDetach()
{
	shmdt((void *)_gstrpMsg);	
	shmdt((void *)_gstrpPid);	
	shmdt((void *)_gstrpShm);	
	_gstrpMsg = NULL;
	_gstrpPid = NULL;
	_gstrpShm = NULL;
	vLogTrace(EL_INFO,"Detach all shm filish!");
	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tSemInit
 * 功能    : 创建相应的信号量
 * 算法实现 :
 * 参数说明 :
 * 返回值说明: 成功   0
 *             失败   -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/31    v1.0         YLI                          创建
 * ====================================================================*/
int _tSemInit()
{
	union semnu
	{
		int val;
		struct semid_ds *buf;
		ushort *array;
	}arg;

	ASSERT(_gstrpShm != NULL);
	ASSERT(_gstrpMsg != NULL);
	ASSERT(_gstrpPid != NULL);

	vLogTrace(EL_INFO,"Initial msg sem ...");
	if((_gstrpShm->rc.tSemMsgId = 
		semget(IPC_PRIVATE,1,IPC_CREAT|IPC_EXCL|0660)) == -1)
	{
		if(errno == EEXIST)
		{
			vLogErr("Initial msgsem invoke semget fail,sem already exist!");
		}
		else
		{
			vLogErr("Initial msgsem invoke semget fail [%d]",errno);
		}
		return FAILUER;
	}
	arg.val = 1;
	if(semctl(_gstrpShm->rc.tSemMsgId,0,SETVAL,arg) == -1)
	{
		vLogErr("Initial msgsem invoke semctl fail [%d]",errno);
		return FAILUER;
	}

	vLogTrace(EL_INFO,"Initial base sem ...");
	if((_gstrpShm->rc.tSemPidId = 
		semget(IPC_PRIVATE,1,IPC_CREAT|IPC_EXCL|0660)) == -1)
	{
		if(errno == EEXIST)
		{
			vLogErr("Initial pidsem invoke semget fail,sem already exist!");
		}
		else
		{
			vLogErr("Initial pidsem invoke semget fail [%d]",errno);
		}
		return FAILUER;
	}
	arg.val = 1;
	if(semctl(_gstrpShm->rc.tSemPidId,0,SETVAL,arg) == -1)
	{
		vLogErr("Initial pidsem invoke semctl fail [%d]",errno);
		return FAILUER;
	}

	vLogTrace(EL_INFO,"Initial boot sem ...");
	if((_gstrpShm->rc.tSemBoot = 
		semget(IPC_PRIVATE,1,IPC_CREAT|IPC_EXCL|0660)) == -1)
	{
		if(errno == EEXIST)
		{
			vLogErr("Initial bootsem invoke semget fail,sem already exist!");
		}
		else
		{
			vLogErr("Initial bootsem invoke semget fail [%d]",errno);
		}
		return FAILUER;
	}
	arg.val = 1;
	if(semctl(_gstrpShm->rc.tSemBoot,0,SETVAL,arg) == -1)
	{
		vLogErr("Initial msgsem invoke semctl fail [%d]",errno);
		return FAILUER;
	}

	vLogTrace(EL_INFO,"Initial all sem filish!");
	return SUCCESS;
}

/*====================================================================
 * 函数名   : _tSemRm
 * 功能    : 移除所有创建的信号量
 * 算法实现 :
 * 参数说明 :
 * 返回值说明: 成功  0 
 *             失败  -1
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/31    v1.0          YLI                        创建
 * ====================================================================*/
int _tSemRm()
{
	ASSERT(_gstrpShm != NULL);
	ASSERT(_gstrpMsg != NULL);
	ASSERT(_gstrpPid != NULL);
	if(semctl(_gstrpShm->rc.tSemMsgId,0,IPC_RMID) == -1)
	{
		vLogErr("Remove msgsem invoke semctl fail [%d]",errno);
		return FAILUER;
	}
	if(semctl(_gstrpShm->rc.tSemPidId,0,IPC_RMID) == -1)
	{
		vLogErr("Remove msgsem invoke semctl fail [%d]",errno);
		return FAILUER;
	}
	if(semctl(_gstrpShm->rc.tSemBoot,0,IPC_RMID) == -1)
	{
		vLogErr("Remove msgsem invoke semctl fail [%d]",errno);
		return FAILUER;
	}

	vLogTrace(EL_INFO,"remove all sem filish!");
	return SUCCESS;
}
