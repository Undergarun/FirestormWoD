/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "TicketMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Chat.h"
#include "World.h"

inline float GetAge(uint64 p_T) { return float(time(NULL) - p_T) / DAY; }

///////////////////////////////////////////////////////////////////////////////////////////////////
// GM ticket
GmTicket::GmTicket() { }

GmTicket::GmTicket(Player* p_Player, WorldPacket& p_RecvData) : m_CreateTime(time(NULL)), m_LastModifiedTime(time(NULL)), m_ClosedBy(0), m_AssignedTo(0), m_Completed(false),
m_EscalatedStatus(TICKET_UNASSIGNED), m_NeedResponse(false), m_Viewed(false), m_HaveTicket(false)
{
    m_ID = sTicketMgr->GenerateTicketId();
    m_PlayerName = p_Player->GetName();
    m_PlayerGUID = p_Player->GetGUID();

    p_RecvData >> m_MapID;
    p_RecvData >> m_PositionX;
    p_RecvData >> m_PositionY;
    p_RecvData >> m_PositionZ;
    p_RecvData >> m_HaveTicket;

    uint32 l_MessageLen = p_RecvData.ReadBits(11);
    p_RecvData.FlushBits();
    m_Message = p_RecvData.ReadString(l_MessageLen);

    bool l_NeedMoreHelp = p_RecvData.ReadBit(); /// non used
    
    m_NeedResponse = p_RecvData.ReadBit();
    p_RecvData.FlushBits();

    uint32 l_UnkLen = p_RecvData.read<uint32>();
    std::string l_UnkStr = p_RecvData.ReadString(l_UnkLen);
}

GmTicket::~GmTicket() { }

bool GmTicket::LoadFromDB(Field* p_Fields)
{
    //     0       1     2      3          4        5      6     7     8           9            10         11         12        13        14        15
    // ticketId, guid, name, message, createTime, mapId, posX, posY, posZ, lastModifiedTime, closedBy, assignedTo, comment, completed, escalated, viewed
    uint8 l_Index = 0;
    m_ID                = p_Fields[  l_Index].GetUInt32();
    m_PlayerGUID        = MAKE_NEW_GUID(p_Fields[++l_Index].GetUInt32(), 0, HIGHGUID_PLAYER);
    m_PlayerName        = p_Fields[++l_Index].GetString();
    m_Message           = p_Fields[++l_Index].GetString();
    m_CreateTime        = p_Fields[++l_Index].GetUInt32();
    m_MapID             = p_Fields[++l_Index].GetUInt32();
    m_PositionX         = p_Fields[++l_Index].GetFloat();
    m_PositionY         = p_Fields[++l_Index].GetFloat();
    m_PositionZ         = p_Fields[++l_Index].GetFloat();
    m_LastModifiedTime  = p_Fields[++l_Index].GetUInt32();
    m_ClosedBy          = p_Fields[++l_Index].GetInt32();
    m_AssignedTo        = MAKE_NEW_GUID(p_Fields[++l_Index].GetUInt32(), 0, HIGHGUID_PLAYER);
    m_Comment           = p_Fields[++l_Index].GetString();
    m_Completed         = p_Fields[++l_Index].GetBool();
    m_EscalatedStatus   = GMTicketEscalationStatus(p_Fields[++l_Index].GetUInt8());
    m_Viewed            = p_Fields[++l_Index].GetBool();
    m_HaveTicket        = p_Fields[++l_Index].GetBool();
    m_Response          = p_Fields[++l_Index].GetString();

    return true;
}

void GmTicket::SaveToDB(SQLTransaction& p_Transaction) const
{
    //     0       1     2      3          4        5      6     7     8           9            10         11         12        13        14        15
    // ticketId, guid, name, message, createTime, mapId, posX, posY, posZ, lastModifiedTime, closedBy, assignedTo, comment, completed, escalated, viewed
    uint8 l_Index = 0;
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_REP_GM_TICKET);
    l_Statement->setUInt32(  l_Index, m_ID);
    l_Statement->setUInt32(++l_Index, GUID_LOPART(m_PlayerGUID));
    l_Statement->setString(++l_Index, m_PlayerName);
    l_Statement->setString(++l_Index, m_Message);
    l_Statement->setUInt32(++l_Index, uint32(m_CreateTime));
    l_Statement->setUInt32(++l_Index, m_MapID);
    l_Statement->setFloat (++l_Index, m_PositionX);
    l_Statement->setFloat (++l_Index, m_PositionY);
    l_Statement->setFloat (++l_Index, m_PositionZ);
    l_Statement->setUInt32(++l_Index, uint32(m_LastModifiedTime));
    l_Statement->setInt32 (++l_Index, GUID_LOPART(m_ClosedBy));
    l_Statement->setUInt32(++l_Index, GUID_LOPART(m_AssignedTo));
    l_Statement->setString(++l_Index, m_Comment);
    l_Statement->setBool  (++l_Index, m_Completed);
    l_Statement->setUInt8 (++l_Index, uint8(m_EscalatedStatus));
    l_Statement->setBool  (++l_Index, m_Viewed);
    l_Statement->setBool  (++l_Index, m_HaveTicket);
    l_Statement->setString(++l_Index, m_Response);

    CharacterDatabase.ExecuteOrAppend(p_Transaction, l_Statement);
}

void GmTicket::DeleteFromDB()
{
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GM_TICKET);
    l_Statement->setUInt32(0, m_ID);
    CharacterDatabase.Execute(l_Statement);
}

void GmTicket::WritePacket(WorldPacket& p_Data) const
{
    std::string l_Message = GetMessage();

    p_Data << uint32(GetId());
    p_Data << uint8(std::min(m_EscalatedStatus, TICKET_IN_ESCALATION_QUEUE));
    p_Data << uint32(GetAge(m_LastModifiedTime));

    if (GmTicket* ticket = sTicketMgr->GetOldestOpenTicket())
        p_Data << uint32(GetAge(ticket->GetLastModifiedTime()));
    else
        p_Data << uint32(float(0));

    p_Data << uint32(GetAge(sTicketMgr->GetLastChange()));
    p_Data << uint8(m_HaveTicket);
    p_Data << uint8(m_Viewed ? GMTICKET_OPENEDBYGM_STATUS_OPENED : GMTICKET_OPENEDBYGM_STATUS_NOT_OPENED);
    p_Data << uint32(0);

    p_Data.WriteBits(l_Message.size(), 11);
    p_Data.WriteBits(l_Message.size(), 10);
    p_Data.FlushBits();
    p_Data.WriteString(l_Message);
    p_Data.WriteString(l_Message);
}

void GmTicket::SendResponse(WorldSession* p_Session) const
{
    WorldPacket l_Data(SMSG_GM_TICKET_RESPONSE);
    l_Data << uint32(1);
    l_Data << uint32(m_ID);
    l_Data.WriteBits(m_Message.size(), 11);
    l_Data.WriteBits(m_Response.size(), 14);
    l_Data.FlushBits();
    l_Data.WriteString(m_Message);
    l_Data.WriteString(m_Response);
    p_Session->SendPacket(&l_Data);
}

std::string GmTicket::FormatMessageString(ChatHandler& p_Handler, bool p_Detailed) const
{
    time_t l_CurTime = time(NULL);

    std::stringstream l_SS;
    std::string l_NameLink = p_Handler.playerLink(m_PlayerName);

    l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, m_ID);
    l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, l_NameLink.c_str());
    l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTAGECREATE, (secsToTimeString(l_CurTime - m_CreateTime, true, false)).c_str());
    l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTAGE, (secsToTimeString(l_CurTime - m_LastModifiedTime, true, false)).c_str());

    std::string l_Name;
    if (sObjectMgr->GetPlayerNameByGUID(m_AssignedTo, l_Name))
        l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, l_Name.c_str());

    if (p_Detailed)
    {
        l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTMESSAGE, m_Message.c_str());
        if (!m_Comment.empty())
            l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTCOMMENT, m_Comment.c_str());
    }
    return l_SS.str();
}

std::string GmTicket::FormatMessageString(ChatHandler& p_Handler, const char* p_ClosedName, const char* p_AssignedToName, const char* p_UnassignedName, const char* p_DeletedName) const
{
    std::stringstream l_SS;
    l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTGUID, m_ID);
    l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTNAME, m_PlayerName.c_str());
    if (p_ClosedName)
        l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETCLOSED, p_ClosedName);
    if (p_AssignedToName)
        l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTASSIGNEDTO, p_AssignedToName);
    if (p_UnassignedName)
        l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETLISTUNASSIGNED, p_UnassignedName);
    if (p_DeletedName)
        l_SS << p_Handler.PGetParseString(LANG_COMMAND_TICKETDELETED, p_DeletedName);
    return l_SS.str();
}

void GmTicket::SetUnassigned()
{
    m_AssignedTo = 0;
    switch (m_EscalatedStatus)
    {
        case TICKET_ASSIGNED: m_EscalatedStatus = TICKET_UNASSIGNED; break;
        case TICKET_ESCALATED_ASSIGNED: m_EscalatedStatus = TICKET_IN_ESCALATION_QUEUE; break;
        case TICKET_UNASSIGNED:
        case TICKET_IN_ESCALATION_QUEUE:
        default:
            break;
    }
}

void GmTicket::TeleportTo(Player* p_Player) const
{
    p_Player->TeleportTo(m_MapID, m_PositionX, m_PositionY, m_PositionZ, 0.0f, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Ticket manager
TicketMgr::TicketMgr() : m_Status(true), m_LastTicketId(0), m_LastSurveyId(0), m_OpenTicketCount(0), m_LastChange(time(NULL)) { }

TicketMgr::~TicketMgr()
{
    for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
        delete l_Iter->second;
}

void TicketMgr::Initialize() { SetStatus(sWorld->getBoolConfig(CONFIG_ALLOW_TICKETS)); }

void TicketMgr::ResetTickets()
{
    for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
    {
        if (l_Iter->second->IsClosed())
            sTicketMgr->RemoveTicket(l_Iter->second->GetId());
    }

    m_LastTicketId = 0;

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ALL_GM_TICKETS);
    CharacterDatabase.Execute(l_Statement);
}

void TicketMgr::LoadTickets()
{
    uint32 l_OldMSTime = getMSTime();

    for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
        delete l_Iter->second;

    /// Delete old tickets (> 2 day)
    CharacterDatabase.PQuery("DELETE FROM gm_tickets WHERE createtime < UNIX_TIMESTAMP() - 60*60*24*2");

    m_TicketList.clear();

    m_LastTicketId = 0;
    m_OpenTicketCount = 0;

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GM_TICKETS);
    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Statement);
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 GM tickets. DB table `gm_tickets` is empty!");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();
        GmTicket* l_Ticket = new GmTicket();
        if (!l_Ticket->LoadFromDB(l_Fields))
        {
            delete l_Ticket;
            continue;
        }

        if (!l_Ticket->IsClosed())
            ++m_OpenTicketCount;

        // Update max ticket id if necessary
        uint32 l_ID = l_Ticket->GetId();
        if (m_LastTicketId < l_ID)
            m_LastTicketId = l_ID;

        m_TicketList[l_ID] = l_Ticket;
        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u GM tickets in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void TicketMgr::LoadSurveys()
{
    // we don't actually load anything into memory here as there's no reason to
    m_LastSurveyId = 0;

    uint32 l_OldMSTime = getMSTime();
    if (QueryResult l_Result = CharacterDatabase.Query("SELECT MAX(surveyId) FROM gm_surveys"))
        m_LastSurveyId = (*l_Result)[0].GetUInt32();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded GM Survey count from database in %u ms", GetMSTimeDiffToNow(l_OldMSTime));
}

void TicketMgr::AddTicket(GmTicket* p_Ticket)
{
    m_TicketList[p_Ticket->GetId()] = p_Ticket;
    if (!p_Ticket->IsClosed())
        ++m_OpenTicketCount;
    SQLTransaction l_Transaction = SQLTransaction(NULL);
    p_Ticket->SaveToDB(l_Transaction);
}

void TicketMgr::CloseTicket(uint32 p_TicketID, int64 p_Source)
{
    if (GmTicket* l_Ticket = GetTicket(p_TicketID))
    {
        SQLTransaction l_Transaction = SQLTransaction(NULL);
        l_Ticket->SetClosedBy(p_Source);
        if (p_Source)
            --m_OpenTicketCount;
        l_Ticket->SaveToDB(l_Transaction);
    }
}

void TicketMgr::RemoveTicket(uint32 p_TicketID)
{
    if (GmTicket* l_Ticket = GetTicket(p_TicketID))
    {
        l_Ticket->DeleteFromDB();
        m_TicketList.erase(p_TicketID);
        delete l_Ticket;
    }
}

void TicketMgr::ShowList(ChatHandler& p_Handler, bool p_OnlineOnly) const
{
    p_Handler.SendSysMessage(p_OnlineOnly ? LANG_COMMAND_TICKETSHOWONLINELIST : LANG_COMMAND_TICKETSHOWLIST);

    for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
    {
        if (!l_Iter->second->IsClosed() && !l_Iter->second->IsCompleted())
        {
            if (!p_OnlineOnly || l_Iter->second->GetPlayer())
                p_Handler.SendSysMessage(l_Iter->second->FormatMessageString(p_Handler).c_str());
        }
    }
}

void TicketMgr::ShowClosedList(ChatHandler& p_Handler) const
{
    p_Handler.SendSysMessage(LANG_COMMAND_TICKETSHOWCLOSEDLIST);

    for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
    {
        if (l_Iter->second->IsClosed())
            p_Handler.SendSysMessage(l_Iter->second->FormatMessageString(p_Handler).c_str());
    }
}

void TicketMgr::ShowEscalatedList(ChatHandler& p_Handler) const
{
    p_Handler.SendSysMessage(LANG_COMMAND_TICKETSHOWESCALATEDLIST);

    for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
    {
        if (!l_Iter->second->IsClosed() && l_Iter->second->GetEscalatedStatus() == TICKET_IN_ESCALATION_QUEUE)
            p_Handler.SendSysMessage(l_Iter->second->FormatMessageString(p_Handler).c_str());
    }
}

void TicketMgr::SendTicket(WorldSession* p_Session, GmTicket* p_Ticket) const
{
    uint32 l_Status = GMTICKET_STATUS_DEFAULT;
    if (p_Ticket)
        l_Status = GMTICKET_STATUS_HASTEXT;

    WorldPacket l_Data(SMSG_GM_TICKET_GET_TICKET_RESPONSE, 2 * 1024);
    l_Data << uint32(l_Status);
    l_Data.WriteBit(l_Status == GMTICKET_STATUS_HASTEXT);
    l_Data.FlushBits();

    if (l_Status == GMTICKET_STATUS_HASTEXT)
        p_Ticket->WritePacket(l_Data);

    p_Session->SendPacket(&l_Data);
}
