#--------------------------------------------------------
# The CMakeLists.txt for:                       pJoystickTest
# Author(s):                              MB
#--------------------------------------------------------

SET(SRC
  JoystickTest.cpp
  main.cpp
)

ADD_EXECUTABLE(pJoystickTest ${SRC})

TARGET_LINK_LIBRARIES(pJoystickTest
pthread
)

