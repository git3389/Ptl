#|----------------------------------------------------------------------------|
#| FILE NAME     : database.ora8.mak                                          |
#|----------------------------------------------------------------------------|

DBINCP = .
DBLIBP = -L$(ORACLE_HOME)/lib
DBLIBS = -lclntsh -lclient8 `cat $(ORACLE_HOME)/lib/sysliblist`
DBD    = _DB_ORA
DBDEF  = -DORA8 -D$(DBD)
DBFIX  = .pc
DBTYPE = ora
