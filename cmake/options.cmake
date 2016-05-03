#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

option(WORLDSERVER      "Build worldserver"                                           1)
option(SCRIPTS          "Build core with scripts included"                            1)
option(TOOLS            "Build map/vmap extraction/assembler tools"                   0)
option(USE_SCRIPTPCH    "Use precompiled headers when compiling scripts"              1)
option(USE_COREPCH      "Use precompiled headers when compiling server"               1)
option(WITH_WARNINGS    "Show all warnings during compile"                            0)
option(WITH_COREDEBUG   "Include additional debug-code in core"                       0)
set(WITH_SOURCE_TREE "no" CACHE STRING "Build the source tree for IDE's.")
set_property(CACHE WITH_SOURCE_TREE PROPERTY STRINGS no flat hierarchical)
option(WITHOUT_GIT      "Disable the GIT testing routines"                            0)
