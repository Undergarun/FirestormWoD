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
	Custom/event_april.cpp
	Custom/duel_reset.cpp
	Custom/promo_new_players.cpp
	Custom/first_time_connexion.cpp
	Custom/user_reporting.cpp
	Custom/warning_update_client.cpp
	Custom/npc_custom.cpp
        Custom/stress_test.cpp
)

message("  -> Prepared: Custom")
