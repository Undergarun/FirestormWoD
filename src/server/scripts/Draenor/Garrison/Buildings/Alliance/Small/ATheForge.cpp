////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ATheForge.hpp"
#include "ATheForge_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77359 - Auria Irondreamer                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AuriaIrondreamerAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(AuriaIrondreamer_Level1::MovePointLoc, AuriaIrondreamer_Level1::Sequence, sizeof(AuriaIrondreamer_Level1::Sequence), AuriaIrondreamer_Level1::MovePointIDs::ForgeOre);

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeOre, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                      [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                  [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeOre, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ForgeFront] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeFront, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                        [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeFront - AuriaIrondreamer_Level1::MovePointIDs::ForgeFront].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                    [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeFront, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::Workorder] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::Workorder, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                       [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::Workorder - AuriaIrondreamer_Level1::MovePointIDs::Workorder].O); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderDown, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                           [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown - AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                       [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderDown, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::WorkorderFront] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderFront, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                            [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::WorkorderFront].O); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ChestLeft] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ChestLeft, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                       [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::ChestLeft].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                   [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ChestLeft, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        char ScriptName[] = "npc_AuriaIrondreamer_Garr";

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
    /// 77792 - Yulia Samras                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YuliaSamrasAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(YuliaSamras_Level1::MovePointLoc, YuliaSamras_Level1::Sequence, sizeof(YuliaSamras_Level1::Sequence), YuliaSamras_Level1::MovePointIDs::Workorder);

            p_This->m_OnPointReached[YuliaSamras_Level1::MovePointIDs::Workorder] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(YuliaSamras_Level1::DestPointDuration::Workorder, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                  [p_This]() -> void { p_This->SetFacingBuildingRelative(YuliaSamras_Level1::MovePointLoc[YuliaSamras_Level1::MovePointIDs::Workorder - YuliaSamras_Level1::MovePointIDs::Workorder].O); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        char ScriptName[] = "npc_YuliaSamras_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS