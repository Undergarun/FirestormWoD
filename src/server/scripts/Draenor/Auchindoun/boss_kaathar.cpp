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

enum eKaatharSpells
{
    SpellHallowedGround                = 154526,
    SpellHallowedGroundVisual          = 155646,
    SpellHallowedGroundHackedVisual    = 178266,
    SpellHallowedGroundHackedVisual2   = 174212,
    SpellHallowedGroundHackedVisual3   = 174184,
    SpellConsecratedLight              = 153006,
    SpellConsecratedLightDamage        = 156746,
    SpellHolyShieldThrow               = 153002,
    SpellHolyShieldLos                 = 153028,
    SpellHolyShieldLosSpells           = 153452,
    SpellHolyShieldOffHandDisarm       = 174205,
    SpellHolyShieldTwoHandDisarm       = 174206,
    SpellHolyShieldKnockBack           = 153481,
    SpellHolyShieldDamageFromKnockBack = 153480,
    SpellHolyShieldAreaTrigger         = 153478,
    SpellSanctifiedStrikeDummy         = 152954,
    SpellSanctifiedGroundTickDamage    = 161457,
    SpellSanctifiedGroundAura          = 153430,
    SpellSanctifiedGroundDamageTick    = 161457,
    SpellSanctifiedStrikeAreaTrigger   = 165064,
    SpellSanctifiedStrikeAreaTrigger2  = 163559,
    SpellSancitfiedStrikeAreaTrigger3  = 165055,
    SpellSanctifiedStrikeAreaTrigger4  = 165065,
    SpellFate                          = 157465
};

enum eKaatharEvents
{
    EventHallowedGround = 1,
    EventHolyShield,
    EventConsecratedLight,
    EventSanctifiedStrike,
    EventHolyShieldReturn,
    EventFate,
    EventCheckPlayer,
    EventDecreaseSize
};

enum eKaatharActions
{
    ActionActivateBoss = 1,
    ActionFateHallowedGround
};

enum eKaatharTalks
{
    VigilantKaatherIntro  = 18, ///< None Live p_Who Assault The Holy Auchenai. (46436)
    VigilantKaatherAgro   = 19, ///< I Will Strike You Down. (46434)
    VigilantKaatherSpell1 = 20, ///< Bathe In The Glory Of The Light! (46438)
    VigilantKaatherSpell2 = 21, ///< The Light Guide My Hand! (46439)
    VigilantKaatherKill   = 22, ///< Light Guide You. (46434)
    VigilantKaatherDeath  = 23, ///< Auchindoun...Is...Doomed...(46435)
};

enum eKaatharTriggers
{
    TriggerHallowedGround  = 537324,
    TriggerHolyShield      = 76071,
    TriggerFissureSummoner = 543536
};

class EventNyamiEscape : public BasicEvent
{
    public:
        explicit EventNyamiEscape(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value), BasicEvent()
        {
        }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (m_Obj)
            {
                if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
                {
                    if (Creature* l_Tuulani = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataTuulani)))
                    {
                        if (Creature* l_Nyami = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataNyami)))
                        {
                            if (Creature* l_Kaathar = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataBossKathaar)))
                            {
                                if (l_Tuulani->GetAI() && l_Nyami->GetAI() &&  l_Kaathar->GetAI())
                                {
                                    switch (m_Modifier)
                                    {
                                        case 100:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK6);
                                            l_Nyami->CastSpell(l_Nyami, eAuchindonSpells::SpellNyamiExplodeCrystal);
                                            l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 101), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 101:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK7);
                                            l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 0), l_Tuulani->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 0:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK8);
                                            l_Nyami->GetMotionMaster()->MovePoint(eAuchindonMovementInform::MovementInformNyamiEscape05, g_PositionNyamiEscape01);
                                            break;
                                        }
                                        case 1:
                                        {
                                            l_Nyami->GetMotionMaster()->MovePoint(eAuchindonMovementInform::MovementInformNyamiEscape06, g_PositionNyamiEscape02);
                                            break;
                                        }
                                        case 2:
                                        {
                                            l_Nyami->RemoveAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison);
                                            l_Nyami->AddAura(eAuchindonSpells::SpellDispersionVisualNyami, l_Nyami);
                                            l_Nyami->SetSpeed(UnitMoveType::MOVE_RUN, 10.0f, true);
                                            l_Nyami->CastSpell(l_Nyami, eAuchindonSpells::SpellCrystalEarthquake);
                                            l_Nyami->GetMotionMaster()->MovePoint(eAuchindonMovementInform::MovementInformNyamiEscape07, g_PositionNyamiEscape03);

                                            l_Kaathar->setFaction(eAuchindonInformation::InformationHostileFaction);
                                            l_Kaathar->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                                            l_Kaathar->GetMotionMaster()->MoveJump(g_PositionKaatharCombatJump.GetPositionX(), g_PositionKaatharCombatJump.GetPositionY(), g_PositionKaatharCombatJump.GetPositionZ(), 10.0f, 10.0f, 10.0f, 0);
                                            break;
                                        }
                                        case 3:
                                        {
                                            l_Nyami->GetMotionMaster()->MovePoint(eAuchindonMovementInform::MovementInformNyamiEscape08, g_PositionNyamiEscape04);
                                            break;
                                        }
                                        case 4:
                                        {
                                            l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 6), l_Tuulani->m_Events.CalculateTime(500));
                                            break;
                                        }
                                        case 6:
                                        {
                                            l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK10);
                                            l_Tuulani->AddAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison, l_Tuulani);
                                            l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 7), l_Tuulani->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 7:
                                        {
                                            l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK11);
                                            l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 8), l_Tuulani->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 8:
                                        {
                                            l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK9);
                                            l_Tuulani->AddAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison, l_Tuulani);

                                            l_Nyami->GetMotionMaster()->MovePoint(9, g_PositionNyamiEscape05);
                                            l_Nyami->DespawnOrUnsummon(10 * TimeConstants::IN_MILLISECONDS);
                                            break;
                                        }
                                    }
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
        int m_Event;
};

class EventPostKaathar : public BasicEvent
{
    public:
        explicit EventPostKaathar(Unit* unit, int value) : m_Obj(unit), m_Modifier(value), BasicEvent()
        {
        }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (m_Obj)
            {
                if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
                {
                    if (Creature* l_Tuulani = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataTuulani)))
                    {
                        if (l_Tuulani->GetAI())
                        {
                            switch (m_Modifier)
                            {
                                case 0:
                                {
                                    l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK14);
                                    l_Tuulani->RemoveAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison); // void prison

                                    for (int32 i = 0; i < 4; i++)
                                    {
                                        if (Creature* l_Defender = m_Obj->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                        {
                                            l_Defender->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 505);
                                            l_Defender->GetMotionMaster()->MovePoint(0, g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[i]);
                                        }
                                    }

                                    if (Creature* l_Magus = m_Obj->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionMageSpawning, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Magus->GetMotionMaster()->MovePoint(0, g_PositionMageMoveTo);
                                        l_Magus->m_Events.AddEvent(new EventPostKaathar(l_Magus, 1), l_Magus->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    }
                                    break;
                                }
                                case 1:
                                {
                                    l_Tuulani->SummonCreature(eAuchindonBosses::BossNyami, g_PositionNyamiSpawn, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                                    /// Corpses
                                    for (int32 i = 0; i < 2; i++)
                                    {
                                        if (Creature* l_Corpses = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionThreeCorpses[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                        {
                                            l_Corpses->SetReactState(ReactStates::REACT_PASSIVE);
                                            l_Corpses->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH | eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                            l_Corpses->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                            l_Corpses->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                            l_Corpses->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                        }
                                    }

                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiDefender, g_PositionMagusAndDefenderHostile[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);

                                    /// Hostile near Two corpses
                                    Creature* l_Magus = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiMagus, g_PositionMagusAndDefenderHostile[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);   
                                    {
                                        l_Magus->CastSpell(l_Magus, SpellArcaneChanneling);
                                    }

                                    /// Two defender
                                    for (int32 i = 0; i < 2; i++)
                                    {
                                        l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiDefender, g_PositionSargereiDefenders[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                    }

                                    // Magus p_Who control footmans
                                    if (Creature* l_Magus = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiMagus, g_PositionMagusp_WhoControlFootmans, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Magus->CastSpell(l_Magus, SpellArcaneChanneling);
                                    }

                                    /// Three prisonners
                                    for (int32 i = 0; i < 3; i++)
                                    {
                                        if (Creature* l_Prisoners = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, g_PositionThreePrisoners[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                        {
                                            l_Prisoners->SetCanFly(true);
                                            l_Prisoners->SetDisableGravity(true);
                                            l_Prisoners->CastSpell(l_Prisoners, eAuchindonSpells::SpellPrisonAura);
                                            l_Prisoners->CastSpell(l_Prisoners, eAuchindonSpells::SpellStrangulate);
                                            l_Prisoners->SetReactState(ReactStates::REACT_PASSIVE);
                                            l_Prisoners->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                            l_Prisoners->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                            l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,  eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                            l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                        }
                                    }

                                    /// Twelve prisoners (cosmetic)
                                    for (int32 i = 0; i <= 11; i++)
                                    {
                                        if (Creature* l_Prisoners = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionCorpsesNearNyomi[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                        {
                                            l_Prisoners->SetReactState(ReactStates::REACT_PASSIVE);
                                            l_Prisoners->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                            l_Prisoners->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                            l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                            l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN | eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
                                        }
                                    }

                                    /// Pack of arbitrer and magus and defender.
                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSpitefulArbitrerTrash, g_PositionThreeHostileArbitrerMagusSoulPriest[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiMagus, g_PositionThreeHostileArbitrerMagusSoulPriest[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargeriSoulPriest, g_PositionThreeHostileArbitrerMagusSoulPriest[2], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);

                                    /// Warden
                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargeriWarden, g_PositionWardenAndGuards, TempSummonType::TEMPSUMMON_DEAD_DESPAWN);

                                    /// Defenders
                                    for (int32 i = 0; i < 2; i++)
                                    {
                                        l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiDefender, g_PositionGuardsAndWardens[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                    }

                                    /// Holy Wall, Object In MIddle
                                    l_Tuulani->SummonGameObject(eAuchindonObjects::GameobjectHolyWall, g_PositionWallInMiddleFromNyami.GetPositionX(), g_PositionWallInMiddleFromNyami.GetPositionY(), g_PositionWallInMiddleFromNyami.GetPositionZ(), g_PositionWallInMiddleFromNyami.GetOrientation(), 0, 0, 0, 0, 0);

                                    for (int32 i = 0; i < 4; i++)
                                    {
                                        if (Creature* l_Defenders = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniWarden, g_PositionDefenderBehindMiddleWallOfNyami[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                        {
                                            l_Defenders->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                            l_Defenders->SetReactState(ReactStates::REACT_PASSIVE);

                                            if (Creature* l_Stalker = l_Defenders->FindNearestCreature(eAuchindonCreatures::CreatureLightWallTargets, 8.0f))
                                                l_Defenders->CastSpell(l_Stalker, eAuchindonSpells::SpellHolyBeam);
                                        }
                                    }

                                    /// Holy Wall, Object Behind
                                    l_Tuulani->SummonGameObject(eAuchindonObjects::GameobjectHolyWall, g_PositionWallInBackFromNyami.GetPositionX(), g_PositionWallInBackFromNyami.GetPositionY(), g_PositionWallInBackFromNyami.GetPositionZ(), g_PositionWallInBackFromNyami.GetOrientation(), 0, 0, 0, 0, 0);

                                    for (int32 i = 0; i < 2; i++)
                                    {
                                        if (Creature* l_Defenders = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniWarden, g_PositionDefenderBehindBackWallOfNyami[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                        {
                                            l_Defenders->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                            l_Defenders->SetReactState(ReactStates::REACT_PASSIVE);

                                            if (Creature* l_Stalker = l_Defenders->FindNearestCreature(eAuchindonCreatures::CreatureLightWallTargets, 8.0f))
                                                l_Defenders->CastSpell(l_Stalker, eAuchindonSpells::SpellHolyBeam);
                                        }
                                    }

                                    l_Tuulani->AI()->Talk(TUULANITALK15);

                                    /// Magus
                                    m_Obj->CastSpell(m_Obj, eAuchindonSpells::SpellArcaneChanneling);
                                    m_Obj->SummonGameObject(eAuchindonObjects:: GameobjectTaladorPortal, g_PositionTuulaniGobjectPortalSpawn.GetPositionX(), g_PositionTuulaniGobjectPortalSpawn.GetPositionY(), g_PositionTuulaniGobjectPortalSpawn.GetPositionZ(), g_PositionTuulaniGobjectPortalSpawn.GetOrientation(), 0, 0, 0, 0, 0);
                                    m_Obj->m_Events.AddEvent(new EventPostKaathar(m_Obj, 2), m_Obj->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));           
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
          int m_Event;
          std::list<Creature*> l_Dispeonsor;
};

/// Nyami Mob - 77810
class auchindon_mob_nyami : public CreatureScript
{
    public:
        auchindon_mob_nyami() : CreatureScript("auchindon_mob_nyami") {}

        struct auchindon_creaturesAI : public ScriptedAI
        {
            auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->CastSpell(me, eAuchindonSpells::SpellLevitateNyami);

                me->setFaction(eAuchindonInformation::InformationFriendlyFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                switch (p_Id)
                {
                    case eAuchindonMovementInform::MovementInformNyamiEscape05:
                        me->m_Events.AddEvent(new EventNyamiEscape(me, 1), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformNyamiEscape06:
                        me->m_Events.AddEvent(new EventNyamiEscape(me, 2), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformNyamiEscape07:
                        me->m_Events.AddEvent(new EventNyamiEscape(me, 3), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformNyamiEscape08:
                        me->m_Events.AddEvent(new EventNyamiEscape(me, 4), me->m_Events.CalculateTime(200));
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
            return new auchindon_creaturesAI(p_Creature);
        }
};

/// Vigilant Kaathar - 75839
class auchindon_boss_kaathar : public CreatureScript
{
    public:
        auchindon_boss_kaathar() : CreatureScript("auchindon_boss_kaathar") { }

        struct boss_kaatharAI : public BossAI
        {
            boss_kaatharAI(Creature* p_Creature) : BossAI(p_Creature, eDataAuchindonDatas::DataBossKathaar)
            {
                m_Instance = me->GetInstanceScript();

                ActivateDoors();

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(eAuchindonInformation::InformationFriendlyFaction);     
            
                me->CastSpell(me, eAuchindonSpells::SpellGuard);

                m_Counting  = 0;
                m_IntroDone = false;
                m_Intro     = false;
            }

            InstanceScript* m_Instance;
            uint32 m_Counting;
            bool m_IntroDone;
            bool m_Intro;

            void Reset() override
            {
                _Reset();
                events.Reset(); 

                me->SetCurrentEquipmentId(1); ///< Equipment Id
            }

            void ActivateDoors()
            {
                if (m_Instance != nullptr)
                {
                    if (GameObject* l_HolyBarrier = m_Instance->instance->GetGameObject(m_Instance->GetData64(DataHolyBarrier)))
                    {
                        l_HolyBarrier->SetLootState(LootState::GO_READY);
                        l_HolyBarrier->UseDoorOrButton();
                    }
                }
            }

            void JustReachedHome() override
            {
                _JustReachedHome();
                summons.DespawnAll();

                ActivateDoors();

                if (m_Instance != nullptr)
                {
                    instance->SetBossState(DataBossKathaar, EncounterState::FAIL);
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(p_Who, 10.0f) && m_IntroDone)
                {
                    m_Intro = true;
                    Talk(eKaatharTalks::VigilantKaatherIntro);

                    me->SetHomePosition(1911.47f, 3152.13f, 30.972f, 1.166194f);
                }
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eKaatharTalks::VigilantKaatherKill);
            }

            void EnterCombat(Unit* p_Who) override
            {
                _EnterCombat();
                Talk(eKaatharTalks::VigilantKaatherAgro);

                ActivateDoors();

                /// Heroics
                if (me->GetMap() && me->GetMap()->IsHeroic())
                    events.ScheduleEvent(eKaatharEvents::EventFate, 45 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eKaatharEvents::EventHallowedGround, urand(12 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
                    events.ScheduleEvent(eKaatharEvents::EventSanctifiedStrike, 8 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eKaatharEvents::EventHolyShield, 30 * TimeConstants::IN_MILLISECONDS);

                if (m_Instance != nullptr)
                {
                    instance->SetBossState(eDataAuchindonDatas::DataBossKathaar, EncounterState::IN_PROGRESS);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                    DoZoneInCombat();
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAuchindonGeneralActions::ActionCountPre1StBossKill:
                        m_Counting = m_Counting + 1;

                        if (m_Counting >= 12)
                        {
                            me->RemoveAura(eAuchindonSpells::SpellGuard);

                            if (Creature* l_Tuulani = instance->instance->GetCreature(instance->GetData64(eDataAuchindonDatas::DataTuulani)))
                            {
                                if (Creature* l_Nyami = instance->instance->GetCreature(instance->GetData64(eDataAuchindonDatas::DataNyami)))
                                {
                                    l_Tuulani->m_Events.KillAllEvents(true);
                                    l_Nyami->m_Events.KillAllEvents(true);
                                }
                            }

                            m_IntroDone = true;
                            me->m_Events.AddEvent(new EventNyamiEscape(me, 100), me->m_Events.CalculateTime(500));
                        }
                        break;
                }
            }

            void DespawnAllAucheniDraeneis()
            {
                int32 l_Entries[14] = {76595, CreatureAuchenaiDefenderUnique, CreatureAucheniMagus2, CreatureAucheniVigiliant, CreatureAucheniHoplite, CreatureAucheniZealot, CreatureAucheniWarden, CreatureAucheniRitualist, CreatureAucheniMagus, CreatureAucheniSoulPriest, CreatureAucheniArbiter, CreatureAucheniCleric, CreatureAucheniDefender};

                for (int32 i = 0; i < 14; i++)
                    DespawnCreaturesInArea(l_Entries[i], me);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();
                Talk(eKaatharTalks::VigilantKaatherDeath);

                DespawnAllAucheniDraeneis();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eDataAuchindonDatas::DataBossKathaar, EncounterState::DONE);
                }

                me->m_Events.AddEvent(new EventPostKaathar(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eKaatharEvents::EventFate:
                    {
                        me->CastSpell(me, eKaatharSpells::SpellFate);
                        me->MonsterTextEmote("|cffff0000[Fate]|cfffaeb00!", me->GetGUID(), true);
                        events.ScheduleEvent(eKaatharEvents::EventFate, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eKaatharEvents::EventHallowedGround:
                    {
                        Position g_Position;
                        me->GetRandomNearPosition(g_Position, 40.0f);

                        if (Creature* l_Trigger = me->SummonCreature(eKaatharTriggers::TriggerHallowedGround, g_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                            me->CastSpell(l_Trigger, eKaatharSpells::SpellHallowedGroundVisual);

                        events.ScheduleEvent(eKaatharEvents::EventHallowedGround, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eKaatharEvents::EventHolyShield:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            Talk(eKaatharTalks::VigilantKaatherSpell2);
                            me->CastSpell(l_Target, eKaatharSpells::SpellHolyShieldThrow);

                            events.ScheduleEvent(eKaatharEvents::EventHolyShield, 30 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eKaatharEvents::EventConsecratedLight, 4 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eKaatharEvents::EventHolyShieldReturn, 14 * TimeConstants::IN_MILLISECONDS);

                            if (me->HasAura(eKaatharSpells::SpellHolyShieldOffHandDisarm))
                                me->AddAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm, me);
                            else
                                me->AddAura(eKaatharSpells::SpellHolyShieldOffHandDisarm, me);

                            std::string l_Str;
                            l_Str += "Vigilant kaathar hurls his |cffff0000[Holy Shield]|cfffaeb00! at ";
                            l_Str += l_Target->GetName();

                            me->MonsterTextEmote(l_Str.c_str(), me->GetGUID(), true);
                        }
                        break;
                    }
                    case eKaatharEvents::EventHolyShieldReturn:
                    {
                        if (Creature* l_Shield = me->FindNearestCreature(eKaatharTriggers::TriggerHolyShield, 50.0f, true))
                        {
                            l_Shield->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 20.0f, 10.0f, 10.0f);
                            l_Shield->DespawnOrUnsummon(3 * TimeConstants::IN_MILLISECONDS);

                            me->SetCurrentEquipmentId(1); ///< Equipment Id

                            if (me->HasAura(eKaatharSpells::SpellHolyShieldOffHandDisarm))
                                me->RemoveAura(eKaatharSpells::SpellHolyShieldOffHandDisarm);
                            else if (me->HasAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm))
                                me->RemoveAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm);

                            DespawnCreaturesInArea(eKaatharTriggers::TriggerHolyShield, me);

                            /// Despawn previous Holy Shields.
                            std::list<AreaTrigger*> l_ListAreaTriggers;
                            me->GetAreaTriggerList(l_ListAreaTriggers, eKaatharSpells::SpellHolyShieldAreaTrigger);

                            if (l_ListAreaTriggers.empty())
                                return;

                            for (auto itr : l_ListAreaTriggers)
                            {
                                itr->Remove(10);
                            }
                        }
                        break;
                    }
                    case eKaatharEvents::EventConsecratedLight:
                    {
                        Talk(eKaatharTalks::VigilantKaatherSpell1);
                        DoCast(eKaatharSpells::SpellConsecratedLight);

                        me->MonsterTextEmote("Vigilant Kaathar readies |cffff0000[Consecrated Light]|cfffaeb00!. Hide!", me->GetGUID(), true);
                        break;
                    }
                    case eKaatharEvents::EventSanctifiedStrike:
                    {
                        DoCastVictim(eKaatharSpells::SpellSanctifiedStrikeDummy);

                        float l_PosX = me->GetPositionX();
                        float l_PosY = me->GetPositionY();
                        float l_Orientation = me->GetOrientation() * M_PI;

                        for (int32 i = 0; i <= 10; i++)
                        {
                            if (i == 9 || i == 5 || i == 6)
                                l_Orientation = l_Orientation / 4;

                            me->SummonCreature(eKaatharTriggers::TriggerFissureSummoner, l_PosX, l_PosY, me->GetPositionZ(), l_Orientation, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);

                            l_PosX += frand(0.5f, 1.8f);
                            l_PosY += frand(0.7f, 1.9f);
                        }

                        for (int32 i = 0; i <= 4; i++)
                        {
                            l_PosX -= 3;
                            l_PosY -= float(2.7);

                            me->SummonCreature(eKaatharTriggers::TriggerFissureSummoner, l_PosX, l_PosY, me->GetPositionZ(), l_Orientation / 2 + i, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);
                        }

                        events.ScheduleEvent(eKaatharEvents::EventSanctifiedStrike, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_kaatharAI(p_Creature);
        }
};

/// Sanctified Strike Fissure
class auchindon_kaathar_areatrigger_fissure : public AreaTriggerEntityScript
{
    public:
        auchindon_kaathar_areatrigger_fissure() : AreaTriggerEntityScript("auchindon_kaathar_areatrigger_fissure")
        {
        }

        uint32 m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Diff <= p_Time)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_PlayerList, check);
                p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

                if (l_PlayerList.empty())
                    return;

                for (std::list<Player*>::const_iterator itr = l_PlayerList.begin(); itr != l_PlayerList.end(); ++itr)
                {
                    if (!(*itr))
                        return;

                    if (!(*itr)->HasAura(eKaatharSpells::SpellSanctifiedGroundAura))
                    {
                        (*itr)->CastSpell((*itr), eKaatharSpells::SpellSanctifiedGroundAura);
                        m_Targets.push_back((*itr)->GetGUID());

                        if ((*itr)->HasAura(eKaatharSpells::SpellSanctifiedGroundAura))
                        {
                            Aura* l_Aura = (*itr)->GetAura(eKaatharSpells::SpellSanctifiedGroundAura);

                            if (l_Aura)
                            {
                                l_Aura->SetDuration(1);
                            }
                        }
                    }
                }

                m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
            }
            else
            {
                m_Diff -= p_Time;
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            for (auto l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eKaatharSpells::SpellSanctifiedGroundAura))
                    l_Target->RemoveAura(eKaatharSpells::SpellSanctifiedGroundAura);
            }
        }

        auchindon_kaathar_areatrigger_fissure* GetAI() const override
        {
            return new auchindon_kaathar_areatrigger_fissure();
        }
};

/// Hallowed Ground - 537324
class auchindon_trigger_hallowed_ground : public CreatureScript
{
    public:
        auchindon_trigger_hallowed_ground() : CreatureScript("auchindon_trigger_hallowed_ground")
        {
        }

        struct trigger_hallowed_groundAI : public Scripted_NoMovementAI
        {
            trigger_hallowed_groundAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetObjectScale(1.0f);
            }

            uint32 m_VisualDiff;
            bool m_HasExploded;

            void Reset() override
            {
                events.Reset();
 
                m_VisualDiff = 1 * TimeConstants::IN_MILLISECONDS;
                m_HasExploded = false;

                me->setFaction(eAuchindonInformation::InformationHostileFaction);
                events.ScheduleEvent(eKaatharEvents::EventCheckPlayer, 4 * TimeConstants::IN_MILLISECONDS); ///< Takes 4 seconds to charge
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eKaatharActions::ActionFateHallowedGround:
                        DoCast(me, eKaatharSpells::SpellHallowedGround);
                        me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                /// Visual
                if (m_VisualDiff <= p_Diff)
                {
                    me->CastSpell(me, eKaatharSpells::SpellHallowedGroundVisual);

                    m_VisualDiff = 1 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_VisualDiff -= p_Diff;

                switch (events.ExecuteEvent())
                {
                    case eKaatharEvents::EventCheckPlayer:
                        if (m_HasExploded)
                            return;

                            if (Player* l_Player = me->FindNearestPlayer(4.0f, true))
                            {
                                if (l_Player->IsWithinDistInMap(me, 4.0f))
                                {
                                    m_HasExploded = true;

                                    DoCast(me, eKaatharSpells::SpellHallowedGround);
                                    me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                                }
                            }

                            events.ScheduleEvent(eKaatharEvents::EventCheckPlayer, 1 * TimeConstants::IN_MILLISECONDS);
                            break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new trigger_hallowed_groundAI(p_Creature);
        }
};

/// Holy Shield - 76071
class auchindon_trigger_holy_shield : public CreatureScript
{
    public:
        auchindon_trigger_holy_shield() : CreatureScript("auchindon_trigger_holy_shield") { }

        struct trigger_holy_shieldAI : public Scripted_NoMovementAI
        {
            trigger_holy_shieldAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();

                me->setFaction(eAuchindonInformation::InformationHostileFaction);
                me->SetDisplayId(eAuchindonInformation::InformationDisplayIdInvis);

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                me->CastSpell(me, eKaatharSpells::SpellHolyShieldLos);

                /// Knockback
                std::list<Player*> l_ListPlayers;
                me->GetPlayerListInGrid(l_ListPlayers, 5.0f);

                if (l_ListPlayers.empty())
                    return;

                for (auto itr : l_ListPlayers)
                {
                    if (!itr)
                        return;

                    itr->CastSpell(itr, eKaatharSpells::SpellHolyShieldKnockBack);
                    me->CastSpell(itr,  eKaatharSpells::SpellHolyShieldDamageFromKnockBack);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new trigger_holy_shieldAI(p_Creature);
        }
};

/// Fissure Spawner Trigger - 543536
class auchindon_trigger_spawn_fissures : public CreatureScript
{
    public:
        auchindon_trigger_spawn_fissures() : CreatureScript("auchindon_trigger_spawn_fissures") { }

        struct trigger_spawn_fissuresAI : public Scripted_NoMovementAI
        {
            trigger_spawn_fissuresAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(eAuchindonInformation::InformationHostileFaction);
                me->SetDisplayId(eAuchindonInformation::InformationDisplayIdInvis);
                me->CastSpell(me, eKaatharSpells::SpellSanctifiedStrikeAreaTrigger4);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new trigger_spawn_fissuresAI(p_Creature);
        }
};

/// Consecrated Light - 153006
class auchindon_spell_consecrated_light : public SpellScriptLoader
{
    public:
        auchindon_spell_consecrated_light() : SpellScriptLoader("auchindon_spell_consecrated_light") { }

        class auchindon_spells : public AuraScript
        {
            PrepareAuraScript(auchindon_spells);

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                if (GetCaster())
                {
                    InstanceScript* m_Instance = GetCaster()->GetInstanceScript();

                    if (!m_Instance)
                        return;

                    std::list<Player*> l_ListPlayers;
                    GetCaster()->GetPlayerListInGrid(l_ListPlayers, 200.0f);

                    if (l_ListPlayers.empty())
                        return;

                    for (auto itr : l_ListPlayers)
                    {
                        if (!itr)
                            return;

                         itr->CastSpell(itr, eKaatharSpells::SpellConsecratedLightDamage, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new auchindon_spells();
        }
};

/// Consecrated Light - 156746
class auchindon_spell_conscreated_damage : public SpellScriptLoader
{
    public:
        auchindon_spell_conscreated_damage() : SpellScriptLoader("auchindon_spell_conscreated_damage") { }

        class auchindon_spells : public SpellScript
        {
            PrepareSpellScript(auchindon_spells);

            void RecalculateDamage(SpellEffIndex /*p_EffIndex*/)
            {
                if (!GetCaster() && !GetHitUnit())
                    return;

                InstanceScript* l_Instance = GetCaster()->GetInstanceScript();

                if (!l_Instance)
                    return;

                if (!GetHitDamage())
                    return;

                if (Creature* l_Kaathar = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataBossKathaar)))
                    if (Creature* l_Shield = GetCaster()->FindNearestCreature(eKaatharTriggers::TriggerHolyShield, 200.0f))
                        if (l_Shield->IsInBetween(GetHitUnit(), l_Kaathar))
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

/// Sanctified Ground Periodic Dummy - 153430
class auchindon_spell_sanctified_ground_periodic_dummy : public SpellScriptLoader
{
    public:
        auchindon_spell_sanctified_ground_periodic_dummy() : SpellScriptLoader("auchindon_spell_sanctified_ground_periodic_dummy") { }

        class auchindon_spells : public AuraScript
        {
            PrepareAuraScript(auchindon_spells);

            void HandlePeriodic(AuraEffect const* p_AurEff)
            {
                PreventDefaultAction();

                if (GetCaster() && GetTarget())
                {
                    GetTarget()->CastSpell(GetTarget(),  eKaatharSpells::SpellSanctifiedGroundTickDamage);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new auchindon_spells();
        }
};

/// Fate - 157465
class auchindon_spell_fate : public SpellScriptLoader
{
    public:
        auchindon_spell_fate() : SpellScriptLoader("auchindon_spell_fate") { }

        class auchindon_spells : public SpellScript
        {
            PrepareSpellScript(auchindon_spells);

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Creature*> l_HallowedGroundCreatures;
                    GetCaster()->GetCreatureListWithEntryInGrid(l_HallowedGroundCreatures, eKaatharTriggers::TriggerHallowedGround, 150.0f);

                    if (l_HallowedGroundCreatures.empty())
                        return;

                    for (auto itr : l_HallowedGroundCreatures)
                    {
                        if (itr->GetAI())
                        itr->GetAI()->DoAction(eKaatharActions::ActionFateHallowedGround);
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

#ifndef __clang_analyzer__
void AddSC_kaathar()
{
    new auchindon_boss_kaathar();
    new auchindon_trigger_hallowed_ground();
    new auchindon_trigger_holy_shield();
    new auchindon_trigger_spawn_fissures();
    new auchindon_kaathar_areatrigger_fissure();
    new auchindon_spell_consecrated_light();
    new auchindon_spell_fate();
    new auchindon_spell_sanctified_ground_periodic_dummy();
    new auchindon_spell_conscreated_damage();
    new auchindon_mob_nyami();
}
#endif