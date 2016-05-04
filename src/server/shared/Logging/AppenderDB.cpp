////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AppenderDB.h"
#include "Database/DatabaseEnv.h"

AppenderDB::AppenderDB(uint8 id, std::string const& name, LogLevel level, uint8 realmId):
Appender(id, name, APPENDER_DB, level), realm(realmId), enable(false)
{
}

AppenderDB::~AppenderDB()
{
}

void AppenderDB::_write(LogMessage& message)
{
    if (!enable)
        return;
    switch (message.type)
    {
        case LOG_FILTER_SQL:
        case LOG_FILTER_SQL_DRIVER:
        case LOG_FILTER_SQL_DEV:
            break; // Avoid infinite loop, PExecute triggers Logging with LOG_FILTER_SQL type
        default:
            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_LOG);
            stmt->setUInt64(0, message.mtime);
            stmt->setUInt32(1, realm);
            stmt->setUInt8(2, message.type);
            stmt->setUInt8(3, message.level);
            stmt->setString(4, message.text);
            LoginDatabase.Execute(stmt);
            break;
    }
}

void AppenderDB::setEnable(bool _enable)
{
    enable = _enable;
}
