////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "Common.h"
#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "GossipDef.h"

#include "Buildings/Alliance/Large/ABarracks.hpp"
#include "Buildings/Alliance/Large/AGnomishGearworks.hpp"
#include "Buildings/Alliance/Large/ADwarvenBunker.hpp"
#include "Buildings/Alliance/Large/AMageTower.hpp"
#include "Buildings/Alliance/Large/AStables.hpp"
#include "Buildings/Alliance/Medium/ATradingPost.hpp"
#include "Buildings/Alliance/Medium/ALunarfallInn.hpp"
#include "Buildings/Alliance/Medium/ABarn.hpp"
#include "Buildings/Alliance/Medium/ALumberMill.hpp"
#include "Buildings/Alliance/Medium/AGladiatorsSanctum.hpp"
#include "Buildings/Alliance/Small/ATheForge.hpp"
#include "Buildings/Alliance/Small/ATailoringEmporium.hpp"
#include "Buildings/Alliance/Small/AAlchemyLab.hpp"
#include "Buildings/Alliance/Small/ATheTannery.hpp"
#include "Buildings/Alliance/Small/AEnchanterStudy.hpp"
#include "Buildings/Alliance/Small/AGemBoutique.hpp"
#include "Buildings/Alliance/Small/AEngineeringWorks.hpp"
#include "Buildings/Alliance/Small/AScribeQuarters.hpp"
#include "Buildings/Alliance/AFishingHut.hpp"
#include "Buildings/Alliance/ALunarfallExcavation.hpp"
#include "Buildings/Alliance/AHerbGarden.hpp"

#include "Buildings/Horde/Large/HWarMill.hpp"
#include "Buildings/Horde/Large/HGoblinWorkshop.hpp"
#include "Buildings/Horde/Large/HStables.hpp"
#include "Buildings/Horde/Large/HSpiritLodge.hpp"
#include "Buildings/Horde/Medium/HTradingPost.hpp"
#include "Buildings/Horde/Medium/HBarn.hpp"
#include "Buildings/Horde/Medium/HLumberMill.hpp"
#include "Buildings/Horde/Medium/HFrostwallTavern.hpp"
#include "Buildings/Horde/Small/HTheForge.hpp"
#include "Buildings/Horde/Small/HTailoringEmporium.hpp"
#include "Buildings/Horde/Small/HAlchemyLab.hpp"
#include "Buildings/Horde/Small/HTheTannery.hpp"
#include "Buildings/Horde/Small/HEnchanterStudy.hpp"
#include "Buildings/Horde/Small/HGemBoutique.hpp"
#include "Buildings/Horde/Small/HScribeQuarters.hpp"
#include "Buildings/Horde/Small/HEngineeringWorks.hpp"
#include "Buildings/Horde/HFishingHut.hpp"
#include "Buildings/Horde/HFrostwallMines.hpp"
#include "Buildings/Horde/HHerbGarden.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GarrisonFord::npc_GarrisonFord()
        : CreatureScript("npc_GarrisonFord")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_GarrisonFord::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (!p_Player->GetGarrison())
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::GarrisonCreation, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_GarrisonFord::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
    {
        if (p_Player->getLevel() >= 88 && !p_Player->GetGarrison())
        {
            p_Player->CLOSE_GOSSIP_MENU();
            p_Player->CreateGarrison();

            uint32 l_MovieID    = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->MovieID;
            uint32 l_MapID      = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->MapID;
            uint32 l_TeamID     = p_Player->GetTeamId();

            p_Player->AddMovieDelayedTeleport(l_MovieID, l_MapID,   gGarrisonCreationCoords[l_TeamID][0],
                                                                    gGarrisonCreationCoords[l_TeamID][1],
                                                                    gGarrisonCreationCoords[l_TeamID][2],
                                                                    gGarrisonCreationCoords[l_TeamID][3]);
            p_Player->SendMovieStart(l_MovieID);

            p_Player->RemoveRewardedQuest(Quests::Alliance_BiggerIsBetter);
            p_Player->RemoveRewardedQuest(Quests::Horde_BiggerIsBetter);

            if (l_TeamID == TEAM_ALLIANCE && p_Player->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_A) != QUEST_STATUS_REWARDED)
            {
                p_Player->AddQuest(sObjectMgr->GetQuestTemplate(Quests::QUEST_ETABLISH_YOUR_GARRISON_A), p_Creature);
                p_Player->CompleteQuest(Quests::QUEST_ETABLISH_YOUR_GARRISON_A);
            }
            else if (l_TeamID == TEAM_HORDE && p_Player->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_H) != QUEST_STATUS_REWARDED)
            {
                p_Player->AddQuest(sObjectMgr->GetQuestTemplate(Quests::QUEST_ETABLISH_YOUR_GARRISON_H), p_Creature);
                p_Player->CompleteQuest(Quests::QUEST_ETABLISH_YOUR_GARRISON_H);
            }

            /// HACK until shadowmoon quest are done : add follower Qiana Moonshadow / Olin Umberhide
            /// @Morgoporc Dont worry, this code will disappear
            p_Player->GetGarrison()->AddFollower(153);
            p_Player->GetGarrison()->AddFollower(92);
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_CallToArms::npc_CallToArms()
        : CreatureScript("npc_CallToArms_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_CallToArms::GetAI(Creature* p_Creature) const
    {
        return new npc_CallToArmsAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_CallToArms::npc_CallToArmsAI::npc_CallToArmsAI(Creature* p_Creature)
        : CreatureAI(p_Creature)
    {
        m_Owner = sObjectAccessor->FindPlayer(me->GetCreatorGUID());

        if (me->GetEntry() == NPCs::NPC_ARCHER || me->GetEntry() == NPCs::Marksman)
        {
            m_Ranged = true;
            me->m_CombatDistance    = 8;
            me->m_SightDistance     = 8;

            for (int8 i = 0; i < MAX_SPELL_SCHOOL; ++i)
                me->m_threatModifier[i] *= 0.001f;
        }
        else
        {
            m_Ranged = false;

            if (me->m_spells[0] && !me->m_spells[1])
                me->m_spells[1] = me->m_spells[0];

            if (me->m_spells[0] && !me->m_spells[2])
                me->m_spells[2] = me->m_spells[0];

            for (int8 i = 0; i < MAX_SPELL_SCHOOL; ++i)
                me->m_threatModifier[i] *= 10000.0f;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On reset
    void npc_CallToArms::npc_CallToArmsAI::Reset()
    {
        me->LoadEquipment(1, true);

        if (m_Owner && m_Owner->getVictim() && m_Owner->getVictim() != me->getVictim())
        {
            if (!m_Ranged)
                AttackStart(m_Owner->getVictim());
            else
                AttackStartCaster(m_Owner->getVictim(), 8);
        }
    }
    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_CallToArms::npc_CallToArmsAI::UpdateAI(const uint32 /*p_Diff*/)
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (m_Ranged)
        {
            if (me->IsWithinMeleeRange(me->getVictim(), 1.0f))
            {
                me->CastSpell(me, Spells::Disengage, TRIGGERED_FULL_MASK);
                me->resetAttackTimer();
            }

            DoSpellAttackIfReady(RAND(Spells::MultiShot, Spells::Shoot));
        }
        else
        {
            if (roll_chance_i(30))
                DoMeleeAttackIfReady();
            else
                DoSpellAttackIfReady(RAND(me->m_spells[0], me->m_spells[1], me->m_spells[2]));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_garrison_atheeru_palestar::npc_garrison_atheeru_palestar()
        : CreatureScript("npc_garrison_atheeru_palestar")
    {
    }

    /// Constructor
    npc_garrison_atheeru_palestarAI::npc_garrison_atheeru_palestarAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_garrison_atheeru_palestar::GetAI(Creature* p_Creature) const
    {
        return new npc_garrison_atheeru_palestarAI(p_Creature);
    }

    void npc_garrison_atheeru_palestarAI::SpawnAssemblies()
    {
        if (GetOwner() == nullptr)
            return;

        me->DespawnCreaturesInArea(82441);

        const SequencePosition l_SpawnPositions[4] =
        {
            /// Alliance
            { 24.2378f, -8.7516f, 0.0211f, 4.3490f },
            { 25.6718f, 11.0212f, 0.0001f, 4.4158f },
            /// Horde
            { 23.9077f, -4.0247f, -0.1916f, 4.1163f },
            { 25.5492f, 9.3343f, -0.0021f, 2.0483f }
        };

        if (GetOwner()->GetTeamId() == TEAM_ALLIANCE)
        {
            if (Creature* l_Creature = SummonRelativeCreature(82441, l_SpawnPositions[0].X, l_SpawnPositions[0].Y, l_SpawnPositions[0].Z, l_SpawnPositions[0].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                if (l_Creature->AI())
                    l_Creature->AI()->SetGUID(GetOwner()->GetGUID());
            }

            if (Creature* l_Creature = SummonRelativeCreature(82441, l_SpawnPositions[1].X, l_SpawnPositions[1].Y, l_SpawnPositions[1].Z, l_SpawnPositions[1].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                if (l_Creature->AI())
                    l_Creature->AI()->SetGUID(GetOwner()->GetGUID());
            }
        }
        else if (GetOwner()->GetTeamId() == TEAM_HORDE)
        {
            if (Creature* l_Creature = SummonRelativeCreature(82441, l_SpawnPositions[2].X, l_SpawnPositions[2].Y, l_SpawnPositions[2].Z, l_SpawnPositions[2].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                if (l_Creature->AI())
                    l_Creature->AI()->SetGUID(GetOwner()->GetGUID());
            }

            if (Creature* l_Creature = SummonRelativeCreature(82441, l_SpawnPositions[3].X, l_SpawnPositions[3].Y, l_SpawnPositions[3].Z, l_SpawnPositions[3].O, TEMPSUMMON_MANUAL_DESPAWN))
            {
                if (l_Creature->AI())
                    l_Creature->AI()->SetGUID(GetOwner()->GetGUID());
            }
        }
    }

    void npc_garrison_atheeru_palestarAI::OnPlotInstanceUnload()
    {
        std::list<Creature*> l_CreatureList;
        me->GetCreatureListWithEntryInGrid(l_CreatureList, 82441, 250.0f);

        for (Creature* l_Creature : l_CreatureList)
            l_Creature->DespawnOrUnsummon();
    }

    bool npc_garrison_atheeru_palestar::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "Bring back the assemblies.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "Do you want to respawn the assemblies ?", 10000, false);
        p_Player->SEND_GOSSIP_MENU(MiscDatas::NpcTextID, p_Creature->GetGUID());

        return true;
    }

    bool npc_garrison_atheeru_palestar::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1 && p_Creature->AI() && p_Player->HasEnoughMoney((int64)10000))
        {
            p_Player->ModifyMoney(-10000);
            p_Creature->AI()->DoAction(1);
        }
        return true;
    }

    void npc_garrison_atheeru_palestarAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
    {
        DoAction(1);
    }

    void npc_garrison_atheeru_palestarAI::DoAction(int32 const p_Action)
    {
        if (p_Action == 1)
            SpawnAssemblies();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_garrison_amperial_construct::npc_garrison_amperial_construct()
        : CreatureScript("npc_garrison_amperial_construct")
    {
    }

    /// Constructor
    npc_garrison_amperial_construct::npc_garrison_amperial_constructAI::npc_garrison_amperial_constructAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        m_OwnerGuid  = 0;
        m_CheckTimer = 1500;
        me->AddAura(Spells::SpellAuraAmperialConstructVisual, me);
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_garrison_amperial_construct::GetAI(Creature* p_Creature) const
    {
        return new npc_garrison_amperial_constructAI(p_Creature);
    }

    void npc_garrison_amperial_construct::npc_garrison_amperial_constructAI::OnSpellClick(Unit* p_Clicker)
    {
        if (m_OwnerGuid == p_Clicker->GetGUID())
            p_Clicker->CastSpell(me, 166052, true);
    }

    void npc_garrison_amperial_construct::npc_garrison_amperial_constructAI::PassengerBoarded(Unit* p_Passenger, int8 /*p_SeatID*/, bool p_Apply)
    {
        if (!p_Apply)
        {
            p_Passenger->RemoveAurasDueToSpell(166052);
            me->RemoveAurasDueToSpell(166052);
        }
    }

    void npc_garrison_amperial_construct::npc_garrison_amperial_constructAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (!p_Id)
            m_OwnerGuid = p_Guid;
        else if (p_Id == 1)
            m_CheckTimer = p_Guid;
    }

    void npc_garrison_amperial_construct::npc_garrison_amperial_constructAI::UpdateAI(const uint32 p_Diff)
    {
        if (m_CheckTimer)
        {
            if (m_CheckTimer <= p_Diff)
            {
                std::list<Player*> l_PlayerList;
                std::list<Creature*> l_CreatureList;

                GetCreatureListWithEntryInGrid(l_CreatureList, me, 82441, 100.0f);
                GetPlayerListInGrid(l_PlayerList, me, 2.0f);

                if (l_CreatureList.size() == 2)
                {
                    l_CreatureList.remove_if([this](Creature* p_Creature) -> bool
                    {
                        if (p_Creature->GetGUID() == me->GetGUID())
                            return true;

                        return false;
                    });

                    l_PlayerList.remove_if([this](Player* p_Player) -> bool
                    {
                        if (p_Player->GetGUID() != m_OwnerGuid)
                            return true;

                        return false;
                    });

                    if (l_PlayerList.empty() || !m_OwnerGuid || l_CreatureList.empty())
                        return;

                    if (Creature* l_Creature = l_CreatureList.front())
                    {
                        if (Player* l_Player = l_PlayerList.front())
                        {
                            if (l_Creature->AI() && l_Creature->IsInMap(me) && me->HasInArc(M_PI / 2, l_Player))
                            {
                                l_Creature->AI()->SetGUID(2000, 1);
                                l_Player->NearTeleportTo(l_Creature->m_positionX, l_Creature->m_positionY, l_Creature->m_positionZ, l_Creature->GetOrientation());
                            }
                        }
                    }
                }

                m_CheckTimer = 2000;
            }
            else
                m_CheckTimer -= p_Diff;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GarrisonStablesCreatures::npc_GarrisonStablesCreatures()
        : CreatureScript("npc_GarrisonStablesCreatures")
    {
    }

    /// Constructor
    npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::npc_GarrisonStablesCreaturesAI(Creature* p_Creature)
        : npc_escortAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_GarrisonStablesCreatures::GetAI(Creature* p_Creature) const
    {
        return new npc_GarrisonStablesCreaturesAI(p_Creature);
    }

    void npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::Reset()
    {
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetMaxPlayerDistance(200.0f);
        SetDespawnAtFar(false);
        SetDespawnAtEnd(false);
        SetEscortPaused(true);
    }

    void npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
    {
        if (p_SpellInfo && (p_SpellInfo->Id == StablesData::g_LassoAllianceAura || p_SpellInfo->Id == StablesData::g_LassoHordeAura))
        {
            Start(false, true, p_Caster->GetGUID());
            SetRun(true);
            me->SetWalk(false);
            SetEscortPaused(false);
        }
    }

    void npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::MovementInform(uint32 p_Type, uint32 p_ID)
    {
        npc_escortAI::MovementInform(p_Type, p_ID);

        if (p_Type != EFFECT_MOTION_TYPE)
            return;

        using namespace StablesData;

        switch (p_ID)
        {
            case CreatureJumps::MeadowstomperSecondJump:
                me->GetMotionMaster()->MoveJump(g_CreaturesJumps[2].x, g_CreaturesJumps[2].y, g_CreaturesJumps[2].z, 10.0f, 10.0f, me->GetOrientation(), CreatureJumps::MeadowstomperThirdJump);
                break;
            case CreatureJumps::SilverpeltFirstJump:
                me->GetMotionMaster()->MoveJump(g_CreaturesJumps[11].x, g_CreaturesJumps[11].y, g_CreaturesJumps[11].z, 10.0f, 10.0f, me->GetOrientation(), CreatureJumps::SilverpeltSecondJump);
                break;
            case CreatureJumps::MeadowstomperFirstJump:
            case CreatureJumps::MeadowstomperThirdJump:
            case CreatureJumps::SnarlerFirstJump:
            case CreatureJumps::SnarlerSecondJump:
            case CreatureJumps::SnarlerThirdJump:
            case CreatureJumps::SnarlerFourthJump:
            case CreatureJumps::SnarlerFifthJump:
            case CreatureJumps::SnarlerSixthJump:
            case CreatureJumps::SnarlerSeventhJump:
            case CreatureJumps::SilverpeltSecondJump:
                SetEscortPaused(false);
                break;
            default:
                break;

        }
    }

    void npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::WaypointReached(uint32 p_PointId)
    {
        using namespace StablesData;

        uint32 l_Index  = 0;
        uint32 l_JumpID = 0;

        switch (me->GetEntry())
        {
            case eCreaturesEntries::NpcMeadowstomper:
            {
                switch (p_PointId)
                {
                    case 17:
                        l_JumpID = CreatureJumps::MeadowstomperFirstJump;
                        break;
                    case 26:
                        l_Index = 1;
                        l_JumpID = CreatureJumps::MeadowstomperSecondJump;
                        break;
                    case 29:
                        StopEscortEvent(eKillCredits::ElekkKillCredit, g_LassoAllianceAura);
                        StopEscortEvent(eKillCredits::ElekkKillCredit, g_LassoHordeAura);
                        return;
                    default:
                        break;
                }
                break;
            }
            case eCreaturesEntries::NpcSnarler:
            {
                switch (p_PointId)
                {
                    case 4:
                        l_Index = 3;
                        l_JumpID = CreatureJumps::SnarlerFirstJump;
                        break;
                    case 5:
                        l_Index = 4;
                        l_JumpID = CreatureJumps::SnarlerSecondJump;
                        break;
                    case 7:
                        l_Index = 5;
                        l_JumpID = CreatureJumps::SnarlerThirdJump;
                        break;
                    case 17:
                        l_Index = 6;
                        l_JumpID = CreatureJumps::SnarlerFourthJump;
                        break;
                    case 21:
                        l_Index = 7;
                        l_JumpID = CreatureJumps::SnarlerFifthJump;
                        break;
                    case 23:
                        l_Index = 8;
                        l_JumpID = CreatureJumps::SnarlerSixthJump;
                        break;
                    case 24:
                        l_Index = 9;
                        l_JumpID = CreatureJumps::SnarlerSeventhJump;
                        break;
                    case 36:
                        StopEscortEvent(eKillCredits::WolfKillCredit, g_LassoAllianceAura);
                        StopEscortEvent(eKillCredits::WolfKillCredit, g_LassoHordeAura);
                        return;
                    default:
                        break;
                }
                break;
            }
            case eCreaturesEntries::NpcSilverpelt:
            {
                switch (p_PointId)
                {
                    case 24:
                        l_Index = 10;
                        l_JumpID = CreatureJumps::SilverpeltFirstJump;
                        break;
                    case 30:
                        StopEscortEvent(eKillCredits::TalbukKillCredit, g_LassoAllianceAura);
                        StopEscortEvent(eKillCredits::TalbukKillCredit, g_LassoHordeAura);
                        return;
                    default:
                        break;
                }
                break;
            }
            case eCreaturesEntries::NpcIcehoof:
            {
                if (p_PointId == 43)
                {
                    StopEscortEvent(eKillCredits::ClefthoofKillCredit, g_LassoAllianceAura);
                    StopEscortEvent(eKillCredits::ClefthoofKillCredit, g_LassoHordeAura);
                }
                return;
            }
            case eCreaturesEntries::NpcRiverwallow:
            {
                if (p_PointId == 28)
                {
                    StopEscortEvent(eKillCredits::RiverbeasttKillCredit, g_LassoAllianceAura);
                    StopEscortEvent(eKillCredits::RiverbeasttKillCredit, g_LassoHordeAura);
                }
                return;
            }
            case eCreaturesEntries::NpcRocktusk:
            {
                if (p_PointId == 45)
                {
                    StopEscortEvent(eKillCredits::BoarKillCredit, g_LassoAllianceAura);
                    StopEscortEvent(eKillCredits::BoarKillCredit, g_LassoHordeAura);
                }
                return;
            }
            default:
                break;
        }

        if (!l_JumpID)
            return;

        SetEscortPaused(true);
        me->GetMotionMaster()->MoveJump(g_CreaturesJumps[l_Index].x, g_CreaturesJumps[l_Index].y, g_CreaturesJumps[l_Index].z, 10.0f, 10.0f, me->GetOrientation(), l_JumpID);
    }

    void npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::StopEscortEvent(uint32 p_KillCredit, uint32 p_SpellID)
    {
        if (Player* l_Player = HashMapHolder<Player>::Find(GetEventStarterGUID()))
        {
            l_Player->KilledMonsterCredit(p_KillCredit);
            me->RemoveAura(p_SpellID);
            SetEscortPaused(true);
            me->DespawnOrUnsummon();
        }
    }

    void npc_GarrisonStablesCreatures::npc_GarrisonStablesCreaturesAI::UpdateAI(uint32 const p_Diff)
    {
        npc_escortAI::UpdateAI(p_Diff);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // Because blizzard does it this way - also icon type 27 could be for this purpose only

    npc_FleetCommandTable::npc_FleetCommandTable() : CreatureScript("npc_FleetCommandTable")
    {
    }

    bool npc_FleetCommandTable::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
    {
        if (!p_Player->GetGarrison())
            return true;

        p_Player->GetSession()->SendGarrisonSetMissionNpc(p_Creature->GetGUID());
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AncientTradingMechanism_Garr::npc_AncientTradingMechanism_Garr()
        : CreatureScript("npc_AncientTradingMechanism_Garr")
    {
    }

    bool npc_AncientTradingMechanism_Garr::OnQuestReward(Player* p_Player, Creature* /*p_Creature*/, const Quest* /*p_Quest*/, uint32 /*p_Option*/)
    {
        if (p_Player->GetGarrison() == nullptr)
            return true;

        std::vector<uint64> l_CreatureGuids = p_Player->GetGarrison()->GetBuildingCreaturesByBuildingType(BuildingType::TradingPost);

        for (std::vector<uint64>::iterator l_Itr = l_CreatureGuids.begin(); l_Itr != l_CreatureGuids.end(); l_Itr++)
        {
            if (Creature* l_Creature = sObjectAccessor->GetCreature(*p_Player, *l_Itr))
            {
                if (l_Creature->ToGarrisonNPCAI())
                {
                    GarrisonNPCAI* l_GarrisonAI = l_Creature->ToGarrisonNPCAI();

                    if (l_GarrisonAI != nullptr && l_GarrisonAI->GetOwner() != nullptr)
                        l_GarrisonAI->GetOwner()->GetGarrison()->UpdatePlot(l_GarrisonAI->GetPlotInstanceID());
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_follower_generic_script::npc_follower_generic_script()
        : CreatureScript("npc_follower_generic_script")
    {
    }

    bool npc_follower_generic_script::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_GarrisonMgr == nullptr || l_AI == nullptr)
            return true;

        uint32 l_BuildingPlotInstanceID = l_GarrisonMgr->GetCreaturePlotInstanceID(p_Creature->GetGUID());

        /// @TODO : if (!l_BuildingPlotInstanceID), script the possibility to get the follower escorting the player
        if (l_BuildingPlotInstanceID)
        {
            GarrisonBuilding l_Building = l_GarrisonMgr->GetBuilding(l_BuildingPlotInstanceID);

            switch (l_Building.BuildingID)
            {
                /// Gem Boutique has custom handling
                case Buildings::TailoringEmporium_TailoringEmporium_Level2:
                case Buildings::TailoringEmporium_TailoringEmporium_Level3:
                {
                    std::vector<RecipesConditions> l_Recipes;

                    switch (p_Player->GetTeamId())
                    {
                        case TEAM_HORDE:
                        {
                            l_Recipes =
                            {
                                { 176315, 0 },
                                { 176316, 0 }
                            };
                            break;
                        }
                        case TEAM_ALLIANCE:
                        {
                            l_Recipes =
                            {
                                { 176313, 0 },
                                { 176314, 0 }
                            };
                            break;
                        }
                        default:
                            break;
                    }

                    GarrisonNPCAI* l_GarrisonAI = p_Creature->ToGarrisonNPCAI();

                    if (l_GarrisonAI == nullptr)
                        return false;

                    l_GarrisonAI->SetRecipes(l_Recipes, SkillType::SKILL_TAILORING);
                    l_GarrisonAI->SendTradeSkillUI(p_Player);

                    break;
                }
                case Buildings::TheTannery_TheTannery_Level2:
                case Buildings::TheTannery_TheTannery_Level3:
                {
                    std::vector<RecipesConditions> l_Recipes;

                    switch (p_Player->GetTeamId())
                    {
                        case TEAM_HORDE:
                        {
                            l_Recipes =
                            {
                                { 176424, 0 },
                                { 176413, 0 },
                                { 176418, 0 }
                            };
                            break;
                        }
                        case TEAM_ALLIANCE:
                        {
                            l_Recipes =
                            {
                                { 176422, 0 },
                                { 176414, 0 },
                                { 176417, 0 }
                            };
                            break;
                        }
                        default:
                            break;
                    }

                    l_Recipes.push_back({ 176392, 0 }); ///< Fine Blue and Gold Tent
                    l_Recipes.push_back({ 176397, 0 }); ///< Fine Blue and Purple Tent
                    l_Recipes.push_back({ 176399, 0 }); ///< Fine Blue and Green Tent
                    l_Recipes.push_back({ 176401, 0 }); ///< Ironskin Tent
                    l_Recipes.push_back({ 176402, 0 }); ///< Outcast's Tent
                    l_Recipes.push_back({ 176404, 0 }); ///< Enchanter's Tent
                    l_Recipes.push_back({ 176405, 0 }); ///< Savage Leather Tent
                    l_Recipes.push_back({ 176408, 0 }); ///< Archmage's Tent
                    l_Recipes.push_back({ 176409, 0 }); ///< Brute's Tent
                    l_Recipes.push_back({ 176411, 0 }); ///< Sturdy Tent
                    l_Recipes.push_back({ 176412, 0 }); ///< Crusader's Tent
                    l_Recipes.push_back({ 176416, 0 }); ///< Patchwork Hut
                    l_Recipes.push_back({ 176415, 0 }); ///< Deathweaver's Hovel
                    l_Recipes.push_back({ 176426, 0 }); ///< Nomad's Spiked Tent
                    l_Recipes.push_back({ 176421, 0 }); ///< Distressingly Furry Tent
                    l_Recipes.push_back({ 176425, 0 }); ///< Voodoo Doctor's Hovel
                    l_Recipes.push_back({ 176420, 0 }); ///< Simple Tent

                    GarrisonNPCAI* l_GarrisonAI = p_Creature->ToGarrisonNPCAI();

                    if (l_GarrisonAI == nullptr)
                        return false;

                    l_GarrisonAI->SetRecipes(l_Recipes, SkillType::SKILL_LEATHERWORKING);
                    l_GarrisonAI->SendTradeSkillUI(p_Player);

                    break;
                }
                case Buildings::TheForge_TheForge_Level2:
                case Buildings::TheForge_TheForge_Level3:
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Grant me the blessings of the anvil and the forge.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());
                    break;
                case Buildings::AlchemyLab_AlchemyLab_Level2:
                case Buildings::AlchemyLab_AlchemyLab_Level3:
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(37270))
                    {
                        if (p_Player->CanTakeQuest(l_Quest, false) || p_Player->CanRewardQuest(l_Quest, false))
                            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);

                        p_Player->PlayerTalkClass->SendGossipMenu(1, p_Creature->GetGUID());
                    }
                    break;
                case Buildings::EnchanterStudy_EnchanterStudy_Level2:
                case Buildings::EnchanterStudy_EnchanterStudy_Level3:
                {
                    std::vector<RecipesConditions> l_Recipes =
                    {
                        { 173718, 0 },
                        { 174979, 0 },
                        { 173729, 0 },
                        { 173721, 0 },
                        { 173716, 0 },
                        { 173732, 0 },
                        { 173727, 0 },
                        { 175085, 0 },
                        { 175076, 0 },
                        { 175070, 0 },
                        { 173731, 0 },
                        { 173730, 0 },
                        { 175086, 0 },
                        { 175071, 0 },
///                        { 62948, 0 }, ///< not sure about this one
                        { 173728, 0 },
                        { 173724, 0 },
                        { 181870, 0 },
                        { 173725, 0 },
                        { 175078, 0 },
                        { 173723, 0 },
                        { 173721, 0 },
                        { 175072, 0 },
                        { 173722, 0 },
                        { 173720, 0 },
                        { 173717, 0 },
                        { 175074, 0 }
                    };

                    GarrisonNPCAI* l_GarrisonAI = p_Creature->ToGarrisonNPCAI();

                    if (l_GarrisonAI == nullptr)
                        return false;

                    /// There's also a SkillType SKILL_GARRENCHANTING, dunno what it's used for
                    l_GarrisonAI->SetRecipes(l_Recipes, SkillType::SKILL_ENCHANTING);
                    l_GarrisonAI->SendTradeSkillUI(p_Player);
                    break;
                }
                case Buildings::ScribesQuarters_ScribesQuarters_Level2:
                case Buildings::ScribesQuarters_ScribesQuarters_Level3:
                {
                    std::vector<RecipesConditions> l_Recipes =
                    {
                        { 176513, 0 }
                    };

                    GarrisonNPCAI* l_GarrisonAI = p_Creature->ToGarrisonNPCAI();

                    if (l_GarrisonAI == nullptr)
                        return false;

                    l_GarrisonAI->SetRecipes(l_Recipes, SkillType::SKILL_INSCRIPTION);
                    l_GarrisonAI->SendTradeSkillUI(p_Player);
                    break;
                }
                default:
                    break;
            }
        }
        else
            p_Creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        return true;
    }

    bool npc_follower_generic_script::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_GarrisonMgr == nullptr || l_AI == nullptr)
            return true;

        uint32 l_BuildingPlotInstanceID = l_GarrisonMgr->GetCreaturePlotInstanceID(p_Creature->GetGUID());

        if (l_BuildingPlotInstanceID)
        {
            GarrisonBuilding l_Building = l_GarrisonMgr->GetBuilding(l_BuildingPlotInstanceID);

            switch (l_Building.BuildingID)
            {
                case Buildings::TheForge_TheForge_Level2:
                case Buildings::TheForge_TheForge_Level3:
                {
                    if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
                    {
                        p_Player->AddAura(eSpells::SpellSongOfTheAnvil, p_Player);
                        p_Player->AddAura(eSpells::SpellSolaceOfTheForge, p_Player);
                    }

                    break;
                }
                default:
                    break;
            }
        }

        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_follower_generic_script::GetAI(Creature* p_Creature) const
    {
        return new npc_follower_generic_scriptAI(p_Creature);
    }

    /// Constructor
    npc_follower_generic_script::npc_follower_generic_scriptAI::npc_follower_generic_scriptAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_follower_generic_script::npc_follower_generic_scriptAI::Reset()
    {
        m_UpdateTimer = 10000;
        m_OwnerGUID   = 0;
    }

    void npc_follower_generic_script::npc_follower_generic_scriptAI::UpdateAI(uint32 p_Diff)
    {
        if (m_UpdateTimer)
        {
            if (m_UpdateTimer <= p_Diff)
            {
                if (Player* l_Player = HashMapHolder<Player>::Find(m_OwnerGUID))
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(37270))
                    {
                        if (l_Player->CanTakeQuest(l_Quest, false) || l_Player->CanRewardQuest(l_Quest, false))
                        {
                            if (l_Player->GetQuestStatus(37270) == QUEST_STATUS_NONE)
                                l_Player->PlayerTalkClass->SendQuestGiverStatus(__QuestGiverStatus::DIALOG_STATUS_AVAILABLE_REP, me->GetGUID());
                            else if (l_Player->GetQuestStatus(37270) == QUEST_STATUS_COMPLETE)
                                l_Player->PlayerTalkClass->SendQuestGiverStatus(__QuestGiverStatus::DIALOG_STATUS_REWARD_REP, me->GetGUID());
                            else
                                l_Player->PlayerTalkClass->SendQuestGiverStatus(__QuestGiverStatus::DIALOG_STATUS_NONE, me->GetGUID());
                        }
                    }
                }

                m_UpdateTimer = 1000;
            }
            else
                m_UpdateTimer -= p_Diff;
        }
    }

    void npc_follower_generic_script::npc_follower_generic_scriptAI::SetGUID(uint64 p_GUID, int32 p_ID)
    {
        if (p_ID == 1)
            m_OwnerGUID = p_GUID;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Fearsome Battle Standard (87594 / 86734)

    /// Constructor
    npc_FearsomeBattleStandard::npc_FearsomeBattleStandard()
        : CreatureScript("npc_FearsomeBattleStandard")
    {
    }

    /// Constructor
    npc_FearsomeBattleStandard::npc_FearsomeBattleStandardAI::npc_FearsomeBattleStandardAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_FearsomeBattleStandard::GetAI(Creature* p_Creature) const
    {
        return new npc_FearsomeBattleStandardAI(p_Creature);
    }

    void npc_FearsomeBattleStandard::npc_FearsomeBattleStandardAI::Reset()
    {
        m_Events.Reset();

        m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 1000);

        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
        me->DespawnOrUnsummon(60 * TimeConstants::IN_MILLISECONDS);
    }

    void npc_FearsomeBattleStandard::npc_FearsomeBattleStandardAI::UpdateAI(uint32 const p_Diff)
    {
        m_Events.Update(p_Diff);

        if (m_Events.ExecuteEvent() == eEvents::EventCheckPlayers)
        {
            float l_AuraRadius = 8.0f;
            float l_CheckRadius = l_AuraRadius + 5.0f;
            std::list<Unit*> l_TargetList;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(me, me, l_CheckRadius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(me, l_TargetList, l_Check);

            for (Unit* l_Unit : l_TargetList)
            {
                if (me->GetDistance(l_Unit) <= l_AuraRadius && !l_Unit->HasAura(eSpells::SpellAuraFearsomeBattleStandardPeriodicDmg))
                    me->AddAura(eSpells::SpellAuraFearsomeBattleStandardPeriodicDmg, l_Unit);
                else if (me->GetDistance(l_Unit) > l_AuraRadius && l_Unit->HasAura(eSpells::SpellAuraFearsomeBattleStandardPeriodicDmg))
                    l_Unit->RemoveAura(eSpells::SpellAuraFearsomeBattleStandardPeriodicDmg);
            }

            m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 1000);

        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Inspiring Battle Standard (88277 / 88010)

    /// Constructor
    npc_InspiringBattleStandard::npc_InspiringBattleStandard()
        : CreatureScript("npc_InspiringBattleStandard")
    {
    }

    /// Constructor
    npc_InspiringBattleStandard::npc_InspiringBattleStandardAI::npc_InspiringBattleStandardAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_InspiringBattleStandard::GetAI(Creature* p_Creature) const
    {
        return new npc_InspiringBattleStandardAI(p_Creature);
    }

    void npc_InspiringBattleStandard::npc_InspiringBattleStandardAI::Reset()
    {
        m_Events.Reset();

        m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 1000);

        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
        me->DespawnOrUnsummon(60 * TimeConstants::IN_MILLISECONDS);
    }

    void npc_InspiringBattleStandard::npc_InspiringBattleStandardAI::UpdateAI(uint32 const p_Diff)
    {

        m_Events.Update(p_Diff);

        if (m_Events.ExecuteEvent() == eEvents::EventCheckPlayers)
        {
            float l_AuraRadius = 8.0f;
            float l_CheckRadius = l_AuraRadius + 5.0f;
            std::list<Unit*> l_TargetList;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(me, me, l_CheckRadius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(me, l_TargetList, l_Check);

            for (Unit* l_Unit : l_TargetList)
            {
                if (me->GetDistance(l_Unit) <= l_AuraRadius && !l_Unit->HasAura(eSpells::SpellAuraInspiringBattleStandardPeriodicDmg))
                    me->AddAura(eSpells::SpellAuraInspiringBattleStandardPeriodicDmg, l_Unit);
                else if (me->GetDistance(l_Unit) > l_AuraRadius && l_Unit->HasAura(eSpells::SpellAuraInspiringBattleStandardPeriodicDmg))
                    l_Unit->RemoveAura(eSpells::SpellAuraInspiringBattleStandardPeriodicDmg);
            }

            m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 1000);

        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Inspiring Battle Standard (88277 / 88010)

    /// Constructor
    npc_LeatherWorkingTent_Garr::npc_LeatherWorkingTent_Garr()
        : CreatureScript("npc_LeatherWorkingTent_Garr")
    {
    }

    /// Constructor
    npc_LeatherWorkingTent_Garr::npc_LeatherWorkingTent_GarrAI::npc_LeatherWorkingTent_GarrAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        m_SummonerGuid = 0;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_LeatherWorkingTent_Garr::GetAI(Creature* p_Creature) const
    {
        return new npc_LeatherWorkingTent_GarrAI(p_Creature);
    }

    void npc_LeatherWorkingTent_Garr::npc_LeatherWorkingTent_GarrAI::Reset()
    {
        m_Events.Reset();

        m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 1000);

        std::map<uint32, uint32> l_AssociatedEntryAndAura =
        {
            { 86327, 172446 },
            { 86333, 172462 },
            { 86334, 172463 },
            { 86335, 172464 },
            { 86336, 172465 },
            { 86337, 172467 },
            { 86338, 172468 },
            { 86339, 172470 },
            { 86340, 172471 },
            { 86341, 172472 },
            { 86342, 172473 },
            { 86346, 172484 },
            { 86345, 172480 },
            { 86354, 172494 },
            { 86350, 172490 },
            { 86353, 172493 },
            { 86349, 172489 },
            { 86344, 172478 },
            { 86351, 172491 },
            { 86347, 172487 },
            { 86343, 172476 },
            { 86352, 172492 },
            { 86348, 172488 },
        };

        for (auto l_Association : l_AssociatedEntryAndAura)
        {
            if (l_Association.first == me->GetEntry())
                me->AddAura(l_Association.second, me);
        }

        me->SetDisplayId(11686);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        me->DespawnOrUnsummon(300 * TimeConstants::IN_MILLISECONDS);
    }

    void npc_LeatherWorkingTent_Garr::npc_LeatherWorkingTent_GarrAI::IsSummonedBy(Unit* p_Summoner)
    {
        m_SummonerGuid = p_Summoner->GetGUID();
    }

    void npc_LeatherWorkingTent_Garr::npc_LeatherWorkingTent_GarrAI::UpdateAI(uint32 const p_Diff)
    {
        m_Events.Update(p_Diff);

        if (m_Events.ExecuteEvent() == eEvents::EventCheckPlayers)
        {
            Player* l_Summoner = HashMapHolder<Player>::Find(m_SummonerGuid);
            float l_AuraRadius = 5.0f;
            float l_CheckRadius = l_AuraRadius + 20.0f;
            std::list<Player*> l_PlayerList;

            if (l_Summoner == nullptr)
            {
                m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 250);
                return;
            }

            GetPlayerListInGrid(l_PlayerList, me, l_CheckRadius);

            for (Player* l_Player : l_PlayerList)
            {
                if (l_Player->GetTeamId() != l_Summoner->GetTeamId())
                    continue;

                if (me->GetDistance(l_Player) <= l_AuraRadius && !l_Player->HasAura(eSpells::SpellAuraWellRestedTrackingAura))
                    me->AddAura(eSpells::SpellAuraWellRestedTrackingAura, l_Player);
                else if (me->GetDistance(l_Player) > l_AuraRadius && l_Player->HasAura(eSpells::SpellAuraWellRestedTrackingAura))
                    l_Player->RemoveAura(eSpells::SpellAuraWellRestedTrackingAura);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void npc_StablesTrainingMounts_Garr::npc_StablesTrainingMounts_GarrAI::IsSummonedBy(Unit* p_Summoner)
    {
        m_SummonerGUID = p_Summoner->GetGUID();
        p_Summoner->CastSpell(me, eSpells::SpellAuraRideVehicle, true);
    }

    void npc_StablesTrainingMounts_Garr::npc_StablesTrainingMounts_GarrAI::PassengerBoarded(Unit* p_Passenger, int8 /*p_SeatID*/, bool p_Apply)
    {
        if (Player* l_Player = HashMapHolder<Player>::Find(m_SummonerGUID))
        {
            if (MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison())
            {
                if (p_Apply)
                {
                    switch (l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(BuildingType::Stable)))
                    {
                        case 2:
                        {
                            if (l_Player->HasItemCount(eItems::ItemBlackClawOfSethe))
                                l_Player->AddAura(eSpells::SpellAuraBlackClawOfSethe, l_Player);

                            l_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_3, eUnitFlags3::UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                            break;
                        }
                        case 3:
                        {
                            if (l_Player->HasItemCount(eItems::ItemGarnToothNecklace))
                                l_Player->AddAura(eSpells::SpellAuraGarnToothNecklace, l_Player);

                            l_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_3, eUnitFlags3::UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                            break;
                        }
                        default:
                            break;
                    }
                }
                else
                {
                    l_Player->RemoveAura(eSpells::SpellAuraGarnToothNecklace);
                    l_Player->RemoveAura(eSpells::SpellAuraBlackClawOfSethe);
                    l_Player->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_3, UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                }
            }
        }
    }

    void npc_StablesTrainingMounts_Garr::npc_StablesTrainingMounts_GarrAI::JustDied(Unit* /*p_Killer*/)
    {
        if (Player* l_Player = HashMapHolder<Player>::Find(m_SummonerGUID))
            l_Player->SetUInt32Value(EUnitFields::UNIT_FIELD_FLAGS_3, 0);
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_StablesTrainingMounts_Garr::GetAI(Creature* p_Creature) const
    {
        return new npc_StablesTrainingMounts_GarrAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void npc_robot_rooster::npc_robot_roosterAI::EnterCombat(Unit* /*p_Attacker*/)
    {
        m_Events.ScheduleEvent(eDatas::EventBerserk, 5000);
        m_Events.ScheduleEvent(eDatas::EventNitroBoosts, 20000);
    }

    void npc_robot_rooster::npc_robot_roosterAI::EnterEvadeMode()
    {
        m_Events.Reset();
    }

    void npc_robot_rooster::npc_robot_roosterAI::UpdateAI(uint32 const p_Diff)
    {
        if (!UpdateVictim())
            return;

        m_Events.Update(p_Diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        switch (m_Events.ExecuteEvent())
        {
            case eDatas::EventBerserk:
                me->CastSpell(me, eDatas::SpellBerserk, false);
                events.ScheduleEvent(eDatas::EventBerserk, 30000);
                break;
            case eDatas::EventNitroBoosts:
                me->CastSpell(me, eDatas::SpellNitroBoosts, false);
                events.ScheduleEvent(eDatas::EventNitroBoosts, 30000);
                break;
            default:
                break;
        }

        DoMeleeAttackIfReady();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_GarrisonWalter::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr)
            return true;

        if (p_Player->GetGUID() == l_AI->GetGUID(eData::DataOwnerGUID))
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultBanker, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultTrader, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_GarrisonWalter::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        if (!p_Player->GetSession())
            return false;

        if (p_Action == GOSSIP_ACTION_INFO_DEF)
            p_Player->GetSession()->SendShowBank(p_Creature->GetGUID());
        else if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());

        return true;
    }

    void npc_GarrisonWalter::npc_GarrisonWalterAI::IsSummonedBy(Unit* p_Summoner)
    {
        m_SummonerGUID = p_Summoner->GetGUID();
    }

    uint64 npc_GarrisonWalter::npc_GarrisonWalterAI::GetGUID(int32 p_ID)
    {
        return p_ID == eData::DataOwnerGUID ? m_SummonerGUID : 0;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_GarrisonWalter::GetAI(Creature* p_Creature) const
    {
        return new npc_GarrisonWalterAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void npc_GearshopWorkshopTurret_Garr::npc_GearshopWorkshopTurret_GarrAI::IsSummonedBy(Unit* p_Summoner)
    {
        m_SummonerGUID = p_Summoner->GetGUID();
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_GearshopWorkshopTurret_Garr::GetAI(Creature* p_Creature) const
    {
        return new npc_GearshopWorkshopTurret_GarrAI(p_Creature);
    }

    void npc_GearshopWorkshopTurret_Garr::npc_GearshopWorkshopTurret_GarrAI::Reset()
    {
        m_SummonerGUID = 0;
        m_AttackTimer  = 0;
    }

    void npc_GearshopWorkshopTurret_Garr::npc_GearshopWorkshopTurret_GarrAI::EnterCombat(Unit* p_Attacker)
    {
        m_AttackTimer = 500;
    }

    void npc_GearshopWorkshopTurret_Garr::npc_GearshopWorkshopTurret_GarrAI::UpdateAI(uint32 const p_Diff)
    {

        if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (m_AttackTimer)
        {
            if (m_AttackTimer <= p_Diff)
            {
                if (Unit* l_Victim = me->getVictim())
                    me->CastSpell(l_Victim, eSpells::SpellTurretFire, false);

                m_AttackTimer = 4000;
            }
            else
                m_AttackTimer -= p_Diff;
        }

        DoMeleeAttackIfReady();
    }

}   ///< namespace Garrison
}   ///< namespace MS

#ifndef __clang_analyzer__
void AddSC_Garrison_NPC()
{
    /// Generic
    new MS::Garrison::npc_GarrisonFord;
    new MS::Garrison::npc_CallToArms;
    new MS::Garrison::npc_garrison_amperial_construct;
    new MS::Garrison::npc_garrison_atheeru_palestar;
    new MS::Garrison::npc_GarrisonStablesCreatures;
    new MS::Garrison::npc_follower_generic_script;
    new MS::Garrison::npc_StablesTrainingMounts_Garr;
    new MS::Garrison::npc_LeatherWorkingTent_Garr;
    new MS::Garrison::npc_InspiringBattleStandard;
    new MS::Garrison::npc_FearsomeBattleStandard;
    new MS::Garrison::npc_GarrisonWalter;
    new MS::Garrison::npc_GearshopWorkshopTurret_Garr;

    /// Alliance
    {
        new MS::Garrison::npc_GarrisonCartRope;
        new MS::Garrison::npc_AssistantBrightstone;
        new MS::Garrison::npc_ShellyHamby;
        new MS::Garrison::npc_BarosAlexsom;
        new MS::Garrison::npc_VindicatorMaraad;
        new MS::Garrison::npc_LunarfallLaborer;
        new MS::Garrison::npc_AncientTradingMechanism_Garr;

        /// Barracks
        new MS::Garrison::npc_JonathanStephens;

        /// Dwarven Bunker
        new MS::Garrison::npc_GussofForgefire;
        new MS::Garrison::npc_KristenStoneforge;

        /// The forge
        new MS::Garrison::npc_AuriaIrondreamer;
        new MS::Garrison::npc_YuliaSamras;
        new MS::Garrison::npc_DalanaClarke_Garr;

        /// Lunarfall excavation
        new MS::Garrison::npc_TimothyLeens;

        /// Fishing hut
        new MS::Garrison::npc_TharisStrongcast;
        new MS::Garrison::npc_Segumi;
        new MS::Garrison::npc_RonAshton;

        /// Trading post
        new MS::Garrison::npc_TraderJoseph;

        /// TailoringEmporium
        new MS::Garrison::npc_ChristopherMacdonald;
        new MS::Garrison::npc_KaylieMacdonald;

        /// Alchemy lab
        new MS::Garrison::npc_MaryKearie;
        new MS::Garrison::npc_PeterKearie;

        /// The tannery
        new MS::Garrison::npc_AndersLongstitch;
        new MS::Garrison::npc_MarianneLevine;

        /// Enchanter's Study
        new MS::Garrison::npc_AyadaTheWhite;
        new MS::Garrison::npc_Garm;

        /// Gem boutique
        new MS::Garrison::npc_CostanHighwall;
        new MS::Garrison::npc_KayaSolasen;

        /// Engineering works
        new MS::Garrison::npc_Zaren;
        new MS::Garrison::npc_HelaynWhent;

        /// Scribe quarters
        new MS::Garrison::npc_EricBroadoak;
        new MS::Garrison::npc_KurtBroadoak;

        /// Herb garden
        new MS::Garrison::npc_OllyNimkip;
        new MS::Garrison::npc_NaronBloomthistle;

        /// Barn
        new MS::Garrison::npc_HomerStonefield_Garr;
        new MS::Garrison::npc_HomerStonefield_Garr_Trap;

        /// Lumber Mill
        new MS::Garrison::npc_JustinTimberLord;

        /// Lunarfall Inn
        new MS::Garrison::npc_MadisonClark;
        new MS::Garrison::npc_lysa_serion_garr;

        /// Mage Tower
        new MS::Garrison::npc_ApprenticeVarNath;
        new MS::Garrison::npc_AncientWaygateProtector;

        /// Stables
        new MS::Garrison::npc_FannyFirebeard;
        new MS::Garrison::npc_KeeganFirebeard;

        /// Gladiator's Sanctum
        new MS::Garrison::npc_AltarOfBones;
        new MS::Garrison::npc_Kuros_Garr;

        /// Gnomish Gearworks
        new MS::Garrison::npc_Zee_Garrison;
    }

    /// Horde
    {
        new MS::Garrison::npc_FrostwallPeon("npc_FrostwallPeon_Dynamic");
        new MS::Garrison::npc_FrostwallPeon("npc_FrostwallPeon");
        new MS::Garrison::npc_Skaggit;
        new MS::Garrison::npc_LadySena;
        new MS::Garrison::npc_SergeantGrimjaw;
        new MS::Garrison::npc_SeniorPeonII;
        new MS::Garrison::npc_Gazlowe;

        /// War Mill
        new MS::Garrison::npc_GrunLek;
        new MS::Garrison::npc_FrostWallGrunt;
        new MS::Garrison::npc_FrostWallSmith;
        new MS::Garrison::npc_Magrish_Garr;

        /// The forge
        new MS::Garrison::npc_OrgekIronhand;
        new MS::Garrison::npc_Kinja;

        /// Trading post
        new MS::Garrison::npc_FaylaFairfeather;

        /// Tailoring Emporium
        new MS::Garrison::npc_WarraTheWeaver;
        new MS::Garrison::npc_Turga;

        /// Alchemy lab
        new MS::Garrison::npc_AlbertDeHyde;
        new MS::Garrison::npc_KeyanaTone;

        /// The tannery
        new MS::Garrison::npc_MurneGreenhoof;
        new MS::Garrison::npc_Yanney;

        /// Enchanter's Study
        new MS::Garrison::npc_YuklaGreenshadow;
        new MS::Garrison::npc_Garra;

        /// Gem boutique
        new MS::Garrison::npc_Dorogarr;
        new MS::Garrison::npc_ElrondirSurrion;

        /// Engineering works
        new MS::Garrison::npc_Pozzlow;
        new MS::Garrison::npc_GarbraFizzwonk;

        /// Scribe quarters
        new MS::Garrison::npc_Urgra;
        new MS::Garrison::npc_Yrogg;

        /// Herb garden
        new MS::Garrison::npc_Tarnon;
        new MS::Garrison::npc_NaliSoftOil;

        /// Barn
        new MS::Garrison::npc_FarmerLokLub;
        new MS::Garrison::npc_IronTrap;
        new MS::Garrison::npc_FarmerLokLub_Trap;

        /// Lumber Mill
        new MS::Garrison::npc_LumberLordOktron;

        /// Frostwall Mines
        new MS::Garrison::npc_Gorsol;

        /// Frostwall Tavern
        new MS::Garrison::npc_Murg;
        new MS::Garrison::npc_akanja_garr;

        /// Spirit Lodge
        new MS::Garrison::npc_Varsha;

        /// Stables
        new MS::Garrison::npc_Tormak;
        new MS::Garrison::npc_SagePaluna;

        /// Fishing Hut
        new MS::Garrison::npc_MakJin_Garr;

        /// Goblin Workshop
        new MS::Garrison::npc_Vee_Garrison;
    }

    /// General
    {
        /// Shipyard
        new MS::Garrison::npc_FleetCommandTable;
    }
}
#endif
#endif
