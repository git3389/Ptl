#ifndef _SERVER_h
#define _SERVER_h

//system libary headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//libary headers
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"
#include "event2/listener.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"

//const define
#define WELCOME 	"welcome to ....."
#define DATA_MAX     1024

//typedef
typedef int bool;

typedef struct read_buf_q      //保存读数据的缓冲区
{
	struct evbuffer *evb;
	evutil_socket_t fd;
}RBQ,*PRBQ; 

//funtion define
PRBQ read_buf_q_new();
void read_buf_q_free(PRBQ pr);

void evserver(char *argv[]);

void listener_cb(struct evconnlistener *listener, evutil_socket_t socket, struct sockaddr *addr, int socklen, void *arg);

void write_cb(struct bufferevent *bev, void *ctx);
void read_cb(struct bufferevent *bev, void *ctx);
void event_cb(struct bufferevent *bev,short events, void *ctx);

#endif
