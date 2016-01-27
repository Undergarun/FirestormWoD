////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "ScriptedCreature.h"

/// Mini Mana Bomb - 65903
class npc_toy_mini_maba_bomb : public CreatureScript
{
    public:
        npc_toy_mini_maba_bomb() : CreatureScript("npc_toy_mini_maba_bomb") { }

        enum eSpell
        {
            ArcaneExplosionVisualLarge = 98122
        };

        struct npc_toy_mini_maba_bombAI : public ScriptedAI
        {
            npc_toy_mini_maba_bombAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS + 900, [this]() -> void
                {
                    me->CastSpell(me, eSpell::ArcaneExplosionVisualLarge, true);
                    me->DespawnOrUnsummon();
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_toy_mini_maba_bombAI(p_Creature);
        }
};

void AddSC_npc_toy()
{
    new npc_toy_mini_maba_bomb();
}