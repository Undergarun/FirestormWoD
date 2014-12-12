#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

enum 
{
    SPELL_LEFT_ROPE             = 164400,
    SPELL_RIGHT_ROPE            = 164422,
    NPC_GARRISON_ALLIANCE_CART  = 81627,
    NPC_GARRISON_ALLIANCE_HORSE = 81633,
};

/// Garrison ROPE owner
class npc_GarrisonCartRope : public CreatureScript
{
    public:
        /// Constructor
        npc_GarrisonCartRope() 
            : CreatureScript("npc_GarrisonCartRope")
        { 

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_GarrisonCartRopeAI(p_Creature);
        }

        struct npc_GarrisonCartRopeAI : public CreatureAI
        {
            /// Constructor
            npc_GarrisonCartRopeAI(Creature * p_Creature) 
                : CreatureAI(p_Creature)
            {
                HasRope = false;
            }

            void Reset()
            {
                HasRope = false;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (me->GetEntry() == NPC_GARRISON_ALLIANCE_CART && !HasRope)
                {
                    Creature * l_Horse = me->FindNearestCreature(NPC_GARRISON_ALLIANCE_HORSE, 2.5f);

                    if (l_Horse)
                    {
                        me->CastSpell(l_Horse, SPELL_LEFT_ROPE, TRIGGERED_FULL_MASK);
                        HasRope = true;
                    }
                }
                else if (me->GetEntry() == NPC_GARRISON_ALLIANCE_HORSE && !HasRope)
                {
                    Creature * l_Cart = me->FindNearestCreature(NPC_GARRISON_ALLIANCE_CART, 4.0f);

                    if (l_Cart && l_Cart->GetEntry() == NPC_GARRISON_ALLIANCE_CART)
                    {
                        me->CastSpell(l_Cart, SPELL_RIGHT_ROPE, TRIGGERED_FULL_MASK);
                        HasRope = true;
                    }
                }
            }

            bool HasRope;
        };
};

void AddSC_Garrison_NPC()
{
    new npc_GarrisonCartRope;
}