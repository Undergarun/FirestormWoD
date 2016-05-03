////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_H
#define LOG_H

#include "Common.h"
#include "Define.h"
#include "Appender.h"
#include "LogWorker.h"
#include "Logger.h"
#include "LogOperation.h"

#include <cstdarg>
#include <cstdio>

class Log
{
    friend class ACE_Singleton<Log, ACE_Thread_Mutex>;

    typedef std::map<uint8, Logger> LoggerMap;

    private:
        Log();
        ~Log();

    public:
        void LoadFromConfig();
        void Close();
        inline bool ShouldLog(LogFilterType type, LogLevel level);
        bool SetLogLevel(std::string const& name, char const* level, bool isLogger = true);

        inline void outTrace(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_TRACE))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_TRACE, str, ap);

            va_end(ap);
        }
        inline void outDebug(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_DEBUG))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_DEBUG, str, ap);

            va_end(ap);
        }
        inline void outInfo(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_INFO))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_INFO, str, ap);

            va_end(ap);
        }
        inline void outWarn(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_WARN))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_WARN, str, ap);

            va_end(ap);
        }
        inline void outError(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_ERROR))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_ERROR, str, ap);

            va_end(ap);
        }
        inline void outFatal(LogFilterType f, char const* str, ...) ATTR_PRINTF(3, 4)
        {
            if (!str || !ShouldLog(f, LOG_LEVEL_FATAL))
                return;

            va_list ap;
            va_start(ap, str);

            vlog(f, LOG_LEVEL_FATAL, str, ap);

            va_end(ap);
        }
        inline void outCharDump(char const* str, uint32 account_id, uint32 guid, char const* name)
        {
            if (!str || !ShouldLog(LOG_FILTER_PLAYER_DUMP, LOG_LEVEL_INFO))
                return;

            std::ostringstream ss;
            ss << "== START DUMP == (account: " << account_id << " guid: " << guid << " name: " << name
                << ")\n" << str << "\n== END DUMP ==\n";

            LogMessage* msg = new LogMessage(LOG_LEVEL_INFO, LOG_FILTER_PLAYER_DUMP, ss.str());
            ss.clear();
            ss << guid << '_' << name;

            msg->param1 = ss.str();

            write(msg);
        }

        /// No filters
        void outArena(const char * str, ...)               ATTR_PRINTF(2, 3);
        void outCommand(uint32 gm_account_id, std::string gm_account_name,
                        uint32 gm_character_id, std::string gm_character_name,
                        uint32 sc_account_id, std::string sc_account_name,
                        uint32 sc_character_id, std::string sc_character_name,
                        const char * str, ...)              ATTR_PRINTF(10, 11);
        void outGmChat(uint32 message_type,
                       uint32 from_account_id, std::string from_account_name,
                       uint32 from_character_id, std::string from_character_name,
                       uint32 to_account_id, std::string to_account_name,
                       uint32 to_character_id, std::string to_character_name,
                       const char * str);
        void outAshran(const char* str, ...);
        void outSlack(std::string const& p_Dest, std::string const& p_Color, bool p_IsAttachement, const char* p_Message, ...);

        void EnableDBAppenders();
        static std::string GetTimestampStr();
        
        void SetRealmID(uint32 id);
        uint32 GetRealmID() const { return realm; }

    private:
        void vlog(LogFilterType f, LogLevel level, char const* str, va_list argptr);
        void write(LogMessage* msg);

        inline Logger* GetLoggerByType(LogFilterType filter);
        Appender* GetAppenderByName(std::string const& name);
        uint8 NextAppenderId();
        void CreateAppenderFromConfig(const char* name);
        void CreateLoggerFromConfig(const char* name);
        void ReadAppendersFromConfig();
        void ReadLoggersFromConfig();

        AppenderMap appenders;
        LoggerMap loggers;
        uint8 AppenderId;
        LogLevel lowestLogLevel;

        std::string m_logsDir;
        std::string m_logsTimestamp;

        uint32 realm;
        LogWorker* worker;

        FILE* ashranLog;

        /// Slack
        bool        m_SlackEnable;
        std::string m_SlackApiUrl;
        std::string m_SlackAppName;

        uint8 m_LogLevelTypeByFilterCache[MAX_LOG_FILTER];
        uint8 m_LogTypePresentCache[MAX_LOG_FILTER];
};

#define sLog ACE_Singleton<Log, ACE_Thread_Mutex>::instance()

// Returns default logger if the requested logger is not found
inline Logger* Log::GetLoggerByType(LogFilterType filter)
{
    return m_LogTypePresentCache[filter] ? &loggers[filter] : &(loggers[0]);
}

inline bool Log::ShouldLog(LogFilterType type, LogLevel level)
{
    // Don't even look for a logger if the LogLevel is lower than lowest log levels across all loggers
    if (level < lowestLogLevel)
        return false;

    if (m_LogTypePresentCache[type])
    {
        LogLevel loggerLevel = (LogLevel)m_LogLevelTypeByFilterCache[type];
        return  loggerLevel != LOG_LEVEL_DISABLED && loggerLevel <= level;
    }

    if (type != LOG_FILTER_GENERAL)
        return ShouldLog(LOG_FILTER_GENERAL, level);

    return false;
}

#endif
