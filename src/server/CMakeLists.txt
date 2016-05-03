#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

# Enforce compileparameters for corebuilds under GCC
# This to stop a few silly crashes that could have been avoided IF people
# weren't doing some -O3 psychooptimizations etc.

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-fno-delete-null-pointer-checks)
endif()

if( WORLDSERVER )
  set(sources_Debugging
    ${CMAKE_SOURCE_DIR}/src/common/Debugging/WheatyExceptionReport.cpp
    ${CMAKE_SOURCE_DIR}/src/common/Debugging/WheatyExceptionReport.h
  )
  add_subdirectory(database)
  add_subdirectory(shared)
  add_subdirectory(game)
  add_subdirectory(scripts)
  add_subdirectory(worldserver)
endif()
