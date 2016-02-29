////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonQuest.hpp"
#include "Buildings/BuildingScripts.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonScriptData.hpp"

namespace MS { namespace Garrison 
{
    /// Constructor
    GarrisonQuestPlayerScript::GarrisonQuestPlayerScript()
        : PlayerScript("GarrisonQuestPlayerScript")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player loot an item
    /// @p_Player : Player instance
    /// @p_Item   : New looted item instance
    void GarrisonQuestPlayerScript::OnItemLooted(Player* p_Player, Item* p_Item)
    {
        uint32 l_NoSpaceForCount = 0;
        ItemPosCountVec l_Destination;

        if (p_Player->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS) && p_Item && p_Item->GetEntry() == Items::ITEM_GARRISON_BLUEPRINT_BARRACKS_LEVEL1)
        {
            p_Player->QuestObjectiveSatisfy(39015, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
            return;
        }

        if ((p_Player->HasQuest(Quests::Horde_UnconventionalInventions) || p_Player->HasQuest(Quests::Alliance_UnconventionalInventions)) && p_Item->GetEntry() == WorkshopGearworks::InventionItemIDs::ItemStickyGrenades)
        {
            p_Player->QuestObjectiveSatisfy(39320, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
            p_Player->QuestObjectiveSatisfy(39307, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
            return;
        }

        switch (p_Item->GetVisibleEntry())
        {
            case Items::ItemGarrisonResources:
            case Items::ItemSecondGarrisonResources:
            {
                uint64 l_PlayerGuid = p_Player->GetGUID();
                uint64 l_ItemGuid = p_Item->GetGUID();
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_GARRISON_RESSOURCES, 30);

                p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> void
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                    {
                        uint32 l_DestroyCount = 1;

                        if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                            l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                    }
                });
                break;
            }
            case Items::ItemMine:
            {
                uint64 l_PlayerGuid  = p_Player->GetGUID();
                uint64 l_ItemGuid    = p_Item->GetGUID();
                uint32 l_RewardID    = roll_chance_i(50) ? Items::ItemTrueIronOre : Items::ItemBlackrockOre;
                uint32 l_RewardCount = 5;

                p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> void
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                    {
                        uint32 l_DestroyCount = 1;

                        if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                            l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                    }
                });

                InventoryResult l_Message = p_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, l_RewardID, l_RewardCount, &l_NoSpaceForCount);

                if (l_Message == EQUIP_ERR_OK)
                {
                    if (Item* l_Item = p_Player->StoreNewItem(l_Destination, l_RewardID, true, Item::GenerateItemRandomPropertyId(l_RewardID)))
                        p_Player->SendNewItem(l_Item, l_RewardCount, true, false, false);
                }
                else
                    p_Player->SendEquipError(l_Message, nullptr, nullptr, l_RewardID);

                if (roll_chance_i(30))
                    p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_APEXIS_CRYSTAL, roll_chance_i(25) ? urand(5, 10) : urand(1, 5));

                break;
            }
            case WorkshopGearworks::InventionItemIDs::ItemStickyGrenades:
            case WorkshopGearworks::InventionItemIDs::ItemRoboBooster:
            case WorkshopGearworks::InventionItemIDs::ItemPneumaticPowerGauntlet:
            case WorkshopGearworks::InventionItemIDs::ItemSkyTerrorPersonnalDeliverySystem:
            case WorkshopGearworks::InventionItemIDs::ItemNukularTargetPainter:
            case WorkshopGearworks::InventionItemIDs::ItemXD57BullseyeGuidedRocketKit:
            case WorkshopGearworks::InventionItemIDs::ItemGG117MicroJetpack:
            case WorkshopGearworks::InventionItemIDs::ItemSentryTurretDispenser:
                p_Player->SetCharacterWorldState(CharacterWorldStates::CharWorldStateGarrisonWorkshopGearworksInvention, 0);
            default:
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    spell_learning_blueprint::spell_learning_blueprint()
        : SpellScriptLoader("spell_learning_blueprint")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// After cast
    void spell_learning_blueprint::spell_learning_blueprint_SpellScript::HandleAfterCast()
    {
        if (GetCaster() && GetCaster()->ToPlayer())
        {
            Player * l_Player = GetCaster()->ToPlayer();

            if (l_Player->GetGarrison() && l_Player->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS))
                l_Player->QuestObjectiveSatisfy(39012, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, l_Player->GetGUID());
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SpellScript/AuraScript interface base
    /// these functions are safe to override, see notes below for usage instructions
    ///
    /// Function in which handler functions are registered, must be implemented in script
    void spell_learning_blueprint::spell_learning_blueprint_SpellScript::Register()
    {
        AfterCast += SpellCastFn(spell_learning_blueprint_SpellScript::HandleAfterCast);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Should return a fully valid SpellScript pointer.
    SpellScript* spell_learning_blueprint::GetSpellScript() const
    {
        return new spell_learning_blueprint_SpellScript();
    }

    void playerScript_Garrison_Portals_Phases::UpdateGarrisonPhaseMask(Player* p_Player)
    {
        /// UPDATE MAGE TOWER PHASEMASK DEPENDING OF QUESTS

        uint32   l_PhaseMask   = p_Player->GetPhaseMask();
        Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        if (l_GarrisonMgr->HasBuildingType(BuildingType::MageTower))
        {
            if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestFrostfireRidge))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalFrostfireRidge;
            if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestGorgrond))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalGorgrond;
            if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestNagrand))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalNagrand;
            if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestShadowmoon))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalShadowmoon;
            if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestSpiresOfArak))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalSpiresOfArak;
            if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestTalador))
                l_PhaseMask |= GarrisonPhases::PhaseMagePortalTalador;
        }

        p_Player->SetPhaseMask(l_PhaseMask, true);
    }

    void playerScript_Garrison_Portals_Phases::UpdateDraenorPhaseMask(Player* p_Player)
    {
        /// UPDATE MAGE TOWER PHASEMASK DEPENDING OF QUESTS

        uint32   l_PhaseMask   = p_Player->GetPhaseMask();

        /// Can see each deactivated portal
        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;

        Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;


        if (l_GarrisonMgr->HasBuildingType(BuildingType::MageTower))
        {
            /// World Map Phases
            switch (p_Player->GetZoneId())
            {
                case GarrisonPortals::DraenorZones::ZoneFrostfireRidge:
                {
                    if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestFrostfireRidge))
                    {
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                        break;
                    }
                    else
                    {
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                    }
                }
                case GarrisonPortals::DraenorZones::ZoneGorgrond:
                {
                    if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestGorgrond))
                    {
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                        break;
                    }
                    else
                    {
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                    }
                }
                case GarrisonPortals::DraenorZones::ZoneNagrand:
                {
                    if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestNagrand))
                    {
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                        break;
                    }
                    else
                    {
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                    }
                }
                case GarrisonPortals::DraenorZones::ZoneShadowmoon:
                {
                    if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestShadowmoon))
                    {
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                        break;
                    }
                    else
                    {
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                    }
                }
                case GarrisonPortals::DraenorZones::ZoneSpiresOfArak:
                {
                    if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestSpiresOfArak))
                    {
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                        break;
                    }
                    else
                    {
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                    }
                }
                case GarrisonPortals::DraenorZones::ZoneTalador:
                {
                    if (p_Player->IsQuestRewarded(GarrisonPortals::PortalsQuests::QuestTalador))
                    {
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                        break;
                    }
                    else
                    {
                        l_PhaseMask |= GarrisonPortals::DraenorPortalPhases::WorldPhaseDeactivatedPortal;
                        l_PhaseMask &= ~GarrisonPortals::DraenorPortalPhases::WorldPhaseActivatedPortal;
                    }
                }
                    break;
                default:
                    break;
            }
        }

        p_Player->SetPhaseMask(l_PhaseMask, true);
    }

    void playerScript_Garrison_Portals_Phases::OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneID, uint32 p_NewAreaId)
    {
        /// World Map Phases
        switch (p_NewZoneId)
        {
            case GarrisonPortals::DraenorZones::ZoneFrostfireRidge:
            case GarrisonPortals::DraenorZones::ZoneGorgrond:
            case GarrisonPortals::DraenorZones::ZoneNagrand:
            case GarrisonPortals::DraenorZones::ZoneShadowmoon:
            case GarrisonPortals::DraenorZones::ZoneSpiresOfArak:
            case GarrisonPortals::DraenorZones::ZoneTalador:
                UpdateDraenorPhaseMask(p_Player);
                break;
            default:
                break;
        }

        if (!p_Player->IsInGarrison())
            return;

        Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        if (p_Player->IsInGarrison())
            UpdateGarrisonPhaseMask(p_Player);
    }

    void playerScript_Garrison_Portals_Phases::OnLogin(Player* p_Player)
    {
        switch (p_Player->GetMapId())
        {
            /// Garrison Phases
            case MapIDs::MapGarrisonAllianceLevel1:
            case MapIDs::MapGarrisonAllianceLevel2:
            case MapIDs::MapGarrisonAllianceLevel3:
            case MapIDs::MapGarrisonHordeLevel1:
            case MapIDs::MapGarrisonHordeLevel2:
            case MapIDs::MapGarrisonHordeLevel3:
                UpdateGarrisonPhaseMask(p_Player);
                break;
            default:
                break;
        }

        /// World Map Phases
        switch (p_Player->GetZoneId())
        {
            case GarrisonPortals::DraenorZones::ZoneFrostfireRidge:
            case GarrisonPortals::DraenorZones::ZoneGorgrond:
            case GarrisonPortals::DraenorZones::ZoneNagrand:
            case GarrisonPortals::DraenorZones::ZoneShadowmoon:
            case GarrisonPortals::DraenorZones::ZoneSpiresOfArak:
            case GarrisonPortals::DraenorZones::ZoneTalador:
                UpdateDraenorPhaseMask(p_Player);
                break;
            default:
                break;
        }
    }

    void playerScript_Garrison_Portals_Phases::OnQuestCleared(Player* p_Player, Quest const* p_Quest)
    {
        switch (p_Player->GetMapId())
        {
            /// Garrison Phases
            case MapIDs::MapGarrisonAllianceLevel1:
            case MapIDs::MapGarrisonAllianceLevel2:
            case MapIDs::MapGarrisonAllianceLevel3:
            case MapIDs::MapGarrisonHordeLevel1:
            case MapIDs::MapGarrisonHordeLevel2:
            case MapIDs::MapGarrisonHordeLevel3:
                UpdateGarrisonPhaseMask(p_Player);
                break;
            default:
                break;
        }

        /// World Map Phases
        switch (p_Player->GetZoneId())
        {
            case GarrisonPortals::DraenorZones::ZoneFrostfireRidge:
            case GarrisonPortals::DraenorZones::ZoneGorgrond:
            case GarrisonPortals::DraenorZones::ZoneNagrand:
            case GarrisonPortals::DraenorZones::ZoneShadowmoon:
            case GarrisonPortals::DraenorZones::ZoneSpiresOfArak:
            case GarrisonPortals::DraenorZones::ZoneTalador:
                UpdateDraenorPhaseMask(p_Player);
                break;
            default:
                break;
        }
    }

    void playerScript_Garrison_Portals_Phases::OnQuestReward(Player* p_Player, const Quest* p_Quest)
    {
        switch (p_Player->GetMapId())
        {
            /// Garrison Phases
            case MapIDs::MapGarrisonAllianceLevel1:
            case MapIDs::MapGarrisonAllianceLevel2:
            case MapIDs::MapGarrisonAllianceLevel3:
            case MapIDs::MapGarrisonHordeLevel1:
            case MapIDs::MapGarrisonHordeLevel2:
            case MapIDs::MapGarrisonHordeLevel3:
                UpdateGarrisonPhaseMask(p_Player);
                break;
            default:
                break;
        }

        /// World Map Phases
        switch (p_Player->GetZoneId())
        {
            case GarrisonPortals::DraenorZones::ZoneFrostfireRidge:
            case GarrisonPortals::DraenorZones::ZoneGorgrond:
            case GarrisonPortals::DraenorZones::ZoneNagrand:
            case GarrisonPortals::DraenorZones::ZoneShadowmoon:
            case GarrisonPortals::DraenorZones::ZoneSpiresOfArak:
            case GarrisonPortals::DraenorZones::ZoneTalador:
                UpdateDraenorPhaseMask(p_Player);
                p_Player->QuestObjectiveSatisfy(ObjectIDs::MageTowerQuestActivateOgreWaygate, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
                break;
            default:
                break;
        }
    }

    void playerScript_Garrison_Quests_Phases::OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneID, uint32 p_NewAreaId)
    {
        if (!p_Player->IsInGarrison())
            return;

        if ((p_Player->GetTeamId() == TEAM_ALLIANCE && p_Player->HasQuest(Quests::Alliance_LostInTransition)) ||
            (p_Player->GetTeamId() == TEAM_HORDE && p_Player->HasQuest(Quests::Horde_LostInTransition)))
        {
            uint32 l_PhaseMask = p_Player->GetPhaseMask();
            l_PhaseMask |= GarrisonPhases::PhaseLostInTransitionQuest;
            p_Player->SetPhaseMask(l_PhaseMask, true);
        }
    }

    void playerScript_Garrison_Quests_Phases::OnQuestAccept(Player* p_Player, const Quest* p_Quest)
    {
        if (!p_Player->IsInGarrison())
            return;

        if (p_Player->GetTeamId() == TEAM_ALLIANCE && p_Quest->GetQuestId() == Quests::Alliance_LostInTransition ||
            p_Player->GetTeamId() == TEAM_HORDE && p_Quest->GetQuestId() == Quests::Horde_LostInTransition)
        {
            uint32 l_PhaseMask = p_Player->GetPhaseMask();
            l_PhaseMask |= GarrisonPhases::PhaseLostInTransitionQuest;
            p_Player->SetPhaseMask(l_PhaseMask, true);
        }
    }

    void playerScript_Garrison_Quests_Phases::OnQuestReward(Player* p_Player, const Quest* p_Quest)
    {
        if (!p_Player->IsInGarrison())
            return;

        if (p_Player->GetTeamId() == TEAM_ALLIANCE && p_Quest->GetQuestId() == Quests::Alliance_LostInTransition ||
            p_Player->GetTeamId() == TEAM_HORDE && p_Quest->GetQuestId() == Quests::Horde_LostInTransition)
        {
            uint32 l_PhaseMask = p_Player->GetPhaseMask();
            l_PhaseMask &= ~GarrisonPhases::PhaseLostInTransitionQuest;
            p_Player->SetPhaseMask(l_PhaseMask, true);
        }
    }
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_Quest()
{
    new MS::Garrison::GarrisonBuildingAuraPlayerScript;
    new MS::Garrison::GarrisonQuestPlayerScript;
    new MS::Garrison::playerScript_Garrison_Portals_Phases;
    new MS::Garrison::playerScript_Garrison_Quests_Phases;
    new MS::Garrison::spell_learning_blueprint;
}