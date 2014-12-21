#include "ScriptPCH.h"
#include "LFGMgr.h"
#include "Group.h"
#include "shadowfang_keep.h"

class npc_apothecary_hummel : public CreatureScript
{
    public:
        npc_apothecary_hummel() : CreatureScript("npc_apothecary_hummel") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_apothecary_hummelAI (pCreature);
        }

        bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
        {
            pPlayer->PlayerTalkClass->ClearMenus();
            pPlayer->CLOSE_GOSSIP_MENU();

            if (uiAction == 1)
            {
                if (Creature* pFrye = pCreature->FindNearestCreature(NPC_APOTHECARY_FRYE, 100.0f))
                    pFrye->setFaction(14);
                if (Creature* pBaxter = pCreature->FindNearestCreature(NPC_APOTHECARY_BAXTER, 100.0f))
                    pBaxter->setFaction(14);
                pCreature->setFaction(14);
            }
            return false;
        }

        struct npc_apothecary_hummelAI : public ScriptedAI
        {
            npc_apothecary_hummelAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
            }

            void JustDied(Unit* /*killer*/)
            {
            }
        };
};

void AddSC_shadowfang_keep()
{
    new npc_apothecary_hummel();
}
