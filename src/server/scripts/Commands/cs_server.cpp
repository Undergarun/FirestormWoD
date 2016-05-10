////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: server_commandscript
%Complete: 100
Comment: All server related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "GitRevision.h"
#include "Config.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include <regex>

class server_commandscript : public CommandScript
{
public:
    server_commandscript() : CommandScript("server_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand serverIdleRestartCommandTable[] =
        {
            { "cancel",         SEC_ADMINISTRATOR,  true,  &HandleServerShutDownCancelCommand,      "", NULL },
            { ""   ,            SEC_ADMINISTRATOR,  true,  &HandleServerIdleRestartCommand,         "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };

        static ChatCommand serverIdleShutdownCommandTable[] =
        {
            { "cancel",         SEC_ADMINISTRATOR,  true,  &HandleServerShutDownCancelCommand,      "", NULL },
            { ""   ,            SEC_ADMINISTRATOR,  true,  &HandleServerIdleShutDownCommand,        "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };

        static ChatCommand serverRestartCommandTable[] =
        {
            { "cancel",         SEC_ADMINISTRATOR,  true,  &HandleServerShutDownCancelCommand,      "", NULL },
            { ""   ,            SEC_ADMINISTRATOR,  true,  &HandleServerRestartCommand,             "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };

        static ChatCommand serverShutdownCommandTable[] =
        {
            { "cancel",         SEC_ADMINISTRATOR,  true,  &HandleServerShutDownCancelCommand,      "", NULL },
            { ""   ,            SEC_ADMINISTRATOR,  true,  &HandleServerShutDownCommand,            "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };

        static ChatCommand serverSetCommandTable[] =
        {
            { "difftime",       SEC_CONSOLE,        true,  &HandleServerSetDiffTimeCommand,         "", NULL },
            { "loglevel",       SEC_CONSOLE,        true,  &HandleServerSetLogLevelCommand,         "", NULL },
            { "closed",         SEC_ADMINISTRATOR,  true,  &HandleServerSetClosedCommand,           "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };

        static ChatCommand serverCommandTable[] =
        {
            { "corpses",        SEC_GAMEMASTER,     true,  &HandleServerCorpsesCommand,             "", NULL },
            { "exit",           SEC_CONSOLE,        true,  &HandleServerExitCommand,                "", NULL },
            { "idlerestart",    SEC_ADMINISTRATOR,  true,  NULL,                                    "", serverIdleRestartCommandTable },
            { "idleshutdown",   SEC_ADMINISTRATOR,  true,  NULL,                                    "", serverIdleShutdownCommandTable },
            { "info",           SEC_PLAYER,         true,  &HandleServerInfoCommand,                "", NULL },
            { "motd",           SEC_PLAYER,         true,  &HandleServerMotdCommand,                "", NULL },
            { "plimit",         SEC_ADMINISTRATOR,  true,  &HandleServerPLimitCommand,              "", NULL },
            { "restart",        SEC_ADMINISTRATOR,  true,  NULL,                                    "", serverRestartCommandTable },
            { "shutdown",       SEC_ADMINISTRATOR,  true,  NULL,                                    "", serverShutdownCommandTable },
            { "set",            SEC_ADMINISTRATOR,  true,  NULL,                                    "", serverSetCommandTable },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };

         static ChatCommand commandTable[] =
        {
            { "server",         SEC_ADMINISTRATOR,  true,  NULL,                                    "", serverCommandTable },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };
        return commandTable;
    }

    // Triggering corpses expire check in world
    static bool HandleServerCorpsesCommand(ChatHandler* /*handler*/, char const* /*args*/)
    {
        sObjectAccessor->RemoveOldCorpses();
        return true;
    }

    static bool HandleServerInfoCommand(ChatHandler* p_Handler, char const* /*args*/)
    {
        uint32 l_ActiveClientsNum     = sWorld->GetActiveSessionCount();
        uint32 l_QueuedClientsNum     = sWorld->GetQueuedSessionCount();
        uint32 l_MaxActiveClientsNum  = sWorld->GetMaxActiveSessionCount();
        uint32 l_MaxQueuedClientsNum  = sWorld->GetMaxQueuedSessionCount();
        std::string l_Uptime          = secsToTimeString(sWorld->GetUptime());
        uint32 l_UpdateTime           = sWorld->GetUpdateTime();

        p_Handler->PSendSysMessage(GitRevision::GetFullVersion());
        p_Handler->PSendSysMessage(GitRevision::GetDate());
        p_Handler->PSendSysMessage(LANG_CONNECTED_USERS, l_ActiveClientsNum, l_MaxActiveClientsNum, l_QueuedClientsNum, l_MaxQueuedClientsNum);
        p_Handler->PSendSysMessage(LANG_UPTIME, l_Uptime.c_str());
        p_Handler->PSendSysMessage("Server delay: %u ms", l_UpdateTime);

        if (l_UpdateTime > 100)
        {
            p_Handler->PSendSysMessage("Global map manager diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_MAP));

            auto& l_MapsDelay = sMapMgr->GetMapsDelay();
            for (auto l_MapDelayRow : l_MapsDelay)
                p_Handler->PSendSysMessage("Map %u diff : %u ms", l_MapDelayRow.first, l_MapDelayRow.second);

            p_Handler->PSendSysMessage("Battleground diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_BATTLEGROUND));
            p_Handler->PSendSysMessage("Session diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_SESSION));
            p_Handler->PSendSysMessage("Battlefield diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_BATTLEFIELD));
            p_Handler->PSendSysMessage("Outdoor PVP diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_OUTDOORPVP));
            p_Handler->PSendSysMessage("LFG Mgr diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_LFG));
            p_Handler->PSendSysMessage("Callback diff : %u ms", sWorld->GetRecordDiff(RECORD_DIFF_CALLBACK));
        }

        // Can't use sWorld->ShutdownMsg here in case of console command
        if (sWorld->IsShuttingDown())
            p_Handler->PSendSysMessage(LANG_SHUTDOWN_TIMELEFT, secsToTimeString(sWorld->GetShutDownTimeLeft()).c_str());

        return true;
    }
    // Display the 'Message of the day' for the realm
    static bool HandleServerMotdCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->PSendSysMessage(LANG_MOTD_CURRENT, sWorld->GetMotd().Text.c_str());
        return true;
    }

    static bool HandleServerPLimitCommand(ChatHandler* handler, char const* args)
    {
        if (*args)
        {
            char* paramStr = strtok((char*)args, " ");
            if (!paramStr)
                return false;

            int32 limit = strlen(paramStr);

            if (strncmp(paramStr, "player", limit) == 0)
                sWorld->SetPlayerSecurityLimit(SEC_PLAYER);
            else if (strncmp(paramStr, "moderator", limit) == 0)
                sWorld->SetPlayerSecurityLimit(SEC_MODERATOR);
            else if (strncmp(paramStr, "gamemaster", limit) == 0)
                sWorld->SetPlayerSecurityLimit(SEC_GAMEMASTER);
            else if (strncmp(paramStr, "administrator", limit) == 0)
                sWorld->SetPlayerSecurityLimit(SEC_ADMINISTRATOR);
            else if (strncmp(paramStr, "reset", limit) == 0)
            {
                sWorld->SetPlayerAmountLimit(ConfigMgr::GetIntDefault("PlayerLimit", 100));
                sWorld->LoadDBAllowedSecurityLevel();
            }
            else
            {
                int32 value = atoi(paramStr);
                if (value < 0)
                    sWorld->SetPlayerSecurityLimit(AccountTypes(-value));
                else
                    sWorld->SetPlayerAmountLimit(uint32(value));
            }
        }

        uint32 playerAmountLimit = sWorld->GetPlayerAmountLimit();
        AccountTypes allowedAccountType = sWorld->GetPlayerSecurityLimit();
        char const* secName = "";
        switch (allowedAccountType)
        {
            case SEC_PLAYER:
                secName = "Player";
                break;
            case SEC_MODERATOR:
                secName = "Moderator";
                break;
            case SEC_GAMEMASTER:
                secName = "Gamemaster";
                break;
            case SEC_ADMINISTRATOR:
                secName = "Administrator";
                break;
            default:
                secName = "<unknown>";
                break;
        }
        handler->PSendSysMessage("Player limits: amount %u, min. security level %s.", playerAmountLimit, secName);

        return true;
    }

    static bool HandleServerShutDownCancelCommand(ChatHandler* /*handler*/, char const* /*args*/)
    {
        sWorld->ShutdownCancel();

        return true;
    }

    static bool HandleServerShutDownCommand(ChatHandler* /*handler*/, char const* args)
    {
        return ShutdownServer(args, 0, SHUTDOWN_EXIT_CODE);
    }

    static bool HandleServerRestartCommand(ChatHandler* /*handler*/, char const* args)
    {
        return ShutdownServer(args, SHUTDOWN_MASK_RESTART, RESTART_EXIT_CODE);
    }

    static bool HandleServerIdleRestartCommand(ChatHandler* /*handler*/, char const* args)
    {
        return ShutdownServer(args, SHUTDOWN_MASK_RESTART | SHUTDOWN_MASK_IDLE, RESTART_EXIT_CODE);
    }

    static bool HandleServerIdleShutDownCommand(ChatHandler* /*handler*/, char const* args)
    {
        return ShutdownServer(args, SHUTDOWN_MASK_IDLE, SHUTDOWN_EXIT_CODE);
    }

    // Exit the realm
    static bool HandleServerExitCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->SendSysMessage(LANG_COMMAND_EXIT);
        World::StopNow(SHUTDOWN_EXIT_CODE);
        return true;
    }

    // Set whether we accept new clients
    static bool HandleServerSetClosedCommand(ChatHandler* handler, char const* args)
    {
        if (strncmp(args, "on", 3) == 0)
        {
            handler->SendSysMessage(LANG_WORLD_CLOSED);
            sWorld->SetClosed(true);
            return true;
        }
        else if (strncmp(args, "off", 4) == 0)
        {
            handler->SendSysMessage(LANG_WORLD_OPENED);
            sWorld->SetClosed(false);
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }

    // Set the level of logging
    static bool HandleServerSetLogLevelCommand(ChatHandler* /*handler*/, char const* args)
    {
        if (!*args)
            return false;

        char* type = strtok((char*)args, " ");
        char* name = strtok(NULL, " ");
        char* level = strtok(NULL, " ");

        if (!type || !name || !level || *name == '\0' || *level == '\0' || (*type != 'a' && *type != 'l'))
            return false;

        sLog->SetLogLevel(name, level, *type == 'l');
        return true;
    }

    // set diff time record interval
    static bool HandleServerSetDiffTimeCommand(ChatHandler* /*handler*/, char const* args)
    {
        if (!*args)
            return false;

        char* newTimeStr = strtok((char*)args, " ");
        if (!newTimeStr)
            return false;

        int32 newTime = atoi(newTimeStr);
        if (newTime < 0)
            return false;

        sWorld->SetRecordDiffInterval(newTime);
        printf("Record diff every %d ms\n", newTime);

        return true;
    }

private:
    static bool ParseExitCode(std::string const& exitCodeStr, int32& exitCode)
    {
        exitCode = atoi(exitCodeStr.c_str());

        // Handle atoi() errors
        if (exitCode == 0 && (exitCodeStr[0] != '0' || exitCodeStr[1] != '\0'))
            return false;

        // Exit code should be in range of 0-125, 126-255 is used
        // in many shells for their own return codes and code > 255
        // is not supported in many others
        if (exitCode < 0 || exitCode > 125)
            return false;

        return true;
    }

    static bool ShutdownServer(char const* args, uint32 shutdownMask, int32 defaultExitCode)
    {
        if (!*args)
            return false;

        // #delay [#exit_code] [reason]
        std::regex regex("([0-9]+) ?([0-9]*) ?(.*)");
        std::cmatch cm;

        if (!std::regex_match(args, cm, regex))
            return false;

        std::string delayStr = cm[1];
        std::string exitCodeStr = cm[2];
        std::string reason = cm[3];

        int32 delay = atoi(delayStr.c_str());

        // Prevent interpret wrong arg value as 0 secs shutdown time
        if ((delay == 0 && (delayStr[0] != '0' || delayStr[1] != '\0')) || delay < 0)
            return false;

        int32 exitCode = defaultExitCode;
        if (exitCodeStr.length() > 0)
            if (!ParseExitCode(exitCodeStr, exitCode))
                return false;

        sWorld->ShutdownServ(delay, shutdownMask, static_cast<uint8>(exitCode), reason);

        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_server_commandscript()
{
    new server_commandscript();
}
#endif
