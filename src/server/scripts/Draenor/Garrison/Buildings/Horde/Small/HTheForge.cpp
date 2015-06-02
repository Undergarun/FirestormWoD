////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HTheForge.hpp"
#include "HTheForge_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79867 - Orgek Ironhand                                             ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_OrgekIronhandAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(OrgekIronhand_Level1::MovePointLoc, OrgekIronhand_Level1::Sequence, sizeof(OrgekIronhand_Level1::Sequence), OrgekIronhand_Level1::MovePointIDs::Anvil);

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Anvil] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Anvil, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Anvil - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(1, true);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Anvil, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Front] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Front, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Front - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Front, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Forge1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge1, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                 [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Forge1 - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge1, [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Forge2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge2, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                 [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Forge2 - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                             [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge2, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Chest - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Chest, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        char ScriptName[] = "npc_OrgekIronhand_Garr";

        std::vector<SkillNPC_RecipeEntry> Recipes
        {
            { 171692,     0 },
            { 171693,     0 },
            { 171691,     0 },
            { 171694, 28366 },
            { 171695, 28366 },
            { 171696, 28366 },
            { 171697, 28366 },
            { 171698, 28366 },
            { 171699, 28366 },
            { 171700, 28367 },
            { 171701, 28367 },
            { 171702, 28367 },
            { 171703, 28367 },
            { 171704, 28367 },
            { 171705, 28367 },
            { 171706, 28367 },
            { 171707, 28367 },
            { 173355, 28366 },
            { 178243, 28366 },
            { 178245, 28366 },
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79817 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KinjaAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(Kinja_Level1::MovePointLoc, Kinja_Level1::Sequence, sizeof(Kinja_Level1::Sequence), Kinja_Level1::MovePointIDs::Anvil);

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Anvil] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Anvil, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                        [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Anvil - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                    [p_Me]() -> void
                {
                    p_Me->LoadEquipment(1, true);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Anvil, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Front] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Front, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                        [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Front - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                    [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Front, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Forge1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge1, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                         [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Forge1 - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge1, [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Forge2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge2, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                         [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Forge2 - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge2, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        char ScriptName[] = "npc_Kinja_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS