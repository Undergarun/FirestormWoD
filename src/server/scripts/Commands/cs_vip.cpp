////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "Common.h"
#include "Chat.h"
#include "Player.h"
#include "World.h"
#include "Config.h"
#include "WorldSession.h"
#include "Language.h"
#include "Log.h"
#include "SpellAuras.h"


class vip_commandscript: public CommandScript
{
public:
    vip_commandscript() : CommandScript("vip_commandscript") { }

    ChatCommand* GetCommands() const
    {

        static ChatCommand vipCommandTable[] =
        {
            { "bank",           SEC_PLAYER,         false, &HandleVipBankCommand,               "", NULL },
            { "home",           SEC_PLAYER,         false, &HandleVipHomeCommand,               "", NULL },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };


        static ChatCommand commandTable[] =
        {
            { "vip",            SEC_PLAYER,      false, NULL,                                "", vipCommandTable },
            { NULL,             0,               false, NULL,                                "", NULL }
        };
        return commandTable;
    }


    static bool HandleVipBankCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player *plr = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_BANK_COMMAND))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (plr->isInCombat() || plr->isInFlight() || plr->GetMap()->IsBattlegroundOrArena() || plr->HasStealthAura() || plr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || plr->isDead())
        {
            handler->SendSysMessage(LANG_VIP_ERROR);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());

        return true;
    }

    static bool HandleVipHomeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player *plr = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremium())
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_HOME_COMMAND))
        {
            handler->SendSysMessage(LANG_VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (plr->isInCombat() || plr->isInFlight() || plr->GetMap()->IsBattlegroundOrArena() || plr->HasStealthAura() || plr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || plr->isDead())
        {
            handler->SendSysMessage(LANG_VIP_ERROR);
            handler->SetSentErrorMessage(true);
            return false;
        }

        plr->RemoveSpellCooldown(8690,true);
        plr->CastSpell(plr,8690,false);

        return true;
    }

};

#ifndef __clang_analyzer__
void AddSC_vip_commandscript()
{
    new vip_commandscript();
}
#endif
