////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "WebDatabase.h"

void WebDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_WEBDATABASE_STATEMENTS);

    PREPARE_STATEMENT(WEB_SEL_ACCOUNT_POINTS, "SELECT COALESCE(SUM(`Points`), 0) - (SELECT COALESCE(SUM(`Price`), 0) AS `balance` FROM `api_purchase` WHERE `AccountID` = ? AND TypePoints = 1) FROM `api_points` WHERE `AccountID` = ? AND TypePoints = 1", CONNECTION_ASYNC);
    PREPARE_STATEMENT(WEB_INS_PURCHASE, "INSERT INTO `api_purchase` (AccountID, Expansion, Realm, CharacterGUID, ShopEntryData, Count, TypePrice, Price, IP) VALUES (?, 5, ?, ?, ?, ?, 1, ?, ?)", CONNECTION_ASYNC);
}
