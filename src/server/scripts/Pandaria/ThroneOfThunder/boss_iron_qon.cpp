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
#include "Vehicle.h"
#include "throne_of_thunder.h"
#include "MoveSplineInit.h"
#include "GameObjectAI.h"

enum eQonSpells
{
    // Iron Qon
        // Impale
    SPELL_IMPALE                = 134691,
        // Throw spear
    SPELL_THROW_QON_SPEAR       = 134925,
    SPELL_SPEAR_VISUAL          = 134927,
    SPELL_INACTIVE_STUN         = 118319,
    SPELL_SPEAR_BACK            = 137712,   // Triggers 137714 and remove 135104 aura
    SPELL_SPEAR_JUMP            = 137714,   // Jump back to Iron Qon
        // Rising anger
    SPELL_RISING_ANGER          = 136324,   // Triggers 136323 every 15s
        // Fist Smash - only when disarmed
    SPELL_FIST_SMASH            = 136146,   // Triggers 136147 every 0.75 sec for 7.5s
        // Change cyclone
    SPELL_IGNITE_CYCLONE        = 136159,
    SPELL_FREEZE_CYCLONE        = 136153,
        // Ride a Quilen
    SPELL_RIDE_VEHICLE          = 46598,
        // Berserk
    SPELL_BERSERK               = 26662,

    // Ro'Shak
        // Reset Power
    SPELL_ZERO_POWER_REGEN      = 118357,
        // Burning Cinders
    SPELL_BURNING_CINDERS_BOLT  = 136330,
    SPELL_BURNING_CINDERS_AT    = 134758,
    SPELL_BURNING_CINDERS_DMG   = 137668,
    SPELL_BURNING_CINDERS_AURA  = 135052,
        // Unleashed Shot + Molten spells
    SPELL_UNLEASHED_SHOT        = 139138,
    SPELL_MOLTEN_INFERNO        = 134664,
    SPELL_MOLTEN_OVERLOAD       = 137221,
    SPELL_SCORCHED              = 134647,

    // Quet'zal
        // Arcing Lightning
    SPELL_LIGHTNING_STORM       = 136192,
    SPELL_SUMMON_STORM          = 136198,
        // Storm Cloud
    SPELL_STORM_CLOUD_BOLT      = 136419,
    SPELL_STORM_CLOUD_AT        = 136421,
    SPELL_STORM_CLOUD_DMG       = 137669,
    SPELL_STORM_CLOUD_AURA      = 136425,
        // Windstorm
    SPELL_VISUAL_TORNADO        = 135570,
    SPELL_WINDSTORM_AT          = 136574,
    SPELL_WINDSTORM_DAMAGE      = 136161,   // Triggers 135583 which have a script effect to trigger 137654
    SPELL_RUSHING_WINDS         = 136590,
    SPELL_RUSHING_WINDS_AURA    = 136596,
    SPELL_WINDSTORM_JUMP        = 136592,
    SPELL_WINDSTORM_KNOCKBACK   = 137710,
    SPELL_WINDSTORM_RIDE        = 137654,
    SPELL_WINDSTORM_PERIODIC    = 135583,
        // Heroic
    SPELL_FROST_SPIKE           = 139180,

    // Dam'ren
        // Dead zones
    SPELL_DEAD_ZONE_FR          = 137226,   // Front + Right
    SPELL_DEAD_ZONE_LR          = 137227,   // Left  + Right
    SPELL_DEAD_ZONE_FL          = 137228,   // Front + Left
    SPELL_DEAD_ZONE_BF          = 137229,   // Back  + Front
    SPELL_DEAD_ZONE_BL          = 137230,   // Back  + Left
    SPELL_DEAD_ZONE_BR          = 137231,   // Back  + Right
    SPELL_DEAD_ZONE             = 137232,   // Call one of the 137226 - 137231 spells
        // Frozen Resilience
    SPELL_FROZEN_RESILIENCE_LFR = 135142,
    SPELL_FROZEN_RESILIENCE     = 135144,
        // Freeze
    SPELL_FROZEN                = 135145,
    SPELL_SHATTER               = 135146,
        // Frozen Blood
    SPELL_FROZEN_BLOOD_BOLT     = 136449,
    SPELL_FROZEN_BLOOD_AT       = 136451,
    SPELL_FROZEN_BLOOD_DMG      = 137664,
    SPELL_FROZEN_BLOOD_AURA     = 136452
};

enum eQonEvents
{
    // Iron Qon
    EVENT_THROW_SPEAR           = 1,
    EVENT_ACTIVATE_SPEAR,
    EVENT_SPEAR_BACK,
    EVENT_SPEAR_DISAPPEAR,
    EVENT_IMPALE,
    EVENT_FIST_SMASH,
    EVENT_MODIFY_CYCLONE,
    EVENT_QUILEN_TALK,
    EVENT_BERSERK,
    EVENT_CHANGE_MOUNT,

    // Ro'Shak
    EVENT_MOLTEN_INFERNO,
    EVENT_UNLEASHED_FLAME,
    EVENT_LINE_SPEAR_STEP,
    EVENT_LAND,

    // Quet'zal
    EVENT_ARCING_LIGHTNING,
    EVENT_WINDSTORM,
    EVENT_WINDSTORM_END,
    EVENT_WINDSTORM_LINGER,
    EVENT_STORM_CLOUD,
    EVENT_STRIKE_SPEARS,
    EVENT_CHECK_LIGHTED_PLAYER,
    EVENT_WINDSTORM_MOVE,
    EVENT_CHECK_STORM_PLAYER,
    EVENT_STORM_DAMAGE_PLAYER,

    // Dam'Ren
    EVENT_DEAD_ZONE,
    EVENT_FREEZE
};

enum eQonActions
{
    ACTION_LAUNCH_MANCHU        = 2,
    ACTION_LAUNCH_WEISHENG      = 3,
    ACTION_QUILEN_DIES          = 4,
    ACTION_MOGU_DIES            = 5,
    ACTION_IRON_QON_TRASH_DONE  = 6,
    ACTION_LINING_SPEAR         = 7,
    ACTION_CHANGE_MOUNT         = 8,
    ACTION_QUILEN_ACTIVATE      = 9,
    ACTION_MOUNT_DIES           = 10,
    ACTION_ACTIVATE_PHASE       = 11,
    ACTION_WINDSTORM_DELAY      = 12,
    ACTION_WINDSTORM_LINGER     = 13
};

enum eQonTypes
{
    TYPE_GET_PHASE              = 1,
    TYPE_UPDATE_PHASE,
    TYPE_GET_SPEAR_INDEX,
    TYPE_SET_STORM_INDEX
};

enum eQonAdds
{
    NPC_IRON_QON_SPEAR          = 68480,
    NPC_SPEAR_LINE              = 69159,
    NPC_IRON_QON_INVISIBLE_MAN  = 64693,
    NPC_RUSHING_WINDS_A         = 68852,
    NPC_RUSHING_WINDS_B         = 69703,
    NPC_STORM_CLOUD             = 70660
};

enum eQonWeapons
{
    EQUIP_IRON_QON              = 93234
};

enum eQonDisplay
{
    DISPLAYID_RUSHING_WINDS     = 38497
};

enum eQonTalks
{
    TALK_LAUNCH_MANCHU          = 1,    // Strike quickly, Manchu! Show them no mercy!
    TALK_LAUNCH_WEISHENG        = 2,    // Focus, Weisheng! Show your enemy the meaning of pain!
    TALK_QUILEN_DIES            = 3,    // All brawn, no brains. A recoverable loss.
    TALK_MOGU_DIES              = 4,    // He fought with honor... his first and last mistake.
    TALK_QON_AGGRO              = 5,    // Stare into the face of true power!
    TALK_CALL_DAMREN            = 6,    // Dam'ren! Freeze them to oblivion!
    TALK_CALL_QUETZAL           = 7,    // Quet'zal! Unleash the storm upon my enemies!
    TALK_CALL_ROSHAK            = 8,    // Ro'shak! Grant me wings of flame!
    TALK_ROSHAK_DIES            = 9,    // How can this be?!
    TALK_QUETZAL_DIES           = 10,   // Gaaaahh!
    TALK_DAMREN_DIES            = 11,   // USELESS!
    TALK_LAST_PHASE             = 12,   // ARGH! Worthless mongrels! I will kill you myself!
    TALK_QON_SLAY               = 13,   // I run you through! - Fall at my feet!
    TALK_QON_WIPE               = 14,   // What a pathetic display.
    TALK_QON_DEATH              = 15,   // More... training...
    TALK_QON_INTRO              = 16,   // Turn back now, strangers. You face the army of Iron Qon! - Enough of this! Come now, weaklings. You have earned your reward... you may now face the Iron Qon!
};

uint32 g_QonBossEntries[4] = { NPC_IRON_QON, NPC_RO_SHAK, NPC_QUET_ZAL, NPC_DAM_REN };

uint32 g_QuilenSpells[5] = { 0, SPELL_BURNING_CINDERS_AT, SPELL_STORM_CLOUD_AT, SPELL_FROZEN_BLOOD_AT, 0 }; // No AT spell for Iron Qon

uint32 g_DeadZonesSpells[6] =
{
    SPELL_DEAD_ZONE_FR,
    SPELL_DEAD_ZONE_LR,
    SPELL_DEAD_ZONE_FL,
    SPELL_DEAD_ZONE_BF,
    SPELL_DEAD_ZONE_BL,
    SPELL_DEAD_ZONE_BR
};

Position g_WindstormPoint = { 6045.468f, 4693.226f, 142.92f };

void RemoveEncounterAuras(InstanceScript* p_Instance)
{
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCORCHED);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IMPALE);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FROZEN);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHTNING_STORM);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARCING_LIGHTNING);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ARCING_PERIODIC_CHECK);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BURNING_CINDERS_DMG);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STORM_CLOUD_DMG);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FROZEN_BLOOD_DMG);
}

void EncounterDone(Creature* me)
{
    InstanceScript* l_Instance = me->GetInstanceScript();

    if (!l_Instance)
        return;

    bool l_AllDead = me->GetEntry() == NPC_IRON_QON ? true : false;

    if (l_Instance->instance->IsHeroic())
    {
        l_AllDead = true;
        for (uint8 l_Idx = 0; l_Idx < 4; ++l_Idx)
        {
            if (Creature* l_Boss = l_Instance->instance->GetCreature(l_Instance->GetData64(g_QonBossEntries[l_Idx])))
            {
                if (l_Boss->isAlive())
                    l_AllDead = false;
            }
        }
    }

    if (!l_AllDead)
        return;

    l_Instance->SetBossState(DATA_IRON_QON, DONE);
    RemoveEncounterAuras(l_Instance);

    me->RemoveAllAreasTrigger();

    uint32 l_AddEntries[6] =
    {
        NPC_RUSHING_WINDS_A,
        NPC_RUSHING_WINDS_B,
        NPC_STORM_CLOUD,
        NPC_IRON_QON_INVISIBLE_MAN,
        NPC_IRON_QON_SPEAR,
        NPC_SPEAR_LINE,
    };

    for (uint8 l_Idx = 0; l_Idx < 6; ++l_Idx)
    {
        std::list<Creature*> l_AddList;
        GetCreatureListWithEntryInGrid(l_AddList, me, l_AddEntries[l_Idx], 200.0f);
        for (Creature* l_Add : l_AddList)
            l_Add->DespawnOrUnsummon();
    }
}

// 68078 - Iron Qon
class boss_iron_qon : public CreatureScript
{
public:
    boss_iron_qon() : CreatureScript("boss_iron_qon") { }

    struct boss_iron_qonAI : public BossAI
    {
        boss_iron_qonAI(Creature* p_Creature) : BossAI(p_Creature, DATA_IRON_QON)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        EventMap m_Events;

        uint8 m_Phase;
        uint8 m_DeadQuilensCount;

        uint32 m_SpearIndex;
        uint32 m_SpearSpell;

        std::list<uint64> m_CycloneList;

        bool m_IntroDone;

        void Reset()
        {
            m_Events.Reset();
            m_Phase = 0;
            m_SpearSpell = 0;
            m_SpearIndex = 0;
            m_CycloneList.clear();

            me->SetReactState(REACT_DEFENSIVE);
            me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, EQUIP_IRON_QON);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

            RemoveEncounterAuras(m_Instance);

            if (m_Instance)
            {
                if (Creature* l_RoShak = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_RO_SHAK)))
                    me->CastSpell(l_RoShak, SPELL_RIDE_VEHICLE, false);

                m_Instance->SetBossState(DATA_IRON_QON, NOT_STARTED);
            }

            m_IntroDone = false;
        }

        void JustReachedHome()
        {
            Reset();
        }

        void JustDied(Unit* /*p_Killer*/)
        {
            if (!m_Instance)
                return;

            Talk(TALK_QON_DEATH);

            EncounterDone(me);
            _JustDied();

            // LFR Loot
            if (m_Instance->GetBossState(DATA_IRON_QON) == DONE)
            {
                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                }
            }
        }

        void EnterCombat(Unit* /*p_Attacker*/)
        {
            if (!m_Phase)
            {
                Talk(TALK_QON_AGGRO);
                m_Phase = 1;
                m_SpearSpell = SPELL_BURNING_CINDERS_BOLT;
            }

            me->ReenableEvadeMode();
            if (m_Instance)
                m_Instance->SetBossState(DATA_IRON_QON, IN_PROGRESS);

            if (!me->IsOnVehicle())
            {
                if (m_Instance)
                {
                    if (Creature* l_RoShak = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_RO_SHAK)))
                        me->CastSpell(l_RoShak, SPELL_RIDE_VEHICLE, false);
                }
            }

            m_Events.ScheduleEvent(EVENT_THROW_SPEAR, 15000, m_Phase);
            m_Events.ScheduleEvent(EVENT_IMPALE, 40000, m_Phase);
            m_Events.ScheduleEvent(EVENT_BERSERK, 720000, 8);
        }

        void EnterEvadeMode()
        {
            m_Events.Reset();
            summons.DespawnAll();
            me->AttackStop();

            me->RemoveAllAuras();
            me->RemoveAllDynObjects();
            me->RemoveAllAreasTrigger();

            me->SetReactState(REACT_PASSIVE);
            me->DisableEvadeMode();

            if (m_Instance)
            {
                m_Instance->SetBossState(DATA_IRON_QON, FAIL);
                m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                RemoveEncounterAuras(m_Instance);
            }

            uint32 l_AddEntries[4] = { NPC_INVISIBLE_MAN, NPC_RUSHING_WINDS_A, NPC_RUSHING_WINDS_B, NPC_STORM_CLOUD };
            for (uint8 l_Idx = 0; l_Idx < 4; ++l_Idx)
            {
                std::list<Creature*> l_AddList;
                GetCreatureListWithEntryInGrid(l_AddList, me, l_AddEntries[l_Idx], 200.0f);
                for (Creature* l_Add : l_AddList)
                    l_Add->DespawnOrUnsummon();
            }

            // Respawn quilens - Starting from 1 as 0 is for Iron Qon
            for (uint8 l_Idx = 1; l_Idx < 4; ++l_Idx)
            {
                if (Creature* l_Quilen = m_Instance->instance->GetCreature(m_Instance->GetData64(g_QonBossEntries[l_Idx])))
                    l_Quilen->AI()->EnterEvadeMode();
            }

            me->GetMotionMaster()->MoveTargetedHome();
        }

        void DoAction(int32 const p_Action)
        {
            switch (p_Action)
            {
                case ACTION_LAUNCH_MANCHU:
                {
                    Talk(TALK_LAUNCH_MANCHU);
                    break;
                }
                case ACTION_LAUNCH_WEISHENG:
                {
                    Talk(TALK_LAUNCH_WEISHENG);
                    break;
                }
                case ACTION_MOGU_DIES:
                {
                    Talk(TALK_MOGU_DIES);
                    break;
                }
                case ACTION_QUILEN_DIES:
                {
                    Talk(TALK_QUILEN_DIES);
                    break;
                }
                case ACTION_IRON_QON_TRASH_DONE:
                {
                    Talk(TALK_QON_INTRO);
                    break;
                }
                // Changing mount or entering in phase 4
                case ACTION_CHANGE_MOUNT:
                {
                    if (!m_Instance)
                        break;

                    // Can't change phase if spear is away and producing effects
                    if (!me->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS))
                    {
                        m_Events.ScheduleEvent(EVENT_CHANGE_MOUNT, 1000, m_Phase);
                        break;
                    }

                    me->SetReactState(REACT_PASSIVE);
                    // Just cancel events of the phase, but keep the enrage timer
                    m_Events.CancelEventGroup(m_Phase);

                    // Mounted on Quilens
                    if (++m_Phase < 4)
                    {
                        if (Creature* l_NewMount = m_Instance->instance->GetCreature(m_Instance->GetData64(g_QonBossEntries[m_Phase])))
                            l_NewMount->AI()->DoAction(ACTION_QUILEN_ACTIVATE);
                        m_SpearSpell = m_Phase == 2 ? SPELL_STORM_CLOUD_BOLT : SPELL_FROZEN_BLOOD_BOLT;
                    }
                    // Phase 4, not mounted anymore
                    else
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetReactState(REACT_AGGRESSIVE);
                        Talk(TALK_LAST_PHASE);
                        m_SpearSpell = 0;
                        me->CastSpell(me, SPELL_RISING_ANGER, true);
                        if (!IsHeroic())
                            m_Events.ScheduleEvent(EVENT_FIST_SMASH, 8000, m_Phase);

                        // Retreiving cyclone list
                        std::list<Creature*> l_CycloneList;
                        GetCreatureListWithEntryInGrid(l_CycloneList, me, NPC_RUSHING_WINDS_B, 200.0f);
                        // Reminding Guids
                        for (Creature* l_Cyclone : l_CycloneList)
                            m_CycloneList.push_back(l_Cyclone->GetGUID());
                        m_Events.ScheduleEvent(EVENT_MODIFY_CYCLONE, 20000);
                        m_Events.ScheduleEvent(EVENT_THROW_SPEAR, 6000);
                    }

                    break;
                }
                // Now on the new mount, can attack anew - phase 2 or 3
                case ACTION_ACTIVATE_PHASE:
                {
                    Talk(m_Phase == 2 ? TALK_CALL_QUETZAL : TALK_CALL_DAMREN);
                    std::list<Creature*> l_SpearList;
                    GetCreatureListWithEntryInGrid(l_SpearList, me, NPC_IRON_QON_SPEAR, 300.0f);
                    for (Creature* l_Spear : l_SpearList)
                        l_Spear->AI()->SetData(TYPE_UPDATE_PHASE, m_Phase);

                    m_Events.ScheduleEvent(EVENT_THROW_SPEAR, 10000, m_Phase);
                    break;
                }
                // Heroic mode: Fist smash is only available when 2 of the 3 Quilens are dead
                case ACTION_MOUNT_DIES:
                {
                    if (++m_DeadQuilensCount >= 2)
                        m_Events.ScheduleEvent(EVENT_FIST_SMASH, 8000, m_Phase);
                    break;
                }
                case ACTION_WINDSTORM_DELAY:
                {
                    // Delay only phase 3 events (and so not the berserk event)
                    m_Events.DelayEvents(25000, m_Phase);
                    break;
                }
                default:
                    break;
            }
        }

        uint32 GetData(uint32 p_Type)
        {
            return p_Type == TYPE_GET_PHASE ? m_Phase : p_Type == TYPE_GET_SPEAR_INDEX ? m_SpearIndex : 0;
        }

        Creature* GetActiveSpear()
        {
            std::list<Creature*> l_SpearList;
            GetCreatureListWithEntryInGrid(l_SpearList, me, NPC_IRON_QON_SPEAR, 200.0f);
            uint32 l_IndexMax = 0;
            Creature* l_AltSpear = nullptr;

            // Looking for active spear
            for (Creature* l_Spear : l_SpearList)
            {
                uint32 l_SpearIndex = l_Spear->AI()->GetData(TYPE_GET_SPEAR_INDEX);
                // We've found the last spear, let's use it
                if (l_SpearIndex == m_SpearIndex)
                    return l_Spear;

                // Else, we'll take the spear with max index as an alternative spear
                if (l_SpearIndex > l_IndexMax)
                {
                    l_IndexMax = l_SpearIndex;
                    l_AltSpear = l_Spear;
                }
            }

            // Not able to find the active spear : we return the spear which has the greatest index
            return l_AltSpear;
        }

        Creature* GetValidCyclone()
        {
            if (m_CycloneList.empty())
                return nullptr;

            std::list<uint64>::iterator itr = m_CycloneList.begin();
            Creature* l_ValidStorm = nullptr;

            // Looking for the first valid cyclone, removing invalid ones
            while (!l_ValidStorm && itr != m_CycloneList.end())
            {
                // Return if valid cyclone found
                if (l_ValidStorm = Creature::GetCreature(*me, *itr))
                   return l_ValidStorm;

                // No return: the cyclone wasn't valid, we remove it from the list and continue with the new first element of the list
                m_CycloneList.remove(*itr);
                itr = m_CycloneList.begin();
            }

            // Still no return at the end of the loop: no valid cyclone found (and now, m_CycloneList is empty)
            return nullptr;
        }

        // Cast spell on cyclone target, remove target from list, and return true if list isn't empty
        bool CastSpellOnCyclone(Creature* p_Target, uint32 p_Spell)
        {
            // No cast if target is empty or if target isn't a cyclone
            if (!p_Target || p_Target->GetEntry() != NPC_RUSHING_WINDS_B)
                return false;

            // Cast spell and remove the cyclone, which is the first of the list
            me->CastSpell(p_Target, p_Spell, false);
            m_CycloneList.remove(*m_CycloneList.begin());

            // If the list is empty, we return false, as there's no remaining cyclone to freeze or ignite
            return !m_CycloneList.empty();
        }

        void UpdateAI(uint32 const p_Diff)
        {
            if (m_Instance)
            {
                if (m_Instance->IsWipe() && m_Instance->GetBossState(DATA_IRON_QON) == IN_PROGRESS)
                {
                    EnterEvadeMode();
                    return;
                }
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            m_Events.Update(p_Diff);

            while (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                switch (l_EventId)
                {
                    // Generic events, used in all phases
                    case EVENT_IMPALE:
                    {
                        // If mounted, attacks the victim of the mounted quilen
                        if (m_Phase < 4)
                        {
                            if (Creature* l_Quilen = me->GetVehicleCreatureBase())
                            {
                                if (Unit* l_Target = l_Quilen->getVictim())
                                    me->CastSpell(l_Target, SPELL_IMPALE, false);
                            }
                        }
                        // Else, attacks the main target
                        else
                            DoCastVictim(SPELL_IMPALE, false);

                        m_Events.ScheduleEvent(EVENT_IMPALE, 20000, m_Phase);
                        break;
                    }
                    case EVENT_BERSERK:
                    {
                        me->CastSpell(me, SPELL_BERSERK, false);
                        for (uint8 l_Idx = 1; l_Idx < 4; ++l_Idx)
                        {
                            if (Creature* l_Quilen = m_Instance->instance->GetCreature(m_Instance->GetData64(g_QonBossEntries[l_Idx])))
                            {
                                if (l_Quilen->isAlive())
                                    me->CastSpell(l_Quilen, SPELL_BERSERK, false);
                            }
                        }
                        break;
                    }
                    case EVENT_THROW_SPEAR:
                    {
                        if (!m_Instance)
                            break;

                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 300.0f);

                        if (l_PlayerList.empty())
                            break;

                        JadeCore::RandomResizeList(l_PlayerList, 1);
                        if (Unit* l_Target = l_PlayerList.front())
                        {
                            ++m_SpearIndex;
                            me->CastSpell(l_Target, SPELL_THROW_QON_SPEAR, true);
                            me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, 0);
                            m_Events.ScheduleEvent(EVENT_ACTIVATE_SPEAR, 4000, m_Phase);
                        }

                        m_Events.ScheduleEvent(EVENT_THROW_SPEAR, 30000, m_Phase);
                        break;
                    }
                    // Phase 1 to 3 - On Quilen
                    case EVENT_ACTIVATE_SPEAR:
                    {
                        if (m_SpearSpell)
                        {
                            if (Creature* l_Spear = GetActiveSpear())
                                me->CastSpell(l_Spear, m_SpearSpell, false);
                        }
                        break;
                    }
                    case EVENT_CHANGE_MOUNT:
                    {
                        // Try to change phase
                        DoAction(ACTION_CHANGE_MOUNT);
                        break;
                    }
                    // Phase 4 - no more quilens
                    case EVENT_FIST_SMASH:
                    {
                        // Can only perform Fist Smash if not wearing the spear
                        if (!me->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS))
                        {
                            me->CastSpell(me, SPELL_FIST_SMASH, true);
                            m_Events.ScheduleEvent(EVENT_FIST_SMASH, 20000, m_Phase);
                        }
                        else
                            m_Events.ScheduleEvent(EVENT_FIST_SMASH, 1000);
                        break;
                    }
                    case EVENT_MODIFY_CYCLONE:
                    {
                        // Ignite Cyclone
                        if (!CastSpellOnCyclone(GetValidCyclone(), SPELL_IGNITE_CYCLONE))
                            return;

                        // Freeze Cyclone
                        if (!CastSpellOnCyclone(GetValidCyclone(), SPELL_FREEZE_CYCLONE))
                            return;

                        m_Events.ScheduleEvent(EVENT_MODIFY_CYCLONE, 20000);
                        break;
                    }
                    default:
                        break;
                }
            }
            if (m_Phase == 4)
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const
    {
        return new boss_iron_qonAI(p_Creature);
    }
};

// 68079 - Ro'Shak
class mob_roshak : public CreatureScript
{
    public:
        mob_roshak() : CreatureScript("mob_roshak") { }

        struct mob_roshakAI : public ScriptedAI
        {
            mob_roshakAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            bool m_IntroDone;
            uint32 m_PowerTimer;
            uint32 m_UnleashedCount;
            std::list<uint32> m_SpellList;

            void Reset()
            {
                m_UnleashedCount = 0;
                m_Events.Reset();
                me->SetDisableGravity(true);
                me->SetCanFly(true);
                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SetReactState(REACT_PASSIVE);
                m_IntroDone = false;
                me->setPowerType(POWER_MANA);
                me->SetMaxPower(POWER_MANA, 100);
                me->SetPower(POWER_MANA, 0);
                m_PowerTimer = 1000;
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                me->SetPower(POWER_MANA, 0);

                if (!m_Instance)
                    return;

                if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                {
                    l_IronQon->AI()->DoAction(IsHeroic() ? ACTION_MOUNT_DIES : ACTION_CHANGE_MOUNT);
                    l_IronQon->AI()->Talk(TALK_ROSHAK_DIES);
                }

                EncounterDone(me);
            }

            void EnterEvadeMode()
            {
                me->RemoveAllAuras();
                m_Events.Reset();
                summons.DespawnAll();
                me->Respawn(true);
                me->GetMotionMaster()->MoveTargetedHome();
                me->SetPower(POWER_MANA, 0);
            }

            bool TrashDone()
            {
                uint32 l_TrashEntries[4] = { NPC_UNTRAINED_QUILEN, NPC_UNTRAINED_QUILEN2, NPC_MANCHU, NPC_WEISHENG };
                for (uint8  l_Idx = 0; l_Idx < 4; ++l_Idx)
                {
                    if (GetClosestCreatureWithEntry(me, l_TrashEntries[l_Idx], 200.0f))
                        return false;
                }

                return true;
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damages, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!m_Instance || !TrashDone())
                {
                    p_Damages = 0;
                    if (m_Instance)
                    {
                        if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                            l_IronQon->AI()->EnterEvadeMode();
                    }
                    return;
                }

                EnterCombat(p_Attacker);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (!m_Instance || m_IntroDone)
                    return;

                if (m_Instance->GetBossState(DATA_IRON_QON) == IN_PROGRESS)
                    return;

                if (!TrashDone() || !m_Instance->CheckRequiredBosses(DATA_IRON_QON))
                {
                    if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                        l_IronQon->AI()->EnterEvadeMode();
                    return;
                }

                if (!p_Attacker)
                    Unit* p_Attacker = SelectTarget(SELECT_TARGET_TOPAGGRO);

                m_IntroDone = true;
                uint32 l_TabEntries[3] = { NPC_IRON_QON, NPC_QUET_ZAL, NPC_DAM_REN };
                for (uint8 l_Idx = 0; l_Idx < 3; ++l_Idx)
                {
                    if (Creature* l_Mob = m_Instance->instance->GetCreature(m_Instance->GetData64(l_TabEntries[l_Idx])))
                        l_Mob->AI()->EnterCombat(p_Attacker);
                }

                me->SetInCombatWith(p_Attacker);
                AttackStart(p_Attacker);
                me->Attack(p_Attacker, true);
                me->CastSpell(me, SPELL_ZERO_POWER_REGEN, false);

                m_Events.ScheduleEvent(EVENT_QUILEN_TALK, 1000);
                m_Events.ScheduleEvent(EVENT_LAND, 3000);
                m_Events.ScheduleEvent(EVENT_MOLTEN_INFERNO, 20000);
                m_Events.ScheduleEvent(EVENT_UNLEASHED_FLAME, 6000);
                m_PowerTimer = 1000;
            }

            uint32 GetData(uint32 /*id*/ /* = 0 */)
            {
                return m_UnleashedCount;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!m_IntroDone)
                    return;

                // Power
                if (!me->IsFlying() && !me->HasAura(SPELL_MOLTEN_OVERLOAD))
                {
                    if (m_PowerTimer < p_Diff)
                    {
                        me->ModifyPower(POWER_MANA, 5);
                        // If Ro'shak reaches full energy, it triggers Molten Overload
                        if (me->GetPower(POWER_MANA) >= me->GetMaxPower(POWER_MANA))
                        {
                            me->CastSpell(me, SPELL_MOLTEN_OVERLOAD, true);
                            me->SetPower(POWER_MANA, 0);
                        }
                        m_PowerTimer = 1000;
                    }
                    else
                        m_PowerTimer -= p_Diff;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING) || !m_Instance)
                    return;

                m_Events.Update(p_Diff);

                while (uint32 m_EventId = m_Events.ExecuteEvent())
                {
                    switch (m_EventId)
                    {
                        case EVENT_QUILEN_TALK:
                        {
                            if (m_Instance)
                            {
                                if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                                    l_IronQon->AI()->Talk(TALK_CALL_ROSHAK);
                            }
                            break;
                        }
                        case EVENT_LAND:
                        {
                            if (me->getVictim())
                            {
                                me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                                me->SetDisableGravity(false);
                                me->SetCanFly(false);
                            }
                            else
                                m_Events.ScheduleEvent(EVENT_LAND, 500);

                            break;
                        }
                        case EVENT_MOLTEN_INFERNO:
                        {
                            if (me->GetPower(POWER_MANA) >= 9)
                            {
                                DoCast(SPELL_MOLTEN_INFERNO);
                                m_Events.ScheduleEvent(EVENT_MOLTEN_INFERNO, urand(15000, 20000));
                            }
                            else
                                m_Events.ScheduleEvent(EVENT_MOLTEN_INFERNO, 1000);
                            break;
                        }
                        case EVENT_UNLEASHED_FLAME:
                        {
                            // Cast on largest group of at least 3 or 5 players
                            uint8 l_MaxGroup = Is25ManRaid() ? 5 : 3;
                            // Target to cast spell on
                            Player* l_PlayerInMaxGroup = 0;
                            // Size of the largest group found
                            m_UnleashedCount = 0;

                            // Get all players
                            Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();

                            // For each player, we get all the players around in 10.0f
                            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                            {
                                std::list<Player*> l_AroundPlayers;
                                l_Itr->getSource()->GetPlayerListInGrid(l_AroundPlayers, 10.0f);
                                // We keep the player and the size of his group if his group a) is big enough and b) is the largest found
                                if ((l_AroundPlayers.size() > m_UnleashedCount) && (l_AroundPlayers.size() >= l_MaxGroup))
                                {
                                    l_PlayerInMaxGroup = l_Itr->getSource();
                                    m_UnleashedCount = l_AroundPlayers.size();
                                }
                            }

                            // If we have a valid target, we cast the spell, which costs 30 of power
                            if (l_PlayerInMaxGroup)
                            {
                                me->CastSpell(l_PlayerInMaxGroup, SPELL_UNLEASHED_SHOT, false);
                                me->ModifyPower(POWER_MANA, -30);
                                m_Events.ScheduleEvent(EVENT_UNLEASHED_FLAME, 15000);
                            }
                            /// Retry to cast Unleashed flame if no target found now
                            else
                                m_Events.ScheduleEvent(EVENT_UNLEASHED_FLAME, 1000);
                            
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_roshakAI(p_Creature);
        }
};

// 68080 - Quet'zal
class mob_quetzal : public CreatureScript
{
    public:
        mob_quetzal() : CreatureScript("mob_quetzal") { }

        struct mob_quetzalAI : public ScriptedAI
        {
            mob_quetzalAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint64 m_PreWindstormTargetGuid;

            bool m_IsInCombat;

            void Reset()
            {
                m_Events.Reset();
                m_IsInCombat = false;
                m_PreWindstormTargetGuid = 0;
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (!m_Instance)
                    return;

                if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                {
                    l_IronQon->AI()->DoAction(IsHeroic() ? ACTION_MOUNT_DIES : ACTION_CHANGE_MOUNT);
                    l_IronQon->AI()->Talk(TALK_QUETZAL_DIES);
                }

                EncounterDone(me);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (m_IsInCombat)
                    return;

                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SetDisableGravity(true);
                me->SetCanFly(true);
                me->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 5.0f);
            }

            void EnterEvadeMode()
            {
                me->RemoveAllAuras();
                m_Events.Reset();
                summons.DespawnAll();
                me->Respawn(true);
                me->GetMotionMaster()->MoveTargetedHome();
                me->SetDisableGravity(false);
                me->SetCanFly(false);
                me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
            }

            void MovementInform(uint32 p_Type, uint32 p_Value)
            {
                if ((p_Type != POINT_MOTION_TYPE && p_Type != EFFECT_MOTION_TYPE))
                    return;

                // Reaching Iron Qon to be mounted and to enter in combat
                if (p_Value == 1)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetReactState(REACT_AGGRESSIVE);

                    me->SetDisableGravity(false);
                    me->SetCanFly(false);
                    me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);

                    if (m_Instance)
                    {
                        if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                        {
                            l_IronQon->CastSpell(me, SPELL_RIDE_VEHICLE, false);
                            l_IronQon->AI()->DoAction(ACTION_ACTIVATE_PHASE);
                        }
                    }

                    me->SetReactState(REACT_AGGRESSIVE);
                    m_IsInCombat = true;
                }
                // Windstorm
                else if (p_Value == 8)
                {
                    me->SummonCreature(NPC_IRON_QON_INVISIBLE_MAN, g_WindstormPoint);
                    m_Events.ScheduleEvent(EVENT_WINDSTORM_END, 20000);
                }
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_QUILEN_ACTIVATE:
                    {
                        if (!m_Instance)
                            return;

                        if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                        {
                            float l_Ori  = l_IronQon->GetOrientation() > M_PI ? l_IronQon->GetOrientation() - M_PI : l_IronQon->GetOrientation() + M_PI;
                            float l_PosX = l_IronQon->GetPositionX() + 3.0f * cos(l_Ori);
                            float l_PosY = l_IronQon->GetPositionY() + 3.0f * sin(l_Ori);

                            me->GetMotionMaster()->MovePoint(1, l_PosX, l_PosY, l_IronQon->GetPositionZ());
                            m_Events.ScheduleEvent(EVENT_ARCING_LIGHTNING, urand(10000, 20000));
                            m_Events.ScheduleEvent(EVENT_WINDSTORM, 45000);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                while (uint32 l_EventId = m_Events.ExecuteEvent())
                {
                    switch (l_EventId)
                    {
                        case EVENT_ARCING_LIGHTNING:
                        {
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                me->CastSpell(l_Target, SPELL_LIGHTNING_STORM, true);
                            m_Events.ScheduleEvent(EVENT_ARCING_LIGHTNING, 25000);
                            break;
                        }
                        case EVENT_WINDSTORM:
                        {
                            if (!m_Instance)
                                return;

                            Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON));
                            if (!l_IronQon)
                                return;

                            // If Iron Qon's has his spear way, we need to wait before he get it back
                            if (!l_IronQon->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS))
                            {
                                m_Events.ScheduleEvent(EVENT_WINDSTORM, 1000);
                                break;
                            }

                            // We can perform Windstorm : let's delay events for Quet'zal and Iron Qon. Windstorm lasts 20 secs + time to go the central point and make player jumps.
                            m_Events.DelayEvents(25000);
                            l_IronQon->AI()->DoAction(ACTION_WINDSTORM_DELAY);

                            // Reminding current target
                            if (Unit* l_Victim = me->getVictim())
                                m_PreWindstormTargetGuid = l_Victim->GetGUID();

                            // Going to central point. Next part in Movement Inform...
                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                            me->SetDisableGravity(true);
                            me->SetCanFly(true);
                            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                            me->GetMotionMaster()->MoveTakeoff(8, g_WindstormPoint.GetPositionX(), g_WindstormPoint.GetPositionY(), g_WindstormPoint.GetPositionZ() + 15.0f);

                            // Preparing next Windstorm
                            m_Events.ScheduleEvent(EVENT_WINDSTORM, 90000);
                            break;
                        }
                        case EVENT_WINDSTORM_END:
                        {
                            // Get a victim
                            Player* l_Victim = nullptr;

                            // Try to  get back the previous target
                            if (m_PreWindstormTargetGuid)
                                l_Victim = ObjectAccessor::GetPlayer(*me, m_PreWindstormTargetGuid);

                            // Unable to find the previous target: pick a random player around
                            if (!l_Victim)
                            {
                                std::list<Player*> l_PlayerList;
                                GetPlayerListInGrid(l_PlayerList, me, 300.0f);

                                if (!l_PlayerList.empty())
                                {
                                    JadeCore::RandomResizeList(l_PlayerList, 1);
                                    l_Victim = l_PlayerList.front();
                                }
                            }

                            // We have a target
                            if (l_Victim)
                            {
                                me->SetInCombatWith(l_Victim);
                                AttackStart(l_Victim);
                            }
                            // No target (shouldn't happen...)
                            else
                                me->GetMotionMaster()->MoveLand(0, g_WindstormPoint);

                            // Return in normal combat state
                            me->SetCanFly(false);
                            me->SetDisableGravity(false);
                            me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                            me->SetReactState(REACT_AGGRESSIVE);
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_quetzalAI(p_Creature);
        }
};

// 68081 - Dam'ren
class mob_damren : public CreatureScript
{
    public:
        mob_damren() : CreatureScript("mob_damren") { }

        struct mob_damrenAI : public ScriptedAI
        {
            mob_damrenAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            bool m_IsInCombat;

            void Reset()
            {
                m_Events.Reset();
                me->SetReactState(REACT_PASSIVE);
                m_IsInCombat = false;
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (m_IsInCombat)
                    return;

                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SetDisableGravity(true);
                me->SetCanFly(true);
                me->GetMotionMaster()->MoveTakeoff(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 5.0f);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (!m_Instance)
                    return;

                if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                {
                    l_IronQon->AI()->DoAction(IsHeroic() ? ACTION_MOUNT_DIES : ACTION_CHANGE_MOUNT);
                    l_IronQon->AI()->Talk(TALK_DAMREN_DIES);
                }

                EncounterDone(me);
            }

            bool CanBeAttacked(Unit* p_Attacker)
            {
                // Retreive active dead zone spell
                uint32 l_DeadZoneSpell = 0;
                for (uint8 l_Idx = 0; l_Idx < 6; ++l_Idx)
                {
                    if (me->HasAura(g_DeadZonesSpells[l_Idx]))
                        l_DeadZoneSpell = g_DeadZonesSpells[l_Idx];
                }

                // No Dead zone spell applied, then Dam'ren can be attacked
                if (!l_DeadZoneSpell)
                    return true;

                // Checking which zone is protected
                bool l_HasFrontZone = (l_DeadZoneSpell == SPELL_DEAD_ZONE_FR || l_DeadZoneSpell == SPELL_DEAD_ZONE_FL || l_DeadZoneSpell == SPELL_DEAD_ZONE_BF);
                bool l_HasBackZone  = (l_DeadZoneSpell == SPELL_DEAD_ZONE_BF || l_DeadZoneSpell == SPELL_DEAD_ZONE_BL || l_DeadZoneSpell == SPELL_DEAD_ZONE_BR);
                bool l_HasLeftZone  = (l_DeadZoneSpell == SPELL_DEAD_ZONE_BL || l_DeadZoneSpell == SPELL_DEAD_ZONE_FL || l_DeadZoneSpell == SPELL_DEAD_ZONE_LR);
                bool l_HasRightZone = (l_DeadZoneSpell == SPELL_DEAD_ZONE_BR || l_DeadZoneSpell == SPELL_DEAD_ZONE_FR || l_DeadZoneSpell == SPELL_DEAD_ZONE_LR);

                // For some reason, at start, Dam'ren has is well turned at orientation 1.42266, but me->GetOrientation() returns 0.0f, so isInFront returns possibly wrong values...
                float l_Angle = me->GetAngle(p_Attacker->GetPositionX(), p_Attacker->GetPositionY());

                bool l_AttackerInFront = (l_Angle > 0.0f)           && (l_Angle < M_PI);
                bool l_AttackerInBack  = (l_Angle > M_PI)           && (l_Angle < (2 * M_PI));
                bool l_AttackerOnLeft  = (l_Angle > (M_PI / 2))     && (l_Angle < (3 * M_PI / 2));
                bool l_AttackerOnRight = (l_Angle > (3 * M_PI / 2)) || (l_Angle < (M_PI / 2));

                // Check according activated zones
                if ((l_HasFrontZone && l_AttackerInFront) ||
                    (l_HasBackZone  && l_AttackerInBack)  ||
                    (l_HasLeftZone  && l_AttackerOnLeft)  ||
                    (l_HasRightZone && l_AttackerOnRight))
                    return false;

                return true;
            }

            void CheckHitResult(MeleeHitOutcome& p_MeleeResult, SpellMissInfo& p_SpellResult, Unit* p_Attacker)
            {
                if (!CanBeAttacked(p_Attacker))
                {
                    p_MeleeResult = MELEE_HIT_MISS;
                    p_SpellResult = SPELL_MISS_DEFLECT;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_Value)
            {
                if ((p_Type != POINT_MOTION_TYPE && p_Type != EFFECT_MOTION_TYPE) || p_Value != 1)
                    return;

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_AGGRESSIVE);

                me->SetDisableGravity(false);
                me->SetCanFly(false);
                me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);

                if (m_Instance)
                {
                    if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                    {
                        l_IronQon->CastSpell(me, SPELL_RIDE_VEHICLE, false);
                        l_IronQon->AI()->DoAction(ACTION_ACTIVATE_PHASE);
                    }
                }

                me->SetReactState(REACT_AGGRESSIVE);
                m_IsInCombat = true;

                m_Events.ScheduleEvent(EVENT_DEAD_ZONE, urand(7000, 17000));
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_QUILEN_ACTIVATE:
                    {
                        if (m_IsInCombat)
                            return;

                        m_IsInCombat = true;
                        me->AddAura(IsLFR() ? SPELL_FROZEN_RESILIENCE_LFR : SPELL_FROZEN_RESILIENCE, me);

                        if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                        {
                            float l_Ori = l_IronQon->GetOrientation() > M_PI ? l_IronQon->GetOrientation() - M_PI : l_IronQon->GetOrientation() + M_PI;
                            float l_PosX = l_IronQon->GetPositionX() + 3.0f * cos(l_Ori);
                            float l_PosY = l_IronQon->GetPositionY() + 3.0f * sin(l_Ori);

                            me->GetMotionMaster()->MovePoint(1, l_PosX, l_PosY, l_IronQon->GetPositionZ());
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void JustReachedHome()
            {
                me->SetCanFly(false);
                me->SetDisableGravity(false);
                me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
            }

            void EnterEvadeMode()
            {
                me->RemoveAllAuras();
                m_Events.Reset();
                summons.DespawnAll();
                me->Respawn(true);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                while (uint32 l_EventId = m_Events.ExecuteEvent())
                {
                    switch (l_EventId)
                    {
                        case EVENT_DEAD_ZONE:
                        {
                            // Could have 6 possibilities, covered by spells from 137226 to 137231
                            me->CastSpell(me, SPELL_DEAD_ZONE, true);
                            m_Events.ScheduleEvent(EVENT_DEAD_ZONE, 15000);
                            break;
                        }
                        case EVENT_FREEZE:
                        {
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(l_Target, SPELL_FROZEN, true);
                            m_Events.ScheduleEvent(EVENT_FREEZE, 15000);
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }

        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_damrenAI(p_Creature);
        }
};

// 68480 - Iron Qon's Spear
class mob_iron_qon_spear : public CreatureScript
{
    public:
        mob_iron_qon_spear() : CreatureScript("mob_iron_qon_spear") { }

        struct mob_iron_qon_spearAI : public ScriptedAI
        {
            mob_iron_qon_spearAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint8 m_Phase;
            uint32 m_Index; // Index in order to know which spear has been summoned in last position; spear summoned to jump back to Iron Qon have index remaining to 0.

            void Reset()
            {
                m_Events.Reset();
                me->AddAura(SPELL_SPEAR_VISUAL, me);
                me->AddAura(SPELL_INACTIVE_STUN, me);
                m_Phase = 0;
                m_Index = 0;
                if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                {
                    m_Phase = l_IronQon->AI()->GetData(TYPE_GET_PHASE);
                    m_Index = l_IronQon->AI()->GetData(TYPE_GET_SPEAR_INDEX);
                }

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                // If summoned by another spear, just need to jump on boss before disappearing
                if (p_Summoner->GetEntry() == NPC_IRON_QON_SPEAR)
                {
                    if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                    {
                        l_IronQon->CastSpell(me, SPELL_SPEAR_BACK, false);
                        me->CastSpell(l_IronQon, SPELL_SPEAR_JUMP, false);
                        m_Events.ScheduleEvent(EVENT_SPEAR_DISAPPEAR, 500);
                    }
                }
                else
                    m_Events.ScheduleEvent(EVENT_SPEAR_BACK, 10000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_LINING_SPEAR)
                {
                    me->CastSpell(me, g_QuilenSpells[m_Phase], false);
                    for (uint8 l_Idx = 0; l_Idx < 5; ++l_Idx)
                        me->SummonCreature(NPC_SPEAR_LINE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), frand(0.0f, 2 * M_PI), TEMPSUMMON_MANUAL_DESPAWN, 15000);
                }
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                if (p_Type == TYPE_UPDATE_PHASE)
                    m_Phase = p_Value;
            }

            uint32 GetData(uint32 p_Type)
            {
                return p_Type == TYPE_GET_SPEAR_INDEX ? m_Index : 0;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                uint32 m_EventId = m_Events.ExecuteEvent();

                if (m_EventId == EVENT_SPEAR_BACK)
                {
                    me->SummonCreature(NPC_IRON_QON_SPEAR, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                    me->RemoveAura(SPELL_SPEAR_VISUAL);
                }
                else if (m_EventId == EVENT_SPEAR_DISAPPEAR)
                    me->RemoveAura(SPELL_SPEAR_VISUAL);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_iron_qon_spearAI(p_Creature);
        }
};

// 69159 - Iron Qon's Spear
class mob_magma_line : public CreatureScript
{
    public:
        mob_magma_line() : CreatureScript("mob_magma_line") { }

        struct mob_magma_lineAI : public ScriptedAI
        {
            mob_magma_lineAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint64 m_SpearGuid;
            uint8 m_Phase;
            uint8 m_StepCount;

            void Reset()
            {
                m_Events.Reset();
                m_StepCount = 0;
                m_SpearGuid = 0;

                m_Phase = 0;
                if (Creature* l_IronQon = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_IRON_QON)))
                    m_Phase = l_IronQon->AI()->GetData(TYPE_GET_PHASE);

                me->SetSpeed(MOVE_WALK, 1.5f);
                me->SetSpeed(MOVE_RUN,  1.5f);

                me->DespawnOrUnsummon(60000);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_SpearGuid = p_Summoner->GetGUID();

                m_Events.ScheduleEvent(EVENT_LINE_SPEAR_STEP, 100);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_LINE_SPEAR_STEP)
                {
                    if (Creature* l_Spear = Creature::GetCreature(*me, m_SpearGuid))
                    {
                        l_Spear->CastSpell(me, g_QuilenSpells[m_Phase], false);

                        // Random change of orientation to trace not straight lines
                        if (!(m_StepCount % 3))
                        {
                            float l_NewOrientation = me->GetOrientation() + frand(-M_PI / 12, M_PI / 12);
                            me->SetFacingTo(l_NewOrientation);
                            me->SetOrientation(l_NewOrientation);

                            float l_NewX = me->GetPositionX() + 50 * cos(l_NewOrientation);
                            float l_NewY = me->GetPositionY() + 50 * sin(l_NewOrientation);
                            me->GetMotionMaster()->MovePoint(0, l_NewX, l_NewY, me->GetPositionZ());
                        }

                        // End after 15 AT spawned
                        if (++m_StepCount >= 15)
                        {
                            me->GetMotionMaster()->Clear();
                            l_Spear->DespawnOrUnsummon(45000); // Spell should last for 45 secs and despawn of l_Spear will interrupt it
                        }
                        else
                            m_Events.ScheduleEvent(EVENT_LINE_SPEAR_STEP, 300);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_magma_lineAI(p_Creature);
        }
};

// 70660 - Lightning Cloud
class mob_lightning_cloud : public CreatureScript
{
    public:
        mob_lightning_cloud() : CreatureScript("mob_lightning_cloud") { }

        struct mob_lightning_cloudAI : public ScriptedAI
        {
            mob_lightning_cloudAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            InstanceScript* m_Instance;
            uint64 m_TargetGuid;
            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_TargetGuid = p_Summoner->GetGUID();
                m_Events.ScheduleEvent(EVENT_CHECK_LIGHTED_PLAYER, 1000);
            }

            void OnSpellClick(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER || p_Who->HasAura(SPELL_LIGHTNING_STORM))
                    return;

                // Assuming several players can have the Arcing Lightning aura
                if (Player* l_Plr = Player::GetPlayer(*me, m_TargetGuid))
                {
                    // Free from main spell...
                    l_Plr->RemoveAura(SPELL_LIGHTNING_STORM);
                    // ... but now carry the debuff
                    me->AddAura(SPELL_ARCING_LIGHTNING, l_Plr);
                    me->AddAura(SPELL_ARCING_PERIODIC_CHECK, l_Plr);
                    // Storm cloud disappear
                    me->DespawnOrUnsummon();
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_CHECK_LIGHTED_PLAYER)
                {
                    Player* l_Player = ObjectAccessor::GetPlayer(*me, m_TargetGuid);
                    // Target doesn't exist anymore: find a new random player as target
                    if (!l_Player)
                    {
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);
                        if (l_PlayerList.empty())
                            return;

                        // Selecting only one player
                        JadeCore::RandomResizeList(l_PlayerList, 1);
                        l_Player = l_PlayerList.front();
                        me->CastSpell(l_Player, SPELL_LIGHTNING_STORM, false);
                        me->DespawnOrUnsummon();
                    }

                    m_Events.ScheduleEvent(EVENT_CHECK_LIGHTED_PLAYER, 1000);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_lightning_cloudAI(p_Creature);
        }
};

// 64693 - Invisible Man
class mob_iron_qon_invisible_man : public CreatureScript
{
    public:
        mob_iron_qon_invisible_man() : CreatureScript("mob_iron_qon_invisible_man") { }

        struct mob_iron_qon_invisible_manAI : public ScriptedAI
        {
            mob_iron_qon_invisible_manAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            std::list<uint64> m_StormList;

            void Reset()
            {
                m_StormList.clear();
                m_Events.Reset();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->DespawnOrUnsummon(20000);
                me->CastSpell(me, SPELL_WINDSTORM_AT, false);

                // Creating 14 tornadoes
                for (uint8 i = 0; i < 14; ++i)
                {
                    float l_Ori = i * M_PI / 7;
                    if (Creature* l_Storm = me->SummonCreature(NPC_RUSHING_WINDS_A, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), l_Ori))
                    {
                        l_Storm->AI()->SetData(TYPE_SET_STORM_INDEX, i);
                        m_StormList.push_back(l_Storm->GetGUID());
                    }
                }

                m_Events.ScheduleEvent(EVENT_WINDSTORM_LINGER, 19500);
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                // Make players jump to the room center
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 300.0f);
                for (Player* l_Player : l_PlayerList)
                    l_Player->CastSpell(me, SPELL_WINDSTORM_JUMP, true);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_WINDSTORM_LINGER)
                {
                    if (m_StormList.empty())
                        return;

                    // Selecting storms that will remain - Keeping only 4 storms
                    JadeCore::RandomResizeList(m_StormList, 4);
                    for (uint64 l_StormGuid : m_StormList)
                    {
                        if (Creature* l_Storm = Creature::GetCreature(*me, l_StormGuid))
                            l_Storm->AI()->DoAction(ACTION_WINDSTORM_LINGER);
                    }

                    me->DespawnOrUnsummon();
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_iron_qon_invisible_manAI(p_Creature);
        }
};

// 68852 - Rushing winds (A - during Windstorm)
class mob_rushing_winds : public CreatureScript
{
    public:
        mob_rushing_winds() : CreatureScript("mob_rushing_winds") { }

        struct mob_rushing_windsAI : public ScriptedAI
        {
            mob_rushing_windsAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                ASSERT(m_Vehicle);
                m_Instance = p_Creature->GetInstanceScript();
            }

            Vehicle* m_Vehicle;
            InstanceScript* m_Instance;
            EventMap m_Events;
            uint8 m_Index;

            void Reset()
            {
                m_Index = 0;
                m_Events.Reset();
                me->AddAura(SPELL_VISUAL_TORNADO, me);
                me->AddAura(SPELL_WINDSTORM_DAMAGE, me);
                me->AddAura(SPELL_RUSHING_WINDS, me);
                me->SetDisplayId(DISPLAYID_RUSHING_WINDS);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->DespawnOrUnsummon(20000);
                m_Events.ScheduleEvent(EVENT_CHECK_STORM_PLAYER, 500);
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                if (p_Type == TYPE_SET_STORM_INDEX)
                {
                    m_Index = p_Value;

                    float l_PosX = me->GetPositionX() + 2.0f * (m_Index + 1) * cos(me->GetOrientation());
                    float l_PosY = me->GetPositionY() + 2.0f * (m_Index + 1) * sin(me->GetOrientation());

                    me->GetMotionMaster()->MovePoint(1, l_PosX, l_PosY, me->GetPositionZ());
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE || p_Id != 1)
                    return;

                // spinning around the central point
                if (Creature* l_InvisibleMan = GetClosestCreatureWithEntry(me, NPC_IRON_QON_INVISIBLE_MAN, 100.0f))
                {
                    Movement::MoveSplineInit l_Init(me);
                    FillCirclePath(g_WindstormPoint, me->GetExactDist2d(g_WindstormPoint.m_positionX, g_WindstormPoint.m_positionY), me->GetPositionZ(), l_Init.Path(), m_Index % 2);
                    l_Init.SetWalk(true);
                    l_Init.SetCyclic();
                    l_Init.Launch();
                }
                else
                    me->DespawnOrUnsummon();
            }

            void FillCirclePath(Position const& p_CenterPos, float p_Radius, float p_PosZ, Movement::PointsArray& p_Path, bool p_Clockwise)
            {
                float l_Step  = p_Clockwise ? -M_PI / 8.0f : M_PI / 8.0f;
                float l_Angle = p_CenterPos.GetAngle(me->GetPositionX(), me->GetPositionY());

                for (uint8 i = 0; i < 16; l_Angle += l_Step, ++i)
                {
                    G3D::Vector3 l_Point;
                    l_Point.x = p_CenterPos.GetPositionX() + p_Radius * cosf(l_Angle);
                    l_Point.y = p_CenterPos.GetPositionY() + p_Radius * sinf(l_Angle);
                    l_Point.z = me->GetMap()->GetHeight(me->GetPhaseMask(), l_Point.x, l_Point.y, p_PosZ + 5.0f);
                    p_Path.push_back(l_Point);
                }
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_WINDSTORM_LINGER)
                {
                    me->SummonCreature(NPC_RUSHING_WINDS_B, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                    me->DespawnOrUnsummon();
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                uint32 l_EventId = m_Events.ExecuteEvent();
                if (l_EventId == EVENT_CHECK_STORM_PLAYER)
                {
                    if (Player* l_Player = me->SelectNearestPlayer(2.0f))
                    {
                        if (!l_Player->HasAura(SPELL_WINDSTORM_PERIODIC))
                        {
                            l_Player->CastSpell(me, SPELL_WINDSTORM_RIDE, true);
                            me->AddAura(SPELL_WINDSTORM_PERIODIC, l_Player);
                            m_Events.ScheduleEvent(EVENT_STORM_DAMAGE_PLAYER, 900);
                        }
                    }

                    m_Events.ScheduleEvent(EVENT_CHECK_STORM_PLAYER, 500);
                }
                else if (l_EventId == EVENT_STORM_DAMAGE_PLAYER)
                {
                    if (m_Vehicle)
                    {
                        if (Unit* l_Passenger = m_Vehicle->GetPassenger(0))
                        {
                            l_Passenger->CastSpell(me, SPELL_WINDSTORM_RIDE, true);
                            m_Events.ScheduleEvent(EVENT_STORM_DAMAGE_PLAYER, 900);
                        }
                    }
                }

            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_rushing_windsAI(p_Creature);
        }
};

// 69703 - Rushing winds (B - remaining tornado after Windstorm ends)
class mob_lingering_winds : public CreatureScript
{
    public:
        mob_lingering_winds() : CreatureScript("mob_lingering_winds") { }

        struct mob_lingering_windsAI : public ScriptedAI
        {
            mob_lingering_windsAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            Vehicle* m_Vehicle;

            void Reset()
            {
                m_Events.Reset();
                me->AddAura(SPELL_WINDSTORM_KNOCKBACK, me);
                me->AddAura(SPELL_WINDSTORM_DAMAGE, me);
                me->AddAura(SPELL_RUSHING_WINDS, me);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetWalk(true);
                me->SetDisplayId(DISPLAYID_RUSHING_WINDS);

                m_Events.ScheduleEvent(EVENT_WINDSTORM_MOVE, 100);
            }

            void Move(bool p_Turn = false)
            {

                // Check if we're too far and may pass through walls
                bool l_TooFar = me->GetExactDist2d(g_WindstormPoint.m_positionX, g_WindstormPoint.m_positionY) > 50.0f;

                // If so, we move back to the room center
                if (l_TooFar)
                {
                    me->GetMotionMaster()->MovePoint(0, g_WindstormPoint);
                    m_Events.ScheduleEvent(EVENT_WINDSTORM_MOVE, 5000);
                    return;
                }

                // Else, choose a random point in a random direction to move towards
                float l_Ori = me->GetOrientation();

                if (p_Turn)
                {
                    l_Ori += frand(-M_PI, M_PI);

                    me->SetOrientation(l_Ori);
                    me->SetFacingTo(l_Ori);
                }

                float l_PosX = me->GetPositionX() + 20.0f * cos(l_Ori);
                float l_PosY = me->GetPositionY() + 20.0f * sin(l_Ori);

                me->GetMotionMaster()->MovePoint(0, l_PosX, l_PosY, me->GetPositionZ());

                // Will change direction in 2-3 secs
                m_Events.ScheduleEvent(EVENT_WINDSTORM_MOVE, urand(2000, 3000));
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                uint32 l_EventId = m_Events.ExecuteEvent();
                // Move
                if (l_EventId == EVENT_WINDSTORM_MOVE)
                    Move(true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_lingering_windsAI(p_Creature);
        }
};

// 137714 - Return spear
class spell_return_spear : public SpellScriptLoader
{
    public:
        spell_return_spear() : SpellScriptLoader("spell_return_spear") { }

        class spell_return_spear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_return_spear_SpellScript);

            void Rearm()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Creature* l_IronQon = l_Caster->GetInstanceScript()->instance->GetCreature(l_Caster->GetInstanceScript()->GetData64(NPC_IRON_QON)))
                        l_IronQon->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, EQUIP_IRON_QON);

                    if (Creature* l_Spear = l_Caster->ToCreature())
                        l_Spear->DespawnOrUnsummon(500);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_return_spear_SpellScript::Rearm);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_return_spear_SpellScript();
        }
};

// 136330 - Burning Cinders
// 136419 - Storm Cloud
// 136449 - Frozen Blood
class spell_activate_spear : public SpellScriptLoader
{
    public:
        spell_activate_spear() : SpellScriptLoader("spell_activate_spear") { }

        class spell_activate_spear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_activate_spear_SpellScript);

            void Ignite()
            {
                if (Creature* l_Spear = GetHitCreature())
                    l_Spear->AI()->DoAction(ACTION_LINING_SPEAR);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_activate_spear_SpellScript::Ignite);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_activate_spear_SpellScript();
        }
};

// 136192 - Ligthning Storm
class spell_lightning_storm : public SpellScriptLoader
{
    public:
        spell_lightning_storm() : SpellScriptLoader("spell_lightning_storm") { }

        class spell_lightning_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_storm_SpellScript);

            void AddingAura()
            {
                Unit* l_Caster = GetCaster();
                Player* l_Target = GetHitPlayer();

                if (!l_Caster || !l_Target)
                    return;

                l_Caster->AddAura(SPELL_ARCING_PERIODIC_CHECK, l_Target);
                l_Target->CastSpell(l_Target, SPELL_SUMMON_STORM, false);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_lightning_storm_SpellScript::AddingAura);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lightning_storm_SpellScript();
        }
};

// 136193 - Arcing Lightning - Periodic
class spell_arcing_lightning_dmg : public SpellScriptLoader
{
    public:
        spell_arcing_lightning_dmg() : SpellScriptLoader("spell_arcing_lightning_dmg") { }

        class spell_arcing_lightning_dmg_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arcing_lightning_dmg_AuraScript);

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (WorldObject* l_Owner = GetOwner())
                {
                    if (Unit* l_Unit = GetOwner()->ToUnit())
                    {
                        if (l_Unit->GetMapId() != 1098)
                        {
                            PreventDefaultAction();
                            l_Unit->RemoveAura(SPELL_ARCING_LIGHTNING);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_arcing_lightning_dmg_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arcing_lightning_dmg_AuraScript();
        }
};

// 138434 - Arcing Lightning - Main Aura
class spell_arcing_lightning_main : public SpellScriptLoader
{
    public:
        spell_arcing_lightning_main() : SpellScriptLoader("spell_arcing_lightning_main") { }

        class spell_arcing_lightning_main_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arcing_lightning_main_AuraScript);

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (WorldObject* l_Owner = GetOwner())
                {
                    Unit* l_Unit = l_Owner->ToUnit();
                    if (l_Unit && l_Owner->GetMapId() != 1098)
                    {
                        l_Unit->RemoveAura(SPELL_ARCING_LIGHTNING);
                        l_Unit->RemoveAura(SPELL_ARCING_PLAYER_CHECK);
                        l_Unit->RemoveAura(SPELL_ARCING_PERIODIC_CHECK);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_arcing_lightning_main_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arcing_lightning_main_AuraScript();
        }
};

// 138435 - Arcing Lightning
class spell_arcing_lightning : public SpellScriptLoader
{
    public:
        spell_arcing_lightning() : SpellScriptLoader("spell_arcing_lightning") { }

        class spell_arcing_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_arcing_lightning_SpellScript);

            void NewTargets()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                // Prevent players to have the aura outside of Throne of Thunder
                if (l_Caster->GetMapId() != 1098)
                {
                    l_Caster->RemoveAura(SPELL_ARCING_LIGHTNING);
                    l_Caster->RemoveAura(SPELL_ARCING_PERIODIC_CHECK);
                    return;
                }

                // Get nearby players to "infect"
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, l_Caster, 20.0f);

                if (!l_PlayerList.empty())
                {
                    for (Player* l_Plr : l_PlayerList)
                    {
                        if (!l_Plr->HasAura(SPELL_ARCING_LIGHTNING) && l_Plr != l_Caster)
                        {
                            l_Caster->CastSpell(l_Plr, SPELL_ARCING_LIGHTNING, false);  // Lightning to new target
                            l_Caster->AddAura(SPELL_ARCING_LIGHTNING, l_Plr);           // Adding electric aura
                            l_Caster->AddAura(SPELL_ARCING_PERIODIC_CHECK, l_Plr);      // Adding checking aura
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_arcing_lightning_SpellScript::NewTargets);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_arcing_lightning_SpellScript();
        }
};

// 135145 - Frozen
class spell_frozen : public SpellScriptLoader
{
    public:
        spell_frozen() : SpellScriptLoader("spell_frozen") { }

        class spell_frozen_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_frozen_AuraScript);

            void Duration(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                SetDuration(urand(1, 5) * 1000);
            }

            void Shatter(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    if (Creature* l_IronQon = l_Target->GetInstanceScript()->instance->GetCreature(l_Target->GetInstanceScript()->GetData64(NPC_IRON_QON)))
                        l_IronQon->CastSpell(l_Target, SPELL_SHATTER, false);
            }

            void Register()
            {
                OnEffectApply  += AuraEffectApplyFn(spell_frozen_AuraScript::Duration, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_frozen_AuraScript::Shatter, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_frozen_AuraScript();
        }
};

// 137232 - Dead Zones
class spell_dead_zones : public SpellScriptLoader
{
    public:
        spell_dead_zones() : SpellScriptLoader("spell_dead_zones") { }

        class spell_dead_zones_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dead_zones_SpellScript);

            void RandomZones()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_DEAD_ZONE_FR + urand(0, 5), true);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_dead_zones_SpellScript::RandomZones);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dead_zones_SpellScript();
        }
};

/// 134628
class spell_unleashed_flame : public SpellScriptLoader
{
    public:
        spell_unleashed_flame() : SpellScriptLoader("spell_unleashed_flame") { }

        class spell_unleashed_flame_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_unleashed_flame_SpellScript);

            std::list<uint64> l_TargetList;

            void HandleHit()
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (l_Caster->GetEntry() == NPC_RO_SHAK)
                {
                    uint32 l_NumTarget = l_Caster->GetAI()->GetData(0);
                    l_NumTarget = l_NumTarget ? l_NumTarget : 1;    ///< Prevent l_NumTarget to cause a division by 0.
                    SetHitDamage(GetHitDamage() / l_NumTarget);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_unleashed_flame_SpellScript::HandleHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_unleashed_flame_SpellScript();
        }
};

/// Created by spells
/// 134758 - Burning Cinders
/// 136421 - Storm Cloud
/// 136451 - Frozen Blood
class at_quilen_spear : public AreaTriggerEntityScript
{
    public:
        at_quilen_spear() : AreaTriggerEntityScript("at_quilen_spear") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 3.5f;

            uint32 l_SpellId = p_AreaTrigger->GetSpellId();

            uint32 l_AuraTrigger = l_SpellId == SPELL_BURNING_CINDERS_AT ? SPELL_BURNING_CINDERS_AURA : l_SpellId == SPELL_STORM_CLOUD_AT ? SPELL_STORM_CLOUD_AURA : SPELL_FROZEN_BLOOD_AURA;
            uint32 l_AuraDamage  = l_SpellId == SPELL_BURNING_CINDERS_AT ? SPELL_BURNING_CINDERS_DMG  : l_SpellId == SPELL_STORM_CLOUD_AT ? SPELL_STORM_CLOUD_DMG  : SPELL_FROZEN_BLOOD_DMG;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit->ToPlayer())
                    continue;

                if (l_Unit->GetExactDist2d(p_AreaTrigger) <= l_Radius && !l_Unit->HasAura(l_AuraTrigger) && !l_Unit->HasAura(l_AuraDamage))
                    l_Caster->AddAura(l_AuraTrigger, l_Unit);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_quilen_spear();
        }
};

/// Created by spell 136574
class at_rushing_winds : public AreaTriggerEntityScript
{
    public:
        at_rushing_winds() : AreaTriggerEntityScript("at_rushing_winds") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 50.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger  , l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (!l_Unit->ToPlayer())
                    continue;

                if (l_Unit->GetExactDist2d(p_AreaTrigger) > (l_Radius - 10.0f) && l_Unit->HasAura(SPELL_RUSHING_WINDS_AURA))
                    l_Unit->RemoveAura(SPELL_RUSHING_WINDS_AURA);

                if (l_Unit->GetExactDist2d(p_AreaTrigger) <= (l_Radius - 10.0f) && !l_Unit->HasAura(SPELL_RUSHING_WINDS_AURA))
                    l_Caster->AddAura(SPELL_RUSHING_WINDS_AURA, l_Unit);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_rushing_winds();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_iron_qon()
{
    new boss_iron_qon();                ///< 68078
    new mob_roshak();                   ///< 68079
    new mob_quetzal();                  ///< 68080
    new mob_damren();                   ///< 68081
    new mob_iron_qon_spear();           ///< 68480 - Iron Qon's Spear
    new mob_magma_line();               ///< 69159 - Iron Qon's Spear
    new mob_lightning_cloud();          ///< 70660
    new mob_iron_qon_invisible_man();   ///< 64693
    new mob_rushing_winds();            ///< 68852
    new mob_lingering_winds();          ///< 69703
    new spell_return_spear();           ///< 137714
    new spell_activate_spear();         ///< 136330 (Burning Cinders) - 136419 (Storm Cloud) - 136449 (Frozen Blood)
    new spell_lightning_storm();        ///< 136192
    new spell_arcing_lightning_dmg();   ///< 136193
    new spell_arcing_lightning_main();  ///< 138434
    new spell_arcing_lightning();       ///< 138435
    new spell_frozen();                 ///< 135145
    new spell_dead_zones();             ///< 137232
    new spell_unleashed_flame();        ///< 134628
    new at_quilen_spear();              ///< 134758 - 136421 - 136451 
    new at_rushing_winds();             ///< 136574
}
#endif