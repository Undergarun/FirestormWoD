#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

SET(zlib_STAT_SRCS
  adler32.c                                                                                                                                           
  compress.c                                                                                                                                          
  crc32.c                                                                                                                                             
  deflate.c                                                                                                                                           
  example.c                                                                                                                                           
  infback.c                                                                                                                                           
  inffast.c                                                                                                                                           
  inflate.c                                                                                                                                           
  inftrees.c                                                                                                                                          
  trees.c                                                                                                                                             
  uncompr.c                                                                                                                                           
  zutil.c   
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(zlib STATIC ${zlib_STAT_SRCS})

set_property(TARGET zlib PROPERTY FOLDER "dep")
