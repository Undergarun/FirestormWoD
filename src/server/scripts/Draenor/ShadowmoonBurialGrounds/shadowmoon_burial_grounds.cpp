
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "shadowmoon_burial_grounds.hpp"

static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
{
    std::list<Creature*> creatures;
    GetCreatureListWithEntryInGrid(creatures, object, entry, 10.0f);
    if (creatures.empty())
        return;

    for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

Position l_PositionDisturbedSpirits[2] = 
{
    {1699.829f, -243.255f, 250.065f, 1.164130f},
    {1699.862f, -242.144f, 250.318f, 0.891596f},
};

// Weeping Spirit - 75507
// Restless Wanderer - 75729
class shadowmoon_burial_grounds_spirits : public CreatureScript
{
public:
    shadowmoon_burial_grounds_spirits() : CreatureScript("shadowmoon_burial_grounds_spirits") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(35);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

            switch (urand(0, 4))
            {
            case 0:
                me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellKneel);
                break;
            case 1:
                me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellCry);
                break;
            case 2:
                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventWander, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            default:
                break;
            }        
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eShadowmoonBurialGroundsEvents::EventWander:
                me->GetMotionMaster()->MoveRandom(4.0f);
                me->RemoveAllAuras();

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventWander, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Restless Wanderer - 75729
class shadowmoon_burial_grounds_restless_wanderer : public CreatureScript
{
public:
    shadowmoon_burial_grounds_restless_wanderer() : CreatureScript("shadowmoon_burial_grounds_restless_wanderer") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(who, 40.0f))
            {
                m_Intro = true;

                switch (m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataWandererTalk))
                {
                    case 0:
                        Talk(eShadowmoonBurialGroundsTalks::TalkRestlessWanderer01);
                        break;
                    case 1:
                        Talk(eShadowmoonBurialGroundsTalks::TalkRestlessWanderer02);
                        break;
                    case 2:
                        Talk(eShadowmoonBurialGroundsTalks::TalkRestlessWanderer03);
                        break;
                    case 3:
                        me->MonsterYell("Nerzul cannot hear us.. He only hears Darkness now.", LANG_UNIVERSAL, me->GetGUID());
                        break;
                }  

                if (m_Instance != nullptr)
                {
                    m_Instance->SetData64(eShadowmoonBurialGroundsDatas::DataWandererTalk, 0);
                }
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Monstrous Corpse Spider - 76104
class shadowmoon_burial_grounds_monstrous_corpse_spider : public CreatureScript
{
public:
    shadowmoon_burial_grounds_monstrous_corpse_spider() : CreatureScript("shadowmoon_burial_grounds_monstrous_corpse_spider") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(16);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventDeathVenom, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventNecroticBurst, 25 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eShadowmoonBurialGroundsEvents::EventDeathVenom:
                    if (Unit* target = me->getVictim())
                        me->CastSpell(target, eShadowmoonBurialGroundsSpells::SpellDeathVenoumDamage);

                    events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventDeathVenom, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eShadowmoonBurialGroundsEvents::EventNecroticBurst:
                    me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellNecroticBurstDamage, true);
               
                    events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventNecroticBurst + 1, 4 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eShadowmoonBurialGroundsEvents::EventNecroticBurst + 1:
                    me->Kill(me);
                    break;
            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Exhume Spirits - 75979
class shadowmoon_burial_grounds_exhume_spirits : public CreatureScript
{
public:
    shadowmoon_burial_grounds_exhume_spirits() : CreatureScript("shadowmoon_burial_grounds_exhume_spirits") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(16);
            DoZoneInCombat();
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Defiled Spirits - 75966
class shadowmoon_burial_grounds_defiled_spirit : public CreatureScript
{
public:
    shadowmoon_burial_grounds_defiled_spirit() : CreatureScript("shadowmoon_burial_grounds_defiled_spirit") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();

            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellVortexVisual);
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellChains);
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFacingTo(35);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->RemoveAllAuras();
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Plague bat - 75459
class shadowmoon_burial_grounds_plague_bat : public CreatureScript
{
public:
    shadowmoon_burial_grounds_plague_bat() : CreatureScript("shadowmoon_burial_grounds_plague_bat") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(16);
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventPlagueSpit, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eShadowmoonBurialGroundsEvents::EventPlagueSpit:
                    if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(target, eShadowmoonBurialGroundsSpells::SpellPlagueSpitDamage);

                    events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventPlagueSpit, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Shadowmoon Bone-Mender - 75713
class shadowmoon_burial_grounds_bone_mender : public CreatureScript
{
public:
    shadowmoon_burial_grounds_bone_mender() : CreatureScript("shadowmoon_burial_grounds_bone_mender") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventRattlingBones, 16 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventFratlity,      10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventShadowBolt,    6 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventShadowMend,    23 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eShadowmoonBurialGroundsEvents::EventRattlingBones:
                me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellRattlingBones);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventRattlingBones, urand(30 * TimeConstants::IN_MILLISECONDS, 38 * TimeConstants::IN_MILLISECONDS));
                break;
            case eShadowmoonBurialGroundsEvents::EventFratlity:
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(l_Target, eShadowmoonBurialGroundsSpells::SpellShadowWordFrality);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventFratlity, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                break;
            case eShadowmoonBurialGroundsEvents::EventShadowBolt:
                if (Unit* l_Target = me->getVictim())
                    me->CastSpell(l_Target, eShadowmoonBurialGroundsSpells::SpellShadowBolt);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                break;
            case eShadowmoonBurialGroundsEvents::EventShadowMend:
                if (Unit* nearestWoundedCreature = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureReinmatedRitualOfBones, 10.0f, true))
                    me->CastSpell(nearestWoundedCreature, eShadowmoonBurialGroundsSpells::SpellShadowMend);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventShadowMend, urand(45 * TimeConstants::IN_MILLISECONDS, 50 * TimeConstants::IN_MILLISECONDS));
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Shadowmoon Exhumer - 77700
class shadowmoon_burial_grounds_exhumer : public CreatureScript
{
public:
    shadowmoon_burial_grounds_exhumer() : CreatureScript("shadowmoon_burial_grounds_exhumer") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(16);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventVoidBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventExhumeTheCrypts, urand(30 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eShadowmoonBurialGroundsEvents::EventVoidBolt:
                    if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 30.0f, true))
                        me->CastSpell(target, eShadowmoonBurialGroundsSpells::SpellVoidBolt);

                    events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventVoidBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eShadowmoonBurialGroundsEvents::EventExhumeTheCrypts:
                    me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellExhumeTheCryptsScriptEffect);

                    events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventExhumeTheCrypts, urand(30 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Disturbed Soul - 76400
class shadowmoon_burial_grounds_disturbed_soul : public CreatureScript
{
public:
    shadowmoon_burial_grounds_disturbed_soul() : CreatureScript("shadowmoon_burial_grounds_disturbed_soul") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = true;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            events.Reset();
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);

            me->GetMotionMaster()->MovePoint(0, 1720.86f, -195.381f, 251.670f);
        }

        void MovementInform(uint32 /*type*/, uint32 id) override
        {
            switch (id)
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
                me->DespawnOrUnsummon(1000);
                break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            /*switch (events.ExecuteEvent())
            {
            }*/

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Shadowmoon Enslaver - 76446
class shadowmoon_burial_grounds_enslaver : public CreatureScript
{
public:
    shadowmoon_burial_grounds_enslaver() : CreatureScript("shadowmoon_burial_grounds_enslaver") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = true;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            events.Reset();

            // Void ray - cosmetic
            if (Creature* l_NearestSubjugatedSoul = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureSubjugatedSoul, 10.0f, true))
                me->CastSpell(l_NearestSubjugatedSoul, eShadowmoonBurialGroundsSpells::SpellVoidRay);

            me->setFaction(16);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            if (!m_Intro)
            {
                m_Intro = true;

                for (int i = 0; i < 2; i++)
                {
                    Creature* l_DisturbedSoul = me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureDisturbedSoul, l_PositionDisturbedSpirits[i]);
                }
            }

            // Exhume Crypts
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellExhumeTheCryptsScriptEffect);

            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventRendingVoidLash, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventDomination, 25 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eShadowmoonBurialGroundsEvents::EventRendingVoidLash:
                if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 30.0f, true))
                    me->CastSpell(target, eShadowmoonBurialGroundsSpells::SpellRendingVoidLash);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventRendingVoidLash, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                break;
            case eShadowmoonBurialGroundsEvents::EventDomination:
                if (Unit* l_NearestExhumedSpirit = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureExhumeSpirit, 10.0f, true))
                me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellDomination);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventDomination, urand(30 * TimeConstants::IN_MILLISECONDS, 35 * TimeConstants::IN_MILLISECONDS));
                break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Shadowmoon Loyalist - 75506
class shadowmoon_burial_grounds_loyalist : public CreatureScript
{
public:
    shadowmoon_burial_grounds_loyalist() : CreatureScript("shadowmoon_burial_grounds_loyalist") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();

            m_Intro = false;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            events.Reset();
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(who, 15.0f))
            {
                m_Intro = true;

                if (Creature* nearestAltarTrigger = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureCustomizedAltarTrigger, 10.0f, true))
                {
                    Position position;
                    me->GetPosition(&position);

                    nearestAltarTrigger->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellRagingSpiritVisual);
                    nearestAltarTrigger->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureVoidSpawn, position, TEMPSUMMON_DEAD_DESPAWN);
                }
            }
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            if (roll_chance_i(50))
            {
                me->MonsterYell("Death is only the beginning!", LANG_UNIVERSAL, me->GetGUID());
            }

            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventShadowBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eShadowmoonBurialGroundsEvents::EventShadowBolt:
                if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 25.0f, true))
                    me->CastSpell(target, eShadowmoonBurialGroundsSpells::SpellShadowBolt);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventShadowBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                break;
            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Subjugated Soul - 76444
class shadowmoon_burial_grounds_subjugated_soul : public CreatureScript
{
public:
    shadowmoon_burial_grounds_subjugated_soul() : CreatureScript("shadowmoon_burial_grounds_subjugated_soul") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(16);
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellStrangulate);
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellVioletLook);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            /*switch (events.ExecuteEvent())
            {
            }*/

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Void spawn - 75652
class shadowmoon_burial_grounds_void_spawn : public CreatureScript
{
public:
    shadowmoon_burial_grounds_void_spawn() : CreatureScript("shadowmoon_burial_grounds_void_spawn") { }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            me->setFaction(16);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventVoidPulse, 15 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eShadowmoonBurialGroundsEvents::EventVoidPulse:
                if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 8.0f, true))
                    me->CastSpell(target, eShadowmoonBurialGroundsSpells::SpellVoidPulseFullDamage);

                events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventVoidPulse, urand(14 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                break;
            }

            DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Defiled Burial Site - 75977
class shadowmoon_burial_grounds_site : public CreatureScript
{
public:
    shadowmoon_burial_grounds_site() : CreatureScript("shadowmoon_burial_grounds_site") { }

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetDisplayId(59486);

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(pCreature);
    }
};

// Exhume the Crypts - 153268 
class spell_exhume_the_crypts_script_effect : public SpellScriptLoader
{
public:
    spell_exhume_the_crypts_script_effect() : SpellScriptLoader("spell_exhume_the_crypts_script_effect")
    {
    }

    class spell_exhume_the_crypts_script_effect_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_exhume_the_crypts_script_effect_SpellScript)

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            std::list<Creature*> l_ListCrypts;
            GetCaster()->GetCreatureListWithEntryInGrid(l_ListCrypts, eShadowmoonBurialGroundsCreatures::CreatureCrypts, 10.0f);

            if (l_ListCrypts.empty())
                return;

            for (auto itr : l_ListCrypts)
            {
                itr->CastSpell(itr, eShadowmoonBurialGroundsSpells::SpellExhumeTheCryptsAura);
                itr->CastSpell(itr, eShadowmoonBurialGroundsSpells::SpellExhumeTheCryptsSummon, true);
                itr->DespawnOrUnsummon(500);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_exhume_the_crypts_script_effect_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_exhume_the_crypts_script_effect_SpellScript();
    }
};

void AddSC_shadowmoon_burial_grounds()
{
    // creatures
    new shadowmoon_burial_grounds_bone_mender();
    new shadowmoon_burial_grounds_defiled_spirit();
    new shadowmoon_burial_grounds_enslaver();
    new shadowmoon_burial_grounds_exhume_spirits();
    new shadowmoon_burial_grounds_loyalist();
    new shadowmoon_burial_grounds_monstrous_corpse_spider();
    new shadowmoon_burial_grounds_plague_bat();
    new shadowmoon_burial_grounds_subjugated_soul();
    new shadowmoon_burial_grounds_void_spawn();
    new shadowmoon_burial_grounds_site();

    // spells
    new spell_exhume_the_crypts_script_effect();
}