#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.main.mak                                             |
#| DESCRIPTIONS : Sample main makefile for product                            |
#|----------------------------------------------------------------------------|

release clean all debug:
	@make -f $(MKHOME)/src/module1/module1.mak $@
	@make -f $(MKHOME)/src/module2/module2.mak $@
	@make -f $(MKHOME)/src/modulen/modulen.mak $@
	@make -f $(MKHOME)/src/product.mak $@

ora7 ora8.0 ora8.1 infx7 db2 mysql3:
	@cp $(MKHOME)/mak/database/database.$@.mak $(MKHOME)/mak/database.mak

tux8 tux6 notux:
	@cp $(MKHOME)/mak/tuxedo/tuxedo.$@.mak $(MKHOME)/mak/tuxedo.mak

aix aix64 hpux hpux64 scoos scouw nsux sunos osf tblux rhlux:
	@cp $(MKHOME)/mak/platform/platform.$@.mak $(MKHOME)/mak/platform.mak

