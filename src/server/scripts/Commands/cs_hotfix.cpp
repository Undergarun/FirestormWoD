#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "Chat.h"
#include "GarrisonMgr.hpp"
#include "ObjectMgr.h"

/// HotFix commands
class hotfix_commandscript : public CommandScript
{
    public:
        /// Constructor
        hotfix_commandscript() 
            : CommandScript("hotfix_commandscript")
        { 

        }

        /// Get command table
        ChatCommand * GetCommands() const
        {
            static ChatCommand reloadCommandTable[] =
            {
                { "blob",       SEC_GAMEMASTER, true,   &HandleReloadBlob,          "", NULL },
                { "hash",       SEC_GAMEMASTER, true,   &HandleReloadHash,          "", NULL },
                { NULL,         0,              false,  NULL,                       "", NULL }
            };

            static ChatCommand hotfixCommandTable[] =
            {
                { "reload",     SEC_GAMEMASTER, true,   NULL,                       "", reloadCommandTable  },
                { NULL,         0,              false,  NULL,                       "", NULL                }
            };

            static ChatCommand commandTable[] =
            {
                { "hotfix",     SEC_GAMEMASTER, false,  NULL,                       "", hotfixCommandTable  },
                { NULL,         0,              false,  NULL,                       "", NULL                }
            };
            return commandTable;
        }

        static bool HandleReloadBlob(ChatHandler * p_Handler, char const* p_Args)
        {
            sObjectMgr->LoadHotfixData(true);
            return true;
        }

        static bool HandleReloadHash(ChatHandler * p_Handler, char const* p_Args)
        {
            sObjectMgr->LoadHotfixTableHashs();
            return true;
        }

};

void AddSC_hotfix_commandscript()
{
    new hotfix_commandscript();
}
