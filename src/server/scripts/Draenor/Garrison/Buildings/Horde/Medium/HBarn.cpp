////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HBarn.hpp"
#include "GarrisonMgr.hpp"
#include "GameObjectAI.h"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 85048 - Farmer Lok'Lub                                             ///
    //////////////////////////////////////////////////////////////////////////

    namespace npc_FarmerLokLubAIData
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
    npc_FarmerLokLub::npc_FarmerLokLub()
        : CreatureScript("npc_FarmerLokLub_Garr")
    {

    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_FarmerLokLubAI::npc_FarmerLokLubAI(Creature* p_Creature)
        : SimpleSequenceCosmeticScriptAI(p_Creature)
    {
    }

    void npc_FarmerLokLub::OnShipmentCreated(Player* p_Player, Creature* p_Creature, uint32 p_BuildingID)
    {
        switch (p_BuildingID)
        {
            case Buildings::Barn__Barn_Level1:
                if (p_Player->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                    p_Player->KilledMonsterCredit(40670);
                break;
            case Buildings::Barn__Barn_Level2:
                if (p_Player->GetQuestStatus(Quests::Horde_FeedingAnArmy) == QUEST_STATUS_INCOMPLETE)
                    p_Player->KilledMonsterCredit(40674);
                break;
            case Buildings::Barn__Barn_Level3:
                if (p_Player->GetQuestStatus(Quests::Horde_BiggerTrapBetterRewards) == QUEST_STATUS_INCOMPLETE)
                    p_Player->KilledMonsterCredit(40693);
                break;
            default:
                break;
        }
    }

    bool npc_FarmerLokLub::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return true;

        switch (l_GarrisonMgr->GetGarrisonLevel())
        {
            case 1:
            {
                /// Level 1
                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Horde_BreakingIntoTheTrapGame);

                if (l_Quest == nullptr)
                    return true;

                if (p_Player->IsQuestRewarded(Quests::Horde_BreakingIntoTheTrapGame) ||
                    (p_Player->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(276190)))
                {
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                }
                else if (p_Player->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_NONE)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
                else if (p_Player->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_COMPLETE)
                    p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

                 break;
            }
            case 2:
            {
                /// Level 2
                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Horde_FeedingAnArmy);

                if (l_Quest == nullptr)
                    return true;

                if (p_Player->IsQuestRewarded(Quests::Horde_FeedingAnArmy) ||
                    (p_Player->GetQuestStatus(Quests::Horde_FeedingAnArmy) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(276193)))
                {
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                }
                else if (p_Player->GetQuestStatus(Quests::Horde_FeedingAnArmy) == QUEST_STATUS_NONE)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
                else if (p_Player->GetQuestStatus(Quests::Horde_FeedingAnArmy) == QUEST_STATUS_COMPLETE)
                    p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

                break;
            }
            case 3:
            {
                /// Level 3
                Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Horde_BiggerTrapBetterRewards);

                if (l_Quest == nullptr)
                    return true;

                if (p_Player->IsQuestRewarded(Quests::Horde_BiggerTrapBetterRewards) ||
                    (p_Player->GetQuestStatus(Quests::Horde_BiggerTrapBetterRewards) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(276195)))
                {
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                }
                else if (p_Player->GetQuestStatus(Quests::Horde_BiggerTrapBetterRewards) == QUEST_STATUS_NONE)
                    p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
                else if (p_Player->GetQuestStatus(Quests::Horde_BiggerTrapBetterRewards) == QUEST_STATUS_COMPLETE)
                    p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

                break;
            }
            default:
                break;
        }


        return true;
    }

    bool npc_FarmerLokLub::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr || p_Creature == nullptr || p_Creature->GetScriptName() != CreatureScript::GetName())
            return true;

        switch (p_Action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Sumptuous Fur.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Raw Beast Hide.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Sumptuous Fur.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Raw Beast Hide.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Savage Feast.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Sumptuous Fur.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Raw Beast Hide.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Savage Feast.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Sumptuous Fur with Savage blood.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Raw Beast Hide with Savage blood.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Savage Feast with Savage blood.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 4: ///< Send shipment for fur
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentFur);
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentFur); ///< Fur
                break;
            case GOSSIP_ACTION_INFO_DEF + 5: ///< Send shipment for leather
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather);
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather); ///< Leather
                break;
            case GOSSIP_ACTION_INFO_DEF + 6: ///< Send shipment for Meat
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentMeat);
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentMeat); ///< Meat
                break;
            case GOSSIP_ACTION_INFO_DEF + 7: ///< Send shipment for ShipmentLeather2
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather2);
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather2); ///< ShipmentLeather2
                break;
            case GOSSIP_ACTION_INFO_DEF + 8: ///< Send shipment for ShipmentLeather3
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather3);
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentMeat); ///< ShipmentLeather3
                break;
            case GOSSIP_ACTION_INFO_DEF + 9: ///< Send shipment for ShipmentLeather4
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather4);
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather4); ///< ShipmentLeather4
                break;
            default:
                break;
        }
        return true;
    }

    void npc_FarmerLokLubAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        GarrisonNPCAI::SetData(p_ID, p_Value);

        if (p_ID == 1)
            m_ProductionChosen = p_Value;
    }

    int npc_FarmerLokLubAI::OnShipmentIDRequest(Player* p_Player)
    {
        return m_ProductionChosen ? m_ProductionChosen : -1;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_FarmerLokLub::GetAI(Creature* p_Creature) const
    {
        return new npc_FarmerLokLubAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// 83925 - Iron Trap                                                  ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_IronTrap::npc_IronTrap()
        : CreatureScript("npc_IronTrap_Garr")
    {
    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_IronTrapAI::npc_IronTrapAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        m_SummonerGuid = 0;
        m_FoundEntry = false;
    }

    void npc_IronTrapAI::IsSummonedBy(Unit* p_Summoner)
    {
        m_SummonerGuid = p_Summoner->GetGUID();

        AddTimedDelayedOperation(30 * IN_MILLISECONDS, [this]() -> void
        {
            if (!m_FoundEntry)
            {
                if (Unit* l_Summoner = sObjectAccessor->FindUnit(m_SummonerGuid))
                {
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellIronTrap);
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellImprovedIronTrap);
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellDeadlyIronTrap);
                }
            }
        });

        me->DespawnOrUnsummon(30 * IN_MILLISECONDS);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_SummonerGuid = p_Summoner->GetGUID();
        me->AddAura(MS::Garrison::Spells::SpellArming, me);
    }

    void npc_IronTrapAI::MoveInLineOfSight(Unit* p_Who)
    {
        Creature* l_Creature = p_Who->ToCreature();
        Unit* l_Owner        = me->GetOwner();
        Player* l_Player     =  l_Owner->ToPlayer();
        const std::vector<uint32> l_EliteEntries = { 86731, 87021, 86932 };

        if (l_Owner == nullptr || l_Player == nullptr || l_Creature == nullptr || me->GetDistance2d(p_Who) >= 2.0f || m_FoundEntry || l_Creature->GetHealthPct() >= 50.0f)
            return;

        /// Capture Algorithm

        auto l_Template                      = l_Creature->GetCreatureTemplate();
        MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

        if (l_Creature->GetMapId() != Globals::BaseMap || l_Template == nullptr || l_GarrisonMgr == nullptr)
            return;

        if (l_Template != nullptr)
        {
            switch (l_GarrisonMgr->GetGarrisonLevel())
            {
                case 1:
                    if (l_Template->rank == CreatureEliteType::CREATURE_ELITE_NORMAL && l_Template->type == CreatureType::CREATURE_TYPE_BEAST)
                    {
                        if (l_Template->family == CREATURE_FAMILY_WOLF || l_Template->family == CREATURE_FAMILY_CLEFTHOOF)
                            m_FoundEntry = true;
                    }
                    break;
                case 2:
                    if (l_Template->rank == CreatureEliteType::CREATURE_ELITE_NORMAL && l_Template->type == CreatureType::CREATURE_TYPE_BEAST)
                    {
                        if (l_Template->family == CREATURE_FAMILY_WOLF || l_Template->family == CREATURE_FAMILY_CLEFTHOOF ||
                            l_Template->family == CREATURE_FAMILY_BOAR || l_Template->family == CREATURE_FAMILY_RIVERBEAST)
                            m_FoundEntry = true;
                    }
                    break;
                case 3:

                    if (l_Template->rank == CreatureEliteType::CREATURE_ELITE_NORMAL && l_Template->type == CreatureType::CREATURE_TYPE_BEAST)
                    {
                        if (l_Template->family == CREATURE_FAMILY_WOLF || l_Template->family == CREATURE_FAMILY_CLEFTHOOF ||
                            l_Template->family == CREATURE_FAMILY_BOAR || l_Template->family == CREATURE_FAMILY_RIVERBEAST)
                            m_FoundEntry = true;
                    }
                    if (std::find(l_EliteEntries.begin(), l_EliteEntries.end(), l_Template->Entry) != l_EliteEntries.end())
                        m_FoundEntry = true;
                    break;
                default:
                    break;
            }
        }

        if (m_FoundEntry)
        {
            l_Creature->AddUnitState(UNIT_STATE_ROOT);
            l_Creature->setFaction(35);
            l_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            l_Creature->ToCreature()->DespawnOrUnsummon(8 * IN_MILLISECONDS);
            me->DespawnOrUnsummon(8 * IN_MILLISECONDS);

            if (!m_SummonerGuid)
                return;

            if (Player* l_Summoner = sObjectAccessor->GetPlayer(*me, m_SummonerGuid))
            {
                Position const l_Pos = *l_Summoner;

                AddTimedDelayedOperation(8 * IN_MILLISECONDS, [this, l_Summoner]() -> void
                {
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellIronTrap);
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellImprovedIronTrap);
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellDeadlyIronTrap);
                });

                if (l_Summoner->GetTeamId() == TEAM_HORDE)
                    me->SummonCreature(MS::Garrison::NPCs::NpcFarmerLokLubSummon, l_Pos);
                else if (l_Summoner->GetTeamId() == TEAM_ALLIANCE)
                {
                    me->SummonCreature(MS::Garrison::NPCs::NpcHomerStonefield, l_Pos);
                    me->SummonCreature(MS::Garrison::NPCs::NpcTommyJoeStonefield, l_Pos.m_positionX + 1, l_Pos.m_positionY, l_Pos.m_positionZ);
                }

                me->RemoveAura(MS::Garrison::Spells::SpellArming);

                uint32 l_RewardItemID = 0;

                if (l_Template->family == CREATURE_FAMILY_WOLF)
                    l_RewardItemID = MS::Garrison::Items::ItemFurryCagedBeast;
                else if (l_Template->family == CREATURE_FAMILY_CLEFTHOOF)
                    l_RewardItemID = MS::Garrison::Items::ItemLeatheryCagedBeast;
                else if (l_Template->family == CREATURE_FAMILY_BOAR || l_Template->family == CREATURE_FAMILY_RIVERBEAST)
                    l_RewardItemID = MS::Garrison::Items::ItemMeatyCagedBeast;

                if (l_Template->Entry == l_EliteEntries[0])
                    l_RewardItemID = MS::Garrison::Items::ItemCagedMightyClefthoof;
                else if (l_Template->Entry == l_EliteEntries[1])
                    l_RewardItemID = MS::Garrison::Items::ItemCagedMightRiverbeast;
                else if (l_Template->Entry == l_EliteEntries[2])
                    l_RewardItemID = MS::Garrison::Items::ItemCagedMightyWolf;


                if (l_RewardItemID)
                    l_Summoner->AddItem(l_RewardItemID, 1);

                if (l_Summoner->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE || l_Summoner->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                    l_Summoner->KilledMonsterCredit(NPCs::TrapL1QuestKillCredit);
            }
        }
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_IronTrap::GetAI(Creature* p_Creature) const
    {
        return new npc_IronTrapAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// 85093 - Farmer Lok lub                                             ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FarmerLokLub_Trap::npc_FarmerLokLub_Trap()
        : CreatureScript("npc_FarmerLokLub_Trap")
    {
    }

    /// Constructor
    /// @p_Creature : AI Owner
    npc_FarmerLokLub_TrapAI::npc_FarmerLokLub_TrapAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    void npc_FarmerLokLub_TrapAI::Reset()
    {
        Talk(urand(0, 7));
        me->DespawnOrUnsummon(8 * IN_MILLISECONDS);
    }

    CreatureAI* npc_FarmerLokLub_Trap::GetAI(Creature* p_Creature) const
    {
        return new npc_FarmerLokLub_TrapAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS