////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AMageTower.hpp"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 91582 - Apprentice Var'Nath                                        ///
    //////////////////////////////////////////////////////////////////////////

    namespace npc_ApprenticeVarNathAIData
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
    npc_ApprenticeVarNath::npc_ApprenticeVarNath()
        : CreatureScript("npc_ApprenticeVarNath_Garr")
    {
    }

    /// Constructor
    npc_ApprenticeVarNathAI::npc_ApprenticeVarNathAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_ApprenticeVarNath::GetAI(Creature* p_Creature) const
    {
        return new npc_ApprenticeVarNathAI(p_Creature);
    }

    bool npc_ApprenticeVarNath::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        if (p_Player->GetQuestStatus(Quests::Alliance_PortablePortals) == QUEST_STATUS_INCOMPLETE)
            p_Player->QuestObjectiveSatisfy(ObjectIDs::FirstMageTowerSpiritLodgeObjectID, 1, 3);

        l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    void npc_ApprenticeVarNathAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
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
                if (l_Owner->IsQuestRewarded(l_QuestID) && l_Itr < l_BuildingEntry->Level)
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

    //////////////////////////////////////////////////////////////////////////
    /// 89233, 89236, 89234, 89235, 89232, 8923                            ///
    /// Ancient Waygate Protectors                                          ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AncientWaygateProtector::npc_AncientWaygateProtector()
        : CreatureScript("npc_AncientWaygateProtector_Garr")
    {

    }

    bool npc_AncientWaygateProtector::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        switch (p_Creature->GetZoneId())
        {
            case GarrisonPortals::DraenorZones::ZoneFrostfireRidge:
            {
                if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestFrostfireRidge))
                    p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "I would like to deactivate this waygate.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "Do you want really to deactivate this portal ?", 100000);
                break;
            }
            case GarrisonPortals::DraenorZones::ZoneGorgrond:
            {
                if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestGorgrond))
                    p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "I would like to deactivate this waygate.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, "Do you want really to deactivate this portal ?", 100000);
                break;
            }
            case GarrisonPortals::DraenorZones::ZoneNagrand:
            {
                if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestNagrand))
                    p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "I would like to deactivate this waygate.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, "Do you want really to deactivate this portal ?", 100000);
                break;
            }
            case GarrisonPortals::DraenorZones::ZoneShadowmoon:
            {
                if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestShadowmoon))
                    p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "I would like to deactivate this waygate.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "Do you want really to deactivate this portal ?", 100000);
                break;
            }
            case GarrisonPortals::DraenorZones::ZoneSpiresOfArak:
            {
                if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestSpiresOfArak))
                    p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "I would like to deactivate this waygate.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5, "Do you want really to deactivate this portal ?", 100000);
                break;
            }
            case GarrisonPortals::DraenorZones::ZoneTalador:
            {
                if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestTalador))
                    p_Player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, GOSSIP_ICON_CHAT, "I would like to deactivate this waygate.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6, "Do you want really to deactivate this portal ?", 100000);
                break;
            }
            default:
                break;
        }

        p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

        return true;
    }

    bool npc_AncientWaygateProtector::OnGossipSelect(Player* p_Player, Creature* /*p_Creature*/, uint32 /*p_Sender*/, uint32 p_Action)
    {
            p_Player->PlayerTalkClass->ClearMenus();

            switch (p_Action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    p_Player->CastSpell(p_Player, GarrisonPortals::DeactivationSpells::FrostfireRidgeDeactivationSpell, true);
                    p_Player->ModifyMoney(-100000);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    p_Player->CastSpell(p_Player, GarrisonPortals::DeactivationSpells::GorgrondDeactivationSpell, true);
                    p_Player->ModifyMoney(-100000);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    p_Player->CastSpell(p_Player, GarrisonPortals::DeactivationSpells::NagrandDeactivationSpell, true);
                    p_Player->ModifyMoney(-100000);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    p_Player->CastSpell(p_Player, GarrisonPortals::DeactivationSpells::ShadowmoonDeactivationSpell, true);
                    p_Player->ModifyMoney(-100000);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    p_Player->CastSpell(p_Player, GarrisonPortals::DeactivationSpells::SpiresOfArakDeactivationSpell, true);
                    p_Player->ModifyMoney(-100000);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6:
                    p_Player->CastSpell(p_Player, GarrisonPortals::DeactivationSpells::TaladorDeactivationSpell, true);
                    p_Player->ModifyMoney(-100000);
                    break;
                default:
                    break;
            }

        p_Player->PlayerTalkClass->SendCloseGossip();

        return true;
    }
}   ///< namespace Garrison
}   ///< namespace MS
