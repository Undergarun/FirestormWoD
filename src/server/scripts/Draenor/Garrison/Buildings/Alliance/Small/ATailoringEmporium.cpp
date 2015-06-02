////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ATailoringEmporium.hpp"
#include "ATailoringEmporium_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77382 - Christopher Macdonald                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ChristopherMacdonaldData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(ChristopherMacdonald_Level1::MovePointLoc, ChristopherMacdonald_Level1::Sequence, sizeof(ChristopherMacdonald_Level1::Sequence), ChristopherMacdonald_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Home, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Home - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Home, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Chest - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Chest, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Armory] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Armory, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Armory - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Armory, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Front] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Front, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Front - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Front, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Materials] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Materials, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Materials - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Materials, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        char gScriptName[] = "npc_ChristopherMacdonald_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77778 - Kaylie Macdonald                                          ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KaylieMacdonaldAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(KaylieMacdonald_Level1::MovePointLoc, KaylieMacdonald_Level1::Sequence, sizeof(KaylieMacdonald_Level1::Sequence), KaylieMacdonald_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[KaylieMacdonald_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Home, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(KaylieMacdonald_Level1::MovePointLoc[KaylieMacdonald_Level1::MovePointIDs::Home - KaylieMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Home, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[KaylieMacdonald_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(KaylieMacdonald_Level1::MovePointLoc[KaylieMacdonald_Level1::MovePointIDs::Chest - KaylieMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Chest, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[KaylieMacdonald_Level1::MovePointIDs::Body] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Body, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(KaylieMacdonald_Level1::MovePointLoc[KaylieMacdonald_Level1::MovePointIDs::Body - KaylieMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                });
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Body, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
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

    /// Constructor
    npc_KaylieMacdonald::npc_KaylieMacdonald()
        : CreatureScript("npc_KaylieMacdonald_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_KaylieMacdonald::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_KaylieMacdonald::GetAI(Creature * p_Creature) const
    {
        return new npc_KaylieMacdonaldAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS