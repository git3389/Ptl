#ifndef _EVTIMER_H
#define _EVTIMER_H

//system libary headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//libary headers
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"
struct arg
{
	int num;
	struct event *timer;
	struct event_base *base;
};

void evtimer();

void evtimer_cb(evutil_socket_t fd,short evs_flags,void *arg);

#endif
