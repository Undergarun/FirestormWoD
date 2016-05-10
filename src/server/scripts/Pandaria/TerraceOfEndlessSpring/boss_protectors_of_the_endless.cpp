////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GameObjectAI.h"
#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
#include "LFGMgr.h"

#define ENTRANCE_ORIENTATION 4.723f

enum eProtectorsSpells
{
    // Shared
    SPELL_SHA_CORRUPTION                = 117052,
    SPELL_SHA_MASK                      = 118221,
    SPELL_PROTECTORS_BONUS              = 132200,
    SPELL_PROTECTORS_BONUS_ELITE        = 132204,
    SPELL_OVERPOWERED                   = 117230,

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
    SPELL_CORRUPTING_WATERS_SUMMON      = 117227,
    SPELL_CORRUPTING_WATERS_AURA        = 117217,
    SPELL_PURIFIED                      = 117235,

    // Minions of Fear
    SPELL_CORRUPTED_ESSENCE             = 118191,
    SPELL_ESSENCE_OF_FEAR               = 118198,
    SPELL_SUPERIOR_CORRUPTED_ESSENCE    = 117905,
    SPELL_SUPERIOR_ESSENCE_OF_FEAR      = 118186
};

enum eProtectorsActions
{
    // Shared
    ACTION_FIRST_PROTECTOR_DIED     = 0,
    ACTION_SECOND_PROTECTOR_DIED    = 1,
    ACTION_DESPAWN_SUMMONS          = 3,
    ACTION_INIT_MINION_CONTROLLER   = 4,
    ACTION_RESET_MINION_CONTROLLER  = 5,
    ACTION_BOSS_WIPE                = 6,
    ACTION_BOSS_ENTER_COMBAT        = 7
};

enum eProtectorsEvents
{
    // Protector Kaolan
    EVENT_TOUCH_OF_SHA              = 1,
    EVENT_DEFILED_GROUND            = 2,
    EVENT_EXPEL_CORRUPTION          = 3,

    // Ancient Regail
    EVENT_LIGHTNING_BOLT            = 4,
    EVENT_LIGHTNING_PRISON          = 5,
    EVENT_LIGHTNING_STORM           = 6,
    EVENT_OVERWHELMING_CORRUPTION   = 7,

    // Ancient Asani
    EVENT_WATER_BOLT                = 8,
    EVENT_CLEANSING_WATERS          = 9,
    EVENT_CORRUPTING_WATERS         = 10,

    // Adds
    EVENT_REFRESH_CLEANSING_WATERS  = 11,
    EVENT_DESPAWN_CLEANSING_WATERS  = 12,
    EVENT_SPAWN_MINION_OF_FEAR      = 13
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

enum eProtectorsType
{
    TYPE_SET_WIPE = 1,
    TYPE_UNSET_WIPE
};

uint32 bossEntries[3] = { NPC_ANCIENT_ASANI, NPC_ANCIENT_REGAIL, NPC_PROTECTOR_KAOLAN };

uint8 ProtectorsAlive(InstanceScript* instance, Creature* me)
{
    uint8 count = 0;
    if (!instance || !me)
        return count;

    for (uint8 i = 0; i < 3; ++i)
    {
        if (Creature* protector = instance->instance->GetCreature(instance->GetData64(bossEntries[i])))
        {
            if (protector->isAlive())
                ++count;
        }
    }

        return count;
}

void RespawnProtectors(InstanceScript* instance, Creature* me)
{
    if (!instance || !me)
        return;

    for (int i = 0; i < 3; ++i)
    {
        if (Creature* protector = instance->instance->GetCreature(instance->GetData64(bossEntries[i])))
        {
            protector->Respawn();
            protector->GetMotionMaster()->MoveTargetedHome();
        }
    }
}

bool StartProtectors(InstanceScript* instance, Creature* /*me*/, Unit* target)
{
    if (!instance)
        return false;

    if (instance->GetBossState(DATA_PROTECTORS) == IN_PROGRESS)
        return false; // Prevent recursive calls

    instance->SetBossState(DATA_PROTECTORS, IN_PROGRESS);

    for (uint8 i = 0; i < 3; ++i)
    {
        if (Creature* protector = instance->instance->GetCreature(instance->GetData64(bossEntries[i])))
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, protector);
            protector->AI()->SetData(TYPE_UNSET_WIPE, 0);
            protector->AI()->DoAction(ACTION_BOSS_ENTER_COMBAT);
            protector->AI()->AttackStart(target);
            protector->SetInCombatWith(target);
        }
    }

    if (instance->instance->IsHeroic())
    {
        if (Creature* minionController = instance->instance->GetCreature(instance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
            minionController->AI()->DoAction(ACTION_INIT_MINION_CONTROLLER);
    }

    return true;
}

bool IntroDone(InstanceScript* instance, Creature* me)
{
    if (!instance || !me)
        return false;

    if (instance->GetData(INTRO_DONE) > 0)
        return true;

    bool done = false;
    if (!GetClosestCreatureWithEntry(me, NPC_APPARITION_OF_FEAR, 100.0f))
    {
        if (!GetClosestCreatureWithEntry(me, NPC_APPARITION_OF_TERROR, 100.0f))
            done = true;
    }

    if (done && instance)
        instance->SetData(INTRO_DONE, 1);

    return done;
}

void ProtectorsWipe(InstanceScript* pInstance)
{
    if (!pInstance)
        return;

    for (uint8 i = 0; i < 3; ++i)
    {
        if (Creature* protector = pInstance->instance->GetCreature(pInstance->GetData64(bossEntries[i])))
        {
            protector->SetReactState(REACT_DEFENSIVE);
            protector->AI()->SetData(TYPE_SET_WIPE, 0);
            protector->GetMotionMaster()->MoveTargetedHome();
            protector->SetFullHealth();
            pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, protector);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SHA_CORRUPTION);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERPOWERED);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);
        }
    }

    if (pInstance->instance->IsHeroic())
    {
        if (Creature* minionController = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
            minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);
    }

    pInstance->SetBossState(DATA_PROTECTORS, FAIL);
}

//  60585 - Elder Regail
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
            bool isInWipeState;
            
            void Reset()
            {
                if (!pInstance || pInstance->GetBossState(DATA_PROTECTORS) == NOT_STARTED)
                    return;

                SetEquipmentSlots(false, REGAIL_ITEMS, REGAIL_ITEMS, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);
                me->SetReactState(REACT_DEFENSIVE);

                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_OVERWHELMING_CORRUPTION);
                me->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);
                
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);

                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    if (Creature* minionController = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
                        minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);

                    RespawnProtectors(pInstance, me);
                }
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();
                Reset();
            }

            void DamageTaken(Unit* attacker, uint32& /*damage*/, const SpellInfo*  /*p_SpellInfo*/)
            {
                if (pInstance)
                    if (pInstance->GetBossState(DATA_PROTECTORS) != IN_PROGRESS)
                        EnterCombat(attacker);
            }

            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                    StartProtectors(pInstance, me, attacker);
            }

            void EnterEvadeMode()
            {
                if (!pInstance || isInWipeState)
                    return;

                ProtectorsWipe(pInstance);
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
                if (who->IsPlayer())
                    Talk(TALK_REGAIL_SLAY);
            }
            
            void JustDied(Unit* killer)
            {
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
                            {
                                asani->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                me->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                me->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (asani && asani->isAlive())
                            {
                                asani->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                asani->AI()->Talk(TALK_REGAIL_DIES_SECOND_ASANI);

                                if (Aura* shaCorruption = asani->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_REGAIL_DIES_SECOND_KAOLAN);

                                if (Aura* shaCorruption = kaolan->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            pInstance->SetBossState(DATA_PROTECTORS, DONE);
                            if (killer && killer->IsPlayer())
                                me->GetMap()->ToInstanceMap()->PermBindAllPlayers(killer->ToPlayer());
                            else if (killer && killer->GetTypeId() == TYPEID_UNIT && killer->GetOwner() && killer->GetOwner()->ToPlayer())
                                me->GetMap()->ToInstanceMap()->PermBindAllPlayers(killer->GetOwner()->ToPlayer());
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);
                            _JustDied();

                            if (Creature* minionController = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
                                minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);

                            if (kaolan)
                                kaolan->AI()->DoAction(ACTION_DESPAWN_SUMMONS);
                            if (asani)
                                asani->AI()->DoAction(ACTION_DESPAWN_SUMMONS);

                            Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                            {
                                if (Player* l_Player = l_Itr->getSource())
                                    me->CastSpell(l_Player, SPELL_PROTECTORS_BONUS, true);
                            }

                            if (kaolan && IsLFR())
                            {
                                me->SetLootRecipient(NULL);
                                Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                                if (l_Player && l_Player->GetGroup())
                                    sLFGMgr->AutomaticLootAssignation(kaolan, l_Player->GetGroup());
                            }

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void CurrenciesRewarder(bool& result)
            {
                // Must be 1, because currencies are given before death
                if (ProtectorsAlive(pInstance, me) > 1)
                    result = false;
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
                    case ACTION_INTRO_FINISHED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_LOOTING);
                        me->SetFacingTo(ENTRANCE_ORIENTATION);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_COUNCILS_VORTEX)))
                            vortex->SetGoState(GO_STATE_ACTIVE);
                        if (GameObject* wall = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WALL_OF_COUNCILS_VORTEX)))
                            wall->SetGoState(GO_STATE_ACTIVE);

                        break;
                    case ACTION_DESPAWN_SUMMONS:
                        summons.DespawnAll();
                        break;
                    case ACTION_BOSS_ENTER_COMBAT:
                        Talk(TALK_REGAIL_AGGRO);
                        events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 5000);
                        events.ScheduleEvent(EVENT_LIGHTNING_PRISON, 25000);
                    default:
                        break;
                }
            }

            void SetData(uint32 type, uint32 /*value*/)
            {
                switch (type)
                {
                    case TYPE_SET_WIPE:
                    {
                        isInWipeState = true;
                        break;
                    }
                    case TYPE_UNSET_WIPE:
                    {
                        isInWipeState = false;
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe() && !isInWipeState)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!IntroDone(pInstance, me))
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_LOOTING);

                    if (GameObject* vortex = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_COUNCILS_VORTEX)))
                    {
                        me->SetFacingToObject(vortex);
                        vortex->SetGoState(GO_STATE_READY);
                    }

                    if (GameObject* wall = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WALL_OF_COUNCILS_VORTEX)))
                        wall->SetGoState(GO_STATE_READY);
                }

                if (!UpdateVictim())
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                    EnterEvadeMode();
                    return;
                }

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

// 60586 - Elder Asani
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
            bool isInWipeState;
            
            void Reset()
            {
                if (!pInstance || pInstance->GetBossState(DATA_PROTECTORS) == NOT_STARTED)
                    return;

                SetEquipmentSlots(false, ASANI_MH_ITEM, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);
                me->SetReactState(REACT_DEFENSIVE);

                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_OVERWHELMING_CORRUPTION);
                me->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);
                
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);

                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    RespawnProtectors(pInstance, me);
                }
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();
                Reset();
            }

            void DamageTaken(Unit* attacker, uint32& /*damage*/, const SpellInfo*  /*p_SpellInfo*/)
            {
                if (pInstance)
                    if (pInstance->GetBossState(DATA_PROTECTORS) != IN_PROGRESS)
                        EnterCombat(attacker);
            }

            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                    StartProtectors(pInstance, me, attacker);
            }

            void EnterEvadeMode()
            {
                if (!pInstance || isInWipeState)
                    return;

                ProtectorsWipe(pInstance);
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
                if (who->IsPlayer())
                    Talk(TALK_ASANI_SLAY);
            }
            
            void JustDied(Unit* killer)
            {
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
                                me->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_ASANI_DIES_FIRST_KAOLAN);
                                me->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
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

                                if (Aura* shaCorruption = regail->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (kaolan && kaolan->isAlive())
                            {
                                kaolan->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);
                                kaolan->AI()->Talk(TALK_ASANI_DIES_SECOND_KAOLAN);

                                if (Aura* shaCorruption = kaolan->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(kaolan, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            pInstance->SetBossState(DATA_PROTECTORS, DONE);
                            if (killer && killer->IsPlayer())
                                me->GetMap()->ToInstanceMap()->PermBindAllPlayers(killer->ToPlayer());
                            else if (killer && killer->GetTypeId() == TYPEID_UNIT && killer->GetOwner() && killer->GetOwner()->ToPlayer())
                                me->GetMap()->ToInstanceMap()->PermBindAllPlayers(killer->GetOwner()->ToPlayer());
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);
                            _JustDied();

                            if (Creature* minionController = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
                                minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);

                            if (kaolan)
                                kaolan->AI()->DoAction(ACTION_DESPAWN_SUMMONS);
                            if (regail)
                                regail->AI()->DoAction(ACTION_DESPAWN_SUMMONS);

                            if (GameObject* wall = pInstance->instance->GetGameObject(GOB_WALL_OF_COUNCILS_VORTEX))
                                wall->SetGoState(GO_STATE_READY);

                            if (GameObject* vortex = pInstance->instance->GetGameObject(GOB_COUNCILS_VORTEX))
                                vortex->SetGoState(GO_STATE_READY);

                            Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                            {
                                if (Player* l_Player = l_Itr->getSource())
                                    me->CastSpell(l_Player, SPELL_PROTECTORS_BONUS, true);
                            }

                            if (kaolan && IsLFR())
                            {
                                me->SetLootRecipient(NULL);
                                Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                                if (l_Player && l_Player->GetGroup())
                                    sLFGMgr->AutomaticLootAssignation(kaolan, l_Player->GetGroup());
                            }

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void CurrenciesRewarder(bool& result)
            {
                // Must be 1, because currencies are given before death
                if (ProtectorsAlive(pInstance, me) > 1)
                    result = false;
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
                    case ACTION_INTRO_FINISHED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_LOOTING);
                        me->SetFacingTo(ENTRANCE_ORIENTATION);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_COUNCILS_VORTEX)))
                            vortex->SetGoState(GO_STATE_ACTIVE);
                        if (GameObject* wall = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WALL_OF_COUNCILS_VORTEX)))
                            wall->SetGoState(GO_STATE_ACTIVE);

                        break;
                    case ACTION_DESPAWN_SUMMONS:
                        summons.DespawnAll();
                        break;
                    case ACTION_BOSS_ENTER_COMBAT:
                        Talk(TALK_ASANI_AGGRO);
                        events.ScheduleEvent(EVENT_WATER_BOLT, 5000);
                        events.ScheduleEvent(EVENT_CLEANSING_WATERS, 32500);
                        break;
                    default:
                        break;
                }
            }
            
            void SetData(uint32 type, uint32 /*value*/)
            {
                switch (type)
                {
                case TYPE_SET_WIPE:
                    {
                        isInWipeState = true;
                        break;
                    }
                case TYPE_UNSET_WIPE:
                    {
                        isInWipeState = false;
                        break;
                    }
                default:
                    break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe() && !isInWipeState)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!IntroDone(pInstance, me))
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_LOOTING);

                    if (GameObject* vortex = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_COUNCILS_VORTEX)))
                    {
                        me->SetFacingToObject(vortex);
                        vortex->SetGoState(GO_STATE_READY);
                    }

                    if (GameObject* wall = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WALL_OF_COUNCILS_VORTEX)))
                        wall->SetGoState(GO_STATE_READY);
                }

                if (!UpdateVictim())
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                    EnterEvadeMode();
                    return;
                }

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

// 60583 - Protector Kaolan
class boss_protector_kaolan : public CreatureScript
{
    public:
        boss_protector_kaolan() : CreatureScript("boss_protector_kaolan") { }

        struct boss_protector_kaolanAI : public BossAI
        {
            boss_protector_kaolanAI(Creature* creature) : BossAI(creature, DATA_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
                introDone = false;
            }

            InstanceScript* pInstance;
            EventMap events;

            enum LootMode
            {
                NormalModeLoot  = 1,
                HardModeLoot    = 2
            };

            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool isInWipeState;
            bool introDone;

            void Reset()
            {
                if (!pInstance || pInstance->GetBossState(DATA_PROTECTORS) == NOT_STARTED)
                    return;

                SetEquipmentSlots(false, KAOLAN_MH_ITEM, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->CastSpell(me, SPELL_SHA_MASK, true);
                me->SetReactState(REACT_DEFENSIVE);

                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();

                firstSpecialEnabled = false;
                secondSpecialEnabled = false;

                me->RemoveAura(SPELL_SHA_CORRUPTION);
                me->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);

                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    RespawnProtectors(pInstance, me);
                }
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();
                Reset();
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, const SpellInfo*  /*p_SpellInfo*/)
            {
                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_PROTECTORS) != IN_PROGRESS)
                        EnterCombat(p_Attacker);
                }

                /// Handle here elite loots, Kaolan needs to be the last killed
                if (p_Damage > me->GetHealth() && ProtectorsAlive(pInstance, me) <= 1)  ///< Kaolan is the last !
                {
                    me->RemoveLootMode(LootMode::NormalModeLoot);
                    me->AddLootMode(LootMode::HardModeLoot);
                }
            }

            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                    StartProtectors(pInstance, me, attacker);
            }

            void EnterEvadeMode()
            {
                if (!pInstance || isInWipeState)
                    return;

                ProtectorsWipe(pInstance);
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
                if (IntroDone(pInstance, me) && !introDone && who->IsPlayer())
                {
                    Talk(TALK_INTRO);
                    introDone = true;
                }
            }
            
            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    Talk(TALK_KAOLAN_SLAY);
            }
            
            void JustDied(Unit* killer)
            {
                Talk(TALK_KAOLAN_DEATH);

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_SHA);

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
                                me->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (asani && asani->isAlive())
                            {
                                asani->AI()->DoAction(ACTION_FIRST_PROTECTOR_DIED);
                                asani->AI()->Talk(TALK_KAOLAN_DIES_FIRST_ASANI);
                                me->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 1:
                        {
                            if (regail && regail->isAlive())
                            {
                                regail->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);

                                if (Aura* shaCorruption = regail->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(regail, SPELL_SHA_CORRUPTION, true);
                            }

                            if (asani && asani->isAlive())
                            {
                                asani->AI()->DoAction(ACTION_SECOND_PROTECTOR_DIED);

                                if (Aura* shaCorruption = asani->GetAura(SPELL_SHA_CORRUPTION))
                                    if (Creature* corruptionCaster = me->GetMap()->GetCreature(shaCorruption->GetCasterGUID()))
                                        corruptionCaster->CastSpell(asani, SPELL_SHA_CORRUPTION, true);
                            }

                            me->SetLootRecipient(NULL);
                            break;
                        }
                        case 0:
                        {
                            pInstance->SetBossState(DATA_PROTECTORS, DONE);
                            if (killer && killer->IsPlayer())
                                me->GetMap()->ToInstanceMap()->PermBindAllPlayers(killer->ToPlayer());
                            else if (killer && killer->GetTypeId() == TYPEID_UNIT && killer->GetOwner() && killer->GetOwner()->ToPlayer())
                                me->GetMap()->ToInstanceMap()->PermBindAllPlayers(killer->GetOwner()->ToPlayer());
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEFILED_GROUND_STACKS);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_CORRUPTION_STACK);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_PRISON_STUN);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_ESSENCE);
                            _JustDied();

                            if (Creature* minionController = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MINION_OF_FEAR_CONTROLLER)))
                                minionController->AI()->DoAction(ACTION_RESET_MINION_CONTROLLER);

                            if (asani)
                                asani->AI()->DoAction(ACTION_DESPAWN_SUMMONS);
                            if (regail)
                                regail->AI()->DoAction(ACTION_DESPAWN_SUMMONS);

                            Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                            {
                                if (Player* l_Player = l_Itr->getSource())
                                    me->CastSpell(l_Player, SPELL_PROTECTORS_BONUS, true);
                            }

                            if (IsLFR())
                            {
                                me->SetLootRecipient(NULL);
                                Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                                if (l_Player && l_Player->GetGroup())
                                    sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                            }

                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void CurrenciesRewarder(bool& result)
            {
                // Must be 1, because currencies are given before death
                if (ProtectorsAlive(pInstance, me) > 1)
                    result = false;
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
                    case ACTION_INTRO_FINISHED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_LOOTING);
                        me->SetFacingTo(ENTRANCE_ORIENTATION);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_COUNCILS_VORTEX)))
                            vortex->SetGoState(GO_STATE_ACTIVE);
                        if (GameObject* wall = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WALL_OF_COUNCILS_VORTEX)))
                            wall->SetGoState(GO_STATE_ACTIVE);

                        break;
                    case ACTION_DESPAWN_SUMMONS:
                        summons.DespawnAll();
                        break;
                    case ACTION_BOSS_ENTER_COMBAT:
                        events.ScheduleEvent(EVENT_TOUCH_OF_SHA, 12000);
                        break;
                    default:
                        break;
                }
            }
            
            void SetData(uint32 type, uint32 /*value*/)
            {
                switch (type)
                {
                    case TYPE_SET_WIPE:
                    {
                        isInWipeState = true;
                        break;
                    }
                    case TYPE_UNSET_WIPE:
                    {
                        isInWipeState = false;
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe() && !isInWipeState)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!IntroDone(pInstance, me))
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_LOOTING);

                    if (GameObject* vortex = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_COUNCILS_VORTEX)))
                    {
                        me->SetFacingToObject(vortex);
                        vortex->SetGoState(GO_STATE_READY);
                    }

                    if (GameObject* wall = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WALL_OF_COUNCILS_VORTEX)))
                        wall->SetGoState(GO_STATE_READY);
                }

                if (!UpdateVictim())
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                    EnterEvadeMode();
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_TOUCH_OF_SHA:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true, -SPELL_TOUCH_OF_SHA))
                            me->CastSpell(target, SPELL_TOUCH_OF_SHA, false);
                        events.ScheduleEvent(EVENT_TOUCH_OF_SHA, 12000);
                        break;
                    }
                    case EVENT_DEFILED_GROUND:
                    {
                        if (!firstSpecialEnabled)
                            break;

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            me->CastSpell(target, SPELL_DEFILED_GROUND_SUMMON, true);
                        events.ScheduleEvent(EVENT_DEFILED_GROUND, 15500);
                        break;
                    }
                    case EVENT_EXPEL_CORRUPTION:
                    {
                        if (!secondSpecialEnabled)
                            break;

                        Talk(TALK_EXPEL_CORRUPTION);
                        me->CastSpell(me, SPELL_EXPEL_CORRUPTION_SUMMON, false);
                        events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, 38500);
                        break;
                    }
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

            void UpdateAI(const uint32 /*p_Diff*/) { }
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

// Cleansing Water - 60646
class mob_cleansing_water : public CreatureScript
{
    public:
        mob_cleansing_water() : CreatureScript("mob_cleansing_water") { }

        struct mob_cleansing_waterAI : public ScriptedAI
        {
            mob_cleansing_waterAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_REFRESH_CLEANSING_WATERS, 1000);
                events.ScheduleEvent(EVENT_DESPAWN_CLEANSING_WATERS, 8000);

                me->CastSpell(me, SPELL_CLEANSING_WATERS_VISUAL, true);
                me->CastSpell(me, SPELL_CLEANSING_WATERS_REGEN, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_REFRESH_CLEANSING_WATERS:
                        me->CastSpell(me, SPELL_CLEANSING_WATERS_REGEN, true);
                        events.ScheduleEvent(EVENT_REFRESH_CLEANSING_WATERS, 1000);
                        break;
                    case EVENT_DESPAWN_CLEANSING_WATERS:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
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

            void JustDied(Unit* /*p_Killer*/)
            {
                me->CastSpell(me, SPELL_PURIFIED, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_corrupting_watersAI(creature);
        }
};

#define MOVE_POINT_PROTECTOR 9999

// Minion of Fear - 60885
class mob_minion_of_fear : public CreatureScript
{
    public:
        mob_minion_of_fear() : CreatureScript("mob_minion_of_fear") { }

        struct mob_minion_of_fearAI : public ScriptedAI
        {
            mob_minion_of_fearAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                protectorTargetedGuid = 0;
                creature->SetReactState(REACT_PASSIVE);
            }

            InstanceScript* pInstance;
            uint64 protectorTargetedGuid;

            void Reset()
            {
                if (pInstance && !protectorTargetedGuid)
                {
                    std::list<Creature*> targets;

                    for (uint8 i = 0; i < 3; ++i)
                        if (Creature* protector = pInstance->instance->GetCreature(pInstance->GetData64(bossEntries[i])))
                            if (protector->isAlive())
                                targets.push_back(protector);

                    if (targets.empty())
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }

                    targets.sort(JadeCore::HealthPctOrderPred());

                    Creature* target = targets.front();
                    if (!target)
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }

                    protectorTargetedGuid = target->GetGUID();
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                me->CastSpell(me, SPELL_CORRUPTED_ESSENCE, true);
                me->DespawnOrUnsummon(3000);
                me->GetMotionMaster()->Clear();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (pInstance)
                    if (pInstance->IsWipe())
                        me->DespawnOrUnsummon();

                if (!protectorTargetedGuid)
                    return;

                if (Creature* protector = me->GetMap()->GetCreature(protectorTargetedGuid))
                {
                    if (protector->IsWithinDist(me, 2.0f, false))
                    {
                        if (Aura* superiorCorruptedEssence = protector->GetAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE))
                        {
                            superiorCorruptedEssence->ModStackAmount(1);
                            superiorCorruptedEssence->RefreshDuration();
                        }
                        else
                            me->AddAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE, protector);

                        me->DespawnOrUnsummon(50);
                        protectorTargetedGuid = 0;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_minion_of_fearAI(creature);
        }
};

// Minion of Fear Controller - 60957
class mob_minion_of_fear_controller : public CreatureScript
{
    public:
        mob_minion_of_fear_controller() : CreatureScript("mob_minion_of_fear_controller") { }

        struct mob_minion_of_fear_controllerAI : public ScriptedAI
        {
            mob_minion_of_fear_controllerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            bool started;

            void Reset()
            {
                events.Reset();
                started = false;
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_INIT_MINION_CONTROLLER:
                    {
                        if (started)
                            break;

                        started = true;
                        events.ScheduleEvent(EVENT_SPAWN_MINION_OF_FEAR, 12000);
                        break;
                    }
                    case ACTION_RESET_MINION_CONTROLLER:
                    {
                        if (!started)
                            break;

                        started = false;
                        events.Reset();
                        summons.DespawnAll();
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        DoAction(ACTION_RESET_MINION_CONTROLLER);
                        return;
                    }
                }

                if (!started)
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SPAWN_MINION_OF_FEAR:
                    {
                        Position pos;
                        me->GetPosition(&pos);

                        me->SummonCreature(NPC_MINION_OF_FEAR, pos);
                        events.ScheduleEvent(EVENT_SPAWN_MINION_OF_FEAR, 15000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_minion_of_fear_controllerAI(creature);
        }
};

// Defiled Ground (damage) - 117988
class spell_defiled_ground_damage : public SpellScriptLoader
{
    public:
        spell_defiled_ground_damage() : SpellScriptLoader("spell_defiled_ground_damage") { }

        class spell_defiled_ground_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_defiled_ground_damage_SpellScript);

            void DealDamage()
            {
                if (Unit* target = GetHitUnit())
                {
                    if (AuraEffect* defiledGround = target->GetAuraEffect(SPELL_DEFILED_GROUND_STACKS, EFFECT_0))
                    {
                        uint32 damage = GetHitDamage();
                        AddPct(damage, defiledGround->GetAmount());
                        SetHitDamage(damage);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_defiled_ground_damage_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_defiled_ground_damage_SpellScript();
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

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetSpellInfo()->Id != SPELL_LIGHTNING_STORM)
                    return;

                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_LIGHTNING_STORM_FIRST, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

// Lightning Prison - 111850
class spell_lightning_prison : public SpellScriptLoader
{
    public:
        spell_lightning_prison() : SpellScriptLoader("spell_lightning_prison") { }

        class spell_lightning_prison_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_prison_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                JadeCore::Containers::RandomResizeList(targets, GetCaster()->GetMap()->Is25ManRaid() ? 3 : 2);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_lightning_prison_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lightning_prison_SpellScript();
        }
};

// Corrupted Essence - 118191
class spell_corrupted_essence : public SpellScriptLoader
{
    public:
        spell_corrupted_essence() : SpellScriptLoader("spell_corrupted_essence") { }

        class spell_corrupted_essence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_corrupted_essence_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura* corruptedEssence = target->GetAura(SPELL_CORRUPTED_ESSENCE))
                    {
                        if (corruptedEssence->GetStackAmount() >= 10)
                        {
                            target->RemoveAura(SPELL_CORRUPTED_ESSENCE);
                            target->CastSpell(target, SPELL_ESSENCE_OF_FEAR, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_corrupted_essence_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_corrupted_essence_AuraScript();
        }

        class spell_corrupted_essence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_corrupted_essence_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                Unit* caster = GetCaster();
                if (targets.empty() || !caster)
                    return;

                targets.clear();

                // Retreiving all players in 100.0f around the caster
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, caster, 100.0f);

                if (playerList.empty())
                    return;

                // Sorting players by distance from caster
                playerList.sort(JadeCore::DistanceCompareOrderPred(caster));

                // Reducing the list (only if too big)
                uint32 maxSize = caster->GetMap()->Is25ManRaid() ? 5 : 2;
                if (playerList.size() > maxSize)
                    playerList.resize(maxSize);

                // Adding players to the targets list
                for (Player* plr : playerList)
                    targets.push_back(plr);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_corrupted_essence_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_corrupted_essence_SpellScript();
        }
};

// Superior Corrupted Essence - 117905
class spell_superior_corrupted_essence : public SpellScriptLoader
{
    public:
        spell_superior_corrupted_essence() : SpellScriptLoader("spell_superior_corrupted_essence") { }

        class spell_superior_corrupted_essence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_superior_corrupted_essence_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura* corruptedEssence = target->GetAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE))
                    {
                        if (corruptedEssence->GetStackAmount() >= 5)
                        {
                            target->RemoveAura(SPELL_SUPERIOR_CORRUPTED_ESSENCE);
                            target->CastSpell(target, SPELL_SUPERIOR_ESSENCE_OF_FEAR, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_superior_corrupted_essence_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_superior_corrupted_essence_AuraScript();
        }
};

// Cleansing Waters - 117283
class spell_cleansing_waters_regen : public SpellScriptLoader
{
    public:
        spell_cleansing_waters_regen() : SpellScriptLoader("spell_cleansing_waters_regen") { }

        class spell_cleansing_waters_regen_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_cleansing_waters_regen_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_CLEANSING_WATERS_REGEN));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_cleansing_waters_regen_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_cleansing_waters_regen_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_cleansing_waters_regen_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_protectors_of_the_endless()
{
    new boss_ancient_regail();              // 60585
    new boss_ancient_asani();               // 60586
    new boss_protector_kaolan();            // 60583
    new mob_defiled_ground();               // 60906
    new mob_coalesced_corruption();         // 60886
    new mob_cleansing_water();              // 60646
    new mob_corrupting_waters();            // 60621
    new mob_minion_of_fear();               // 60885
    new mob_minion_of_fear_controller();    // 60957
    new spell_defiled_ground_damage();      // 117988
    new spell_expelled_corruption();        // 117955
    new spell_lightning_storm_aura();       // 118064, 118040, 118053, 118054, 118055, 118077
    new spell_lightning_storm_damage();     // 118004, 118005, 118007, 118008
    new spell_lightning_prison();           // 111850
    new spell_corrupted_essence();          // 118191
    new spell_superior_corrupted_essence(); // 117905
    new spell_cleansing_waters_regen();     // 117283
}
#endif