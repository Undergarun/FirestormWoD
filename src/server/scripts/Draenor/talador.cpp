////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "GameObjectAI.h"
#include "Language.h"

/// Warlock Soulstealer - 79482
class npc_talador_warlock_soulstealer : public CreatureScript
{
    enum
    {
        WarlockSoulstealer              = 79482,
        DraeneiSpirit                   = 79478,
        KillCredit_RescueDraeneiSpirits = 79499,
        TormentedSoul                   = 160508
    };

    public:
        npc_talador_warlock_soulstealer() : CreatureScript("npc_talador_warlock_soulstealer") { }

        struct npc_talador_warlock_soulstealerAI : public ScriptedAI
        {
            npc_talador_warlock_soulstealerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            /// Called when the creature is killed
            void JustDied(Unit* p_Killer) override
            {
                if (p_Killer->IsPlayer())
                {
                    std::list<Unit*> l_NearCreatures;
                    JadeCore::AllCreaturesOfEntryInRange l_UCheck(me, DraeneiSpirit, 15.0f);
                    JadeCore::UnitListSearcher<JadeCore::AllCreaturesOfEntryInRange> l_Searcher(me, l_NearCreatures, l_UCheck);
                    me->VisitNearbyObject(15.0f, l_Searcher);

                    for (Unit* l_Unit : l_NearCreatures)
                    {
                        if (l_Unit->HasAura(TormentedSoul))
                        {
                            l_Unit->RemoveAura(TormentedSoul);
                            l_Unit->GetMotionMaster()->MoveFleeing(me, 5 * TimeConstants::IN_MILLISECONDS);
                            l_Unit->ToCreature()->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
                            p_Killer->ToPlayer()->KilledMonsterCredit(KillCredit_RescueDraeneiSpirits);
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_talador_warlock_soulstealerAI(p_Creature);
        }
};

/// Zorka's Void Gate - 79520
class npc_talador_zorkas_void_gate : public CreatureScript
{
    enum
    {
        BonusObjectiveZorkraFall = 34660,
        ZorkaVoidGate = 79520
    };

    public:
        npc_talador_zorkas_void_gate() : CreatureScript("npc_talador_zorkas_void_gate") { }

        struct npc_talador_zorkas_void_gateAI : public ScriptedAI
        {
            npc_talador_zorkas_void_gateAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void OnSpellClick(Unit* p_Unit) override
            {
                if (p_Unit->IsPlayer() && p_Unit->ToPlayer()->HasQuest(BonusObjectiveZorkraFall))
                {
                    p_Unit->ToPlayer()->KilledMonsterCredit(ZorkaVoidGate);
                    me->DespawnOrUnsummon(0);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_talador_zorkas_void_gateAI(p_Creature);
        }
};

void AddSC_talador()
{
    /// Npcs
    new npc_talador_warlock_soulstealer();
    new npc_talador_zorkas_void_gate();
}