#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jansson.h"

 /*Program entrance*/
int main(int argc, char** argv)
{
	json_t *root = json_object();
	json_object_set(root,"boo",json_string("boo"));
	json_object_set(root,"far",json_pack("{}"));
	json_object_set(root,"sheel",json_pack("[]"));

	printf("%s\n",json_dumps(root,0));
	json_decref(root);
	return 0;
}



