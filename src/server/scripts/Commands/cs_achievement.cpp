////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: achievement_commandscript
%Complete: 100
Comment: All achievement related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"

class achievement_commandscript: public CommandScript
{
public:
    achievement_commandscript() : CommandScript("achievement_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand achievementCommandTable[] =
        {
            { "add",            SEC_ADMINISTRATOR,  false,  &HandleAchievementAddCommand,            "", NULL },
            { "generatereward", SEC_ADMINISTRATOR,  false,  &HandleAchievementGenerateRewardCommand, "", NULL },
            { NULL,             0,                  false,  NULL,                                    "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "achievement",    SEC_ADMINISTRATOR,  false, NULL,            "", achievementCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleAchievementAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 achievementId = atoi((char*)args);
        if (!achievementId)
        {
            if (char* id = handler->extractKeyFromLink((char*)args, "Hachievement"))
                achievementId = atoi(id);
            if (!achievementId)
                return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievementId))
            target->CompletedAchievement(achievementEntry);

        return true;
    }

    static bool HandleAchievementGenerateRewardCommand(ChatHandler* /*p_ChatHandler*/, char const* /*p_Args*/)
    {
        FILE* l_File = fopen("AchievementReward.sql", "w+");
        if (l_File == nullptr)
            return false;

        for (uint32 l_I = 0; l_I < sAchievementStore.GetNumRows(); l_I++)
        {
            auto l_AchievementEntry = sAchievementStore.LookupEntry(l_I);
            if (l_AchievementEntry == nullptr)
                continue;

            std::string l_RewardText = l_AchievementEntry->RewardLang;
            if (l_RewardText.empty())
                continue;


            /// - Handle title
            std::string l_Match = "Title Reward: ";
            if (l_RewardText.find(l_Match) != std::string::npos)
            {
                std::string l_TitleNameToMatch = l_RewardText.substr(l_Match.size());

                for (uint32 l_TitleIdx = 0; l_TitleIdx < sCharTitlesStore.GetNumRows(); l_TitleIdx++)
                {
                    auto l_TitleEntry = sCharTitlesStore.LookupEntry(l_TitleIdx);
                    if (l_TitleEntry == nullptr)
                        continue;

                    std::string l_TitleName = l_TitleEntry->NameLang;
                    if (l_TitleName.find(l_TitleNameToMatch) != std::string::npos)
                    {
                        std::ostringstream l_Query;
                        l_Query << "INSERT INTO achievement_reward (entry, title_A, title_H) VALUES (" << l_AchievementEntry->ID << ", " << l_TitleEntry->ID << ", " << l_TitleEntry->ID << ");\r\n";
                        fputs(l_Query.str().c_str(), l_File);
                        break;
                    }
                }
            }
        }

        fflush(l_File);
        fclose(l_File);

        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_achievement_commandscript()
{
    new achievement_commandscript();
}
#endif