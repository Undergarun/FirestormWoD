////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: ticket_commandscript
%Complete: 100
Comment: All ticket related commands
Category: commandscripts
EndScriptData */

#ifndef CROSS
#include "ScriptMgr.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ObjectMgr.h"
#include "TicketMgr.h"

class ticket_commandscript: public CommandScript
{
public:
    ticket_commandscript() : CommandScript("ticket_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand ticketResponseCommandTable[] =
        {
            { "append",         SEC_MODERATOR,      true,  &HandleGMTicketResponseAppendCommand,    "", NULL },
            { "appendln",       SEC_MODERATOR,      true,  &HandleGMTicketResponseAppendLnCommand,  "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };
        static ChatCommand ticketCommandTable[] =
        {
            { "assign",         SEC_GAMEMASTER,     true,  &HandleGMTicketAssignToCommand,          "", NULL },
            { "close",          SEC_MODERATOR,      true,  &HandleGMTicketCloseByIdCommand,         "", NULL },
            { "closedlist",     SEC_MODERATOR,      true,  &HandleGMTicketListClosedCommand,        "", NULL },
            { "comment",        SEC_MODERATOR,      true,  &HandleGMTicketCommentCommand,           "", NULL },
            { "complete",       SEC_MODERATOR,      true,  &HandleGMTicketCompleteCommand,          "", NULL },
            { "delete",         SEC_ADMINISTRATOR,  true,  &HandleGMTicketDeleteByIdCommand,        "", NULL },
            { "escalate",       SEC_MODERATOR,      true,  &HandleGMTicketEscalateCommand,          "", NULL },
            { "escalatedlist",  SEC_GAMEMASTER,     true,  &HandleGMTicketListEscalatedCommand,     "", NULL },
            { "list",           SEC_MODERATOR,      true,  &HandleGMTicketListCommand,              "", NULL },
            { "onlinelist",     SEC_MODERATOR,      true,  &HandleGMTicketListOnlineCommand,        "", NULL },
            { "reset",          SEC_ADMINISTRATOR,  true,  &HandleGMTicketResetCommand,             "", NULL },
            { "response",       SEC_MODERATOR,      true,  NULL,                                    "", ticketResponseCommandTable },
            { "togglesystem",   SEC_ADMINISTRATOR,  true,  &HandleToggleGMTicketSystem,             "", NULL },
            { "unassign",       SEC_GAMEMASTER,     true,  &HandleGMTicketUnAssignCommand,          "", NULL },
            { "viewid",         SEC_MODERATOR,      true,  &HandleGMTicketGetByIdCommand,           "", NULL },
            { "viewname",       SEC_MODERATOR,      true,  &HandleGMTicketGetByNameCommand,         "", NULL },
            { "fsccreate",      SEC_PLAYER,         true,  &FSCHandleGMTicketCreate,                "", NULL },
            { "fscupdate",      SEC_PLAYER,         true,  &FSCHandleGMTicketUpdate,                "", NULL },
            { "fscappend",      SEC_PLAYER,         true,  &FSCHandleGMTicketAppend,                "", NULL },
            { "fscend",         SEC_PLAYER,         true,  &FSCHandleGMTicketEnd,                "", NULL },
            { "fscdelete",      SEC_PLAYER,         true,  &FSCHandleGMTicketDelete,                "", NULL },
            { "fscticketget",   SEC_PLAYER,         true,  &FSCHandleGMTicketGet,                   "", NULL },
            { "fscticketres",   SEC_PLAYER,         true,  &FSCHandleGMResponseResolve,             "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "ticket",         SEC_PLAYER,         false, NULL,                                    "", ticketCommandTable },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };
        return commandTable;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    static bool FSCHandleGMTicketCreate(ChatHandler* p_Handler, char const* p_Args)
    {
        auto l_ReplaceAll = [](std::string& str, const std::string& from, const std::string& to) {
            if (from.empty())
                return;
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        };

        // Don't accept tickets if the ticket queue is disabled. (Ticket UI is greyed out but not fully dependable)
        if (sTicketMgr->GetStatus() == GMTICKET_QUEUE_STATUS_DISABLED)
            return true;

        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        if (l_Player->getLevel() < sWorld->getIntConfig(CONFIG_TICKET_LEVEL_REQ))
        {
            p_Handler->GetSession()->SendNotification(p_Handler->GetSession()->GetTrinityString(LANG_TICKET_REQ), sWorld->getIntConfig(CONFIG_TICKET_LEVEL_REQ));
            return true;
        }

        std::string l_Message = p_Args;
        l_ReplaceAll(l_Message, "$$n", "\n");

        GMTicketResponse l_Response = GMTICKET_RESPONSE_CREATE_ERROR;
        // Player must not have opened ticket
        if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(l_Player->GetGUID()))
        {
            if (l_Ticket->IsCompleted())
            {
                sTicketMgr->CloseTicket(l_Ticket->GetId(), l_Player->GetGUID());
                sTicketMgr->SendTicket(p_Handler->GetSession(), NULL);

                p_Handler->GetSession()->SendTicketStatusUpdate(GMTICKET_RESPONSE_TICKET_DELETED);

                GmTicket* l_NewTicket = new GmTicket(l_Player->GetName(), l_Player->GetGUID(), l_Player->GetMapId(), *l_Player, l_Message);
                sTicketMgr->AddTicket(l_NewTicket);
                sTicketMgr->UpdateLastChange();

                sWorld->SendGMText(LANG_COMMAND_TICKETNEW, l_Player->GetName(), l_NewTicket->GetId());

                //sTicketMgr->SendTicket(p_Handler->GetSession(), l_NewTicket);

                l_Response = GMTICKET_RESPONSE_CREATE_SUCCESS;
            }
            else
                l_Response = GMTICKET_RESPONSE_ALREADY_EXIST;
        }
        else
        {
            GmTicket* l_NewTicket = new GmTicket(l_Player->GetName(), l_Player->GetGUID(), l_Player->GetMapId(), *l_Player, l_Message);
            sTicketMgr->AddTicket(l_NewTicket);
            sTicketMgr->UpdateLastChange();

            sWorld->SendGMText(LANG_COMMAND_TICKETNEW, l_Player->GetName(), l_NewTicket->GetId());

            //sTicketMgr->SendTicket(p_Handler->GetSession(), l_NewTicket);

            l_Response = GMTICKET_RESPONSE_CREATE_SUCCESS;
        }

        p_Handler->GetSession()->SendTicketStatusUpdate(l_Response);
        return true;
    }

    static bool FSCHandleGMTicketUpdate(ChatHandler* p_Handler, char const* p_Args)
    {
        auto l_ReplaceAll = [](std::string& str, const std::string& from, const std::string& to) {
            if (from.empty())
                return;
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        };

        // Don't accept tickets if the ticket queue is disabled. (Ticket UI is greyed out but not fully dependable)
        if (sTicketMgr->GetStatus() == GMTICKET_QUEUE_STATUS_DISABLED)
            return true;

        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        std::string l_Message = p_Args;
        l_ReplaceAll(l_Message, "$$n", "\n");

        GMTicketResponse l_Response = GMTICKET_RESPONSE_UPDATE_ERROR;
        if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(l_Player->GetGUID()))
        {
            SQLTransaction l_Transaction = SQLTransaction(NULL);
            l_Ticket->SetMessage(l_Message);
            l_Ticket->SaveToDB(l_Transaction);

            sWorld->SendGMText(LANG_COMMAND_TICKETUPDATED, l_Player->GetName(), l_Ticket->GetId());

            sTicketMgr->SendTicket(p_Handler->GetSession(), l_Ticket);
            l_Response = GMTICKET_RESPONSE_UPDATE_SUCCESS;
        }

        p_Handler->GetSession()->SendTicketStatusUpdate(l_Response);

        return true;
    }

    static bool FSCHandleGMTicketAppend(ChatHandler* p_Handler, char const* p_Args)
    {
        auto l_ReplaceAll = [](std::string& str, const std::string& from, const std::string& to) {
            if (from.empty())
                return;
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        };

        // Don't accept tickets if the ticket queue is disabled. (Ticket UI is greyed out but not fully dependable)
        if (sTicketMgr->GetStatus() == GMTICKET_QUEUE_STATUS_DISABLED)
            return true;

        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        std::string l_Message = p_Args;
        l_ReplaceAll(l_Message, "$$n", "\n");

        GMTicketResponse l_Response = GMTICKET_RESPONSE_UPDATE_ERROR;
        if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(l_Player->GetGUID()))
        {
            SQLTransaction l_Transaction = SQLTransaction(NULL);
            l_Ticket->SetMessage(l_Ticket->GetMessage() + l_Message);
            l_Ticket->SaveToDB(l_Transaction);
        }

        p_Handler->GetSession()->SendTicketStatusUpdate(l_Response);

        return true;
    }
    
    static bool FSCHandleGMTicketEnd(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        // Don't accept tickets if the ticket queue is disabled. (Ticket UI is greyed out but not fully dependable)
        if (sTicketMgr->GetStatus() == GMTICKET_QUEUE_STATUS_DISABLED)
            return true;

        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        GMTicketResponse l_Response = GMTICKET_RESPONSE_UPDATE_ERROR;
        if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(l_Player->GetGUID()))
        {
            sTicketMgr->SendTicket(p_Handler->GetSession(), l_Ticket);
        }

        return true;
    }

    static bool FSCHandleGMTicketDelete(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(l_Player->GetGUID()))
        {
            p_Handler->GetSession()->SendTicketStatusUpdate(GMTICKET_RESPONSE_TICKET_DELETED);

            sWorld->SendGMText(LANG_COMMAND_TICKETPLAYERABANDON, l_Player->GetName(), l_Ticket->GetId());

            sTicketMgr->CloseTicket(l_Ticket->GetId(), l_Player->GetGUID());
            sTicketMgr->SendTicket(p_Handler->GetSession(), NULL);
        }

        return true;
    }

    static bool FSCHandleGMTicketGet(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        p_Handler->GetSession()->OnGMTicketGetTicketEvent();

        return true;
    }

    static bool FSCHandleGMResponseResolve(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        Player* l_Player = p_Handler->GetSession()->GetPlayer();

        if (!l_Player)
            return true;

        if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(l_Player->GetGUID()))
        {
            p_Handler->GetSession()->SendTicketStatusUpdate(GMTICKET_RESPONSE_TICKET_DELETED);

            sTicketMgr->CloseTicket(l_Ticket->GetId(), l_Player->GetGUID());
            sTicketMgr->SendTicket(p_Handler->GetSession(), NULL);
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    static bool HandleGMTicketAssignToCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* ticketIdStr = strtok((char*)args, " ");
        uint32 ticketId = atoi(ticketIdStr);

        char* targetStr = strtok(NULL, " ");
        if (!targetStr)
            return false;

        std::string target(targetStr);
        if (!normalizePlayerName(target))
            return false;

        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        // Get target information
        uint64 targetGuid = sObjectMgr->GetPlayerGUIDByName(target.c_str());
        uint64 targetAccountId = sObjectMgr->GetPlayerAccountIdByGUID(targetGuid);
        uint32 targetGmLevel = AccountMgr::GetSecurity(targetAccountId, g_RealmID);

        // Target must exist and have administrative rights
        if (!targetGuid || AccountMgr::IsPlayerAccount(targetGmLevel))
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETASSIGNERROR_A);
            return true;
        }

        // If already assigned, leave
        if (ticket->IsAssignedTo(targetGuid))
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETASSIGNERROR_B, ticket->GetId());
            return true;
        }

        // If assigned to different player other than current, leave
        //! Console can override though
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        if (player && ticket->IsAssignedNotTo(player->GetGUID()))
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETALREADYASSIGNED, ticket->GetId(), target.c_str());
            return true;
        }

        // Assign ticket
        SQLTransaction trans = SQLTransaction(NULL);
        ticket->SetAssignedTo(targetGuid, AccountMgr::IsAdminAccount(targetGmLevel));
        ticket->SaveToDB(trans);
        sTicketMgr->UpdateLastChange();

        std::string msg = ticket->FormatMessageString(*handler, NULL, target.c_str(), NULL, NULL);
        handler->SendGlobalGMSysMessage(msg.c_str());
        return true;
    }

    static bool HandleGMTicketCloseByIdCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 ticketId = atoi(args);
        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed() || ticket->IsCompleted())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        // Ticket should be assigned to the player who tries to close it.
        // Console can override though
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        if (player && ticket->IsAssignedNotTo(player->GetGUID()))
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETCANNOTCLOSE, ticket->GetId());
            return true;
        }

        sTicketMgr->CloseTicket(ticket->GetId(), player ? player->GetGUID() : -1);
        sTicketMgr->UpdateLastChange();

        std::string msg = ticket->FormatMessageString(*handler, player ? player->GetName() : "Console", NULL, NULL, NULL);
        handler->SendGlobalGMSysMessage(msg.c_str());

        /*
        // Inform player, who submitted this ticket, that it is closed
        if (Player* submitter = ticket->GetPlayer())
        {
            if (submitter->IsInWorld())
                submitter->GetSession()->SendTicketStatusUpdate(GMTICKET_RESPONSE_TICKET_DELETED);
        }
        */

        return true;
    }

    static bool HandleGMTicketCommentCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* ticketIdStr = strtok((char*)args, " ");
        uint32 ticketId = atoi(ticketIdStr);

        char* comment = strtok(NULL, "\n");
        if (!comment)
            return false;

        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed())
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        // Cannot comment ticket assigned to someone else
        //! Console excluded
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        if (player && ticket->IsAssignedNotTo(player->GetGUID()))
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETALREADYASSIGNED, ticket->GetId());
            return true;
        }

        SQLTransaction trans = SQLTransaction(NULL);
        ticket->SetComment(comment);
        ticket->SaveToDB(trans);
        sTicketMgr->UpdateLastChange();

        std::string msg = ticket->FormatMessageString(*handler, NULL, ticket->GetAssignedToName().c_str(), NULL, NULL);
        msg += handler->PGetParseString(LANG_COMMAND_TICKETLISTADDCOMMENT, player ? player->GetName() : "Console", comment);
        handler->SendGlobalGMSysMessage(msg.c_str());

        return true;
    }

    static bool HandleGMTicketListClosedCommand(ChatHandler* handler, char const* /*args*/)
    {
        sTicketMgr->ShowClosedList(*handler);
        return true;
    }

    static bool HandleGMTicketCompleteCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 ticketId = atoi(args);
        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed() || ticket->IsCompleted())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        ticket->SetCompleted(true);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        ticket->SaveToDB(trans);
        CharacterDatabase.CommitTransaction(trans);

        if (Player* player = ticket->GetPlayer())
            if (player->IsInWorld())
                ticket->SendResponse(player->GetSession());

        sTicketMgr->UpdateLastChange();
        return true;
    }

    static bool HandleGMTicketDeleteByIdCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 ticketId = atoi(args);
        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket)
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        if (!ticket->IsClosed())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETCLOSEFIRST);
            return true;
        }

        std::string msg = ticket->FormatMessageString(*handler, NULL, NULL, NULL, handler->GetSession() ? handler->GetSession()->GetPlayer()->GetName() : "Console");
        handler->SendGlobalGMSysMessage(msg.c_str());

        /*
        if (Player* player = ticket->GetPlayer())
        {
            if (player->IsInWorld())
                player->GetSession()->SendTicketStatusUpdate(GMTICKET_RESPONSE_TICKET_DELETED);
        }
        */

        sTicketMgr->RemoveTicket(ticket->GetId());
        sTicketMgr->UpdateLastChange();

        return true;
    }

    static bool HandleGMTicketEscalateCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 ticketId = atoi(args);
        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed() || ticket->IsCompleted() || ticket->GetEscalatedStatus() != TICKET_UNASSIGNED)
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        ticket->SetEscalatedStatus(TICKET_IN_ESCALATION_QUEUE);

        if (Player* player = ticket->GetPlayer())
            if (player->IsInWorld())
                sTicketMgr->SendTicket(player->GetSession(), ticket);

        sTicketMgr->UpdateLastChange();
        return true;
    }

    static bool HandleGMTicketListEscalatedCommand(ChatHandler* handler, char const* /*args*/)
    {
        sTicketMgr->ShowEscalatedList(*handler);
        return true;
    }

    static bool HandleGMTicketListCommand(ChatHandler* handler, char const* /*args*/)
    {
        sTicketMgr->ShowList(*handler, false);
        return true;
    }

    static bool HandleGMTicketListOnlineCommand(ChatHandler* handler, char const* /*args*/)
    {
        sTicketMgr->ShowList(*handler, true);
        return true;
    }

    static bool HandleGMTicketResetCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (sTicketMgr->GetOpenTicketCount())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETPENDING);
            return true;
        }
        else
        {
            sTicketMgr->ResetTickets();
            handler->SendSysMessage(LANG_COMMAND_TICKETRESET);
        }

        return true;
    }

    static bool HandleToggleGMTicketSystem(ChatHandler* handler, char const* /*args*/)
    {
        bool status = !sTicketMgr->GetStatus();
        sTicketMgr->SetStatus(status);
        handler->PSendSysMessage(status ? LANG_ALLOW_TICKETS : LANG_DISALLOW_TICKETS);
        return true;
    }

    static bool HandleGMTicketUnAssignCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 ticketId = atoi(args);
        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }
        // Ticket must be assigned
        if (!ticket->IsAssigned())
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETNOTASSIGNED, ticket->GetId());
            return true;
        }

        // Get security level of player, whom this ticket is assigned to
        uint32 security = SEC_PLAYER;
        Player* assignedPlayer = ticket->GetAssignedPlayer();
        if (assignedPlayer && assignedPlayer->IsInWorld())
            security = assignedPlayer->GetSession()->GetSecurity();
        else
        {
            uint64 guid = ticket->GetAssignedToGUID();
            uint32 accountId = sObjectMgr->GetPlayerAccountIdByGUID(guid);
            security = AccountMgr::GetSecurity(accountId, g_RealmID);
        }

        // Check security
        //! If no m_session present it means we're issuing this command from the console
        uint32 mySecurity = handler->GetSession() ? handler->GetSession()->GetSecurity() : SEC_CONSOLE;
        if (security > mySecurity)
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETUNASSIGNSECURITY);
            return true;
        }

        SQLTransaction trans = SQLTransaction(NULL);
        ticket->SetUnassigned();
        ticket->SaveToDB(trans);
        sTicketMgr->UpdateLastChange();

        std::string msg = ticket->FormatMessageString(*handler, NULL, ticket->GetAssignedToName().c_str(),
            handler->GetSession() ? handler->GetSession()->GetPlayer()->GetName() : "Console", NULL);
        handler->SendGlobalGMSysMessage(msg.c_str());

        return true;
    }

    static bool HandleGMTicketGetByIdCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 ticketId = atoi(args);
        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed() || ticket->IsCompleted())
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        SQLTransaction trans = SQLTransaction(NULL);
        ticket->SetViewed();
        ticket->SaveToDB(trans);

        handler->SendSysMessage(ticket->FormatMessageString(*handler, true).c_str());
        return true;
    }

    static bool HandleGMTicketGetByNameCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string name(args);
        if (!normalizePlayerName(name))
            return false;

        // Detect target's GUID
        uint64 guid = 0;
        if (Player* player = sObjectAccessor->FindPlayerByName(name.c_str()))
            guid = player->GetGUID();
        else
            guid = sObjectMgr->GetPlayerGUIDByName(name);

        // Target must exist
        if (!guid)
        {
            handler->SendSysMessage(LANG_NO_PLAYERS_FOUND);
            return true;
        }

        // Ticket must exist
        GmTicket* ticket = sTicketMgr->GetTicketByPlayer(guid);
        if (!ticket)
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        SQLTransaction trans = SQLTransaction(NULL);
        ticket->SetViewed();
        ticket->SaveToDB(trans);

        handler->SendSysMessage(ticket->FormatMessageString(*handler, true).c_str());
        return true;
    }

    static bool _HandleGMTicketResponseAppendCommand(char const* args, bool newLine, ChatHandler* handler)
    {
        if (!*args)
            return false;

        char* ticketIdStr = strtok((char*)args, " ");
        uint32 ticketId = atoi(ticketIdStr);

        char* response = strtok(NULL, "\n");
        if (!response)
            return false;

        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if (!ticket || ticket->IsClosed())
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        // Cannot add response to ticket, assigned to someone else
        //! Console excluded
        Player* player = handler->GetSession() ? handler->GetSession()->GetPlayer() : NULL;
        if (player && ticket->IsAssignedNotTo(player->GetGUID()))
        {
            handler->PSendSysMessage(LANG_COMMAND_TICKETALREADYASSIGNED, ticket->GetId());
            return true;
        }

        SQLTransaction trans = SQLTransaction(NULL);
        ticket->AppendResponse(response);
        if (newLine)
            ticket->AppendResponse("\n");
        ticket->SaveToDB(trans);

        return true;
    }

    static bool HandleGMTicketResponseAppendCommand(ChatHandler* handler, char const* args)
    {
        return _HandleGMTicketResponseAppendCommand(args, false, handler);
    }

    static bool HandleGMTicketResponseAppendLnCommand(ChatHandler* handler, char const* args)
    {
        return _HandleGMTicketResponseAppendCommand(args, true, handler);
    }
};

#ifndef __clang_analyzer__
void AddSC_ticket_commandscript()
{
    new ticket_commandscript();
}
#endif
#endif
