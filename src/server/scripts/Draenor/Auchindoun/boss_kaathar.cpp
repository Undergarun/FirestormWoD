
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "auchindoun.hpp"

enum eKaatharSpells
{
    SpellHallowedGround                = 154526,
    SpellHallowedGroundsTriggerMissile = 155646,
    SpellConsecratedLight              = 153006,
    SpellConsecratedLightDamage        = 156746,
    SpellHolyShieldThrow               = 153002,
    SpellHolyShieldLos                 = 153028,
    SpellHolyShieldLosSpells           = 153452,
    SpellHolyShieldAreatrigger         = 153478,
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
    ActionFateHallowedGround,
	ActionDespawnCreatures
};

enum eKaatharTalks
{
    VigilantKaatherIntro  = 18,   ///< None Live p_Who Assault The Holy Auchenai. (46436)
    VigilantKaatherAgro   = 19,   ///< I Will Strike You Down. (46434)
    VigilantKaatherSpell1 = 20,   ///< Bathe In The Glory Of The Light! (46438)
    VigilantKaatherSpell2 = 21,   ///< The Light Guide My Hand! (46439)
    VigilantKaatherKill   = 22,   ///< Light Guide You. (46434)
    VigilantKaatherDeath  = 23    ///< Auchindoun...Is...Doomed...(46435)
};

enum eKaatharCreatures
{
    TriggerHallowedGround  = 537324,
    TriggerHolyShield      = 76071,
    TriggerFissureSummoner = 543536
};

Position const g_KaatharNewHomePosition = { 1911.47f, 3152.13f, 30.972f, 1.166194f };


/// 1st Starting Event
class EventKaatharDespawnCreatures : public BasicEvent
{
public:

	explicit EventKaatharDespawnCreatures(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value), BasicEvent()
	{
	}

	bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
	{
		if (m_Obj)
		{
			if (InstanceScript * l_Instance = m_Obj->GetInstanceScript())
			{
				if (Creature* l_Kaathar = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
				{
					if (l_Kaathar->IsAIEnabled)
						l_Kaathar->GetAI()->DoAction(eKaatharActions::ActionDespawnCreatures);
				}
			}
		}

		return true;
	}

private:
	InstanceScript* m_InstanceScript;
	Unit* m_Obj;
	int m_Modifier;
	int m_Event;
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
                if (Creature* l_Tuulani = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataTuulani)))
                {
                    if (Creature* l_Nyami = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataNyami)))
                    {
                        if (Creature* l_Kaathar = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                        {
                            if (l_Tuulani->IsAIEnabled && l_Nyami->IsAIEnabled && l_Kaathar->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                    {
                                        l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK6);
                                        l_Nyami->CastSpell(l_Nyami, eAuchindounSpells::SpellNyamiExplodeCrystal);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 1), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 1:
                                    {
                                        /// Cosmetic crystal projectiles flies toward the middle
                                        if (Creature* l_Teronoger = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                                        {
                                            for (uint8 l_I = 0; l_I < 30; l_I++)
                                            {
                                                if (Unit* l_Caster = l_Nyami->FindNearestCreature(eAuchindounCreatures::CreatureLeftCrystalTrigger, 60.0f, true))
                                                {
													G3D::Vector3 l_Source(1911.741f, 3183.639f, 56.50413f);
													G3D::Vector3 l_Dest(1911.731f, 3183.639f, 56.50413f);
                                                    G3D::Vector3 l_Orientation(0.0f, 0.0f, 0.0f);

                                                    l_Caster->PlayOrphanSpellVisual(l_Source, l_Orientation, l_Dest, eAuchindounSpellVisualKit::SpellVisualKitBlackOrbFallingDownInSpiral, 8.0f);									
                                                }
                                            }
                                        }

										//if (GameObject* l_NearestCrystal = l_Nyami->FindNearestGameObject(eAuchindounObjects::))

                                        l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK7);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 2), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 2:
                                    {
										if (Creature* l_AuchindounProtection = l_Nyami->FindNearestCreature(eAuchindounCreatures::CreatureShieldStalker, 1000.0f, true))
											l_AuchindounProtection->DespawnOrUnsummon();

                                        l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK8);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 3), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 3:
                                    {
                                        l_Nyami->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformNyamiEscape06, g_PositionNyamiEscapeMovement[1]);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 4), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 4:
                                    {
                                        l_Kaathar->setFaction(HostileFaction);
                                        l_Nyami->SetSpeed(UnitMoveType::MOVE_RUN, 10.0f, true);
                                        l_Nyami->RemoveAura(eAuchindounSpells::SpellTuulaniCapturedVoidPrison);
                                        l_Nyami->AddAura(eAuchindounSpells::SpellDispersionVisualNyami, l_Nyami);                                     
                                        l_Nyami->CastSpell(l_Nyami, eAuchindounSpells::SpellCrystalEarthquake);
                                        l_Nyami->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformNyamiEscape07, g_PositionNyamiEscapeMovement[2]);                                 
                                        l_Kaathar->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                                        l_Kaathar->GetMotionMaster()->MoveJump(g_PositionKaatharCombatJump.GetPositionX(), g_PositionKaatharCombatJump.GetPositionY(), g_PositionKaatharCombatJump.GetPositionZ(), 10.0f, 10.0f, 10.0f, 0);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 7), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 7:
                                    {
                                        l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK10);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 8), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 8:
                                    {
                                        l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK11);
                                        l_Tuulani->m_Events.AddEvent(new EventNyamiEscape(l_Tuulani, 9), l_Tuulani->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 9:
                                    {
                                        l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK9);
                                        l_Nyami->DespawnOrUnsummon(10 * TimeConstants::IN_MILLISECONDS);
                                        l_Tuulani->AddAura(eAuchindounSpells::SpellTuulaniCapturedVoidPrison, l_Tuulani);                                        
                                        break;
                                    }
                                    default:
                                        break;
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
                if (Creature* l_Tuulani = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataTuulani)))
                {
                    if (l_Tuulani->IsAIEnabled)
                    {
                        switch (m_Modifier)
                        {
                            case 0:
                            {
                                l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK14);
                                l_Tuulani->RemoveAura(eAuchindounSpells::SpellTuulaniCapturedVoidPrison);

                                for (int8 l_I = 0; l_I < 4; l_I++)
                                {
                                    if (Creature* l_Defender = m_Obj->SummonCreature(eAuchindounCreatures::CreatureAucheniDefender, g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Defender->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 505);
                                        l_Defender->GetMotionMaster()->MovePoint(0, g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[l_I]);
                                    }
                                }

								if (Creature* l_Nyami = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossNyami)))
									l_Nyami->SetPhaseMask(1, true);

                                if (Creature* l_Magus = m_Obj->SummonCreature(eAuchindounCreatures::CreatureAucheniMagus, g_PositionMageSpawning, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                {
                                    l_Magus->GetMotionMaster()->MovePoint(0, g_PositionMageMoveTo);
                                    l_Magus->m_Events.AddEvent(new EventPostKaathar(l_Magus, 1), l_Magus->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));							
                                }                      
                                break;
                            }
							case 1:
							{
								l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK15);

								/// Holy Wall, Object In MIddle
								l_Tuulani->SummonGameObject(eAuchindounObjects::GameobjectHolyWall, g_PositionWallInMiddleFromNyami.GetPositionX(), g_PositionWallInMiddleFromNyami.GetPositionY(), g_PositionWallInMiddleFromNyami.GetPositionZ(), g_PositionWallInMiddleFromNyami.GetOrientation(), 0, 0, 0, 0, 0);
								/// Holy Wall, Object Behind
								l_Tuulani->SummonGameObject(eAuchindounObjects::GameobjectHolyWall, g_PositionWallInBackFromNyami.GetPositionX(), g_PositionWallInBackFromNyami.GetPositionY(), g_PositionWallInBackFromNyami.GetPositionZ(), g_PositionWallInBackFromNyami.GetOrientation(), 0, 0, 0, 0, 0);

								/// Three prisonners
								for (int8 l_I = 0; l_I < 3; l_I++)
								{
									if (Creature* l_Prisoners = l_Tuulani->SummonCreature(eAuchindounCreatures::CreatureAucheniSoulPriest, g_PositionThreePrisoners[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
									{
										l_Prisoners->SetCanFly(true);
										l_Prisoners->SetDisableGravity(true);
										l_Prisoners->SetReactState(ReactStates::REACT_PASSIVE);
										l_Prisoners->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
										l_Prisoners->CastSpell(l_Prisoners, eAuchindounSpells::SpellPrisonAura, true);
										l_Prisoners->CastSpell(l_Prisoners, eAuchindounSpells::SpellStrangulate, true);
										l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
										l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
										l_Prisoners->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
									}
								}

								/// Twelve prisoners (cosmetic)
								for (int8 l_I = 0; l_I < 11; l_I++)
								{
									if (Creature* l_Prisoners = l_Tuulani->SummonCreature(eAuchindounCreatures::CreatureAucheniDefender, g_PositionCorpsesNearNyomi[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
									{
										l_Prisoners->SetReactState(ReactStates::REACT_PASSIVE);
										l_Prisoners->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
										l_Prisoners->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
										l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
										l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN | eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
									}
								}

								/// Defenders
								for (int8 l_I = 0; l_I < 4; l_I++)
								{
									if (Creature* l_Defenders = l_Tuulani->SummonCreature(eAuchindounCreatures::CreatureAucheniWarden, g_PositionDefenderBehindMiddleWallOfNyami[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
									{
										l_Defenders->SetReactState(ReactStates::REACT_PASSIVE);
										l_Defenders->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

										if (Creature* l_Stalker = l_Defenders->FindNearestCreature(eAuchindounCreatures::CreatureLightWallTargets, 8.0f))
											l_Defenders->CastSpell(l_Stalker, eAuchindounSpells::SpellHolyBeam);
									}
								}

								/// Cosmetic Wardens
								for (int8 l_I = 0; l_I < 2; l_I++)
								{
									if (Creature* l_Defenders = l_Tuulani->SummonCreature(eAuchindounCreatures::CreatureAucheniWarden, g_PositionDefenderBehindBackWallOfNyami[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
									{
										l_Defenders->SetReactState(ReactStates::REACT_PASSIVE);
										l_Defenders->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

										if (Creature* l_Stalker = l_Defenders->FindNearestCreature(eAuchindounCreatures::CreatureLightWallTargets, 15.0f))
											l_Defenders->CastSpell(l_Stalker, eAuchindounSpells::SpellHolyBeam);
									}
								}

								/// Phases Teronogor and all the other creatures back to phase 1
								uint32 l_CreaturesTeronogorPhaseIn[7] = { eAuchindounCreatures::CreatureZipteq, eAuchindounCreatures::CreatureZashoo, eAuchindounCreatures::CreatureShaadum,
									eAuchindounCreatures::CreatureGromtashTheDestructor, eAuchindounCreatures::CreatureGulkosh, eAuchindounCreatures::CreatureDurem, eAuchindounBosses::BossTeronogor };

								std::list<Creature*> l_CreaturesTeronogorPhaseInList;

								for (uint8 l_I = 0; l_I < 7; l_I++)
								{
									l_Tuulani->GetCreatureListWithEntryInGrid(l_CreaturesTeronogorPhaseInList, l_CreaturesTeronogorPhaseIn[l_I], 700.0f);
								}

								if (!l_CreaturesTeronogorPhaseInList.empty())
								{
									for (Creature* l_Itr : l_CreaturesTeronogorPhaseInList)
									{
										if (!l_Itr)
											continue;

										l_Itr->SetPhaseMask(1, true);
									}
								}

								uint32 l_EntriesOfSargereiDraeneis[8] = { eAuchindounCreatures::CreatureSargeriMagus, eAuchindounCreatures::CreatureAucheniArbiter,
									eAuchindounCreatures::CreatureSargeriSoulPriest, eAuchindounCreatures::CreatureSargeriWarden,
									eAuchindounCreatures::CreatureAuchenaiAssainated, eAuchindounCreatures::CreatureSargereiAssasinating, eAuchindounCreatures::CreatureWardenAzzakael, eAuchindounBosses::BossNyami };

								std::list<Creature*> l_ListSargereiDraeneis;
								for (uint8 l_I = 0; l_I < 8; l_I++)
								{
									l_Tuulani->GetCreatureListWithEntryInGrid(l_ListSargereiDraeneis, l_EntriesOfSargereiDraeneis[l_I], 1000.0f);
								}

								if (!l_ListSargereiDraeneis.empty())
								{
									for (Creature* l_Itr : l_ListSargereiDraeneis)
									{
										l_Itr->SetPhaseMask(1, true);
										l_Itr->setFaction(HostileFaction);
										l_Itr->SetReactState(ReactStates::REACT_AGGRESSIVE);
										l_Itr->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
									}
								}

								/// Magus
								m_Obj->CastSpell(m_Obj, eAuchindounSpells::SpellArcaneChanneling);
								m_Obj->SummonGameObject(eAuchindounObjects::GameobjectTaladorPortal, g_PositionTuulaniGobjectPortalSpawn.GetPositionX(), g_PositionTuulaniGobjectPortalSpawn.GetPositionY(), g_PositionTuulaniGobjectPortalSpawn.GetPositionZ(), g_PositionTuulaniGobjectPortalSpawn.GetOrientation(), 0, 0, 0, 0, 0);
								break;
							}
                            default:
                                break;
                            
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

/// Nyami Mob - 77810
class auchindoun_kaathar_mob_nyami : public CreatureScript
{
    public:

    auchindoun_kaathar_mob_nyami() : CreatureScript("auchindoun_kaathar_mob_nyami") {}

    struct auchindoun_kaathar_mob_nyamiAI : public ScriptedAI
    {
        auchindoun_kaathar_mob_nyamiAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->CastSpell(me, eAuchindounSpells::SpellLevitateNyami);  
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_kaathar_mob_nyamiAI(p_Creature);
    }
};

/// Vigilant Kaathar - 75839
class boss_kaathar : public CreatureScript
{
    public:

    boss_kaathar() : CreatureScript("boss_kaathar") { }

    struct boss_kaatharAI : public BossAI
    {
        boss_kaatharAI(Creature* p_Creature) : BossAI(p_Creature, eAuchindounDatas::DataBossKathaar)
        {
            m_Instance = me->GetInstanceScript();
            m_False = true;
        }

        InstanceScript* m_Instance;
        uint32 m_Counting;
        bool m_IntroDone;
        bool m_Intro;
        bool m_False;

        void Reset() override
        {
            _Reset();
            events.Reset(); 
            me->SetCurrentEquipmentId(1); // Equipment Id    
            ClearDelayedOperations();

            std::list<AreaTrigger*> l_listAreaTriggers;
            me->GetAreatriggerListInRange(l_listAreaTriggers, 300.0f);
            if (!l_listAreaTriggers.empty())
            {
                for (AreaTrigger* l_Itr : l_listAreaTriggers)
                {
                    if (!l_Itr)
                        continue;

                    if (l_Itr->GetSpellId() != eKaatharSpells::SpellHolyShieldAreatrigger)
                        continue;

                    l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
                }
            }

            if (m_False)
            {
                m_Counting = 0;
                m_Intro = false;
                m_False = false;
                m_IntroDone = false;
                me->setFaction(FriendlyFaction);
                me->CastSpell(me, eAuchindounSpells::SpellGuard);
				ActivateDoors();
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);                                                
            }
		
        }

        void ActivateDoors()
        {
            if (m_Instance != nullptr)
            {
                if (GameObject* l_HolyBarrier = m_Instance->instance->GetGameObject(m_Instance->GetData64(eAuchindounDatas::DataHolyBarrier)))
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
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(p_Who, 10.0f) && m_IntroDone)
            {
                m_Intro = true;
                Talk(eKaatharTalks::VigilantKaatherIntro);
                me->SetHomePosition(g_KaatharNewHomePosition);
            }
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(eKaatharTalks::VigilantKaatherKill);
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();
            ActivateDoors();     

            ///< Heroics
            if (me->GetMap() && me->GetMap()->IsHeroic())
                events.ScheduleEvent(eKaatharEvents::EventFate,           45 * TimeConstants::IN_MILLISECONDS);

            events.ScheduleEvent(eKaatharEvents::EventHallowedGround, urand(12 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eKaatharEvents::EventSanctifiedStrike, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eKaatharEvents::EventHolyShield, 30 * TimeConstants::IN_MILLISECONDS);

            if (m_Instance != nullptr)
				m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
			Talk(eKaatharTalks::VigilantKaatherAgro);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAuchindounActions::ActionCountPre1StBossKill:
                {
                    m_Counting = m_Counting + 1;

                    if (m_Counting >= 12)
                    {
                        me->RemoveAura(eAuchindounSpells::SpellGuard);

                        if (m_Instance != nullptr)
                        {
                            if (Creature* l_Tuulani = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataTuulani)))
                            {
                                if (Creature* l_Nyami = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataNyami)))
                                {
                                    l_Tuulani->m_Events.KillAllEvents(true);
                                    l_Nyami->m_Events.KillAllEvents(true);
                                }
                            }
                        }

                        m_IntroDone = true;
                        me->m_Events.AddEvent(new EventNyamiEscape(me, 0), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
				case eKaatharActions::ActionDespawnCreatures:
					DespawnAllAucheniDraeneis();
					break;
                }
                default:
                    break;
            }
        }

        /// Responsible for the phase change after Kaathar fight - despawning
        void DespawnAllAucheniDraeneis()
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_AssinatingGuard = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataAssinatingGuard)))
                {
                    if (Creature* l_AssinatedGuard = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataAssinatedGuard)))
                    {
                        l_AssinatingGuard->GetMotionMaster()->MovePoint(0, *l_AssinatedGuard);
                        l_AssinatingGuard->Attack(l_AssinatedGuard, true);
                        l_AssinatedGuard->Kill(l_AssinatedGuard);
                    }
                }
            }

            uint32 l_Entries[13] = { 77693, 76595, eAuchindounCreatures::CreatureAuchenaiDefenderUnique, eAuchindounCreatures::CreatureAucheniMagus2, eAuchindounCreatures::CreatureAucheniHoplite,
                eAuchindounCreatures::CreatureAucheniZealot, eAuchindounCreatures::CreatureAucheniWarden, eAuchindounCreatures::CreatureAucheniRitualist, eAuchindounCreatures::CreatureAucheniMagus,
                eAuchindounCreatures::CreatureAucheniSoulPriest, eAuchindounCreatures::CreatureAucheniArbiter, eAuchindounCreatures::CreatureAucheniCleric, eAuchindounCreatures::CreatureAucheniDefender };

            for (uint8 l_I = 0; l_I < 13; l_I++)
                DespawnCreaturesInArea(l_Entries[l_I], me);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
		
            /// From here Teronogor spawns
            std::list<Player*> l_ListPlayers;
            me->GetPlayerListInGrid(l_ListPlayers, 600.0f, true);
            if (!l_ListPlayers.empty())
            {
                for (Player* l_Itr : l_ListPlayers)
                {
                    if (!l_Itr)
                        continue;

                    l_Itr->PlayScene(eAuchindounScenes::SpellAuchindounSceneTeronogorSpawn, l_Itr);
                }               
            }

            /// Phases Teronogor and all the other creatures back to phase 1
            uint32 l_CreaturesTeronogorPhaseIn[7] = { eAuchindounCreatures::CreatureZipteq, eAuchindounCreatures::CreatureZashoo, eAuchindounCreatures::CreatureShaadum,
                eAuchindounCreatures::CreatureGromtashTheDestructor, eAuchindounCreatures::CreatureGulkosh, eAuchindounCreatures::CreatureDurem, eAuchindounBosses::BossTeronogor };

            std::list<Creature*> l_ListCreatures;

            for (uint8 l_I = 0; l_I < 7; l_I++)
            {
                me->GetCreatureListWithEntryInGrid(l_ListCreatures, l_CreaturesTeronogorPhaseIn[l_I], 700.0f);
            }

            if (!l_ListCreatures.empty())
            {
                for (Creature* l_Itr : l_ListCreatures)
                {
                    if (!l_Itr)
                        continue;

                    l_Itr->SetPhaseMask(4, true);
                }
            }

			DespawnAllAucheniDraeneis();

            /// Remove the auchenai shield npc
            if (Creature* l_Nearest = me->FindNearestCreature(eAuchindounCreatures::CreatureAuchenaiShield, 300.0f))
                l_Nearest->DespawnOrUnsummon();

            Talk(eKaatharTalks::VigilantKaatherDeath);  
            me->m_Events.AddEvent(new EventPostKaathar(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);   

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
                    Position l_Position;
                    me->GetRandomNearPosition(l_Position, 20.0f);
                    me->SummonCreature(eKaatharCreatures::TriggerHallowedGround, l_Position, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eKaatharEvents::EventHallowedGround, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eKaatharEvents::EventHolyShield:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        float l_X = l_Target->m_positionX + 10 * cos(l_Target->m_orientation);
                        float l_Y = l_Target->m_positionY + 10 * sin(l_Target->m_orientation);

                        Talk(eKaatharTalks::VigilantKaatherSpell2);
                        me->CastSpell(l_Target, eKaatharSpells::SpellHolyShieldThrow);
                        if (me->HasAura(eKaatharSpells::SpellHolyShieldOffHandDisarm))
                            me->AddAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm, me);
                        else
                            me->AddAura(eKaatharSpells::SpellHolyShieldOffHandDisarm, me);

                        std::string l_Str;
                        l_Str += "Vigilant kaathar hurls his |cffff0000[Holy Shield]|cfffaeb00! at ";
                        l_Str += l_Target->GetName();
                        me->MonsterTextEmote(l_Str.c_str(), me->GetGUID(), true);                
                        events.ScheduleEvent(eKaatharEvents::EventConsecratedLight, 4 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eKaatharEvents::EventHolyShieldReturn, 14 * TimeConstants::IN_MILLISECONDS);
                    }
                    events.ScheduleEvent(eKaatharEvents::EventHolyShield, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eKaatharEvents::EventHolyShieldReturn:
                {
                    if (Creature* l_Shield = me->FindNearestCreature(eKaatharCreatures::TriggerHolyShield, 50.0f, true))
                    {
                        me->SetCurrentEquipmentId(1); /// Equipment Id
                        l_Shield->DespawnOrUnsummon(3 * TimeConstants::IN_MILLISECONDS);
                        l_Shield->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 20.0f, 10.0f, 10.0f);
                        DespawnCreaturesInArea(eKaatharCreatures::TriggerHolyShield, me);

                        if (me->HasAura(eKaatharSpells::SpellHolyShieldOffHandDisarm))
                            me->RemoveAura(eKaatharSpells::SpellHolyShieldOffHandDisarm);
                        else if (me->HasAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm))
                            me->RemoveAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm);
                 
                        /// Despawn previous Holy Shields.
                        std::list<AreaTrigger*> l_ListAreaTriggers;
                        me->GetAreaTriggerList(l_ListAreaTriggers, eKaatharSpells::SpellHolyShieldAreaTrigger);
                        if (!l_ListAreaTriggers.empty())
                        {
                            for (AreaTrigger* l_Itr : l_ListAreaTriggers)
                                l_Itr->Remove(4 * TimeConstants::IN_MILLISECONDS);
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
                    for (uint8 l_I = 0; l_I <= 10; l_I++)
                    {
                        if (l_I == 9 || l_I == 5 || l_I == 6)
                            l_Orientation = l_Orientation / 4;
                        me->SummonCreature(eKaatharCreatures::TriggerFissureSummoner, l_PosX, l_PosY, me->GetPositionZ(), l_Orientation, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);
                        l_PosX += frand(0.5f, 1.8f);
                        l_PosY += frand(0.7f, 1.9f);
                    }
                    for (uint8 l_I = 0; l_I <= 4; l_I++)
                    {
                        l_PosX -= 3;
                        l_PosY -= float(2.7);
                        me->SummonCreature(eKaatharCreatures::TriggerFissureSummoner, l_PosX, l_PosY, me->GetPositionZ(), l_Orientation / 2 + l_I, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);
                    }
                    events.ScheduleEvent(eKaatharEvents::EventSanctifiedStrike, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_kaatharAI(p_Creature);
    }
};

/// Hallowed Ground - 537324
class auchindoun_kaathar_mob_hallowed_ground : public CreatureScript
{
    public:

    auchindoun_kaathar_mob_hallowed_ground() : CreatureScript("auchindoun_kaathar_mob_hallowed_ground") { }

    struct auchindoun_kaathar_mob_hallowed_groundAI : public Scripted_NoMovementAI
    {
        auchindoun_kaathar_mob_hallowed_groundAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_First = false;
        }

        uint32 m_VisualDiff;
        bool m_HasExploded;
        bool m_First;

        void Reset() override
        {
            events.Reset();
            m_HasExploded = false;
            me->setFaction(HostileFaction);
            m_VisualDiff = 1 * TimeConstants::IN_MILLISECONDS;
            events.ScheduleEvent(eKaatharEvents::EventCheckPlayer, 4 * TimeConstants::IN_MILLISECONDS); // Takes 4 seconds to charge

            if (!m_First)
            {
                me->SetObjectScale(1.0f);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eKaatharActions::ActionFateHallowedGround:
                DoCast(me, eKaatharSpells::SpellHallowedGround);
                me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                break;
            default:
                break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            // Visual
            if (m_VisualDiff <= p_Diff)
            {
                me->CastSpell(me, eKaatharSpells::SpellHallowedGroundsTriggerMissile);
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
            default:
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_kaathar_mob_hallowed_groundAI(p_Creature);
    }
};

/// Fissure Spawner Trigger - 543536
class auchindoun_kaathar_mob_spawn_fissures : public CreatureScript
{
public:
    auchindoun_kaathar_mob_spawn_fissures() : CreatureScript("auchindoun_kaathar_mob_spawn_fissures") { }

    struct auchindoun_kaathar_mob_spawn_fissuresAI : public Scripted_NoMovementAI
    {
        auchindoun_kaathar_mob_spawn_fissuresAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->CastSpell(me, eKaatharSpells::SpellSanctifiedStrikeAreaTrigger4);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_kaathar_mob_spawn_fissuresAI(p_Creature);
    }
};


/// Holy Shield - 76071
class auchindoun_kaathar_mob_holy_shield : public CreatureScript
{
public:
    auchindoun_kaathar_mob_holy_shield() : CreatureScript("auchindoun_kaathar_mob_holy_shield") { }

    struct auchindoun_kaathar_mob_holy_shieldAI : public Scripted_NoMovementAI
    {
        auchindoun_kaathar_mob_holy_shieldAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->CastSpell(me, eKaatharSpells::SpellHolyShieldLos);    
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);      
            /// Knockback
            std::list<Player*> l_ListPlayers;
            me->GetPlayerListInGrid(l_ListPlayers, 5.0f);
            if (!l_ListPlayers.empty())
            {
                for (Player* l_itr : l_ListPlayers)
                {
                    if (!l_itr)
                        continue;

                    l_itr->CastSpell(l_itr, eKaatharSpells::SpellHolyShieldKnockBack);
                    me->CastSpell(l_itr, eKaatharSpells::SpellHolyShieldDamageFromKnockBack);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_kaathar_mob_holy_shieldAI(p_Creature);
    }
};

/// Consecrated Light - 153006
class auchindoun_kaathar_spell_consecrated_light : public SpellScriptLoader
{
public:

    auchindoun_kaathar_spell_consecrated_light() : SpellScriptLoader("auchindoun_kaathar_spell_consecrated_light") { }

    class auchindoun_kaathar_spell_consecrated_light_SpellScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_kaathar_spell_consecrated_light_SpellScript);

        void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
        {
            if (GetCaster())
            {
                if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
                {
                    std::list<Player*> l_ListPlayers;
                    GetCaster()->GetPlayerListInGrid(l_ListPlayers, 200.0f);
                    if (!l_ListPlayers.empty())
                    {
                        for (Player* l_Itr : l_ListPlayers)
                        {
                            if (!l_Itr)
                                continue;

                            l_Itr->CastSpell(l_Itr, eKaatharSpells::SpellConsecratedLightDamage, true);
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_kaathar_spell_consecrated_light_SpellScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_kaathar_spell_consecrated_light_SpellScript();
    }
};

/// Consecrated Light - 156746
class auchindoun_kaathar_spell_conscreated_damage : public SpellScriptLoader
{
    public:

    auchindoun_kaathar_spell_conscreated_damage() : SpellScriptLoader("auchindoun_kaathar_spell_conscreated_damage") { }

    class auchindoun_kaathar_spell_conscreated_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_kaathar_spell_conscreated_damage_SpellScript);

        void RecalculateDamage(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster() && !GetHitUnit())
                return;

              if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
              {
                  if (!GetHitDamage())
                      return;

                  if (Creature* l_Kaathar = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                      if (Creature* l_Shield = GetCaster()->FindNearestCreature(eKaatharCreatures::TriggerHolyShield, 200.0f))
                          if (l_Shield->IsInBetween(GetHitUnit(), l_Kaathar))
                              SetHitDamage(0);
              }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_kaathar_spell_conscreated_damage_SpellScript::RecalculateDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_kaathar_spell_conscreated_damage_SpellScript();
    }
};

/// Sanctified Ground Periodic Dummy - 153430
class auchindoun_kaathar_spell_sanctified_ground : public SpellScriptLoader
{
    public:

    auchindoun_kaathar_spell_sanctified_ground() : SpellScriptLoader("auchindoun_kaathar_spell_sanctified_ground") { }

    class auchindoun_kaathar_spell_sanctified_ground_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_kaathar_spell_sanctified_ground_AuraScript);

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
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_kaathar_spell_sanctified_ground_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_kaathar_spell_sanctified_ground_AuraScript();
    }
};

/// Fate - 157465
class auchindoun_kaathar_spell_fate : public SpellScriptLoader
{
   public: 

       auchindoun_kaathar_spell_fate() : SpellScriptLoader("auchindoun_kaathar_spell_fate") { }

    class auchindoun_kaathar_spell_fate_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_kaathar_spell_fate_SpellScript);

        void HandleDummy(SpellEffIndex p_EffIndex)
        {
            if (Unit* l_Caster = GetCaster())
            {
                std::list<Creature*> l_HallowedGroundCreatures;
                GetCaster()->GetCreatureListWithEntryInGrid(l_HallowedGroundCreatures, eKaatharCreatures::TriggerHallowedGround, 150.0f);
                if (!l_HallowedGroundCreatures.empty())
                {
                    for (Creature* l_Itr : l_HallowedGroundCreatures)
                    {
                        if (l_Itr->IsAIEnabled)
                            l_Itr->GetAI()->DoAction(eKaatharActions::ActionFateHallowedGround);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_kaathar_spell_fate_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_kaathar_spell_fate_SpellScript();
    }
};

/// Sanctified Strike Fissure - 
class auchindoun_kaathar_at_fissure : public AreaTriggerEntityScript
{
    public:

    auchindoun_kaathar_at_fissure() : AreaTriggerEntityScript("auchindoun_kaathar_at_fissure")
    {
    }

    uint32 m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Diff <= p_Time)
        {
            std::list<Player*> l_PlayerList;
            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 1.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_PlayerList, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (!l_PlayerList.empty())
            {
                for (std::list<Player*>::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (!(*l_Itr))
                        continue;

                    if (!(*l_Itr)->HasAura(eKaatharSpells::SpellSanctifiedGroundAura))
                        (*l_Itr)->CastSpell((*l_Itr), eKaatharSpells::SpellSanctifiedGroundAura);
                }
            }

            m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
        }
        else
            m_Diff -= p_Time;
    }

    auchindoun_kaathar_at_fissure* GetAI() const override
    {
        return new auchindoun_kaathar_at_fissure();
    }
};

/// Hallowed Ground  - 
class auchindoun_kaathar_at_hallowed_ground : public AreaTriggerEntityScript
{
    public:

    auchindoun_kaathar_at_hallowed_ground() : AreaTriggerEntityScript("auchindoun_kaathar_at_hallowed_ground")
    {
    }

    float m_Size;
    uint32 m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
    uint32 m_Explosion = 15 * TimeConstants::IN_MILLISECONDS;
    std::list<uint64> m_Targets;

    void OnCreate(AreaTrigger* p_AreaTrigger)
    {
        m_Size = 3.0f;
        p_AreaTrigger->SetObjectScale(m_Size);
    }

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Diff <= p_Time)
        {
            m_Size -= float(0.3f);
            p_AreaTrigger->SetObjectScale(m_Size);

            std::list<Player*> l_PlayerList;
            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_PlayerList, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (!l_PlayerList.empty())
            {
                for (std::list<Player*>::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (!(*l_Itr))
                        continue;

                    if (!(*l_Itr)->HasAura(eKaatharSpells::SpellSanctifiedGroundAura))
                    {
                        (*l_Itr)->CastSpell((*l_Itr), eKaatharSpells::SpellSanctifiedGroundAura);
                        m_Targets.push_back((*l_Itr)->GetGUID());
                    }
                }
            }

            m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
        }
        else
            m_Diff -= p_Time;
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (!m_Targets.empty())
        {
            for (uint64 l_Guid : m_Targets)
            {
                if (!l_Guid)
                    continue;

                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eKaatharSpells::SpellSanctifiedGroundAura))
                    l_Target->RemoveAura(eKaatharSpells::SpellSanctifiedGroundAura);
            }
        }
    }

    auchindoun_kaathar_at_hallowed_ground* GetAI() const override
    {
        return new auchindoun_kaathar_at_hallowed_ground();
    }
};

void AddSC_boss_kaathar()
{
    new boss_kaathar();                                                 ///< 75839
    new auchindoun_kaathar_mob_spawn_fissures();                        ///< 543536
    new auchindoun_kaathar_mob_hallowed_ground();                       ///< 537324
    new auchindoun_kaathar_mob_holy_shield();                           ///< 76071
    new auchindoun_kaathar_mob_nyami();                                 ///< 77810
    new auchindoun_kaathar_spell_consecrated_light();                   ///< 153006
    new auchindoun_kaathar_spell_fate();                                ///< 157465
    new auchindoun_kaathar_spell_sanctified_ground();                   ///< 153430
    new auchindoun_kaathar_spell_conscreated_damage();                  ///< 156746
    new auchindoun_kaathar_at_fissure();                                ///< 165065
}