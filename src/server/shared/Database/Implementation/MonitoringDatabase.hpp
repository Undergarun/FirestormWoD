#pragma once
#ifndef MONITORINGDATABASE_HPP_IMPLEMENTATION
#define MONITORINGDATABASE_HPP_IMPLEMENTATION

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

class MonitoringDatabaseConnection : public MySQLConnection
{
    public:
        /// Constructors for sync and async connections
        MonitoringDatabaseConnection(MySQLConnectionInfo & p_ConnectionInfo) 
            : MySQLConnection(p_ConnectionInfo)
        {

        }
        /// Constructor
        MonitoringDatabaseConnection(ACE_Activation_Queue * p_ActivationQueue, MySQLConnectionInfo & p_ConnectionInfo)
            : MySQLConnection(p_ActivationQueue, p_ConnectionInfo)
        {

        }

        /// Loads database type specific prepared statements
        void DoPrepareStatements();
};

typedef DatabaseWorkerPool<MonitoringDatabaseConnection> MonitoringDatabaseWorkerPool;

enum MonitoringDatabaseStatements
{
    /// Naming standard for defines:
    /// {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
    /// When updating more than one field, consider looking at the calling function
    /// name for a suiting suffix.
    MONITORING_UPD_LAST_UPDATE,
    MONITORING_INS_STATS,

    MAX_MONITORINGDATABASE_STATEMENTS
};

#endif  ///< MONITORINGDATABASE_HPP_IMPLEMENTATION
