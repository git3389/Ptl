#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.osf.mak                                           |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   =
DLINK = ld
SLINK = ar
DLFLG = -shared
SLFLG = ruc
CCFLG = -std -shared -assume noaligned_objects
CXXFLG=
EXFLG = -call_shared
DBFLG = 
OPFLG = -O3
DLFIX = .so
SLFIX = .a
UXLIBS = -lm 
DLLIBS = -ldl
NETLIBS = 
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS =
XLIBS   = -lXext -lX11
XLIBP   = /usr/X11R6/lib
OSDEF = -DOSF_UNIX -D_REENTRANT
OSTYPE = osf
