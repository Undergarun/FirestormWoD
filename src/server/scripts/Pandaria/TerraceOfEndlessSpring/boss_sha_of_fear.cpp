////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GameObjectAI.h"
#include "GridNotifiers.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"

enum eShaOfFearSpells
{
    // Sha of Fear
    SPELL_BERSERK                   = 47008,
    SPELL_REACHING_ATTACK           = 119775,
    SPELL_EERIE_SKULL               = 119519,
    SPELL_ENERGY_TO_ZERO            = 119417,
    SPELL_BREATH_OF_FEAR            = 119414,
    SPELL_THRASH_AURA               = 131996,
    SPELL_THRASH_EXTRA_ATTACKS      = 131994,
    SPELL_CONJURE_TERROR_SPAWN_TICK = 119108,
    // 4 spells for spawn, cauz' of different spawn coordinates
    SPELL_CONJURE_TERROR_SPAWN_01   = 119312,
    SPELL_CONJURE_TERROR_SPAWN_02   = 119370,
    SPELL_CONJURE_TERROR_SPAWN_03   = 119371,
    SPELL_CONJURE_TERROR_SPAWN_04   = 119372,
    SPELL_OMINOUS_CACKLE_CAST       = 119593,
    SPELL_OMINOUS_CACKLE_TRANSFORM  = 129147,
    SPELL_TELEPORT_TO_SHRINE_1      = 129234,
    SPELL_TELEPORT_TO_SHRINE_2      = 129235,
    SPELL_TELEPORT_TO_SHRINE_3      = 129236,

    // Heroic Mode
    SPELL_LEI_SHIS_HOPE             = 129368,
    SPELL_DREAD_EXPANSE_AURA        = 120289,
    SPELL_FADING_LIGHT              = 129378,
    SPELL_CHAMPION_OF_LIGHT_HEROIC  = 120268,
    SPELL_TRANSFER_LIGHT            = 120285,
    SPELL_DREAD_THRASH_AURA         = 132007,
    SPELL_DREAD_THRASH_EXTRA_ATT    = 132000,
    SPELL_IMPLACABLE_STRIKE         = 120672,
    SPELL_WATERSPOUT_CHANNELING     = 120519,
    SPELL_WATERSPOUT_DAMAGE         = 120521,
    SPELL_NAKED_AND_AFRAID          = 120669,
    SPELL_HUDDLE_IN_TERROR          = 120629,
    SPELL_SUBMERGE_TRANSFORM        = 120455,
    SPELL_EMERGE_DAMAGE             = 120458,
    SPELL_EMERGE_STUN_ONE_SECOND    = 120475,

    // Other mobs
    SPELL_LIGHT_WALL                = 117865,
    SPELL_CHAMPION_OF_LIGHT         = 117866,
    SPELL_LIGHT_WALL_READY          = 117770,
    SPELL_LIGHT_WALL_VISUAL         = 107141,
    SPELL_FEARLESS                  = 118977,
    SPELL_WALL_OF_LIGHT_BUFF        = 117964,
    SPELL_PENETRATING_BOLT          = 129075,
    SPELL_PENETRATING_BOLT_MISSILE  = 129077,
    SPELL_DARK_BULWARK              = 119083,
    SPELL_PURE_LIGHT_VISUAL         = 120216,
    SPELL_SHOT                      = 119862,
    SPELL_DEATH_BLOSSOM             = 119888,
    SPELL_DEATH_BLOSSOM_RAND_ARROW  = 119890,
    SPELL_DREAD_SPRAY               = 120047,
    SPELL_DREAD_SPRAY_STACKS        = 119983,
    SPELL_DREAD_SPRAY_FEAR_ROOT     = 119985,
    SPELL_DREAD_SPRAY_TRIGGERED     = 119958,
    SPELL_DREAD_SPRAY_VISUAL        = 119956,
    SPELL_SHA_GLOBE_SPAWN           = 129178,
    SPELL_SHA_GLOBE_PERIODIC_VISUAL = 129187,
    SPELL_SHA_GLOBE_HEAL_SUMMONER   = 129190,
    SPELL_SHA_CORRUPTION            = 120000,
    SPELL_ETERNAL_DARKNESS          = 120394,
    SPELL_GATHERING_SPEED           = 128419,
    SPELL_SHA_SPINES                = 120438,

    SPELL_SHA_OF_FEAR_BONUS         = 132203
};

enum eShaOfFearEvents
{
    EVENT_CHECK_MELEE       = 1,
    EVENT_EERIE_SKULL       = 2,
    EVENT_CHECK_ENERGY      = 3,
    EVENT_FIRST_TERRORS     = 4,
    EVENT_PENETRATING_BOLT  = 5,
    EVENT_OMINOUS_CACKLE    = 6,
    EVENT_ENRAGE            = 7,
    EVENT_CHECK_GUARDIAN    = 8,
    EVENT_SHOT              = 9,
    EVENT_DEATH_BLOSSOM     = 10,
    EVENT_DREAD_SPRAY       = 11,
    EVENT_DESPAWN_SHA_GLOBE = 12,

    // Heroic
    EVENT_IMPLACABLE_STRIKE = 13,
    EVENT_NAKED_AND_AFRAID  = 14,
    EVENT_WATERSPOUT        = 15,
    EVENT_HUDDLE_IN_TERROR  = 16,
    EVENT_SUBMERGE          = 17,
    EVENT_SPAWN_DREAD       = 18,
    EVENT_EMERGE            = 19,
    EVENT_GATHERING_SPEED   = 20,
    EVENT_TO_DREAD_EXPANSE  = 21,
    EVENT_CLEAR_AURA_PLAYER = 22,
    EVENT_VISIBLE_EXPANSE   = 23,
    EVENT_CHECK_CHAMPION    = 24
};

enum eShaOfFearActions
{
    ACTION_ACTIVATE_WALL_OF_LIGHT,
    ACTION_DESACTIVATE_WALL_OF_LIGHT,
    ACTION_SPAWN_TERROR,
    ACTION_ACTIVATE_SHRINE_1,
    ACTION_ACTIVATE_SHRINE_2,
    ACTION_ACTIVATE_SHRINE_3,
    ACTION_DESACTIVATE_SHRINE_1,
    ACTION_DESACTIVATE_SHRINE_2,
    ACTION_DESACTIVATE_SHRINE_3,
    ACTION_TRANSFERT_OF_LIGHT,
    ACTION_RETURNS_PLAYERS,
    ACTION_ACTIVATE_RETURN,
    ACTION_ARRIVE_IN_DREAD_EXPANSE,
    ACTION_TELEPORT_SHA,
    ACTION_GET_PLAYERS_BACK
};

enum eShaOfFearSays
{
    TALK_INTRO,             // Your lights will go out. The darkness will envelop you. And you will fear the shadows that move within it.
    TALK_AGGRO,             // You will know fear!
    TALK_BREATH_OF_FEAR,    // Fleeeee!
    TALK_SLAY,              // Drown in terror.
    TALK_HEROIC,            // You think it is so easy to vanquish your fears?
    TALK_SUBMERGE,          // Dread rises! It seeks the light. It hungers.
    TALK_HUDDLE             // Huddle in terror!
};

enum eEquipsId
{
    EQUIP_ID_GUARDIAN   = 72283
};

Player* GetChampionOfLight(Creature* me)
{
    if (!me)
        return NULL;

    Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        if (Player* player = itr->getSource())
            if (player->HasAura(SPELL_CHAMPION_OF_LIGHT))
                return player;

    return NULL;
}

Position spawnTerrorPos[4] =
{
    { -1052.588f, -2788.164f, 38.268f, 0.0f },
    { -983.4550f, -2787.942f, 38.269f, 0.0f },
    { -989.6860f, -2772.245f, 38.303f, 0.0f },
    { -1046.274f, -2772.215f, 38.303f, 0.0f }
};

Position shrinesPos[3] =
{
    { -832.076f,  -2745.400f, 31.678f, 0.0f }, // North
    { -1075.200f, -2577.820f, 15.852f, 0.0f }, // South/West
    { -1214.801f, -2824.821f, 41.243f, 0.0f }  // South
};

Position returnPos[2] =
{
    { -1057.645f, -2706.035f, 37.736f, 5.261f },
    { -978.451f,  -2706.773f, 37.737f, 4.134f }
};

Position heroicSha = { -1597.43f, -3815.56f, -279.50f, 3.141593f }; // Position for Sha in Heroic P2
Position heroicPos = { -1722.40f, -3815.56f, -279.50f, 0.000000f }; // Base position for players in Heroic P2

// 60999 - Sha of fear
class boss_sha_of_fear : public CreatureScript
{
    public:
        boss_sha_of_fear() : CreatureScript("boss_sha_of_fear") { }

        struct boss_sha_of_fearAI : public BossAI
        {
            boss_sha_of_fearAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_FEAR)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 attacksCounter;
            uint8 terrorCounter;
            uint8 submergeCounter;

            bool shrine1;
            bool shrine2;
            bool shrine3;
            bool isInSecondPhase;
            bool submerged;
            bool isInTeleport;
            bool introDone;

            uint8 healthPctForSecondPhase;

            void Reset()
            {
                if (pInstance && pInstance->GetBossState(DATA_PROTECTORS) != DONE)
                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 35);

                if (pInstance && pInstance->GetBossState(DATA_LEI_SHI) != DONE)
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetReactState(REACT_PASSIVE);
                    return;
                }

                _Reset();

                me->ReenableEvadeMode();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->AddUnitState(UNIT_STATE_CANNOT_TURN);
                me->SetPower(POWER_ENERGY, 0);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->CastSpell(me, SPELL_ENERGY_TO_ZERO, true);
                if (!me->IsVisible())
                    me->SetVisible(true);
 
                summons.DespawnAll();

                me->RemoveAura(SPELL_CONJURE_TERROR_SPAWN_TICK);
                me->RemoveAura(SPELL_THRASH_AURA);

                events.Reset();

                attacksCounter  = 0;
                terrorCounter   = 0;
                submergeCounter = 0;
                shrine1         = false;
                shrine2         = false;
                shrine3         = false;
                isInSecondPhase = false;
                submerged       = false;
                isInTeleport    = false;
                introDone       = false;
                healthPctForSecondPhase = 67;

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_EXPANSE_AURA);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEARLESS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_REACHING_ATTACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT_HEROIC);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);

                    if (IsHeroic())
                    {
                        // Removing Lei Shi's hope's aura if we're not on a new try
                        if (pInstance->GetBossState(DATA_SHA_OF_FEAR) != FAIL)
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LEI_SHIS_HOPE);
                        // Else, we ensure each players of the raid has the buff Lei Shi's hope
                        else
                            pInstance->DoAddAuraOnPlayers(SPELL_LEI_SHIS_HOPE);
                    }
 
                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);

                    if (Creature* wallOfLight = Creature::GetCreature(*me, pInstance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                        wallOfLight->AI()->DoAction(ACTION_DESACTIVATE_WALL_OF_LIGHT);
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();
 
                if (pInstance)
                    if (pInstance->IsWipe())
                        pInstance->SetBossState(DATA_SHA_OF_FEAR, FAIL);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (pInstance)
                {
                    if (!pInstance->CheckRequiredBosses(DATA_SHA_OF_FEAR))
                    {
                        EnterEvadeMode();
                        return;
                    }

                    if (!introDone)
                    {
                        Talk(TALK_AGGRO);
                        introDone = true;
                    }

                    pInstance->SetBossState(DATA_SHA_OF_FEAR, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                    me->SetReactState(REACT_AGGRESSIVE);

                    events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                    events.ScheduleEvent(EVENT_EERIE_SKULL, 5000);
                    events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                    events.ScheduleEvent(EVENT_FIRST_TERRORS, 30000);
                    events.ScheduleEvent(EVENT_OMINOUS_CACKLE, 25500);
                    events.ScheduleEvent(EVENT_ENRAGE, 900000);
                }
            }

            void EnterEvadeMode()
            {
                if (!pInstance)
                    return;

                if (!pInstance->IsWipe())
                    return;

                events.Reset();
                summons.DespawnAll();

                me->SetPower(POWER_ENERGY, 0);
                me->RemoveAllAuras();
                me->SetFullHealth();
                if (!me->IsVisible())
                    me->SetVisible(true);


                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT_HEROIC);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEARLESS);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_REACHING_ATTACK);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_EXPANSE_AURA);

                if (me->HasUnitState(UNIT_STATE_CANNOT_TURN))
                    me->ClearUnitState(UNIT_STATE_CANNOT_TURN);

                shrine1 = false;
                shrine2 = false;
                shrine3 = false;

                attacksCounter = 0;
                terrorCounter  = 0;

                if (isInSecondPhase)
                {
                    Position homePos = me->GetHomePosition();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                    isInSecondPhase = false;
                    submerged       = false;
                    isInTeleport    = false;
                    submergeCounter = 0;
                    if (pInstance->GetBossState(DATA_SHA_OF_FEAR) != FAIL)
                        pInstance->SetBossState(DATA_SHA_OF_FEAR, FAIL);
                    me->NearTeleportTo(homePos.GetPositionX(), homePos.GetPositionY(), homePos.GetPositionZ(), homePos.GetOrientation());
                }
                else
                    me->GetMotionMaster()->MoveTargetedHome();

                _EnterEvadeMode();
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                events.Reset();
                if (pInstance)
                {
                    // Normal mode or Heroic mode P1
                    if (!isInSecondPhase)
                    {
                        // In NM, before despawning Return to the terrace mobs, we get the players back.
                        std::list<Creature*> returnMobList;
                        GetCreatureListWithEntryInGrid(returnMobList, me, NPC_RETURN_TO_THE_TERRACE, 300.0f);

                        if (!returnMobList.empty())
                            for (Creature* returnMob : returnMobList)
                                returnMob->AI()->DoAction(ACTION_RETURNS_PLAYERS);

                        summons.DespawnAll();

                    }
                    // Heroic mode - P2
                    else
                    {
                        // Here, we must despawn all firs to avoid NPC for getting back are also despawned
                        summons.DespawnAll();

                        // In HM, we summon a Return to the terrace for the player to return to the normal world
                        // Choosing a position for summoning near Sha and reachable for players
                        Position summonPos;
                        me->GetPosition(&summonPos);
                        float x = summonPos.GetPositionX();
                        float y = summonPos.GetPositionY() + 20.0f;

                        x = x < -1847.0f ? -1847.0f : (x > -1597.0f ? -1597.0f : x);
                        y = y < -3940.0f ? -3940.0f : (y > -3690.0f ? -3690.0f : y);
                        summonPos.Relocate(x, y);

                        me->SummonCreature(NPC_RETURN_TO_THE_TERRACE, summonPos);
                    }

                    pInstance->SetBossState(DATA_SHA_OF_FEAR, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT_HEROIC);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEARLESS);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_REACHING_ATTACK);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DREAD_EXPANSE_AURA);
                    if (isInSecondPhase)
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LEI_SHIS_HOPE);

                    me->SetPower(POWER_ENERGY, 0);
                    pInstance->SaveToDB();

                    Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = l_Itr->getSource())
                            me->CastSpell(l_Player, SPELL_SHA_OF_FEAR_BONUS, true);
                    }

                    if (me->GetMap()->IsLFR())
                    {
                        me->SetLootRecipient(NULL);
                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == NPC_TERROR_SPAWN)
                    ++terrorCounter;

                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                if (summon->GetEntry() == NPC_TERROR_SPAWN)
                    --terrorCounter;

                summons.Despawn(summon);
            }

            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SPAWN_TERROR:
                    {
                        if (!terrorCounter)
                        {
                            me->CastSpell(spawnTerrorPos[0].GetPositionX(), spawnTerrorPos[0].GetPositionY(),
                                          spawnTerrorPos[0].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_01, true);
                            me->CastSpell(spawnTerrorPos[1].GetPositionX(), spawnTerrorPos[1].GetPositionY(),
                                          spawnTerrorPos[1].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_02, true);
                        }
                        else
                        {
                            me->CastSpell(spawnTerrorPos[2].GetPositionX(), spawnTerrorPos[2].GetPositionY(),
                                          spawnTerrorPos[2].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_03, true);
                            me->CastSpell(spawnTerrorPos[3].GetPositionX(), spawnTerrorPos[3].GetPositionY(),
                                          spawnTerrorPos[3].GetPositionZ(), SPELL_CONJURE_TERROR_SPAWN_04, true);
                        }

                        break;
                    }
                    case ACTION_ACTIVATE_SHRINE_1:
                    {
                        if (shrine1)
                        {
                            if (!shrine2)
                                DoAction(ACTION_ACTIVATE_SHRINE_2);
                            else if (!shrine3)
                                DoAction(ACTION_ACTIVATE_SHRINE_3);
                            else
                                break;
                        }

                        shrine1 = true;
                        me->SummonCreature(NPC_YANG_GUOSHI + (urand(0, 2) * 4), shrinesPos[0].GetPositionX(), shrinesPos[0].GetPositionY(), shrinesPos[0].GetPositionZ());
                        me->SummonCreature(NPC_RETURN_TO_THE_TERRACE, shrinesPos[0].GetPositionX(), shrinesPos[0].GetPositionY(), shrinesPos[0].GetPositionZ());
                        break;
                    }
                    case ACTION_ACTIVATE_SHRINE_2:
                    {
                        if (shrine2)
                        {
                            if (!shrine1)
                                DoAction(ACTION_ACTIVATE_SHRINE_1);
                            else if (!shrine3)
                                DoAction(ACTION_ACTIVATE_SHRINE_3);
                            else
                                break;
                        }

                        shrine2 = true;
                        me->SummonCreature(NPC_YANG_GUOSHI + (urand(0, 2) * 4), shrinesPos[1].GetPositionX(), shrinesPos[1].GetPositionY(), shrinesPos[1].GetPositionZ());
                        me->SummonCreature(NPC_RETURN_TO_THE_TERRACE, shrinesPos[1].GetPositionX(), shrinesPos[1].GetPositionY(), shrinesPos[1].GetPositionZ());
                        break;
                    }
                    case ACTION_ACTIVATE_SHRINE_3:
                    {
                        if (shrine3)
                        {
                            if (!shrine1)
                                DoAction(ACTION_ACTIVATE_SHRINE_1);
                            else if (!shrine2)
                                DoAction(ACTION_ACTIVATE_SHRINE_2);
                            else
                                break;
                        }

                        shrine3 = true;
                        me->SummonCreature(NPC_YANG_GUOSHI + (urand(0, 2) * 4), shrinesPos[2].GetPositionX(), shrinesPos[2].GetPositionY(), shrinesPos[2].GetPositionZ());
                        me->SummonCreature(NPC_RETURN_TO_THE_TERRACE, shrinesPos[2].GetPositionX(), shrinesPos[2].GetPositionY(), shrinesPos[2].GetPositionZ());
                        break;
                    }
                    case ACTION_DESACTIVATE_SHRINE_1:
                        shrine1 = false;
                        break;
                    case ACTION_DESACTIVATE_SHRINE_2:
                        shrine2 = false;
                        break;
                    case ACTION_DESACTIVATE_SHRINE_3:
                        shrine3 = false;
                        break;
                    case ACTION_ACTIVATE_SHA_OF_FEAR:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetReactState(REACT_DEFENSIVE);
                        break;
                    case ACTION_ARRIVE_IN_DREAD_EXPANSE:
                    {
                        // New abilities
                        events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                        events.ScheduleEvent(EVENT_VISIBLE_EXPANSE, 1000);
                        events.ScheduleEvent(EVENT_IMPLACABLE_STRIKE, 32000);
                        events.ScheduleEvent(EVENT_WATERSPOUT, 50000);
                        events.ScheduleEvent(EVENT_HUDDLE_IN_TERROR, 44000);
                        events.ScheduleEvent(EVENT_NAKED_AND_AFRAID, 20000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, const SpellInfo*  /*p_SpellInfo*/)
            {
                if (!IsHeroic() || isInSecondPhase)
                    return;

                // Heroic - Phase 2
                if (me->HealthBelowPctDamaged(healthPctForSecondPhase, damage))
                {
                    events.Reset();
                    summons.DespawnAll();

                    isInSecondPhase = true;
                    Talk(TALK_HEROIC);
                    me->MonsterTextEmote("The Sha of Fear retreats to the Dread Expanse", 0, true);

                    // Champion of light now gets Pure Light
                    if (Player* champion = GetChampionOfLight(me))
                        champion->CastSpell(champion, SPELL_CHAMPION_OF_LIGHT_HEROIC, true);

                    // Teleport part
                    me->SetVisible(false);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->DeleteThreatList();
                    me->getThreatManager().clearReferences();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                    // Teleporting players
                    Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                    {
                        float x = heroicPos.GetPositionX() + frand(-5.0f, 5.0f);
                        float y = heroicPos.GetPositionY() + frand(-5.0f, 5.0f);

                        itr->getSource()->NearTeleportTo(x, y, heroicPos.GetPositionZ(), heroicPos.GetOrientation());
                        itr->getSource()->ModifyAuraState(AURA_STATE_UNKNOWN22, true);
                    }

                    if (pInstance)
                        if (Creature* travel = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TRAVEL_TO_DREAD_EXPANSE)))
                            travel->AI()->DoAction(ACTION_TELEPORT_SHA);

                    isInTeleport = true;
                }
            }

            void DamageDealt(Unit* /*victim*/, uint32& /*damage*/, DamageEffectType damageType)
            {
                if (damageType == DIRECT_DAMAGE)
                {
                    if (!me->m_extraAttacks)
                    {
                        // Using Thrash in normal mode and in phase 1 of heroic mode
                        if (!isInSecondPhase)
                        {
                            if (!attacksCounter && me->HasAura(SPELL_THRASH_AURA))
                                me->RemoveAura(SPELL_THRASH_AURA);

                            if (attacksCounter >= 2)
                            {
                                if (attacksCounter >= 3)
                                {
                                    me->CastSpell(me, SPELL_THRASH_EXTRA_ATTACKS, true);
                                    me->RemoveAura(SPELL_THRASH_AURA);
                                    attacksCounter = 0;
                                }
                                else
                                    me->CastSpell(me, SPELL_THRASH_AURA, false);
                            }
                        }
                        // Using Dread Thrash in phase 2 of heroic mode
                        else
                        {
                            if (!attacksCounter && me->HasAura(SPELL_DREAD_THRASH_AURA))
                                    me->RemoveAura(SPELL_DREAD_THRASH_AURA);

                            if (attacksCounter >= 3)
                            {
                                if (attacksCounter >= 4)
                                {
                                    me->CastSpell(me, SPELL_DREAD_THRASH_EXTRA_ATT, true);
                                    me->RemoveAura(SPELL_DREAD_THRASH_AURA);
                                    attacksCounter = 0;
                                }
                                else
                                    me->CastSpell(me, SPELL_DREAD_THRASH_EXTRA_ATT, false);
                            }
                        }
                        ++attacksCounter;
                    }
                }
            }

            void OnAddThreat(Unit* victim, float& fThreat, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/)
            {
                if (!victim || victim->GetTypeId() != TYPEID_PLAYER || victim->ToPlayer() != GetChampionOfLight(me))
                {
                    fThreat = 0;
                    return;
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (!me->isInCombat())
                {
                    value = 0;
                    return;
                }

                // Sha of Fear regenerates 6 energy every 2s (15 energy for 5s)
                value = 6;
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance)
                {
                    if (pInstance->IsWipe())
                    {
                        if (!me->IsInEvadeMode())
                        {
                            EnterEvadeMode();
                            return;
                        }
                    }
                }

                if (!UpdateVictim())
                {
                    if (!isInSecondPhase)
                    {
                        if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                            me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);

                        if (me->isInCombat())
                            EnterEvadeMode();
                    }

                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
 
                events.Update(diff);
 
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_MELEE:
                        {
                            if (!me->IsWithinMeleeRange(me->getVictim(), 2.0f))
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f))
                                    me->CastSpell(target, SPELL_REACHING_ATTACK, false);
                            }
                            else
                            {
                                // Always attack champion of light
                                if (Player* target = GetChampionOfLight(me))
                                    if (me->getVictim() && me->getVictim()->GetGUID() != target->GetGUID())
                                        AttackStart(target);
                            }
                            events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                            break;
                        }
                        case EVENT_EERIE_SKULL:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 60.0f))
                                me->CastSpell(target, SPELL_EERIE_SKULL, false);
                            events.ScheduleEvent(EVENT_EERIE_SKULL, 5000);
                            break;
                        }
                        case EVENT_CHECK_ENERGY:
                        {
                            if (!pInstance)
                            {
                                events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                                break;
                            }

                            if (me->GetPower(POWER_ENERGY) >= me->GetMaxPower(POWER_ENERGY))
                            {
                                // P2 Heroic Mode - Submerge - Placed in another event for legibility as it triggers 2 others events coded just after
                                if (isInSecondPhase)
                                    events.ScheduleEvent(EVENT_SUBMERGE, 100);
                                // Normal mode or P1 Heroic mode - Breath of fear
                                else
                                {
                                    if (Creature* pureLight = Creature::GetCreature(*me, pInstance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                                        me->CastSpell(pureLight, SPELL_BREATH_OF_FEAR, false);
                                    Talk(TALK_BREATH_OF_FEAR);
                                }
                                me->SetPower(POWER_ENERGY, 0);
                                me->SetInt32Value(UNIT_FIELD_POWER, 0);
                            }

                            events.ScheduleEvent(EVENT_CHECK_ENERGY, 1000);
                            break;
                        }
                        case EVENT_FIRST_TERRORS:
                        {
                            me->CastSpell(me, SPELL_CONJURE_TERROR_SPAWN_TICK, true);
                            break;
                        }
                        case EVENT_OMINOUS_CACKLE:
                        {
                            me->CastSpell(me, SPELL_OMINOUS_CACKLE_CAST, false);
                            events.ScheduleEvent(EVENT_OMINOUS_CACKLE, Is25ManRaid() ? 45000 : 90000);
                            break;
                        }
                        case EVENT_ENRAGE:
                        {
                            me->CastSpell(me, SPELL_BERSERK, true);
                            break;
                        }
                        // Heroic events - phase 2
                        case EVENT_IMPLACABLE_STRIKE:
                        {
                            if (!IsHeroic())
                                break;
                            me->CastSpell(me, SPELL_IMPLACABLE_STRIKE, false);

                            events.ScheduleEvent(EVENT_IMPLACABLE_STRIKE, 60000);
                            break;
                        }
                        case EVENT_NAKED_AND_AFRAID:
                        {
                            if (!IsHeroic())
                                break;

                            if (Unit* target = me->getVictim())
                                me->CastSpell(target, SPELL_NAKED_AND_AFRAID, false);
                            events.ScheduleEvent(EVENT_NAKED_AND_AFRAID, 30000);
                            break;
                        }
                        case EVENT_WATERSPOUT:
                        {
                            if (!IsHeroic())
                                break;

                            DoCast(me, SPELL_WATERSPOUT_CHANNELING, true);
                            events.ScheduleEvent(EVENT_WATERSPOUT, 42000);
                            break;
                        }
                        case EVENT_HUDDLE_IN_TERROR:
                        {
                            if (!IsHeroic())
                                break;

                            Talk(TALK_HUDDLE);
                            //me->CastSpell(me, SPELL_HUDDLE_IN_TERROR, false);
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 300.0f);

                            if (!playerList.empty())
                            {
                                if (playerList.size() > uint8(Is25ManRaid() ? 5 : 2))
                                    JadeCore::RandomResizeList(playerList, Is25ManRaid() ? 5 : 2);

                                for (Player* plr : playerList)
                                    me->AddAura(SPELL_HUDDLE_IN_TERROR, plr);
                            }

                            events.ScheduleEvent(EVENT_HUDDLE_IN_TERROR, urand(45000, 55000));
                            break;
                        }
                        // Sha disappears underwater
                        case EVENT_SUBMERGE:
                        {
                            if (!IsHeroic())
                                break;

                            submerged = true;
                            me->MonsterTextEmote("The Sha of Fear submerges beneath the Dread Expanse", 0, true);

                            me->CastSpell(me, SPELL_SUBMERGE_TRANSFORM, true);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                            events.ScheduleEvent(EVENT_SPAWN_DREAD, 2000);
                            events.ScheduleEvent(EVENT_EMERGE, 6000);

                            break;
                        }
                        // Sha summons Dread Spawns while underwater
                        case EVENT_SPAWN_DREAD:
                        {
                            Talk(TALK_SUBMERGE);
                            ++submergeCounter;

                            for (int i = 0; i < submergeCounter; ++i)
                            {
                                float rotation = frand(0, 2 * M_PI);
                                float x = me->GetPositionX() + ((40.0f) * cos(rotation));
                                float y = me->GetPositionY() + ((40.0f) * sin(rotation));

                                me->SummonCreature(NPC_DREAD_SPAWN, x, y, me->GetPositionZ(), rotation);
                            }
                            break;
                        }
                        // Sha comes out from water
                        case EVENT_EMERGE:
                        {
                            Position pos;
                            me->GetNearPosition(pos, frand(0.0f, 100.0f), 0.0f);
                            me->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), true);

                            me->CastSpell(me, SPELL_EMERGE_DAMAGE, true);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                            break;
                        }
                        case EVENT_VISIBLE_EXPANSE:
                        {
                            me->SetVisible(true);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_IMMUNE_TO_PC);
                            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                            me->ClearUnitState(UNIT_STATE_CANNOT_TURN);
                            isInTeleport = false;

                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 200.0f);

                            for (Player* player : playerList)
                                me->AddThreat(player, 0.0f);

                            if (!playerList.empty())
                            {
                                JadeCore::RandomResizeList(playerList, 1);

                                Player* victim  = playerList.front();
                                AttackStart(victim);
                                me->SetInCombatWith(victim);
                            }

                            if (pInstance)
                            {
                                if (Creature* travel = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TRAVEL_TO_DREAD_EXPANSE)))
                                    travel->AI()->SetData(0, 1);

                                // Removing auras from p1
                                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WALL_OF_LIGHT_BUFF);
                                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEARLESS);
                                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_REACHING_ATTACK);

                                // Applying new auras
                                pInstance->DoAddAuraOnPlayers(SPELL_LEI_SHIS_HOPE);
                                pInstance->DoAddAuraOnPlayers(SPELL_DREAD_EXPANSE_AURA);
                                pInstance->DoAddAuraOnPlayers(SPELL_FADING_LIGHT);
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_fearAI(creature);
        }
};

// Pure Light Terrace - 60788
class mob_pure_light_terrace : public CreatureScript
{
    public:
        mob_pure_light_terrace() : CreatureScript("mob_pure_light_terrace") { }

        struct mob_pure_light_terraceAI : public ScriptedAI
        {
            mob_pure_light_terraceAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                wallActivated = false;
            }

            InstanceScript* pInstance;

            bool wallActivated;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->CastSpell(me, SPELL_LIGHT_WALL, true);
                me->CastSpell(me, SPELL_LIGHT_WALL_READY, true);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ACTIVATE_WALL_OF_LIGHT:
                    {
                        if (wallActivated)
                            break;

                        wallActivated = true;
                        me->CastSpell(me, SPELL_LIGHT_WALL_VISUAL, true);
                        me->RemoveAura(SPELL_LIGHT_WALL_READY);
                        break;
                    }
                    case ACTION_DESACTIVATE_WALL_OF_LIGHT:
                    {
                        if (!wallActivated)
                            break;

                        wallActivated = false;
                        me->CastSpell(me, SPELL_LIGHT_WALL_READY, true);
                        me->RemoveAura(SPELL_LIGHT_WALL_VISUAL);
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                damage = 0;
                return;
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                    {
                        if (wallActivated && (me->isInBack(player, M_PI / 3)  || player->GetDistance2d(me) < 1.6f) && !player->HasAura(SPELL_WALL_OF_LIGHT_BUFF))
                            player->AddAura(SPELL_WALL_OF_LIGHT_BUFF, player);
                        else if ((!me->isInBack(player, M_PI / 3) && player->HasAura(SPELL_WALL_OF_LIGHT_BUFF) && player->GetDistance2d(me) >= 1.6f) || !wallActivated)
                            player->RemoveAura(SPELL_WALL_OF_LIGHT_BUFF);

                        if (player->GetDistance(me) <= 3.0f)
                        {
                            if (pInstance)
                            {
                                if (!pInstance->CheckRequiredBosses(DATA_SHA_OF_FEAR))
                                    return;

                                if (!player->HasAura(SPELL_CHAMPION_OF_LIGHT))
                                {
                                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHAMPION_OF_LIGHT);
                                    player->AddAura(SPELL_CHAMPION_OF_LIGHT, player);
                                    DoAction(ACTION_ACTIVATE_WALL_OF_LIGHT);

                                    if (Player* champion = GetChampionOfLight(me))
                                    {
                                        if (Creature* sha = Creature::GetCreature(*me, pInstance->GetData64(NPC_SHA_OF_FEAR)))
                                        {
                                            sha->AddThreat(champion, 1000000.0f);
                                            sha->AI()->AttackStart(champion);
                                            if (pInstance->GetBossState(DATA_SHA_OF_FEAR) == FAIL || pInstance->GetBossState(DATA_SHA_OF_FEAR) == NOT_STARTED)
                                                sha->AI()->EnterCombat(champion);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (player->HasAura(SPELL_CHAMPION_OF_LIGHT))
                                player->RemoveAura(SPELL_CHAMPION_OF_LIGHT);

                            Player* champion = GetChampionOfLight(me);
                            if (!champion)
                                DoAction(ACTION_DESACTIVATE_WALL_OF_LIGHT);
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_pure_light_terraceAI(creature);
        }
};

// Return to the Terrace - 65736 (All modes, to return from shrines to the main platform)
class mob_return_to_the_terrace : public CreatureScript
{
    public:
        mob_return_to_the_terrace() : CreatureScript("mob_return_to_the_terrace") { }

        struct mob_return_to_the_terraceAI : public ScriptedAI
        {
            mob_return_to_the_terraceAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            bool inDreadExpanse;

            void Reset()
            {
                inDreadExpanse = (me->GetPositionZ() < 0.0f);
                events.Reset();
                if (Creature* Sha = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SHA_OF_FEAR)))
                {
                    if (!inDreadExpanse)
                    {
                        events.ScheduleEvent(EVENT_CHECK_GUARDIAN, 1000);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        me->RemoveFlag(UNIT_FIELD_INTERACT_SPELL_ID, SPELL_FEARLESS);
                        me->RemoveAura(SPELL_PURE_LIGHT_VISUAL);
                    }
                    else
                        DoAction(ACTION_ACTIVATE_RETURN);
                }
            }

            void OnSpellClick(Unit* clicker)
            {
                if (clicker->GetTypeId() != TYPEID_PLAYER)
                    return;

                DoAction(inDreadExpanse ? ACTION_GET_PLAYERS_BACK : ACTION_RETURNS_PLAYERS);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    // Get players back from shrine
                    case ACTION_RETURNS_PLAYERS:
                    {
                        std::list<Player*> playersToRecall;
                        me->GetPlayerListInGrid(playersToRecall, 50.0f);

                        if (!playersToRecall.empty())
                        {
                            uint8 pos = urand(0, 1);

                            for (auto itr : playersToRecall)
                            {
                                itr->CastSpell(itr, SPELL_FEARLESS, true);
                                itr->NearTeleportTo(returnPos[pos].GetPositionX(), returnPos[pos].GetPositionY(), returnPos[pos].GetPositionZ(), returnPos[pos].GetOrientation());
                                if (Pet* pet = itr->GetPet())
                                    pet->NearTeleportTo(returnPos[pos].GetPositionX(), returnPos[pos].GetPositionY(), returnPos[pos].GetPositionZ(), returnPos[pos].GetOrientation());
                            }
                        }
                        break;
                    }
                    // Activate mob once mini-boss is dead
                    case ACTION_ACTIVATE_RETURN:
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        me->SetFlag(UNIT_FIELD_INTERACT_SPELL_ID, SPELL_FEARLESS);
                        me->CastSpell(me, SPELL_PURE_LIGHT_VISUAL, true);
                        break;
                    }
                    // Get players back from Dread Expanse
                    case ACTION_GET_PLAYERS_BACK:
                    {
                        Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                        if (Creature* terrace = pInstance->instance->GetCreature(pInstance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                        {
                            for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                            {
                                // Choosing a position near terrace of pure light
                                Position plrPosition;
                                terrace->GetNearPosition(plrPosition, 10.0f, 0.0f);
                                itr->getSource()->NearTeleportTo(plrPosition.GetPositionX(), plrPosition.GetPositionY(), plrPosition.GetPositionZ(), plrPosition.GetOrientation());
                                if (Pet* pet = itr->getSource()->GetPet())
                                    pet->NearTeleportTo(plrPosition.GetPositionX(), plrPosition.GetPositionY(), plrPosition.GetPositionZ(), plrPosition.GetOrientation());
                            }
                        }
                        break;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (inDreadExpanse)
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_GUARDIAN:
                    {
                        std::list<Creature*> guardianList;
                        me->GetCreatureListWithEntryInGrid(guardianList, NPC_YANG_GUOSHI, 30.0f);
                        if (guardianList.empty())
                            me->GetCreatureListWithEntryInGrid(guardianList, NPC_CHENG_KANG, 30.0f);
                        if (guardianList.empty())
                            me->GetCreatureListWithEntryInGrid(guardianList, NPC_JINLUN_KUN, 30.0f);

                        bool died = false;
                        if (guardianList.empty())
                            died = true;
                        else
                        {
                            for (auto itr : guardianList)
                            {
                                if (itr->isDead())
                                    died = true;
                                else
                                    died = false;
                            }
                        }

                        if (died)
                        {
                            events.ScheduleEvent(EVENT_CHECK_GUARDIAN, 1000);
                            DoAction(ACTION_ACTIVATE_RETURN);
                            me->DespawnOrUnsummon(10000);
                        }
                        else
                        {
                            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                            me->RemoveFlag(UNIT_FIELD_INTERACT_SPELL_ID, SPELL_FEARLESS);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            me->RemoveAura(SPELL_PURE_LIGHT_VISUAL);
                        }

                        events.ScheduleEvent(EVENT_CHECK_GUARDIAN, 1000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_return_to_the_terraceAI(creature);
        }
};

// 61735 - Travel to Dread Expanse
class mob_travel_to_dread_expanse : public CreatureScript
{
public:
    mob_travel_to_dread_expanse() : CreatureScript("mob_travel_to_dread_expanse") { }

    struct mob_travel_to_dread_expanseAI : public ScriptedAI
    {
        mob_travel_to_dread_expanseAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;
        bool teleportTriggered;

        void Reset()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_TO_DREAD_EXPANSE, 100);
            teleportTriggered = false;
        }

        void DoAction(int32 const action)
        {
            switch (action)
            {
                case ACTION_TELEPORT_SHA:
                {
                    if (events.Empty())
                        events.ScheduleEvent(EVENT_TO_DREAD_EXPANSE, 100);
                    break;
                }
                default:
                    break;
            }
        }

        void SetData(uint32 /*type*/, uint32 id)
        {
            if (id)
                teleportTriggered = false;
        }

        void UpdateAI(uint32 const diff)
        {
            if (!pInstance || pInstance->IsWipe())
                return;

            events.Update(diff);

            if (events.ExecuteEvent() != EVENT_TO_DREAD_EXPANSE || teleportTriggered)
                return;

            // Teleporting sha
            if (Creature* Sha = pInstance->instance->GetCreature(pInstance->GetData64(NPC_SHA_OF_FEAR)))
            {
                if (Sha->GetPositionZ() > 0.0f)
                {
                    if (!CAST_AI(boss_sha_of_fear::boss_sha_of_fearAI, Sha->GetAI())->isInTeleport)
                        return;

                    Sha->NearTeleportTo(heroicSha.GetPositionX(), heroicSha.GetPositionY(), heroicSha.GetPositionZ(), heroicSha.GetOrientation());
                    Sha->AI()->DoAction(ACTION_ARRIVE_IN_DREAD_EXPANSE);

                    teleportTriggered = true;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_travel_to_dread_expanseAI(creature);
    }
};

// Terror Spawn - 61034
class mob_terror_spawn : public CreatureScript
{
    public:
        mob_terror_spawn() : CreatureScript("mob_terror_spawn") { }

        struct mob_terror_spawnAI : public ScriptedAI
        {
            mob_terror_spawnAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                if (pInstance)
                {
                    if (Creature* pureLight = Creature::GetCreature(*me, pInstance->GetData64(NPC_PURE_LIGHT_TERRACE)))
                        me->SetFacingToObject(pureLight);
                }

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->AddUnitState(UNIT_STATE_CANNOT_TURN);

                me->CastSpell(me, SPELL_DARK_BULWARK, true);

                events.Reset();
                events.ScheduleEvent(EVENT_PENETRATING_BOLT, 1500);
            }

            void CheckHitResult(MeleeHitOutcome& p_MeleeHitResult, SpellMissInfo& p_SpellInfo, Unit* p_Attaker)
            {
                if (me->isInBack(p_Attaker))
                {
                    p_MeleeHitResult = MELEE_HIT_NORMAL;
                    p_SpellInfo = SPELL_MISS_NONE;
                }
                else
                {
                    p_MeleeHitResult = MELEE_HIT_MISS;
                    p_SpellInfo = SPELL_MISS_DEFLECT;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_PENETRATING_BOLT:
                    {
                        me->CastSpell(me, SPELL_PENETRATING_BOLT, false);
                        events.ScheduleEvent(EVENT_PENETRATING_BOLT, 1500);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_terror_spawnAI(creature);
        }
};

#define GUARDIAN_SAY_AGGRO 0

// Yang Guoshi - 61038
// Cheng Kang - 61042
// Jinlun Kun - 61046
class mob_shrine_guardian : public CreatureScript
{
    public:
        mob_shrine_guardian() : CreatureScript("mob_shrine_guardian") { }

        struct mob_shrine_guardianAI : public ScriptedAI
        {
            mob_shrine_guardianAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            uint8 nextGlobePct;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->ReenableEvadeMode();
                me->CastSpell(me, SPELL_SHA_CORRUPTION, true);
                SetEquipmentSlots(false, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE, EQUIP_ID_GUARDIAN);

                events.Reset();

                events.ScheduleEvent(EVENT_SHOT, 500);
                events.ScheduleEvent(EVENT_DEATH_BLOSSOM, 10000);
                events.ScheduleEvent(EVENT_DREAD_SPRAY, 20000);

                me->SetFloatValue(UNIT_FIELD_MIN_RANGED_DAMAGE, me->GetFloatValue(UNIT_FIELD_MIN_DAMAGE));
                me->SetFloatValue(UNIT_FIELD_MAX_RANGED_DAMAGE, me->GetFloatValue(UNIT_FIELD_MAX_DAMAGE));

                nextGlobePct = 95;

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISTRACT, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            }

            void EnterCombat(Unit* /*attacker*/)
            {
                Talk(GUARDIAN_SAY_AGGRO);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, const SpellInfo*  /*p_SpellInfo*/)
            {
                if (nextGlobePct < 5)
                    return;

                if (me->HealthBelowPctDamaged(nextGlobePct, damage))
                {
                    nextGlobePct -= 5;
                    me->CastSpell(me, SPELL_SHA_GLOBE_SPAWN, true);
                }
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!pInstance)
                    return;

                Creature* sha = me->GetMap()->GetCreature(pInstance->GetData64(NPC_SHA_OF_FEAR));
                if (!sha)
                    return;

                if (me->GetPositionX() == shrinesPos[0].GetPositionX() &&
                    me->GetPositionY() == shrinesPos[0].GetPositionY() &&
                    me->GetPositionZ() == shrinesPos[0].GetPositionZ())
                {
                    sha->AI()->DoAction(ACTION_DESACTIVATE_SHRINE_1);
                }
                else if (me->GetPositionX() == shrinesPos[1].GetPositionX() &&
                         me->GetPositionY() == shrinesPos[1].GetPositionY() &&
                         me->GetPositionZ() == shrinesPos[1].GetPositionZ())
                {
                    sha->AI()->DoAction(ACTION_DESACTIVATE_SHRINE_2);
                }
                else
                    sha->AI()->DoAction(ACTION_DESACTIVATE_SHRINE_3);

                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SHOT:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_SHOT, 500);
                            break;
                        }

                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 30.0f))
                            me->CastSpell(target, SPELL_SHOT, false);
                        events.ScheduleEvent(EVENT_SHOT, 500);
                        break;
                    }
                    case EVENT_DEATH_BLOSSOM:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_DEATH_BLOSSOM, 0);
                            break;
                        }

                        me->CastSpell(me, SPELL_DEATH_BLOSSOM, false);
                        events.ScheduleEvent(EVENT_DEATH_BLOSSOM, 20000);
                        break;
                    }
                    case EVENT_DREAD_SPRAY:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.ScheduleEvent(EVENT_DREAD_SPRAY, 0);
                            break;
                        }

                        me->CastSpell(me, SPELL_DREAD_SPRAY, false);
                        events.ScheduleEvent(EVENT_DREAD_SPRAY, 30000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_shrine_guardianAI(creature);
        }
};

// Sha Globe - 65691
class mob_sha_globe : public CreatureScript
{
    public:
        mob_sha_globe() : CreatureScript("mob_sha_globe") { }

        struct mob_sha_globeAI : public ScriptedAI
        {
            mob_sha_globeAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE);
                me->CastSpell(me, SPELL_SHA_GLOBE_PERIODIC_VISUAL, true);

                events.Reset();
                events.ScheduleEvent(EVENT_DESPAWN_SHA_GLOBE, 8000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_DESPAWN_SHA_GLOBE:
                    {
                        Unit* guardian = me->ToTempSummon()->GetSummoner();
                        if (!guardian)
                            return;

                        me->HealBySpell(guardian, sSpellMgr->GetSpellInfo(SPELL_SHA_GLOBE_HEAL_SUMMONER),
                                        guardian->CountPctFromMaxHealth(IsHeroic() ? 8 : 5));
                        me->CastSpell(me, SPELL_SHA_GLOBE_HEAL_SUMMONER, true);
                        me->DespawnOrUnsummon();
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_globeAI(creature);
        }
};

// Dread Spawn - 61003
class mob_dread_spawn : public CreatureScript
{
    public:
        mob_dread_spawn() : CreatureScript("mob_dread_spawn") { }

        struct mob_dread_spawnAI : public ScriptedAI
        {
            mob_dread_spawnAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            uint64 championGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GATHERING_SPEED, 5000);
                me->SetReactState(REACT_PASSIVE);
                championGuid = 0;
                ChaseChampionOfLight();
                events.ScheduleEvent(EVENT_CHECK_CHAMPION, 1000);
            }

            void ChaseChampionOfLight()
            {
                if (championGuid)
                {
                    if (Player* plr = ObjectAccessor::GetPlayer(*me, championGuid))
                        if (!plr->HasAura(SPELL_CHAMPION_OF_LIGHT_HEROIC))
                            DoAction(ACTION_TRANSFERT_OF_LIGHT);
                }
                else
                    DoAction(ACTION_TRANSFERT_OF_LIGHT);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == 1)
                    if (Player* plr = ObjectAccessor::GetPlayer(*me, championGuid))
                        me->CastSpell(plr, SPELL_ETERNAL_DARKNESS, false);
            }

            void JustDied(Unit* /*killer*/)
            {
                // Prevents dead mob to move towards player with pure light on light transfer
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_TRANSFERT_OF_LIGHT:
                    {
                        // Retreiving new champion of light
                        championGuid = 0;
                        me->GetMotionMaster()->Clear();
                        Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                        Map::PlayerList::const_iterator itr = playerList.begin();

                        while (!championGuid && itr != playerList.end())
                        {
                            if (Player* plr = itr->getSource())
                            {
                                if (plr->HasAura(SPELL_CHAMPION_OF_LIGHT_HEROIC))
                                {
                                    championGuid = plr->GetGUID();
                                    me->GetMotionMaster()->MovePoint(1, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ());
                                }
                            }
                            ++itr;
                        }

                        me->RemoveAura(SPELL_GATHERING_SPEED);
                        me->CastSpell(me, SPELL_SHA_SPINES, false);
                        events.RescheduleEvent(EVENT_GATHERING_SPEED, 5000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_GATHERING_SPEED:
                    {
                        me->CastSpell(me, SPELL_GATHERING_SPEED, true);
                        events.ScheduleEvent(EVENT_GATHERING_SPEED, 5000);
                        break;
                    }
                    case EVENT_CHECK_CHAMPION:
                    {
                        ChaseChampionOfLight();
                        events.ScheduleEvent(EVENT_CHECK_CHAMPION, 1000);
                        break;
                    }
                    default:
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dread_spawnAI(creature);
        }
};

// Breath of Fear - 119414 / 125786
class spell_breath_of_fear : public SpellScriptLoader
{
    public:
        spell_breath_of_fear() : SpellScriptLoader("spell_breath_of_fear") { }

        class spell_breath_of_fear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_breath_of_fear_SpellScript);

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                // All targets on the platform, who are not afflicted by Wall of Light or Fearless will be hit by this spell.
                targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_WALL_OF_LIGHT_BUFF));
                targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_FEARLESS));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_breath_of_fear_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_breath_of_fear_SpellScript();
        }
};

// Conjure Terror Spawn - 119108
class spell_conjure_terror_spawn : public SpellScriptLoader
{
    public:
        spell_conjure_terror_spawn() : SpellScriptLoader("spell_conjure_terror_spawn") { }

        class spell_conjure_terror_spawn_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_conjure_terror_spawn_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->AI()->DoAction(ACTION_SPAWN_TERROR);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_conjure_terror_spawn_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_conjure_terror_spawn_AuraScript();
        }
};

// Penetrating Bolt - 129075
class spell_penetrating_bolt : public SpellScriptLoader
{
    public:
        spell_penetrating_bolt() : SpellScriptLoader("spell_penetrating_bolt") { }

        class spell_penetrating_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_penetrating_bolt_SpellScript);

            uint64 targetGuid;

            void CorrectRange(std::list<WorldObject*>& targets)
            {
                targetGuid = 0;

                if (!targets.empty())
                    JadeCore::Containers::RandomResizeList(targets, 1);

                for (auto itr : targets)
                    if (itr->GetGUID())
                        targetGuid = itr->GetGUID();
            }

            void HandleDummy(SpellEffIndex /*index*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (InstanceScript* instance = caster->GetInstanceScript())
                    {
                        if (Player* target = Player::GetPlayer(*caster, targetGuid))
                            caster->CastSpell(target, SPELL_PENETRATING_BOLT_MISSILE, true, NULL, nullptr, instance->GetData64(NPC_SHA_OF_FEAR));
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_penetrating_bolt_SpellScript::CorrectRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectLaunch += SpellEffectFn(spell_penetrating_bolt_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_penetrating_bolt_SpellScript();
        }
};

// Ominous Cackle - 119593
class spell_ominous_cackle_cast : public SpellScriptLoader
{
    public:
        spell_ominous_cackle_cast() : SpellScriptLoader("spell_ominous_cackle_cast") { }

        class spell_ominous_cackle_cast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ominous_cackle_cast_SpellScript);

            bool activationDone;
            uint32 shrineTeleport;
            uint8 shrine;

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                Creature* caster = GetCaster()->ToCreature();
                if (!caster)
                    return;

                std::list<WorldObject*> realTargets;
                bool tankDone = false;
                bool healerDone = false;
                uint8 targetsToAdd = 5;

                for (auto itr : targets)
                {
                    if (!targetsToAdd)
                        break;

                    Player* player = itr->ToPlayer();
                    if (!player)
                        continue;

                    // Don't send player to shrine if he is already in one
                    std::list<Creature*> shrineMobs;
                    player->GetCreatureListWithEntryInGrid(shrineMobs, NPC_YANG_GUOSHI, 50.0f);
                    if (shrineMobs.size() > 0)
                        continue;
                    player->GetCreatureListWithEntryInGrid(shrineMobs, NPC_CHENG_KANG, 50.0f);
                    if (shrineMobs.size() > 0)
                        continue;
                    player->GetCreatureListWithEntryInGrid(shrineMobs, NPC_JINLUN_KUN, 50.0f);
                    if (shrineMobs.size() > 0)
                        continue;
                    player->GetCreatureListWithEntryInGrid(shrineMobs, NPC_RETURN_TO_THE_TERRACE, 50.0f);
                    if (shrineMobs.size() > 0)
                        continue;

                    if (!tankDone && player->GetRoleForGroup(player->GetSpecializationId(player->GetActiveSpec())) == Roles::ROLE_TANK)
                    {
                        if (player != GetChampionOfLight(caster))
                            continue;

                        tankDone = true;
                        targetsToAdd--;
                        realTargets.push_back(itr);
                        continue;
                    }

                    if (!healerDone && player->GetRoleForGroup(player->GetSpecializationId(player->GetActiveSpec())) == Roles::ROLE_HEALER)
                    {
                        healerDone = true;
                        targetsToAdd--;
                        realTargets.push_back(itr);
                        continue;
                    }

                    targetsToAdd--;
                    realTargets.push_back(itr);
                }

                activationDone = false;
                shrineTeleport = SPELL_TELEPORT_TO_SHRINE_1;
                shrine         = urand(0, 2);
                shrineTeleport += shrine;

                for (auto itr : realTargets)
                {
                    if (Creature* caster = GetCaster()->ToCreature())
                    {
                        if (Unit* target = itr->ToUnit())
                        {
                            target->CastSpell(target, shrineTeleport, true);

                            if (Pet* pet = target->ToPlayer()->GetPet())
                                pet->CastSpell(pet, shrineTeleport, false);

                            if (!activationDone)
                                caster->AI()->DoAction(ACTION_ACTIVATE_SHRINE_1 + shrine);
                            activationDone = true;
                        }
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ominous_cackle_cast_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ominous_cackle_cast_SpellScript();
        }
};

// Dread Spray - 120047
class spell_dread_spray : public SpellScriptLoader
{
    public:
        spell_dread_spray() : SpellScriptLoader("spell_dread_spray") { }

        class spell_dread_spray_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dread_spray_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    float rotation = frand(0, 2 * M_PI);
                    float x = caster->GetPositionX() + ((35.0f) * cos(rotation));
                    float y = caster->GetPositionY() + ((35.0f) * sin(rotation));

                    caster->CastSpell(x, y, caster->GetPositionZ(), SPELL_DREAD_SPRAY_TRIGGERED, true);
                    caster->SetFacingTo(rotation);
                    caster->SetOrientation(rotation);
                    caster->CastSpell(x, y, caster->GetPositionZ(), SPELL_DREAD_SPRAY_VISUAL, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dread_spray_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dread_spray_AuraScript();
        }
};

// Dread Spray (stacks) - 119983
class spell_dread_spray_stacks : public SpellScriptLoader
{
    public:
        spell_dread_spray_stacks() : SpellScriptLoader("spell_dread_spray_stacks") { }

        class spell_dread_spray_stacks_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dread_spray_stacks_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                Unit* caster = GetCaster();
                if (!caster)
                    return;

                std::list<WorldObject*> targetsToErase;

                for (auto itr : targets)
                    if (!caster->isInFront(itr, (M_PI / 2)))
                        targetsToErase.push_back(itr);

                for (auto itr : targetsToErase)
                    targets.remove(itr);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_spray_stacks_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_spray_stacks_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_spray_stacks_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dread_spray_stacks_SpellScript();
        }

        class spell_dread_spray_stacks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dread_spray_stacks_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura* dreadSpray = target->GetAura(SPELL_DREAD_SPRAY_STACKS))
                    {
                        if (dreadSpray->GetStackAmount() > 1)
                        {
                            target->RemoveAura(SPELL_DREAD_SPRAY_STACKS);
                            target->CastSpell(target, SPELL_DREAD_SPRAY_FEAR_ROOT, true);
                        }
                    }
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dread_spray_stacks_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dread_spray_stacks_AuraScript();
        }
};

// Sha Globe (periodic) - 129189
class spell_sha_globe_periodic : public SpellScriptLoader
{
    public:
        spell_sha_globe_periodic() : SpellScriptLoader("spell_sha_globe_periodic") { }

        class spell_sha_globe_periodic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_globe_periodic_SpellScript);

            void HandleOnHit()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    caster->RemoveAura(SPELL_SHA_GLOBE_PERIODIC_VISUAL);
                    caster->DespawnOrUnsummon();
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_globe_periodic_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_globe_periodic_SpellScript();
        }
};

// Death Blossom - 119888
class spell_death_blossom_periodic : public SpellScriptLoader
{
    public:
        spell_death_blossom_periodic() : SpellScriptLoader("spell_death_blossom_periodic") { }

        class spell_death_blossom_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_death_blossom_periodic_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    float rotation = frand(0, 2 * M_PI);
                    float x = caster->GetPositionX() + ((35.0f) * cos(rotation));
                    float y = caster->GetPositionY() + ((35.0f) * sin(rotation));

                    caster->CastSpell(x, y, caster->GetPositionZ(), SPELL_DEATH_BLOSSOM_RAND_ARROW, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_death_blossom_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_death_blossom_periodic_AuraScript();
        }
};

// Death Blossom (damage) - 119887
class spell_death_blossom_damage : public SpellScriptLoader
{
    public:
        spell_death_blossom_damage() : SpellScriptLoader("spell_death_blossom_damage") { }

        class spell_death_blossom_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_death_blossom_damage_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                Unit* caster = GetCaster();
                if (!caster)
                    return;

                std::list<WorldObject*> targetsToErase;

                for (auto itr : targets)
                    if (!caster->IsWithinLOSInMap(itr))
                        targetsToErase.push_back(itr);

                for (auto itr : targetsToErase)
                    targets.remove(itr);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_death_blossom_damage_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_death_blossom_damage_SpellScript();
        }
};

// Submerge - 120455
class spell_submerge : public SpellScriptLoader
{
    public:
        spell_submerge() : SpellScriptLoader("spell_submerge") { }

        class spell_submerge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_submerge_SpellScript);

            void HandleOnHit()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    if (Unit* target = caster->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        caster->NearTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation());
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_submerge_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_submerge_SpellScript();
        }
};

// Emerge - 120458
class spell_emerge : public SpellScriptLoader
{
    public:
        spell_emerge() : SpellScriptLoader("spell_emerge") { }

        class spell_emerge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_emerge_SpellScript);

            void HandleAfterCast()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    caster->RemoveAura(SPELL_SUBMERGE_TRANSFORM);
                    caster->CastSpell(caster, SPELL_EMERGE_STUN_ONE_SECOND, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_emerge_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_emerge_SpellScript();
        }
};

// 120519 - Waterspout
class spell_sha_waterspout : public SpellScriptLoader
{
    public:
        spell_sha_waterspout() : SpellScriptLoader("spell_sha_waterspout") { }

        class spell_sha_waterspout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_waterspout_SpellScript);

            void Damage()
            {
                Unit* caster = GetCaster();

                if (!caster)
                    return;

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, caster, 200.0f);

                if (playerList.empty())
                    return;

                if (playerList.size() > uint8(caster->GetInstanceScript()->instance->Is25ManRaid() ? 5 : 2))
                    JadeCore::RandomResizeList(playerList, caster->GetInstanceScript()->instance->Is25ManRaid() ? 5 : 2);

                for (Player* target : playerList)
                    caster->CastSpell(target, SPELL_WATERSPOUT_DAMAGE, false);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_waterspout_SpellScript::Damage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_waterspout_SpellScript();
        }
};

// 120285 - Transfer Light
class spell_transfer_light : public SpellScriptLoader
{
    public:
        spell_transfer_light() : SpellScriptLoader("spell_transfer_light") { }

        class spell_transfer_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_transfer_light_SpellScript);

            void Transfer()
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target || target->GetTypeId() != TYPEID_PLAYER)
                    return;

                // Switching aura
                caster->RemoveAura(SPELL_CHAMPION_OF_LIGHT_HEROIC);
                target->AddAura(SPELL_CHAMPION_OF_LIGHT_HEROIC, target);

                // All Dread Spawns change their target
                std::list<Creature*> dreadList;
                GetCreatureListWithEntryInGrid(dreadList, caster, NPC_DREAD_SPAWN, 300.0f);

                if (!dreadList.empty())
                    for (Creature* dreadSpawn : dreadList)
                        dreadSpawn->AI()->DoAction(ACTION_TRANSFERT_OF_LIGHT);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_transfer_light_SpellScript::Transfer);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_transfer_light_SpellScript();
        }
};

// 120672 - Implacable Strike
class spell_implacable_strike : public SpellScriptLoader
{
    public:
        spell_implacable_strike() : SpellScriptLoader("spell_implacable_strike") { }

        class spell_implacable_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_implacable_strike_SpellScript);

            void Turn()
            {
                if (Unit* caster = GetCaster())
                    caster->SetFacingTo(caster->GetOrientation());
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_implacable_strike_SpellScript::Turn);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_implacable_strike_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_sha_of_fear()
{
    new boss_sha_of_fear();             // 60999
    new mob_pure_light_terrace();       // 60788
    new mob_return_to_the_terrace();    // 65736
    new mob_travel_to_dread_expanse();  // 61735
    new mob_terror_spawn();             // 61034
    new mob_shrine_guardian();          // 61038 - 61042 - 61046
    new mob_sha_globe();                // 65691
    new mob_dread_spawn();              // 61003
    new spell_breath_of_fear();         // 119414 - 125786
    new spell_conjure_terror_spawn();   // 119108
    new spell_penetrating_bolt();       // 129075
    new spell_ominous_cackle_cast();    // 119593
    new spell_dread_spray();            // 120047
    new spell_dread_spray_stacks();     // 119983
    new spell_sha_globe_periodic();     // 129189
    new spell_death_blossom_periodic(); // 119888
    new spell_death_blossom_damage();   // 119887
    new spell_submerge();               // 120455
    new spell_emerge();                 // 120458
    new spell_sha_waterspout();         // 120519
    new spell_transfer_light();         // 120285
    new spell_implacable_strike();      // 120672
}
#endif
