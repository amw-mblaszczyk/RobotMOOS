#--------------------------------------------------------
# The CMakeLists.txt for:                       pRemote
# Author(s):                              m.blaszczyk
#--------------------------------------------------------

SET(SRC
  MsgRemote.cpp
  Joystick.cpp
  Remote.cpp
  Remote_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pRemote ${SRC})

TARGET_LINK_LIBRARIES(pRemote
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

