#--------------------------------------------------------
# The CMakeLists.txt for:                       pRemoteVehicle
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  RemoteVehicle.cpp
  RemoteVehicle_Info.cpp
  MsgRemote.cpp
  main.cpp
)

ADD_EXECUTABLE(pRemoteVehicle ${SRC})

TARGET_LINK_LIBRARIES(pRemoteVehicle
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

