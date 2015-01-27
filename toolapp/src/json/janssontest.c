#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jansson.h"

void json_get_obj(json_t **obj)
{
	json_t *root;
	root = json_object();
	json_object_set(root,"obj",json_string("test"));
	*obj = root;
}

json_t* json_get_obj1()
{
	json_t *root;
	root = json_object();
	json_object_set(root,"obj",json_string("test"));
	return root;
}

void test(char **str)
{
	char *buf = (char *)malloc(10);
	strcpy(buf,"test");
	*str = buf;
}

void json_object_display(json_t *obj)
{
	const char *key;
	json_t *value;
	void *iter;
	iter = json_object_iter(obj);
	while(iter)
	{
		key = json_object_iter_key(iter);
		value = json_object_iter_value(iter);
		if(!json_is_object(value))
		{
			printf("%s:",key);
			if(json_is_string(value))
				printf("%s\n",json_string_value(value));
			if(json_is_integer(value))
				printf("%d\n",json_integer_value(value));
			if(json_is_boolean(value))
				printf("%d\n",json_boolean_value(value));
		}
		else
		{
			json_object_display(value);
		}
		iter = json_object_iter_next(obj,iter);
		if(iter == NULL)
		{
			break;
		}
	}
	
}

void doit(char *text)
{
	json_error_t error;
	json_t *object;

	//1 解析字符串到一个对象中
	object = json_loads(text,0,&error);
	if(object == NULL)
	{
		printf("%s\n",error.text);
	}
	printf("len:%d\n",json_object_size(object));
	json_object_display(object);
}
void test_foreach(char *text)
{
	json_error_t error;

	json_t *object;
	const char *key;
	json_t *value;
	object = json_loads(text,0,&error);
	json_object_foreach(object,key,value)
	{
		printf("%s:%s\n",key,json_string_value(value));
	}

}
void pack()
{
	json_t *obj = json_object();
	json_t *other = json_object();
	const char *key;
	json_t *value;	
	json_object_set(obj,"name",json_string("Consloas"));
	json_object_set(other,"type",json_string("rect"));
	json_object_set(other,"frame",json_integer(24));
	json_object_set(other,"interlace",json_false());
	json_object_set(obj,"foramt",other);
	
	printf("len:%d\n",json_object_size(obj));
	void *iter;
	void *iter1;
	iter = json_object_iter(obj);
	while(iter)
	{
		key = json_object_iter_key(iter);
		value = json_object_iter_value(iter);
		if(!json_is_object(value))
		{
			printf("%s:",key);
			if(json_is_string(value))
				printf("%s\n",json_string_value(value));
			if(json_is_integer(value))
				printf("%d\n",json_integer_value(value));
			if(json_is_boolean(value))
				printf("%d\n",json_boolean_value(value));
		}
		else
		{
			
		}
		iter = json_object_iter_next(obj,iter);	
	}
	printf("%s\n",json_dumps(obj,0));
		
}
 /*Program entrance*/
int main(int argc, char** argv)
{

	//json text
//	char *text = "{\"name\":\"Consloas\",\"format\":{\"type\":\"rect\",\"width\":1024,\"height\":768,\"interlace\":false,\"frame\":24}}";
	char *text = "{\"name\":\"Consloas\",\"type\":\"rect\",\"width\":1024,\"height\":768,\"interlace\":false,\"frame\":24}";

	//parse jsontext
//	doit(text);
//	pack();
#if 0
//	test_foreach(text);
	char *conext = "{\"string\":\"String\",\"Int\":10}";
	json_error_t error;
	json_t *obj = json_loads(conext,0,&error);
   	char str[10];
	int i;
	if(json_unpack(obj,"{s?s,s?i}","string",str,"int",&i) == -1)
	{
		printf("unpack fail!\n");
		return 0;
	}
	printf("string:%s\nint:%d\n",str,i);
#endif
	char *str;
	json_t *obj;
	json_get_obj(&obj);
//	obj = json_get_obj1();
	test(&str);
	printf("obj:%s\n",json_dumps(obj,0));
	printf("str:%s\n",str);
	free(str);
	return 0;

}


