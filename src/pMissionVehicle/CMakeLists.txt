#--------------------------------------------------------
# The CMakeLists.txt for:                       pMissionVehicle
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  MissionVehicle.cpp
  MissionVehicle_Info.cpp
  SearchAreaTask.cpp
  main.cpp
)

ADD_EXECUTABLE(pMissionVehicle ${SRC})

TARGET_LINK_LIBRARIES(pMissionVehicle
   ${MOOS_LIBRARIES}
   apputil
   geometry
   mbutil
   m
   pthread)

