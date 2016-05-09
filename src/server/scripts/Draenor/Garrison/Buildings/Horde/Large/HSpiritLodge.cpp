////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HSpiritLodge.hpp"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 91582 - Apprentice Var'Nath                                        ///
    //////////////////////////////////////////////////////////////////////////

    namespace npc_VarshaData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {
        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {
        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Varsha::npc_Varsha()
        : CreatureScript("npc_Varsha_Garr")
    {

    }

    /// Constructor
    npc_VarshaAI::npc_VarshaAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Varsha::GetAI(Creature* p_Creature) const
    {
        return new npc_VarshaAI(p_Creature);
    }

    void npc_VarshaAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
    {
        using namespace MS::Garrison::GarrisonPortals;

        if (Player* l_Owner = GetOwner())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

            std::vector<uint32> const l_QuestList =
            {
                GarrisonPortals::PortalsQuests::QuestFrostfireRidge,
                GarrisonPortals::PortalsQuests::QuestGorgrond,
                GarrisonPortals::PortalsQuests::QuestNagrand,
                GarrisonPortals::PortalsQuests::QuestShadowmoon,
                GarrisonPortals::PortalsQuests::QuestSpiresOfArak,
                GarrisonPortals::PortalsQuests::QuestTalador
            };

            uint8 l_Itr = 0;

            for (uint32 l_QuestID : l_QuestList)
            {
                if (l_Owner->IsQuestRewarded(l_QuestID) && l_BuildingEntry && l_Itr < l_BuildingEntry->Level)
                {
                    switch (l_QuestID)
                    {
                        case GarrisonPortals::PortalsQuests::QuestFrostfireRidge:
                            if (GameObject* l_Gob = SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::FrostfireRidgeBuildingGob, g_GobPos[l_Owner->GetTeamId()][l_BuildingEntry->Level - 1][0]))
                            {
                                l_Gob->SetPhaseMask(GarrisonPhases::PhaseMagePortalFrostfireRidge, true);
                                l_Itr++;
                            }
                            break;
                        case GarrisonPortals::PortalsQuests::QuestGorgrond:
                            if (GameObject* l_Gob = SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::GorgrondBuildingGob, g_GobPos[l_Owner->GetTeamId()][l_BuildingEntry->Level - 1][1]))
                            {
                                l_Gob->SetPhaseMask(GarrisonPhases::PhaseMagePortalGorgrond, true);
                                l_Itr++;
                            }
                            break;
                        case GarrisonPortals::PortalsQuests::QuestNagrand:
                            if (GameObject* l_Gob = SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::NagrandBuildingGob, g_GobPos[l_Owner->GetTeamId()][l_BuildingEntry->Level - 1][2]))
                            {
                                l_Gob->SetPhaseMask(GarrisonPhases::PhaseMagePortalNagrand, true);
                                l_Itr++;
                            }
                            break;
                        case GarrisonPortals::PortalsQuests::QuestShadowmoon:
                            if (GameObject* l_Gob = SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::ShadowmoonBuildingGob, g_GobPos[l_Owner->GetTeamId()][l_BuildingEntry->Level - 1][3]))
                            {
                                l_Gob->SetPhaseMask(GarrisonPhases::PhaseMagePortalShadowmoon, true);
                                l_Itr++;
                            }
                            break;
                        case GarrisonPortals::PortalsQuests::QuestSpiresOfArak:
                            if (GameObject* l_Gob = SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::SpiresOfArakBuildingGob, g_GobPos[l_Owner->GetTeamId()][l_BuildingEntry->Level - 1][4]))
                            {
                                l_Gob->SetPhaseMask(GarrisonPhases::PhaseMagePortalSpiresOfArak, true);
                                l_Itr++;
                            }
                            break;
                        case GarrisonPortals::PortalsQuests::QuestTalador:
                            if (GameObject* l_Gob = SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::TaladorBuildingGob, g_GobPos[l_Owner->GetTeamId()][l_BuildingEntry->Level - 1][5]))
                            {
                                l_Gob->SetPhaseMask(GarrisonPhases::PhaseMagePortalTalador, true);
                                l_Itr++;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    bool npc_Varsha::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        p_Player->PlayerTalkClass->ClearMenus();

        if (p_Player->GetQuestStatus(Quests::Horde_PortablePortals) == QUEST_STATUS_INCOMPLETE)
            p_Player->QuestObjectiveSatisfy(ObjectIDs::FirstMageTowerSpiritLodgeObjectID, 1, 3);

        l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }
}   ///< namespace Garrison
}   ///< namespace MS