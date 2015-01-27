#ifndef _PTLCONST_H
#define _PTLCONST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#ifdef __linux
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#endif /*__linux*/

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#endif /*WIN32*/

#ifdef _WIN64
#include <windows.h>
#include <winsock2.h>
#endif /*_WIN64*/


#ifdef WIN32 
#define _stat stat
#elif _WIN64
#define _stat stat
#endif /*WIN32 | _WIN64*/

#include "ptltypes.h"
//const
#define true    	 1
#define false 		 0
#define FAILUER       -1
#define SUCCESS      0
//errno
#define E_SUBSIG     50

//len
#define L_FILE_NAME_MAX_LEN  32
#define L_PATH_MAX_LEN       256
#define L_ARGV_MAX_LEN       255
#define L_PACK_BAG_MAX_LEN   1024


//json config file
//app
#define J_APP_KEY    "app"
#define K_TCN_KEY    "TestCfgName"
#define K_MTFN_KEY   "MsgTypesFN"
#define K_BMN_KEY    "BaseMaxNum"
#define K_IPC_KEY    "IPCKEY"

//log
#define J_LOG_KEY     "log"
#define F_LOG_INIT     1
#define LOG_SIZE_UNIT  1000000L



//cfg
#define K_PHYNUM_KEY  	"PhySrvNum"
#define K_LOGICNUM_KEY  "LogicSrvNum"
#define K_TERMNUM_KEY  	"TermNum"
#define K_IP_KEY   	  	"IP"
#define K_PORT_KEY   	"Port"
#define K_MSGTYPE_KEY   "MsgType"
#define K_TIMER_KEY     "Timer"
#define K_MSGID_KEY     "MsgId"
#define K_TESTTIME_KEY  "TestTime"
#define K_COMMSG_KEY    "ComMsg"
#define K_TERMMSG_KEY   "TermMsg"
#define K_PHYMSG_KEY    "PHYMsg"
#define K_LOGICMSG_KEY   "LogicMsg"
#define K_GENCONNTIMER_KEY "GenConnTimer"
#define K_DEVTYPE_KEY     "DevType"
#define K_PHYTYPE_KEY     "PhyType"
#define K_LOGICTYPE_KEY    "LogicType"
//size
#define S_RAND_RANG             10000
#define S_BMN_SIZE     		 	100
#define S_LOGFILE_SIZE  		100
#define S_DEFAULT_TESTTIME  	1
#define S_DEFAULT_PHY    		1
#define S_DEFAULT_HEARTBEAT  	10
#define S_GENCONN_TIME   		10
#define S_TESTTIME_MAX_SIZE 	100
#define S_TIMER_MAX_SIZE    	1000000L
#define S_KEEP_ALIVE_CHECK  	10
#define S_KEEP_NUM          	3

//flag
#define F_PRE_FLAG      201
#define F_CURR_FLAG     200
#define F_PHY_FLAG     100
#define F_LOGIC_FLAG    101
#define F_TERM_FLAG    102
#define F_NULL_FLAG    -1
#define F_CONN_OK      0
#define F_CONN_ERR     -1
#define F_REG_OK       0
#define F_REG_ERR      -1
#define F_KEEP_INIT    0
#define F_KEEP_DEC     -1
#define F_HEART_OK     0
#define F_HEART_ERR    -1

//sig
#define S_TESTFINISH_SIG  (SIGUSR1 + 1)
#define S_GENSUBBASE_SIG  (SIGUSR1 + 2)         //generate base signal
#define S_SUBBASEINT_SIG  (SIGUSR2 + 3)

#endif /* _PTLCONST_H */
