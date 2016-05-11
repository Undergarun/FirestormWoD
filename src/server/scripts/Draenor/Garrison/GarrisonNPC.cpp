////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2014-2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

#include "Buildings/Alliance/Large/ABarracks.hpp"
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
#include "Buildings/Horde/HFrostwallMines.hpp"
#include "Buildings/Horde/HHerbGarden.hpp"

#include <random>

namespace MS { namespace Garrison 
{
    /// Constructor
    GarrisonNPCAI::GarrisonNPCAI(Creature * p_Creature)
        : MS::AI::CosmeticAI(p_Creature), m_PlotInstanceLocation(nullptr), m_BuildingID(0), m_SequenceSize(0), m_Recipes(nullptr), m_Owner(nullptr)
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

    /// Set NPC recipes
    /// @p_Recipes          : Recipes
    /// @p_RecipesSkillID   : Skill line ID
    void GarrisonNPCAI::SetRecipes(std::vector<SkillNPC_RecipeEntry> * p_Recipes, uint32 p_RecipesSkillID)
    {
        m_Recipes           = p_Recipes;
        m_RecipesSkillID    = p_RecipesSkillID;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get building ID
    uint32 GarrisonNPCAI::GetBuildingID()
    {
        return m_BuildingID;
    }

    /// Get plot instance ID
    uint32 GarrisonNPCAI::GetPlotInstanceID()
    {
        return m_PlotInstanceLocation ? m_PlotInstanceLocation->PlotInstanceID : 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Setup action sequence
    /// @p_CoordTable       : Coordinates table
    /// @p_SequenceTable    : Sequence table
    /// @p_SequenceSize     : Size of sequence table,
    /// @p_FirstMovePointID : First move point ID
    void GarrisonNPCAI::SetupActionSequence(SequencePosition * p_CoordTable, uint8 * p_SequenceTable, uint32 p_SequenceSize, uint32 p_FirstMovePointID)
    {
        m_CoordTable        = p_CoordTable;
        m_SequencePosition  = 0xFF;
        m_SequenceTable     = p_SequenceTable;
        m_SequenceSize      = p_SequenceSize;
        m_FirstMovePointID  = p_FirstMovePointID;
    }

    /// Do next sequence element
    void GarrisonNPCAI::DoNextSequenceAction()
    {
        if (!m_SequenceSize)
            return;

        if (m_SequencePosition >= m_SequenceSize)
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = m_SequenceTable[m_SequencePosition] - m_FirstMovePointID;
            MoveBuildingRelative( m_SequenceTable[m_SequencePosition], m_CoordTable[l_LocationID].X,
                                                                       m_CoordTable[l_LocationID].Y,
                                                                       m_CoordTable[l_LocationID].Z);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Spawn a creature with building relative coords
    /// @p_Entry      : Creature entry
    /// @p_RelX       : X Relative coord
    /// @p_RelY       : Y Relative coord
    /// @p_RelZ       : Z Relative coord
    /// @p_RelO       : Relative orientation coord
    /// @p_SummonType : Summon type
    Creature* GarrisonNPCAI::SummonRelativeCreature(uint32 p_Entry, float p_RelX, float p_RelY, float p_RelZ, float p_RelO, TempSummonType p_SummonType)
    {
        if (!m_PlotInstanceLocation)
            return nullptr;

        G3D::Vector3 l_Position = G3D::Vector3(p_RelX, p_RelY, 0);

        G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
        l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

        l_Position.x += m_NonRotatedPlotPosition.x;
        l_Position.y += m_NonRotatedPlotPosition.y;

        l_Position = l_Mat * l_Position;

        l_Position.z = p_RelZ + m_PlotInstanceLocation->Z;

        float l_Angle = p_RelO;

        if (m_PlotInstanceLocation)
            l_Angle += m_PlotInstanceLocation->O;

        return me->SummonCreature(p_Entry, l_Position.x, l_Position.y, l_Position.z, l_Angle, p_SummonType);
    }

    /// Spawn a gameobject with building relative coords
    /// @p_Entry      : GameObject entry
    /// @p_RelX       : X Relative coord
    /// @p_RelY       : Y Relative coord
    /// @p_RelZ       : Z Relative coord
    /// @p_RelO       : Relative orientation coord
    GameObject* GarrisonNPCAI::SummonRelativeGameObject(uint32 p_Entry, float p_RelX, float p_RelY, float p_RelZ, float p_RelO)
    {
        if (!m_PlotInstanceLocation)
            return nullptr;

        G3D::Vector3 l_Position = G3D::Vector3(p_RelX, p_RelY, 0);

        G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
        l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), m_PlotInstanceLocation->O);

        l_Position.x += m_NonRotatedPlotPosition.x;
        l_Position.y += m_NonRotatedPlotPosition.y;

        l_Position = l_Mat * l_Position;

        l_Position.z = p_RelZ + m_PlotInstanceLocation->Z;

        float l_Angle = p_RelO;

        if (m_PlotInstanceLocation)
            l_Angle += m_PlotInstanceLocation->O;

        return me->SummonGameObject(p_Entry, l_Position.x, l_Position.y, l_Position.z, l_Angle, 0, 0, 0, 0, 0);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Transform coord
    /// @p_X : X coord
    /// @p_Y : Y coord
    /// @p_Z : Z coord
    void GarrisonNPCAI::TransformCoord(float& p_X, float &p_Y, float &p_Z)
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

        p_X = l_Position.x;
        p_Y = l_Position.y;
        p_Z = l_Position.z;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the building ID is set
    /// @p_BuildingID : Set building ID
    void GarrisonNPCAI::OnSetBuildingID(uint32 p_BuildingID)
    {

    }

    /// When the PlotInstance ID is set
    /// @p_BuildingID : Set plot instance ID
    void GarrisonNPCAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {

    }

    /// When the daily garrison datas are reset
    void GarrisonNPCAI::OnDataReset()
    {

    }

    void GarrisonNPCAI::OnPlotInstanceUnload()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void GarrisonNPCAI::SetData(uint32 p_ID, uint32 p_Value)
    {

        switch (p_ID)
        {
            case CreatureAIDataIDs::PlotInstanceID:
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
            
                    OnSetPlotInstanceID(m_PlotInstanceLocation->PlotInstanceID);
                }
                break;
            }
            case CreatureAIDataIDs::BuildingID:
            {
                m_BuildingID = p_Value;
                OnSetBuildingID(m_BuildingID);
            }
            case CreatureAIDataIDs::DailyReset:
                OnDataReset();
                break;
            case CreatureAIDataIDs::DespawnData:
                OnPlotInstanceUnload();
                break;
            default:
                break;
        }
    }

    void GarrisonNPCAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_Owner = ObjectAccessor::GetPlayer(*me, p_Guid);
    }

    /// Get UInt32 value
    /// @p_ID    : Value ID
    uint32 GarrisonNPCAI::GetData(uint32 p_ID)
    {
        if ((p_ID & CreatureAIDataIDs::HasRecipe) != 0)
        {
            if (!m_Recipes)
                return (uint32)-1;

            uint32 l_RecipeID = p_ID & ~CreatureAIDataIDs::HasRecipe;

            auto l_It = std::find_if(m_Recipes->begin(), m_Recipes->end(), [l_RecipeID](SkillNPC_RecipeEntry const& p_Entry) -> bool
            {
                return p_Entry.AbilitySpellID == l_RecipeID;
            });

            if (l_It == m_Recipes->end())
                return (uint32)-1;

            return l_It->AbilitySpellIDPlayerCondition;
        }
        
        return (uint32)-1;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Show shipment crafter UI
    void GarrisonNPCAI::SendShipmentCrafterUI(Player* p_Player, uint32 p_ShipmentID /*= 0*/)
    {
        if (p_Player->IsInGarrison())
        {
            if (!p_ShipmentID && !(p_ShipmentID = sGarrisonShipmentManager->GetShipmentIDForBuilding(m_BuildingID, p_Player, false)))
                p_Player->PlayerTalkClass->SendCloseGossip();
            else
            {
                WorldPacket l_Data(SMSG_OPEN_SHIPMENT_NPCFROM_GOSSIP);
                l_Data.appendPackGUID(me->GetGUID());
                l_Data << uint32(p_ShipmentID);

                p_Player->SendDirectMessage(&l_Data);
            }
        }
    }

    /// Show trade skill crafter UI
    void GarrisonNPCAI::SendTradeSkillUI(Player * p_Player)
    {
        if (p_Player->IsInGarrison())
        {
            if (m_Recipes)
            {
                WorldPacket l_Data(SMSG_GARRISON_OPEN_TRADESKILL_NPC, 512);
                l_Data.appendPackGUID(me->GetGUID());
                l_Data << uint32(0);                    ///< SpellID
                l_Data << uint32(1);                    ///< Skill line ID count
                l_Data << uint32(0);                    ///< Skill rank count
                l_Data << uint32(0);                    ///< Skill max rank count
                l_Data << uint32(m_Recipes->size());    ///< Skill known ability spell id count

                l_Data << uint32(m_RecipesSkillID);     ///< Skill line ID

                for (uint32 l_I = 0; l_I < m_Recipes->size(); ++l_I)
                    l_Data << m_Recipes->at(l_I).AbilitySpellID;

                l_Data << uint32(m_Recipes->size());    ///< Skill known ability spell id condition count

                for (uint32 l_I = 0; l_I < m_Recipes->size(); ++l_I)
                    l_Data << m_Recipes->at(l_I).AbilitySpellIDPlayerCondition;

                p_Player->SendDirectMessage(&l_Data);
            }
            else
                p_Player->PlayerTalkClass->SendCloseGossip();
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
            p_Player->GetGarrison()->AddFollower(34);
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
        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bring back the assemblies.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        p_Player->SEND_GOSSIP_MENU(MiscDatas::NpcTextID, p_Creature->GetGUID());

        return true;
    }

    bool npc_garrison_atheeru_palestar::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1 && p_Creature->AI())
            p_Creature->AI()->DoAction(1);
        return true;
    }

    void npc_garrison_atheeru_palestarAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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

    void npc_garrison_amperial_construct::npc_garrison_amperial_constructAI::PassengerBoarded(Unit* p_Passenger, int8 p_SeatID, bool p_Apply)
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
                            if (l_Creature->IsInMap(me) && me->HasInArc(M_PI / 2, l_Player))
                                l_Player->NearTeleportTo(l_Creature->m_positionX, l_Creature->m_positionY, l_Creature->m_positionZ, l_Creature->GetOrientation());
                        }
                    }
                }

                m_CheckTimer = 500;
            }
            else
                m_CheckTimer -= p_Diff;
        }
    }

    // Because blizzard does it this way - also icon type 27 could be for this purpose only

    npc_FleetCommandTable::npc_FleetCommandTable() : CreatureScript("npc_FleetCommandTable")
    {
    }

    bool npc_FleetCommandTable::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (!p_Player->GetGarrison())
            return true;

        p_Player->GetSession()->SendGarrisonSetMissionNpc(p_Creature->GetGUID());
        return true;
    }


}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_NPC()
{
    /// Generic
    new MS::Garrison::npc_GarrisonFord;
    new MS::Garrison::npc_CallToArms;
    new MS::Garrison::npc_garrison_amperial_construct;
    new MS::Garrison::npc_garrison_atheeru_palestar;

    /// Alliance
    {
        new MS::Garrison::npc_GarrisonCartRope;
        new MS::Garrison::npc_AssistantBrightstone;
        new MS::Garrison::npc_ShellyHamby;
        new MS::Garrison::npc_BarosAlexsom;
        new MS::Garrison::npc_VindicatorMaraad;
        new MS::Garrison::npc_LunarfallLaborer;

        /// Barracks
        new MS::Garrison::npc_JonathanStephens;

        /// Dwarven Bunker
        new MS::Garrison::npc_GussofForgefire;
        new MS::Garrison::npc_KristenStoneforge;

        /// The forge
        new MS::Garrison::npc_AuriaIrondreamer;
        new MS::Garrison::npc_YuliaSamras;

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
        new MS::Garrison::npc_HomerStonefield;
        new MS::Garrison::npc_HomerStonefield_Garr_Trap;

        /// Lumber Mill
        new MS::Garrison::npc_JustinTimberLord;

        /// Lunarfall Inn
        new MS::Garrison::npc_MadisonClark;

        /// Mage Tower
        new MS::Garrison::npc_ApprenticeVarNath;
        new MS::Garrison::npc_AncientWaygateProtector;

        /// Stables
        new MS::Garrison::npc_FannyFirebeard;
        new MS::Garrison::npc_KeeganFirebeard;

        /// Gladiator's Sanctum
        new MS::Garrison::npc_AltarOfBones;
        new MS::Garrison::npc_Kuros_Garr();
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

        /// Spirit Lodge
        new MS::Garrison::npc_Varsha;

        /// Stables
        new MS::Garrison::npc_Tormak;
        new MS::Garrison::npc_SagePaluna;
    }

    /// General
    {
        /// Shipyard
        new MS::Garrison::npc_FleetCommandTable;
    }
}