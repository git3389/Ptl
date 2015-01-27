#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void doit(char *text)
{
	cJSON *root;
	cJSON *format;
	root = cJSON_Parse(text);
	if(!root)
		printf("cJSON_Parse error!\n");
	printf("json text:%s\n",cJSON_Print(root));
	format = cJSON_GetObjectItem(root,"format");
	printf("name:%s\nformat:\ntype:%s\nwidtd:%d\nheight:%d\ninterlace:%d\nframe:%d",
			cJSON_GetObjectItem(root,"name")->valuestring,
			cJSON_GetObjectItem(format,"type")->valuestring,
			cJSON_GetObjectItem(format,"width")->valueint,
			cJSON_GetObjectItem(format,"height")->valueint,
			cJSON_GetObjectItem(format,"interlace")->valueint,
			cJSON_GetObjectItem(format,"frame")->valueint);
	cJSON_Delete(format);
	cJSON_Delete(root);
	

}

int main(int avgc,char* avgr[])
{
	//json text
	char *text = "{\"name\":\"Consloas\",\"format\":{\"type\":\"rect\",\"width\":1024,\"height\":768,\"interlace\":false,\"frame\":24}}";
	//parse json text
	doit(text);

	return 0;
}
