# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
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
  Draenor/BloodmaulSlagMines/boss_forgemaster_gogduh.cpp
  Draenor/BloodmaulSlagMines/boss_roltall.cpp
  Draenor/BloodmaulSlagMines/boss_gugrokk.cpp
  Draenor/Garrison/GarrisonNPC.cpp
  Draenor/Garrison/GarrisonNPC_Horde.cpp
  Draenor/Garrison/GarrisonNPC_Alliance.cpp
  Draenor/Garrison/GarrisonQuest.cpp
  Draenor/Garrison/GarrisonSpell.cpp
  Draenor/Garrison/GarrisonGO.cpp
  Draenor/Garrison/GarrisonScriptData.hpp
  Draenor/Garrison/GarrisonNPC.hpp
  Draenor/Garrison/GarrisonQuest.hpp
  Draenor/Garrison/GarrisonSpell.hpp
  Draenor/Garrison/GarrisonGO.hpp
  Draenor/Garrison/Sites/GarrisonAllianceLevel1.cpp
  Draenor/Garrison/Sites/GarrisonAllianceLevel1.hpp
  Draenor/Garrison/Sites/GarrisonAllianceLevel2.cpp
  Draenor/Garrison/Sites/GarrisonAllianceLevel2.hpp
  Draenor/Garrison/Sites/GarrisonAllianceLevel3.cpp
  Draenor/Garrison/Sites/GarrisonAllianceLevel3.hpp
  Draenor/Garrison/Sites/GarrisonHordeLevel1.hpp
  Draenor/Garrison/Sites/GarrisonHordeLevel1.cpp
  Draenor/Garrison/Sites/GarrisonHordeLevel2.hpp
  Draenor/Garrison/Sites/GarrisonHordeLevel2.cpp
  Draenor/Garrison/Sites/GarrisonHordeLevel3.hpp
  Draenor/Garrison/Sites/GarrisonHordeLevel3.cpp
  Draenor/Garrison/Sites/GarrisonSiteBase.hpp
  Draenor/Garrison/Sites/GarrisonSiteBase.cpp
)

message("  -> Prepared: Draenor")
