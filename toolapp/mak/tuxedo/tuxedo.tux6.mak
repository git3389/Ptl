#|----------------------------------------------------------------------------|
#| FILE NAME     : tuxedo.tux6.mak                                            |
#|----------------------------------------------------------------------------|

TUXINCP = $(TUXDIR)/include
TUXLIBP = $(TUXDIR)/lib
TUXLIBS = -lwsc -lbuft -lnws -lnwi -lfml -lfml32 -lgp
TUXDEF = -DTUX6 -DNOWHAT
TUXBS  = buildserver

