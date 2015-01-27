#|----------------------------------------------------------------------------|
#| FILE NAME     : database.ora8.mak                                          |
#|----------------------------------------------------------------------------|

DBINCP = .
DBLIBP = -L$(ORACLE_HOME)/lib
DBLIBS = -lclntsh -lclient9 `cat $(ORACLE_HOME)/lib/sysliblist`
DBD    = _DB_ORA
DBDEF  = -DORA9 -D$(DBD)
DBFIX  = .pc
DBTYPE = ora
