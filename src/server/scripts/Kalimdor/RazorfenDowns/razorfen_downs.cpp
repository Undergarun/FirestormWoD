////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Razorfen_Downs
SD%Complete: 100
SDComment: Support for Henry Stern(2 recipes)
SDCategory: Razorfen Downs
EndScriptData */

/* ContentData
npc_henry_stern
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "razorfen_downs.h"

/*###
# npc_henry_stern
####*/

enum eEnums
{
    SPELL_GOLDTHORN_TEA                         = 13028,
    SPELL_TEACHING_GOLDTHORN_TEA                = 13029,
    SPELL_MIGHT_TROLLS_BLOOD_POTION             = 3451,
    SPELL_TEACHING_MIGHTY_TROLLS_BLOOD_POTION   = 13030,
    GOSSIP_TEXT_TEA_ANSWER                      = 2114,
    GOSSIP_TEXT_POTION_ANSWER                   = 2115
};

#define GOSSIP_ITEM_TEA     "Teach me the cooking recipe"
#define GOSSIP_ITEM_POTION  "Teach me the alchemy recipe"

class npc_henry_stern : public CreatureScript
{
public:
    npc_henry_stern() : CreatureScript("npc_henry_stern") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->CastSpell(player, SPELL_TEACHING_GOLDTHORN_TEA, true);
            player->SEND_GOSSIP_MENU(GOSSIP_TEXT_TEA_ANSWER, creature->GetGUID());
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 2)
        {
            player->CastSpell(player, SPELL_TEACHING_MIGHTY_TROLLS_BLOOD_POTION, true);
            player->SEND_GOSSIP_MENU(GOSSIP_TEXT_POTION_ANSWER, creature->GetGUID());
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetBaseSkillValue(SKILL_COOKING) >= 175 && !player->HasSpell(SPELL_GOLDTHORN_TEA))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TEA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (player->GetBaseSkillValue(SKILL_ALCHEMY) >= 180 && !player->HasSpell(SPELL_MIGHT_TROLLS_BLOOD_POTION))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_POTION, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};

/*######
## go_gong
######*/

class go_gong : public GameObjectScript
{
public:
    go_gong() : GameObjectScript("go_gong") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go)
    {
        //basic support, not blizzlike data is missing...
        InstanceScript* instance = go->GetInstanceScript();

        if (instance)
        {
            instance->SetData(DATA_GONG_WAVES, instance->GetData(DATA_GONG_WAVES)+1);
            return true;
        }

        return false;
    }

};

enum eTombCreature
{
    SPELL_WEB                   = 745
};

class npc_tomb_creature : public CreatureScript
{
public:
    npc_tomb_creature() : CreatureScript("npc_tomb_creature") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_tomb_creatureAI (creature);
    }

    struct npc_tomb_creatureAI : public ScriptedAI
    {
        npc_tomb_creatureAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiWebTimer;

        void Reset()
        {
            uiWebTimer = urand(5000, 8000);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            //from acid
            if (me->GetEntry() == CREATURE_TOMB_REAVER)
            {
                if (uiWebTimer <= uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_WEB);
                    uiWebTimer = urand(7000, 16000);
                } else uiWebTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (instance)
                instance->SetData(DATA_GONG_WAVES, instance->GetData(DATA_GONG_WAVES)+1);
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_razorfen_downs()
{
    new npc_henry_stern();
    new go_gong();
    new npc_tomb_creature();
}
#endif
