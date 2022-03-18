#--------------------------------------------------------
# The CMakeLists.txt for:                       pControlVehicleScanning
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  ControlVehicleScanning.cpp
  ControlVehicleScanning_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pControlVehicleScanning ${SRC})

TARGET_LINK_LIBRARIES(pControlVehicleScanning
   ${MOOS_LIBRARIES}
   apputil
   geometry
   mbutil
   m
   pthread)

