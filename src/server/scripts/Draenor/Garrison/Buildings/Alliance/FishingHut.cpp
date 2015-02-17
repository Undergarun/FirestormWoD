////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "FishingHut.hpp"
#include "FishingHut_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{



    namespace npc_TharisStrongcastData
    {
        std::function<void(GarrisonNPCAI*, Creature*)> FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetupActionSequence(TharisStrongcast_Level1::MovePointLoc, TharisStrongcast_Level1::Sequence, sizeof(TharisStrongcast_Level1::Sequence), TharisStrongcast_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Home, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Home - TharisStrongcast_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Rack] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Rack, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Rack - TharisStrongcast_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [p_This, p_Me]() -> void
                {
                    p_Me->RemoveAura(SpellFishingVisual);
                    p_Me->LoadEquipment(1, true);
                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing1 - TharisStrongcast_Level1::MovePointIDs::Home].O);
                    p_Me->LoadEquipment(0, true);
                    p_Me->AddAura(SpellFishingVisual, p_Me);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [p_This, p_Me]() -> void
                {
                    p_Me->RemoveAura(SpellFishingVisual);
                    p_Me->LoadEquipment(1, true);
                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing2 - TharisStrongcast_Level1::MovePointIDs::Home].O);
                    p_Me->LoadEquipment(0, true);
                    p_Me->AddAura(SpellFishingVisual, p_Me);
                });
            };

            p_This->DoNextSequenceAction();
        };

        std::function<void(GarrisonNPCAI*, Creature*)> FnLevel2 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        std::function<void(GarrisonNPCAI*, Creature*)> FnLevel3 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        char gScriptName[] = "npc_TharisStrongcast_Garr";
    }





    //////////////////////////////////////////////////////////////////////////
    /// 82436 - Tharis Strongcast                                         ////
    //////////////////////////////////////////////////////////////////////////

//     /// Constructor
//     npc_TharisStrongcast::npc_TharisStrongcast()
//         : CreatureScript("npc_TharisStrongcast_Garr")
//     {
// 
//     }
// 
//     //////////////////////////////////////////////////////////////////////////
//     //////////////////////////////////////////////////////////////////////////
// 
//     /// Called when a CreatureAI object is needed for the creature.
//     /// @p_Creature : Target creature instance
//     CreatureAI * npc_TharisStrongcast::GetAI(Creature * p_Creature) const
//     {
//         return new npc_TharisStrongcastAI(p_Creature);
//     }
// 
//     //////////////////////////////////////////////////////////////////////////
//     //////////////////////////////////////////////////////////////////////////
// 
//     /// Constructor
//     npc_TharisStrongcast::npc_TharisStrongcastAI::npc_TharisStrongcastAI(Creature * p_Creature)
//         : GarrisonNPCAI(p_Creature)
//     {
//         SetAIObstacleManagerEnabled(true);
//     }
// 
//     //////////////////////////////////////////////////////////////////////////
//     //////////////////////////////////////////////////////////////////////////
// 
//     /// When the building ID is set
//     /// @p_BuildingID : Set building ID
//     void npc_TharisStrongcast::npc_TharisStrongcastAI::OnSetBuildingID(uint32 p_BuildingID)
//     {
//         m_OnPointReached.clear();
// 
//         GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);
// 
//         if (!l_BuildingEntry)
//             return;
// 
//         switch (l_BuildingEntry->BuildingLevel)
//         {
//             case 1:
//                 SetupLevel1();
//                 break;
// 
//             case 2:
//                 SetupLevel2();
//                 break;
// 
//             case 3:
//                 SetupLevel3();
//                 break;
// 
//             default:
//                break;
//         }
//     }
// 
//     //////////////////////////////////////////////////////////////////////////
//     //////////////////////////////////////////////////////////////////////////
// 
//     /// Setup AI for level 1
//     void npc_TharisStrongcast::npc_TharisStrongcastAI::SetupLevel1()
//     {
//         SetupActionSequence(TharisStrongcast_Level1::MovePointLoc, TharisStrongcast_Level1::Sequence, sizeof(TharisStrongcast_Level1::Sequence), TharisStrongcast_Level1::MovePointIDs::Home);
// 
//         m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Home] = [this]() -> void
//         {
//             AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Home, [this]() -> void { DoNextSequenceAction(); });
//             m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Home - TharisStrongcast_Level1::MovePointIDs::Home].O); });
//         };
// 
//         m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Rack] = [this]() -> void
//         {
//             AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Rack, [this]() -> void { DoNextSequenceAction(); });
//             m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Rack - TharisStrongcast_Level1::MovePointIDs::Home].O); });
//         };
// 
//         m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing1] = [this]() -> void
//         {
//             AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [this]() -> void
//             {
//                 me->RemoveAura(SpellFishingVisual);
//                 me->LoadEquipment(1, true);
//                 DoNextSequenceAction();
//             });
//             m_DelayedOperations.push([this]() -> void 
//             {
//                 SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing1 - TharisStrongcast_Level1::MovePointIDs::Home].O);
//                 me->LoadEquipment(0, true);
//                 me->AddAura(SpellFishingVisual, me);
//             });
//         };
// 
//         m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing2] = [this]() -> void
//         {
//             AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [this]() -> void 
//             {
//                 me->RemoveAura(SpellFishingVisual); 
//                 me->LoadEquipment(1, true);
//                 DoNextSequenceAction();
//             });
//             m_DelayedOperations.push([this]() -> void
//             {
//                 SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing2 - TharisStrongcast_Level1::MovePointIDs::Home].O);
//                 me->LoadEquipment(0, true);
//                 me->AddAura(SpellFishingVisual, me);
//             });
//         };
// 
//         DoNextSequenceAction();
//     }
// 
//     /// Setup AI for level 2
//     void npc_TharisStrongcast::npc_TharisStrongcastAI::SetupLevel3()
//     {
// 
//     }
// 
//     /// Setup AI for level 3
//     void npc_TharisStrongcast::npc_TharisStrongcastAI::SetupLevel2()
//     {
// 
//     }

    //////////////////////////////////////////////////////////////////////////
    /// 85708 - Segumi                                                    ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Segumi::npc_Segumi()
        : CreatureScript("npc_Segumi_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Segumi::GetAI(Creature * p_Creature) const
    {
        return new npc_SegumiAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Segumi::npc_SegumiAI::npc_SegumiAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the building ID is set
    /// @p_BuildingID : Set building ID
    void npc_Segumi::npc_SegumiAI::OnSetBuildingID(uint32 p_BuildingID)
    {
        m_OnPointReached.clear();

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

        if (!l_BuildingEntry)
            return;

        switch (l_BuildingEntry->BuildingLevel)
        {
            case 1:
                SetupLevel1();
                break;

            case 2:
                SetupLevel2();
                break;

            case 3:
                SetupLevel3();
                break;

            default:
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Setup AI for level 1
    void npc_Segumi::npc_SegumiAI::SetupLevel1()
    {
        SetupActionSequence(Segumi_Level1::MovePointLoc, Segumi_Level1::Sequence, sizeof(Segumi_Level1::Sequence), Segumi_Level1::MovePointIDs::Home);

        m_OnPointReached[Segumi_Level1::MovePointIDs::Home] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Home, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Home - Segumi_Level1::MovePointIDs::Home].O); });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Logs] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Logs, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Logs - Segumi_Level1::MovePointIDs::Home].O); });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Fire] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Fire, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Fire - Segumi_Level1::MovePointIDs::Home].O); });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Rack] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Rack, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                DoNextSequenceAction();
            });
            m_DelayedOperations.push([this]() -> void
            {
                SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Rack - Segumi_Level1::MovePointIDs::Home].O);
                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
            });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Mesh] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Mesh, [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                DoNextSequenceAction();
            });
            m_DelayedOperations.push([this]() -> void
            {
                SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Mesh - Segumi_Level1::MovePointIDs::Home].O);
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
            });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Chests] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Chests, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Chests - Segumi_Level1::MovePointIDs::Home].O); });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Home2] = [this]() -> void
        {
            AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Home2, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Home2 - Segumi_Level1::MovePointIDs::Home].O); });
        };

        m_OnPointReached[Segumi_Level1::MovePointIDs::Porch] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };

        DoNextSequenceAction();
    }

    /// Setup AI for level 2
    void npc_Segumi::npc_SegumiAI::SetupLevel3()
    {

    }

    /// Setup AI for level 3
    void npc_Segumi::npc_SegumiAI::SetupLevel2()
    {

    }


}   ///< namespace Garrison
}   ///< namespace MS