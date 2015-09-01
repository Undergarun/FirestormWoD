////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Gruul - 76877
class boss_gruul_foundry : public CreatureScript
{
    public:
        boss_gruul_foundry() : CreatureScript("boss_gruul_foundry") { }

        enum eSpells
        {
            /// Misc
            GruulBonus  = 177529
        };

        enum eEvents
        {
        };

        enum eActions
        {
        };

        enum eCreatures
        {
        };

        enum eTalks
        {
            Intro,
            Aggro,
            Petrify,
            DestructiveRampage,
            Berserk,
            Slay,
            Death
        };

        struct boss_gruul_foundryAI : public BossAI
        {
            boss_gruul_foundryAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataGruul)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            EventMap m_Events;

            bool m_IntroDone;

            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                summons.DespawnAll();

                me->RemoveAura(eFoundrySpells::Berserker);

                _Reset();
            }

            void JustReachedHome() override
            {
                m_Events.Reset();
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::Aggro);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                m_Events.Reset();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    CastSpellToPlayers(me->GetMap(), me, eSpells::GruulBonus, true);

                    if (IsLFR())
                    {
                        Map::PlayerList const& l_PlayerList = m_Instance->instance->GetPlayers();
                        if (l_PlayerList.isEmpty())
                            return;

                        for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                        {
                            if (Player* l_Player = l_Itr->getSource())
                            {
                                uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                if (!me || l_Player->IsAtGroupRewardDistance(me))
                                    sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                            }
                        }

                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void EnterEvadeMode() override
            {
                me->RemoveAllAuras();

                me->InterruptNonMeleeSpells(true);

                /// Just in case, to prevent the fail Return to Home
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                me->ClearUnitState(UnitState::UNIT_STATE_DISTRACTED);
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eFoundryDatas::DataGruul, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (m_IntroDone)
                    return;

                if (p_Who->GetDistance(me) > 60.0f)
                    return;

                Talk(eTalks::Intro);
                m_IntroDone = true;
            }

            uint32 GetData(uint32 p_ID) override
            {
                return 0;
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_gruul_foundryAI(p_Creature);
        }
};

void AddSC_boss_gruul_foundry()
{
    /// Boss
    new boss_gruul_foundry();
}