#### core dump setting ####
# Set core file max size(blocks)
ulimit -c 1048576
# Set stack mas size(kbytes)
ulimit -s 102400
# for aix
export CORE_NAMING=YES;


#App Env
export PATH=$PATH:$MKHOME/bin:$MKHOME/sbin:$MKHOME/run:.
export APP_PROFILE=$MKHOME/etc/app.cfg
export APP_PROFILE_PATH=$MKHOME/etc
export A_PROFILE_NAME=$MKHOME/etc/ptl.cfg
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MKHOME/lib


#set GBK
export LANG=ZN.CH
export NLS_LANG=American_America.ZHS16GBK




#Other env set
alias rm='rm -i'
alias l='ls -l'
alias la='ls -la'
alias cds='cd $MKHOME/src'
alias cdh='cd $MKHOME/include'
alias cdd='cd $MKHOME'
alias cdm='cd $MKHOME/mak'
alias cdb='cd $MKHOME/bin'
alias cdr='cd $MKHOME/run'
alias cde='cd $MKHOME/etc'
alias cdsb='cd $MKHOME/sbin'
alias cdl='cd $HOME/log'
alias log='cd $MKHOME/log'

alias findc='find . -name "*.c"|xargs grep'
alias findh='find . -name "*.h"|xargs grep'

#set -o vi
