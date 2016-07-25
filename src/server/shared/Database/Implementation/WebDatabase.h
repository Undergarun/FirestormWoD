////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef WEBDATABASE_H_INCLUDED
#define WEBDATABASE_H_INCLUDED

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

enum WebDatabaseStatements
{
    /*  Naming standard for defines:
    {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
    When updating more than one field, consider looking at the calling function
    name for a suiting suffix.
    */

    WEB_SEL_ACCOUNT_POINTS = 1,
    WEB_INS_PURCHASE,
    WEB_SEL_ACC_VALIDATE,
    WEB_SEL_POINTS_PURCHASE,
    WEB_INS_LOYALTY_POINTS,
    MAX_WEBDATABASE_STATEMENTS
};

class WebDatabaseConnection : public MySQLConnection
{
public:
    typedef WebDatabaseStatements Statements;

    /// Constructors for sync and async connections
    WebDatabaseConnection(MySQLConnectionInfo& p_ConnectionInfo) : MySQLConnection(p_ConnectionInfo) { }
    WebDatabaseConnection(ACE_Activation_Queue* p_Queue, MySQLConnectionInfo& p_ConnectionInfo) : MySQLConnection(p_Queue, p_ConnectionInfo) { }

    /// Loads database type specific prepared statements
    void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<WebDatabaseConnection> WebDatabaseWorkerPool;

#endif  ///< LOGINMOPDATABASE_H_INCLUDED
