#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.aix.mak                                           |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = xlC
DLINK = ld
SLINK = ar
DLFLG = -G -bE:$(MKHOME)/mak/exp/lib$(TARGET).exp -bnoentry
SLFLG = ruc
CCFLG = -qchars=signed
CXXFLG=
EXFLG = -brtl
DBFLG = 
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lm 
DLLIBS = -ldl
NETLIBS =
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthreads
XLIBS   = -lXext -lX11
XLIBP   = .
OSDEF = -DAIX_UNIX
OSTYPE = aix
JAVAMD = aix
