# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


file(GLOB_RECURSE sources_Garrison Draenor/Garrison/*.hpp Draenor/Garrison/*.cpp Draenor/Garrison/*.hxx)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Garrison}
  Draenor/tanaan_jungle.cpp
  Draenor/tanaan_jungle.h
  Draenor/gorgrond.cpp
  Draenor/item_strongboxes.cpp
  Draenor/Skyreach/instance_skyreach.h
  Draenor/Skyreach/instance_skyreach.cpp
  Draenor/Skyreach/mob_skyreach.cpp
  Draenor/Skyreach/spell_skyreach.cpp
  Draenor/Skyreach/boss_ranjit.cpp
  Draenor/Skyreach/boss_araknath.cpp
  Draenor/Skyreach/boss_rukhran.cpp
  Draenor/Skyreach/boss_high_sage_viryx.cpp
  Draenor/BloodmaulSlagMines/instance_bloodmaul.h
  Draenor/BloodmaulSlagMines/instance_bloodmaul.cpp
  Draenor/BloodmaulSlagMines/mob_bloodmaul.cpp
  Draenor/BloodmaulSlagMines/spell_bloodmaul.cpp
  Draenor/BloodmaulSlagMines/areatrigger_bloodmaul.cpp
  Draenor/BloodmaulSlagMines/boss_slave_watcher_crushto.cpp
)

message("  -> Prepared: Draenor")
