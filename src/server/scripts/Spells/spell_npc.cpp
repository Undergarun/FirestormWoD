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
#include "ScriptedCreature.h"
#include "Containers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "Vehicle.h"
#include "VMapFactory.h"
#include "TotemAI.h"

/// Prismatic Crystal - 76933
class spell_npc_mage_prismatic_crystal : public CreatureScript
{
    public:
        spell_npc_mage_prismatic_crystal() : CreatureScript("npc_mage_prismatic_crystal") { }

        enum eSpells
        {
            PrismaticCrystalAura    = 155153,
            PrismaticCrystalDamage  = 155152
        };

        struct spell_npc_mage_prismatic_crystalAI : public ScriptedAI
        {
            spell_npc_mage_prismatic_crystalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eDatas
            {
                FactionHostile = 14,
                FactionFriend  = 35
            };

            uint64 m_Owner = 0;

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Owner = p_Summoner->GetGUID();

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                if (Player* l_Player = p_Summoner->ToPlayer())
                {
                    if (AuraPtr l_Aura = me->AddAura(eSpells::PrismaticCrystalAura, me))
                    {
                        if (AuraEffectPtr l_DamageTaken = l_Aura->GetEffect(SpellEffIndex::EFFECT_0))
                        {
                            if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SpecIndex::SPEC_MAGE_FROST)
                                l_DamageTaken->ChangeAmount(10);    ///< BasePoint = 30, but only for Arcane and Fire spec
                        }
                    }
                }

                me->setFaction(eDatas::FactionHostile);
                me->ForceValuesUpdateAtIndex(EUnitFields::UNIT_FIELD_FACTION_TEMPLATE);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (p_Attacker->GetGUID() != m_Owner)
                {
                    p_Damage = 0;
                    return;
                }

                if (p_SpellInfo && p_SpellInfo->Id != eSpells::PrismaticCrystalDamage)
                {
                    if (Unit* l_Owner = sObjectAccessor->FindUnit(m_Owner))
                    {
                        int32 l_BasePoint = p_Damage;
                        l_Owner->CastCustomSpell(me, eSpells::PrismaticCrystalDamage, nullptr, &l_BasePoint, nullptr, true);
                    }
                }
            }

            void OnSendFactionTemplate(uint32& p_FactionID, Player* p_Target)
            {
                if (m_Owner == p_Target->GetGUID())
                    p_FactionID = eDatas::FactionHostile;
                else
                    p_FactionID = eDatas::FactionFriend;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_mage_prismatic_crystalAI(p_Creature);
        }
};

/// Frozen Orb - 45322
class spell_npc_mage_frozen_orb : public CreatureScript
{
    public:
        spell_npc_mage_frozen_orb() : CreatureScript("spell_npc_mage_frozen_orb") { }

        enum Constants
        {
            CheckDist          = 5,                   ///< Every AI update, the orb will try to travel this distance
            DamageDelay        = 1 * IN_MILLISECONDS, ///< Delay between damage cast (and self-snare check)
            HeightMaxStep      = 3,                   ///< Maximum step height the orb can go before stopping (this value goes along with CheckDist)
            HoverHeight        = 0,                   ///< "Display" height modification (some modelid are centered at the origin),
            HeightCompensation = -1                   ///< Looks like the orb is rising by itself, let's compensate (hackfix though)
        };

        enum Spells
        {
            FingersOfFrostVisual    = 126084,
            FingersOfFrost          = 44544,
            FrozenOrbVisual         = 123605,
            SelfSnare90Pct          = 82736,
            TargetSnareAndDamage    = 84721,
            T17Frost2P              = 165470
        };

        enum eEvent
        {
            EventFingerOfFrost = 1
        };

        struct spell_npc_mage_frozen_orbAI : public ScriptedAI
        {
            uint32 m_DamageTimer;
            bool m_KeepMoving;

            spell_npc_mage_frozen_orbAI(Creature* creature) : ScriptedAI(creature)
            {
                m_DamageTimer = Constants::DamageDelay; ///< As we want the damage to proc on summon
                m_KeepMoving = true;
            }

            void EnterEvadeMode() override
            {
                ///< No evade mode for orbs
            }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddAura(Spells::FrozenOrbVisual, me);
                me->SetCanFly(true);

                /// Give it a movement
                UpdateMovement();

                if (Unit* l_Owner = me->GetOwner())
                {
                    if (AuraEffectPtr l_AurEff = l_Owner->GetAuraEffect(Spells::T17Frost2P, EFFECT_0))
                        events.ScheduleEvent(eEvent::EventFingerOfFrost, l_AurEff->GetAmount());
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (events.ExecuteEvent() == eEvent::EventFingerOfFrost)
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        l_Owner->CastSpell(l_Owner, Spells::FingersOfFrostVisual, true);
                        l_Owner->CastSpell(l_Owner, Spells::FingersOfFrost, true);

                        if (AuraEffectPtr l_AurEff = l_Owner->GetAuraEffect(Spells::T17Frost2P, EFFECT_0))
                            events.ScheduleEvent(eEvent::EventFingerOfFrost, l_AurEff->GetAmount());
                    }
                }

                m_DamageTimer += p_Diff;

                if (m_DamageTimer > Constants::DamageDelay)
                {
                    /// Frozen Orb slows down when it damages an enemy
                    if (!me->HasAura(Spells::SelfSnare90Pct))
                    {
                        const float l_MaxRadius = 10.f; ///< Spell radius

                        /// Find all the enemies in range
                        std::list<Unit*> l_Targets;

                        JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(me, me, l_MaxRadius);
                        JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(me, l_Targets, l_Check);
                        me->VisitNearbyObject(l_MaxRadius, l_Searcher);

                        for (Unit* l_Target : l_Targets)
                        {
                            if (l_Target->isAlive() && me->GetExactDistSq(l_Target) < l_MaxRadius * l_MaxRadius && me->IsWithinLOSInMap(l_Target) && me->IsValidAttackTarget(l_Target))
                            {
                                me->AddAura(Spells::SelfSnare90Pct, me);

                                /// Frozen Orb gives one stack of FoF on first hit
                                if (Unit* l_Owner = me->GetOwner())
                                {
                                    if (l_Owner->HasAura(Spells::FingersOfFrost))
                                        l_Owner->CastSpell(l_Owner, Spells::FingersOfFrostVisual, true); ///< Fingers of frost visual
                                    l_Owner->CastSpell(l_Owner, Spells::FingersOfFrost, true);  ///< Fingers of frost proc
                                }
                                break;
                            }
                        }
                    }

                    Unit* l_DamageCaster = me;
                    if (Unit* l_Owner = me->GetOwner())
                        l_DamageCaster = l_Owner;

                    l_DamageCaster->CastSpell(me, TargetSnareAndDamage, true);

                    m_DamageTimer -= Constants::DamageDelay;
                }

                /// Keep updating movement
                UpdateMovement();
            }

            void UpdateMovement()
            {
                if (!m_KeepMoving)
                    return;

                const float l_CheckDist = float(Constants::CheckDist);
                const float l_MaxStep = float(Constants::HeightMaxStep);

                float l_Rotation = me->GetOrientation();
                float l_RotCos = std::cos(l_Rotation);
                float l_RotSin = std::sin(l_Rotation);

                Position l_Origin = *me;
                Position l_Dest(l_Origin);
                l_Dest.m_positionX += l_CheckDist * l_RotCos;
                l_Dest.m_positionY += l_CheckDist * l_RotSin;
                l_Dest.m_positionZ += Constants::HeightCompensation - Constants::HoverHeight;

                float l_DestHeight = std::max(l_Dest.GetPositionZ(), me->GetMap()->GetHeight(l_Dest.GetPositionX(), l_Dest.GetPositionY(), MAX_HEIGHT));
                float l_Diff = l_DestHeight - l_Dest.m_positionZ + 1.f; ///< +1 because reasons (I have no idea why this is required, but it is)

                if (l_Diff > l_MaxStep)
                {
                    float l_Step = l_CheckDist / 10.f;

                    for (uint32 l_I = 0; l_I < 10; ++l_I)
                    {
                        l_Dest.m_positionX -= l_Step * l_RotCos;
                        l_Dest.m_positionY -= l_Step * l_RotSin;

                        l_DestHeight = me->GetMap()->GetHeight(l_Dest.m_positionX, l_Dest.m_positionY, MAX_HEIGHT);
                        l_Diff = l_DestHeight - l_Dest.m_positionZ + 1.f;
                        if (l_Diff < l_MaxStep)
                            break;
                    }

                    m_KeepMoving = false;
                }

                l_Dest.m_positionZ = l_DestHeight + Constants::HoverHeight;

                /// Let's cast some rays to see if there's an obstacle in front of us
                Position l_StaticHit;
                VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(me->GetMapId(),
                    l_Origin.m_positionX, l_Origin.m_positionY, l_Origin.m_positionZ,
                    l_Dest.m_positionX, l_Dest.m_positionY, l_Dest.m_positionZ,
                    l_StaticHit.m_positionX, l_StaticHit.m_positionY, l_StaticHit.m_positionZ, 0.f);

                Position l_DynamicHit;
                me->GetMap()->getObjectHitPos(me->GetPhaseMask(),
                    l_Origin.m_positionX, l_Origin.m_positionY, l_Origin.m_positionZ,
                    l_Dest.m_positionX, l_Dest.m_positionY, l_Dest.m_positionZ,
                    l_DynamicHit.m_positionX, l_DynamicHit.m_positionY, l_DynamicHit.m_positionZ, 0.f);

                /// Get the closer hit pos (for obvious reasons)
                Position l_FinalPos;
                if (l_Origin.GetExactDistSq(&l_StaticHit) < l_Origin.GetExactDistSq(&l_DynamicHit))
                    l_FinalPos = l_StaticHit;
                else
                    l_FinalPos = l_DynamicHit;

                me->GetMotionMaster()->MovePoint(0, l_FinalPos);

                /// If we hit something, stop moving
                if (l_Dest.GetExactDistSq(&l_FinalPos) > 0.1f)
                    m_KeepMoving = false;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_mage_frozen_orbAI(creature);
        }
};

/// Shadow Reflection - 77726
class spell_npc_rogue_shadow_reflection : public CreatureScript
{
    public:
        spell_npc_rogue_shadow_reflection() : CreatureScript("npc_rogue_shadow_reflection") { }

        struct spell_npc_rogue_shadow_reflectionAI : public ScriptedAI
        {
            spell_npc_rogue_shadow_reflectionAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Queuing(true) { }

            enum eSpells
            {
                SpellShadowReflectionAura = 156744,
                ShadowReflectionClone     = 168691
            };

            enum eDatas
            {
                AddSpellToQueue = 0,
                FinishFirstPhase
            };

            struct SpellData
            {
                SpellData(uint32 p_ID, uint32 p_Time)
                {
                    ID = p_ID;
                    Time = p_Time;
                }

                uint32 ID;
                uint32 Time;
            };

            bool m_Queuing;

            /// Automatically stored in good time order
            std::queue<SpellData> m_SpellQueue;

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->CastSpell(me, eSpells::SpellShadowReflectionAura, true);
            }

            void SetGUID(uint64 p_Data, int32 p_ID)
            {
                switch (p_ID)
                {
                    case eDatas::AddSpellToQueue:
                    {
                        if (!m_Queuing)
                            break;

                        uint32 l_SpellID = ((uint32*)(&p_Data))[0];
                        uint32 l_Time = ((uint32*)(&p_Data))[1];
                        m_SpellQueue.push(SpellData(l_SpellID, l_Time));
                        break;
                    }
                    case eDatas::FinishFirstPhase:
                    {
                        if (!m_Queuing || me->ToTempSummon() == nullptr)
                            break;

                        m_Queuing = false;
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                        if (Unit* l_Owner = me->ToTempSummon()->GetOwner())
                        {
                            Unit* l_OwnerTarget = nullptr;
                            if (Player* l_Player = l_Owner->ToPlayer())
                                l_OwnerTarget = l_Player->GetSelectedUnit();
                            else
                                l_OwnerTarget = l_Owner->getVictim();

                            if (l_OwnerTarget)
                                AttackStart(l_OwnerTarget);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::ShadowReflectionClone && p_Caster != nullptr && p_Caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    me->SetMaxPower(Powers::POWER_COMBO_POINT, 5);
                    me->SetPower(Powers::POWER_COMBO_POINT, p_Caster->GetPower(Powers::POWER_COMBO_POINT));

                    for (uint8 l_AttType = 0; l_AttType < WeaponAttackType::MaxAttack; ++l_AttType)
                    {
                        me->SetBaseWeaponDamage((WeaponAttackType)l_AttType, MAXDAMAGE, p_Caster->GetWeaponDamageRange((WeaponAttackType)l_AttType, MAXDAMAGE));
                        me->SetBaseWeaponDamage((WeaponAttackType)l_AttType, MINDAMAGE, p_Caster->GetWeaponDamageRange((WeaponAttackType)l_AttType, MINDAMAGE));
                    }

                    me->UpdateAttackPowerAndDamage();
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_Queuing)
                    return;

                if (!UpdateVictim())
                    return;

                if (SpellData* l_SpellData = GetNextSpellData())
                {
                    if (l_SpellData->Time <= p_Diff)
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, l_SpellData->ID, true);

                        m_SpellQueue.pop();
                    }
                    else
                        l_SpellData->Time -= p_Diff;
                }

                DoMeleeAttackIfReady();
            }

            SpellData* GetNextSpellData()
            {
                if (m_SpellQueue.empty())
                    return nullptr;

                return &m_SpellQueue.front();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_rogue_shadow_reflectionAI(p_Creature);
        }
};

/// Capacitor Totem - 61245
class spell_npc_sha_capacitor_totem : public CreatureScript
{
    public:
        spell_npc_sha_capacitor_totem() : CreatureScript("npc_capacitor_totem") { }

        enum eSpells
        {
            StaticCharge = 118905
        };

        struct spell_npc_sha_capacitor_totemAI : public TotemAI
        {
            bool m_HasBeenCasted;

            spell_npc_sha_capacitor_totemAI(Creature* p_Creature) : TotemAI(p_Creature)
            {
                m_HasBeenCasted = false;
            }

            void Reset()
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void UpdateAI(uint32 const /*p_Diff*/)
            {
                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (!me->HasAura(eSpells::StaticCharge) && !m_HasBeenCasted)
                {
                    me->CastSpell(me, eSpells::StaticCharge, false);
                    m_HasBeenCasted = true;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_capacitor_totemAI(p_Creature);
        }
};

/// Spirit Link Totem - 53006
class spell_npc_sha_spirit_link_totem : public CreatureScript
{
    public:
        spell_npc_sha_spirit_link_totem() : CreatureScript("npc_spirit_link_totem") { }

        struct spell_npc_sha_spirit_link_totemAI : public TotemAI
        {
            uint32 CastTimer;

            spell_npc_sha_spirit_link_totemAI(Creature* creature) : TotemAI(creature)
            {
                CastTimer = 1000;
            }

            void Reset()
            {
                if (me->GetOwner() && me->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                {
                    me->CastSpell(me, 98007, false);
                    me->CastSpell(me, 98017, true);
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (CastTimer >= diff)
                {
                    if (me->GetOwner() && me->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (me->GetEntry() == 53006)
                        {
                            me->CastSpell(me, 98007, false);
                            me->CastSpell(me, 98017, true);
                        }
                    }
                }

                CastTimer = 0;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_sha_spirit_link_totemAI(creature);
        }
};

/// Greater Storm Elemental - 77936
/// Primal Storm Elemental - 77942
class spell_npc_sha_storm_elemental : public CreatureScript
{
    public:
        spell_npc_sha_storm_elemental() : CreatureScript("npc_storm_elemental") { }

        enum eSpells
        {
            SpellWindGust       = 157331,
            SpellCallLightning  = 157348
        };

        struct spell_npc_sha_storm_elementalAI : public ScriptedAI
        {
            spell_npc_sha_storm_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eEvents
            {
                EventWindGust = 1,
                EventCallLightning
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(eEvents::EventWindGust, 500);
                m_Events.ScheduleEvent(eEvents::EventCallLightning, 8000);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        Unit* l_OwnerTarget = NULL;
                        if (Player* l_Plr = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Plr->GetSelectedUnit();
                        else
                            l_OwnerTarget = l_Owner->getVictim();

                        if (l_OwnerTarget && me->isTargetableForAttack(l_OwnerTarget) && !l_Owner->IsFriendlyTo(l_OwnerTarget))
                            AttackStart(l_OwnerTarget);
                    }

                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventWindGust:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellWindGust, false);
                        m_Events.ScheduleEvent(eEvents::EventWindGust, 500);
                        break;
                    case eEvents::EventCallLightning:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellCallLightning, false);
                        m_Events.ScheduleEvent(eEvents::EventCallLightning, 15000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_storm_elementalAI(p_Creature);
        }
};

/// Greater Fire Elemental - 15438
/// Primal Fire Elemental - 61029
class spell_npc_sha_fire_elemental : public CreatureScript
{
    public:
        spell_npc_sha_fire_elemental() : CreatureScript("npc_fire_elemental") { }

        struct spell_npc_sha_fire_elementalAI : public ScriptedAI
        {
            spell_npc_sha_fire_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) {}

            enum fireEvents
            {
                EVENT_FIRE_NOVA     = 1,
                EVENT_FIRE_BLAST    = 2,
                EVENT_FIRE_SHIELD   = 3
            };

            enum fireSpells
            {
                SPELL_SHAMAN_FIRE_BLAST     = 57984,
                SPELL_SHAMAN_FIRE_NOVA      = 117588,
                SPELL_SHAMAN_FIRE_SHIELD    = 13376
            };

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_FIRE_NOVA, urand(5000, 20000));
                events.ScheduleEvent(EVENT_FIRE_BLAST, urand(5000, 20000));
                events.ScheduleEvent(EVENT_FIRE_SHIELD, 0);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        Unit* l_OwnerTarget = NULL;
                        if (Player* l_Plr = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Plr->GetSelectedUnit();
                        else
                            l_OwnerTarget = l_Owner->getVictim();

                        if (l_OwnerTarget && me->isTargetableForAttack(l_OwnerTarget) && !l_Owner->IsFriendlyTo(l_OwnerTarget))
                            AttackStart(l_OwnerTarget);
                    }

                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FIRE_NOVA:
                        DoCastVictim(SPELL_SHAMAN_FIRE_NOVA);
                        events.ScheduleEvent(EVENT_FIRE_NOVA, urand(5000, 20000));
                        break;
                    case EVENT_FIRE_BLAST:
                        DoCastVictim(SPELL_SHAMAN_FIRE_BLAST);
                        events.ScheduleEvent(EVENT_FIRE_BLAST, urand(5000, 20000));
                        break;
                    case EVENT_FIRE_SHIELD:
                        DoCastVictim(SPELL_SHAMAN_FIRE_SHIELD);
                        events.ScheduleEvent(EVENT_FIRE_SHIELD, 4000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI *GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_fire_elementalAI(p_Creature);
        }
};

/// Greater Earth Elemental - 15352
/// Primal Earth Elemental - 61056
class spell_npc_sha_earth_elemental : public CreatureScript
{
    public:
        spell_npc_sha_earth_elemental() : CreatureScript("npc_earth_elemental") { }

        enum eSpells
        {
            AngeredEarth = 36213
        };

        struct spell_npc_sha_earth_elementalAI : public ScriptedAI
        {
            uint32 AngeredEarth_Timer;

            spell_npc_sha_earth_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                AngeredEarth_Timer = 0;
            }

            void Reset()
            {
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        Unit* l_OwnerTarget = NULL;
                        if (Player* l_Plr = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Plr->GetSelectedUnit();
                        else
                            l_OwnerTarget = l_Owner->getVictim();

                        if (l_OwnerTarget && me->isTargetableForAttack(l_OwnerTarget) && !l_Owner->IsFriendlyTo(l_OwnerTarget))
                            AttackStart(l_OwnerTarget);
                    }

                    return;
                }

                if (AngeredEarth_Timer <= diff)
                {
                    DoCast(me->getVictim(), eSpells::AngeredEarth);
                    AngeredEarth_Timer = 5000 + rand() % 15000; // 5-20 sec cd
                }
                else
                    AngeredEarth_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_earth_elementalAI(p_Creature);
        }
};

/// Spirit Wolf - 29264
class spell_npc_sha_feral_spirit : public CreatureScript
{
    public:
        spell_npc_sha_feral_spirit() : CreatureScript("npc_feral_spirit") { }

        enum eSpells
        {
            SpiritLeap                  = 58867,
            SpiritWalk                  = 58875,
            SpiritHunt                  = 58877,
            GlyphOfSpiritRaptors        = 147783,
            RaptorTranform              = 147908,
            FeralSpiritWindfuryDriver   = 170523,
            T17Enhancement4P            = 165610,
            WindfuryAttack              = 170512
        };

        enum eAction
        {
            ActionWindfury
        };

        struct spell_npc_sha_feral_spiritAI : public ScriptedAI
        {
            spell_npc_sha_feral_spiritAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_WindFuryCooldown(0) { }

            uint32 m_WindFuryCooldown;

            void Reset() override
            {
                if (Unit* l_Owner = me->GetOwner())
                {
                    if (l_Owner->HasAura(eSpells::GlyphOfSpiritRaptors))
                        me->CastSpell(me, eSpells::RaptorTranform, true);

                    /// While Feral Spirits is active, [...] your wolves can proc Windfury.
                    if (l_Owner->HasAura(eSpells::T17Enhancement4P))
                        me->CastSpell(me, eSpells::FeralSpiritWindfuryDriver, true);
                }

                me->CastSpell(me, eSpells::SpiritWalk, true);
                me->CastSpell(me, eSpells::SpiritHunt, true);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->CastSpell(p_Attacker, eSpells::SpiritLeap, true);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionWindfury && !m_WindFuryCooldown)
                {
                    if (Unit* l_Target = me->getVictim())
                    {
                        m_WindFuryCooldown = 5 * TimeConstants::IN_MILLISECONDS;

                        uint8 l_Count = 3;

                        for (uint8 l_I = 0; l_I < l_Count; l_I++)
                            me->CastSpell(l_Target, eSpells::WindfuryAttack, true);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_WindFuryCooldown > 0 && m_WindFuryCooldown > p_Diff)
                    m_WindFuryCooldown -= p_Diff;
                else if (m_WindFuryCooldown > 0 && p_Diff > m_WindFuryCooldown)
                    m_WindFuryCooldown = 0;

                if (!UpdateVictim())
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        Unit* l_OwnerTarget = nullptr;
                        if (Player* l_Player = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Player->GetSelectedUnit();
                        else
                            l_OwnerTarget = l_Owner->getVictim();

                        if (l_OwnerTarget)
                            AttackStart(l_OwnerTarget);
                    }

                    return;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_feral_spiritAI(p_Creature);
        }
};

class spell_npc_sha_healing_rain : public CreatureScript
{
    public:
        spell_npc_sha_healing_rain() : CreatureScript("spell_npc_sha_healing_rain") { }

        struct spell_npc_sha_healing_rainAI : public CreatureAI
        {
            enum eSpells : uint32
            {
                HealingRainVisual = 147490
            };

            spell_npc_sha_healing_rainAI(Creature* p_Creature) : CreatureAI(p_Creature)
            {
                me->CastSpell(me, eSpells::HealingRainVisual, true);
            }

            void UpdateAI(uint32 const /*p_Diff*/)
            {
                if (!me->HasAura(eSpells::HealingRainVisual))
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_healing_rainAI(p_Creature);
        }
};

/// Ravager - 76168
class spell_npc_warr_ravager : public CreatureScript
{
    public:
        spell_npc_warr_ravager() : CreatureScript("npc_warrior_ravager") { }

        enum eDatas
        {
            DisplayID = 55644,
            RavagerAura = 153709
        };

        struct spell_npc_warr_ravagerAI : public ScriptedAI
        {
            spell_npc_warr_ravagerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void IsSummonedBy(Unit* p_Summoner)
            {
                me->SetDisplayId(eDatas::DisplayID);
                me->CastSpell(me, eDatas::RavagerAura, true);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE |
                                                           eUnitFlags::UNIT_FLAG_UNK_15 |
                                                           eUnitFlags::UNIT_FLAG_PVP_ATTACKABLE);

                if (p_Summoner == nullptr || p_Summoner->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (Player* l_Player = p_Summoner->ToPlayer())
                {
                    if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND))
                    {
                        /// Display Transmogrifications on player's clone
                        if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->GetDynamicValue(EItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS, 0)))
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEMS, l_Proto->ItemId);
                        else
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEMS, l_Item->GetTemplate()->ItemId);
                    }

                    if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND))
                    {
                        /// Display Transmogrifications on player's clone
                        if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->GetDynamicValue(EItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS, 0)))
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEMS + 2, l_Proto->ItemId);
                        else
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEMS + 2, l_Item->GetTemplate()->ItemId);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warr_ravagerAI(p_Creature);
        }
};

/// Mocking Banner - 59390
class spell_npc_warr_mocking_banner : public CreatureScript
{
    public:
        spell_npc_warr_mocking_banner() : CreatureScript("spell_npc_warr_mocking_banner") { }

        struct spell_npc_warr_mocking_bannerAI : public ScriptedAI
        {
            spell_npc_warr_mocking_bannerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warr_mocking_bannerAI(p_Creature);
        }
};

/// Wild Imp - 55659
class spell_npc_warl_wild_imp : public CreatureScript
{
    public:
        spell_npc_warl_wild_imp() : CreatureScript("npc_wild_imp") { }

        enum eSpells
        {
            Firebolt = 104318,
            MoltenCore = 122351,
            MoltenCoreAura = 122355
        };

        struct spell_npc_warl_wild_impAI : public ScriptedAI
        {
            uint32 m_Charges;

            spell_npc_warl_wild_impAI(Creature *creature) : ScriptedAI(creature)
            {
                m_Charges = 10;
            }

            void Reset()
            {
                me->SetReactState(REACT_HELPER);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (Unit* l_Owner = me->GetOwner())
                {
                    Unit* l_OwnerTarget = l_Owner->getVictim();

                    if (l_OwnerTarget == nullptr)
                        if (Player* l_Player = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Player->GetSelectedUnit();

                    if (l_OwnerTarget && me->isTargetableForAttack(l_OwnerTarget) && (!me->getVictim() || me->getVictim() != l_OwnerTarget))
                        AttackStart(l_OwnerTarget);
                }

                if (!me->getVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (m_Charges == 0)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                me->CastSpell(me->getVictim(), eSpells::Firebolt, false);
                m_Charges--;

                if (Unit* l_Owner = me->GetOwner())
                {
                    me->EnergizeBySpell(l_Owner, eSpells::Firebolt, 5 * l_Owner->GetPowerCoeff(POWER_DEMONIC_FURY), POWER_DEMONIC_FURY);

                    if (AuraEffectPtr l_MoltenCore = l_Owner->GetAuraEffect(eSpells::MoltenCore, EFFECT_0))
                        if (roll_chance_i(l_MoltenCore->GetAmount()))
                            l_Owner->CastSpell(l_Owner, eSpells::MoltenCoreAura, true);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_warl_wild_impAI(creature);
        }
};

/// Last Update 6.2.3
/// Doomguard - 78158
class spell_npc_warl_doomguard: public CreatureScript
{
    public:
        spell_npc_warl_doomguard() : CreatureScript("npc_doomguard") { }

        enum eSpells
        {
            DoomBolt = 85692
        };

        struct spell_npc_warl_doomguardAI : public ScriptedAI
        {
            spell_npc_warl_doomguardAI(Creature *creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetPower(Powers::POWER_ENERGY, me->GetMaxPower(Powers::POWER_ENERGY));
                me->SetReactState(REACT_HELPER);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (Unit* l_Owner = me->GetOwner())
                {
                    Unit* l_OwnerTarget = l_Owner->getVictim();

                    if (l_OwnerTarget == nullptr)
                        if (Player* l_Player = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Player->GetSelectedUnit();

                    if (l_OwnerTarget && me->isTargetableForAttack(l_OwnerTarget) && (!me->getVictim() || me->getVictim() != l_OwnerTarget))
                        AttackStart(l_OwnerTarget);
                }

                if (!me->getVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                me->CastSpell(me->getVictim(), eSpells::DoomBolt, false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_warl_doomguardAI(creature);
        }
};

enum eGatewaySpells
{
    PortalVisual = 113900,
    GatewayInteract = 113902,
    CooldownMarker = 113942
};

enum eGatewayNpc
{
    GreenGate = 59262,
    PurpleGate = 59271
};

/// npc_demonic_gateway_purple - 59271
class spell_npc_warl_demonic_gateway_purple : public CreatureScript
{
    public:
        spell_npc_warl_demonic_gateway_purple() : CreatureScript("npc_demonic_gateway_purple") { }

        struct spell_npc_warl_demonic_gateway_purpleAI : public ScriptedAI
        {
            spell_npc_warl_demonic_gateway_purpleAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, eGatewaySpells::PortalVisual, true);

                me->SetFlag(UNIT_FIELD_INTERACT_SPELL_ID, eGatewaySpells::GatewayInteract);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void OnSpellClick(Unit* p_Clicker)
            {
                if (p_Clicker->GetTypeId() != TYPEID_PLAYER)
                    return;

                // Demonic Gateway cooldown marker
                if (p_Clicker->HasAura(eGatewaySpells::CooldownMarker))
                    return;

                /// Can't use gates in control
                if (p_Clicker->isFeared() || p_Clicker->isInStun() || p_Clicker->isConfused())
                    return;

                Unit* l_Owner = me->GetOwner();
                if (!l_Owner || !l_Owner->ToPlayer())
                    return;

                if (Group* l_Group = p_Clicker->ToPlayer()->GetGroup())
                {
                    if (l_Owner->ToPlayer()->GetGroup() != l_Group)
                        return;
                }
                else if (l_Owner != p_Clicker)
                    return;

                std::list<Creature*> l_GreenGates;
                me->GetCreatureListWithEntryInGrid(l_GreenGates, eGatewayNpc::GreenGate, 75.0f);

                if (l_GreenGates.empty())
                    return;

                l_GreenGates.sort(JadeCore::DistanceCompareOrderPred(me));
                for (auto itr : l_GreenGates)
                {
                    p_Clicker->CastSpell(p_Clicker, eGatewaySpells::CooldownMarker, true);

                    Unit* l_CurrentGateOwner = itr->GetOwner();

                    /// Can't teleport to other players Gates
                    if (l_CurrentGateOwner && l_Owner != l_CurrentGateOwner)
                        continue;

                    // Init dest coordinates
                    float x, y, z;
                    itr->GetPosition(x, y, z);

                    float speedXY;
                    float speedZ = 5;

                    speedXY = p_Clicker->GetExactDist2d(x, y) * 10.0f / speedZ;
                    p_Clicker->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warl_demonic_gateway_purpleAI(p_Creature);
        }
};

/// npc_demonic_gateway_green - 59262
class spell_npc_warl_demonic_gateway_green : public CreatureScript
{
    public:
        spell_npc_warl_demonic_gateway_green() : CreatureScript("npc_demonic_gateway_green") { }

        struct spell_npc_warl_demonic_gateway_greenAI : public ScriptedAI
        {
            spell_npc_warl_demonic_gateway_greenAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, eGatewaySpells::PortalVisual, true);

                me->SetFlag(UNIT_FIELD_INTERACT_SPELL_ID, eGatewaySpells::GatewayInteract);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void OnSpellClick(Unit* p_Clicker)
            {
                if (p_Clicker->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_Clicker->HasAura(eGatewaySpells::CooldownMarker))
                    return;

                /// Can't use gates in control
                if (p_Clicker->isFeared() || p_Clicker->isInStun() || p_Clicker->isConfused())
                    return;

                Unit* l_Owner = me->GetOwner();
                if (!l_Owner || !l_Owner->ToPlayer())
                    return;

                if (Group* l_Group = p_Clicker->ToPlayer()->GetGroup())
                {
                    if (l_Owner->ToPlayer()->GetGroup() != l_Group)
                        return;
                }
                else if (l_Owner != p_Clicker)
                    return;

                std::list<Creature*> l_PurpleGates;
                me->GetCreatureListWithEntryInGrid(l_PurpleGates, eGatewayNpc::PurpleGate, 75.0f);

                if (l_PurpleGates.empty())
                    return;

                l_PurpleGates.sort(JadeCore::DistanceCompareOrderPred(me));
                for (auto itr : l_PurpleGates)
                {
                    Unit* l_CurrentGateOwner = itr->GetOwner();

                    /// Can't teleport to other players Gates
                    if (l_CurrentGateOwner && l_Owner != l_CurrentGateOwner)
                        continue;

                    p_Clicker->CastSpell(p_Clicker, eGatewaySpells::CooldownMarker, true);

                    // Init dest coordinates
                    float x, y, z;
                    itr->GetPosition(x, y, z);

                    float speedXY;
                    float speedZ = 5;

                    speedXY = p_Clicker->GetExactDist2d(x, y) * 10.0f / speedZ;
                    p_Clicker->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warl_demonic_gateway_greenAI(p_Creature);
        }
};

/// Inner Demon - 82927
/// Spawned by Inner Demon - 166862
class spell_npc_warl_inner_demon : public CreatureScript
{
    public:
        spell_npc_warl_inner_demon() : CreatureScript("spell_npc_warl_inner_demon") { }

        struct spell_npc_warl_inner_demonAI : public ScriptedAI
        {
            spell_npc_warl_inner_demonAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                InnerDemonAura  = 181608,
                SoulFire        = 166864
            };

            enum eEvent
            {
                EventSoulFire = 1
            };

            EventMap m_Events;

            void Reset() override
            {
                if (Unit* l_Owner = me->GetOwner())
                    l_Owner->CastSpell(me, eSpells::InnerDemonAura, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(eEvent::EventSoulFire, 100);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (Unit* l_Owner = me->GetOwner())
                    {
                        Unit* l_OwnerTarget = nullptr;
                        if (Player* l_Player = l_Owner->ToPlayer())
                            l_OwnerTarget = l_Player->GetSelectedUnit();
                        else
                            l_OwnerTarget = l_Owner->getVictim();

                        if (l_OwnerTarget)
                            AttackStart(l_OwnerTarget);
                    }

                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (m_Events.ExecuteEvent() == eEvent::EventSoulFire)
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSpells::SoulFire, false);

                    m_Events.ScheduleEvent(eEvent::EventSoulFire, 50);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new spell_npc_warl_inner_demonAI(p_Creature);
        }
};

/// last update : 6.1.2 19802
/// Force of Nature (treant resto) - 54983
class spell_npc_dru_force_of_nature_resto : public CreatureScript
{
    public:
        spell_npc_dru_force_of_nature_resto() : CreatureScript("npc_dru_force_of_nature_resto") { }

        enum eSpells
        {
            HealingTouch = 113828
        };

        struct spell_npc_dru_force_of_nature_resto_impAI : public ScriptedAI
        {
            spell_npc_dru_force_of_nature_resto_impAI(Creature *creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                me->SetReactState(REACT_HELPER);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                Unit* l_Target = nullptr;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (Unit* l_Owner = me->GetOwner())
                {
                    l_Target = l_Owner->getVictim();

                    if (l_Target == nullptr)
                    {
                        if (Player* l_Player = l_Owner->ToPlayer())
                            l_Target = l_Player->GetSelectedUnit();
                    }

                }

                if (l_Target == nullptr)
                {
                    std::list<Unit*> l_FriendlyUnitList;
                    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(me, me, 40.0f);
                    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(me, l_FriendlyUnitList, l_Check);
                    me->VisitNearbyObject(40.0f, l_Searcher);

                    if (l_FriendlyUnitList.size() > 1)
                    {
                        l_FriendlyUnitList.sort(JadeCore::HealthPctOrderPred());
                        l_FriendlyUnitList.resize(1);
                    }

                    l_Target = l_FriendlyUnitList.front();
                }

                if (l_Target == nullptr)
                    return;

                if (!me->IsValidAssistTarget(l_Target))
                    return;

                me->CastSpell(l_Target, eSpells::HealingTouch, false);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_dru_force_of_nature_resto_impAI(creature);
        }
};

void AddSC_npc_spell_scripts()
{
    /// Mage NPC
    new spell_npc_mage_prismatic_crystal();
    new spell_npc_mage_frozen_orb();

    /// Rogue NPC
    new spell_npc_rogue_shadow_reflection();

    /// Shaman NPC
    new spell_npc_sha_capacitor_totem();
    new spell_npc_sha_spirit_link_totem();
    new spell_npc_sha_storm_elemental();
    new spell_npc_sha_fire_elemental();
    new spell_npc_sha_earth_elemental();
    new spell_npc_sha_feral_spirit();
    new spell_npc_sha_healing_rain();

    /// Warrior NPC
    new spell_npc_warr_ravager();
    new spell_npc_warr_mocking_banner();

    /// Warlock NPC
    new spell_npc_warl_wild_imp();
    new spell_npc_warl_doomguard();
    new spell_npc_warl_demonic_gateway_purple();
    new spell_npc_warl_demonic_gateway_green();
    new spell_npc_warl_inner_demon();

    /// Druid NPC
    new spell_npc_dru_force_of_nature_resto();
}
