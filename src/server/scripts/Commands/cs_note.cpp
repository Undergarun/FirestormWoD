////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"

class note_commandscript: public CommandScript
{
    public:
        note_commandscript() : CommandScript("note_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand l_Notetable[] =
            {
                { "write",          SEC_ADMINISTRATOR,  true,  &HandleAddNoteAccountCommand,    "", NULL },
                { "list",           SEC_ADMINISTRATOR,  true,  &HandleListNoteAccountCommand,   "", NULL },
                { NULL,             0,                  false, NULL,                            "", NULL }
            };
            static ChatCommand l_CommandTable[] =
            {
                { "note",           SEC_ADMINISTRATOR,  true,  NULL,                            "", l_Notetable },
                { NULL,             0,                  false, NULL,                            "", NULL }
            };
            return l_CommandTable;
        }

        static bool HandleAddNoteAccountCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            return HandleAddNoteHelper(p_Args, p_Handler);
        }

        static bool HandleAddNoteHelper(char const* p_Args, ChatHandler* p_Handler)
        {
#ifndef CROSS
            if (!*p_Args)
                return false;

            char* l_CnameOrIP = strtok((char*)p_Args, " ");
            if (!l_CnameOrIP)
                return false;

            std::string l_AccountName = l_CnameOrIP;

            char* l_Note = strtok(NULL, "");
            if (!l_Note)
            {
                p_Handler->PSendSysMessage("Note is missing");
                return false;
            }

            uint32 l_AccountID = AccountMgr::GetId(l_AccountName);
            if (!l_AccountID)
            {
                p_Handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, l_AccountName.c_str());
                return true;
            }

            PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_NOTE);

            if (l_Stmt == nullptr)
                return false;

            WorldSession* l_Session = p_Handler->GetSession();
            std::string l_GMIdentification;
            if (l_Session == nullptr)
                l_GMIdentification = "<none>";
            else
            {
                if (Player* l_Player = l_Session->GetPlayer())
                    l_GMIdentification = l_Player->GetName();
                else
                    l_GMIdentification = "<none>";
            }

            l_GMIdentification += "(" + sWorld->GetRealmName() + ")";
            const char* l_NotedBy = l_GMIdentification.c_str();
            l_Stmt->setUInt32(0, l_AccountID);
            l_Stmt->setString(1, l_Note);
            l_Stmt->setString(2, l_NotedBy);
            LoginDatabase.Execute(l_Stmt);
            p_Handler->PSendSysMessage("Note has been added");
#endif /* not CROSS */
            return true;
        }

        static bool HandleListNoteAccountCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            return HandleListNoteHelper(p_Args, p_Handler);
        }

        static bool HandleListNoteHelper(char const* p_Args, ChatHandler* p_Handler)
        {
#ifndef CROSS
            if (!*p_Args)
                return false;

            char* l_CnameOrIP = strtok((char*)p_Args, " ");
            if (!l_CnameOrIP)
                return false;

            std::string l_AccountName = l_CnameOrIP;

            uint32 l_AccountID = AccountMgr::GetId(l_AccountName);
            if (!l_AccountID)
            {
                p_Handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, l_AccountName.c_str());
                return true;
            }

            PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_NOTE);

            if (l_Stmt == nullptr)
                return false;

            l_Stmt->setUInt32(0, l_AccountID);
            PreparedQueryResult l_Result = LoginDatabase.Query(l_Stmt);


            p_Handler->PSendSysMessage("=====Notes====");
            do
            {
                Field* l_Fields = l_Result->Fetch();

                if (l_Fields == nullptr)
                    return false;

                time_t l_Time = time_t(l_Fields[2].GetUInt32());
                tm* l_Tm = localtime(&l_Time);

                if (l_Tm == nullptr)
                    return false;

                p_Handler->PSendSysMessage("%02d-%02d-%02d %02d:%02d by %s", 1900 + l_Tm->tm_year, l_Tm->tm_mon + 1, l_Tm->tm_mday, l_Tm->tm_hour, l_Tm->tm_min, l_Fields[3].GetCString());
                p_Handler->PSendSysMessage("%s", l_Fields[1].GetCString());
            } while (l_Result->NextRow());
            p_Handler->PSendSysMessage("==============");

#endif /* not CROSS */
            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_note_commandscript()
{
    new note_commandscript();
}
#endif
