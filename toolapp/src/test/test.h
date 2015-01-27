#include <stdio.h>

#define TEST(fn) \
	void fn() \
	{\
		printf("hello\n");\
	}

TEST(test)
