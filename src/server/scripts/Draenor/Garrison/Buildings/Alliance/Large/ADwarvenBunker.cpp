////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ADwarvenBunker.hpp"
#include "ADwarvenBunker_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 84594 - Gussof Forgefire                                           ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GussofForgefireData
    {
        extern InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(GussofForgefire_Level1::MovePointLoc, GussofForgefire_Level1::Sequence, sizeof(GussofForgefire_Level1::Sequence), GussofForgefire_Level1::MovePointIDs::Nothing);

            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Nothing] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Nothing, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(GussofForgefire_Level1::MovePointLoc[GussofForgefire_Level1::MovePointIDs::Nothing - GussofForgefire_Level1::MovePointIDs::Nothing].O);
                });
            };

            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Stairs1] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };
            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Stairs2] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };
            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Stairs3] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };

            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Stairs4] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Stairs4, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(GussofForgefire_Level1::MovePointLoc[GussofForgefire_Level1::MovePointIDs::Stairs4 - GussofForgefire_Level1::MovePointIDs::Nothing].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE); });
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Stairs4, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Anvil] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Anvil, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(GussofForgefire_Level1::MovePointLoc[GussofForgefire_Level1::MovePointIDs::Anvil - GussofForgefire_Level1::MovePointIDs::Nothing].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(1, true);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Anvil, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[GussofForgefire_Level1::MovePointIDs::Canon] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Canon, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(GussofForgefire_Level1::MovePointLoc[GussofForgefire_Level1::MovePointIDs::Canon - GussofForgefire_Level1::MovePointIDs::Nothing].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                              [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE); });
                p_This->AddTimedDelayedOperation(GussofForgefire_Level1::DestPointDuration::Canon, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->DoNextSequenceAction();
        };

        extern InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        extern InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        extern char gScriptName[] = "npc_GussofForgefire_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77377 - KristenStone forge                                         ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KristenStoneforgeData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(KristenStoneforge_Level1::MovePointLoc, KristenStoneforge_Level1::Sequence, sizeof(KristenStoneforge_Level1::Sequence), KristenStoneforge_Level1::MovePointIDs::Table);

            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::Table] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::Table, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(KristenStoneforge_Level1::MovePointLoc[KristenStoneforge_Level1::MovePointIDs::Table - KristenStoneforge_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::Table, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::Stairs1] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };
            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::Stairs2] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };
            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::Stairs3] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };
            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::Stairs4] = [p_This]() -> void { p_This->m_DelayedOperations.push([p_This]() -> void { p_This->DoNextSequenceAction(); }); };

            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(KristenStoneforge_Level1::MovePointLoc[KristenStoneforge_Level1::MovePointIDs::Chest - KristenStoneforge_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::Chest, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::UpTable] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::UpTable, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(KristenStoneforge_Level1::MovePointLoc[KristenStoneforge_Level1::MovePointIDs::UpTable - KristenStoneforge_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                  [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::UpTable, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[KristenStoneforge_Level1::MovePointIDs::CanonBalls] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::CanonBalls, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(KristenStoneforge_Level1::MovePointLoc[KristenStoneforge_Level1::MovePointIDs::CanonBalls - KristenStoneforge_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                     [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(KristenStoneforge_Level1::DestPointDuration::CanonBalls, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        char gScriptName[] = "npc_KristenStoneforge_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 89065 - Dalana Clarke
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_DalanaClarke_Garr::npc_DalanaClarke_Garr()
        : CreatureScript("npc_DalanaClarke_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_DalanaClarke_Garr::GetAI(Creature* p_Creature) const
    {
        return new npc_DalanaClarke_GarrAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_DalanaClarke_Garr::npc_DalanaClarke_GarrAI::npc_DalanaClarke_GarrAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    bool npc_DalanaClarke_Garr::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_GarrisonMgr && l_AI)
        {
            if (l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuilding(static_cast<GarrisonNPCAI*>(l_AI)->GetPlotInstanceID())) >= 2)
            {
                if (!p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonArmoryWeeklyCurrencyGain))
                    p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::ArmoryWeeklySeal, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            }
        }

        p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    bool npc_DalanaClarke_Garr::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Action == GOSSIP_ACTION_INFO_DEF)
        {
            if (p_Creature->AI())
                static_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);
        }
        else if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_SEAL_OF_TEMPERED_FATE, 1, 1);
            p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonArmoryWeeklyCurrencyGain, 1);
            p_Creature->SendPlaySpellVisualKit(179, 0); /// 53 SpellCastDirected
            p_Player->SendPlaySpellVisualKit(362, 1);   /// 113 EmoteSalute
        }

        return true;
    }

}   ///< namespace Garrison
}   ///< namespace MS
