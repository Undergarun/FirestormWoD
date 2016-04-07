#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

file(GLOB_RECURSE sources *.cpp *.h)

# uncomment next line to disable debug mode
add_definitions("-DIOMAP_DEBUG")

# build setup currently only supports libmpq 0.4.x
if( NOT MSVC )
  add_definitions("-Wall")
  add_definitions("-ggdb")
  add_definitions("-O3")
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/CascLib/src
)

add_executable(vmap4extractor ${sources})

target_link_libraries(vmap4extractor
  casc
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

add_dependencies(vmap4extractor casc)

if( UNIX )
  install(TARGETS vmap4extractor DESTINATION bin)
elseif( WIN32 )
  install(TARGETS vmap4extractor DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

set_property(TARGET vmap4extractor PROPERTY FOLDER "tools")