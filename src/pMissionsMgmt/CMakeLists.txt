#--------------------------------------------------------
# The CMakeLists.txt for:                       pMissionsMgmt
# Author(s):                              MarekB
#--------------------------------------------------------

SET(SRC
  MissionsMgmt.cpp
  MissionsMgmt_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pMissionsMgmt ${SRC})

TARGET_LINK_LIBRARIES(pMissionsMgmt
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   rmsg
   pthread)

