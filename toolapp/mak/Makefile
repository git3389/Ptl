#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.main.mak                                             |
#| DESCRIPTIONS : Sample main makefile for product                            |
#|----------------------------------------------------------------------------|

release clean all debug:
	@make -f $(MKHOME)/src/Common/Makefile $@

ora7 ora8 ora9 ora10 infx7 db2 mysql3:
	@cp $(MKHOME)/mak/database/database.$@.mak $(MKHOME)/mak/database.mak

dux tux8 tux6 notux:
	@cp $(MKHOME)/mak/tuxedo/tuxedo.$@.mak $(MKHOME)/mak/tuxedo.mak

aix aix64 hpux hpux64 hpuxia64 scoos scouw nsux sunspc sunspc64 sunx86 sunamd64 osf rhx86 rhamd64:
	@cp $(MKHOME)/mak/platform/platform.$@.mak $(MKHOME)/mak/platform.mak

