/*====================================================================
 * 模块名  : ptlget
 * 文件名  : ptlget.c
 * 相关文件:
 * 实现功能: 获得相关资源提供的所有接口
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *  2015/1/8    v1.0        YLI                          创建
 * ====================================================================*/
#include "ptl.h"
#include "common.h"
int _tGetMsgId(char *vspMsg,int *nId)
{

}
int _tGetMsgIdx(int nMsgId,int *nIdx)
{
	int i = 0;
	for (i = 0; i < _gstrpShm->msgInf.nCount; ++i)
	{
		if(nMsgId == _gstrpMsg[i].nMsgType)
		{
			*nIdx = i;
			return SUCCESS;
		}
	}
	*nIdx = 0;
	return FAILUER;
}
/*====================================================================
 * 函数名    : _tGetTestTime
 * 功能      : 获得测试的时间
 * 算法实现  : 
 * 参数说明  : nTimer  保存获得测试时间的秒数
 * 返回值说明: 成功  0
 *             失败  -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
int _tGetTestTime(long *nTimer)
{
	double hr;
	hr = _gstrpShm->rc.nTestTime;
	if (FLOAT_EQ(hr,0.0) == 1)
	{
		return FAILUER;
	}
	*nTimer = CovertHourToSec(hr);
	return SUCCESS;
}
/*====================================================================
 * 函数名    : _tGetPidIdx
 * 功能      : 根据进程pid查找该pid在内存中的位置
 * 算法实现  : 
 * 参数说明  : pid 要查找的pid
 *             nIdx 找到的位置
 * 返回值说明: 成功 0
 *             失败 -1             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 *        v1.0        YLI                          创建
 * ====================================================================*/
int _tGetPidIdx(const pid_t pid,int *nIdx)
{
	int i = 0;
	for (i = 0; i < _gstrpShm->pidInf.nUseCount; ++i)
	{
		if (_gstrpPid[i].selfPid == pid)
		{
			*nIdx = _gstrpPid[i].nIdx;
			return SUCCESS;
		}
	}
	*nIdx = 0;
	return FAILUER;
}

/*====================================================================
 * 函数名    : _tGetGenTermOrSrvFlag
 * 功能      : 随机产生term/Srv的标志，以便随机产生term/Srv连接
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
int _tGetGenTermOrSrvFlag()
{
	int nTermSumCount = 0;
	int nPhySumCount  = 0;
	int nLogicSumCount  = 0;
	int i = 0;
	int n = 0;
	for (i = 0; i < _gstrpShm->pidInf.nUseCount; ++i)
	{
		nTermSumCount += _gstrpPid[i].nTermCount;
		nPhySumCount += _gstrpPid[i].nPhyCount;
		nLogicSumCount += _gstrpPid[i].nLogicCount;
	}
	//gen phy srv
	if ((nTermSumCount >= _gstrpShm->rc.nTermNum) 
		&& (nPhySumCount < _gstrpShm->rc.nPhyNum)
		&& (nLogicSumCount >= _gstrpShm->rc.nLogicNum))
	{
		return F_PHY_FLAG;
	}
	//gen term
	if ((nPhySumCount >= _gstrpShm->rc.nPhyNum) 
		&& (nTermSumCount < _gstrpShm->rc.nTermNum)
		&& (nLogicSumCount >= _gstrpShm->rc.nLogicNum))
	{
		return F_TERM_FLAG;
	}
	//gen logic srv
	if ((nPhySumCount >= _gstrpShm->rc.nPhyNum) 
		&& (nTermSumCount >= _gstrpShm->rc.nTermNum)
		&& (nLogicSumCount < _gstrpShm->rc.nLogicNum))
	{
		return F_LOGIC_FLAG;
	}

	if ((nTermSumCount >= _gstrpShm->rc.nTermNum) 
		&& (nPhySumCount >= _gstrpShm->rc.nPhyNum)
		&& (nLogicSumCount >= _gstrpShm->rc.nLogicNum))
	{
		return F_NULL_FLAG;
	}
	
	//rand generate
	srand((unsigned)time(NULL));
	n = rand()%3 + 1;
	if (n == 1)
	{
		return F_PHY_FLAG;
	}
	else if(n == 2)
		return F_LOGIC_FLAG;
	else 
		return F_TERM_FLAG;
}

/*====================================================================
 * 函数名    : GetCpuInfo
 * 功能      : 获得当前Cpu的一些信息，保存到json数据中
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:  成功 当前的cpu信息
 *				失败  NULL            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/13       v1.0        YLI                          创建
 * ====================================================================*/
json_t *GetCpuInfo()
{
	FILE *fp;
	json_t *cpuinfo;
	char buf[1024];
	char tmp[1024];
	int count = 0;
	char c;
	int i = 0;
	memset(buf,0x00,1024);
	cpuinfo = json_object();

	fp = fopen("/proc/cpuinfo","r");
	if (fp == NULL)
	{
		vLogErr("open file /proc/cpuinfo fail");
		return NULL;
	}

	while((c = fgetc(fp)) != EOF)
	{
		switch(c)
		{
			case '\r':
			case '\n':
				Trim(buf);
				if(strncmp(buf,"cpuMHz",6) == 0)
				{
					memset(tmp,0x00,1024);
					strcpy(tmp,buf+6);
					Trim(tmp);
					json_object_set(cpuinfo,"cpuMHz",json_real(atoi(tmp)));
					count++;
				}
				else if(strncmp(buf,"modelname",9) == 0)
				{
					memset(tmp,0x00,1024);
					strcpy(tmp,buf+9);
					Trim(tmp);
					json_object_set(cpuinfo,"modelname",json_string(tmp));
					count++;
				}
				i = 0;
				memset(buf,0x00,1024);
				break;
			case ' ':
			case '\t':
			case ':':
				break;
			default:
				buf[i++] = c;
				break;
 		}
 		if (count == 2)
 		{
 			break;
 		}
	}
	fclose(fp);
	fp = NULL;
	if (json_object_size(cpuinfo) == 0)
	{
		return NULL;
	}
	return cpuinfo;
}
json_t *GetMemInfo()
{
	FILE *fp;
	json_t *Meminfo;
	char buf[1024];
	char tmp[1024];
	int count = 0;
	char c;
	int i = 0;
	memset(buf,0x00,1024);
	Meminfo = json_object();

	fp = fopen("/proc/meminfo","r");
	if (fp == NULL)
	{
		vLogErr("open file /proc/meminfo fail");
		return NULL;
	}

	while((c = fgetc(fp)) != EOF)
	{
		switch(c)
		{
			case '\r':
			case '\n':
				Trim(buf);
				if(strncmp(buf,"MemTotal",8) == 0)
				{
					memset(tmp,0x00,1024);
					strncpy(tmp,buf+8,strlen(buf)-7);
					Trim(tmp);
					json_object_set(Meminfo,"MemTotal",json_integer(atoi(tmp)/1024));
					count++;
				}
				i = 0;
				memset(buf,0x00,1024);
				break;
			case ' ':
			case '\t':
			case ':':
				break;
			default:
				buf[i++] = c;
				break;
 		}
 		if (count == 1)
 		{
 			break;
 		}
	}
	fclose(fp);
	fp = NULL;
	if (json_object_size(Meminfo) == 0)
	{
		return NULL;
	}
	return Meminfo;
}
void _tPrintList(int nIdx)
{
	PVPDLIST head[3];
	int i = 0;
	if ((_gstrpPid[nIdx].strpTerm == NULL)
		|| (_gstrpPid[nIdx].strpPhy == NULL)
		|| (_gstrpPid[nIdx].strpLogic == NULL))
	{
		return;
	}
	head[0] = _gstrpPid[nIdx].strpTerm->next;
	head[1] = _gstrpPid[nIdx].strpPhy->next;
	head[2] = _gstrpPid[nIdx].strpLogic->next;
	while(i < 3)
	{
		if (i == 0)
		{
			vLogNM(EL_DEBUG,"list.log","Term list  [%p]:",head[0]);
		}
		else if (i == 1)
		{
			vLogNM(EL_DEBUG,"list.log","Phy srv list  [%p]:",head[1]);
		}
		else
			vLogNM(EL_DEBUG,"list.log","Logic srv list  [%p]:",head[2]);
		while(head[i])
		{
			vLogNM(EL_DEBUG,"list.log","devid [%s] devtype [%s]",head[i]->vpd.saDevId,
				head[i]->vpd.saDevType);
			head[i] = head[i]->next;
		}
		i++;
	}
}
/*====================================================================
 * 函数名    : _get_code_error_to_string
 * 功能      : 获得错误代码的字符串描述
 * 算法实现  : 
 * 参数说明  : errcode  错误代码
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/20       v1.0        YLI                          创建
 * ====================================================================*/
const char *_get_code_error_to_string(int errcode)
{
	int i = 0;
	for (i = 0; _reg_code_errors[i].code > 0; ++i)
	{
		if (errcode == _reg_code_errors[i].code)
		{
			return _reg_code_errors[i].msg;
		}
	}
	return strerror(errcode);
}
/*====================================================================
 * 函数名    : _tGetShmAddr
 * 功能      : 获得共享内存的入口地址
 * 算法实现  : 
 * 参数说明  : 
 * 返回值说明:              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2014/12/30       v1.0        YLI                          创建
 * ====================================================================*/
ShmDef *_tGetShmAddr()
{
	return _gstrpShm;
}
PidDef *_tGetPidAddr()
{
	return _gstrpPid;
}