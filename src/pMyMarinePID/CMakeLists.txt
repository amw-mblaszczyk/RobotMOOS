#--------------------------------------------------------
# The CMakeLists.txt for:                      pMarinePID
# Author(s):                                Mike Benjamin
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  SET(SYSTEM_LIBS
    wsock32)
else (${WIN32})
  SET(SYSTEM_LIBS
    m
    pthread)
endif (${WIN32})

SET(SRC
   MarinePID.cpp
   MarinePID_Info.cpp
   PIDEngine.cpp
   ScalarPID.cpp
   main.cpp
)

ADD_EXECUTABLE(pMyMarinePID ${SRC})
   
TARGET_LINK_LIBRARIES(pMyMarinePID 
  ${MOOS_LIBRARIES}
  mbutil
   m
   pthread
   geometry
   mbutil
  ${SYSTEM_LIBS})

