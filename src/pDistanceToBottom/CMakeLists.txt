#--------------------------------------------------------
# The CMakeLists.txt for:                       pDistanceToBottom
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  DistanceToBottom.cpp
  DistanceToBottom_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pDistanceToBottom ${SRC})

TARGET_LINK_LIBRARIES(pDistanceToBottom
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   geometry
   mbutil)

