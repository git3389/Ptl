#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.tuxexec.mak                                          |
#| DESCRIPTIONS : Sample makefile for Tuxedo server                           |
#|----------------------------------------------------------------------------|

include $(MKHOME)/mak/platform.mak
include $(MKHOME)/mak/database.mak
include $(MKHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(MKHOME)/src/module/code1.o \
	$(MKHOME)/src/module/code2.o \
	$(MKHOME)/src/module/coden.o

PRGTARG = server
PRGLIBS = -lmodule $(DBLIBS) $(NETLIBS) $(UXLIBS) $(TUXLIBS)
PRGDEFS =

# used for TUXEDO
PRGSVCS = -s :_balmain
PRGRSMN = Oracle_XA

# used for db2 database
PRGDBNM =

debug all: debugtuxsvrexec
release: releasetuxsvrexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec releasetuxsvrexec debugtuxsvrexec clean:
	@make -f $(MKHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" TUXSVCS="$(PRGSVCS)" TUXRSMN="$(PRGRSMN)" DBNM="$(PRGDBNM)"
