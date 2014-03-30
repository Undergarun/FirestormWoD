/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "ulduar.h"

enum AuriayaSpells
{
    // Auriaya
    SPELL_TERRIFYING_SCREECH        = 64386,
    SPELL_SENTINEL_BLAST_10         = 64389,
    SPELL_SENTINEL_BLAST_25         = 64678,
    SPELL_SONIC_SCREECH_10          = 64422,
    SPELL_SONIC_SCREECH_25          = 64688,
    SPELL_SUMMON_SWARMING_GUARDIAN  = 64396,
    SPELL_ACTIVATE_DEFENDER         = 64449,
    SPELL_DEFENDER_TRIGGER          = 64448,
    SPELL_SUMMON_DEFENDER           = 64447,
    SPELL_BERSERK                   = 47008,

    // Feral Defender
    SPELL_FERAL_RUSH_10             = 64496,
    SPELL_FERAL_RUSH_25             = 64674,
    SPELL_FERAL_POUNCE_10           = 64478,
    SPELL_FERAL_POUNCE_25           = 64669,
    SPELL_SEEPING_FERAL_ESSENCE_10  = 64458,
    SPELL_SEEPING_FERAL_ESSENCE_25  = 64676,
    SPELL_SUMMON_ESSENCE            = 64457,
    SPELL_FERAL_ESSENCE             = 64455,
    SPELL_FULL_HEAL                 = 64460,

    // Sanctum Sentry
    SPELL_SAVAGE_POUNCE_10          = 64666,
    SPELL_SAVAGE_POUNCE_25          = 64374,
    SPELL_RIP_FLESH_10              = 64375,
    SPELL_RIP_FLESH_25              = 64667,
    SPELL_STRENGHT_OF_THE_PACK      = 64369, // NIY
    SPELL_STRENGHT_OF_THE_PACK_TR   = 64381, // works
};

#define SPELL_SENTINEL_BLAST        RAID_MODE(SPELL_SENTINEL_BLAST_10, SPELL_SENTINEL_BLAST_25)
#define SPELL_SONIC_SCREECH         RAID_MODE(SPELL_SONIC_SCREECH_10, SPELL_SONIC_SCREECH_25)
#define SPELL_FERAL_RUSH            RAID_MODE(SPELL_FERAL_RUSH_10, SPELL_FERAL_RUSH_25)
#define SPELL_FERAL_POUNCE          RAID_MODE(SPELL_FERAL_POUNCE_10, SPELL_FERAL_POUNCE_25)
#define SPELL_SEEPING_FERAL_ESSENCE RAID_MODE(SPELL_SEEPING_FERAL_ESSENCE_10, SPELL_SEEPING_FERAL_ESSENCE_25)
#define SPELL_SAVAGE_POUNCE         RAID_MODE(SPELL_SAVAGE_POUNCE_10, SPELL_SAVAGE_POUNCE_25)
#define SPELL_RIP_FLESH             RAID_MODE(SPELL_RIP_FLESH_10, SPELL_RIP_FLESH_25)

enum AuriayaNPCs
{
    NPC_SANCTUM_SENTRY          = 34014,
    NPC_FERAL_DEFENDER          = 34035,
    NPC_FERAL_DEFENDER_TRIGGER  = 34096,
    NPC_SEEPING_TRIGGER         = 34098,
};

enum AuriayaEvents
{
    // Auriaya
    EVENT_SONIC_SCREECH             = 1,
    EVENT_SENTINEL_BLAST            = 2,
    EVENT_TERRIFYING_SCREECH        = 3,
    EVENT_SUMMON_SWARMING_GUARDIAN  = 4,
    EVENT_ACTIVATE_DEFENDER         = 5,
    EVENT_RESPAWN_DEFENDER          = 6,
    EVENT_BERSERK                   = 7,

    // Sanctum Sentry
    EVENT_RIP                       = 8,
    EVENT_POUNCE                    = 9,
    EVENT_STRENGTH_OF_THE_PACK      = 10,

    // Feral Defender
    EVENT_FERAL_POUNCE              = 11,
    EVENT_RUSH                      = 12,
};

enum AuriayaYells
{
    // Yells
    SAY_AGGRO       = 0,
    SAY_SLAY        = 1,
    SAY_BERSERK     = 2,

    // Emotes
    EMOTE_FEAR      = 3,
    EMOTE_DEFENDER  = 4,
};

enum SoundIds
{
    SOUND_ID_DEATH  = 15476
};

enum AuriayaActions
{
    ACTION_CRAZY_CAT_LADY   = 0,
    ACTION_RESPAWN_DEFENDER = 1
};

#define SENTRY_NUMBER   RAID_MODE<uint8>(2, 4)

enum Data
{
    DATA_NINE_LIVES         = 1,
    DATA_CRAZY_CAT_LADY     = 2,

    MODEL_INVISIBLE         = 11686
};

class SanctumSentrySearcher
{
    public:
        SanctumSentrySearcher(Creature const* source, float range) : _source(source), _range(range) {}

        bool operator()(Unit* unit)
        {
            if (!unit->isAlive() || unit->GetEntry() != NPC_SANCTUM_SENTRY || !unit->IsWithinDist(_source, _range, false))
                return false;

            return true;
        }

    private:
        Creature const* _source;
        float _range;
};

class boss_auriaya : public CreatureScript
{
    public:
        boss_auriaya() : CreatureScript("boss_auriaya") { }

        struct boss_auriayaAI : public BossAI
        {
            boss_auriayaAI(Creature* creature) : BossAI(creature, BOSS_AURIAYA)
            {
            }

            void Reset()
            {
                _Reset();
                defenderLives = 9;
                crazyCatLady = true;
                nineLives = false;
                
                // Guardians are despawned by _Reset, but since they walk around with Auriaya, summon them again.
                for (uint8 i = 0; i < SENTRY_NUMBER; i++)
                    if (Creature* sentry = me->SummonCreature(NPC_SANCTUM_SENTRY, *me, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000)) // 30 secs equal the automated respawn time (due to script)
                    {
                        sentry->GetMotionMaster()->MoveFollow(me, (i < 2) ? 0.5f : 4.0f, M_PI - i - 1.5f);
                        summons.Summon(sentry);
                    }
            }

            void EnterCombat(Unit* who)
            {
                if (!instance->CheckRequiredBosses(BOSS_AURIAYA, who->ToPlayer()))
                {
                    EnterEvadeMode();
                    instance->DoCastSpellOnPlayers(SPELL_BASE_CAMP_TELEPORT);
                    return;
                }

                _EnterCombat();
                summons.DoZoneInCombat();
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_SONIC_SCREECH, urand(45000, 65000));
                events.ScheduleEvent(EVENT_TERRIFYING_SCREECH, urand(30000, 50000));
                events.ScheduleEvent(EVENT_ACTIVATE_DEFENDER, urand(40000, 55000));
                events.ScheduleEvent(EVENT_SUMMON_SWARMING_GUARDIAN, urand(45000, 55000));
                events.ScheduleEvent(EVENT_BERSERK, 600000);
            }

            void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_SLAY);
            }

            void JustSummoned(Creature* summoned)
            {
                summons.Summon(summoned);

                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                {
                    summoned->AI()->AttackStart(target);
                    summoned->AddThreat(target, 250.0f);
                    DoZoneInCombat(summoned);
                }

                if (summoned->GetEntry() == NPC_FERAL_DEFENDER)
                {
                    if (!summoned->isInCombat() && me->getVictim())
                        summoned->AI()->AttackStart(me->getVictim());
                    summoned->SetAuraStack(SPELL_FERAL_ESSENCE, summoned, defenderLives);
                    DoZoneInCombat(summoned);
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_NINE_LIVES:
                        return nineLives ? 1 : 0;
                    case DATA_CRAZY_CAT_LADY:
                        return crazyCatLady ? 1 : 0;
                }

                return 0;
            }

            void SetData(uint32 id, uint32 data)
            {
               switch (id)
               {
                   case DATA_NINE_LIVES:
                        nineLives = data ? true : false;
                        break;
                    case DATA_CRAZY_CAT_LADY:
                        crazyCatLady = data ? true : false;
                        break;
               }
            }
            
            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                switch (summon->GetEntry())
                {
                    case NPC_FERAL_DEFENDER:
                        --defenderLives;
                        if (!defenderLives)
                        {
                            SetData(DATA_NINE_LIVES, 1);
                            break;
                        }
                        me->SummonCreature(NPC_SEEPING_TRIGGER, *summon);
                        events.ScheduleEvent(EVENT_RESPAWN_DEFENDER, 30000);
                        break;
                    case NPC_SANCTUM_SENTRY:
                        SetData(DATA_CRAZY_CAT_LADY, 0);
                        break;
                    default:
                        break;
                }
                summons.Despawn(summon);
            }

            void JustDied(Unit* /*who*/)
            {
                me->PlayDirectSound(SOUND_ID_DEATH);
                _JustDied();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SONIC_SCREECH:
                            DoCast(SPELL_SONIC_SCREECH);
                            events.ScheduleEvent(EVENT_SONIC_SCREECH, urand(40000, 60000));
                            return;
                        case EVENT_TERRIFYING_SCREECH:
                            Talk(EMOTE_FEAR);
                            DoCast(SPELL_TERRIFYING_SCREECH);
                            events.ScheduleEvent(EVENT_TERRIFYING_SCREECH, urand(30000, 50000));
                            events.ScheduleEvent(EVENT_SENTINEL_BLAST, 1*IN_MILLISECONDS);
                            return;
                        case EVENT_SENTINEL_BLAST:
                            DoCastAOE(SPELL_SENTINEL_BLAST);
                            return;
                        case EVENT_ACTIVATE_DEFENDER:
                            Talk(EMOTE_DEFENDER);
                            DoCast(SPELL_DEFENDER_TRIGGER);
                            if (Creature* trigger = me->FindNearestCreature(NPC_FERAL_DEFENDER_TRIGGER, 50.0f))
                                DoCast(trigger, SPELL_ACTIVATE_DEFENDER, true);
                            return;
                        case EVENT_SUMMON_SWARMING_GUARDIAN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_SUMMON_SWARMING_GUARDIAN);
                            events.ScheduleEvent(EVENT_SUMMON_SWARMING_GUARDIAN, urand(30000, 45000));
                            return;
                        case EVENT_RESPAWN_DEFENDER:
                            if (defenderLives > 0)
                                if (Creature* corpse = me->FindNearestCreature(NPC_FERAL_DEFENDER, 50.0f, false))
                                {
                                    corpse->DespawnOrUnsummon();
                                    if (Creature* feralDefender = me->SummonCreature(NPC_FERAL_DEFENDER, *corpse))
                                        feralDefender->CastSpell(feralDefender, SPELL_FULL_HEAL, true);                                    
                                }
                            return;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK, true);
                            Talk(SAY_BERSERK);
                            return;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            uint8 defenderLives;
            bool crazyCatLady;
            bool nineLives;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_auriayaAI>(creature);
        }
};

class npc_auriaya_seeping_trigger : public CreatureScript
{
    public:
        npc_auriaya_seeping_trigger() : CreatureScript("npc_auriaya_seeping_trigger") { }

        struct npc_auriaya_seeping_triggerAI : public ScriptedAI
        {
            npc_auriaya_seeping_triggerAI(Creature* creature) : ScriptedAI(creature), instance(me->GetInstanceScript()) {}
            
            void Reset()
            {
                me->SetDisplayId(MODEL_INVISIBLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE);
                me->DespawnOrUnsummon(600000);
                DoCast(me, SPELL_SEEPING_FERAL_ESSENCE);
            }

            void UpdateAI(uint32 const /*diff*/)
            {
                if (instance->GetBossState(BOSS_AURIAYA) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_auriaya_seeping_triggerAI(creature);
        }
};

class npc_sanctum_sentry : public CreatureScript
{
    public:
        npc_sanctum_sentry() : CreatureScript("npc_sanctum_sentry") { }

        struct npc_sanctum_sentryAI : public ScriptedAI
        {
            npc_sanctum_sentryAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.ScheduleEvent(EVENT_RIP, urand(4000, 8000));
                events.ScheduleEvent(EVENT_POUNCE, urand(12000, 15000));
                events.ScheduleEvent(EVENT_STRENGTH_OF_THE_PACK, 1000);
            }

            void EnterCombat(Unit* /*who*/)
            {
                //DoCast(me, SPELL_STRENGHT_OF_THE_PACK, true);
                
                if (me->ToTempSummon())
                {
                    Unit* auriaya = me->ToTempSummon()->GetSummoner();
                    if (auriaya && auriaya->ToCreature() && !auriaya->isInCombat())
                        auriaya->ToCreature()->SetInCombatWithZone();
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RIP:
                            DoCastVictim(SPELL_RIP_FLESH);
                            events.ScheduleEvent(EVENT_RIP, urand(12000, 15000));
                            break;
                        case EVENT_POUNCE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoResetThreat();
                                me->AddThreat(target, 100.0f);
                                me->AI()->AttackStart(target);
                                DoCast(target, SPELL_SAVAGE_POUNCE);
                            }
                            events.ScheduleEvent(EVENT_POUNCE, urand(12000, 17000));
                            break;
                        case EVENT_STRENGTH_OF_THE_PACK:
                        {
                            std::list<Creature*> sanctumSentrys;
                            SanctumSentrySearcher check(me, 10.0f);
                            JadeCore::CreatureListSearcher<SanctumSentrySearcher> searcher(me, sanctumSentrys, check);
                            me->VisitNearbyGridObject(10.0f, searcher);
                            for (std::list<Creature*>::iterator itr = sanctumSentrys.begin(); itr != sanctumSentrys.end(); ++itr)
                            {
                                if ((*itr) != me)
                                {
                                    me->AddAura(SPELL_STRENGHT_OF_THE_PACK_TR, me);
                                    break;
                                }
                                else
                                    continue;
                            }
                            events.ScheduleEvent(EVENT_STRENGTH_OF_THE_PACK, 1500);
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sanctum_sentryAI(creature);
        }
};

class npc_feral_defender : public CreatureScript
{
    public:
        npc_feral_defender() : CreatureScript("npc_feral_defender") { }

        struct npc_feral_defenderAI : public ScriptedAI
        {
            npc_feral_defenderAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.ScheduleEvent(EVENT_FERAL_POUNCE, 5000);
                events.ScheduleEvent(EVENT_RUSH, 10000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FERAL_POUNCE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoResetThreat();
                                me->AddThreat(target, 100.0f);
                                me->AI()->AttackStart(target);
                                DoCast(target, SPELL_FERAL_POUNCE);
                            }
                            events.ScheduleEvent(EVENT_FERAL_POUNCE, urand(10000, 12000));
                            break;
                        case EVENT_RUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoResetThreat();
                                me->AddThreat(target, 100.0f);
                                me->AI()->AttackStart(target);
                                DoCast(target, SPELL_FERAL_RUSH);
                            }
                            events.ScheduleEvent(EVENT_RUSH, urand(10000, 12000));
                            break;
                    default:
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*who*/)
            {
                DoCast(me, SPELL_SUMMON_ESSENCE);
            }

        private:
            InstanceScript* instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_feral_defenderAI(creature);
        }
};
/*
class SanctumSentryCheck
{
    public:
        bool operator()(WorldObject* object) const
        {
            if (object->GetEntry() == NPC_SANCTUM_SENTRY)
                return false;

            return true;
        }
};

class spell_auriaya_strenght_of_the_pack : public SpellScriptLoader
{
    public:
        spell_auriaya_strenght_of_the_pack() : SpellScriptLoader("spell_auriaya_strenght_of_the_pack") { }

        class spell_auriaya_strenght_of_the_pack_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_auriaya_strenght_of_the_pack_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(SanctumSentryCheck());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_auriaya_strenght_of_the_pack_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_auriaya_strenght_of_the_pack_SpellScript();
        }
};
*/
class spell_auriaya_sentinel_blast : public SpellScriptLoader
{
    public:
        spell_auriaya_sentinel_blast() : SpellScriptLoader("spell_auriaya_sentinel_blast") { }

        class spell_auriaya_sentinel_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_auriaya_sentinel_blast_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(PlayerOrPetCheck());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_auriaya_sentinel_blast_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_auriaya_sentinel_blast_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_auriaya_sentinel_blast_SpellScript();
        }
};


class achievement_nine_lives : public AchievementCriteriaScript
{
    public:
        achievement_nine_lives() : AchievementCriteriaScript("achievement_nine_lives")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Auriaya = target->ToCreature())
                if (Auriaya->AI()->GetData(DATA_NINE_LIVES))
                    return true;

            return false;
        }
};

class achievement_crazy_cat_lady : public AchievementCriteriaScript
{
    public:
        achievement_crazy_cat_lady() : AchievementCriteriaScript("achievement_crazy_cat_lady")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Auriaya = target->ToCreature())
                if (Auriaya->AI()->GetData(DATA_CRAZY_CAT_LADY))
                    return true;

            return false;
        }
};

void AddSC_boss_auriaya()
{
    new boss_auriaya();
    new npc_auriaya_seeping_trigger();
    new npc_feral_defender();
    new npc_sanctum_sentry();
    //new spell_auriaya_strenght_of_the_pack();
    new spell_auriaya_sentinel_blast();
    new achievement_nine_lives();
    new achievement_crazy_cat_lady();
}

#undef SPELL_SENTINEL_BLAST 
#undef SPELL_SONIC_SCREECH 
#undef SPELL_FERAL_RUSH 
#undef SPELL_FERAL_POUNCE 
#undef SPELL_SEEPING_FERAL_ESSENCE 
#undef SPELL_SAVAGE_POUNCE 
#undef SPELL_RIP_FLESH