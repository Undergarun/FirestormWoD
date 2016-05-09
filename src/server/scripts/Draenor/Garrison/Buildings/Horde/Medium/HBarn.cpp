////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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
            case Buildings::Barn_Barn_Level1:
                if (p_Player->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                    p_Player->KilledMonsterCredit(40670);
                break;
            case Buildings::Barn_Barn_Level2:
                if (p_Player->GetQuestStatus(Quests::Horde_FeedingAnArmy) == QUEST_STATUS_INCOMPLETE)
                    p_Player->QuestObjectiveSatisfy(40674, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
                break;
            case Buildings::Barn_Barn_Level3:
                if (p_Player->GetQuestStatus(Quests::Horde_BiggerTrapBetterRewards) == QUEST_STATUS_INCOMPLETE)
                    p_Player->QuestObjectiveSatisfy(40693, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
                break;
            default:
                break;
        }
    }

    bool npc_FarmerLokLub::HandleGossipActions(Player* p_Player, Creature* p_Creature, uint32 p_QuestID, uint32 p_Action)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(p_QuestID);

        if (l_Quest == nullptr)
            return false;

        if (p_Player->IsQuestRewarded(p_QuestID) || (p_Player->GetQuestStatus(p_QuestID) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(276190)))
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, p_Action);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_Quest->GetQuestId(), 4);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    bool npc_FarmerLokLub::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        uint32 l_PlotInstanceID = l_AI->GetPlotInstanceID();

        if (!l_PlotInstanceID)
            return false;

        GarrisonBuilding l_Building = l_GarrisonMgr->GetBuilding(l_PlotInstanceID);

        switch (l_Building.BuildingID)
        {
            case Buildings::Barn_Barn_Level1:
                return HandleGossipActions(p_Player, p_Creature, Quests::Horde_BreakingIntoTheTrapGame, GOSSIP_ACTION_INFO_DEF + 1);
            case Buildings::Barn_Barn_Level2:
                return HandleGossipActions(p_Player, p_Creature, Quests::Horde_FeedingAnArmy, GOSSIP_ACTION_INFO_DEF + 2);
            case Buildings::Barn_Barn_Level3:
                return HandleGossipActions(p_Player, p_Creature, Quests::Horde_BiggerTrapBetterRewards, GOSSIP_ACTION_INFO_DEF + 3);
            default:
                break;
        }

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    bool npc_FarmerLokLub::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
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
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentFurredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentFurredBeast); ///< Fur
                break;
            case GOSSIP_ACTION_INFO_DEF + 5: ///< Send shipment for leather
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentLeatheredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeatheredBeast); ///< Leather
                break;
            case GOSSIP_ACTION_INFO_DEF + 6: ///< Send shipment for Meat
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentMeatyBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentMeatyBeast); ///< Meat
                break;
            case GOSSIP_ACTION_INFO_DEF + 7: ///< Send shipment for more fur + savage blood
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulFurredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulFurredBeast); ///< ShipmentLeather2
                break;
            case GOSSIP_ACTION_INFO_DEF + 8: ///< Send shipment for more leather + savage blood
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulLeatheredBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulLeatheredBeast); ///< ShipmentLeather3
                break;
            case GOSSIP_ACTION_INFO_DEF + 9: ///< Send shipment for more meat + savage blood
                l_AI->SetData(1, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulMeatyBeast);
                l_AI->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentPowerfulMeatyBeast); ///< ShipmentLeather4
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

    int npc_FarmerLokLubAI::OnShipmentIDRequest(Player* /*p_Player*/)
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
        Player* l_Summoner = sObjectAccessor->GetPlayer(*me, m_SummonerGuid);
        const std::vector<uint32> l_EliteEntries = { 86731, 87021, 86932 };

        if (l_Summoner == nullptr || l_Creature == nullptr || me->GetDistance2d(p_Who) >= 2.0f || m_FoundEntry || l_Creature->GetHealthPct() >= 50.0f)
            return;

        /// Capture Algorithm

        auto l_Template                      = l_Creature->GetCreatureTemplate();
        MS::Garrison::Manager* l_GarrisonMgr = l_Summoner->GetGarrison();

        if (l_Template == nullptr || l_GarrisonMgr == nullptr)
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

            Position const l_Pos = *l_Summoner;
            uint64 l_GUID = l_Summoner->GetGUID();

            AddTimedDelayedOperation(8 * IN_MILLISECONDS, [this, l_GUID]() -> void
            {
                Player* l_Summoner = HashMapHolder<Player>::Find(l_GUID);

                if (l_Summoner != nullptr)
                {
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellIronTrap);
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellImprovedIronTrap);
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellDeadlyIronTrap);
                }
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
            else if (l_Summoner->GetQuestStatus(Quests::Horde_FeedingAnArmy) == QUEST_STATUS_INCOMPLETE || l_Summoner->GetQuestStatus(Quests::Alliance_FeedingAnArmy) == QUEST_STATUS_INCOMPLETE)
                l_Summoner->KilledMonsterCredit(NPCs::TrapL2QuestKillCredit);
            else if (l_Summoner->GetQuestStatus(Quests::Horde_BiggerTrapBetterRewards) == QUEST_STATUS_INCOMPLETE || l_Summoner->GetQuestStatus(Quests::Alliance_BiggerTrapBetterRewards) == QUEST_STATUS_INCOMPLETE)
                l_Summoner->KilledMonsterCredit(NPCs::TrapL3QuestKillCredit);
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