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
#include "MapManager.h"
#include "../Draenor/tanaan_jungle.h"

/*######
## npc_deathly_usher
######*/

#define GOSSIP_ITEM_USHER "I would like to visit the Rise of the Defiler."

#define SPELL_TELEPORT_SINGLE           12885
#define SPELL_TELEPORT_SINGLE_IN_GROUP  13142
#define SPELL_TELEPORT_GROUP            27686

#define QUEST_START_DRAENOR             36881

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

/// OLDWorld Trigger (DO NOT DELETE) - 15384
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
                            if (l_Player->getLevel() < 90 || l_Player->isGameMaster())
                                continue;

                            /// If Quest suit isn't done, tp to tanaan. If done, tp to faction respective Staging Area.
                            if (l_Player->GetQuestStatus(34446) != QUEST_STATUS_REWARDED)
                                l_Player->TeleportTo(1265, 4066.7370f, -2381.9917f, 94.858f, 2.90f);
                            else
                            {
                                switch (l_Player->GetTeamId())
                                {
                                    case TEAM_ALLIANCE:
                                        l_Player->TeleportTo(1116, 3779.889f, -3888.14063f, 31.6778469f, 2.65f);
                                        break;
                                    case TEAM_HORDE:
                                        l_Player->TeleportTo(1116, 5200.19f, -3910.11f, 1.30844f, 0.588f);
                                        break;
                                    default:
                                        break;
                                }
                            }
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

/// Archmage Khadgar - 76643
class npc_archmage_khadgar_gossip : public CreatureScript
{
    public:
        npc_archmage_khadgar_gossip() : CreatureScript("npc_archmage_khadgar_gossip")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetTeamId() == TEAM_ALLIANCE)
            {
                if (p_Player->GetQuestStatus(35884) != QUEST_STATUS_REWARDED)
                {
                    p_Player->AddMovieDelayedTeleport(199, 1265, 4066.7370f, -2381.9917f, 94.858f, 2.90f);
                    p_Player->SendMovieStart(TanaanMovies::MovieEnterPortal);
                    p_Player->KilledMonsterCredit(78419);
                }
            }
            else if (p_Player->GetTeamId() == TEAM_HORDE)
            {
                if (p_Player->GetQuestStatus(34446) != QUEST_STATUS_REWARDED)
                {
                    p_Player->AddMovieDelayedTeleport(199, 1265, 4066.7370f, -2381.9917f, 94.858f, 2.90f);
                    p_Player->SendMovieStart(TanaanMovies::MovieEnterPortal);
                    p_Player->KilledMonsterCredit(78419);
                }
            }
            return true;
        }
};

/// Zidormi - 88206
class npc_blasted_lands_zidormi : public CreatureScript
{
    public:
        npc_blasted_lands_zidormi() : CreatureScript("npc_blasted_lands_zidormi") { }

        enum eSpell
        {
            TimeTravelling = 176111
        };

        enum eGossips
        {
            TraveledGossip  = 88207,
            BaseGossip      = 88206
        };

        enum eMaps
        {
            NewBlastedLands = 1190,
            EasternKingdoms = 0
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            p_Player->PlayerTalkClass->ClearMenus();
            p_Player->TalkedToCreature(p_Creature->GetEntry(), p_Creature->GetGUID());
            p_Player->PrepareGossipMenu(p_Creature, p_Player->HasAura(eSpell::TimeTravelling) ? eGossips::TraveledGossip : eGossips::BaseGossip, true);
            p_Player->SendPreparedGossip(p_Creature);
            return true;
        }

        struct npc_blasted_lands_zidormiAI : public ScriptedAI
        {
            npc_blasted_lands_zidormiAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                if (p_Player->HasAura(eSpell::TimeTravelling))
                {
                    p_Player->RemoveAura(eSpell::TimeTravelling);

                    uint64 l_Guid = p_Player->GetGUID();
                    sMapMgr->AddCriticalOperation([l_Guid]() -> bool
                    {
                        if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                            l_Player->SwitchToPhasedMap(eMaps::NewBlastedLands);

                        return true;
                    });
                }
                else
                {
                    p_Player->CastSpell(p_Player, eSpell::TimeTravelling, true);

                    uint64 l_Guid = p_Player->GetGUID();
                    sMapMgr->AddCriticalOperation([l_Guid]() -> bool
                    {
                        if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                            l_Player->SwitchToPhasedMap(eMaps::EasternKingdoms);

                        return true;
                    });
                }

                p_Player->PlayerTalkClass->SendCloseGossip();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_blasted_lands_zidormiAI(p_Creature);
        }
};

/// Dark Portal phasing
class PlayerScript_DarkPortal_Phasing: public PlayerScript
{
    public:
        PlayerScript_DarkPortal_Phasing() : PlayerScript("PlayerScript_DarkPortal_Phasing")
        {
            m_AlreadyInSwitchMapState = false;
        }

        bool m_AlreadyInSwitchMapState;

        enum eMaps
        {
            BLASTED_LANDS_DRAENOR_PHASE = 1190,
            EASTERN_KINGDOM_MAP_ID      = 0,
            BLASTER_LANDS_ZONE_ID       = 4
        };

        void OnUpdateZone(Player* p_Player, uint32 p_NewZoneID, uint32 p_OldZoneID, uint32 p_NewAreaID)
        {
            if (p_Player->GetMapId() == BLASTED_LANDS_DRAENOR_PHASE || p_Player->GetMapId() == EASTERN_KINGDOM_MAP_ID)
            {
                if (p_NewZoneID != p_OldZoneID && (p_NewZoneID == BLASTER_LANDS_ZONE_ID || p_OldZoneID == BLASTER_LANDS_ZONE_ID))
                {
                    uint64 l_PlayerGuid = p_Player->GetGUID();

                    if (p_NewZoneID == BLASTER_LANDS_ZONE_ID && p_Player->GetMapId() == EASTERN_KINGDOM_MAP_ID)
                    {
                        sMapMgr->AddCriticalOperation([l_PlayerGuid, p_NewZoneID]() -> bool
                        {
                            if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                                l_Player->SwitchToPhasedMap(BLASTED_LANDS_DRAENOR_PHASE);

                            return true;
                        });
                    }

                    if (p_NewZoneID != BLASTER_LANDS_ZONE_ID && p_Player->GetMapId() == BLASTER_LANDS_ZONE_ID)
                    {
                        sMapMgr->AddCriticalOperation([l_PlayerGuid, p_NewZoneID]() -> bool
                        {
                            if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                                l_Player->SwitchToPhasedMap(EASTERN_KINGDOM_MAP_ID);

                            return true;
                        });
                    }
                }
            }
        }
};

void AddSC_blasted_lands()
{
    new npc_deathly_usher();
    new npc_world_invisible_trigger();
    new npc_archmage_khadgar_gossip();
    new npc_blasted_lands_zidormi();

    /// Player script
    new PlayerScript_DarkPortal_Phasing();
}
