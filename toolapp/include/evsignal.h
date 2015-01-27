#ifndef _EVSIGNAL_H
#define _EVSIGNAL_H

//system libary headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

//libary headers
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/util.h"

void evsignal();

void evsignal_cb(evutil_socket_t fd,short evs_flags,void *arg);

#endif 
