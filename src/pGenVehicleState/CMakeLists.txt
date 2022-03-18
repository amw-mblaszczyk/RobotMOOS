#--------------------------------------------------------
# The CMakeLists.txt for:                       pGenVehicleState
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  GenVehicleState.cpp
  GenVehicleState_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pGenVehicleState ${SRC})

TARGET_LINK_LIBRARIES(pGenVehicleState
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

