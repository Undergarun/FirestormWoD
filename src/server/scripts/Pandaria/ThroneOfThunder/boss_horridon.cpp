////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"
#include "Vehicle.h"

enum eSpells
{
    // Horridon
    SPELL_TRIPLE_PUNCTURE           = 136767,
    SPELL_DOUBLE_SWIPE_FRONT        = 136739,
    SPELL_DOUBLE_SWIPE_BACK         = 136740,
    SPELL_DOUBLE_SWIPE              = 136741,
    SPELL_CHARGE                    = 136769,
    SPELL_RAMPAGE                   = 136821,
    // Heroic
    SPELL_DIRE_CALL                 = 137458,
    SPELL_DIRE_FIXATION             = 140946,
    SPELL_WEAK_LINK                 = 140948,

    // War-God Jalak
    SPELL_BESTIAL_CRY               = 136817,

    // Debuffs after charging door
    SPELL_CRACKED_SHELL             = 137240,
    SPELL_HEADACHE                  = 137294,

    // Control of Horridon - Orb's spells
    SPELL_FARRAKI_CONTROL           = 137433,
    SPELL_GURUBASHI_CONTROL         = 137442,
    SPELL_DRAKKARI_CONTROL          = 137443,
    SPELL_AMANI_CONTROL             = 137444,

    // Spawned mobs
    SPELL_STONE_GAZE                = 136708,
    SPELL_BLAZING_SUNLIGHT          = 136719,
    SPELL_SAND_TRAP                 = 136725,
    SPELL_SAND_TRAP_DAMAGE          = 136724,
    SPELL_DINO_FORM                 = 137237,
    SPELL_DINO_MENDING              = 136797,
    SPELL_RENDING_CHARGE            = 136653,
    SPELL_RENDING_CHARGE_DOT        = 136654,
    SPELL_VENOM_BOLT_VOLLEY         = 136587,
    SPELL_VENOMOUS_EFFUSION         = 136644,
    SPELL_LIVING_POISON             = 136645,
    SPELL_UNCONTROLLED_ABOMINATION  = 136709,
    SPELL_MORTAL_STRIKE             = 136670,
    SPELL_FROZEN_ORB                = 136564,
    SPELL_FROZEN_BOLT               = 136572,
    SPELL_FIREBALL                  = 136465,
    SPELL_SWIPE                     = 136463,
    SPELL_CHAIN_LIGHTNING           = 136480,
    SPELL_HEX_OF_CONFUSION          = 136512,
    SPELL_LIGHTNING_NOVA_TOTEM      = 136487,
    SPELL_LIGHTNING_NOVA            = 136489,
    SPELL_DEADLY_PLAGUE             = 136710
};

enum eEvents
{
    // Horridon
    EVENT_ENRAGE                = 1,
    EVENT_TRIPLE_PUNCTURE,
    EVENT_DOUBLE_SWIPE,
    EVENT_CHARGE_PLAYER,
    EVENT_NEXT_DOOR,
    EVENT_SPAWN_WAVE,
    EVENT_SPAWN_MORE_WAVE,
    EVENT_SPAWN_DINOMANCER,
    EVENT_BESTIAL_CRY,

    // Heroic mode
    EVENT_DIRE_CALL,

    // Spawned mobs
    EVENT_STONE_GAZE,
    EVENT_BLAZING_SUNLIGHT,
    EVENT_SAND_TRAP,
    EVENT_DINO_MENDING,
    EVENT_RENDING_CHARGE,
    EVENT_VENOM_BOLT_VOLLEY,
    EVENT_VENOMOUS_EFFUSION,
    EVENT_SWITCH_TARGET,
    EVENT_MORTAL_STRIKE,
    EVENT_FROZEN_ORB,
    EVENT_FIREBALL,
    EVENT_SWIPE,
    EVENT_CHAIN_LIGHTNING,
    EVENT_HEX_OF_CONFUSION,
    EVENT_LIGHTNING_NOVA_TOTEM
};

enum eSays
{
    TALK_INTRO_01,
    TALK_INTRO_02,
    TALK_INTRO_03,
    TALK_FARRAKI,
    TALK_GURUBASHI,
    TALK_DRAKKARI,
    TALK_AMANI,
    TALK_AGGRO,
    TALK_SLAY,
    TALK_DEATH
};

enum eActions
{
    ACTION_HORRIDON_OUT,
    ACTION_DINOMANCER_TRANSFORM,
    ACTION_HORRIDON_CHARGE_DOOR,
    ACTION_DISMOUNT_SHAMAN,
    ACTION_JALAK_DIED,
    ACTION_JALAK_START_INTRO,
    ACTION_SPAWN_JALAK
};

enum eDoors
{
    DOOR_NONE,
    DOOR_FARRAKI,
    DOOR_GURUBASHI,
    DOOR_DRAKKARI,
    DOOR_AMANI,
    DOOR_ZANDALARI
};

enum eMovesId
{
    MOVE_DINOMANCER_JUMP    = 2, // Prevent some bugs with movement types
    MOVE_HORRIDON_OUT,
    MOVE_CHARGE_DOOR,
    MOVE_JALAK_JUMP
};

enum eEquipsId
{
    JALAK_TWO_HANDS         = 86617,
    SKIRMISHER_DUAL_HANDS   = 9511,
    WASTEWALKER_DUAL_HANDS  = 70733,
    DINOMANCER_MAIN_HAND    = 92358,
    BLOODLORD_MAIN_HAND     = 19874,
    VENOM_PRIEST_MAIN_HAND  = 19909,
    WARRIOR_MAIN_HAND       = 37721,
    CHAMPION_MAIN_HAND      = 49839,
    WARLORLD_MAIN_HAND      = 50415,
    PROTECTOR_MAIN_HAND     = 33984,
    PROTECTOR_OFF_HAND      = 33789,
    FLAME_CASTER_MAIN_HAND  = 33983,
    SHAMAN_MAIN_HAND        = 93667
};

enum eDatas
{
    DATA_ACTUAL_DOOR,
    DATA_ACTUAL_PHASE
};

Position const horridonWaitPosition = { 5429.462f, 5693.064f, 129.605f, 1.555f };

Position const horridonChargePositions[4] =
{
    { 5492.974f, 5815.393f, 130.038f, 0.833f },
    { 5493.177f, 5692.510f, 130.038f, 5.492f },
    { 5369.846f, 5691.081f, 130.037f, 4.039f },
    { 5367.355f, 5816.851f, 130.037f, 2.357f }
};

Position const zandalariDinomancerJumpPos[4] =
{
    { 5504.55f, 5790.55f, 129.61f, 2.69f },
    { 5506.54f, 5716.90f, 129.55f, 3.62f },
    { 5354.80f, 5719.41f, 129.60f, 5.77f },
    { 5355.26f, 5790.53f, 129.60f, 0.47f }
};

Position const zandalariDinomancerPositions[4] =
{
    { 5537.03f, 5784.27f, 169.238f, 3.67703f },
    { 5536.14f, 5721.06f, 169.238f, 2.95279f },
    { 5325.51f, 5720.81f, 169.237f, 0.32948f },
    { 5326.83f, 5787.72f, 169.237f, 6.10253f }
};

Position const farrakiSkirmisherPosition[3] =
{
    { 5542.23f, 5855.67f, 130.20f, 3.91f },
    { 5538.34f, 5861.23f, 130.22f, 3.91f },
    { 5532.87f, 5865.14f, 130.20f, 3.91f }
};

Position const sullithuzStonegazerPosition[2] =
{
    { 5531.42f, 5854.76f, 130.22f, 3.91f },
    { 5535.67f, 5849.05f, 130.21f, 3.91f }
};

Position const gurubashiBloodlordPosition[3] =
{
    { 5544.20f, 5653.88f, 130.19f, 2.36f },
    { 5535.34f, 5651.03f, 130.22f, 2.36f },
    { 5531.49f, 5641.90f, 130.20f, 2.36f }
};

Position const drakkariWarriorPosition[3] =
{
    { 5325.57f, 5657.94f, 130.20f, 0.83f },
    { 5329.81f, 5649.94f, 130.22f, 0.83f },
    { 5337.26f, 5645.75f, 130.19f, 0.83f }
};

Position const drakkariChampionPosition[2] =
{
    { 5331.21f, 5640.74f, 130.19f, 0.82f },
    { 5320.41f, 5652.38f, 130.20f, 0.82f }
};

Position const amaniFlameCasterPosition[2] =
{
    { 5318.15f, 5849.92f, 130.19f, 5.44f },
    { 5335.19f, 5867.05f, 130.18f, 5.44f }
};

Position const amaniProtectorPosition = { 5327.11f, 5850.43f, 130.21f, 5.56f };

class DireCallCheck
{
    public:
        DireCallCheck(bool inCombat) : _inCombat(inCombat) { }
        bool operator()(Unit* unit) const
        {
            return unit->isInCombat() == _inCombat || unit->GetTypeId() != TYPEID_UNIT;
        }

        bool operator()(WorldObject* object) const
        {
            return object->GetTypeId() != TYPEID_UNIT || object->ToUnit()->isInCombat() == _inCombat;
        }

    private:
        bool _inCombat;
};

class DirehornValidateCheck
{
    public:
        bool operator()(Player* player) const
        {
            return player->HasAura(SPELL_DIRE_FIXATION);
        }

        bool operator()(WorldObject* object) const
        {
            return object->GetTypeId() != TYPEID_PLAYER || object->ToPlayer()->HasAura(SPELL_DIRE_FIXATION);
        }
};

class DoubleSwipeCheck
{
    public:
        DoubleSwipeCheck(Unit* caster, bool front = false) : _caster(caster), _front(front) { }
        bool operator()(Unit* unit) const
        {
            return _front ? !_caster->isInFront(unit, 1.81f) : !_caster->isInBack(unit, 1.81f);
        }

        bool operator()(WorldObject* object) const
        {
            return _front ? !_caster->isInFront(object, 1.81f) : !_caster->isInBack(object, 1.81f);
        }

    private:
        Unit* _caster;
        bool _front;
};

// Horridon - 68476
class boss_horridon : public CreatureScript
{
    public:
        boss_horridon() : CreatureScript("boss_horridon") { }

        struct boss_horridonAI : public BossAI
        {
            boss_horridonAI(Creature* creature) : BossAI(creature, DATA_HORRIDON)
            {
                pInstance       = creature->GetInstanceScript();
                introDone       = false;
                initialSpeed[0] = 1.0f;
                initialSpeed[1] = 1.14286f;
            }

            EventMap events;
            InstanceScript* pInstance;
            bool introDone;

            float initialSpeed[2];

            bool actualDoorDestroyed;
            bool jalakSpawned;
            uint8 actualDoor;
            uint32 chargeTimer;
            uint32 direhornPhase;

            void Reset()
            {
                me->ReenableEvadeMode();

                actualDoorDestroyed = false;
                jalakSpawned        = false;
                actualDoor          = DOOR_NONE;
                chargeTimer         = 0;
                direhornPhase       = 2;

                events.Reset();
                summons.DespawnAll();

                CleanUpZone();
                ResetPhases(me);

                me->RemoveAura(SPELL_ENRAGE);
                me->RemoveAura(SPELL_CRACKED_SHELL);
                me->RemoveAura(SPELL_RAMPAGE);

                _Reset();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_HORRIDON, NOT_STARTED);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TRIPLE_PUNCTURE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_VENOM_BOLT_VOLLEY);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEADLY_PLAGUE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DIRE_FIXATION);

                    if (Creature* jalak = Creature::GetCreature(*me, pInstance->GetData64(NPC_WAR_GOD_JALAK)))
                    {
                        jalak->Respawn();
                        jalak->GetMotionMaster()->MoveTargetedHome();
                    }
                }
            }

            void JustReachedHome()
            {
                if (pInstance)
                    pInstance->SetBossState(DATA_HORRIDON, FAIL);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (!pInstance)
                    return;

                if (pInstance->GetBossState(DATA_JIN_ROKH_THE_BREAKER) != DONE)
                {
                    EnterEvadeMode();
                    return;
                }

                _EnterCombat();
                pInstance->SetBossState(DATA_HORRIDON, IN_PROGRESS);
                pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_ENRAGE, 720000); // 12min
                events.ScheduleEvent(EVENT_TRIPLE_PUNCTURE, 10000);
                events.ScheduleEvent(EVENT_DOUBLE_SWIPE, 16000);
                events.ScheduleEvent(EVENT_CHARGE_PLAYER, 31000);
                events.ScheduleEvent(EVENT_NEXT_DOOR, 10000);

                // Heroic mode
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_DIRE_CALL, 60000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();
                CleanUpZone();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_HORRIDON, DONE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TRIPLE_PUNCTURE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_VENOM_BOLT_VOLLEY);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEADLY_PLAGUE);
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootDistribution(me, l_Player->GetGroup());
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (jalakSpawned)
                    return;

                if (me->HealthBelowPctDamaged(30, damage))
                {
                    jalakSpawned = true;
                    DoAction(ACTION_SPAWN_JALAK);
                }
            }

            void MovementInform(uint32 /*type*/, uint32 id)
            {
                switch (id)
                {
                    case MOVE_HORRIDON_OUT:
                        me->SetSpeed(MOVE_WALK, initialSpeed[0]);
                        me->SetSpeed(MOVE_RUN, initialSpeed[1]);
                        break;
                    case MOVE_CHARGE_DOOR:
                    {
                        me->SetSpeed(MOVE_WALK, initialSpeed[0]);
                        me->SetSpeed(MOVE_RUN, initialSpeed[1]);

                        GameObject* door = NULL;

                        switch (actualDoor)
                        {
                            case DOOR_FARRAKI:
                                door = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_FARRAKI_TRIBAL_DOOR));
                                break;
                            case DOOR_GURUBASHI:
                                door = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_GURUBASHI_TRIBAL_DOOR));
                                break;
                            case DOOR_DRAKKARI:
                                door = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_DRAKKARI_TRIBAL_DOOR));
                                break;
                            case DOOR_AMANI:
                                door = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_AMANI_TRIBAL_DOOR));
                                break;
                            default:
                                break;
                        }

                        actualDoorDestroyed = true;
                        door->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                        me->CastSpell(me, SPELL_HEADACHE, true);

                        if (actualDoor == DOOR_AMANI)
                        {
                            jalakSpawned = true;
                            DoAction(ACTION_SPAWN_JALAK);
                        }
                        else
                            events.ScheduleEvent(EVENT_NEXT_DOOR, 40000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_HORRIDON_OUT:
                    {
                        if (!pInstance)
                            break;

                        me->SetSpeed(MOVE_WALK, 3.0f);
                        me->SetSpeed(MOVE_RUN, 3.0f);
                        me->SetHomePosition(horridonWaitPosition);
                        me->GetMotionMaster()->MovePoint(MOVE_HORRIDON_OUT, horridonWaitPosition);
                        break;
                    }
                    case ACTION_HORRIDON_CHARGE_DOOR:
                        chargeTimer = 4000;
                        break;
                    case ACTION_JALAK_DIED:
                        me->CastSpell(me, SPELL_RAMPAGE, true);
                        break;
                    case ACTION_SPAWN_JALAK:
                        if (!pInstance)
                            break;

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f))
                        {
                            if (Creature* jalak = Creature::GetCreature(*me, pInstance->GetData64(NPC_WAR_GOD_JALAK)))
                            {
                                Position pos;
                                target->GetPosition(&pos);
                                jalak->SetSpeed(MOVE_RUN, 5.0f);
                                jalak->SetSpeed(MOVE_WALK, 5.0f);
                                jalak->GetMotionMaster()->MoveJump(pos.m_positionX, pos.m_positionY, pos.m_positionZ, 20.0f, 20.0f, 10.0f, MOVE_JALAK_JUMP);
                                jalak->AI()->SetGUID(target->GetGUID(), 0);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (chargeTimer)
                {
                    if (chargeTimer <= diff)
                    {
                        me->SetSpeed(MOVE_WALK, 5.0f);
                        me->SetSpeed(MOVE_RUN, 5.0f);
                        me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                        me->GetMotionMaster()->MovePoint(MOVE_CHARGE_DOOR, horridonChargePositions[actualDoor - 1]);
                        chargeTimer = 0;
                    }
                    else
                        chargeTimer -= diff;
                }

                if (IsInControl())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_ENRAGE:
                        me->CastSpell(me, SPELL_ENRAGE, true);
                        break;
                    case EVENT_TRIPLE_PUNCTURE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_TRIPLE_PUNCTURE, false);
                        events.ScheduleEvent(EVENT_TRIPLE_PUNCTURE, 11000);
                        break;
                    case EVENT_DOUBLE_SWIPE:
                        me->CastSpell(me, SPELL_DOUBLE_SWIPE, false);
                        events.ScheduleEvent(EVENT_DOUBLE_SWIPE, 17000);
                        break;
                    case EVENT_CHARGE_PLAYER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_CHARGE, false);
                        events.ScheduleEvent(EVENT_CHARGE, 50000);
                        break;
                    case EVENT_NEXT_DOOR:
                    {
                        if (!pInstance)
                            break;

                        if (actualDoor <= DOOR_DRAKKARI)
                        {
                            if (Creature* jalak = Creature::GetCreature(*me, pInstance->GetData64(NPC_WAR_GOD_JALAK)))
                                jalak->AI()->Talk(actualDoor + 3);
                        }

                        DespawnTriggerForDoor();
                        actualDoorDestroyed = false;
                        ++actualDoor;

                        if (actualDoor < DOOR_ZANDALARI)
                            events.ScheduleEvent(EVENT_SPAWN_DINOMANCER, 60000);
                        else
                            break;

                        events.ScheduleEvent(EVENT_SPAWN_WAVE, 1000);
                        break;
                    }
                    case EVENT_SPAWN_WAVE:
                    {
                        if (actualDoorDestroyed || !pInstance)
                            break;

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f))
                        {
                            switch (actualDoor)
                            {
                                case DOOR_FARRAKI:
                                {
                                    if (Creature* stonegazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, sullithuzStonegazerPosition[urand(0, 1)]))
                                        stonegazer->AI()->AttackStart(target);
                                    if (Creature* skirmisher = me->SummonCreature(NPC_FARRAKI_SKIRMICHER, farrakiSkirmisherPosition[urand(0, 2)]))
                                        skirmisher->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* stonegazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, sullithuzStonegazerPosition[urand(0, 1)]))
                                            stonegazer->AI()->AttackStart(target);
                                    }

                                    if (GameObject* farrakiDoor = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_FARRAKI_TRIBAL_DOOR)))
                                        farrakiDoor->Use(me);

                                    events.ScheduleEvent(EVENT_SPAWN_MORE_WAVE, 5000);
                                    break;
                                }
                                case DOOR_GURUBASHI:
                                {
                                    if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, gurubashiBloodlordPosition[urand(0, 2)]))
                                        bloodlord->AI()->AttackStart(target);
                                    if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, gurubashiBloodlordPosition[urand(0, 2)]))
                                        bloodlord->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, gurubashiBloodlordPosition[urand(0, 2)]))
                                            bloodlord->AI()->AttackStart(target);
                                    }

                                    if (GameObject* gurubashiDoor = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_GURUBASHI_TRIBAL_DOOR)))
                                        gurubashiDoor->Use(me);

                                    events.ScheduleEvent(EVENT_SPAWN_MORE_WAVE, 5000);
                                    break;
                                }
                                case DOOR_DRAKKARI:
                                {
                                    if (Creature* warrior = me->SummonCreature(NPC_RISEN_DRAKKARI_WARRIOR, drakkariWarriorPosition[urand(0, 2)]))
                                        warrior->AI()->AttackStart(target);
                                    if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, drakkariChampionPosition[urand(0, 1)]))
                                        champion->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, drakkariChampionPosition[urand(0, 1)]))
                                            champion->AI()->AttackStart(target);
                                    }

                                    if (GameObject* drakkariDoor = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_DRAKKARI_TRIBAL_DOOR)))
                                        drakkariDoor->Use(me);

                                    events.ScheduleEvent(EVENT_SPAWN_MORE_WAVE, 5000);
                                    break;
                                }
                                case DOOR_AMANI:
                                {
                                    if (Creature* flameCaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, amaniFlameCasterPosition[urand(0, 1)]))
                                        flameCaster->AI()->AttackStart(target);
                                    if (Creature* protector = me->SummonCreature(NPC_AMANI_SHI_PROTECTOR, amaniProtectorPosition))
                                        protector->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* flameCaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, amaniFlameCasterPosition[urand(0, 1)]))
                                            flameCaster->AI()->AttackStart(target);
                                    }

                                    if (GameObject* amaniDoor = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_AMANI_TRIBAL_DOOR)))
                                        amaniDoor->Use(me);

                                    events.ScheduleEvent(EVENT_SPAWN_MORE_WAVE, 5000);
                                    break;
                                }
                                default:
                                    break;
                            }
                        }

                        events.ScheduleEvent(EVENT_SPAWN_WAVE, 20000);
                        break;
                    }
                    case EVENT_SPAWN_MORE_WAVE:
                    {
                        if (actualDoorDestroyed || !pInstance)
                            break;

                        Position pos;
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f))
                        {
                            target->GetPosition(&pos);

                            switch (actualDoor)
                            {
                                case DOOR_FARRAKI:
                                    if (Creature* wastewalker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, pos))
                                        wastewalker->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* wastewalker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, pos))
                                            wastewalker->AI()->AttackStart(target);
                                    }

                                    break;
                                case DOOR_GURUBASHI:
                                    if (Creature* venomPriest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, pos))
                                        venomPriest->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* venomPriest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, pos))
                                            venomPriest->AI()->AttackStart(target);
                                    }

                                    break;
                                case DOOR_DRAKKARI:
                                    if (Creature* frozenWarlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, pos))
                                        frozenWarlord->AI()->AttackStart(target);

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* frozenWarlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, pos))
                                            frozenWarlord->AI()->AttackStart(target);
                                    }

                                    break;
                                case DOOR_AMANI:
                                {
                                    if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, pos))
                                    {
                                        warbear->AI()->AttackStart(target);

                                        if (Creature* shaman = me->SummonCreature(NPC_AMANI_SHI_BEAST_SHAMAN, pos))
                                        {
                                            warbear->AI()->SetGUID(shaman->GetGUID(), 0);
                                            shaman->AI()->AttackStart(target);
                                        }
                                    }

                                    if (Is25ManRaid())
                                    {
                                        if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, pos))
                                        {
                                            warbear->AI()->AttackStart(target);

                                            if (Creature* shaman = me->SummonCreature(NPC_AMANI_SHI_BEAST_SHAMAN, pos))
                                            {
                                                warbear->AI()->SetGUID(shaman->GetGUID(), 0);
                                                shaman->AI()->AttackStart(target);
                                            }
                                        }
                                    }

                                    break;
                                }
                                default:
                                    break;
                            }
                        }

                        break;
                    }
                    case EVENT_SPAWN_DINOMANCER:
                    {
                        if (actualDoor == DOOR_NONE || actualDoor == DOOR_ZANDALARI)
                            break;

                        Creature* dinomancer = NULL;
                        std::list<Creature*> dinoList;
                        me->GetCreatureListWithEntryInGrid(dinoList, NPC_ZANDALARI_DINOMANCER, 200.0f);

                        for (auto itr : dinoList)
                        {
                            if (itr->GetPositionX() >= zandalariDinomancerPositions[actualDoor - 1].GetPositionX() - 1.0f &&
                                itr->GetPositionX() <= zandalariDinomancerPositions[actualDoor - 1].GetPositionX() + 1.0f &&
                                itr->GetPositionY() >= zandalariDinomancerPositions[actualDoor - 1].GetPositionY() - 1.0f &&
                                itr->GetPositionY() <= zandalariDinomancerPositions[actualDoor - 1].GetPositionY() + 1.0f)
                            {
                                dinomancer = itr;
                                break;
                            }
                        }

                        if (dinomancer)
                        {
                            dinomancer->GetMotionMaster()->MoveJump(zandalariDinomancerJumpPos[actualDoor - 1].GetPositionX(),
                                                                    zandalariDinomancerJumpPos[actualDoor - 1].GetPositionY(),
                                                                    zandalariDinomancerJumpPos[actualDoor - 1].GetPositionZ(),
                                                                    20.0f, 20.0f, 10.0f, MOVE_DINOMANCER_JUMP);
                            dinomancer->AI()->SetData(0, 1);
                        }

                        break;
                    }
                    case EVENT_DIRE_CALL:
                    {
                        if (!IsHeroic())
                            break;

                        me->CastSpell(me, SPELL_DIRE_CALL, false);
                        events.ScheduleEvent(EVENT_DIRE_CALL, 60000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            uint32 GetData(uint32 id)
            {
                switch (id)
                {
                    case DATA_ACTUAL_DOOR:
                        return actualDoor;
                    case DATA_ACTUAL_PHASE:
                        return direhornPhase;
                    default:
                        return 0;
                }
            }

            void SetData(uint32 type, uint32 value)
            {
                switch (type)
                {
                    case DATA_ACTUAL_DOOR:
                        actualDoor = value;
                        break;
                    case DATA_ACTUAL_PHASE:
                        direhornPhase = value;
                        break;
                    default:
                        break;
                }
            }

            bool IsInControl()
            {
                return me->HasAura(SPELL_FARRAKI_CONTROL) || me->HasAura(SPELL_GURUBASHI_CONTROL) ||
                       me->HasAura(SPELL_DRAKKARI_CONTROL) || me->HasAura(SPELL_AMANI_CONTROL) ||
                       me->HasAura(SPELL_HEADACHE);
            }

            void CleanUpZone()
            {
                std::list<Creature*> creatureList;
                std::list<GameObject*> gameobjectList;

                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_FARRAKI_ORB_OF_CONTROL, 300.0f);
                for (auto itr : gameobjectList)
                    itr->RemoveFromWorld();

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_GURUBASHI_ORB_OF_CONTROL, 300.0f);
                for (auto itr : gameobjectList)
                    itr->RemoveFromWorld();

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_DRAKKARI_ORB_OF_CONTROL, 300.0f);
                for (auto itr : gameobjectList)
                    itr->RemoveFromWorld();

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_AMANI_ORB_OF_CONTROL, 300.0f);
                for (auto itr : gameobjectList)
                    itr->RemoveFromWorld();

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_FARRAKI_TRIBAL_DOOR, 300.0f);
                for (auto itr : gameobjectList)
                    itr->SetGoState(GO_STATE_READY);

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_GURUBASHI_TRIBAL_DOOR, 300.0f);
                for (auto itr : gameobjectList)
                    itr->SetGoState(GO_STATE_READY);

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_DRAKKARI_TRIBAL_DOOR, 300.0f);
                for (auto itr : gameobjectList)
                    itr->SetGoState(GO_STATE_READY);

                gameobjectList.clear();
                me->GetGameObjectListWithEntryInGrid(gameobjectList, GOB_AMANI_TRIBAL_DOOR, 300.0f);
                for (auto itr : gameobjectList)
                    itr->SetGoState(GO_STATE_READY);

                me->GetCreatureListWithEntryInGrid(creatureList, NPC_ZANDALARI_DINOMANCER, 300.0f);
                for (auto itr : creatureList)
                {
                    itr->Respawn();
                    itr->GetMotionMaster()->MoveTargetedHome();
                }

                creatureList.clear();
                me->GetCreatureListWithEntryInGrid(creatureList, NPC_SAND_TRAP, 300.0f);
                for (auto itr : creatureList)
                    itr->DespawnOrUnsummon();

                creatureList.clear();
                me->GetCreatureListWithEntryInGrid(creatureList, NPC_VENOMOUS_EFFUSION, 300.0f);
                for (auto itr : creatureList)
                    itr->DespawnOrUnsummon();

                creatureList.clear();
                me->GetCreatureListWithEntryInGrid(creatureList, NPC_FROZEN_ORB, 300.0f);
                for (auto itr : creatureList)
                    itr->DespawnOrUnsummon();

                creatureList.clear();
                me->GetCreatureListWithEntryInGrid(creatureList, NPC_LIGHTNING_NOVA_TOTEM, 300.0f);
                for (auto itr : creatureList)
                    itr->DespawnOrUnsummon();

                creatureList.clear();
                me->GetCreatureListWithEntryInGrid(creatureList, NPC_DIREHORN_SPIRIT, 300.0f);
                for (auto itr : creatureList)
                    itr->DespawnOrUnsummon();
            }

            void ResetPhases(Creature* me)
            {
                Map::PlayerList const& plrList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator player = plrList.begin(); player != plrList.end(); ++player)
                {
                    if (Player* plr = player->getSource())
                        plr->SetPhaseMask(1, true);
                }
            }

            void DespawnTriggerForDoor()
            {
                std::list<Creature*> creatureList;

                switch (actualDoor)
                {
                    case DOOR_FARRAKI:
                        me->GetCreatureListWithEntryInGrid(creatureList, NPC_SAND_TRAP, 300.0f);
                        for (auto itr : creatureList)
                            itr->DespawnOrUnsummon();
                        break;
                    case DOOR_GURUBASHI:
                        me->GetCreatureListWithEntryInGrid(creatureList, NPC_VENOMOUS_EFFUSION, 300.0f);
                        for (auto itr : creatureList)
                            itr->DespawnOrUnsummon();
                        break;
                    case DOOR_DRAKKARI:
                        me->GetCreatureListWithEntryInGrid(creatureList, NPC_FROZEN_ORB, 300.0f);
                        for (auto itr : creatureList)
                            itr->DespawnOrUnsummon();
                        break;
                    case DOOR_AMANI:
                        me->GetCreatureListWithEntryInGrid(creatureList, NPC_LIGHTNING_NOVA_TOTEM, 300.0f);
                        for (auto itr : creatureList)
                            itr->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_horridonAI(creature);
        }
};

// War-God Jalak - 69374
class boss_war_god_jalak : public CreatureScript
{
    public:
        boss_war_god_jalak() : CreatureScript("boss_war_god_jalak") { }

        struct boss_war_god_jalakAI : public BossAI
        {
            boss_war_god_jalakAI(Creature* creature) : BossAI(creature, DATA_HORRIDON)
            {
                pInstance = creature->GetInstanceScript();
                introDone = false;
                initialSpeed[0] = 1.0f;
                initialSpeed[1] = 1.14286f;
            }

            EventMap events;
            InstanceScript* pInstance;
            bool introDone;

            float initialSpeed[2];

            uint64 targetGuid;
            uint32 secondTalkTimer;
            uint32 thirdTalkTimer;
            uint32 openGateTimer;
            uint32 horridonOutTimer;

            void Reset()
            {
                SetEquipmentSlots(false, JALAK_TWO_HANDS, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->ReenableEvadeMode();

                events.Reset();

                targetGuid          = 0;
                secondTalkTimer     = 0;
                thirdTalkTimer      = 0;
                openGateTimer       = 0;
                horridonOutTimer    = 0;

                _Reset();
            }

            void JustReachedHome()
            {
            }

            void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                Talk(TALK_AGGRO);
                events.ScheduleEvent(EVENT_BESTIAL_CRY, 5000);
            }

            void MovementInform(uint32 /*type*/, uint32 id)
            {
                if (id == MOVE_JALAK_JUMP)
                {
                    if (Unit* target = Unit::GetUnit(*me, targetGuid))
                        AttackStart(target);

                    me->SetSpeed(MOVE_WALK, initialSpeed[0]);
                    me->SetSpeed(MOVE_RUN, initialSpeed[1]);
                }
            }

            void SetGUID(uint64 guid, int32 /*type*/)
            {
                targetGuid = guid;
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                Talk(TALK_DEATH);

                if (pInstance)
                {
                    if (Creature* horridon = Creature::GetCreature(*me, pInstance->GetData64(NPC_HORRIDON)))
                        horridon->AI()->DoAction(ACTION_JALAK_DIED);
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_JALAK_START_INTRO:
                        if (introDone)
                            break;

                        introDone = true;
                        Talk(TALK_INTRO_01);
                        secondTalkTimer = 6000;
                        thirdTalkTimer  = 25000;
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (secondTalkTimer)
                {
                    if (secondTalkTimer <= diff)
                    {
                        Talk(TALK_INTRO_02);
                        secondTalkTimer = 0;
                    }
                    else
                        secondTalkTimer -= diff;
                }

                if (thirdTalkTimer)
                {
                    if (thirdTalkTimer <= diff)
                    {
                        Talk(TALK_INTRO_03);
                        thirdTalkTimer  = 0;
                        openGateTimer   = 7000;
                    }
                    else
                        thirdTalkTimer -= diff;
                }

                if (openGateTimer)
                {
                    if (openGateTimer <= diff)
                    {
                        if (!pInstance)
                            return;

                        if (GameObject* horridonGate = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_HORRIDON_GATE)))
                            horridonGate->SetGoState(GO_STATE_ACTIVE);

                        openGateTimer       = 0;
                        horridonOutTimer    = 3000;
                    }
                    else
                        openGateTimer -= diff;
                }

                if (horridonOutTimer)
                {
                    if (horridonOutTimer <= diff)
                    {
                        if (!pInstance)
                            return;

                        if (Creature* horridon = Creature::GetCreature(*me, pInstance->GetData64(NPC_HORRIDON)))
                            horridon->AI()->DoAction(ACTION_HORRIDON_OUT);

                        horridonOutTimer = 0;
                    }
                    else
                        horridonOutTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BESTIAL_CRY:
                        me->CastSpell(me, SPELL_BESTIAL_CRY, true);
                        events.ScheduleEvent(EVENT_BESTIAL_CRY, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_war_god_jalakAI(creature);
        }
};

// Sul'lithuz Stonegazer - 69172
class mob_sul_lithuz_stonegazer : public CreatureScript
{
    public:
        mob_sul_lithuz_stonegazer() : CreatureScript("mob_sul_lithuz_stonegazer") { }

        struct mob_sul_lithuz_stonegazerAI : public ScriptedAI
        {
            mob_sul_lithuz_stonegazerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_STONE_GAZE, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_STONE_GAZE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_STONE_GAZE, false);
                        events.ScheduleEvent(EVENT_STONE_GAZE, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sul_lithuz_stonegazerAI(creature);
        }
};

// Farraki Skirmisher - 69173
class mob_farraki_skirmisher : public CreatureScript
{
    public:
        mob_farraki_skirmisher() : CreatureScript("mob_farraki_skirmisher") { }

        struct mob_farraki_skirmisherAI : public ScriptedAI
        {
            mob_farraki_skirmisherAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                SetEquipmentSlots(false, SKIRMISHER_DUAL_HANDS, SKIRMISHER_DUAL_HANDS, EQUIP_NO_CHANGE);
                me->ReenableEvadeMode();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_farraki_skirmisherAI(creature);
        }
};

// Farraki Wastewalker - 69175
class mob_farraki_wastewalker : public CreatureScript
{
    public:
        mob_farraki_wastewalker() : CreatureScript("mob_farraki_wastewalker") { }

        struct mob_farraki_wastewalkerAI : public ScriptedAI
        {
            mob_farraki_wastewalkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, WASTEWALKER_DUAL_HANDS, WASTEWALKER_DUAL_HANDS, EQUIP_NO_CHANGE);
                me->ReenableEvadeMode();

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_BLAZING_SUNLIGHT, 5000);
                events.ScheduleEvent(EVENT_SAND_TRAP, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BLAZING_SUNLIGHT:
                        me->CastSpell(me, SPELL_BLAZING_SUNLIGHT, false);
                        events.ScheduleEvent(EVENT_BLAZING_SUNLIGHT, 15000);
                        break;
                    case EVENT_SAND_TRAP:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_SAND_TRAP, false);
                        events.ScheduleEvent(EVENT_SAND_TRAP, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_farraki_wastewalkerAI(creature);
        }
};

// Sand Trap - 69346
class mob_sand_trap : public CreatureScript
{
    public:
        mob_sand_trap() : CreatureScript("mob_sand_trap") { }

        struct mob_sand_trapAI : public ScriptedAI
        {
            mob_sand_trapAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->SetReactState(REACT_PASSIVE);

                me->CastSpell(me, SPELL_SAND_TRAP_DAMAGE, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sand_trapAI(creature);
        }
};

// Zandalari Dinomancer - 69221
class mob_zandalari_dinomancer : public CreatureScript
{
    public:
        mob_zandalari_dinomancer() : CreatureScript("mob_zandalari_dinomancer") { }

        struct mob_zandalari_dinomancerAI : public ScriptedAI
        {
            mob_zandalari_dinomancerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            bool transformed;
            bool canEnterCombat;

            void Reset()
            {
                SetEquipmentSlots(false, DINOMANCER_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                me->RemoveAura(SPELL_DINO_FORM);
                me->ReenableEvadeMode();

                transformed     = false;
                canEnterCombat  = false;

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_DINO_MENDING, 10000);
            }

            void MovementInform(uint32 /*type*/, uint32 id)
            {
                if (id == MOVE_DINOMANCER_JUMP)
                {
                    if (Player* target = me->SelectNearestPlayerNotGM(50.0f))
                        AttackStart(target);
                }
            }

            void SetData(uint32 /*id*/, uint32 value)
            {
                canEnterCombat = value != 0;
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (transformed)
                    return;

                if (me->HealthBelowPctDamaged(50, damage))
                    DoAction(ACTION_DINOMANCER_TRANSFORM);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_DINOMANCER_TRANSFORM:
                    {
                        if (!pInstance)
                            break;

                        me->CastSpell(me, SPELL_DINO_FORM, true);
                        transformed = true;

                        if (Creature* horridon = Creature::GetCreature(*me, pInstance->GetData64(NPC_HORRIDON)))
                        {
                            uint8 actualDoor = horridon->AI()->GetData(DATA_ACTUAL_DOOR);
                            Position pos;
                            me->GetPosition(&pos);
                            GameObject* orb = NULL;
                            Unit* target = me->getVictim() ? me->getVictim() : SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f);
                            if (!target)
                                break;

                            switch (actualDoor)
                            {
                                case DOOR_FARRAKI:
                                    orb = target->SummonGameObject(GOB_FARRAKI_ORB_OF_CONTROL, pos.GetPositionX(), pos.GetPositionY(), 131.60f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                                    break;
                                case DOOR_GURUBASHI:
                                    orb = target->SummonGameObject(GOB_GURUBASHI_ORB_OF_CONTROL, pos.GetPositionX(), pos.GetPositionY(), 131.60f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                                    break;
                                case DOOR_DRAKKARI:
                                    orb = target->SummonGameObject(GOB_DRAKKARI_ORB_OF_CONTROL, pos.GetPositionX(), pos.GetPositionY(), 131.60f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                                    break;
                                case DOOR_AMANI:
                                    orb = target->SummonGameObject(GOB_AMANI_ORB_OF_CONTROL, pos.GetPositionX(), pos.GetPositionY(), 131.60f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                                    break;
                                default:
                                    break;
                            }

                            // Make the orb clickable !
                            if (orb)
                                orb->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, GO_DYNFLAG_LO_ACTIVATE);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (!canEnterCombat)
                {
                    EnterEvadeMode();
                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_DINO_MENDING:
                    {
                        if (!pInstance)
                            break;

                        if (Creature* horridon = Creature::GetCreature(*me, pInstance->GetData64(NPC_HORRIDON)))
                            me->CastSpell(horridon, SPELL_DINO_MENDING, false);

                        events.ScheduleEvent(EVENT_DINO_MENDING, 20000);
                        return;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_dinomancerAI(creature);
        }
};

// Gurubashi Bloodlord - 69167
class mob_gurubashi_bloodlord : public CreatureScript
{
    public:
        mob_gurubashi_bloodlord() : CreatureScript("mob_gurubashi_bloodlord") { }

        struct mob_gurubashi_bloodlordAI : public ScriptedAI
        {
            mob_gurubashi_bloodlordAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, BLOODLORD_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_RENDING_CHARGE, 2000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_RENDING_CHARGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(target, SPELL_RENDING_CHARGE, true);
                            me->CastSpell(target, SPELL_RENDING_CHARGE_DOT, true);
                        }
                        events.ScheduleEvent(EVENT_RENDING_CHARGE, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gurubashi_bloodlordAI(creature);
        }
};

// Gurubashi Venom Priest - 69164
class mob_gurubashi_venom_priest : public CreatureScript
{
    public:
        mob_gurubashi_venom_priest() : CreatureScript("mob_gurubashi_venom_priest") { }

        struct mob_gurubashi_venom_priestAI : public ScriptedAI
        {
            mob_gurubashi_venom_priestAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, VENOM_PRIEST_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, 5000);
                events.ScheduleEvent(EVENT_VENOMOUS_EFFUSION, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_VENOM_BOLT_VOLLEY:
                        me->CastSpell(me, SPELL_VENOM_BOLT_VOLLEY, false);
                        events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, 15000);
                        break;
                    case EVENT_VENOMOUS_EFFUSION:
                        me->CastSpell(me, SPELL_VENOMOUS_EFFUSION, false);
                        events.ScheduleEvent(EVENT_VENOMOUS_EFFUSION, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gurubashi_venom_priestAI(creature);
        }
};

// Venomous Effusion - 69314
class mob_venomous_effusion : public CreatureScript
{
    public:
        mob_venomous_effusion() : CreatureScript("mob_venomous_effusion") { }

        struct mob_venomous_effusionAI : public ScriptedAI
        {
            mob_venomous_effusionAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->CastSpell(me, SPELL_LIVING_POISON, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                events.Reset();

                events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_VENOM_BOLT_VOLLEY:
                        me->CastSpell(me, SPELL_VENOM_BOLT_VOLLEY, true);
                        events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_venomous_effusionAI(creature);
        }
};

// Risen Drakkari Warrior - 69184
class mob_risen_drakkari_warrior : public CreatureScript
{
    public:
        mob_risen_drakkari_warrior() : CreatureScript("mob_risen_drakkari_warrior") { }

        struct mob_risen_drakkari_warriorAI : public ScriptedAI
        {
            mob_risen_drakkari_warriorAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->CastSpell(me, SPELL_UNCONTROLLED_ABOMINATION, true);

                SetEquipmentSlots(false, WARRIOR_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_SWITCH_TARGET, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SWITCH_TARGET:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);
                        events.ScheduleEvent(EVENT_SWITCH_TARGET, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_risen_drakkari_warriorAI(creature);
        }
};

// Risen Drakkari Champion - 69185
class mob_risen_drakkari_champion : public CreatureScript
{
    public:
        mob_risen_drakkari_champion() : CreatureScript("mob_risen_drakkari_champion") { }

        struct mob_risen_drakkari_championAI : public ScriptedAI
        {
            mob_risen_drakkari_championAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->CastSpell(me, SPELL_UNCONTROLLED_ABOMINATION, true);

                SetEquipmentSlots(false, CHAMPION_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_SWITCH_TARGET, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SWITCH_TARGET:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);
                        events.ScheduleEvent(EVENT_SWITCH_TARGET, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_risen_drakkari_championAI(creature);
        }
};

// Drakkari Frozen Warlord - 69178
class mob_drakkari_frozen_warlord : public CreatureScript
{
    public:
        mob_drakkari_frozen_warlord() : CreatureScript("mob_drakkari_frozen_warlord") { }

        struct mob_drakkari_frozen_warlordAI : public ScriptedAI
        {
            mob_drakkari_frozen_warlordAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, WARLORLD_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_MORTAL_STRIKE, 5000);
                events.ScheduleEvent(EVENT_FROZEN_ORB, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_MORTAL_STRIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_MORTAL_STRIKE, true);
                        events.ScheduleEvent(EVENT_MORTAL_STRIKE, 10000);
                        break;
                    case EVENT_FROZEN_ORB:
                        me->CastSpell(me, SPELL_FROZEN_ORB, false);
                        events.ScheduleEvent(EVENT_FROZEN_ORB, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_drakkari_frozen_warlordAI(creature);
        }
};

// Frozen Orb - 69268
class mob_frozen_orb : public CreatureScript
{
    public:
        mob_frozen_orb() : CreatureScript("mob_frozen_orb") { }

        struct mob_frozen_orbAI : public ScriptedAI
        {
            mob_frozen_orbAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_FROZEN_BOLT, true);
                me->CastSpell(me, SPELL_SELF_ROOT, true);

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_frozen_orbAI(creature);
        }
};

// Amani'Shi Protector - 69169
class mob_amani_shi_protector : public CreatureScript
{
    public:
        mob_amani_shi_protector() : CreatureScript("mob_amani_shi_protector") { }

        struct mob_amani_shi_protectorAI : public ScriptedAI
        {
            mob_amani_shi_protectorAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                SetEquipmentSlots(false, PROTECTOR_MAIN_HAND, PROTECTOR_OFF_HAND, EQUIP_NO_CHANGE);
                me->ReenableEvadeMode();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_amani_shi_protectorAI(creature);
        }
};

// Amani'Shi Flame Caster - 69168
class mob_amani_shi_flame_caster : public CreatureScript
{
    public:
        mob_amani_shi_flame_caster() : CreatureScript("mob_amani_shi_flame_caster") { }

        struct mob_amani_shi_flame_casterAI : public ScriptedAI
        {
            mob_amani_shi_flame_casterAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, FLAME_CASTER_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->ReenableEvadeMode();

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_FIREBALL, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FIREBALL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_FIREBALL, false);
                        events.ScheduleEvent(EVENT_FIREBALL, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_amani_shi_flame_casterAI(creature);
        }
};

// Amani Warbear - 69177
class mob_amani_warbear : public CreatureScript
{
    public:
        mob_amani_warbear() : CreatureScript("mob_amani_warbear") { }

        struct mob_amani_warbearAI : public ScriptedAI
        {
            mob_amani_warbearAI(Creature* creature) : ScriptedAI(creature), vehicle(creature->GetVehicleKit())
            {
                shamanGuid = 0;
            }

            EventMap events;
            Vehicle* vehicle;
            uint64 shamanGuid;

            void Reset()
            {
                me->ReenableEvadeMode();

                events.Reset();
            }

            void SetGUID(uint64 guid, int32 /*type*/)
            {
                shamanGuid = guid;

                if (Creature* shaman = Creature::GetCreature(*me, shamanGuid))
                    shaman->EnterVehicle(me, 0);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_SWIPE, 5000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (Creature* shaman = Creature::GetCreature(*me, shamanGuid))
                    shaman->AI()->DoAction(ACTION_DISMOUNT_SHAMAN);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SWIPE:
                        me->CastSpell(me, SPELL_SWIPE, true);
                        events.ScheduleEvent(EVENT_SWIPE, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_amani_warbearAI(creature);
        }
};

// Amani'shi Beast Shaman - 69176
class mob_amani_shi_beast_shaman : public CreatureScript
{
    public:
        mob_amani_shi_beast_shaman() : CreatureScript("mob_amani_shi_beast_shaman") { }

        struct mob_amani_shi_beast_shamanAI : public ScriptedAI
        {
            mob_amani_shi_beast_shamanAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, SHAMAN_MAIN_HAND, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->ReenableEvadeMode();

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 5000);
                events.ScheduleEvent(EVENT_HEX_OF_CONFUSION, 8000);
                events.ScheduleEvent(EVENT_LIGHTNING_NOVA_TOTEM, 10000);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_DISMOUNT_SHAMAN:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHAIN_LIGHTNING:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_CHAIN_LIGHTNING, false);
                        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 10000);
                        break;
                    case EVENT_HEX_OF_CONFUSION:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_HEX_OF_CONFUSION, false);
                        events.ScheduleEvent(EVENT_HEX_OF_CONFUSION, 15000);
                        break;
                    case EVENT_LIGHTNING_NOVA_TOTEM:
                        me->CastSpell(me, SPELL_LIGHTNING_NOVA_TOTEM, true);
                        events.ScheduleEvent(EVENT_LIGHTNING_NOVA_TOTEM, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_amani_shi_beast_shamanAI(creature);
        }
};

// Lightning Nova Totem - 69215
class mob_lightning_nova_totem : public CreatureScript
{
    public:
        mob_lightning_nova_totem() : CreatureScript("mob_lightning_nova_totem") { }

        struct mob_lightning_nova_totemAI : public ScriptedAI
        {
            mob_lightning_nova_totemAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_LIGHTNING_NOVA, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lightning_nova_totemAI(creature);
        }
};

// Direhorn Spirit - 70688
class mob_direhorn_spirit : public CreatureScript
{
    public:
        mob_direhorn_spirit() : CreatureScript("mob_direhorn_spirit") { }

        struct mob_direhorn_spiritAI : public ScriptedAI
        {
            mob_direhorn_spiritAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            uint64 playerGuid;
            InstanceScript* pInstance;

            void Reset()
            {
                me->SetSpeed(MOVE_WALK, 0.5f);
                me->SetSpeed(MOVE_RUN, 0.5f);

                playerGuid = 0;

                std::list<Player*> plrList;
                me->GetPlayerListInGrid(plrList, 300.0f);
                plrList.remove_if(DirehornValidateCheck());

                if (plrList.empty())
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                JadeCore::RandomResizeList(plrList, 1);

                if (Unit* target = plrList.front())
                {
                    playerGuid  = target->GetGUID();
                    me->CastSpell(target, SPELL_DIRE_FIXATION, true);
                    me->CastSpell(me, SPELL_WEAK_LINK, true);
                    me->getThreatManager().addThreat(target, 100000000.0f);
                    me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                    AttackStart(target);

                    if (pInstance)
                    {
                        if (Creature* horridon = Creature::GetCreature(*me, pInstance->GetData64(NPC_HORRIDON)))
                        {
                            uint32 phase = horridon->AI()->GetData(DATA_ACTUAL_PHASE);
                            me->SetPhaseMask(phase, true);
                            target->SetPhaseMask(phase | target->GetPhaseMask(), true);
                            horridon->AI()->SetData(DATA_ACTUAL_PHASE, (phase << 1));
                        }
                    }
                }
                else
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_direhorn_spiritAI(creature);
        }
};

// Control of Horridon - 137433 / 137442 / 137443 / 137444
class spell_control_of_horridon: public SpellScriptLoader
{
    public:
        spell_control_of_horridon() : SpellScriptLoader("spell_control_of_horridon") { }

        class spell_control_of_horridon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_control_of_horridon_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget() || !GetTarget()->ToCreature())
                    return;

                Creature* horridon = GetTarget()->ToCreature();
                horridon->AI()->DoAction(ACTION_HORRIDON_CHARGE_DOOR);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_control_of_horridon_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_control_of_horridon_AuraScript();
        }
};

// Headache - 137294
class spell_headache: public SpellScriptLoader
{
    public:
        spell_headache() : SpellScriptLoader("spell_headache") { }

        class spell_headache_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_headache_AuraScript);

            void AfterRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->GetMotionMaster()->MoveChase(GetTarget()->getVictim());
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_headache_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_headache_AuraScript();
        }
};

// Dire Call - 137458
class spell_dire_call: public SpellScriptLoader
{
    public:
        spell_dire_call() : SpellScriptLoader("spell_dire_call") { }

        class spell_dire_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dire_call_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                // Targets must be activated
                targets.remove_if(DireCallCheck(false));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dire_call_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dire_call_SpellScript();
        }
};

// Double Swipe - 136739 / 136740
class spell_double_swipe: public SpellScriptLoader
{
    public:
        spell_double_swipe() : SpellScriptLoader("spell_double_swipe") { }

        class spell_double_swipe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_double_swipe_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                // Targets must be activated
                if (GetSpellInfo()->Id == SPELL_DOUBLE_SWIPE_FRONT)
                    targets.remove_if(DoubleSwipeCheck(GetCaster(), true));
                else if (GetSpellInfo()->Id == SPELL_DOUBLE_SWIPE_BACK)
                    targets.remove_if(DoubleSwipeCheck(GetCaster()));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_double_swipe_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_double_swipe_SpellScript();
        }
};

// Wrought Iron Door - 218667
class go_horridon_intro_door : public GameObjectScript
{
    public:
        go_horridon_intro_door() : GameObjectScript("go_horridon_intro_door") { }

        bool OnGossipHello(Player* /*player*/, GameObject* go)
        {
            InstanceScript* pInstance = go->GetInstanceScript();
            if (!pInstance)
                return false;

            if (pInstance->GetBossState(DATA_JIN_ROKH_THE_BREAKER) != DONE)
                return false;

            if (Creature* jalak = Creature::GetCreature(*go, pInstance->GetData64(NPC_WAR_GOD_JALAK)))
                jalak->AI()->DoAction(ACTION_JALAK_START_INTRO);

            go->SetGoState(GO_STATE_ACTIVE);
            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_horridon()
{
    new boss_horridon();
    new boss_war_god_jalak();
    new mob_sul_lithuz_stonegazer();
    new mob_farraki_skirmisher();
    new mob_farraki_wastewalker();
    new mob_sand_trap();
    new mob_zandalari_dinomancer();
    new mob_gurubashi_bloodlord();
    new mob_gurubashi_venom_priest();
    new mob_venomous_effusion();
    new mob_risen_drakkari_warrior();
    new mob_risen_drakkari_champion();
    new mob_drakkari_frozen_warlord();
    new mob_frozen_orb();
    new mob_amani_shi_protector();
    new mob_amani_shi_flame_caster();
    new mob_amani_warbear();
    new mob_amani_shi_beast_shaman();
    new mob_lightning_nova_totem();
    new mob_direhorn_spirit();
    new spell_control_of_horridon();
    new spell_headache();
    new spell_dire_call();
    new spell_double_swipe();
    new go_horridon_intro_door();
}
#endif
