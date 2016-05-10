////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: reload_commandscript
%Complete: 100
Comment: All reload related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "TicketMgr.h"
#include "MapManager.h"
#include "DisableMgr.h"
#include "LFGMgr.h"
#include "AuctionHouseMgr.h"
#include "CreatureTextMgr.h"
#include "SmartAI.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "Chat.h"
#include "WaypointManager.h"
#include "WardenCheckMgr.h"
#include "ScriptSystem.h"
#include "GuildMgr.h"

class reload_commandscript: public CommandScript
{
public:
    reload_commandscript() : CommandScript("reload_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand reloadAllCommandTable[] =
        {
            { "achievement", SEC_ADMINISTRATOR, true,  &HandleReloadAllAchievementCommand, "", NULL },
            { "area",       SEC_ADMINISTRATOR,  true,  &HandleReloadAllAreaCommand,       "", NULL },
            { "gossips",    SEC_ADMINISTRATOR,  true,  &HandleReloadAllGossipsCommand,    "", NULL },
            { "item",       SEC_ADMINISTRATOR,  true,  &HandleReloadAllItemCommand,       "", NULL },
            { "locales",    SEC_ADMINISTRATOR,  true,  &HandleReloadAllLocalesCommand,    "", NULL },
            { "loot",       SEC_ADMINISTRATOR,  true,  &HandleReloadAllLootCommand,       "", NULL },
            { "npc",        SEC_ADMINISTRATOR,  true,  &HandleReloadAllNpcCommand,        "", NULL },
            { "quest",      SEC_ADMINISTRATOR,  true,  &HandleReloadAllQuestCommand,      "", NULL },
            { "scripts",    SEC_ADMINISTRATOR,  true,  &HandleReloadAllScriptsCommand,    "", NULL },
            { "spell",      SEC_ADMINISTRATOR,  true,  &HandleReloadAllSpellCommand,      "", NULL },
            { "",           SEC_ADMINISTRATOR,  true,  &HandleReloadAllCommand,           "", NULL },
            { NULL,         0,                  false, NULL,                              "", NULL }
        };
        static ChatCommand reloadCommandTable[] =
        {
            { "auctions",                     SEC_ADMINISTRATOR, true,  &HandleReloadAuctionsCommand,                   "", NULL },
            { "access_requirement",           SEC_ADMINISTRATOR, true,  &HandleReloadAccessRequirementCommand,          "", NULL },
            { "lfr_access_requirement",       SEC_ADMINISTRATOR, true,  &HandleReloadLFRAccessRequirementCommand,       "", NULL },
            { "lfg_entrances",                SEC_ADMINISTRATOR, true,  &HandleReloadLFGEntrancesCommand,               "", NULL },
            { "achievement_criteria_data",    SEC_ADMINISTRATOR, true,  &HandleReloadAchievementCriteriaDataCommand,    "", NULL },
            { "achievement_reward",           SEC_ADMINISTRATOR, true,  &HandleReloadAchievementRewardCommand,          "", NULL },
            { "all",                          SEC_ADMINISTRATOR, true,  NULL,                                           "", reloadAllCommandTable },
            { "area_skip_update",             SEC_ADMINISTRATOR, true,  &HandleReloadAreaSkipUpdateCommand,             "", NULL },
            { "areatrigger_involvedrelation", SEC_ADMINISTRATOR, true,  &HandleReloadQuestAreaTriggersCommand,          "", NULL },
            { "areatrigger_tavern",           SEC_ADMINISTRATOR, true,  &HandleReloadAreaTriggerTavernCommand,          "", NULL },
            { "areatrigger_teleport",         SEC_ADMINISTRATOR, true,  &HandleReloadAreaTriggerTeleportCommand,        "", NULL },
            { "autobroadcast",                SEC_ADMINISTRATOR, true,  &HandleReloadAutobroadcastCommand,              "", NULL },
            { "command",                      SEC_ADMINISTRATOR, true,  &HandleReloadCommandCommand,                    "", NULL },
            { "conditions",                   SEC_ADMINISTRATOR, true,  &HandleReloadConditions,                        "", NULL },
            { "config",                       SEC_ADMINISTRATOR, true,  &HandleReloadConfigCommand,                     "", NULL },
            { "motd",                         SEC_ADMINISTRATOR, true,  &HandleReloadMotdCommand,                       "", NULL },
            { "creature_area",                SEC_ADMINISTRATOR, true,  &HandleReloadCreatureArea,                      "", NULL },
            { "creature_text",                SEC_ADMINISTRATOR, true,  &HandleReloadCreatureText,                      "", NULL },
            { "creature_questender",          SEC_ADMINISTRATOR, true,  &HandleReloadCreatureQuestEnderCommand,         "", NULL },
            { "creature_linked_respawn",      SEC_GAMEMASTER,    true,  &HandleReloadLinkedRespawnCommand,              "", NULL },
            { "creature_loot_template",       SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesCreatureCommand,      "", NULL },
            { "creature_onkill_reputation",   SEC_ADMINISTRATOR, true,  &HandleReloadOnKillReputationCommand,           "", NULL },
            { "creature_queststarter",        SEC_ADMINISTRATOR, true,  &HandleReloadLoadCreatureQuestStartersCommand,  "", NULL },
            { "creature_template",            SEC_ADMINISTRATOR, true,  &HandleReloadCreatureTemplateCommand,           "", NULL },
            //{ "db_script_string",             SEC_ADMINISTRATOR, true,  &HandleReloadDbScriptStringCommand,            "", NULL },
            { "disables",                     SEC_ADMINISTRATOR, true,  &HandleReloadDisablesCommand,                   "", NULL },
            { "disenchant_loot_template",     SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesDisenchantCommand,    "", NULL },
            { "event_scripts",                SEC_ADMINISTRATOR, true,  &HandleReloadEventScriptsCommand,               "", NULL },
            { "fishing_loot_template",        SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesFishingCommand,       "", NULL },
            { "game_graveyard_zone",          SEC_ADMINISTRATOR, true,  &HandleReloadGameGraveyardZoneCommand,          "", NULL },
            { "game_tele",                    SEC_ADMINISTRATOR, true,  &HandleReloadGameTeleCommand,                   "", NULL },
            { "gameobject_questender",        SEC_ADMINISTRATOR, true,  &HandleReloadGOQuestInvRelationsCommand,        "", NULL },
            { "gameobject_loot_template",     SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesGameobjectCommand,    "", NULL },
            { "gameobject_queststarter",      SEC_ADMINISTRATOR, true,  &HandleReloadGOQuestRelationsCommand,           "", NULL },
            { "gameobject_scripts",           SEC_ADMINISTRATOR, true,  &HandleReloadGameObjectScriptsCommand,          "", NULL },
            { "gm_tickets",                   SEC_ADMINISTRATOR, true,  &HandleReloadGMTicketsCommand,                  "", NULL },
            { "gossip_menu",                  SEC_ADMINISTRATOR, true,  &HandleReloadGossipMenuCommand,                 "", NULL },
            { "gossip_menu_option",           SEC_ADMINISTRATOR, true,  &HandleReloadGossipMenuOptionCommand,           "", NULL },
            { "guild_rewards",                SEC_ADMINISTRATOR, true,  &HandleReloadGuildRewardsCommand,               "", NULL },
            { "item_enchantment_template",    SEC_ADMINISTRATOR, true,  &HandleReloadItemEnchantementsCommand,          "", NULL },
            { "item_loot_template",           SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesItemCommand,          "", NULL },
            { "lfg_dungeon_rewards",          SEC_ADMINISTRATOR, true,  &HandleReloadLfgRewardsCommand,                 "", NULL },
            { "locales_achievement_reward",   SEC_ADMINISTRATOR, true,  &HandleReloadLocalesAchievementRewardCommand,   "", NULL },
            { "locales_creature",             SEC_ADMINISTRATOR, true,  &HandleReloadLocalesCreatureCommand,            "", NULL },
            { "locales_creature_text",        SEC_ADMINISTRATOR, true,  &HandleReloadLocalesCreatureTextCommand,        "", NULL },
            { "locales_gameobject",           SEC_ADMINISTRATOR, true,  &HandleReloadLocalesGameobjectCommand,          "", NULL },
            { "locales_gossip_menu_option",   SEC_ADMINISTRATOR, true,  &HandleReloadLocalesGossipMenuOptionCommand,    "", NULL },
            { "locales_npc_text",             SEC_ADMINISTRATOR, true,  &HandleReloadLocalesNpcTextCommand,             "", NULL },
            { "locales_page_text",            SEC_ADMINISTRATOR, true,  &HandleReloadLocalesPageTextCommand,            "", NULL },
            { "locales_points_of_interest",   SEC_ADMINISTRATOR, true,  &HandleReloadLocalesPointsOfInterestCommand,    "", NULL },
            { "locales_quest",                SEC_ADMINISTRATOR, true,  &HandleReloadLocalesQuestCommand,               "", NULL },
            { "mail_level_reward",            SEC_ADMINISTRATOR, true,  &HandleReloadMailLevelRewardCommand,            "", NULL },
            { "mail_loot_template",           SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesMailCommand,          "", NULL },
            { "milling_loot_template",        SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesMillingCommand,       "", NULL },
            { "npc_spellclick_spells",        SEC_ADMINISTRATOR, true,  &HandleReloadSpellClickSpellsCommand,           "", NULL },
            { "npc_trainer",                  SEC_ADMINISTRATOR, true,  &HandleReloadNpcTrainerCommand,                 "", NULL },
            { "npc_vendor",                   SEC_ADMINISTRATOR, true,  &HandleReloadNpcVendorCommand,                  "", NULL },
            { "page_text",                    SEC_ADMINISTRATOR, true,  &HandleReloadPageTextsCommand,                  "", NULL },
            { "phasedefinitions",             SEC_ADMINISTRATOR, true,  &HandleReloadPhaseDefinitionsCommand,           "", NULL },
            { "pickpocketing_loot_template",  SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesPickpocketingCommand, "", NULL },
            { "points_of_interest",           SEC_ADMINISTRATOR, true,  &HandleReloadPointsOfInterestCommand,           "", NULL },
            { "prospecting_loot_template",    SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesProspectingCommand,   "", NULL },
            { "quest_end_scripts",            SEC_ADMINISTRATOR, true,  &HandleReloadQuestEndScriptsCommand,            "", NULL },
            { "quest_poi",                    SEC_ADMINISTRATOR, true,  &HandleReloadQuestPOICommand,                   "", NULL },
            { "quest_start_scripts",          SEC_ADMINISTRATOR, true,  &HandleReloadQuestStartScriptsCommand,          "", NULL },
            { "quest_template",               SEC_ADMINISTRATOR, true,  &HandleReloadQuestTemplateCommand,              "", NULL },
            { "reference_loot_template",      SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesReferenceCommand,     "", NULL },
            { "reserved_name",                SEC_ADMINISTRATOR, true,  &HandleReloadReservedNameCommand,               "", NULL },
            { "reputation_reward_rate",       SEC_ADMINISTRATOR, true,  &HandleReloadReputationRewardRateCommand,       "", NULL },
            { "reputation_spillover_template", SEC_ADMINISTRATOR, true,  &HandleReloadReputationRewardRateCommand,       "", NULL },
            { "script_waypoint",              SEC_ADMINISTRATOR, true,  &HandleReloadScriptWaypointCommand,             "", NULL },
            { "skill_discovery_template",     SEC_ADMINISTRATOR, true,  &HandleReloadSkillDiscoveryTemplateCommand,     "", NULL },
            { "skill_extra_item_template",    SEC_ADMINISTRATOR, true,  &HandleReloadSkillExtraItemTemplateCommand,     "", NULL },
            { "skill_fishing_base_level",     SEC_ADMINISTRATOR, true,  &HandleReloadSkillFishingBaseLevelCommand,      "", NULL },
            { "skinning_loot_template",       SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesSkinningCommand,      "", NULL },
            { "smart_scripts",                SEC_ADMINISTRATOR, true,  &HandleReloadSmartScripts,                      "", NULL },
            { "spell_required",               SEC_ADMINISTRATOR, true,  &HandleReloadSpellRequiredCommand,              "", NULL },
            { "spell_area",                   SEC_ADMINISTRATOR, true,  &HandleReloadSpellAreaCommand,                  "", NULL },
            { "spell_bonus_data",             SEC_ADMINISTRATOR, true,  &HandleReloadSpellBonusesCommand,               "", NULL },
            { "spell_group",                  SEC_ADMINISTRATOR, true,  &HandleReloadSpellGroupsCommand,                "", NULL },
            { "spell_learn_spell",            SEC_ADMINISTRATOR, true,  &HandleReloadSpellLearnSpellCommand,            "", NULL },
            { "spell_loot_template",          SEC_ADMINISTRATOR, true,  &HandleReloadLootTemplatesSpellCommand,         "", NULL },
            { "spell_linked_spell",           SEC_ADMINISTRATOR, true,  &HandleReloadSpellLinkedSpellCommand,           "", NULL },
            { "spell_pet_auras",              SEC_ADMINISTRATOR, true,  &HandleReloadSpellPetAurasCommand,              "", NULL },
            { "spell_proc_event",             SEC_ADMINISTRATOR, true,  &HandleReloadSpellProcEventCommand,             "", NULL },
            { "spell_proc",                   SEC_ADMINISTRATOR, true,  &HandleReloadSpellProcsCommand,                 "", NULL },
            { "spell_scripts",                SEC_ADMINISTRATOR, true,  &HandleReloadSpellScriptsCommand,               "", NULL },
            { "spell_target_position",        SEC_ADMINISTRATOR, true,  &HandleReloadSpellTargetPositionCommand,        "", NULL },
            { "spell_threats",                SEC_ADMINISTRATOR, true,  &HandleReloadSpellThreatsCommand,               "", NULL },
            { "spell_group_stack_rules",      SEC_ADMINISTRATOR, true,  &HandleReloadSpellGroupStackRulesCommand,       "", NULL },
            { "trinity_string",               SEC_ADMINISTRATOR, true,  &HandleReloadTrinityStringCommand,              "", NULL },
            { "warden_action",                SEC_ADMINISTRATOR, true,  &HandleReloadWardenactionCommand,               "", NULL },
            { "waypoint_scripts",             SEC_ADMINISTRATOR, true,  &HandleReloadWpScriptsCommand,                  "", NULL },
            { "waypoint_data",                SEC_ADMINISTRATOR, true,  &HandleReloadWpCommand,                         "", NULL },
            { "vehicle_accessory",            SEC_ADMINISTRATOR, true,  &HandleReloadVehicleAccessoryCommand,           "", NULL },
            { "vehicle_template_accessory",   SEC_ADMINISTRATOR, true,  &HandleReloadVehicleTemplateAccessoryCommand,   "", NULL },
            { "areatrigger_template",         SEC_ADMINISTRATOR, true,  &HandleReloadAreatriggerTemplateCommand,        "", NULL },
            { "areatrigger_move",             SEC_ADMINISTRATOR, true,  &HandleReloadAreatriggerMoveCommand,            "", NULL },
            { "disabled_ranking",             SEC_ADMINISTRATOR, true,  &HandleReloadDisabledRankingCommand,            "", NULL },
            { NULL,                           0,                 false, NULL,                                           "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "reload",         SEC_ADMINISTRATOR,  true,  NULL,                 "", reloadCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleReloadDisabledRankingCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        sObjectMgr->LoadDisabledEncounters();
        p_Handler->SendGlobalGMSysMessage("DB table `instance_disabled_rankings` reloaded.");
        return true;
    }

    static bool HandleReloadAreatriggerTemplateCommand(ChatHandler* /*p_Handler*/, const char* /*p_Args*/)
    {
        sObjectMgr->LoadAreaTriggerTemplates();
        return true;
    }

    static bool HandleReloadAreatriggerMoveCommand(ChatHandler* /*p_Handler*/, const char* /*p_Args*/)
    {
        sObjectMgr->LoadAreaTriggerMoveTemplates();
        sObjectMgr->LoadAreaTriggerMoveSplines();
        return true;
    }

    //reload commands
    static bool HandleReloadGMTicketsCommand(ChatHandler* /*handler*/, const char* /*args*/)
    {
        sTicketMgr->LoadTickets();
        return true;
    }

    static bool HandleReloadAllCommand(ChatHandler* handler, const char* /*args*/)
    {
        HandleReloadSkillFishingBaseLevelCommand(handler, "");

        HandleReloadAllAchievementCommand(handler, "");
        HandleReloadAllAreaCommand(handler, "");
        HandleReloadAllLootCommand(handler, "");
        HandleReloadAllNpcCommand(handler, "");
        HandleReloadAllQuestCommand(handler, "");
        HandleReloadAllSpellCommand(handler, "");
        HandleReloadAllItemCommand(handler, "");
        HandleReloadAllGossipsCommand(handler, "");
        HandleReloadAllLocalesCommand(handler, "");

        HandleReloadAccessRequirementCommand(handler, "");
        HandleReloadMailLevelRewardCommand(handler, "");
        HandleReloadCommandCommand(handler, "");
        HandleReloadReservedNameCommand(handler, "");
        HandleReloadTrinityStringCommand(handler, "");
        HandleReloadGameTeleCommand(handler, "");

        HandleReloadVehicleAccessoryCommand(handler, "");
        HandleReloadVehicleTemplateAccessoryCommand(handler, "");

        HandleReloadAutobroadcastCommand(handler, "");
        return true;
    }

    static bool HandleReloadAllAchievementCommand(ChatHandler* handler, const char* /*args*/)
    {
        HandleReloadAchievementCriteriaDataCommand(handler, "");
        HandleReloadAchievementRewardCommand(handler, "");
        return true;
    }

    static bool HandleReloadAllAreaCommand(ChatHandler* handler, const char* /*args*/)
    {
        //HandleReloadQuestAreaTriggersCommand(handler, ""); -- reloaded in HandleReloadAllQuestCommand
        HandleReloadAreaTriggerTeleportCommand(handler, "");
        HandleReloadAreaTriggerTavernCommand(handler, "");
        HandleReloadGameGraveyardZoneCommand(handler, "");
        return true;
    }

    static bool HandleReloadAllLootCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables...");
        LoadLootTables();
        handler->SendGlobalGMSysMessage("DB tables `*_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadAllNpcCommand(ChatHandler* handler, const char* args)
    {
        if (*args != 'a')                                          // will be reloaded from all_gossips
        HandleReloadNpcTrainerCommand(handler, "a");
        HandleReloadNpcVendorCommand(handler, "a");
        HandleReloadPointsOfInterestCommand(handler, "a");
        HandleReloadSpellClickSpellsCommand(handler, "a");
        return true;
    }

    static bool HandleReloadAllQuestCommand(ChatHandler* handler, const char* /*args*/)
    {
        HandleReloadQuestAreaTriggersCommand(handler, "a");
        HandleReloadQuestPOICommand(handler, "a");
        HandleReloadQuestTemplateCommand(handler, "a");

        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Quests Relations...");
        sObjectMgr->LoadQuestRelations();
        handler->SendGlobalGMSysMessage("DB tables `*_queststarter` and `*_questender` reloaded.");
        return true;
    }

    static bool HandleReloadAllScriptsCommand(ChatHandler* handler, const char* /*args*/)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->PSendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts...");
        HandleReloadGameObjectScriptsCommand(handler, "a");
        HandleReloadEventScriptsCommand(handler, "a");
        HandleReloadQuestEndScriptsCommand(handler, "a");
        HandleReloadQuestStartScriptsCommand(handler, "a");
        HandleReloadSpellScriptsCommand(handler, "a");
        handler->SendGlobalGMSysMessage("DB tables `*_scripts` reloaded.");
        HandleReloadDbScriptStringCommand(handler, "a");
        HandleReloadWpScriptsCommand(handler, "a");
        HandleReloadWpCommand(handler, "a");
        return true;
    }

    static bool HandleReloadAllSpellCommand(ChatHandler* handler, const char* /*args*/)
    {
        HandleReloadSkillDiscoveryTemplateCommand(handler, "a");
        HandleReloadSkillExtraItemTemplateCommand(handler, "a");
        HandleReloadSpellRequiredCommand(handler, "a");
        HandleReloadSpellAreaCommand(handler, "a");
        HandleReloadSpellGroupsCommand(handler, "a");
        HandleReloadSpellLearnSpellCommand(handler, "a");
        HandleReloadSpellLinkedSpellCommand(handler, "a");
        HandleReloadSpellProcEventCommand(handler, "a");
        HandleReloadSpellProcsCommand(handler, "a");
        HandleReloadSpellBonusesCommand(handler, "a");
        HandleReloadSpellTargetPositionCommand(handler, "a");
        HandleReloadSpellThreatsCommand(handler, "a");
        HandleReloadSpellGroupStackRulesCommand(handler, "a");
        HandleReloadSpellPetAurasCommand(handler, "a");
        return true;
    }

    static bool HandleReloadAllGossipsCommand(ChatHandler* handler, const char* args)
    {
        HandleReloadGossipMenuCommand(handler, "a");
        HandleReloadGossipMenuOptionCommand(handler, "a");
        if (*args != 'a')                                          // already reload from all_scripts
        HandleReloadPointsOfInterestCommand(handler, "a");
        return true;
    }

    static bool HandleReloadAllItemCommand(ChatHandler* handler, const char* /*args*/)
    {
        HandleReloadPageTextsCommand(handler, "a");
        HandleReloadItemEnchantementsCommand(handler, "a");
        return true;
    }

    static bool HandleReloadAllLocalesCommand(ChatHandler* handler, const char* /*args*/)
    {
        HandleReloadLocalesAchievementRewardCommand(handler, "a");
        HandleReloadLocalesCreatureCommand(handler, "a");
        HandleReloadLocalesCreatureTextCommand(handler, "a");
        HandleReloadLocalesGameobjectCommand(handler, "a");
        HandleReloadLocalesGossipMenuOptionCommand(handler, "a");
        HandleReloadLocalesNpcTextCommand(handler, "a");
        HandleReloadLocalesPageTextCommand(handler, "a");
        HandleReloadLocalesPointsOfInterestCommand(handler, "a");
        HandleReloadLocalesQuestCommand(handler, "a");
        return true;
    }

    static bool HandleReloadConfigCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading config settings...");
        sWorld->LoadConfigSettings(true);
        sMapMgr->InitializeVisibilityDistanceInfo();
        handler->SendGlobalGMSysMessage("World config settings reloaded.");
        return true;
    }

    static bool HandleReloadMotdCommand(ChatHandler* p_Handler, const char* /*p_Args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading server motd...");
        sWorld->LoadDBMotd();
        p_Handler->SendGlobalGMSysMessage("Server motd reloaded.");
        return true;
    }

    static bool HandleReloadAccessRequirementCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Access Requirement definitions...");
        sObjectMgr->LoadAccessRequirements();
        handler->SendGlobalGMSysMessage("DB table `access_requirement` reloaded.");
        return true;
    }

    static bool HandleReloadLFRAccessRequirementCommand(ChatHandler* p_Handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading LFR Access Requirement definitions...");
        sObjectMgr->LoadLFRAccessRequirements();
        p_Handler->SendGlobalGMSysMessage("DB table `lfr_access_requirement` reloaded.");
        return true;
    }

    static bool HandleReloadLFGEntrancesCommand(ChatHandler* p_Handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading LFG Entrances definitions...");
        sLFGMgr->LoadEntrancePositions();
        p_Handler->SendGlobalGMSysMessage("DB table `lfg_entrances` reloaded.");
        return true;
    }

    static bool HandleReloadAchievementCriteriaDataCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Additional Achievement Criteria Data...");
        sAchievementMgr->LoadAchievementCriteriaData();
        handler->SendGlobalGMSysMessage("DB table `achievement_criteria_data` reloaded.");
        return true;
    }

    static bool HandleReloadAchievementRewardCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Achievement Reward Data...");
        sAchievementMgr->LoadRewards();
        handler->SendGlobalGMSysMessage("DB table `achievement_reward` reloaded.");
        return true;
    }

    static bool HandleReloadAreaTriggerTavernCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Tavern Area Triggers...");
        sObjectMgr->LoadTavernAreaTriggers();
        handler->SendGlobalGMSysMessage("DB table `areatrigger_tavern` reloaded.");
        return true;
    }

    static bool HandleReloadAreaTriggerTeleportCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading AreaTrigger teleport definitions...");
        sObjectMgr->LoadAreaTriggerTeleports();
        handler->SendGlobalGMSysMessage("DB table `areatrigger_teleport` reloaded.");
        return true;
    }

    static bool HandleReloadAutobroadcastCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Autobroadcasts...");
        sWorld->LoadAutobroadcasts();
        handler->SendGlobalGMSysMessage("DB table `autobroadcast` reloaded.");
        return true;
    }

    static bool HandleReloadCommandCommand(ChatHandler* handler, const char* /*args*/)
    {
        handler->SetLoadCommandTable(true);
        handler->SendGlobalGMSysMessage("DB table `command` will be reloaded at next chat command use.");
        return true;
    }

    static bool HandleReloadOnKillReputationCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading creature award reputation definitions...");
        sObjectMgr->LoadReputationOnKill();
        handler->SendGlobalGMSysMessage("DB table `creature_onkill_reputation` reloaded.");
        return true;
    }

    static bool HandleReloadCreatureTemplateCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Tokenizer entries(std::string(args), ' ');

        for (Tokenizer::const_iterator itr = entries.begin(); itr != entries.end(); ++itr)
        {
            uint32 entry = uint32(atoi(*itr));

            PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CREATURE_TEMPLATE);
            stmt->setUInt32(0, entry);
            PreparedQueryResult result = WorldDatabase.Query(stmt);

            if (!result)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATURETEMPLATE_NOTFOUND, entry);
                continue;
            }

            CreatureTemplate* cInfo = const_cast<CreatureTemplate*>(sObjectMgr->GetCreatureTemplate(entry));
            if (!cInfo)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATURESTORAGE_NOTFOUND, entry);
                continue;
            }

            sLog->outInfo(LOG_FILTER_GENERAL, "Reloading creature template entry %u", entry);

            Field* fields = result->Fetch();
            uint32 l_Index = 0;

            cInfo->KillCredit[0]       = fields[l_Index++].GetUInt32();
            cInfo->KillCredit[1]       = fields[l_Index++].GetUInt32();
            cInfo->Modelid1            = fields[l_Index++].GetUInt32();
            cInfo->Modelid2            = fields[l_Index++].GetUInt32();
            cInfo->Modelid3            = fields[l_Index++].GetUInt32();
            cInfo->Modelid4            = fields[l_Index++].GetUInt32();
            cInfo->Name                = fields[l_Index++].GetString();
            cInfo->SubName             = fields[l_Index++].GetString();
            cInfo->IconName            = fields[l_Index++].GetString();
            cInfo->GossipMenuId        = fields[l_Index++].GetUInt32();
            cInfo->minlevel            = fields[l_Index++].GetUInt8();
            cInfo->maxlevel            = fields[l_Index++].GetUInt8();
            cInfo->expansion           = fields[l_Index++].GetUInt16();
            cInfo->expansionUnknown    = fields[l_Index++].GetUInt16();
            cInfo->faction             = fields[l_Index++].GetUInt16();
            cInfo->NpcFlags1           = fields[l_Index++].GetUInt32();
            cInfo->NpcFlags2           = fields[l_Index++].GetUInt32();
            cInfo->speed_walk          = fields[l_Index++].GetFloat();
            cInfo->speed_run           = fields[l_Index++].GetFloat();
            cInfo->speed_fly           = fields[l_Index++].GetFloat();
            cInfo->scale               = fields[l_Index++].GetFloat();
            cInfo->rank                = fields[l_Index++].GetUInt8();
            cInfo->dmgschool           = fields[l_Index++].GetUInt8();
            cInfo->dmg_multiplier      = fields[l_Index++].GetFloat();
            cInfo->baseattacktime      = fields[l_Index++].GetUInt32();
            cInfo->rangeattacktime     = fields[l_Index++].GetUInt32();
            cInfo->baseVariance        = fields[l_Index++].GetFloat();
            cInfo->rangeVariance       = fields[l_Index++].GetFloat();
            cInfo->unit_class          = fields[l_Index++].GetUInt8();
            cInfo->UnitFlags1          = fields[l_Index++].GetUInt32();
            cInfo->UnitFlags2          = fields[l_Index++].GetUInt32();
            cInfo->UnitFlags3          = fields[l_Index++].GetUInt32();
            cInfo->dynamicflags        = fields[l_Index++].GetUInt32();
            cInfo->WorldEffectID       = fields[l_Index++].GetUInt32();
            cInfo->family              = fields[l_Index++].GetUInt8();
            cInfo->trainer_type        = fields[l_Index++].GetUInt8();
            cInfo->trainer_spell       = fields[l_Index++].GetUInt32();
            cInfo->trainer_class       = fields[l_Index++].GetUInt8();
            cInfo->trainer_race        = fields[l_Index++].GetUInt8();
            cInfo->type                = fields[l_Index++].GetUInt8();
            cInfo->type_flags          = fields[l_Index++].GetUInt32();
            cInfo->type_flags2         = fields[l_Index++].GetUInt32();
            cInfo->lootid              = fields[l_Index++].GetUInt32();
            cInfo->pickpocketLootId    = fields[l_Index++].GetUInt32();
            cInfo->SkinLootId          = fields[l_Index++].GetUInt32();

            for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
                cInfo->resistance[i] = fields[l_Index++ + i - 1].GetUInt16();

            cInfo->spells[0]           = fields[l_Index++].GetUInt32();
            cInfo->spells[1]           = fields[l_Index++].GetUInt32();
            cInfo->spells[2]           = fields[l_Index++].GetUInt32();
            cInfo->spells[3]           = fields[l_Index++].GetUInt32();
            cInfo->spells[4]           = fields[l_Index++].GetUInt32();
            cInfo->spells[5]           = fields[l_Index++].GetUInt32();
            cInfo->spells[6]           = fields[l_Index++].GetUInt32();
            cInfo->spells[7]           = fields[l_Index++].GetUInt32();
            cInfo->VehicleId           = fields[l_Index++].GetUInt32();
            cInfo->mingold             = fields[l_Index++].GetUInt32();
            cInfo->maxgold             = fields[l_Index++].GetUInt32();
            cInfo->AIName              = fields[l_Index++].GetString();
            cInfo->MovementType        = fields[l_Index++].GetUInt8();
            cInfo->InhabitType         = fields[l_Index++].GetUInt8();
            cInfo->HoverHeight         = fields[l_Index++].GetFloat();
            cInfo->ModHealth           = fields[l_Index++].GetFloat();
            cInfo->ModMana             = fields[l_Index++].GetFloat();
            cInfo->ModManaExtra        = fields[l_Index++].GetFloat();
            cInfo->ModArmor            = fields[l_Index++].GetFloat();
            cInfo->RacialLeader        = fields[l_Index++].GetBool();
            cInfo->questItems[0]       = fields[l_Index++].GetUInt32();
            cInfo->questItems[1]       = fields[l_Index++].GetUInt32();
            cInfo->questItems[2]       = fields[l_Index++].GetUInt32();
            cInfo->questItems[3]       = fields[l_Index++].GetUInt32();
            cInfo->questItems[4]       = fields[l_Index++].GetUInt32();
            cInfo->questItems[5]       = fields[l_Index++].GetUInt32();
            cInfo->movementId          = fields[l_Index++].GetUInt32();
            cInfo->RegenHealth         = fields[l_Index++].GetBool();
            cInfo->MechanicImmuneMask  = fields[l_Index++].GetUInt32();
            cInfo->flags_extra         = fields[l_Index++].GetUInt32();
            cInfo->ScriptID            = sObjectMgr->GetScriptId(fields[l_Index++].GetCString());

            sObjectMgr->CheckCreatureTemplate(cInfo);
        }

        handler->PSendSysMessage("Creature template reloaded.");
        return true;
    }

    static bool HandleReloadLoadCreatureQuestStartersCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Loading Quests Relations... (`creature_queststarter`)");
        sObjectMgr->LoadCreatureQuestStarters();
        handler->SendGlobalGMSysMessage("DB table `creature_queststarter` reloaded.");
        return true;
    }

    static bool HandleReloadLinkedRespawnCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Loading Linked Respawns... (`creature_linked_respawn`)");
        sObjectMgr->LoadLinkedRespawn();
        handler->SendGlobalGMSysMessage("DB table `creature_linked_respawn` (creature linked respawns) reloaded.");
        return true;
    }

    static bool HandleReloadCreatureQuestEnderCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Loading Quests Relations... (`creature_questender`)");
        sObjectMgr->LoadCreatureQuestEnders();
        handler->SendGlobalGMSysMessage("DB table `creature_questender` reloaded.");
        return true;
    }

    static bool HandleReloadGossipMenuCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `gossip_menu` Table!");
        sObjectMgr->LoadGossipMenu();
        handler->SendGlobalGMSysMessage("DB table `gossip_menu` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadGossipMenuOptionCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `gossip_menu_option` Table!");
        sObjectMgr->LoadGossipMenuItems();
        handler->SendGlobalGMSysMessage("DB table `gossip_menu_option` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadGuildRewardsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `guild_rewards` Table!");
        sGuildMgr->LoadGuildRewards();
        handler->SendGlobalGMSysMessage("DB table `guild_rewards` reloaded.");
        return true;
    }

    static bool HandleReloadGOQuestRelationsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Loading Quests Relations... (`gameobject_queststarter`)");
        sObjectMgr->LoadGameobjectQuestStarters();
        handler->SendGlobalGMSysMessage("DB table `gameobject_queststarter` reloaded.");
        return true;
    }

    static bool HandleReloadGOQuestInvRelationsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Loading Quests Relations... (`gameobject_questender`)");
        sObjectMgr->LoadGameobjectQuestEnders();
        handler->SendGlobalGMSysMessage("DB table `gameobject_questender` reloaded.");
        return true;
    }

    static bool HandleReloadQuestAreaTriggersCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Quest Area Triggers...");
        sObjectMgr->LoadQuestAreaTriggers();
        handler->SendGlobalGMSysMessage("DB table `areatrigger_involvedrelation` (quest area triggers) reloaded.");
        return true;
    }

    static bool HandleReloadQuestTemplateCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Quest Templates...");
        sObjectMgr->LoadQuests();
        handler->SendGlobalGMSysMessage("DB table `quest_template` (quest definitions) reloaded.");

        /// dependent also from `gameobject` but this table not reloaded anyway
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading GameObjects for quests...");
        sObjectMgr->LoadGameObjectForQuests();
        handler->SendGlobalGMSysMessage("Data GameObjects for quests reloaded.");
        
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Quest Objectives...");
        sObjectMgr->LoadQuestObjectives();
        handler->SendGlobalGMSysMessage("Quest objectives have been reloaded.");
        return true;
    }

    static bool HandleReloadLootTemplatesCreatureCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`creature_loot_template`)");
        LoadLootTemplates_Creature();
        LootTemplates_Creature.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `creature_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesDisenchantCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`disenchant_loot_template`)");
        LoadLootTemplates_Disenchant();
        LootTemplates_Disenchant.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `disenchant_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesFishingCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`fishing_loot_template`)");
        LoadLootTemplates_Fishing();
        LootTemplates_Fishing.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `fishing_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesGameobjectCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`gameobject_loot_template`)");
        LoadLootTemplates_Gameobject();
        LootTemplates_Gameobject.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `gameobject_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesItemCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`item_loot_template`)");
        LoadLootTemplates_Item();
        LootTemplates_Item.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `item_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesMillingCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`milling_loot_template`)");
        LoadLootTemplates_Milling();
        LootTemplates_Milling.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `milling_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesPickpocketingCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`pickpocketing_loot_template`)");
        LoadLootTemplates_Pickpocketing();
        LootTemplates_Pickpocketing.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `pickpocketing_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesProspectingCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`prospecting_loot_template`)");
        LoadLootTemplates_Prospecting();
        LootTemplates_Prospecting.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `prospecting_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesMailCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`mail_loot_template`)");
        LoadLootTemplates_Mail();
        LootTemplates_Mail.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `mail_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesReferenceCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`reference_loot_template`)");
        LoadLootTemplates_Reference();
        handler->SendGlobalGMSysMessage("DB table `reference_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesSkinningCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`skinning_loot_template`)");
        LoadLootTemplates_Skinning();
        LootTemplates_Skinning.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `skinning_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadLootTemplatesSpellCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Loot Tables... (`spell_loot_template`)");
        LoadLootTemplates_Spell();
        LootTemplates_Spell.CheckLootRefs();
        handler->SendGlobalGMSysMessage("DB table `spell_loot_template` reloaded.");
        sConditionMgr->LoadConditions(true);
        return true;
    }

    static bool HandleReloadTrinityStringCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading trinity_string Table!");
        sObjectMgr->LoadTrinityStrings();
        handler->SendGlobalGMSysMessage("DB table `trinity_string` reloaded.");
        return true;
    }

    static bool HandleReloadWardenactionCommand(ChatHandler* handler, const char* /*args*/)
    {
        if (!sWorld->getBoolConfig(CONFIG_WARDEN_ENABLED))
        {
            handler->SendSysMessage("Warden system disabled by config - reloading warden_action skipped.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading warden_action Table!");
        sWardenCheckMgr->LoadWardenOverrides();
        handler->SendGlobalGMSysMessage("DB table `warden_action` reloaded.");
        return true;
    }

    static bool HandleReloadNpcTrainerCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `npc_trainer` Table!");
        sObjectMgr->LoadTrainerSpell();
        handler->SendGlobalGMSysMessage("DB table `npc_trainer` reloaded.");
        return true;
    }

    static bool HandleReloadNpcVendorCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `npc_vendor` Table!");
        sObjectMgr->LoadVendors();
        handler->SendGlobalGMSysMessage("DB table `npc_vendor` reloaded.");
        return true;
    }

    static bool HandleReloadPointsOfInterestCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `points_of_interest` Table!");
        sObjectMgr->LoadPointsOfInterest();
        handler->SendGlobalGMSysMessage("DB table `points_of_interest` reloaded.");
        return true;
    }

    static bool HandleReloadQuestPOICommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Quest POI ..." );
        sObjectMgr->LoadQuestPOI();
        handler->SendGlobalGMSysMessage("DB Table `quest_poi` and `quest_poi_points` reloaded.");
        return true;
    }

    static bool HandleReloadSpellClickSpellsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `npc_spellclick_spells` Table!");
        sObjectMgr->LoadNPCSpellClickSpells();
        handler->SendGlobalGMSysMessage("DB table `npc_spellclick_spells` reloaded.");
        return true;
    }

    static bool HandleReloadReservedNameCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Loading ReservedNames... (`reserved_name`)");
        sObjectMgr->LoadReservedPlayersNames();
        handler->SendGlobalGMSysMessage("DB table `reserved_name` (player reserved names) reloaded.");
        return true;
    }

    static bool HandleReloadReputationRewardRateCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `reputation_reward_rate` Table!" );
        sObjectMgr->LoadReputationRewardRate();
        handler->SendGlobalSysMessage("DB table `reputation_reward_rate` reloaded.");
        return true;
    }

    static bool HandleReloadReputationSpilloverTemplateCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading `reputation_spillover_template` Table!" );
        sObjectMgr->LoadReputationSpilloverTemplate();
        handler->SendGlobalSysMessage("DB table `reputation_spillover_template` reloaded.");
        return true;
    }

    static bool HandleReloadSkillDiscoveryTemplateCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Skill Discovery Table...");
        LoadSkillDiscoveryTable();
        handler->SendGlobalGMSysMessage("DB table `skill_discovery_template` (recipes discovered at crafting) reloaded.");
        return true;
    }

    static bool HandleReloadSkillExtraItemTemplateCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Skill Extra Item Table...");
        LoadSkillExtraItemTable();
        handler->SendGlobalGMSysMessage("DB table `skill_extra_item_template` (extra item creation when crafting) reloaded.");
        return true;
    }

    static bool HandleReloadSkillFishingBaseLevelCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Skill Fishing base level requirements...");
        sObjectMgr->LoadFishingBaseSkillLevel();
        handler->SendGlobalGMSysMessage("DB table `skill_fishing_base_level` (fishing base level for zone/subzone) reloaded.");
        return true;
    }

    static bool HandleReloadSpellAreaCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading SpellArea Data...");
        sSpellMgr->LoadSpellAreas();
        handler->SendGlobalGMSysMessage("DB table `spell_area` (spell dependences from area/quest/auras state) reloaded.");
        return true;
    }

    static bool HandleReloadSpellRequiredCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Required Data... ");
        sSpellMgr->LoadSpellRequired();
        handler->SendGlobalGMSysMessage("DB table `spell_required` reloaded.");
        return true;
    }

    static bool HandleReloadSpellGroupsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Groups...");
        sSpellMgr->LoadSpellGroups();
        handler->SendGlobalGMSysMessage("DB table `spell_group` (spell groups) reloaded.");
        return true;
    }

    static bool HandleReloadSpellLearnSpellCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Learn Spells...");
        sSpellMgr->LoadSpellLearnSpells();
        handler->SendGlobalGMSysMessage("DB table `spell_learn_spell` reloaded.");
        return true;
    }

    static bool HandleReloadSpellLinkedSpellCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Linked Spells...");
        sSpellMgr->LoadSpellLinked();
        handler->SendGlobalGMSysMessage("DB table `spell_linked_spell` reloaded.");
        return true;
    }

    static bool HandleReloadSpellProcEventCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Proc Event conditions...");
        sSpellMgr->LoadSpellProcEvents();
        handler->SendGlobalGMSysMessage("DB table `spell_proc_event` (spell proc trigger requirements) reloaded.");
        return true;
    }

    static bool HandleReloadSpellProcsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Proc conditions and data...");
        sSpellMgr->LoadSpellProcs();
        handler->SendGlobalGMSysMessage("DB table `spell_proc` (spell proc conditions and data) reloaded.");
        return true;
    }

    static bool HandleReloadSpellBonusesCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Bonus Data...");
        sSpellMgr->LoadSpellBonusess();
        handler->SendGlobalGMSysMessage("DB table `spell_bonus_data` (spell damage/healing coefficients) reloaded.");
        return true;
    }

    static bool HandleReloadSpellTargetPositionCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell target coordinates...");
        sSpellMgr->LoadSpellTargetPositions();
        handler->SendGlobalGMSysMessage("DB table `spell_target_position` (destination coordinates for spell targets) reloaded.");
        return true;
    }

    static bool HandleReloadSpellThreatsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Aggro Spells Definitions...");
        sSpellMgr->LoadSpellThreats();
        handler->SendGlobalGMSysMessage("DB table `spell_threat` (spell aggro definitions) reloaded.");
        return true;
    }

    static bool HandleReloadSpellGroupStackRulesCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell Group Stack Rules...");
        sSpellMgr->LoadSpellGroupStackRules();
        handler->SendGlobalGMSysMessage("DB table `spell_group_stack_rules` (spell stacking definitions) reloaded.");
        return true;
    }

    static bool HandleReloadSpellPetAurasCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Spell pet auras...");
        sSpellMgr->LoadSpellPetAuras();
        handler->SendGlobalGMSysMessage("DB table `spell_pet_auras` reloaded.");
        return true;
    }

    static bool HandleReloadPageTextsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Page Texts...");
        sObjectMgr->LoadPageTexts();
        handler->SendGlobalGMSysMessage("DB table `page_texts` reloaded.");
        return true;
    }

    static bool HandleReloadItemEnchantementsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Item Random Enchantments Table...");
        LoadRandomEnchantmentsTable();
        handler->SendGlobalGMSysMessage("DB table `item_enchantment_template` reloaded.");
        return true;
    }

    static bool HandleReloadGameObjectScriptsCommand(ChatHandler* handler, const char* args)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts from `gameobject_scripts`...");

        sObjectMgr->LoadGameObjectScripts();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB table `gameobject_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadEventScriptsCommand(ChatHandler* handler, const char* args)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts from `event_scripts`...");

        sObjectMgr->LoadEventScripts();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB table `event_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadWpScriptsCommand(ChatHandler* handler, const char* args)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts from `waypoint_scripts`...");

        sObjectMgr->LoadWaypointScripts();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB table `waypoint_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadWpCommand(ChatHandler* handler, const char* args)
    {
        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Waypoints data from 'waypoints_data'");

        sWaypointMgr->Load();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB Table 'waypoint_data' reloaded.");

        return true;
    }

    static bool HandleReloadQuestEndScriptsCommand(ChatHandler* handler, const char* args)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts from `quest_end_scripts`...");

        sObjectMgr->LoadQuestEndScripts();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB table `quest_end_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadQuestStartScriptsCommand(ChatHandler* handler, const char* args)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts from `quest_start_scripts`...");

        sObjectMgr->LoadQuestStartScripts();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB table `quest_start_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadSpellScriptsCommand(ChatHandler* handler, const char* args)
    {
        if (sScriptMgr->IsScriptScheduled())
        {
            handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (*args != 'a')
            sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Scripts from `spell_scripts`...");

        sObjectMgr->LoadSpellScripts();

        if (*args != 'a')
            handler->SendGlobalGMSysMessage("DB table `spell_scripts` reloaded.");

        return true;
    }

    static bool HandleReloadDbScriptStringCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Script strings from `db_script_string`...");
        sObjectMgr->LoadDbScriptStrings();
        handler->SendGlobalGMSysMessage("DB table `db_script_string` reloaded.");
        return true;
    }

    static bool HandleReloadGameGraveyardZoneCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Graveyard-zone links...");

        sObjectMgr->LoadGraveyardZones();

        handler->SendGlobalGMSysMessage("DB table `game_graveyard_zone` reloaded.");

        return true;
    }

    static bool HandleReloadGameTeleCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Game Tele coordinates...");

        sObjectMgr->LoadGameTele();

        handler->SendGlobalGMSysMessage("DB table `game_tele` reloaded.");

        return true;
    }

    static bool HandleReloadDisablesCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading disables table...");
        DisableMgr::LoadDisables();
        sLog->outInfo(LOG_FILTER_GENERAL, "Checking quest disables...");
        DisableMgr::CheckQuestDisables();
        handler->SendGlobalGMSysMessage("DB table `disables` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesAchievementRewardCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Achievement Reward Data...");
        sAchievementMgr->LoadRewardLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_achievement_reward` reloaded.");
        return true;
    }

    static bool HandleReloadLfgRewardsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading lfg dungeon rewards...");
        sLFGMgr->LoadRewards();
        handler->SendGlobalGMSysMessage("DB table `lfg_dungeon_rewards` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesCreatureCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Creature ...");
        sObjectMgr->LoadCreatureLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_creature` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesCreatureTextCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Creature Texts...");
        sCreatureTextMgr->LoadCreatureTextLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_creature_text` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesGameobjectCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Gameobject ... ");
        sObjectMgr->LoadGameObjectLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_gameobject` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesGossipMenuOptionCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Gossip Menu Option ... ");
        sObjectMgr->LoadGossipMenuItemsLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_gossip_menu_option` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesNpcTextCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales NPC Text ... ");
        sObjectMgr->LoadNpcTextLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_npc_text` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesPageTextCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Page Text ... ");
        sObjectMgr->LoadPageTextLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_page_text` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesPointsOfInterestCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Points Of Interest ... ");
        sObjectMgr->LoadPointOfInterestLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_points_of_interest` reloaded.");
        return true;
    }

    static bool HandleReloadLocalesQuestCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Locales Quest ... ");
        sObjectMgr->LoadQuestLocales();
        handler->SendGlobalGMSysMessage("DB table `locales_quest` reloaded.");
        return true;
    }

    static bool HandleReloadMailLevelRewardCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Player level dependent mail rewards...");
        sObjectMgr->LoadMailLevelRewards();
        handler->SendGlobalGMSysMessage("DB table `mail_level_reward` reloaded.");
        return true;
    }

    static bool HandleReloadAuctionsCommand(ChatHandler* handler, const char* /*args*/)
    {
        ///- Reload dynamic data tables from the database
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Auctions...");
        sAuctionMgr->LoadAuctionItems();
        sAuctionMgr->LoadAuctions();
        handler->SendGlobalGMSysMessage("Auctions reloaded.");
        return true;
    }

    static bool HandleReloadConditions(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Conditions...");
        sConditionMgr->LoadConditions(true);
        handler->SendGlobalGMSysMessage("Conditions reloaded.");
        return true;
    }

    static bool HandleReloadCreatureText(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Creature Texts...");
        sCreatureTextMgr->LoadCreatureTexts();
        handler->SendGlobalGMSysMessage("Creature Texts reloaded.");
        return true;
    }

    static bool HandleReloadSmartScripts(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Re-Loading Smart Scripts...");
        sSmartScriptMgr->LoadSmartAIFromDB();
        handler->SendGlobalGMSysMessage("Smart Scripts reloaded.");
        return true;
    }

    static bool HandleReloadVehicleAccessoryCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Reloading vehicle_accessory table...");
        sObjectMgr->LoadVehicleAccessories();
        handler->SendGlobalGMSysMessage("Vehicle accessories reloaded.");
        return true;
    }

    static bool HandleReloadVehicleTemplateAccessoryCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Reloading vehicle_template_accessory table...");
        sObjectMgr->LoadVehicleTemplateAccessories();
        handler->SendGlobalGMSysMessage("Vehicle template accessories reloaded.");
        return true;
    }

    static bool HandleReloadScriptWaypointCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Reloading script_waypoint table...");
        sScriptSystemMgr->LoadScriptWaypoints();
        handler->SendGlobalGMSysMessage("script_waypoint table reloaded.");
        return true;
    }

    static bool HandleReloadPhaseDefinitionsCommand(ChatHandler* handler, const char* /*args*/)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Reloading phase_definitions table...");
        sObjectMgr->LoadPhaseDefinitions();
        sWorld->UpdatePhaseDefinitions();
        handler->SendGlobalGMSysMessage("Phase Definitions reloaded.");
        return true;
    }

    static bool HandleReloadCreatureArea(ChatHandler* handler, const char* args)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "Updating Creature Area...");

        QueryResult result;

        if (!*args)
            return false;

        char* mapIdStr = strtok((char*) args, " ");
        uint32 mapId = uint32(atoi(mapIdStr));
        result = WorldDatabase.PQuery("SELECT guid, map, position_x, position_y, position_z FROM creature WHERE map = %u", mapId);

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Updated 0 creature area.");
            return true;
        }

        SQLTransaction trans = WorldDatabase.BeginTransaction();

        do
        {
            Field* fields = result->Fetch();

            uint32 guid  = fields[0].GetUInt32();
            uint32 mapId = fields[1].GetUInt32();
            float  posX  = fields[2].GetFloat();
            float  poxY  = fields[3].GetFloat();
            float  posZ  = fields[4].GetFloat();

            uint32 zoneId = 0, areaId = 0;
            sMapMgr->GetZoneAndAreaId(zoneId, areaId, mapId, posX, poxY, posZ);

            std::ostringstream outCreatureAreaStream;
            outCreatureAreaStream << "REPLACE INTO creature_area (`guid`, `zone`, `area`) VALUES (" << guid << ", " << zoneId << ", " << areaId << ");";
            trans->Append(outCreatureAreaStream.str().c_str());
        }
        while (result->NextRow());

        WorldDatabase.CommitTransaction(trans);

        handler->SendGlobalGMSysMessage("Creature Areas Updated.");
        return true;
    }

    static bool HandleReloadAreaSkipUpdateCommand(ChatHandler* handler, const char* /*args*/)
    {
        sObjectMgr->LoadSkipUpdateZone();
        handler->SendGlobalGMSysMessage("Area skip update reloaded");
        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_reload_commandscript()
{
    new reload_commandscript();
}
#endif
