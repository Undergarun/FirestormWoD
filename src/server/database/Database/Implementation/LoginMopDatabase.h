////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef LOGINMOPDATABASE_H_INCLUDED
#define LOGINMOPDATABASE_H_INCLUDED

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

enum LoginMopDatabaseStatements
{
    /*  Naming standard for defines:
    {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
    When updating more than one field, consider looking at the calling function
    name for a suiting suffix.
    */

    LOGINMOP_SEL_TRANSFER,

    MAX_LOGINMOPDATABASE_STATEMENTS
};

class LoginMopDatabaseConnection : public MySQLConnection
{
public:
    typedef LoginMopDatabaseStatements Statements;

    /// Constructors for sync and async connections
    LoginMopDatabaseConnection(MySQLConnectionInfo& p_ConnectionInfo) : MySQLConnection(p_ConnectionInfo) { }
    LoginMopDatabaseConnection(ACE_Activation_Queue* p_Queue, MySQLConnectionInfo& p_ConnectionInfo) : MySQLConnection(p_Queue, p_ConnectionInfo) { }

    /// Loads database type specific prepared statements
    void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<LoginMopDatabaseConnection> LoginMopDatabaseWorkerPool;

#endif  ///< LOGINMOPDATABASE_H_INCLUDED