#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Maps
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
  ${ZLIB_INCLUDE_DIR}
  ${ACE_INCLUDE_DIR}
)

add_executable(vmap4assembler VMapAssembler.cpp)
add_dependencies(vmap4assembler casc)

target_link_libraries(vmap4assembler
  collision
  g3dlib
  ${ACE_LIBRARY}
  ${ZLIB_LIBRARIES}
)

if( UNIX )
  install(TARGETS vmap4assembler DESTINATION bin)
elseif( WIN32 )
  install(TARGETS vmap4assembler DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

set_property(TARGET vmap4assembler PROPERTY FOLDER "tools")