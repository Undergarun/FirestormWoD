////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HotfixDatabase.h"

void HotfixDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_HOTFIXDATABASE_STATEMENTS);

    PREPARE_STATEMENT(HOTFIX_SEL_ITEM_TEMPLATE_BY_NAME, "SELECT ID FROM item_sparse WHERE Name = ?", CONNECTION_SYNCH);
}