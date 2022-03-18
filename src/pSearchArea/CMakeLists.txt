#--------------------------------------------------------
# The CMakeLists.txt for:                       pSearchArea
# Author(s):                              m.blaszczyk
#--------------------------------------------------------

SET(SRC
  SearchAreaTask.cpp
  SearchArea.cpp
  SearchArea_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSearchArea ${SRC})

TARGET_LINK_LIBRARIES(pSearchArea
   ${MOOS_LIBRARIES}
   geometry
   mbutil
   m
   rmsg
   pthread)

