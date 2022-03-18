#--------------------------------------------------------
# The CMakeLists.txt for:                       pTworzenieMisji
# Author(s):                              Tomasz Praczyk
#--------------------------------------------------------

SET(SRC
  TworzenieMisji.cpp
  TworzenieMisji_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTworzenieMisji ${SRC})

TARGET_LINK_LIBRARIES(pTworzenieMisji
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   geometry
   mbutil)

