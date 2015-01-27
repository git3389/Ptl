#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.nsux.mak                                          |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   =
DLINK = ld
SLINK = ar
DLFLG = -shared
SLFLG = ruc
CCFLG = -shared -KPIC -signed -Wc,-align8
CXXFLG=
EXFLG = -call_shared -KPIC
DBFLG = 
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lgen -lm 
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS =
XLIBS   = -lXext -lX11
XLIBP   = .
OSDEF = -DNONSTOP_UNIX -DHAVENO_USLEEP -Dconst=""
OSTYPE = nsux
