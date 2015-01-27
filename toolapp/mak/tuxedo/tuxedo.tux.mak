#|----------------------------------------------------------------------------|
#| FILE NAME     : tuxedo.tux8.mak                                            |
#|----------------------------------------------------------------------------|

TUXINCP = $(TUXDIR)/include
TUXLIBP = $(TUXDIR)/lib
TUXLIBS = -lwsc -lbuft -lfml -lfml32 -lengine $(THREADLIBS)
TUXDEF = -DTUX -DNOWHAT
TUXBS  = buildserver

