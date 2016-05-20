////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "SpellLogMgr.h"

class spellog_commandscript: public CommandScript
{
public:
    spellog_commandscript() : CommandScript("spelllog_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand setCommandTable[] =
        {
            { nullptr, 0, false, nullptr,                                                       "", nullptr }
        };

        static ChatCommand spelllogCommandTable[] =
        {
            { "enable",   SEC_ADMINISTRATOR,  true,  &HandleSpellogEnableCommand,               "", nullptr },
            { "disale",   SEC_ADMINISTRATOR,  true,  &HandleSpellogDisableCommand,              "", nullptr },
            { "log",      SEC_ADMINISTRATOR,  true,  &HandleSpelllogLogCommand,                 "", nullptr },
            { "unlog",    SEC_ADMINISTRATOR,  true,  &HandleSpelllogUnlogCommand,               "", nullptr },
            { "set",      SEC_ADMINISTRATOR,  true,  nullptr,                           "", setCommandTable },
            { nullptr, 0, false, nullptr,                                                       "", nullptr }
        };

        static ChatCommand commandTable[] =
        {
            { "spelllog",          SEC_ADMINISTRATOR,  true, nullptr,              "", spelllogCommandTable },
            { nullptr,             0,                  false, nullptr,                          "", nullptr }
        };

        return commandTable;
    }

    static bool HandleSpellogEnableCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        sSpellLogMgr->AddOrSetListener(p_Handler->GetSession()->GetPlayer(), 0);
        return true;
    }

    static bool HandleSpellogDisableCommand(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        sSpellLogMgr->RemoveListener(p_Handler->GetSession()->GetPlayer());
        return true;
    }

    static bool HandleSpelllogLogCommand(ChatHandler* p_Handler, char const* p_Args)
    {
        if (!p_Args)
            return false;

        int32 l_SpellID = atoi(p_Args);

        if (!sSpellMgr->GetSpellInfo(l_SpellID))
            return false;

        sSpellLogMgr->AddOrSetListener(p_Handler->GetSession()->GetPlayer(), l_SpellID);
        return true;
    }

    static bool HandleSpelllogUnlogCommand(ChatHandler* p_Handler, char const* p_Args)
    {
        if (!p_Args)
            return false;

        int32 l_SpellID = atoi(p_Args);

        if (!sSpellMgr->GetSpellInfo(l_SpellID))
            return false;

        sSpellLogMgr->RemoveSpellFromListener(p_Handler->GetSession()->GetPlayer(), l_SpellID);
        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_spellog_commandscript()
{
    new spellog_commandscript();
}
#endif
