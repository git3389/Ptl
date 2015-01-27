#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.staticlib.mak                                        |
#| DESCRIPTIONS : Sample makefile for static library                          |
#|----------------------------------------------------------------------------|

include $(MKHOME)/mak/platform.mak
include $(MKHOME)/mak/database.mak
include $(MKHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(MKHOME)/src/libevent/timeval/evtimer.o \
	$(MKHOME)/src/libevent/timeval/main.o


PRGTARG = evtimer
PRGDEFS = 
PRGLIBS = -levent -levent_core
# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(MKHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
