#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.sunos.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = CC
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG = -Xa -misalign
CXXFLG= -misalign -KPIC
EXFLG = -Xa -misalign
DBFLG = 
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lmalloc -lm 
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lrt -lpthread
XLIBS   = -lXext -lX11
XLIBP   = /usr/openwin/lib
OSDEF = -DSUNOS_UNIX -D_REENTRANT
OSTYPE = sunos
JAVAMD = solaris
