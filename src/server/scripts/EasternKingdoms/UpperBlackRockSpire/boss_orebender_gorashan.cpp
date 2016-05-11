////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# include "upper_blackrock_spire.hpp"

enum eSpells
{
    // Boss
    SPELL_POWER_CONDUIT_AURA    = 166168,
    SPELL_POWER_CONDUIT_STUN    = 166169,
    SPELL_BLADES_OF_STEEL       = 154417,
    SPELL_SHRAPNEL_NOVA         = 154448,
    SPELL_LODESTONE_SPIKE_DUMMY = 154435,
    SPELL_LODESTONE_SPIKE_DMG   = 154428,
    SPELL_THUNDER_CACOPHONY     = 154899,
    SPELL_SEAL_CONDUIT_FIRST    = 154302,
    SPELL_SEAL_CONDUIT_SECOND   = 154900,
    SPELL_SEAL_CONDUIT_THIRD    = 154901,

    // Misc
    SPELL_POWER_CONDUIT_VISUAL  = 154294,   // Casted by Rune of Power on the boss
    SPELL_UNHARNESSED_POWER     = 154265,
    SPELL_ELECTRIC_PULSE_AURA   = 154335,
    SPELL_DISRUPTING_CONDUCTOR  = 154296
};

enum eEvents
{
    EVENT_POWER_CONDUIT = 1,
    EVENT_BLADES_OF_STEEL,
    EVENT_SHRAPNEL_NOVA,
    EVENT_ELECTRIC_PULSE,
    EVENT_LODESTONE_SPIKE,
    EVENT_THUNDER_CACOPHONY,
    EVENT_MOVE_LIGHTNING,
    EVENT_SEAL_CONDUIT
};

enum eSays
{
    TALK_AGGRO,
    TALK_DEATH,
    TALK_SLAY,
    TALK_SPELL_1,
    TALK_SPELL_2,
    TALK_SPELL_3,
    TALK_SPELL_4
};

enum eActions
{
    ACTION_POWER_CONDUIT,
    ACTION_CLOSE_POWER_CONDUIT,
    ACTION_ENGAGE_APPRENTICE
};

enum ePositions
{
    DATA_POS_NE,
    DATA_POS_SE,
    DATA_POS_SW,
    DATA_POS_NW,
    DATA_MOVE_LIGHT = 76464
};

Position const g_LightningFieldInitPos[2] =
{
    { 164.569473f, -278.615875f, 94.607742f, M_PI               },  ///< DATA_POS_NE
    { 124.726387f, -239.007599f, 94.607742f, 2 * M_PI           }   ///< DATA_POS_SW
};

Position const g_LightningFieldMovePos[4] =
{
    { 164.569473f, -278.615875f, 94.607742f, M_PI               },  ///< DATA_POS_NE
    { 124.726387f, -278.615875f, 94.607742f, M_PI / 2.0f        },  ///< DATA_POS_SE
    { 124.726387f, -239.007599f, 94.607742f, 0.0f               },  ///< DATA_POS_SW
    { 164.569473f, -239.007599f, 94.607742f, M_PI + M_PI / 2.0f }   ///< DATA_POS_NW
};

///< Orebender Gor'Ashan - 76413
class boss_orebender_gorashan : public CreatureScript
{
    public:
        boss_orebender_gorashan() : CreatureScript("boss_orebender_gorashan") { }

        struct boss_orebender_gorashanAI : public BossAI
        {
            boss_orebender_gorashanAI(Creature* p_Creature) : BossAI(p_Creature, DATA_OREBENDER_GORASHAN)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_ApprenticeEngaged = false;

                if (m_Instance)
                    m_Instance->SetBossState(DATA_OREBENDER_GORASHAN, TO_BE_DECIDED);
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            uint8 m_SealConduitCount;
            bool m_Active;
            bool m_ApprenticeEngaged;
            uint32 m_InitializeTimer;

            void Reset()
            {
                _Reset();

                m_Events.Reset();

                me->ReenableEvadeMode();

                me->RemoveAllAreasTrigger();

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED | UNIT_FLAG_PREPARATION | UNIT_FLAG_PVP_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                m_SealConduitCount = 0;
                m_InitializeTimer  = 2000;
                m_Active           = false;
                me->SetReactState(REACT_PASSIVE);

                if (m_Instance)
                    m_Instance->SetBossState(DATA_OREBENDER_GORASHAN, NOT_STARTED);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (!m_Active)
                {
                    EnterEvadeMode();
                    return;
                }

                Talk(TALK_AGGRO);

                _EnterCombat();

                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_BLADES_OF_STEEL, 1000);
                m_Events.ScheduleEvent(EVENT_SHRAPNEL_NOVA, 10000);
                m_Events.ScheduleEvent(EVENT_ELECTRIC_PULSE, 1000);
                m_Events.ScheduleEvent(EVENT_LODESTONE_SPIKE, 14000);
                m_Events.ScheduleEvent(EVENT_SEAL_CONDUIT, 30000);

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_OREBENDER_GORASHAN, IN_PROGRESS);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                Talk(TALK_DEATH);

                _JustDied();

                if (m_Instance)
                {
                    // Defeat Orebender Gor'ashan without allowing him to cast Thundering Cacophony 4 times in Upper Blackrock Spire on Heroic difficulty.
                    if (IsHeroic() && m_Instance->GetData(DATA_MAGNETS_ACHIEVEMENT) < 4)
                        m_Instance->DoCompleteAchievement(eAchievements::AchievementMagnetsHowDoTheyWork);

                    m_Instance->SetBossState(DATA_OREBENDER_GORASHAN, DONE);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                me->RemoveAllAreasTrigger();
            }

            void EnterEvadeMode()
            {
                _EnterEvadeMode();

                if (m_Instance)
                {
                    _JustReachedHome();
                    m_Instance->SetBossState(DATA_OREBENDER_GORASHAN, FAIL);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                Reset();
            }

            void SpellHit(Unit* /*p_Caster*/, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == SPELL_POWER_CONDUIT_VISUAL)
                {
                    if (m_Active && !me->HasAura(SPELL_POWER_CONDUIT_AURA))
                        m_Events.ScheduleEvent(EVENT_THUNDER_CACOPHONY, 1000);

                    me->CastSpell(me, SPELL_POWER_CONDUIT_AURA, true);
                }
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_CLOSE_POWER_CONDUIT:
                    {
                        if ((m_Active = CanActivate()))
                        {
                            me->SetReactState(REACT_AGGRESSIVE);
                            m_Events.CancelEvent(EVENT_THUNDER_CACOPHONY);
                        }
                        break;
                    }
                    case ACTION_ENGAGE_APPRENTICE:
                    {
                        if (m_ApprenticeEngaged)
                            break;

                        if (Player* l_Player = me->FindNearestPlayer(30.0f))
                        {
                            m_ApprenticeEngaged = true;

                            std::list<Creature*> l_ApprenticeList;
                            me->GetCreatureListWithEntryInGrid(l_ApprenticeList, NPC_BLACK_IRON_APPRENTICE, 150.0f);

                            bool l_FirstTalk = false;
                            for (Creature* l_Apprentice : l_ApprenticeList)
                            {
                                l_Apprentice->AI()->AttackStart(l_Player);

                                if (!l_FirstTalk)
                                {
                                    l_Apprentice->AI()->Talk(TALK_AGGRO);
                                    l_FirstTalk = true;
                                }
                            }

                            Reset();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                InitializeCombat(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasUnitState(UNIT_STATE_STUNNED))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_BLADES_OF_STEEL:
                        // Boss doesn't melee attacks, only cast Blades of Steel instead
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_BLADES_OF_STEEL, false);
                        m_Events.ScheduleEvent(EVENT_BLADES_OF_STEEL, 2000);
                        break;
                    case EVENT_SHRAPNEL_NOVA:
                        me->CastSpell(me, SPELL_SHRAPNEL_NOVA, false);
                        m_Events.ScheduleEvent(EVENT_SHRAPNEL_NOVA, 15000);
                        break;
                    case EVENT_ELECTRIC_PULSE:
                    {
                        for (uint8 l_I = 0; l_I < (IsHeroic() ? 2 : 1); ++l_I)
                        {
                            if (Creature* l_Lightning = me->SummonCreature(NPC_LIGHTNING_FIELD, g_LightningFieldInitPos[l_I]))
                                l_Lightning->AI()->DoAction(l_I);
                        }

                        break;
                    }
                    case EVENT_LODESTONE_SPIKE:
                        me->CastSpell(me, SPELL_LODESTONE_SPIKE_DUMMY, false);
                        m_Events.ScheduleEvent(EVENT_LODESTONE_SPIKE, 30000);
                        break;
                    case EVENT_THUNDER_CACOPHONY:
                        if (m_Instance)
                            m_Instance->SetData(DATA_MAGNETS_ACHIEVEMENT, 1);
                        me->CastSpell(me, SPELL_THUNDER_CACOPHONY, false);
                        m_Events.ScheduleEvent(EVENT_THUNDER_CACOPHONY, 3000);
                        break;
                    case EVENT_SEAL_CONDUIT:
                    {
                        switch (m_SealConduitCount)
                        {
                            case 0: ///< 2 Conduits
                                ++m_SealConduitCount;
                                Talk(TALK_SPELL_1);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_FIRST, true);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_FIRST, true);
                                break;
                            case 1: ///< 4 Conduits
                                ++m_SealConduitCount;
                                Talk(TALK_SPELL_2);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_SECOND, true);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_SECOND, true);
                                break;
                            case 2: ///< 6 Conduits
                                ++m_SealConduitCount;
                                Talk(TALK_SPELL_3);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_THIRD, true);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_THIRD, true);
                                break;
                            case 3: ///< 7 Conduits
                            default:///< 7 Conduits
                                ++m_SealConduitCount;
                                Talk(TALK_SPELL_4);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_THIRD, true);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_THIRD, true);
                                me->CastSpell(me, SPELL_SEAL_CONDUIT_FIRST, true);
                                break;
                        }

                        me->MonsterTextEmote(LANG_DISRUPT_RUNE_CONDUIT, 0, true);
                        m_Events.ScheduleEvent(EVENT_SEAL_CONDUIT, 30000);
                        break;
                    }
                    default:
                        break;
                }
            }

            bool CanActivate()
            {
                std::list<Creature*> l_RunesOfPower;
                me->GetCreatureListWithEntryInGrid(l_RunesOfPower, NPC_RUNE_OF_POWER, 50.0f);

                for (Creature* l_Rune : l_RunesOfPower)
                {
                    if (!l_Rune->HasAura(SPELL_UNHARNESSED_POWER))
                        return false;
                }

                return true;
            }

            void InitializeCombat(uint32 const p_Diff)
            {
                if (!m_InitializeTimer)
                    return;

                if (m_InitializeTimer <= p_Diff)
                {
                    m_InitializeTimer = 0;

                    std::list<Creature*> l_RunesOfPower;
                    me->GetCreatureListWithEntryInGrid(l_RunesOfPower, NPC_RUNE_OF_POWER, 50.0f);

                    for (Creature* l_Rune : l_RunesOfPower)
                        l_Rune->AI()->DoAction(ACTION_POWER_CONDUIT);
                }
                else
                    m_InitializeTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_orebender_gorashanAI(p_Creature);
        }
};

///< Black Iron Apprentice - 76773
class mob_black_iron_apprentice : public CreatureScript
{
    public:
        mob_black_iron_apprentice() : CreatureScript("mob_black_iron_apprentice") { }

        struct mob_black_iron_apprenticeAI : public ScriptedAI
        {
            mob_black_iron_apprenticeAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (m_Instance)
                {
                    if (Creature* l_Orebender = Creature::GetCreature(*me, m_Instance->GetData64(NPC_OREBENDER_GORASHAN)))
                        l_Orebender->AI()->DoAction(ACTION_ENGAGE_APPRENTICE);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_apprenticeAI(p_Creature);
        }
};

///< Rune of Power - 76417
class mob_ubrs_rune_of_power : public CreatureScript
{
    public:
        mob_ubrs_rune_of_power() : CreatureScript("mob_ubrs_rune_of_power") { }

        struct mob_ubrs_rune_of_powerAI : public ScriptedAI
        {
            mob_ubrs_rune_of_powerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveAura(SPELL_UNHARNESSED_POWER);
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_POWER_CONDUIT:
                        me->RemoveAura(SPELL_UNHARNESSED_POWER);
                        me->CastSpell(me, SPELL_POWER_CONDUIT_VISUAL, false);
                        m_Events.ScheduleEvent(EVENT_POWER_CONDUIT, 1000);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* /*p_Caster*/, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == SPELL_DISRUPTING_CONDUCTOR)
                {
                    if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                    {
                        if (GameObject* l_Conductor = me->FindNearestGameObject(GOB_RUNIC_CONDUIT, 5.0f))
                        {
                            if (Creature* l_Orebender = Creature::GetCreature(*l_Conductor, l_InstanceScript->GetData64(NPC_OREBENDER_GORASHAN)))
                            {
                                if (Aura* l_Aura = l_Orebender->GetAura(SPELL_POWER_CONDUIT_VISUAL, me->GetGUID()))
                                    l_Aura->Remove();
                                if (Aura* l_ConduitAura = l_Orebender->GetAura(SPELL_POWER_CONDUIT_AURA))
                                    l_ConduitAura->ModStackAmount(-1);

                                me->CastSpell(me, SPELL_UNHARNESSED_POWER, true);
                                l_Orebender->AI()->DoAction(ACTION_CLOSE_POWER_CONDUIT);
                                l_Conductor->SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE | GO_FLAG_NODESPAWN);
                                l_Conductor->SetGoState(GO_STATE_READY);
                            }
                        }
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_POWER_CONDUIT:
                    {
                        if (GameObject* l_Conduit = me->FindNearestGameObject(GOB_RUNIC_CONDUIT, 5.0f))
                        {
                            l_Conduit->SetGoState(GO_STATE_ACTIVE);
                            l_Conduit->SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NODESPAWN);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_ubrs_rune_of_powerAI(p_Creature);
        }
};

///< Lightning Field - 76464
class mob_lightning_field : public CreatureScript
{
    public:
        mob_lightning_field() : CreatureScript("mob_lightning_field") { }

        struct mob_lightning_fieldAI : public ScriptedAI
        {
            mob_lightning_fieldAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint8 m_PositionID;
            EventMap m_Events;

            enum Actions
            {
                SetPosToNE,
                SetPosToSW
            };

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_CONFUSED);

                me->CastSpell(me, SPELL_ELECTRIC_PULSE_AURA, true);

                me->SetSpeed(MOVE_FLIGHT, 1.0f);

                m_PositionID = DATA_POS_NE;
                m_Events.ScheduleEvent(EVENT_MOVE_LIGHTNING, 1000);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case Actions::SetPosToNE:
                        m_PositionID = DATA_POS_NE;
                        break;
                    case Actions::SetPosToSW:
                        m_PositionID = DATA_POS_SW;
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID)
            {
                if (p_Type != POINT_MOTION_TYPE || p_ID != DATA_MOVE_LIGHT)
                    return;

                m_Events.ScheduleEvent(EVENT_MOVE_LIGHTNING, 400);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_MOVE_LIGHTNING:
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_CONFUSED);
                        me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                        if (m_PositionID < DATA_POS_NW)
                            me->GetMotionMaster()->MovePoint(DATA_MOVE_LIGHT, g_LightningFieldMovePos[++m_PositionID]);
                        else
                        {
                            m_PositionID = DATA_POS_NE;
                            me->GetMotionMaster()->MovePoint(DATA_MOVE_LIGHT, g_LightningFieldMovePos[m_PositionID]);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_lightning_fieldAI(p_Creature);
        }
};

///< Lodestone Spike - 154435
class spell_lodestone_spike: public SpellScriptLoader
{
    public:
        spell_lodestone_spike() : SpellScriptLoader("spell_lodestone_spike") { }

        class spell_lodestone_spike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lodestone_spike_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Target, SPELL_LODESTONE_SPIKE_DMG, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_lodestone_spike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lodestone_spike_SpellScript();
        }
};

///< Power Conduit - 166168
class spell_power_conduit_hangover: public SpellScriptLoader
{
    public:
        spell_power_conduit_hangover() : SpellScriptLoader("spell_power_conduit_hangover") { }

        class spell_power_conduit_hangover_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_power_conduit_hangover_AuraScript);

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, SPELL_POWER_CONDUIT_STUN, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_power_conduit_hangover_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_power_conduit_hangover_AuraScript();
        }
};

///< Seal Conduit - 154302 / 154900 / 154901
class spell_seal_conduit: public SpellScriptLoader
{
    public:
        spell_seal_conduit() : SpellScriptLoader("spell_seal_conduit") { }

        class spell_seal_conduit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_seal_conduit_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (l_Target->ToCreature() && l_Target->ToCreature()->GetAI())
                        l_Target->ToCreature()->AI()->DoAction(ACTION_POWER_CONDUIT);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_seal_conduit_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_seal_conduit_SpellScript();
        }
};

///< Electric Pulse - 154345
class spell_electric_pulse: public SpellScriptLoader
{
    public:
        spell_electric_pulse() : SpellScriptLoader("spell_electric_pulse") { }

        class spell_electric_pulse_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_electric_pulse_SpellScript);

            void CorrectRange(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    Position l_Pos;
                    l_Caster->GetPosition(&l_Pos);
                    if (p_Object->GetExactDist(&l_Pos) > 14.0f)
                        return true;

                    return false;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_electric_pulse_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_electric_pulse_SpellScript();
        }
};

///< Runic Conduit - 226704
class go_runic_conduit : public GameObjectScript
{
    public:
        go_runic_conduit() : GameObjectScript("go_runic_conduit") { }

        struct go_runic_conduitAI : public GameObjectAI
        {
            go_runic_conduitAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject), m_Activated(false), m_ResetTime(0) { }

            bool m_Activated;
            uint32 m_ResetTime;

            bool GossipHello(Player* p_Player)
            {
                if (go->GetGoState() == GO_STATE_ACTIVE && !m_Activated)
                {
                    m_Activated = true;
                    p_Player->CastSpell(go, SPELL_DISRUPTING_CONDUCTOR, false);
                    m_ResetTime = 1000;
                }

                return false;
            }

            void OnStateChanged(uint32 p_State)
            {
                if (p_State == GO_STATE_READY)
                    m_Activated = false;
            }

            void UpdateAI(uint32 p_Diff)
            {
                if (m_ResetTime)
                {
                    if (m_ResetTime <= p_Diff)
                    {
                        m_ResetTime = 0;
                        m_Activated = false;
                    }
                    else
                        m_ResetTime -= p_Diff;
                }
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const
        {
            return new go_runic_conduitAI(p_GameObject);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_orebender_gorashan()
{
    new boss_orebender_gorashan();
    new mob_black_iron_apprentice();
    new mob_ubrs_rune_of_power();
    new mob_lightning_field();
    new spell_lodestone_spike();
    new spell_power_conduit_hangover();
    new spell_seal_conduit();
    new spell_electric_pulse();
    new go_runic_conduit();
}
#endif
