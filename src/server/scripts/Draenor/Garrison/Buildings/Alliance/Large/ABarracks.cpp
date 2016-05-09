////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ABarracks.hpp"
#include "ABarracks_Level1Data.hpp"
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
    namespace npc_JonathanStephensData
    {
        extern InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(JonathanStephens_Level1::MovePointLoc, JonathanStephens_Level1::Sequence, sizeof(JonathanStephens_Level1::Sequence), JonathanStephens_Level1::MovePointIDs::Table);

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::Table] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Table, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::Table - JonathanStephens_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                               [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Table, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::Chair] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Chair, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::Chair - JonathanStephens_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                               [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_SIT_CHAIR_LOW); });
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Chair, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::Armory] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Armory, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::Armory - JonathanStephens_Level1::MovePointIDs::Table].O);
                });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE); });
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Armory, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::Middle] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Middle, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::Middle - JonathanStephens_Level1::MovePointIDs::Table].O);
                });
            };

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::Middle2] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Middle2, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::Middle2 - JonathanStephens_Level1::MovePointIDs::Table].O);
                });
            };

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::BackBed] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::BackBed, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::BackBed - JonathanStephens_Level1::MovePointIDs::Table].O);
                });
            };

            p_This->m_OnPointReached[JonathanStephens_Level1::MovePointIDs::Shields] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(JonathanStephens_Level1::DestPointDuration::Shields, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void
                {
                    p_This->SetFacingBuildingRelative(JonathanStephens_Level1::MovePointLoc[JonathanStephens_Level1::MovePointIDs::Shields - JonathanStephens_Level1::MovePointIDs::Table].O);
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

        extern char gScriptName[] = "npc_JonathanStephens_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS