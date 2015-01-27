#include "testunit.h"
#include "common.h"

void test_getcpuinfo(void)
{
	json_t *cpuinfo = GetCpuInfo();
	json_t *meminfo = GetMemInfo();
	printf("%s\n",json_dumps(cpuinfo,0));
	printf("%s\n",json_dumps(meminfo,0));
}
