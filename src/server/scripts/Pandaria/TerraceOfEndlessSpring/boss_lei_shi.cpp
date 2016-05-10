////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
#include "GridNotifiers.h"
#include "AreaTrigger.h"

enum eLeiShiSpells
{
    SPELL_AFRAID            = 123181,
    SPELL_SPRAY             = 123121,
    SPELL_HIDE              = 123244,
    SPELL_HIDE_SUMMON       = 123213,
    SPELL_HIDE_STACKS       = 123233,
    SPELL_GET_AWAY          = 123461,
    SPELL_GET_AWAY_TELEPORT = 123441,
    SPELL_PROTECT           = 123250,
    SPELL_PROTECT_RESPAWN   = 123493,
    SPELL_PROTECT_VISUAL    = 123505,
    SPELL_BERSERK           = 26662,

    // This is for Heroic Mode
    SPELL_SCARY_FOG_CIRCLE  = 123797,
    SPELL_SCARY_FOG_DOT     = 123705,
    SPELL_SCARY_FOG_STACKS  = 123712,

    SPELL_LEI_SHI_TRANSFORM = 127535,

    SPELL_LEI_SHI_BONUS     = 132202
};

enum eLeiShiEvents
{
    // Lei Shi
    EVENT_SPECIAL       = 1,
    EVENT_SPRAY         = 2,
    EVENT_GET_AWAY      = 3,
    EVENT_GET_AWAY_CAST = 4,
    EVENT_PROTECT       = 5,
    EVENT_HIDE          = 6,

    // Lei Shi (hidden)
    EVENT_HIDDEN_SPRAY  = 7,
    EVENT_BERSERK       = 8
};

enum eLeiShiActions
{
    ACTION_ANIMATED_PROTECTOR_DIED,
    ACTION_ACTIVATE_ANIMATED_PROTECTORS,
    ACTION_TERMINATE_GET_AWAY_PHASE,
    ACTION_TERMINATE_HIDE_PHASE
};

enum eLeiShiSays
{
    TALK_AGGRO,
    TALK_HIDE,
    TALK_GET_AWAY,
    TALK_SLAY,
    TALK_DEFEATED
};

Position leiShiPos = { -1017.93f, -2911.3f, 19.902f, 4.74f };

Position hidePositions[4] =
{
    { -990.678f,  -2890.043f, 19.172f, 3.781f },
    { -994.835f,  -2933.835f, 19.172f, 2.377f },
    { -1041.316f, -2932.84f,  19.172f, 0.733f },
    { -1045.101f, -2890.742f, 19.172f, 5.646f }
};

#define HEROIC_DIST_TO_VORTEX 21.0f
#define DIST_TO_SCARY_FOG_DOT 4.5f

// 62983 - Lei Shi
class boss_lei_shi : public CreatureScript
{
    public:
        boss_lei_shi() : CreatureScript("boss_lei_shi") { }

        enum eMiscs
        {
            EvilDisplay = 43783,
            GoodDisplay = 40357
        };

        struct boss_lei_shiAI : public BossAI
        {
            boss_lei_shiAI(Creature* creature) : BossAI(creature, DATA_LEI_SHI)
            {
                pInstance = creature->GetInstanceScript();
                leiShiFreed = pInstance ? pInstance->GetBossState(DATA_LEI_SHI) == DONE : false;
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 nextAfraidPct;
            uint8 nextProtectPct;

            uint8 endCombatPct;

            std::list<uint64> animatedProtectors;

            bool hidden;
            bool shielded;
            bool getAwayPhase;
            bool protectScheduled;
            bool leiShiFreed;

            float getAwayHealthPct;

            void Reset()
            {
                if (pInstance && pInstance->GetBossState(DATA_TSULONG) != DONE)
                {
                    me->SetDisplayId(11686);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetReactState(REACT_PASSIVE);
                    return;
                }

                if (pInstance)
                    if (pInstance->GetBossState(DATA_LEI_SHI) == DONE)
                        FreeLeiShi();

                if (leiShiFreed)
                    return;

                _Reset();
 
                summons.DespawnAll();

                me->ReenableEvadeMode();
 
                hidden              = false;
                getAwayPhase        = false;
                shielded            = false;
                protectScheduled    = false;
 
                nextAfraidPct       = 90;
                nextProtectPct      = 80;
                endCombatPct        = 2;

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
                me->SetDisplayId(eMiscs::EvilDisplay);
                me->RemoveAura(SPELL_AFRAID);
                me->RemoveAura(SPELL_HIDE);
                me->RemoveAura(SPELL_HIDE_STACKS);
                me->RemoveAura(SPELL_SCARY_FOG_CIRCLE);
                me->RemoveAura(SPELL_SCARY_FOG_DOT);
                me->RemoveAura(SPELL_SCARY_FOG_STACKS);
                me->RemoveAllAreasTrigger();

                events.Reset();

                events.ScheduleEvent(EVENT_SPRAY, 400);

                if (urand(0, 1))
                    events.ScheduleEvent(EVENT_GET_AWAY,    32000);
                else
                    events.ScheduleEvent(EVENT_HIDE,        32000);

                events.ScheduleEvent(EVENT_BERSERK, 600000);
 
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_CIRCLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_DOT);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_STACKS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPRAY);
 
                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    std::list<Creature*> protectors;
                    me->GetCreatureListWithEntryInGrid(protectors, NPC_ANIMATED_PROTECTOR, 100.0f);

                    for (auto itr : protectors)
                    {
                        animatedProtectors.push_back(itr->GetGUID());
                        itr->Respawn();
                        itr->AddAura(SPELL_PROTECT_RESPAWN, itr);
                        itr->RemoveAura(SPELL_PROTECT_VISUAL);
                        itr->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
                    }
                }
            }

            void JustReachedHome()
            {
                if (leiShiFreed)
                    return;

                _JustReachedHome();
 
                if (pInstance && pInstance->GetBossState(DATA_LEI_SHI) != DONE)
                    pInstance->SetBossState(DATA_LEI_SHI, FAIL);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (leiShiFreed)
                    return;

                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_TSULONG) != DONE)
                    {
                        EnterEvadeMode();
                        return;
                    }

                    me->CastSpell(me, SPELL_AFRAID, true);
                    pInstance->SetBossState(DATA_LEI_SHI, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();

                    if (me->GetMap()->IsHeroic())
                        me->CastSpell(me, SPELL_SCARY_FOG_CIRCLE, false);

                    Talk(TALK_AGGRO);
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
                if (who->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, const SpellInfo*  /*p_SpellInfo*/)
            {
                if (!pInstance)
                    return;

                if (leiShiFreed)
                {
                    damage = 0;
                    return;
                }

                if (nextAfraidPct)
                {
                    if (me->HealthBelowPctDamaged(nextAfraidPct, damage))
                    {
                        if (AuraEffect* afraid = me->GetAuraEffect(SPELL_AFRAID, EFFECT_0))
                            afraid->ChangeAmount(afraid->GetAmount() + 8);
 
                        nextAfraidPct -= 10;
                    }
 
                    if (me->HealthBelowPctDamaged(int32(getAwayHealthPct - 4.0f), damage))
                        me->RemoveAura(SPELL_GET_AWAY);
 
                    if (nextProtectPct >= 20)
                    {
                        if (me->HealthBelowPctDamaged(nextProtectPct, damage) && !protectScheduled)
                        {
                            protectScheduled = true;
                            events.ScheduleEvent(EVENT_PROTECT, 100);
                        }
                    }
                }

                if (me->HealthBelowPctDamaged(endCombatPct, damage))
                {
                    damage = 0;
                    FreeLeiShi();

                    Talk(TALK_DEFEATED);

                    if (pInstance)
                    {
                        pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_CIRCLE);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_DOT);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCARY_FOG_STACKS);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SPRAY);
                        pInstance->SetBossState(DATA_LEI_SHI, DONE);

                        if (GameObject* wall = pInstance->instance->GetGameObject(GOB_WALL_OF_LEI_SHI))
                            wall->SetGoState(GO_STATE_READY);

                        if (GameObject* vortex = pInstance->instance->GetGameObject(GOB_LEI_SHIS_VORTEX))
                            vortex->SetGoState(GO_STATE_READY);
                    }

                    switch (me->GetMap()->GetSpawnMode())
                    {
                        case Difficulty::Difficulty10N:
                            me->SummonGameObject(GOB_LEI_SHI_CHEST_NORMAL_10, leiShiPos.GetPositionX(), leiShiPos.GetPositionY(), leiShiPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        case Difficulty::Difficulty25N:
                            me->SummonGameObject(GOB_LEI_SHI_CHEST_NORMAL_25, leiShiPos.GetPositionX(), leiShiPos.GetPositionY(), leiShiPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        case Difficulty::Difficulty10HC:
                            me->SummonGameObject(GOB_LEI_SHI_CHEST_HEROIC_10, leiShiPos.GetPositionX(), leiShiPos.GetPositionY(), leiShiPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        case Difficulty::Difficulty25HC:
                            me->SummonGameObject(GOB_LEI_SHI_CHEST_HEROIC_25, leiShiPos.GetPositionX(), leiShiPos.GetPositionY(), leiShiPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        default:
                            break;
                    }

                    Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = l_Itr->getSource())
                        {
                            // Combat stop, avoid fight bug
                            l_Player->CombatStop();

                            // Lei shi bonus
                            me->CastSpell(l_Player, SPELL_LEI_SHI_BONUS, true);

                            // Valor points
                            if (!IsLFR())
                                l_Player->ModifyCurrency(CURRENCY_TYPE_VALOR_POINTS, 40);
                        }
                    }

                    if (IsLFR())
                    {
                        me->SetLootRecipient(NULL);
                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }

                    for (auto itr : animatedProtectors)
                    {
                        if (Creature* protector = Creature::GetCreature(*me, itr))
                            protector->DespawnOrUnsummon();
                    }
                }
            }

            void FreeLeiShi()
            {
                me->setFaction(35);
                me->RemoveAllAreasTrigger();
                me->RemoveAllAuras();
                me->SetDisplayId(eMiscs::GoodDisplay);
                me->SetFullHealth();
                me->CastSpell(me, SPELL_LEI_SHI_TRANSFORM, true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                leiShiFreed = true;
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (!hidden)
                    return;

                if (!caster)
                    return;

                if (caster->GetTypeId() == TYPEID_UNIT && (caster->GetEntry() == NPC_LEI_SHI || caster->GetEntry() == NPC_LEI_SHI_HIDDEN))
                    return;

                if (!spell)
                    return;

                if (!spell->HasEffect(SPELL_EFFECT_SCHOOL_DAMAGE) &&
                    !spell->HasEffect(SPELL_EFFECT_NORMALIZED_WEAPON_DMG) &&
                    !spell->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE) &&
                    !spell->HasEffect(SPELL_EFFECT_WEAPON_PERCENT_DAMAGE) &&
                    !spell->HasEffect(SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL))
                    return;

                if (Aura* hide = me->GetAura(SPELL_HIDE_STACKS))
                    hide->ModCharges(-1);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ANIMATED_PROTECTOR_DIED:
                    {
                        if (!shielded)
                            break;

                        me->RemoveAura(SPELL_PROTECT);
                        shielded = false;
                        protectScheduled = false;
                        nextProtectPct -= 20;

                        Creature* protector = NULL;

                        for (auto itr : animatedProtectors)
                        {
                            if (Creature* elemental = me->GetMap()->GetCreature(itr))
                            {
                                if (!elemental->isAlive())
                                    protector = elemental;

                                elemental->RemoveAura(SPELL_PROTECT_VISUAL);
                                elemental->AddAura(SPELL_PROTECT_RESPAWN, elemental);
                                elemental->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
                            }
                        }

                        if (protector)
                            animatedProtectors.remove(protector->GetGUID());

                        break;
                    }
                    case ACTION_ACTIVATE_ANIMATED_PROTECTORS:
                    {
                        if (animatedProtectors.empty())
                            break;

                        me->CastSpell(me, SPELL_PROTECT, false);
                        shielded = true;

                        uint8 protectorsActivated = 0;
                        for (auto itr : animatedProtectors)
                        {
                            // 3 Animated Protectors at 80% and 60%
                            if (((me->GetMap()->IsHeroic() && protectorsActivated >= 4) ||
                                (!me->GetMap()->IsHeroic() && protectorsActivated >= 3)) &&
                                nextProtectPct > 40)
                                break;

                            // 4 Animated Protectors at 40% and 20%
                            if (((me->GetMap()->IsHeroic() && protectorsActivated >= 5) ||
                                (!me->GetMap()->IsHeroic() && protectorsActivated >= 4)) &&
                                nextProtectPct <= 40)
                                break;

                            if (Creature* protector = me->GetMap()->GetCreature(itr))
                            {
                                protector->RemoveAura(SPELL_PROTECT_RESPAWN);
                                protector->CastSpell(protector, SPELL_PROTECT_VISUAL, true);
                                protector->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);

                                if (Unit* target = protector->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f))
                                    protector->AI()->AttackStart(target);

                                ++protectorsActivated;
                            }
                        }

                        break;
                    }
                    case ACTION_TERMINATE_GET_AWAY_PHASE:
                    {
                        getAwayPhase = false;
                        me->RemoveAllAreasTrigger();

                        if (urand(0, 1))
                            events.ScheduleEvent(EVENT_HIDE, 32000);
                        else
                            events.ScheduleEvent(EVENT_GET_AWAY, 32000);
                        break;
                    }
                    case ACTION_TERMINATE_HIDE_PHASE:
                    {
                        me->RemoveAura(SPELL_HIDE);
                        me->RemoveAura(SPELL_HIDE_STACKS);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->RestoreDisplayId();
                        hidden = false;

                        if (me->GetMap()->IsHeroic())
                            me->CastSpell(me, SPELL_SCARY_FOG_CIRCLE, false);

                        // Only have Lei Shi (hidden) in summons
                        summons.DespawnAll();

                        if (urand(0, 1))
                            events.ScheduleEvent(EVENT_HIDE, 32000);
                        else
                            events.ScheduleEvent(EVENT_GET_AWAY, 32000);
                        break;
                    }
                    case ACTION_SHOW_LEI_SHI:
                    {
                        me->RestoreDisplayId();
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetReactState(REACT_AGGRESSIVE);

                        if (GameObject* vortex = me->GetMap()->GetGameObject(pInstance->GetData64(GOB_LEI_SHIS_VORTEX)))
                            vortex->SetGoState(GO_STATE_ACTIVE);

                        if (GameObject* wall = me->GetMap()->GetGameObject(pInstance->GetData64(GOB_WALL_OF_LEI_SHI)))
                            wall->SetGoState(GO_STATE_ACTIVE);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                    std::list<Creature*> protectors;
                    me->GetCreatureListWithEntryInGrid(protectors, NPC_ANIMATED_PROTECTOR, 100.0f);

                    protectors.remove_if(JadeCore::UnitAuraCheck(true, SPELL_PROTECT_RESPAWN));

                    for (auto itr : protectors)
                    {
                        animatedProtectors.push_back(itr->GetGUID());
                        itr->Respawn();
                        itr->AddAura(SPELL_PROTECT_RESPAWN, itr);
                        itr->RemoveAura(SPELL_PROTECT_VISUAL);
                        itr->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_IMMUNE_TO_PC);
                    }
 
                    return;
                }
 
                if (hidden)
                    return;
 
                events.Update(diff);
 
                switch (events.ExecuteEvent())
                {
                    case EVENT_SPRAY:
                    {
                        if (getAwayPhase || me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_SPRAY, 400);
                            break;
                        }

                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SPRAY, false);
                        events.ScheduleEvent(EVENT_SPRAY, 400);
                        break;
                    }
                    case EVENT_GET_AWAY:
                    {
                        if ((me->HasUnitState(UNIT_STATE_CASTING) && !getAwayPhase) || shielded)
                        {
                            events.ScheduleEvent(EVENT_GET_AWAY, 0);
                            break;
                        }

                        // Teleport Lei Shi to the center of the room
                        Talk(TALK_GET_AWAY);
                        me->CastSpell(me, SPELL_GET_AWAY_TELEPORT, true);
                        getAwayPhase = true;
                        getAwayHealthPct = me->GetHealthPct();
                        events.ScheduleEvent(EVENT_GET_AWAY_CAST, 300);
                        break;
                    }
                    case EVENT_GET_AWAY_CAST:
                    {
                        // Wait the real teleportation before cast
                        if (getAwayPhase && (me->GetPositionX() != leiShiPos.GetPositionX() ||
                                             me->GetPositionY() != leiShiPos.GetPositionY() ||
                                             me->GetPositionZ() != leiShiPos.GetPositionZ()))
                        {
                            me->CastSpell(me, SPELL_GET_AWAY_TELEPORT, true);
                            events.ScheduleEvent(EVENT_GET_AWAY_CAST, 100);
                            break;
                        }

                        me->CastSpell(me, SPELL_GET_AWAY, false);
                        break;
                    }
                    case EVENT_PROTECT:
                    {
                        if (getAwayPhase || me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_PROTECT, 0);
                            break;
                        }

                        DoAction(ACTION_ACTIVATE_ANIMATED_PROTECTORS);
                        break;
                    }
                    case EVENT_HIDE:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_HIDE, 0);
                            break;
                        }
                        
                        Talk(TALK_HIDE);
                        me->CastSpell(me, SPELL_HIDE, false);
                        hidden = true;
                        me->RemoveAura(SPELL_SCARY_FOG_CIRCLE);
                        me->RemoveAurasByType(SPELL_AURA_MOD_STALKED);
                        me->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                        break;
                    }
                    case EVENT_BERSERK:
                    {
                        me->CastSpell(me, SPELL_BERSERK, true);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lei_shiAI(creature);
        }
};

// Animated Protector - 62995
class mob_animated_protector : public CreatureScript
{
    public:
        mob_animated_protector() : CreatureScript("mob_animated_protector") { }

        struct mob_animated_protectorAI : public ScriptedAI
        {
            mob_animated_protectorAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            void JustDied(Unit* /*p_Killer*/)
            {
                if (Creature* leiShi = me->GetMap()->GetCreature(pInstance->GetData64(NPC_LEI_SHI)))
                    leiShi->AI()->DoAction(ACTION_ANIMATED_PROTECTOR_DIED);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_animated_protectorAI(creature);
        }
};

// Lei Shi (hidden) - 63099
class mob_lei_shi_hidden : public CreatureScript
{
    public:
        mob_lei_shi_hidden() : CreatureScript("mob_lei_shi_hidden") { }

        struct mob_lei_shi_hiddenAI : public ScriptedAI
        {
            mob_lei_shi_hiddenAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_HIDDEN_SPRAY, 400);

                if (pInstance)
                {
                    if (Creature* leiShi = me->GetMap()->GetCreature(pInstance->GetData64(NPC_LEI_SHI)))
                    {
                        AuraEffect* afraid = leiShi->GetAuraEffect(SPELL_AFRAID, EFFECT_0);
                        if (!afraid)
                            return;

                        if (!me->HasAura(SPELL_AFRAID))
                        {
                            if (Aura* newAfraid = me->AddAura(SPELL_AFRAID, me))
                                if (newAfraid->GetEffect(0))
                                    newAfraid->GetEffect(0)->ChangeAmount(afraid->GetAmount());
                        }
                        else
                        {
                            if (AuraEffect* newAfraid = me->GetAuraEffect(SPELL_AFRAID, EFFECT_0))
                                newAfraid->ChangeAmount(afraid->GetAmount());
                        }
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_HIDDEN_SPRAY:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_HIDDEN_SPRAY, 0);
                            break;
                        }

                        uint64 leiShiGuid = 0;
                        if (pInstance)
                            leiShiGuid = pInstance->GetData64(NPC_LEI_SHI);

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f))
                            me->CastSpell(target, SPELL_SPRAY, false, NULL, nullptr, leiShiGuid);
                        events.ScheduleEvent(EVENT_HIDDEN_SPRAY, 400);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lei_shi_hiddenAI(creature);
        }
};

// Get Away! - 123461
class spell_get_away : public SpellScriptLoader
{
    public:
        spell_get_away() : SpellScriptLoader("spell_get_away") { }

        class spell_get_away_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_get_away_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->ToCreature())
                    {
                        if (caster->ToCreature()->AI())
                            caster->ToCreature()->AI()->DoAction(ACTION_TERMINATE_GET_AWAY_PHASE);
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_get_away_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_get_away_AuraScript();
        }
};

// Get Away! - 123467
class spell_get_away_damage : public SpellScriptLoader
{
    public:
        spell_get_away_damage() : SpellScriptLoader("spell_get_away_damage") { }

        class spell_get_away_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_get_away_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->IsMoving() && target->isInFront(caster))
                            SetHitDamage(GetHitDamage() / 2);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_get_away_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_get_away_damage_SpellScript();
        }
};

// Hide - 123244
class spell_hide : public SpellScriptLoader
{
    public:
        spell_hide() : SpellScriptLoader("spell_hide") { }

        class spell_hide_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hide_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_HIDE_STACKS, true);
                    caster->CastSpell(caster, SPELL_HIDE_SUMMON, true);

                    uint8 pos = urand(0, 3);

                    caster->NearTeleportTo(hidePositions[pos].GetPositionX(),
                                           hidePositions[pos].GetPositionY(),
                                           hidePositions[pos].GetPositionZ(),
                                           hidePositions[pos].GetOrientation());

                    caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    caster->SetDisplayId(11686);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->RemoveAura(SPELL_HIDE_STACKS);
                    caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    caster->RestoreDisplayId();
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_hide_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_hide_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hide_AuraScript();
        }
};

// Hide (stacks) - 123233
class spell_hide_stacks : public SpellScriptLoader
{
    public:
        spell_hide_stacks() : SpellScriptLoader("spell_hide_stacks") { }

        class spell_hide_stacks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hide_stacks_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->ToCreature())
                    {
                        if (caster->ToCreature()->AI())
                            caster->ToCreature()->AI()->DoAction(ACTION_TERMINATE_HIDE_PHASE);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_hide_stacks_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hide_stacks_AuraScript();
        }
};

// Scary Fog (DoT) - 123705
class spell_scary_fog_dot : public SpellScriptLoader
{
    public:
        spell_scary_fog_dot() : SpellScriptLoader("spell_scary_fog_dot") { }

        class spell_scary_fog_dot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_scary_fog_dot_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                targets.clear();

                Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
                if (!players.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if (Player* player = itr->getSource())
                        {
                            if (player->GetExactDist2d(GetCaster()->GetPositionX(), GetCaster()->GetPositionY()) >= HEROIC_DIST_TO_VORTEX &&
                                !player->HasAura(SPELL_SCARY_FOG_DOT))
                                targets.push_back(player);
                        }
                    }
                }

                for (auto itr : targets)
                {
                    if (itr->ToUnit())
                    {
                        if (Aura* scaryFog = GetCaster()->AddAura(SPELL_SCARY_FOG_STACKS, itr->ToUnit()))
                        {
                            scaryFog->SetDuration(35000);
                            scaryFog->SetMaxDuration(35000);
                        }
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scary_fog_dot_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_scary_fog_dot_SpellScript();
        }

        class spell_scary_fog_dot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_scary_fog_dot_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_SCARY_FOG_STACKS, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_scary_fog_dot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_scary_fog_dot_AuraScript();
        }
};

// Scary Fog (stacks) - 123712
class spell_scary_fog_stacks : public SpellScriptLoader
{
    public:
        spell_scary_fog_stacks() : SpellScriptLoader("spell_scary_fog_stacks") { }

        class spell_scary_fog_stacks_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_scary_fog_stacks_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                for (auto itr : targets)
                {
                    if (!itr->ToUnit() || itr->ToUnit()->GetEntry() == NPC_LEI_SHI_HIDDEN || itr->GetDistance2d(GetCaster()) > 10.0f)
                        continue;

                    if (Aura* scary = GetCaster()->GetAura(SPELL_SCARY_FOG_STACKS))
                    {
                        if (Aura* scaryTarget = itr->ToUnit()->GetAura(SPELL_SCARY_FOG_STACKS))
                            scaryTarget->SetStackAmount(scary->GetStackAmount());
                        else if (Aura* scaryTarget = GetCaster()->AddAura(SPELL_SCARY_FOG_STACKS, itr->ToUnit()))
                            scaryTarget->SetStackAmount(scary->GetStackAmount());
                    }
                }

                targets.clear();
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_scary_fog_stacks_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_scary_fog_stacks_SpellScript();
        }
};

/// Created by spell 123461
class at_get_away : public AreaTriggerEntityScript
{
    public:
        at_get_away() : AreaTriggerEntityScript("at_get_away") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Position l_Pos;
            p_AreaTrigger->GetPosition(&l_Pos);
            uint64 l_AreatTriggerGuid = p_AreaTrigger->GetGUID();

            std::list<Player*> l_PlayerList;
            GetPlayerListInGrid(l_PlayerList, p_AreaTrigger, 60.0f);

            for (Player* l_Player : l_PlayerList)
            {
                if (l_Player->IsWithinDist(p_AreaTrigger, 40.0f, false))
                {
                    if (!l_Player->isAlive() && l_Player->HasMovementForce(l_AreatTriggerGuid))
                        l_Player->SendApplyMovementForce(l_AreatTriggerGuid, false, l_Pos);

                    if (l_Player->isAlive() && !l_Player->HasMovementForce(l_AreatTriggerGuid))
                        l_Player->SendApplyMovementForce(l_AreatTriggerGuid, true, l_Pos, -3.0f, 1);
                }
                /// Remove movement force if we're outside of the range
                else if (l_Player->HasMovementForce(l_AreatTriggerGuid))
                    l_Player->SendApplyMovementForce(l_AreatTriggerGuid, false, l_Pos);
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Position l_Pos;
            p_AreaTrigger->GetPosition(&l_Pos);

            Map::PlayerList const &l_PlayerList = p_AreaTrigger->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
            {
                if (Player* l_Player = l_Itr->getSource())
                    l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, l_Pos, 1);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_get_away();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_lei_shi()
{
    new boss_lei_shi();             ///< 62983
    new mob_animated_protector();   ///< 62995
    new mob_lei_shi_hidden();       ///< 63099
    new spell_get_away();           ///< 123461
    new spell_get_away_damage();    ///< 123467
    new spell_hide();               ///< 123244
    new spell_hide_stacks();        ///< 123233
    new spell_scary_fog_dot();      ///< 123705
    new spell_scary_fog_stacks();   ///< 123712
    new at_get_away();              ///< 123461
}
#endif