#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.sunx86.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = CC
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG = -m32 
CXXFLG= -m32
EXFLG = -m32
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
XLIBS   = 
XLIBP   = 
OSDEF = -DSUNX86 -D_REENTRANT
BITDEF = -DBIT32
OSTYPE = sunos
JAVAMD = solaris
