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
                { "start",              SEC_GAMEMASTER,     false, &HandleConversationStart,            "", NULL },
                { "actor",              SEC_GAMEMASTER,     false, &HandleConversationActor,            "", NULL },
                { NULL,                 0,                  false, NULL,                                "", NULL }
            };

            static ChatCommand m_CommandTable[] =
            {
                { "conv",               SEC_GAMEMASTER,     false, NULL,                                "", m_ConvCommandTable },
                { NULL,                 0,                  false, NULL,                                "", NULL }
            };

            return m_CommandTable;
        }

        static uint32 GetConversationLowGUID(ChatHandler* p_Handler, char const* p_Args)
        {
            if (*p_Args)
            {
                // number or [name] Shift-click form |color|Hconversation:conv_guid|h[name]|h|r
                char* l_ID = p_Handler->extractKeyFromLink((char*)p_Args, "Hconversation");
                if (!l_ID)
                    return 0;

                return atoi(l_ID);
            }

            return 0;
        }

        static bool HandleConversationStart(ChatHandler* p_Handler, char const* p_Args)
        {
            if (p_Args == nullptr)
                return false;

            Unit* l_Target = p_Handler->getSelectedUnit();
            if (!l_Target || !p_Handler->GetSession()->GetPlayer()->GetSelection())
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_SELECT_CHAR_OR_CREATURE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_Value = strtok(nullptr, " ");
            if (!l_Value)
                return false;

            uint32 l_Entry = atoi(l_Value);
            if (l_Entry == 0)
                return false;

            l_Value = strtok(nullptr, " ");
            if (!l_Value)
                return false;

            uint32 l_Duration = atoi(l_Value);
            if (l_Entry == 0)
                return false;

            Conversation* l_Conversation = new Conversation;
            if (!l_Conversation->StartConversation(l_Target, l_Entry, l_Duration))
            {
                delete l_Conversation;
                p_Handler->PSendSysMessage("Error while creating conversation...");
                return false;
            }

            return true;
        }

        static bool HandleConversationActor(ChatHandler* p_Handler, char const* p_Args)
        {
            Unit* l_Target = p_Handler->getSelectedUnit();
            if (!l_Target || !p_Handler->GetSession()->GetPlayer()->GetSelection())
            {
                p_Handler->SendSysMessage(TrinityStrings::LANG_SELECT_CHAR_OR_CREATURE);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_conversation_commandscript()
{
    new conversation_commandscript();
}
#endif
