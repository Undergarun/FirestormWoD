#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Toys/spell_toy.cpp
  Toys/npc_toy.cpp
#ifndef CROSS
  Toys/go_toy.cpp
#endif /* not CROSS */
)

message("  -> Prepared: Toys")
