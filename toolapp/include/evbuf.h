#ifndef _EVBUF_H
#define _EVBUF_H

//system libary headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void evbuf(char *argv[]);


//callback
void listener_cb(struct evconnlistener *listener, evutil_socket_t socket, struct sockaddr *addr, int socklen, void *arg);
void read_cb(struct bufferevent *bev, void *ctx);
void write_cb(struct bufferevent *bev, void *ctx);
void event_cb(struct bufferevent *bev,short events, void *ctx);

#endif
