#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "Chat.h"
#include "Garrison.h"

/// Garrison commands
class garrison_commandscript : public CommandScript
{
    public:
        /// Constructor
        garrison_commandscript() 
            : CommandScript("garrison_commandscript") 
        { 

        }

        /// Get command table
        ChatCommand * GetCommands() const
        {
            static ChatCommand blueprintCommandTable[] =
            {
                { "learn", SEC_GAMEMASTER, true, &HandleBlueprintLearnCommand, "", NULL },
            };

            static ChatCommand garrisonCommandTable[] =
            {
                { "blueprint", SEC_GAMEMASTER,  true,   NULL, "", blueprintCommandTable },
                { NULL,         0,              false,  NULL, "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "garrison",   SEC_GAMEMASTER, false, NULL, "", garrisonCommandTable },
                { NULL,         0,              false, NULL, "", NULL }
            };
            return commandTable;
        }

        static bool HandleBlueprintLearnCommand(ChatHandler * p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
                    {
                        const GarrBuildingEntry * l_Entry = sGarrBuildingStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->LearnBlueprint(l_Entry->BuildingID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_Blueprint = atoi(p_Args);

                    if (!l_Blueprint)
                        return false;

                    return l_TargetPlayer->GetGarrison()->LearnBlueprint(l_Blueprint);
                }
            }

            return false;
        }
};

void AddSC_garrison_commandscript()
{
    new garrison_commandscript();
}
