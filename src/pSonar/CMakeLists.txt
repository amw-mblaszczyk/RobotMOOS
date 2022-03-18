#--------------------------------------------------------
# The CMakeLists.txt for:                       pSonar
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  Sonar.cpp
  Sonar_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSonar ${SRC})

TARGET_LINK_LIBRARIES(pSonar
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

