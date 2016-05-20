#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

file(GLOB sources *.c)

set(bzip2_STAT_SRCS
  ${sources}
)

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(bzip2 STATIC ${bzip2_STAT_SRCS})

set_property(TARGET bzip2 PROPERTY FOLDER "dep")