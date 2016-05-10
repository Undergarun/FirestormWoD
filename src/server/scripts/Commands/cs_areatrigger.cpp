////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "TicketMgr.h"
#include "Chat.h"

class at_commandscript: public CommandScript
{
    public:
        at_commandscript() : CommandScript("at_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand atSetCommandTable[] =
            {
                { "scale",          SEC_GAMEMASTER,     false, &HandleAreatriggerSetScaleCommand,  "", NULL             },
                { NULL,             0,                  false, NULL,                               "", NULL             }
            };

            static ChatCommand atCommandTable[] =
            {
                { "set",            SEC_GAMEMASTER,     false, NULL,                              "", atSetCommandTable },
                { "near",           SEC_GAMEMASTER,     false, &HandleAreatriggerNearCommand,     "", NULL              },
                { NULL,             0,                  false, NULL,                              "", NULL              }
            };

            static ChatCommand commandTable[] =
            {
                { "at",             SEC_MODERATOR,      false, NULL,                               "", atCommandTable   },
                { NULL,             0,                  false, NULL,                               "", NULL             }
            };

            return commandTable;
        }

        static uint32 GetAreaTriggerGUIDLow(ChatHandler* p_Handler, char const* p_Args)
        {
            if (*p_Args)
            {
                // number or [name] Shift-click form |color|Hareatrigger:at_guid|h[name]|h|r
                char* l_ID = p_Handler->extractKeyFromLink((char*)p_Args, "Hareatrigger");
                if (!l_ID)
                    return 0;

                return atoi(l_ID);
            }

            return 0;
        }

        static bool HandleAreatriggerSetScaleCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_Guid = GetAreaTriggerGUIDLow(p_Handler, p_Args);
            if (!l_Guid)
                return false;

            std::list<AreaTrigger*> l_ATList;
            AreaTrigger* l_AT = nullptr;
            Player* l_Player = p_Handler->GetSession()->GetPlayer();
            l_Player->GetAreatriggerListInRange(l_ATList, 50.0f);

            for (AreaTrigger* l_AreaTrigger : l_ATList)
            {
                if (l_AreaTrigger->GetGUIDLow() != l_Guid)
                    continue;

                l_AT = l_AreaTrigger;
                break;
            }

            if (l_AT == nullptr)
            {
                p_Handler->PSendSysMessage("No areatrigger found with guid %u.", l_Guid);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_Value = strtok(NULL, " ");
            if (!l_Value)
                return false;

            float l_Scale = atof(l_Value);
            l_AT->SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, l_Scale);
            p_Handler->PSendSysMessage("Set areatrigger scale %f", l_Scale);
            return true;
        }

        static bool HandleAreatriggerNearCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            float l_Distance = (!*p_Args) ? 10.0f : (float)(atof(p_Args));
            if (l_Distance > 50.0f)
                l_Distance = 50.0f;

            std::list<AreaTrigger*> l_ATList;
            Player* l_Player = p_Handler->GetSession()->GetPlayer();
            l_Player->GetAreatriggerListInRange(l_ATList, l_Distance);

            for (AreaTrigger* l_AreaTrigger : l_ATList)
            {
                p_Handler->PSendSysMessage("%u (Entry: %u, Spell: %u) - |cffffffff|Hareatrigger:%u|h[%s X:%f Y:%f Z:%f MapID:%u]|h|r",
                    l_AreaTrigger->GetGUIDLow(),
                    l_AreaTrigger->GetEntry(),
                    l_AreaTrigger->GetSpellId(),
                    l_AreaTrigger->GetGUIDLow(),
                    sSpellMgr->GetSpellInfo(l_AreaTrigger->GetSpellId())->SpellName,
                    l_AreaTrigger->m_positionX,
                    l_AreaTrigger->m_positionY,
                    l_AreaTrigger->m_positionZ,
                    l_AreaTrigger->GetMapId());
            }

            p_Handler->PSendSysMessage("Nearest AreaTriggers in %f yards : %lu.", l_Distance, l_ATList.size());
            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_at_commandscript()
{
    new at_commandscript();
}
#endif
