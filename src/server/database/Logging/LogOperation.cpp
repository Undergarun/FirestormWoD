////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "LogOperation.h"
#include "Logger.h"

LogOperation::~LogOperation()
{
    delete msg;
}

int LogOperation::call()
{
    if (logger && msg)
        logger->write(*msg);
    return 0;
}
