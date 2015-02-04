////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

#include <random>

namespace MS { namespace Garrison 
{
    /// Constructor
    GarrisonNPCAI::GarrisonNPCAI(Creature * p_Creature)
        : MS::AI::CosmeticAI(p_Creature), m_PlotInstanceLocation(nullptr)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set to relative position from building
    /// @p_X : Relative X
    /// @p_Y : Relative Y
    /// @p_Z : Relative Z
    void GarrisonNPCAI::MoveBuildingRelative(uint32 p_PointID, float p_X, float p_Y, float p_Z)
    {
        if (!m_PlotInstanceLocation)
            return;

        G3D::Vector3 l_Position = G3D::Vector3(p_X, p_Y, 0);

        G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
        l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

        l_Position.x += m_NonRotatedPlotPosition.x;
        l_Position.y += m_NonRotatedPlotPosition.y;

        l_Position = l_Mat * l_Position;

        l_Position.z = p_Z + m_PlotInstanceLocation->Z;

        me->GetMotionMaster()->MovePoint(p_PointID, l_Position.x, l_Position.y, l_Position.z);
    }
    /// Set facing to relative angle from the building
    /// @p_O : Relative angle
    void GarrisonNPCAI::SetFacingBuildingRelative(float p_O)
    {
        float l_Angle = p_O;

        if (m_PlotInstanceLocation)
            l_Angle += m_PlotInstanceLocation->O;

        me->SetFacingTo(Position::NormalizeOrientation(l_Angle));
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void GarrisonNPCAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::PlotInstanceID)
        {
            m_PlotInstanceLocation = nullptr;

            for (uint32 l_I = 0; l_I < Globals::PlotInstanceCount; ++l_I)
            {
                if (gGarrisonPlotInstanceInfoLocation[l_I].PlotInstanceID == (p_Value & 0x0000FFFF) && gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == ((p_Value >> 16) & 0x0000FFFF))
                {
                    m_PlotInstanceLocation = &gGarrisonPlotInstanceInfoLocation[l_I];
                    break;
                }
            }

            if (m_PlotInstanceLocation)
            {
                G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -m_PlotInstanceLocation->O);

                /// transform plot coord
                m_NonRotatedPlotPosition = l_Mat * G3D::Vector3(m_PlotInstanceLocation->X, m_PlotInstanceLocation->Y, m_PlotInstanceLocation->Z);
            }
        }
    }

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
    bool npc_GarrisonFord::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (!p_Player->GetGarrison())
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Create me a garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        else
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Delete my garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_GarrisonFord::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (!p_Player->GetGarrison())
        {
            p_Player->CLOSE_GOSSIP_MENU();
            p_Player->CreateGarrison();

            uint32 l_MovieID    = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->CreationMovie;
            uint32 l_MapID      = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->MapID;
            uint32 l_TeamID     = p_Player->GetTeamId();

            p_Player->AddMovieDelayedTeleport(l_MovieID, l_MapID,   MS::Garrison::gGarrisonCreationCoords[l_TeamID][0],
                                                                    MS::Garrison::gGarrisonCreationCoords[l_TeamID][1],
                                                                    MS::Garrison::gGarrisonCreationCoords[l_TeamID][2],
                                                                    MS::Garrison::gGarrisonCreationCoords[l_TeamID][3]);
            p_Player->SendMovieStart(l_MovieID);

            if (l_TeamID == TEAM_ALLIANCE && p_Player->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_A) != QUEST_STATUS_REWARDED)
            {
                p_Player->AddQuest(sObjectMgr->GetQuestTemplate(Quests::QUEST_ETABLISH_YOUR_GARRISON_A), p_Creature);
                p_Player->CompleteQuest(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A);
            }
            else if (l_TeamID == TEAM_HORDE && p_Player->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_H) != QUEST_STATUS_REWARDED)
            {
                p_Player->AddQuest(sObjectMgr->GetQuestTemplate(Quests::QUEST_ETABLISH_YOUR_GARRISON_H), p_Creature);
                p_Player->CompleteQuest(Quests::QUEST_ETABLISH_YOUR_GARRISON_H);
            }

            /// HACK until shadowmoon quest are done : add follower Qiana Moonshadow / Olin Umberhide
            /// @Morgoporc Dont worry, this code will disappear
            p_Player->GetGarrison()->AddFollower(34);
            p_Player->GetGarrison()->AddFollower(89);
            p_Player->GetGarrison()->AddFollower(92);
        }
        else
        {
            if (p_Player->GetCurrency(MS::Garrison::Globals::CurrencyID, false))
                p_Player->ModifyCurrency(MS::Garrison::Globals::CurrencyID, -(int32)p_Player->GetCurrency(MS::Garrison::Globals::CurrencyID, false));

            p_Player->DeleteGarrison();
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
    CreatureAI * npc_CallToArms::GetAI(Creature * p_Creature) const
    {
        return new npc_CallToArmsAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_CallToArms::npc_CallToArmsAI::npc_CallToArmsAI(Creature * p_Creature)
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
                me->m_threatModifier[i] *= 10000.f;
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
    void npc_CallToArms::npc_CallToArmsAI::UpdateAI(const uint32 p_Diff)
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        if (m_Ranged)
        {
            if (me->IsWithinMeleeRange(me->getVictim(), 1.f))
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

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_NPC()
{
    /// Generic
    new MS::Garrison::npc_GarrisonFord;
    new MS::Garrison::npc_CallToArms;

    /// Alliance
    new MS::Garrison::npc_GarrisonCartRope;
    new MS::Garrison::npc_AssistantBrightstone;
    new MS::Garrison::npc_ShellyHamby;
    new MS::Garrison::npc_BarosAlexsom;
    new MS::Garrison::npc_VindicatorMaraad;
    new MS::Garrison::npc_LunarfallLaborer;
    new MS::Garrison::npc_GussofForgefire;
    new MS::Garrison::npc_KristenStoneforge;
    new MS::Garrison::npc_JonathanStephens;
    new MS::Garrison::npc_AuriaIrondreamer;

    /// Horde
    new MS::Garrison::npc_FrostwallPeon("npc_FrostwallPeon_Dynamic");
    new MS::Garrison::npc_FrostwallPeon("npc_FrostwallPeon");
    new MS::Garrison::npc_Skaggit;
    new MS::Garrison::npc_LadySena;
    new MS::Garrison::npc_SergeantGrimjaw;
    new MS::Garrison::npc_SeniorPeonII;
    new MS::Garrison::npc_Gazlowe;
    new MS::Garrison::npc_GrunLek;
    new MS::Garrison::npc_FrostWallGrunt;
    new MS::Garrison::npc_FrostWallSmith;
    new MS::Garrison::npc_OrgekIronhand;
    new MS::Garrison::npc_Kinja;
}