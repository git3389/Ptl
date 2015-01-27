#|----------------------------------------------------------------------------|
#| FILE NAME    : mksstand.mak                                                |
#| DESCRIPTIONS : Makefile for all kind of platform, for c and pc file,       |
#|                for lib(dynamic & static) and execuatable object            |
#|----------------------------------------------------------------------------|

include $(MKHOME)/mak/platform.mak
include $(MKHOME)/mak/database.mak
include $(MKHOME)/mak/tuxedo.mak

# environment variables must be define outside of makefile
#
# $MKHOME : home directory of application 
# $CCFLG  : compiler options for c source code
# $CXXFLG : compiler options for c++ source code
# $DLFLG  : link options for dynamic library
# $SLFLG  : link options for static library
# $EXFLG  : link options for excecuteable object
# $DBFLG  : debug mode options
# $OPFLG  : optimize mode options
# $DLFIX  : suffix of dynamic library
# $SLFIX  : suffix of static library

# $OBJS   : c object files list
# $LIBS   : libs required for executable object
# $DEFS   : define flag list
# $TARGET : target objetc name

# $TUXSVCS: tuxedo services name
# $TUXRSMN: tuxedo resource manager name

# $DBNM   : db name (for db2)

# commands
 
PC		=	proc
EC		=	esql
DB2		=	db2

JAVAC   =   javac
JAVAH   =   javah
RMIC    =   rmic

RM		=	rm -f
CP		=	cp
MV		=   mv
CD		=   cd
ECHO	=	@echo

# paths and flags

SYSINCP	=	/usr/include
SYSLIBP	=	
JINCP	=	
JMDINCP	=	

SRCP	=	$(MKHOME)/src
BINP	= 	$(MKHOME)/bin
TUXBINP =	$(MKHOME)/bin/tux
LIBP	=	$(MKHOME)/lib
INCP	=	$(MKHOME)/include
OBJP	=	$(MKHOME)/obj
CLSP    =	$(BINP)/classes
JLIBP   =	$(JAVA_HOME)/lib

INCPS	=	-I$(SYSINCP) -I$(INCP) -I$(DBINCP) -I$(TUXINCP) -I$(JINCP) -I$(JMDINCP) 
LIBPS	=	-L$(SYSLIBP) -L$(XLIBP) -L$(LIBP) $(DBLIBP) -L$(TUXLIBP)
 
CCFLGS	=	$(RLFLG) $(CCFLG) $(DEFS) $(TUXDEF) $(DBDEF) $(OSDEF) $(BITDEF) $(INCPS)
CXXFLGS =	$(RLFLG) $(CXXFLG) $(DEFS) $(TUXDEF) $(DBDEF) $(OSDEF) $(BITDEF) $(INCPS)
DLFLGS	=	$(DLFLG) $(LIBPS)
EXFLGS	=	$(EXFLG) $(LIBPS)
 
DLTARG	= $(LIBP)/lib$(TARGET)$(DLFIX)
SLTARG	= $(LIBP)/lib$(TARGET)$(SLFIX)
EXTARG	= $(BINP)/$(TARGET)
TUXTARG	= $(TUXBINP)/$(TARGET)

debugdynamic: setdebug $(DLTARG)
releasedynamic: setrelease $(DLTARG)
debugstatic: setdebug $(SLTARG)
releasestatic: setrelease $(SLTARG)
debugexec: setdebug $(EXTARG)
releaseexec : setrelease $(EXTARG)
debugtuxsvrexec: setdebug $(TUXTARG)
releasetuxsvrexec : setrelease $(TUXTARG)
javaclasses : $(OBJS)
	$(ECHO) "Finish java package [$(TARGET)]"
rmi :
	$(ECHO) "rmic [$(TARGET)] ..."
	@$(CD) `dirname $(TARGET)`; \
	$(RMIC) `basename $(TARGET)`

setdebug:
	$(ECHO) $(DBFLG) > $(MKHOME)/mak/.mak.tmp

setrelease:
	$(ECHO) $(OPFLG) > $(MKHOME)/mak/.mak.tmp

$(SLTARG): $(OBJS) 
	$(ECHO) "Linking   [$(SLTARG)] ..."
	@$(SLINK) $(SLFLG) $@ $(OBJS)
	@$(RM) $(MKHOME)/mak/.mak.tmp

$(DLTARG): $(OBJS) 
	$(ECHO) "Linking   [$(DLTARG)] ..."
	@if [ $(OSTYPE) = aix ]; \
	then \
		$(DLINK) -o $@ $(DLFLGS) $(OBJS) $(LIBS) $(UXLIBS); \
	else \
		$(DLINK) -o `basename $@` $(DLFLGS) $(OBJS) $(LIBS); \
		$(MV) `basename $@` `dirname $@`; \
	fi
	@$(RM) $(MKHOME)/mak/.mak.tmp

$(EXTARG): $(OBJS)
	$(ECHO) "Linking   [$(EXTARG)] ..."
	$(CC) -o $@ $(EXFLGS) $(OBJS) $$LIBS
	@$(RM) $(MKHOME)/mak/.mak.tmp

$(TUXTARG): $(OBJS)
	$(ECHO) "buildserver [$(TUXTARG)] ..."
	@TUXDIR="$(TUXDIR)" \
	CFLAGS="$(EXFLGS)" \
	$(TUXBS) -o $@ -f "$(OBJS)" -v $(TUXSVCS) -f "$(LIBS)" 
#-r $(TUXRSMN)

# implicit
 
.SUFFIXES:
.SUFFIXES: .java .class .xc .ec .pc .sqc .c .o
 
.xc.o:
	$(ECHO) "Compiling [$@] ..."
	@$(RM) $*$(DBFIX)
	@$(CP) $*.xc $*$(DBFIX)
	@if [ $(DBTYPE) = ora ]; \
	then \
		$(PC) define=$(DBD) include=($(DBINCP),$(TUXINCP),$(INCP),$(JINCP),$(JMDINCP)) code=ANSI_C iname=$*$(DBFIX) \
		$(CC) -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CCFLGS) -c $*.c; \
		$(RM) $*.lis; \
	else \
		$(EC) -e -ED$(DBD) $(INCPS) $*$(DBFIX); \
		$(CC) -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c; \
		$(RM) `basename $*`.c; \
	fi
	@$(RM) $*$(DBFIX) $*.c

.pc.o:
	$(ECHO) "Compiling [$@] ..."
	$(PC) define=$(DBD) include=$(INCP)  code=ANSI_C iname=$*$(DBFIX) sqlcheck=semantics userid=$(DB_USER)/$(DB_PASSWORD)
	$(CC) -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CCFLGS) -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/rdbms/demo -c $*.c
	$(RM) $*.lis $*.c tp*

.ec.o:
	$(ECHO) "Compiling [$@] ..."
	$(EC) -e -ED$(DBD) $(INCPS) $*$(DBFIX); \
	$(CC) -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c; \
	$(RM) `basename $*`.c; \

.sqc.o:
	$(ECHO) "Compiling [$@] ..."
	@$(RM) $*.bnd
	@$(DB2) connect to $(DBNM)
	@$(DB2) prep `basename $<` bindfile
	@$(DB2) bind `basename $*`.bnd
	@$(DB2) connect reset
	@$(DB2) terminate
	@$(MV) `basename $*`.bnd $(BNDDIR)
	@$(CC) -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c
	@$(RM) $*.c
	
.cpp.o:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@` ; \
	$(CXX) -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CXXFLGS) -c `basename $<`

.c.o:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@`; \
	$(CC) -g -W -o $@ `cat $(MKHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $<`
 
.java.class:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@`; \
	$(JAVAC) `basename $<`

# clearing object codes
 
clean: cleanup
	@$(RM) $(SLTARG) $(DLTARG) $(EXTARG) $(TUXTARG)
 	
cleanup:
	@$(RM) $(OBJS)
