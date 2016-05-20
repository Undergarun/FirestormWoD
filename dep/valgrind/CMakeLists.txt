#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

add_library(valgrind INTERFACE)

target_include_directories(valgrind
  INTERFACE
    "${VALGRIND_INCLUDE_DIR}")
