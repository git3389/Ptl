#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.sunamd64.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = CC
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG = -m64 
CXXFLG= -m64
EXFLG = -m64
DBFLG = -g -D_DEBUG
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = 
DLLIBS = 
NETLIBS = 
CURLIBS = 
CRYLIBS =
THREADLIBS = 
XLIBS   = 
XLIBP   = 
OSDEF = -DSUNAMD64 -D_REENTRANT
BITDEF = -DBIT64
OSTYPE = sunos
JAVAMD = solaris
