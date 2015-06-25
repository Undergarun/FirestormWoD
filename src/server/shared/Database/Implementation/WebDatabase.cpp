////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "WebDatabase.h"

void WebDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_WEBDATABASE_STATEMENTS);

    PREPARE_STATEMENT(WEB_SEL_ACCOUNT_POINTS, "SELECT COALESCE(SUM(`nbPoints`), 0) - (SELECT COALESCE(SUM(`points`), 0) AS `balance` FROM `achats` WHERE `account_id` = ?) FROM `points` WHERE `account_id` = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(WEB_INS_PURCHASE, "INSERT INTO `achats` (account_id, realm, guid, item_id, name, points, ip, type) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
}