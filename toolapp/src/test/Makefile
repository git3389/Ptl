#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.staticlib.mak                                        |
#| DESCRIPTIONS : Sample makefile for static library                          |
#|----------------------------------------------------------------------------|

include $(MKHOME)/mak/platform.mak
include $(MKHOME)/mak/database.mak
include $(MKHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(MKHOME)/src/test/run.o \
	$(MKHOME)/src/test/test_log.o \
	$(MKHOME)/src/test/test_util.o \
	$(MKHOME)/src/test/test_parse_json.o \
	$(MKHOME)/src/test/test_ptl_list.o \
	$(MKHOME)/src/test/test_ptl_cm.o \
	$(MKHOME)/src/test/test_ptl_pack.o \
	$(MKHOME)/src/test/test_ptl_get.o \
	$(MKHOME)/src/test/test_ptl_gen.o \
	$(MKHOME)/src/test/test_ipc_sem.o 


PRGTARG = testrun
PRGDEFS = 
PRGLIBS = -lPtlCm -ljansson -levent
# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(MKHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
