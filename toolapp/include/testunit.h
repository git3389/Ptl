#ifndef _TESTUNIT_H
#define _TESTUNIT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TESTMOD(nm,f) \
	printf("test %s start ...\n",nm); \
	f(); \
	printf("test %s filish!!!\n",nm);\

void run();

extern void test_log(void);
extern void test_parse_json(void);
extern void test_util(void);
extern void test_ipc_sem(void);
extern void test_ptl_list(void);
extern void test_load_param(void);
extern void test_init_done(void);
extern void test_genbase(void);
extern void test_ptl_pack(void);
extern void test_ptl_unpack(void);
extern void test_getcpuinfo(void);
extern void test_gen_reg_msg(void);
extern void test_gen_msg(void);
extern void test_gen_msg1(void);


#endif /*_TESTUNIT_H*/
