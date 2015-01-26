#include "instance_bloodmaul.h"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {
            class boss_forgemaster_gogduh : public CreatureScript
            {
            public:
                boss_forgemaster_gogduh() : CreatureScript("boss_forgemaster_gogduh")
                {
                }

                enum Spells
                {
                    MagmaBarrage = 150004,
                };

                enum Events
                {
                    MagmaBarrageCast    = 1,
                    SpawnRuination      = 2,
                    SpawnCalamity       = 3,
                };

                enum NPCs
                {
                    Ruination   = 74570,
                    Calamity    = 74571,
                    Magmolatus  = 86221   
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new boss_AI(creature);
                }

                struct boss_AI : public BossAI
                {
                    boss_AI(Creature* p_Creature) : BossAI(p_Creature, BossIds::BossForgemasterGogduh)
                    {
                        if (instance)
                            instance->SetBossState(BossIds::BossForgemasterGogduh, EncounterState::TO_BE_DECIDED);
                    }

                    Position const m_RuinationSpawnPos = Position{2093.f, 113.f, 225.f, 4.583f};
                    Position const m_CalamitySpawnPos  = Position{2071.f, 111.f, 225.f, 5.003f};
                    
                    void DoCast(Unit* p_UnitTarget, Spells p_SpellId, bool p_Triggered)
                    {
                        me->CastSpell(p_UnitTarget, (uint32)p_SpellId, p_Triggered);
                    }
                    
                    void DoCast(float p_X, float p_Y, float p_Z, Spells p_SpellId, bool p_Triggered)
                    {
                        me->CastSpell(p_X, p_Y, p_Z, (uint32)p_SpellId, p_Triggered);
                    }

                    void Reset()
                    {
                        DespawnAllSummons();
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

                    void JustDied(Unit* /*killer*/)
                    {
                        DespawnAllSummons();
                        
                        if (instance)
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                        if (Unit* p_Boss = me->FindNearestCreature((uint32)NPCs::Magmolatus, VISIBLE_RANGE, true))
                            if (UnitAI* p_AI = p_Boss->GetAI())
                                p_AI->DoAction(0);
                    }


                    void EnterCombat(Unit* who)
                    {
                        _EnterCombat();

                        if (instance)
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                        events.ScheduleEvent((uint32)Events::MagmaBarrageCast, 5000);
                        events.ScheduleEvent((uint32)Events::SpawnRuination, 15000);
                        events.ScheduleEvent((uint32)Events::SpawnCalamity, 30000);
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
                                me->SetControlled(true, UNIT_STATE_ROOT);
                                DoCast(me, Spells::MagmaBarrage, false);
                                events.ScheduleEvent((uint32)Events::MagmaBarrageCast, 10000);
                                break;
                            case Events::SpawnRuination:
                                if (Unit* l_Summon = SummonCreature(NPCs::Ruination, m_RuinationSpawnPos))
                                    l_Summon->Attack(me->getVictim(), true);
                                break;
                            case Events::SpawnCalamity:
                                if (Unit* l_Summon = SummonCreature(NPCs::Calamity, m_CalamitySpawnPos))
                                    l_Summon->Attack(me->getVictim(), true);
                                break;
                            default:
                                break;
                        }

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            return;

                        if (me->HasUnitState(UNIT_STATE_ROOT))
                            me->SetControlled(false, UNIT_STATE_ROOT);
                    }

                    Unit* SummonCreature(NPCs p_Creature, Position const& p_Position)
                    {
                        if (TempSummon* l_Summon = me->SummonCreature((uint32)p_Creature, p_Position, TEMPSUMMON_DEAD_DESPAWN))
                        {
                            m_SpawnedCreatures.push_back(l_Summon->GetGUID());
                            return l_Summon;
                        }

                        return nullptr;
                    }

                    void DespawnAllSummons()
                    {
                        for (auto& l_GUID : m_SpawnedCreatures)
                            if (Unit* l_Spawn = sObjectAccessor->GetCreature(*me, l_GUID))
                                if (TempSummon* l_TempSummon = l_Spawn->ToTempSummon())
                                    l_TempSummon->UnSummon();
                    }

                    std::list<uint64> m_SpawnedCreatures;
                };
            };

            class boss_magmolatus : public CreatureScript
            {
            public:
                boss_magmolatus() : CreatureScript("boss_magmolatus")
                {
                }

                enum Spells
                {
                    WitheringFlames = 150032,
                    MoltenImpact    = 150045
                };

                enum Events
                {
                    CastFlames  = 1,
                    CastImpact  = 2
                };

                enum NPCs
                {
                    ForgemasterGogduh   = 74366
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new boss_AI(creature);
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

                    void DoAction(const int32 p_Action)
                    {
                        me->SetControlled(false, UNIT_STATE_ROOT);
                        me->GetMotionMaster()->MovePoint(1, 2082.f, 116.f, 225.f);
                    }

                    void JustReachedHome()
                    {
                        me->SetControlled(true, UNIT_STATE_ROOT);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
                        me->SetReactState(REACT_PASSIVE);
                        _JustReachedHome();

                        if (Creature* p_Gogduh = me->FindNearestCreature((uint32)NPCs::ForgemasterGogduh, VISIBLE_RANGE, false))
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
                    
                    void JustDied(Unit* /*killer*/)
                    {
                        if (instance)
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                        _JustDied();
                    }

                    void MovementInform(uint32 p_MoveType, uint32 p_Id)
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

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            return;
                            
                        events.Update(diff);

                        switch ((Events)events.ExecuteEvent())
                        {
                            case Events::CastFlames:
                                DoCast(me, (uint32)Spells::WitheringFlames, false);
                                events.ScheduleEvent(uint32(urand(0, 1) ? Events::CastFlames : Events::CastImpact), 5000);
                                break;
                            case Events::CastImpact:
                                if (Unit* l_Target = me->SelectNearbyTarget(nullptr, VISIBLE_RANGE))
                                    me->CastSpell(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), (uint32)Spells::MoltenImpact, false);
                                events.ScheduleEvent(uint32(urand(0, 1) ? Events::CastFlames : Events::CastImpact), 5000);
                                break;
                            default:
                                break;
                        }

                        DoMeleeAttackIfReady();
                    }

                };
            };

            class npc_ruination : public CreatureScript
            {
            public:
                npc_ruination() : CreatureScript("npc_ruination")
                {
                }

                enum Spells
                {
                    ShatterEarth    = 150324,
                    EarthSmash      = 149941
                };

                enum Events
                {
                    CastSpell = 1
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new npc_ruinationAI(creature);
                }

                struct npc_ruinationAI : public ScriptedAI
                {
                    npc_ruinationAI(Creature* p_Creature) : ScriptedAI(p_Creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                        events.ScheduleEvent((uint32)Events::CastSpell, 6000);
                    }
                    
                    void DoCast(Unit* p_UnitTarget, Spells p_SpellId, bool p_Triggered)
                    {
                        me->CastSpell(p_UnitTarget, (uint32)p_SpellId, p_Triggered);
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

            class npc_calamity : public CreatureScript
            {
            public:
                npc_calamity() : CreatureScript("npc_calamity")
                {
                }

                enum Spells
                {
                    DancingFlames   = 149975,
                    Firestorm       = 144461,
                    Scorch          = 150290
                };

                enum Events
                {
                    CastSpell   = 1,
                    CastScorch  = 2
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new npc_calamityAI(creature);
                }

                struct npc_calamityAI : public ScriptedAI
                {
                    npc_calamityAI(Creature* p_Creature) : ScriptedAI(p_Creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                        events.ScheduleEvent((uint32)Events::CastSpell, 12000);
                        events.ScheduleEvent((uint32)Events::CastScorch, 3000);
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;

                        events.Update(diff);
                        
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
                            default:
                                break;
                        }
                        
                        DoMeleeAttackIfReady();
                    }

                };
            };

            class npc_calamity_firestorm : public CreatureScript
            {
            public:
                npc_calamity_firestorm() : CreatureScript("npc_calamity_firestorm")
                {
                }

                enum Spells
                {
                    Firestorm = 144463
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new npc_calamity_firestormAI(creature);
                }

                struct npc_calamity_firestormAI : public ScriptedAI
                {
                    npc_calamity_firestormAI(Creature* p_Creature) : ScriptedAI(p_Creature)
                    {
                    }

                    void Reset()
                    {
                        me->CastSpell(me, (uint32)Spells::Firestorm, true);
                        m_Timer = 12000;
                    }

                    void UpdateAI(uint32 const p_Diff)
                    {
                        if (m_Timer < p_Diff)
                        {
                            me->DisappearAndDie();
                        }
                        else m_Timer -= p_Diff;
                    }
                    uint32 m_Timer;
                };
            };
            
            class spell_magma_barrage: public SpellScriptLoader
            {
                public:
                    spell_magma_barrage() : SpellScriptLoader("spell_magma_barrage") { }
 
                    class spell_magma_barrage_AuraScript : public AuraScript
                    {
                        PrepareAuraScript(spell_magma_barrage_AuraScript)

                        void OnPeriodic(constAuraEffectPtr aurEff)
                        {
                            Unit* l_Caster = GetCaster();
                            if (!l_Caster)
                                return;

                            if (Unit* l_Target = l_Caster->SelectNearbyTarget(nullptr, VISIBLE_RANGE))
                                l_Caster->CastSpell(l_Target, GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true);
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
                            return !m_CasterPosition->HasInArc(float(M_PI) / 3.f, p_Unit);
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
            };

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
                        uint32 const ShatterEarthSpikeSpells[] {(uint32)Spells::ShatterEarthNE, (uint32)Spells::ShatterEarthNW, (uint32)Spells::ShatterEarthSW, (uint32)Spells::ShatterEarthSE } ;

                        if (Unit* l_Target = GetCaster()->SelectNearbyTarget(nullptr, VISIBLE_RANGE))
                            for (int l_I = 0; l_I < 4; l_I++)
                                GetCaster()->CastSpell(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), ShatterEarthSpikeSpells[l_I], true);

                        GetCaster()->CastSpell(GetCaster(), (uint32)Spells::ShatterEarthDamage, true);
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
                areatrigger_shatter_earth() : AreaTriggerEntityScript("areatrigger_shatter_earth")
                {
                }
        
                void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition)
                {
                    if (!p_Caster)
                        return;

                    float l_Orientation;
                    switch ((Spells)p_AreaTrigger->GetSpellId())
                    {
                        case Spells::ShatterEarthNW:
                            l_Orientation = M_PI / 4.f; // 45°
                            break;
                        case Spells::ShatterEarthSW:
                            l_Orientation = M_PI / 4.f * 3.f; // 135°
                            break;
                        case  Spells::ShatterEarthSE:
                            l_Orientation = M_PI / 4.f * 5.f; // 225°
                            break;
                        case Spells::ShatterEarthNE:
                            l_Orientation = M_PI / 4.f * 7.f; // 315°
                            break;
                        default:
                            l_Orientation = 0.f;
                    }
                    
                    p_SourcePosition.m_positionX = p_SourcePosition.GetPositionX() + (cos(l_Orientation) * 8.f);
                    p_SourcePosition.m_positionY = p_SourcePosition.GetPositionY() + (sin(l_Orientation) * 8.f);
                    p_SourcePosition.m_positionZ = p_SourcePosition.GetPositionZ();
                    p_Caster->UpdateGroundPositionZ(p_SourcePosition.m_positionX, p_SourcePosition.m_positionY, p_SourcePosition.m_positionZ);
                    p_SourcePosition.SetOrientation(l_Orientation);

                    p_DestinationPosition.m_positionX = p_SourcePosition.GetPositionX() + (cos(l_Orientation) * 20.f);
                    p_DestinationPosition.m_positionY = p_SourcePosition.GetPositionY() + (sin(l_Orientation) * 20.f);
                    p_DestinationPosition.m_positionZ = p_SourcePosition.GetPositionZ();
                    p_Caster->UpdateGroundPositionZ(p_DestinationPosition.m_positionX, p_DestinationPosition.m_positionY, p_DestinationPosition.m_positionZ);
                    p_DestinationPosition.SetOrientation(l_Orientation);
                }

                void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
                {
                    Unit* l_Caster = p_AreaTrigger->GetCaster();
                    std::list<Unit*> l_TargetList;
                    float l_Radius = 3.f;

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

            class npc_shatter_earth : public CreatureScript
            {
            public:
                npc_shatter_earth() : CreatureScript("npc_shatter_earth")
                {
                }

                enum Spells
                {
                    ShatterEarthDamage  = 149963
                };


                CreatureAI* GetAI(Creature* creature) const
                {
                    return new npc_shatter_earthAI(creature);
                }

                struct npc_shatter_earthAI : public ScriptedAI
                {
                    npc_shatter_earthAI(Creature* p_Creature) : ScriptedAI(p_Creature)
                    {
                    }

                    void Reset()
                    {
                        if (Unit* l_Target = me->SelectNearbyTarget(nullptr, 4.f, (uint32)Spells::ShatterEarthDamage))
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
                        else m_DeathTimer -= p_Diff;
                    }

                    int32 m_DeathTimer;
                };
            };

            class spell_dancing_flames: public SpellScriptLoader
            {
                public:
                    spell_dancing_flames() : SpellScriptLoader("spell_dancing_flames") { }

                    enum Spells
                    {
                        DancingFlames = 149975
                    };

                    class spell_dancing_flames_AuraScript : public AuraScript
                    {
                        PrepareAuraScript(spell_dancing_flames_AuraScript);
                        
                        void HandleApplyEffect(constAuraEffectPtr aurEff, AuraEffectHandleModes mode)
                        {
                            m_Dispelled = false;
                        }

                        void HandleDispel(DispelInfo* dispelInfo)
                        {
                            m_Dispelled = true;
                        }

                        void HandleRemoveEffect(constAuraEffectPtr aurEff, AuraEffectHandleModes mode)
                        {
                            if (m_Dispelled)
                                return;

                            if (Unit* l_Caster = GetCaster())
                                l_Caster->CastSpell(l_Caster, (uint32)Spells::DancingFlames, true);
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

                        void OnPeriodic(constAuraEffectPtr aurEff)
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

            class spell_molten_impact: public SpellScriptLoader
            {
                public:
                    spell_molten_impact() : SpellScriptLoader("spell_molten_impact") { }

                    class spell_molten_impact_SpellScript : public SpellScript
                    {
                        PrepareSpellScript(spell_molten_impact_SpellScript);

                        void HandleScript(SpellEffIndex effIndex)
                        {
                            SetHitDamage((float)GetHitDamage() * (30.f - std::min((float)GetHitDest()->GetExactDist(GetHitUnit()), 30.f)) / 30.f);
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
                areatrigger_volcanic_trantum() : AreaTriggerEntityScript("areatrigger_volcanic_trantum")
                {
                }
        
                void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition)
                {
                    if (!p_Caster)
                        return;

                    float l_Orientation;
                    switch ((Spells)p_AreaTrigger->GetSpellId())
                    {
                        case Spells::VolcanicTrantrumNW:
                            l_Orientation = M_PI / 4.f; // 45°
                            break;
                        case Spells::VolcanicTrantrumSW:
                            l_Orientation = M_PI / 4.f * 3.f; // 135°
                            break;
                        case Spells::VolcanicTrantrumSE:
                            l_Orientation = M_PI / 4.f * 5.f; // 225°
                            break;
                        case Spells::VolcanicTrantrumNE:
                            l_Orientation = M_PI / 4.f * 7.f; // 315°
                            break;
                        default:
                            l_Orientation = 0.f;
                    }
                    
                    p_SourcePosition.m_positionX = p_Caster->GetPositionX() + (cos(l_Orientation) * 8.f);
                    p_SourcePosition.m_positionY = p_Caster->GetPositionY() + (sin(l_Orientation) * 8.f);
                    p_SourcePosition.m_positionZ = p_Caster->GetPositionZ();
                    p_Caster->UpdateGroundPositionZ(p_SourcePosition.m_positionX, p_SourcePosition.m_positionY, p_SourcePosition.m_positionZ);
                    p_SourcePosition.SetOrientation(l_Orientation);

                    p_DestinationPosition.m_positionX = p_SourcePosition.GetPositionX() + (cos(l_Orientation) * 20.f);
                    p_DestinationPosition.m_positionY = p_SourcePosition.GetPositionY() + (sin(l_Orientation) * 20.f);
                    p_DestinationPosition.m_positionZ = p_SourcePosition.GetPositionZ();
                    p_Caster->UpdateGroundPositionZ(p_DestinationPosition.m_positionX, p_DestinationPosition.m_positionY, p_DestinationPosition.m_positionZ);
                    p_DestinationPosition.SetOrientation(l_Orientation);
                }

                void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
                {
                    Unit* l_Caster = p_AreaTrigger->GetCaster();
                    Unit* l_Target = nullptr;
                    std::list<Unit*> l_TargetList;
                    float l_Radius = 3.f;

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

            class npc_gugdoh_molten_elemental : public CreatureScript
            {
            public:
                npc_gugdoh_molten_elemental() : CreatureScript("npc_gugdoh_molten_elemental")
                {
                }

                enum Spells
                {
                    MagmaBarrage    = 150004,
                    VolcanicTantrum = 150048
                };

                enum Events
                {
                    CastSpell = 1
                };

                CreatureAI* GetAI(Creature* creature) const
                {
                    return new npc_gugdoh_molten_elementalAI(creature);
                }

                struct npc_gugdoh_molten_elementalAI : public ScriptedAI
                {
                    npc_gugdoh_molten_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature)
                    {
                    }

                    void Reset()
                    {
                        events.Reset();
                        events.ScheduleEvent((uint32)Events::CastSpell, 100);
                    }

                    void UpdateAI(const uint32 diff)
                    {
                        if (!UpdateVictim())
                            return;
                            
                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            return;

                        events.Update(diff);

                        if ((Events)events.ExecuteEvent() == Events::CastSpell)
                        {
                            me->CastSpell(me, uint32(urand(0, 1) ? Spells::VolcanicTantrum : Spells::MagmaBarrage), false);
                            events.ScheduleEvent((uint32)Events::CastSpell, 5000);
                        }

                        DoMeleeAttackIfReady();
                    }
                };
            };
        }
    }
}

void AddSC_boss_forgemaster_gogduh()
{
    new MS::Instances::Bloodmaul::boss_forgemaster_gogduh();
    new MS::Instances::Bloodmaul::npc_ruination();
    new MS::Instances::Bloodmaul::npc_calamity_firestorm();
    new MS::Instances::Bloodmaul::npc_calamity();
    new MS::Instances::Bloodmaul::boss_magmolatus();
    new MS::Instances::Bloodmaul::spell_magma_barrage();
    new MS::Instances::Bloodmaul::spell_rough_smash();
    new MS::Instances::Bloodmaul::spell_shatter_earth();
    new MS::Instances::Bloodmaul::areatrigger_shatter_earth();
    new MS::Instances::Bloodmaul::npc_shatter_earth();
    new MS::Instances::Bloodmaul::spell_dancing_flames();
    new MS::Instances::Bloodmaul::spell_withering_flames();
    new MS::Instances::Bloodmaul::spell_molten_impact();
    new MS::Instances::Bloodmaul::areatrigger_volcanic_trantum();
    new MS::Instances::Bloodmaul::npc_gugdoh_molten_elemental();
}