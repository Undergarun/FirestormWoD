////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "AGnomishGearworks.hpp"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS {
    namespace Garrison
    {
        //////////////////////////////////////////////////////////////////////////
        /// 85369 - Vee                                                        ///
        //////////////////////////////////////////////////////////////////////////

        namespace npc_ZeeData
        {
            InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
            {
            };

            InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
            {
            };

            InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
            {
            };
        }

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// Constructor
        npc_Zee_Garrison::npc_Zee_Garrison()
            : CreatureScript("npc_Zee_Garrison")
        {
        }

        /// Constructor
        npc_Zee_GarrisonAI::npc_Zee_GarrisonAI(Creature* p_Creature)
            : GarrisonNPCAI(p_Creature)
        {
        }

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        CreatureAI* npc_Zee_Garrison::GetAI(Creature* p_Creature) const
        {
            return new npc_Zee_GarrisonAI(p_Creature);
        }

        bool npc_Zee_Garrison::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
        {
            if (p_Quest->GetQuestId() == Quests::Alliance_UnconventionalInventions)
            {
                if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
                {
                    GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

                    if (l_AI == nullptr)
                        return true;

                    l_GarrisonMgr->ActivateBuilding(l_AI->GetPlotInstanceID());
                }
            }

            return true;
        }

        bool npc_Zee_Garrison::OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
        {
            if (p_Quest->GetQuestId() == Quests::Alliance_UnconventionalInventions)
            {
                if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
                {
                    GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

                    if (l_AI == nullptr)
                        return true;

                    if (p_Player->HasItemCount(WorkshopGearworks::InventionItemIDs::ItemStickyGrenades))
                        return true;

                    SequencePosition const l_GameObjectPos = { 7.4031f, -15.7592f, 1.6757f, 2.0719f };

                    l_AI->SummonRelativeGameObject(WorkshopGearworks::InventionsGobIDs::GobStickyGrenades, l_GameObjectPos.X, l_GameObjectPos.Y, l_GameObjectPos.Z, l_GameObjectPos.O);
                    p_Player->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonWorkshopGearworksInvention, WorkshopGearworks::InventionsGobIDs::GobStickyGrenades);
                    p_Player->SaveToDB();
                }
            }
            return true;
        }

        void npc_Zee_GarrisonAI::SetGUID(uint64 p_Guid, int32 p_Id)
        {
            if (p_Id == CreatureAIDataIDs::OwnerGuid)
                m_OwnerGUID = p_Guid;
        }

        void npc_Zee_GarrisonAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
        {
            Player* l_Owner = ObjectAccessor::GetPlayer(*me, m_OwnerGUID);

            if (l_Owner == nullptr)
                return;

            Manager* l_GarrisonMgr = l_Owner->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            uint32 l_GobID = l_Owner->GetCharacterWorldStateValue(CharacterWorldStates::CharWorldStateGarrisonWorkshopGearworksInvention);

            if (!l_GobID) ///< Quest or daily refill not done
                return;

            if (GameObject* l_Gob = me->FindNearestGameObject(l_GobID, 30.0f))
                l_Gob->RemoveFromWorld();

            if (l_Owner->HasQuest(Quests::Alliance_UnconventionalInventions))
            {
                if (l_Owner->HasItemCount(WorkshopGearworks::InventionItemIDs::ItemStickyGrenades))
                    return;
            }
            else if (l_Owner->HasItemCount(WorkshopGearworks::g_GobItemRelations[l_GobID]))
                return;

            SequencePosition const l_GameObjectPos = { 7.4031f, -15.7592f, 1.6757f, 2.0719f };
            SummonRelativeGameObject(l_GobID, l_GameObjectPos.X, l_GameObjectPos.Y, l_GameObjectPos.Z, l_GameObjectPos.O);

            l_Owner->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonWorkshopGearworksInvention, l_GobID);
            l_Owner->SaveToDB();
        }

        void npc_Zee_GarrisonAI::OnDailyDataReset()
        {
            if (Player* l_Owner = ObjectAccessor::GetPlayer(*me, m_OwnerGUID))
            {
                MS::Garrison::Manager* l_GarrisonMgr = l_Owner->GetGarrison();

                if (l_GarrisonMgr == nullptr || !l_Owner->IsQuestRewarded(Quests::Alliance_UnconventionalInventions))
                    return;

                uint32 l_Entry = 0;

                switch (l_GarrisonMgr->GetBuilding(GetPlotInstanceID()).BuildingID)
                {
                    case Buildings::GnomishGearworks_GoblinWorkshop_Level1:
                        l_Entry = WorkshopGearworks::g_FirstLevelInventions[urand(0, WorkshopGearworks::g_FirstLevelInventions.size() - 1)];
                        break;
                    case Buildings::GnomishGearworks_GoblinWorkshop_Level2:
                        l_Entry = WorkshopGearworks::g_SecondLevelInventions[urand(0, WorkshopGearworks::g_FirstLevelInventions.size() - 1)];
                        break;
                    case Buildings::GnomishGearworks_GoblinWorkshop_Level3:
                        l_Entry = WorkshopGearworks::g_ThirdLevelInvention;
                        break;
                    default:
                        break;
                }

                l_Owner->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonWorkshopGearworksInvention, l_Entry);
                l_Owner->SaveToDB();

                OnSetPlotInstanceID(GetPlotInstanceID());
                l_GarrisonMgr->UpdatePlot(GetPlotInstanceID());
            }
        }
    }   ///< namespace Garrison
}   ///< namespace MS