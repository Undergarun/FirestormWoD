////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <ScriptPCH.h>
#include <ScriptMgr.h>

#include "GameEventMgr.h"

#define MAX_WEEKLY_EVENT                    7

class PlayerScript_Weekly_Event_Bonus : public PlayerScript
{
public:
    PlayerScript_Weekly_Event_Bonus() :PlayerScript("PlayerScript_Weekly_Event_Bonus") {}

    void OnLogin(Player* p_Player)
    {
        int32 l_EventTab[MAX_WEEKLY_EVENT] =
        {
            90, ///< Apexis Bonus Event
            91, ///< Arena Skirmish Bonus Event
            92, ///< Battleground Bonus Event
            93, ///< Draenor Dungeon Event
            94, ///< Pet Battle Bonus Event
            95, ///< (Northrend Timewalking Dungeon Event, Outland Timewalking Dungeon Event, Cataclysm Timewalking Dungeon Event)
            96  ///< (Northrend Timewalking Dungeon Event, Outland Timewalking Dungeon Event, Cataclysm Timewalking Dungeon Event)
        };

        int32 l_AuraTab[MAX_WEEKLY_EVENT] = {
            186400, ///< Sign of Apexis
            186401, ///< Sign of the Skirmisher
            186403, ///< Sign of Battle
            186404, ///< Sign of the Emissary
            186406, ///< Sign of the Critter
            0,      ///< No spell for Timewalking Event
            0       ///< No spell for Timewalking Event
        };

        for (uint8 i = 0; i < MAX_WEEKLY_EVENT; ++i)
        {
            /// There is 3 timewalking event (Northrend Timewalking Dungeon Event, Outland Timewalking Dungeon Event, Cataclysm Timewalking Dungeon Event)
            /// Devide on two occurence event (95, 96), there is no spell on timewalking event
            if (sGameEventMgr->IsActiveEvent(l_EventTab[i]))
            {
                if (l_AuraTab[i])
                {
                    p_Player->AddAura(l_AuraTab[i], p_Player);
                    break;
                }
            }
        }
    }

    void OnEnterBG(Player* p_Player, uint32 p_MapID)
    {
        int32 l_EventTab[MAX_WEEKLY_EVENT] =
        {
            90, ///< Apexis Bonus Event
            91, ///< Arena Skirmish Bonus Event
            92, ///< Battleground Bonus Event
            93, ///< Draenor Dungeon Event
            94, ///< Pet Battle Bonus Event
            95, ///< (Northrend Timewalking Dungeon Event, Outland Timewalking Dungeon Event, Cataclysm Timewalking Dungeon Event)
            96  ///< (Northrend Timewalking Dungeon Event, Outland Timewalking Dungeon Event, Cataclysm Timewalking Dungeon Event)
        };

        int32 l_AuraTab[MAX_WEEKLY_EVENT] = {
            186400, ///< Sign of Apexis
            186401, ///< Sign of the Skirmisher
            186403, ///< Sign of Battle
            186404, ///< Sign of the Emissary
            186406, ///< Sign of the Critter
            0,      ///< No spell for Timewalking Event
            0       ///< No spell for Timewalking Event
        };

        for (uint8 i = 0; i < MAX_WEEKLY_EVENT; ++i)
        {
            /// There is 3 timewalking event (Northrend Timewalking Dungeon Event, Outland Timewalking Dungeon Event, Cataclysm Timewalking Dungeon Event)
            /// Devide on two occurence event (95, 96), there is no spell on timewalking event
            if (sGameEventMgr->IsActiveEvent(l_EventTab[i]))
            {
                if (l_AuraTab[i])
                {
                    p_Player->AddAura(l_AuraTab[i], p_Player);
                    break;
                }
            }
        }
    }

    void OnLogout(Player* p_Player)
    {
        int32 l_AuraTab[MAX_WEEKLY_EVENT] = {
            186400, ///< Sign of Apexis
            186401, ///< Sign of the Skirmisher
            186403, ///< Sign of Battle
            186404, ///< Sign of the Emissary
            186406, ///< Sign of the Critter
            0,      ///< No spell for Timewalking Event
            0       ///< No spell for Timewalking Event
        };

        for (uint8 i = 0; i < MAX_WEEKLY_EVENT - 2; ++i)
        {
            p_Player->RemoveAura(l_AuraTab[i]);
        }
    }

    void OnleaveBG(Player* p_Player, uint32 p_MapID)
    {
        int32 l_AuraTab[MAX_WEEKLY_EVENT] = {
            186400, ///< Sign of Apexis
            186401, ///< Sign of the Skirmisher
            186403, ///< Sign of Battle
            186404, ///< Sign of the Emissary
            186406, ///< Sign of the Critter
            0,      ///< No spell for Timewalking Event
            0       ///< No spell for Timewalking Event
        };

        for (uint8 i = 0; i < MAX_WEEKLY_EVENT - 2; ++i)
        {
            p_Player->RemoveAura(l_AuraTab[i]);
        }
    }
};

void AddSC_WeeklyEventBonusScripts()
{
    new PlayerScript_Weekly_Event_Bonus();
};