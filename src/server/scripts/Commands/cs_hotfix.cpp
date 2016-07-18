#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "Chat.h"
#ifndef CROSS
#include "GarrisonMgr.hpp"
#endif /* not CROSS */
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
                { "blob",       SEC_GAMEMASTER, true,   &HandleReloadBlob,          "", NULL                },
                { "hash",       SEC_GAMEMASTER, true,   &HandleReloadHash,          "", NULL                },
                { "store",      SEC_GAMEMASTER, true,   &HandleReloadStore,         "", NULL                },
                { NULL,         0,              false,  NULL,                       "", NULL                }
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

        static bool HandleReloadBlob(ChatHandler * /*p_Handler*/, char const* /*p_Args*/)
        {
            sObjectMgr->LoadHotfixData(true);
            return true;
        }

        static bool HandleReloadHash(ChatHandler * /*p_Handler*/, char const* /*p_Args*/)
        {
            sObjectMgr->LoadHotfixTableHashs();
            return true;
        }

        static bool HandleReloadStore(ChatHandler * p_Handler, char const* p_Args)
        {
            char* l_Arg1Str = strtok((char*)p_Args, " ");

            if (!l_Arg1Str)
                return false;

            std::string l_TableName     = l_Arg1Str;
            std::string l_OriginalName  = "";
            std::transform(l_TableName.begin(), l_TableName.end(), l_TableName.begin(), ::tolower);

            DB2StorageBase * l_Store = nullptr;

            for (auto l_Pair : sObjectMgr->HotfixTableID)
            {
                std::string l_PairName = l_Pair.first;
                std::transform(l_PairName.begin(), l_PairName.end(), l_PairName.begin(), ::tolower);

                if (l_PairName == l_TableName)
                {
                    l_OriginalName  = l_Pair.first;
                    l_Store         = sDB2PerHash[l_Pair.second];
                    break;
                }
            }

            if (!l_Store)
            {
                p_Handler->PSendSysMessage("Store '%s' not found", l_TableName.c_str());
                return false;
            }

            std::string l_OutMessage = "";
            if (!l_Store->Reload(l_OutMessage))
            {
                p_Handler->PSendSysMessage("Store reload error : '%s'", l_OutMessage.c_str());
                return false;
            }

            p_Handler->PSendSysMessage("Store '%s' reloaded", l_OriginalName.c_str());

            if (l_TableName == "item" || l_TableName == "itemsparse" || l_TableName == "itemeffect" || l_TableName == "itemappearance" || l_TableName == "itemmodifiedappearance")
            {
                p_Handler->PSendSysMessage("Rebuilding core item_templates");
                sObjectMgr->LoadItemTemplates();
                sObjectMgr->LoadItemTemplateAddon();
                sObjectMgr->LoadItemTemplateCorrections();
            }

            return true;
        }

};

#ifndef __clang_analyzer__
void AddSC_hotfix_commandscript()
{
    new hotfix_commandscript();
}
#endif
