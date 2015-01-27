#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.staticlib.mak                                        |
#| DESCRIPTIONS : Sample makefile for static library                          |
#|----------------------------------------------------------------------------|

include $(MKHOME)/mak/platform.mak
include $(MKHOME)/mak/database.mak
include $(MKHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(MKHOME)/src/Common/userlog.o \
	$(MKHOME)/src/Common/ptlpack.o \
	$(MKHOME)/src/Common/ptlutil.o \
	$(MKHOME)/src/Common/ptlcm.o \
	$(MKHOME)/src/Common/ptlipc.o \
	$(MKHOME)/src/Common/ptllist.o \
	$(MKHOME)/src/Common/ptlget.o \
	$(MKHOME)/src/Common/ptlmsgtimercb.o \
	$(MKHOME)/src/Common/ptlgen.o \
	$(MKHOME)/src/Common/ptlgenmsg.o \
	$(MKHOME)/src/Common/ptlput.o \


PRGTARG = PtlCm
PRGDEFS = 
PRGLIBS = -levent -ljansson 
# used for db2 database
PRGDBNM =

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(MKHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
