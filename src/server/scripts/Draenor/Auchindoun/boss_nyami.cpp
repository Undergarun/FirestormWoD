////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "auchindon.hpp"

enum eNyamiSpells
{
    SpellShadowWordPain             = 154477,
    SpellSoulVesselDummy            = 155327,
    SpellSoulBubbleVisual           = 177549,
    SpellSoulBubbleBuff             = 177550,
    SpellSoulVesselDmg              = 154187,
    SpellSoulVesselAreatrigger      = 153888,
    SpellTornSpritsDummy            = 153994,
    SpellTornSpritsDummyTwo         = 153991, 
    SpellArbitrerHammer             = 154218,
    SpellRadiantFuryVisualStar      = 157787,
    SpellRadiantFuryDummy           = 154261,
    SpellRadiantFullyVisual         = 154264,
    SpellRadiantDamage              = 154301,
    SpellRadiantFuryJump            = 154262,
    SpellCrusaderStrike             = 176931,
    SpellArcaneBolt                 = 154235,
    SpellStrangulateState           = 78037
};

enum eNyamiEvents
{
    EventShadowWordPain = 1,
    EventMindSpikeNyami,
    EventSoulVessel,
    EventTornSpirit,
    EventTornSpiritNyamiEffect,
    EventTornSpiritsDummy,
    EventArbitrerHammer,
    EventRadiantFury,
    EventRadiantFurySummonTrigger,
    EventRadiantFuryStop,
    EventArcaneBolt,
    EventCrusaderStrike,
    EventArcaneBombNyami
};

enum eNyamiTalks
{
    NyamiSpell1     = 1,    ///< Your Oath Is Unfinished! (43647)
    NyamiSpell3     = 2,    ///< The Spirits Are Mine To Command! (43649)
    NyamiSpell2     = 3,    ///< Return To This World! (43648)
    NyamiSpell4     = 4,    ///< I Will Drown This World In Shadows!(43650)
    NyamiAggro      = 5,    ///< I Will Coil Your Souls Into Darkness!(43636)
    NyamiSlay       = 6,    ///< Shadows Envelop You! (43646)
    NyamiDeath      = 7,    ///< Too Late...My Master...Comes...(43637)
    Auchenaiwarden1 = 32,   ///< Champions! Nyami'S Agents - They Went This Way.
    Auchenaiwarden2 = 33,   ///< No...Oh...No...
    Auchenaiwarden3 = 34,   ///< The Barrier Protecting Auchindoun Is Asunder.
    Auchenaiwarden4 = 35,   ///< A Foul Force Has Penetrated These Sacerd Chambers.
    Auchenaiwarden5 = 36    ///< Scount Ahead While We Establish A Foothold.
};

enum eNyamiActions
{
    ActionSummonSpirits = 1,
    ActionBreakLoose
};

enum eNyamiCreatures
{
    CreatureSpitefulArbitrer           = 76284,
    CreatureTwistedMagus               = 76296,
    CreatureMaleficDefender            = 76283,
    CreatureRadiantFury                = 432626,
    CreatureSoulVesselHackBubbleEffect = 342652
};

/// Nyami after death event
class EventPostNyamiFight : public BasicEvent
{
    public:
        explicit EventPostNyamiFight(Unit* p_Unit, int p_Value) : BasicEvent(), m_Obj(p_Unit), m_Modifier(p_Value)
        {
        }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (m_Obj)
            {
                if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
                {
                    if (Creature* l_Warden = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataWarden)))
                    {
                        if (l_Warden->GetAI())
                        {
                            switch (m_Modifier)
                            {
                                case 10:
                                {
                                    l_Warden->RemoveAllAuras();
                                    l_Warden->SetDisableGravity(false);
                                    l_Warden->SetCanFly(false);
                           

                                    l_Warden->AddAura(eAuchindonSpells::SpellKneel, l_Warden);
                                    l_Warden->m_Events.AddEvent(new EventPostNyamiFight(l_Warden, 0), l_Warden->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 0:
                                {
                                    l_Warden->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                    l_Warden->RemoveAllAuras();
                                    l_Warden->AI()->Talk(eNyamiTalks::Auchenaiwarden1);
                                    l_Warden->RemoveAura(eAuchindonSpells::SpellKneel);

                                    if (GameObject* l_Door = l_Warden->FindNearestGameObject(eAuchindonObjects::GameobjectHolyWall, 30.0f))
                                        l_Door->Delete();

                                    l_Warden->GetMotionMaster()->MovePoint(0, g_PositionWardenPosition1st);
                                    break;
                                }
                                case 1:
                                {
                                    if (GameObject* l_Door = l_Instance->instance->GetGameObject(l_Instance->GetData64(eDataAuchindonDatas::DataAuchindonWindow)))
                                    {
                                        /// Activate
                                        l_Door->SetLootState(LootState::GO_READY);
                                        l_Door->UseDoorOrButton(500000);
                                    }

                                    /// Fel bourne
                                    l_Warden->SummonCreature(eAuchindonCreatures::CreatureFelborneAbyssal, l_Warden->GetPositionX(), l_Warden->GetPositionY(), l_Warden->GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN);

                                    l_Warden->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                    l_Warden->SetReactState(ReactStates::REACT_PASSIVE);

                                    l_Warden->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                                    l_Warden->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                                    l_Warden->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                    l_Warden->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                                    l_Warden->AI()->Talk(eNyamiTalks::Auchenaiwarden2);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            return true;
        }

    private:
        Unit* m_Obj;
        int m_Modifier;
};

/// Warden - 76572
class auchindon_mob_warden_cosmetic : public CreatureScript
{
    public:
        auchindon_mob_warden_cosmetic() : CreatureScript("auchindon_mob_warden_cosmetic") {}

        struct mob_wardenAI : public ScriptedAI
        {
            mob_wardenAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->CastSpell(me, eNyamiSpells::SpellStrangulateState);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                switch (p_Id)
                {
                    case 0:
                        me->m_Events.AddEvent(new EventPostNyamiFight(me, 1), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_wardenAI(p_Creature);
        }
};

/// Nyami - 76177
class auchindon_boss_nyami : public CreatureScript
{
    public:
        auchindon_boss_nyami() : CreatureScript("auchindon_boss_nyami") { }

        struct boss_nyamiAI : public BossAI
        {
            boss_nyamiAI(Creature* p_Creature) : BossAI(p_Creature, eDataAuchindonDatas::DataBossNyami)
            {
                m_Instance = me->GetInstanceScript();

                me->SummonCreature(eAuchindonCreatures::CreatureWardenAzzakael, g_PositionWardenSpawnPoint);
                me->SetRespawnTime(6000000);
            }

            InstanceScript* m_Instance;
            uint32 m_DiffVisual;
            uint32 m_DiffChannel;
            bool m_CanChain;

            void Reset() override
            {
                _Reset();
                events.Reset();

                m_DiffVisual = 8 * TimeConstants::IN_MILLISECONDS;
                m_DiffChannel = 2 * TimeConstants::IN_MILLISECONDS;

                m_CanChain = false;
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                    m_Instance->SetBossState(eDataAuchindonDatas::DataBossNyami, FAIL);
        
                uint32 l_Entries[3] = {CreatureTwistedMagus, CreatureMaleficDefender, CreatureSpitefulArbitrer};
                for (uint32 l_Entry : l_Entries)
                    DespawnCreaturesInArea(l_Entry, me);

                summons.DespawnAll();
            }


            void EnterCombat(Unit* /*p_Who*/) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eDataAuchindonDatas::DataBossNyami, EncounterState::IN_PROGRESS);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }

                Talk(eNyamiTalks::NyamiAggro);

                events.ScheduleEvent(EventMindSpikeNyami, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(EventShadowWordPain, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(EventSoulVessel, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(EventTornSpirit, 35 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eNyamiTalks::NyamiSlay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
                Talk(eNyamiTalks::NyamiDeath);
     
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);;
                    m_Instance->SetBossState(eDataAuchindonDatas::DataBossNyami, EncounterState::DONE);

                    if (Creature* l_Warden = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataWarden)))
                    {
                        l_Warden->GetMotionMaster()->MoveTakeoff(2, l_Warden->GetPositionX(), l_Warden->GetPositionY(), 34.764f);
                    }
                } 

                me->m_Events.AddEvent(new EventPostNyamiFight(me, 10), me->m_Events.CalculateTime(10 * TimeConstants::IN_MILLISECONDS));
            }

            void HandleNonCombatVisuals(uint32 const p_Diff)
            {
                /// Non Combat
                if (!UpdateVictim())
                {
                    if (m_DiffVisual <= p_Diff)
                    {
                        std::list<Creature*> l_ListCorpsesTriggers;
                        me->GetCreatureListWithEntryInGrid(l_ListCorpsesTriggers, eAuchindonCreatures::CreatureCorpsesNyamiFight, 40.0f);

                        if (l_ListCorpsesTriggers.empty())
                            return;

                        std::list<Creature*>::const_iterator it = l_ListCorpsesTriggers.begin();
                        std::advance(it, urand(0, l_ListCorpsesTriggers.size() - 1));

                        me->CastSpell((*it), 153992);
                        me->RemoveAura(160677);
                        me->GetMotionMaster()->MoveJump((*it)->GetPositionX(), (*it)->GetPositionY(), (*it)->GetPositionZ(), 15.0f, 5.0f);

                        m_CanChain = true;
                        m_DiffChannel = 2 * TimeConstants::IN_MILLISECONDS;
                        m_DiffVisual = 15 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_DiffVisual -= p_Diff;

                    if (m_DiffChannel <= p_Diff && m_CanChain)
                    {
                        me->CastSpell(me, 160677);
                        m_CanChain = false;
                    }
                    else
                        m_DiffChannel -= p_Diff;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                HandleNonCombatVisuals(p_Diff);

                events.Update(p_Diff);

                /// Combat
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eNyamiEvents::EventMindSpikeNyami:
                            if (Unit * l_Target = me->getVictim())
                                me->CastSpell(l_Target, eAuchindonSpells::SpellMindSpike);

                            events.ScheduleEvent(eNyamiEvents::EventMindSpikeNyami, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                            break;
                    case eNyamiEvents::EventShadowWordPain:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                me->CastSpell(l_Target, eNyamiSpells::SpellShadowWordPain);
             
                            events.ScheduleEvent(eNyamiEvents::EventShadowWordPain, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                            break;
                    case eNyamiEvents::EventSoulVessel:
                            m_Instance->DoRemoveAurasDueToSpellOnPlayers(eNyamiSpells::SpellSoulBubbleBuff);

                            me->MonsterTextEmote("Soulbinder Nyami begins to cast|cffff0000[Soul Vessel]|cfffaeb00!", me->GetGUID(), true);
                            me->SummonCreature(eNyamiCreatures::CreatureSoulVesselHackBubbleEffect, g_PositionBubble, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            me->CastSpell(me, eNyamiSpells::SpellSoulVesselDummy);

                            events.ScheduleEvent(eNyamiEvents::EventSoulVessel, 25 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eNyamiEvents::EventTornSpirit:
                    {
                            std::list<Creature*> l_ListCorpsesTriggers;
                            me->GetCreatureListWithEntryInGrid(l_ListCorpsesTriggers, eAuchindonCreatures::CreatureCorpsesNyamiFight, 40.0f);

                            if (l_ListCorpsesTriggers.empty())
                                return;

                            std::list<Creature*>::const_iterator it = l_ListCorpsesTriggers.begin();
                            std::advance(it, urand(0, l_ListCorpsesTriggers.size() - 1));

                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->SetSpeed(UnitMoveType::MOVE_RUN, 20.0f, true);
                            me->CastSpell((*it), eAuchindonSpells::SpellDispersionVisualNyami);
               
                            me->GetMotionMaster()->MovePoint(0, (*it)->GetPositionX(), (*it)->GetPositionY(), (*it)->GetPositionZ());

                            events.ScheduleEvent(eNyamiEvents::EventTornSpiritsDummy, 3 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eNyamiEvents::EventTornSpirit, 35 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eNyamiEvents::EventTornSpiritsDummy:
                            me->SetSpeed(UnitMoveType::MOVE_RUN, 1.5f, true);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            me->CastSpell(me, eNyamiSpells::SpellTornSpritsDummy);
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_nyamiAI(p_Creature);
        }
};

/// Bubble Creature - 342652
class auchindon_nyami_bubble : public CreatureScript
{
    public:
        auchindon_nyami_bubble() : CreatureScript("auchindon_nyami_bubble") {}

        struct auchindon_creaturesAI : public Scripted_NoMovementAI
        {
            auchindon_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) {}

            uint32 m_SpellDiff;

            void Reset() override
            {
                me->CastSpell(me, eNyamiSpells::SpellSoulBubbleVisual);
                me->CastSpell(me, eNyamiSpells::SpellSoulBubbleBuff);
                me->SetDisplayId(eAuchindonInformation::InformationDisplayIdInvis);

                m_SpellDiff = 1 * TimeConstants::IN_MILLISECONDS;
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                if (m_SpellDiff <= p_Diff)
                {
                    std::list<Player*> l_PlayerList;
                    JadeCore::AnyPlayerInObjectRangeCheck check(me, 20.0f);
                    JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, l_PlayerList, check);
                    me->VisitNearbyObject(12.0f, searcher);

                    if (l_PlayerList.empty())
                        return;

                    for (std::list<Player*>::const_iterator it = l_PlayerList.begin(); it != l_PlayerList.end(); ++it)
                    {
                        if (!(*it))
                            return;

                        if ((*it)->IsWithinDistInMap(me, 4.0f))
                        {
                            if (!(*it)->HasAura(eNyamiSpells::SpellSoulBubbleBuff))
                            {
                                me->AddAura(eNyamiSpells::SpellSoulBubbleBuff, (*it));

                                Aura* aura = me->GetAura(eNyamiSpells::SpellSoulBubbleBuff);

                                if (aura)
                                {
                                    aura->SetDuration(1);
                                }
                            }
                        }
                        else
                        {
                            if ((*it)->HasAura(eNyamiSpells::SpellSoulBubbleBuff))
                                (*it)->RemoveAura(eNyamiSpells::SpellSoulBubbleBuff);
                        }
                    }
                }
                else
                    m_SpellDiff -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchindon_creaturesAI(p_Creature);
        }
};

/// Radiant Star AreaTrigger
class auchindon_nyami_radiant_star : public AreaTriggerEntityScript
{
    public:
        auchindon_nyami_radiant_star() : AreaTriggerEntityScript("auchindon_nyami_radiant_star") { }

        uint32 m_Diff = 1 * TimeConstants::IN_MILLISECONDS;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Diff <= p_Time)
            {
                std::list<Player*> l_ListPlayers;
                JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 3.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListPlayers, check);
                p_AreaTrigger->VisitNearbyObject(3.0f, searcher);

                if (l_ListPlayers.empty())
                    return;

                for (std::list<Player*>::const_iterator itr = l_ListPlayers.begin(); itr != l_ListPlayers.end(); ++itr)
                {
                    if ((*itr) && (*itr)->IsInWorld())
                    (*itr)->CastSpell((*itr), eNyamiSpells::SpellRadiantDamage);
                }

                p_AreaTrigger->Remove(3 * TimeConstants::IN_MILLISECONDS);
            }
            else
            {
                m_Diff -= p_Time;
            }
        }

        void OnRemove(AreaTrigger* /*p_AreaTrigger*/, uint32 /*p_Time*/) override
        {
            /// Does nothing.
        }

        auchindon_nyami_radiant_star* GetAI() const override
        {
            return new auchindon_nyami_radiant_star();
        }
};

/// Malefic Defender - 76283
class auchindon_nyami_malefic_defender : public CreatureScript
{
    public:
        auchindon_nyami_malefic_defender() : CreatureScript("auchindon_nyami_malefic_defender") { }

        struct auchindon_nyami_malefic_defenderAI : public ScriptedAI
        {
            auchindon_nyami_malefic_defenderAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventCrusaderStirke, 5 * TimeConstants::IN_MILLISECONDS);
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
                    case eAuchindonEvents::EventCrusaderStirke:
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eAuchindonSpells::SpellCrusaderStirke);

                            events.ScheduleEvent(eAuchindonEvents::EventCrusaderStirke, urand(7 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchindon_nyami_malefic_defenderAI(p_Creature);
        }
};

/// Spiteful Arbitrer - 76284
class auchindon_nyami_spiteful_arbitrer : public CreatureScript
{
    public:
        auchindon_nyami_spiteful_arbitrer() : CreatureScript("auchindon_nyami_spiteful_arbitrer") { }

        struct auchindon_nyami_spiteful_arbitrerAI : public ScriptedAI
        {
            auchindon_nyami_spiteful_arbitrerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            bool m_Radiant;
            uint32 m_RadiantDiff;

            void Reset() override
            {
                events.Reset();

                m_Radiant = false;
                m_RadiantDiff = 500;
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eNyamiEvents::EventRadiantFury, 8 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eNyamiEvents::EventArbitrerHammer, 14 * TimeConstants::IN_MILLISECONDS);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                switch (p_Id)
                {
                    case 500:
                        m_Radiant = false;
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                if (m_Radiant)
                {
                    if (m_RadiantDiff <= p_Diff)
                    {
                        Position l_Position;
                        me->GetPosition(&l_Position);

                        /// Right
                        me->SummonCreature(eNyamiCreatures::CreatureRadiantFury, l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetOrientation() * M_PI, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                        /// Left
                        me->SummonCreature(eNyamiCreatures::CreatureRadiantFury, l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetOrientation() * ((0 - 2) * M_PI), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                        m_RadiantDiff = 500;
                    }
                    else
                        m_RadiantDiff -= p_Diff;
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eNyamiEvents::EventArbitrerHammer:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            {
                                me->CastSpell(l_Target, SpellArbitrerHammer);
                                events.ScheduleEvent(eNyamiEvents::EventArbitrerHammer, 14 * TimeConstants::IN_MILLISECONDS);
                            }
                            break;
                    case eNyamiEvents::EventRadiantFury:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                            {
                                m_Radiant = true;
                                m_RadiantDiff = 500;

                                me->GetMotionMaster()->MoveJump(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), 4.0f, 4.0f, 10.0f, 500);
                                events.ScheduleEvent(eNyamiEvents::EventRadiantFuryStop, 3 * TimeConstants::IN_MILLISECONDS);
                            }
                            break;
                    case eNyamiEvents::EventRadiantFuryStop:
                        {
                            events.CancelEvent(eNyamiEvents::EventRadiantFurySummonTrigger);
                            events.ScheduleEvent(eNyamiEvents::EventRadiantFury, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchindon_nyami_spiteful_arbitrerAI(p_Creature);
        }
};

/// Radiant Fury - 432626
class auchindon_nyami_radiant_fury_trigger : public CreatureScript
{
    public:
        auchindon_nyami_radiant_fury_trigger() : CreatureScript("auchindon_nyami_radiant_fury_trigger") { }

        struct auchindon_nyami_radiant_fury_triggerAI : public Scripted_NoMovementAI
        {
            auchindon_nyami_radiant_fury_triggerAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                Reset();
                me->Respawn(true);
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->CastSpell(me, SpellRadiantFullyVisual);
                me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchindon_nyami_radiant_fury_triggerAI(p_Creature);
        }
};

/// Twisted Magus - 76296
class auchindon_nyami_twisted_magus : public CreatureScript
{
    public:
        auchindon_nyami_twisted_magus() : CreatureScript("auchindon_nyami_twisted_magus") { }

        struct auchindon_nyami_twisted_magusAI : public ScriptedAI
        {
            auchindon_nyami_twisted_magusAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eNyamiEvents::EventArcaneBolt, 4 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eNyamiEvents::EventArcaneBombNyami, 10 * TimeConstants::IN_MILLISECONDS);
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
                    case eNyamiEvents::EventArcaneBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, SpellArcaneBolt);

                            events.ScheduleEvent(eNyamiEvents::EventArcaneBolt, 6 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eNyamiEvents::EventArcaneBombNyami:
                            me->CastSpell(me, SpellArcaneBombDummy);

                            events.ScheduleEvent(eAuchindonEvents::EventArcaneBomb, 25 * TimeConstants::IN_MILLISECONDS);
                            break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchindon_nyami_twisted_magusAI(p_Creature);
        }
};

/// Torn Spirits - 153994
class auchindon_nyami_torn_spirits : public SpellScriptLoader
{
    public:
        auchindon_nyami_torn_spirits() : SpellScriptLoader("auchindon_nyami_torn_spirits") { }

        class auchindon_spells : public SpellScript
        {
            PrepareSpellScript(auchindon_spells);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 l_Entries[3] = { eNyamiCreatures::CreatureMaleficDefender, eNyamiCreatures::CreatureTwistedMagus, eNyamiCreatures::CreatureSpitefulArbitrer };

                if (Unit* l_Caster = GetCaster())
                {
                    Position l_Pos;

                    if (Creature* l_Trigger = GetCaster()->FindNearestCreature(eAuchindonCreatures::CreatureCorpsesNyamiFight, 100.0f, true))
                    {
                        l_Trigger->GetRandomNearPosition(l_Pos, 4.0f);

                        for (uint32 l_Entry : l_Entries)
                            l_Caster->SummonCreature(l_Entry, l_Pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(auchindon_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new auchindon_spells();
        }
};

/// Soul Vessel - 154187
class auchindon_nyami_spell_soul_vessel_damage : public SpellScriptLoader
{
    public:
        auchindon_nyami_spell_soul_vessel_damage() : SpellScriptLoader("auchindon_nyami_spell_soul_vessel_damage") { }

        class auchindon_spells : public SpellScript
        {
            PrepareSpellScript(auchindon_spells);

            void RecalculateDamage(SpellEffIndex /*l_EffIndex*/)
            {
                if (GetHitUnit() && GetHitUnit()->HasAura(eNyamiSpells::SpellSoulBubbleBuff))
                    SetHitDamage(0);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(auchindon_spells::RecalculateDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new auchindon_spells();
        }
};

/// Soul Vessel - 155327
class auchindon_nyami_spell_soul_vessel_dummy : public SpellScriptLoader
{
    public:
        auchindon_nyami_spell_soul_vessel_dummy() : SpellScriptLoader("auchindon_nyami_spell_soul_vessel_dummy") { }

        class auchindon_spells : public AuraScript
        {
            PrepareAuraScript(auchindon_spells);

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                    {
                        if (l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataTriggerMiddleNyamiFightBubble)))
                        {
                            std::list<Player*> l_ListPlayers;
                            l_Caster->GetPlayerListInGrid(l_ListPlayers, 200.0f);

                            if (l_ListPlayers.empty())
                                return;

                            for (auto itr : l_ListPlayers)
                            {
                                if (itr && itr->IsInWorld())
                                    itr->CastSpell(itr, eNyamiSpells::SpellSoulVesselDmg);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new auchindon_spells();
        }
};

#ifndef __clang_analyzer__
void AddSC_nyami()
{
    new auchindon_boss_nyami();
    new auchindon_nyami_malefic_defender();
    new auchindon_nyami_spiteful_arbitrer();
    new auchindon_nyami_twisted_magus();
    new auchindon_mob_warden_cosmetic();
    new auchindon_nyami_radiant_fury_trigger();
    new auchindon_nyami_bubble();
    new auchindon_nyami_radiant_star();
    new auchindon_nyami_spell_soul_vessel_damage();
    new auchindon_nyami_torn_spirits();
    new auchindon_nyami_spell_soul_vessel_dummy();
}
#endif
