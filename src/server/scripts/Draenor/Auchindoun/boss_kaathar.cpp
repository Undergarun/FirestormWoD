
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
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
    SpellSanctifiedStrikeDummy         = 152954,
    SpellSanctifiedGroundTickDamage    = 161457,
    SpellSanctifiedGroundAura          = 153430,
    SpellSanctifiedGroundDamageTick    = 161457,
    SpellSanctifiedStrikeAreaTrigger   = 165064,
    SpellSanctifiedStrikeAreaTrigger2  = 163559,
    SpellSancitfiedStrikeAreaTrigger3  = 165055,
    SpellEmoteArcaneChanneling         = 32783,
    SpellFate                          = 157465,
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
    EventDecreaseSize,
};

enum eKaatharActions
{
    ActionActivateBoss = 1,
    ActionFateHallowedGround,
};

enum eKaatharTalks
{
    VigilantKaatherIntro  = 18,   ///< None Live p_Who Assault The Holy Auchenai. (46436)
    VigilantKaatherAgro   = 19,   ///< I Will Strike You Down. (46434)
    VigilantKaatherSpell1 = 20,   ///< Bathe In The Glory Of The Light! (46438)
    VigilantKaatherSpell2 = 21,   ///< The Light Guide My Hand! (46439)
    VigilantKaatherKill   = 22,   ///< Light Guide You. (46434)
    VigilantKaatherDeath  = 23,   ///< Auchindoun...Is...Doomed...(46435)
};

enum eKaatharTriggers
{
    TriggerHallowedGround  = 537324,
    TriggerHolyShield      = 76071,
    TriggerFissureSummoner = 543536,
};

Position l_PositionNep_WhomePosition         = { 1908.63f, 3163.94f, 30.799f, 1.498386f };
Position l_PositionKaatharCombatJump         = { 1911.47f, 3152.13f, 30.972f, 1.550403f };
Position l_PositionMageSpawning              = { 1857.07f, 3167.93f, 29.422f, 0.275066f };
Position l_PositionTuulaniGobjectPortalSpawn = { 1912.11f, 3183.67f, 30.799f, 3.173182f };
Position l_PositionNyamiSpawn                = { 1660.17f, 2953.36f, 34.291f, 1.413091f };
Position l_PositionWardenAndGuards           = { 1666.69f, 3015.78f, 35.145f, 1.302555f };
Position l_PositionMagusp_WhoControlFootmans = { 1708.01f, 3107.09f, 35.139f, 6.217793f };
Position l_PositionWallInMiddleFromNyami     = { 1689.59f, 2953.11f, 35.613f, 3.139696f };
Position l_PositionNyamiEscape05 = { 1801.23f, 3178.36f, 35.139f };
Position l_PositionNyamiEscape04 = { 1912.99f, 3165.06f, 48.221f };
Position l_PositionNyamiEscape03 = { 1891.66f, 3184.09f, 49.976f };
Position l_PositionNyamiEscape02 = { 1911.14f, 3211.44f, 51.915f };
Position l_PositionNyamiEscape01 = { 1935.88f, 3185.43f, 44.528f };
Position l_PositionMageMoveTo    = { 1895.09f, 3183.77f, 30.799f };

Position l_PositionThreeCorpses[2] = 
{
    {1749.40f, 3145.85f, 35.140f, 3.690412f},
    {1751.31f, 3141.03f, 35.140f, 3.278078f},
};
Position l_PositionMagusAndDefenderHostile[2] =
{
    {1735.47f, 3139.28f, 35.140f, 0.185946f},
    {1738.86f, 3145.71f, 35.140f, 5.495237f},
};
Position l_PositionSargereiDefenders[2] =
{
    {1713.76f, 3104.75f, 35.140f, 0.902229f},
    {1710.27f, 3108.19f, 35.140f, 0.815836f},
};
Position l_PositionThreePrisoners[3] =
{
    {1700.59f, 3097.08f, 40.908f, 0.701154f},
    {1707.98f, 3103.10f, 42.982f, 1.499199f},
    {1714.57f, 3111.26f, 43.196f, 3.066774f},
};
Position l_PositionThreeHostileArbitrerMagusSoulPriest[3] =
{
    { 1671.63f, 3042.13f, 35.138f, 0.704871f },
    { 1678.88f, 3040.05f, 35.138f, 1.837415f },
    { 1668.86f, 3034.10f, 35.139f, 1.298632f },
};
Position l_PositionCorpsesNearNyomi[12] = 
{
    {1673.98f, 2971.02f, 35.151f, 4.399558f},
    {1671.73f, 2971.23f, 35.200f, 4.517365f},
    {1674.24f, 2967.03f, 35.194f, 4.327302f},
    {1648.88f, 2967.87f, 35.211f, 4.877079f},
    {1651.50f, 2968.30f, 35.219f, 4.888861f},
    {1649.75f, 2937.92f, 35.215f, 1.334930f},
    {1644.28f, 2937.78f, 35.173f, 1.460592f},
    {1672.23f, 2938.41f, 35.208f, 1.825676f},
    {1674.24f, 2933.18f, 35.131f, 1.916782f},
    {1669.27f, 2931.85f, 34.764f, 1.751064f},
};
Position l_PositionFourMagesThatSpawnAfterKaatharIsKaaput[5] = 
{
    {1846.22f, 3198.23f, 29.424f, 6.066411f},
    {1849.56f, 3203.73f, 29.424f, 6.023211f},
    {1844.11f, 3170.28f, 29.410f, 6.176361f},
    {1848.42f, 3162.75f, 29.423f, 0.003129f},
};
Position l_PositionAfterKaatharDeathForFourMages[4] =
{
    {1879.55f, 3201.58f, 34.418f},
    {1876.59f, 3191.82f, 31.418f},
    {1876.75f, 3171.40f, 31.418f},
    {1877.25f, 3165.11f, 31.418f},
};
Position l_PositionGuardsAndWardens[2] =
{
    {1672.54f, 3012.73f, 35.139f, 1.318899f},
    {1659.39f, 3016.55f, 35.165f, 1.212869f},
};
Position l_PositionDefenderBehindMiddleWallOfNyami[4] = 
{
    {1696.44f, 2957.14f, 35.613f, 3.265362f},
    {2952.62f, 2952.80f, 35.613f, 3.120063f},
    {1695.18f, 2947.33f, 35.613f, 3.040738f},
    {1692.70f, 2953.55f, 35.613f, 3.140484f},
};

class tuulani_nyami_escape_2nd : public BasicEvent
{
public:
    explicit tuulani_nyami_escape_2nd(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value)
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
                                        l_Tuulani->m_Events.AddEvent(new tuulani_nyami_escape_2nd(l_Tuulani, 101), l_Tuulani->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 101:
                                    {
                                        l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK7);
                                        l_Tuulani->m_Events.AddEvent(new tuulani_nyami_escape_2nd(l_Tuulani, 0), l_Tuulani->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 0:
                                    {

                                        l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK8);
                                        l_Nyami->GetMotionMaster()->MovePoint(5, l_PositionNyamiEscape01);
                                        break;
                                    }
                                    case 1:
                                    {
                                        l_Nyami->GetMotionMaster()->MovePoint(6, l_PositionNyamiEscape02);
                                        break;
                                    }
                                    case 2:
                                    {
                                        l_Nyami->RemoveAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison);
                                        l_Nyami->AddAura(eAuchindonSpells::SpellDispersionVisualNyami, l_Nyami);
                                        l_Nyami->SetSpeed(UnitMoveType::MOVE_RUN, 10.0f, true);
                                        l_Nyami->CastSpell(l_Nyami, eAuchindonSpells::SpellCrystalEarthquake);
                                        l_Nyami->GetMotionMaster()->MovePoint(7, l_PositionNyamiEscape03);

                                        l_Kaathar->setFaction(eAuchindonInformation::InformationHostileFaction);
                                        l_Kaathar->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                                        l_Kaathar->GetMotionMaster()->MoveJump(l_PositionKaatharCombatJump.GetPositionX(), l_PositionKaatharCombatJump.GetPositionY(), l_PositionKaatharCombatJump.GetPositionZ(), 10.0f, 10.0f, 10.0f, 0);
                                        break;
                                    }
                                    case 3:
                                    {
                                        l_Nyami->GetMotionMaster()->MovePoint(8, l_PositionNyamiEscape04);
                                        break;
                                    }
                                    case 4:
                                    {
                                        l_Tuulani->m_Events.AddEvent(new tuulani_nyami_escape_2nd(l_Tuulani, 6), l_Tuulani->m_Events.CalculateTime(500));
                                        break;
                                    }
                                    case 6:
                                    {
                                        l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK10);
                                        l_Tuulani->AddAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison, l_Tuulani);
                                        l_Tuulani->m_Events.AddEvent(new tuulani_nyami_escape_2nd(l_Tuulani, 7), l_Tuulani->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 7:
                                    {
                                        l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK11);
                                        l_Tuulani->m_Events.AddEvent(new tuulani_nyami_escape_2nd(l_Tuulani, 8), l_Tuulani->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 8:
                                    {
                                        l_Tuulani->AI()->Talk(eAuchindonTalk::TUULANITALK9);
                                        l_Tuulani->AddAura(eAuchindonSpells::SpellTuulaniCapturedVoidPrison, l_Tuulani);

                                        l_Nyami->GetMotionMaster()->MovePoint(9, l_PositionNyamiEscape05);
                                        l_Nyami->DespawnOrUnsummon(10 * TimeConstants::IN_MILLISECONDS);                                  
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    return true;
                }
            }
        }
    }

private:
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};


class kaathar_death_post_nyami : public BasicEvent
{
public:
    explicit kaathar_death_post_nyami(Unit* unit, int value) : m_Obj(unit), m_Modifier(value)
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
                                l_Tuulani->AI()->Talk(TUULANITALK14);
                                l_Tuulani->RemoveAura(154840); // void prison

                                for (int32 i = 0; i < 4; i++)
                                {
                                    if (Creature* l_Defender = m_Obj->SummonCreature(CreatureAucheniDefender, l_PositionFourMagesThatSpawnAfterKaatharIsKaaput[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Defender->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 505);
                                        l_Defender->GetMotionMaster()->MovePoint(0, l_PositionFourMagesThatSpawnAfterKaatharIsKaaput[i]);
                                    }
                                }

                                if (Creature* l_Magus = m_Obj->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, l_PositionMageSpawning, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                {
                                    l_Magus->GetMotionMaster()->MovePoint(0, l_PositionMageMoveTo);
                                    l_Magus->m_Events.AddEvent(new kaathar_death_post_nyami(l_Magus, 1), l_Magus->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                }
                                break;
                            }
                            case 1:
                            {                        
                                l_Tuulani->SummonCreature(eAuchindonBosses::BossNyami, l_PositionNyamiSpawn, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                                // Corpses
                                for (int32 i = 0; i < 2; i++)
                                {
                                    if (Creature* l_Corpses = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, l_PositionThreeCorpses[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Corpses->SetReactState(ReactStates::REACT_PASSIVE);
                                        l_Corpses->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH | eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                        l_Corpses->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                        l_Corpses->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                        l_Corpses->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                    }
                                }

                                l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiDefender, l_PositionMagusAndDefenderHostile[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);

                                // Hostile near Two corpses          
                                if (Creature* l_Magus = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiMagus, l_PositionMagusAndDefenderHostile[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                {
                                    l_Magus->CastSpell(l_Magus, 157794);
                                }

                                // Two defender
                                for (int32 i = 0; i < 3; i++)
                                {
                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiDefender, l_PositionSargereiDefenders[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                }

                                // Magus p_Who control footmans                            
                                if (Creature* Magusp_WhoControlFootman = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiMagus, l_PositionMagusp_WhoControlFootmans, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                {
                                    Magusp_WhoControlFootman->CastSpell(Magusp_WhoControlFootman, SpellArcaneChanneling);
                                }

                                // Three prisonners
                                for (int32 i = 0; i < 3; i++)
                                {                           
                                    if (Creature* l_Prisoners = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, l_PositionThreePrisoners[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        l_Prisoners->SetCanFly(true);
                                        l_Prisoners->SetDisableGravity(true);
                                        l_Prisoners->CastSpell(l_Prisoners, eAuchindonSpells::SpellPrisonAura);
                                        l_Prisoners->CastSpell(l_Prisoners, eAuchindonSpells::SpellStrangulate);
                                        l_Prisoners->SetReactState(ReactStates::REACT_PASSIVE);                   
                                        l_Prisoners->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                        l_Prisoners->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                        l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,  eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                        l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                    }
                                }

                                // Twelve prisoners (cosmetic)
                                for (int32 i = 0; i <= 11; i++)
                                {                              
                                    if (Creature* l_Prisoners = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, l_PositionCorpsesNearNyomi[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);)
                                    {
                                        l_Prisoners->SetReactState(REACT_PASSIVE);
                                        l_Prisoners->SetReactState(ReactStates::REACT_PASSIVE);
                                        l_Prisoners->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                        l_Prisoners->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                        l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                        l_Prisoners->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN | eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
                                    }
                                }

                                // Pack of arbitrer and magus and defender.
                                l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSpitefulArbitrerTrash, l_PositionThreeHostileArbitrerMagusSoulPriest[0], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargeriMagus, l_PositionThreeHostileArbitrerMagusSoulPriest[1], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargeriSoulPriest, l_PositionThreeHostileArbitrerMagusSoulPriest[2], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);

                                // Warden
                                l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargeriWarden, l_PositionWardenAndGuards, TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                     
                                // Defenders
                                for (int32 i = 0; i < 3; i++)
                                {
                                    l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureSargereiDefender, l_PositionGuardsAndWardens[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN);
                                }

                                // Holy Wall, Object
                                l_Tuulani->SummonGameObject(eAuchindonObjects::GameobjectHolyWall, l_PositionWallInMiddleFromNyami.GetPositionX(), l_PositionWallInMiddleFromNyami.GetPositionY(), l_PositionWallInMiddleFromNyami.GetPositionZ(), l_PositionWallInMiddleFromNyami.GetOrientation(), 0, 0, 0, 0, 0);

                                for (int32 i = 0; i < 4; i++)
                                {
                                    if (Creature* l_Defenders = l_Tuulani->SummonCreature(eAuchindonCreatures::CreatureAucheniWarden, l_PositionDefenderBehindMiddleWallOfNyami[i], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                                    {
                                        if (Creature* l_Stalker = l_Defenders->FindNearestCreature(eAuchindonCreatures::CreatureLightWallTargets, 10.0f))
                                            l_Defenders->CastSpell(l_Stalker, SpellHolyBeam);
                                    } 
                                }

                                l_Tuulani->AI()->Talk(TUULANITALK15);

                                // Magus
                                m_Obj->CastSpell(m_Obj, SpellEmoteArcaneChanneling);
                                m_Obj->SummonGameObject(GameobjectTaladorPortal, l_PositionTuulaniGobjectPortalSpawn.GetPositionX(), l_PositionTuulaniGobjectPortalSpawn.GetPositionY(), l_PositionTuulaniGobjectPortalSpawn.GetPositionZ(), l_PositionTuulaniGobjectPortalSpawn.GetOrientation(), 0, 0, 0, 0, 0);
                                m_Obj->m_Events.AddEvent(new kaathar_death_post_nyami(m_Obj, 2), m_Obj->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));           
                                break;
                            }
                        }
                        return true;
                    }
                }
            }
        }
    }
  private:
      Unit* m_Obj;
      int m_Modifier;
      int m_Event;
      std::list<Creature*> l_Dispeonsor;
};

/// Nyami Mob - 
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
        bool m_StartEvent;

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eAuchindonMovementInform::MovementInformNyamiEscape05:
                    me->m_Events.AddEvent(new tuulani_nyami_escape_2nd(me, 1), me->m_Events.CalculateTime(200));
                    break;
                case eAuchindonMovementInform::MovementInformNyamiEscape06:
                    me->m_Events.AddEvent(new tuulani_nyami_escape_2nd(me, 2), me->m_Events.CalculateTime(200));
                    break;
                case eAuchindonMovementInform::MovementInformNyamiEscape07:
                    me->m_Events.AddEvent(new tuulani_nyami_escape_2nd(me, 3), me->m_Events.CalculateTime(200));
                    break;
                case eAuchindonMovementInform::MovementInformNyamiEscape08:
                    me->m_Events.AddEvent(new tuulani_nyami_escape_2nd(me, 4), me->m_Events.CalculateTime(200));
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

/// Vigilant Kaathar - 
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

            me->SetCurrentEquipmentId(1); // Equipment Id              
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

                me->SetHomePosition(l_PositionNewHome);
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
            Talk(eKaatharTalks::VigilantKaatherAggro);

            ActivateDoors();

            if (me->GetMap() && me->GetMap()->IsHeroic())
                events.ScheduleEvent(EventFate,           45 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(EventHallowedGround,     urand(12 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(EventSanctifiedStrike,   8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(EventHolyShield,         30 * TimeConstants::IN_MILLISECONDS);

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
                        me->m_Events.AddEvent(new tuulani_2nd_event_nyaomi_escape(me, 100), me->m_Events.CalculateTime(500));
                    }
                    break;
            }
        }

        void DespawnAllAucheniDraeneis()
        {
            int32 l_Entries[11] = {CreatureAucheniMagus2, CreatureAucheniVigiliant, CreatureAucheniHoplite, CreatureAucheniZealot, CreatureAucheniWarden, CreatureAucheniRitualist, CreatureAucheniMagus, CreatureAucheniSoulPriest, CreatureAucheniArbiter, CreatureAucheniCleric, CreatureAucheniDefender};

            for (int32 i = 0; i < 12; i++)
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

            me->m_Events.AddEvent(new kaathar_death_post_nyami(me, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
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
                    Position l_Position;
                    me->GetRandomNearPosition(l_Position, 40.0f);

                    if (Creature* l_Trigger = me->SummonCreature(eKaatharTriggers::TriggerHallowedGround, l_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
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

                        me->SetCurrentEquipmentId(1); // Equipment Id

                        if (me->HasAura(eKaatharSpells::SpellHolyShieldOffHandDisarm))
                            me->RemoveAura(eKaatharSpells::SpellHolyShieldOffHandDisarm);
                        else if (me->HasAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm))
                            me->RemoveAura(eKaatharSpells::SpellHolyShieldTwoHandDisarm);

                        DespawnCreaturesInArea(eKaatharTriggers::TriggerHolyShield, me);

                        // Despawn previous Holy Shields.
                        std::list<AreaTrigger*> l_ListAreaTriggers;
                        me->GetAreaTriggerList(l_ListAreaTriggers, eKaatharSpells::153000);
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
                        l_PosY -= 2.7;

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

/// Sanctified Strike Fissure - 
class auchindon_kaathar_areatrigger_fissure : public AreaTriggerEntityScript
{
public:
    auchindon_kaathar_areatrigger_fissure() : AreaTriggerEntityScript("auchindon_kaathar_areatrigger_fissure")
    {
    }

    int m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
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
                        AuraPtr l_Aura = (*itr)->GetAura(eKaatharSpells::SpellSanctifiedGroundAura);

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

/// Hallowed Ground - 
class auchindon_trigger_hallowed_ground : public CreatureScript
{
public:
    auchindon_trigger_hallowed_ground() : CreatureScript("auchindon_trigger_hallowed_ground") { }

    struct trigger_hallowed_groundAI : public Scripted_NoMovementAI
    {
        trigger_hallowed_groundAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {        
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(ReactStates::REACT_PASSIVE);

            me->SetObjectScale(1.0f);
        }

        bool m_HasExploded;

        void Reset() override
        {
            events.Reset();
            me->setFaction(eAuchindonInformation::InformationHostileFaction);
     
            m_HasExploded = false;
            events.ScheduleEvent(eKaatharEvents::EventCheckPlayer, 4 * TimeConstants::IN_MILLISECONDS); // Takes 4 seconds to charge
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
                case eKaatharEvents::EventCheckPlayer:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new trigger_hallowed_groundAI(p_Creature);
    }
};

/// Holy Shield - 
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
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

            me->CastSpell(me, SpellHolyShieldLos);

            // Knockback
            std::list<Player*> l_ListPlayers;
            me->GetPlayerListInGrid(l_ListPlayers, 5.0f);

            if (l_ListPlayers.empty())
                return;

            for (auto itr : l_ListPlayers)
            {
                if (!itr)
                    return;

                itr->CastSpell(itr, SpellHolyShieldKnockBack);
                me->CastSpell(itr, SpellHolyShieldDamageFromKnockBack);
            }
        }

        void UpdateAI(uint32 const p_Diff)
        {
            events.Update(p_Diff);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new trigger_holy_shieldAI(p_Creature);
    }
};

/// Fissure Spawner Trigger - 
class auchindon_trigger_spawn_fissures : public CreatureScript
{
public:
    auchindon_trigger_spawn_fissures() : CreatureScript("auchindon_trigger_spawn_fissures") { }

    struct trigger_spawn_fissuresAI : public Scripted_NoMovementAI
    {
        trigger_spawn_fissuresAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(eAuchindonInformation::InformationHostileFaction);
            me->SetDisplayId(eAuchindonInformation::InformationDisplayIdInvis);
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->CastSpell(me, 165065);
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

/// Consecrated Light - 
class auchindon_spell_consecrated_light : public SpellScriptLoader
{
public:
    auchindon_spell_consecrated_light() : SpellScriptLoader("auchindon_spell_consecrated_light") { }

    class auchindon_spells : public AuraScript
    {
        PrepareAuraScript(auchindon_spells);

        void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
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

                     itr->CastSpell(itr, SpellConsecratedLightDamage, true);
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

/// Consecrated Light - 
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
                if (Creature* l_Shield = GetCaster()->FindNearestCreature(TriggerHolyShield, 200.0f))
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

/// Sanctified Ground Periodic Dummy - 
class auchindon_spell_sanctified_ground_periodic_dummy : public SpellScriptLoader
{
public:
    auchindon_spell_sanctified_ground_periodic_dummy() : SpellScriptLoader("auchindon_spell_sanctified_ground_periodic_dummy") { }

    class auchindon_spells : public AuraScript
    {
        PrepareAuraScript(auchindon_spells);

        void HandlePeriodic(constAuraEffectPtr p_AurEff)
        {
            PreventDefaultAction();

            if (GetCaster() && GetTarget())
            {
                GetTarget()->CastSpell(GetTarget(), SpellSanctifiedGroundTickDamage);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_auras::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindon_spells();
    }
};

/// Fate - 
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
                GetCaster()->GetCreatureListWithEntryInGrid(l_HallowedGroundCreatures, TriggerHallowedGround, 150.0f);

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
            OnEffectLaunch += SpellEffectFn(auchindon_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindon_spells();
    }
};

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