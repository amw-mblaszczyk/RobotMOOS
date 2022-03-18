##############################################################################
# FILE:        moos-ivp-extend/src/CMakeLists.txt
# DATE:        2010/09/07
#              2020/05/09 minor mods
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-extend source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================

ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(lib_rmsg)
ADD_SUBDIRECTORY(pExampleApp)
ADD_SUBDIRECTORY(pXRelayTest)
ADD_SUBDIRECTORY(pRemote)
ADD_SUBDIRECTORY(pSearchArea)
ADD_SUBDIRECTORY(pMissionsMgmt)
ADD_SUBDIRECTORY(pJoystickTest)

# Tomek
ADD_SUBDIRECTORY(lib_behavior_new_waypoint)
ADD_SUBDIRECTORY(lib_behavior_keepDistanceToBottom)
ADD_SUBDIRECTORY(lib_behavior_AvoidCollisionUpDown)
ADD_SUBDIRECTORY(pTworzenieMisji)
ADD_SUBDIRECTORY(pSonar)
ADD_SUBDIRECTORY(pRemoteVehicle)
ADD_SUBDIRECTORY(pMyObstacleMgr)
ADD_SUBDIRECTORY(pMyMarinePID)
ADD_SUBDIRECTORY(pMissionVehicle)
ADD_SUBDIRECTORY(pGenVehicleState)
ADD_SUBDIRECTORY(pDistanceToBottom)
ADD_SUBDIRECTORY(pControlVehicleScanning)



##############################################################################
#                           END of CMakeLists.txt
##############################################################################

