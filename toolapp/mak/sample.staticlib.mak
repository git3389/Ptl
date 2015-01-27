#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.staticlib.mak                                        |
#| DESCRIPTIONS : Sample makefile for static library                          |
#|----------------------------------------------------------------------------|

include $(MKHOME)/mak/platform.mak
include $(MKHOME)/mak/database.mak
include $(MKHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(MKHOME)/src/module/code1.o \
	$(MKHOME)/src/module/code2.o \
	$(MKHOME)/src/module/coden.o

PRGTARG = module
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugstatic
release: releasestatic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(MKHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
