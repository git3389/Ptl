#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.aix.mak                                           |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = xlC
DLINK = ld
SLINK = ar
DLFLG = -b64 -G -bE:$(MKHOME)/mak/exp/lib$(TARGET).exp -bnoentry
SLFLG = -X64 -ruc
CCFLG = -q64 -qchars=signed
CXXFLG=
EXFLG = -q64 -brtl
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
BITDEF = -DBIT64
OSTYPE = aix
JAVAMD = aix
