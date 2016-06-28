////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: guild_commandscript
%Complete: 100
Comment: All guild related commands
Category: commandscripts
EndScriptData */

#ifndef CROSS
#include "ScriptMgr.h"
#include "Chat.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "ObjectAccessor.h"

class guild_commandscript: public CommandScript
{
    public:
        guild_commandscript() : CommandScript("guild_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand guildCommandTable[] =
            {
                { "create",             SEC_GAMEMASTER,     true,  &HandleGuildCreateCommand,            "", NULL },
                { "delete",             SEC_GAMEMASTER,     true,  &HandleGuildDeleteCommand,            "", NULL },
                { "invite",             SEC_GAMEMASTER,     true,  &HandleGuildInviteCommand,            "", NULL },
                { "uninvite",           SEC_GAMEMASTER,     true,  &HandleGuildUninviteCommand,          "", NULL },
                { "rank",               SEC_GAMEMASTER,     true,  &HandleGuildRankCommand,              "", NULL },
                { "rename",             SEC_GAMEMASTER,     true,  &HandleGuildRenameCommand,            "", NULL },
                { "challengecomplete",  SEC_ADMINISTRATOR,  true,  &HandleGuildChallengeCompleteCommand,   "", NULL },
                { NULL,                 0,                  false, NULL,                                 "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "guild",          SEC_ADMINISTRATOR,  true, NULL,                                 "", guildCommandTable },
                { NULL,             0,                  false, NULL,                                "", NULL }
            };
            return commandTable;
        }

        /** \brief GM command level 3 - Create a guild.
         *
         * This command allows a GM (level 3) to create a guild.
         *
         * The "args" parameter contains the name of the guild leader
         * and then the name of the guild.
         *
         */
        static bool HandleGuildCreateCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            // if not guild name only (in "") then player name
            Player* target;
            if (!handler->extractPlayerTarget(*args != '"' ? (char*)args : NULL, &target))
                return false;

            char* tailStr = *args != '"' ? strtok(NULL, "") : (char*)args;
            if (!tailStr)
                return false;

            char* guildStr = handler->extractQuotedArg(tailStr);
            if (!guildStr)
                return false;

            std::string guildName = guildStr;

            if (target->GetGuildId())
            {
                handler->SendSysMessage(LANG_PLAYER_IN_GUILD);
                return true;
            }

            Guild* guild = new Guild;
            if (!guild->Create(target, guildName))
            {
                delete guild;
                handler->SendSysMessage(LANG_GUILD_NOT_CREATED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            sGuildMgr->AddGuild(guild);

            return true;
        }

        static bool HandleGuildDeleteCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* guildStr = handler->extractQuotedArg((char*)args);
            if (!guildStr)
                return false;

            std::string guildName = guildStr;

            Guild* targetGuild = sGuildMgr->GetGuildByName(guildName);
            if (!targetGuild)
                return false;

            targetGuild->Disband();
            delete targetGuild;

            return true;
        }

        static bool HandleGuildInviteCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            // if not guild name only (in "") then player name
            uint64 targetGuid;
            if (!handler->extractPlayerTarget(*args != '"' ? (char*)args : NULL, NULL, &targetGuid))
                return false;

            char* tailStr = *args != '"' ? strtok(NULL, "") : (char*)args;
            if (!tailStr)
                return false;

            char* guildStr = handler->extractQuotedArg(tailStr);
            if (!guildStr)
                return false;

            std::string guildName = guildStr;
            Guild* targetGuild = sGuildMgr->GetGuildByName(guildName);
            if (!targetGuild)
                return false;

            // player's guild membership checked in AddMember before add
            return targetGuild->AddMember(targetGuid);
        }

        static bool HandleGuildUninviteCommand(ChatHandler* handler, char const* args)
        {
            Player* target;
            uint64 targetGuid;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid))
                return false;

            uint32 guildId = target ? target->GetGuildId() : Player::GetGuildIdFromDB(targetGuid);
            if (!guildId)
                return false;

            Guild* targetGuild = sGuildMgr->GetGuildById(guildId);
            if (!targetGuild)
                return false;

            targetGuild->DeleteMember(targetGuid, false, true, true);
            return true;
        }

        static bool HandleGuildRankCommand(ChatHandler* handler, char const* args)
        {
            char* nameStr;
            char* rankStr;
            handler->extractOptFirstArg((char*)args, &nameStr, &rankStr);
            if (!rankStr)
                return false;

            Player* target;
            uint64 targetGuid;
            std::string target_name;
            if (!handler->extractPlayerTarget(nameStr, &target, &targetGuid, &target_name))
                return false;

            uint32 guildId = target ? target->GetGuildId() : Player::GetGuildIdFromDB(targetGuid);
            if (!guildId)
                return false;

            Guild* targetGuild = sGuildMgr->GetGuildById(guildId);
            if (!targetGuild)
                return false;

            uint8 newRank = uint8(atoi(rankStr));
           return targetGuild->ChangeMemberRank(targetGuid, newRank);
        }

        static bool HandleGuildRenameCommand(ChatHandler* handler, char const* _args)
        {
            if (!*_args || !handler)
                return false;

            char* tailStr = *_args != '"' ? strtok(NULL, "") : (char*)_args;
            if (!tailStr)
                return false;

            char const* oldGuildStr = handler->extractQuotedArg(tailStr);
            if (!oldGuildStr)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char const* newGuildStr = handler->extractQuotedArg(strtok(NULL, ""));
            if (!newGuildStr)
            {
                handler->SendSysMessage(LANG_INSERT_GUILD_NAME);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Guild* guild = sGuildMgr->GetGuildByName(oldGuildStr);
            if (!guild)
            {
                handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, oldGuildStr);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (sGuildMgr->GetGuildByName(newGuildStr))
            {
                handler->PSendSysMessage(LANG_GUILD_RENAME_ALREADY_EXISTS, newGuildStr);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!guild->SetName(newGuildStr))
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            handler->PSendSysMessage(LANG_GUILD_RENAME_DONE, oldGuildStr, newGuildStr);
            return true;
        }

        static bool HandleGuildChallengeCompleteCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* x = strtok((char*)args, " ");

            uint32 l_ChallengeType = (uint32)atoi(x);

            WorldObject* target = handler->getSelectedObject();

            Player* l_Player = target->ToPlayer();
            if (!l_Player)
                return false;

            uint32 guildId = l_Player->GetGuildId();
            if (!guildId)
                return false;

            Guild* targetGuild = sGuildMgr->GetGuildById(guildId);
            if (!targetGuild)
                return false;

            switch (l_ChallengeType)
            {
                case ChallengeNone:
                    break;
                case ChallengeDungeon:
                    targetGuild->CompleteGuildChallenge(ChallengeDungeon);
                    return true;
                case ChallengeScenario:
                    break;
                case ChallengeDungeonChallenge:
                    targetGuild->CompleteGuildChallenge(ChallengeDungeonChallenge);
                    return true;
                case ChallengeRaid:
                    targetGuild->CompleteGuildChallenge(ChallengeRaid);
                    return true;
                case ChallengeRatedBG:
                    targetGuild->CompleteGuildChallenge(ChallengeRatedBG);
                    return true;
                default:
                    break;
            }

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_guild_commandscript()
{
    new guild_commandscript();
}
#endif
#endif
