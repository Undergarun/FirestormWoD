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

#include "Language.h"
#include "WorldPacket.h"
#include "Common.h"
#include "ObjectMgr.h"
#include "TicketMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldSession.h"
#include "Util.h"
#include "WowTime.hpp"

void WorldSession::OnGMTicketGetTicketEvent()
{
    SendQueryTimeResponse();

    if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(GetPlayer()->GetGUID()))
    {
        if (l_Ticket->IsCompleted())
            l_Ticket->SendResponse(this);
        else
            sTicketMgr->SendTicket(this, l_Ticket);
    }
    else
        sTicketMgr->SendTicket(this, NULL);
}

void WorldSession::HandleGMTicketGetWebTicketOpcode(WorldPacket& /*p_RecvData*/)
{
    time_t l_Now = time(nullptr);
    WorldPacket l_Data(SMSG_GM_TICKET_CASE_STATUS);
    l_Data << MS::Utilities::WowTime::Encode(l_Now);
    l_Data << uint32(l_Now - GetLoginTime());
    l_Data << uint32(0);
    SendPacket(&l_Data);
}

void WorldSession::HandleGMTicketSystemStatusOpcode(WorldPacket& /*p_RecvData*/)
{
    WorldPacket l_Data(SMSG_GM_TICKET_SYSTEM_STATUS, 4);
    l_Data << uint32(sTicketMgr->GetStatus() ? GMTICKET_QUEUE_STATUS_ENABLED : GMTICKET_QUEUE_STATUS_DISABLED);
    SendPacket(&l_Data);
}

void WorldSession::HandleGMSurveySubmit(WorldPacket& recvData)
{
    uint32 nextSurveyID = sTicketMgr->GetNextSurveyID();
    // just put the survey into the database
    uint32 mainSurvey; // GMSurveyCurrentSurvey.dbc, column 1 (all 9) ref to GMSurveySurveys.dbc
    recvData >> mainSurvey;

    // sub_survey1, r1, comment1, sub_survey2, r2, comment2, sub_survey3, r3, comment3, sub_survey4, r4, comment4, sub_survey5, r5, comment5, sub_survey6, r6, comment6, sub_survey7, r7, comment7, sub_survey8, r8, comment8, sub_survey9, r9, comment9, sub_survey10, r10, comment10,
    for (uint8 i = 0; i < 10; i++)
    {
        uint32 subSurveyId; // ref to i'th GMSurveySurveys.dbc field (all fields in that dbc point to fields in GMSurveyQuestions.dbc)
        recvData >> subSurveyId;
        if (!subSurveyId)
            break;

        uint8 rank; // probably some sort of ref to GMSurveyAnswers.dbc
        recvData >> rank;
        std::string comment; // comment ("Usage: GMSurveyAnswerSubmit(question, rank, comment)")
        recvData >> comment;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GM_SUBSURVEY);
        stmt->setUInt32(0, nextSurveyID);
        stmt->setUInt32(1, subSurveyId);
        stmt->setUInt32(2, rank);
        stmt->setString(3, comment);
        CharacterDatabase.Execute(stmt);
    }

    std::string comment; // just a guess
    recvData >> comment;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GM_SURVEY);
    stmt->setUInt32(0, GUID_LOPART(GetPlayer()->GetGUID()));
    stmt->setUInt32(1, nextSurveyID);
    stmt->setUInt32(2, mainSurvey);
    stmt->setString(3, comment);

    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleReportLag(WorldPacket& p_RecvData)
{
    uint32 l_Type, l_MapID = 0;
    p_RecvData >> l_Type;
    p_RecvData >> l_MapID;
    float l_X, l_Y, l_Z;
    p_RecvData >> l_X;
    p_RecvData >> l_Y;
    p_RecvData >> l_Z;

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_LAG_REPORT);
    l_Statement->setUInt32(0, GUID_LOPART(m_Player->GetGUID()));
    l_Statement->setUInt8 (1, l_Type);
    l_Statement->setUInt16(2, l_MapID);
    l_Statement->setFloat (3, l_X);
    l_Statement->setFloat (4, l_Y);
    l_Statement->setFloat (5, l_Z);
    l_Statement->setUInt32(6, GetLatency());
    l_Statement->setUInt32(7, time(NULL));
    CharacterDatabase.Execute(l_Statement);
}

void WorldSession::SendTicketStatusUpdate(uint8 p_Response)
{
    if (!GetPlayer())
        return;

    switch (p_Response)
    {
        case GMTICKET_RESPONSE_ALREADY_EXIST:       ///< = 1
            SendGameError(GameError::ERR_TICKET_ALREADY_EXISTS);
            break;
        case GMTICKET_RESPONSE_UPDATE_ERROR:        ///< = 5
            SendGameError(GameError::ERR_TICKET_UPDATE_ERROR);
            break;
        case GMTICKET_RESPONSE_CREATE_ERROR:        ///< = 3
            SendGameError(GameError::ERR_TICKET_CREATE_ERROR);
            break;

        case GMTICKET_RESPONSE_CREATE_SUCCESS:      ///< = 2
        case GMTICKET_RESPONSE_UPDATE_SUCCESS:      ///< = 4
            OnGMTicketGetTicketEvent();
            break;

        case GMTICKET_RESPONSE_TICKET_DELETED:      ///< = 9
            GetPlayer()->SendCustomMessage("FSC_TICKET_DELETED");
            break;

        default:
            SendGameError(GameError::ERR_TICKET_DB_ERROR);
            break;

    }
}
