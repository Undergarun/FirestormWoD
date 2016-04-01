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
file(GLOB_RECURSE sources_Highmaul Draenor/Highmaul/*.hpp Draenor/Highmaul/*.cpp Draenor/Highmaul/*.hxx)
file(GLOB_RECURSE sources_ShadowmoonBurialGrounds Draenor/ShadowmoonBurialGrounds/*.hpp Draenor/ShadowmoonBurialGrounds/*.cpp Draenor/ShadowmoonBurialGrounds/*.hxx) 
file(GLOB_RECURSE sources_IronDocks Draenor/IronDocks/*.hpp Draenor/IronDocks/*.cpp Draenor/IronDocks/*.hxx)
file(GLOB_RECURSE sources_Everbloom Draenor/Everbloom/*.hpp Draenor/Everbloom/*.cpp Draenor/Everbloom/*.hxx)
file(GLOB_RECURSE sources_Auchindoun Draenor/Auchindoun/*.hpp Draenor/Auchindoun/*.cpp Draenor/Auchindoun/*.hxx)
file(GLOB_RECURSE sources_Skyreach Draenor/Skyreach/*.hpp Draenor/Skyreach/*.cpp Draenor/Skyreach/*.hxx)
file(GLOB_RECURSE sources_BloodmaulSlagMines Draenor/BloodmaulSlagMines/*.hpp Draenor/BloodmaulSlagMines/*.cpp Draenor/BloodmaulSlagMines/*.hxx)
file(GLOB_RECURSE sources_BlackrockFoundry Draenor/BlackrockFoundry/*.hpp Draenor/BlackrockFoundry/*.cpp Draenor/BlackrockFoundry/*.hxx)
file(GLOB_RECURSE sources_ScenarioFateOfKarabor Draenor/FateOfKaraborScenario/*.hpp Draenor/FateOfKaraborScenario/*.cpp Draenor/FateOfKaraborScenario/*.hxx)
file(GLOB_RECURSE sources_GrimrailDepot Draenor/GrimrailDepot/*.hpp Draenor/GrimrailDepot/*.cpp Draenor/GrimrailDepot/*.hxx)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Garrison}
  ${sources_Highmaul}
  ${sources_IronDocks}
  ${sources_Auchindoun}
  ${sources_Skyreach}
  ${sources_Everbloom}
  ${sources_BloodmaulSlagMines}
  ${sources_BlackrockFoundry}
  ${sources_ShadowmoonBurialGrounds}
  ${sources_ScenarioFateOfKarabor}
  ${sources_GrimrailDepot}

  Draenor/draenor_shadowmoon_valley.cpp
  Draenor/draenor_shadowmoon_valley.hpp
  Draenor/tanaan_jungle.cpp
  Draenor/tanaan_jungle.h
  Draenor/frostfire_ridge.cpp
  Draenor/frostfire_ridge.h
  Draenor/spires_of_arak.cpp
  Draenor/spires_of_arak.h
  Draenor/gorgrond.cpp
  Draenor/item_strongboxes.cpp
)

message("  -> Prepared: Draenor")
