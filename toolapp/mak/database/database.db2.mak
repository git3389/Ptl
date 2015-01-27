#|----------------------------------------------------------------------------|
#| FILE NAME     : database.db2.mak                                           |
#|----------------------------------------------------------------------------|

DBINCP = $(DB2_HOME)/include
DBLIBP = -L$(DB2_HOME)/lib
DBLIBS = -ldb2
DBD    = _DB_DB2
DBDEF  = -DDB2 -D$(DBD)
DBFIX  = .sqc
DBTYPE = db2
