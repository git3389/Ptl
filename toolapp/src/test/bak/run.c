#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "testunit.h"

void run()
{
#if 0
	TESTMOD("log",test_log);
	TESTMOD("json_parse",test_parse_json);
	TESTMOD("util",test_util);
	TESTMOD("ipc/sem",test_ipc_sem);
	TESTMOD("ptl list",test_ptl_list);

	TESTMOD("ptl init and done",test_init_done);

#endif
	//TESTMOD("ptl unpack",test_ptl_unpack);
//	TESTMOD("ptl pack",test_ptl_pack);	
	TESTMOD("ptl gen msg",test_gen_msg);
}	

 /*Program entrance*/
int main(int argc, char** argv)
{
	run();	
	return 0;
}


