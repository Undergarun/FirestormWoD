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
        if (p_BuildingID == 24) ///< Barn
        {
            if (p_Player->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(40670);
        }
    }

    bool npc_FarmerLokLub::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
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

        return true;
    }

    bool npc_FarmerLokLub::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr)
            return true;

        switch (p_Action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Sumptuous Fur.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to get Raw Beast Hide.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2: ///< Send shipment for fur
                if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
                {
                    l_AI->SetData(1, MS::Garrison::Barn::ProductionTypes::TypeFur);
                    reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentFur); ///< Fur
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 3: ///< Send shipment for leather
                if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
                {
                    l_AI->SetData(1, MS::Garrison::Barn::ProductionTypes::TypeLeather);
                    reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player, MS::Garrison::Barn::ShipmentIDS::ShipmentLeather); ///< Leather
                }
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
        switch (m_ProductionChosen)
        {
            case MS::Garrison::Barn::ProductionTypes::TypeFur:
                return MS::Garrison::Barn::ShipmentIDS::ShipmentFur;
            case MS::Garrison::Barn::ProductionTypes::TypeLeather:
                return MS::Garrison::Barn::ShipmentIDS::ShipmentLeather;
            default:
                return -1;
        }
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
                    l_Summoner->RemoveAura(MS::Garrison::Spells::SpellIronTrap);
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

        if (me->GetDistance2d(p_Who) >= 2.0f || l_Creature == nullptr || m_FoundEntry || l_Creature->GetHealthPct() >= 50.0f)
            return;

        for (uint32 l_Entry : MS::Garrison::Barn::TameableEntries::gFirstLevel)
        {
            if (l_Entry == p_Who->GetEntry())
            {
                m_FoundEntry = true;
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
                if (l_Summoner->GetQuestStatus(Quests::Horde_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE ||
                    l_Summoner->GetQuestStatus(Quests::Alliance_BreakingIntoTheTrapGame) == QUEST_STATUS_INCOMPLETE)
                {
                    AddTimedDelayedOperation(8 * IN_MILLISECONDS, [this, l_Summoner]() -> void
                    {
                        l_Summoner->RemoveAura(MS::Garrison::Spells::SpellIronTrap);
                    });

                    Position const l_Pos = *l_Summoner;

                    if (l_Summoner->GetTeamId() == TEAM_HORDE)
                        me->SummonCreature(MS::Garrison::NPCs::NpcFarmerLokLubSummon, l_Pos);
                    else if (l_Summoner->GetTeamId() == TEAM_ALLIANCE)
                    {
                        me->SummonCreature(MS::Garrison::NPCs::NpcHomerStonefield, l_Pos);
                        me->SummonCreature(MS::Garrison::NPCs::NpcTommyJoeStonefield, l_Pos.m_positionX + 1, l_Pos.m_positionY, l_Pos.m_positionZ);
                    }

                    me->RemoveAura(MS::Garrison::Spells::SpellArming);
                    l_Summoner->KilledMonsterCredit(NPCs::TrapL1QuestKillCredit);

                    if (l_Creature->GetCreatureTemplate())
                    {
                        if (l_Creature->GetCreatureTemplate()->family == 1)
                            l_Summoner->AddItem(MS::Garrison::Items::ItemFurryCagedBeast, 1);
                        else if (l_Creature->GetCreatureTemplate()->family == 43)
                            l_Summoner->AddItem(MS::Garrison::Items::ItemLeatheryCagedBeast, 1);
                    }
                }
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