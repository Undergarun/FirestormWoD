////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AppenderFile.h"
#include "Common.h"

AppenderFile::AppenderFile(uint8 id, std::string const& name, LogLevel level, const char* _filename, const char* _logDir, const char* _mode, AppenderFlags _flags)
    : Appender(id, name, APPENDER_FILE, level, _flags)
    , filename(_filename)
    , logDir(_logDir)
    , mode(_mode)
{
    dynamicName = std::string::npos != filename.find("%s");
    backup = _flags & APPENDER_FLAGS_MAKE_FILE_BACKUP;

    logfile = !dynamicName ? OpenFile(_filename, _mode, backup) : NULL;
}

AppenderFile::~AppenderFile()
{
    if (logfile)
    {
        fclose(logfile);
        logfile = NULL;
    }
}

void AppenderFile::_write(LogMessage& message)
{
    if (dynamicName)
    {
        char namebuf[TRINITY_PATH_MAX];
        snprintf(namebuf, TRINITY_PATH_MAX, filename.c_str(), message.param1.c_str());
        logfile = OpenFile(namebuf, mode, backup);
    }

    if (logfile)
    {
        fprintf(logfile, "%s%s", message.prefix.c_str(), message.text.c_str());
        fflush(logfile);

        if (dynamicName)
        {
            fclose(logfile);
            logfile = NULL;
        }
    }
}

FILE* AppenderFile::OpenFile(std::string const &filename, std::string const &mode, bool backup)
{
    if (mode == "w" && backup)
    {
        std::string newName(filename);
        newName.push_back('.');
        newName.append(LogMessage::getTimeStr(time(NULL)));
        rename(filename.c_str(), newName.c_str()); // no error handling... if we couldn't make a backup, just ignore
    }
    return fopen((logDir + filename).c_str(), mode.c_str());
}
