////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "instance_bloodmaul.hpp"

namespace MS { namespace Instances { namespace Bloodmaul
{
    /// Forgemaster Gog'duh <Bloodmaul Slave Traders> - 74366
    class boss_forgemaster_gogduh : public CreatureScript
    {
        public:
            boss_forgemaster_gogduh() : CreatureScript("boss_forgemaster_gogduh") { }

            enum Spells
            {
                MagmaBarrage    = 150004,
                ThrowFlame      = 150078,
                ThrowEarth      = 150076,
            };

            enum Events
            {
                MagmaBarrageCast    = 1,
                SpawnRuination      = 2,
                SpawnCalamity       = 3,
            };

            enum NPCs
            {
                Ruination           = 74570,
                Calamity            = 74571,
                Magmolatus          = 74475,
                ForgemasterGogduh   = 74366
            };
                
            enum Yells
            {
                Aggro           = 0,
                RuinationSpawn  = 1,
                RuinationDeath  = 2,
                CalamitySpawn   = 3,
                CalamityDeath   = 4,
                Death           = 5
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new boss_AI(p_Creature);
            }

            struct boss_AI : public BossAI
            {
                boss_AI(Creature* p_Creature) : BossAI(p_Creature, BossIds::BossForgemasterGogduh)
                {
                    m_SaidAggro = false;

                    if (instance)
                        instance->SetBossState(BossIds::BossForgemasterGogduh, EncounterState::TO_BE_DECIDED);
                }

                void DoCast(Unit* p_UnitTarget, Spells p_SpellId, bool p_Triggered)
                {
                    me->CastSpell(p_UnitTarget, (uint32)p_SpellId, p_Triggered);
                }

                void DoCast(float p_X, float p_Y, float p_Z, Spells p_SpellId, bool p_Triggered)
                {
                    me->CastSpell(p_X, p_Y, p_Z, (uint32)p_SpellId, p_Triggered);
                }

                void MoveInLineOfSight(Unit* p_Who)
                {
                    if (!m_SaidAggro && me->IsWithinDist2d(p_Who, 30.0f))
                    {
                        m_SaidAggro = true;
                        Talk((uint8)Yells::Aggro);

                        if (Unit* p_Boss = me->FindNearestCreature((uint32)NPCs::Magmolatus, VISIBLE_RANGE, true))
                        {
                            if (BossAI* p_AI = CAST_AI(BossAI, p_Boss->GetAI()))
                                p_AI->Talk(Yells::Aggro);
                        }
                    }
                }

                void Reset()
                {
                    _Reset();
                }

                void JustReachedHome()
                {
                    _JustReachedHome();

                    if (instance)
                    {
                        instance->SetBossState(BossIds::BossForgemasterGogduh, FAIL);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    }
                }

                void JustDied(Unit*)
                {
                    Talk(Yells::Death);

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    if (Unit* p_Boss = me->FindNearestCreature((uint32)NPCs::Magmolatus, VISIBLE_RANGE, true))
                    {
                        if (UnitAI* p_AI = p_Boss->GetAI())
                            p_AI->DoAction(0);
                    }
                }

                void JustSummoned(Creature* p_Summon)
                {
                    BossAI::JustSummoned(p_Summon);
                }

                void EnterCombat(Unit*)
                {
                    _EnterCombat();

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                    events.ScheduleEvent((uint32)Events::MagmaBarrageCast, 4000);
                    events.ScheduleEvent((uint32)Events::SpawnRuination, 6000);
                    events.ScheduleEvent((uint32)Events::SpawnCalamity, 25000);
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim())
                    {
                        if (me->HasUnitState(UNIT_STATE_ROOT))
                            me->SetControlled(false, UNIT_STATE_ROOT);
                        return;
                    }

                    events.Update(diff);

                    switch ((Events)events.ExecuteEvent())
                    {
                        case Events::MagmaBarrageCast:
                        {
                            me->SetControlled(true, UNIT_STATE_ROOT);
                            DoCast(me, Spells::MagmaBarrage, false);
                            events.ScheduleEvent((uint32)Events::MagmaBarrageCast, 10000);
                            break;
                        }
                        case Events::SpawnRuination:
                        {
                            Talk((uint8)Yells::RuinationSpawn);
                            DoCastAOE((uint32)Spells::ThrowEarth, true);
                            break;
                        }
                        case Events::SpawnCalamity:
                        {
                            Talk((uint8)Yells::CalamitySpawn);
                            DoCastAOE((uint32)Spells::ThrowFlame, true);
                            break;
                        }
                        default:
                            break;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                        return;

                    if (me->HasUnitState(UNIT_STATE_ROOT))
                        me->SetControlled(false, UNIT_STATE_ROOT);

                    DoMeleeAttackIfReady();
                }

                bool m_SaidAggro = false;
            };
    };

    /// Magmolatus <Son of Slag> - 74475
    class boss_magmolatus : public CreatureScript
    {
        public:
            boss_magmolatus() : CreatureScript("boss_magmolatus") { }

            enum Spells
            {
                WitheringFlames = 150032,
                MoltenImpact    = 150045,
                SpellSlagSmash  = 150023
            };

            enum Events
            {
                CastFlames  = 1,
                CastImpact  = 2,
                CastSmash   = 3
            };

            enum Yells
            {
                Aggro           = 0,
                Release         = 1,
                SlagSmashYell   = 2,
                Elemental       = 3,
                Kill            = 4,
                Death           = 5
            };

            enum Achievements
            {
                AGiftOfEarthAndFire = 8993
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new boss_AI(p_Creature);
            }

            struct boss_AI : public BossAI
            {
                boss_AI(Creature* p_Creature) : BossAI(p_Creature, BossIds::BossForgemasterGogduh)
                {
                    me->SetControlled(true, UNIT_STATE_ROOT);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                    me->SetReactState(REACT_PASSIVE);

                    if (instance)
                        instance->SetBossState(BossIds::BossForgemasterGogduh, EncounterState::TO_BE_DECIDED);
                }

                void DoAction(const int32 /*p_Action*/)
                {
                    Talk((uint8)Yells::Release);
                    me->SetControlled(false, UNIT_STATE_ROOT);
                    me->GetMotionMaster()->MovePoint(1, 2082.0f, 116.0f, 225.0f);
                }

                void JustReachedHome()
                {
                    me->SetControlled(true, UNIT_STATE_ROOT);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                    me->SetReactState(REACT_PASSIVE);
                    _JustReachedHome();

                    if (Creature* p_Gogduh = me->FindNearestCreature((uint32)boss_forgemaster_gogduh::NPCs::ForgemasterGogduh, VISIBLE_RANGE, false))
                    {
                        p_Gogduh->Respawn(true);
                        p_Gogduh->GetMotionMaster()->MoveTargetedHome();
                    }

                    if (instance)
                    {
                        instance->SetBossState(uint32(BossIds::BossForgemasterGogduh), FAIL);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    }
                }

                void KilledUnit(Unit* p_Victim)
                {
                    if (p_Victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                        Talk((uint8)Yells::Kill);
                }

                void JustDied(Unit*)
                {
                    Talk((uint8)Yells::Death);

                    if (instance)
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    Creature* l_Calamity = me->FindNearestCreature((uint32)boss_forgemaster_gogduh::NPCs::Calamity, VISIBLE_RANGE);
                    Creature* l_Ruination = me->FindNearestCreature((uint32)boss_forgemaster_gogduh::NPCs::Ruination, VISIBLE_RANGE);

                    if (l_Calamity && l_Ruination && instance && me->GetMap()->IsHeroic())
                        instance->DoCompleteAchievement((uint32)Achievements::AGiftOfEarthAndFire);

                    if (l_Calamity)
                        l_Calamity->DisappearAndDie();

                    if (l_Ruination)
                        l_Ruination->DisappearAndDie();

                    _JustDied();
                }

                void MovementInform(uint32, uint32 p_Id)
                {
                    if (p_Id == 1)
                    {
                        if (instance)
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);

                        if (Unit* l_Victim = me->SelectNearestPlayer(VISIBLE_RANGE))
                            AttackStart(l_Victim);

                        events.ScheduleEvent((uint32)Events::CastFlames, 5000);
                    }
                }

                void NextEvent()
                {
                    switch (urand(0, 2))
                    {
                        case 0:
                            events.ScheduleEvent(uint32(Events::CastFlames), 5000);
                            break;
                        case 1:
                            events.ScheduleEvent(uint32(Events::CastImpact), 5000);
                            break;
                        case 2:
                            events.ScheduleEvent(uint32(Events::CastSmash), 5000);
                            break;
                        default:
                            break;
                                
                    }
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                        return;

                    events.Update(p_Diff);

                    switch ((Events)events.ExecuteEvent())
                    {
                        case Events::CastFlames:
                        {
                            DoCast(me, (uint32)Spells::WitheringFlames, false);
                            NextEvent();
                            break;
                        }
                        case Events::CastImpact:
                        {
                            Talk((uint8)Yells::Elemental);

                            if (Unit* l_Target = me->SelectNearbyTarget(nullptr, VISIBLE_RANGE))
                                me->CastSpell(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), (uint32)Spells::MoltenImpact, false);

                            NextEvent();
                            break;
                        }
                        case Events::CastSmash:
                        {
                            Talk((uint8)Yells::SlagSmashYell);
                            DoCastAOE((uint32)Spells::SpellSlagSmash);
                            NextEvent();
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };
    };

    /// Ruination <Servant of the Earth> - 74570
    class npc_ruination : public CreatureScript
    {
        public:
            npc_ruination() : CreatureScript("npc_ruination") { }

            enum Spells
            {
                ShatterEarth    = 150324,
                EarthSmash      = 149941
            };

            enum Events
            {
                CastSpell = 1
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_ruinationAI(p_Creature);
            }

            struct npc_ruinationAI : public ScriptedAI
            {
                npc_ruinationAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                    events.ScheduleEvent((uint32)Events::CastSpell, 6000);
                }

                void DoCast(Unit* p_UnitTarget, Spells p_SpellId, bool p_Triggered)
                {
                    me->CastSpell(p_UnitTarget, (uint32)p_SpellId, p_Triggered);
                }

                void JustDied(Unit* /* p_Killer */)
                {
                    if (Creature* l_Boss = me->FindNearestCreature((uint32)boss_forgemaster_gogduh::NPCs::ForgemasterGogduh, VISIBLE_RANGE))
                    {
                        if (CreatureAI* l_AI = CAST_AI(CreatureAI, l_Boss->GetAI()))
                            l_AI->Talk((uint8)boss_forgemaster_gogduh::Yells::RuinationDeath);
                    }
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim())
                    {
                        if (me->HasUnitState(UNIT_STATE_ROOT))
                            me->SetControlled(false, UNIT_STATE_ROOT);
                        return;
                    }

                    if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                        return;

                    if (me->HasUnitState(UNIT_STATE_ROOT))
                        me->SetControlled(false, UNIT_STATE_ROOT);

                    events.Update(diff);

                    if ((Events)events.ExecuteEvent() == Events::CastSpell)
                    {
                        me->SetControlled(true, UNIT_STATE_ROOT);
                        DoCast(me->getVictim(), urand(0, 1) ? Spells::ShatterEarth : Spells::EarthSmash, false);
                        events.ScheduleEvent((uint32)Events::CastSpell, 5000);
                    }

                    DoMeleeAttackIfReady();
                }
            };
    };

    /// Calamity <Servant of the Flame> - 74571
    class npc_calamity : public CreatureScript
    {
        public:
            npc_calamity() : CreatureScript("npc_calamity") { }

            enum Spells
            {
                DancingFlames   = 149975,
                Firestorm       = 144461,
                Scorch          = 150290
            };

            enum Events
            {
                CastSpell = 1,
                CastScorch,
                RefreshFlames
            };

            enum Action
            {
                ActionJumpFlames
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_calamityAI(p_Creature);
            }

            struct npc_calamityAI : public ScriptedAI
            {
                npc_calamityAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                    events.ScheduleEvent((uint32)Events::CastSpell, 12000);
                    events.ScheduleEvent((uint32)Events::CastScorch, 3000);
                }

                void JustDied(Unit*)
                {
                    if (Creature* l_Boss = me->FindNearestCreature((uint32)boss_forgemaster_gogduh::NPCs::ForgemasterGogduh, VISIBLE_RANGE))
                    {
                        if (CreatureAI* l_AI = CAST_AI(CreatureAI, l_Boss->GetAI()))
                            l_AI->Talk((uint8)boss_forgemaster_gogduh::Yells::CalamityDeath);
                    }
                }

                void DoAction(int32 const p_Action)
                {
                    if (p_Action == Action::ActionJumpFlames)
                        events.ScheduleEvent(Events::RefreshFlames, 100);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(p_Diff);

                    switch ((Events)events.ExecuteEvent())
                    {
                        case Events::CastSpell:
                        {
                            me->InterruptNonMeleeSpells(false);
                            me->CastSpell(me, uint32(urand(0, 1) ? Spells::DancingFlames : Spells::Firestorm), false);
                            events.ScheduleEvent((uint32)Events::CastSpell, 12000);
                            break;
                        }
                        case Events::CastScorch:
                        {
                            events.ScheduleEvent((uint32)Events::CastScorch, 3000);

                            if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                                break;

                            me->CastSpell(me->getVictim(), (uint32)Spells::Scorch, false);
                            break;
                        }
                        case Events::RefreshFlames:
                        {
                            me->CastSpell(me, Spells::DancingFlames, true);
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };
    };

    /// Firestorm - 71971
    class npc_calamity_firestorm : public CreatureScript
    {
        public:
            npc_calamity_firestorm() : CreatureScript("npc_calamity_firestorm") { }

            enum Spells
            {
                Firestorm = 144463
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_calamity_firestormAI(p_Creature);
            }

            struct npc_calamity_firestormAI : public ScriptedAI
            {
                npc_calamity_firestormAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    me->CastSpell(me, (uint32)Spells::Firestorm, true);
                    m_Timer = 12000;
                }

                void UpdateAI(uint32 const p_Diff)
                {
                    if (m_Timer <= p_Diff)
                        me->DisappearAndDie();
                    else
                        m_Timer -= p_Diff;
                }

                uint32 m_Timer;
            };
    };

    /// Shatter Earth - 74766
    class npc_shatter_earth : public CreatureScript
    {
        public:
            npc_shatter_earth() : CreatureScript("npc_shatter_earth") { }

            enum Spells
            {
                ShatterEarthDamage  = 149963
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_shatter_earthAI(p_Creature);
            }

            struct npc_shatter_earthAI : public ScriptedAI
            {
                npc_shatter_earthAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    if (Unit* l_Target = me->SelectNearbyTarget(nullptr, 4.0f, (uint32)Spells::ShatterEarthDamage))
                        me->CastSpell(l_Target, (uint32)Spells::ShatterEarthDamage, true);
                    m_DeathTimer = 2000;
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (me->isDead())
                        return;

                    if (m_DeathTimer <= 0)
                    {
                        me->DealDamage(me, me->GetHealth(), NULL, NODAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        me->DespawnOrUnsummon(0);
                    }
                    else
                        m_DeathTimer -= p_Diff;
                }

                int32 m_DeathTimer;
            };
    };

    /// Molten Elemental - 74579
    class npc_gugdoh_molten_elemental : public CreatureScript
    {
        public:
            npc_gugdoh_molten_elemental() : CreatureScript("npc_gugdoh_molten_elemental") { }

            enum Spells
            {
                MagmaBarrage    = 150004,
                VolcanicTantrum = 150048
            };

            enum Events
            {
                CastSpell = 1
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_gugdoh_molten_elementalAI(p_Creature);
            }

            struct npc_gugdoh_molten_elementalAI : public ScriptedAI
            {
                npc_gugdoh_molten_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                void Reset()
                {
                    events.Reset();
                    events.ScheduleEvent((uint32)Events::CastSpell, 100);
                }

                void UpdateAI(const uint32 p_Diff)
                {
                    if (!UpdateVictim())
                        return;

                    if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                        return;

                    events.Update(p_Diff);

                    if ((Events)events.ExecuteEvent() == Events::CastSpell)
                    {
                        me->CastSpell(me, uint32(urand(0, 1) ? Spells::VolcanicTantrum : Spells::MagmaBarrage), false);
                        events.ScheduleEvent((uint32)Events::CastSpell, 5000);
                    }

                    DoMeleeAttackIfReady();
                }
            };
    };

    /// Magma Barrage - 150004
    class spell_magma_barrage: public SpellScriptLoader
    {
        public:
            spell_magma_barrage() : SpellScriptLoader("spell_magma_barrage") { }

            class spell_magma_barrage_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_magma_barrage_AuraScript)

                void OnPeriodic(AuraEffect const* p_AurEff)
                {
                    Unit* l_Caster = GetCaster();
                    if (!l_Caster)
                        return;

                    if (Unit* l_Target = l_Caster->SelectNearbyTarget(nullptr, VISIBLE_RANGE))
                        l_Caster->CastSpell(l_Target, GetSpellInfo()->Effects[p_AurEff->GetEffIndex()].TriggerSpell, true);
                }

                void Register()
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_magma_barrage_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_magma_barrage_AuraScript();
            }
    };

    /// Rough Smash - 149941
    class spell_rough_smash: public SpellScriptLoader
    {
        public:
            spell_rough_smash() : SpellScriptLoader("spell_rough_smash") { }

            class spell_rough_smash_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_rough_smash_SpellScript);

                class RoughSmashTargetFilter
                {
                    public:
                        explicit RoughSmashTargetFilter(Position* p_CasterPosition) : m_CasterPosition(p_CasterPosition) { }

                        bool operator()(WorldObject* p_Unit) const
                        {
                            return !m_CasterPosition->HasInArc(float(M_PI) / 3.0f, p_Unit);
                        }

                    private:
                        Position* m_CasterPosition;
                };

                void FilterTargets(std::list<WorldObject*>& p_Targets)
                {
                    p_Targets.remove_if(RoughSmashTargetFilter(&m_CastPosition));
                }

                void HandleOnPrepare()
                {
                    GetCaster()->GetPosition(&m_CastPosition);
                }

                Position m_CastPosition;

                void Register()
                {
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rough_smash_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rough_smash_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
                    OnPrepare += SpellOnPrepareFn(spell_rough_smash_SpellScript::HandleOnPrepare);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_rough_smash_SpellScript();
            }
    };

    /// Shatter Earth - 150324
    class spell_shatter_earth: public SpellScriptLoader
    {
        public:
            spell_shatter_earth() : SpellScriptLoader("spell_shatter_earth") { }

            enum Spells
            {
                ShatterEarthDamage = 150344,
                ShatterEarthNE     = 149959,
                ShatterEarthNW     = 149968,
                ShatterEarthSW     = 149970,
                ShatterEarthSE     = 149969
            };

            class spell_shatter_earth_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_shatter_earth_SpellScript);

                void OnSpellHit()
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        uint32 const ShatterEarthSpikeSpells[] { (uint32)Spells::ShatterEarthNE, (uint32)Spells::ShatterEarthNW, (uint32)Spells::ShatterEarthSW, (uint32)Spells::ShatterEarthSE };

                        if (Unit* l_Target = l_Caster->SelectNearbyTarget(nullptr, VISIBLE_RANGE))
                        {
                            for (uint8 l_I = 0; l_I < 4; l_I++)
                                l_Caster->CastSpell(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), ShatterEarthSpikeSpells[l_I], true);
                        }

                        l_Caster->CastSpell(l_Caster, (uint32)Spells::ShatterEarthDamage, true);
                    }
                }

                void Register()
                {
                    OnHit += SpellHitFn(spell_shatter_earth_SpellScript::OnSpellHit);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_shatter_earth_SpellScript();
            }
    };

    /// Dancing Flames - 149975
    class spell_dancing_flames: public SpellScriptLoader
    {
        public:
            spell_dancing_flames() : SpellScriptLoader("spell_dancing_flames") { }

            enum Spells
            {
                DancingFlames = 149975
            };

            enum Action
            {
                ActionJumpFlames
            };

            class spell_dancing_flames_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_dancing_flames_AuraScript);

                void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
                {
                    m_Dispelled = false;
                }

                void HandleDispel(DispelInfo* /*dispelInfo*/)
                {
                    m_Dispelled = true;
                }

                void HandleRemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
                {
                    if (m_Dispelled || !GetCaster())
                        return;

                    if (Creature* l_Caster = GetCaster()->ToCreature())
                    {
                        if (l_Caster->IsAIEnabled)
                            l_Caster->AI()->DoAction(Action::ActionJumpFlames);
                    }
                }

                bool m_Dispelled;

                void Register()
                {
                    OnEffectApply += AuraEffectApplyFn(spell_dancing_flames_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                    OnDispel += AuraDispelFn(spell_dancing_flames_AuraScript::HandleDispel);
                    OnEffectRemove += AuraEffectRemoveFn(spell_dancing_flames_AuraScript::HandleRemoveEffect, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_dancing_flames_AuraScript();
            }
    };

    /// Withering Flames - 150032
    class spell_withering_flames: public SpellScriptLoader
    {
        public:
            spell_withering_flames() : SpellScriptLoader("spell_withering_flames") { }
 
            enum Spells
            {
                ChokingAshes = 150034
            };

            class spell_withering_flames_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_withering_flames_AuraScript)

                void OnPeriodic(AuraEffect const*)
                {
                    Unit* l_Caster = GetCaster();
                    if (!l_Caster)
                        return;

                    if (Unit* l_Target = l_Caster->SelectNearbyTarget(GetOwner()->ToUnit(), VISIBLE_RANGE, (uint32)Spells::ChokingAshes))
                        l_Caster->CastSpell(l_Target, (uint32)Spells::ChokingAshes, true);
                }
 
                void Register()
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_withering_flames_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_withering_flames_AuraScript();
            }
    };

    /// Molten Impact - 150045
    class spell_molten_impact: public SpellScriptLoader
    {
        public:
            spell_molten_impact() : SpellScriptLoader("spell_molten_impact") { }

            class spell_molten_impact_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_molten_impact_SpellScript);

                void HandleScript(SpellEffIndex)
                {
                    SetHitDamage((float)GetHitDamage() * (30.0f - std::min((float)GetHitDest()->GetExactDist(GetHitUnit()), 30.0f)) / 30.0f);
                }

                void Register()
                {
                    OnEffectHitTarget += SpellEffectFn(spell_molten_impact_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_molten_impact_SpellScript();
            }
    };

    /// Magma Barrage (triggered) - 150007, 150306
    class spell_magma_barrage_trigger: public SpellScriptLoader
    {
        public:
            spell_magma_barrage_trigger() : SpellScriptLoader("spell_magma_barrage_trigger") { }

            enum Spells
            {
                MagmaBarrageTrigger = 167465
            };

            class spell_magma_barrage_trigger_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_magma_barrage_trigger_SpellScript);

                void OnSpellHit(SpellEffIndex)
                {
                    Unit* l_Caster = GetCaster();
                    WorldLocation const* l_Dest = GetExplTargetDest();

                    if (!l_Caster || !l_Dest)
                        return;

                    l_Caster->CastSpell(l_Dest->m_positionX, l_Dest->m_positionY, l_Dest->m_positionZ, (uint32)Spells::MagmaBarrageTrigger, true);
                }

                void Register()
                {
                    OnEffectHitTarget += SpellEffectFn(spell_magma_barrage_trigger_SpellScript::OnSpellHit, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_magma_barrage_trigger_SpellScript();
            }
    };

    /// Magma Barrage (damage) - 150011
    class spell_magma_barrage_damage: public SpellScriptLoader
    {
        public:
            spell_magma_barrage_damage() : SpellScriptLoader("spell_magma_barrage_damage") { }

            enum Spells
            {
                MagmaBarrageAreaTrigger = 167465
            };

            class spell_magma_barrage_damage_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_magma_barrage_damage_AuraScript)

                void OnPeriodic(AuraEffect const* p_AurEff)
                {
                    Unit* l_Owner = GetOwner()->ToPlayer();
                    if (!l_Owner)
                        return;

                    AreaTrigger* l_Trigger = nullptr;
                    JadeCore::NearestAreaTriggerWithIdInObjectRangeCheck u_check(l_Owner, (uint32)Spells::MagmaBarrageAreaTrigger, 3.0f);
                    JadeCore::AreaTriggerSearcher<JadeCore::NearestAreaTriggerWithIdInObjectRangeCheck> searcher(l_Owner, l_Trigger, u_check);
                    l_Owner->VisitNearbyObject(3.0f, searcher);

                    if (!l_Trigger)
                        p_AurEff->GetBase()->Remove();
                }

                void Register()
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_magma_barrage_damage_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_magma_barrage_damage_AuraScript();
            }
    };

    /// Shatter Earth - 149959, 149968, 149969, 149970
    class areatrigger_shatter_earth : public AreaTriggerEntityScript
    {
        enum Spells
        {
            ShatterEarthNE     = 149959,
            ShatterEarthNW     = 149968,
            ShatterEarthSW     = 149970,
            ShatterEarthSE     = 149969,
            ShatterEarthSpawn  = 150325,
            ShatterEarthDamage = 149963
        };

        public:
            areatrigger_shatter_earth() : AreaTriggerEntityScript("areatrigger_shatter_earth") { }

            void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& /*p_PathToLinearDestination*/)
            {
                if (!p_Caster)
                    return;

                float l_Orientation;
                switch ((Spells)p_AreaTrigger->GetSpellId())
                {
                    case Spells::ShatterEarthNW:
                        l_Orientation = M_PI / 4.0f; // 45
                        break;
                    case Spells::ShatterEarthSW:
                        l_Orientation = M_PI / 4.0f * 3.0f; // 135
                        break;
                    case  Spells::ShatterEarthSE:
                        l_Orientation = M_PI / 4.0f * 5.0f; // 225
                        break;
                    case Spells::ShatterEarthNE:
                        l_Orientation = M_PI / 4.0f * 7.0f; // 315
                        break;
                    default:
                        l_Orientation = 0.0f;
                }

                p_SourcePosition.m_positionX = p_SourcePosition.GetPositionX() + (cos(l_Orientation) * 8.0f);
                p_SourcePosition.m_positionY = p_SourcePosition.GetPositionY() + (sin(l_Orientation) * 8.0f);
                p_SourcePosition.m_positionZ = p_SourcePosition.GetPositionZ();
                p_Caster->UpdateGroundPositionZ(p_SourcePosition.m_positionX, p_SourcePosition.m_positionY, p_SourcePosition.m_positionZ);
                p_SourcePosition.SetOrientation(l_Orientation);

                p_DestinationPosition.m_positionX = p_SourcePosition.GetPositionX() + (cos(l_Orientation) * 20.0f);
                p_DestinationPosition.m_positionY = p_SourcePosition.GetPositionY() + (sin(l_Orientation) * 20.0f);
                p_DestinationPosition.m_positionZ = p_SourcePosition.GetPositionZ();
                p_Caster->UpdateGroundPositionZ(p_DestinationPosition.m_positionX, p_DestinationPosition.m_positionY, p_DestinationPosition.m_positionZ);
                p_DestinationPosition.SetOrientation(l_Orientation);
            }

            void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
            {
                Unit* l_Caster = p_AreaTrigger->GetCaster();
                std::list<Unit*> l_TargetList;
                float l_Radius = 3.0f;

                if (!l_Caster)
                    return;

                JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_TargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);
                bool l_HasTarget = false;

                for (auto& l_Itr : l_TargetList)
                {
                    if (!l_Itr->HasAura((uint32)Spells::ShatterEarthDamage))
                    {
                        l_HasTarget = true;
                        break;
                    }
                }

                if (l_HasTarget)
                    l_Caster->CastSpell(p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), (uint32)Spells::ShatterEarthSpawn, true);
            }

            AreaTriggerEntityScript* GetAI() const
            {
                return new areatrigger_shatter_earth();
            }
    };

    /// Volcanic Trantum - 150050, 150051, 150052, 150053
    class areatrigger_volcanic_trantum : public AreaTriggerEntityScript
    {
        enum Spells
        {
            VolcanicTrantrumNE      = 150050,
            VolcanicTrantrumNW      = 150051,
            VolcanicTrantrumSW      = 150052,
            VolcanicTrantrumSE      = 150053,
            VolcanicTrantrumDamage  = 150055
        };

        public:
            areatrigger_volcanic_trantum() : AreaTriggerEntityScript("areatrigger_volcanic_trantum") { }

            void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& /*p_PathToLinearDestination*/)
            {
                if (!p_Caster)
                    return;

                float l_Orientation;
                switch ((Spells)p_AreaTrigger->GetSpellId())
                {
                    case Spells::VolcanicTrantrumNW:
                        l_Orientation = M_PI / 4.0f; // 45
                        break;
                    case Spells::VolcanicTrantrumSW:
                        l_Orientation = M_PI / 4.0f * 3.0f; // 135
                        break;
                    case Spells::VolcanicTrantrumSE:
                        l_Orientation = M_PI / 4.0f * 5.0f; // 225
                        break;
                    case Spells::VolcanicTrantrumNE:
                        l_Orientation = M_PI / 4.0f * 7.0f; // 315
                        break;
                    default:
                        l_Orientation = 0.0f;
                }

                p_SourcePosition.m_positionX = p_Caster->GetPositionX() + (cos(l_Orientation) * 8.0f);
                p_SourcePosition.m_positionY = p_Caster->GetPositionY() + (sin(l_Orientation) * 8.0f);
                p_SourcePosition.m_positionZ = p_Caster->GetPositionZ();
                p_Caster->UpdateGroundPositionZ(p_SourcePosition.m_positionX, p_SourcePosition.m_positionY, p_SourcePosition.m_positionZ);
                p_SourcePosition.SetOrientation(l_Orientation);

                p_DestinationPosition.m_positionX = p_SourcePosition.GetPositionX() + (cos(l_Orientation) * 20.0f);
                p_DestinationPosition.m_positionY = p_SourcePosition.GetPositionY() + (sin(l_Orientation) * 20.0f);
                p_DestinationPosition.m_positionZ = p_SourcePosition.GetPositionZ();
                p_Caster->UpdateGroundPositionZ(p_DestinationPosition.m_positionX, p_DestinationPosition.m_positionY, p_DestinationPosition.m_positionZ);
                p_DestinationPosition.SetOrientation(l_Orientation);
            }

            void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
            {
                Unit* l_Caster = p_AreaTrigger->GetCaster();
                Unit* l_Target = nullptr;
                std::list<Unit*> l_TargetList;
                float l_Radius = 3.0f;

                if (!l_Caster)
                    return;

                JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_TargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

                for (auto& l_Itr : l_TargetList)
                {
                    if (!l_Itr->HasAura((uint32)Spells::VolcanicTrantrumDamage))
                    {
                        l_Target = l_Itr;
                        break;
                    }
                }

                if (l_Target)
                    l_Caster->CastSpell(l_Target, (uint32)Spells::VolcanicTrantrumDamage, true);
            }
                
            AreaTriggerEntityScript* GetAI() const
            {
                return new areatrigger_volcanic_trantum();
            }
    };

    /// Magma Barrage - 167465
    class areatrigger_magma_barrage : public AreaTriggerEntityScript
    {
        enum Spells
        {
            MagmaBarrageDamage = 150011
        };

        public:
            areatrigger_magma_barrage() : AreaTriggerEntityScript("areatrigger_magma_barrage") { }

            void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
            {
                Unit* l_Caster = p_AreaTrigger->GetCaster();
                std::list<Unit*> l_TargetList;
                float l_Radius = 3.0f;

                if (!l_Caster)
                    return;

                JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(p_AreaTrigger, l_TargetList, u_check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, searcher);

                for (auto& l_Itr : l_TargetList)
                {
                    if (!l_Itr->HasAura((uint32)Spells::MagmaBarrageDamage))
                        l_Caster->CastSpell(l_Itr, (uint32)Spells::MagmaBarrageDamage, true);
                }
            }

            AreaTriggerEntityScript* GetAI() const
            {
                return new areatrigger_magma_barrage();
            }
    };
}
} } ///< namespace MS::Instances

#ifndef __clang_analyzer__
void AddSC_boss_forgemaster_gogduh()
{
    /// Bosses
    new MS::Instances::Bloodmaul::boss_forgemaster_gogduh();
    new MS::Instances::Bloodmaul::boss_magmolatus();

    /// Summons
    new MS::Instances::Bloodmaul::npc_ruination();
    new MS::Instances::Bloodmaul::npc_calamity_firestorm();
    new MS::Instances::Bloodmaul::npc_calamity();
    new MS::Instances::Bloodmaul::npc_shatter_earth();
    new MS::Instances::Bloodmaul::npc_gugdoh_molten_elemental();

    /// Spells
    new MS::Instances::Bloodmaul::spell_magma_barrage();
    new MS::Instances::Bloodmaul::spell_rough_smash();
    new MS::Instances::Bloodmaul::spell_shatter_earth();
    new MS::Instances::Bloodmaul::spell_dancing_flames();
    new MS::Instances::Bloodmaul::spell_withering_flames();
    new MS::Instances::Bloodmaul::spell_molten_impact();
    new MS::Instances::Bloodmaul::spell_magma_barrage_damage();
    new MS::Instances::Bloodmaul::spell_magma_barrage_trigger();

    /// AreaTriggers
    new MS::Instances::Bloodmaul::areatrigger_shatter_earth();
    new MS::Instances::Bloodmaul::areatrigger_volcanic_trantum();
    new MS::Instances::Bloodmaul::areatrigger_magma_barrage();
}
#endif