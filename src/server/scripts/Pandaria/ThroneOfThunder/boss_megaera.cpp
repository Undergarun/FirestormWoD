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
#include "WorldPacket.h"

enum eSpells
{
    // Heads summons
    SPELL_SUMMON_FLAMING_HEAD   = 134176,
    SPELL_SUMMON_FROZEN_HEAD    = 134177,
    SPELL_SUMMON_VENOMOUS_HEAD  = 134178,
    SPELL_SUMMON_ARCANE_HEAD    = 134179,

    // Shared
    SPELL_MEGAERAS_RAGE_FIRE    = 139758,
    SPELL_MEGAERAS_RAGE_FROST   = 139816,
    SPELL_MEGAERAS_RAGE_VENOM   = 139818,
    SPELL_MEGAERAS_RAGE_ARCANE  = 139820,

    SPELL_SUBMERGED             = 139832,
    SPELL_HYDRA_FRENZY          = 139942,

    SPELL_RAMPAGE_PERIODIC      = 139458,
    SPELL_RAMPAGE_FIRE          = 139548,
    SPELL_RAMPAGE_FIRE_BALL     = 140280,
    SPELL_RAMPAGE_FROST         = 139549,
    SPELL_RAMPAGE_FROST_BALL    = 140285,
    SPELL_RAMPAGE_VENOM         = 139551,
    SPELL_RAMPAGE_VENOM_BALL    = 140286,
    SPELL_RAMPAGE_ARCANE        = 139552,
    SPELL_RAMPAGE_ARCANE_BALL   = 140287,

    SPELL_ELEMENTAL_BLOOD_FIRE  = 139586,
    SPELL_ELEMENTAL_BLOOD_FROST = 139587,
    SPELL_ELEMENTAL_BLOOD_VENOM = 139588,
    SPELL_ELEMENTAL_BLOOD_ARCANE= 139589,

    // Back Flaming Head
    SPELL_CINDERS               = 139822,
    SPELL_CINDERS_SUMMON        = 139834,
    SPELL_CINDERS_AURA          = 139835,
    // Flaming Head
    SPELL_IGNITE_FLESH          = 137729,
    SPELL_IGNITE_FLESH_AURA     = 137731,

    // Black Venomous Head
    SPELL_ACID_RAIN_MISSILE     = 139848,
    SPELL_ACID_RAIN_AURA        = 139847,
    SPELL_ACID_RAIN_DAMAGE      = 139850,
    // Venomous Head
    SPELL_ROT_ARMOR             = 139838,
    SPELL_ROT_ARMOR_AURA        = 139840,

    // Back Frozen Head
    SPELL_TORRENT_OF_ICE        = 139866,
    SPELL_ICY_GROUND_VISUAL     = 139875,
    SPELL_ICY_GROUND_DMG_SNARE  = 139909,
    SPELL_ICY_GROUND_MOD_SCALE  = 140213,
    // Frozen Head
    SPELL_ARCTIC_FREEZE         = 139841,
    SPELL_ARCTIC_FREEZE_AURA    = 139843,
    SPELL_ARCTIC_FREEZE_STUN    = 139844,

    // Back Arcane Head
    SPELL_NETHER_TEAR           = 140138,
    SPELL_NETHER_TEAR_VISUAL    = 140258,
    // Arcane Head
    SPELL_DIFFUSION             = 139991,
    SPELL_DIFFUSION_AURA        = 139993,
    SPELL_DIFFUSION_HEAL        = 139994,

    // Nether Wyrm
    SPELL_NETHER_SPIKE          = 140178,
    SPELL_SUPPRESSION           = 140179
};

enum eEvents
{
    EVENT_CHECK_MELEE       = 1,
    EVENT_END_OF_RAMPAGE,
    EVENT_RAMPAGE_BALL,

    // Back Flaming Head
    EVENT_CINDERS,
    EVENT_CHECK_ICE,
    // Flaming Head
    EVENT_IGNITE_FLESH,

    // Black Venomous Head
    EVENT_ACID_RAIN,
    EVENT_ACID_RAIN_DAMAGE,
    // Venomous Head
    EVENT_ROT_ARMOR,

    // Back Frozen Head
    EVENT_TORRENT_OF_ICE,
    EVENT_CHECK_PLAYERS,
    EVENT_MOD_SCALE,
    // Frozen Head
    EVENT_ARCTIC_FREEZE,

    // Back Arcane Head
    EVENT_NETHER_TEAR,
    // Arcane Head
    EVENT_DIFFUSION,

    // Nether Wyrm
    EVENT_NETHER_SPIKE,
    EVENT_SUPPRESSION
};

enum eDatas
{
    DATA_LAST_HEAD_KILLED
};

enum eActions
{
    ACTION_KILL_VENOMOUS_HEAD = ACTION_MEGAERA_SPAWN + 1,
    ACTION_KILL_FROZEN_HEAD,
    ACTION_KILL_FLAMING_HEAD,
    ACTION_KILL_ARCANE_HEAD,
    ACTION_SPAWN_NETHER_WYRM,
    ACTION_ENABLE_HEAD,
    ACTION_DISABLE_HEAD
};

enum eSounds
{
    SOUND_HYDRA_EMERGE  = 36700
};

enum eHeads
{
    HEAD_NONE       = 0x00,
    HEAD_VENOMOUS   = 0x01,
    HEAD_FROZEN     = 0x02,
    HEAD_FLAMING    = 0x04,
    HEAD_ARCANE     = 0x08
};

// Only initial positions
Position const spawnPositions[3] =
{
    { 6395.12f, 4494.94f, -209.61f, 1.83f }, // Venomous
    { 6419.33f, 4504.38f, -209.61f, 2.30f }, // Frozen
    { 6457.64f, 4471.69f, -209.98f, 2.18f }  // Back Flaming
};

Position const chestPos = { 6467.56f, 4483.96f, 182.452f, 2.502f };

uint32 const headsEntries[4]        = { NPC_FLAMING_HEAD,       NPC_FROZEN_HEAD,        NPC_VENOMOUS_HEAD,      NPC_ARCANE_HEAD         };
uint32 const backHeadsEntries[4]    = { NPC_BACK_FLAMING_HEAD,  NPC_BACK_FROZEN_HEAD,   NPC_BACK_VENOMOUS_HEAD, NPC_BACK_ARCANE_HEAD    };

static void BindPlayersToInstance(Creature* creature)
{
    Map* map = creature->GetMap();

    if (map && map->IsDungeon() && map->IsRaidOrHeroicDungeon())
    {
        Map::PlayerList const &PlList = map->GetPlayers();
        if (!PlList.isEmpty() && PlList.begin()->getSource())
            ((InstanceMap*)map)->PermBindAllPlayers(PlList.begin()->getSource());
    }
};

// Megaera - 68065
class boss_megaera : public CreatureScript
{
    public:
        boss_megaera() : CreatureScript("boss_megaera") { }

        struct boss_megaeraAI : public BossAI
        {
            boss_megaeraAI(Creature* creature) : BossAI(creature, DATA_MEGAERA)
            {
                m_Instance = creature->GetInstanceScript();
                m_Activated = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            uint8 m_LastHeadKilled;
            uint8 m_ActivesHeads;
            uint8 m_BackHead;
            bool  m_Activated;

            void Reset()
            {
                _Reset();

                m_Events.Reset();

                DisableActualHeads();
                summons.DespawnAll();

                // Raid needs to kill 7 heads to defeat Megaera
                m_BackHead        = HEAD_NONE;
                m_LastHeadKilled  = HEAD_NONE;
                m_ActivesHeads    = HEAD_NONE;

                me->ReenableEvadeMode();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_MEGAERA, NOT_STARTED);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CINDERS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARCTIC_FREEZE_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARCTIC_FREEZE_STUN);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ROT_ARMOR_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DIFFUSION_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICY_GROUND_DMG_SNARE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IGNITE_FLESH_AURA);

                    if (m_Activated)
                        SummonInitialHeads();
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                _EnterCombat();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_MEGAERA, IN_PROGRESS);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                    for (uint8 i = 0; i < 4; ++i)
                    {
                        std::list<Creature*> headList;
                        me->GetCreatureListWithEntryInGrid(headList, backHeadsEntries[i], 100.0f);

                        for (auto itr : headList)
                            itr->SetInCombatWithZone();
                    }
                }
            }

            void JustDied(Unit* p_Killer)
            {
                _JustDied();

                DisableActualHeads(true);

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_MEGAERA, DONE);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CINDERS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARCTIC_FREEZE_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARCTIC_FREEZE_STUN);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ROT_ARMOR_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DIFFUSION_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICY_GROUND_DMG_SNARE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IGNITE_FLESH_AURA);

                    BindPlayersToInstance(me);
                }

                if (p_Killer)
                {
                    switch (me->GetMap()->GetSpawnMode())
                    {
                        case Difficulty::Difficulty10N:
                            p_Killer->SummonGameObject(GOB_MEGAERA_CHEST_10_NORMAL, chestPos.m_positionX, chestPos.m_positionY, chestPos.m_positionZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        case Difficulty::Difficulty25N:
                            p_Killer->SummonGameObject(GOB_MEGAERA_CHEST_25_NORMAL, chestPos.m_positionX, chestPos.m_positionY, chestPos.m_positionZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        case Difficulty::Difficulty10HC:
                            p_Killer->SummonGameObject(GOB_MEGAERA_CHEST_10_HEROIC, chestPos.m_positionX, chestPos.m_positionY, chestPos.m_positionZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        case Difficulty::Difficulty25HC:
                            p_Killer->SummonGameObject(GOB_MEGAERA_CHEST_25_HEROIC, chestPos.m_positionX, chestPos.m_positionY, chestPos.m_positionZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            break;
                        default:
                            break;
                    }
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootDistribution(me, l_Player->GetGroup());
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_MEGAERA, FAIL);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_MEGAERA_SPAWN:
                        SummonInitialHeads();
                        m_Activated = true;
                        break;
                    case ACTION_KILL_VENOMOUS_HEAD:
                        m_ActivesHeads    &= ~HEAD_VENOMOUS;
                        m_ActivesHeads    |= m_BackHead;
                        m_BackHead        = HEAD_VENOMOUS;
                        m_LastHeadKilled  = HEAD_VENOMOUS;
                        HandleNextHeadSpawn();
                        break;
                    case ACTION_KILL_FROZEN_HEAD:
                        m_ActivesHeads    &= ~HEAD_FROZEN;
                        m_ActivesHeads    |= m_BackHead;
                        m_BackHead        = HEAD_FROZEN;
                        m_LastHeadKilled  = HEAD_FROZEN;
                        HandleNextHeadSpawn();
                        break;
                    case ACTION_KILL_FLAMING_HEAD:
                        m_ActivesHeads    &= ~HEAD_FLAMING;
                        m_ActivesHeads    |= m_BackHead;
                        m_BackHead        = HEAD_FLAMING;
                        m_LastHeadKilled  = HEAD_FLAMING;
                        HandleNextHeadSpawn();
                        break;
                    case ACTION_KILL_ARCANE_HEAD:
                        m_ActivesHeads    &= ~HEAD_ARCANE;
                        m_ActivesHeads    |= m_BackHead;
                        m_BackHead        = HEAD_ARCANE;
                        m_LastHeadKilled  = HEAD_ARCANE;
                        HandleNextHeadSpawn();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_END_OF_RAMPAGE:
                        if (Creature* l_GenericMop = me->FindNearestCreature(NPC_SLG_GENERIC_MOP, 200.0f))
                            l_GenericMop->MonsterTextEmote("Megaera's rage subsides.", 0, true);
                        me->RemoveAura(SPELL_RAMPAGE_PERIODIC);
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_ID)
            {
                if (p_ID == DATA_LAST_HEAD_KILLED)
                    return m_LastHeadKilled;

                return HEAD_NONE;
            }

            void SummonInitialHeads()
            {
                Position l_Pos = spawnPositions[0];
                if (Creature* l_Head = me->SummonCreature(NPC_VENOMOUS_HEAD, l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, l_Pos.m_orientation))
                    l_Head->AI()->DoAction(ACTION_ENABLE_HEAD);

                l_Pos = spawnPositions[1];
                if (Creature* l_Head = me->SummonCreature(NPC_FROZEN_HEAD, l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, l_Pos.m_orientation))
                    l_Head->AI()->DoAction(ACTION_ENABLE_HEAD);

                l_Pos = spawnPositions[2];
                if (Creature* l_BackHead = me->SummonCreature(NPC_BACK_FLAMING_HEAD, l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, l_Pos.m_orientation))
                    l_BackHead->AI()->DoAction(ACTION_ENABLE_HEAD);

                m_ActivesHeads = HEAD_VENOMOUS | HEAD_FROZEN;
                m_BackHead = HEAD_FLAMING;
            }

            void HandleNextHeadSpawn()
            {
                DisableActualHeads();
                ActivateNextHeads();
                TriggerElementalBlood();
            }

            void DisableActualHeads(bool p_EndCombat = false)
            {
                std::list<Creature*> headList;
                for (uint8 i = 0; i < 4; ++i)
                {
                    headList.clear();
                    me->GetCreatureListWithEntryInGrid(headList, headsEntries[i], 250.0f);

                    for (Creature* head : headList)
                    {
                        if (p_EndCombat)
                        {
                            head->AI()->EnterEvadeMode();
                            continue;
                        }

                        head->AI()->DoAction(ACTION_DISABLE_HEAD);
                    }
                }

                for (uint8 i = 0; i < 4; ++i)
                {
                    headList.clear();
                    me->GetCreatureListWithEntryInGrid(headList, backHeadsEntries[i], 250.0f);

                    for (Creature* backHead : headList)
                    {
                        if (p_EndCombat)
                        {
                            backHead->AI()->EnterEvadeMode();
                            continue;
                        }

                        backHead->AI()->DoAction(ACTION_DISABLE_HEAD);
                    }
                }
            }

            void ActivateNextHeads()
            {
                if (m_ActivesHeads == HEAD_NONE)
                {
                    EnterEvadeMode();
                    return;
                }
                else
                {
                    me->CastSpell(me, SPELL_RAMPAGE_PERIODIC, true);
                    m_Events.ScheduleEvent(EVENT_END_OF_RAMPAGE, 20000);

                    if (Creature* l_GenericMop = me->FindNearestCreature(NPC_SLG_GENERIC_MOP, 200.0f))
                        l_GenericMop->MonsterTextEmote("Megaera begins to |cFFF00000|Hspell:139458|h[Rampage]|h|r !", 0, true);

                    std::list<Creature*> headList;
                    if (m_ActivesHeads & HEAD_VENOMOUS)
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_VENOMOUS_HEAD, 250.0f);
                        EnableActivesHeads(headList, false);
                    }

                    headList.clear();
                    if (m_ActivesHeads & HEAD_FROZEN)
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_FROZEN_HEAD, 250.0f);
                        EnableActivesHeads(headList, false);
                    }

                    headList.clear();
                    if (m_ActivesHeads & HEAD_FLAMING)
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_FLAMING_HEAD, 250.0f);
                        EnableActivesHeads(headList, false);
                    }

                    headList.clear();
                    if (m_ActivesHeads & HEAD_ARCANE)
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_ARCANE_HEAD, 250.0f);
                        EnableActivesHeads(headList, false);
                    }

                    headList.clear();
                    switch (m_BackHead)
                    {
                        case HEAD_VENOMOUS:
                            me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_VENOMOUS_HEAD, 250.0f);
                            break;
                        case HEAD_FROZEN:
                            me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_FROZEN_HEAD, 250.0f);
                            break;
                        case HEAD_FLAMING:
                            me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_FLAMING_HEAD, 250.0f);
                            break;
                        case HEAD_ARCANE:
                            me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_ARCANE_HEAD, 250.0f);
                            break;
                        default:
                            break;
                    }

                    EnableActivesHeads(headList, true);
                }
            }

            void EnableActivesHeads(std::list<Creature*> headList, bool twoHeads)
            {
                if (headList.empty())
                    return;

                JadeCore::RandomResizeList(headList, twoHeads ? 2 : 1);
                for (Creature* head : headList)
                    head->AI()->DoAction(ACTION_ENABLE_HEAD);
            }

            void TriggerElementalBlood()
            {
                std::list<Creature*> headList;
                switch (m_LastHeadKilled)
                {
                    case HEAD_FLAMING:
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_FLAMING_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_FIRE, true);

                        headList.clear();
                        me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_FLAMING_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_FIRE, true);
                        break;
                    }
                    case HEAD_FROZEN:
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_FROZEN_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_FROST, true);

                        headList.clear();
                        me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_FROZEN_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_FROST, true);
                        break;
                    }
                    case HEAD_VENOMOUS:
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_VENOMOUS_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_VENOM, true);

                        headList.clear();
                        me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_VENOMOUS_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_VENOM, true);
                        break;
                    }
                    case HEAD_ARCANE:
                    {
                        me->GetCreatureListWithEntryInGrid(headList, NPC_ARCANE_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_ARCANE, true);

                        headList.clear();
                        me->GetCreatureListWithEntryInGrid(headList, NPC_BACK_ARCANE_HEAD, 250.0f);
                        for (Creature* head : headList)
                            head->CastSpell(head, SPELL_ELEMENTAL_BLOOD_ARCANE, true);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_megaeraAI(creature);
        }
};

// Flaming Head - 70212
// Frozen Head - 70235
// Venomous Head - 70247
// Arcane Head - 70248
class boss_megaera_head : public CreatureScript
{
    public:
        boss_megaera_head() : CreatureScript("boss_megaera_head") { }

        struct boss_megaera_headAI : public BossAI
        {
            boss_megaera_headAI(Creature* creature) : BossAI(creature, DATA_MEGAERA)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            bool rampage;

            void Reset()
            {
                events.Reset();

                summons.DespawnAll();

                rampage = false;

                PlaySoundAtSpawn();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->ReenableEvadeMode();
                me->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD | UNIT_DYNFLAG_TAPPED_BY_PLAYER | UNIT_DYNFLAG_TAPPED);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_SUBMERGED, true);
            }

            void JustReachedHome()
            {
                if (pInstance)
                {
                    if (Creature* megaera = Creature::GetCreature(*me, pInstance->GetData64(NPC_MEGAERA)))
                        megaera->AI()->EnterEvadeMode();

                    DisableHead();
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);

                switch (me->GetEntry())
                {
                    case NPC_FLAMING_HEAD:
                        events.ScheduleEvent(EVENT_IGNITE_FLESH, 5000);
                        break;
                    case NPC_VENOMOUS_HEAD:
                        events.ScheduleEvent(EVENT_ROT_ARMOR, 5000);
                        break;
                    case NPC_FROZEN_HEAD:
                        events.ScheduleEvent(EVENT_ARCTIC_FREEZE, 5000);
                        break;
                    case NPC_ARCANE_HEAD:
                        events.ScheduleEvent(EVENT_DIFFUSION, 5000);
                        break;
                    default:
                        break;
                }

                if (pInstance)
                {
                    if (Creature* megaera = Creature::GetCreature(*me, pInstance->GetData64(NPC_MEGAERA)))
                        megaera->SetInCombatWithZone();

                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (damage >= me->GetHealth())
                {
                    me->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD | UNIT_DYNFLAG_TAPPED_BY_PLAYER | UNIT_DYNFLAG_TAPPED);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    me->SetReactState(REACT_PASSIVE);
                    me->CastSpell(me, SPELL_SUBMERGED, true);
                    me->RemoveAura(SPELL_HYDRA_FRENZY);

                    if (pInstance)
                    {
                        if (Creature* megaera = Creature::GetCreature(*me, pInstance->GetData64(NPC_MEGAERA)))
                        {
                            // Must add 2 because Megaera is at 12hp after killing 7 heads
                            attacker->DealDamage(megaera, me->GetMaxHealth() + 2);

                            switch (me->GetEntry())
                            {
                                case NPC_VENOMOUS_HEAD:
                                    megaera->AI()->DoAction(ACTION_KILL_VENOMOUS_HEAD);
                                    break;
                                case NPC_FROZEN_HEAD:
                                    megaera->AI()->DoAction(ACTION_KILL_FROZEN_HEAD);
                                    break;
                                case NPC_FLAMING_HEAD:
                                    megaera->AI()->DoAction(ACTION_KILL_FLAMING_HEAD);
                                    break;
                                case NPC_ARCANE_HEAD:
                                    megaera->AI()->DoAction(ACTION_KILL_ARCANE_HEAD);
                                    break;
                                default:
                                    break;
                            }
                        }

                        pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    }

                    damage = 0;
                    return;
                }
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_ENABLE_HEAD:
                        EnableHead();
                        break;
                    case ACTION_DISABLE_HEAD:
                        DisableHead();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasAura(SPELL_SUBMERGED))
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_IGNITE_FLESH:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_IGNITE_FLESH, 100);
                            break;
                        }
                        me->CastSpell(me, SPELL_IGNITE_FLESH, false);
                        events.ScheduleEvent(EVENT_IGNITE_FLESH, 15000);
                        break;
                    }
                    case EVENT_ROT_ARMOR:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_ROT_ARMOR, 100);
                            break;
                        }
                        me->CastSpell(me, SPELL_ROT_ARMOR, false);
                        events.ScheduleEvent(EVENT_ROT_ARMOR, 15000);
                        break;
                    }
                    case EVENT_ARCTIC_FREEZE:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_ARCTIC_FREEZE, 100);
                            break;
                        }
                        me->CastSpell(me, SPELL_ARCTIC_FREEZE, false);
                        events.ScheduleEvent(EVENT_ARCTIC_FREEZE, 15000);
                        break;
                    }
                    case EVENT_DIFFUSION:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_DIFFUSION, 100);
                            break;
                        }
                        me->CastSpell(me, SPELL_DIFFUSION, false);
                        events.ScheduleEvent(EVENT_DIFFUSION, 15000);
                        break;
                    }
                    case EVENT_CHECK_MELEE:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_CHECK_MELEE, 100);
                            break;
                        }
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        {
                            if (!me->IsWithinMeleeRange(target))
                            {
                                switch (me->GetEntry())
                                {
                                    case NPC_FLAMING_HEAD:
                                        me->CastSpell(target, SPELL_MEGAERAS_RAGE_FIRE, false);
                                        break;
                                    case NPC_FROZEN_HEAD:
                                        me->CastSpell(target, SPELL_MEGAERAS_RAGE_FROST, false);
                                        break;
                                    case NPC_VENOMOUS_HEAD:
                                        me->CastSpell(target, SPELL_MEGAERAS_RAGE_VENOM, false);
                                        break;
                                    case NPC_ARCANE_HEAD:
                                        me->CastSpell(target, SPELL_MEGAERAS_RAGE_ARCANE, false);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                        break;
                    }
                    case EVENT_RAMPAGE_BALL:
                    {
                        if (!rampage)
                            break;

                        switch (me->GetEntry())
                        {
                            case NPC_FLAMING_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_FIRE_BALL, true);
                                break;
                            case NPC_FROZEN_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_FROST_BALL, true);
                                break;
                            case NPC_VENOMOUS_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_VENOM_BALL, true);
                                break;
                            case NPC_ARCANE_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_ARCANE_BALL, true);
                                break;
                            default:
                                break;
                        }

                        events.ScheduleEvent(EVENT_RAMPAGE_BALL, 1000);
                        break;
                    }
                    case EVENT_END_OF_RAMPAGE:
                        rampage = false;
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void PlaySoundAtSpawn()
            {
                Map::PlayerList const& plrList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = plrList.begin(); itr != plrList.end(); ++itr)
                {
                    if (Player* plr = itr->getSource())
                        plr->SendPlaySound(SOUND_HYDRA_EMERGE, true);
                }
            }

            void DisableHead()
            {
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_SUBMERGED, true);
            }

            void EnableHead()
            {
                me->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, 0);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveAura(SPELL_SUBMERGED);
                me->SetFullHealth();

                if (pInstance)
                {
                    if (me->isInCombat())
                        pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    else
                    {
                        if (Player* l_Player = me->FindNearestPlayer(50.0f))
                        {
                            AttackStart(l_Player);
                            pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                        }
                    }

                    if (Creature* megaera = Creature::GetCreature(*me, pInstance->GetData64(NPC_MEGAERA)))
                    {
                        if (megaera->HasAura(SPELL_RAMPAGE_PERIODIC))
                        {
                            me->CastSpell(me, SPELL_HYDRA_FRENZY, true);
                            rampage = true;
                            events.ScheduleEvent(EVENT_RAMPAGE_BALL, 100);
                            events.ScheduleEvent(EVENT_END_OF_RAMPAGE, 20000);
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_megaera_headAI(creature);
        }
};

// Back Flaming Head - 70229
// Back Frozen Head - 70250
// Back Venomous Head - 70251
// Back Arcane Head - 70252
class boss_megaera_back_head : public CreatureScript
{
    public:
        boss_megaera_back_head() : CreatureScript("boss_megaera_back_head") { }

        struct boss_megaera_back_headAI : public BossAI
        {
            boss_megaera_back_headAI(Creature* creature) : BossAI(creature, DATA_MEGAERA)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;
            WorldLocation const* netherTearDest;
            bool rampage;

            void Reset()
            {
                events.Reset();

                summons.DespawnAll();

                rampage = false;

                std::list<Creature*> cinders;
                me->GetCreatureListWithEntryInGrid(cinders, NPC_CINDERS, 250.0f);
                for (Creature* cinder : cinders)
                    cinder->DespawnOrUnsummon();

                cinders.clear();
                me->GetCreatureListWithEntryInGrid(cinders, NPC_ICY_GROUND, 250.0f);
                for (Creature* cinder : cinders)
                    cinder->DespawnOrUnsummon();

                cinders.clear();
                me->GetCreatureListWithEntryInGrid(cinders, NPC_ACID_RAIN, 250.0f);
                for (Creature* cinder : cinders)
                    cinder->DespawnOrUnsummon();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->ReenableEvadeMode();

                netherTearDest = NULL;
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_SUBMERGED, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                switch (me->GetEntry())
                {
                    case NPC_BACK_FLAMING_HEAD:
                        events.ScheduleEvent(EVENT_CINDERS, 5000);
                        break;
                    case NPC_BACK_VENOMOUS_HEAD:
                        events.ScheduleEvent(EVENT_ACID_RAIN, 5000);
                        break;
                    case NPC_BACK_FROZEN_HEAD:
                        events.ScheduleEvent(EVENT_TORRENT_OF_ICE, 5000);
                        break;
                    case NPC_BACK_ARCANE_HEAD:
                        events.ScheduleEvent(EVENT_NETHER_TEAR, 5000);
                        break;
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void SetDestTarget(WorldLocation const* dest)
            {
                netherTearDest = dest;
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SPAWN_NETHER_WYRM:
                        me->SummonCreature(NPC_NETHER_WYRM, netherTearDest->m_positionX, netherTearDest->m_positionY, netherTearDest->m_positionZ);
                        break;
                    case ACTION_ENABLE_HEAD:
                        EnableHead();
                        break;
                    case ACTION_DISABLE_HEAD:
                        DisableHead();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(SPELL_SUBMERGED))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CINDERS:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_CINDERS, 100);
                            break;
                        }
                        me->CastSpell(me, SPELL_CINDERS, false);
                        events.ScheduleEvent(EVENT_CINDERS, 15000);
                        break;
                    }
                    case EVENT_ACID_RAIN:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_ACID_RAIN, 100);
                            break;
                        }
                        for (uint8 i = 0; i < 3; ++i)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->SummonCreature(NPC_ACID_RAIN, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());

                            if (!IsHeroic())
                                break;
                        }

                        me->CastSpell(me, SPELL_ACID_RAIN_MISSILE, true);
                        events.ScheduleEvent(EVENT_ACID_RAIN, 15000);
                        break;
                    }
                    case EVENT_TORRENT_OF_ICE:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_TORRENT_OF_ICE, 100);
                            break;
                        }

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_TORRENT_OF_ICE, false);
                        events.ScheduleEvent(EVENT_TORRENT_OF_ICE, 15000);
                        break;
                    }
                    case EVENT_NETHER_TEAR:
                    {
                        if (rampage)
                        {
                            events.ScheduleEvent(EVENT_NETHER_TEAR, 100);
                            break;
                        }

                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_NETHER_TEAR, false);
                        events.ScheduleEvent(EVENT_NETHER_TEAR, 15000);
                        break;
                    }
                    case EVENT_RAMPAGE_BALL:
                    {
                        if (!rampage)
                            break;

                        switch (me->GetEntry())
                        {
                            case NPC_BACK_FLAMING_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_FIRE_BALL, true);
                                break;
                            case NPC_BACK_FROZEN_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_FROST_BALL, true);
                                break;
                            case NPC_BACK_VENOMOUS_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_VENOM_BALL, true);
                                break;
                            case NPC_BACK_ARCANE_HEAD:
                                me->CastSpell(me, SPELL_RAMPAGE_ARCANE_BALL, true);
                                break;
                            default:
                                break;
                        }

                        events.ScheduleEvent(EVENT_RAMPAGE_BALL, 1000);
                        break;
                    }
                    case EVENT_END_OF_RAMPAGE:
                        rampage = false;
                        break;
                    default:
                        break;
                }
            }

            void DisableHead()
            {
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_SUBMERGED, true);
            }

            void EnableHead()
            {
                me->SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, 0);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveAura(SPELL_SUBMERGED);

                if (pInstance)
                {
                    if (Creature* megaera = Creature::GetCreature(*me, pInstance->GetData64(NPC_MEGAERA)))
                    {
                        if (megaera->HasAura(SPELL_RAMPAGE_PERIODIC))
                        {
                            rampage = true;
                            events.ScheduleEvent(EVENT_RAMPAGE_BALL, 100);
                            events.ScheduleEvent(EVENT_END_OF_RAMPAGE, 20000);
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_megaera_back_headAI(creature);
        }
};

// Cinders - 70432
class mob_cinders : public CreatureScript
{
    public:
        mob_cinders() : CreatureScript("mob_cinders") { }

        struct mob_cindersAI : public ScriptedAI
        {
            mob_cindersAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->CastSpell(me, SPELL_CINDERS_AURA, true);

                events.ScheduleEvent(EVENT_CHECK_ICE, 500);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_ICE:
                    {
                        std::list<Player*> plrList;
                        me->GetPlayerListInGrid(plrList, 5.0f);

                        for (auto itr : plrList)
                        {
                            if (itr->HasAura(SPELL_TORRENT_OF_ICE))
                            {
                                me->DespawnOrUnsummon();
                                break;
                            }
                        }

                        events.ScheduleEvent(EVENT_CHECK_ICE, 500);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_cindersAI(creature);
        }
};

// Acid Rain - 70435
class mob_acid_rain : public CreatureScript
{
    public:
        mob_acid_rain() : CreatureScript("mob_acid_rain") { }

        struct mob_acid_rainAI : public ScriptedAI
        {
            mob_acid_rainAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->CastSpell(me, SPELL_ACID_RAIN_AURA, true);

                events.ScheduleEvent(EVENT_ACID_RAIN_DAMAGE, 3000);
            }

            void UpdateAI(uint32 const diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_ACID_RAIN_DAMAGE:
                        me->CastSpell(me, SPELL_ACID_RAIN_DAMAGE, true);
                        me->RemoveAura(SPELL_ACID_RAIN_AURA);
                        me->DespawnOrUnsummon(5000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_acid_rainAI(creature);
        }
};

// Icy Ground - 70446
class mob_icy_ground : public CreatureScript
{
    public:
        mob_icy_ground() : CreatureScript("mob_icy_ground") { }

        struct mob_icy_groundAI : public ScriptedAI
        {
            mob_icy_groundAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->CastSpell(me, SPELL_ICY_GROUND_VISUAL, true);

                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 500);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_MOD_SCALE, 600);
            }

            void UpdateAI(uint32 const diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_PLAYERS:
                    {
                        std::list<Player*> plrList;
                        me->GetPlayerListInGrid(plrList, 5.0f);

                        for (auto itr : plrList)
                        {
                            if (itr->HasAura(SPELL_CINDERS))
                            {
                                me->DespawnOrUnsummon();
                                return;
                            }

                            me->CastSpell(itr, SPELL_ICY_GROUND_DMG_SNARE, true);
                        }

                        events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1500);
                        break;
                    }
                    case EVENT_MOD_SCALE:
                        me->CastSpell(me, SPELL_ICY_GROUND_MOD_SCALE, true);
                        events.ScheduleEvent(EVENT_MOD_SCALE, 600);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_icy_groundAI(creature);
        }
};

// Nether Wyrm - 70507
class mob_nether_wyrm : public CreatureScript
{
    public:
        mob_nether_wyrm() : CreatureScript("mob_nether_wyrm") { }

        struct mob_nether_wyrmAI : public ScriptedAI
        {
            mob_nether_wyrmAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_NETHER_SPIKE, 5000);
                events.ScheduleEvent(EVENT_SUPPRESSION, 10000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_NETHER_SPIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_NETHER_SPIKE, false);
                        events.ScheduleEvent(EVENT_NETHER_SPIKE, 10000);
                        break;
                    case EVENT_SUPPRESSION:
                        me->CastSpell(me, SPELL_SUPPRESSION, false);
                        events.ScheduleEvent(EVENT_SUPPRESSION, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_nether_wyrmAI(creature);
        }
};

// Cinders - 139822
class spell_cinders: public SpellScriptLoader
{
    public:
        spell_cinders() : SpellScriptLoader("spell_cinders") { }

        class spell_cinders_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_cinders_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (caster->GetMap()->IsHeroic() && aurEff->GetTickNumber() % 3)
                            caster->CastSpell(target, SPELL_CINDERS_SUMMON, true);
                    }
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Unit* caster = GetCaster())
                    {
                        caster->CastSpell(target, SPELL_CINDERS_SUMMON, true);

                        if (caster->GetMap()->IsHeroic() && aurEff->GetTickNumber() % 3)
                            caster->CastSpell(target, SPELL_CINDERS_SUMMON, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_cinders_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
                AfterEffectRemove += AuraEffectRemoveFn(spell_cinders_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_cinders_AuraScript();
        }
};

// Arctic Freeze (aura) - 139843
class spell_arctic_freeze: public SpellScriptLoader
{
    public:
        spell_arctic_freeze() : SpellScriptLoader("spell_arctic_freeze") { }

        class spell_arctic_freeze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arctic_freeze_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura* arcticFreeze = aurEff->GetBase())
                    {
                        if (arcticFreeze->GetStackAmount() >= 5)
                        {
                            target->CastSpell(target, SPELL_ARCTIC_FREEZE_STUN, true);
                            target->RemoveAura(SPELL_ARCTIC_FREEZE_AURA);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_arctic_freeze_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arctic_freeze_AuraScript();
        }
};

// Acid Rain (damage) - 139850
class spell_acid_rain_damage: public SpellScriptLoader
{
    public:
        spell_acid_rain_damage() : SpellScriptLoader("spell_acid_rain_damage") { }

        class spell_acid_rain_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_acid_rain_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        float dist = std::max(1.0f, caster->GetDistance(target));
                        SetHitDamage(int32(ceil(GetHitDamage() / dist)));
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_acid_rain_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_acid_rain_damage_SpellScript();
        }
};

// Diffusion - 139993
class spell_diffusion: public SpellScriptLoader
{
    public:
        spell_diffusion() : SpellScriptLoader("spell_diffusion") { }

        class spell_diffusion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_diffusion_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();

                if (!target || !eventInfo.GetActor() || !target->ToPlayer())
                    return;

                if (target->ToPlayer()->HasSpellCooldown(SPELL_DIFFUSION_HEAL))
                    return;

                float ratio = 0.9f;
                int32 bp = eventInfo.GetHealInfo() ? eventInfo.GetHealInfo()->GetHeal() : 0;
                bp /= ratio;

                target->CastCustomSpell(target, SPELL_DIFFUSION_HEAL, &bp, NULL, NULL, true);
                target->ToPlayer()->AddSpellCooldown(SPELL_DIFFUSION_HEAL, 0, 1000);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_diffusion_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_diffusion_AuraScript();
        }
};

// Nether Tear - 140138
class spell_nether_tear: public SpellScriptLoader
{
    public:
        spell_nether_tear() : SpellScriptLoader("spell_nether_tear") { }

        class spell_nether_tear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_nether_tear_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster() || !GetCaster()->ToCreature())
                    return;

                if (Creature* caster = GetCaster()->ToCreature())
                {
                    if (WorldLocation const* dest = GetExplTargetDest())
                    {
                        caster->CastSpell(dest->m_positionX, dest->m_positionY, dest->m_positionZ, SPELL_NETHER_TEAR_VISUAL, true);
                        caster->AI()->SetDestTarget(dest);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_nether_tear_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_nether_tear_SpellScript();
        }

        class spell_nether_tear_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_nether_tear_AuraScript);

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (!GetTarget() || !GetTarget()->ToCreature())
                    return;

                if (Creature* arcaneHead = GetTarget()->ToCreature())
                    arcaneHead->AI()->DoAction(ACTION_SPAWN_NETHER_WYRM);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_nether_tear_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_nether_tear_AuraScript();
        }
};

// Rampage (periodic) - 139458
class spell_rampage_periodic: public SpellScriptLoader
{
    public:
        spell_rampage_periodic() : SpellScriptLoader("spell_rampage_periodic") { }

        class spell_rampage_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rampage_periodic_AuraScript);

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (!GetTarget() || !GetTarget()->ToCreature())
                    return;

                if (Creature* megaera = GetTarget()->ToCreature())
                {
                    switch (megaera->AI()->GetData(DATA_LAST_HEAD_KILLED))
                    {
                        case HEAD_FLAMING:
                            megaera->CastSpell(megaera, SPELL_RAMPAGE_FIRE, true);
                            break;
                        case HEAD_FROZEN:
                            megaera->CastSpell(megaera, SPELL_RAMPAGE_FROST, true);
                            break;
                        case HEAD_VENOMOUS:
                            megaera->CastSpell(megaera, SPELL_RAMPAGE_VENOM, true);
                            break;
                        case HEAD_ARCANE:
                            megaera->CastSpell(megaera, SPELL_RAMPAGE_ARCANE, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rampage_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rampage_periodic_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_megaera()
{
    new boss_megaera();
    new boss_megaera_head();
    new boss_megaera_back_head();
    new mob_cinders();
    new mob_acid_rain();
    new mob_icy_ground();
    new mob_nether_wyrm();
    new spell_cinders();
    new spell_arctic_freeze();
    new spell_acid_rain_damage();
    new spell_diffusion();
    new spell_nether_tear();
    new spell_rampage_periodic();
}
#endif
