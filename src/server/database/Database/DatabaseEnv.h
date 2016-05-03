////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DATABASEENV_H
#define DATABASEENV_H

#include "Common.h"
#include "Errors.h"
#include "Log.h"

#include "Field.h"
#include "QueryResult.h"

#include "MySQLThreading.h"
#include "Transaction.h"

#define _LIKE_           "LIKE"
#define _TABLE_SIM_      "`"
#define _CONCAT3_(A, B, C) "CONCAT( " A ", " B ", " C " )"
#define _OFFSET_         "LIMIT %d, 1"

#include "Implementation/LoginDatabase.h"
#include "Implementation/CharacterDatabase.h"
#include "Implementation/WorldDatabase.h"
#include "Implementation/HotfixDatabase.h"
#include "Implementation/LoginMopDatabase.h"
#include "Implementation//WebDatabase.h"

extern WorldDatabaseWorkerPool WorldDatabase;
extern CharacterDatabaseWorkerPool CharacterDatabase;
extern LoginDatabaseWorkerPool LoginDatabase;
extern HotfixDatabaseWorkerPool HotfixDatabase;
extern LoginMopDatabaseWorkerPool LoginMopDatabase;
extern WebDatabaseWorkerPool WebDatabase;

#endif

