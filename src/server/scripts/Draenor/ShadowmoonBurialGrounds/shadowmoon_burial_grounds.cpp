
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////  MILLENIUM-STUDIO
////  Copyright 2016 Millenium-studio SARL
////  All Rights Reserved.
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "shadowmoon_burial_grounds.hpp"

Position const g_PositionDisturbedSpirits[2] = 
{
    {1699.829f, -243.255f, 250.065f, 1.164130f},
    {1699.862f, -242.144f, 250.318f, 0.891596f}
};

Position const g_PositionNerzulFightLocation = {1712.679f, -797.858f, 73.736f, 4.6856f};

/// Restless Wanderer - 980004
/// Restless Wanderer - 980005
/// Restless Wanderer - 980006
class shadowmoon_burial_grounds_creature_talker : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_talker() : CreatureScript("shadowmoon_burial_grounds_creature_talker") { }

    struct shadowmoon_burial_grounds_creature_talkerAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_talkerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        enum eTalks
        {
            TalkSayMoveLosSay = 1
        };

        enum eSpells
        {
            SpellTalkerVisual = 145945
        };

        void Reset() override
        {
            m_Intro = false;
            me->AddAura(eSpells::SpellTalkerVisual, me);
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (m_Instance != nullptr)
            {
                /// Reset handling - encounter.
                if (Creature * l_Sadana = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossSadana)))
                {
                    if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro && (l_Sadana->isDead() ? me->GetEntry() == 980006 : l_Sadana->isAlive()))
                    {
                        m_Intro = true;
                        Talk(eTalks::TalkSayMoveLosSay);
                    }
                }
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_talkerAI(p_Creature);
    }
};

/// Weeping Spirit - 75507
/// Lost Soul - 75728
/// Restless Wanderer - 75729
class shadowmoon_burial_grounds_creature_spirits : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_spirits() : CreatureScript("shadowmoon_burial_grounds_creature_spirits") { }

    struct shadowmoon_burial_grounds_creature_spiritsAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_spiritsAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }
        
        enum eSpiritSpells
        {
            SpellCry    = 175902,
            SpellKneel  = 139276
        };

        enum eSpiritEvents
        {
            EventWander = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.2f, true);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            events.ScheduleEvent(eSpiritEvents::EventWander, urand(6 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void Emotes()
        {
            me->RemoveAura(eSpiritSpells::SpellCry);
            me->RemoveAura(eSpiritSpells::SpellKneel);
            switch (urand(0, 3))
            {
                case 0:
                    me->CastSpell(me, eSpiritSpells::SpellKneel);
                    break;
                case 1:
                    me->CastSpell(me, eSpiritSpells::SpellCry);
                    break;
                case 2:
                case 3:
                    Position l_Position;
                    me->GetRandomNearPosition(l_Position, 4.0f);
                    l_Position.m_positionZ = me->GetPositionZ();
                    me->GetMotionMaster()->MovePoint(0, l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetPositionZ());           
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eSpiritEvents::EventWander:
                        //Emotes();
                        events.ScheduleEvent(eSpiritEvents::EventWander, urand(6 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_spiritsAI(p_Creature);
    }
};

/// Restless Wanderer - 75729
class shadowmoon_burial_grounds_creature_restless_wanderer : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_restless_wanderer() : CreatureScript("shadowmoon_burial_grounds_creature_restless_wanderer") { }

    struct shadowmoon_burial_grounds_creature_restless_wandererAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_restless_wandererAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
        }

        enum eRestlessWandererEvents
        {
            EventTalk01 = 1,
            EventTalk02
        };

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_restless_wandererAI(p_Creature);
    }
};

/// Monstrous Corpse Spider - 76104
class shadowmoon_burial_grounds_creature_monstrous_corpse_spider : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_monstrous_corpse_spider() : CreatureScript("shadowmoon_burial_grounds_creature_monstrous_corpse_spider") { }

    struct shadowmoon_burial_grounds_creature_monstrous_corpse_spiderAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_monstrous_corpse_spiderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eMonstrousCorpseSpiderSpells
        {
            SpellDeathVenoumDamage   = 156717,
            SpellNecroticBurstDamage = 156718
        };

        enum eMonstrousCorpseSpiderEvents
        {
            EventDeathVenom = 1,
            EventNecroticBurst,
            EventNecroticBurstKillSelf
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eMonstrousCorpseSpiderEvents::EventDeathVenom, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eMonstrousCorpseSpiderEvents::EventNecroticBurst, 25 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);       

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eMonstrousCorpseSpiderEvents::EventDeathVenom:
                        if (Unit* target = me->getVictim())
                            me->CastSpell(target, eMonstrousCorpseSpiderSpells::SpellDeathVenoumDamage);
                        events.ScheduleEvent(eMonstrousCorpseSpiderEvents::EventDeathVenom, 16 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eMonstrousCorpseSpiderEvents::EventNecroticBurst:
                        me->CastSpell(me, eMonstrousCorpseSpiderSpells::SpellNecroticBurstDamage);
                        events.ScheduleEvent(eMonstrousCorpseSpiderEvents::EventNecroticBurst + 1, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eMonstrousCorpseSpiderEvents::EventNecroticBurstKillSelf:
                        me->Kill(me);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_monstrous_corpse_spiderAI(p_Creature);
    }
};

/// Exhume Spirits - 75979
class shadowmoon_burial_grounds_creature_exhume_spirits : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_exhume_spirits() : CreatureScript("shadowmoon_burial_grounds_creature_exhume_spirits") { }

    struct shadowmoon_burial_grounds_creature_exhume_spiritsAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_exhume_spiritsAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            DoZoneInCombat();
            me->setFaction(HostileFaction);     
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            me->AddAura(eShadowmoonBurialGroundsSpells::SpellVortexVisual, me); 
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_exhume_spiritsAI(p_Creature);
    }
};

/// Plague bat - 75459
class shadowmoon_burial_grounds_creature_plague_bat : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_plague_bat() : CreatureScript("shadowmoon_burial_grounds_creature_plague_bat") { }

    struct shadowmoon_burial_grounds_creature_plague_batAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_plague_batAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum ePlagueBatSpells
        {
            SpellPlagueSpitDamage = 153524
        };

        enum ePlagueBatEvents
        {
            EventPlagueSpit = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(ePlagueBatEvents::EventPlagueSpit, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);   

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case ePlagueBatEvents::EventPlagueSpit:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, ePlagueBatSpells::SpellPlagueSpitDamage);
                        events.ScheduleEvent(ePlagueBatEvents::EventPlagueSpit, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_plague_batAI(p_Creature);
    }
};

/// Shadowmoon Bone-Mender - 75713
class shadowmoon_burial_grounds_creature_bone_mender : public CreatureScript
{
public:
    shadowmoon_burial_grounds_creature_bone_mender() : CreatureScript("shadowmoon_burial_grounds_creature_bone_mender") { }

    struct shadowmoon_burial_grounds_creature_bone_menderAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_bone_menderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eShadowmoonBoneMenderSpells
        {
            SpellRattlingBones     = 152816,
            SpellShadowWordFrality = 152819,
            SpellShadowBolt        = 152814,
            SpellShadowMend        = 152818
        };

        enum eShadowmoonBoneMenderEvents
        {
            EventRattlingBones = 1,
            EventFratlity,
            EventShadowBolt,
            EventShadowMend
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventRattlingBones, 16 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventFratlity, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventShadowBolt, 6 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventShadowMend, 23 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);
        
            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eShadowmoonBoneMenderEvents::EventRattlingBones:
                        me->CastSpell(me, eShadowmoonBoneMenderSpells::SpellRattlingBones);
                        events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventRattlingBones, urand(30 * TimeConstants::IN_MILLISECONDS, 38 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eShadowmoonBoneMenderEvents::EventFratlity:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eShadowmoonBoneMenderSpells::SpellShadowWordFrality);
                        events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventFratlity, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eShadowmoonBoneMenderEvents::EventShadowBolt:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eShadowmoonBoneMenderSpells::SpellShadowBolt);
                        events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eShadowmoonBoneMenderEvents::EventShadowMend:
                        if (Unit* l_NearestWoundedCreature = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureReinmatedRitualOfBones, 10.0f, true))
                            me->CastSpell(l_NearestWoundedCreature, eShadowmoonBoneMenderSpells::SpellShadowMend);
                        events.ScheduleEvent(eShadowmoonBoneMenderEvents::EventShadowMend, urand(45 * TimeConstants::IN_MILLISECONDS, 50 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_bone_menderAI(p_Creature);
    }
};

/// Shadowmoon Exhumer - 77700
class shadowmoon_burial_grounds_creature_exhumer : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_exhumer() : CreatureScript("shadowmoon_burial_grounds_creature_exhumer") { }

    struct shadowmoon_burial_grounds_creature_exhumerAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_exhumerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eExhumerSpells
        {
            SpellExhumeTheCryptsScriptEffect = 153268,
            SpellVoidBolt                    = 156722
        };

        enum eExhumerEvents
        {
            EventVoidBolt = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            me->CastSpell(me, eExhumerSpells::SpellExhumeTheCryptsScriptEffect);
            events.ScheduleEvent(eExhumerEvents::EventVoidBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eExhumerEvents::EventVoidBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eExhumerSpells::SpellVoidBolt);
                        events.ScheduleEvent(eExhumerEvents::EventVoidBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_exhumerAI(p_Creature);
    }
};

/// Disturbed Soul - 76400
class shadowmoon_burial_grounds_creature_disturbed_soul : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_disturbed_soul() : CreatureScript("shadowmoon_burial_grounds_creature_disturbed_soul") { }

    struct shadowmoon_burial_grounds_creature_disturbed_soulAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_disturbed_soulAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = true;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->GetMotionMaster()->MovePoint(0, 1720.86f, -195.381f, 251.670f);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case 0:
                    me->GetMotionMaster()->MovePoint(1, 1776.33f, -192.384f, 253.420f);
                    break;
                case 1:
                    me->GetMotionMaster()->MovePoint(2, 1793.44f, -165.260f, 263.614f);
                    break;
                case 2:
                    me->GetMotionMaster()->MovePoint(3, 1793.738f, -141.923f, 273.013f);
                    break;
                case 3:
                    me->GetMotionMaster()->MovePoint(4, 1795.166f, -98.746f, 273.802f);
                    break;
                case 4:
                    me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_disturbed_soulAI(p_Creature);
    }
};

/// Shadowmoon Enslaver - 76446
class shadowmoon_burial_grounds_creature_enslaver : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_enslaver() : CreatureScript("shadowmoon_burial_grounds_creature_enslaver") { }

    struct shadowmoon_burial_grounds_creature_enslaverAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_enslaverAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = true;
            m_First = true;
        }

        enum eEnslaverSpells
        {
            SpellDomination      = 154327,
            SpellVoidRay         = 156920,
            SpellRendingVoidLash = 156776
        };

        enum eEnslaverEvents
        {
            EventRendingVoidLash = 1,
            EventDomination
        };

        InstanceScript* m_Instance;
        bool m_Intro;
        bool m_First;

        void Reset() override
        {
            events.Reset();
            if (m_First)
            {
                m_First = false;
                Position l_Position;
                me->GetRandomNearPosition(l_Position, 2.0f);
                if (Creature* l_SubjugatedSoul = me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureSubjugatedSoul, l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetPositionZ(), l_Position.GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    me->CastSpell(l_SubjugatedSoul, eEnslaverSpells::SpellDomination);
            }
            /// Void ray - cosmetic
            if (Creature* l_NearestSubjugatedSoul = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureSubjugatedSoul, 10.0f, true))
                me->CastSpell(l_NearestSubjugatedSoul, eEnslaverSpells::SpellVoidRay);
            me->setFaction(HostileFaction);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            if (!m_Intro)
            {
                m_Intro = true;
                for (uint8 l_I = 0; l_I < 2; l_I++)
                    me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureDisturbedSoul, g_PositionDisturbedSpirits[l_I]);
            }

            events.ScheduleEvent(eEnslaverEvents::EventRendingVoidLash, urand(4 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEnslaverEvents::EventDomination, 13 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);
   
            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEnslaverEvents::EventRendingVoidLash:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 30.0f, true, -eShadowmoonBurialGroundsSpells::SpellRendingVoidLash))
                            me->CastSpell(l_Target, eEnslaverSpells::SpellRendingVoidLash);
                        events.ScheduleEvent(eEnslaverEvents::EventRendingVoidLash, urand(8 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEnslaverEvents::EventDomination:
                        if (Unit* l_NearestExhumedSpirit = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureSubjugatedSoul, 10.0f, true))
                            me->CastSpell(l_NearestExhumedSpirit, eEnslaverSpells::SpellDomination);
                        events.ScheduleEvent(eEnslaverEvents::EventDomination, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_enslaverAI(p_Creature);
    }
};

/// Shadowmoon Loyalist - 75506
class shadowmoon_burial_grounds_creature_loyalist : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_loyalist() : CreatureScript("shadowmoon_burial_grounds_creature_loyalist") { }

    struct shadowmoon_burial_grounds_creature_loyalistAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_loyalistAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_AltarActivated = false;
            m_Intro = false;
        }

        enum eLoyalistSpells
        {
            SpellPrisonAura = 152309,
            SpellShadowBolt = 152814
        };

        enum eLoyalistEvents
        {
            EventShadowBolt = 1
        };

        InstanceScript* m_Instance;
        bool m_Intro;
        bool m_AltarActivated;

        void Reset() override
        {
            events.Reset();
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(p_Who, 15.0f))
            {
                m_Intro = true;

                /// Activate Altar
                if (Creature* l_NearestAltar = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureWorldTriggerAltar, 10.0f, true))
                {
                    if (!m_AltarActivated)
                    {
                        l_NearestAltar->CastSpell(l_NearestAltar, eShadowmoonBurialGroundsSpells::SpellRagingSpiritVisual);

                        if (Creature* l_VoidSpawn = me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureVoidSpawn, l_NearestAltar->GetPositionX(), l_NearestAltar->GetPositionY(), l_NearestAltar->GetPositionZ(), l_NearestAltar->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            if (l_VoidSpawn->IsAIEnabled)
                                l_VoidSpawn->GetAI()->DoAction(eShadowmoonBurialGroundsActions::ActionSizeGrowth);
                            m_AltarActivated = true;
                        }
                    }
                }
            }
        }

        void ActivateDefiledSpirits()
        {
            std::list<Creature*> l_ListDefiledSpirits;
            me->GetCreatureListWithEntryInGrid(l_ListDefiledSpirits, eShadowmoonBurialGroundsCreatures::CreatureDefiledSpirit02, 20.0f);
            if (!l_ListDefiledSpirits.empty())
            {
                for (Creature* l_Itr : l_ListDefiledSpirits)
                {
                    l_Itr->RemoveAura(eLoyalistSpells::SpellPrisonAura);
                    l_Itr->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    l_Itr->SetReactState(ReactStates::REACT_AGGRESSIVE);
                }
            }
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            ActivateDefiledSpirits();
            events.ScheduleEvent(eLoyalistEvents::EventShadowBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
            if (roll_chance_i(50))
                me->MonsterYell("Death is only the beginning!", LANG_UNIVERSAL, me->GetGUID());         
        }

        void JustSummoned(Creature* p_Summon) override
        {
            if (p_Summon)
            {
                switch (p_Summon->GetEntry())
                {
                    case eShadowmoonBurialGroundsCreatures::CreatureVoidSpawn:
                        p_Summon->SetObjectScale(0.1f);
                        break;
                    default:
                        break;
                }
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eLoyalistEvents::EventShadowBolt:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 25.0f, true))
                        me->CastSpell(l_Target, eLoyalistSpells::SpellShadowBolt);
                    events.ScheduleEvent(eLoyalistEvents::EventShadowBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_loyalistAI(p_Creature);
    }
};

/// Defiled Spirit - 75451
class shadowmoon_burial_grounds_creature_defiled_spirit_trash : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_defiled_spirit_trash() : CreatureScript("shadowmoon_burial_grounds_creature_defiled_spirit_trash") { }

    struct shadowmoon_burial_grounds_creature_defiled_spirit_trashAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_defiled_spirit_trashAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_First = false;
            m_Intro = false;     
        }

        InstanceScript* m_Instance;
        bool m_Intro;
        bool m_First;

        void Reset() override
        {
            events.Reset();
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellVortexVisual);
            if (!m_First)
            {
                m_First = true;
                me->AddAura(eShadowmoonBurialGroundsSpells::SpellChains, me);
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_defiled_spirit_trashAI(p_Creature);
    }
};

/// Subjugated Soul - 76444
class shadowmoon_burial_grounds_creature_subjugated_soul : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_subjugated_soul() : CreatureScript("shadowmoon_burial_grounds_creature_subjugated_soul") { }

    struct shadowmoon_burial_grounds_creature_subjugated_soulAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_subjugated_soulAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellStrangulate);
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellVioletLook);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            me->RemoveAura(eShadowmoonBurialGroundsSpells::SpellStrangulate);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_subjugated_soulAI(p_Creature);
    }
};

/// Defiled Burial Site - 75977
class shadowmoon_burial_grounds_creature_site : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_site() : CreatureScript("shadowmoon_burial_grounds_creature_site") { }

    struct shadowmoon_burial_grounds_creature_siteAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creature_siteAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSiteSpells
        {
            SpellExhumeTheCryptsAura   = 153276,
            SpellExhumeTheCryptsSummon = 153275
        };

        InstanceScript* m_Instance;
        bool m_HasActivated;

        void Reset() override
        {
            events.Reset();
            m_HasActivated = false;
            me->SetDisplayId(59486);         
            me->setFaction(FriendlyFaction);        
            me->SetReactState(ReactStates::REACT_PASSIVE);          
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eShadowmoonBurialGroundsActions::ActionCrypts:
                {
                    if (!m_HasActivated)
                    {
                        m_HasActivated = true;
                        me->CastSpell(me, eSiteSpells::SpellExhumeTheCryptsAura);
                        me->CastSpell(me, eSiteSpells::SpellExhumeTheCryptsSummon, true);
                        me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_siteAI(p_Creature);
    }
};

/// Reanimated Ritual Boens - 75715
class shadowmoon_burial_grounds_creature_ritual_of_bones_mob : public CreatureScript
{
public:

    shadowmoon_burial_grounds_creature_ritual_of_bones_mob() : CreatureScript("shadowmoon_burial_grounds_creature_ritual_of_bones_mob") { }

    struct shadowmoon_burial_grounds_creature_ritual_of_bones_mobAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creature_ritual_of_bones_mobAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eRitualOfBonesSpells
        {
            SpellShadowRitualBonesAuraCosmetic = 160530,
            SpellVoidCleaveDamage = 164907
        };

        enum eRitualOfBonesEvents
        {
            EventVoidCleave = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->AddAura(eRitualOfBonesSpells::SpellShadowRitualBonesAuraCosmetic, me);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eRitualOfBonesEvents::EventVoidCleave, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eRitualOfBonesEvents::EventVoidCleave:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eRitualOfBonesSpells::SpellVoidCleaveDamage);
                        events.ScheduleEvent(eRitualOfBonesEvents::EventVoidCleave, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creature_ritual_of_bones_mobAI(p_Creature);
    }
};

/// Exhume the Crypts - 153268 
class shadowmoon_burial_grounds_spell_exhume_the_crypts : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_spell_exhume_the_crypts() : SpellScriptLoader("shadowmoon_burial_grounds_spell_exhume_the_crypts")
    {
    }

    class shadowmoon_burial_grounds_spell_exhume_the_crypts_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_spell_exhume_the_crypts_SpellScript)

        void HandleScript(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster())
                return;

            if (Creature* l_NearestBurialGround = GetCaster()->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureCrypts, 100.0f, true))
            {
                if (l_NearestBurialGround->IsAIEnabled)
                    l_NearestBurialGround->GetAI()->DoAction(eShadowmoonBurialGroundsActions::ActionCrypts);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(shadowmoon_burial_grounds_spell_exhume_the_crypts_SpellScript::HandleScript, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_spell_exhume_the_crypts_SpellScript();
    }
}; 

/// Necrotic Burst - 156718  
class shadowmoon_burial_grounds_spell_necrotic_burst : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_spell_necrotic_burst() : SpellScriptLoader("shadowmoon_burial_grounds_spell_exhume_the_crypts")
    {
    }

    class shadowmoon_burial_grounds_spell_necrotic_burst_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_spell_necrotic_burst_SpellScript)

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            GetCaster()->Kill(GetCaster());
        }

        void Register()
        {
            AfterCast += SpellCastFn(shadowmoon_burial_grounds_spell_necrotic_burst_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_spell_necrotic_burst_SpellScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_shadowmoon_burial_grounds()
{
    new shadowmoon_burial_grounds_creature_talker;                          ///< 980004, 980005, 980006
    new shadowmoon_burial_grounds_creature_subjugated_soul();               ///< 76444
    new shadowmoon_burial_grounds_creature_plague_bat();                    ///< 75459
    new shadowmoon_burial_grounds_creature_site();                          ///< 75977
    new shadowmoon_burial_grounds_creature_spirits();               
    new shadowmoon_burial_grounds_creature_enslaver();                      ///< 76446
    new shadowmoon_burial_grounds_creature_exhumer();                       ///< 77700
    new shadowmoon_burial_grounds_creature_loyalist();                      ///< 75506
    new shadowmoon_burial_grounds_creature_bone_mender();                   ///< 75713
    new shadowmoon_burial_grounds_creature_exhume_spirits();                ///< 75979
    new shadowmoon_burial_grounds_creature_restless_wanderer();             ///< 75729
    new shadowmoon_burial_grounds_creature_ritual_of_bones_mob();           ///< 75715
    new shadowmoon_burial_grounds_creature_defiled_spirit_trash();          ///< 75451  
    new shadowmoon_burial_grounds_creature_monstrous_corpse_spider();       ///< 76104
    new shadowmoon_burial_grounds_spell_exhume_the_crypts();                ///< 153268
    new shadowmoon_burial_grounds_spell_necrotic_burst();                   ///< 156718 
}
#endif