////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_the_niuzao_temple.h"

enum spells
{
    MALLEABLE_RESIN = 121421,
    RESIN_WEAVING = 121114,
    ENCASED_IN_RESIN = 121116,
    RESIN_SHELL = 120946,
    RESIDUE = 120938
};

class mob_sikthik_guardian : public CreatureScript
{
    public:
        mob_sikthik_guardian() : CreatureScript("mob_sikthik_guardian") {}

        struct mob_sikthik_guardianAI : public ScriptedAI
        {
            mob_sikthik_guardianAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 malleableResinTimer;

            void Reset()
            {
                malleableResinTimer = urand(5000, 8000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                if (malleableResinTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                        if (!target->IsFriendlyTo(me))
                            me->CastSpell(target, MALLEABLE_RESIN, true);
                    malleableResinTimer = urand(8000, 12000);
                }
                else
                    malleableResinTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sikthik_guardianAI(creature);
        }
};

class mob_resin_flake : public CreatureScript
{
    public:
        mob_resin_flake() : CreatureScript("mob_resin_flake") {}

        struct mob_resin_flakeAI : public ScriptedAI
        {
            mob_resin_flakeAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 residueTimer;

            void Reset()
            {
                residueTimer = urand(5000, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                if (residueTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                        if (!target->IsFriendlyTo(me))
                            me->CastSpell(target, RESIDUE, true);
                    residueTimer = urand(5000, 7000);
                }
                else
                    residueTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_resin_flakeAI(creature);
        }
};

class mob_sikthik_amber_weaver : public CreatureScript
{
    public:
        mob_sikthik_amber_weaver() : CreatureScript("mob_sikthik_amber_weaver") {}

        struct mob_sikthik_amber_weaverAI : public ScriptedAI
        {
            mob_sikthik_amber_weaverAI(Creature* creature) : ScriptedAI(creature) { }

            uint32 resinWeavingTimer;
            uint32 resinShellTimer;
            bool resinShellOnlyOne;

            void Reset()
            {
                resinWeavingTimer = urand(10000, 12000);
                resinShellTimer = urand(8000, 15000);
                resinShellOnlyOne = true;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                if (resinWeavingTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                        if (!target->IsFriendlyTo(me))
                            me->CastSpell(target, RESIN_WEAVING, true);
                    resinWeavingTimer = urand(10000, 12000);
                }
                else
                    resinWeavingTimer -= diff;

                if (resinShellTimer <= diff && resinShellOnlyOne == true)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                    {
                        me->CastSpell(me, RESIN_SHELL, true);
                        resinShellOnlyOne = false;
                    }
                }
                else
                    resinShellTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sikthik_amber_weaverAI(creature);
        }
};

class spell_resin_weaving: public SpellScriptLoader
{
    public:
        spell_resin_weaving() : SpellScriptLoader("spell_resin_weaving") { }

        class spell_resin_weaving_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_resin_weaving_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                        if (removeMode == AURA_REMOVE_BY_EXPIRE)
                            caster->CastSpell(target, ENCASED_IN_RESIN, true);
                    }
                }
            }
            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_resin_weaving_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_resin_weaving_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_siege_of_the_niuzao_temple()
{
    new mob_sikthik_guardian();
    new mob_resin_flake();
    new mob_sikthik_amber_weaver();
    new spell_resin_weaving();
}
#endif
