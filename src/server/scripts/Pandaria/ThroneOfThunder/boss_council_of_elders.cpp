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
#include "ThreatManager.h"

enum eSpells
{
    // Gara'Jal's Soul
    SPELL_LINGERING_PRESENCE             = 136467,
    SPELL_POSSESSED                      = 136442,
    // Frost King Malakk
    SPELL_FRIGID_ASSAULT                 = 136904,
    SPELL_FRIGID_ASSAULT_STUN            = 136910,
    SPELL_BITING_COLD                    = 136917,
    SPELL_BITING_COLD_AURA               = 136992,
    SPELL_FROSTBITE                      = 136990,
    SPELL_FROSTBITE_PERIODIC             = 136922,
    SPELL_FROSTBITE_AURA                 = 137575,
    // Kaz'Ra Jin
    SPELL_RECKLESS_CHARGE_AREATRIGGER    = 138026,
    SPELL_RECKLESS_CHARGE_ROLLING        = 137117, // aura
    SPELL_RECKLESS_CHARGE_MOVEMENT       = 137131,
    SPELL_RECKLESS_CHARGE_DAMAGE         = 137133,
    SPELL_RECKLESS_CHARGE_KNOCK_BACK     = 137122,
    SPELL_OVERLOAD                       = 137149,
    SPELL_GENERIC_STUN                   = 135781,
    // Sul
    SPELL_SAND_BOLT                      = 136189,
    SPELL_QUICKSAND                      = 136521,
    SPELL_SAND_PERIODIC_DMG              = 136860,
    SPELL_SAND_VISUAL                    = 136851,
    SPELL_ENSNARED                       = 136878,
    SPELL_ENTRAPPED                      = 136857,
    SPELL_SANDSTORM                      = 136894,
    SPELL_SANDSTORM_DUMMY                = 136895,
    SPELL_FORTIFIED                      = 136864,
    // High Priestress Mar'Li
    SPELL_WRATH_OF_THE_LOA_BLESSED       = 137344,
    SPELL_WRATH_OF_THE_LOA_SHADOW        = 137347,
    SPELL_BLESSED_LOA_SPIRIT_SUMMON      = 137200,
    SPELL_SHADOWED_LOA_SPIRIT_SUMMONED   = 137351,
    SPELL_SHADOWED_GIFT                  = 137407,
    SPELL_MARKED_SOUL                    = 137359,
    SPELL_DARK_POWER                     = 136507,
    // Heroic Mode
    SPELL_SOUL_FRAGMENT                  = 137641,
    SPELL_SHADOWED_SOUL                  = 137650,
    SPELL_SOUL_FRAGMENT_SWITCH           = 137643,
    SPELL_DISCHARGE                      = 137166,
    SPELL_BODY_HEAT                      = 137084,
    SPELL_CHILLED_TO_THE_BONE            = 137085,
    SPELL_TREACHEROUS_GROUND             = 137614,
    SPELL_TREACHEROUS_GROUND_RESIZE      = 137629,
    SPELL_TWISTED_FATE_PRINCIPAL         = 137891,
    SPELL_TWISTED_FATE_SECOND_PRINCIPAL  = 137962,
    SPELL_TWISTED_FATE_LINK_VISUAL       = 137967,
    SPELL_TWISTED_FATE_PERIODIC          = 137986,
    SPELL_SHADOW_VISUAL                  = 32395,
    SPELL_BLESSED_TRANSFORMATION_LIGHT   = 140799,
    SPELL_SHADOWED_TRANSFORMATION        = 137271,
    SPELL_BLESSED_TRANSFORMATION         = 137198
};

enum eEvents
{
    EVENT_RECKLESS_CHARGE                        = 1,
    EVENT_RECKLESS_CHARGE_AREATRIGGER            = 2,
    EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG        = 3,
    EVENT_FRIGID_ASSAULT                         = 4,
    EVENT_LINGERING_PRESENCE_MALAKK              = 5,
    EVENT_LINGERING_PRESENCE_KAZRA_JIN           = 6,
    EVENT_LINGERING_PRESENCE_HIGH_PRIESTRESS     = 7,
    EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER = 8,
    EVENT_BITING_COLD                            = 9,
    EVENT_FROSTBITE                              = 10,
    EVENT_SAND_BOLT                              = 11,
    EVENT_QUICKSAND                              = 12,
    EVENT_SANDSTORM                              = 13,
    EVENT_WRATH_OF_THE_LOA_BLESSED               = 14,
    EVENT_WRATH_OF_THE_LOA_SHADOW                = 15,
    EVENT_BLESSED_LOA_SPIRIT_SUMMON              = 16,
    EVENT_HEAL_WEAKER_TROLL                      = 17,
    EVENT_SHADOWED_LOA_SPIRIT_SUMMON             = 18,
    EVENT_OS_PLAYER                              = 19,
    EVENT_DARK_POWER                             = 20,
    EVENT_SOUL_FRAGMENT                          = 21,
    EVENT_TWISTED_FATE                           = 22,
    EVENT_TWISTED_FATE_SECOND                    = 23,
    EVENT_ENSNARED                               = 24
};

enum eActions
{
    ACTION_SCHEDULE_AT_DMG                  = 1,
    ACTION_SCHEDULE_FRIGID_ASSAULT          = 2,
    ACTION_SCHEDULE_POSSESSION              = 3,
    ACTION_SCHEDULE_FROSTBITE               = 4,
    ACTION_SCHEDULE_SANDSTROM               = 5,
    ACTION_SANDSTORM                        = 6,
    ACTION_SCHEDULE_SHADOWED_SPIRIT_SPAWN   = 8,
    ACTION_SOUL_FRAGMENT                    = 9,
    ACTION_TREACHEROUS_GROUND               = 10,
    ACTION_GARAJAL_TALK_1                   = 11,
    ACTION_TALK_AGGRO_KAZRA_JIN             = 12,
    ACTION_TALK_AGGRO_SUL                   = 13,
    ACTION_TALK_AGGRO_MAR_LI                = 14,
    ACTION_TALK_AGGRO_MALAKK                = 15
};

enum eCouncilGuid
{
    GUID_RECKLESS_TARGET
};

enum eDatas
{
    DATA_LINGERING_PRESENCE_COEF,
    DATA_SPELL_DMG_MULTIPLIER
};

enum eMiscs
{
    ToTFirstPart    = 610,
    LastDoor        = 218469
};

enum eCouncilEquip
{
    EQUIP_SUL       = 93238,
    EQUIP_MARLI     = 94313,
    EQUIP_MALAKK    = 93758
};

uint32 g_CouncilBossEntries[4] = { NPC_FROST_KING_MALAKK, NPC_KAZRA_JIN, NPC_SUL_THE_SANDCRAWLER, NPC_HIGH_PRIESTRESS_MAR_LI };
// uint32 g_Possessed[4] = {0, 0, 0, 0};

// Removing all persistent auras in combat end
void RemoveAllEncounterAuras(InstanceScript* p_Instance)
{
    if (p_Instance)
    {
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SHADOWED_SOUL);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SOUL_FRAGMENT);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FROSTBITE_PERIODIC);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CHILLED_TO_THE_BONE);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARKED_SOUL);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ENTRAPPED);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SAND_PERIODIC_DMG);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BITING_COLD_AURA);
    }
}

void WipeOnFight(InstanceScript* p_Instance, Creature* me)
{
    if (!p_Instance)
        return;

    RemoveAllEncounterAuras(p_Instance);
    p_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, FAIL);

    std::list<Creature*> l_LivingSandList;
    GetCreatureListWithEntryInGrid(l_LivingSandList, me, NPC_LIVING_SAND, 200.0f);

    for (Creature* l_LivingSand : l_LivingSandList)
        l_LivingSand->DespawnOrUnsummon();

    if (Creature* l_GaraJal = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_GARA_JAL_SOUL)))
        l_GaraJal->AI()->Reset();

    for (uint8 l_Idx = 0; l_Idx < 4; ++l_Idx)
    {
        if (Creature* l_Boss = Creature::GetCreature(*me, p_Instance->GetData64(g_CouncilBossEntries[l_Idx])))
        {
            if (!l_Boss->isAlive())
                l_Boss->Respawn(true);

            p_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, l_Boss);

            l_Boss->SetFullHealth();
            l_Boss->RemoveAllAuras();
            l_Boss->GetMotionMaster()->MoveTargetedHome();

            if (l_Boss != me)
                l_Boss->AI()->EnterEvadeMode();
        }
    }
}

void StartFight(InstanceScript* p_Instance, Creature* me, Unit* /*target*/)
{
    if (!p_Instance)
        return;

    if (!p_Instance->CheckRequiredBosses(DATA_COUNCIL_OF_ELDERS))
    {
        if (me->GetAI())
            me->AI()->EnterEvadeMode();

        return;
    }

    if (p_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == IN_PROGRESS)
        return; // Prevent recursive calls

    p_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, IN_PROGRESS);
    p_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

    uint32 mobEntries[4] = {NPC_KAZRA_JIN, NPC_SUL_THE_SANDCRAWLER, NPC_HIGH_PRIESTRESS_MAR_LI, NPC_FROST_KING_MALAKK};

    for (int i = 0; i < 4; i++)
    {
        uint32 entry = mobEntries[i];
        if (Creature* boss = p_Instance->instance->GetCreature(p_Instance->GetData64(entry)))
        {
            boss->SetInCombatWithZone();

            switch (entry)
            {
                case NPC_KAZRA_JIN:
                    if (boss->GetAI())
                        boss->AI()->DoAction(ACTION_TALK_AGGRO_KAZRA_JIN);
                    break;
                case NPC_SUL_THE_SANDCRAWLER:
                    if (boss->GetAI())
                        boss->AI()->DoAction(ACTION_TALK_AGGRO_SUL);
                    break;
                case NPC_HIGH_PRIESTRESS_MAR_LI:
                    if (boss->GetAI())
                        boss->AI()->DoAction(ACTION_TALK_AGGRO_MAR_LI);
                    break;
                case NPC_FROST_KING_MALAKK:
                    if (boss->GetAI())
                        boss->AI()->DoAction(ACTION_TALK_AGGRO_MALAKK);
                    break;
                default:
                    break;
            }
        }
    }

    if (Creature* garaJalSoul = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_GARA_JAL_SOUL)))
    {
        if (garaJalSoul->GetAI())
            garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
    }


    if (Creature* garaJal = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_GARA_JAL)))
    {
        if (garaJal->GetAI())
            garaJal->AI()->Talk(0);
            // garaJal->AI()->DoAction(ACTION_GARAJAL_TALK_1);
    }
}

bool isAlonePossessed(InstanceScript* p_Instance)
{
    uint32 bossEntries[4] = {NPC_FROST_KING_MALAKK, NPC_HIGH_PRIESTRESS_MAR_LI, NPC_SUL_THE_SANDCRAWLER, NPC_KAZRA_JIN};
    for (int i = 0; i < 4; i++)
    {
        uint32 entry = bossEntries[i];
        if (Creature* boss = p_Instance->instance->GetCreature(p_Instance->GetData64(entry)))
        {
            if (boss->HasAura(SPELL_POSSESSED))
                return false;
        }
    }

    return true;
}

bool AllBossesDead(Creature* me)
{
    for (uint8 l_I = 0; l_I < 4; l_I++)
    {
        if (g_CouncilBossEntries[l_I] != me->GetEntry())
        {
            if (GetClosestCreatureWithEntry(me, g_CouncilBossEntries[l_I], 200.0f, true))
                return false;
            else
                continue;
        }
    }

    // All bosses done - removing auras
    if (InstanceScript* l_Instance = me->GetInstanceScript())
        RemoveAllEncounterAuras(l_Instance);

    return true;
}

// Gara'Jal's Soul - 69182
class npc_gara_jal_s_soul : public CreatureScript
{
    public:
        npc_gara_jal_s_soul() : CreatureScript("npc_gara_jal_s_soul") { }

        struct npc_gara_jal_s_soulAI : public ScriptedAI
        {
            npc_gara_jal_s_soulAI(Creature* creature) : ScriptedAI(creature)
            {
                m_Instance = creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            uint32 m_TargetFaction;
            bool m_FirstEmpowerment;

            void Reset()
            {
                m_TargetFaction = 0;
                m_Events.Reset();
                m_FirstEmpowerment = true;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_SCHEDULE_POSSESSION)
                {
                    /// First empowered troll is always Malakk
                    uint32 l_EntryToPossess = NPC_FROST_KING_MALAKK;
                    if (m_FirstEmpowerment)
                        m_FirstEmpowerment = false;
                    else
                    {
                        std::list<Creature*> l_BossList;
                        for (uint8 l_Idx = 0; l_Idx < 4; ++l_Idx)
                        {
                            if (Creature* l_Boss = m_Instance->instance->GetCreature(m_Instance->GetData64(g_CouncilBossEntries[l_Idx])))
                            {
                                if (l_Boss->isAlive() && !l_Boss->HasAura(SPELL_POSSESSED))
                                    l_BossList.push_back(l_Boss);
                            }
                        }

                        l_BossList.sort(JadeCore::HealthPctOrderPred(false));
                        l_EntryToPossess = l_BossList.front()->GetEntry();
                    }

                    switch (l_EntryToPossess)
                    {
                        case NPC_KAZRA_JIN:
                            m_Events.ScheduleEvent(EVENT_LINGERING_PRESENCE_KAZRA_JIN, 1000);
                            break;
                        case NPC_FROST_KING_MALAKK:
                            m_Events.ScheduleEvent(EVENT_LINGERING_PRESENCE_MALAKK, 1000);
                            break;
                        case NPC_HIGH_PRIESTRESS_MAR_LI:
                            m_Events.ScheduleEvent(EVENT_LINGERING_PRESENCE_HIGH_PRIESTRESS, 1000);
                            break;
                        case NPC_SUL_THE_SANDCRAWLER:
                            m_Events.ScheduleEvent(EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER, 1000);
                            break;
                        default:
                            break;
                    }
                }
                else if (action == ACTION_SOUL_FRAGMENT)
                {
                    if (IsHeroic())
                        m_Events.ScheduleEvent(EVENT_SOUL_FRAGMENT, 1000);
                }
            }

            uint32 GetData(uint32 /*p_Type*/)
            {
                return m_TargetFaction;
            }

            void UpdateAI(const uint32 diff)
            {
                m_Events.Update(diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_LINGERING_PRESENCE_MALAKK:
                        if (Creature* malakk = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_FROST_KING_MALAKK)))
                        {
                            if (isAlonePossessed(malakk->GetInstanceScript()))
                                me->AddAura(SPELL_POSSESSED, malakk);

                            if (malakk->GetAI())
                            {
                                malakk->AI()->DoAction(ACTION_SCHEDULE_FROSTBITE);
                                malakk->AI()->Talk(urand(2, 3));
                                malakk->AI()->Talk(1);
                            }
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_KAZRA_JIN:
                        if (Creature* kazraJin = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_KAZRA_JIN)))
                        {
                            if (isAlonePossessed(kazraJin->GetInstanceScript()) && kazraJin->GetAI())
                            {
                                kazraJin->AI()->Talk(1);
                                me->AddAura(SPELL_POSSESSED, kazraJin);
                            }
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_HIGH_PRIESTRESS:
                        if (Creature* priestress = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_HIGH_PRIESTRESS_MAR_LI)))
                        {
                            if (isAlonePossessed(priestress->GetInstanceScript()))
                                me->AddAura(SPELL_POSSESSED, priestress);

                            if (priestress->GetAI())
                            {
                                priestress->AI()->Talk(1);
                                priestress->AI()->DoAction(ACTION_SCHEDULE_SHADOWED_SPIRIT_SPAWN);
                            }
                        }
                        break;
                    case EVENT_LINGERING_PRESENCE_SUL_THE_SANDCRAWLER:
                        if (Creature* sul = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                        {
                            if (isAlonePossessed(sul->GetInstanceScript()))
                                me->AddAura(SPELL_POSSESSED, sul);

                            if (sul->GetAI())
                            {
                                sul->AI()->DoAction(ACTION_SCHEDULE_SANDSTROM);
                                sul->AI()->Talk(1);
                            }
                        }
                        break;
                    case EVENT_SOUL_FRAGMENT:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 200.0f);

                        if (!playerList.empty())
                        {
                            JadeCore::RandomResizeList(playerList, 1);

                            if (Player* l_Player = playerList.front())
                            {
                                m_TargetFaction = l_Player->getFaction();
                                l_Player->setFaction(35);
                                l_Player->CastSpell(l_Player, SPELL_SOUL_FRAGMENT, false);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_gara_jal_s_soulAI(creature);
        }
};

// Frost King Malakk - 69131
class boss_king_malakk : public CreatureScript
{
    public:
        boss_king_malakk() : CreatureScript("boss_king_malakk") { }

        struct boss_king_malakkAI : public BossAI
        {
            boss_king_malakkAI(Creature* creature) : BossAI(creature, DATA_COUNCIL_OF_ELDERS)
            {
                m_Instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* m_Instance;
            bool firstPossessSwitched;
            bool secondPossessSwitched;
            uint32 coefficient;
            uint32 counter;

            void Reset()
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, NOT_STARTED);

                    RemoveAllEncounterAuras(m_Instance);
                }

                me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, EQUIP_MALAKK);

                coefficient   = 0;
                counter       = 0;
                firstPossessSwitched = false;
                secondPossessSwitched = false;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);
            }

            void EnterEvadeMode()
            {
                firstPossessSwitched = false;
                secondPossessSwitched = false;

                events.Reset();
                summons.DespawnAll();

                _EnterEvadeMode();

                if (!m_Instance || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == NOT_STARTED)
                    return;

                WipeOnFight(m_Instance, me);
            }

            void JustReachedHome()
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                    Reset();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* who)
            {
                StartFight(m_Instance, me, who);

                events.Reset();
                events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                events.ScheduleEvent(EVENT_BITING_COLD, 62000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                summons.DespawnAll();
                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                if (!AllBossesDead(me))
                    me->SetLootRecipient(NULL);
                else
                {
                    if (m_Instance)
                    {
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, DONE);
                        _JustDied();

                        if (IsLFR())
                        {
                            me->SetLootRecipient(nullptr);

                            if (Creature* l_KillCredit = Creature::GetCreature(*me, m_Instance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                            {
                                bool l_Assigned = false;
                                Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                                for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                                {
                                    if (Player* l_Player = l_Itr->getSource())
                                    {
                                        uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                        if (l_DungeonID != eMiscs::ToTFirstPart)
                                            continue;

                                        if (!me || l_Player->IsAtGroupRewardDistance(me))
                                            sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);

                                        if (l_Assigned)
                                            continue;

                                        if (Group* l_Group = l_Player->GetGroup())
                                        {
                                            l_Assigned = true;
                                            sLFGMgr->AutomaticLootAssignation(l_KillCredit, l_Group);
                                        }

                                        /// For first LFR part, this door should not be opened
                                        if (GameObject* l_Door = l_KillCredit->FindNearestGameObject(eMiscs::LastDoor, 200.0f))
                                            l_Door->SetGoState(GO_STATE_READY);
                                    }
                                }
                            }
                        }
                    }
                }

                Talk(7);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SCHEDULE_FRIGID_ASSAULT:
                        events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 40000);
                        break;
                    case ACTION_SCHEDULE_FROSTBITE:
                        events.Reset();
                        events.ScheduleEvent(EVENT_FROSTBITE, 2000);
                        break;
                    case ACTION_TALK_AGGRO_MALAKK:
                        Talk(0);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(67.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                            events.ScheduleEvent(EVENT_BITING_COLD, 62000);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_FRIGID_ASSAULT, 30000);
                            events.ScheduleEvent(EVENT_BITING_COLD, 62000);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            secondPossessSwitched = true;
                        }
                    }
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

                value = 0;

                if (me->HasAura(SPELL_POSSESSED))
                    value = 2; // Generates 1 point per minute

                if (me->HasAura(SPELL_LINGERING_PRESENCE))
                    value = 2 + coefficient;
            }

            uint32 GetData(uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    return coefficient;
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(urand(5, 6));
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_Instance)
                {
                    if (m_Instance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();
                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FRIGID_ASSAULT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        {
                            if (!target->HasAura(SPELL_FRIGID_ASSAULT_STUN))
                                me->CastSpell(target, SPELL_FRIGID_ASSAULT, true);
                        }
                        break;
                    case EVENT_BITING_COLD:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_BITING_COLD, true);
                        events.ScheduleEvent(EVENT_BITING_COLD, 62000);
                        break;
                    case EVENT_FROSTBITE:
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            {
                                me->AddAura(SPELL_FROSTBITE_AURA, target);
                                if (IsHeroic())
                                    Talk(4);
                                me->CastSpell(target, SPELL_FROSTBITE, true);
                                events.ScheduleEvent(EVENT_FROSTBITE, 52000);
                            }
                        }
                        break;
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_king_malakkAI(creature);
        }
};

// Kazra'jin - 69134
class boss_kazra_jin : public CreatureScript
{
    public:
        boss_kazra_jin() : CreatureScript("boss_kazra_jin") { }

        struct boss_kazra_jinAI : public BossAI
        {
            boss_kazra_jinAI(Creature* creature) : BossAI(creature, DATA_COUNCIL_OF_ELDERS)
            {
                m_Instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* m_Instance;
            uint64 playerGuid;
            bool touchedTarget;
            bool firstPossessSwitched;
            bool secondPossessSwitched;
            bool hasSpawned;
            uint32 coefficient;
            uint32 counter;
            uint64 recklessTargetGuid;

            void Reset()
            {
                events.Reset();

                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, NOT_STARTED);

                    RemoveAllEncounterAuras(m_Instance);
                }

                touchedTarget         = true;
                firstPossessSwitched  = false;
                secondPossessSwitched = false;
                hasSpawned            = false;

                coefficient        = 0;
                counter            = 0;
                playerGuid         = 0;
                recklessTargetGuid = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);
                me->SetReactState(REACT_AGGRESSIVE);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_SCHEDULE_AT_DMG:
                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG, 1000);
                        break;
                    case ACTION_TALK_AGGRO_KAZRA_JIN:
                        Talk(0);
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                summons.DespawnAll();
                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                if (!AllBossesDead(me))
                    me->SetLootRecipient(NULL);
                else
                {
                    if (m_Instance)
                    {
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, DONE);
                        _JustDied();

                        if (IsLFR())
                        {
                            me->SetLootRecipient(nullptr);

                            if (Creature* l_KillCredit = Creature::GetCreature(*me, m_Instance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                            {
                                bool l_Assigned = false;
                                Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                                for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                                {
                                    if (Player* l_Player = l_Itr->getSource())
                                    {
                                        uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                        if (l_DungeonID != eMiscs::ToTFirstPart)
                                            continue;

                                        if (!me || l_Player->IsAtGroupRewardDistance(me))
                                            sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);

                                        if (l_Assigned)
                                            continue;

                                        if (Group* l_Group = l_Player->GetGroup())
                                        {
                                            l_Assigned = true;
                                            sLFGMgr->AutomaticLootAssignation(l_KillCredit, l_Group);
                                        }

                                        /// For first LFR part, this door should not be opened
                                        if (GameObject* l_Door = l_KillCredit->FindNearestGameObject(eMiscs::LastDoor, 200.0f))
                                            l_Door->SetGoState(GO_STATE_READY);
                                    }
                                }
                            }
                        }
                    }
                }

                Talk(7);
            }

            void EnterEvadeMode()
            {
                firstPossessSwitched = false;
                secondPossessSwitched = false;
                hasSpawned = false;
                touchedTarget = true;

                events.Reset();
                summons.DespawnAll();

                _EnterEvadeMode();

                if (!m_Instance || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == NOT_STARTED)
                    return;

                WipeOnFight(m_Instance, me);
            }

            void JustReachedHome()
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                    Reset();
            }

            void EnterCombat(Unit* who)
            {
                StartFight(m_Instance, me, who);
                events.ScheduleEvent(EVENT_RECKLESS_CHARGE, 5000);
            }

            void SetGUID(uint64 guid, int32 id)
            {
                if (id == 1)
                    playerGuid = guid;
            }

            uint64 GetGUID(int32 p_Guid)
            {
                if (p_Guid == GUID_RECKLESS_TARGET)
                    return recklessTargetGuid;

                return 0;
            }

            void DamageTaken(Unit* attacker, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (attacker == me)
                    return;

                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            secondPossessSwitched = true;
                        }
                    }
                }

                if (me->HasAura(SPELL_OVERLOAD))
                {
                    me->DealDamage(attacker, damage / 100 * 50);
                    me->SendSpellNonMeleeDamageLog(attacker, SPELL_OVERLOAD, damage / 100 * 40, SPELL_SCHOOL_MASK_NATURE, 0, 0, false, 0, false);
                }
                else if (me->HasAura(SPELL_DISCHARGE))
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 200.0f);

                    for (auto player : playerList)
                    {
                        me->DealDamage(player, damage / 100 * 10);
                        me->SendSpellNonMeleeDamageLog(player, SPELL_DISCHARGE, damage / 100 * 10, SPELL_SCHOOL_MASK_NATURE, 0, 0, false, 0, false);
                    }
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

                value = 0;

                if (me->HasAura(SPELL_POSSESSED))
                    value = 2; // Generates 1 point per minute

                if (me->HasAura(SPELL_LINGERING_PRESENCE))
                    value = 2 + coefficient;
            }

            uint32 GetData(uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    return coefficient;
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(urand(5, 6));
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_Instance)
                {
                    if (m_Instance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!me->HasAura(SPELL_RECKLESS_CHARGE_ROLLING))
                {
                    if (!UpdateVictim())
                    {
                        if (me->isInCombat())
                            me->CombatStop();
                        EnterEvadeMode();
                        return;
                    }
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();
                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasAura(SPELL_OVERLOAD))
                    me->SendMeleeAttackStop(me->getVictim());

                if (me->HasAura(SPELL_RECKLESS_CHARGE_ROLLING))
                {
                    if (!touchedTarget)
                    {
                        if (!hasSpawned)
                        {
                            events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER, 400);
                            hasSpawned = true;
                        }

                        if (Player* target = Player::GetPlayer(*me, playerGuid))
                        {
                            if (me->GetDistance(target) <= 3.5f)
                            {
                                if (target->GetGUID() == playerGuid)
                                {
                                    events.CancelEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER);
                                    events.CancelEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG);
                                    me->CastSpell(target, SPELL_RECKLESS_CHARGE_KNOCK_BACK, true);
                                    me->canStartAttack(target, true);
                                    me->RemoveAura(SPELL_RECKLESS_CHARGE_ROLLING);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    me->Attack(target, true);
                                    playerGuid = 0;
                                    touchedTarget = true;
                                    hasSpawned = false;
                                    me->SetReactState(REACT_AGGRESSIVE);
                                    me->SetSpeed(MOVE_WALK, 1.0f);
                                    me->SetSpeed(MOVE_RUN, 1.0f);

                                    std::list<Creature*> creatureList;
                                    GetCreatureListWithEntryInGrid(creatureList, me, 69453, 200.0f);

                                    for (Creature* creature : creatureList)
                                        creature->DespawnOrUnsummon();

                                    if (me->HasAura(SPELL_POSSESSED))
                                    {
                                        if (!IsHeroic())
                                            me->AddAura(SPELL_OVERLOAD, me);
                                        else
                                        {
                                            me->AddAura(SPELL_DISCHARGE, me);
                                            Talk(4);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_RECKLESS_CHARGE:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 200.0f);

                        me->SetSpeed(MOVE_WALK, 2.2f);
                        me->SetSpeed(MOVE_RUN, 2.2f);

                        std::list<Player*>::iterator itr = playerList.begin();
                        Player* target = NULL;

                        if (!playerList.empty())
                        {
                            while (!target)
                            {
                                if (urand(0, 1))
                                    target = *itr;

                                ++itr;

                                if (itr == playerList.end())
                                    itr = playerList.begin();
                            }
                        }

                        if (target)
                            recklessTargetGuid = target->GetGUID();

                        me->SetReactState(REACT_PASSIVE);
                        me->CastSpell(me, SPELL_RECKLESS_CHARGE_ROLLING, true);
                        touchedTarget = false;
                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE, 10000);

                        Talk(urand(2, 3));
                    }
                        break;
                    case EVENT_RECKLESS_CHARGE_AREATRIGGER:
                        me->CastSpell(me, SPELL_RECKLESS_CHARGE_AREATRIGGER, false);
                        me->SummonCreature(NPC_RECKLESS_CHARGE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        hasSpawned = false;
                        break;
                    case EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG:
                    {
                        std::list<Creature*> creatureList;
                        GetCreatureListWithEntryInGrid(creatureList, me, 69453, 200.0f);

                        for (Creature* creature : creatureList)
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, creature, 1.0f);

                            for (Player* player : playerList)
                                creature->CastSpell(player, SPELL_RECKLESS_CHARGE_DAMAGE, false);
                        }

                        events.ScheduleEvent(EVENT_RECKLESS_CHARGE_AREATRIGGER_DMG, 1000);
                        break;
                    }
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
                    default:
                        break;
                }

                if (!me->HasAura(SPELL_RECKLESS_CHARGE_ROLLING))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_kazra_jinAI(creature);
        }
};

// Sul the Sandcrawler - 69078
class boss_sul_the_sandcrawler : public CreatureScript
{
    public:
        boss_sul_the_sandcrawler() : CreatureScript("boss_sul_the_sandcrawler") { }

        struct boss_sul_the_sandcrawlerAI : public BossAI
        {
            boss_sul_the_sandcrawlerAI(Creature* creature) : BossAI(creature, DATA_COUNCIL_OF_ELDERS)
            {
                m_Instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* m_Instance;
            bool firstPossessSwitched;
            bool secondPossessSwitched;
            uint32 coefficient;
            uint32 counter;

            void Reset()
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, NOT_STARTED);

                    RemoveAllEncounterAuras(m_Instance);
                }

                me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, EQUIP_SUL);

                events.Reset();

                firstPossessSwitched = false;
                secondPossessSwitched = false;
                coefficient = 0;
                counter     = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);

                std::list<Creature*> livingSandList;
                GetCreatureListWithEntryInGrid(livingSandList, me, NPC_LIVING_SAND, 200.0f);

                std::list<Creature*> firstTwistedFateList;
                GetCreatureListWithEntryInGrid(firstTwistedFateList, me, NPC_FIRST_TWISTED_FATE, 200.0f);

                std::list<Creature*> secondTwistedFateList;
                GetCreatureListWithEntryInGrid(secondTwistedFateList, me, NPC_SECOND_TWISTED_FATE, 200.0f);

                for (auto creature : livingSandList)
                    creature->DespawnOrUnsummon();

                for (auto creature : firstTwistedFateList)
                    creature->DespawnOrUnsummon();

                for (auto creature : secondTwistedFateList)
                    creature->DespawnOrUnsummon();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* who)
            {
                StartFight(m_Instance, me, who);

                events.ScheduleEvent(EVENT_SAND_BOLT, 10000);
                events.ScheduleEvent(EVENT_QUICKSAND, 35000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                summons.DespawnAll();
                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GENERIC_STUN);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                if (!AllBossesDead(me))
                    me->SetLootRecipient(NULL);
                else
                {
                    if (m_Instance)
                    {
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, DONE);
                        _JustDied();
                    }

                    if (IsLFR())
                    {
                        me->SetLootRecipient(nullptr);

                        if (Creature* l_KillCredit = Creature::GetCreature(*me, m_Instance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                        {
                            bool l_Assigned = false;
                            Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                            {
                                if (Player* l_Player = l_Itr->getSource())
                                {
                                    uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                    if (l_DungeonID != eMiscs::ToTFirstPart)
                                        continue;

                                    if (!me || l_Player->IsAtGroupRewardDistance(me))
                                        sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);

                                    if (l_Assigned)
                                        continue;

                                    if (Group* l_Group = l_Player->GetGroup())
                                    {
                                        l_Assigned = true;
                                        sLFGMgr->AutomaticLootAssignation(l_KillCredit, l_Group);
                                    }

                                    /// For first LFR part, this door should not be opened
                                    if (GameObject* l_Door = l_KillCredit->FindNearestGameObject(eMiscs::LastDoor, 200.0f))
                                        l_Door->SetGoState(GO_STATE_READY);
                                }
                            }
                        }
                    }
                }

                Talk(7);
            }

            void EnterEvadeMode()
            {
                firstPossessSwitched = false;
                secondPossessSwitched = false;

                events.Reset();
                summons.DespawnAll();

                _EnterEvadeMode();

                if (!m_Instance || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == NOT_STARTED)
                    return;

                WipeOnFight(m_Instance, me);
            }

            void JustReachedHome()
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                    Reset();
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(urand(5, 6));
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            secondPossessSwitched = true;
                        }
                    }
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

                value = 0;

                if (me->HasAura(SPELL_POSSESSED))
                    value = 2; // Generates 1 point per minute

                if (me->HasAura(SPELL_LINGERING_PRESENCE))
                    value = 2 + coefficient;
            }

            uint32 GetData(uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    return coefficient;
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_SCHEDULE_SANDSTROM:
                        events.ScheduleEvent(EVENT_SANDSTORM, 5000);
                        break;
                    case ACTION_TALK_AGGRO_SUL:
                        Talk(0);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_Instance)
                {
                    if (m_Instance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();
                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SAND_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_SAND_BOLT, true);
                        events.ScheduleEvent(EVENT_SAND_BOLT, 10000);
                        break;
                    case EVENT_QUICKSAND:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(target, SPELL_QUICKSAND, true);
                            Talk(3);
                            me->SummonCreature(NPC_LIVING_SAND, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        }
                        events.ScheduleEvent(EVENT_QUICKSAND, 34000);
                        break;
                    case EVENT_SANDSTORM:
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SANDSTORM, true);

                            Talk(2);
                            me->AddAura(SPELL_SANDSTORM_DUMMY, me);
                            events.ScheduleEvent(EVENT_SANDSTORM, 40000);
                        }
                        break;
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, true);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sul_the_sandcrawlerAI(creature);
        }
};

// Twisted Fate (first) - 69740
class mob_first_twisted_fate : public CreatureScript
{
    public:
        mob_first_twisted_fate() : CreatureScript("mob_first_twisted_fate") { }

        struct mob_first_twisted_fateAI : public ScriptedAI
        {
            mob_first_twisted_fateAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            bool visualLinked;
            uint16 linkTimer;
            uint16 distanceMax;
            uint64 summonerGuid;
            InstanceScript* instance;

            void Reset()
            {
                distanceMax  = 0;
                linkTimer    = 0;
                summonerGuid = 0;
                visualLinked = false;
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_SHADOW_VISUAL , me);
            }

            void IsSummonedBy(Unit* summoner)
            {
                summonerGuid = summoner->GetGUID();
                me->AddAura(SPELL_TWISTED_FATE_PERIODIC, me);
                linkTimer = 1500;
            }

            void UpdateAI(uint32 const diff)
            {
                if (linkTimer <= diff)
                {
                    if (!visualLinked)
                    {
                        if (Creature* twistedFate = GetClosestCreatureWithEntry(me, NPC_SECOND_TWISTED_FATE, 200.0f))
                        {
                            me->GetMotionMaster()->MoveChase(twistedFate, 1.0f, 1.0f);
                            visualLinked = true;
                        }
                    }
                }
                else
                   linkTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_first_twisted_fateAI(creature);
        }
};

// Twisted Fate (second) - 69746
class mob_second_twisted_fate : public CreatureScript
{
    public:
        mob_second_twisted_fate() : CreatureScript("mob_second_twisted_fate") { }

        struct mob_second_twisted_fateAI : public ScriptedAI
        {
            mob_second_twisted_fateAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            bool visualLinked;
            uint64 summonerGuid;
            uint16 linkTimer;
            uint16 distanceMax;
            InstanceScript* instance;

            void Reset()
            {
                linkTimer      = 0;
                summonerGuid   = 0;
                distanceMax    = 0;
                visualLinked = false;
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_SHADOW_VISUAL , me);
            }

            void IsSummonedBy(Unit* summoner)
            {
                summonerGuid = summoner->GetGUID();
                me->AddAura(SPELL_TWISTED_FATE_PERIODIC, me);
                linkTimer = 1500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (linkTimer <= diff)
                {
                    if (!visualLinked)
                    {
                        if (Creature* twistedFate = GetClosestCreatureWithEntry(me, NPC_FIRST_TWISTED_FATE, 200.0f))
                        {
                            me->CastSpell(twistedFate, SPELL_TWISTED_FATE_LINK_VISUAL, false);
                            me->GetMotionMaster()->MoveChase(twistedFate, 1.0f, 1.0f);
                            visualLinked = true;
                        }
                    }
                }
                else
                   linkTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_second_twisted_fateAI(creature);
        }
};

// High Priestess Mar'li - 69132
class boss_high_priestress_mar_li : public CreatureScript
{
    public:
        boss_high_priestress_mar_li() : CreatureScript("boss_high_priestress_mar_li") { }

        struct boss_high_priestress_mar_liAI : public BossAI
        {
            boss_high_priestress_mar_liAI(Creature* creature) : BossAI(creature, DATA_COUNCIL_OF_ELDERS)
            {
                m_Instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* m_Instance;
            bool firstPossessSwitched;
            bool secondPossessSwitched;
            uint32 coefficient;
            uint32 counter;

            void Reset()
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL)
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, NOT_STARTED);

                    RemoveAllEncounterAuras(m_Instance);
                }

                me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, EQUIP_MARLI);

                events.Reset();

                firstPossessSwitched  = false;
                secondPossessSwitched = false;

                coefficient = 0;
                counter     = 0;
                _Reset();
                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->SetPower(POWER_ENERGY, 0, false);

                std::list<Creature*> l_ShadowedSpiritList;
                GetCreatureListWithEntryInGrid(l_ShadowedSpiritList, me, NPC_SHADOWED_LOA_SPIRIT, 200.0f);

                for (Creature* l_ShadowedSpirit : l_ShadowedSpiritList)
                    l_ShadowedSpirit->DespawnOrUnsummon();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_SCHEDULE_SHADOWED_SPIRIT_SPAWN:
                    {
                        if (IsHeroic())
                        {
                            events.Reset();

                            events.ScheduleEvent(EVENT_TWISTED_FATE, 2000);
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_SHADOW, 3000);
                            events.ScheduleEvent(EVENT_TWISTED_FATE_SECOND, 5000);
                            return;
                        }

                        events.Reset();
                        events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_SHADOW, 3000);
                        events.ScheduleEvent(EVENT_SHADOWED_LOA_SPIRIT_SUMMON, 5000);
                    }
                    break;
                    case ACTION_TALK_AGGRO_MAR_LI:
                        Talk(0);
                        break;
                    default:
                        break;
                }
            }

            void EnterEvadeMode()
            {
                firstPossessSwitched = false;
                secondPossessSwitched = false;

                events.Reset();
                summons.DespawnAll();

                _EnterEvadeMode();

                if (!m_Instance || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == FAIL || m_Instance->GetBossState(DATA_COUNCIL_OF_ELDERS) == NOT_STARTED)
                    return;

                WipeOnFight(m_Instance, me);
            }

            void EnterCombat(Unit* who)
            {
                StartFight(m_Instance, me, who);

                events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 10000);
            }

            void DamageTaken(Unit* /*killer*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!firstPossessSwitched)
                {
                    if (me->HasAura(SPELL_POSSESSED))
                    {
                        if (me->HealthBelowPctDamaged(70.0f, damage))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                            events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 10000);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            firstPossessSwitched = true;
                        }
                    }
                }

                if (!secondPossessSwitched)
                {
                    if (me->HealthBelowPctDamaged(25.0f, damage))
                    {
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            me->SetPower(POWER_ENERGY, 0, false);
                            me->RemoveAura(SPELL_POSSESSED);
                            me->CastSpell(me, SPELL_LINGERING_PRESENCE, true);

                            events.Reset();
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                            events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 10000);

                            if (Creature* garaJalSoul = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                            {
                                if (garaJalSoul->GetAI())
                                {
                                    garaJalSoul->AI()->DoAction(ACTION_SCHEDULE_POSSESSION);
                                    garaJalSoul->AI()->DoAction(ACTION_SOUL_FRAGMENT);
                                }
                            }

                            secondPossessSwitched = true;
                        }
                    }
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                summons.DespawnAll();
                if (m_Instance)
                {
                    RemoveAllEncounterAuras(m_Instance);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                if (!AllBossesDead(me))
                    me->SetLootRecipient(NULL);
                else
                {
                    if (m_Instance)
                    {
                        m_Instance->SetBossState(DATA_COUNCIL_OF_ELDERS, DONE);
                        _JustDied();

                        if (IsLFR())
                        {
                            me->SetLootRecipient(nullptr);

                            if (Creature* l_KillCredit = Creature::GetCreature(*me, m_Instance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                            {
                                bool l_Assigned = false;
                                Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                                for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                                {
                                    if (Player* l_Player = l_Itr->getSource())
                                    {
                                        uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                        if (l_DungeonID != eMiscs::ToTFirstPart)
                                            continue;

                                        if (!me || l_Player->IsAtGroupRewardDistance(me))
                                            sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);

                                        if (l_Assigned)
                                            continue;

                                        if (Group* l_Group = l_Player->GetGroup())
                                        {
                                            l_Assigned = true;
                                            sLFGMgr->AutomaticLootAssignation(l_KillCredit, l_Group);
                                        }

                                        /// For first LFR part, this door should not be opened
                                        if (GameObject* l_Door = l_KillCredit->FindNearestGameObject(eMiscs::LastDoor, 200.0f))
                                            l_Door->SetGoState(GO_STATE_READY);
                                    }
                                }
                            }
                        }
                    }
                }

                Talk(8);
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) > 100)
                {
                    me->SetPower(POWER_ENERGY, 100, false);
                    return;
                }

                value = 0;

                if (me->HasAura(SPELL_POSSESSED))
                    value = 2; // Generates 1 point per minute

                if (me->HasAura(SPELL_LINGERING_PRESENCE))
                    value = 2 + coefficient;
            }

            uint32 GetData(uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    return coefficient;
                else if (index == DATA_SPELL_DMG_MULTIPLIER)
                    return counter;

                return 0;
            }

            void SetData(uint32 value, uint32 index)
            {
                if (index == DATA_LINGERING_PRESENCE_COEF)
                    value = coefficient;
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(urand(6,7));
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_Instance)
                {
                    if (m_Instance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();
                    EnterEvadeMode();
                    return;
                }

                if (me->GetPower(POWER_ENERGY) == 100)
                {
                    events.Reset();
                    events.ScheduleEvent(EVENT_DARK_POWER, 1000);
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WRATH_OF_THE_LOA_BLESSED:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_WRATH_OF_THE_LOA_BLESSED, false);
                        events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_BLESSED, 10000);
                        break;
                    case EVENT_WRATH_OF_THE_LOA_SHADOW:
                        if (me->HasAura(SPELL_POSSESSED))
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_WRATH_OF_THE_LOA_SHADOW, false);
                            events.ScheduleEvent(EVENT_WRATH_OF_THE_LOA_SHADOW, 10000);
                        }
                        break;
                    case EVENT_BLESSED_LOA_SPIRIT_SUMMON:
                        if (!GetClosestCreatureWithEntry(me, NPC_BLESSED_LOA_SPIRIT, 200.0f))
                            me->CastSpell(me, SPELL_BLESSED_LOA_SPIRIT_SUMMON, true);
                        events.ScheduleEvent(EVENT_BLESSED_LOA_SPIRIT_SUMMON, 35000);
                        break;
                    case EVENT_SHADOWED_LOA_SPIRIT_SUMMON:
                        if (me->HasAura(SPELL_POSSESSED))
                            me->CastSpell(me, SPELL_SHADOWED_LOA_SPIRIT_SUMMONED, true);

                        Talk(urand(2, 5));
                        events.ScheduleEvent(EVENT_SHADOWED_LOA_SPIRIT_SUMMON, 37000);
                        break;
                    case EVENT_DARK_POWER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            counter++;
                            me->CastSpell(me, SPELL_DARK_POWER, false);
                            events.ScheduleEvent(EVENT_DARK_POWER, 10000);
                        }
                        break;
                    case EVENT_TWISTED_FATE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                            me->CastSpell(me, SPELL_TWISTED_FATE_PRINCIPAL, true);
                        break;
                    case EVENT_TWISTED_FATE_SECOND:
                        if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST))
                        {
                            me->CastSpell(me, SPELL_TWISTED_FATE_SECOND_PRINCIPAL, false);

                            if (Creature* twistedFirst = GetClosestCreatureWithEntry(me, NPC_FIRST_TWISTED_FATE, 200.0f))
                            {
                                if (Creature* twistedSecond = GetClosestCreatureWithEntry(me, NPC_SECOND_TWISTED_FATE, 200.0f))
                                {
                                    float distance = twistedFirst->GetDistance(twistedSecond);

                                    CAST_AI(mob_first_twisted_fate::mob_first_twisted_fateAI, twistedFirst->AI())->distanceMax = distance;
                                    CAST_AI(mob_second_twisted_fate::mob_second_twisted_fateAI, twistedSecond->AI())->distanceMax = distance;
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_high_priestress_mar_liAI(creature);
        }
};

// Living Sand - 69153
class mob_living_sand : public CreatureScript
{
    public:
        mob_living_sand() : CreatureScript("mob_living_sand") { }

        struct mob_living_sandAI : public ScriptedAI
        {
            mob_living_sandAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* instance;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_SAND_VISUAL, me);
                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);
                me->SetFullHealth();
                events.Reset();

                if (me->HasAura(SPELL_FORTIFIED))
                    me->RemoveAura(SPELL_FORTIFIED);

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 7.0f);

                for (auto player: playerList)
                {
                    if (!player->HasAura(SPELL_SAND_PERIODIC_DMG))
                        me->AddAura(SPELL_SAND_PERIODIC_DMG, player);

                    player->AddAura(SPELL_ENSNARED, player);
                }
            }

            void IsSummonedBy(Unit* /*p_Attacker*/)
            {
                std::list<Creature*> livingSangList;
                GetCreatureListWithEntryInGrid(livingSangList, me, NPC_LIVING_SAND, 4.5f);

                for (Creature* livingSand : livingSangList)
                {
                    if (livingSand->HasAura(SPELL_TREACHEROUS_GROUND_RESIZE))
                    {
                        livingSand->DespawnOrUnsummon();
                        me->AddAura(SPELL_TREACHEROUS_GROUND, me);

                        if (Creature* sul = instance->instance->GetCreature(instance->GetData64(NPC_SUL_THE_SANDCRAWLER)))
                        {
                            if (sul->GetAI())
                                sul->AI()->Talk(4);
                        }
                    }
                }

                events.ScheduleEvent(EVENT_ENSNARED, 1000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_SANDSTORM)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RemoveAura(SPELL_SAND_VISUAL);
                    me->SetReactState(REACT_AGGRESSIVE);

                    if (Player* l_Player = me->SelectNearestPlayer(30.0f))
                        me->Attack(l_Player, true);
                }
                else if (action == ACTION_TREACHEROUS_GROUND)
                {
                    std::list<Creature*> livingSangList;
                    GetCreatureListWithEntryInGrid(livingSangList, me, NPC_LIVING_SAND, 4.5f);

                    for (Creature* livingSand : livingSangList)
                    {
                        if (livingSand != me)
                        {
                            livingSand->DespawnOrUnsummon();
                            me->AddAura(SPELL_TREACHEROUS_GROUND_RESIZE, me);
                        }
                    }
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (damage > me->GetHealth())
                {
                    damage = 0;
                    Reset();
                }
            }

            void UpdateAI(uint32 const diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_ENSNARED:
                        if (me->HasAura(SPELL_SAND_VISUAL))
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 7.0f);

                            for (Player* player : playerList)
                            {
                                if (!player->HasAura(SPELL_ENTRAPPED))
                                    player->AddAura(SPELL_ENSNARED, player);
                            }
                        }

                        events.ScheduleEvent(EVENT_ENSNARED, 1000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_living_sandAI(creature);
        }
};

// Blessed Loa Spirit - 69480
class mob_blessed_loa_spirit : public CreatureScript
{
    public:
        mob_blessed_loa_spirit() : CreatureScript("mob_blessed_loa_spirit") { }

        struct mob_blessed_loa_spiritAI : public ScriptedAI
        {
            mob_blessed_loa_spiritAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* instance;
            uint32 bossEntry;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HEAL_WEAKER_TROLL, 20000);
                me->SetReactState(REACT_PASSIVE);
                bossEntry = 0;
            }

            void IsSummonedBy(Unit * /*p_Summoner*/)
            {
                me->AddAura(SPELL_BLESSED_TRANSFORMATION_LIGHT, me);
                me->AddAura(SPELL_BLESSED_TRANSFORMATION, me);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (bossEntry)
                {
                    if (Creature* boss = instance->instance->GetCreature(instance->GetData64(bossEntry)))
                    {
                        if (me->GetDistance(boss) <= 2.0f)
                        {
                            boss->ModifyHealth(boss->GetHealth() * 5 / 100);
                            me->DespawnOrUnsummon();
                        }
                    }
                }

                switch (events.ExecuteEvent())
                {
                    case EVENT_HEAL_WEAKER_TROLL:
                        {
                            uint32 mobEntries[4] = {NPC_KAZRA_JIN, NPC_SUL_THE_SANDCRAWLER, NPC_HIGH_PRIESTRESS_MAR_LI, NPC_FROST_KING_MALAKK};
                            uint32 minHealth = 0;
                            Creature* minBoss = NULL;

                            for (int i = 0; i < 4; i++)
                            {
                                uint32 entry = mobEntries[i];
                                if (Creature* boss = instance->instance->GetCreature(instance->GetData64(entry)))
                                {
                                    if (entry == NPC_KAZRA_JIN)
                                    {
                                        minHealth = boss->GetMaxHealth();
                                        minBoss = boss;
                                    }

                                    if (minHealth > boss->GetHealth() && boss->isAlive())
                                    {
                                        minHealth = boss->GetHealth();
                                        minBoss = boss;
                                    }
                                }
                            }

                            if (minBoss)
                            {
                                if (bossEntry != minBoss->GetEntry())
                                    bossEntry = minBoss->GetEntry();
                            }

                            me->GetMotionMaster()->MoveChase(minBoss, 0.7f, 0.7f);
                        }
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_blessed_loa_spiritAI(creature);
        }
};

// Shadowed Loa Spirit - 69548
class mob_shadowed_lua_spirit : public CreatureScript
{
    public:
        mob_shadowed_lua_spirit() : CreatureScript("mob_shadowed_lua_spirit") { }

        struct mob_shadowed_lua_spiritAI : public ScriptedAI
        {
            mob_shadowed_lua_spiritAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            EventMap events;
            uint32 despawnTimer;
            uint64 targetGuid;
            InstanceScript* instance;

            void Reset()
            {
                events.Reset();
                despawnTimer = 0;
                targetGuid   = 0;

                events.ScheduleEvent(EVENT_OS_PLAYER, 20000);
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit * /*p_Summoner*/)
            {
                me->AddAura(SPELL_SHADOWED_TRANSFORMATION, me);
            }

            void SetGUID(uint64 guid, int32 /*index*/)
            {
                targetGuid = guid;
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (Player* player = Player::GetPlayer(*me, targetGuid))
                {
                    if (despawnTimer <= diff)
                    {

                        if (player->HasAura(SPELL_MARKED_SOUL))
                            player->RemoveAura(SPELL_MARKED_SOUL);

                        me->CastSpell(player, SPELL_SHADOWED_GIFT, false);
                    }
                    else
                        despawnTimer -= diff;

                    if (player->GetDistance(me) <= 6.0f)
                    {
                        me->CastSpell(player, SPELL_SHADOWED_GIFT, false);
                        me->DespawnOrUnsummon();
                    }
                }

                switch (events.ExecuteEvent())
                {
                    case EVENT_OS_PLAYER:
                        {
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 200.0f);

                            if(playerList.empty())
                                break;

                            JadeCore::RandomResizeList(playerList, 1);

                            if (Player* target = playerList.front())
                            {
                                me->AddAura(SPELL_MARKED_SOUL, target);
                                SetGUID(target->GetGUID(), 0);
                                despawnTimer = 20000;
                            }
                            else
                                events.ScheduleEvent(EVENT_OS_PLAYER, 500);
                            break;
                        }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_shadowed_lua_spiritAI(creature);
    }
};

// Reckless Charge (rolling) - 137117
class spell_reckless_charge_rolling : public SpellScriptLoader
{
    public:
        spell_reckless_charge_rolling() : SpellScriptLoader("spell_reckless_charge_rolling") { }

        class spell_reckless_charge_rolling_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_reckless_charge_rolling_SpellScript);

            void HandleAfterCast()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    if (caster->GetEntry() == NPC_KAZRA_JIN && caster->GetAI())
                    {
                        uint64 targetGuid = CAST_AI(boss_kazra_jin::boss_kazra_jinAI, caster->AI())->recklessTargetGuid;

                        if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                        {
                            caster->AttackStop();

                            if (caster->GetDistance(target) >= 3.5f)
                                caster->CastSpell(target, SPELL_RECKLESS_CHARGE_MOVEMENT, false);

                            if (Player* player = target->ToPlayer())
                            {
                                caster->AI()->SetGUID(player->GetGUID(), 1);
                                caster->AI()->DoAction(ACTION_SCHEDULE_AT_DMG);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_reckless_charge_rolling_SpellScript::HandleAfterCast);
            }
        };

        class spell_reckless_charge_rolling_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_reckless_charge_rolling_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster() || !GetTarget())
                    return;

                if (Creature* caster = GetCaster()->ToCreature())
                {
                    if (caster->GetEntry() == NPC_KAZRA_JIN && caster->GetAI())
                    {
                        caster->SetReactState(REACT_AGGRESSIVE);

                        if (Player* target = GetTarget()->ToPlayer())
                            caster->CastSpell(target, SPELL_RECKLESS_CHARGE_KNOCK_BACK, false);
                        else
                        {
                            std::list<Creature*> creatureList;
                            GetCreatureListWithEntryInGrid(creatureList, caster, NPC_RECKLESS_CHARGE, 200.0f);

                            for (Creature* creature : creatureList)
                                creature->DespawnOrUnsummon();
                        }

                        caster->SetSpeed(MOVE_WALK, 1.0f);
                        caster->SetSpeed(MOVE_RUN, 1.0f);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_reckless_charge_rolling_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_reckless_charge_rolling_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_reckless_charge_rolling_SpellScript();
        }
};

// Reckless Charge (movement) - 137131
class spell_reckless_charge_movement : public SpellScriptLoader
{
    public:
        spell_reckless_charge_movement() : SpellScriptLoader("spell_reckless_charge_movement") { }

        class spell_reckless_charge_movement_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_reckless_charge_movement_SpellScript);

            void HandleAfterCast()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    //uint64 targetGuid = CAST_AI(boss_kazra_jin::boss_kazra_jinAI, caster->AI())->recklessTargetGuid;
                    uint64 targetGuid = caster->AI()->GetGUID(GUID_RECKLESS_TARGET);

                    if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                    {
                        if (caster->GetEntry() == NPC_KAZRA_JIN && caster->GetAI())
                        {
                            caster->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                            caster->GetMotionMaster()->MovePoint(1, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_reckless_charge_movement_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_reckless_charge_movement_SpellScript();
        }
};

// Frigid Assault (Stun) - 136904
class spell_frigid_assault : public SpellScriptLoader
{
    public:
        spell_frigid_assault() : SpellScriptLoader("spell_frigid_assault") { }

        class spell_frigid_assault_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_frigid_assault_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Aura* frigidAssault = aurEff->GetBase())
                    if (frigidAssault->GetStackAmount() == 15)
                    {
                        if (Unit* caster = GetCaster())
                        {
                            if (Unit* target = GetTarget())
                            {
                                target->RemoveAurasDueToSpell(SPELL_FRIGID_ASSAULT);
                                caster->RemoveAurasDueToSpell(SPELL_FRIGID_ASSAULT);
                                target->CastSpell(target, SPELL_FRIGID_ASSAULT_STUN, true);

                                if (Creature* malakk = caster->ToCreature())
                                {
                                    if (malakk->GetAI())
                                        malakk->AI()->DoAction(ACTION_SCHEDULE_FRIGID_ASSAULT);
                                }
                            }
                        }
                    }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Creature* malakk = caster->ToCreature())
                    {
                        if (malakk->GetAI())
                            malakk->AI()->DoAction(ACTION_SCHEDULE_FRIGID_ASSAULT);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_frigid_assault_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                AfterEffectRemove += AuraEffectRemoveFn(spell_frigid_assault_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_frigid_assault_AuraScript();
        }
};

// Lingering Presence - 136467
class spell_lingering_presence : public SpellScriptLoader
{
    public:
        spell_lingering_presence() : SpellScriptLoader("spell_lingering_presence") { }

        class spell_lingering_presence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lingering_presence_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                uint32 coefficient = 2 * 10 / 100;

                if (Unit* target = GetTarget())
                {
                    if (Creature* creature = target->ToCreature())
                    {
                        if (creature->GetAI())
                        {
                            creature->SetPower(POWER_ENERGY, 0, true);
                            creature->AI()->SetData(coefficient, DATA_LINGERING_PRESENCE_COEF);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_lingering_presence_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lingering_presence_AuraScript();
        }
};

// Biting Cold - 136917
class spell_biting_cold_malakk : public SpellScriptLoader
{
    public:
        spell_biting_cold_malakk() : SpellScriptLoader("spell_biting_cold_malakk") { }

        class spell_biting_cold_malakk_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_biting_cold_malakk_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (Creature* caster = GetCaster()->ToCreature())
                    {
                        if (caster->GetEntry() == NPC_FROST_KING_MALAKK && caster->GetAI())
                            caster->AddAura(SPELL_BITING_COLD_AURA, target);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_biting_cold_malakk_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_biting_cold_malakk_SpellScript();
        }
};

// Frostbite - 136990
class spell_frosbite_malakk : public SpellScriptLoader
{
    public:
        spell_frosbite_malakk() : SpellScriptLoader("spell_frosbite_malakk") { }

        class spell_frosbite_malakk_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_frosbite_malakk_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (Creature* caster = GetCaster()->ToCreature())
                    {
                        if (caster->GetEntry() == NPC_FROST_KING_MALAKK && caster->GetAI())
                            caster->CastSpell(target, SPELL_FROSTBITE_PERIODIC, true);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_frosbite_malakk_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_frosbite_malakk_SpellScript();
        }
};

// Frostbite (aura) - 136922
class spell_frosbite_malakk_aura : public SpellScriptLoader
{
    public:
        spell_frosbite_malakk_aura() : SpellScriptLoader("spell_frosbite_malakk_aura") { }

        class spell_frosbite_malakk_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_frosbite_malakk_aura_AuraScript);

            InstanceScript* pInstance;

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Aura* frostBite = aurEff->GetBase())
                {
                    if (uint8 stack = frostBite->GetStackAmount())
                    {
                        if (Unit* target = GetTarget())
                        {
                            std::list<Player*> playerList;
                            target->GetPlayerListInGrid(playerList, 4.0f);
                            uint8 size = playerList.size();

                            if (pInstance = target->GetInstanceScript())
                            {
                                if (!pInstance->instance->IsHeroic())
                                {
                                    uint8 amount = std::max(1, (stack - (2 * size)));
                                    return;
                                }

                                for (auto player : playerList)
                                {
                                    if (player->HasAura(SPELL_BODY_HEAT))
                                        uint8 amount = std::max(1, (stack - (2 * size)));
                                }
                            }
                        }
                    }
                }
            }

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                {
                    std::list<Player*> playerList;
                    target->GetPlayerListInGrid(playerList, 4.0f);

                    if (pInstance = target->GetInstanceScript())
                    {
                        if (pInstance->instance->IsHeroic())
                        {
                            for (auto player : playerList)
                            {
                                if (!player->HasAura(SPELL_FROSTBITE_AURA) && !player->HasAura(SPELL_BODY_HEAT) && !player->HasAura(SPELL_CHILLED_TO_THE_BONE))
                                    player->AddAura(SPELL_BODY_HEAT, player);
                            }
                        }
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (!target->HasAura(SPELL_BODY_HEAT))
                        target->AddAura(SPELL_CHILLED_TO_THE_BONE, target);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_frosbite_malakk_aura_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE);
                OnEffectApply += AuraEffectApplyFn(spell_frosbite_malakk_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_frosbite_malakk_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_frosbite_malakk_aura_AuraScript();
        }
};

// Overload (aura) - 137149 / Discharge - 137166
class spell_overload_discharge_kazra_jin : public SpellScriptLoader
{
    public:
        spell_overload_discharge_kazra_jin() : SpellScriptLoader("spell_overload_discharge_kazra_jin") { }

        class spell_overload_discharge_kazra_jin_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_overload_discharge_kazra_jin_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_GENERIC_STUN, caster);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAura(SPELL_GENERIC_STUN);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case 137166:
                    OnEffectApply += AuraEffectApplyFn(spell_overload_discharge_kazra_jin_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_overload_discharge_kazra_jin_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    break;
                default:
                    OnEffectApply += AuraEffectApplyFn(spell_overload_discharge_kazra_jin_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    AfterEffectRemove += AuraEffectRemoveFn(spell_overload_discharge_kazra_jin_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_overload_discharge_kazra_jin_AuraScript();
        }
};

// Ensnared - 136878
class spell_ensnared : public SpellScriptLoader
{
    public:
        spell_ensnared() : SpellScriptLoader("spell_ensnared") { }

        class spell_ensnared_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ensnared_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Aura* ensnared = aurEff->GetBase())
                {
                    if (uint8 stack = ensnared->GetStackAmount())
                    {
                        if (Unit* caster = GetCaster())
                        {
                            std::list<Player*> playerList;
                            caster->GetPlayerListInGrid(playerList, 7.0f);

                            for (auto player: playerList)
                            {
                                if (stack >= 5)
                                {
                                    player->RemoveAura(SPELL_ENSNARED);
                                    player->AddAura(SPELL_ENTRAPPED, player);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_ensnared_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ensnared_AuraScript();
        }
};

// SandStorm - 136895
class spell_sandstorm : public SpellScriptLoader
{
    public:
        spell_sandstorm() : SpellScriptLoader("spell_sandstorm") { }

        class spell_sandstorm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sandstorm_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        std::list<Creature*> creatureList;
                        GetCreatureListWithEntryInGrid(creatureList, caster, NPC_LIVING_SAND, 200.0f);

                        for (auto livingSand : creatureList)
                        {
                            if (livingSand->GetAI())
                            {
                                if (livingSand->HasAura(SPELL_SAND_VISUAL))
                                    livingSand->AI()->DoAction(ACTION_SANDSTORM);
                                else
                                    livingSand->CastSpell(livingSand, SPELL_FORTIFIED, false);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sandstorm_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sandstorm_AuraScript();
        }
};

// Dark Power - 136507
class spell_dark_power : public SpellScriptLoader
{
    public:
        spell_dark_power() : SpellScriptLoader("spell_dark_power") { }

        class spell_dark_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_power_SpellScript);

            void HandleOnHit()
            {
                if (Unit* target = GetExplTargetUnit())
                    if (Creature* caster = GetCaster()->ToCreature())
                    {
                        if (caster->GetAI())
                        {
                            uint32 counter = caster->AI()->GetData(DATA_SPELL_DMG_MULTIPLIER);
                            SetHitDamage(GetSpellInfo()->Effects[0].BasePoints * 1 + (counter * 0.10));
                        }
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dark_power_SpellScript ::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_power_SpellScript();
        }
};

// Soul Fragment - 137641
class spell_soul_fragment : public SpellScriptLoader
{
    public:
        spell_soul_fragment() : SpellScriptLoader("spell_soul_fragment") { }

        class spell_soul_fragment_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_soul_fragment_AuraScript);

            InstanceScript* instance;

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (instance = caster->GetInstanceScript())
                    {
                        if (Creature* garaJal = instance->instance->GetCreature(instance->GetData64(NPC_GARA_JAL_SOUL)))
                        {
                            if (garaJal->GetAI())
                                caster->setFaction(garaJal->GetAI()->GetData(0));
                        }
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_soul_fragment_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_soul_fragment_AuraScript();
        }
};

// Soul Fragment (switch) - 137643
class spell_soul_fragment_switch : public SpellScriptLoader
{
    public:
        spell_soul_fragment_switch() :  SpellScriptLoader("spell_soul_fragment_switch") { }

        class spell_soul_fragment_switch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_soul_fragment_switch_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (target->ToPlayer())
                        {
                            if (caster->ToPlayer())
                            {
                                caster->RemoveAura(SPELL_SOUL_FRAGMENT);
                                caster->CastSpell(target, SPELL_SOUL_FRAGMENT, false);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_soul_fragment_switch_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_soul_fragment_switch_SpellScript();
        }
};

// Twisted Fate (first)- 137943
class spell_first_twisted_fate : public SpellScriptLoader
{
    public:
        spell_first_twisted_fate() :  SpellScriptLoader("spell_first_twisted_fate") { }

        class spell_first_twisted_fate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_first_twisted_fate_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint64 targetGuid = 0;

                if (Unit* caster = GetCaster())
                {
                    if (Creature* twistedFate = caster->ToCreature())
                    {
                        targetGuid = CAST_AI(mob_first_twisted_fate::mob_first_twisted_fateAI, twistedFate->AI())->summonerGuid;

                        if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                            twistedFate->SetDisplayId(target->GetDisplayId());
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_first_twisted_fate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_first_twisted_fate_SpellScript();
        }
};

/// Twisted Fate (second) - 137964
class spell_second_twisted_fate : public SpellScriptLoader
{
    public:
        spell_second_twisted_fate() :  SpellScriptLoader("spell_second_twisted_fate") { }

        class spell_second_twisted_fate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_second_twisted_fate_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint64 targetGuid = 0;

                if (Unit* caster = GetCaster())
                {
                    if (Creature* twistedFate = caster->ToCreature())
                    {
                        targetGuid = CAST_AI(mob_second_twisted_fate::mob_second_twisted_fateAI, twistedFate->AI())->summonerGuid;

                        if (Unit* target = ObjectAccessor::FindUnit(targetGuid))
                            twistedFate->SetDisplayId(target->GetDisplayId());
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_second_twisted_fate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_second_twisted_fate_SpellScript();
        }
};

// Twisted Fate (damage) - 137972
class spell_twisted_fate_damage : public SpellScriptLoader
{
    public:
        spell_twisted_fate_damage() :  SpellScriptLoader("spell_twisted_fate_damage") { }

        class spell_twisted_fate_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_twisted_fate_damage_SpellScript);

            void DealDamage()
            {
                Unit* caster = GetCaster();
                uint16 distance = 0;
                float distanceMax = 0;

                if (!caster)
                    return;

                if (Creature* twistedFate = caster->ToCreature())
                {
                    if (twistedFate->GetEntry() == NPC_FIRST_TWISTED_FATE)
                    {
                        if (Creature* creature = GetClosestCreatureWithEntry(twistedFate, NPC_SECOND_TWISTED_FATE, 200.0f))
                        {
                            distance = creature->GetDistance(twistedFate);
                            distanceMax = CAST_AI(mob_first_twisted_fate::mob_first_twisted_fateAI, twistedFate->AI())->distanceMax;
                        }
                    }

                    else if (twistedFate->GetEntry() == NPC_SECOND_TWISTED_FATE)
                    {
                        if (Creature* creature = GetClosestCreatureWithEntry(twistedFate, NPC_FIRST_TWISTED_FATE, 200.0f))
                        {
                            distance = creature->GetDistance(twistedFate);
                            distanceMax = CAST_AI(mob_second_twisted_fate::mob_second_twisted_fateAI, twistedFate->AI())->distanceMax;
                        }
                    }

                    if (distance >= 0.0f && distance <= distanceMax)
                        SetHitDamage(100000.0f + int32(250000.0f * (distance / distanceMax)));
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_twisted_fate_damage_SpellScript::DealDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_twisted_fate_damage_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_council_of_elders()
{
    new npc_gara_jal_s_soul();                  ///< 69182
    new boss_king_malakk();                     ///< 69131
    new boss_kazra_jin();                       ///< 69134
    new boss_sul_the_sandcrawler();             ///< 69078
    new boss_high_priestress_mar_li();          ///< 69132
    new mob_living_sand();                      ///< 69153
    new mob_blessed_loa_spirit();               ///< 69040
    new mob_shadowed_lua_spirit();              ///< 69548
    new mob_first_twisted_fate();               ///< 69740
    new mob_second_twisted_fate();              ///< 69746
    new spell_reckless_charge_rolling();        ///< 137117
    new spell_reckless_charge_movement();       ///< 137131
    new spell_frigid_assault();                 ///< 136904
    new spell_lingering_presence();             ///< 136467
    new spell_biting_cold_malakk();             ///< 136917
    new spell_frosbite_malakk();                ///< 136990
    new spell_frosbite_malakk_aura();           ///< 136922
    new spell_overload_discharge_kazra_jin();   ///< 137149 - 137166
    new spell_ensnared();                       ///< 136878
    new spell_sandstorm();                      ///< 136895
    new spell_dark_power();                     ///< 136507
    new spell_soul_fragment();                  ///< 137641
    new spell_soul_fragment_switch();           ///< 137643
    new spell_first_twisted_fate();             ///< 137943
    new spell_second_twisted_fate();            ///< 137964
    new spell_twisted_fate_damage();            ///< 137972
}
#endif