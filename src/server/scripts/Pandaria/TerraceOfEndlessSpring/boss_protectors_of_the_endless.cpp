/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"

enum eProtectorsSpells
{
    // Shared
    SPELL_SHA_CORRUPTION                = 117052,
    SPELL_SHA_MASK                      = 118221,

    // Protector Kaolan
    SPELL_TOUCH_OF_SHA                  = 117519,
    SPELL_DEFILED_GROUND_SUMMON         = 117986,
    SPELL_DEFILED_GROUND_VISUAL         = 117989,
    SPELL_DEFILED_GROUND_STACKS         = 118091,
    SPELL_EXPEL_CORRUPTION_SUMMON       = 117975,
    SPELL_EXPEL_CORRUPTION_VISUAL       = 117943,

    // Ancient Regail
    SPELL_LIGHTNING_BOLT                = 117187,
    SPELL_LIGHTNING_PRISON              = 111850,
    SPELL_LIGHTNING_PRISON_STUN         = 117436,
    SPELL_LIGHTNING_STORM               = 118077,
    SPELL_LIGHTNING_STORM_FIRST         = 118064,
    SPELL_LIGHTNING_STORM_SECOND        = 118040,
    SPELL_LIGHTNING_STORM_THIRD         = 118053,
    SPELL_LIGHTNING_STORM_FOURTH        = 118054,
    SPELL_LIGHTNING_STORM_FIFTH         = 118055,
    SPELL_LIGHTNING_STORM_FIRST_DMG     = 118003,
    SPELL_LIGHTNING_STORM_SECOND_DMG    = 118004,
    SPELL_LIGHTNING_STORM_THIRD_DMG     = 118005,
    SPELL_LIGHTNING_STORM_FOURTH_DMG    = 118007,
    SPELL_LIGHTNING_STORM_FIFTH_DMG     = 118008,
    SPELL_OVERWHELMING_CORRUPTION       = 117351,
    SPELL_OVERWHELMING_CORRUPTION_STACK = 117353,

    // Ancient Asani
    SPELL_WATER_BOLT                    = 118312,
    SPELL_CLEANSING_WATERS_SUMMON       = 117309,
    SPELL_CLEANSING_WATERS_VISUAL       = 117250,
    SPELL_CLEANSING_WATERS_REGEN        = 117283,
    SPELL_CLEANSING_WATERS_MISSILE      = 117268,
    SPELL_CORRUPTING_WATERS_SUMMON      = 117227,
    SPELL_CORRUPTING_WATERS_AURA        = 117217,
    SPELL_PURIFIED                      = 117235
};

enum eProtectorsActions
{
    // Shared
    ACTION_FIRST_PROTECTOR_DIED,
    ACTION_SECOND_PROTECTOR_DIED
};

enum eProtectorsEvents
{
    // Protector Kaolan
    EVENT_TOUCH_OF_SHA,
    EVENT_DEFILED_GROUND,
    EVENT_EXPEL_CORRUPTION,

    // Ancient Regail
    EVENT_LIGHTNING_BOLT,
    EVENT_LIGHTNING_PRISON,
    EVENT_LIGHTNING_STORM,
    EVENT_OVERWHELMING_CORRUPTION,

    // Ancient Asani
    EVENT_WATER_BOLT,
    EVENT_CLEANSING_WATERS,
    EVENT_CORRUPTING_WATERS
};

enum eProtectorsSays
{
    TALK_INTRO,
    TALK_ASANI_AGGRO,
    TALK_REGAIL_AGGRO,
    TALK_KAOLAN_DIES_FIRST_ASANI,
    TALK_KAOLAN_DIES_FIRST_REGAIL,
    TALK_ASANI_DIES_FIRST_KAOLAN,
    TALK_ASANI_DIES_FIRST_REGAIL,
    TALK_CORRUPTED_WATERS,
    TALK_LIGHTNING_STORM,
    TALK_EXPEL_CORRUPTION,
    TALK_REGAIL_DIES_SECOND_ASANI,
    TALK_REGAIL_DIES_SECOND_KAOLAN,
    TALK_ASANI_DIES_SECOND_REGAIL,
    TALK_ASANI_DIES_SECOND_KAOLAN,
    TALK_ASANI_SLAY,
    TALK_REGAIL_SLAY,
    TALK_KAOLAN_SLAY,
    TALK_ASANI_DEATH,
    TALK_REGAIL_DEATH,
    TALK_KAOLAN_DEATH
};

enum eProtectorsEquipId
{
    ASANI_MH_ITEM   = 79832,
    KAOLAN_MH_ITEM  = 81390,
    REGAIL_ITEMS    = 81389
};

uint8 ProtectorsAlive(InstanceScript* instance, Creature* me)
{
    uint8 count = 0;
    if (!instance || !me)
        return count;

    Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));
    if (asani && asani->isAlive())
        ++count;

    Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
    if (regail && regail->isAlive())
        ++count;

    Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));
    if (kaolan && kaolan->isAlive())
        ++count;

    return count;
}

void RespawnProtectors(InstanceScript* instance, Creature* me)
{
    if (!instance || !me)
        return;

    Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));
    if (asani)
    {
        asani->Respawn();
        asani->GetMotionMaster()->MoveTargetedHome();
    }

    Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
    if (regail)
    {
        regail->Respawn();
        regail->GetMotionMaster()->MoveTargetedHome();
    }

    Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));
    if (kaolan)
    {
        kaolan->Respawn();
        kaolan->GetMotionMaster()->MoveTargetedHome();
    }
}

void StartProtectors(InstanceScript* instance, Creature* me, Unit* /*target*/)
{
    if (!instance)
        return;

    if (instance->GetBossState(DATA_PROTECTORS) == IN_PROGRESS)
        return; // Prevent recursive calls

    instance->SetBossState(DATA_PROTECTORS, IN_PROGRESS);

    Creature* asani = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_ASANI));
    if (asani)
        asani->SetInCombatWithZone();

    Creature* regail = instance->instance->GetCreature(instance->GetData64(NPC_ANCIENT_REGAIL));
    if (regail)
        regail->SetInCombatWithZone();

    Creature* kaolan = instance->instance->GetCreature(instance->GetData64(NPC_PROTECTOR_KAOLAN));
    if (kaolan)
        kaolan->SetInCombatWithZone();
}

class boss_ancient_regail : public CreatureScript
{
    public:
        boss_ancient_regail() : CreatureScript("boss_ancient_regail") { }

        struct boss_ancient_regailAI : public BossAI
        {
            boss_ancient_regailAI(Creature* creature) : BossAI(creature, DATA_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
			
			void Reset()
			{
                SetEquipmentSlots(false, REGAIL_ITEMS, REGAIL_ITEMS, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);

				_Reset();
				
				events.Reset();
				
				summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_OVERWHELMING_CORRUPTION);
                events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 5000);
                events.ScheduleEvent(EVENT_LIGHTNING_PRISON, 25000);
				
				if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);

                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    RespawnProtectors(pInstance, me);
                }
			}
			
			void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PROTECTORS, FAIL);
            }
			
			void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    StartProtectors(pInstance, me, attacker);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                    Talk(TALK_REGAIL_AGGRO);
                }
            }
			
			void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }
			
			void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_REGAIL_SLAY);
            }
			
			void JustDied(Unit* killer)
            {
                DoCast(SPELL_SHA_CORRUPTION);
                Talk(TALK_REGAIL_DEATH);

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    Creature* asani = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ANCIENT_ASANI));
                    Creature* kaolan = pInstance->instance->GetCreature(pInstance->GetData64(NPC_PROTECTOR_KAOLAN));

                    switch (ProtectorsAlive(pInstance, me))
                    {
                        case 2:
                        {
                            if (asani && asani->isAlive())
                                asani->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                            if (kaolan && kaolan->isAlive())
                                kaolan->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (asani && asani->isAlive())
                            {
                                asani->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                asani->AI()->Talk(TALK_REGAIL_DIES_SECOND_ASANI);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                asani->AI()->Talk(TALK_REGAIL_DIES_SECOND_KAOLAN);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            pInstance->SetBossState(DATA_PROTECTORS, DONE);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                            _JustDied();
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_FIRST_PROTECTOR_DIED:
                        firstSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_LIGHTNING_STORM, 10000);
                        me->SetFullHealth();
                        break;
                    case ACTION_SECOND_PROTECTOR_DIED:
                        secondSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_OVERWHELMING_CORRUPTION, 5000);
                        me->SetFullHealth();
                        break;
                    default:
                        break;
                }
            }
			
			void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_LIGHTNING_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_LIGHTNING_BOLT, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 3000);
                        break;
                    case EVENT_LIGHTNING_PRISON:
                        me->CastSpell(me, SPELL_LIGHTNING_PRISON, true);
                        events.ScheduleEvent(EVENT_LIGHTNING_PRISON, 25000);
                        break;
                    case EVENT_LIGHTNING_STORM:
                        if (!firstSpecialEnabled)
                            break;

                        Talk(TALK_LIGHTNING_STORM);
                        me->CastSpell(me, SPELL_LIGHTNING_STORM, true);

                        // Shorter CD in phase 3 (32s)
                        if (!secondSpecialEnabled)
                            events.ScheduleEvent(EVENT_LIGHTNING_STORM, 42000);
                        else
                            events.ScheduleEvent(EVENT_LIGHTNING_STORM, 32000);
                        break;
                    case EVENT_OVERWHELMING_CORRUPTION:
                        me->CastSpell(me, SPELL_OVERWHELMING_CORRUPTION, true);
                        break;
                    default:
                        break;
				}

                DoMeleeAttackIfReady();
			}
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_ancient_regailAI(creature);
        }
};

class boss_ancient_asani : public CreatureScript
{
    public:
        boss_ancient_asani() : CreatureScript("boss_ancient_asani") { }

        struct boss_ancient_asaniAI : public BossAI
        {
            boss_ancient_asaniAI(Creature* creature) : BossAI(creature, DATA_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
			
			void Reset()
			{
                SetEquipmentSlots(false, ASANI_MH_ITEM, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);

				_Reset();
				
				events.Reset();
				
				summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_OVERWHELMING_CORRUPTION);
                events.ScheduleEvent(EVENT_WATER_BOLT, 5000);
                events.ScheduleEvent(EVENT_CLEANSING_WATERS, 32500);
				
				if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);

                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    RespawnProtectors(pInstance, me);
                }
			}
			
			void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PROTECTORS, FAIL);
            }
			
			void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    StartProtectors(pInstance, me, attacker);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                    Talk(TALK_ASANI_AGGRO);
                }
            }
			
			void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }
			
			void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_ASANI_SLAY);
            }
			
			void JustDied(Unit* killer)
            {
                DoCast(SPELL_SHA_CORRUPTION);
                Talk(TALK_ASANI_DEATH);

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    Creature* regail = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ANCIENT_REGAIL));
                    Creature* kaolan = pInstance->instance->GetCreature(pInstance->GetData64(NPC_PROTECTOR_KAOLAN));

                    switch (ProtectorsAlive(pInstance, me))
                    {
                        case 2:
                        {
                            if (regail && regail->isAlive())
                            {
                                regail->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_ASANI_DIES_FIRST_REGAIL);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_ASANI_DIES_FIRST_KAOLAN);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (regail && regail->isAlive())
                            {
                                regail->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_ASANI_DIES_SECOND_REGAIL);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_ASANI_DIES_SECOND_KAOLAN);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            pInstance->SetBossState(DATA_PROTECTORS, DONE);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                            _JustDied();
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_FIRST_PROTECTOR_DIED:
                        firstSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_CORRUPTING_WATERS, 10000);
                        me->SetFullHealth();
                        break;
                    case ACTION_SECOND_PROTECTOR_DIED:
                        secondSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_OVERWHELMING_CORRUPTION, 5000);
                        me->SetFullHealth();
                        break;
                    default:
                        break;
                }
            }
			
			void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WATER_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_WATER_BOLT, false);
                        events.ScheduleEvent(EVENT_WATER_BOLT, 1000);
                        break;
                    case EVENT_CLEANSING_WATERS:
                        me->CastSpell(me, SPELL_CLEANSING_WATERS_SUMMON, false);
                        events.ScheduleEvent(EVENT_CLEANSING_WATERS, 32500);
                        break;
                    case EVENT_CORRUPTING_WATERS:
                        Talk(TALK_CORRUPTED_WATERS);
                        me->CastSpell(me, SPELL_CORRUPTING_WATERS_SUMMON, false);
                        events.ScheduleEvent(EVENT_CORRUPTING_WATERS, 42000);
                        break;
                    case EVENT_OVERWHELMING_CORRUPTION:
                        me->CastSpell(me, SPELL_OVERWHELMING_CORRUPTION, true);
                        break;
                    default:
                        break;
				}

                DoMeleeAttackIfReady();
			}
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_ancient_asaniAI(creature);
        }
};

class boss_protector_kaolan : public CreatureScript
{
    public:
        boss_protector_kaolan() : CreatureScript("boss_protector_kaolan") { }

        struct boss_protector_kaolanAI : public BossAI
        {
            boss_protector_kaolanAI(Creature* creature) : BossAI(creature, DATA_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool introDone;
			
			void Reset()
			{
                SetEquipmentSlots(false, KAOLAN_MH_ITEM, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);

				_Reset();
				
				events.Reset();
				
				summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;
                introDone = false;

                me->RemoveAura(SPELL_SHA_CORRUPTION);

                events.ScheduleEvent(EVENT_TOUCH_OF_SHA, 12000);
				
				if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);

                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    RespawnProtectors(pInstance, me);
                }
			}
			
			void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_PROTECTORS, FAIL);
            }
			
			void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    StartProtectors(pInstance, me, attacker);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }
            }
			
			void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!introDone && who->GetTypeId() == TYPEID_PLAYER)
                {
                    Talk(TALK_INTRO);
                    introDone = true;
                }
            }
			
			void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_KAOLAN_SLAY);
            }
			
			void JustDied(Unit* killer)
            {
                DoCast(SPELL_SHA_CORRUPTION);
                Talk(TALK_KAOLAN_DEATH);

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    Creature* regail = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ANCIENT_REGAIL));
                    Creature* asani = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ANCIENT_ASANI));

                    switch (ProtectorsAlive(pInstance, me))
                    {
                        case 2:
                        {
                            if (regail && regail->isAlive())
                            {
                                regail->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_KAOLAN_DIES_FIRST_REGAIL);
                            }

                            if (asani && asani->isAlive())
                            {
                                asani->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                regail->AI()->Talk(TALK_KAOLAN_DIES_FIRST_ASANI);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (regail && regail->isAlive())
                                regail->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                            if (asani && asani->isAlive())
                                asani->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            pInstance->SetBossState(DATA_PROTECTORS, DONE);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                            _JustDied();
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_FIRST_PROTECTOR_DIED:
                        firstSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_DEFILED_GROUND, 5000);
                        me->SetFullHealth();
                        break;
                    case ACTION_SECOND_PROTECTOR_DIED:
                        secondSpecialEnabled = true;
                        events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, urand(5000, 10000)); // 5-10s variation for first cast
                        me->SetFullHealth();
                        break;
                    default:
                        break;
                }
            }
			
			void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_TOUCH_OF_SHA:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true, -SPELL_TOUCH_OF_SHA))
                            me->CastSpell(target, SPELL_TOUCH_OF_SHA, false);
                        events.ScheduleEvent(EVENT_TOUCH_OF_SHA, 12000);
                        break;
                    case EVENT_DEFILED_GROUND:
                        if (!firstSpecialEnabled)
                            break;

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            me->CastSpell(target, SPELL_DEFILED_GROUND_SUMMON, true);
                        events.ScheduleEvent(EVENT_DEFILED_GROUND, 15500);
                        break;
                    case EVENT_EXPEL_CORRUPTION:
                        if (!secondSpecialEnabled)
                            break;

                        Talk(TALK_EXPEL_CORRUPTION);
                        me->CastSpell(me, SPELL_EXPEL_CORRUPTION_SUMMON, false);
                        events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, 38500);
                        break;
                    default:
                        break;
				}

                DoMeleeAttackIfReady();
			}
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_protector_kaolanAI(creature);
        }
};

// Defiled Ground - 60906
class mob_defiled_ground : public CreatureScript
{
    public:
        mob_defiled_ground() : CreatureScript("mob_defiled_ground") { }

        struct mob_defiled_groundAI : public ScriptedAI
        {
            mob_defiled_groundAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_DEFILED_GROUND_VISUAL, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_defiled_groundAI(creature);
        }
};

// Coalesced Corruption - 60886
class mob_coalesced_corruption : public CreatureScript
{
    public:
        mob_coalesced_corruption() : CreatureScript("mob_coalesced_corruption") { }

        struct mob_coalesced_corruptionAI : public ScriptedAI
        {
            mob_coalesced_corruptionAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_EXPEL_CORRUPTION_VISUAL, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_coalesced_corruptionAI(creature);
        }
};

// Cleansing Water - 60464
class mob_cleansing_water : public CreatureScript
{
    public:
        mob_cleansing_water() : CreatureScript("mob_cleansing_water") { }

        struct mob_cleansing_waterAI : public ScriptedAI
        {
            mob_cleansing_waterAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_CLEANSING_WATERS_VISUAL, true);
                me->CastSpell(me, SPELL_CLEANSING_WATERS_MISSILE, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_cleansing_waterAI(creature);
        }
};

// Corrupting Waters - 60621
class mob_corrupting_waters : public CreatureScript
{
    public:
        mob_corrupting_waters() : CreatureScript("mob_corrupting_waters") { }

        struct mob_corrupting_watersAI : public ScriptedAI
        {
            mob_corrupting_watersAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_CORRUPTING_WATERS_AURA, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void JustDied(Unit* killer)
            {
                me->CastSpell(me, SPELL_PURIFIED, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_corrupting_watersAI(creature);
        }
};

// Expelled Corruption (triggered) - 117955
class spell_expelled_corruption : public SpellScriptLoader
{
    public:
        spell_expelled_corruption() : SpellScriptLoader("spell_expelled_corruption") { }

        class spell_expelled_corruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_expelled_corruption_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;
                
                float distance = caster->GetExactDist2d(target);

                if (distance >= 0.0f && distance <= 30.0f)
                    SetHitDamage(GetHitDamage() * (1 - (distance / 30.0f)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_expelled_corruption_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_expelled_corruption_SpellScript();
        }
};

// Lightning Storm - 118064, 118040, 118053, 118054, 118055, 118077
class spell_lightning_storm_aura : public SpellScriptLoader
{
    public:
        spell_lightning_storm_aura() : SpellScriptLoader("spell_lightning_storm_aura") { }

        class spell_lightning_storm_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lightning_storm_aura_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetSpellInfo()->Id != SPELL_LIGHTNING_STORM)
                    return;

                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIRST, true);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    switch (GetSpellInfo()->Id)
                    {
                        case SPELL_LIGHTNING_STORM_FIRST:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIRST_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_SECOND, true);
                            break;
                        case SPELL_LIGHTNING_STORM_SECOND:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_SECOND_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_THIRD, true);
                            break;
                        case SPELL_LIGHTNING_STORM_THIRD:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_THIRD_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FOURTH, true);
                            break;
                        case SPELL_LIGHTNING_STORM_FOURTH:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FOURTH_DMG, true);
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIFTH, true);
                            break;
                        case SPELL_LIGHTNING_STORM_FIFTH:
                            caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIFTH_DMG, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_lightning_storm_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_lightning_storm_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lightning_storm_aura_AuraScript();
        }
};

// Lightning Storm (damage) - 118004, 118005, 118007, 118008
class spell_lightning_storm_damage : public SpellScriptLoader
{
    public:
        spell_lightning_storm_damage() : SpellScriptLoader("spell_lightning_storm_damage") { }

        class spell_lightning_storm_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_storm_damage_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                float MaxDist = 0.0f;
                float MinDist = 0.0f;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_LIGHTNING_STORM_SECOND_DMG:
                        MinDist = 10.0f;
                        MaxDist = 20.0f;
                        break;
                    case SPELL_LIGHTNING_STORM_THIRD_DMG:
                        MinDist = 30.0f;
                        MaxDist = 40.0f;
                        break;
                    case SPELL_LIGHTNING_STORM_FOURTH_DMG:
                        MinDist = 50.0f;
                        MaxDist = 60.0f;
                        break;
                    case SPELL_LIGHTNING_STORM_FIFTH_DMG:
                        MinDist = 70.0f;
                        MaxDist = 80.0f;
                        break;
                    default:
                        break;
                }

                Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (Player* player = itr->getSource())
                            if (player->GetExactDist2d(GetCaster()->GetPositionX(), GetCaster()->GetPositionY()) <= MaxDist &&
                                player->GetExactDist2d(GetCaster()->GetPositionX(), GetCaster()->GetPositionY()) >= MinDist)
                                targets.push_back(player);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lightning_storm_damage_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lightning_storm_damage_SpellScript();
        }
};

void AddSC_boss_protectors_of_the_endless()
{
    new boss_ancient_regail();
    new boss_ancient_asani();
    new boss_protector_kaolan();
    new mob_defiled_ground();
    new mob_coalesced_corruption();
    new mob_cleansing_water();
    new mob_corrupting_waters();
    new spell_expelled_corruption();
    new spell_lightning_storm_aura();
    new spell_lightning_storm_damage();
}