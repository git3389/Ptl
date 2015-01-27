#ifndef _SERVER_h
#define _SERVER_h

//system libary headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

//libary headers
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"

#define THREAD_MAX 50000
#define SEND_MSG   "{\"enentid\":\"EV_HEARTBEAT_REQ\"}"

typedef struct 
{
	struct bufferevent *bev;
	struct event *sndTimer;
	struct event_base *base;
	int nSndCount;
}PtlDef;

void sysinit(char *argv[]);
//相关回调
void read_cb(struct bufferevent *bev, void *ctx);
void write_cb(struct bufferevent *bev, void *ctx);
void event_cb(struct bufferevent *bev,short events, void *ctx);
#endif