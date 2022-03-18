#!/bin/bash -e
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------

printf "===================================================\n"
printf "This script tests missions functionality using , \n"
printf "First new mission 'mission_test1' is created. \n"
printf "Press<ENTER> to create mission..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=create, Name="mission_test1", Who=swarm, Speed=1.2, Distance=1.5, DepthDistance=5, lane_width=5, lane_offset=0.2, turn_way=20, Formation=triangle, StartPoint={10,10}, EndPoint={20,20}, Rectangle={-37.65544,-17.09936:-7.34456,0.40064:17.65544,-42.90064:-12.65544,-60.40064},SafetyReg={-100,-100:-75,-90:-50,-75:0,10:100,80:150,-99}, MaxTime=3600.00000"
printf "Check file 'mission_test1.mission' was created.\n"
printf "Now mission will be modiefied and stored. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=update, Name="mission_test1", Speed=1, Distance=1, Who=vehicle1"
printf "'mission_test1' updated.\n"
printf "Now mission will be stored. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=save, Name="mission_test1""
printf "Check file 'mission_test1.mission' was updated.\n"
printf "Now new mission 'mission_test2' is created. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=create, Name="mission_test2", Speed=1.2, Distance=1.5, Who=vehicle3"

printf "Check file 'mission_test2.mission' was created.\n"
printf "Now we list all missions. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=list"
printf "Check 'MISSION' message all missions are presented.\n"
printf "Now 'mission_test2' will be deleted. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=delete, Name="mission_test2" "
printf "Check file 'mission_test2.mission' was deleted.\n"
printf "Now 'mission_test1' will be read from file. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=read, Name="mission_test1" "
printf "Check warnings. \n"
printf "Now 'mission_test1' will be sent to vehicles. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION_CONF="Action=publish, Name="mission_test1" "
printf "Check warnings. \n"
printf "Now 'mission_test1' will be started. \n"
printf "Press<ENTER> to continue..."
read input_1
uPokeDB shoreside.moos MISSION="Action=start, Name="mission_test1" "

printf "Press<ENTER> to finish tests..."
read input_1