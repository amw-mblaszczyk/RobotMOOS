#--------------------------------------------------------
# The CMakeLists.txt for:                    lib_geometry
# Author(s):                                Mike Benjamin
#--------------------------------------------------------

SET(SRC
  SearchAreaTask.cpp
  )

SET(HEADERS
  SearchAreaTask.h
)

# Build Library
ADD_LIBRARY(rmsg ${SRC})

TARGET_LINK_LIBRARIES( rmsg
   geometry
   mbutil
   )

