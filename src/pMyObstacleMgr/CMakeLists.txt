#--------------------------------------------------------
# The CMakeLists.txt for:                    pObstacleMgr
# Author(s):                                   M.Benjamin   
#--------------------------------------------------------

SET(SRC
  MyObstacleManager.cpp
  MyObstacleManager_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pMyObstacleMgr ${SRC})

TARGET_LINK_LIBRARIES(pMyObstacleMgr
   ${MOOS_LIBRARIES}
   apputil
   geometry
   mbutil
   m
   pthread)

