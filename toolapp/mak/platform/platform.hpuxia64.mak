#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.hpux64.mak                                        |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = aCC
DLINK = ld
SLINK = ar
DLFLG = -b +s
SLFLG = ruc
CCFLG = +z -D_REENTRANT -D_XOPEN_SOURCE_EXTENDED
CXXFLG=
EXFLG = +z 
DBFLG = 
OPFLG = -O
DLFIX = .sl
SLFIX = .a
UXLIBS = -lm 
DLLIBS = -ldl
NETLIBS = -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthread
XLIBS = -lXext -lX11
XLIBP = /usr/lib/X11R6
OSDEF = -DHP_UNIX
BITDEF = +DD64
OSTYPE = hpux
JAVAMD = hp-ux
