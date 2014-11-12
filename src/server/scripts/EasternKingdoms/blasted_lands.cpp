/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Blasted_Lands
SD%Complete: 90
SDComment: Quest support: 3628. Teleporter to Rise of the Defiler missing group support.
SDCategory: Blasted Lands
EndScriptData */

/* ContentData
npc_deathly_usher
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

/*######
## npc_deathly_usher
######*/

#define GOSSIP_ITEM_USHER "I wish to to visit the Rise of the Defiler."

#define SPELL_TELEPORT_SINGLE           12885
#define SPELL_TELEPORT_SINGLE_IN_GROUP  13142
#define SPELL_TELEPORT_GROUP            27686

class npc_deathly_usher : public CreatureScript
{
public:
    npc_deathly_usher() : CreatureScript("npc_deathly_usher") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->CastSpell(player, SPELL_TELEPORT_SINGLE, true);
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(3628) == QUEST_STATUS_INCOMPLETE && player->HasItemCount(10757))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_USHER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

// OLDWorld Trigger (DO NOT DELETE) - 15384
class npc_world_invisible_trigger : public CreatureScript
{
    public:
        npc_world_invisible_trigger() : CreatureScript("npc_world_invisible_trigger") { }

        struct npc_world_invisible_triggerAI : public ScriptedAI
        {
            npc_world_invisible_triggerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_CheckPlayerTimer;

            void Reset()
            {
                if (me->GetMapId() == 1190)
                    m_CheckPlayerTimer = 1000;
                else
                    m_CheckPlayerTimer = 0;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_CheckPlayerTimer)
                {
                    if (m_CheckPlayerTimer <= p_Diff)
                    {
                        m_CheckPlayerTimer = 1000;

                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 15.0f);

                        for (Player* l_Player : l_PlayerList)
                        {
                            if (l_Player->getLevel() < 90)
                                continue;

                            if (l_Player->GetTeamId() == TEAM_ALLIANCE)
                                l_Player->TeleportTo(1116, 1585.22f, 104.39f, 65.76f, l_Player->GetOrientation());
                            else if (l_Player->GetTeamId() == TEAM_HORDE)
                                l_Player->TeleportTo(1116, 5859.56f, 4477.74f, 117.71f, l_Player->GetOrientation());
                        }
                    }
                    else
                        m_CheckPlayerTimer -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_world_invisible_triggerAI(p_Creature);
        }
};

// Dark Portal phasing
class PlayerScript_DarkPortal_Phasing : public PlayerScript
{
    public:
        PlayerScript_DarkPortal_Phasing() : PlayerScript("PlayerScript_DarkPortal_Phasing") { }

        enum eMaps
        {
            BLASTED_LANDS_DRAENOR_PHASE = 1190,
            EASTERN_KINGDOM_MAP_ID      = 0,
            BLASTER_LANDS_ZONE_ID       = 4
        };

        void OnUpdateZone(Player* p_Player, uint32 p_NewZoneID, uint32 p_OldZoneID, uint32 p_NewAreaID)
        {
            if (p_NewZoneID == BLASTER_LANDS_ZONE_ID && p_NewZoneID != p_OldZoneID)
                p_Player->SwitchToPhasedMap(BLASTED_LANDS_DRAENOR_PHASE);
            else if (p_Player->GetMapId() == BLASTED_LANDS_DRAENOR_PHASE && p_NewZoneID != BLASTER_LANDS_ZONE_ID && p_NewZoneID != p_OldZoneID)
                p_Player->SwitchToPhasedMap(EASTERN_KINGDOM_MAP_ID);
        }
};

void AddSC_blasted_lands()
{
    new npc_deathly_usher();
    new npc_world_invisible_trigger();

    /// Player script
    new PlayerScript_DarkPortal_Phasing();
}
