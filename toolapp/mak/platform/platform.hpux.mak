#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.hpux.mak                                          |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = aCC
DLINK = ld
SLINK = ar
DLFLG = -b +s
SLFLG = ruc
CCFLG = +z -Ae +DAportable +u1 -D_REENTRANT -D_XOPEN_SOURCE_EXTENDED
CXXFLG= +z -Ae +DAportable +u1
EXFLG = +z -Ae +DAportable +u1
DBFLG = 
OPFLG = -O
DLFIX = .sl
SLFIX = .a
UXLIBS = -lm 
DLLIBS =
NETLIBS =
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthread
XLIBS = -lXext -lX11
XLIBP = /usr/lib/X11R6
OSDEF = -DHP_UNIX
OSTYPE = hpux
JAVAMD = hp-ux
