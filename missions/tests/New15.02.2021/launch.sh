#!/bin/bash -e
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1

for ARGI; do
    if [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 1
    fi
done

SNAME="shoreside" 
VNAME1="vehicle1" 
VNAME2="navigation1" 
VNAME3="vehicle2" 
VNAME4="navigation2" 
VNAME5="vehicle3" 
VNAME6="navigation3" 

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler vehicle1.moos >& /dev/null &
printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler navigation1.moos >& /dev/null &
printf "Launching $VNAME3 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler vehicle2.moos >& /dev/null &
printf "Launching $VNAME4 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler navigation2.moos >& /dev/null &
printf "Launching $VNAME5 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler vehicle3.moos >& /dev/null &
printf "Launching $VNAME6 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler navigation3.moos >& /dev/null &
printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler shoreside.moos >& /dev/null &
printf "Done \n"

uMAC shoreside.moos 

printf "Killing all processes ... \n"
mykill
printf "Done killing processes.   \n"
