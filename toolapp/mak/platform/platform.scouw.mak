#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.scouw.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = g++
DLINK = ld
SLINK = ar
DLFLG = -b -G
SLFLG = ruc
CCFLG = -KPIC
CXXFLG= -Wall
EXFLG =
DBFLG = 
OPFLG = -O3
DLFIX = .so
SLFIX = .a
UXLIBS = -lm
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -Kthread
XLIBS   = -lXext -lX11 -lresolv
XLIBP   = .
OSDEF = -DSCOUW_UNIX
OSTYPE = scouw
JAVAMD = unixware
