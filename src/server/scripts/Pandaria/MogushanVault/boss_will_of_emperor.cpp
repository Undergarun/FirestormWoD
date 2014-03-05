/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "mogu_shan_vault.h"

enum eSpells
{
    // Jan Xi & Qin Xi
    SPELL_STOMP                 = 116969,
    SPELL_DEVAST_ARC            = 117006,
    SPELL_DEVAST_ARC_2          = 116835,
    SPELL_ARC_VISUAL            = 116968,
    SPELL_MAGNETIC_ARMOR_QIN    = 116815,
    SPELL_MAGNETIZED_QIN        = 116818,
    SPELL_MAGNETIC_PULL_QIN     = 116919,
    SPELL_MAGNETIC_ARMOR_JAN    = 117193,
    SPELL_MAGNETIZED_JAN        = 117195,
    SPELL_MAGNETIC_PULL_JAN     = 117194,
    SPELL_FREEZE_ANIM           = 16245,
    SPELL_OPPORTUNISTIC_STRIKE  = 116808,
    SPELL_GROWING_OPPORTUNITY   = 117854,

    // Ancient Mogu Machine
    SPELL_TITAN_GAS             = 116779,
    SPELL_TITAN_GAS_AURA        = 116803,
    SPELL_TITAN_GAS_AURA2       = 118327,

    // Rage
    SPELL_FOCALISED_ASSAULT     = 116525,
    SPELL_WITHOUT_ARMOR         = 116535,

    // Courage
    SPELL_FOCALISED_DEFENSE     = 116778,
    SPELL_IMPEDING_THRUST       = 117485,
    SPELL_HALF_PLATE            = 116537,
    SPELL_PHALANX_WALL          = 116549,

    // Strength
    SPELL_ENERGIZING_SMASH      = 116550,
    SPELL_ENERGIZING_VISUAL     = 116556,
    SPELL_ENERGIZED             = 116605,
    SPELL_FULL_PLATE            = 116540,

    // Titan Spark - Heroic
    SPELL_SUMMON_TITAN_SPARK    = 117746,
    SPELL_FOCALISED_ENERGY      = 116829,
    SPELL_ENERGY_OF_CREATION    = 116805,

    // Shared
    SPELL_TERRACOTTA_SKYBEAM_S  = 118063,
    SPELL_TERRACOTTA_SKYBEAM_M  = 118060,
    SPELL_TERRACOTTA_SPAWN      = 118165,

    // Visual
    SPELL_COSMETIC_LIGHTNING    = 127732
};

enum eEvents
{
    // Jan & Qin
    EVENT_DEVASTATING_COMBO     = 1,
    EVENT_CHECK_MAGNETIC_ARMOR  = 2,
    EVENT_SUMMON_RAGE           = 4,
    EVENT_SUMMON_STRENGTH       = 5,
    EVENT_SUMMON_COURAGE        = 6,
    EVENT_BOSS_CAST_SKYBEAM     = 100,
    EVENT_BOSS_CAST_SPAWNIN     = 101,
    EVENT_BOSS_WAIT             = 102,
    EVENT_BOSS_WAIT_VISIBLE     = 103,
    EVENT_BOSS_EMOTE            = 104,
    EVENT_BOSS_FREEZE           = 105,

    // Shared
    EVENT_CHECK_TARGET          = 10,
    EVENT_WAIT                  = 11,
    EVENT_WAIT_VISIBLE          = 12,
    EVENT_CAST_SKYBEAM          = 13,
    EVENT_CAST_SPAWNIN          = 14,
    EVENT_TITAN_GAS             = 16,
    EVENT_END_TITAN_GAS         = 17,

    // Courage
    EVENT_IMPEDING_THRUST       = 20,
    EVENT_CHECK_FOCDEF          = 21,
    EVENT_PHALANX_WALL          = 22,

    // Strenght
    EVENT_ENERGIZING_SMASH      = 30,
    EVENT_SELECT_TARGET         = 31,
    EVENT_ENERGIZING_VISUAL     = 33,

    // Rage
    EVENT_RAGE_FIRST_ATTACK     = 32
};

enum eAddActions
{
    // Bosses action
    ACTION_ACTIVATE             = 0,
    ACTION_REACHHOME            = 1,

    // Adds actions
    ACTION_LAND                 = 2,
    ACTION_COSMECTIC            = 3,
    ACTION_MOGU_ACTIVATE        = 4,
};

enum eDisplayID
{
    DISPLAY_BOSS_INVISIBLE      = 15880,
    DISPLAY_ADD_INVISIBLE       = 11686,
    // For each creature (Boss or adds), we have : entry - 19008 = displayId, so we can get the right modelId with me->SetDisplayID(me->GetEntry() - 19008);
    DISPLAY_VISIBLE             = 19008
};

enum eWeapons
{
    WEAPON_RAGE                 = 81169,
    WEAPON_STRENGTH             = 81170,
    WEAPON_COURAGE_LEFT         = 81171,
    WEAPON_COURAGE_RIGHT        = 81172,
    WEAPON_JAN_XI               = 80289,
    WEAPON_QIN_XI               = 80939
};

enum eTalk
{
    TALK_JAN_QIN_XI             = 0,
    TALK_RAGE                   = 1,
    TALK_STRENGTH               = 2,
    TALK_COURAGE                = 3,
    TALK_TITAN_GAS_START        = 4,
    TALK_TITAN_GAS_END          = 5,
    TALK_DEFEATED               = 6
};

#define CENTER_X 3869.67f
#define CENTER_Y 1550.34f
#define CENTER_Z 362.282f

// Achievement
#define ACHIEVE 6455

// Starting positions for adds
float tabAlcoves[46][4] =
{
    3810.64f, 1550.48f, 428.737f, 6.27265f,
    3810.7f,  1550.43f, 459.262f, 6.27481f,
    3811.0f,  1550.39f, 367.725f, 6.2521f,
    3811.32f, 1536.29f, 519.944f, 0.197641f,
    3811.62f, 1536.66f, 398.334f, 0.749377f,
    3811.73f, 1536.68f, 489.336f, 0.178363f,
    3811.95f, 1536.81f, 428.738f, 0.172334f,
    3811.99f, 1536.68f, 367.726f, 0.22558f,
    3812.12f, 1550.07f, 398.25f,  0.00872824f,
    3812.12f, 1564.02f, 428.738f, 5.77924f,
    3812.33f, 1564.24f, 367.726f, 6.03922f,
    3812.41f, 1563.64f, 489.336f, 5.78941f,
    3812.53f, 1563.66f, 519.944f, 5.78551f,
    3812.83f, 1563.62f, 367.643f, 5.77969f,
    3813.13f, 1563.47f, 398.252f, 5.77872f,
    3813.35f, 1537.36f, 367.643f, 0.150286f,
    3816.55f, 1583.32f, 489.92f,  5.64359f,
    3816.64f, 1516.6f,  489.92f,  0.509466f,
    3816.74f, 1516.6f,  459.844f, 0.512551f,
    3816.99f, 1583.02f, 520.525f, 5.64269f,
    3817.89f, 1517.4f,  520.525f, 0.503707f,
    3831.79f, 1602.73f, 398.912f, 5.41683f,
    3832.32f, 1498.53f, 368.308f, 0.842962f,
    3850.7f,  1613.53f, 368.308f, 5.07364f,
    3851.34f, 1486.52f, 368.308f, 1.23063f,
    3873.66f, 1616.41f, 520.521f, 4.70287f,
    3873.6f,  1616.08f, 459.841f, 4.70587f,
    3873.93f, 1615.6f,  429.237f, 4.68575f,
    3874.22f, 1484.84f, 429.236f, 1.57923f,
    3874.31f, 1483.42f, 368.308f, 1.55837f,
    3874.32f, 1618.01f, 368.308f, 4.71767f,
    3874.39f, 1484.9f,  520.521f, 1.58646f,
    3874.55f, 1483.5f,  398.912f, 1.59109f,
    3874.61f, 1617.9f,  398.912f, 4.75497f,
    3874.7f,  1485.1f,  489.917f, 1.59977f,
    3874.86f, 1484.86f, 459.841f, 1.60658f,
    3894.75f, 1485.93f, 368.308f, 1.48814f,
    3894.93f, 1615.93f, 459.841f, 4.62488f,
    3895.03f, 1483.62f, 398.912f, 1.48814f,
    3895.04f, 1616.19f, 520.521f, 4.61926f,
    3895.24f, 1484.05f, 520.522f, 1.58261f,
    3895.25f, 1485.82f, 368.226f, 1.56936f,
    3895.26f, 1485.07f, 489.917f, 1.58457f,
    3895.27f, 1484.05f, 459.841f, 1.58405f,
    3895.53f, 1617.47f, 398.912f, 4.8269f,
    3895.62f, 1485.9f,  398.829f, 1.56936f
};

#define NB_ALCOVES 46

// Qin-Xi - 60399
// Jan-Xi - 60400
class boss_jin_qin_xi : public CreatureScript
{
    public:
        boss_jin_qin_xi() : CreatureScript("boss_jin_qin_xi") {}

        struct boss_jin_qin_xiAI : public BossAI
        {
            boss_jin_qin_xiAI(Creature* creature) : BossAI(creature, DATA_WILL_OF_EMPEROR), summons(creature)
            {
                pInstance = creature->GetInstanceScript();
                isActive = false;
                me->SetDisplayId(DISPLAY_BOSS_INVISIBLE);
                me->setFaction(35);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                sumCourage = 0;
            }

            InstanceScript* pInstance;
            EventMap events;
            SummonList summons;
            int sumCourage;
            uint8 maxCombo;
            int comboArc;
            uint8 janHitCount;
            uint8 qinHitCount;
            bool achievement;
            Position homePos;

            uint8 devastatingComboPhase;
            uint64 victimWithMagneticArmor;

            float moveTurn;
            float moveWalk;
            float moveRun;

            bool isActive;

            std::list<uint64> playerList;

            void Reset()
            {
                if (!me->isAlive())
                    return;

                me->ResetLootMode();
                events.Reset();
                summons.DespawnAll();

                moveTurn = me->GetSpeed(MOVE_TURN_RATE);
                moveWalk = me->GetSpeed(MOVE_WALK);
                moveRun  = me->GetSpeed(MOVE_RUN);
                homePos  = me->GetHomePosition();
                
                victimWithMagneticArmor = 0;

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIC_ARMOR_QIN);
                }

                playerList.clear();

                achievement = false;
                janHitCount = 0;
                qinHitCount = 0;

            }

            void JustReachedHome()
            {
                _JustReachedHome();

                me->SetDisplayId(DISPLAY_BOSS_INVISIBLE);
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
                events.Reset();
                summons.DespawnAll();

                if (Creature* otherBoss = getOtherBoss())
                    otherBoss->AI()->DoAction(ACTION_REACHHOME);

                if (pInstance)
                    pInstance->SetBossState(DATA_WILL_OF_EMPEROR, FAIL);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                // Applies only if hit by opportunistic strike
                if (spell->Id != SPELL_OPPORTUNISTIC_STRIKE)
                    return;

                // Increases the right counter as all the strikes must be performed on the same boss
                if (me->GetEntry() == NPC_JAN_XI)
                    ++janHitCount;
                else
                    ++qinHitCount;

                // Get the number of players
                uint8 team = me->GetMap()->GetPlayers().getSize();

                // If all the players have done the opportunistic strike on the same boss, the achievement is done
                if (janHitCount == team || qinHitCount == team)
                    if (me->GetInstanceScript()->GetData(ACHIEVE))
                        me->GetInstanceScript()->SetData(ACHIEVE, 1);
            }

            void EnterCombat(Unit* who)
            {
                DoZoneInCombat();

                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            }

            void MovementInform(uint32 uiType, uint32 id)
            {
                if (uiType != POINT_MOTION_TYPE && uiType != EFFECT_MOTION_TYPE)
                    return;

                if (id == 2)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                    events.ScheduleEvent(EVENT_BOSS_EMOTE, 2000);
                }
            }

            void JustDied(Unit* killer)
            {
                events.Reset();
                summons.DespawnAll();

                if (Creature* cho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 60.0f, true))
                        cho->AI()->DoAction(ACTION_EMPERORS_DEATH);

                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                if (me->GetEntry() == NPC_QIN_XI)
                {
                    if (Creature* anc_mogu_machine = GetClosestCreatureWithEntry(me, NPC_ANCIENT_MOGU_MACHINE, 200.0f))
                    {
                        anc_mogu_machine->AI()->DoAction(TALK_DEFEATED);
                        me->Kill(anc_mogu_machine->ToUnit());
                    }
                }

                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STOMP);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEVAST_ARC);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEVAST_ARC_2);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TITAN_GAS);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARC_VISUAL);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIC_ARMOR_JAN);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIC_ARMOR_QIN);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ENERGY_OF_CREATION);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIZED_JAN);
                pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIZED_QIN);

                pInstance->SetBossState(DATA_WILL_OF_EMPEROR, DONE);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ACTIVATE:
                        if (isActive)
                            return;

                        // Appearing effect
                        isActive = true;
                        events.ScheduleEvent(EVENT_BOSS_CAST_SKYBEAM, 90000);

                        // 5 or 10 attacks, each combo is made of 2 phases - Using 10 by default
                        maxCombo = IsHeroic() ? 20 : 10;
                        devastatingComboPhase = 0;
                        
                        // --- Summoning adds ---
                        events.ScheduleEvent(EVENT_SUMMON_RAGE, 11000);

                        // --- Attacks ---
                        events.ScheduleEvent(EVENT_DEVASTATING_COMBO, 115000);
                        events.ScheduleEvent(EVENT_CHECK_MAGNETIC_ARMOR, 112000);
                        break;
                    case ACTION_REACHHOME:
                        me->GetMotionMaster()->MovePoint(9, homePos);
                        me->SetReactState(REACT_PASSIVE);
                        break;
                }
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id != SPELL_DEVAST_ARC && spell->Id != SPELL_STOMP)
                    return;

                for (uint64 guid : playerList)
                {
                    Player* player = me->GetPlayer(*me, guid);
                    if (player && player->GetGUID() == target->GetGUID())
                    {
                        playerList.remove(guid);
                        break;
                    }
                }
            }

            Creature* getOtherBoss()
            {
                if (pInstance)
                    return pInstance->instance->GetCreature(pInstance->GetData64(me->GetEntry() == NPC_QIN_XI ? NPC_JAN_XI: NPC_QIN_XI));
                else
                    return NULL;
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                Creature* otherBoss = getOtherBoss();

                if (!otherBoss)
                    return;

                if (attacker != otherBoss)
                    me->DealDamage(otherBoss, damage);
                else
                    me->LowerPlayerDamageReq(damage);
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateVictim();

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch(events.ExecuteEvent())
                {
                    case EVENT_BOSS_CAST_SKYBEAM:
                    {
                        // 1st halo, still invisible
                        me->CastSpell(me, SPELL_TERRACOTTA_SKYBEAM_S, false);
                        events.ScheduleEvent(EVENT_BOSS_CAST_SPAWNIN, 6000);
                        break;
                    }
                    case EVENT_BOSS_CAST_SPAWNIN:
                    {
                        // 2nd halo, still invisible
                        me->CastSpell(me, SPELL_TERRACOTTA_SPAWN, false);
                        if (me->GetEntry() == NPC_JAN_XI)
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 300.0f);
                            for (auto ply : playerList)
                            {
                                ply->MonsterTextEmote("Two titanic constructs appear in the large alcoves!", 0, true);
                                break;
                            }
                        }
                        events.ScheduleEvent(EVENT_BOSS_WAIT, 500);
                        break;
                    }
                    case EVENT_BOSS_WAIT:
                    {
                        // Add weapon
                        me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, (me->GetEntry() == NPC_JAN_XI ? WEAPON_JAN_XI : WEAPON_QIN_XI));
                        // Become visible, but wait till the end of halos
                        me->SetDisplayId(me->GetEntry() - DISPLAY_VISIBLE);
                        // Only Qin-Xi makes the machine talk, to avoid "double voices"
                        if (me->GetEntry() == NPC_QIN_XI)
                        {
                            if (Creature* anc_mogu_machine = GetClosestCreatureWithEntry(me, NPC_ANCIENT_MOGU_MACHINE, 200.0f))
                                anc_mogu_machine->AI()->DoAction(TALK_JAN_QIN_XI);
                        }
                        events.ScheduleEvent(EVENT_BOSS_WAIT_VISIBLE, 2000);
                        break;
                    }
                    case EVENT_BOSS_WAIT_VISIBLE:
                    {
                        // Turn into ennemy and jump in the room
                        me->setFaction(14);
                        // Landing coords 1st boss
                        float x = me->GetPositionX() + (15 * cos(me->GetOrientation()));
                        float y = me->GetPositionY() + (15 * sin(me->GetOrientation()));
                        // Jump
                        me->GetMotionMaster()->MoveJump(x, y, 362.19f, 20.0f, 20.0f, 2);
                        break;
                    }
                    case EVENT_BOSS_EMOTE:
                    {
                        Map::PlayerList const& players = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        {
                            if (Player* plr = itr->getSource())
                            {
                                if (plr->GetDistance(me) > 50.0f)
                                    continue;
                                me->SetInCombatWith(plr);
                                me->Attack(plr, true);
                                break;
                            }
                        }
                        break;
                    }
                    case EVENT_SUMMON_RAGE:
                    {
                        // Jan-Xi is the only one who summons, to avoid double summon
                        if (me->GetEntry() == NPC_JAN_XI)
                        {
                            // Two Emperor's rage at each summon
                            int alcove1 = urand(0, NB_ALCOVES - 1);
                            int alcove2 = urand(0, NB_ALCOVES - 1);
                            while (alcove1 == alcove2)
                                alcove2 = urand(0, NB_ALCOVES - 1);
                            me->SummonCreature(NPC_EMPEROR_RAGE, tabAlcoves[alcove1][0], tabAlcoves[alcove1][1], tabAlcoves[alcove1][2], tabAlcoves[alcove1][3]);
                            me->SummonCreature(NPC_EMPEROR_RAGE, tabAlcoves[alcove2][0], tabAlcoves[alcove2][1], tabAlcoves[alcove2][2], tabAlcoves[alcove2][3]);
                            
                            // Summoning other add : Emperor's Strength (if sumCourage == 0) or Empreror's Courage (if sumCourage == 1)
                            uint32 delaySummon = 1000 * urand(5, 10);
                            events.ScheduleEvent(EVENT_SUMMON_STRENGTH + sumCourage, delaySummon);
                            // Changing sumCourage from 0 to 1 or from 1 to 0
                            sumCourage = 1 - (sumCourage % 2);

                            // Scheduling next summon phase
                            uint32 delay = 1000 * urand(32, 40);
                            events.ScheduleEvent(EVENT_SUMMON_RAGE, delay);
                        }
                        break;
                    }
                    case EVENT_SUMMON_STRENGTH:
                    {
                        // Choose an alcove
                        int alcove = urand(0, NB_ALCOVES - 1);
                        me->SummonCreature(NPC_EMPEROR_STRENGHT, tabAlcoves[alcove][0], tabAlcoves[alcove][1], tabAlcoves[alcove][2], tabAlcoves[alcove][3]);
                        break;
                    }
                    case EVENT_SUMMON_COURAGE:
                    {
                        // Choose an alcove
                        int alcove = urand(0, NB_ALCOVES - 1);
                        me->SummonCreature(NPC_EMPEROR_COURAGE, tabAlcoves[alcove][0], tabAlcoves[alcove][1], tabAlcoves[alcove][2], tabAlcoves[alcove][3]);
                        break;
                    }
                    /* DEVASTATING COMBO */
                    case EVENT_DEVASTATING_COMBO:
                    {
                        // Initializing Combo
                        if (!devastatingComboPhase)
                        {
                            // reset oppotunistic strikes counters
                            janHitCount = 0;
                            qinHitCount = 0;

                            // Stop looking after victim
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                            
                            // Emptying player list from previous combo
                            playerList.clear();

                            std::list<Player*> tmpPlayerList;
                            // We check which players are in zone on combo beginning
                            GetPlayerListInGrid(tmpPlayerList, me, 12.0f);

                            for (auto itr : tmpPlayerList)
                                playerList.push_back(itr->GetGUID());

                            // Set Turning Speed
                            me->SetSpeed(MOVE_TURN_RATE, 10.0f, true);
                            me->SetSpeed(MOVE_WALK, 0.0f, true);
                            me->SetSpeed(MOVE_RUN, 0.0f, true);
                        }

                        // Either devastatingComboPhase = 0 (not started) or is pair (attack ended) : starting attack + freeze for 2.5 secs
                        if (!(devastatingComboPhase % 2))
                        {
                            // Cancel previous emote in order to properly play a new one
                            me->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
                            // Pick a random new orientation
                            float angle = me->GetOrientation() + frand(-0.75f, 0.75f);
                            // angle corrections
                            angle = angle < 0.0f ? 0.0f : (angle > 6.28f ? 6.28f : angle);
                            // Stomp or arc
                            if (comboArc = urand(0, 1))
                            {
                                // Emote for arc attack
                                me->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK2HLOOSE);
                                // Set new orientation (after emote)
                                me->SetFacingTo(angle);
                                // Wait a bit the anim starts to play before freezing it
                                events.ScheduleEvent(EVENT_BOSS_FREEZE, 300);
                            }
                            else
                            {
                                // Emote for stomp
                                me->HandleEmoteCommand(EMOTE_ONESHOT_STOMP);
                                // Set orientation (after emote)
                                me->SetFacingTo(angle);
                                // Freezing right now
                                me->AddAura(SPELL_FREEZE_ANIM, me);
                                // Reaching a not pair number to release the attack
                                ++devastatingComboPhase;
                                // Waiting for releasing attack
                                events.ScheduleEvent(EVENT_DEVASTATING_COMBO, 2600);
                            }
                        }
                        // devastatingComboPhase not pair : releasing attack
                        else
                        {
                            // Releasing attack
                            if (comboArc)
                                me->CastSpell(me, SPELL_DEVAST_ARC, true);
                            else
                                me->CastSpell(me, SPELL_STOMP, true);
                            // Can move anew
                            me->RemoveAura(SPELL_FREEZE_ANIM);
                            // Resetting weapons (removed by emote)
                            me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, (me->GetEntry() == NPC_JAN_XI ? WEAPON_JAN_XI : WEAPON_QIN_XI));
                            me->CastSpell(me, SPELL_DEVAST_ARC_2, true);
                            // Ending the attack by reaching a pair number
                            ++devastatingComboPhase;

                            std::list<Player*> aliveList;
                            std::list<Player*> plyrList;
                            aliveList.clear();
                            GetPlayerListInGrid(plyrList, me, 500.0f);
                            for (auto plyr : plyrList)
                                if (plyr->isAlive())
                                    aliveList.push_back(plyr);

                            // Scheduling
                            if (devastatingComboPhase < maxCombo && !aliveList.empty())
                            {
                                // Adding Growing Opportunity for remaining players
                                for (auto playerGuid : playerList)
                                {
                                    if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                                        me->AddAura(SPELL_GROWING_OPPORTUNITY, player);
                                }
                                // Combos still remain to perform - wait a bit (emote must end) before a new move
                                events.ScheduleEvent(EVENT_DEVASTATING_COMBO, 700);
                            }
                            else
                            {
                                if (aliveList.empty())
                                {
                                    devastatingComboPhase = 0;
                                    pInstance->SetBossState(DATA_WILL_OF_EMPEROR, FAIL);
                                    DoAction(ACTION_REACHHOME);
                                    if (Creature* otherBoss = getOtherBoss())
                                        otherBoss->AI()->DoAction(ACTION_REACHHOME);
                                }
                                // All combo have been done, and each player who has been hit is away from playerList
                                // If players remains in playerList, they gain Opportunistic Strike
                                for (auto guid: playerList)
                                    if (Player* player = ObjectAccessor::FindPlayer(guid))
                                        player->AddAura(SPELL_OPPORTUNISTIC_STRIKE, player);
                                // Reset next combo
                                devastatingComboPhase = 0;
                                me->SetSpeed(MOVE_TURN_RATE, moveTurn, false);
                                me->SetSpeed(MOVE_WALK, moveWalk, false);
                                me->SetSpeed(MOVE_RUN, moveRun, false);
                                // Reset attacking to normal
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                                

                                events.ScheduleEvent(EVENT_DEVASTATING_COMBO, urand(20000, 30000));
                            }
                        }
                        break;
                    }
                    case EVENT_BOSS_FREEZE:
                    {
                        // Stop moving
                        me->AddAura(SPELL_FREEZE_ANIM, me);
                        // Reaching a not pair number to release the attack
                        ++devastatingComboPhase;
                        // Wait for releasing attack
                        events.ScheduleEvent(EVENT_DEVASTATING_COMBO, 2300);
                        break;
                    }
                    /* END OF DEVASTATING COMBO */
                    case EVENT_CHECK_MAGNETIC_ARMOR:
                    {
                        if (Unit* victim = me->getVictim())
                        {
                            bool isJan = me->GetEntry() == NPC_JAN_XI;
                            if (!victim->HasAura((isJan ? SPELL_MAGNETIC_ARMOR_JAN : SPELL_MAGNETIC_ARMOR_QIN)))
                            {
                                if (Player* player = ObjectAccessor::FindPlayer(victimWithMagneticArmor))
                                    player->RemoveAurasDueToSpell(isJan ? SPELL_MAGNETIC_ARMOR_JAN : SPELL_MAGNETIC_ARMOR_QIN);

                                me->AddAura(isJan ? SPELL_MAGNETIC_ARMOR_JAN : SPELL_MAGNETIC_ARMOR_QIN, victim);
                                victimWithMagneticArmor = victim->GetGUID();
                            }
                        }
                        events.ScheduleEvent(EVENT_CHECK_MAGNETIC_ARMOR, 1000);
                        break;
                    }
                    default:
                        break;
                }
                // Not in combo : attack players
                if (!devastatingComboPhase)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_jin_qin_xiAI(creature);
        }
};

// Emperor's Rage - 60396
// Emperor's Strength - 60397
// Emperor's Courage - 60398
class mob_woe_add_generic : public CreatureScript
{
    public:
        mob_woe_add_generic() : CreatureScript("mob_woe_add_generic") {}

        struct mob_woe_add_genericAI : public ScriptedAI
        {
            mob_woe_add_genericAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            uint64 targetGuid;
            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();

                if (!pInstance)
                    return;
                // Won't attack
                me->setFaction(35);
                // Invisible
                me->SetDisplayId(DISPLAY_ADD_INVISIBLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                
                // Wait before casting
                events.ScheduleEvent(EVENT_CAST_SKYBEAM, 1000);

                if (me->GetEntry() == NPC_EMPEROR_COURAGE || me->GetEntry() == NPC_EMPEROR_RAGE)
                {
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_THREAT, true);
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_PULL, true);
                }
            }

            void MovementInform(uint32 uiType, uint32 id)
            {
                if (uiType != POINT_MOTION_TYPE && uiType != EFFECT_MOTION_TYPE)
                    return;

                if (id == 1)
                {
                    targetGuid = 0;
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    // Don't remove Immune to PC/NPC for Rage & Stength
                    if (me->GetEntry() == NPC_EMPEROR_COURAGE)
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);

                    switch (me->GetEntry())
                    {
                        case NPC_EMPEROR_RAGE:
                        {
                            me->AddAura(SPELL_WITHOUT_ARMOR, me);
                            me->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
                            events.ScheduleEvent(EVENT_RAGE_FIRST_ATTACK, 2000);
                            break;
                        }
                        case NPC_EMPEROR_COURAGE:
                        {
                            me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                            me->AddAura(SPELL_HALF_PLATE, me);

                            events.ScheduleEvent(EVENT_IMPEDING_THRUST, 5000);
                            events.ScheduleEvent(EVENT_PHALANX_WALL, 3000);
                            events.ScheduleEvent(EVENT_CHECK_FOCDEF, 3500);
                            break;
                        }
                        case NPC_EMPEROR_STRENGHT:
                        {
                            DoAction(ACTION_LAND);
                            events.ScheduleEvent(EVENT_ENERGIZING_SMASH, urand(5000, 10000));
                            me->AddAura(SPELL_FULL_PLATE, me);
                            break;
                        }
                    }
                }
            }

            void JustDied(Unit* attacker)
            {
                if (IsHeroic())
                    me->CastSpell(me, SPELL_SUMMON_TITAN_SPARK, true);
                // Cancel aura for add which are spawning in when bosses die
                me->RemoveAllAuras();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_LAND:
                    {
                        // Retreiving players around
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 200.0f);

                        // Add players in the list
                        for (auto player: playerList)
                            me->getThreatManager().addThreat(player, 300.0f);

                        // Pick a player to attack
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->getThreatManager().resetAllAggro();
                            targetGuid = target->GetGUID();
                            me->getThreatManager().addThreat(target, 300.0f);
                            AttackStart(target);
                            me->SetInCombatWith(target);
                            if (me->GetEntry() == NPC_EMPEROR_RAGE)
                                me->CastSpell(target, SPELL_FOCALISED_ASSAULT, true);
                        }
                        break;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateVictim();
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CAST_SKYBEAM:
                        {
                            // Cast 1st halo
                            if (me->GetEntry() == NPC_EMPEROR_RAGE)
                                me->CastSpell(me, SPELL_TERRACOTTA_SKYBEAM_S, false);
                            else
                                me->CastSpell(me, SPELL_TERRACOTTA_SKYBEAM_M, false);
                            // Wait invisible
                            events.ScheduleEvent(EVENT_CAST_SPAWNIN, 7000);
                            break;
                        }
                        case EVENT_CAST_SPAWNIN:
                        {
                            // Cast 2nd halo
                            me->CastSpell(me, SPELL_TERRACOTTA_SPAWN, false);

                            // Boss Emote - Not displayed if done by mob as he's invisible, so done by a random player
                            if (me->GetEntry() != NPC_EMPEROR_RAGE)
                            {
                                std::ostringstream text;
                                text << "The " << (me->GetEntry() == NPC_EMPEROR_STRENGHT ? "Emperor's Strength" : "Emperor's Courage") << " appears in the alcoves!";
                                std::list<Player*> playerList;
                                GetPlayerListInGrid(playerList, me, 300.0f);
                                if (!playerList.empty())
                                {
                                    for (auto plr : playerList)
                                    {
                                        plr->MonsterTextEmote(text.str().c_str(), 0, true);
                                        break;
                                    }
                                }
                            }
                            // Wait invisible
                            events.ScheduleEvent(EVENT_WAIT, 5000);
                            break;
                        }
                        case EVENT_WAIT:
                        {
                            // Add weapons
                            switch(me->GetEntry())
                            {
                                case NPC_EMPEROR_RAGE:
                                {
                                    me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, WEAPON_RAGE);
                                    me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, WEAPON_RAGE);
                                    break;
                                }
                                case NPC_EMPEROR_STRENGHT:
                                {
                                    me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, WEAPON_STRENGTH);
                                    break;
                                }
                                case NPC_EMPEROR_COURAGE:
                                {
                                    me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, WEAPON_COURAGE_LEFT);
                                    me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, WEAPON_COURAGE_RIGHT);
                                }
                            }
                            // Set visible
                            me->SetDisplayId(me->GetEntry() - DISPLAY_VISIBLE);
                            // Talk - Choose the right sentence according to the mob
                            int32 words = me->GetEntry() == NPC_EMPEROR_RAGE ? TALK_RAGE : (me->GetEntry() == NPC_EMPEROR_STRENGHT ? TALK_STRENGTH : TALK_COURAGE);
                            if (Creature* anc_mogu_machine = GetClosestCreatureWithEntry(me, NPC_ANCIENT_MOGU_MACHINE, 200.0f))
                                anc_mogu_machine->AI()->DoAction(words);
                            // Wait til spell ends, before jumping
                            events.ScheduleEvent(EVENT_WAIT_VISIBLE, 2000);
                            break;
                        }
                        case EVENT_WAIT_VISIBLE:
                        {
                            me->setFaction(14);
                            // Landing coords
                            float x = me->GetPositionX() + (15 * cos(me->GetOrientation()));
                            float y = me->GetPositionY() + (15 * sin(me->GetOrientation()));
                            // Jump
                            me->GetMotionMaster()->MoveJump(x, y, 362.19f, 20.0f, 20.0f, 1);
                            break;
                        }
                        // Rage
                        case EVENT_RAGE_FIRST_ATTACK:
                        {
                            DoAction(ACTION_LAND);
                            events.ScheduleEvent(EVENT_CHECK_TARGET, 2000);
                            break;
                        }
                        case EVENT_CHECK_TARGET:
                        {
                            bool shouldSwitchVictim = false;

                            if (me->getVictim())
                            {
                                if (me->getVictim()->GetGUID() != targetGuid)
                                {
                                    if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                                    {
                                        if (!target->isAlive())
                                            shouldSwitchVictim = true;
                                    }
                                    else
                                        shouldSwitchVictim = true;
                                }
                                else
                                {
                                    if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                                    {
                                        if (target->isAlive())
                                        {
                                            targetGuid = target->GetGUID();
                                            AttackStart(target);
                                            me->SetInCombatWith(target);
                                            me->CastSpell(target, SPELL_FOCALISED_ASSAULT, true);
                                        }
                                    }
                                }
                            }

                            if (shouldSwitchVictim)
                                DoAction(ACTION_LAND);

                            events.ScheduleEvent(EVENT_CHECK_TARGET, 2000);
                            break;
                        }
                        // Courage
                        case EVENT_PHALANX_WALL:
                        {
                            me->CastSpell(me, SPELL_PHALANX_WALL, false);
                            break;
                        }
                        case EVENT_IMPEDING_THRUST:
                        {
                            if (Unit* victim = me->getVictim())
                            {
                                if (me->IsWithinMeleeRange(victim))
                                {
                                    me->CastSpell(victim, SPELL_IMPEDING_THRUST, false);
                                    events.ScheduleEvent(EVENT_IMPEDING_THRUST, 10000);
                                }
                                else
                                    events.ScheduleEvent(EVENT_IMPEDING_THRUST, 2000);
                            }
                            else
                                events.ScheduleEvent(EVENT_IMPEDING_THRUST, 2000);
                            break;
                        }
                        case EVENT_CHECK_FOCDEF:
                        {
                            Unit* target;
                            if (!me->getVictim())
                            {
                                // Check bosses
                                if (Creature* janxi = pInstance->instance->GetCreature(pInstance->GetData64(NPC_JAN_XI)))
                                    if (Creature* qinxi = pInstance->instance->GetCreature(pInstance->GetData64(NPC_QIN_XI)))
                                {
                                    // Retrieving tanks on bosses
                                    Unit* tankj = janxi->getVictim();
                                    Unit* tankq = qinxi->getVictim();

                                    // Checking distance : Courage will go on the further tank
                                    if (tankj && tankq)
                                        if (me->GetDistance(tankj) > me->GetDistance(tankq))
                                            target = tankj;
                                        else
                                            target = tankq;
                                    // Not 2 tanks : check if there's 1 at least and go on him
                                    else if (tankj)
                                        target = tankj;
                                    else if (tankq)
                                        target = tankq;
                                    // No tank : pick a random player
                                    else
                                    {
                                        std::list<Player*> playerList;
                                        GetPlayerListInGrid(playerList, me, 150.0f);

                                        for (auto ply : playerList)
                                            me->getThreatManager().addThreat(ply, 150.0f);
                                    
                                        if (target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                        {
                                            targetGuid = target->GetGUID();
                                            AttackStart(target);
                                            me->Attack(target, true);
                                            me->SetInCombatWith(target);

                                            me->CastSpell(target, SPELL_FOCALISED_DEFENSE, false);
                                            me->SetUInt32Value(UNIT_CHANNEL_SPELL, SPELL_FOCALISED_DEFENSE);
                                        }
                                    }
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_FOCDEF, 2000);
                            break;
                        }
                        // Strenght
                        case EVENT_ENERGIZING_SMASH:
                        {
                            me->CastSpell(me, SPELL_ENERGIZING_SMASH, false);

                            AuraPtr energized = me->GetAura(SPELL_ENERGIZED);
                            float dist = 10.0f + (energized ? energized->GetStackAmount() : 0.0f);

                            std::list<Player*> tarList;
                            GetPlayerListInGrid(tarList, me, dist);
                            
                            for (auto target : tarList)
                                me->AddAura(SPELL_ENERGIZING_VISUAL, target);

                            if (Unit* victim = me->getVictim())
                                me->CastSpell(victim, SPELL_ENERGIZING_VISUAL, true);

                            events.ScheduleEvent(EVENT_ENERGIZING_SMASH, urand(15000, 20000));

                            break;
                        }
                        case EVENT_SELECT_TARGET:
                        {
                            if (!me->getVictim())
                                DoAction(ACTION_LAND);
                            events.ScheduleEvent(EVENT_SELECT_TARGET, 2000);
                            break;
                        }
                    }
                    if (!me->HasUnitState(UNIT_STATE_CASTING))
                        DoMeleeAttackIfReady();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_woe_add_genericAI(creature);
        }
};

// Titan Spark - 60480
class mob_woe_titan_spark : public CreatureScript
{
    public:
        mob_woe_titan_spark() : CreatureScript("mob_woe_titan_spark") {}

        struct mob_woe_titan_sparkAI : public ScriptedAI
        {
            mob_woe_titan_sparkAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            uint32 distanceCheckTimer;
            uint64 targetGuid;

            void Reset()
            {
                targetGuid = 0;
                distanceCheckTimer = 500;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                DoZoneInCombat();
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                {
                    targetGuid = target->GetGUID();
                    AttackStart(target);
                    me->SetInCombatWith(target);
                    me->getThreatManager().addThreat(target, 300.0f);
                    me->CastSpell(target, SPELL_FOCALISED_ENERGY, false);
                }
            }

            void JustDied(Unit* attacker)
            {
                me->CastSpell(me, SPELL_ENERGY_OF_CREATION, false);
            }

            void UpdateAI(const uint32 diff)
            {
                if (distanceCheckTimer <= diff)
                {
                    if (me->SelectNearestPlayerNotGM())
                        me->Kill(me);
                    distanceCheckTimer = 500;
                }
                else
                    distanceCheckTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_woe_titan_sparkAI(creature);
        }
};

// Ancient Mogu Machine - 60648
class mob_ancient_mogu_machine : public CreatureScript
{
    public:
        mob_ancient_mogu_machine() : CreatureScript("mob_ancient_mogu_machine") { }
    
        struct mob_ancient_mogu_machineAI : public ScriptedAI
        {

            mob_ancient_mogu_machineAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Reset()
            {
                events.Reset();
            }


            // Talk
            void DoAction(const int32 action)
            {
                if (action == ACTION_MOGU_ACTIVATE)
                    events.ScheduleEvent(EVENT_TITAN_GAS, 225000);
                else
                    Talk(action);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    // Boss emote for the end of Titan Gas. Done by a player as Mogu Machine is invisible.
                    case EVENT_TITAN_GAS:
                    {
                        if (!me->GetMap()->GetPlayers().isEmpty())
                        {
                            if (Player* plr = me->GetMap()->GetPlayers().begin()->getSource())
                            {
                                //|cffffd000|Hspell:spell_id|h[name]|h|r
                                plr->MonsterTextEmote("The Ancient Mogu Machine breaks down! |cffBA0022|Hspell:116779|h[Titan Gas]|h|r floods the room!", 0, true);
                            }
                        }
                        me->CastSpell(CENTER_X, CENTER_Y, CENTER_Z, SPELL_TITAN_GAS, false);
                        // Talk
                        DoAction(TALK_TITAN_GAS_START);
                        events.ScheduleEvent(EVENT_END_TITAN_GAS, 30000);
                        events.ScheduleEvent(EVENT_TITAN_GAS, 210000);
                        break;
                    }
                    case EVENT_END_TITAN_GAS:
                    {
                        // Talk for Titan Gas ending
                        DoAction(TALK_TITAN_GAS_END);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ancient_mogu_machineAI(creature);
        }

};

// General Purpose Bunny JMF - 55091
class mob_general_purpose_bunnyJMF : public CreatureScript
{
    public:
        mob_general_purpose_bunnyJMF() : CreatureScript("mob_general_purpose_bunnyJMF") { }
    
        struct mob_general_purpose_bunnyJMFAI : public ScriptedAI
        {
            bool hasCast;
            bool isCentralMob;

            mob_general_purpose_bunnyJMFAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                hasCast = false;
                isCentralMob = false;
            }

            void Reset()
            {
                me->RemoveAllAuras();
                // Retreiving central mob
                if ((me->GetPositionX() >= (CENTER_X - 2.0f)) && (me->GetPositionX() <= (CENTER_X + 2.0f)) &&
                    (me->GetPositionY() >= (CENTER_Y - 2.0f)) && (me->GetPositionY() <= (CENTER_Y + 2.0f)) &&
                    (me->GetPositionZ() >= (CENTER_Z - 2.0f)) && (me->GetPositionZ() <= (CENTER_Z + 2.0f)))
                    isCentralMob = true;

            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_COSMECTIC)
                {
                    if (!isCentralMob)
                        return;

                    // Central mob casts cosmetic lightning once
                    if (!hasCast)
                    {
                        me->CastSpell(me, SPELL_COSMETIC_LIGHTNING, false);
                        hasCast = true;
                    }
                }
            }

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_general_purpose_bunnyJMFAI(creature);
        }

};

// Cosmetic Lightning Spell - 127732
class spell_cosmetic_lightning : public SpellScriptLoader
{
    public:
        spell_cosmetic_lightning() : SpellScriptLoader("spell_cosmetic_lightning") { }

        class spell_cosmetic_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_cosmetic_lightning_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* caster = GetCaster())
                {
                    targets.clear();

                    std::list<Creature*> focus;
                    GetCreatureListWithEntryInGrid(focus, caster, NPC_GENERAL_PURPOSE_BUNNY_JMF, 500.0f);

                    focus.remove(caster->ToCreature());
                
                    for (auto cible: focus)
                    {
                        targets.push_back(cible);
                        caster->AddAura(SPELL_COSMETIC_LIGHTNING, cible);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_cosmetic_lightning_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_cosmetic_lightning_SpellScript();
        }
};

//  Terracotta spawn visual spawn-in - 118165
class spell_terracota_spawn : public SpellScriptLoader
{
    public:
        spell_terracota_spawn() : SpellScriptLoader("spell_terracota_spawn") { }

        class spell_terracota_spawn_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_terracota_spawn_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_TERRACOTTA_SPAWN, GetCaster());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_terracota_spawn_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuracript() const
        {
            return new spell_terracota_spawn_AuraScript();
        }
};

// Magnetized (Qin-Xi) - 116818
class spell_magnetized_qin : public SpellScriptLoader
{
    public:
        spell_magnetized_qin() : SpellScriptLoader("spell_magnetized_qin") { }

        class spell_magnetized_qin_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_magnetized_qin_AuraScript);

            void Apply(constAuraEffectPtr /*aurAff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetTarget()->ToPlayer())
                    player->AddAura(SPELL_MAGNETIZED_QIN, player);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_magnetized_qin_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_magnetized_qin_AuraScript();
        }
};

// Magnetized (Jan-Xi) - 117195
class spell_magnetized_jan : public SpellScriptLoader
{
    public:
        spell_magnetized_jan() : SpellScriptLoader("spell_magnetized_jan") { }

        class spell_magnetized_jan_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_magnetized_jan_AuraScript);

            void Apply(constAuraEffectPtr /*aurAff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetTarget()->ToPlayer())
                    player->AddAura(SPELL_MAGNETIZED_JAN, player);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_magnetized_jan_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_magnetized_jan_AuraScript();
        }
};

// Arc Left - 116968
class spell_arc_visual : public SpellScriptLoader
{
    public:
        spell_arc_visual() : SpellScriptLoader("spell_arc_visual") { }

        class spell_arc_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arc_visual_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                std::list<Player*> players;
                GetPlayerListInGrid(players, GetCaster(), 10.0f);
                if (Unit* caster = GetCaster())
                    for (auto target : players)
                        caster->AddAura(SPELL_ARC_VISUAL, target);
            }

            void Stun(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                std::list<Player*> players;
                GetPlayerListInGrid(players, GetCaster(), 100.0f);
                if (Unit* caster = GetCaster())
                    for (auto target : players)
                        caster->AddAura(SPELL_ARC_VISUAL, target);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_arc_visual_AuraScript::Stun,  EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_arc_visual_AuraScript::Apply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arc_visual_AuraScript();
        }
};

//  Devastating Arc - 117006
class spell_devastating_arc : public SpellScriptLoader
{
    public:
        spell_devastating_arc() : SpellScriptLoader("spell_devastating_arc") { }

        class spell_devastating_arc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_devastating_arc_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 25.0f);

                    for (auto target : playerList)
                        caster->AddAura(SPELL_DEVAST_ARC, target);
                    }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_devastating_arc_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_devastating_arc_AuraScript();
        }
};

// Impeding Thrust - 117485
class spell_impeding_thrust : public SpellScriptLoader
{
    public:
        spell_impeding_thrust() : SpellScriptLoader("spell_impeding_thrust") { }

        class spell_impeding_thrust_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_impeding_thrust_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetTarget()->ToPlayer())
                {
                    AuraPtr impeding = player->GetAura(SPELL_IMPEDING_THRUST);
                    if (impeding->GetStackAmount() < 4)
                        impeding->SetStackAmount(impeding->GetStackAmount() + 1);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_impeding_thrust_AuraScript::Apply, EFFECT_0, SPELL_AURA_NONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_impeding_thrust_AuraScript();
        }
};

// Titan gas - 116803 - triggered by Titan Gas (116779)
class spell_titan_gas : public SpellScriptLoader
{
    public:
        spell_titan_gas() : SpellScriptLoader("spell_titan_gas") { }

        class spell_titan_gas_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_titan_gas_AuraScript);

            void Apply(constAuraEffectPtr /*aurAff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->AddAura(SPELL_TITAN_GAS_AURA, target);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_titan_gas_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_titan_gas_AuraScript();
        }
};

// Titan gas - 118327 - triggered by Titan Gas (116779)
class spell_titan_gas2 : public SpellScriptLoader
{
    public:
        spell_titan_gas2() : SpellScriptLoader("spell_titan_gas2") { }

        class spell_titan_gas2_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_titan_gas2_AuraScript);

            void Apply(constAuraEffectPtr /*aurAff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->AddAura(SPELL_TITAN_GAS_AURA2, target);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_titan_gas2_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_titan_gas2_AuraScript();
        }
};

// Energizing smash - 116550
class spell_energizing_smash : public SpellScriptLoader
{
    public:
        spell_energizing_smash() : SpellScriptLoader("spell_energizing_smash") { }
        
        class spell_ernergizing_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ernergizing_smash_SpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();

                if (Unit* caster = GetCaster())
                {
                    AuraPtr energized = caster->GetAura(SPELL_ENERGIZED);
                    float dist = 10.0f + (energized ? energized->GetStackAmount() : 0.0f);

                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, dist);

                    for (auto tar : playerList)
                        targets.push_back(tar);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ernergizing_smash_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ernergizing_smash_SpellScript::SelectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ernergizing_smash_SpellScript();
        }
};

// Energizing Smash - Visual - 116556
class spell_energizing_visual : public SpellScriptLoader
{
    public:
        spell_energizing_visual() : SpellScriptLoader("spell_energizing_visual") { }

        class spell_energizing_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_energizing_visual_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 10.0f);

                    for (auto player : playerList)
                        caster->AddAura(SPELL_ENERGIZING_VISUAL, player);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_energizing_visual_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_energizing_visual_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_energizing_visual_AuraScript();
        }
};

// Energized - 116605
class spell_energized : public SpellScriptLoader
{
    public :
        spell_energized() : SpellScriptLoader("spell_energized") { }

        class spell_energized_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_energized_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_ENERGIZED, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_energized_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };
};

// Ancient Control Console - 211584
class go_ancien_control_console : public GameObjectScript
{
    public:
        go_ancien_control_console() : GameObjectScript("go_ancien_control_console")
        {
            activated = false;
        }

        bool activated;

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (InstanceScript* pInstance = player->GetInstanceScript())
                if (!activated && pInstance->CheckRequiredBosses(DATA_WILL_OF_EMPEROR))
                {
                    // Activate central mob
                    std::list<Creature*> bunny;
                    GetCreatureListWithEntryInGrid(bunny, go, NPC_GENERAL_PURPOSE_BUNNY_JMF, 100.0f);

                    for (auto mob : bunny)
                        mob->AI()->DoAction(ACTION_COSMECTIC);

                    if (Creature* jan_xi = pInstance->instance->GetCreature(pInstance->GetData64(NPC_JAN_XI)))
                        jan_xi->AI()->DoAction(ACTION_ACTIVATE);

                    if (Creature* qin_xi = pInstance->instance->GetCreature(pInstance->GetData64(NPC_QIN_XI)))
                        qin_xi->AI()->DoAction(ACTION_ACTIVATE);

                    if (Creature* console = GetClosestCreatureWithEntry(go, NPC_ANCIENT_MOGU_MACHINE, 200.0f))
                        console->AI()->DoAction(ACTION_MOGU_ACTIVATE);

                    player->MonsterTextEmote("The machine hums to life! Get to the lower level!", 0, true); 

                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);

                    if (Creature* cho = GetClosestCreatureWithEntry(go, NPC_LOREWALKER_CHO, 20.0f, true))
                        cho->AI()->DoAction(ACTION_TALK_WILL_OF_EMPEROR);
                }
            return false;
        }
};

// Achievement - 6455
class achievement_show_me_you_moves : public AchievementCriteriaScript
{
    public:
        achievement_show_me_you_moves() : AchievementCriteriaScript("achievement_show_me_you_moves") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (source->GetInstanceScript()->GetData(ACHIEVE))
                return true;

            return false;
        }
};

void AddSC_boss_will_of_emperor()
{
    new boss_jin_qin_xi();
    new mob_woe_add_generic();
    new mob_woe_titan_spark();
    new mob_general_purpose_bunnyJMF();
    new mob_ancient_mogu_machine();
    new spell_cosmetic_lightning();
    new spell_terracota_spawn();
    new spell_devastating_arc();
    new spell_arc_visual();
    new spell_impeding_thrust();
    new spell_magnetized_jan();
    new spell_magnetized_qin();
    new spell_titan_gas();
    new spell_titan_gas2();
    new spell_energizing_smash();
    new spell_energizing_visual();
    new spell_energized();
    new go_ancien_control_console();
    new achievement_show_me_you_moves();
}
