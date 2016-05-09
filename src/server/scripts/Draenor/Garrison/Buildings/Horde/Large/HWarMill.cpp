////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HWarMill.hpp"
#include "HWarMill_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 82436 - Grun Lek                                                  ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GrunLekData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(GrunLek::MovePointLoc, GrunLek::Sequence, sizeof(GrunLek::Sequence), GrunLek::MovePointIDs::Out);

            p_This->m_OnPointReached[GrunLek::MovePointIDs::Out] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(GrunLek::DestPointDuration::Out, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                 [p_This]() -> void { p_This->SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Out - GrunLek::MovePointIDs::Out].O); });
            };

            p_This->m_OnPointReached[GrunLek::MovePointIDs::Carpet] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(GrunLek::DestPointDuration::Carpet, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                    [p_This]() -> void { p_This->SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Carpet - GrunLek::MovePointIDs::Out].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_ONESHOT_TALK); });
                p_This->AddTimedDelayedOperation(3 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_ONESHOT_TALK); });
            };

            p_This->m_OnPointReached[GrunLek::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(GrunLek::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                   [p_This]() -> void { p_This->SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Chest - GrunLek::MovePointIDs::Out].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(5 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[GrunLek::MovePointIDs::Table] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(GrunLek::DestPointDuration::Table, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                   [p_This]() -> void { p_This->SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Table - GrunLek::MovePointIDs::Out].O); });

                p_This->AddTimedDelayedOperation(1 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(9 * IN_MILLISECONDS, [p_This, p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                    p_Me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                });
                p_This->AddTimedDelayedOperation(11 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(19 * IN_MILLISECONDS, [p_This, p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);        });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        char gScriptName[] = "npc_GrunLek_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79781 - Frostwall Grunt
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallGrunt::npc_FrostWallGrunt()
        : CreatureScript("npc_FrostwallGrunt_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_FrostWallGrunt::GetAI(Creature * p_Creature) const
    {
        return new npc_FrostWallGruntAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallGrunt::npc_FrostWallGruntAI::npc_FrostWallGruntAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_FrostWallGrunt::npc_FrostWallGruntAI::UpdateAI(const uint32 /*p_Diff*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void npc_FrostWallGrunt::npc_FrostWallGruntAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::BuildingID && p_Value == Buildings::DwarvenBunker_WarMill_Level1)
            me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
    }

    //////////////////////////////////////////////////////////////////////////
    /// 87630 - Frostwall Smith
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallSmith::npc_FrostWallSmith()
        : CreatureScript("npc_FrostWallSmith_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_FrostWallSmith::GetAI(Creature * p_Creature) const
    {
        return new npc_FrostWallSmithAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallSmith::npc_FrostWallSmithAI::npc_FrostWallSmithAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_FrostWallSmith::npc_FrostWallSmithAI::UpdateAI(const uint32 /*p_Diff*/)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void npc_FrostWallSmith::npc_FrostWallSmithAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::BuildingID && p_Value == Buildings::DwarvenBunker_WarMill_Level1)
        {
            me->LoadEquipment(1, true);
            me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS