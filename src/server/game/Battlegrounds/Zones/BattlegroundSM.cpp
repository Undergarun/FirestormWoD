/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "BattlegroundSM.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "ScriptedCreature.h"
#include "BattlegroundMgr.hpp"
#include "GossipDef.h"

BattlegroundSM::BattlegroundSM() : Battleground()
{
    m_BuffChange = true;
    BgObjects.resize(BG_SM_OBJECT_MAX);
    BgCreatures.resize(BG_SM_CREATURES_MAX);

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_SM_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SM_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_SM_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SM_HAS_BEGUN;
}

BattlegroundSM::~BattlegroundSM()
{
}

void BattlegroundSM::Reset()
{
    // Call parent's class reset
    Battleground::Reset();

    m_TeamScores[TEAM_ALLIANCE] = 0;
    m_TeamScores[TEAM_HORDE] = 0;
    m_HonorScoreTics[TEAM_ALLIANCE] = 0;
    m_HonorScoreTics[TEAM_HORDE] = 0;
    m_MineCartCheckTimer = 1000;
    m_FirstMineCartSummonTimer = 110 * IN_MILLISECONDS; // 10 sec before opening doors
    bool l_IsBGWeekEnd = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTics = (l_IsBGWeekEnd) ? BG_SM_SMWeekendHonorTicks : BG_SM_NotSMWeekendHonorTicks;
    m_IsInformedNearVictory = false;
    m_MineCartAddPointsTimer = 2000;
    m_FirstMineCartSpawned = false;
    m_WaterfallPathDone = false;
    m_TrackSwitch[SM_EAST_TRACK_SWITCH] = true;
    m_TrackSwitch[SM_NORTH_TRACK_SWITCH] = false;
    m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] = 3000;
    m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] = 3000;
    m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = true;
    m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = true;

    for (uint8 l_Iter = 0; l_Iter < SM_MINE_CART_MAX; ++l_Iter)
    {
        m_MineCartsProgressBar[l_Iter] = BG_SM_PROGRESS_BAR_NEUTRAL;
        m_MineCartReachedDepot[l_Iter] = false;
        m_MineCartNearDepot[l_Iter] = false;
        m_MineCartSpawned[l_Iter] = false;
    }

    for (uint8 l_Iter = 0; l_Iter < 4; ++l_Iter)
    {
        m_Depot[l_Iter] = false;
        m_DepotCloseTimer[l_Iter] = 3000;
    }

    for (uint8 l_Iter = 0; l_Iter < SM_MAX_PATHS; ++l_Iter)
        m_PathDone[l_Iter][l_Iter] = false;

    if (GetBgMap())
        GetBgMap()->SetObjectVisibility(1000.0f);   ///< Hardcode visibility to see carts on the whole map (shoudn't be a performance issue since we have cross realm)
}

void BattlegroundSM::PostUpdateImpl(uint32 p_Diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (m_FirstMineCartSpawned)
            BattlegroundSM::SummonMineCart(p_Diff);

        BattlegroundSM::CheckPlayerNearMineCart(p_Diff);
        BattlegroundSM::CheckMineCartNearDepot();
        BattlegroundSM::EventReopenDepot(p_Diff);
        BattlegroundSM::MineCartAddPoints(p_Diff);
        BattlegroundSM::CheckTrackSwitch();
        BattlegroundSM::MineCartsMoves();
    }

    if (!m_FirstMineCartSpawned)
        BattlegroundSM::FirstMineCartSummon(p_Diff);

    if (Creature* l_FirstCart = GetBGCreature(NPC_SM_MINE_CART_1)) ///< l_FirstCart is unused
        m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] = true;
    else
    {
        m_MineCartSpawned[BG_SM_MINE_CART_1 - 1] = false;
        m_PathDone[SM_EAST_PATH][0] = false;
        m_PathDone[SM_EAST_PATH][1] = false;
    }

    if (Creature* l_SecondCart = GetBGCreature(NPC_SM_MINE_CART_2)) ///< l_SecondCart is unused
        m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] = true;
    else
    {
        m_MineCartSpawned[BG_SM_MINE_CART_2 - 1] = false;
        m_WaterfallPathDone = false;
    }

    if (Creature* l_ThirdCart = GetBGCreature(NPC_SM_MINE_CART_3)) //< l_ThirdCart is unused
        m_MineCartSpawned[BG_SM_MINE_CART_3 - 1] = true;
    else
    {
        m_MineCartSpawned[BG_SM_MINE_CART_3 - 1] = false;
        m_PathDone[SM_NORTH_PATH][0] = false;
        m_PathDone[SM_NORTH_PATH][1] = false;
    }

    if (!m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH])
    {
        if (m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] <= 0)
        {
            if (Creature* l_TrackSwitch = GetBgMap()->GetCreature(BgCreatures[SM_TRACK_SWITCH_EAST]))
            {
                for (BattlegroundPlayerMap::const_iterator l_Iter = GetPlayers().begin(); l_Iter != GetPlayers().end(); ++l_Iter)
                {
                    if (Player* l_Player = ObjectAccessor::FindPlayer(l_Iter->first))
                    {
                        if (l_Player->GetExactDist2d(l_TrackSwitch->GetPositionX(), l_TrackSwitch->GetPositionY()) <= 10.0f)
                            l_Player->PlayerTalkClass->SendCloseGossip(); // Prevent from using multiple times track switches
                    }
                }

                l_TrackSwitch->RemoveAurasDueToSpell(BG_SM_PREVENTION_AURA);
                m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = true;
            }
        }
        else
            m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] -= p_Diff;
    }

    if (!m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH])
    {
        if (m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] <= 0)
        {
            if (Creature* l_TrackSwitch = GetBgMap()->GetCreature(BgCreatures[SM_TRACK_SWITCH_NORTH]))
            {
                for (BattlegroundPlayerMap::const_iterator l_Iter = GetPlayers().begin(); l_Iter != GetPlayers().end(); ++l_Iter)
                {
                    if (Player* l_Player = ObjectAccessor::FindPlayer(l_Iter->first))
                    {
                        if (l_Player->GetExactDist2d(l_TrackSwitch->GetPositionX(), l_TrackSwitch->GetPositionY()) <= 10.0f)
                            l_Player->PlayerTalkClass->SendCloseGossip(); // Prevent from using multiple times track switches
                    }
                }

                l_TrackSwitch->RemoveAurasDueToSpell(BG_SM_PREVENTION_AURA);
                m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = true;
            }
        }
        else
            m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] -= p_Diff;
    }
}

void BattlegroundSM::StartingEventCloseDoors()
{
    // Starting doors
    for (int l_DoorType = BG_SM_OBJECT_DOOR_A_1; l_DoorType <= BG_SM_OBJECT_DOOR_H_2; ++l_DoorType)
    {
        DoorClose(l_DoorType);
        SpawnBGObject(l_DoorType, RESPAWN_IMMEDIATELY);
    }

    for (uint8 l_Iter = BG_SM_OBJECT_WATERFALL_DEPOT; l_Iter < BG_SM_OBJECT_MAX; ++l_Iter)
        SpawnBGObject(l_Iter, RESPAWN_ONE_DAY);
}

void BattlegroundSM::CheckTrackSwitch()
{
    if (m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH])
    {
        if (Creature* l_TrackSwitch = GetBgMap()->GetCreature(BgCreatures[SM_TRACK_SWITCH_EAST]))
        {
            if (l_TrackSwitch->HasAura(BG_SM_TRACK_SWITCH_OPENED) && !m_TrackSwitch[SM_EAST_TRACK_SWITCH])
            {
                SendMessageToAll(LANG_BG_SM_EAST_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] = 3000;
                m_TrackSwitch[SM_EAST_TRACK_SWITCH] = true;
                m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = false;
            }

            if (l_TrackSwitch->HasAura(BG_SM_TRACK_SWITCH_CLOSED) && m_TrackSwitch[SM_EAST_TRACK_SWITCH])
            {
                SendMessageToAll(LANG_BG_SM_EAST_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                m_TrackSwitchClickTimer[SM_EAST_TRACK_SWITCH] = 3000;
                m_TrackSwitch[SM_EAST_TRACK_SWITCH] = false;
                m_TrackSwitchCanInterract[SM_EAST_TRACK_SWITCH] = false;
            }
        }
    }

    if (m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH])
    {
        if (Creature* l_TrackSwitch = GetBgMap()->GetCreature(BgCreatures[SM_TRACK_SWITCH_NORTH]))
        {
            if (l_TrackSwitch->HasAura(BG_SM_TRACK_SWITCH_CLOSED) && m_TrackSwitch[SM_NORTH_TRACK_SWITCH])
            {
                SendMessageToAll(LANG_BG_SM_NORTH_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] = 3000;
                m_TrackSwitch[SM_NORTH_TRACK_SWITCH] = false;
                m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = false;
            }

            if (l_TrackSwitch->HasAura(BG_SM_TRACK_SWITCH_OPENED) && !m_TrackSwitch[SM_NORTH_TRACK_SWITCH])
            {
                SendMessageToAll(LANG_BG_SM_NORTH_DIRECTION_CHANGED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                m_TrackSwitchClickTimer[SM_NORTH_TRACK_SWITCH] = 3000;
                m_TrackSwitch[SM_NORTH_TRACK_SWITCH] = true;
                m_TrackSwitchCanInterract[SM_NORTH_TRACK_SWITCH] = false;
            }
        }
    }
}

void BattlegroundSM::FirstMineCartSummon(uint32 p_Diff) ///< p_Diff is unused
{
    for (uint32 l_Cart = 0; l_Cart < BG_SM_MINE_CART_3; l_Cart++)
    {
        if (Creature* l_MineCart = AddCreature(BG_SM_CartIDs[l_Cart], BG_SM_CartTypes[l_Cart], 0, BG_SM_StartPos[l_Cart][0], BG_SM_StartPos[l_Cart][1], BG_SM_StartPos[l_Cart][2], BG_SM_StartPos[l_Cart][3]))
        {
            l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
            l_MineCart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
            l_MineCart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            l_MineCart->SetSpeed(MOVE_WALK, 0.8f);
            m_MineCartsProgressBar[l_Cart] = BG_SM_PROGRESS_BAR_NEUTRAL;
        }
    }

    m_FirstMineCartSpawned = true;
}

void BattlegroundSM::StartingEventOpenDoors()
{
    //Open doors
    for (int l_DoorType = BG_SM_OBJECT_DOOR_A_1; l_DoorType <= BG_SM_OBJECT_DOOR_H_2; ++l_DoorType)
        DoorOpen(l_DoorType);
    
    for (uint8 l_Iter = BG_SM_OBJECT_WATERFALL_DEPOT; l_Iter < BG_SM_OBJECT_MAX; ++l_Iter)
        SpawnBGObject(l_Iter, RESPAWN_IMMEDIATELY);
}

void BattlegroundSM::SummonMineCart(uint32 p_Diff) ///< p_Diff is unused
{
    for (int l_I = 0; l_I < BG_SM_MINE_CART_3; ++l_I)
    {
        if (!m_MineCartSpawned[l_I])
        {
            if (Creature* l_MineCart = AddCreature(BG_SM_CartIDs[l_I], BG_SM_CartTypes[l_I], 0, BG_SM_StartPos[l_I][0], BG_SM_StartPos[l_I][1], BG_SM_StartPos[l_I][2], BG_SM_StartPos[l_I][3]))
            {
                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                l_MineCart->SetUnitMovementFlags(MOVEMENTFLAG_BACKWARD);
                l_MineCart->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                l_MineCart->SetSpeed(MOVE_WALK, 0.8f);
                m_MineCartsProgressBar[l_I] = BG_SM_PROGRESS_BAR_NEUTRAL;
                SendMessageToAll(LANG_BG_SM_MINE_CART_SPAWNED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                PlaySoundToAll(BG_SM_SOUND_MINE_CART_SPAWNED);
            }
        }
    }
}

void BattlegroundSM::CheckPlayerNearMineCart(uint32 p_Diff)
{
    if (m_MineCartCheckTimer <= 0)
    {
        for (BattlegroundPlayerMap::const_iterator l_Iter = GetPlayers().begin(); l_Iter != GetPlayers().end(); ++l_Iter)
        {
            if (Player* l_Player = ObjectAccessor::FindPlayer(l_Iter->first))
            {
                if (l_Player->HasAuraType(AuraType::SPELL_AURA_MOD_STEALTH))
                    continue;

                if (l_Player->isDead()) 
                {
                    UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_DONT_SHOW, l_Player);
                    continue;
                }

                if (l_Player->GetBGTeam() == ALLIANCE)
                {
                    if (Creature* l_MineCart = l_Player->FindNearestCreature(NPC_MINE_CART_1, 22.0f, true))
                    {
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW, l_Player);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] >= 100)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] = 100;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1], l_Player);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]++;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1], l_Player);
                        }
                        
                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                            {
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_ALLIANCE, true);
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }
                    else if (Creature* l_MineCart = l_Player->FindNearestCreature(NPC_MINE_CART_2, 22.0f, true))
                    {
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW, l_Player);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] >= 100)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] = 100;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1], l_Player);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]++;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1], l_Player);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                            {
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_ALLIANCE, true);
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }
                    else if (Creature* l_MineCart = l_Player->FindNearestCreature(NPC_MINE_CART_3, 22.0f, true))
                    {
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW, l_Player);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] >= 100)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] = 100;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1], l_Player);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]++;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1], l_Player);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                            {
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_ALLIANCE, true);
                                SendMessageToAll(LANG_BG_SM_ALLIANCE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }
                    else
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_DONT_SHOW, l_Player);
                }
                else // for GetBGTeam() == HORDE
                {
                    if (Creature* l_MineCart = l_Player->FindNearestCreature(NPC_MINE_CART_1, 22.0f, true))
                    {
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW, l_Player);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] <= 0)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] = 0;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1], l_Player);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1]--;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1], l_Player);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                            {
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_HORDE, true);
                                SendMessageToAll(LANG_BG_SM_HORDE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }

                    else if (Creature* l_MineCart = l_Player->FindNearestCreature(NPC_MINE_CART_2, 22.0f, true))
                    {
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW, l_Player);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] <= 0)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] = 0;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1], l_Player);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1]--;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1], l_Player);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                            {
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_HORDE, true);
                                SendMessageToAll(LANG_BG_SM_HORDE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }
                    else if (Creature* l_MineCart = l_Player->FindNearestCreature(NPC_MINE_CART_3, 22.0f, true))
                    {
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_SHOW, l_Player);

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] <= 0)
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] = 0;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1], l_Player);
                        }
                        else
                        {
                            m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1]--;
                            UpdateWorldStateForPlayer(SM_PROGRESS_BAR_STATUS, m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1], l_Player);
                        }
                        
                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_NEUTRAL, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                            {
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_HORDE, true);
                                SendMessageToAll(LANG_BG_SM_HORDE_CONTROLLED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            }
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] == BG_SM_PROGRESS_BAR_NEUTRAL)
                        {
                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_ALLIANCE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_ALLIANCE, l_MineCart->GetGUID());

                            if (l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_HORDE))
                                l_MineCart->RemoveAurasDueToSpell(BG_SM_CONTROL_VISUAL_HORDE, l_MineCart->GetGUID());

                            if (!l_MineCart->HasAura(BG_SM_CONTROL_VISUAL_NEUTRAL))
                                l_MineCart->CastSpell(l_MineCart, BG_SM_CONTROL_VISUAL_NEUTRAL, true);
                        }
                    }
                    else
                        UpdateWorldStateForPlayer(SM_DISPLAY_PROGRESS_BAR, BG_SM_PROGRESS_BAR_DONT_SHOW, l_Player);
                }

                m_MineCartCheckTimer = 1000;
            }
        }
    }
    else
        m_MineCartCheckTimer -= p_Diff;
}

void BattlegroundSM::CheckMineCartNearDepot()
{
    if (!m_MineCartNearDepot[BG_SM_MINE_CART_1 - 1])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
        {
            if (l_MineCart->GetExactDist2d(BG_SM_DepotPos[SM_LAVA_DEPOT][0], BG_SM_DepotPos[SM_LAVA_DEPOT][1]) <= 6.0f)
            {
                m_Depot[SM_LAVA_DEPOT] = true;
                BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), BG_SM_MINE_CART_1);
                m_MineCartNearDepot[BG_SM_MINE_CART_1 - 1] = true;
            }

            if (l_MineCart->GetExactDist2d(BG_SM_DepotPos[SM_DIAMOND_DEPOT][0], BG_SM_DepotPos[SM_DIAMOND_DEPOT][1]) <= 6.0f)
            {
                m_Depot[SM_DIAMOND_DEPOT] = true;
                BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), BG_SM_MINE_CART_1);
                m_MineCartNearDepot[BG_SM_MINE_CART_1 - 1] = true;
            }
        }
    }

    if (!m_MineCartNearDepot[BG_SM_MINE_CART_2 - 1])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_2))
        {
            if (l_MineCart->GetExactDist2d(BG_SM_DepotPos[SM_WATERFALL_DEPOT][0], BG_SM_DepotPos[SM_WATERFALL_DEPOT][1]) <= 6.0f)
            {
                m_Depot[SM_WATERFALL_DEPOT] = true;
                BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_2), BG_SM_MINE_CART_2);
                m_MineCartNearDepot[BG_SM_MINE_CART_2 - 1] = true;
            }
        }
    }

    if (!m_MineCartNearDepot[BG_SM_MINE_CART_3 - 1])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
        {
            if (l_MineCart->GetExactDist2d(BG_SM_DepotPos[SM_DIAMOND_DEPOT][0], BG_SM_DepotPos[SM_DIAMOND_DEPOT][1]) <= 6.0f)
            {
                m_Depot[SM_DIAMOND_DEPOT] = true;
                BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), BG_SM_MINE_CART_3);
                m_MineCartNearDepot[BG_SM_MINE_CART_3 - 1] = true;
            }

            if (l_MineCart->GetExactDist2d(BG_SM_DepotPos[SM_TROLL_DEPOT][0], BG_SM_DepotPos[SM_TROLL_DEPOT][1]) <= 6.0f)
            {
                m_Depot[SM_TROLL_DEPOT] = true;
                BattlegroundSM::EventTeamCapturedMineCart(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), BG_SM_MINE_CART_3);
                m_MineCartNearDepot[BG_SM_MINE_CART_3 - 1] = true;
            }
        }
    }
}

void BattlegroundSM::EventTeamCapturedMineCart(uint32 p_Team, uint8 p_MineCart)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    for (BattlegroundPlayerMap::const_iterator l_Iter = GetPlayers().begin(); l_Iter != GetPlayers().end(); ++l_Iter)
    {
        if (Player* l_Player = ObjectAccessor::FindPlayer(l_Iter->first))
        {
            if (l_Player->GetBGTeam() == p_Team)
                if (l_Player->FindNearestCreature(NPC_MINE_CART_1, 22.0f, true) ||
                    l_Player->FindNearestCreature(NPC_MINE_CART_2, 22.0f, true) ||
                    l_Player->FindNearestCreature(NPC_MINE_CART_3, 22.0f, true))
                {
                    UpdatePlayerScore(l_Player, SCORE_CART_CONTROLLED, 1);
                    l_Player->RewardHonor(l_Player, 1, irand(10, 12));
                }
        }
    }

    switch (p_MineCart)
    {
        case BG_SM_MINE_CART_1:
        {
            if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
            {
                if (m_Depot[SM_LAVA_DEPOT])
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_LAVA_DEPOT]))
                    {
                        l_MineCart->StopMoving();
                        l_Depot->UseDoorOrButton();
                        m_MineCartReachedDepot[BG_SM_MINE_CART_1 - 1] = true;
                    }
                }

                if (m_Depot[SM_DIAMOND_DEPOT])
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                    {
                        l_MineCart->StopMoving();
                        l_Depot->UseDoorOrButton();
                        m_MineCartReachedDepot[BG_SM_MINE_CART_1 - 1] = true;
                    }
                }
            }
            break;
        }
        case BG_SM_MINE_CART_2:
        {
            if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_2))
            {
                if (m_Depot[SM_WATERFALL_DEPOT])
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_WATERFALL_DEPOT]))
                    {
                        l_MineCart->StopMoving();
                        l_Depot->UseDoorOrButton();
                        m_MineCartReachedDepot[BG_SM_MINE_CART_2 - 1] = true;
                    }
                }
            }
            break;
        }
        case BG_SM_MINE_CART_3:
        {
            if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
            {
                if (m_Depot[SM_DIAMOND_DEPOT])
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                    {
                        l_MineCart->StopMoving();
                        l_Depot->UseDoorOrButton();
                        m_MineCartReachedDepot[BG_SM_MINE_CART_3 - 1] = true;
                    }
                }

                if (m_Depot[SM_TROLL_DEPOT])
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_TROLL_DEPOT]))
                    {
                        l_MineCart->StopMoving();
                        l_Depot->UseDoorOrButton();
                        m_MineCartReachedDepot[BG_SM_MINE_CART_3 - 1] = true;
                    }
                }
            }
            break;
        }
    }
}

void BattlegroundSM::EventReopenDepot(uint32 p_Diff)
{
    if (m_MineCartReachedDepot[BG_SM_MINE_CART_1 - 1])
    {
        if (m_Depot[SM_LAVA_DEPOT])
        {
            if (m_DepotCloseTimer[SM_LAVA_DEPOT] <= 0)
            {
                if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_LAVA_DEPOT]))
                    {
                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE)
                        {
                            SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                        }

                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE)
                        {
                            SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                            BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), POINTS_PER_MINE_CART);

                        BattlegroundSM::ResetDepotsAndMineCarts(SM_LAVA_DEPOT, BG_SM_MINE_CART_1);
                        l_Depot->ResetDoorOrButton();
                        l_MineCart->DespawnOrUnsummon();
                        DelCreature(NPC_SM_MINE_CART_1);
                    }
                }
            }
            else
                m_DepotCloseTimer[SM_LAVA_DEPOT] -= p_Diff;
        }

        if (m_Depot[SM_DIAMOND_DEPOT])
        {
            if (m_DepotCloseTimer[SM_DIAMOND_DEPOT] <= 0)
            {
                if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                    {
                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == ALLIANCE)
                        {
                            SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                        }

                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_1) == HORDE)
                        {
                            SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_1 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                            BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_1), POINTS_PER_MINE_CART);
                            
                        BattlegroundSM::ResetDepotsAndMineCarts(SM_DIAMOND_DEPOT, BG_SM_MINE_CART_1);
                        l_Depot->ResetDoorOrButton();
                        l_MineCart->DespawnOrUnsummon();
                        DelCreature(NPC_SM_MINE_CART_1);
                    }
                }
            }
            else
                m_DepotCloseTimer[SM_DIAMOND_DEPOT] -= p_Diff;
        }
    }

    if (m_MineCartReachedDepot[BG_SM_MINE_CART_2 - 1])
    {
        if (m_Depot[SM_WATERFALL_DEPOT])
        {
            if (m_DepotCloseTimer[SM_WATERFALL_DEPOT] <= 0)
            {
                if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_2))
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_WATERFALL_DEPOT]))
                    {
                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == ALLIANCE)
                        {
                            SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                        }

                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_2) == HORDE)
                        {
                            SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_2 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                            BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_2), POINTS_PER_MINE_CART);
                            
                        BattlegroundSM::ResetDepotsAndMineCarts(SM_WATERFALL_DEPOT, BG_SM_MINE_CART_2);
                        l_Depot->ResetDoorOrButton();
                        l_MineCart->DespawnOrUnsummon();
                        DelCreature(NPC_SM_MINE_CART_2);
                    }
                }
            }
            else
                m_DepotCloseTimer[SM_WATERFALL_DEPOT] -= p_Diff;
        }
    }

    if (m_MineCartReachedDepot[BG_SM_MINE_CART_3 - 1])
    {
        if (m_Depot[SM_DIAMOND_DEPOT])
        {
            if (m_DepotCloseTimer[SM_DIAMOND_DEPOT] <= 0)
            {
                if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_DIAMOND_DEPOT]))
                    {
                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
                        {
                            SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                        }

                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
                        {
                            SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                            BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), POINTS_PER_MINE_CART);
                            
                        BattlegroundSM::ResetDepotsAndMineCarts(SM_DIAMOND_DEPOT, BG_SM_MINE_CART_3);
                        l_Depot->ResetDoorOrButton();
                        l_MineCart->DespawnOrUnsummon();
                        DelCreature(NPC_SM_MINE_CART_3);
                    }
                }
            }
            else
                m_DepotCloseTimer[SM_DIAMOND_DEPOT] -= p_Diff;
        }

        if (m_Depot[SM_TROLL_DEPOT])
        {
            if (m_DepotCloseTimer[SM_TROLL_DEPOT] <= 0)
            {
                if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
                {
                    if (GameObject* l_Depot = GetBgMap()->GetGameObject(BgObjects[BG_SM_OBJECT_TROLL_DEPOT]))
                    {
                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == ALLIANCE)
                        {
                            SendMessageToAll(LANG_BG_SM_ALLIANCE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_ALLIANCE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_ALLIANCE);
                        }

                        if (GetMineCartTeamKeeper(BG_SM_MINE_CART_3) == HORDE)
                        {
                            SendMessageToAll(LANG_BG_SM_HORDE_CAPTURED_MINE_CART, CHAT_MSG_BG_SYSTEM_HORDE);
                            PlaySoundToAll(BG_SM_SOUND_MINE_CART_CAPTURED_HORDE);
                        }

                        if (m_MineCartsProgressBar[BG_SM_MINE_CART_3 - 1] != BG_SM_PROGRESS_BAR_NEUTRAL)
                            BattlegroundSM::AddPoints(GetMineCartTeamKeeper(BG_SM_MINE_CART_3), POINTS_PER_MINE_CART);
                            
                        BattlegroundSM::ResetDepotsAndMineCarts(SM_TROLL_DEPOT, BG_SM_MINE_CART_3);
                        l_Depot->ResetDoorOrButton();
                        l_MineCart->DespawnOrUnsummon();
                        DelCreature(NPC_SM_MINE_CART_3);
                    }
                }
            }
            else
                m_DepotCloseTimer[SM_TROLL_DEPOT] -= p_Diff;
        }
    }
}

uint32 BattlegroundSM::GetMineCartTeamKeeper(uint8 p_MineCart)
{
    if (m_MineCartsProgressBar[p_MineCart - 1] > BG_SM_PROGRESS_BAR_NEUTRAL)
        return ALLIANCE;

    if (m_MineCartsProgressBar[p_MineCart - 1] < BG_SM_PROGRESS_BAR_NEUTRAL)
        return HORDE;

    return 0;
}

void BattlegroundSM::MineCartAddPoints(uint32 p_Diff)
{
    if (m_MineCartAddPointsTimer <= p_Diff)
    {
        uint8 l_AllianceCount = 0;
        uint8 l_HordeCount = 0;

        for (uint32 l_I = 1; l_I <= BG_SM_MINE_CART_3; ++l_I)
        {
            if (GetMineCartTeamKeeper(l_I) == ALLIANCE)
                ++l_AllianceCount;
            else if (GetMineCartTeamKeeper(l_I) == HORDE)
                ++l_HordeCount;
        }

        if (l_AllianceCount)
            BattlegroundSM::AddPoints(ALLIANCE, l_AllianceCount);

        if (l_HordeCount)
            BattlegroundSM::AddPoints(HORDE, l_HordeCount);
            

        m_MineCartAddPointsTimer = 1000;
    }
    else
        m_MineCartAddPointsTimer -= p_Diff;
}

void BattlegroundSM::ResetDepotsAndMineCarts(uint8 p_Depot, uint8 p_MineCart)
{
    m_DepotCloseTimer[p_Depot] = 3000;
    m_Depot[p_Depot] = false;
    m_MineCartsProgressBar[p_MineCart - 1] = BG_SM_PROGRESS_BAR_NEUTRAL;
    m_MineCartNearDepot[p_MineCart - 1] = false;
    m_MineCartReachedDepot[p_MineCart - 1] = false;
    m_MineCartSpawned[p_MineCart - 1] = false;
}

void BattlegroundSM::MineCartsMoves()
{
    if (!m_PathDone[SM_EAST_PATH][0])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
        {
            l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_1, false);
            m_PathDone[SM_EAST_PATH][0] = true;
        }
    }

    if (m_PathDone[SM_EAST_PATH][0] && !m_PathDone[SM_EAST_PATH][1])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
        {
            if (l_MineCart->GetExactDist2d(717.169312f, 114.258339f) < 0.5f) // East pos
            {
                if (Creature* l_Track = GetBgMap()->GetCreature(BgCreatures[SM_TRACK_SWITCH_EAST]))
                {
                    if (l_Track->HasAura(BG_SM_TRACK_SWITCH_OPENED))
                    {
                        l_MineCart->GetMotionMaster()->Clear(true);
                        l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_1 * 10, false);
                        m_PathDone[SM_EAST_PATH][1] = true;
                    }
                    else if ((!l_Track->HasAura(BG_SM_TRACK_SWITCH_OPENED) && !l_Track->HasAura(BG_SM_TRACK_SWITCH_CLOSED)) ||
                        l_Track->HasAura(BG_SM_TRACK_SWITCH_CLOSED))
                    {
                        l_MineCart->GetMotionMaster()->Clear(true);
                        l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_1 * 100, false);
                        m_PathDone[SM_EAST_PATH][1] = true;
                    }
                }
            }
        }
    }

    if (!m_WaterfallPathDone)
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_2))
        {
            l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_2, false);
            m_WaterfallPathDone = true;
        }
    }

    if (!m_PathDone[SM_NORTH_PATH][0])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
        {
            l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_3, false);
            m_PathDone[SM_NORTH_PATH][0] = true;
        }
    }

    if (m_PathDone[SM_NORTH_PATH][0] && !m_PathDone[SM_NORTH_PATH][1])
    {
        if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
        {
            if (l_MineCart->GetExactDist2d(834.727234f, 299.809753f) < 0.5f) // North pos
            {
                if (Creature* l_Track = GetBgMap()->GetCreature(BgCreatures[SM_TRACK_SWITCH_NORTH]))
                {
                    if ((!l_Track->HasAura(BG_SM_TRACK_SWITCH_OPENED) && !l_Track->HasAura(BG_SM_TRACK_SWITCH_CLOSED)) ||
                        l_Track->HasAura(BG_SM_TRACK_SWITCH_CLOSED))
                    {
                        l_MineCart->GetMotionMaster()->Clear(true);
                        l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_3 * 10, false);
                        m_PathDone[SM_NORTH_PATH][1] = true;
                    }
                    else
                    {
                        l_MineCart->GetMotionMaster()->Clear(true);
                        l_MineCart->GetMotionMaster()->MovePath(NPC_MINE_CART_3 * 100, false);
                        m_PathDone[SM_NORTH_PATH][1] = true;
                    }
                }
            }
        }
    }
}

void BattlegroundSM::AddPoints(uint32 p_Team, uint32 p_Points)
{
    BattlegroundTeamId l_TeamIndex = GetTeamIndexByTeamId(p_Team);
    m_TeamScores[l_TeamIndex] += p_Points;
    m_HonorScoreTics[l_TeamIndex] += p_Points;

    if (m_HonorScoreTics[l_TeamIndex] >= m_HonorTics)
    {
        RewardHonorToTeam(GetBonusHonorFromKill(6), p_Team, MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundObjectif);
        m_HonorScoreTics[l_TeamIndex] -= m_HonorTics;
    }

    UpdateTeamScore(p_Team);
}

void BattlegroundSM::UpdateTeamScore(uint32 p_Team)
{
    uint32 l_Score = GetTeamScore(p_Team);
    if (!m_IsInformedNearVictory && l_Score >= BG_SM_WARNING_NEAR_VICTORY_SCORE)
    {
        if (p_Team == ALLIANCE)
            SendMessageToAll(LANG_BG_SM_A_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
        else
            SendMessageToAll(LANG_BG_SM_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
        PlaySoundToAll(BG_SM_SOUND_NEAR_VICTORY);
        m_IsInformedNearVictory = true;
    }

    if (l_Score >= BG_SM_MAX_TEAM_SCORE)
    {
        l_Score = BG_SM_MAX_TEAM_SCORE;
        EndBattleground(p_Team);
    }

    if (p_Team ==  ALLIANCE)
        UpdateWorldState(SM_ALLIANCE_RESOURCES, l_Score);
    else
        UpdateWorldState(SM_HORDE_RESOURCES, l_Score);
}

void BattlegroundSM::EndBattleground(uint32 p_Winner)
{
    Battleground::EndBattleground(p_Winner);

    if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_1))
        l_MineCart->DespawnOrUnsummon();

    if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_2))
        l_MineCart->DespawnOrUnsummon();

    if (Creature* l_MineCart = GetBGCreature(NPC_SM_MINE_CART_3))
        l_MineCart->DespawnOrUnsummon();
}

void BattlegroundSM::AddPlayer(Player* p_Player)
{
    Battleground::AddPlayer(p_Player);
    //create score and add it to map
    BattlegroundSMScore* l_Score = new BattlegroundSMScore();

    PlayerScores[p_Player->GetGUID()] = l_Score;
}

bool BattlegroundSM::SetupBattleground()
{
    // doors
    if (!AddObject(BG_SM_OBJECT_WATERFALL_DEPOT, BG_SM_WATERFALL_DEPOT, BG_SM_DepotPos[SM_WATERFALL_DEPOT][0], BG_SM_DepotPos[SM_WATERFALL_DEPOT][1], BG_SM_DepotPos[SM_WATERFALL_DEPOT][2], BG_SM_DepotPos[SM_WATERFALL_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)    // Waterfall
        || !AddObject(BG_SM_OBJECT_LAVA_DEPOT, BG_SM_LAVA_DEPOT, BG_SM_DepotPos[SM_LAVA_DEPOT][0], BG_SM_DepotPos[SM_LAVA_DEPOT][1], BG_SM_DepotPos[SM_LAVA_DEPOT][2], BG_SM_DepotPos[SM_LAVA_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)                            // Lava
        || !AddObject(BG_SM_OBJECT_DIAMOND_DEPOT, BG_SM_DIAMOND_DEPOT, BG_SM_DepotPos[SM_DIAMOND_DEPOT][0], BG_SM_DepotPos[SM_DIAMOND_DEPOT][1], BG_SM_DepotPos[SM_DIAMOND_DEPOT][2], BG_SM_DepotPos[SM_DIAMOND_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)            // Diamond
        || !AddObject(BG_SM_OBJECT_TROLL_DEPOT, BG_SM_TROLL_DEPOT, BG_SM_DepotPos[SM_TROLL_DEPOT][0], BG_SM_DepotPos[SM_TROLL_DEPOT][1], BG_SM_DepotPos[SM_TROLL_DEPOT][2], BG_SM_DepotPos[SM_TROLL_DEPOT][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)                        // Troll
        || !AddObject(BG_SM_OBJECT_DOOR_A_1, BG_SM_DOOR, BG_SM_DoorPos[0][0], BG_SM_DoorPos[0][1], BG_SM_DoorPos[0][2], BG_SM_DoorPos[0][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_DOOR_A_2, BG_SM_DOOR, BG_SM_DoorPos[1][0], BG_SM_DoorPos[1][1], BG_SM_DoorPos[1][2], BG_SM_DoorPos[1][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_DOOR_H_1, BG_SM_DOOR, BG_SM_DoorPos[2][0], BG_SM_DoorPos[2][1], BG_SM_DoorPos[2][2], BG_SM_DoorPos[2][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_DOOR_H_2, BG_SM_DOOR, BG_SM_DoorPos[3][0], BG_SM_DoorPos[3][1], BG_SM_DoorPos[3][2], BG_SM_DoorPos[3][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_BERSERKING_BUFF_EAST, BG_OBJECTID_BERSERKERBUFF_ENTRY, BG_SM_BuffPos[0][0], BG_SM_BuffPos[0][1], BG_SM_BuffPos[0][2], BG_SM_BuffPos[0][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_BERSERKING_BUFF_WEST, BG_OBJECTID_BERSERKERBUFF_ENTRY, BG_SM_BuffPos[1][0], BG_SM_BuffPos[1][1], BG_SM_BuffPos[1][2], BG_SM_BuffPos[1][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_RESTORATION_BUFF_WATERFALL, BG_OBJECTID_REGENBUFF_ENTRY, BG_SM_BuffPos[2][0], BG_SM_BuffPos[2][1], BG_SM_BuffPos[2][2], BG_SM_BuffPos[2][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_SM_OBJECT_RESTORATION_BUFF_LAVA, BG_OBJECTID_REGENBUFF_ENTRY, BG_SM_BuffPos[3][0], BG_SM_BuffPos[3][1], BG_SM_BuffPos[3][2], BG_SM_BuffPos[3][3], 0, 0, 0.710569f, -0.703627f, RESPAWN_IMMEDIATELY))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundSM: Failed to spawn some object Battleground not created!");
        return false;
    }

    if (Creature* l_Tracker = AddCreature(NPC_TRACK_SWITCH_EAST, SM_TRACK_SWITCH_EAST, 0, BG_SM_TrackPos[SM_EAST_PATH][0], BG_SM_TrackPos[SM_EAST_PATH][1], BG_SM_TrackPos[SM_EAST_PATH][2], BG_SM_TrackPos[SM_EAST_PATH][3]))
        l_Tracker->CastSpell(l_Tracker, BG_SM_FEIGN_DEATH_STUN, true);
    else
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundSM: Failed to spawn some creatures Battleground not created!");
        return false;
    }

    if (Creature* l_Tracker = AddCreature(NPC_TRACK_SWITCH_NORTH, SM_TRACK_SWITCH_NORTH, 0, BG_SM_TrackPos[SM_NORTH_PATH][0], BG_SM_TrackPos[SM_NORTH_PATH][1], BG_SM_TrackPos[SM_NORTH_PATH][2], BG_SM_TrackPos[SM_NORTH_PATH][3]))
        l_Tracker->CastSpell(l_Tracker, BG_SM_FEIGN_DEATH_STUN, true);
    else
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundSM: Failed to spawn some creatures Battleground not created!");
        return false;
    }

    WorldSafeLocsEntry const* l_SafeLoc = sWorldSafeLocsStore.LookupEntry(SM_GRAVEYARD_MAIN_ALLIANCE);
    if (!l_SafeLoc || !AddSpiritGuide(SM_SPIRIT_ALLIANCE, l_SafeLoc->x, l_SafeLoc->y, l_SafeLoc->z, l_SafeLoc->o, ALLIANCE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundSM: Failed to spawn spirit guide! Battleground not created!");
        return false;
    }

    l_SafeLoc = sWorldSafeLocsStore.LookupEntry(SM_GRAVEYARD_MAIN_HORDE);
    if (!l_SafeLoc || !AddSpiritGuide(SM_SPIRIT_HORDE, l_SafeLoc->x, l_SafeLoc->y, l_SafeLoc->z, l_SafeLoc->o, HORDE))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundSM: Failed to spawn spirit guide! Battleground not created!");
        return false;
    }

    return true;
}

WorldSafeLocsEntry const* BattlegroundSM::GetClosestGraveYard(Player* p_Player)
{
    SMBattlegroundGaveyards l_BGGraveyard = p_Player->GetBGTeam() == ALLIANCE ? SM_GRAVEYARD_MAIN_ALLIANCE : SM_GRAVEYARD_MAIN_HORDE;
    WorldSafeLocsEntry const* l_SafeLoc = sWorldSafeLocsStore.LookupEntry(l_BGGraveyard);
    return l_SafeLoc;
}

void BattlegroundSM::HandleKillPlayer(Player* p_Player, Player* p_Killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    Battleground::HandleKillPlayer(p_Player, p_Killer);
    EventPlayerDroppedFlag(p_Player);
}

void BattlegroundSM::UpdatePlayerScore(Player* p_Player, uint32 p_Type, uint32 p_Value, bool p_AddHonor)
{
    BattlegroundScoreMap::iterator l_Iter = PlayerScores.find(p_Player->GetGUID());
    if (l_Iter == PlayerScores.end())                         // player not found
        return;

    switch (p_Type)
    {
        case SCORE_CART_CONTROLLED:                           // Mine Carts captures
            ((BattlegroundSMScore*)l_Iter->second)->MineCartCaptures += p_Value;
            break;
        default:
            Battleground::UpdatePlayerScore(p_Player, NULL, p_Type, p_Value, p_AddHonor);
            break;
    }
}

void BattlegroundSM::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << uint32(SM_MINE_CARTS_DISPLAY) << uint32(DISPLAY_WORLDSTATE);
    p_Data << uint32(SM_ALLIANCE_RESOURCES) << uint32(m_TeamScores[TEAM_ALLIANCE]);
    p_Data << uint32(SM_HORDE_RESOURCES) << uint32(m_TeamScores[TEAM_HORDE]);
    p_Data << uint32(SM_MINE_CART_1) << uint32(DISPLAY_WORLDSTATE);
    p_Data << uint32(SM_MINE_CART_2) << uint32(DISPLAY_WORLDSTATE);
    p_Data << uint32(SM_DISPLAY_ALLIANCE_RESSOURCES) << uint32(DISPLAY_WORLDSTATE);
    p_Data << uint32(SM_MINE_CART_3) << uint32(DISPLAY_WORLDSTATE);
    p_Data << uint32(SM_DISPLAY_HORDE_RESSOURCES) << uint32(DISPLAY_WORLDSTATE);
    p_Data << uint32(SM_DISPLAY_PROGRESS_BAR) << uint32(BG_SM_PROGRESS_BAR_DONT_SHOW); // This shows the mine cart control bar
    p_Data << uint32(SM_PROGRESS_BAR_STATUS) << uint32(BG_SM_PROGRESS_BAR_NEUTRAL); // Neutral
    p_Data << uint32(SM_UNK) << uint32(0);
}

void BattlegroundSM::EventPlayerClickedOnFlag(Player* p_Player, Unit* p_Target)
{
    if (GetStatus() != STATUS_IN_PROGRESS || !p_Player || !p_Target)
        return;

    if (p_Target->HasAura(BG_SM_PREVENTION_AURA)) // Prevention aura
        return;

    p_Target->CastSpell(p_Target, BG_SM_PREVENTION_AURA, true);

    if (!p_Target->HasAura(BG_SM_TRACK_SWITCH_OPENED) &&
        !p_Target->HasAura(BG_SM_TRACK_SWITCH_CLOSED))
        p_Target->CastSpell(p_Target, BG_SM_TRACK_SWITCH_OPENED, false);
    else if (p_Target->HasAura(BG_SM_TRACK_SWITCH_OPENED))
    {
        p_Target->CastSpell(p_Target, BG_SM_TRACK_SWITCH_CLOSED, false);
        p_Target->RemoveAura(BG_SM_TRACK_SWITCH_OPENED);
    }
    else if (p_Target->HasAura(BG_SM_TRACK_SWITCH_CLOSED))
    {
        p_Target->CastSpell(p_Target, BG_SM_TRACK_SWITCH_OPENED, false);
        p_Target->RemoveAura(BG_SM_TRACK_SWITCH_CLOSED);
    }
}
