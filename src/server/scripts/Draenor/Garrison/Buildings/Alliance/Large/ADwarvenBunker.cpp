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

}   ///< namespace Garrison
}   ///< namespace MS
