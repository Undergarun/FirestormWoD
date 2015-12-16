////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
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

    void npc_VarshaAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        std::vector<SequencePosition> l_GobPos =
        {
            { 13.5690f, -5.1295f, 1.3013f, 2.1965f },
            { 9.8864f,  -7.8239f, 1.2993f, 1.8823f },
            { 1.7612f,  -6.5470f, 1.3018f, 0.7749f },
            { -0.8525f, -1.5060f, 1.3028f, 0.1977f },
            { 0.5427f,   6.0054f, 1.3089f, 5.6326f },
            { 9.6880f,   8.1694f, 1.3003f, 4.5645f },
        };

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
                if (l_Owner->IsQuestRewarded(l_QuestID) && l_Itr < l_BuildingEntry->Level)
                {
                    l_Itr++;

                    switch (l_QuestID)
                    {
                        case GarrisonPortals::PortalsQuests::QuestFrostfireRidge:
                            SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::FrostfireRidgeBuildingGob, l_GobPos[0].X, l_GobPos[0].Y, l_GobPos[0].Z, l_GobPos[0].O);
                            break;
                        case GarrisonPortals::PortalsQuests::QuestGorgrond:
                            SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::GorgrondBuildingGob, l_GobPos[1].X, l_GobPos[1].Y, l_GobPos[1].Z, l_GobPos[1].O);
                            break;
                        case GarrisonPortals::PortalsQuests::QuestNagrand:
                            SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::NagrandBuildingGob, l_GobPos[2].X, l_GobPos[2].Y, l_GobPos[2].Z, l_GobPos[2].O);
                            break;
                        case GarrisonPortals::PortalsQuests::QuestShadowmoon:
                            SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::ShadowmoonBuildingGob, l_GobPos[3].X, l_GobPos[3].Y, l_GobPos[3].Z, l_GobPos[3].O);
                            break;
                        case GarrisonPortals::PortalsQuests::QuestSpiresOfArak:
                            SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::SpiresOfArakBuildingGob, l_GobPos[4].X, l_GobPos[4].Y, l_GobPos[4].Z, l_GobPos[4].O);
                            break;
                        case GarrisonPortals::PortalsQuests::QuestTalador:
                            SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::TaladorBuildingGob, l_GobPos[5].X, l_GobPos[5].Y, l_GobPos[5].Z, l_GobPos[5].O);
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
        p_Player->PlayerTalkClass->ClearMenus();
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr)
            return true;

        if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
        {
            if (p_Player->GetQuestStatus(Quests::Horde_PortablePortals) == QUEST_STATUS_INCOMPLETE)
                p_Player->QuestObjectiveSatisfy(ObjectIDs::FirstMageTowerSpiritLodgeObjectID, 1, 3);

            l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentFur);
            reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player);
        }
        return true;
    }
}   ///< namespace Garrison
}   ///< namespace MS