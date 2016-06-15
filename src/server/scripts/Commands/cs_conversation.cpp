////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "TicketMgr.h"
#include "Chat.h"

class conversation_commandscript: public CommandScript
{
    public:
        conversation_commandscript() : CommandScript("conversation_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand m_ConvCommandTable[] =
            {
                { NULL,             0,                  false, NULL,                              "", NULL }
            };

            static ChatCommand m_CommandTable[] =
            {
                { "conv",             SEC_MODERATOR,      false, NULL,                               "", m_ConvCommandTable },
                { NULL,               0,                  false, NULL,                               "", NULL }
            };

            return m_CommandTable;
        }
};

#ifndef __clang_analyzer__
void AddSC_conversation_commandscript()
{
    new conversation_commandscript();
}
#endif
