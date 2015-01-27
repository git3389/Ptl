#|----------------------------------------------------------------------------|
#| FILE NAME     : database.mysql3.mak                                        |
#|----------------------------------------------------------------------------|

DBINCP = $(MYSQL_HOME)/include
DBLIBP = -L$(MYSQL_HOME)/lib 
DBLIBS = -lmysqlclient
DBDEF  = -DMYSQL3 -D_DB_MYSQL
DBTYPE = mysql
