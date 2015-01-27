/*====================================================================
 * 模块名  : ptlgenmsg
 * 文件名  : ptlgenmsg.c
 * 相关文件:
 * 实现功能: 生成所有定义好的消息内容
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/21  v1.0        YLI                          创建
 * ====================================================================*/
#include "common.h"

/*====================================================================
 * 函数名    : vGenRegTermMsg
 * 功能      : 生成终端注册消息
 * 算法实现  : 
 * 参数说明  : 
 *				nIdx 标识是哪个的base
 * 返回值说明: 成功 生成的json字符串
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/13       v1.0        YLI                          创建
 * ====================================================================*/
json_t *vGenRegTermMsg(int nIdx)
{
	json_t *root;
	json_t *mt_info;
	json_t *netinfo;
	json_t *aps_addr;
	json_t *jt;
	const char *key;
	json_t *value;
	int n,size;
	char saCfgPath[L_PATH_MAX_LEN];
	char tmp[100];
	root = json_object();
	memset(tmp,0x00,sizeof tmp);
	memset(saCfgPath,0x00,sizeof saCfgPath);
	
	srand((unsigned)time(NULL));

	//set eventid
	if (json_object_set(root,"eventid",
		json_string("EV_REG_REQ")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	n = rand() % (S_RAND_RANG + _gstrpPid[nIdx].nTermCount);
	sprintf(tmp,"1.2.%02d",n);
	if (json_object_set(root,"devid",
		json_string(tmp)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if(getenv("APP_PROFILE_PATH") == NULL)
	{
		json_decref(root);
		vLogErr("devtypes config file path error,please check evn value!!!");
		return NULL;
	}
	strcpy(saCfgPath,getenv("APP_PROFILE_PATH"));
	strcat(saCfgPath,"/devtypes.json");
	jt = tPflGetJsonObj(saCfgPath,K_DEVTYPE_KEY);
	if (jt == NULL)
	{
		json_decref(root);
		vLogErr("devtypes file open error,please check devtypes.json is exist!!!");
		return NULL;
	}
	
	
	if (json_is_array(jt))
	{
		size = json_array_size(jt);
		n = rand()%size;
		if (json_object_set(root,"devtype",
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
	}

	/*mt_info*/
	mt_info = json_object();
	//devver
	if (json_object_set(mt_info,"devver",
		json_string("123")) == FAILUER)
	{
		json_decref(root);
		vLogErr("devver set error!!!");
		return NULL;
	}
	//devname
	json_object_set(mt_info,"devname",json_string("truelink"));
	//netinfo
	netinfo = json_object();
	json_object_set(netinfo,"ip",
		json_string(_gstrpShm->rc.nSrvIP));
	json_object_set(netinfo,"dns",
		json_string("172.16.0.65"));
	json_object_set(mt_info,"netinfo",netinfo);

	//aps_addr
	aps_addr = json_object();
	json_object_set(aps_addr,"domain",
		json_string("fdaf"));
	json_object_set(aps_addr,"ip",
		json_string(_gstrpShm->rc.nSrvIP));
	json_object_set(mt_info,"aps_addr",aps_addr);

	//oem
	json_object_set(mt_info,"oem",json_string("dfd"));
	//os
	json_object_set(mt_info,"os",json_string("Centos 6.4"));
	//cpu_num
	json_object_set(mt_info,"cpu_num",json_integer(4));
	//cpu_type
	//cpu_freq
	json_decref(jt);
	jt = GetCpuInfo();
	json_object_foreach(jt,key,value)
	{
		if(strncmp(key,"cpuMHz",6) == 0)
		{
			json_object_set(mt_info,"cpu_freq",value);
		}
		if (strncmp(key,"modelname",9) == 0)
		{
			json_object_set(mt_info,"cpu_type",value);
		}
	}
	json_decref(jt);
	jt = GetMemInfo();
	//memory
	json_object_foreach(jt,key,value)
	{
		if(strncmp(key,"MemTotal",8) == 0)
		{
			json_object_set(mt_info,"memory",value);
		}
	}
	json_object_set(root,"mt_info",mt_info);
	json_decref(mt_info);
	return root;
}
/*====================================================================
 * 函数名    : vGenRegSrvMsg
 * 功能      : 生成服务器注册消息
 * 算法实现  : 
 * 参数说明  : nFlag 标志物理或逻辑服务器
 *			   nIdx  base索引
 * 返回值说明: 
 *			  成功 生成的注册消息
 *			  失败  NULL              
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/14       v1.0        YLI                          创建
 * ====================================================================*/
json_t *vGenRegSrvMsg(int nFlag,int nIdx)
{
	json_t *root;
	json_t *jt;
	json_t *srv_info;
	char saBuf[100];
	char saCfgPath[L_PATH_MAX_LEN];
	int n,m;
	memset(saBuf,0x00,sizeof saBuf);
	memset(saCfgPath,0x00,sizeof saCfgPath);
	srand((unsigned)time(NULL));
	root = json_object();
	//devid
	if (nFlag == F_PHY_FLAG)  //phy srv
	{
		sprintf(saBuf,"11%d",_gstrpPid[nIdx].nPhyCount);
	}
	else if (nFlag == F_LOGIC_FLAG) //logic srv
	{
		memset(saBuf,0x00,sizeof saBuf);
		//vLogNM(EL_DEBUG,"phy.log","phy count [%d] [%d]",_gstrpShm->rc.nPhyNum,_gstrpPid[nIdx].nPhyCount);
		vLogNM(EL_ERR,"phy.log","phy count [%d] [%d]",_gstrpShm->rc.nPhyNum,_gstrpPid[nIdx].nPhyCount);
		if (_gstrpPid[nIdx].nPhyCount == 0)
		{
			json_decref(root);
			vLogErr("phy server is not exist !!!!");
			return NULL;
		}
		n = rand() % _gstrpPid[nIdx].nPhyCount;
		//m = rand() % (_gstrpPid[nIdx].nLogicCount + 1);
		sprintf(saBuf,"11%d%d",n,_gstrpPid[nIdx].nLogicCount);
	}
	else
	{
		json_decref(root);
		vLogErr("devid generate error!!!");
		return NULL;
	}
	if (json_object_set(root,"devid",
		json_string(saBuf)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if(getenv("APP_PROFILE_PATH") == NULL)
	{
		json_decref(root);
		vLogErr("devtypes config file path error,please check evn value!!!");
		return NULL;
	}
	strcpy(saCfgPath,getenv("APP_PROFILE_PATH"));
	strcat(saCfgPath,"/devtypes.json");

	if (nFlag == F_PHY_FLAG)
	{
		jt = tPflGetJsonObj(saCfgPath,K_PHYTYPE_KEY);
	}
	else
	{
		jt = tPflGetJsonObj(saCfgPath,K_LOGICTYPE_KEY);
	}
	if (jt == NULL)
	{
		json_decref(root);
		vLogErr("devtypes file open error,please check devtypes.json is exist!!!");
		return NULL;
	}
	if (json_is_array(jt))
	{
		m = json_array_size(jt);
		n = rand()%m;
		if (json_object_set(root,"devtype",
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
	}
	json_decref(jt);
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_REG_REQ")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	/*srv_info*/
	srv_info = json_object();
	//devip
	json_object_set(srv_info,"devip",
		json_string(_gstrpShm->rc.nSrvIP));
	//devver
	if (json_object_set(srv_info,"devver",
		json_string("123")) == FAILUER)
	{
		json_decref(root);
		vLogErr("devadpver set error!!!");
		return NULL;
	}
	//devadpver
	json_object_set(srv_info,"devadpver",json_string("123"));
	//devname
	json_object_set(srv_info,"devname",json_string("truelink"));
	json_object_set(root,"srv_info",srv_info);
	json_decref(srv_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenPFMINFOMsg
 * 功能      : 生成终端上报性能消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/21       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPFMINFOMsg(VPD vpd)
{
	json_t *root;
	json_t *pfm_info;

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PFMINFO_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	/*pfm_info*/
	pfm_info = json_object();
	//cpu_userate
	json_object_set(pfm_info,"cpu_userate",json_integer(13));
	//disk_userate
	json_object_set(pfm_info,"disk_userate",json_integer(40));
	//mem_userate
	json_object_set(pfm_info,"mem_userate",json_integer(58));

	json_object_set(root,"pfm_info",pfm_info);
	json_decref(pfm_info);
	return root;
}
/*====================================================================
 * 函数名    : vGenPFMINFOCPUMsg
 * 功能      : 上报设备CPU性能消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/21       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPFMINFOCPUMsg(VPD vpd)
{
	json_t *root;
	json_t *cpuinfo;
	json_t *coreinfo;
	char saLocalTime[256];
	root = json_object();
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PFMINFO_CPU")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	json_object_set(root,"rpttime",json_string(saLocalTime));

	/*cpuinfo*/
	cpuinfo = json_object();
	//cpuusage
	json_object_set(cpuinfo,"cpuusage",json_integer(13));
	//cpucorecount
	json_object_set(cpuinfo,"cpucorecount",json_integer(3));
	//coreinfo
	coreinfo = json_array();
	json_array_insert(coreinfo,0,json_pack("{s:i}","cpucore1",143));
	json_array_insert(coreinfo,1,json_pack("{s:i}","cpucore2",143));
	json_array_insert(coreinfo,2,json_pack("{s:i}","cpucore3",143));

	json_object_set(cpuinfo,"coreinfo",coreinfo);
	json_object_set(root,"cpuinfo",cpuinfo);

	json_decref(coreinfo);
	json_decref(cpuinfo);
	return root;
}

/*====================================================================
 * 函数名    : vGenPFMINFOMEMMsg
 * 功能      : 主动上报设备MEM性能消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/21       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPFMINFOMEMMsg(VPD vpd)
{
	json_t *root;
	json_t *meminfo;
	char saLocalTime[256];
	root = json_object();
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PFMINFO_MEM")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	json_object_set(root,"rpttime",json_string(saLocalTime));

	/*meminfo*/
	meminfo = json_object();
	//total
	json_object_set(meminfo,"total",json_integer(134556));
	//used
	json_object_set(meminfo,"used",json_integer(30000));
	//userate
	json_object_set(meminfo,"userate",json_integer(30));
	json_object_set(root,"meminfo",meminfo);

	json_decref(meminfo);
	return root;
}

/*====================================================================
 * 函数名    : vGenPFMINFONETCARDMsg
 * 功能      : 主动上报设备NETCARD性能
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/21       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPFMINFONETCARDMsg(VPD vpd)
{
	json_t *root;
	json_t *netcardinfo;
	json_t *netcards;
	char saLocalTime[256];
	root = json_object();
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PFMINFO_NETCARD")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	json_object_set(root,"rpttime",json_string(saLocalTime));

	/*netcardinfo*/
	netcardinfo = json_object();
	//netcardcount
	json_object_set(netcardinfo,"netcardcount",json_integer(3));
	//sendkbps
	json_object_set(netcardinfo,"sendkbps",json_integer(3000));
	//recvkbps
	json_object_set(netcardinfo,"recvkbps",json_integer(3000));
	//recvpktloserate
	json_object_set(netcardinfo,"recvpktloserate",json_integer(3));
	//netcards
	netcards = json_array();

	json_array_insert(netcards,0,
		json_pack("{s:{s:i,s:i,s:i}}",
				"netcard1",
				"sendkbps",234,
				"recvkbps",234,
				"recvpktloserate",23));

	json_array_insert(netcards,1,
		json_pack("{s:{s:i,s:i,s:i}}",
				"netcard2",
				"sendkbps",234,
				"recvkbps",234,
				"recvpktloserate",23));

	json_array_insert(netcards,2,
		json_pack("{s:{s:i,s:i,s:i}}",
				"netcard3",
				"sendkbps",234,
				"recvkbps",234,
				"recvpktloserate",23));

	json_object_set(netcardinfo,"netcards",netcards);
	json_object_set(root,"netcardinfo",netcardinfo);
	json_decref(netcards);
	json_decref(netcardinfo);
	return root;
}

/*====================================================================
 * 函数名    : vGenPFMINFODISKMsg
 * 功能      : 主动上报设备DISK性能消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/21       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPFMINFODISKMsg(VPD vpd)
{
	json_t *root;
	json_t *diskinfo;
	char saLocalTime[256];
	root = json_object();
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PFMINFO_DISK")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	json_object_set(root,"rpttime",json_string(saLocalTime));

	/*diskinfo*/
	diskinfo = json_object();
	//total
	json_object_set(diskinfo,"total",json_integer(134556));
	//used
	json_object_set(diskinfo,"used",json_integer(30000));
	//userate
	json_object_set(diskinfo,"userate",json_integer(30));
	json_object_set(root,"diskinfo",diskinfo);

	json_decref(diskinfo);
	return root;
}

/*====================================================================
 * 函数名    : vGenSHOULDCONNSRVMsg
 * 功能      : 终端上报应该连接的逻辑服务器列表消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenSHOULDCONNSRVMsg(VPD vpd)
{
	json_t *root;
	json_t *conn_srv_type_info;
	json_t *jt;
	char saCfgPath[L_PATH_MAX_LEN];
	int n,m;
	root = json_object();
	memset(saCfgPath,0x00,sizeof saCfgPath);
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_SHOULD_CONNSRV_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	conn_srv_type_info = json_array();
	if(getenv("APP_PROFILE_PATH") == NULL)
	{
		json_decref(root);
		vLogErr("devtypes config file path error,please check evn value!!!");
		return NULL;
	}
	strcpy(saCfgPath,getenv("APP_PROFILE_PATH"));
	strcat(saCfgPath,"/devtypes.json");

	/*conn_srv_type_info*/
	jt = tPflGetJsonObj(saCfgPath,K_LOGICTYPE_KEY);
	if(jt == NULL)
	{
		json_decref(root);
		return NULL;
	}
	srand((unsigned)time(NULL));
	if (json_is_array(jt))
	{
		int it;
		m = json_array_size(jt);
		n = rand()%m;
		if (json_array_insert(conn_srv_type_info,0,
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
		it = rand()%m;
		while(it == n) it = rand()%m;
		n = it;
		if (json_array_insert(conn_srv_type_info,1,
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
	}
	json_object_set(root,"conn_srv_type_info",conn_srv_type_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenCONNSRVDESCMsg
 * 功能      : 终端与某服务器断开时上报消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenCONNSRVDESCMsg(VPD vpd)
{
	json_t *root;
	json_t *conn_srv_type_info;
	json_t *jt;
	char saCfgPath[L_PATH_MAX_LEN];
	int n,m;
	root = json_object();
	memset(saCfgPath,0x00,sizeof saCfgPath);
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_CONNSRV_DESC_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	conn_srv_type_info = json_array();
	if(getenv("APP_PROFILE_PATH") == NULL)
	{
		json_decref(root);
		vLogErr("devtypes config file path error,please check evn value!!!");
		return NULL;
	}
	strcpy(saCfgPath,getenv("APP_PROFILE_PATH"));
	strcat(saCfgPath,"/devtypes.json");

	/*conn_srv_type_info*/
	jt = tPflGetJsonObj(saCfgPath,K_LOGICTYPE_KEY);
	if(jt == NULL)
	{
		json_decref(root);
		return NULL;
	}
	srand((unsigned)time(NULL));
	if (json_is_array(jt))
	{
		int it;
		m = json_array_size(jt);
		n = rand()%m;
		if (json_array_insert(conn_srv_type_info,0,
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
		it = rand()%m;
		while(it == n) it = rand()%m;
		n = it;
		if (json_array_insert(conn_srv_type_info,1,
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
	}
	json_decref(jt);
	json_decref(conn_srv_type_info);
	json_object_set(root,"conn_srv_type_info",conn_srv_type_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenCONNSRVCONNMsg
 * 功能      : 终端与某服务器连上时上报消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenCONNSRVCONNMsg(VPD vpd)
{
	json_t *root;
	json_t *conn_srv_state_info;
	json_t *jt;
	json_t *jp;
	char saCfgPath[L_PATH_MAX_LEN];
	int n,m;
	root = json_object();
	memset(saCfgPath,0x00,sizeof saCfgPath);
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_CONNSRV_CONN_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	conn_srv_state_info = json_array();
	if(getenv("APP_PROFILE_PATH") == NULL)
	{
		json_decref(root);
		vLogErr("devtypes config file path error,please check evn value!!!");
		return NULL;
	}
	strcpy(saCfgPath,getenv("APP_PROFILE_PATH"));
	strcat(saCfgPath,"/devtypes.json");

	/*conn_srv_state_info*/
	jt = tPflGetJsonObj(saCfgPath,K_LOGICTYPE_KEY);
	if(jt == NULL)
	{
		json_decref(root);
		return NULL;
	}
	srand((unsigned)time(NULL));
	if (json_is_array(jt))
	{
		int it;
		m = json_array_size(jt);
		n = rand()%m;
		jp = json_pack("{s:s,s:s}",
			"type",json_string_value(json_array_get(jt,n)),
			"ip",_gstrpShm->rc.nSrvIP);

		if ((jp ==NULL) || (json_array_insert(conn_srv_state_info,0,
		jp)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
		it = rand()%m;
		while(it == n) it = rand()%m;
		n = it;
		jp = json_pack("{s:s,s:s}",
			"type",json_string_value(json_array_get(jt,n)),
			"ip",_gstrpShm->rc.nSrvIP);
		if (json_array_insert(conn_srv_state_info,1,
		json_array_get(jt,n)) == FAILUER)
		{
			json_decref(root);
			vLogErr("devtype set error!!!");
			return NULL;
		}	
	}
	json_decref(jt);
	json_decref(jp);
	json_decref(conn_srv_state_info);
	json_object_set(root,"conn_srv_state_info",conn_srv_state_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenALARMMsg
 * 功能      : 终端告警时上报消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenALARMMsg(VPD vpd)
{
	json_t *root;
	json_t *alarm_info;
	char saLocalTime[256];
	int n;
	root = json_object();

	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_ALARM_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	alarm_info = json_object();
	srand((unsigned)time(NULL));
	/*alarm_info*/
	//code_id
	n = rand()%10;
	json_object_set(alarm_info,"code_id",json_integer(alarm_code[n]));

	//status
	json_object_set(alarm_info,"status",json_true());

	//report_time
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	json_object_set(alarm_info,"report_time",json_string(saLocalTime));
	
	json_decref(alarm_info);
	json_object_set(root,"alarm_info",alarm_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenCONFINFOMsg
 * 功能      : 终端会议信息上报消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenCONFINFOMsg(VPD vpd)
{
	json_t *root;
	json_t *conf_info;
	char buf[100];
	int n;
	root = json_object();

	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_CONF_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	conf_info = json_object();
	srand((unsigned)time(NULL));

	//mt_state
	json_object_set(root,"mt_state",json_integer(mt_state[0]));

	/*conf_info*/
	//conf_e164
	memset(buf,0x00,sizeof buf);
	sprintf(buf,"0512e164123");
	json_object_set(conf_info,"conf_e164",json_string(buf));
	//mt_e164
	memset(buf,0x00,sizeof buf);
	sprintf(buf,"0512e164331");
	json_object_set(conf_info,"mt_e164",json_string(buf));
	//bitrate
	json_object_set(conf_info,"bitrate",json_integer(234));
	//privideo_send
	json_object_set(conf_info,"privideo_send",json_pack("[]"));
	//assvideo_send
	json_object_set(conf_info,"assvideo_send",json_pack("[]"));
	//privideo_recv
	json_object_set(conf_info,"privideo_recv",json_pack("[]"));
	//assvideo_recv
	json_object_set(conf_info,"assvideo_recv",json_pack("[]"));
	//audio_send
	json_object_set(conf_info,"audio_send",json_pack("[]"));
	//audio_recv
	json_object_set(conf_info,"audio_recv",json_pack("[]"));
	
	json_decref(conf_info);
	json_object_set(root,"conf_info",conf_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenVERSIONMsg
 * 功能      : 终端上报版本消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenVERSIONMsg(VPD vpd)
{
	json_t *root;
	root = json_object();

	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_VERSION_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//recommend
	if (json_object_set(root,"recommend",
		json_true()) == FAILUER)
	{
		json_decref(root);
		vLogErr("recommend set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vEXCEPTIONFILEMsg
 * 功能      : 终端上报崩溃日志消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenEXCEPTIONFILEMsg(VPD vpd)
{
	json_t *root;
	json_t *execption_info;
	char saLocalTime[256];
	root = json_object();

	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_EXCEPTION_FILE")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	execption_info = json_object();
	/*execption_info*/
	//devver
	if (json_object_set(execption_info,"devver",
		json_string("123")) == FAILUER)
	{
		json_decref(root);
		vLogErr("devver set error!!!");
		return NULL;
	}
	//execption_time
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(execption_info,"execption_time",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("execption_time set error!!!");
		return NULL;
	}
	//execption_file
	if (json_object_set(execption_info,"execption_file",
		json_string("ef.txt")) == FAILUER)
	{
		json_decref(root);
		vLogErr("execption_file set error!!!");
		return NULL;
	}

	json_decref(execption_info);
	json_object_set(root,"execption_info",execption_info);
	return root;
}

/*====================================================================
 * 函数名    : vGenBANDWIDTHMsg
 * 功能      : 带宽检测上下行带宽上报消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenBANDWIDTHMsg(VPD vpd)
{
	json_t *root;
	json_t *netstatus;

	root = json_object();

	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_BANDWIDTH_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	netstatus = json_object();
	/*netstatus*/
	//send_bandwidth
	if (json_object_set(netstatus,"send_bandwidth",
		json_pack("{}")) == FAILUER)
	{
		json_decref(root);
		vLogErr("send_bandwidth set error!!!");
		return NULL;
	}
	//recv_bandwidth
	if (json_object_set(netstatus,"recv_bandwidth",
		json_pack("{}")) == FAILUER)
	{
		json_decref(root);
		vLogErr("recv_bandwidth set error!!!");
		return NULL;
	}
	json_decref(netstatus);
	json_object_set(root,"netstatus",netstatus);
	return root;
}

/*====================================================================
 * 函数名    : vGenNETINFOMsg
 * 功能      : 网络信息改变消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenNETINFOMsg(VPD vpd)
{
	json_t *root;
	json_t *netinfo;

	root = json_object();

	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_NETINFO_MSG")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	netinfo = json_object();
	/*netinfo*/
	//ip
	if (json_object_set(netinfo,"ip",
		json_string(_gstrpShm->rc.nSrvIP)) == FAILUER)
	{
		json_decref(root);
		vLogErr("ip set error!!!");
		return NULL;
	}
	//dns
	if (json_object_set(netinfo,"dns",
		json_string("172.16.0.65")) == FAILUER)
	{
		json_decref(root);
		vLogErr("dns set error!!!");
		return NULL;
	}
	json_decref(netinfo);
	json_object_set(root,"netinfo",netinfo);
	return root;
}

/*====================================================================
 * 函数名    : vGenSYSTIMESYNCMsg
 * 功能      : 主动上报物理服务器和NTP服务器时间同步状态
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenSYSTIMESYNCMsg(VPD vpd)
{
	json_t *root;
	char saLocalTime[256];

	root = json_object();
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_SYSTIME_SYNC")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}

	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//syncstate
	if (json_object_set(root,"syncstate",
		json_true()) == FAILUER)
	{
		json_decref(root);
		vLogErr("syncstate set error!!!");
		return NULL;
	}
	
	return root;
}

/*====================================================================
 * 函数名    : vGenCOLLECTORHEARTBEATMsg
 * 功能      : 上报collector心跳消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenCOLLECTORHEARTBEATMsg(VPD vpd)
{
	json_t *root;
	char saLocalTime[256];

	root = json_object();
	//collectorid
	if (json_object_set(root,"collectorid",
		json_integer(12345)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_COLLECTOR_HEARTBEAT")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string("COLLECTOR")) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	return root;
}

/*====================================================================
 * 函数名    : vGenDEVONLINEMsg
 * 功能      : 设备上线消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenDEVONLINEMsg(VPD vpd)
{
	json_t *root;
	char saLocalTime[256];

	root = json_object();
	//collectorid
	if (json_object_set(root,"collectorid",
		json_integer(12345)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_DEV_ONLINE")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenDEVOFFLINEMsg
 * 功能      : 设备下线消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenDEVOFFLINEMsg(VPD vpd)
{
	json_t *root;
	char saLocalTime[256];

	root = json_object();
	//collectorid
	if (json_object_set(root,"collectorid",
		json_integer(12345)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devid set error!!!");
		return NULL;
	}
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_DEV_OFFLINE")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenPASINFOMsg
 * 功能      : PAS上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPASINFOMsg(VPD vpd)
{
	json_t *root;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PAS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//version
	json_object_set(root,"version",json_string("1.06"));
	//pidchange
	json_object_set(root,"pidchange",json_false());
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	/*pasinfo*/
	json_object_set(root,"pasinfo",
		json_pack("{s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i}",
			"maxcallcount",10000,
			"maxonlinecount",10000,
			"curonlinecount",10000,
			"siponlinecount",10000,
			"h323onlinecount",10000,
			"monitoronlinecount",10,
			"callingcount",100,
			"confmtcount",100));
	return root;
}

/*====================================================================
 * 函数名    : vGenPASP2PCONFCREATEMsg
 * 功能      : PAS上报创建点对点会议信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPASP2PCONFCREATEMsg(VPD vpd)
{
	json_t *root;
	json_t *p2pconfinfo;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PAS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//version
	json_object_set(root,"version",json_string("1.06"));
	//pidchange
	json_object_set(root,"pidchange",json_false());
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	/*p2pconfinfo*/
	p2pconfinfo = json_object();
	//confe164
	json_object_set(p2pconfinfo,"confe164",json_string("e164"));
	//confname
	json_object_set(p2pconfinfo,"confname",json_string("Share"));
	//bitrate
	json_object_set(p2pconfinfo,"bitrate",json_string("Share"));
	//begintime
	json_object_set(p2pconfinfo,"begintime",json_string(saLocalTime));
	//duration
	json_object_set(p2pconfinfo,"duration",json_integer(1));
	//caller
	json_object_set(p2pconfinfo,"caller",
		json_pack("{s:s,s:s,s:s,s:s}",
			"devtype",vpd.saDevType,
			"devname","sm",
			"deve164","e164",
			"devguid",vpd.saDevId));
	//callee
	json_object_set(p2pconfinfo,"callee",
		json_pack("{s:s,s:s,s:s,s:s}",
			"devtype",vpd.saDevType,
			"devname","sm",
			"deve164","e164",
			"devguid",vpd.saDevId));
	//endtime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	json_object_set(p2pconfinfo,"endtime",json_string(saLocalTime));

	json_decref(p2pconfinfo);
	json_object_set(root,"p2pconfinfo",p2pconfinfo);

	return root;
}

/*====================================================================
 * 函数名    : vGenPASP2PCONFDESTROYMsg
 * 功能      : PAS上报销毁点对点会议信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPASP2PCONFDESTROYMsg(VPD vpd)
{
	json_t *root;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PAS_P2PCONF_DESTROY")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//confe164
	json_object_set(root,"confe164",json_string("05121232"));
	//confendreason
	json_object_set(root,"confendreason",json_string("caller down"));

	return root;
}

/*====================================================================
 * 函数名    : vGenMPCDINFOMsg
 * 功能      : MPCD上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMPCDINFOMsg(VPD vpd)
{
	json_t *root;
	json_t *mpcdinfo;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MPCD_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//pidchange
	json_object_set(root,"pidchange",json_false());
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	/*mpcdinfo*/
	mpcdinfo = json_object();
	//connectedmpccount
	json_object_set(mpcdinfo,"connectedmpccount",json_integer(123));
	//connectednucount
	json_object_set(mpcdinfo,"connectednucount",json_integer(123));
	//conftempcount
	json_object_set(mpcdinfo,"conftempcount",json_integer(123));
	//maxorderconfcount
	json_object_set(mpcdinfo,"maxorderconfcount",json_integer(123));
	//connectedbmc
	json_object_set(mpcdinfo,"connectedbmc",json_true());
	//connectedmpcinfo
	res = json_object_set(mpcdinfo,"connectedmpcinfo",
		json_pack("[{s:b,s:i,s:i},{s:b,s:s,s:i,s:i}]",
			"connectedstate",1,
			"sptconfcount",123,
			"curconfcount",123,
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP,
			"sptconfcount",123,
			"curconfcount",123));

	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("mpcdinfo set error!!!");
		return NULL;
	}
	//connectednuinfo
	res = json_object_set(mpcdinfo,"connectednuinfo",
		json_pack("[{s:b,s:s},{s:b,s:s}]",
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP,
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP));

	if (res == FAILUER)
	{
		json_decref(root);
		json_decref(mpcdinfo);
		vLogErr("mpcdinfo set error!!!");
		return NULL;
	}

	json_object_set(root,"mpcdinfo",mpcdinfo);
	json_decref(mpcdinfo);

	return root;
}

/*====================================================================
 * 函数名    : vGenMCUINFOMsg
 * 功能      : MCU上报逻辑服务器基本信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMCUINFOMsg(VPD vpd)
{
	json_t *root;
	json_t *mcuinfo;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MCU_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//pidchange
	json_object_set(root,"pidchange",json_false());
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	/*mcuinfo*/
	mcuinfo = json_object();
	//traditionconfcount
	json_object_set(mcuinfo,"traditionconfcount",json_integer(123));
	//portconfcount
	json_object_set(mcuinfo,"portconfcount",json_integer(123));
	//spttraditionconfcount
	json_object_set(mcuinfo,"spttraditionconfcount",json_integer(123));
	//sptportconfcount
	json_object_set(mcuinfo,"sptportconfcount",json_integer(123));
	//multiconfmtcount
	json_object_set(mcuinfo,"multiconfmtcount",json_integer(123));
	//connectedmpcd
	json_object_set(mcuinfo,"connectedmpcd",json_true());
	//connectedbmc
	json_object_set(mcuinfo,"connectedbmc",json_true());
	//connectednucount
	json_object_set(mcuinfo,"connectednucount",json_integer(123));
	//connectedmpcount
	json_object_set(mcuinfo,"connectedmpcount",json_integer(123));
	//connectedmpcadaptcount
	json_object_set(mcuinfo,"connectedmpcadaptcount",json_integer(123));
	//connectedprscount
	json_object_set(mcuinfo,"connectedprscount",json_integer(123));
	
	//connectednuinfo
	res = json_object_set(mpcdinfo,"connectednuinfo",
		json_pack("[{s:b,s:s},{s:b,s:s}]",
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP,
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP));

	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("connectednuinfo set error!!!");
		return NULL;
	}
	//connectedmpinfo
	res = json_object_set(mpcdinfo,"connectedmpinfo",
		json_pack("[{s:b,s:s},{s:b,s:s}]",
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP,
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP));

	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("connectedmpinfo set error!!!");
		return NULL;
	}
	//connectedmpcadaptinfo
	res = json_object_set(mpcdinfo,"connectedmpcadaptinfo",
		json_pack("[{s:b,s:s},{s:b,s:s}]",
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP,
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP));

	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("connectedmpcadaptinfo set error!!!");
		return NULL;
	}
	//connectedprsinfo
	res = json_object_set(mpcdinfo,"connectedprsinfo",
		json_pack("[{s:b,s:s},{s:b,s:s}]",
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP,
			"connectedstate",1,
			"connectedip",_gstrpShm->rc.nSrvIP));

	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("connectedprsinfo set error!!!");
		return NULL;
	}
	json_object_set(root,"mpcdinfo",mpcdinfo);
	json_decref(mpcdinfo);

	return root;
}

/*====================================================================
 * 函数名    : vGenMCUCONFCREATEMsg
 * 功能      : MCU上报多点会议创建会议/结束会议消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMCUCONFCREATEMsg(VPD vpd)
{
	json_t *root;
	json_t *multiconfinfo;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MCU_CONF_CREATE")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	/*multiconfinfo*/
	multiconfinfo = json_object();
	//mtnums
	json_object_set(multiconfinfo,"mtnums",json_integer(123));
	//conftype
	json_object_set(multiconfinfo,"conftype",json_integer(1));
	//confe164
	json_object_set(multiconfinfo,"confe164",json_string("e164"));
	//confname
	json_object_set(multiconfinfo,"confname",json_string("name"));
	//confbandwidth
	json_object_set(multiconfinfo,"confbandwidth",json_integer(123));
	//domainguid
	json_object_set(multiconfinfo,"domainguid",json_string("12345"));
	//domainname
	json_object_set(multiconfinfo,"domainname",json_string("name"));
	//portcount
	json_object_set(multiconfinfo,"portcount",json_integer(123));
	//begintime
	json_object_set(multiconfinfo,"begintime",json_string(saLocalTime));
	//endtime
	json_object_set(multiconfinfo,"endtime",json_string(saLocalTime));
	
	json_object_set(root,"multiconfinfo",multiconfinfo);
	json_decref(multiconfinfo);

	return root;
}

/*====================================================================
 * 函数名    : vGenMCUCONFDESTROYMsg
 * 功能      : MCU上报多点会议结会消息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMCUCONFDESTROYMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MCU_CONF_DESTROY")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//confe164
	json_object_set(root,"confe164",json_string("e164"));

	return root;
}

/*====================================================================
 * 函数名    : vGenMCUMTADDMsg
 * 功能      : MCU上报多点会议中终端入会消息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMCUMTADDMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MCU_MT_ADD")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//confe164
	json_object_set(root,"confe164",json_string("e164"));

	/*mtinfo*/
	res = json_object_set(root,"mtinfo",
		json_pack("{s:s,s:s,s:s,s:s,s:s,s:s}",
			"devtype",vpd.saDevType,
			"devip",_gstrpShm->rc.nSrvIP,
			"deve164","e164",
			"devname","mt",
			"softversion","1.11",
			"begintime",saLocalTime));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("connectednuinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenMCUMTDELMsg
 * 功能      : MCU上报多点会议中终端离会消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMCUMTDELMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MCU_MT_DEL")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//confe164
	json_object_set(root,"confe164",json_string("e164"));
	//mte164
	json_object_set(root,"mte164",json_string("e164"));
	//leavereason
	json_object_set(root,"leavereason",json_string("off line"));

	return root;
}

/*====================================================================
 * 函数名    : vGenMPINFOMsg
 * 功能      : MP上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenMPINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MP_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*mpinfo*/
	res = json_object_set(root,"mpinfo",
		json_pack("{s:i,s:i,s:i,s:b,s:s,s:s,s:b}",
			"transfercount",100,
			"maxtransbitrate",100,
			"deve164",100,
			"connectedmpc",1,
			"mpip",_gstrpShm->rc.nSrvIP,
			"connectedmpcdip",_gstrpShm->rc.nSrvIP,
			"connectedbmc",1));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("mpinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenPRSINFOMsg
 * 功能      : PRS上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenPRSINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_PRS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//confe164
	json_object_set(root,"confe164",json_string("e164"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*prsinfo*/
	res = json_object_set(root,"prsinfo",
		json_pack("{s:s,s:b,s:s,s:b}",
			"prsip",_gstrpShm->rc.nSrvIP,
			"connectedmpc",1,
			"connectedmpcdip",_gstrpShm->rc.nSrvIP,
			"connectedbmc",0));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("connectednuinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenSUSINFOMsg
 * 功能      : SUS上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenSUSINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_SUS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*susinfo*/
	res = json_object_set(root,"susinfo",
		json_pack("{s:i,s:i,s:b,s:b}",
			"maxregdevcount",100,
			"curregdevcount",100,
			"connectedsusmgr",1,
			"connectedsusmgr",1));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("susinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenSUSMGRINFOMsg
 * 功能      : SUSMGR上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenSUSMGRINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_MP_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*susmgrinfo*/
	res = json_object_set(root,"susmgrinfo",
		json_pack("{s:i,s:i,s:b,s:b}",
			"maxregsussrvcount",100,
			"curregsussrvcount",100,
			"connectedbmc",1));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("susmgrinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenAPSINFOMsg
 * 功能      : APS上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenAPSINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_APS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*apsinfo*/
	res = json_object_set(root,"apsinfo",
		json_pack("{s:i,s:i,s:b}",
			"maxusercount",1000,
			"currusercount",1000,
			"connectedbmc",1));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("apsinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenLGSINFOMsg
 * 功能      : LGS上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenLGSINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_LGS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*lgsinfo*/
	res = json_object_set(root,"lgsinfo",
		json_pack("{s:i,s:i,s:i,s:b}",
			"maxsrvcount",1000,
			"currsrvcount",1000,
			"logcount",1,
			"connectedbmc",1));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("lgsinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenNTSINFOMsg
 * 功能      : NTS上报逻辑服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenNTSINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_NTS_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*ntsinfo*/
	res = json_object_set(root,"ntsinfo",
		json_pack("{s:i,s:i,s:b}",
			"maxtestcount",1000,
			"currtestcount",1000,
			"connectedbmc",1));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("ntsinfo set error!!!");
		return NULL;
	}
	return root;
}

/*====================================================================
 * 函数名    : vGenXMPPINFOMsg
 * 功能      : XMPP上报服务器业务信息消息
 * 算法实现  : 
 * 参数说明  : vpd 要上报的设备
 * 返回值说明: 成功 生成的消息
 *			   失败 NULL             
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/26       v1.0        YLI                          创建
 * ====================================================================*/
json_t * vGenXMPPINFOMsg(VPD vpd)
{
	json_t *root;
	int res;
	char saLocalTime[256];

	root = json_object();
	//eventid
	if (json_object_set(root,"eventid",
		json_string("EV_XMPP_INFO")) == FAILUER)
	{
		json_decref(root);
		vLogErr("eventid set error!!!");
		return NULL;
	}
	//devid
	if (json_object_set(root,"devid",
		json_string(vpd.saDevId)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//devtype
	if (json_object_set(root,"devtype",
		json_string(vpd.saDevType)) == FAILUER)
	{
		json_decref(root);
		vLogErr("devtype set error!!!");
		return NULL;
	}
	//rpttime
	memset(saLocalTime,0x00,sizeof saLocalTime);
	GetLocalTime(saLocalTime);
	if (json_object_set(root,"rpttime",
		json_string(saLocalTime)) == FAILUER)
	{
		json_decref(root);
		vLogErr("rpttime set error!!!");
		return NULL;
	}
	
	//version
	json_object_set(root,"version",json_string("1.06"));
	//belongphy
	json_object_set(root,"belongphy",json_string("1123"));
	//pidchange
	json_object_set(root,"pidchange",json_true());

	/*xmppinfo*/
	res = json_object_set(root,"xmppinfo",
		json_pack("{s:i}","onlinecount",1000));
	if (res == FAILUER)
	{
		json_decref(root);
		vLogErr("xmppinfo set error!!!");
		return NULL;
	}
	return root;
}
