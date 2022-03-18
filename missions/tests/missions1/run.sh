#!/bin/bash -e
#-------------------------------------------------------
#  Run mission tests scripts
#-------------------------------------------------------

xterm -T "pMarineViewer" -e ./launch.sh&
xterm -T "mission tests script" -e ./test.sh&
