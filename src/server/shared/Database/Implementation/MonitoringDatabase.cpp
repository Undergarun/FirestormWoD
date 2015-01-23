#include "MonitoringDatabase.hpp"

void MonitoringDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_MONITORINGDATABASE_STATEMENTS);

    PREPARE_STATEMENT(MONITORING_UPD_LAST_UPDATE, "UPDATE last_update SET timestamp = UNIX_TIMESTAMP()", CONNECTION_ASYNC);
    PREPARE_STATEMENT(MONITORING_INS_STATS, "INSERT INTO stats (TimeStamp, OnlinePlayers, OnlineGameMasters, Uptime, UpdateDiff, Upload, Download) VALUES (UNIX_TIMESTAMP(), ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
}
