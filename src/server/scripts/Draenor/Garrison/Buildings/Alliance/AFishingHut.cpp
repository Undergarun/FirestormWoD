////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AFishingHut.hpp"
#include "AFishingHut_Level1Data.hpp"
#include "../../Sites/GarrisonSiteBase.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 82436 - Tharis Strongcast                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_TharisStrongcastData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
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
                    p_Me->RemoveAura(FishingHut::SpellFishingVisual);
                    p_Me->LoadEquipment(1, true);
                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing1 - TharisStrongcast_Level1::MovePointIDs::Home].O);
                    p_Me->LoadEquipment(0, true);
                    p_Me->AddAura(FishingHut::SpellFishingVisual, p_Me);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [p_This, p_Me]() -> void
                {
                    p_Me->RemoveAura(FishingHut::SpellFishingVisual);
                    p_Me->LoadEquipment(1, true);
                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing2 - TharisStrongcast_Level1::MovePointIDs::Home].O);
                    p_Me->LoadEquipment(0, true);
                    p_Me->AddAura(FishingHut::SpellFishingVisual, p_Me);
                });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        char gScriptName[] = "npc_TharisStrongcast_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 85708 - Segumi                                                    ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_SegumiData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(Segumi_Level1::MovePointLoc, Segumi_Level1::Sequence, sizeof(Segumi_Level1::Sequence), Segumi_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Home, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Home - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Logs] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Logs, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Logs - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Fire] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Fire, [p_This, p_Me]() -> void{ p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Fire - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Rack] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Rack, [p_This, p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Rack - Segumi_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Mesh] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Mesh, [p_This, p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Mesh - Segumi_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Chests] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Chests, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Chests - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Home2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Home2, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Home2 - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Porch] = [p_This, p_Me]() -> void { p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); }); };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        char gScriptName[] = "npc_Segumi_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77733 - Ron Ashton                                                ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_RonAshtonData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(RonAshton_Level1::MovePointLoc, RonAshton_Level1::Sequence, sizeof(RonAshton_Level1::Sequence), RonAshton_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Home, [p_This, p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Home - RonAshton_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Hut1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Hut1, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Hut1 - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Chests] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Chests, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Chests - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Fire] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Fire, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Fire - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Hut2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Hut2, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Hut2 - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Mesh] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Mesh, [p_This, p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Mesh - RonAshton_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Logs] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Logs, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Logs - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Porch] = [p_This, p_Me]() -> void {p_This->DoNextSequenceAction(); };

            p_This->DoNextSequenceAction();
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

    /// Constructor
    npc_RonAshton::npc_RonAshton()
        : CreatureScript("npc_RonAshton_Garr")
    {

    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_RonAshton::GetAI(Creature* p_Creature) const
    {
        return new npc_RonAshtonAI(p_Creature);
    }

    bool npc_RonAshton::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)p_Creature->GetInstanceScript();

        if (!l_GarrisonSite)
        return true;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (l_Owner != p_Player)
            return true;

        if (p_Quest->GetQuestId() == Quests::Alliance_AnglinInOurGarrison)
        {
            if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
            {
                GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

                if (l_AI == nullptr)
                    return true;

                if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Quest_FishFight))
                {
                    p_Player->AddQuest(l_Quest, p_Creature);
                    p_Player->CompleteQuest(l_Quest->GetQuestId());
                    p_Player->RewardQuest(l_Quest, 0, p_Creature, false);
                }

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_GarrisonMgr->GetBuildingWithType(BuildingType::Fishing).BuildingID);
                Position l_Pos = *l_Owner;

                if (l_BuildingEntry)
                    l_Owner->PlayStandaloneScene(l_Owner->GetGarrison()->GetGarrisonFactionIndex() ? l_BuildingEntry->HordeActivationScenePackageID : l_BuildingEntry->AllianceActivationScenePackageID, 0, l_Pos);

                l_GarrisonMgr->ActivateBuilding(l_AI->GetPlotInstanceID());
            }
        }

        return true;
    }

}   ///< namespace Garrison
}   ///< namespace MS