#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "test.h"

typedef void (*fn)(void);
fn fns;
#define PRINT print

#define FUN(id) (fun000##id0000_cb)
#define init(msg) \
	int i = 5;\
	strcmp(msg,"msg");

#define FUN_(fn) \
	void fn() \
	{\
		printf("hello\n");\
	}
#define BEGIN
FUN_(PRINT)
FUN_(ls)
#define END

 /*Program entrance*/
int main(int argc, char** argv)
{
//	fns = NULL;
//	init(str)
//	print();
//	test();
	char tmp[10];
	sprintf(tmp,"%d %02d %04d",1,234,34);
	printf("%s\n",tmp);
	return 0;
}


