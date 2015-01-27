#|----------------------------------------------------------------------------|
#| FILE NAME     : database.ora7.mak                                          |
#|----------------------------------------------------------------------------|

DBINCP = .
DBLIBP = -L$(ORACLE_HOME)/lib
DBLIBS = -lsql -lclient -lsqlnet -lcommon -lncr -lsqlnet -lgeneric -lepc -lnlsrtl3 -lcore3 -lnlsrtl3 -lcore3 -lc3v6
DBD    = _DB_ORA
DBDEF  = -DORA7 -D$(DBD)
DBFIX  = .pc
DBTYPE = ora
