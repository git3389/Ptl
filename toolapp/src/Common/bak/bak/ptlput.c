/*====================================================================
 * 模块名  : ptlput
 * 文件名  : ptlput.c
 * 相关文件:
 * 实现功能: 修改共享内存中的相关数据
 * 作者    : YLI
 * 版权    : <Copyright(c) 2003-2007 Suzhou Keda Technology Co.,Ltd.All right reserved.>
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12   v1.0        YLI                          创建
 * ====================================================================*/

#include "ptl.h"
#include "common.h"


/*====================================================================
 * 函数名    : _tPutConnCountInc
 * 功能      : 修改已经创建的term/srv的数量,增加
 * 算法实现  : 
 * 参数说明  : nIdx 要修改的base的索引
 *             nFlag 要修改的term/srv数量的标识
 * 返回值说明: 成功 0
 *			   失败 -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
 int _tPutConnCountInc(int nIdx,int nFlag)
 {
   	if (_tSemP(_gstrpShm->rc.tSemPidId) != SUCCESS)
   	{
   		return FAILUER;
   	}
   	if (nFlag == F_TERM_FLAG)
   	{
   		_gstrpPid[nIdx].nTermCount++;
   	}
   	if (nFlag == F_PHY_FLAG)
   	{
   		_gstrpPid[nIdx].nPhyCount++;
   	} 
    if (nFlag == F_LOGIC_FLAG)
    {
      _gstrpPid[nIdx].nLogicCount++;
    }
   	if (_tSemV(_gstrpShm->rc.tSemPidId) != SUCCESS)
  	{
  		vLogErr("_tSemV fail!!!");
  		return FAILUER;
  	}	
  	return SUCCESS;
 }
 /*====================================================================
 * 函数名    : _tPutConnCountDec
 * 功能      : 修改已经创建的term/srv的数量,减少
 * 算法实现  : 
 * 参数说明  : nIdx 要修改的base的索引
 *             nFlag 要修改的term/srv数量的标识
 * 返回值说明: 成功 0
 *             失败 -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
int _tPutConnCountDec(int nIdx,int nFlag)
{
    if (_tSemP(_gstrpShm->rc.tSemPidId) != SUCCESS)
    {
      return FAILUER;
    }
    if (nFlag == F_TERM_FLAG)
    {
      _gstrpPid[nIdx].nTermCount--;
    }
    if (nFlag == F_PHY_FLAG)
    {
      _gstrpPid[nIdx].nPhyCount--;
    } 
    if (nFlag == F_LOGIC_FLAG)
    {
      _gstrpPid[nIdx].nLogicCount--;
    }
    if (_tSemV(_gstrpShm->rc.tSemPidId) != SUCCESS)
    {
      vLogErr("_tSemV fail!!!");
      return FAILUER;
    } 
    return SUCCESS;
}
/*====================================================================
 * 函数名    : _tPutConnCount
 * 功能      : 修改成功连接的数量
 * 算法实现  : 
 * 参数说明  : nIdx 要修改的base的索引
 *             
 * 返回值说明: 成功 0
 *             失败 -1            
 * ----------------------------------------------------------------------
 * 修改记录:
 * 日  期        版本        修改人        走读人        修改记录
 * 2015/1/12       v1.0        YLI                          创建
 * ====================================================================*/
int _tPutConnCount(int nIdx,int nCount)
{
    if (_tSemP(_gstrpShm->rc.tSemPidId) != SUCCESS)
    {
      return FAILUER;
    }
    _gstrpPid[nIdx].nConnCount = nCount;
    if (_tSemV(_gstrpShm->rc.tSemPidId) != SUCCESS)
    {
      vLogErr("_tSemV fail!!!");
      return FAILUER;
    } 
    return SUCCESS;
}
 /*====================================================================
  * 函数名    : _tPutConnToList
  * 功能      : 将成功连接并注册好的term/srv插入到对应的base管理的链表中去
  * 算法实现  : 
  * 参数说明  : nIdx  对应的base
  *			    nFlag vpd的标志
  *				vpd   创建好的term/srv
  * 返回值说明: 成功 0
  *             失败 -1               
  * ----------------------------------------------------------------------
  * 修改记录:
  * 日  期        版本        修改人        走读人        修改记录
  * 2015/1/12      v1.0        YLI                          创建
  * ====================================================================*/
 int _tPutConnToList(int nIdx,VPD vpd)
 {
 	if (_tSemP(_gstrpShm->rc.tSemPidId) != SUCCESS)
 	{
 		vLogErr("_tSemP fail!!!");
 		return FAILUER;
 	}
 	_gstrpTermList = _gstrpPid[nIdx].strpTerm;
 	_gstrpPhyList  = _gstrpPid[nIdx].strpPhy;
  _gstrpLogicList  = _gstrpPid[nIdx].strpLogic;
 	if(InsertNode(vpd) != SUCCESS)
 	{
 		vLogErr("Inser a term/srv to list fail !!!");
 		_tSemV(_gstrpShm->rc.tSemPidId);
 		return FAILUER;
 	}
 	if (_tSemV(_gstrpShm->rc.tSemPidId) != SUCCESS)
	{
		vLogErr("_tSemV fail!!!");
		return FAILUER;
	}	
	return SUCCESS;
 }
 

