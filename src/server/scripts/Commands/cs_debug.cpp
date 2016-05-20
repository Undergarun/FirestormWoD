////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: debug_commandscript
%Complete: 100
Comment: All debug related commands
Category: commandscripts
EndScriptData */

#include "Common.h"
#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.hpp"
#include "Chat.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "GossipDef.h"
#include "MapManager.h"
#include "DisableMgr.h"
#include "Group.h"
#include "LFGMgr.h"
#include "World.h"

#include <fstream>
#include "BattlegroundPacketFactory.hpp"

struct UnitStates
{
    UnitState   Flag;
    char const* Text;
};

std::vector<UnitStates> const g_Unitstates =
{
    { UNIT_STATE_DIED,              "UNIT_STATE_DIED"                },
    { UNIT_STATE_MELEE_ATTACKING,   "UNIT_STATE_MELEE_ATTACKING"     },
    { UNIT_STATE_STUNNED,           "UNIT_STATE_STUNNED"             },
    { UNIT_STATE_ROAMING,           "UNIT_STATE_ROAMING"             },
    { UNIT_STATE_CHASE,             "UNIT_STATE_CHASE"               },
    { UNIT_STATE_FLEEING,           "UNIT_STATE_FLEEING"             },
    { UNIT_STATE_IN_FLIGHT,         "UNIT_STATE_IN_FLIGHT"           },
    { UNIT_STATE_FOLLOW,            "UNIT_STATE_FOLLOW"              },
    { UNIT_STATE_ROOT,              "UNIT_STATE_ROOT"                },
    { UNIT_STATE_CONFUSED,          "UNIT_STATE_CONFUSED"            },
    { UNIT_STATE_DISTRACTED,        "UNIT_STATE_DISTRACTED"          },
    { UNIT_STATE_ISOLATED,          "UNIT_STATE_ISOLATED"            },
    { UNIT_STATE_ATTACK_PLAYER,     "UNIT_STATE_ATTACK_PLAYER"       },
    { UNIT_STATE_CASTING,           "UNIT_STATE_CASTING"             },
    { UNIT_STATE_POSSESSED,         "UNIT_STATE_POSSESSED"           },
    { UNIT_STATE_CHARGING,          "UNIT_STATE_CHARGING"            },
    { UNIT_STATE_JUMPING,           "UNIT_STATE_JUMPING"             },
    { UNIT_STATE_ONVEHICLE,         "UNIT_STATE_ONVEHICLE"           },
    { UNIT_STATE_MOVE,              "UNIT_STATE_MOVE"                },
    { UNIT_STATE_ROTATING,          "UNIT_STATE_ROTATING"            },
    { UNIT_STATE_EVADE,             "UNIT_STATE_EVADE"               },
    { UNIT_STATE_ROAMING_MOVE,      "UNIT_STATE_ROAMING_MOVE"        },
    { UNIT_STATE_CONFUSED_MOVE,     "UNIT_STATE_CONFUSED_MOVE"       },
    { UNIT_STATE_FLEEING_MOVE,      "UNIT_STATE_FLEEING_MOVE"        },
    { UNIT_STATE_CHASE_MOVE,        "UNIT_STATE_CHASE_MOVE"          },
    { UNIT_STATE_FOLLOW_MOVE,       "UNIT_STATE_FOLLOW_MOVE"         },
    { UNIT_STATE_UNATTACKABLE,      "UNIT_STATE_UNATTACKABLE"        },
    { UNIT_STATE_MOVING,            "UNIT_STATE_MOVING"              },
    { UNIT_STATE_CONTROLLED,        "UNIT_STATE_CONTROLLED"          },
    { UNIT_STATE_LOST_CONTROL,      "UNIT_STATE_LOST_CONTROL"        },
    { UNIT_STATE_SIGHTLESS,         "UNIT_STATE_SIGHTLESS"           },
    { UNIT_STATE_CANNOT_AUTOATTACK, "UNIT_STATE_CANNOT_AUTOATTACK"   },
    { UNIT_STATE_CANNOT_TURN,       "UNIT_STATE_CANNOT_TURN"         },
    { UNIT_STATE_NOT_MOVE,          "UNIT_STATE_NOT_MOVE"            },
    { UNIT_STATE_ALL_STATE,         "UNIT_STATE_ALL_STATE"           }
};

class debug_commandscript: public CommandScript
{
    public:

        debug_commandscript() : CommandScript("debug_commandscript")
        {
        }

        ChatCommand* GetCommands() const
        {
            static ChatCommand debugPlayCommandTable[] =
            {
                { "cinematic",      SEC_MODERATOR,      false, &HandleDebugPlayCinematicCommand,   "", NULL },
                { "movie",          SEC_MODERATOR,      false, &HandleDebugPlayMovieCommand,       "", NULL },
                { "sound",          SEC_MODERATOR,      false, &HandleDebugPlaySoundCommand,       "", NULL },
                { "scene",          SEC_ADMINISTRATOR,  false, &HandleDebugPlaySceneCommand,       "", NULL },
                { "sscene",         SEC_ADMINISTRATOR,  false, &HandleDebugPlaySSceneCommand,      "", NULL },
                { "oneshotanimkit", SEC_ADMINISTRATOR,  false, &HandleDebugPlayOneShotAnimKit,     "", NULL },
                { "spellvisualkit", SEC_ADMINISTRATOR,  false, &HandleDebugPlaySpellVisualKit,     "", NULL },
                { "orphanvisual",   SEC_ADMINISTRATOR,  false, &HandleDebugPlayOrphanSpellVisual,  "", NULL },
                { "visual",         SEC_ADMINISTRATOR,  false, &HandleDebugPlayVisual,             "", NULL },
                { NULL,             SEC_PLAYER,         false, NULL,                               "", NULL }
            };
            static ChatCommand debugSendCommandTable[] =
            {
                { "buyerror",       SEC_ADMINISTRATOR,  false, &HandleDebugSendBuyErrorCommand,       "", NULL },
                { "channelnotify",  SEC_ADMINISTRATOR,  false, &HandleDebugSendChannelNotifyCommand,  "", NULL },
                { "chatmmessage",   SEC_ADMINISTRATOR,  false, &HandleDebugSendChatMsgCommand,        "", NULL },
                { "equiperror",     SEC_ADMINISTRATOR,  false, &HandleDebugSendEquipErrorCommand,     "", NULL },
                { "largepacket",    SEC_ADMINISTRATOR,  false, &HandleDebugSendLargePacketCommand,    "", NULL },
                { "opcode",         SEC_ADMINISTRATOR,  false, &HandleDebugSendOpcodeCommand,         "", NULL },
                { "qpartymsg",      SEC_ADMINISTRATOR,  false, &HandleDebugSendQuestPartyMsgCommand,  "", NULL },
                { "qinvalidmsg",    SEC_ADMINISTRATOR,  false, &HandleDebugSendQuestInvalidMsgCommand,"", NULL },
                { "sellerror",      SEC_ADMINISTRATOR,  false, &HandleDebugSendSellErrorCommand,      "", NULL },
                { "setphaseshift",  SEC_ADMINISTRATOR,  false, &HandleDebugSendSetPhaseShiftCommand,  "", NULL },
                { "spellfail",      SEC_ADMINISTRATOR,  false, &HandleDebugSendSpellFailCommand,      "", NULL },
                { NULL,             SEC_PLAYER,         false, NULL,                                  "", NULL }
            };
            static ChatCommand debugCommandTable[] =
            {
                { "setbit",                      SEC_ADMINISTRATOR,  false, &HandleDebugSet32BitCommand,             "", NULL },
                { "threat",                      SEC_ADMINISTRATOR,  false, &HandleDebugThreatListCommand,           "", NULL },
                { "hostil",                      SEC_ADMINISTRATOR,  false, &HandleDebugHostileRefListCommand,       "", NULL },
                { "anim",                        SEC_GAMEMASTER,     false, &HandleDebugAnimCommand,                 "", NULL },
                { "arena",                       SEC_ADMINISTRATOR,  false, &HandleDebugArenaCommand,                "", NULL },
                { "bg",                          SEC_ADMINISTRATOR,  false, &HandleDebugBattlegroundCommand,         "", NULL },
                { "getitemstate",                SEC_ADMINISTRATOR,  false, &HandleDebugGetItemStateCommand,         "", NULL },
                { "lootrecipient",               SEC_GAMEMASTER,     false, &HandleDebugGetLootRecipientCommand,     "", NULL },
                { "getvalue",                    SEC_ADMINISTRATOR,  false, &HandleDebugGetValueCommand,             "", NULL },
                { "getitemvalue",                SEC_ADMINISTRATOR,  false, &HandleDebugGetItemValueCommand,         "", NULL },
                { "Mod32Value",                  SEC_ADMINISTRATOR,  false, &HandleDebugMod32ValueCommand,           "", NULL },
                { "play",                        SEC_MODERATOR,      false, NULL,                                    "", debugPlayCommandTable },
                { "send",                        SEC_ADMINISTRATOR,  false, NULL,                                    "", debugSendCommandTable },
                { "setaurastate",                SEC_ADMINISTRATOR,  false, &HandleDebugSetAuraStateCommand,         "", NULL },
                { "setitemvalue",                SEC_ADMINISTRATOR,  false, &HandleDebugSetItemValueCommand,         "", NULL },
                { "setvalue",                    SEC_ADMINISTRATOR,  false, &HandleDebugSetValueCommand,             "", NULL },
                { "spawnvehicle",                SEC_ADMINISTRATOR,  false, &HandleDebugSpawnVehicleCommand,         "", NULL },
                { "setvid",                      SEC_ADMINISTRATOR,  false, &HandleDebugSetVehicleIdCommand,         "", NULL },
                { "entervehicle",                SEC_ADMINISTRATOR,  false, &HandleDebugEnterVehicleCommand,         "", NULL },
                { "uws",                         SEC_ADMINISTRATOR,  false, &HandleDebugUpdateWorldStateCommand,     "", NULL },
                { "update",                      SEC_ADMINISTRATOR,  false, &HandleDebugUpdateCommand,               "", NULL },
                { "itemexpire",                  SEC_ADMINISTRATOR,  false, &HandleDebugItemExpireCommand,           "", NULL },
                { "areatriggers",                SEC_ADMINISTRATOR,  false, &HandleDebugAreaTriggersCommand,         "", NULL },
                { "los",                         SEC_MODERATOR,      false, &HandleDebugLoSCommand,                  "", NULL },
                { "moveflags",                   SEC_ADMINISTRATOR,  false, &HandleDebugMoveflagsCommand,            "", NULL },
                { "phase",                       SEC_MODERATOR,      false, &HandleDebugPhaseCommand,                "", NULL },
                { "tradestatus",                 SEC_ADMINISTRATOR,  false, &HandleSendTradeStatus,                  "", NULL },
                { "mailstatus",                  SEC_ADMINISTRATOR,  false, &HandleSendMailStatus,                   "", NULL },
                { "jump",                        SEC_ADMINISTRATOR,  false, &HandleDebugMoveJump,                    "", NULL },
                { "backward",                    SEC_ADMINISTRATOR,  false, &HandleDebugMoveBackward,                "", NULL },
                { "load_z",                      SEC_ADMINISTRATOR,  false, &HandleDebugLoadZ,                       "", NULL },
                { "joinratedbg",                 SEC_ADMINISTRATOR,  false, &HandleJoinRatedBg,                      "", NULL },
                { "SetMaxMapDiff",               SEC_ADMINISTRATOR,  false, &HandleDebugSetMaxMapDiff,               "", NULL },
                { "packet",                      SEC_ADMINISTRATOR,  false, &HandleDebugPacketCommand,               "", NULL },
                { "guildevent",                  SEC_ADMINISTRATOR,  false, &HandleDebugGuildEventCommand,           "", NULL },
                { "log",                         SEC_ADMINISTRATOR,  false, &HandleDebugLogCommand,                  "", NULL },
                { "movement",                    SEC_ADMINISTRATOR,  false, &HandleDebugMoveCommand,                 "", NULL },
                { "boss",                        SEC_ADMINISTRATOR,  false, &HandleDebugBossCommand,                 "", NULL },
                { "lfg",                         SEC_ADMINISTRATOR,  false, &HandleDebugLfgCommand,                  "", NULL },
                { "scaleitem",                   SEC_ADMINISTRATOR,  true,  &HandleDebugScaleItem,                   "", NULL },
                { "toy",                         SEC_ADMINISTRATOR,  false, &HandleDebugToyCommand,                  "", NULL },
                { "charge",                      SEC_ADMINISTRATOR,  false, &HandleDebugClearSpellCharges,           "", NULL },
                { "bgstart",                     SEC_ADMINISTRATOR,  false, &HandleDebugBattlegroundStart,           "", NULL },
                { "criteria",                    SEC_ADMINISTRATOR,  false, &HandleDebugCriteriaCommand,             "", NULL },
                { "crashtest",                   SEC_ADMINISTRATOR,  true,  &HandleDebugCrashTest,                   "", NULL },
                { "bgaward",                     SEC_ADMINISTRATOR,  false, &HandleDebugBgAward,                     "", NULL },
                { "heirloom",                    SEC_ADMINISTRATOR,  false, &HandleDebugHeirloom,                    "", NULL },
                { "vignette",                    SEC_ADMINISTRATOR,  false, &HandleDebugVignette,                    "", NULL },
                { "setaianimkit",                SEC_ADMINISTRATOR,  false, &HandleDebugSetAIAnimKit,                "", NULL },
                { "dumpchartemplate",            SEC_CONSOLE,        true,  &HandleDebugDumpCharTemplate,            "", NULL },
                { "dumprewardlessmissions",      SEC_CONSOLE,        true,  &HandleDebugDumpRewardlessMissions,      "", NULL },
                { "dumpspellrewardlessmissions", SEC_CONSOLE,        true,  &HandleSpellDebugDumpRewardlessMissions, "", NULL },
                { "playercondition",             SEC_ADMINISTRATOR,  false, &HandleDebugPlayerCondition,             "", NULL },
                { "packetprofiler",              SEC_ADMINISTRATOR,  false, &HandleDebugPacketProfiler,              "", NULL },
                { "hotfix",                      SEC_ADMINISTRATOR,  false, &HandleHotfixOverride,                   "", NULL },
                { "adjustspline",                SEC_ADMINISTRATOR,  false, &HandleDebugAdjustSplineCommand,         "", NULL },
                { "splinesync",                  SEC_ADMINISTRATOR,  false, &HandleDebugSplineSyncCommand,           "", NULL },
                { "mirror",                      SEC_ADMINISTRATOR,  false, &HandleDebugMirrorCommand,               "", NULL },
                { "pvelogs",                     SEC_ADMINISTRATOR,  false, &HandleDebugPvELogsCommand,              "", NULL },
                { "questlog",                    SEC_ADMINISTRATOR,  false, &HandleDebugQuestLogsCommand,            "", NULL },
                { "addunitstate",                SEC_ADMINISTRATOR,  false, &HandleDebugAddUnitStateCommand,         "", NULL },
                { "getunitstate",                SEC_ADMINISTRATOR,  false, &HandleDebugGetUnitStatesCommand,        "", NULL },
                { "removeunitstate",             SEC_ADMINISTRATOR,  false, &HandleDebugRemoveUnitStateCommand,      "", NULL },
                { "stresstest",                  SEC_ADMINISTRATOR,  false, &HandleDebugStressTestCommand,           "", NULL },
                { "showequiperror",              SEC_ADMINISTRATOR,  false, &HandleDebugShowEquipErrorCommand,       "", NULL },
                { "critical",                    SEC_ADMINISTRATOR,  false, &HandleDebugCriticalCommand,             "", NULL },
                { "haste",                       SEC_ADMINISTRATOR,  false, &HandleDebugHasteCommand,                "", NULL },
                { "mastery",                     SEC_ADMINISTRATOR,  false, &HandleDebugMasteryCommand,              "", NULL },
                { "multistrike",                 SEC_ADMINISTRATOR,  false, &HandleDebugMultistrikeCommand,          "", NULL },
                { "setaura",                     SEC_ADMINISTRATOR,  false, &HandleDebugAuraCommand,                 "", NULL },
                { "cleardr",                     SEC_ADMINISTRATOR,  false, &HandleDebugCancelDiminishingReturn,     "", NULL },
                { NULL,                          SEC_PLAYER,         false, NULL,                                    "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "debug",          SEC_MODERATOR,      true,  NULL,                  "", debugCommandTable },
                { "wpgps",          SEC_ADMINISTRATOR,  false, &HandleWPGPSCommand,                "", NULL },
                { NULL,             SEC_PLAYER,         false, NULL,                  "",              NULL }
            };
            return commandTable;
        }

        static bool HandleDebugCancelDiminishingReturn(ChatHandler* handler, char const* args)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            unit->ClearDiminishings();
            return true;
        }



        static bool HandleDebugAuraCommand(ChatHandler* handler, char const* args)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 spellId = handler->extractSpellIdFromLink((char*)args);

            if (Aura* aura = unit->AddAura(spellId, unit))
                aura->SetDuration(HOUR * IN_MILLISECONDS);
            else
            {
                handler->PSendSysMessage("Failed to add aura %d", spellId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            return true;
        }

        static bool HandleDebugCriticalCommand(ChatHandler* handler, char const* args)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int pct = atoi(args);
            if (pct == 0)
                pct = 100;

            if (Aura* perfectAim = unit->AddAura(138963, unit))
            {
                perfectAim->SetDuration(HOUR * IN_MILLISECONDS);
                perfectAim->GetEffect(EFFECT_0)->ChangeAmount(pct);
            }

            return true;
        }

        static bool HandleDebugHasteCommand(ChatHandler* handler, char const* args)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int pct = atoi(args);
            if (pct == 0)
                pct = 100;

            if (Aura* rapidCast = unit->AddAura(8215, unit))
            {
                rapidCast->SetDuration(HOUR * IN_MILLISECONDS);
                rapidCast->GetEffect(EFFECT_0)->ChangeAmount(pct);
            }

            return true;
        }

        static bool HandleDebugMultistrikeCommand(ChatHandler* handler, char const* args)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int pct = atoi(args);
            if (pct == 0)
                pct = 100;

            if (Aura* perfectAim = unit->AddAura(167732, unit))
            {
                perfectAim->SetDuration(HOUR * IN_MILLISECONDS);
                perfectAim->GetEffect(EFFECT_0)->ChangeAmount(pct);
            }

            return true;
        }

        static bool HandleDebugMasteryCommand(ChatHandler* handler, char const* args)
        {
            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int pct = atoi(args);
            if (pct == 0)
                pct = 50000;

            if (Aura* perfectAim = unit->AddAura(113861, unit))
            {
                perfectAim->SetDuration(HOUR * IN_MILLISECONDS);
                perfectAim->GetEffect(EFFECT_0)->ChangeAmount(pct);
            }

            return true;
        }

        static bool HandleDebugShowEquipErrorCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
                return false;

            uint32 l_ErrorID = (uint32)atoi((char*)p_Args);

            Player* l_Target = p_Handler->getSelectedPlayer();
            if (!l_Target)
            {
                p_Handler->SendSysMessage(LANG_SELECT_CREATURE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            l_Target->SendEquipError((InventoryResult)l_ErrorID, nullptr, nullptr);
            return true;
        }

        static bool HandleDebugStressTestCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            std::string l_StrVal = std::string(strtok((char*)p_Args, " "));

            /// Disconnect all bots
            if (l_StrVal == "off")
            {
                SessionMap const& l_Sessions = sWorld->GetAllSessions();
                for (auto l_Session : l_Sessions)
                {
                    if (l_Session.second->IsStressTest())
                        l_Session.second->SetStressTest(false);
                }
                return true;
            }

            /// Add new bots
            if (l_StrVal == "on")
            {
                char* l_StrNumber = strtok(NULL, " ");
                if (l_StrNumber == nullptr)
                    return false;

                uint32 l_Number = atoi(l_StrNumber);

                if (l_Number > 1000)
                    return false;

                std::ostringstream l_Query;
                l_Query << "SELECT account, guid FROM characters WHERE account NOT IN(0";

                SessionMap const& l_Sessions = sWorld->GetAllSessions();
                for (auto l_Session : l_Sessions)
                    l_Query << "," << l_Session.first;

                l_Query << ") GROUP BY account ORDER BY RAND() LIMIT " << l_Number;

                QueryResult l_Result = CharacterDatabase.PQuery(l_Query.str().c_str());

                if (l_Result)
                {
                    do
                    {
                        Field* l_Fields = l_Result->Fetch();

                        uint32 l_AccountId = l_Fields[0].GetUInt32();
                        uint32 l_Guid      = l_Fields[1].GetUInt32();

                        WorldSession* l_NewSession = new WorldSession(l_AccountId, nullptr, SEC_ADMINISTRATOR, false, 0, 5, 0, LOCALE_frFR, 0, false, 0, 0, 0);

                        l_NewSession->SetStressTest(true);
                        l_NewSession->LoadGlobalAccountData();
                        l_NewSession->LoadTutorialsData();
                        sWorld->AddSession(l_NewSession);

                        l_NewSession->LoginPlayer(l_Guid);
                    }
                    while (l_Result->NextRow());
                }
            }

            return true;
        }

        static bool HandleDebugAdjustSplineCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrVal = strtok((char*)p_Args, " ");
            float l_Value = atof(l_StrVal);

            if (Unit* l_Target = p_Handler->getSelectedUnit())
            {
                l_Target->SendAdjustSplineDuration(l_Value);
                return true;
            }
            else
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }
        }

        static bool HandleDebugSplineSyncCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrVal = strtok((char*)p_Args, " ");
            float l_Value = atof(l_StrVal);

            if (Unit* l_Target = p_Handler->getSelectedUnit())
            {
                l_Target->SendFlightSplineSync(l_Value);
                return true;
            }
            else
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }
        }

        static bool HandleDebugCriteriaCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrType = strtok((char*)p_Args, " ");
            uint32 l_Type = uint32(atoi(l_StrType));

            char* l_StrMisc1 = strtok(NULL, " ");
            uint32 l_Misc1 = l_StrMisc1 ? uint32(atoi(l_StrMisc1)) : 0;

            char* l_StrMisc2 = strtok(NULL, " ");
            uint32 l_Misc2 = l_StrMisc2 ? uint32(atoi(l_StrMisc2)) : 0;

            p_Handler->GetSession()->GetPlayer()->UpdateAchievementCriteria(AchievementCriteriaTypes(l_Type), l_Misc1, l_Misc2);
            return true;
        }

        static bool HandleDebugClearSpellCharges(ChatHandler* handler, char const* args)
        {
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);

            if (SpellCategoryEntry const* l_Category = sSpellCategoryStore.LookupEntry(id))
            {
                if (Player* l_Player = handler->GetSession()->GetPlayer())
                    l_Player->ResetCharges(l_Category);

                return true;
            }
            else
            {
                handler->PSendSysMessage("Spell %u doesn't exist !", id);
                handler->SetSentErrorMessage(true);
                return false;
            }
        }

        static bool HandleDebugToyCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrID = strtok(NULL, " ");
            uint32 l_ID = l_StrID ? uint32(atoi(l_StrID)) : 0;

            p_Handler->GetSession()->GetPlayer()->AddDynamicValue(PLAYER_DYNAMIC_FIELD_TOYS, l_ID);
            return true;
        }

        static bool HandleDebugBossCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
                return false;

            char* l_String = p_Handler->extractQuotedArg((char*)p_Args);
            if (!l_String)
                return false;

            char const* l_Text = l_String;//"||TInterface\\Icons\\trade_archaeology_whitehydrafigurine:20||tMegaera begins to ||cFFF00000||Hspell:139458||h[Rampage]||h||r�!";
            char const* l_Name = "Megaera";
            char const* l_Receiver = "Venomous head";

            WorldPacket data;
            uint64 l_Target = p_Handler->GetSession()->GetPlayer()->GetGUID();
            uint32 l_TextLen = l_Text ? strlen(l_Text) : 0;
            uint32 l_SpeakerNameLen = l_Name ? strlen(l_Name) + 1 : 0;
            uint32 l_PrefixLen = 0;
            uint32 l_ReceiverLen = strlen(l_Receiver);
            uint32 l_ChannelLen = 0;
            std::string l_Channel = ""; // no channel
            uint8 l_ChatTag = 0x20;

            ObjectGuid l_Sender = MAKE_NEW_GUID(322021, 0, HIGHGUID_UNIT);
            ObjectGuid l_Group = 0;
            ObjectGuid l_ReceiverGuid = l_Target;
            ObjectGuid l_Guild = 0;

            bool l_UnkBit = false;
            bool l_UnkBit5256 = false;
            bool l_UnkBit5264 = false;
            bool l_UnkBit5269 = false;
            bool l_UnkBit5268 = false;
            bool l_HasPrefix = false;
            bool l_HasLanguage = false;
            bool l_HasSender = true;
            bool l_HasText = l_TextLen != 0;
            bool l_HasGroup = false;
            bool l_HasReceiver = l_Target != 0;
            bool l_HasChatTag = true;
            bool l_HasGuild = false;
            bool l_HasChannel = false;

            data.Initialize(SMSG_CHAT, 200);
            data.WriteBit(l_UnkBit5269);
            data.WriteBit(!l_HasText);
            data.WriteBit(!l_UnkBit5256);
            data.WriteBit(!l_HasSender);
            data.WriteBit(l_HasSender);

            uint8 bitsOrder[8] = { 2, 4, 0, 6, 1, 3, 5, 7 };
            data.WriteBitInOrder(l_Sender, bitsOrder);

            data.WriteBit(l_HasGroup);

            uint8 bitsOrder2[8] = { 6, 0, 4, 1, 2, 3, 7, 5 };
            data.WriteBitInOrder(l_Group, bitsOrder2);

            data.WriteBit(!l_HasPrefix);
            data.WriteBit(l_UnkBit5268);
            data.WriteBit(!l_UnkBit);
            data.WriteBit(!l_UnkBit5264);

            data.WriteBits(l_SpeakerNameLen, 11);

            data.WriteBit(l_HasReceiver);

            uint8 bitsOrder3[8] = { 4, 0, 6, 7, 5, 1, 3, 2 };
            data.WriteBitInOrder(l_ReceiverGuid, bitsOrder3);

            // Never happens for creature
            if (l_PrefixLen)
                data.WriteBits(l_PrefixLen, 5);

            data.WriteBit(!l_HasReceiver);
            data.WriteBit(!l_HasChatTag);

            if (l_TextLen)
                data.WriteBits(l_TextLen, 12);

            data.WriteBit(!l_HasLanguage);
            data.WriteBits(l_ChatTag, 9);
            data.WriteBit(l_HasGuild);

            if (l_HasReceiver)
                data.WriteBits(l_ReceiverLen, 11);

            uint8 bitsOrder4[8] = { 0, 2, 1, 4, 6, 7, 5, 3 };
            data.WriteBitInOrder(l_Guild, bitsOrder4);

            data.WriteBit(!l_HasChannel);
            data.WriteBits(l_ChannelLen, 7);

            if (l_HasChannel)
                data.WriteString(l_Channel);

            if (l_HasSender)
                data.WriteString(l_Name);

            uint8 byteOrder[8] = { 6, 7, 1, 2, 4, 3, 0, 5 };
            data.WriteBytesSeq(l_Group, byteOrder);

            uint8 byteOrder1[8] = { 0, 4, 1, 3, 5, 7, 2, 6 };
            data.WriteBytesSeq(l_ReceiverGuid, byteOrder1);

            data << uint8(CHAT_MSG_RAID_BOSS_EMOTE);

            uint8 byteOrder2[8] = { 7, 6, 5, 4, 0, 2, 1, 3 };
            data.WriteBytesSeq(l_Sender, byteOrder2);

            // Never happens for creatures
            if (l_HasPrefix)
                data.WriteString("");

            if (l_UnkBit)
                data << uint32(0);                                         // unk uint32

            uint8 byteOrder3[8] = { 1, 0, 3, 7, 6, 5, 2, 4 };
            data.WriteBytesSeq(l_Guild, byteOrder3);

            if (l_HasReceiver)
                data.WriteString(l_Receiver);

            if (l_UnkBit5256)
                data << uint32(0);                                         // unk uint32

            if (l_HasLanguage)
                data << uint8(LANG_UNIVERSAL);

            if (l_HasText)
                data.WriteString(l_Text);

            if (l_UnkBit5264)
                data << uint32(0);                                         // unk uint32

            p_Handler->GetSession()->GetPlayer()->SendMessageToSetInRange(&data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), true);
            return true;
        }

        static bool HandleDebugPlaySceneCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);

            if (!sSceneScriptPackageStore.LookupEntry(id))
            {
                handler->PSendSysMessage("Scene %u doesnt exist !", id);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->PSendSysMessage("Start playing scene %u - %s !", id, sSceneScriptPackageStore.LookupEntry(id)->Name);
            handler->GetSession()->GetPlayer()->PlayScene(id, handler->GetSession()->GetPlayer());
            return true;
        }

        static bool HandleDebugPlaySSceneCommand(ChatHandler* p_Handler, char const* args)
        {
            if (!*args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);

            if (!sSceneScriptPackageStore.LookupEntry(id))
            {
                p_Handler->PSendSysMessage("Scene %u doesnt exist !", id);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            Player * l_Player = p_Handler->GetSession()->GetPlayer();

            Position l_Location;
            l_Location.m_positionX = l_Player->m_positionX;
            l_Location.m_positionY = l_Player->m_positionY;
            l_Location.m_positionZ = l_Player->m_positionZ;
            l_Location.m_orientation = l_Player->m_orientation;

            p_Handler->PSendSysMessage("Start playing standalone scene %u - %s !", id, sSceneScriptPackageStore.LookupEntry(id)->Name);
            p_Handler->GetSession()->GetPlayer()->PlayStandaloneScene(id, 16, l_Location);

            return true;
        }

        static bool HandleDebugPlayOneShotAnimKit(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 l_ID = atoi((char*)p_Args);
            if (!l_ID)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Handler->getSelectedUnit())
                p_Handler->getSelectedUnit()->PlayOneShotAnimKit(l_ID);
            else
                p_Handler->GetSession()->GetPlayer()->PlayOneShotAnimKit(l_ID);

            return true;
        }

        static bool HandleDebugSetAIAnimKit(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 l_ID = atoi((char*)p_Args);
            if (!l_ID)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Handler->getSelectedUnit())
                p_Handler->getSelectedUnit()->SetAIAnimKitId(l_ID);
            else
                p_Handler->GetSession()->GetPlayer()->SetAIAnimKitId(l_ID);

            return true;
        }

        static bool HandleDebugPlaySpellVisualKit(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrID = strtok((char*)p_Args, " ");
            char* l_StrType = strtok(NULL, " ");

            if (!l_StrID || !l_StrType)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 l_ID = (uint32)atoi(l_StrID);
            uint32 l_Type = (uint32)atoi(l_StrType);
            if (!l_ID || !l_Type)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Handler->getSelectedUnit())
                p_Handler->getSelectedUnit()->SendPlaySpellVisualKit(l_ID, l_Type);
            else
                p_Handler->GetSession()->GetPlayer()->SendPlaySpellVisualKit(l_ID, l_Type);

            return true;
        }

        static bool HandleDebugPlayOrphanSpellVisual(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrID = strtok((char*)p_Args, " ");
            char* l_StrSpeed = strtok(NULL, " ");

            if (!l_StrID || !l_StrSpeed)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 l_ID = (uint32)atoi(l_StrID);
            float l_Speed = (float)atof(l_StrSpeed);
            if (!l_ID)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            Player* l_Player = p_Handler->GetSession()->GetPlayer();
            if (l_Player == nullptr)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            Unit* l_Target = l_Player->GetSelectedUnit();
            if (l_Target == nullptr)
                l_Target = l_Player;

            WorldPacket l_Data(Opcodes::SMSG_PLAY_ORPHAN_SPELL_VISUAL, 100);

            G3D::Vector3 l_Source(l_Player->m_positionX, l_Player->m_positionY, l_Player->m_positionZ);
            G3D::Vector3 l_Dest(l_Target->m_positionX, l_Target->m_positionY, l_Target->m_positionZ);
            G3D::Vector3 l_Orientation (0.0f, 0.0f, 0.0f);

            l_Data.WriteVector3(l_Source);
            l_Data.WriteVector3(l_Orientation);
            l_Data.WriteVector3(l_Dest);
            l_Data.appendPackGUID(0);

            l_Data << int32(l_ID);
            l_Data << float(l_Speed);
            l_Data << float(0.0f);

            l_Data.WriteBit(true);
            l_Data.FlushBits();

            p_Handler->GetSession()->SendPacket(&l_Data);
            return true;
        }

        static bool HandleDebugPlayVisual(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_StrID = strtok((char*)p_Args, " ");
            char* l_StrSpeed = strtok(NULL, " ");

            if (!l_StrID || !l_StrSpeed)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 l_ID = (uint32)atoi(l_StrID);
            float l_Speed = (float)atof(l_StrSpeed);
            if (!l_ID)
            {
                p_Handler->SendSysMessage(LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (Unit* l_Target = p_Handler->getSelectedUnit())
            {
                p_Handler->GetSession()->GetPlayer()->SendPlaySpellVisual(l_ID, l_Target, l_Speed, 0.0f, Position());
            }
            else
                p_Handler->GetSession()->GetPlayer()->SendPlaySpellVisual(l_ID, p_Handler->GetSession()->GetPlayer(), l_Speed, 0.0f, Position());

            return true;
        }

        static bool HandleJoinRatedBg(ChatHandler* handler, char const* /*args*/)
        {
            // ignore if we already in BG or BG queue
            if (handler->GetSession()->GetPlayer()->InBattleground())
                return false;

            uint32 personalRating = 0;
            uint32 matchmakerRating = 0;

            //check existance
            Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(MS::Battlegrounds::BattlegroundType::RatedBg10v10);
            if (!bg)
            {
                sLog->outError(LOG_FILTER_NETWORKIO, "Battleground: template bg (10 vs 10) not found");
                return false;
            }

            if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_RATED_10_VS_10, NULL))
                return false;

            BattlegroundTypeId bgTypeId = bg->GetTypeID();
            MS::Battlegrounds::BattlegroundType::Type bgQueueTypeId = MS::Battlegrounds::GetTypeFromId(bgTypeId, 0);

            MS::Battlegrounds::Bracket const* bracketEntry = MS::Battlegrounds::Brackets::FindForLevel(handler->GetSession()->GetPlayer()->getLevel());
            if (!bracketEntry)
                return false;

            GroupJoinBattlegroundResult err = ERR_BATTLEGROUND_NONE;

            Group* grp = handler->GetSession()->GetPlayer()->GetGroup();

            // no group found, error
            if (!grp)
                return false;

            if (grp->GetLeaderGUID() != handler->GetSession()->GetPlayer()->GetGUID())
                return false;

            uint32 playerDivider = 0;
            for (GroupReference const* ref = grp->GetFirstMember(); ref != NULL; ref = ref->next())
            {
                if (Player const* groupMember = ref->getSource())
                {
                    personalRating += groupMember->GetArenaPersonalRating(SLOT_RBG);
                    matchmakerRating += groupMember->GetArenaMatchMakerRating(SLOT_RBG);

                    ++playerDivider;
                }
            }

            if (!playerDivider)
                return false;

            personalRating /= playerDivider;
            matchmakerRating /= playerDivider;

            if (personalRating <= 0)
                personalRating = 1;
            if (matchmakerRating <= 0)
                matchmakerRating = 1;

            MS::Battlegrounds::BattlegroundScheduler& l_Scheduler = sBattlegroundMgr->GetScheduler();
            MS::Battlegrounds::BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();

            uint32 avgTime = 0;
            GroupQueueInfo* ginfo;

            err = grp->CanJoinBattlegroundQueue(bg, bgQueueTypeId, 2);
            if (!err)
            {
                sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: leader %s queued", handler->GetSession()->GetPlayer()->GetName());

                ginfo = l_Scheduler.AddGroup(handler->GetSession()->GetPlayer(), grp, bgQueueTypeId, nullptr, bracketEntry, ArenaType::None, true, personalRating, matchmakerRating, false);
                avgTime = l_InvitationsMgr.GetAverageQueueWaitTime(ginfo, bracketEntry->m_Id);
            }

            for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->getSource();
                if (!member)
                    continue;

                if (err)
                {
                    WorldPacket data;
                    MS::Battlegrounds::PacketFactory::StatusFailed(&data, bg, handler->GetSession()->GetPlayer(), 0, err);
                    member->GetSession()->SendPacket(&data);
                    continue;
                }

                 // add to queue
                uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

                // add joined time data
                member->AddBattlegroundQueueJoinTime(bgQueueTypeId, ginfo->m_JoinTime);

                WorldPacket data; // send status packet (in queue)
                MS::Battlegrounds::PacketFactory::Status(&data, bg, member, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->m_JoinTime, ginfo->m_ArenaType, false);
                member->GetSession()->SendPacket(&data);

                sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for rated battleground as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
            }

            //sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, 0, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

            return true;
        }

        static bool HandleDebugMoveCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* result = strtok((char*)args, " ");
            if (!result)
                return false;

            bool apply = bool(atoi(result));

            char* param = strtok(NULL, " ");
            if (!param)
                return false;

            Position pos;
            pos.m_positionX = float(atoi(param));

            param = strtok(NULL, " ");
            if (!param)
                return false;

            pos.m_positionY = float(atoi(param));

            param = strtok(NULL, " ");
            if (!param)
                return false;

            pos.m_positionZ = float(atoi(param));

            param = strtok(NULL, " ");
            if (!param)
                return false;

            float force = float(atoi(param));

            uint64 l_FakeGuid = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_AREATRIGGER), 6452, HIGHGUID_AREATRIGGER);
            handler->GetSession()->GetPlayer()->SendApplyMovementForce(l_FakeGuid, apply, pos, force);

            return true;
        }

        static bool HandleDebugLogCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* result = strtok((char*)args, " ");
            if (!result)
                return false;

            uint32 opcodeId  = uint32(atoi(result));
            if (opcodeId == 0 && *result != '0')
                return false;

            WorldPacket data(Opcodes(opcodeId), 10);
            ObjectGuid playerGuid = handler->GetSession()->GetPlayer()->GetGUID();
            Player* player = handler->GetSession()->GetPlayer();

            switch (opcodeId)
            {
                case 1440:
                {
                    std::string name = player->GetName();

                    data << uint64(playerGuid);
                    data.WriteBit(true);

                    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                        data.WriteBits(0, 7);

                    data.WriteBits(name.size(), 8);

                    data << uint32(3117);
                    data.append(name.c_str(), name.size());

                    break;
                }
                /// @todo Update me ?
                case SMSG_SERVER_FIRST_ACHIEVEMENT:
                {
                    data.WriteBits(1, 21);

                    uint8 bits[8] = { 7, 5, 0, 3, 6, 2, 1, 4 };
                    data.WriteBitInOrder(playerGuid, bits);

                    data.WriteByteSeq(playerGuid[6]);
                    data.WriteByteSeq(playerGuid[3]);
                    data << uint32(3117); // Faucheur de la Mort "Prem's" du royaume
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[0]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[7]);
                    data.WriteByteSeq(playerGuid[5]);
                    break;
                }
                case 4101:
                {
                    data.WriteBit(false);   // Inversed, Unk
                    data.WriteBit(false);   // Inversed, Unk
                    data.WriteBit(false);   // Inversed, has UInt32 - bit8

                    uint8 bits[8] = { 7, 4, 5, 3, 0, 2, 6, 1 };
                    data.WriteBitInOrder(playerGuid, bits);

                    uint8 bits2[8] = { 6, 4, 2, 0, 5, 7, 3, 1 };
                    data.WriteBitInOrder(playerGuid, bits2);

                    data.WriteBit(false);   // Inversed, Unk
                    data.WriteBit(false);   // Inversed, Unk

                    data << uint32(100784);

                    uint8 bytes[8] = { 1, 3, 4, 7, 5, 0, 6, 2 };
                    data.WriteBytesSeq(playerGuid, bytes);

                    data << float(1.0f);    // Unk

                    uint8 bytes2[8] = { 3, 5, 7, 2, 4, 6, 0, 1 };
                    data.WriteBytesSeq(playerGuid, bytes2);

                    data << float(1.0f);    // Unk
                    break;
                }
                case 4385:
                {
                    uint8 bits[8] = { 2, 5, 0, 1, 3, 6, 4, 7 };
                    data.WriteBitInOrder(playerGuid, bits);

                    data.WriteByteSeq(playerGuid[0]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[6]);
                    data << uint8(2);
                    data.WriteByteSeq(playerGuid[3]);
                    data << uint32(100784);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[7]);
                    break;
                }
                case 4501:
                {
                    // Display channel bar
                    data.WriteBit(false);
                    data.WriteBit(true);    // Unk bit56

                    uint8 bits[8] = { 6, 0, 4, 1, 2, 3, 5, 7 };
                    data.WriteBitInOrder(playerGuid, bits);

                    data << uint32(100784);
                    data << uint32(100784);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[6]);
                    data.WriteByteSeq(playerGuid[4]);
                    data << uint32(100784);
                    data.WriteByteSeq(playerGuid[7]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[3]);
                    data << uint32(100784);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[0]);
                    break;
                }
                case 4518:
                {
                    // Show cast bar
                    data.WriteBit(playerGuid[2]);
                    data.WriteBit(playerGuid[1]);
                    data.WriteBit(true);    // Unk bit40
                    data.WriteBit(playerGuid[1]);
                    data.WriteBit(playerGuid[4]);
                    data.WriteBit(playerGuid[0]);
                    data.WriteBit(playerGuid[3]);
                    data.WriteBit(playerGuid[2]);
                    data.WriteBit(playerGuid[3]);
                    data.WriteBit(playerGuid[5]);
                    data.WriteBit(playerGuid[7]);
                    data.WriteBit(playerGuid[6]);
                    data.WriteBit(playerGuid[0]);
                    data.WriteBit(playerGuid[6]);
                    data.WriteBit(playerGuid[5]);
                    data.WriteBit(playerGuid[4]);
                    data.WriteBit(playerGuid[7]);

                    data.WriteByteSeq(playerGuid[0]);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[6]);
                    data << uint32(100784);
                    data << uint32(100784);
                    data << uint32(100784);
                    data << uint32(100784);
                    data.WriteByteSeq(playerGuid[0]);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[3]);
                    data.WriteByteSeq(playerGuid[7]);
                    data.WriteByteSeq(playerGuid[3]);
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[7]);
                    data << uint32(100784);
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[6]);
                    break;
                }
                default:
                    break;
            }

            handler->GetSession()->SendPacket(&data, true);

            return true;
        }

        static bool HandleDebugGuildEventCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* result = strtok((char*)args, " ");
            if (!result)
                return false;

            uint32 opcodeId = uint32(atoi(result));
            if (opcodeId == 0 && *result != '0')
                return false;

            switch (opcodeId)
            {
                case 0x20:
                {
                    ///@todo update me ?
                    // La migration de votre guilde est termin�e. Rendez-vous sur [cette page Internet] pour de plus amples informations.
                    Player* player = handler->GetSession()->GetPlayer();
                    WorldPacket data(Opcodes(1346), 20);
                    ObjectGuid playerGuid = player->GetGUID();

                    data.WriteBit(playerGuid[3]);
                    data.WriteBit(playerGuid[2]);
                    data.WriteBit(playerGuid[6]);
                    data.WriteBit(playerGuid[1]);
                    data.WriteBits(strlen(player->GetName()), 8);
                    data.WriteBit(playerGuid[5]);
                    data.WriteBit(playerGuid[7]);
                    data.WriteBit(playerGuid[0]);
                    data.WriteBit(playerGuid[4]);

                    data.WriteByteSeq(playerGuid[0]);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[3]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[6]);
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[7]);
                    if (strlen(player->GetName()))
                        data.append(player->GetName(), strlen(player->GetName()));

                    player->GetSession()->SendPacket(&data, true);

                    break;
                }
                case 0x2F:
                {
                    // ??
                    Player* player = handler->GetSession()->GetPlayer();
                    WorldPacket data(Opcodes(1375), 20);
                    ObjectGuid playerGuid = player->GetGUID();

                    data.WriteBit(playerGuid[3]);
                    data.WriteBit(playerGuid[1]);
                    data.WriteBit(playerGuid[0]);
                    data.WriteBit(playerGuid[5]);
                    data.WriteBits(strlen(player->GetName()), 8);
                    data.WriteBit(playerGuid[2]);
                    data.WriteBit(playerGuid[4]);
                    data.WriteBit(playerGuid[7]);
                    data.WriteBit(1);               // Unk
                    data.WriteBit(playerGuid[6]);

                    data.WriteByteSeq(playerGuid[3]);
                    data.WriteByteSeq(playerGuid[1]);
                    if (strlen(player->GetName()))
                        data.append(player->GetName(), strlen(player->GetName()));
                    data.WriteByteSeq(playerGuid[6]);
                    data.WriteByteSeq(playerGuid[7]);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[2]);
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[0]);

                    player->GetSession()->SendPacket(&data, true);

                    break;
                }
                case 0x55:
                {
                    // ??
                    Player* player = handler->GetSession()->GetPlayer();
                    WorldPacket data(Opcodes(3179), 20);
                    ObjectGuid playerGuid = player->GetGUID();

                    data.WriteBit(playerGuid[4]);
                    data.WriteBit(playerGuid[0]);
                    data.WriteBit(playerGuid[7]);
                    data.WriteBit(playerGuid[3]);
                    data.WriteBits(strlen(player->GetName()), 7);
                    data.WriteBit(playerGuid[6]);
                    data.WriteBit(playerGuid[5]);
                    data.WriteBit(playerGuid[1]);
                    data.WriteBit(playerGuid[2]);

                    data.WriteByteSeq(playerGuid[0]);
                    data.WriteByteSeq(playerGuid[4]);
                    data.WriteByteSeq(playerGuid[5]);
                    data.WriteByteSeq(playerGuid[1]);
                    data.WriteByteSeq(playerGuid[7]);
                    data.WriteByteSeq(playerGuid[2]);
                    if (strlen(player->GetName()))
                        data.append(player->GetName(), strlen(player->GetName()));
                    data.WriteByteSeq(playerGuid[3]);
                    data.WriteByteSeq(playerGuid[6]);

                    player->GetSession()->SendPacket(&data, true);

                    break;
                }
                default:
                    break;
            }

            return true;
        }

        static bool HandleDebugPacketCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* result = strtok((char*)args, " ");
            if (!result)
                return false;

            uint32 opcodeId  = uint32(atoi(result));
            if (opcodeId == 0 && *result != '0')
                return false;

            char* value = strtok(NULL, " ");
            uint32 sendValue = value ? uint32(atoi(value)) : 0;

            WorldPacket data(Opcodes(opcodeId), 4);
            data << uint32(sendValue);
            handler->GetSession()->SendPacket(&data, true);

            return true;
        }

        static bool HandleSendTradeStatus(ChatHandler* handler, char const* args)
        {
            // USAGE: .debug play cinematic #cinematicid
            // #cinematicid - ID decimal number from CinemaicSequences.dbc (1st column)
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);

            handler->GetSession()->SendTradeStatus(TradeStatus(id));
            return true;
        }

        static bool HandleSendMailStatus(ChatHandler* handler, char const* args)
        {
            // USAGE: .debug play cinematic #cinematicid
            // #cinematicid - ID decimal number from CinemaicSequences.dbc (1st column)
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);
            uint32 id2 = atoi((char*)args);

            handler->GetSession()->GetPlayer()->SendMailResult(0, MailResponseType(id), MailResponseResult(id2));
            return true;
        }

        static bool HandleDebugPlayCinematicCommand(ChatHandler* handler, char const* args)
        {
            // USAGE: .debug play cinematic #cinematicid
            // #cinematicid - ID decimal number from CinemaicSequences.dbc (1st column)
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);

            if (!sCinematicSequencesStore.LookupEntry(id))
            {
                handler->PSendSysMessage(LANG_CINEMATIC_NOT_EXIST, id);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->GetSession()->GetPlayer()->SendCinematicStart(id);
            return true;
        }

        static bool HandleDebugPlayMovieCommand(ChatHandler* handler, char const* args)
        {
            // USAGE: .debug play movie #movieid
            // #movieid - ID decimal number from Movie.dbc (1st column)
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 id = atoi((char*)args);

            if (!sMovieStore.LookupEntry(id))
            {
                handler->PSendSysMessage(LANG_MOVIE_NOT_EXIST, id);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->GetSession()->GetPlayer()->SendMovieStart(id);
            return true;
        }

        //Play sound
        static bool HandleDebugPlaySoundCommand(ChatHandler* handler, char const* args)
        {
            // USAGE: .debug playsound #soundid
            // #soundid - ID decimal number from SoundEntries.dbc (1st column)
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 soundId = atoi((char*)args);

            if (!sSoundEntriesStore.LookupEntry(soundId))
            {
                handler->PSendSysMessage(LANG_SOUND_NOT_EXIST, soundId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (handler->GetSession()->GetPlayer()->GetSelection())
                unit->PlayDistanceSound(unit, soundId, handler->GetSession()->GetPlayer());
            else
                unit->PlayDirectSound(soundId, handler->GetSession()->GetPlayer());

            handler->PSendSysMessage(LANG_YOU_HEAR_SOUND, soundId);
            return true;
        }

        static bool HandleDebugSendSpellFailCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* result = strtok((char*)args, " ");
            if (!result)
                return false;

            uint32 failNum = atoi(result);
            if (failNum == 0 && *result != '0')
                return false;

            char* fail1 = strtok(NULL, " ");
            uint32 failArg1 = fail1 ? atoi(fail1) : 0;

            char* fail2 = strtok(NULL, " ");
            uint32 failArg2 = fail2 ? atoi(fail2) : 0;

            WorldPacket data(SMSG_CAST_FAILED, 5);

            data << uint32(133);        ///< SpellID
            data << uint32(failNum);    ///< Problem

            if (fail1 || fail2)
                data << uint32(failArg1);
            if (fail2)
                data << uint32(failArg2);

            data << uint8(0);           ///< CastCount

            handler->GetSession()->SendPacket(&data);

            return true;
        }

        static bool HandleDebugSendEquipErrorCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            InventoryResult msg = InventoryResult(atoi(args));
            handler->GetSession()->GetPlayer()->SendEquipError(msg, NULL, NULL);
            return true;
        }

        static bool HandleDebugSendSellErrorCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            SellResult msg = SellResult(atoi(args));
            handler->GetSession()->GetPlayer()->SendSellError(msg, 0, 0);
            return true;
        }

        static bool HandleDebugSendBuyErrorCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            BuyResult msg = BuyResult(atoi(args));
            handler->GetSession()->GetPlayer()->SendBuyError(msg, 0, 0, 0);
            return true;
        }

        static bool HandleDebugSendOpcodeCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* unit = handler->getSelectedUnit();
            Player* player = NULL;
            if (!unit || (unit->GetTypeId() != TYPEID_PLAYER))
                player = handler->GetSession()->GetPlayer();
            else
                player = (Player*)unit;

            if (!unit)
                unit = player;

            std::ifstream ifs("opcode.txt");
            if (ifs.bad())
                return false;

            // remove comments from file
            std::stringstream parsedStream;
            while (!ifs.eof())
            {
                char commentToken[2];
                ifs.get(commentToken[0]);
                if (commentToken[0] == '/' && !ifs.eof())
                {
                    ifs.get(commentToken[1]);
                    // /* comment
                    if (commentToken[1] == '*')
                    {
                        while (!ifs.eof())
                        {
                            ifs.get(commentToken[0]);
                            if (commentToken[0] == '*' && !ifs.eof())
                            {
                                ifs.get(commentToken[1]);
                                if (commentToken[1] == '/')
                                    break;
                                else
                                    ifs.putback(commentToken[1]);
                            }
                        }
                        continue;
                    }
                    // line comment
                    else if (commentToken[1] == '/')
                    {
                        std::string str;
                        getline(ifs, str);
                        continue;
                    }
                    // regular data
                    else
                        ifs.putback(commentToken[1]);
                }
                parsedStream.put(commentToken[0]);
            }
            ifs.close();

            uint32 opcode;
            parsedStream >> opcode;

            WorldPacket data(Opcodes(opcode), 0);

            while (!parsedStream.eof())
            {
                std::string type;
                parsedStream >> type;

                if (type == "")
                    break;

                if (type == "uint8")
                {
                    uint16 val1;
                    parsedStream >> val1;
                    data << uint8(val1);
                }
                else if (type == "uint16")
                {
                    uint16 val2;
                    parsedStream >> val2;
                    data << val2;
                }
                else if (type == "uint32")
                {
                    uint32 val3;
                    parsedStream >> val3;
                    data << val3;
                }
                else if (type == "uint64")
                {
                    uint64 val4;
                    parsedStream >> val4;
                    data << val4;
                }
                else if (type == "float")
                {
                    float val5;
                    parsedStream >> val5;
                    data << val5;
                }
                else if (type == "string")
                {
                    std::string val6;
                    parsedStream >> val6;
                    data << val6;
                }
                else if (type == "appitsguid")
                {
                    data.append(unit->GetPackGUID());
                }
                else if (type == "appmyguid")
                {
                    data.append(player->GetPackGUID());
                }
                else if (type == "appgoguid")
                {
                    GameObject* obj = handler->GetNearbyGameObject();
                    if (!obj)
                    {
                        handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, 0);
                        handler->SetSentErrorMessage(true);
                        ifs.close();
                        return false;
                    }
                    data.append(obj->GetPackGUID());
                }
                else if (type == "goguid")
                {
                    GameObject* obj = handler->GetNearbyGameObject();
                    if (!obj)
                    {
                        handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, 0);
                        handler->SetSentErrorMessage(true);
                        ifs.close();
                        return false;
                    }
                    data << uint64(obj->GetGUID());
                }
                else if (type == "myguid")
                {
                    data << uint64(player->GetGUID());
                }
                else if (type == "itsguid")
                {
                    data << uint64(unit->GetGUID());
                }
                else if (type == "itspos")
                {
                    data << unit->GetPositionX();
                    data << unit->GetPositionY();
                    data << unit->GetPositionZ();
                }
                else if (type == "mypos")
                {
                    data << player->GetPositionX();
                    data << player->GetPositionY();
                    data << player->GetPositionZ();
                }
                else
                {
                    sLog->outError(LOG_FILTER_GENERAL, "Sending opcode that has unknown type '%s'", type.c_str());
                    break;
                }
            }
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Sending opcode %u", data.GetOpcode());
            data.hexlike();
            player->GetSession()->SendPacket(&data, true);
            handler->PSendSysMessage(LANG_COMMAND_OPCODESENT, data.GetOpcode(), unit->GetName());
            return true;
        }

        static bool HandleDebugUpdateWorldStateCommand(ChatHandler* handler, char const* args)
        {
            char* w = strtok((char*)args, " ");
            char* s = strtok(NULL, " ");

            if (!w || !s)
                return false;

            uint32 world = (uint32)atoi(w);
            uint32 state = (uint32)atoi(s);
            handler->GetSession()->GetPlayer()->SendUpdateWorldState(world, state);
            return true;
        }

        static bool HandleDebugAreaTriggersCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();
            if (!player->isDebugAreaTriggers)
            {
                handler->PSendSysMessage(LANG_DEBUG_AREATRIGGER_ON);
                player->isDebugAreaTriggers = true;
            }
            else
            {
                handler->PSendSysMessage(LANG_DEBUG_AREATRIGGER_OFF);
                player->isDebugAreaTriggers = false;
            }
            return true;
        }

        //Send notification in channel
        static bool HandleDebugSendChannelNotifyCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char const* name = "test";
            uint8 code = atoi(args);

            WorldPacket data(SMSG_CHANNEL_NOTIFY, (1+10));
            data << code;                                           // notify type
            data << name;                                           // channel name
            data << uint32(0);
            data << uint32(0);
            handler->GetSession()->SendPacket(&data);
            return true;
        }

        //Send notification in chat
        static bool HandleDebugSendChatMsgCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char const* msg = "testtest";
            uint8 type = atoi(args);
            WorldPacket data;
            ChatHandler::FillMessageData(&data, handler->GetSession(), type, 0, "chan", handler->GetSession()->GetPlayer()->GetGUID(), msg, handler->GetSession()->GetPlayer());
            handler->GetSession()->SendPacket(&data);
            return true;
        }

        static bool HandleDebugSendQuestPartyMsgCommand(ChatHandler* handler, char const* args)
        {
            uint32 msg = atol((char*)args);
            handler->GetSession()->GetPlayer()->SendPushToPartyResponse(handler->GetSession()->GetPlayer(), msg);
            return true;
        }

        static bool HandleDebugGetLootRecipientCommand(ChatHandler* handler, char const* /*args*/)
        {
            Creature* target = handler->getSelectedCreature();
            if (!target)
                return false;

            handler->PSendSysMessage("Loot recipient for creature %s (GUID %u, DB GUID %u) is %s", target->GetName(), target->GetGUIDLow(), target->GetDBTableGUIDLow(), target->hasLootRecipient() ? (target->GetLootRecipient() ? target->GetLootRecipient()->GetName() : "offline") : "no loot recipient");
            return true;
        }

        static bool HandleDebugSendQuestInvalidMsgCommand(ChatHandler* handler, char const* args)
        {
            uint32 msg = atol((char*)args);
            handler->GetSession()->GetPlayer()->SendCanTakeQuestResponse(msg);
            return true;
        }

        static bool HandleDebugGetItemStateCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            std::string itemState = args;

            ItemUpdateState state = ITEM_UNCHANGED;
            bool listQueue = false;
            bool checkAll = false;

            if (itemState == "unchanged")
                state = ITEM_UNCHANGED;
            else if (itemState == "changed")
                state = ITEM_CHANGED;
            else if (itemState == "new")
                state = ITEM_NEW;
            else if (itemState == "removed")
                state = ITEM_REMOVED;
            else if (itemState == "queue")
                listQueue = true;
            else if (itemState == "check_all")
                checkAll = true;
            else
                return false;

            Player* player = handler->getSelectedPlayer();
            if (!player)
                player = handler->GetSession()->GetPlayer();

            if (!listQueue && !checkAll)
            {
                itemState = "The player has the following " + itemState + " items: ";
                handler->SendSysMessage(itemState.c_str());
                for (uint8 i = PLAYER_SLOT_START; i < PLAYER_SLOT_END; ++i)
                {
                    if (i >= BUYBACK_SLOT_START && i < BUYBACK_SLOT_END)
                        continue;

                    if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                    {
                        if (Bag* bag = item->ToBag())
                        {
                            for (uint8 j = 0; j < bag->GetBagSize(); ++j)
                                if (Item* item2 = bag->GetItemByPos(j))
                                    if (item2->GetState() == state)
                                        handler->PSendSysMessage("bag: 255 slot: %d guid: %d owner: %d", item2->GetSlot(), item2->GetGUIDLow(), GUID_LOPART(item2->GetOwnerGUID()));
                        }
                        else if (item->GetState() == state)
                            handler->PSendSysMessage("bag: 255 slot: %d guid: %d owner: %d", item->GetSlot(), item->GetGUIDLow(), GUID_LOPART(item->GetOwnerGUID()));
                    }
                }
            }

            if (listQueue)
            {
                std::vector<Item*>& updateQueue = player->GetItemUpdateQueue();
                for (size_t i = 0; i < updateQueue.size(); ++i)
                {
                    Item* item = updateQueue[i];
                    if (!item)
                        continue;

                    Bag* container = item->GetContainer();
                    uint8 bagSlot = container ? container->GetSlot() : uint8(INVENTORY_SLOT_BAG_0);

                    std::string st;
                    switch (item->GetState())
                    {
                        case ITEM_UNCHANGED:
                            st = "unchanged";
                            break;
                        case ITEM_CHANGED:
                            st = "changed";
                            break;
                        case ITEM_NEW:
                            st = "new";
                            break;
                        case ITEM_REMOVED:
                            st = "removed";
                            break;
                    }

                    handler->PSendSysMessage("bag: %d slot: %d guid: %d - state: %s", bagSlot, item->GetSlot(), item->GetGUIDLow(), st.c_str());
                }
                if (updateQueue.empty())
                    handler->PSendSysMessage("The player's updatequeue is empty");
            }

            if (checkAll)
            {
                bool error = false;
                std::vector<Item*>& updateQueue = player->GetItemUpdateQueue();
                for (uint8 i = PLAYER_SLOT_START; i < PLAYER_SLOT_END; ++i)
                {
                    if (i >= BUYBACK_SLOT_START && i < BUYBACK_SLOT_END)
                        continue;

                    Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
                    if (!item)
                        continue;

                    if (item->GetSlot() != i)
                    {
                        handler->PSendSysMessage("Item with slot %d and guid %d has an incorrect slot value: %d", i, item->GetGUIDLow(), item->GetSlot());
                        error = true;
                        continue;
                    }

                    if (item->GetOwnerGUID() != player->GetGUID())
                    {
                        handler->PSendSysMessage("The item with slot %d and itemguid %d does have non-matching owner guid (%d) and player guid (%d) !", item->GetSlot(), item->GetGUIDLow(), GUID_LOPART(item->GetOwnerGUID()), player->GetGUIDLow());
                        error = true;
                        continue;
                    }

                    if (Bag* container = item->GetContainer())
                    {
                        handler->PSendSysMessage("The item with slot %d and guid %d has a container (slot: %d, guid: %d) but shouldn't!", item->GetSlot(), item->GetGUIDLow(), container->GetSlot(), container->GetGUIDLow());
                        error = true;
                        continue;
                    }

                    if (item->IsInUpdateQueue())
                    {
                        uint16 qp = item->GetQueuePos();
                        if (qp > updateQueue.size())
                        {
                            handler->PSendSysMessage("The item with slot %d and guid %d has its queuepos (%d) larger than the update queue size! ", item->GetSlot(), item->GetGUIDLow(), qp);
                            error = true;
                            continue;
                        }

                        if (updateQueue[qp] == NULL)
                        {
                            handler->PSendSysMessage("The item with slot %d and guid %d has its queuepos (%d) pointing to NULL in the queue!", item->GetSlot(), item->GetGUIDLow(), qp);
                            error = true;
                            continue;
                        }

                        if (updateQueue[qp] != item)
                        {
                            handler->PSendSysMessage("The item with slot %d and guid %d has a queuepos (%d) that points to another item in the queue (bag: %d, slot: %d, guid: %d)", item->GetSlot(), item->GetGUIDLow(), qp, updateQueue[qp]->GetBagSlot(), updateQueue[qp]->GetSlot(), updateQueue[qp]->GetGUIDLow());
                            error = true;
                            continue;
                        }
                    }
                    else if (item->GetState() != ITEM_UNCHANGED)
                    {
                        handler->PSendSysMessage("The item with slot %d and guid %d is not in queue but should be (state: %d)!", item->GetSlot(), item->GetGUIDLow(), item->GetState());
                        error = true;
                        continue;
                    }

                    if (Bag* bag = item->ToBag())
                    {
                        for (uint8 j = 0; j < bag->GetBagSize(); ++j)
                        {
                            Item* item2 = bag->GetItemByPos(j);
                            if (!item2)
                                continue;

                            if (item2->GetSlot() != j)
                            {
                                handler->PSendSysMessage("The item in bag %d and slot %d (guid: %d) has an incorrect slot value: %d", bag->GetSlot(), j, item2->GetGUIDLow(), item2->GetSlot());
                                error = true;
                                continue;
                            }

                            if (item2->GetOwnerGUID() != player->GetGUID())
                            {
                                handler->PSendSysMessage("The item in bag %d at slot %d and with itemguid %d, the owner's guid (%d) and the player's guid (%d) don't match!", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow(), GUID_LOPART(item2->GetOwnerGUID()), player->GetGUIDLow());
                                error = true;
                                continue;
                            }

                            Bag* container = item2->GetContainer();
                            if (!container)
                            {
                                handler->PSendSysMessage("The item in bag %d at slot %d with guid %d has no container!", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow());
                                error = true;
                                continue;
                            }

                            if (container != bag)
                            {
                                handler->PSendSysMessage("The item in bag %d at slot %d with guid %d has a different container(slot %d guid %d)!", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow(), container->GetSlot(), container->GetGUIDLow());
                                error = true;
                                continue;
                            }

                            if (item2->IsInUpdateQueue())
                            {
                                uint16 qp = item2->GetQueuePos();
                                if (qp > updateQueue.size())
                                {
                                    handler->PSendSysMessage("The item in bag %d at slot %d having guid %d has a queuepos (%d) larger than the update queue size! ", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow(), qp);
                                    error = true;
                                    continue;
                                }

                                if (updateQueue[qp] == NULL)
                                {
                                    handler->PSendSysMessage("The item in bag %d at slot %d having guid %d has a queuepos (%d) that points to NULL in the queue!", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow(), qp);
                                    error = true;
                                    continue;
                                }

                                if (updateQueue[qp] != item2)
                                {
                                    handler->PSendSysMessage("The item in bag %d at slot %d having guid %d has a queuepos (%d) that points to another item in the queue (bag: %d, slot: %d, guid: %d)", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow(), qp, updateQueue[qp]->GetBagSlot(), updateQueue[qp]->GetSlot(), updateQueue[qp]->GetGUIDLow());
                                    error = true;
                                    continue;
                                }
                            }
                            else if (item2->GetState() != ITEM_UNCHANGED)
                            {
                                handler->PSendSysMessage("The item in bag %d at slot %d having guid %d is not in queue but should be (state: %d)!", bag->GetSlot(), item2->GetSlot(), item2->GetGUIDLow(), item2->GetState());
                                error = true;
                                continue;
                            }
                        }
                    }
                }

                for (size_t i = 0; i < updateQueue.size(); ++i)
                {
                    Item* item = updateQueue[i];
                    if (!item)
                        continue;

                    if (item->GetOwnerGUID() != player->GetGUID())
                    {
                        handler->PSendSysMessage("queue(" SIZEFMTD "): For the item with guid %d, the owner's guid (%d) and the player's guid (%d) don't match!", i, item->GetGUIDLow(), GUID_LOPART(item->GetOwnerGUID()), player->GetGUIDLow());
                        error = true;
                        continue;
                    }

                    if (item->GetQueuePos() != i)
                    {
                        handler->PSendSysMessage("queue(" SIZEFMTD "): For the item with guid %d, the queuepos doesn't match it's position in the queue!", i, item->GetGUIDLow());
                        error = true;
                        continue;
                    }

                    if (item->GetState() == ITEM_REMOVED)
                        continue;

                    Item* test = player->GetItemByPos(item->GetBagSlot(), item->GetSlot());

                    if (test == NULL)
                    {
                        handler->PSendSysMessage("queue(" SIZEFMTD "): The bag(%d) and slot(%d) values for the item with guid %d are incorrect, the player doesn't have any item at that position!", i, item->GetBagSlot(), item->GetSlot(), item->GetGUIDLow());
                        error = true;
                        continue;
                    }

                    if (test != item)
                    {
                        handler->PSendSysMessage("queue(" SIZEFMTD "): The bag(%d) and slot(%d) values for the item with guid %d are incorrect, an item which guid is %d is there instead!", i, item->GetBagSlot(), item->GetSlot(), item->GetGUIDLow(), test->GetGUIDLow());
                        error = true;
                        continue;
                    }
                }
                if (!error)
                    handler->SendSysMessage("All OK!");
            }

            return true;
        }

        static bool HandleDebugBattlegroundCommand(ChatHandler* /*handler*/, char const* /*args*/)
        {
            sBattlegroundMgr->ToggleTesting();
            return true;
        }

        static bool HandleDebugArenaCommand(ChatHandler* /*handler*/, char const* /*args*/)
        {
            sBattlegroundMgr->ToggleArenaTesting();
            return true;
        }

        static bool HandleDebugThreatListCommand(ChatHandler* handler, char const* /*args*/)
        {
            Creature* target = handler->getSelectedCreature();
            if (!target || target->isTotem() || target->isPet())
                return false;

            std::list<HostileReference*>& threatList = target->getThreatManager().getThreatList();
            std::list<HostileReference*>::iterator itr;
            uint32 count = 0;
            handler->PSendSysMessage("Threat list of %s (guid %u)", target->GetName(), target->GetGUIDLow());
            for (itr = threatList.begin(); itr != threatList.end(); ++itr)
            {
                Unit* unit = (*itr)->getTarget();
                if (!unit)
                    continue;
                ++count;
                handler->PSendSysMessage("   %u.   %s   (guid %u)  - threat %f", count, unit->GetName(), unit->GetGUIDLow(), (*itr)->getThreat());
            }
            handler->SendSysMessage("End of threat list.");
            return true;
        }

        static bool HandleDebugHostileRefListCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target)
                target = handler->GetSession()->GetPlayer();
            HostileReference* ref = target->getHostileRefManager().getFirst();
            uint32 count = 0;
            handler->PSendSysMessage("Hostil reference list of %s (guid %u)", target->GetName(), target->GetGUIDLow());
            while (ref)
            {
                if (Unit* unit = ref->getSource()->getOwner())
                {
                    ++count;
                    handler->PSendSysMessage("   %u.   %s   (guid %u)  - threat %f", count, unit->GetName(), unit->GetGUIDLow(), ref->getThreat());
                }
                ref = ref->next();
            }
            handler->SendSysMessage("End of hostil reference list.");
            return true;
        }

        static bool HandleDebugSetVehicleIdCommand(ChatHandler* handler, char const* args)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target || target->IsVehicle())
                return false;

            if (!args)
                return false;

            char* i = strtok((char*)args, " ");
            if (!i)
                return false;

            uint32 id = (uint32)atoi(i);
            //target->SetVehicleId(id);
            handler->PSendSysMessage("Vehicle id set to %u", id);
            return true;
        }

        static bool HandleDebugEnterVehicleCommand(ChatHandler* handler, char const* args)
        {
            Unit* target = handler->getSelectedUnit();
            if (!target)// || !target->IsVehicle())
                return false;

            if (!args)
                return false;

            char* i = strtok((char*)args, " ");
            if (!i)
                return false;

            char* j = strtok(NULL, " ");

            int32 entry = (int32)atoi(i);
            int8 seatId = j ? (int8)atoi(j) : -1;

            if (entry == -1)
                target->EnterVehicle(handler->GetSession()->GetPlayer(), seatId);
            else if (!entry)
                handler->GetSession()->GetPlayer()->EnterVehicle(target, seatId);
            else
            {
                Creature* passenger = NULL;
                JadeCore::AllCreaturesOfEntryInRange check(handler->GetSession()->GetPlayer(), entry, 20.0f);
                JadeCore::CreatureSearcher<JadeCore::AllCreaturesOfEntryInRange> searcher(handler->GetSession()->GetPlayer(), passenger, check);
                handler->GetSession()->GetPlayer()->VisitNearbyObject(30.0f, searcher);
                if (!passenger || passenger == target)
                    return false;
                passenger->EnterVehicle(target, seatId);
            }

            handler->PSendSysMessage("Unit %u entered vehicle %d", entry, (int32)seatId);
            return true;
        }

        static bool HandleDebugSpawnVehicleCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* e = strtok((char*)args, " ");
            char* i = strtok(NULL, " ");

            if (!e)
                return false;

            uint32 entry = (uint32)atoi(e);

            float x, y, z, o = handler->GetSession()->GetPlayer()->GetOrientation();
            handler->GetSession()->GetPlayer()->GetClosePoint(x, y, z, handler->GetSession()->GetPlayer()->GetObjectSize());

            if (!i)
                return handler->GetSession()->GetPlayer()->SummonCreature(entry, x, y, z, o);

            uint32 id = (uint32)atoi(i);

            CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(entry);

            if (!ci)
                return false;

            VehicleEntry const* ve = sVehicleStore.LookupEntry(id);

            if (!ve)
                return false;

            Creature* v = new Creature;

            Map* map = handler->GetSession()->GetPlayer()->GetMap();

            if (!v->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_VEHICLE), map, handler->GetSession()->GetPlayer()->GetPhaseMask(), entry, id, handler->GetSession()->GetPlayer()->GetTeam(), x, y, z, o))
            {
                delete v;
                return false;
            }

            map->AddToMap(v->ToCreature());

            return true;
        }

        static bool HandleDebugSendLargePacketCommand(ChatHandler* handler, char const* /*args*/)
        {
            const char* stuffingString = "This is a dummy string to push the packet's size beyond 128000 bytes. ";
            std::ostringstream ss;
            while (ss.str().size() < 128000)
                ss << stuffingString;
            handler->SendSysMessage(ss.str().c_str());
            return true;
        }

        static bool HandleDebugSendSetPhaseShiftCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* t = strtok((char*)args, " ");
            char* p = strtok(NULL, " ");
            if (!t)
                return false;

            std::set<uint32> terrainswap;
            std::set<uint32> phaseId;
            std::set<uint32> inactiveTerrainSwap;

            terrainswap.insert((uint32)atoi(t));

            if (p)
                phaseId.insert((uint32)atoi(p));

            handler->GetSession()->SendSetPhaseShift(phaseId, terrainswap, inactiveTerrainSwap);
            return true;
        }

        static bool HandleDebugGetItemValueCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* e = strtok((char*)args, " ");
            char* f = strtok(NULL, " ");

            if (!e || !f)
                return false;

            uint32 guid = (uint32)atoi(e);
            uint32 index = (uint32)atoi(f);

            Item* i = handler->GetSession()->GetPlayer()->GetItemByGuid(MAKE_NEW_GUID(guid, 0, HIGHGUID_ITEM));

            if (!i)
                return false;

            if (index >= i->GetValuesCount())
                return false;

            uint32 value = i->GetUInt32Value(index);

            handler->PSendSysMessage("Item %u: value at %u is %u", guid, index, value);

            return true;
        }

        static bool HandleDebugSetItemValueCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* e = strtok((char*)args, " ");
            char* f = strtok(NULL, " ");
            char* g = strtok(NULL, " ");

            if (!e || !f || !g)
                return false;

            uint32 guid = (uint32)atoi(e);
            uint32 index = (uint32)atoi(f);
            uint32 value = (uint32)atoi(g);

            Item* i = handler->GetSession()->GetPlayer()->GetItemByGuid(MAKE_NEW_GUID(guid, 0, HIGHGUID_ITEM));

            if (!i)
                return false;

            if (index >= i->GetValuesCount())
                return false;

            i->SetUInt32Value(index, value);

            return true;
        }

        static bool HandleDebugItemExpireCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* e = strtok((char*)args, " ");
            if (!e)
                return false;

            uint32 guid = (uint32)atoi(e);

            Item* i = handler->GetSession()->GetPlayer()->GetItemByGuid(MAKE_NEW_GUID(guid, 0, HIGHGUID_ITEM));

            if (!i)
                return false;

            handler->GetSession()->GetPlayer()->DestroyItem(i->GetBagSlot(), i->GetSlot(), true);
            sScriptMgr->OnItemExpire(handler->GetSession()->GetPlayer(), i->GetTemplate());

            return true;
        }

        //show animation
        static bool HandleDebugAnimCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            uint32 animId = atoi((char*)args);
            handler->GetSession()->GetPlayer()->HandleEmoteCommand(animId);
            return true;
        }

        static bool HandleDebugLoSCommand(ChatHandler* handler, char const* /*args*/)
        {
            if (Unit* unit = handler->getSelectedUnit())
                handler->PSendSysMessage("Unit %s (GuidLow: %u) is %sin LoS", unit->GetName(), unit->GetGUIDLow(), handler->GetSession()->GetPlayer()->IsWithinLOSInMap(unit) ? "" : "not ");
            return true;
        }

        static bool HandleDebugSetAuraStateCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            int32 state = atoi((char*)args);
            if (!state)
            {
                // reset all states
                for (int i = 1; i <= 32; ++i)
                    unit->ModifyAuraState(AuraStateType(i), false);
                return true;
            }

            unit->ModifyAuraState(AuraStateType(abs(state)), state > 0);
            return true;
        }

        static bool HandleDebugSetValueCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* x = strtok((char*)args, " ");
            char* y = strtok(NULL, " ");
            char* z = strtok(NULL, " ");

            if (!x || !y)
                return false;

            WorldObject* target = handler->getSelectedObject();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint64 guid = target->GetGUID();

            uint32 opcode = (uint32)atoi(x);
            if (opcode >= target->GetValuesCount())
            {
                handler->PSendSysMessage(LANG_TOO_BIG_INDEX, opcode, GUID_LOPART(guid), target->GetValuesCount());
                return false;
            }

            bool isInt32 = true;
            if (z)
                isInt32 = (bool)atoi(z);

            if (isInt32)
            {
                uint32 value = (uint32)atoi(y);
                target->SetUInt32Value(opcode , value);
                handler->PSendSysMessage(LANG_SET_UINT_FIELD, GUID_LOPART(guid), opcode, value);
            }
            else
            {
                float value = (float)atof(y);
                target->SetFloatValue(opcode , value);
                handler->PSendSysMessage(LANG_SET_FLOAT_FIELD, GUID_LOPART(guid), opcode, value);
            }

            return true;
        }

        static bool HandleDebugGetValueCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* x = strtok((char*)args, " ");
            char* z = strtok(NULL, " ");

            if (!x)
                return false;

            Unit* target = handler->getSelectedUnit();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint64 guid = target->GetGUID();

            uint32 opcode = (uint32)atoi(x);
            if (opcode >= target->GetValuesCount())
            {
                handler->PSendSysMessage(LANG_TOO_BIG_INDEX, opcode, GUID_LOPART(guid), target->GetValuesCount());
                return false;
            }

            bool isInt32 = true;
            if (z)
                isInt32 = (bool)atoi(z);

            if (isInt32)
            {
                uint32 value = target->GetUInt32Value(opcode);
                handler->PSendSysMessage(LANG_GET_UINT_FIELD, GUID_LOPART(guid), opcode, value);
            }
            else
            {
                float value = target->GetFloatValue(opcode);
                handler->PSendSysMessage(LANG_GET_FLOAT_FIELD, GUID_LOPART(guid), opcode, value);
            }

            return true;
        }

        static bool HandleDebugMod32ValueCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* x = strtok((char*)args, " ");
            char* y = strtok(NULL, " ");

            if (!x || !y)
                return false;

            uint32 opcode = (uint32)atoi(x);
            int value = atoi(y);

            if (opcode >= handler->GetSession()->GetPlayer()->GetValuesCount())
            {
                handler->PSendSysMessage(LANG_TOO_BIG_INDEX, opcode, handler->GetSession()->GetPlayer()->GetGUIDLow(), handler->GetSession()->GetPlayer()->GetValuesCount());
                return false;
            }

            int currentValue = (int)handler->GetSession()->GetPlayer()->GetUInt32Value(opcode);

            currentValue += value;
            handler->GetSession()->GetPlayer()->SetUInt32Value(opcode, (uint32)currentValue);

            handler->PSendSysMessage(LANG_CHANGE_32BIT_FIELD, opcode, currentValue);

            return true;
        }

        static bool HandleDebugUpdateCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            uint32 updateIndex;
            uint32 value;

            char* index = strtok((char*)args, " ");

            Unit* unit = handler->getSelectedUnit();
            if (!unit)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!index)
                return true;

            updateIndex = atoi(index);
            //check updateIndex
            if (unit->IsPlayer())
            {
                if (updateIndex >= PLAYER_END)
                    return true;
            }
            else if (updateIndex >= UNIT_END)
                return true;

            char* val = strtok(NULL, " ");
            if (!val)
            {
                value = unit->GetUInt32Value(updateIndex);

                handler->PSendSysMessage(LANG_UPDATE, unit->GetGUIDLow(), updateIndex, value);
                return true;
            }

            value = atoi(val);

            handler->PSendSysMessage(LANG_UPDATE_CHANGE, unit->GetGUIDLow(), updateIndex, value);

            unit->SetUInt32Value(updateIndex, value);

            return true;
        }

        static bool HandleDebugSet32BitCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            WorldObject* target = handler->getSelectedObject();
            if (!target)
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char* x = strtok((char*)args, " ");
            char* y = strtok(NULL, " ");

            if (!x || !y)
                return false;

            uint32 opcode = (uint32)atoi(x);
            uint32 val = (uint32)atoi(y);
            if (val > 32)                                         //uint32 = 32 bits
                return false;

            uint32 value = val ? 1 << (val - 1) : 0;
            target->SetUInt32Value(opcode,  value);

            handler->PSendSysMessage(LANG_SET_32BIT_FIELD, opcode, value);
            return true;
        }

        static bool HandleDebugMoveflagsCommand(ChatHandler* handler, char const* /*args*/)
        {
            handler->PSendSysMessage("Method depreciated, need update");
            ///Unit* target = handler->getSelectedUnit();
            ///if (!target)
            ///    target = handler->GetSession()->GetPlayer();
            ///
            ///if (!*args)
            ///{
            ///    //! Display case
            ///    handler->PSendSysMessage(LANG_MOVEFLAGS_GET, target->GetUnitMovementFlags(), target->GetExtraUnitMovementFlags());
            ///}
            ///else
            ///{
            ///    char* mask1 = strtok((char*)args, " ");
            ///    if (!mask1)
            ///        return false;
            ///
            ///    char* mask2 = strtok(NULL, " \n");
            ///
            ///    uint32 moveFlags = (uint32)atoi(mask1);
            ///    target->SetUnitMovementFlags(moveFlags);
            ///
            ///    if (mask2)
            ///    {
            ///        uint32 moveFlagsExtra = uint32(atoi(mask2));
            ///        target->SetExtraUnitMovementFlags(moveFlagsExtra);
            ///    }
            ///
            ///    handler->PSendSysMessage(LANG_MOVEFLAGS_SET, target->GetUnitMovementFlags(), target->GetExtraUnitMovementFlags());
            ///}

            return true;
        }

        static bool HandleWPGPSCommand(ChatHandler* handler, char const* /*args*/)
        {
            Player* player = handler->GetSession()->GetPlayer();

            sLog->outInfo(LOG_FILTER_SQL_DEV, "(@PATH, XX, %.3f, %.3f, %.5f, 0, 0, 0, 100, 0),", player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());

            handler->PSendSysMessage("Waypoint SQL written to SQL Developer log");
            return true;
        }

        static bool HandleDebugPhaseCommand(ChatHandler* handler, char const* /*args*/)
        {
            Unit* unit = handler->getSelectedUnit();
            Player* player = handler->GetSession()->GetPlayer();
            if (unit && unit->IsPlayer())
                player = unit->ToPlayer();

            player->GetPhaseMgr().SendDebugReportToPlayer(handler->GetSession()->GetPlayer());
            return true;
        }

        static bool HandleDebugMoveJump(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            WorldObject* target = handler->getSelectedObject();
            if (!target || !target->ToUnit())
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char* cx        = strtok((char*)args, " ");
            char* cy        = strtok(NULL, " ");
            char* cz        = strtok(NULL, " ");
            char* cspeedXY  = strtok(NULL, " ");
            char* cspeedZ   = strtok(NULL, " ");

            if (!cx || !cy || !cz || !cspeedXY || !cspeedZ)
                return false;

            float x         = (float)atof(cx);
            float y         = (float)atof(cy);
            float z         = (float)atof(cz);
            float speedXY   = (float)atof(cspeedXY);
            float speedZ    = (float)atof(cspeedZ);

            target->ToUnit()->GetMotionMaster()->MoveJump(x, y,z, speedXY, speedZ);
            return true;
        }

        static bool HandleDebugMoveBackward(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            WorldObject* target = handler->getSelectedObject();
            if (!target || !target->ToUnit())
            {
                handler->SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char* cx        = strtok((char*)args, " ");
            char* cy        = strtok(NULL, " ");
            char* cz        = strtok(NULL, " ");

            if (!cx || !cy || !cz)
                return false;

            return true;
        }

        static bool HandleDebugLoadZ(ChatHandler* handler, char const* /*args*/)
        {
            for (auto gameobject: sObjectMgr->_gameObjectDataStore)
            {
                GameObjectData data = gameobject.second;

                if (!data.posZ)
                {
                    Map* map = sMapMgr->FindMap(data.mapid, 0);

                    if (!map)
                        map = sMapMgr->CreateMap(data.mapid, handler->GetSession()->GetPlayer());

                    if (map)
                    {
                        float newPosZ = map->GetHeight(data.phaseMask, data.posX, data.posY, MAX_HEIGHT, true);

                        if (newPosZ && newPosZ != -200000.0f)
                            WorldDatabase.PExecute("UPDATE gameobject SET position_z = %f WHERE guid = %u", newPosZ, gameobject.first);
                    }
                }
            }

            return true;
        }

        static bool HandleDebugSetMaxMapDiff(ChatHandler* handler, char const* /*args*/)
        {
            sMapMgr->SetMapDiffLimit(!sMapMgr->HaveMaxDiff());
            if (sMapMgr->HaveMaxDiff())
                handler->PSendSysMessage("Max creature::update diff limit activate !");
            else
                handler->PSendSysMessage("Max creature::update diff limit disable !");

            return true;
        }

        static bool HandleDebugLfgCommand(ChatHandler* p_Handler, char const * /*p_Args*/)
        {
            if (sLFGMgr->IsInDebug())
            {
                p_Handler->PSendSysMessage("Lfg debug mode is now disable");
                sLFGMgr->SetDebug(false);
            }
            else
            {
                sLFGMgr->SetDebug(true);
                p_Handler->PSendSysMessage("Lfg debug mode is now enable");
            }

            return true;
        }

        static bool HandleDebugScaleItem(ChatHandler* handler, char const* args)
        {
            char* arg1 = strtok((char*)args, " ");
            char* arg2 = strtok(NULL, " ");

            if (!arg1)
                return false;

            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(args ? atoi(args) : 0);
            if (!proto)
                return false;

            uint32 ilvl = arg2 ? atoi(arg2) : proto->ItemLevel;
            ilvl = ilvl > 1000 ? 300 : ilvl;

           uint32 minDamage, maxDamage;
           proto->CalculateMinMaxDamageScaling(ilvl, minDamage, maxDamage);
           handler->PSendSysMessage("%s(%i): %i", proto->Name1->Get(handler->GetSessionDbcLocale()), proto->ItemId, ilvl);
           handler->PSendSysMessage("%i - %i", minDamage, maxDamage);
           handler->PSendSysMessage("Armor: %i", proto->CalculateArmorScaling(ilvl));
           for (int i = 0; i < 10; i++)
            if (proto->ItemStat[i].ItemStatType != -1)
                handler->PSendSysMessage("Stat(%i): %i", proto->ItemStat[i].ItemStatType, proto->CalculateStatScaling(i, ilvl));
            return true;
        }

        static bool HandleDebugBattlegroundStart(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Battleground* l_Battleground = p_Handler->GetSession()->GetPlayer()->GetBattleground();
            if (l_Battleground == nullptr)
            {
                p_Handler->PSendSysMessage("You're not in a battleground !");
                return false;
            }

            l_Battleground->FastStart();
            return true;
        }

        /// This can be reported by static analyse, yes l_Pig is free and make it crash that the point !
        static bool HandleDebugCrashTest(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            p_Handler->PSendSysMessage("You've crash the server by adding pigs in farm that doesn't exists!");

            Player* l_Pig = new Player(p_Handler->GetSession());
            delete l_Pig;
#ifndef __clang_analyzer__
            l_Pig->isAFK();
#endif
            return true;
        }

        static bool HandleDebugBgAward(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Battleground* l_Battleground = p_Handler->GetSession()->GetPlayer()->GetBattleground();

            if (!l_Battleground)
            {
                p_Handler->PSendSysMessage("You're not in a battleground!");
                return false;
            }

            l_Battleground->AwardTeams(l_Battleground->GetOtherTeam(p_Handler->GetSession()->GetPlayer()->GetTeam()));
            return true;
        }

        static bool HandleDebugHeirloom(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!p_Args)
                return false;

            Player* l_Player = p_Handler->GetSession()->GetPlayer();
            char* arg1 = strtok((char*)p_Args, " ");
            char* arg2 = strtok(NULL, " ");

            if (!arg1 || !arg2)
                return false;

            int32 l_ID = atoi(arg1);
            int32 l_Flags = atoi(arg2);

            l_Player->SetDynamicValue(PLAYER_DYNAMIC_FIELD_HEIRLOOMS, l_Player->GetDynamicValues(PLAYER_DYNAMIC_FIELD_HEIRLOOMS).size(), l_ID);
            l_Player->SetDynamicValue(PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS, l_Player->GetDynamicValues(PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS).size(), l_Flags);

            return true;
        }

        static bool HandleDebugVignette(ChatHandler* p_Handler, char const* p_Args)
        {
            char* l_VignetteIDStr = strtok((char*)p_Args, " ");
            if (!l_VignetteIDStr)
                return false;

            Unit* l_SelectedUnit = p_Handler->GetSession()->GetPlayer()->GetSelectedUnit();
            if (!l_SelectedUnit)
                return false;

            uint32 l_VignetteID = atoi(l_VignetteIDStr);
            uint64 l_VignetteGUID = MAKE_NEW_GUID(l_SelectedUnit->GetGUIDLow(), l_VignetteID, HIGHGUID_VIGNETTE);


            WorldPacket l_Data(SMSG_VIGNETTE_UPDATE);
            l_Data.WriteBit(true);                                 ///< ForceUpdate
            l_Data << uint32(0);                                   ///< RemovedCount

            //for ()
            //    l_Data.appendPackGUID(IDs);

            l_Data << uint32(1);                                   ///< Added count

//            for ()
                l_Data.appendPackGUID(l_VignetteGUID);

            l_Data << uint32(1);
            {
                l_Data << float(l_SelectedUnit->GetPositionX());
                l_Data << float(l_SelectedUnit->GetPositionY());
                l_Data << float(l_SelectedUnit->GetPositionZ());
                l_Data.appendPackGUID(l_VignetteGUID);
                l_Data << uint32(l_VignetteID);
                l_Data << uint32(0);                               ///< unk
            }

            l_Data << uint32(0);                                   ///< UpdateCount
            {
            }

            l_Data << uint32(0);                                   ///< UpdateDataCount

            p_Handler->GetSession()->SendPacket(&l_Data);
            return true;
        }

        static bool HandleDebugDumpCharTemplate(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!p_Args)
                return false;

            FILE* l_Output = fopen("./templates.sql", "w+");
            if (!l_Output)
                return false;

            std::ostringstream l_StrBuilder;
            l_StrBuilder << "TRUNCATE TABLE character_template;" << std::endl;
            l_StrBuilder << "TRUNCATE TABLE character_template_item;" << std::endl;
            l_StrBuilder << "TRUNCATE TABLE character_template_spell;" << std::endl;
            l_StrBuilder << "TRUNCATE TABLE character_template_reputation;" << std::endl << std::endl;

            bool l_FirstEntry = true;
            l_StrBuilder << "INSERT INTO character_template VALUES";
            for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
            {
                ChrClassesEntry const* l_ClassEntry = sChrClassesStore.LookupEntry(l_ClassId);
                l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                             << "("
                             << l_ClassId << ", "
                             << l_ClassId << ", "
                             << "\"" << l_ClassEntry->NameLang << " - Level 100\", \"\", 100, "
                             << "100000000, -8833.07, 622.778, 93.9317, 0.6771, 0, 1569.97, -4397.41, 16.0472, 0.543025, 1, 0)";

                l_FirstEntry = false;
            }
            l_StrBuilder << ";" << std::endl << std::endl;

            const uint32 l_HordeMask = 0xFE5FFBB2;
            const uint32 l_AllianceMask = 0xFD7FFC4D;
            const uint32 l_ItemLevel = 695;
            const uint32 l_ItemType = 3; ///< 1 Shop PvE, 2 Shop PvP, 3 Test PvE, 4 Test PvP
            const uint32 l_BagItemId = 114821;

            std::string l_SearchString = p_Args; /// Case sensitive search

            std::list<uint32> l_ArmorSlotFind[3];
            std::list<uint32> l_ClassWeaponFind[3];
            std::list<uint8> l_CloaksFind[3];
            std::map<uint8, uint8> l_TrinketsFind[3];

            l_FirstEntry = true;
            l_StrBuilder << "INSERT INTO character_template_item (id, itemID, faction, count, type) VALUES ";

            ItemTemplateContainer const* l_Store = sObjectMgr->GetItemTemplateStore();
            for (ItemTemplateContainer::const_iterator l_Iter = l_Store->begin(); l_Iter != l_Store->end(); ++l_Iter)
            {
                ItemTemplate const* l_Template = &l_Iter->second;

                uint8 l_Faction = 0;

                if (l_Template->AllowableRace == l_HordeMask)
                    l_Faction = 1;
                else if (l_Template->AllowableRace == l_AllianceMask)
                    l_Faction = 2;

                if (l_Template->InventoryType == INVTYPE_BAG)
                {
                    uint32 l_Count = 4;

                    if (l_Template->ItemId != l_BagItemId)
                        continue;

                    for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
                    {
                        int32 l_ClassMask = 1 << (l_ClassId - 1);
                        if (l_Template->AllowableClass & l_ClassMask)
                        {
                            l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_BagItemId << ", "
                                         << "1, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";
                            l_StrBuilder << "," << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_BagItemId << ", "
                                         << "2, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";

                            l_FirstEntry = false;
                        }
                    }
                }

                /// Must after bags
                if (sSpellMgr->GetItemSourceSkills(l_Template->ItemId) != nullptr)
                    continue;

                //printf("%s == %s\n", l_Template->Name1->Get(sWorld->GetDefaultDbcLocale()), l_SearchString.c_str());
                if (std::string(l_Template->Name1->Get(sWorld->GetDefaultDbcLocale())).find(l_SearchString) == std::string::npos)
                    continue;

                if (l_Template->InventoryType == INVTYPE_TRINKET)
                {
                    uint32 l_Count = 1;

                    if (l_Template->ItemLevel != l_ItemLevel)
                        continue;

                    for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
                    {
                        int32 l_ClassMask = 1 << (l_ClassId - 1);
                        if (l_Template->AllowableClass & l_ClassMask)
                        {
                            if (!l_Template->HasClassSpec(l_ClassId, 100))
                                continue;

                            if (l_TrinketsFind[l_Faction].find(l_ClassId) != l_TrinketsFind[l_Faction].end() && l_TrinketsFind[l_Faction][l_ClassId] == 2)
                                continue;

                            if (l_TrinketsFind[l_Faction].find(l_ClassId) == l_TrinketsFind[l_Faction].end())
                                l_TrinketsFind[l_Faction][l_ClassId] = 1;
                            else
                                l_TrinketsFind[l_Faction][l_ClassId] = 2;

                            l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "1, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";
                            l_StrBuilder << "," << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "2, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";

                             l_FirstEntry = false;
                        }
                    }
                }

                if (l_Template->InventoryType == INVTYPE_CLOAK)
                {
                    uint32 l_Count = 1;

                    if (l_Template->ItemLevel != l_ItemLevel)
                        continue;

                    for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
                    {
                        int32 l_ClassMask = 1 << (l_ClassId - 1);
                        if (l_Template->AllowableClass & l_ClassMask)
                        {
                            if (!l_Template->HasClassSpec(l_ClassId, 100))
                                continue;

                            if (std::find(l_CloaksFind[l_Faction].begin(), l_CloaksFind[l_Faction].end(), l_ClassId) != l_CloaksFind[l_Faction].end())
                                continue;

                            l_CloaksFind[l_Faction].push_back(l_ClassId);

                            l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "1, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";
                            l_StrBuilder << "," << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "2, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";

                            l_FirstEntry = false;
                        }
                    }
                }

                if (l_Template->Class == ITEM_CLASS_WEAPON)
                {
                    uint32 l_Count = 1;

                    if (l_Template->ItemLevel != l_ItemLevel)
                        continue;

                    for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
                    {
                        int32 l_ClassMask = 1 << (l_ClassId - 1);
                        if (l_Template->AllowableClass & l_ClassMask)
                        {
                            if (!l_Template->HasClassSpec(l_ClassId, 100))
                                continue;

                            if (std::find(l_ClassWeaponFind[l_Faction].begin(), l_ClassWeaponFind[l_Faction].end(), l_ClassId | l_Template->SubClass << 16) != l_ClassWeaponFind[l_Faction].end())
                                continue;

                            l_ClassWeaponFind[l_Faction].push_back(l_ClassId | l_Template->SubClass << 16);

                            l_Count = l_Template->IsOneHanded() || (l_Template->IsTwoHandedWeapon() && l_ClassId == CLASS_WARRIOR) ? 2 : 1;
                            l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "1, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";
                            l_StrBuilder << "," << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "2, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";

                            l_FirstEntry = false;
                        }
                    }
                }

                /// Armor
                //if (std::string(l_Template->Name1->Get(LOCALE_enUS)).find(l_SearchString) != std::string::npos)
                {
                    if (l_Template->Class != ITEM_CLASS_ARMOR ||
                        l_Template->InventoryType == INVTYPE_BAG ||
                        l_Template->InventoryType == INVTYPE_TRINKET ||
                        l_Template->InventoryType == INVTYPE_CLOAK)
                        continue;

                    uint32 l_Count = 1;

                    if (l_Template->ItemLevel != l_ItemLevel)
                        continue;

                    for (uint32 l_SpecId = 0; l_SpecId < sChrSpecializationsStore.GetNumRows(); l_SpecId++)
                    {
                        auto l_Specialization = sChrSpecializationsStore.LookupEntry(l_SpecId);
                        if (!l_Specialization)
                            continue;

                        int32 l_ClassId = l_Specialization->ClassID;

                        int32 l_ClassMask = 1 << (l_ClassId - 1);
                        if (l_Template->AllowableClass & l_ClassMask)
                        {
                            if (l_Template->Class == ITEM_CLASS_ARMOR)
                            {
                                switch (l_Template->SubClass)
                                {
                                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                                        switch (l_ClassId)
                                        {
                                            case CLASS_PRIEST:
                                            case CLASS_MAGE:
                                            case CLASS_WARLOCK:
                                                break;
                                            default:
                                                continue;
                                        }
                                        break;
                                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                                        switch (l_ClassId)
                                        {
                                            case CLASS_ROGUE:
                                            case CLASS_MONK:
                                            case CLASS_DRUID:
                                                break;
                                            default:
                                                continue;
                                        }
                                        break;
                                    case ITEM_SUBCLASS_ARMOR_MAIL:
                                        switch (l_ClassId)
                                        {
                                            case CLASS_HUNTER:
                                            case CLASS_SHAMAN:
                                                break;
                                            default:
                                                continue;
                                        }
                                        break;
                                    case ITEM_SUBCLASS_ARMOR_PLATE:
                                        switch (l_ClassId)
                                        {
                                            case CLASS_PALADIN:
                                            case CLASS_WARRIOR:
                                            case CLASS_DEATH_KNIGHT:
                                                break;
                                            default:
                                                continue;
                                        }
                                        break;
                                    default:
                                        break;
                                }
                            }

                            if (!l_Template->HasSpec((SpecIndex)l_SpecId, 100))
                                continue;

                            if (std::find(l_ArmorSlotFind[l_Faction].begin(), l_ArmorSlotFind[l_Faction].end(), l_SpecId | l_Template->InventoryType << 16) != l_ArmorSlotFind[l_Faction].end())
                                continue;

                            l_ArmorSlotFind[l_Faction].push_back(l_SpecId | l_Template->InventoryType << 16);

                            l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "1, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";
                            l_StrBuilder << "," << std::endl
                                         << "("
                                         << l_ClassId << ", "
                                         << l_Template->ItemId << ", "
                                         << "2, "
                                         << l_Count << ", "
                                         << l_ItemType
                                         << ")";

                            l_FirstEntry = false;
                        }
                    }
                }
            }

            const uint32 l_Count = 200;
            const uint32 l_TomeOfTheClearMindId = 79249;

            for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
            {
                l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                            << "("
                            << l_ClassId << ", "
                            << l_TomeOfTheClearMindId << ", "
                            << "1, "
                            << l_Count << ", "
                            << l_ItemType
                            << ")";
                l_StrBuilder << "," << std::endl
                            << "("
                            << l_ClassId << ", "
                            << l_TomeOfTheClearMindId << ", "
                            << "2, "
                            << l_Count << ", "
                            << l_ItemType
                            << ")";
            }

            l_StrBuilder << ";" << std::endl << std::endl;

            l_FirstEntry = true;
            l_StrBuilder << "INSERT INTO character_template_spell VALUES";

//            /// Learn Dual spec for all classes
//            for (int32 l_ClassId = CLASS_WARRIOR; l_ClassId < MAX_CLASSES; l_ClassId++)
//            {
//                l_StrBuilder << std::endl;
//                l_StrBuilder << "(" << l_ClassId << ", 63644)," << std::endl;
//                l_StrBuilder << "(" << l_ClassId << ", 63645),";
//            }

            for (uint32 l_ID = 0; l_ID < sSpellStore.GetNumRows(); ++l_ID)
            {
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_ID);
                if (!l_SpellInfo)
                    continue;

                SpellEffectInfo const* l_EffectInfo = l_SpellInfo->GetEffectByType(SPELL_EFFECT_APPLY_GLYPH);
                if (!l_EffectInfo)
                    continue;

                if (GlyphPropertiesEntry const* l_Glyph = sGlyphPropertiesStore.LookupEntry(l_EffectInfo->MiscValue))
                {
                    if (SpellInfo const* l_GlyphInfo = sSpellMgr->GetSpellInfo(l_Glyph->SpellId))
                    {
                        /// First value should be the template id, but with us, it's the same as classid
                        uint32 l_ClassId = l_GlyphInfo->GetClassIDBySpellFamilyName();

                        l_StrBuilder << (l_FirstEntry ? "" : ",") << std::endl
                                     << "("
                                     << l_ClassId << ", "
                                     << l_SpellInfo->Id
                                     << ")";

                        l_FirstEntry = false;
                    }
                }
            }

            l_StrBuilder << ";" << std::endl;
            std::string l_FinalString = l_StrBuilder.str();

            fwrite(l_FinalString.c_str(), l_FinalString.length(), 1, l_Output);
            fflush(l_Output);
            fclose(l_Output);

            p_Handler->PSendSysMessage("Characters templates generated !");

            return true;
        }

        static bool HandleDebugDumpRewardlessMissions(ChatHandler* /*p_Handler*/, char const* p_Args)
        {
            if (!p_Args)
                return false;

            FILE* l_Output = fopen("./rewardless_ids.txt", "w+");
            if (!l_Output)
                return false;

            std::ostringstream l_StrBuilder;
            std::list<uint32> l_AlreadyInserted;

            for (uint32 l_ID = 0; l_ID < sGarrMissionRewardStore.GetNumRows(); l_ID++)
            {
                GarrMissionRewardEntry const* l_Entry = sGarrMissionRewardStore.LookupEntry(l_ID);

                if (!l_Entry)
                    continue;

                if (!l_Entry->ItemID)
                    continue;

                ItemTemplate const* l_Template = sObjectMgr->GetItemTemplate(l_Entry->ItemID);

                if (!l_Template)
                    continue;

                if (!(l_Template->Flags & ITEM_FLAG_OPENABLE))
                    continue;

                if (LootTemplates_Item.HaveLootFor(l_Template->ItemId))
                    continue;

                if (std::find(l_AlreadyInserted.begin(), l_AlreadyInserted.end(), l_Entry->ItemID) != l_AlreadyInserted.end())
                    continue;

                l_AlreadyInserted.push_back(l_Entry->ItemID);
                l_StrBuilder << l_Entry->ItemID << " \\ " << l_Template->Name1->Get(LOCALE_enUS) << "\n";
            }

            std::string l_String = l_StrBuilder.str();
            fwrite(l_String.c_str(), l_String.length(), 1, l_Output);
            fflush(l_Output);
            fclose(l_Output);

            return true;
        }

        static bool HandleSpellDebugDumpRewardlessMissions(ChatHandler* /*p_Handler*/, char const* p_Args)
        {
            if (!p_Args)
                return false;

            FILE* l_Output = fopen("./rewardless_spell_ids.txt", "w+");
            if (!l_Output)
                return false;

            FILE* l_SQL = fopen("./temp_loot_table.sql", "w+");
            if (!l_SQL)
                return false;

            std::ostringstream l_StrBuilder;
            std::ostringstream l_StrBuilder2;
            std::list<uint32> l_AlreadyInserted;

            for (uint32 l_ID = 0; l_ID < sGarrMissionRewardStore.GetNumRows(); l_ID++)
            {
                GarrMissionRewardEntry const* l_Entry = sGarrMissionRewardStore.LookupEntry(l_ID);

                if (!l_Entry)
                    continue;

                if (!l_Entry->ItemID)
                    continue;

                ItemTemplate const* l_Template = sObjectMgr->GetItemTemplate(l_Entry->ItemID);

                if (!l_Template)
                    continue;

                uint32 l_SpellId = l_Template->Spells[0].SpellId;

                if (!l_SpellId)
                    continue;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellId);

                if (!l_SpellInfo)
                    continue;

                if (!l_SpellInfo->HasEffect(SPELL_EFFECT_CREATE_ITEM_2))
                    continue;

                if (LootTemplates_Spell.HaveLootFor(l_SpellId))
                    continue;

                if (std::find(l_AlreadyInserted.begin(), l_AlreadyInserted.end(), l_Entry->ItemID) != l_AlreadyInserted.end())
                    continue;

                l_AlreadyInserted.push_back(l_Entry->ItemID);
                l_StrBuilder << l_Entry->ItemID << " \\ " << l_Template->Name1->Get(LOCALE_enUS) << "\n";
                l_StrBuilder2 << "UPDATE temp_loot_table SET entry = " << l_SpellId << " WHERE entry = " << l_Entry->ItemID << ";\n";
                l_StrBuilder2 << "DELETE FROM spell_loot_tempalte WHERE entry = " << l_SpellId << ";\n";
            }

            std::string l_String = l_StrBuilder.str();
            fwrite(l_String.c_str(), l_String.length(), 1, l_Output);
            fflush(l_Output);
            fclose(l_Output);

            std::string l_String2 = l_StrBuilder2.str();
            fwrite(l_String2.c_str(), l_String2.length(), 1, l_SQL);
            fflush(l_SQL);
            fclose(l_SQL);

            return true;
        }

        static bool HandleDebugPlayerCondition(ChatHandler* p_Handler, char const* p_Args)
        {
            char* l_ArgStr = strtok((char*)p_Args, " ");
            if (!l_ArgStr)
                return false;

            uint32 l_ConditionID = atoi(l_ArgStr);

            auto l_Result = p_Handler->GetSession()->GetPlayer()->EvalPlayerCondition(l_ConditionID);

            if (l_Result.first)
                p_Handler->PSendSysMessage("Condition %u is satisfied", l_ConditionID);
            else
                p_Handler->PSendSysMessage("Condition %u failed => %s", l_ConditionID,  l_Result.second.c_str());

            return true;
        }

        static bool HandleDebugPacketProfiler(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            gPacketProfilerMutex.lock();
            p_Handler->PSendSysMessage("----------------");

            for (auto l_Pair : gPacketProfilerData)
            {
                p_Handler->PSendSysMessage("%s => %u", GetOpcodeNameForLogging((Opcodes)l_Pair.first, WOW_SERVER_TO_CLIENT).c_str(), l_Pair.second);
            }

            p_Handler->PSendSysMessage("----------------");
            gPacketProfilerMutex.unlock();
            return true;
        }

        static bool HandleHotfixOverride(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!p_Args)
                return false;

            auto l_SendHotfixPacket = [&p_Handler](DB2StorageBase* p_Store, uint32 p_Entry) -> void
            {
                ByteBuffer l_ResponseData(2 * 1024);
                if (p_Store->WriteRecord(p_Entry, l_ResponseData, p_Handler->GetSessionDbLocaleIndex()))
                {
                    WorldPacket l_Data(SMSG_DB_REPLY, 4 + 4 + 4 + 4 + l_ResponseData.size());
                    l_Data << uint32(p_Store->GetHash());
                    l_Data << uint32(p_Entry);
                    l_Data << uint32(sObjectMgr->GetHotfixDate(p_Entry, p_Store->GetHash()));
                    l_Data.WriteBit(1);                                                         ///< Found ???
                    l_Data << uint32(l_ResponseData.size());
                    l_Data.append(l_ResponseData);

                    p_Handler->GetSession()->SendPacket(&l_Data);
                }
                else
                {
                    WorldPacket l_Data(SMSG_DB_REPLY, 4 + 4 + 4 + 4);
                    l_Data << uint32(p_Store->GetHash());
                    l_Data << uint32(-int32(p_Entry));
                    l_Data << uint32(time(NULL));
                    l_Data.WriteBit(0);                                                         ///< Not Found ???
                    l_Data << uint32(0);

                    p_Handler->GetSession()->SendPacket(&l_Data);
                }
            };
            char* arg1 = strtok((char*)p_Args, " ");
            char* arg2 = strtok(NULL, " ");

            if (!arg1 || !arg2)
                return false;

            std::string l_HotfixType = arg1;
            uint32 l_HotfixEntry = atoi(arg2);
            if (l_HotfixType == "item")
            {
                l_SendHotfixPacket(&sItemStore, l_HotfixEntry);
                l_SendHotfixPacket(&sItemSparseStore, l_HotfixEntry);

                for (uint32 i = 0; i < sItemEffectStore.GetNumRows(); ++i)
                {
                    ItemEffectEntry const* l_Entry = sItemEffectStore.LookupEntry(i);
                    if (!l_Entry || l_Entry->ItemID != l_HotfixEntry)
                        continue;

                    l_SendHotfixPacket(&sItemEffectStore, i);
                }

                std::vector<uint32> l_Appearances;
                for (uint32 i = 0; i < sItemModifiedAppearanceStore.GetNumRows(); ++i)
                {
                    ItemModifiedAppearanceEntry const* l_Entry = sItemModifiedAppearanceStore.LookupEntry(i);
                    if (!l_Entry || l_Entry->ItemID != l_HotfixEntry)
                        continue;

                    l_SendHotfixPacket(&sItemModifiedAppearanceStore, i);
                    l_Appearances.push_back(i);
                }

                for (uint32 l_AppearanceID : l_Appearances)
                {
                    ItemAppearanceEntry const* l_Entry = sItemAppearanceStore.LookupEntry(l_AppearanceID);
                    if (!l_Entry)
                        continue;

                    l_SendHotfixPacket(&sItemAppearanceStore, l_AppearanceID);
                }

            }

            return true;
        }

        static bool HandleDebugMirrorCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player*   l_Player = p_Handler->GetSession()->GetPlayer();
            Creature* l_Target = p_Handler->getSelectedCreature();

            if (l_Target == nullptr || l_Player == nullptr)
                return false;

            ///                                                   0   1       2        3         4     5           6         7           8
            QueryResult l_Result = LoginDatabase.PQuery("SELECT race, gender, class, skinColor, face, hairStyle, hairColor, facialHair, equipment FROM character_renderer_queue ORDER BY RAND() LIMIT 1");
            if (!l_Result)
                return false;

            Field* l_Fields = l_Result->Fetch();

            ChrRacesEntry const* l_RaceEntry = sChrRacesStore.LookupEntry(l_Fields[0].GetUInt8());
            if (!l_RaceEntry)
                return false;

            uint32 l_Display = l_Fields[1].GetUInt8() == 0 ? l_RaceEntry->model_m : l_RaceEntry->model_f;
            std::list<uint32> l_Displays;

            Tokenizer l_Equipements(l_Fields[8].GetString(), ']');
            for (Tokenizer::const_iterator l_It = l_Equipements.begin(); l_It != l_Equipements.end(); ++l_It)
            {
                std::string l_DisplayIdTxt = (*l_It);
                if (l_DisplayIdTxt == ",")
                    continue;

                for (auto l_Idx = l_DisplayIdTxt.size(); l_Idx != 0; l_Idx--)
                {
                    if (l_DisplayIdTxt[l_Idx] == ',')
                    {
                        l_Displays.push_back(atoi(l_DisplayIdTxt.substr(l_Idx + 1, std::string::npos).c_str()));
                        break;
                    }
                }
            }

            /// Update display & add mirror image flags
            l_Target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
            l_Target->SetDisplayId(l_Display);

            /// Forge SMSG_UPDATE_OBJECT, client need to receive it before SMSG_MIRROR_IMAGE_COMPONENTED_DATA
            UpdateData  l_UpdateData(l_Target->GetMapId());
            WorldPacket l_Packet;

            l_Target->BuildValuesUpdateBlockForPlayer(&l_UpdateData, l_Player);

            if (l_UpdateData.BuildPacket(&l_Packet))
                l_Player->GetSession()->SendPacket(&l_Packet);


            /// Forge SMSG_MIRROR_IMAGE_COMPONENTED_DATA
            WorldPacket data(SMSG_MIRROR_IMAGE_COMPONENTED_DATA, 76);

            data.appendPackGUID(l_Target->GetGUID());
            data << uint32(l_Display);
            data << uint8(l_Fields[0].GetUInt8());
            data << uint8(l_Fields[1].GetUInt8());
            data << uint8(l_Fields[2].GetUInt8());
            data << uint8(l_Fields[3].GetUInt8());      // skin
            data << uint8(l_Fields[4].GetUInt8());      // face
            data << uint8(l_Fields[5].GetUInt8());      // hair
            data << uint8(l_Fields[6].GetUInt8());      // haircolor
            data << uint8(l_Fields[7].GetUInt8());      // facialhair
            data.appendPackGUID(0);

            data << uint32(l_Displays.size());

            for (auto l_DisplayId : l_Displays)
                data << uint32(l_DisplayId);

            /*static EquipmentSlots const itemSlots[] =
            {
                EQUIPMENT_SLOT_HEAD,
                EQUIPMENT_SLOT_SHOULDERS,
                EQUIPMENT_SLOT_BODY,
                EQUIPMENT_SLOT_CHEST,
                EQUIPMENT_SLOT_WAIST,
                EQUIPMENT_SLOT_LEGS,
                EQUIPMENT_SLOT_FEET,
                EQUIPMENT_SLOT_WRISTS,
                EQUIPMENT_SLOT_HANDS,
                EQUIPMENT_SLOT_TABARD,
                EQUIPMENT_SLOT_BACK,
                EQUIPMENT_SLOT_END
            };

            // Display items in visible slots
            for (EquipmentSlots const* itr = itemSlots; *itr != EQUIPMENT_SLOT_END; ++itr)
            {
                if (*itr == EQUIPMENT_SLOT_HEAD && l_Player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
                    data << uint32(0);
                else if (*itr == EQUIPMENT_SLOT_BACK && l_Player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
                    data << uint32(0);
                else if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Player->GetUInt32Value(PLAYER_FIELD_VISIBLE_ITEMS + (*itr * 3))))
                    data << uint32(l_Proto->DisplayInfoID);
                else
                    data << uint32(0);
            }*/

            l_Player->GetSession()->SendPacket(&data);

            return true;
        }

        static bool HandleDebugPvELogsCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_Player = p_Handler->GetSession()->GetPlayer();

            EncounterDatas l_Datas;

            l_Datas.EncounterID     = 1704;                                         ///< BlackHand
            l_Datas.Expansion       = Expansion::EXPANSION_WARLORDS_OF_DRAENOR;
            l_Datas.RealmID         = g_RealmID;
            l_Datas.GuildID         = 9;
            l_Datas.GuildFaction    = TeamId::TEAM_HORDE;
            l_Datas.GuildName = l_Player->GetGuildName();
            l_Datas.MapID           = 1205;                                         ///< Blackrock Foundry
            l_Datas.DifficultyID    = 16;                                           ///< Mythic mode
            l_Datas.StartTime       = time(nullptr);
            l_Datas.CombatDuration  = 7 * TimeConstants::MINUTE;
            l_Datas.EndTime         = l_Datas.StartTime + l_Datas.CombatDuration;
            l_Datas.Success         = true;
            l_Datas.DeadCount       = 2;

            RosterData l_Data;
            l_Data.GuidLow      = l_Player->GetGUIDLow();
            l_Data.Name         = l_Player->GetName();
            l_Data.Level        = l_Player->getLevel();
            l_Data.Class        = l_Player->getClass();
            l_Data.SpecID       = l_Player->GetSpecializationId();
            l_Data.Role         = l_Player->GetRoleForGroup();
            l_Data.ItemLevel    = l_Player->GetAverageItemLevelEquipped();

            l_Datas.RosterDatas.push_back(l_Data);

            sScriptMgr->OnEncounterEnd(&l_Datas);
            return true;
        }

        static bool HandleDebugQuestLogsCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
        {
            Player* l_Player = p_Handler->GetSession()->GetPlayer();

            if (!l_Player->m_IsDebugQuestLogs)
            {
                p_Handler->PSendSysMessage(LANG_DEBUG_QUEST_LOGS_ON);
                l_Player->m_IsDebugQuestLogs = true;
            }
            else
            {
                p_Handler->PSendSysMessage(LANG_DEBUG_QUEST_LOGS_OFF);
                l_Player->m_IsDebugQuestLogs = false;
            }
            return true;
        }

        static bool HandleDebugAddUnitStateCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            /// Returns and send error if no Unit is selected
            Unit* l_Unit = p_Handler->getSelectedUnit();
            if (!l_Unit)
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_SELECT_CHAR_OR_CREATURE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            int32 l_State = atoi((char*)p_Args);

            /// When p_Args == 0, all unit states are cleared
            if (!l_State)
            {
                l_Unit->ClearUnitState(UnitState::UNIT_STATE_ALL_STATE);
                p_Handler->PSendSysMessage("Unit states cleared");
                return true;
            }

            bool l_Found = false;

            /// If selected Unit has not the p_Args state, it's added
            /// If the state doesn't exist, it returns
            if (!l_Unit->HasUnitState(g_Unitstates[l_State].Flag))
            {
                for (UnitStates l_UnitState : g_Unitstates)
                {
                    if (l_UnitState.Flag == l_State)
                    {
                        l_Found = true;
                        break;
                    }
                }

                if (!l_Found)
                {
                    p_Handler->PSendSysMessage("The unit_state you're looking for doesn't exist.");
                    return true;
                }

                l_Unit->AddUnitState(g_Unitstates[l_State].Flag);
                p_Handler->PSendSysMessage("Unit state %s has been added", g_Unitstates[l_State].Text);
            }
            else
                p_Handler->PSendSysMessage("This unit has already the state %s", g_Unitstates[l_State].Text);

            return true;
        }

        static bool HandleDebugGetUnitStatesCommand(ChatHandler* p_Handler, const char* /*p_Args*/)
        {
            Unit* l_Unit = p_Handler->getSelectedUnit();
            if (!l_Unit)
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_SELECT_CHAR_OR_CREATURE);
                p_Handler->SetSentErrorMessage(true);
                return true;
            }

            /// Checks every unit_state for the Unit selected, and displays it in the handler's chatbox
            for (UnitStates l_UnitState : g_Unitstates)
            {
                if (l_Unit->HasUnitState(l_UnitState.Flag))
                    p_Handler->PSendSysMessage("Selected unit has %s", l_UnitState.Text);
            }

            return true;
        }

        static bool HandleDebugRemoveUnitStateCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            if (!*p_Args)
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_BAD_VALUE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            Unit* l_Unit = p_Handler->getSelectedUnit();
            if (!l_Unit)
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_SELECT_CHAR_OR_CREATURE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            /// Checks if the p_Arg has a correct value
            int32 l_State = atoi((char*)p_Args);
            if (!l_State)
            {
                p_Handler->PSendSysMessage("There is no unit_state with this value.");
                return true;
            }

            /// Removes the state to the Unit if it has it
            if (l_Unit->HasUnitState(g_Unitstates[l_State].Flag))
            {
                l_Unit->ClearUnitState(g_Unitstates[l_State].Flag);
                p_Handler->PSendSysMessage("Unit_state %s has been removed", g_Unitstates[l_State].Text);
            }
            else
                p_Handler->PSendSysMessage("This unit doesn't have the unit state %s", g_Unitstates[l_State].Text);

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_debug_commandscript()
{
    new debug_commandscript();
}
#endif
