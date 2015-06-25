////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef WEBDATABASE_H_INCLUDED
#define WEBDATABASE_H_INCLUDED

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

class WebDatabaseConnection : public MySQLConnection
{
public:
    /// Constructors for sync and async connections
    WebDatabaseConnection(MySQLConnectionInfo& p_ConnectionInfo)
        : MySQLConnection(p_ConnectionInfo)
    {

    }
    /// Constructors
    WebDatabaseConnection(ACE_Activation_Queue* p_Queue, MySQLConnectionInfo& p_ConnectionInfo)
        : MySQLConnection(p_Queue, p_ConnectionInfo)
    {

    }

    /// Loads database type specific prepared statements
    void DoPrepareStatements();
};

typedef DatabaseWorkerPool<WebDatabaseConnection> WebDatabaseWorkerPool;

enum WebDatabaseStatements
{
    /*  Naming standard for defines:
    {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
    When updating more than one field, consider looking at the calling function
    name for a suiting suffix.
    */

    WEB_SEL_ACCOUNT_POINTS,
    WEB_INS_PURCHASE,

    MAX_WEBDATABASE_STATEMENTS
};

#endif  ///< LOGINMOPDATABASE_H_INCLUDED