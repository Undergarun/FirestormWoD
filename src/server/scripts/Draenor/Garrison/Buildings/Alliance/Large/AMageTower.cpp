////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
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
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr)
            return true;

        if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
        {
            if (p_Player->GetQuestStatus(Quests::Alliance_PortablePortals) == QUEST_STATUS_INCOMPLETE)
                p_Player->QuestObjectiveSatisfy(ObjectIDs::FirstMageTowerSpiritLodgeObjectID, 1, 3);

            l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentFur);
            reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player);
        }
        return true;
    }

    void npc_ApprenticeVarNathAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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

                    SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::FrostfireRidgeBuildingGob, l_GobPos[0].X, l_GobPos[0].Y, l_GobPos[0].Z, l_GobPos[0].O);
                    SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::GorgrondBuildingGob, l_GobPos[1].X, l_GobPos[1].Y, l_GobPos[1].Z, l_GobPos[1].O);
                    SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::NagrandBuildingGob, l_GobPos[2].X, l_GobPos[2].Y, l_GobPos[2].Z, l_GobPos[2].O);
                    SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::ShadowmoonBuildingGob, l_GobPos[3].X, l_GobPos[3].Y, l_GobPos[3].Z, l_GobPos[3].O);
                    SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::SpiresOfArakBuildingGob, l_GobPos[4].X, l_GobPos[4].Y, l_GobPos[4].Z, l_GobPos[4].O);
                    SummonRelativeGameObject(GarrisonPortals::GarrisonGobs::TaladorBuildingGob, l_GobPos[5].X, l_GobPos[5].Y, l_GobPos[5].Z, l_GobPos[5].O);

                    /*switch (l_QuestID)
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
                    }*/
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

    bool npc_AncientWaygateProtector::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
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