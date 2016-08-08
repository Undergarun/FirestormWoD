////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "GarrisonQuest.hpp"
#include "Buildings/BuildingScripts.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonScriptData.hpp"
#include "Buildings/Alliance/Medium/ATradingPost.hpp"

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

        if (p_Player->HasQuest(Quests::Horde_BuildYourBarracks) && p_Item && p_Item->GetEntry() == Items::ITEM_GARRISON_BLUEPRINT_BARRACKS_LEVEL1)
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

                p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> bool
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                    {
                        uint32 l_DestroyCount = 1;

                        if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                            l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                    }

                    return true;
                });
                break;
            }
            case Items::ItemMine:
            {
                uint64 l_PlayerGuid  = p_Player->GetGUID();
                uint64 l_ItemGuid    = p_Item->GetGUID();
                uint32 l_RewardID    = roll_chance_i(50) ? Items::ItemTrueIronOre : Items::ItemBlackrockOre;
                uint32 l_RewardCount = 5;

                p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> bool
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                    {
                        uint32 l_DestroyCount = 1;

                        if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                            l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                    }

                    return true;
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
//                p_Item->SetSpellCharges(0, p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonWorkshopGearworksInventionCharges));
                break;
            /// Scribe Quarters
            case 119126:
            {
                uint64 l_PlayerGuid  = p_Player->GetGUID();
                uint64 l_ItemGuid    = p_Item->GetGUID();
                uint32 l_RewardCount = 1;

                std::vector<uint32> l_Rewards =
                {
                    118592,
                    119094,
                    119095,
                    119096,
                    119097,
                    119098,
                    119099,
                    119100,
                    119101,
                    119102
                };

                p_Player->AddCriticalOperation([l_PlayerGuid]() -> bool
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                        l_Player->DestroyItemCount(119126, 1, true, false);

                    return true;
                });

                for (int l_Itr = 0; l_Itr < 2; ++l_Itr)
                {
                    /// check space and find places
                    uint32 l_RewardID = l_Rewards[urand(0, l_Rewards.size() - 1)];

                    InventoryResult l_Message = p_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, l_RewardID, l_RewardCount, &l_NoSpaceForCount);

                    if (l_Message == EQUIP_ERR_OK)
                    {
                        if (Item* l_Item = p_Player->StoreNewItem(l_Destination, l_RewardID, true))
                            p_Player->SendNewItem(l_Item, l_RewardCount, true, false, false);
                    }
                    else
                        p_Player->SendEquipError(l_Message, nullptr, nullptr, l_RewardID);
                }

                break;
            }
            //////////////////
            /// Stables
            case 118469: ///< Black Claw of Sethe
            {
                uint64 l_PlayerGuid = p_Player->GetGUID();

                if (p_Player->HasItemCount(118469, 2))
                {
                    p_Player->AddCriticalOperation([l_PlayerGuid]() -> bool
                    {
                        if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                            l_Player->DestroyItemCount(118469, 1, true, false);

                        return true;
                    });
                }

                if (p_Player->HasItemCount(118470))
                {
                    p_Player->AddCriticalOperation([l_PlayerGuid]() -> bool
                    {
                        if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                            l_Player->DestroyItemCount(118470, 1, true, false);

                        return true;
                    });
                }

                break;
            }
            case 118470: ///< Garn-Tooth Necklace
            {
                uint64 l_PlayerGuid = p_Player->GetGUID();

                if (p_Player->HasItemCount(118470, 2))
                {
                    p_Player->AddCriticalOperation([l_PlayerGuid]() -> bool
                    {
                        if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                            l_Player->DestroyItemCount(118470, 1, true, false);

                        return true;
                    });
                }

                if (p_Player->HasItemCount(118469))
                {
                    p_Player->AddCriticalOperation([l_PlayerGuid]() -> bool
                    {
                        if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                            l_Player->DestroyItemCount(118469, 1, true, false);

                        return true;
                    });
                }
                break;
            }
            /////////////////
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

            if (l_Player->GetGarrison() && l_Player->HasQuest(Quests::Horde_BuildYourBarracks))
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

        if (l_GarrisonMgr->HasBuildingType(Building::Type::MageTower))
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


        if (l_GarrisonMgr->HasBuildingType(Building::Type::MageTower))
        {
            /// World Map Phases
            switch (p_Player->GetZoneId(true))
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

    void playerScript_Garrison_Portals_Phases::OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 /*p_OldZoneID*/, uint32 /*p_NewAreaId*/)
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

        /// Stables bonuses handling
        if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
        {
            if (p_Player->GetMap() && p_Player->GetMap()->GetParent())
            {
                if (p_Player->GetMap()->GetParent()->GetId() == 1116)
                    l_GarrisonMgr->HandleStablesAuraBonuses();
                else
                    l_GarrisonMgr->HandleStablesAuraBonuses(true);
            }
        }
    }

    bool playerScript_Garrison_Portals_Phases::NeedsTradingPostReset(std::vector<uint32> p_Entries, uint64 p_WorldState)
    {
        return !p_WorldState || std::find(p_Entries.begin(), p_Entries.end(), p_WorldState) != p_Entries.end() || p_WorldState <= 196;
    }

    void playerScript_Garrison_Portals_Phases::OnLogin(Player* p_Player)
    {
        /// Little Fix for Trading Post :

        if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
        {
            if (l_GarrisonMgr->GetBuildingWithType(Building::Type::TradingPost).BuildingID)
            {
                if (!p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment))
                {
                    std::vector<uint32> l_TradingPostShipments = { 138, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 196 };
                    p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment, l_TradingPostShipments[urand(0, l_TradingPostShipments.size() - 1)]);
                }

                std::vector<uint32> l_AllianceTradersEntries = { 87203, 87202, 87200, 87201, 87204 };
                std::vector<uint32> l_HordeTradersEntries    = { 86778, 86777, 86779, 86776, 86683 };
                uint64 l_WorldState = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonTradingPostDailyRandomTrader);

                switch (p_Player->GetTeamId())
                {
                    case TEAM_ALLIANCE:
                    {
                        if (NeedsTradingPostReset(l_HordeTradersEntries, l_WorldState))
                            p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomTrader, l_AllianceTradersEntries[urand(0, l_AllianceTradersEntries.size() - 1)]);
                        break;
                    }
                    case TEAM_HORDE:
                    {
                        if (NeedsTradingPostReset(l_AllianceTradersEntries, l_WorldState))
                            p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomTrader, l_HordeTradersEntries[urand(0, l_HordeTradersEntries.size() - 1)]);
                        break;
                    }
                    default:
                        break;
                }

                /// Fix old wrong reputations handling
                uint32 l_FactionID = p_Player->GetTeamId() == TEAM_ALLIANCE ? 1710 : 1708;
                FactionEntry const* l_Entry = sFactionStore.LookupEntry(l_FactionID);

                if (l_Entry != nullptr && !p_Player->GetReputation(l_FactionID))
                    p_Player->GetReputationMgr().SetReputation(l_Entry, 1);
            }
        }

        /// Little fix for The Tannery
        if (p_Player->HasAura(172424))
            p_Player->RemoveAura(172424);

        /// Fix for taxi path to ashran
        uint32 l_QuestID = p_Player->GetTeamId() == TEAM_ALLIANCE ? Quests::Alliance_LostInTransition : Quests::Horde_LostInTransition;

        if (p_Player->GetQuestStatus(l_QuestID) != QUEST_STATUS_NONE)
        {
            p_Player->m_taxi.RemoveTaximaskNode(p_Player->GetTeamId() == TEAM_ALLIANCE ? 1547 : 1549);
            p_Player->m_taxi.SetTaximaskNode(p_Player->GetTeamId()    == TEAM_ALLIANCE ? 1420 : 1408);
        }

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
        switch (p_Player->GetZoneId(true))
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

    void playerScript_Garrison_Portals_Phases::OnQuestCleared(Player* p_Player, Quest const* /*p_Quest*/)
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
        switch (p_Player->GetZoneId(true))
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

    void playerScript_Garrison_Portals_Phases::OnQuestReward(Player* p_Player, const Quest* /*p_Quest*/)
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
        switch (p_Player->GetZoneId(true))
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

    void playerScript_Garrison_Quests_Phases::OnQuestAccept(Player* p_Player, const Quest* p_Quest)
    {
        if (!p_Player->IsInGarrison())
            return;

        uint32 l_PhaseMask = p_Player->GetPhaseMask();

        switch (p_Quest->GetQuestId())
        {
            case Quests::Alliance_LostInTransition:
            case Quests::Horde_LostInTransition:
                l_PhaseMask |= GarrisonPhases::PhaseLostInTransitionQuest;
                break;
            case Quests::Alliance_AshranAppearance:
                p_Player->m_taxi.SetTaximaskNode(1420);
                break;
            case Quests::Horde_AshranAppearance:
                p_Player->m_taxi.SetTaximaskNode(1408);
                break;
            default:
                break;
        }

        p_Player->SetPhaseMask(l_PhaseMask, true);
    }

    void playerScript_Garrison_Quests_Phases::OnQuestReward(Player* p_Player, const Quest* p_Quest)
    {
        if (!p_Player->IsInGarrison())
            return;

        uint32 l_PhaseMask = p_Player->GetPhaseMask();

        switch (p_Quest->GetQuestId())
        {
            case Quests::Alliance_LostInTransition:
            case Quests::Horde_LostInTransition:
                l_PhaseMask &= ~GarrisonPhases::PhaseLostInTransitionQuest;
                break;
            default:
                break;
        }

        p_Player->SetPhaseMask(l_PhaseMask, true);
    }

    void playerScript_Garrison_Quests_Phases::OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
    {
        if (!p_Player->IsInGarrison())
            return;

        uint32 l_PhaseMask = p_Player->GetPhaseMask();

        switch (p_Quest->GetQuestId())
        {
            case Quests::Alliance_LostInTransition:
            case Quests::Horde_LostInTransition:
                l_PhaseMask &= ~GarrisonPhases::PhaseLostInTransitionQuest;
                break;
            default:
                break;
        }

        p_Player->SetPhaseMask(l_PhaseMask, true);
    }

    void playerScript_Garrison_Quests_Phases::OnItemDestroyed(Player* p_Player, Item* p_Item)
    {
        /// Check for Garrison Goblin Workshop Items
        switch (p_Item->GetEntry())
        {
            /// Level 1
            case 114983:
            case 119158:
            case 114974:
            case 114246:
            {
                if (Manager* l_GarrisonMgr = p_Player->GetGarrison())
                {
                    p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonWorkshopGearworksInventionCharges, p_Item->GetSpellCharges());
                    l_GarrisonMgr->UpdatePlot(p_Player->GetPlotInstanceID());
                }
                break;
            }
            default:
                break;
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS

#ifndef __clang_analyzer__
void AddSC_Garrison_Quest()
{
    new MS::Garrison::GarrisonBuildingAuraPlayerScript;
    new MS::Garrison::GarrisonQuestPlayerScript;
    new MS::Garrison::playerScript_Garrison_TradingPost;
    new MS::Garrison::playerScript_Garrison_Portals_Phases;
    new MS::Garrison::playerScript_Garrison_Quests_Phases;
    new MS::Garrison::spell_learning_blueprint;
}
#endif
#endif
