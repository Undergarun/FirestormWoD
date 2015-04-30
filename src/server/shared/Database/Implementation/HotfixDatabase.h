////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef HOTFIXDATABASE_H_INCLUDED
#define HOTFIXDATABASE_H_INCLUDED

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

class HotfixDatabaseConnection : public MySQLConnection
{
    public:
        /// Constructors for sync and async connections
        HotfixDatabaseConnection(MySQLConnectionInfo& p_ConnectionInfo)
            : MySQLConnection(p_ConnectionInfo)
        {

        }
        /// Constructors
        HotfixDatabaseConnection(ACE_Activation_Queue* p_Queue, MySQLConnectionInfo& p_ConnectionInfo)
            : MySQLConnection(p_Queue, p_ConnectionInfo)
        {

        }

        /// Loads database type specific prepared statements
        void DoPrepareStatements();
};

typedef DatabaseWorkerPool<HotfixDatabaseConnection> HotfixDatabaseWorkerPool;

enum HotfixDatabaseStatements
{
    /*  Naming standard for defines:
        {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */

    MAX_HOTFIXDATABASE_STATEMENTS
};

#endif  ///< HOTFIXDATABASE_H_INCLUDED