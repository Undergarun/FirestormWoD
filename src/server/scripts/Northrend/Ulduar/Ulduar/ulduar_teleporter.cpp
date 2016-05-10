////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ulduar.h"
#include "Spell.h"

#define GOSSIP_SENDER_ULDUAR_PORT 603

class ulduar_teleporter : public GameObjectScript
{
    public:
        ulduar_teleporter() : GameObjectScript("ulduar_teleporter") { }

        bool OnGossipHello(Player* player, GameObject* gameObject)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_BASE_CAMP), GOSSIP_SENDER_ULDUAR_PORT, SPELL_BASE_CAMP_TELEPORT);
            if (InstanceScript* instance = gameObject->GetInstanceScript())
            {
                if (instance->GetData(DATA_COLOSSUS) == 2) //count of 2 collossus death
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_FORM_GROUNDS), GOSSIP_SENDER_ULDUAR_PORT, SPELL_FORMATION_GROUNDS_TELEPORT);
                if (instance->GetBossState(BOSS_LEVIATHAN) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_COLOS_FORGE), GOSSIP_SENDER_ULDUAR_PORT, SPELL_COLOSSAL_FORGE_TELEPORT);
                if (instance->GetBossState(BOSS_XT002) == DONE)
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_SCRAPYARD), GOSSIP_SENDER_ULDUAR_PORT, SPELL_SCRAPYARD_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_ANTCHAMBER), GOSSIP_SENDER_ULDUAR_PORT, SPELL_ANTECHAMBER_TELEPORT);
                }
                if (instance->GetBossState(BOSS_KOLOGARN) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_SHATTERED), GOSSIP_SENDER_ULDUAR_PORT, SPELL_SHATTERED_WALKWAY_TELEPORT);
                if (instance->GetBossState(BOSS_AURIAYA) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_CONSERVATORY), GOSSIP_SENDER_ULDUAR_PORT, SPELL_CONSERVATORY_TELEPORT);
                if (instance->GetData(DATA_TRAM) == DONE || instance->GetBossState(BOSS_MIMIRON) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_IMAGINATION), GOSSIP_SENDER_ULDUAR_PORT, SPELL_SPARK_OF_IMAGINATION_TELEPORT);
                /*if (instance->GetBossState(BOSS_VEZAX) == DONE)
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ULD_TEL_DESCENT), GOSSIP_SENDER_ULDUAR_PORT, SPELL_DESCENT_INTO_MADNESS_TELEPORT);*/
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(gameObject), gameObject->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*gameObject*/, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();
            SpellInfo const* spell = sSpellMgr->GetSpellInfo(action);
            if (!spell)
                return false;

            if (player->isInCombat())
            {
                Spell::SendCastResult(player, spell, 0, SPELL_FAILED_AFFECTING_COMBAT);
                return true;
            }

            if (player->GetVehicle())
                player->ExitVehicle();

            if (player->IsMounted())
            {
                player->Dismount();
                player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            }

            if (sender == GOSSIP_SENDER_ULDUAR_PORT)
                player->CastSpell(player, spell, true);

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_ulduar_teleporter()
{
    new ulduar_teleporter();
}
#endif