#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.rhamd64.mak                                        |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = CC
DLINK = cc
SLINK = ar
DLFLG = -shared
SLFLG = ruc
CCFLG = -m64 -fPIC
CXXFLG= -m64
EXFLG = -m64 
DBFLG = -g
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = 
DLLIBS = 
NETLIBS = 
CURLIBS = 
CRYLIBS =
THREADLIBS = 
XLIBS =
XLIBP =
OSDEF = -DRHAMD64 -D_REENTRANT
BITDEF = -DBIT64
OSTYPE = redhatamd64
JAVAMD = 
