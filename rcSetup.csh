#!/bin/tcsh

set myFilename_ToBe="rcSetup.csh"
set myFilename_sh="rcSetup.sh"
set bashBridge="tcsh_BridgeToBash.py"

set myName=""
set sourcedCmd=($_)

alias py_readlink "python -c 'import os,sys;print os.path.realpath(sys.argv[1])'"

if ( "$sourcedCmd" != "" ) then
   # get the directory of this script
   #---------------------------------
   set sourcedCmd=($_)
   set sourcePref=$sourcedCmd[1]
   unsetenv alias_rc

   # check if the saved cmd is a reuse of history (begin with !)
   echo $sourcePref | grep -q "^!" >&/dev/null
   if ( $? == 0 ) then
      set sourcedCmd=(`history -h 1`)
      set sourcePref=$sourcedCmd[1]
   endif

   if ( "$sourcePref" == "source" ) then
      set myName=$sourcedCmd[2]
   else
      set aliased_cmd=(`alias $sourcePref`)
      if ( $#aliased_cmd > 1 ) then
         if ( "$aliased_cmd[1]" == "source" ) then
            set myName=$aliased_cmd[2]
            setenv alias_rc "$sourcePref"
         endif
      endif
   endif
   if ( $?myName == 0 ) then
      echo "The script $myFilename_ToBe is not properly sourced from cmd=$sourcedCmd"
      exit 1
   endif

endif


# expand variable such as $PWD first in "myName" 
set myName=`eval echo $myName`
set myFilename=$myName:t
if ( "$myName" != "" ) then
   # set myName=`py_readlink $myName`
   set myDir=`dirname $myName`
   set myName_target=`py_readlink $myName`
   set myDir_target=`dirname $myName_target`
else
   set myDir=""
   set myDir_target=$myDir
endif
if ( "$myFilename" != "$myFilename_ToBe" ) then
   if ( $?rcSetup == 0) then
      echo "$myFilename_ToBe is NOT sourced directly from interactive shell, BUT from $myFilename"
      echo "please define variable 'rcSetup' to the path of $myFilename_ToBe"
      exit 1
   else
      set myDir=$rcSetup
      set myDir_target=$myDir
      if (! -e $myDir/$myFilename_ToBe) then
         echo "Script '$myFilename_ToBe' does not exist under $myDir"
         exit 1
      endif
   endif
endif

if ( $?alias_rc == 0) then
   setenv alias_rc "source $myDir/$myFilename_ToBe"
endif

set rcsetup_sim=0
set rcsetup_n=0
foreach opt ($*)
   @ rcsetup_n += 1
   switch ("$opt")   
     case --test:
         $myDir/test_rcSetup.csh;
         exit $?;;
     case --simulate:
         set rcsetup_sim=1;
         continue;
     case --simcsh:
         set rcsetup_sim=1;
         # shift;
         set argv[$rcsetup_n]=""
         continue;
     endsw
end
unset rcsetup_n


set setup_temp=/tmp/asg_setup_tcsh-$$

# set myName_target=`py_readlink $myName`
# set myDir_target=`dirname $myName_target`
python $myDir_target/$bashBridge $setup_temp $myDir/$myFilename_sh $*
unsetenv alias_rc

# echo "::tcsh:: setup_temp=$setup_temp"
# cat $setup_temp
if ($rcsetup_sim == 0) then
   if ( -f $setup_temp ) then
      source $setup_temp
      exit $?
   endif
else
   test -f $setup_temp && echo "rcSetup: simulate mode - generated tcsh script: $setup_temp"
endif
