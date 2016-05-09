////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Chat.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "MovementGenerator.h"
#include "ObjectAccessor.h"
#include "SpellAuras.h"
#include "TargetedMovementGenerator.h"
#include "WeatherMgr.h"
#include "ace/INET_Addr.h"
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "BattlegroundMgr.hpp"
#include "DisableMgr.h"
#include <fstream>

#include "GarrisonMgr.hpp"
#include "BattlegroundPacketFactory.hpp"
#include "BattlegroundInvitationsMgr.hpp"
#include "LFGMgr.h"

class misc_commandscript: public CommandScript
{
    public:
        misc_commandscript() : CommandScript("misc_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand groupCommandTable[] =
            {
                { "leader",             SEC_ADMINISTRATOR,  false, &HandleGroupLeaderCommand,   "", NULL },
                { "disband",            SEC_ADMINISTRATOR,  false, &HandleGroupDisbandCommand,  "", NULL },
                { "remove",             SEC_ADMINISTRATOR,  false, &HandleGroupRemoveCommand,   "", NULL },
                { NULL,                 0,                  false, NULL,                        "", NULL }
            };
            static ChatCommand petCommandTable[] =
            {
                { "create",             SEC_GAMEMASTER,     false, &HandleCreatePetCommand,         "", NULL },
                { "learn",              SEC_GAMEMASTER,     false, &HandlePetLearnCommand,          "", NULL },
                { "unlearn",            SEC_GAMEMASTER,     false, &HandlePetUnlearnCommand,        "", NULL },
                { NULL,                 0,                  false, NULL,                            "", NULL }
            };
            static ChatCommand sendCommandTable[] =
            {
                { "items",              SEC_ADMINISTRATOR,  true, &HandleSendItemsCommand,          "", NULL },
                { "mail",               SEC_MODERATOR,      true, &HandleSendMailCommand,           "", NULL },
                { "message",            SEC_ADMINISTRATOR,  true, &HandleSendMessageCommand,        "", NULL },
                { "money",              SEC_ADMINISTRATOR,  true, &HandleSendMoneyCommand,          "", NULL },
                { NULL,                 0,                  false, NULL,                            "", NULL }
            };
            static ChatCommand auraCommandTable[] =
            {
                { "duration",           SEC_ADMINISTRATOR,  true,   &HandleAuraDurationCommand,     "", NULL },
                { "charges",            SEC_ADMINISTRATOR,  true,   &HandleAuraChargesCommand,      "", NULL },
                { "",                   SEC_ADMINISTRATOR,  true,   &HandleAuraCommand,             "", NULL },
                { NULL,                 0,                  false,  NULL,                           "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "dev",                SEC_ADMINISTRATOR,  false,  &HandleDevCommand,              "", NULL },
                { "gps",                SEC_ADMINISTRATOR,  false,  &HandleGPSCommand,              "", NULL },
                { "aura",               SEC_ADMINISTRATOR,  false,  NULL,                           "", auraCommandTable },
                { "unaura",             SEC_ADMINISTRATOR,  false,  &HandleUnAuraCommand,           "", NULL },
                { "appear",             SEC_MODERATOR,      false,  &HandleAppearCommand,           "", NULL },
                { "summon",             SEC_MODERATOR,      false,  &HandleSummonCommand,           "", NULL },
                { "groupsummon",        SEC_MODERATOR,      false,  &HandleGroupSummonCommand,      "", NULL },
                { "commands",           SEC_PLAYER,         true,   &HandleCommandsCommand,         "", NULL },
                { "die",                SEC_ADMINISTRATOR,  false,  &HandleDieCommand,              "", NULL },
                { "revive",             SEC_ADMINISTRATOR,  true,   &HandleReviveCommand,           "", NULL },
                { "dismount",           SEC_PLAYER,         false,  &HandleDismountCommand,         "", NULL },
                { "guid",               SEC_GAMEMASTER,     false,  &HandleGUIDCommand,             "", NULL },
                { "help",               SEC_PLAYER,         true,   &HandleHelpCommand,             "", NULL },
                { "itemmove",           SEC_GAMEMASTER,     false,  &HandleItemMoveCommand,         "", NULL },
                { "cooldown",           SEC_ADMINISTRATOR,  false,  &HandleCooldownCommand,         "", NULL },
                { "distance",           SEC_ADMINISTRATOR,  false,  &HandleGetDistanceCommand,      "", NULL },
                { "recall",             SEC_MODERATOR,      false,  &HandleRecallCommand,           "", NULL },
                { "save",               SEC_PLAYER,         false,  &HandleSaveCommand,             "", NULL },
                { "saveall",            SEC_MODERATOR,      true,   &HandleSaveAllCommand,          "", NULL },
                { "kick",               SEC_GAMEMASTER,     true,   &HandleKickPlayerCommand,       "", NULL },
                { "start",              SEC_PLAYER,         false,  &HandleStartCommand,            "", NULL },
                { "linkgrave",          SEC_ADMINISTRATOR,  false,  &HandleLinkGraveCommand,        "", NULL },
                { "neargrave",          SEC_ADMINISTRATOR,  false,  &HandleNearGraveCommand,        "", NULL },
                { "showarea",           SEC_ADMINISTRATOR,  false,  &HandleShowAreaCommand,         "", NULL },
                { "hidearea",           SEC_ADMINISTRATOR,  false,  &HandleHideAreaCommand,         "", NULL },
                { "additem",            SEC_ADMINISTRATOR,  false,  &HandleAddItemCommand,          "", NULL },
                { "additemset",         SEC_ADMINISTRATOR,  false,  &HandleAddItemSetCommand,       "", NULL },
                { "bank",               SEC_ADMINISTRATOR,  false,  &HandleBankCommand,             "", NULL },
                { "wchange",            SEC_ADMINISTRATOR,  false,  &HandleChangeWeather,           "", NULL },
                { "setskill",           SEC_ADMINISTRATOR,  false,  &HandleSetSkillCommand,         "", NULL },
                { "pinfo",              SEC_GAMEMASTER,     true,   &HandlePInfoCommand,            "", NULL },
                { "respawn",            SEC_ADMINISTRATOR,  false,  &HandleRespawnCommand,          "", NULL },
                { "send",               SEC_MODERATOR,      true,   NULL,                           "", sendCommandTable },
                { "pet",                SEC_GAMEMASTER,     false,  NULL,                           "", petCommandTable },
                { "mute",               SEC_MODERATOR,      true,   &HandleMuteCommand,             "", NULL },
                { "unmute",             SEC_MODERATOR,      true,   &HandleUnmuteCommand,           "", NULL },
                { "movegens",           SEC_ADMINISTRATOR,  false,  &HandleMovegensCommand,         "", NULL },
                { "cometome",           SEC_ADMINISTRATOR,  false,  &HandleComeToMeCommand,         "", NULL },
                { "damage",             SEC_ADMINISTRATOR,  false,  &HandleDamageCommand,           "", NULL },
                { "combatstop",         SEC_GAMEMASTER,     true,   &HandleCombatStopCommand,       "", NULL },
                { "flusharenapoints",   SEC_ADMINISTRATOR,  false,  &HandleFlushArenaPointsCommand, "", NULL },
                { "repairitems",        SEC_GAMEMASTER,     true,   &HandleRepairitemsCommand,      "", NULL },
                { "freeze",             SEC_MODERATOR,      false,  &HandleFreezeCommand,           "", NULL },
                { "unfreeze",           SEC_MODERATOR,      false,  &HandleUnFreezeCommand,         "", NULL },
                { "listfreeze",         SEC_MODERATOR,      false,  &HandleListFreezeCommand,       "", NULL },
                { "group",              SEC_ADMINISTRATOR,  false,  NULL,                           "", groupCommandTable },
                { "possess",            SEC_ADMINISTRATOR,  false,  &HandlePossessCommand,          "", NULL },
                { "unpossess",          SEC_ADMINISTRATOR,  false,  &HandleUnPossessCommand,        "", NULL },
                { "bindsight",          SEC_ADMINISTRATOR,  false,  &HandleBindSightCommand,        "", NULL },
                { "unbindsight",        SEC_ADMINISTRATOR,  false,  &HandleUnbindSightCommand,      "", NULL },
                { "playall",            SEC_GAMEMASTER,     false,  &HandlePlayAllCommand,          "", NULL },
                { "selectfaction",      SEC_ADMINISTRATOR,  false,  &HandleSelectFactionCommand,    "", NULL },
                { "wargame",            SEC_GAMEMASTER,     false,  &HandleWargameCommand,          "", NULL },
                { "chatfilter",         SEC_PLAYER,         false,  &HandleToggleChatFiltering,     "", NULL },
                { "initlfg",            SEC_ADMINISTRATOR,  false,  &HandleInitializeLFGCommand,    "", NULL },
                { NULL,                 0,                  false,  NULL,                           "", NULL }
            };
            return commandTable;
        }

        static bool HandleInitializeLFGCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_Player = p_Handler->getSelectedPlayer();

            if (!l_Player)
                return false;

            sLFGMgr->InitializeLockedDungeons(l_Player);

            return true;
        }

        static bool HandleToggleChatFiltering(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            WorldSession* l_PlayerSession = p_Handler->GetSession();

            if (!l_PlayerSession)
                return true;

            if (l_PlayerSession->HasCustomFlags(AccountCustomFlags::NoChatLocaleFiltering))
            {
                l_PlayerSession->UnsetCustomFlags(AccountCustomFlags::NoChatLocaleFiltering);
                p_Handler->SendSysMessage(TrinityStrings::ChatFilteringOff);
            }
            else
            {
                l_PlayerSession->SetCustomFlags(AccountCustomFlags::NoChatLocaleFiltering);
                p_Handler->SendSysMessage(TrinityStrings::ChatFilteringOn);
            }

            if (l_PlayerSession->GetPlayer())
                l_PlayerSession->GetPlayer()->UpdateChatLocaleFiltering();

            return true;
        }

        static bool HandleWargameCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            /*
            .wargame leaderName1 leaderName2
            */

            if (!*p_Args)
            {
                p_Handler->PSendSysMessage("Invalid syntax.");
                return true;
            }

            char* l_FirstName = strtok((char*)p_Args, " ");
            char* l_SecondName = strtok(NULL, " ");

            if (!*l_FirstName || !*l_SecondName || strlen(l_FirstName) <= 1 || strlen(l_SecondName) <= 1)
            {
                p_Handler->PSendSysMessage("Error about players name.");
                return true;
            }

            Player* l_FirstLeader = sObjectAccessor->FindPlayerByName(l_FirstName);
            if (!l_FirstLeader || l_FirstLeader->getLevel() < 90)
            {
                p_Handler->PSendSysMessage("Error about player %s.", l_FirstName);
                return true;
            }

            Group* l_FirstGroup = l_FirstLeader->GetGroup();
            if (!l_FirstGroup || l_FirstGroup->GetLeaderGUID() != l_FirstLeader->GetGUID())
            {
                p_Handler->PSendSysMessage("Player not in group or not leader.");
                return true;
            }

            Player* l_SecondLeader = sObjectAccessor->FindPlayerByName(l_SecondName);
            if (!l_SecondLeader || l_SecondLeader->getLevel() < 90)
            {
                p_Handler->PSendSysMessage("Error about player %s.", l_SecondName);
                return true;
            }

            Group* l_SecondGroup = l_SecondLeader->GetGroup();
            if (!l_SecondGroup || l_SecondGroup->GetLeaderGUID() != l_SecondLeader->GetGUID())
            {
                p_Handler->PSendSysMessage("Player not in group or not leader.");
                return true;
            }

            uint32 l_GroupSize = l_FirstGroup->GetMembersCount();
            if (l_GroupSize != l_SecondGroup->GetMembersCount())
            {
                p_Handler->PSendSysMessage("Groups are invalid.");
                return true;
            }

            ArenaType l_ArenaType = ArenaType::None;
            switch (l_GroupSize)
            {
            case 2:
                l_ArenaType = ArenaType::Arena2v2;
                break;
            case 3:
                l_ArenaType = ArenaType::Arena3v3;
                break;
            case 5:
                l_ArenaType = ArenaType::Arena5v5;
                break;
            default:
                p_Handler->PSendSysMessage("Groups are invalid.");
                return true;
            }

            Battleground* l_BG = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::AllArenas);
            if (!l_BG)
            {
                p_Handler->PSendSysMessage("Battleground: template BG for All Arenas not found.");
                return true;
            }

            if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
            {
                p_Handler->PSendSysMessage(LANG_ARENA_DISABLED);
                return true;
            }

            MS::Battlegrounds::BattlegroundType::Type l_BGQueueTypeID = MS::Battlegrounds::GetTypeFromId(BATTLEGROUND_AA, l_ArenaType, true);
            MS::Battlegrounds::Bracket const* l_BracketEntry = MS::Battlegrounds::Brackets::FindForLevel(90);
            if (!l_BracketEntry)
            {
                p_Handler->PSendSysMessage("Invalid syntax.");
                return true;
            }

            MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
            MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

            GroupQueueInfo* l_GInfo = l_Scheduler.AddGroup(l_FirstLeader, l_FirstGroup, MS::Battlegrounds::BattlegroundType::AllArenas, nullptr, l_BracketEntry, l_ArenaType, false, 0, 0, true);
            uint32 l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GInfo, l_BracketEntry->m_Id);

            for (GroupReference* l_Itr = l_FirstGroup->GetFirstMember(); l_Itr != NULL; l_Itr = l_Itr->next())
            {
                Player* l_Member = l_Itr->getSource();
                if (!l_Member)
                    continue;

                WorldPacket l_Data;

                // add to queue
                uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

                // send status packet (in queue)
                MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, 0, l_ArenaType, true);
                l_Member->GetSession()->SendPacket(&l_Data);
            }

            l_GInfo = l_Scheduler.AddGroup(l_SecondLeader, l_SecondGroup, MS::Battlegrounds::BattlegroundType::AllArenas, nullptr, l_BracketEntry, l_ArenaType, false, 0, 0, false);
            l_AverageTime = l_InvitationsMgr.GetAverageQueueWaitTime(l_GInfo, l_BracketEntry->m_Id);

            for (GroupReference* l_Itr = l_SecondGroup->GetFirstMember(); l_Itr != NULL; l_Itr = l_Itr->next())
            {
                Player* l_Member = l_Itr->getSource();
                if (!l_Member)
                    continue;

                WorldPacket l_Data;

                // add to queue
                uint32 l_QueueSlot = l_Member->AddBattlegroundQueueId(l_BGQueueTypeID);

                // send status packet (in queue)
                MS::Battlegrounds::PacketFactory::Status(&l_Data, l_BG, l_Member, l_QueueSlot, STATUS_WAIT_QUEUE, l_AverageTime, 0, l_ArenaType, true);
                l_Member->GetSession()->SendPacket(&l_Data);
            }

            return true;
        }

        static bool HandleDevCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
            {
                if (handler->GetSession()->GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_DEVELOPER))
                    handler->GetSession()->SendNotification(LANG_DEV_ON);
                else
                    handler->GetSession()->SendNotification(LANG_DEV_OFF);
                return true;
            }

            std::string argstr = (char*)args;

            if (argstr == "on")
            {
                handler->GetSession()->GetPlayer()->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_DEVELOPER);
                handler->GetSession()->SendNotification(LANG_DEV_ON);
                return true;
            }

            if (argstr == "off")
            {
                handler->GetSession()->GetPlayer()->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_DEVELOPER);
                handler->GetSession()->SendNotification(LANG_DEV_OFF);
                return true;
            }

            handler->SendSysMessage(LANG_USE_BOL);
            handler->SetSentErrorMessage(true);
            return false;
        }

        static bool HandleGPSCommand(ChatHandler* handler, char const* args)
        {
            WorldObject* object = NULL;
            if (*args)
            {
                uint64 guid = handler->extractGuidFromLink((char*)args);
                if (guid)
                    object = (WorldObject*)ObjectAccessor::GetObjectByTypeMask(*handler->GetSession()->GetPlayer(), guid, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);

                if (!object)
                {
                    handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
            else
            {
                object = handler->getSelectedUnit();

                if (!object)
                {
                    handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            CellCoord cellCoord = JadeCore::ComputeCellCoord(object->GetPositionX(), object->GetPositionY());
            Cell cell(cellCoord);

            uint32 zoneId, areaId;
            object->GetZoneAndAreaId(zoneId, areaId);

            MapEntry const* mapEntry = sMapStore.LookupEntry(object->GetMapId());
            AreaTableEntry const* zoneEntry = GetAreaEntryByAreaID(zoneId);
            AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(areaId);

            float zoneX = object->GetPositionX();
            float zoneY = object->GetPositionY();

            Map2ZoneCoordinates(zoneX, zoneY, zoneId);

            Map const* map = object->GetMap();
            float groundZ = map->GetHeight(object->GetPhaseMask(), object->GetPositionX(), object->GetPositionY(), MAX_HEIGHT);
            float floorZ = map->GetHeight(object->GetPhaseMask(), object->GetPositionX(), object->GetPositionY(), object->GetPositionZ());

            GridCoord gridCoord = JadeCore::ComputeGridCoord(object->GetPositionX(), object->GetPositionY());

            // 63? WHY?
            int gridX = 63 - gridCoord.x_coord;
            int gridY = 63 - gridCoord.y_coord;

            uint32 haveMap = Map::ExistMap(object->GetMapId(), gridX, gridY) ? 1 : 0;
            uint32 haveVMap = Map::ExistVMap(object->GetMapId(), gridX, gridY) ? 1 : 0;

            if (haveVMap)
            {
                if (map->IsOutdoors(object->GetPositionX(), object->GetPositionY(), object->GetPositionZ()))
                    handler->PSendSysMessage("You are outdoors");
                else
                    handler->PSendSysMessage("You are indoors");
            }
            else
                handler->PSendSysMessage("no VMAP available for area info");

            handler->PSendSysMessage(LANG_MAP_POSITION,
                object->GetMapId(), (mapEntry ? mapEntry->MapNameLang : "<unknown>"),
                zoneId, (zoneEntry ? zoneEntry->AreaNameLang : "<unknown>"),
                areaId, (areaEntry ? areaEntry->AreaNameLang : "<unknown>"),
                object->GetPhaseMask(),
                object->GetPositionX(), object->GetPositionY(), object->GetPositionZ(), object->GetOrientation(),
                cell.GridX(), cell.GridY(), cell.CellX(), cell.CellY(), object->GetInstanceId(),
                zoneX, zoneY, groundZ, floorZ, haveMap, haveVMap);

#ifdef WIN32
            char   l_Buffer[120];
            char * lPtrData = nullptr;

            sprintf(l_Buffer, "%.4f, %.4f, %.4f, ", object->GetPositionX(), object->GetPositionY(), object->GetPositionZ());

            HANDLE l_Handle;

            int l_BufferSize = strlen(l_Buffer);

            l_Handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, l_BufferSize + 1);

            lPtrData = (char*)GlobalLock(l_Handle);
            memcpy(lPtrData, l_Buffer, l_BufferSize + 1);

            GlobalUnlock(l_Handle);

            OpenClipboard(nullptr);
            EmptyClipboard();
            SetClipboardData(CF_TEXT, l_Handle);
            CloseClipboard();
#endif

            LiquidData liquidStatus;
            ZLiquidStatus status = map->getLiquidStatus(object->GetPositionX(), object->GetPositionY(), object->GetPositionZ(), MAP_ALL_LIQUIDS, &liquidStatus);

            if (status)
                handler->PSendSysMessage(LANG_LIQUID_STATUS, liquidStatus.level, liquidStatus.depth_level, liquidStatus.entry, liquidStatus.type_flags, status);

            return true;
        }

        static bool HandleAuraCommand(ChatHandler* handler, char const* args)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
            uint32 spellId = handler->extractSpellIdFromLink((char*)args);

            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId))
                Aura::TryRefreshStackOrCreate(spellInfo, MAX_EFFECT_MASK, target, target);

            return true;
        }

        static bool HandleAuraDurationCommand(ChatHandler* handler, char const* args)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
            uint32 spellId = handler->extractSpellIdFromLink((char*)args);

            if (!spellId)
                return false;

            char const* dduration = strtok(NULL, " ");
            int32 duration = 1000;

            if (dduration)
                duration = strtol(dduration, NULL, 10);

            if (!duration)
                target->RemoveAurasDueToSpell(spellId);
            else
            {
                if (Aura* aura = target->GetAura(spellId))
                    aura->SetDuration(duration);
                else
                    return false;
            }

            return true;
        }

        static bool HandleAuraChargesCommand(ChatHandler* handler, char const* args)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
            uint32 spellId = handler->extractSpellIdFromLink((char*)args);

            if (!spellId)
                return false;

            char const* ccharges = strtok(NULL, " ");
            int32 charges = 1;

            if (ccharges)
                charges = strtol(ccharges, NULL, 10);

            if (!charges)
                target->RemoveAurasDueToSpell(spellId);
            else
            {
                if (Aura* aura = target->GetAura(spellId))
                    aura->SetCharges(charges);
                else
                    return false;
            }

            return true;
        }

        static bool HandleUnAuraCommand(ChatHandler* handler, char const* args)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            std::string argstr = args;
            if (argstr == "all")
            {
                target->RemoveAllAuras();
                target->SendClearLossOfControl();
                return true;
            }

            // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
            uint32 spellId = handler->extractSpellIdFromLink((char*)args);
            if (!spellId)
                return false;

            target->RemoveAurasDueToSpell(spellId);

            return true;
        }
        // Teleport to Player
        static bool HandleAppearCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            uint64 targetGuid;
            std::string targetName;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;

            Player* _player = handler->GetSession()->GetPlayer();
            if (target == _player || targetGuid == _player->GetGUID())
            {
                handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target)
            {
                // check online security
                if (handler->HasLowerSecurity(target, 0))
                    return false;

                std::string chrNameLink = handler->playerLink(targetName);

                Map* map = target->GetMap();
                if (map->IsBattlegroundOrArena())
                {
                    // only allow if gm mode is on
                    if (!_player->isGameMaster())
                    {
                        handler->PSendSysMessage(LANG_CANNOT_GO_TO_BG_GM, chrNameLink.c_str());
                        handler->SetSentErrorMessage(true);
                        return false;
                    }
                    // if both players are in different bgs
                    else if (_player->GetBattlegroundId() && _player->GetBattlegroundId() != target->GetBattlegroundId())
                        _player->LeaveBattleground(false); // Note: should be changed so _player gets no Deserter debuff

                    // all's well, set bg id
                    // when porting out from the bg, it will be reset to 0
                    _player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
                    // remember current position as entry point for return at bg end teleportation
                    if (!_player->GetMap()->IsBattlegroundOrArena())
                        _player->SetBattlegroundEntryPoint();
                }
                else if (map->IsDungeon())
                {
                    // we have to go to instance, and can go to player only if:
                    //   1) we are in his group (either as leader or as member)
                    //   2) we are not bound to any group and have GM mode on
                    if (_player->GetGroup())
                    {
                        // we are in group, we can go only if we are in the player group
                        if (_player->GetGroup() != target->GetGroup())
                        {
                            handler->PSendSysMessage(LANG_CANNOT_GO_TO_INST_PARTY, chrNameLink.c_str());
                            handler->SetSentErrorMessage(true);
                            return false;
                        }
                    }
                    else
                    {
                        // we are not in group, let's verify our GM mode
                        if (!_player->isGameMaster())
                        {
                            handler->PSendSysMessage(LANG_CANNOT_GO_TO_INST_GM, chrNameLink.c_str());
                            handler->SetSentErrorMessage(true);
                            return false;
                        }
                    }

                    // if the player or the player's group is bound to another instance
                    // the player will not be bound to another one
                    InstancePlayerBind* bind = _player->GetBoundInstance(target->GetMapId(), target->GetDifficultyID(map->GetEntry()));
                    if (!bind)
                    {
                        Group* group = _player->GetGroup();
                        // if no bind exists, create a solo bind
                        InstanceGroupBind* gBind = group ? group->GetBoundInstance(target) : NULL;                // if no bind exists, create a solo bind
                        if (!gBind)
                        if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(target->GetInstanceId()))
                            _player->BindToInstance(save, !save->CanReset());
                    }

                    if (map->IsRaid())
                    {
                        _player->SetRaidDifficultyID(target->GetRaidDifficultyID());
                        _player->SetLegacyRaidDifficultyID(target->GetLegacyRaidDifficultyID());
                    }
                    else
                        _player->SetDungeonDifficultyID(target->GetDungeonDifficultyID());
                }

                handler->PSendSysMessage(LANG_APPEARING_AT, chrNameLink.c_str());

                // stop flight if need
                if (_player->isInFlight())
                {
                    _player->GetMotionMaster()->MovementExpired();
                    _player->CleanupAfterTaxiFlight();
                }
                // save only in non-flight case
                else
                    _player->SaveRecallPosition();

                // to point to see at target with same orientation
                float x, y, z;
                target->GetContactPoint(_player, x, y, z);

                _player->TeleportTo(target->GetMapId(), x, y, z, _player->GetAngle(target), TELE_TO_GM_MODE);
                _player->SetPhaseMask(target->GetPhaseMask(), true);
            }
            else
            {
                // check offline security
                if (handler->HasLowerSecurity(NULL, targetGuid))
                    return false;

                std::string nameLink = handler->playerLink(targetName);

                handler->PSendSysMessage(LANG_APPEARING_AT, nameLink.c_str());

                // to point where player stay (if loaded)
                float x, y, z, o;
                uint32 map;
                bool in_flight;
                if (!Player::LoadPositionFromDB(map, x, y, z, o, in_flight, targetGuid))
                    return false;

                // stop flight if need
                if (_player->isInFlight())
                {
                    _player->GetMotionMaster()->MovementExpired();
                    _player->CleanupAfterTaxiFlight();
                }
                // save only in non-flight case
                else
                    _player->SaveRecallPosition();

                _player->TeleportTo(map, x, y, z, _player->GetOrientation());
            }

            return true;
        }
        // Summon Player
        static bool HandleSummonCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            uint64 targetGuid;
            std::string targetName;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;

            Player* _player = handler->GetSession()->GetPlayer();
            if (target == _player || targetGuid == _player->GetGUID())
            {
                handler->PSendSysMessage(LANG_CANT_TELEPORT_SELF);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target)
            {
                std::string nameLink = handler->playerLink(targetName);
                // check online security
                if (handler->HasLowerSecurity(target, 0))
                    return false;

                if (target->IsBeingTeleported())
                {
                    handler->PSendSysMessage(LANG_IS_TELEPORTED, nameLink.c_str());
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                Map* map = handler->GetSession()->GetPlayer()->GetMap();

                if (map->IsBattlegroundOrArena())
                {
                    // only allow if gm mode is on
                    if (!_player->isGameMaster())
                    {
                        handler->PSendSysMessage(LANG_CANNOT_GO_TO_BG_GM, nameLink.c_str());
                        handler->SetSentErrorMessage(true);
                        return false;
                    }
                    // if both players are in different bgs
                    else if (target->GetBattlegroundId() && handler->GetSession()->GetPlayer()->GetBattlegroundId() != target->GetBattlegroundId())
                        target->LeaveBattleground(false); // Note: should be changed so target gets no Deserter debuff

                    // all's well, set bg id
                    // when porting out from the bg, it will be reset to 0
                    target->SetBattlegroundId(handler->GetSession()->GetPlayer()->GetBattlegroundId(), handler->GetSession()->GetPlayer()->GetBattlegroundTypeId());
                    // remember current position as entry point for return at bg end teleportation
                    if (!target->GetMap()->IsBattlegroundOrArena())
                        target->SetBattlegroundEntryPoint();
                }
                else if (map->IsDungeon())
                {
                    Map* map = target->GetMap();

                    if (map->Instanceable() && map->GetInstanceId() != map->GetInstanceId())
                        target->UnbindInstance(map->GetInstanceId(), target->GetDungeonDifficultyID(), true);

                    // we are in instance, and can summon only player in our group with us as lead
                    if (!handler->GetSession()->GetPlayer()->GetGroup() || !target->GetGroup() ||
                        (target->GetGroup()->GetLeaderGUID() != handler->GetSession()->GetPlayer()->GetGUID()) ||
                        (handler->GetSession()->GetPlayer()->GetGroup()->GetLeaderGUID() != handler->GetSession()->GetPlayer()->GetGUID()))
                        // the last check is a bit excessive, but let it be, just in case
                    {
                        handler->PSendSysMessage(LANG_CANNOT_SUMMON_TO_INST, nameLink.c_str());
                        handler->SetSentErrorMessage(true);
                        return false;
                    }
                }

                handler->PSendSysMessage(LANG_SUMMONING, nameLink.c_str(), "");
                if (handler->needReportToTarget(target))
                    ChatHandler(target).PSendSysMessage(LANG_SUMMONED_BY, handler->playerLink(_player->GetName()).c_str());

                // stop flight if need
                if (target->isInFlight())
                {
                    target->GetMotionMaster()->MovementExpired();
                    target->CleanupAfterTaxiFlight();
                }
                // save only in non-flight case
                else
                    target->SaveRecallPosition();

                // before GM
                float x, y, z;
                handler->GetSession()->GetPlayer()->GetClosePoint(x, y, z, target->GetObjectSize());
                target->TeleportTo(handler->GetSession()->GetPlayer()->GetMapId(), x, y, z, target->GetOrientation());
                target->SetPhaseMask(handler->GetSession()->GetPlayer()->GetPhaseMask(), true);
            }
            else
            {
                // check offline security
                if (handler->HasLowerSecurity(NULL, targetGuid))
                    return false;

                std::string nameLink = handler->playerLink(targetName);

                handler->PSendSysMessage(LANG_SUMMONING, nameLink.c_str(), handler->GetTrinityString(LANG_OFFLINE));

                // in point where GM stay
                Player::SavePositionInDB(handler->GetSession()->GetPlayer()->GetMapId(),
                    handler->GetSession()->GetPlayer()->GetPositionX(),
                    handler->GetSession()->GetPlayer()->GetPositionY(),
                    handler->GetSession()->GetPlayer()->GetPositionZ(),
                    handler->GetSession()->GetPlayer()->GetOrientation(),
                    handler->GetSession()->GetPlayer()->GetZoneId(),
                    targetGuid);
            }

            return true;
        }
        // Summon group of player
        static bool HandleGroupSummonCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            if (!handler->extractPlayerTarget((char*)args, &target))
                return false;

            // check online security
            if (handler->HasLowerSecurity(target, 0))
                return false;

            Group* group = target->GetGroup();

            std::string nameLink = handler->GetNameLink(target);

            if (!group)
            {
                handler->PSendSysMessage(LANG_NOT_IN_GROUP, nameLink.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            Map* gmMap = handler->GetSession()->GetPlayer()->GetMap();
            bool toInstance = gmMap->Instanceable();

            // we are in instance, and can summon only player in our group with us as lead
            if (toInstance && (
                !handler->GetSession()->GetPlayer()->GetGroup() || (group->GetLeaderGUID() != handler->GetSession()->GetPlayer()->GetGUID()) ||
                (handler->GetSession()->GetPlayer()->GetGroup()->GetLeaderGUID() != handler->GetSession()->GetPlayer()->GetGUID())))
                // the last check is a bit excessive, but let it be, just in case
            {
                handler->SendSysMessage(LANG_CANNOT_SUMMON_TO_INST);
                handler->SetSentErrorMessage(true);
                return false;
            }

            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* player = itr->getSource();

                if (!player || player == handler->GetSession()->GetPlayer() || !player->GetSession())
                    continue;

                // check online security
                if (handler->HasLowerSecurity(player, 0))
                    return false;

                std::string plNameLink = handler->GetNameLink(player);

                if (player->IsBeingTeleported() == true)
                {
                    handler->PSendSysMessage(LANG_IS_TELEPORTED, plNameLink.c_str());
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                if (toInstance)
                {
                    Map* playerMap = player->GetMap();

                    if (playerMap->Instanceable() && playerMap->GetInstanceId() != gmMap->GetInstanceId())
                    {
                        // cannot summon from instance to instance
                        handler->PSendSysMessage(LANG_CANNOT_SUMMON_TO_INST, plNameLink.c_str());
                        handler->SetSentErrorMessage(true);
                        return false;
                    }
                }

                handler->PSendSysMessage(LANG_SUMMONING, plNameLink.c_str(), "");
                if (handler->needReportToTarget(player))
                    ChatHandler(player).PSendSysMessage(LANG_SUMMONED_BY, handler->GetNameLink().c_str());

                // stop flight if need
                if (player->isInFlight())
                {
                    player->GetMotionMaster()->MovementExpired();
                    player->CleanupAfterTaxiFlight();
                }
                // save only in non-flight case
                else
                    player->SaveRecallPosition();

                // before GM
                float x, y, z;
                handler->GetSession()->GetPlayer()->GetClosePoint(x, y, z, player->GetObjectSize());
                player->TeleportTo(handler->GetSession()->GetPlayer()->GetMapId(), x, y, z, player->GetOrientation());
            }

            return true;
        }

        static bool HandleCommandsCommand(ChatHandler* handler, char const* /*args*/)
        {
            handler->ShowHelpForCommand(handler->getCommandTable(), "");
            return true;
        }

        static bool HandleDieCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* target = handler->getSelectedUnit();

            if (!target || !handler->GetSession()->GetPlayer()->GetSelection())
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->IsPlayer())
            {
                if (handler->HasLowerSecurity((Player*)target, 0, false))
                    return false;
            }

            if (target->isAlive())
            {
                if (sWorld->getBoolConfig(CONFIG_DIE_COMMAND_MODE))
                    handler->GetSession()->GetPlayer()->Kill(target);
                else
                    handler->GetSession()->GetPlayer()->DealDamage(target, target->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }

            return true;
        }

        static bool HandleReviveCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            uint64 targetGuid;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid))
                return false;

            if (target)
            {
                target->ResurrectPlayer(!AccountMgr::IsPlayerAccount(target->GetSession()->GetSecurity()) ? 1.0f : 0.5f);
                target->SpawnCorpseBones();
                target->SaveToDB();
            }
            else
                // will resurrected at login without corpse
                sObjectAccessor->ConvertCorpseForPlayer(targetGuid);

            return true;
        }

        static bool HandleDismountCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();

            // If player is not mounted, so go out :)
            if (!player->IsMounted())
            {
                handler->SendSysMessage(LANG_CHAR_NON_MOUNTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->isInFlight())
            {
                handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            player->Dismount();
            player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            return true;
        }

        static bool HandleGUIDCommand(ChatHandler* handler, char const* /*args*/)
        {
            uint64 guid = handler->GetSession()->GetPlayer()->GetSelection();

            if (guid == 0)
            {
                handler->SendSysMessage(LANG_NO_SELECTION);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->PSendSysMessage(LANG_OBJECT_GUID, GUID_LOPART(guid), GUID_HIPART(guid));
            return true;
        }

        static bool HandleHelpCommand(ChatHandler* handler, char const* args)
        {
            char const* cmd = strtok((char*)args, " ");
            if (!cmd)
            {
                handler->ShowHelpForCommand(handler->getCommandTable(), "help");
                handler->ShowHelpForCommand(handler->getCommandTable(), "");
            }
            else
            {
                if (!handler->ShowHelpForCommand(handler->getCommandTable(), cmd))
                    handler->SendSysMessage(LANG_NO_HELP_CMD);
            }

            return true;
        }
        // move item to other slot
        static bool HandleItemMoveCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char const* param1 = strtok((char*)args, " ");
            if (!param1)
                return false;

            char const* param2 = strtok(NULL, " ");
            if (!param2)
                return false;

            uint8 srcSlot = uint8(atoi(param1));
            uint8 dstSlot = uint8(atoi(param2));

            if (srcSlot == dstSlot)
                return true;

            if (!handler->GetSession()->GetPlayer()->IsValidPos(INVENTORY_SLOT_BAG_0, srcSlot, true))
                return false;

            if (!handler->GetSession()->GetPlayer()->IsValidPos(INVENTORY_SLOT_BAG_0, dstSlot, false))
                return false;

            uint16 src = ((INVENTORY_SLOT_BAG_0 << 8) | srcSlot);
            uint16 dst = ((INVENTORY_SLOT_BAG_0 << 8) | dstSlot);

            handler->GetSession()->GetPlayer()->SwapItem(src, dst);

            return true;
        }

        static bool HandleCooldownCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            Player* l_Target = p_Handler->getSelectedPlayer();
            if (!l_Target)
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            std::string l_NameLink = p_Handler->GetNameLink(l_Target);

            if (!*p_Args)
            {
                l_Target->RemoveAllSpellCooldown();
                l_Target->ResetAllCharges();
                p_Handler->PSendSysMessage(LANG_REMOVEALL_COOLDOWN, l_NameLink.c_str());
            }
            else
            {
                /// Number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
                uint32 l_SpellID = p_Handler->extractSpellIdFromLink((char*)p_Args);
                if (!l_SpellID)
                    return false;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellID);
                if (l_SpellInfo == nullptr)
                {
                    p_Handler->PSendSysMessage(LANG_UNKNOWN_SPELL, l_Target == p_Handler->GetSession()->GetPlayer() ? p_Handler->GetTrinityString(LANG_YOU) : l_NameLink.c_str());
                    p_Handler->SetSentErrorMessage(true);
                    return false;
                }

                l_Target->RemoveSpellCooldown(l_SpellID, true);
                l_Target->ResetCharges(l_SpellInfo->ChargeCategoryEntry);

                p_Handler->PSendSysMessage(LANG_REMOVE_COOLDOWN, l_SpellID, l_Target == p_Handler->GetSession()->GetPlayer() ? p_Handler->GetTrinityString(LANG_YOU) : l_NameLink.c_str());
            }

            return true;
        }

        static bool HandleGetDistanceCommand(ChatHandler* handler, char const* args)
        {
            WorldObject* obj = NULL;

            if (*args)
            {
                uint64 guid = handler->extractGuidFromLink((char*)args);
                if (guid)
                    obj = (WorldObject*)ObjectAccessor::GetObjectByTypeMask(*handler->GetSession()->GetPlayer(), guid, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);

                if (!obj)
                {
                    handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
            else
            {
                obj = handler->getSelectedUnit();

                if (!obj)
                {
                    handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            handler->PSendSysMessage(LANG_DISTANCE, handler->GetSession()->GetPlayer()->GetDistance(obj), handler->GetSession()->GetPlayer()->GetDistance2d(obj), handler->GetSession()->GetPlayer()->GetExactDist(obj), handler->GetSession()->GetPlayer()->GetExactDist2d(obj));
            return true;
        }
        // Teleport player to last position
        static bool HandleRecallCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            if (!handler->extractPlayerTarget((char*)args, &target))
                return false;

            // check online security
            if (handler->HasLowerSecurity(target, 0))
                return false;

            if (target->IsBeingTeleported())
            {
                handler->PSendSysMessage(LANG_IS_TELEPORTED, handler->GetNameLink(target).c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            // stop flight if need
            if (target->isInFlight())
            {
                target->GetMotionMaster()->MovementExpired();
                target->CleanupAfterTaxiFlight();
            }

            target->TeleportTo(target->m_recallMap, target->m_recallX, target->m_recallY, target->m_recallZ, target->m_recallO);
            return true;
        }

        static bool HandleSaveCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();

            // save GM account without delay and output message
            if (!AccountMgr::IsPlayerAccount(handler->GetSession()->GetSecurity()))
            {
                if (Player* target = handler->getSelectedPlayer())
                    target->SaveToDB();
                else
                    player->SaveToDB();
                handler->SendSysMessage(LANG_PLAYER_SAVED);
                return true;
            }

            // save if the player has last been saved over 20 seconds ago
            uint32 saveInterval = sWorld->getIntConfig(CONFIG_INTERVAL_SAVE);
            if (saveInterval == 0 || (saveInterval > 20 * IN_MILLISECONDS && player->GetSaveTimer() <= saveInterval - 20 * IN_MILLISECONDS))
                player->SaveToDB();

            return true;
        }

        // Save all players in the world
        static bool HandleSaveAllCommand(ChatHandler* handler, char const* /*args*/)
        {
            sObjectAccessor->SaveAllPlayers();
            handler->SendSysMessage(LANG_PLAYERS_SAVED);
            return true;
        }

        // kick player
        static bool HandleKickPlayerCommand(ChatHandler* handler, char const* args)
        {
            Player* target = NULL;
            std::string playerName;
            if (!handler->extractPlayerTarget((char*)args, &target, NULL, &playerName))
                return false;

            if (handler->GetSession() && target == handler->GetSession()->GetPlayer())
            {
                handler->SendSysMessage(LANG_COMMAND_KICKSELF);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // check online security
            if (handler->HasLowerSecurity(target, 0))
                return false;

            if (sWorld->getBoolConfig(CONFIG_SHOW_KICK_IN_WORLD))
                sWorld->SendWorldText(LANG_COMMAND_KICKMESSAGE, playerName.c_str());
            else
                handler->PSendSysMessage(LANG_COMMAND_KICKMESSAGE, playerName.c_str());

            target->GetSession()->KickPlayer();

            return true;
        }

        static bool HandleStartCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();

            if (player->isInFlight())
            {
                handler->SendSysMessage(LANG_YOU_IN_FLIGHT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->isInCombat())
            {
                handler->SendSysMessage(LANG_YOU_IN_COMBAT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->isDead() || player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
            {
                // if player is dead and stuck, send ghost to graveyard
                player->RepopAtGraveyard();
                return true;
            }

            // cast spell Stuck
            player->CastSpell(player, 7355, false);
            return true;
        }

        static bool HandleLinkGraveCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* px = strtok((char*)args, " ");
            if (!px)
                return false;

            uint32 graveyardId = uint32(atoi(px));

            uint32 team;

            char* px2 = strtok(NULL, " ");

            if (!px2)
                team = 0;
            else if (strncmp(px2, "horde", 6) == 0)
                team = HORDE;
            else if (strncmp(px2, "alliance", 9) == 0)
                team = ALLIANCE;
            else
                return false;

            WorldSafeLocsEntry const* graveyard = sWorldSafeLocsStore.LookupEntry(graveyardId);

            if (!graveyard)
            {
                handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDNOEXIST, graveyardId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Player* player = handler->GetSession()->GetPlayer();

            uint32 zoneId = player->GetZoneId();

            AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(zoneId);

            if (!areaEntry || areaEntry->ContinentID != 0)
            {
                handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDWRONGZONE, graveyardId, zoneId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (sObjectMgr->AddGraveYardLink(graveyardId, zoneId, team))
                handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDLINKED, graveyardId, zoneId);
            else
                handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDALRLINKED, graveyardId, zoneId);

            return true;
        }

        static bool HandleNearGraveCommand(ChatHandler* handler, char const* args)
        {
            uint32 team;

            size_t argStr = strlen(args);

            if (!*args)
                team = 0;
            else if (strncmp((char*)args, "horde", argStr) == 0)
                team = HORDE;
            else if (strncmp((char*)args, "alliance", argStr) == 0)
                team = ALLIANCE;
            else
                return false;

            Player* player = handler->GetSession()->GetPlayer();
            uint32 zone_id = player->GetZoneId();

            WorldSafeLocsEntry const* graveyard = sObjectMgr->GetClosestGraveYard(
                player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), team);

            if (graveyard)
            {
                uint32 graveyardId = graveyard->ID;

                GraveYardData const* data = sObjectMgr->FindGraveYardData(graveyardId, zone_id);
                if (!data)
                {
                    handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDERROR, graveyardId);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                team = data->team;

                std::string team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_NOTEAM);

                if (team == 0)
                    team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_ANY);
                else if (team == HORDE)
                    team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_HORDE);
                else if (team == ALLIANCE)
                    team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_ALLIANCE);

                handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDNEAREST, graveyardId, team_name.c_str(), zone_id);
            }
            else
            {
                std::string team_name;

                if (team == 0)
                    team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_ANY);
                else if (team == HORDE)
                    team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_HORDE);
                else if (team == ALLIANCE)
                    team_name = handler->GetTrinityString(LANG_COMMAND_GRAVEYARD_ALLIANCE);

                if (team == ~uint32(0))
                    handler->PSendSysMessage(LANG_COMMAND_ZONENOGRAVEYARDS, zone_id);
                else
                    handler->PSendSysMessage(LANG_COMMAND_ZONENOGRAFACTION, zone_id, team_name.c_str());
            }

            return true;
        }

        static bool HandleShowAreaCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            Player* playerTarget = handler->getSelectedPlayer();
            if (!playerTarget)
            {
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int32 area = GetAreaFlagByAreaID(atoi((char*)args));
            int32 offset = area / 32;
            uint32 val = uint32((1 << (area % 32)));

            if (area<0 || offset >= PLAYER_EXPLORED_ZONES_SIZE)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 currFields = playerTarget->GetUInt32Value(PLAYER_FIELD_EXPLORED_ZONES + offset);
            playerTarget->SetUInt32Value(PLAYER_FIELD_EXPLORED_ZONES + offset, uint32((currFields | val)));

            handler->SendSysMessage(LANG_EXPLORE_AREA);
            return true;
        }

        static bool HandleHideAreaCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            Player* playerTarget = handler->getSelectedPlayer();
            if (!playerTarget)
            {
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int32 area = GetAreaFlagByAreaID(atoi((char*)args));
            int32 offset = area / 32;
            uint32 val = uint32((1 << (area % 32)));

            if (area < 0 || offset >= PLAYER_EXPLORED_ZONES_SIZE)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 currFields = playerTarget->GetUInt32Value(PLAYER_FIELD_EXPLORED_ZONES + offset);
            playerTarget->SetUInt32Value(PLAYER_FIELD_EXPLORED_ZONES + offset, uint32((currFields ^ val)));

            handler->SendSysMessage(LANG_UNEXPLORE_AREA);
            return true;
        }

        static bool HandleAddItemCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
                return false;

            uint32 l_ItemID = 0;
            std::vector<uint32> l_Bonuses;

            /// [name] manual form
            if (p_Args[0] == '[')
            {
                char const* l_ItemNameStr = strtok((char*)p_Args, "]");

                if (l_ItemNameStr && l_ItemNameStr[0])
                {
                    std::string l_ItemName = l_ItemNameStr + 1;
                    WorldDatabase.EscapeString(l_ItemName);

                    PreparedStatement* l_Statement = HotfixDatabase.GetPreparedStatement(HOTFIX_SEL_ITEM_TEMPLATE_BY_NAME);
                    l_Statement->setString(0, l_ItemName);
                    PreparedQueryResult l_Result = HotfixDatabase.Query(l_Statement);

                    if (!l_Result)
                    {
                        p_Handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, l_ItemNameStr + 1);
                        p_Handler->SetSentErrorMessage(true);
                        return false;
                    }

                    l_ItemID = l_Result->Fetch()->GetUInt32();
                }
                else
                    return false;
            }
            /// Item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
            else
                l_ItemID = p_Handler->GetItemIDAndBonusesFromLink((char*)p_Args, l_Bonuses);

            char const* l_CCount = strtok(NULL, " ");

            int32 l_Count = 1;
            if (l_CCount)
                l_Count = strtol(l_CCount, NULL, 10);

            if (l_Count == 0)
                l_Count = 1;

            Player* l_Player = p_Handler->GetSession()->GetPlayer();
            Player* l_PlayerTarget = p_Handler->getSelectedPlayer();
            if (!l_PlayerTarget)
                l_PlayerTarget = l_Player;

            sLog->outDebug(LOG_FILTER_GENERAL, p_Handler->GetTrinityString(LANG_ADDITEM), l_ItemID, l_Count);

            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_ItemID);
            if (!l_ItemTemplate)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_ITEMIDINVALID, l_ItemID);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            /// Subtract
            if (l_Count < 0)
            {
                l_PlayerTarget->DestroyItemCount(l_ItemID, -l_Count, true, false);
                p_Handler->PSendSysMessage(LANG_REMOVEITEM, l_ItemID, -l_Count, p_Handler->GetNameLink(l_PlayerTarget).c_str());
                return true;
            }

            /// Adding items
            uint32 l_NoSpaceForCount = 0;

            /// check space and find places
            ItemPosCountVec l_Dest;
            InventoryResult l_MSG = l_PlayerTarget->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Dest, l_ItemID, l_Count, &l_NoSpaceForCount);

            /// Convert to possible store amount
            if (l_MSG != EQUIP_ERR_OK)
                l_Count -= l_NoSpaceForCount;

            /// Can't add any
            if (l_Count == 0 || l_Dest.empty())
            {
                p_Handler->PSendSysMessage(LANG_ITEM_CANNOT_CREATE, l_ItemID, l_NoSpaceForCount);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            Item* l_Item = l_PlayerTarget->StoreNewItem(l_Dest, l_ItemID, true, Item::GenerateItemRandomPropertyId(l_ItemID));

            /// Remove binding (let GM give it to another player later)
            if (l_Player == l_PlayerTarget)
            {
                for (ItemPosCountVec::const_iterator l_Iter = l_Dest.begin(); l_Iter != l_Dest.end(); ++l_Iter)
                {
                    if (Item* l_Item1 = l_Player->GetItemByPos(l_Iter->pos))
                        l_Item1->SetBinding(false);
                }
            }

            if (l_Count > 0 && l_Item)
            {
                if (!l_Bonuses.empty())
                    l_Item->AddItemBonuses(l_Bonuses);

                l_Player->SendNewItem(l_Item, l_Count, false, true);

                if (l_Player != l_PlayerTarget)
                    l_PlayerTarget->SendNewItem(l_Item, l_Count, true, false);
            }

            if (l_NoSpaceForCount > 0)
                p_Handler->PSendSysMessage(LANG_ITEM_CANNOT_CREATE, l_ItemID, l_NoSpaceForCount);

            return true;
        }

        static bool HandleAddItemSetCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char const* id = handler->extractKeyFromLink((char*)args, "Hitemset"); // number or [name] Shift-click form |color|Hitemset:itemset_id|h[name]|h|r
            if (!id)
                return false;

            uint32 itemSetId = atol(id);

            // prevent generation all items with itemset field value '0'
            if (itemSetId == 0)
            {
                handler->PSendSysMessage(LANG_NO_ITEMS_FROM_ITEMSET_FOUND, itemSetId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Player* player = handler->GetSession()->GetPlayer();
            Player* playerTarget = handler->getSelectedPlayer();
            if (!playerTarget)
                playerTarget = player;

            sLog->outDebug(LOG_FILTER_GENERAL, handler->GetTrinityString(LANG_ADDITEMSET), itemSetId);

            bool found = false;
            ItemTemplateContainer const* its = sObjectMgr->GetItemTemplateStore();
            for (ItemTemplateContainer::const_iterator itr = its->begin(); itr != its->end(); ++itr)
            {
                if (itr->second.ItemSet == itemSetId)
                {
                    found = true;
                    ItemPosCountVec dest;
                    InventoryResult msg = playerTarget->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itr->second.ItemId, 1);
                    if (msg == EQUIP_ERR_OK)
                    {
                        Item* item = playerTarget->StoreNewItem(dest, itr->second.ItemId, true);

                        // remove binding (let GM give it to another player later)
                        if (player == playerTarget)
                            item->SetBinding(false);

                        player->SendNewItem(item, 1, false, true);
                        if (player != playerTarget)
                            playerTarget->SendNewItem(item, 1, true, false);
                    }
                    else
                    {
                        player->SendEquipError(msg, NULL, NULL, itr->second.ItemId);
                        handler->PSendSysMessage(LANG_ITEM_CANNOT_CREATE, itr->second.ItemId, 1);
                    }
                }
            }

            if (!found)
            {
                handler->PSendSysMessage(LANG_NO_ITEMS_FROM_ITEMSET_FOUND, itemSetId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            return true;
        }

        static bool HandleBankCommand(ChatHandler* handler, char const* /*args*/)
        {
            handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());
            return true;
        }

        static bool HandleChangeWeather(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            // Weather is OFF
            if (!sWorld->getBoolConfig(CONFIG_WEATHER))
            {
                handler->SendSysMessage(LANG_WEATHER_DISABLED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // *Change the weather of a cell
            char const* px = strtok((char*)args, " ");
            char const* py = strtok(NULL, " ");

            if (!px || !py)
                return false;

            uint32 type = uint32(atoi(px));                         //0 to 3, 0: fine, 1: rain, 2: snow, 3: sand
            float grade = float(atof(py));                          //0 to 1, sending -1 is instand good weather

            Player* player = handler->GetSession()->GetPlayer();
            uint32 zoneid = player->GetZoneId();

            Weather* weather = WeatherMgr::FindWeather(zoneid);

            if (!weather)
                weather = WeatherMgr::AddWeather(zoneid);
            if (!weather)
            {
                handler->SendSysMessage(LANG_NO_WEATHER);
                handler->SetSentErrorMessage(true);
                return false;
            }

            weather->SetWeather(WeatherType(type), grade);

            return true;
        }

        static bool HandleSetSkillCommand(ChatHandler* handler, char const* args)
        {
            // number or [name] Shift-click form |color|Hskill:skill_id|h[name]|h|r
            char const* skillStr = handler->extractKeyFromLink((char*)args, "Hskill");
            if (!skillStr)
                return false;

            char const* levelStr = strtok(NULL, " ");
            if (!levelStr)
                return false;

            char const* maxPureSkill = strtok(NULL, " ");

            int32 skill = atoi(skillStr);
            if (skill <= 0)
            {
                handler->PSendSysMessage(LANG_INVALID_SKILL_ID, skill);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int32 level = uint32(atol(levelStr));

            Player* target = handler->getSelectedPlayer();
            if (!target)
            {
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            SkillLineEntry const* skillLine = sSkillLineStore.LookupEntry(skill);
            if (!skillLine)
            {
                handler->PSendSysMessage(LANG_INVALID_SKILL_ID, skill);
                handler->SetSentErrorMessage(true);
                return false;
            }

            std::string tNameLink = handler->GetNameLink(target);

            if (!target->GetSkillValue(skill))
            {
                handler->PSendSysMessage(LANG_SET_SKILL_ERROR, tNameLink.c_str(), skill, skillLine->name);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint16 max = maxPureSkill ? atol(maxPureSkill) : target->GetPureMaxSkillValue(skill);

            if (level <= 0 || level > max || max <= 0)
                return false;

            target->SetSkill(skill, target->GetSkillStep(skill), level, max);
            handler->PSendSysMessage(LANG_SET_SKILL, skill, skillLine->name, tNameLink.c_str(), level, max);

            return true;
        }
        // show info of player
        static bool HandlePInfoCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            uint64 targetGuid;
            std::string targetName;

            uint32 parseGUID = MAKE_NEW_GUID(atol((char*)args), 0, HIGHGUID_PLAYER);

            if (sObjectMgr->GetPlayerNameByGUID(parseGUID, targetName))
            {
                target = sObjectMgr->GetPlayerByLowGUID(parseGUID);
                targetGuid = parseGUID;
            }
            else if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;

            uint32 accId = 0;
            uint64 money = 0;
            uint32 totalPlayerTime = 0;
            uint8 level = 0;
            uint32 latency = 0;
            uint8 race;
            uint8 Class;
            int64 muteTime = 0;
            int64 banTime = -1;
            uint32 mapId;
            uint32 areaId;
            uint32 phase = 0;
            uint32 l_GarrisonID = 0;

            // get additional information from Player object
            if (target)
            {
                // check online security
                if (handler->HasLowerSecurity(target, 0))
                    return false;

                accId = target->GetSession()->GetAccountId();
                money = target->GetMoney();
                totalPlayerTime = target->GetTotalPlayedTime();
                level = target->getLevel();
                latency = target->GetSession()->GetLatency();
                race = target->getRace();
                Class = target->getClass();
                muteTime = target->GetSession()->m_muteTime;
                mapId = target->GetMapId();
                areaId = target->GetAreaId();
                phase = target->GetPhaseMask();

                if (MS::Garrison::Manager* l_Garr = target->GetGarrison())
                    l_GarrisonID = l_Garr->GetGarrisonID();
            }
            // get additional information from DB
            else
            {
                // check offline security
                if (handler->HasLowerSecurity(NULL, targetGuid))
                    return false;

                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_PINFO);
                stmt->setUInt32(0, GUID_LOPART(targetGuid));
                PreparedQueryResult result = CharacterDatabase.Query(stmt);

                if (!result)
                    return false;

                Field* fields = result->Fetch();
                totalPlayerTime = fields[0].GetUInt32();
                level = fields[1].GetUInt8();
                money = fields[2].GetUInt64();
                accId = fields[3].GetUInt32();
                race = fields[4].GetUInt8();
                Class = fields[5].GetUInt8();
                mapId = fields[6].GetUInt16();
                areaId = fields[7].GetUInt16();
            }

            std::string userName = handler->GetTrinityString(LANG_ERROR);
            std::string eMail = handler->GetTrinityString(LANG_ERROR);
            std::string lastIp = handler->GetTrinityString(LANG_ERROR);
            uint32 security = 0;
            std::string lastLogin = handler->GetTrinityString(LANG_ERROR);

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PINFO);
            stmt->setInt32(0, int32(g_RealmID));
            stmt->setUInt32(1, accId);
            PreparedQueryResult result = LoginDatabase.Query(stmt);

            if (result)
            {
                Field* fields = result->Fetch();
                userName = fields[0].GetString();
                security = fields[1].GetUInt8();
                eMail = fields[2].GetString();
                muteTime = fields[5].GetUInt64();

                if (eMail.empty())
                    eMail = "-";

                if (!handler->GetSession() || handler->GetSession()->GetSecurity() >= AccountTypes(security))
                {
                    lastIp = fields[3].GetString();
                    lastLogin = fields[4].GetString();

                    uint32 ip = inet_addr(lastIp.c_str());
    #if TRINITY_ENDIAN == BIGENDIAN
                    EndianConvertReverse(ip);
    #endif

                    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_IP2NATION_COUNTRY);

                    stmt->setUInt32(0, ip);

                    PreparedQueryResult result2 = WorldDatabase.Query(stmt);

                    if (result2)
                    {
                        Field* fields2 = result2->Fetch();
                        lastIp.append(" (");
                        lastIp.append(fields2[0].GetString());
                        lastIp.append(")");
                    }
                }
                else
                {
                    lastIp = "-";
                    lastLogin = "-";
                }
            }

            std::string nameLink = handler->playerLink(targetName);

            handler->PSendSysMessage(LANG_PINFO_ACCOUNT, (target ? "" : handler->GetTrinityString(LANG_OFFLINE)), nameLink.c_str(), GUID_LOPART(targetGuid), userName.c_str(), accId, eMail.c_str(), security, lastIp.c_str(), lastLogin.c_str(), latency);

            std::string bannedby = "unknown";
            std::string banreason = "";

            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PINFO_BANS);
            stmt->setUInt32(0, accId);
            PreparedQueryResult result2 = LoginDatabase.Query(stmt);
            if (!result2)
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PINFO_BANS);
                stmt->setUInt32(0, GUID_LOPART(targetGuid));
                result2 = CharacterDatabase.Query(stmt);
            }

            if (result2)
            {
                Field* fields = result2->Fetch();
                banTime = int64(fields[1].GetBool() ? 0 : fields[0].GetUInt32());
                bannedby = fields[2].GetString();
                banreason = fields[3].GetString();
            }

            if (muteTime > 0)
                handler->PSendSysMessage(LANG_PINFO_MUTE, secsToTimeString(muteTime - time(NULL), true).c_str());

            if (banTime >= 0)
                handler->PSendSysMessage(LANG_PINFO_BAN, banTime > 0 ? secsToTimeString(banTime - time(NULL), true).c_str() : "permanently", bannedby.c_str(), banreason.c_str());

            std::string raceStr, ClassStr;
            switch (race)
            {
            case RACE_HUMAN:
                raceStr = "Human";
                break;
            case RACE_ORC:
                raceStr = "Orc";
                break;
            case RACE_DWARF:
                raceStr = "Dwarf";
                break;
            case RACE_NIGHTELF:
                raceStr = "Night Elf";
                break;
            case RACE_UNDEAD_PLAYER:
                raceStr = "Undead";
                break;
            case RACE_TAUREN:
                raceStr = "Tauren";
                break;
            case RACE_GNOME:
                raceStr = "Gnome";
                break;
            case RACE_TROLL:
                raceStr = "Troll";
                break;
            case RACE_BLOODELF:
                raceStr = "Blood Elf";
                break;
            case RACE_DRAENEI:
                raceStr = "Draenei";
                break;
            case RACE_GOBLIN:
                raceStr = "Goblin";
                break;
            case RACE_WORGEN:
                raceStr = "Worgen";
                break;
            case RACE_PANDAREN_ALLI:
                raceStr = "Pandaren alliance";
                break;
            case RACE_PANDAREN_HORDE:
                raceStr = "Pandaren horde";
                break;
            case RACE_PANDAREN_NEUTRAL:
                raceStr = "Pandaren neutral";
                break;
            }

            switch (Class)
            {
            case CLASS_WARRIOR:
                ClassStr = "Warrior";
                break;
            case CLASS_PALADIN:
                ClassStr = "Paladin";
                break;
            case CLASS_HUNTER:
                ClassStr = "Hunter";
                break;
            case CLASS_ROGUE:
                ClassStr = "Rogue";
                break;
            case CLASS_PRIEST:
                ClassStr = "Priest";
                break;
            case CLASS_DEATH_KNIGHT:
                ClassStr = "Death Knight";
                break;
            case CLASS_SHAMAN:
                ClassStr = "Shaman";
                break;
            case CLASS_MAGE:
                ClassStr = "Mage";
                break;
            case CLASS_WARLOCK:
                ClassStr = "Warlock";
                break;
            case CLASS_MONK:
                ClassStr = "Monk";
                break;
            case CLASS_DRUID:
                ClassStr = "Druid";
                break;
            }

            std::string timeStr = secsToTimeString(totalPlayerTime, true, true);
            uint32 gold = money / GOLD;
            uint32 silv = (money % GOLD) / SILVER;
            uint32 copp = (money % GOLD) % SILVER;
            handler->PSendSysMessage(LANG_PINFO_LEVEL, raceStr.c_str(), ClassStr.c_str(), timeStr.c_str(), level, gold, silv, copp);

            // Add map, zone, subzone and phase to output
            std::string areaName = "<unknown>";
            std::string zoneName = "";

            MapEntry const* map = sMapStore.LookupEntry(mapId);

            AreaTableEntry const* area = GetAreaEntryByAreaID(areaId);
            if (area)
            {
                areaName = area->AreaNameLang;

                AreaTableEntry const* zone = GetAreaEntryByAreaID(area->ContinentID);

                if (zone)
                    zoneName = zone->AreaNameLang;
            }

            if (target)
            {
                if (!zoneName.empty())
                    handler->PSendSysMessage(LANG_PINFO_MAP_ONLINE, l_GarrisonID, map->MapNameLang, zoneName.c_str(), areaName.c_str(), phase);
                else
                    handler->PSendSysMessage(LANG_PINFO_MAP_ONLINE, l_GarrisonID, map->MapNameLang, areaName.c_str(), "<unknown>", phase);
            }
            else
                handler->PSendSysMessage(LANG_PINFO_MAP_OFFLINE, map->MapNameLang, areaName.c_str());

            return true;
        }

        static bool HandleRespawnCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();

            // accept only explicitly selected target (not implicitly self targeting case)
            Unit* target = handler->getSelectedUnit();
            if (player->GetSelection() && target)
            {
                if (target->GetTypeId() != TYPEID_UNIT || target->isPet())
                {
                    handler->SendSysMessage(LANG_SELECT_CREATURE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                if (target->isDead())
                    target->ToCreature()->Respawn();
                return true;
            }

            CellCoord p(JadeCore::ComputeCellCoord(player->GetPositionX(), player->GetPositionY()));
            Cell cell(p);
            cell.SetNoCreate();

            JadeCore::RespawnDo u_do;
            JadeCore::WorldObjectWorker<JadeCore::RespawnDo> worker(player, u_do);

            TypeContainerVisitor<JadeCore::WorldObjectWorker<JadeCore::RespawnDo>, GridTypeMapContainer > obj_worker(worker);
            cell.Visit(p, obj_worker, *player->GetMap(), *player, player->GetGridActivationRange());

            return true;
        }
        // mute player for some times
        static bool HandleMuteCommand(ChatHandler* handler, char const* args)
        {
            char* nameStr;
            char* delayStr;
            handler->extractOptFirstArg((char*)args, &nameStr, &delayStr);
            if (!delayStr)
                return false;

            char const* muteReason = strtok(NULL, "\r");
            std::string muteReasonStr = "No reason";
            if (muteReason != NULL)
                muteReasonStr = muteReason;

            Player* target;
            uint64 targetGuid;
            std::string targetName;
            if (!handler->extractPlayerTarget(nameStr, &target, &targetGuid, &targetName))
                return false;

            uint32 accountId = target ? target->GetSession()->GetAccountId() : sObjectMgr->GetPlayerAccountIdByGUID(targetGuid);

            // find only player from same account if any
            if (!target)
            if (WorldSession* session = sWorld->FindSession(accountId))
                target = session->GetPlayer();

            uint32 notSpeakTime = uint32(atoi(delayStr));

            // must have strong lesser security level
            if (handler->HasLowerSecurity(target, targetGuid, true))
                return false;

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);

            if (target)
            {
                // Target is online, mute will be in effect right away.
                int64 muteTime = time(NULL) + notSpeakTime * MINUTE;
                target->GetSession()->m_muteTime = muteTime;
                stmt->setInt64(0, muteTime);
                ChatHandler(target).PSendSysMessage(LANG_YOUR_CHAT_DISABLED, notSpeakTime, muteReasonStr.c_str());
            }
            else
            {
                // Target is offline, mute will be in effect starting from the next login.
                int32 muteTime = -int32(notSpeakTime * MINUTE);
                stmt->setInt64(0, muteTime);
            }

            stmt->setUInt32(1, accountId);
            LoginDatabase.Execute(stmt);
            std::string nameLink = handler->playerLink(targetName);

            handler->PSendSysMessage(target ? LANG_YOU_DISABLE_CHAT : LANG_COMMAND_DISABLE_CHAT_DELAYED, nameLink.c_str(), notSpeakTime, muteReasonStr.c_str());

            if (sWorld->getBoolConfig(CONFIG_ANNOUNCE_MUTE))
            {
                std::string announce;

                announce = "The character '";
                if (nameStr)  announce += nameStr;
                announce += "' was muted for ";
                if (delayStr) announce += delayStr;
                announce += " minutes by the character '";
                announce += handler->GetSession() ? handler->GetSession()->GetPlayerName() : "";
                announce += "'. The reason is: ";
                announce += muteReasonStr;

                char buff[2048];
                sprintf(buff, handler->GetTrinityString(LANG_SYSTEMMESSAGE), announce.c_str());
                sWorld->SendServerMessage(SERVER_MSG_STRING, buff);
            }
            return true;
        }

        // unmute player
        static bool HandleUnmuteCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            uint64 targetGuid;
            std::string targetName;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;

            uint32 accountId = target ? target->GetSession()->GetAccountId() : sObjectMgr->GetPlayerAccountIdByGUID(targetGuid);

            // find only player from same account if any
            if (!target)
            if (WorldSession* session = sWorld->FindSession(accountId))
                target = session->GetPlayer();

            // must have strong lesser security level
            if (handler->HasLowerSecurity(target, targetGuid, true))
                return false;

            if (target)
            {
                if (target->CanSpeak())
                {
                    handler->SendSysMessage(LANG_CHAT_ALREADY_ENABLED);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                target->GetSession()->m_muteTime = 0;
            }

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME);
            stmt->setInt64(0, 0);
            stmt->setUInt32(1, accountId);
            LoginDatabase.Execute(stmt);

            if (target)
                ChatHandler(target).PSendSysMessage(LANG_YOUR_CHAT_ENABLED);

            std::string nameLink = handler->playerLink(targetName);

            handler->PSendSysMessage(LANG_YOU_ENABLE_CHAT, nameLink.c_str());

            return true;
        }


        static bool HandleMovegensCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->PSendSysMessage(LANG_MOVEGENS_LIST, (unit->IsPlayer() ? "Player" : "Creature"), unit->GetGUIDLow());

            MotionMaster* motionMaster = unit->GetMotionMaster();
            float x, y, z;
            motionMaster->GetDestination(x, y, z);

            for (uint8 i = 0; i < MAX_MOTION_SLOT; ++i)
            {
                MovementGenerator* movementGenerator = motionMaster->GetMotionSlot(i);
                if (!movementGenerator)
                {
                    handler->SendSysMessage("Empty");
                    continue;
                }

                switch (movementGenerator->GetMovementGeneratorType())
                {
                case IDLE_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_IDLE);
                    break;
                case RANDOM_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_RANDOM);
                    break;
                case WAYPOINT_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_WAYPOINT);
                    break;
                case ANIMAL_RANDOM_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_ANIMAL_RANDOM);
                    break;
                case CONFUSED_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_CONFUSED);
                    break;
                case CHASE_MOTION_TYPE:
                {
                                          Unit* target = NULL;
                                          if (unit->IsPlayer())
                                              target = static_cast<ChaseMovementGenerator<Player> const*>(movementGenerator)->GetTarget();
                                          else
                                              target = static_cast<ChaseMovementGenerator<Creature> const*>(movementGenerator)->GetTarget();

                                          if (!target)
                                              handler->SendSysMessage(LANG_MOVEGENS_CHASE_NULL);
                                          else if (target->IsPlayer())
                                              handler->PSendSysMessage(LANG_MOVEGENS_CHASE_PLAYER, target->GetName(), target->GetGUIDLow());
                                          else
                                              handler->PSendSysMessage(LANG_MOVEGENS_CHASE_CREATURE, target->GetName(), target->GetGUIDLow());
                                          break;
                }
                case FOLLOW_MOTION_TYPE:
                {
                                           Unit* target = NULL;
                                           if (unit->IsPlayer())
                                               target = static_cast<FollowMovementGenerator<Player> const*>(movementGenerator)->GetTarget();
                                           else
                                               target = static_cast<FollowMovementGenerator<Creature> const*>(movementGenerator)->GetTarget();

                                           if (!target)
                                               handler->SendSysMessage(LANG_MOVEGENS_FOLLOW_NULL);
                                           else if (target->IsPlayer())
                                               handler->PSendSysMessage(LANG_MOVEGENS_FOLLOW_PLAYER, target->GetName(), target->GetGUIDLow());
                                           else
                                               handler->PSendSysMessage(LANG_MOVEGENS_FOLLOW_CREATURE, target->GetName(), target->GetGUIDLow());
                                           break;
                }
                case HOME_MOTION_TYPE:
                {
                                         if (unit->GetTypeId() == TYPEID_UNIT)
                                             handler->PSendSysMessage(LANG_MOVEGENS_HOME_CREATURE, x, y, z);
                                         else
                                             handler->SendSysMessage(LANG_MOVEGENS_HOME_PLAYER);
                                         break;
                }
                case FLIGHT_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_FLIGHT);
                    break;
                case POINT_MOTION_TYPE:
                {
                                          handler->PSendSysMessage(LANG_MOVEGENS_POINT, x, y, z);
                                          break;
                }
                case FLEEING_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_FEAR);
                    break;
                case DISTRACT_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_DISTRACT);
                    break;
                case EFFECT_MOTION_TYPE:
                    handler->SendSysMessage(LANG_MOVEGENS_EFFECT);
                    break;
                default:
                    handler->PSendSysMessage(LANG_MOVEGENS_UNKNOWN, movementGenerator->GetMovementGeneratorType());
                    break;
                }
            }
            return true;
        }
        /*
        ComeToMe command REQUIRED for 3rd party scripting library to have access to PointMovementGenerator
        Without this function 3rd party scripting library will get linking errors (unresolved external)
        when attempting to use the PointMovementGenerator
        */
        static bool HandleComeToMeCommand(ChatHandler* handler, char const* args)
        {
            char const* newFlagStr = strtok((char*)args, " ");
            if (!newFlagStr)
                return false;

            Creature* caster = handler->getSelectedCreature();
            if (!caster)
            {
                handler->SendSysMessage(LANG_SELECT_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Player* player = handler->GetSession()->GetPlayer();

            caster->GetMotionMaster()->MovePoint(0, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());

            return true;
        }

        static bool HandleDamageCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* str = strtok((char*)args, " ");

            if (strcmp(str, "go") == 0)
            {
                char* guidStr = strtok(NULL, " ");
                if (!guidStr)
                {
                    handler->SendSysMessage(LANG_BAD_VALUE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                int32 guid = atoi(guidStr);
                if (!guid)
                {
                    handler->SendSysMessage(LANG_BAD_VALUE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                char* damageStr = strtok(NULL, " ");
                if (!damageStr)
                {
                    handler->SendSysMessage(LANG_BAD_VALUE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                int32 damage = atoi(damageStr);
                if (!damage)
                {
                    handler->SendSysMessage(LANG_BAD_VALUE);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                if (Player* player = handler->GetSession()->GetPlayer())
                {
                    GameObject* go = NULL;

                    if (GameObjectData const* goData = sObjectMgr->GetGOData(guid))
                        go = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guid, goData->id);

                    if (!go)
                    {
                        handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guid);
                        handler->SetSentErrorMessage(true);
                        return false;
                    }

                    if (!go->IsDestructibleBuilding())
                    {
                        handler->SendSysMessage(LANG_INVALID_GAMEOBJECT_TYPE);
                        handler->SetSentErrorMessage(true);
                        return false;
                    }

                    go->ModifyHealth(-damage, player);
                    handler->PSendSysMessage(LANG_GAMEOBJECT_DAMAGED, go->GetName(), guid, -damage, go->GetGOValue()->Building.Health);
                }

                return true;
            }

            Unit* target = handler->getSelectedUnit();
            if (!target || !handler->GetSession()->GetPlayer()->GetSelection())
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->IsPlayer())
            {
                if (handler->HasLowerSecurity((Player*)target, 0, false))
                    return false;
            }

            if (!target->isAlive())
                return true;

            char* damageStr = str;
            if (!damageStr)
                return false;

            int32 damage_int = atoi(damageStr);
            if (damage_int <= 0)
                return true;

            uint32 damage = damage_int;

            char* schoolStr = strtok(nullptr, " ");

            // flat melee damage without resistence/etc reduction
            if (!schoolStr)
            {
                handler->GetSession()->GetPlayer()->DealDamage(target, damage, nullptr, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, false);
                if (target != handler->GetSession()->GetPlayer())
                    handler->GetSession()->GetPlayer()->SendAttackStateUpdate(HITINFO_AFFECTS_VICTIM, target, 1, SPELL_SCHOOL_MASK_NORMAL, damage, 0, 0, VICTIMSTATE_HIT, 0);
                return true;
            }

            uint32 school = schoolStr ? atoi((char*)schoolStr) : SPELL_SCHOOL_NORMAL;
            if (school >= MAX_SPELL_SCHOOL)
                return false;

            SpellSchoolMask schoolmask = SpellSchoolMask(1 << school);

            if (Unit::IsDamageReducedByArmor(schoolmask))
                damage = handler->GetSession()->GetPlayer()->CalcArmorReducedDamage(target, damage, nullptr, WeaponAttackType::BaseAttack);

            char* spellStr = strtok(nullptr, " ");

            // melee damage by specific school
            if (!spellStr)
            {
                uint32 absorb = 0;
                uint32 resist = 0;

                handler->GetSession()->GetPlayer()->CalcAbsorbResist(target, schoolmask, SPELL_DIRECT_DAMAGE, damage, &absorb, &resist);

                if (damage <= absorb + resist)
                    return true;

                damage -= absorb + resist;

                handler->GetSession()->GetPlayer()->DealDamageMods(target, damage, &absorb);
                handler->GetSession()->GetPlayer()->DealDamage(target, damage, nullptr, DIRECT_DAMAGE, schoolmask, nullptr, false);
                handler->GetSession()->GetPlayer()->SendAttackStateUpdate(HITINFO_AFFECTS_VICTIM, target, 1, schoolmask, damage, absorb, resist, VICTIMSTATE_HIT, 0);
                return true;
            }

            // non-melee damage

            // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
            uint32 spellid = handler->extractSpellIdFromLink((char*)args);
            if (!spellid || !sSpellMgr->GetSpellInfo(spellid))
                return false;

            handler->GetSession()->GetPlayer()->SpellNonMeleeDamageLog(target, spellid, damage);
            return true;
        }

        static bool HandleCombatStopCommand(ChatHandler* handler, char const* args)
        {
            Player* target = NULL;

            if (args && strlen(args) > 0)
            {
                target = sObjectAccessor->FindPlayerByName(args);
                if (!target)
                {
                    handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            if (!target)
            {
                if (!handler->extractPlayerTarget((char*)args, &target))
                    return false;
            }

            // check online security
            if (handler->HasLowerSecurity(target, 0))
                return false;

            target->CombatStop();
            target->getHostileRefManager().deleteReferences();
            return true;
        }

        static bool HandleFlushArenaPointsCommand(ChatHandler* /*handler*/, char const* /*args*/)
        {
            sWorld->ResetCurrencyWeekCap();
            return true;
        }

        static bool HandleRepairitemsCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            if (!handler->extractPlayerTarget((char*)args, &target))
                return false;

            // check online security
            if (handler->HasLowerSecurity(target, 0))
                return false;

            // Repair items
            target->DurabilityRepairAll(false, 0, false);

            handler->PSendSysMessage(LANG_YOU_REPAIR_ITEMS, handler->GetNameLink(target).c_str());
            if (handler->needReportToTarget(target))
                ChatHandler(target).PSendSysMessage(LANG_YOUR_ITEMS_REPAIRED, handler->GetNameLink().c_str());

            return true;
        }

        // Send mail by command
        static bool HandleSendMailCommand(ChatHandler* handler, char const* args)
        {
            // format: name "subject text" "mail text"
            Player* target;
            uint64 targetGuid;
            std::string targetName;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;

            char* tail1 = strtok(NULL, "");
            if (!tail1)
                return false;

            char const* msgSubject = handler->extractQuotedArg(tail1);
            if (!msgSubject)
                return false;

            char* tail2 = strtok(NULL, "");
            if (!tail2)
                return false;

            char const* msgText = handler->extractQuotedArg(tail2);
            if (!msgText)
                return false;

            // msgSubject, msgText isn't NUL after prev. check
            std::string subject = msgSubject;
            std::string text = msgText;

            // from console show not existed sender
            MailSender sender(MAIL_NORMAL, handler->GetSession() ? handler->GetSession()->GetPlayer()->GetGUIDLow() : 0, MAIL_STATIONERY_GM);

            //- TODO: Fix poor design
            SQLTransaction trans = CharacterDatabase.BeginTransaction();
            MailDraft(subject, text)
                .SendMailTo(trans, MailReceiver(target, GUID_LOPART(targetGuid)), sender);

            CharacterDatabase.CommitTransaction(trans);

            std::string nameLink = handler->playerLink(targetName);
            handler->PSendSysMessage(LANG_MAIL_SENT, nameLink.c_str());
            return true;
        }
        // Send items by mail
        static bool HandleSendItemsCommand(ChatHandler* handler, char const* args)
        {
            // format: name "subject text" "mail text" item1[:count1] item2[:count2] ... item12[:count12]
            Player* receiver;
            uint64 receiverGuid;
            std::string receiverName;
            if (!handler->extractPlayerTarget((char*)args, &receiver, &receiverGuid, &receiverName))
                return false;

            char* tail1 = strtok(NULL, "");
            if (!tail1)
                return false;

            char const* msgSubject = handler->extractQuotedArg(tail1);
            if (!msgSubject)
                return false;

            char* tail2 = strtok(NULL, "");
            if (!tail2)
                return false;

            char const* msgText = handler->extractQuotedArg(tail2);
            if (!msgText)
                return false;

            // msgSubject, msgText isn't NUL after prev. check
            std::string subject = msgSubject;
            std::string text = msgText;

            // extract items
            typedef std::pair<uint32, uint32> ItemPair;
            typedef std::list< ItemPair > ItemPairs;
            ItemPairs items;

            // get all tail string
            char* tail = strtok(NULL, "");

            // get from tail next item str
            while (char* itemStr = strtok(tail, " "))
            {
                // and get new tail
                tail = strtok(NULL, "");

                // parse item str
                char const* itemIdStr = strtok(itemStr, ":");
                char const* itemCountStr = strtok(NULL, " ");

                uint32 itemId = atoi(itemIdStr);
                if (!itemId)
                    return false;

                ItemTemplate const* item_proto = sObjectMgr->GetItemTemplate(itemId);
                if (!item_proto)
                {
                    handler->PSendSysMessage(LANG_COMMAND_ITEMIDINVALID, itemId);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                uint32 itemCount = itemCountStr ? atoi(itemCountStr) : 1;
                if (itemCount < 1 || (item_proto->MaxCount > 0 && itemCount > uint32(item_proto->MaxCount)))
                {
                    handler->PSendSysMessage(LANG_COMMAND_INVALID_ITEM_COUNT, itemCount, itemId);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                while (itemCount > item_proto->GetMaxStackSize())
                {
                    items.push_back(ItemPair(itemId, item_proto->GetMaxStackSize()));
                    itemCount -= item_proto->GetMaxStackSize();
                }

                items.push_back(ItemPair(itemId, itemCount));

                if (items.size() > MAX_MAIL_ITEMS)
                {
                    handler->PSendSysMessage(LANG_COMMAND_MAIL_ITEMS_LIMIT, MAX_MAIL_ITEMS);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }

            // from console show not existed sender
            MailSender sender(MAIL_NORMAL, handler->GetSession() ? handler->GetSession()->GetPlayer()->GetGUIDLow() : 0, MAIL_STATIONERY_GM);

            // fill mail
            MailDraft draft(subject, text);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();

            for (ItemPairs::const_iterator itr = items.begin(); itr != items.end(); ++itr)
            {
                if (Item* item = Item::CreateItem(itr->first, itr->second, handler->GetSession() ? handler->GetSession()->GetPlayer() : 0))
                {
                    item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted
                    draft.AddItem(item);
                }
            }

            draft.SendMailTo(trans, MailReceiver(receiver, GUID_LOPART(receiverGuid)), sender);
            CharacterDatabase.CommitTransaction(trans);

            std::string nameLink = handler->playerLink(receiverName);
            handler->PSendSysMessage(LANG_MAIL_SENT, nameLink.c_str());
            return true;
        }
        /// Send money by mail
        static bool HandleSendMoneyCommand(ChatHandler* handler, char const* args)
        {
            /// format: name "subject text" "mail text" money

            Player* receiver;
            uint64 receiverGuid;
            std::string receiverName;
            if (!handler->extractPlayerTarget((char*)args, &receiver, &receiverGuid, &receiverName))
                return false;

            char* tail1 = strtok(NULL, "");
            if (!tail1)
                return false;

            char* msgSubject = handler->extractQuotedArg(tail1);
            if (!msgSubject)
                return false;

            char* tail2 = strtok(NULL, "");
            if (!tail2)
                return false;

            char* msgText = handler->extractQuotedArg(tail2);
            if (!msgText)
                return false;

            char* moneyStr = strtok(NULL, "");
            int32 money = moneyStr ? atoi(moneyStr) : 0;
            if (money <= 0)
                return false;

            // msgSubject, msgText isn't NUL after prev. check
            std::string subject = msgSubject;
            std::string text = msgText;

            // from console show not existed sender
            MailSender sender(MAIL_NORMAL, handler->GetSession() ? handler->GetSession()->GetPlayer()->GetGUIDLow() : 0, MAIL_STATIONERY_GM);

            SQLTransaction trans = CharacterDatabase.BeginTransaction();

            MailDraft(subject, text)
                .AddMoney(money)
                .SendMailTo(trans, MailReceiver(receiver, GUID_LOPART(receiverGuid)), sender);

            CharacterDatabase.CommitTransaction(trans);

            std::string nameLink = handler->playerLink(receiverName);
            handler->PSendSysMessage(LANG_MAIL_SENT, nameLink.c_str());
            return true;
        }
        /// Send a message to a player in game
        static bool HandleSendMessageCommand(ChatHandler* handler, char const* args)
        {
            /// - Find the player
            Player* player;
            if (!handler->extractPlayerTarget((char*)args, &player))
                return false;

            char* msgStr = strtok(NULL, "");
            if (!msgStr)
                return false;

            ///- Check that he is not logging out.
            if (player->GetSession()->isLogingOut())
            {
                handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // Confirmation message
            std::string nameLink = handler->GetNameLink(player);
            handler->PSendSysMessage(LANG_SENDMESSAGE, nameLink.c_str(), msgStr);
            return true;
        }

        static bool HandleCreatePetCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();
            Creature* creatureTarget = handler->getSelectedCreature();

            if (!creatureTarget || creatureTarget->isPet() || creatureTarget->IsPlayer())
            {
                handler->PSendSysMessage(LANG_SELECT_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            CreatureTemplate const* creatrueTemplate = sObjectMgr->GetCreatureTemplate(creatureTarget->GetEntry());
            // Creatures with family 0 crashes the server
            if (!creatrueTemplate->family)
            {
                handler->PSendSysMessage("This creature cannot be tamed. (family id: 0).");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetPetGUID())
            {
                handler->PSendSysMessage("You already have a pet");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // Everything looks OK, create new pet
            Pet* l_Pet = player->CreateTamedPetFrom(creatureTarget);
            if (!l_Pet)
            {
                handler->PSendSysMessage("Error 1");
                return false;
            }

            creatureTarget->setDeathState(JUST_DIED);
            creatureTarget->RemoveCorpse();
            creatureTarget->SetHealth(0); // just for nice GM-mode view

            // prepare visual effect for levelup
            l_Pet->SetUInt32Value(UNIT_FIELD_LEVEL, creatureTarget->getLevel() - 1);

            l_Pet->GetMap()->AddToMap(l_Pet->ToCreature());

            // visual effect for levelup
            l_Pet->SetUInt32Value(UNIT_FIELD_LEVEL, creatureTarget->getLevel());

            // caster have pet now
            player->SetMinion(l_Pet, true, player->getSlotForNewPet(), l_Pet->m_Stampeded);

            l_Pet->SavePetToDB(PET_SLOT_ACTUAL_PET_SLOT, l_Pet->m_Stampeded);
            player->PetSpellInitialize();
            player->GetSession()->SendStablePet(0);
            return true;
        }

        static bool HandlePetLearnCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            Player* player = handler->GetSession()->GetPlayer();
            Pet* pet = player->GetPet();

            if (!pet)
            {
                handler->PSendSysMessage("You have no pet");
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 spellId = handler->extractSpellIdFromLink((char*)args);

            if (!spellId || !sSpellMgr->GetSpellInfo(spellId))
                return false;

            // Check if pet already has it
            if (pet->HasSpell(spellId))
            {
                handler->PSendSysMessage("Pet already has spell: %u", spellId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // Check if spell is valid
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
            if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo))
            {
                handler->PSendSysMessage(LANG_COMMAND_SPELL_BROKEN, spellId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            pet->learnSpell(spellId);

            handler->PSendSysMessage("Pet has learned spell %u", spellId);
            return true;
        }

        static bool HandlePetUnlearnCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            Player* player = handler->GetSession()->GetPlayer();
            Pet* pet = player->GetPet();
            if (!pet)
            {
                handler->PSendSysMessage("You have no pet");
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 spellId = handler->extractSpellIdFromLink((char*)args);

            if (pet->HasSpell(spellId))
                pet->removeSpell(spellId, false);
            else
                handler->PSendSysMessage("Pet doesn't have that spell");

            return true;
        }

        static bool HandleFreezeCommand(ChatHandler* handler, char const* args)
        {
            std::string name;
            Player* player;
            char const* TargetName = strtok((char*)args, " "); // get entered name
            if (!TargetName) // if no name entered use target
            {
                player = handler->getSelectedPlayer();
                if (player) //prevent crash with creature as target
                {
                    name = player->GetName();
                    normalizePlayerName(name);
                }
            }
            else // if name entered
            {
                name = TargetName;
                normalizePlayerName(name);
                player = sObjectAccessor->FindPlayerByName(name.c_str());
            }

            if (!player)
            {
                handler->SendSysMessage(LANG_COMMAND_FREEZE_WRONG);
                return true;
            }

            if (player == handler->GetSession()->GetPlayer())
            {
                handler->SendSysMessage(LANG_COMMAND_FREEZE_ERROR);
                return true;
            }

            // effect
            if (player && (player != handler->GetSession()->GetPlayer()))
            {
                handler->PSendSysMessage(LANG_COMMAND_FREEZE, name.c_str());

                // stop combat + make player unattackable + duel stop + stop some spells
                player->setFaction(35);
                player->CombatStop();
                if (player->IsNonMeleeSpellCasted(true))
                    player->InterruptNonMeleeSpells(true);
                player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                // if player class = hunter || warlock remove pet if alive
                if ((player->getClass() == CLASS_HUNTER) || (player->getClass() == CLASS_WARLOCK))
                {
                    if (Pet* pet = player->GetPet())
                    {
                        pet->SavePetToDB(PET_SLOT_ACTUAL_PET_SLOT, pet->m_Stampeded);
                        // not let dismiss dead pet
                        if (pet && pet->isAlive())
                            player->RemovePet(pet, PET_SLOT_HUNTER_FIRST, false, pet->m_Stampeded);
                    }
                }

                if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(9454))
                    Aura::TryRefreshStackOrCreate(spellInfo, MAX_EFFECT_MASK, player, player);

                // save player
                player->SaveToDB();
            }

            return true;
        }

        static bool HandleUnFreezeCommand(ChatHandler* handler, char const*args)
        {
            std::string name;
            Player* player;
            char* targetName = strtok((char*)args, " "); // Get entered name

            if (targetName)
            {
                name = targetName;
                normalizePlayerName(name);
                player = sObjectAccessor->FindPlayerByName(name.c_str());
            }
            else // If no name was entered - use target
            {
                player = handler->getSelectedPlayer();
                if (player)
                    name = player->GetName();
            }

            if (player)
            {
                handler->PSendSysMessage(LANG_COMMAND_UNFREEZE, name.c_str());

                // Reset player faction + allow combat + allow duels
                player->setFactionForRace(player->getRace());
                player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                // Remove Freeze spell (allowing movement and spells)
                player->RemoveAurasDueToSpell(9454);

                // Save player
                player->SaveToDB();
            }
            else
            {
                if (targetName)
                {
                    // Check for offline players
                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_GUID_BY_NAME);
                    stmt->setString(0, name);
                    PreparedQueryResult result = CharacterDatabase.Query(stmt);

                    if (!result)
                    {
                        handler->SendSysMessage(LANG_COMMAND_FREEZE_WRONG);
                        return true;
                    }

                    // If player found: delete his freeze aura
                    Field* fields = result->Fetch();
                    uint32 lowGuid = fields[0].GetUInt32();

                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_AURA_FROZEN);
                    stmt->setUInt32(0, lowGuid);
                    CharacterDatabase.Execute(stmt);

                    handler->PSendSysMessage(LANG_COMMAND_UNFREEZE, name.c_str());
                    return true;
                }
                else
                {
                    handler->SendSysMessage(LANG_COMMAND_FREEZE_WRONG);
                    return true;
                }
            }

            return true;
        }

        static bool HandleListFreezeCommand(ChatHandler* handler, char const* /*args*/)
        {
            // Get names from DB
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURA_FROZEN);
            PreparedQueryResult result = CharacterDatabase.Query(stmt);
            if (!result)
            {
                handler->SendSysMessage(LANG_COMMAND_NO_FROZEN_PLAYERS);
                return true;
            }

            // Header of the names
            handler->PSendSysMessage(LANG_COMMAND_LIST_FREEZE);

            // Output of the results
            do
            {
                Field* fields = result->Fetch();
                std::string player = fields[0].GetString();
                handler->PSendSysMessage(LANG_COMMAND_FROZEN_PLAYERS, player.c_str());
            } while (result->NextRow());

            return true;
        }

        static bool HandleGroupLeaderCommand(ChatHandler* handler, char const* args)
        {
            Player* player = NULL;
            Group* group = NULL;
            uint64 guid = 0;
            char* nameStr = strtok((char*)args, " ");

            if (handler->GetPlayerGroupAndGUIDByName(nameStr, player, group, guid))
            if (group && group->GetLeaderGUID() != guid)
            {
                group->ChangeLeader(guid);
                group->SendUpdate();
            }

            return true;
        }

        static bool HandleGroupDisbandCommand(ChatHandler* handler, char const* args)
        {
            Player* player = NULL;
            Group* group = NULL;
            uint64 guid = 0;
            char* nameStr = strtok((char*)args, " ");

            if (handler->GetPlayerGroupAndGUIDByName(nameStr, player, group, guid))
            if (group)
                group->Disband();

            return true;
        }

        static bool HandleGroupRemoveCommand(ChatHandler* handler, char const* args)
        {
            Player* player = NULL;
            Group* group = NULL;
            uint64 guid = 0;
            char* nameStr = strtok((char*)args, " ");

            if (handler->GetPlayerGroupAndGUIDByName(nameStr, player, group, guid, true))
            if (group)
                group->RemoveMember(guid);

            return true;
        }

        static bool HandlePlayAllCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            uint32 soundId = atoi((char*)args);

            if (!sSoundEntriesStore.LookupEntry(soundId))
            {
                handler->PSendSysMessage(LANG_SOUND_NOT_EXIST, soundId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            WorldPacket data(SMSG_PLAY_SOUND, 2 + 16 + 8);
            ObjectGuid guid = handler->GetSession()->GetPlayer()->GetGUID();
            data << uint32(soundId);
            data.appendPackGUID(guid);
            sWorld->SendGlobalMessage(&data);

            handler->PSendSysMessage(LANG_COMMAND_PLAYED_TO_ALL, soundId);
            return true;
        }

        static bool HandlePossessCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
                return false;

            handler->GetSession()->GetPlayer()->CastSpell(unit, 530, true);
            return true;
        }

        static bool HandleUnPossessCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
                unit = handler->GetSession()->GetPlayer();

            unit->RemoveCharmAuras();

            return true;
        }

        static bool HandleBindSightCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
                return false;

            handler->GetSession()->GetPlayer()->CastSpell(unit, 6277, true);
            return true;
        }

        static bool HandleUnbindSightCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();

            if (player->isPossessing())
                return false;

            player->StopCastingBindSight();
            return true;
        }

        static bool HandleSelectFactionCommand(ChatHandler* handler, char const* /*args*/)
        {
            handler->GetSession()->GetPlayer()->ShowNeutralPlayerFactionSelectUI();
            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_misc_commandscript()
{
    new misc_commandscript();
}
#endif