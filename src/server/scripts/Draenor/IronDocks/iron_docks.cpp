////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

Position const g_StunnedAdds[3] =
{
    { 6848.13f, -557.67f, 5.134f, 0.167117f },
    { 6856.76f, -570.57f, 4.741f, 1.034980f },
    { 6868.90f, -571.93f, 5.115f, 2.466761f }
};

Position const g_CheeringSoldiers[6] =
{
    /// Right
    { 6807.25f, -676.79f, 4.835f, 5.925450f },
    { 6806.89f, -683.76f, 4.997f, 5.921500f },
    { 6806.85f, -690.98f, 4.922f, 6.282260f },
    /// Left
    { 6841.64f, -683.00f, 4.835f, 3.288370f },
    { 6841.33f, -690.54f, 4.835f, 3.146160f },
    { 6846.15f, -696.68f, 4.914f, 2.999297f }
};

Position const g_PracticingWarriors[2] =
{
    { 6826.10f, -688.61f, 4.835f, 2.412430f },
    { 6823.62f, -686.06f, 4.835f, 5.513440f }
};

Position const g_CheeringSoldiers2[10] =
{
    { 6620.58f, -676.56f, 4.599f, 3.959485f },
    { 6623.64f, -680.51f, 4.601f, 3.774916f },
    { 6624.83f, -687.18f, 4.618f, 3.158379f },
    { 6623.84f, -694.58f, 4.607f, 2.514352f },
    { 6620.52f, -697.97f, 4.615f, 2.202549f },
    { 6612.53f, -698.07f, 4.660f, 1.637062f },
    { 6604.38f, -697.10f, 4.623f, 0.887006f },
    { 6601.72f, -693.82f, 4.629f, 0.741707f },
    { 6600.86f, -687.21f, 4.658f, 6.278765f },
    { 6603.08f, -678.38f, 4.626f, 5.607255f }
};

Position const g_PracticingWarriors2[2] =
{
    { 6614.71f, -688.92f, 4.763f, 2.389464f },
    { 6611.09f, -685.70f, 4.773f, 5.494924f }
};

Position const g_OutTeleportPos = { 8852.49f, 1364.35f, 97.0f, 1.581f };

enum eSpells
{
    /// Battlemaster
    SpellBladestorm                     = 167232,
    SpellChainDrag                      = 172860,
    SpellChargingSlash                  = 172889,
    SpellChargingSlashJump              = 172885,
    /// Footsoldier
    SpellTacticalKick                   = 169413,
    SpellDemoralizingShout              = 169341,
    /// Deadeye
    SpellIronShot                       = 167239,
    SpellLegShot                        = 167240,
    /// Incinerator
    SpellIncenarySlugs                  = 176902, ///< Actual full spell, blizzards are just a bunch of r tards.
    SpellSharpnelBlast                  = 167516,
    /// Deckhand
    SpellHatchetToss                    = 173112,
    /// Champion Darona
    SpellBarbedArrowBarrageDummy        = 166923,
    SpellBarbedArrowAreaTrigger         = 164278,
    SpellBarbedArrowAura                = 164370,
    SpellBarbedArrowDoT                 = 164648,
    SpellBurningArrowDummy              = 172810,
    SpellBurningArrowAreaTrigger        = 164234,
    SpellBurningArrowDoT                = 164632,
    SpellChampionsPresence              = 173091,
    /// Siege Master Olugar
    SpellGatecrasherDamage              = 172963,
    SpellThrowGatecrasher               = 172952,
    SpellPitFighter                     = 173455, ///< Upon disarm
    SpellShatteringStrike               = 172982,
    /// Pitwarden Gwarnok
    SpellBrutalInspiration              = 172943,
    /// Gromkar Technician
    SpellFlyingHammer                   = 172703,
    SpellHighExplosiveGrenade           = 178298,
    SpellGreaseVial                     = 172649,
    SpellGreaseVialEffect               = 172636,
    /// Unruly Orgon
    SpellFlurry                         = 178412,
    SpellFlurryDamage                   = 178414,
    SpellThunderingStomp                = 173135,
    /// Rushing
    SpellRushingStampedeVisualMovement  = 173350,
    SpellRushingStampedeVisualHorning   = 173351,
    SpellRushingStampedeVisualJump      = 173352,
    SpellRushingStampedeDummyCast       = 173384,
    SpellRushingStampedeDamage          = 173351,
    SpellSpinyHorns                     = 158349,
    /// Thunderlord Wrangler
    SpellCultTraps                      = 173336,
    SpellCultTrapsDamage                = 173324,
    SpellRendingCleave                  = 167815,
    SpellSpearThrow                     = 167095,
    /// Ironwing Flamespitter
    SpellLavaBarrageDummy               = 176356,
    SpellLavaBarrageAreaTrigger         = 173482,
    SpellLavaBarrageDoT                 = 173489,
    /// Iron Star - Object
    SpellQuietSuicide                   = 163832,
    /// Misc
    SpellSelfStun                       = 94563,
    SpellEmoteWork                      = 43831,
    SpellEmoteFight                     = 36, ///< Actual standstate
    SpellCarryCrate                     = 173166,
    SpellCarrySack                      = 167539,
    SpellApplauseCheer                  = 84062
};

enum eEvents
{
    /// Thunderlord Wrangler
    EventCultTraps          = 1000,
    EventSpearThrow         = 1001,
    EventRendingCleave      = 1002,
    /// Rushing
    EventRushingStampede    = 1003,
    EventRushingStampede2   = 1004,
    /// Gromkar Battlemaster
    EventBladestorm         = 900,
    EventChainDrag          = 901,
    EventChargingSlash      = 902,
    /// Footsoldier
    EventTacticalKick       = 903,
    EventDemoralizingShout  = 904,
    /// Deadeye
    EventIronShot           = 905,
    EventLegShot            = 906,
    /// Incinerator
    EventIncendinarySlug    = 907,
    EventSharpnelBlast      = 908,
    /// Champion Daruna
    EventBurningArrow       = 909,
    EventBarbedArrow        = 910,
    EventChampionPresence   = 911,
    /// Siege Master Olugar
    EventGatecrasher        = 912,
    EventShatteringStrike   = 914,
    /// Gromkar Technician
    EventGreaseVial         = 916,
    EventFlyingHammer       = 917,
    EventExplosiveGrenade   = 918,
    /// Ogron
    EventFlurry             = 919,
    EventThunderingStomp    = 920,
    /// Deck-hand
    EventHatchetToss        = 921,
    /// Ironwing Flamespitter
    EventLavaBarrage        = 922
};

enum eAction
{
    ActionQuietDeath = 744
};

class explosion_iron_star : public BasicEvent
{
    public:
        explicit explosion_iron_star(Unit* p_Unit, Unit* p_OtherUnit, int p_Value) : m_Obj(p_Unit), m_OtherObj(p_OtherUnit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            switch (m_Modifier)
            {
                case 0:
                {
                    if (m_Obj)
                    {
                        if (Creature* l_Crea = m_Obj->ToCreature())
                            l_Crea->DespawnOrUnsummon();
                        m_Obj->CastSpell(m_Obj, eSpells::SpellQuietSuicide);
                    }

                    break;
                }
                case 1:
                {
                    if (m_Obj && m_OtherObj && m_OtherObj->IsInWorld())
                        m_OtherObj->EnterVehicle(m_Obj, 0, true);
                    break;
                }
                default:
                    break;
            }

            return true;
        }

    private:
        Creature* m_Storm;
        Unit* m_Obj;
        Unit* m_OtherObj;
        int m_Modifier;
};

/// Grom'kar Battle Master - 83025
class iron_docks_mob_gromkar_battlemaster : public CreatureScript
{
    public:
        iron_docks_mob_gromkar_battlemaster() : CreatureScript("iron_docks_mob_gromkar_battlemaster") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventChargingSlash, 10 * TimeConstants::IN_MILLISECONDS);
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
                    case eEvents::EventBladestorm:
                        me->CastSpell(me, eSpells::SpellBladestorm);
                        events.ScheduleEvent(eEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventChainDrag:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellChainDrag);
                        events.ScheduleEvent(eEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventChargingSlash:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellChargingSlashJump);
                        events.ScheduleEvent(eEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Dead Eye - 83028
/// Grom'kar Dead Eye - 83764
class iron_docks_mob_gromkar_deadeye : public CreatureScript
{
    public:
        iron_docks_mob_gromkar_deadeye() : CreatureScript("iron_docks_mob_gromkar_deadeye") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            uint32 m_VisualTimer;

            void Reset() override
            {
                m_VisualTimer = 5000;

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEvents::EventIronShot, urand(5 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventLegShot, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (Creature* l_AcheryTarget = me->FindNearestCreature(eIronDocksCreatures::TriggerArcheryTarget, 30.0f))
                        {
                            l_AcheryTarget->setFaction(1);
                            me->CastSpell(l_AcheryTarget, eSpells::SpellIronShot);
                        }

                        m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_VisualTimer -= p_Diff;

                    return;
                }

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventLegShot:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                            me->CastSpell(l_Target, eSpells::SpellLegShot);
                        events.ScheduleEvent(eEvents::EventLegShot, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventIronShot:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellIronShot);
                        events.ScheduleEvent(eEvents::EventIronShot, urand(5 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Foot Soldier - 83765
/// Grom'kar Footsoldier - 85997
/// Grom'kar Footsoldier - 859977
class iron_docks_mob_gromkar_footsoldier : public CreatureScript
{
    public:
        iron_docks_mob_gromkar_footsoldier() : CreatureScript("iron_docks_mob_gromkar_footsoldier") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void EnterCombat(Unit* who) override
            {
                events.ScheduleEvent(eEvents::EventTacticalKick, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventDemoralizingShout, 25 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));

                me->RemoveAura(eSpells::SpellSelfStun);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
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
                    case eEvents::EventDemoralizingShout:
                        me->CastSpell(me, eSpells::SpellDemoralizingShout);
                        events.ScheduleEvent(eEvents::EventDemoralizingShout, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventTacticalKick:
                        me->CastSpell(me->getVictim(), eSpells::SpellTacticalKick);
                        events.ScheduleEvent(eEvents::EventTacticalKick, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventChargingSlash:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellChargingSlashJump);
                        events.ScheduleEvent(eEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_iron_docksAI(creature);
        }
};

/// Grom'kar Incinerator - 86809
class iron_docks_mob_gromkar_incinerator : public CreatureScript
{
    public:
        iron_docks_mob_gromkar_incinerator() : CreatureScript("iron_docks_mob_gromkar_incinerator") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_VisualTimer;

            void Reset() override
            {
                m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

            }

            void EnterCombat(Unit* p_Who) override
            {
                events.ScheduleEvent(eEvents::EventIncendinarySlug, urand(15 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (Creature* l_ArcheryTarget = me->FindNearestCreature(eIronDocksCreatures::TriggerArcheryTarget, 30.0f))
                        {
                            l_ArcheryTarget->setFaction(1);
                            me->CastSpell(l_ArcheryTarget, eSpells::SpellIronShot);

                            m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                        }
                    }
                    else
                        m_VisualTimer -= p_Diff;

                    return;
                }

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventIncendinarySlug:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellIncenarySlugs);
                        events.ScheduleEvent(eEvents::EventIncendinarySlug, urand(15 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoSpellAttackIfReady(eSpells::SpellSharpnelBlast);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_iron_docksAI(creature);
        }
};

/// Grom'kar Technician - 81432
class iron_docks_mob_gromkar_technician : public CreatureScript
{
    public:
        iron_docks_mob_gromkar_technician() : CreatureScript("iron_docks_mob_gromkar_technician") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_VisualTimer;

            void Reset() override
            {
                m_VisualTimer = 6000;
            }

            void EnterCombat(Unit* p_Who) override
            {
                me->RemoveAura(eSpells::SpellEmoteWork);
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

                events.ScheduleEvent(eEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        me->RemoveAura(eSpells::SpellEmoteWork);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

                        switch (urand(0, 2))
                        {
                            case 0: ///< Work
                                me->CastSpell(me, eSpells::SpellEmoteWork);
                                break;
                            case 1: ///< Craft
                                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 133);
                                break;
                            case 2: ///< Idle
                                break;
                        }

                        m_VisualTimer = 6000;
                    }
                    else
                        m_VisualTimer -= p_Diff;

                    return;
                }

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventGreaseVial:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellGreaseVial);
                        events.ScheduleEvent(eEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventFlyingHammer:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellFlyingHammer);
                        events.ScheduleEvent(eEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventExplosiveGrenade:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellHighExplosiveGrenade);
                        events.ScheduleEvent(eEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Siegemaster Olugar - 83026
class iron_docks_siege_master_olugar : public CreatureScript
{
    public:
        iron_docks_siege_master_olugar() : CreatureScript("iron_docks_siege_master_olugar") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_VisualTimer;

            void Reset() override
            {
                m_VisualTimer = 16 * TimeConstants::IN_MILLISECONDS;
                me->SetReactState(ReactStates::REACT_DEFENSIVE);
            }

            void EnterCombat(Unit* p_Who) override
            {
                /// I don't think Darona actually shot barber arrows, wowhead is stupid. Icy veins says it aswell.
                events.ScheduleEvent(eEvents::EventShatteringStrike, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventGatecrasher, urand(15 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        me->MonsterSay("Stop showin' off.", Language::LANG_UNIVERSAL, me->GetGUID());
                        m_VisualTimer = 16 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_VisualTimer -= p_Diff;

                    return;
                }

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                /// Pit fighter
                if (me->HasAuraType(AuraType::SPELL_AURA_MOD_DISARM))
                {
                    if (!me->HasAura(eSpells::SpellPitFighter))
                        me->AddAura(eSpells::SpellPitFighter, me);
                }
                else
                {
                    if (me->HasAura(eSpells::SpellPitFighter))
                        me->RemoveAura(eSpells::SpellPitFighter);
                }

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventShatteringStrike:
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellShatteringStrike);
                        events.ScheduleEvent(eEvents::EventShatteringStrike, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventGatecrasher:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                            me->CastSpell(l_Target, eSpells::SpellThrowGatecrasher);
                        events.ScheduleEvent(eEvents::EventGatecrasher, urand(15 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Champion Druna - 81603
class iron_docks_mob_champion_darona : public CreatureScript
{
    public:
        iron_docks_mob_champion_darona() : CreatureScript("iron_docks_mob_champion_darona") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->CastSpell(me, eSpells::SpellChampionsPresence);
            }

            void EnterCombat(Unit* p_Who) override
            {
                /// I don't think Darona actually shot barber arrows, wowhead is stupid. Icy veins says it aswell.
                events.ScheduleEvent(eEvents::EventBurningArrow, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventBarbedArrow, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
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
                    case eEvents::EventBurningArrow:
                    {
                        me->CastSpell(me, eSpells::SpellBurningArrowDummy);
                        me->MonsterYell("Light them up!", Language::LANG_UNIVERSAL, me->GetGUID());
                        events.ScheduleEvent(eEvents::EventBurningArrow, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventBarbedArrow:
                    {
                        me->CastSpell(me, eSpells::SpellBarbedArrowBarrageDummy);
                        me->AddAura(eSpells::SpellBarbedArrowAura, me);
                        events.ScheduleEvent(eEvents::EventBarbedArrow, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Pitwarden Gwarnok - 84520
class iron_docks_gwarnok : public CreatureScript
{
    public:
        iron_docks_gwarnok() : CreatureScript("iron_docks_gwarnok") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_DEFENSIVE);
            }

            void EnterCombat(Unit* p_Who) override
            {
                events.ScheduleEvent(eEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));

                me->CastSpell(me, eSpells::SpellBrutalInspiration);
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
                    case eEvents::EventBladestorm:
                    {
                        me->CastSpell(me, eSpells::SpellBladestorm);
                        events.ScheduleEvent(eEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventChargingSlash:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                            me->CastSpell(l_Target, eSpells::SpellChargingSlashJump);
                        events.ScheduleEvent(eEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEvents::EventChainDrag:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                        {
                            me->CastSpell(l_Target, eSpells::SpellChainDrag);
                            l_Target->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 8.0f, 5.0f, 10.0f);
                        }
                        events.ScheduleEvent(eEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Iron Star - 81247
class iron_docks_mob_iron_star : public CreatureScript
{
    public:
        iron_docks_mob_iron_star() : CreatureScript("iron_docks_mob_iron_star") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->setFaction(35);

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionQuietDeath:
                        me->setFaction(35);
                        me->SetSpeed(UnitMoveType::MOVE_RUN, 12.0f, true);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FORCE_MOVEMENT);
                        DoAction(eAction::ActionQuietDeath);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Ogron Laborer - 83578
/// Ogron Laborer - 83761
/// Ogron Laborer - 583578
class iron_docks_mob_ogron : public CreatureScript
{
    public:
        iron_docks_mob_ogron() : CreatureScript("iron_docks_mob_ogron") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEvents::EventThunderingStomp, urand(5 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventFlurry, urand(8 * TimeConstants::IN_MILLISECONDS, 13 * TimeConstants::IN_MILLISECONDS));

                if (!urand(0, 2))
                    me->MonsterYell("CRUSH THEM!!", Language::LANG_UNIVERSAL, me->GetGUID());
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
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
                    case eEvents::EventThunderingStomp:
                        me->CastSpell(me, eSpells::SpellThunderingStomp);
                        events.ScheduleEvent(eEvents::EventThunderingStomp, urand(5 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventFlurry:
                        me->CastSpell(me->getVictim(), eSpells::SpellFlurry);
                        events.ScheduleEvent(eEvents::EventFlurry, urand(8 * TimeConstants::IN_MILLISECONDS, 13 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Deck-Hand - 83697
/// Grom'kar Deck-Hand - 83762
class iron_docks_mob_gromkar_deck_hand : public CreatureScript
{
    public:
        iron_docks_mob_gromkar_deck_hand() : CreatureScript("iron_docks_mob_gromkar_deck_hand") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                switch (urand(0, 1))
                {
                    case 0:
                        me->CastSpell(me, eSpells::SpellCarrySack);
                        break;
                    case 1:
                        me->CastSpell(me, eSpells::SpellCarryCrate);
                        break;
                }
            }

            void EnterCombat(Unit* p_Who) override
            {
                me->RemoveAura(eSpells::SpellEmoteWork);
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

                events.ScheduleEvent(eEvents::EventHatchetToss, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
            }

            void MoveInLineOfSight(Unit* who) override
            {
                if (who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(who, 1.2f))
                    who->Kill(me);
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
                    case eEvents::EventHatchetToss:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellHatchetToss);
                        events.ScheduleEvent(eEvents::EventHatchetToss, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Trigger_Summon_Cheering_Soldiers_2 - 99656
class iron_docks_stunned_cheering_practicing_trigger_second_segement : public CreatureScript
{
    public:
        iron_docks_stunned_cheering_practicing_trigger_second_segement() : CreatureScript("iron_docks_stunned_cheering_practicing_trigger_second_segement") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                /// Fighting Soldiers
                for (uint8 l_I = 0; l_I < 2; l_I++)
                {
                    if (Creature* l_FootSoldier = me->SummonCreature(eIronDocksCreatures::NpcGromkarFootSoldier, g_PracticingWarriors2[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                        l_FootSoldier->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eSpells::SpellEmoteFight);
                }

                /// Praciticng Soldiers
                for (uint8 l_I = 0; l_I < 10; l_I++)
                {
                    if (roll_chance_i(30))
                    {
                        if (Creature* l_CheeringSoldier = me->SummonCreature(eIronDocksCreatures::NpcGromkarFootSoldier, g_CheeringSoldiers2[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                            l_CheeringSoldier->CastSpell(l_CheeringSoldier, eSpells::SpellApplauseCheer);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Technician - 83763
class iron_docks_mob_gromkar_technician_deckhand_leader : public CreatureScript
{
public:
    iron_docks_mob_gromkar_technician_deckhand_leader() : CreatureScript("iron_docks_mob_gromkar_technician_deckhand_leader") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            /// Fighting Soldiers
            for (uint8 l_I = 0; l_I < 6; l_I++)
            {
                Position l_Pos;
                me->GetRandomNearPosition(l_Pos, 10.0f);

                std::list<Creature*> l_DeckhandList;

                if (Creature* l_Deckhand = me->SummonCreature(eIronDocksCreatures::NpcGromkarDeckhand, l_Pos, TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                {
                    l_Deckhand->GetMotionMaster()->MoveFollow(me, urand(1, 3), urand(40, 120), MovementSlot::MOTION_SLOT_ACTIVE);
                    l_DeckhandList.push_back(l_Deckhand);
                }

                for (Creature* l_Iter : l_DeckhandList)
                {
                    if (l_Iter->IsAIEnabled)
                        l_Iter->AI()->Reset();

                    m_Workers.push_back(l_Iter->GetGUID());
                }
            }
        }

        std::list<uint64> m_Workers;

        void Reset() override
        {
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.5, true);

            for (uint64 l_Iter : m_Workers)
            {
                if (Creature* l_Worker = Creature::GetCreature(*me, l_Iter))
                {
                    if (l_Worker->isAlive())
                        l_Worker->GetMotionMaster()->MoveFollow(me, urand(1, 3), urand(40, 120), MovementSlot::MOTION_SLOT_ACTIVE);
                }
            }
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->RemoveAura(eSpells::SpellEmoteWork);

            events.ScheduleEvent(eEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
            {
                for (uint64 itr : m_Workers)
                {
                    if (Creature* l_Worker = Unit::GetCreature(*me, itr))
                    {
                        if (l_Worker->isAlive())
                            l_Worker->GetMotionMaster()->MoveFollow(me, urand(1, 3), urand(40, 120), MovementSlot::MOTION_SLOT_ACTIVE);
                    }
                }

                return;
            }

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEvents::EventGreaseVial:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSpells::SpellGreaseVial);
                    events.ScheduleEvent(eEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eEvents::EventFlyingHammer:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSpells::SpellFlyingHammer);
                    events.ScheduleEvent(eEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eEvents::EventExplosiveGrenade:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSpells::SpellHighExplosiveGrenade);
                    events.ScheduleEvent(eEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new mob_iron_docksAI(p_Creature);
    }
};

/// Trigger_Summon_Cheering_Soldiers - 99655
class iron_docks_stunned_cheering_practicing_trigger : public CreatureScript
{
    public:
        iron_docks_stunned_cheering_practicing_trigger() : CreatureScript("iron_docks_stunned_cheering_practicing_trigger") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                /// Fighting Soldiers
                for (uint8 l_I = 0; l_I < 2; l_I++)
                {
                    if (Creature* l_FootSoldiers = me->SummonCreature(eIronDocksCreatures::NpcGromkarFootSoldier, g_PracticingWarriors[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                        l_FootSoldiers->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eSpells::SpellEmoteFight);
                }

                /// Praciticng Soldiers
                for (uint8 l_I = 0; l_I < 5; l_I++)
                {
                    if (Creature* l_CheeringSoldiers = me->SummonCreature(eIronDocksCreatures::NpcGromkarFootSoldier, g_CheeringSoldiers[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                        l_CheeringSoldiers->CastSpell(l_CheeringSoldiers, eSpells::SpellApplauseCheer);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Trigger_Summon_Stunned_Creatures - 99654
class iron_docks_stunned_soldiers_trigger : public CreatureScript
{
    public:
        iron_docks_stunned_soldiers_trigger() : CreatureScript("iron_docks_stunned_soldiers_trigger") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                std::list<Creature*> l_OldStunnedCreatures;
                me->GetCreatureListWithEntryInGrid(l_OldStunnedCreatures, eIronDocksCreatures::NpcGromkarFootSoldier2, 10.0f);

                for (Creature* l_Iter : l_OldStunnedCreatures)
                    l_Iter->DespawnOrUnsummon();

                for (uint8 l_I = 0; l_I <= 2; l_I++)
                {
                    if (Creature* l_FootSoldierStunned = me->SummonCreature(eIronDocksCreatures::NpcGromkarFootSoldier2, g_StunnedAdds[l_I], TempSummonType::TEMPSUMMON_DEAD_DESPAWN))
                        l_FootSoldierStunned->AddAura(eSpells::SpellSelfStun, l_FootSoldierStunned);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Trigger_Cannon_Event_Start - 99658
class iron_docks_trigger_stand_third_event : public CreatureScript
{
    public:
        iron_docks_trigger_stand_third_event() : CreatureScript("iron_docks_trigger_stand_third_event") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_CanEvent;

            void Reset() override
            {
                me->setFaction(35);
                m_CanEvent = false;
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 15.0f) && !m_CanEvent)
                {
                    m_CanEvent = true;

                    if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                        l_InstanceScript->SetData(DataThirdEvent, uint32(true));
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Trigger_Cannon_Crash_Fixed_Target - 99657
/// Trigger_Cannon_Crash_Fixed_Target - 100981
/// Trigger_Cannon_Crash_Fixed_Target - 100982
class iron_docks_trigger_cannon : public CreatureScript
{
    public:
        iron_docks_trigger_cannon() : CreatureScript("iron_docks_trigger_cannon") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            void Reset() override
            {
                me->setFaction(16);
                me->SetHealth(6000000);
                me->SetMaxHealth(6000000);
                me->GetMap()->SetObjectVisibility(1000.0f);
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Archery Target - 79423
class iron_docks_archery_target : public CreatureScript
{
    public:
        iron_docks_archery_target() : CreatureScript("iron_docks_archery_target") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_CANNOT_AUTOATTACK);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// 86534
class iron_docks_gatecrasher_trigger : public CreatureScript
{
    public:
        iron_docks_gatecrasher_trigger() : CreatureScript("iron_docks_gatecrasher_trigger") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            void Reset() override
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetDisplayId(11686);
                me->setFaction(16);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                /// Handle hacked areatrigger dmg (it appears to be blizzlike since dbc handler summon that add,
                /// But why the fuck would you do that if already summon an areatrigger? are they dumb? just areatrigger update blizzard, l2p
                std::list<Player*> l_NearPlrList;
                me->GetPlayerListInGrid(l_NearPlrList, 4.0f);

                for (Player* l_Iter : l_NearPlrList)
                {
                    if (!l_Iter->HasAura(eSpells::SpellGatecrasherDamage))
                    {
                        me->CastSpell(l_Iter, eSpells::SpellGatecrasherDamage);

                        if (AuraPtr l_AuraGatecrasher = l_Iter->GetAura(eSpells::SpellGatecrasherDamage))
                            l_AuraGatecrasher->SetDuration(1);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Thunderlord Wrangler - 83390
class iron_docks_mob_thundering_wandler : public CreatureScript
{
    public:
        iron_docks_mob_thundering_wandler() : CreatureScript("iron_docks_mob_thundering_wandler") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEvents::EventCultTraps, 18 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEvents::EventSpearThrow, urand(6 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eEvents::EventRendingCleave, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
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
                    case eEvents::EventCultTraps:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellCultTraps, true);
                        me->GetMotionMaster()->MoveKnockbackFrom(me->GetPositionX(), me->GetPositionY(), 10.0f, 8.0f);
                        events.ScheduleEvent(eEvents::EventCultTraps, 18 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventSpearThrow:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellSpearThrow);
                        events.ScheduleEvent(eEvents::EventSpearThrow, urand(6 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eEvents::EventRendingCleave:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSpells::SpellRendingCleave);
                        events.ScheduleEvent(eEvents::EventRendingCleave, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Rampaging Clefthoof - 83392
class iron_docks_mob_rampaging_clefthoof : public CreatureScript
{
    public:
        iron_docks_mob_rampaging_clefthoof() : CreatureScript("iron_docks_mob_rampaging_clefthoof") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint64 m_StampedeGuid;

            void Reset() override
            {
                m_StampedeGuid = 0;
                me->AddAura(eSpells::SpellSpinyHorns, me);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEvents::EventRushingStampede, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
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
                    case eEvents::EventRushingStampede:
                    {
                        me->CastSpell(me, eSpells::SpellRushingStampedeVisualMovement);

                        m_StampedeGuid = 0;

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Target, eSpells::SpellRushingStampedeDummyCast);
                            m_StampedeGuid = l_Target->GetGUID();
                        }

                        events.ScheduleEvent(eEvents::EventRushingStampede, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                        events.ScheduleEvent(eEvents::EventRushingStampede2, 2500);
                        break;
                    }
                    case eEvents::EventRushingStampede2:
                    {
                        if (Creature* l_StampedeTarget = Creature::GetCreature(*me, m_StampedeGuid))
                        {
                            me->GetMotionMaster()->MoveCharge(l_StampedeTarget->GetPositionX(), l_StampedeTarget->GetPositionY(), l_StampedeTarget->GetPositionZ(), 42.0f);
                            me->CastSpell(l_StampedeTarget, eSpells::SpellRushingStampedeVisualJump);
                            me->CastSpell(l_StampedeTarget, eSpells::SpellRushingStampedeDamage);
                            me->CastSpell(l_StampedeTarget, eSpells::SpellRushingStampedeVisualHorning);

                            me->GetMotionMaster()->Clear();
                        }

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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Ironwing Flamespitter - 83389
class iron_docks_mob_drake : public CreatureScript
{
    public:
        iron_docks_mob_drake() : CreatureScript("iron_docks_mob_drake") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEvents::EventLavaBarrage, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
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
                    case eEvents::EventLavaBarrage:
                        me->CastSpell(me, eSpells::SpellLavaBarrageDummy); // dummy
                        events.ScheduleEvent(eEvents::EventLavaBarrage, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Flurry - 178421
class iron_docks_spell_flurry_periodic : public SpellScriptLoader
{
    public:
        iron_docks_spell_flurry_periodic() : SpellScriptLoader("iron_docks_spell_flurry_periodic") { }

        class iron_docks_auras : public AuraScript
        {
            PrepareAuraScript(iron_docks_auras);

            void HandlePeriodic(constAuraEffectPtr p_AurEff)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlrList;
                    l_Caster->GetPlayerListInGrid(l_PlrList, 5.0f);

                    for (Player* l_Iter : l_PlrList)
                    {
                        if (l_Caster->isInFront(l_Iter, M_PI * 0.5f))
                            l_Caster->CastSpell(l_Iter, eSpells::SpellFlurryDamage);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_auras::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_auras();
        }
};

/// Launch Forward - 167299
class iron_docks_spell_charge_forward : public SpellScriptLoader
{
    public:
        iron_docks_spell_charge_forward() : SpellScriptLoader("iron_docks_spell_charge_forward") { }

        class iron_docks_spell_charge_forward_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_spell_charge_forward_SpellScript);

            void HandleCharge()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FORCE_MOVEMENT);

                    if (l_Caster->IsAIEnabled)
                        l_Caster->GetAI()->DoAction(eAction::ActionQuietDeath);

                    l_Caster->SetSpeed(UnitMoveType::MOVE_RUN, 12.0f, true);

                    l_Caster->m_Events.AddEvent(new explosion_iron_star(l_Caster, NULL, 0), l_Caster->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));

                    /// Start visual event
                    if (InstanceScript* l_InstanceScript = l_Caster->GetInstanceScript())
                        l_InstanceScript->SetData(DataSecondEvent, uint32(true));

                    l_Caster->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(iron_docks_spell_charge_forward_SpellScript::HandleCharge);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spell_charge_forward_SpellScript();
        }
};

/// Iron Shot - 167239
class iron_docks_spell_shoot_damage_decrease_on_dummies : public SpellScriptLoader
{
    public:
        iron_docks_spell_shoot_damage_decrease_on_dummies() : SpellScriptLoader("iron_docks_spell_shoot_damage_decrease_on_dummies") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (l_Target->GetEntry() == eIronDocksCreatures::TriggerArcheryTarget)
                        PreventHitDamage();
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Barbed Arrow Barrage - 164370
class iron_docks_spell_barbed_arrow_aura : public SpellScriptLoader
{
    public:
        iron_docks_spell_barbed_arrow_aura() : SpellScriptLoader("iron_docks_spell_barbed_arrow_aura") { }

        class iron_docks_Spells : public AuraScript
        {
            PrepareAuraScript(iron_docks_Spells);

            void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->IsAIEnabled)
                        return;

                    if (Unit* target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        l_Caster->CastSpell(target, eSpells::SpellBarbedArrowAreaTrigger);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_Spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_Spells();
        }
};

/// Burning Arrows - 172810
class iron_docks_spell_barbed_arrow_dummy : public SpellScriptLoader
{
    public:
        iron_docks_spell_barbed_arrow_dummy() : SpellScriptLoader("iron_docks_spell_barbed_arrow_dummy") { }

        class iron_docks_spell_barbed_arrow_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_spell_barbed_arrow_dummy_SpellScript);

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpells::SpellBarbedArrowAura);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(iron_docks_spell_barbed_arrow_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spell_barbed_arrow_dummy_SpellScript();
        }
};

/// Burning Arrows - 172810
class iron_docks_spell_burning_arrow_aura : public SpellScriptLoader
{
    public:
        iron_docks_spell_burning_arrow_aura() : SpellScriptLoader("iron_docks_spell_burning_arrow_aura") { }

        class iron_docks_Spells : public AuraScript
        {
            PrepareAuraScript(iron_docks_Spells);

            uint8 m_Counter;

            bool Load() override
            {
                m_Counter = 0;
                return true;
            }

            void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (m_Counter < 3)
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        if (!l_Caster->IsAIEnabled)
                            return;

                        if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        {
                            l_Caster->CastSpell(l_Target, eSpells::SpellBurningArrowAreaTrigger);
                            m_Counter++;
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_Spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_Spells();
        }
};

/// Charging Slash - 172885
class iron_docks_spell_charging_slash_effect : public SpellScriptLoader
{
    public:
        iron_docks_spell_charging_slash_effect() : SpellScriptLoader("iron_docks_spell_charging_slash_effect") { }

        class spells_iron_docks : public SpellScript
        {
            PrepareSpellScript(spells_iron_docks);

            void OnHitTarget()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SpellChargingSlash);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spells_iron_docks::OnHitTarget);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spells_iron_docks();
        }
};

class spell_barrage_targets_los_fix : public SpellScriptLoader
{
    public:
        spell_barrage_targets_los_fix() : SpellScriptLoader("spell_barrage_targets_los_fix") { }

        class spell_barrage_targets_spell_script : public SpellScript
        {
            PrepareSpellScript(spell_barrage_targets_spell_script);

            bool Load() override
            {
                SpellInfo* l_SpellInfo = const_cast<SpellInfo*>(GetSpellInfo());
                l_SpellInfo->AttributesEx2 += SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                return true;
            }

            void Register() override
            {
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_barrage_targets_spell_script();
        }
};

class iron_docks_lava_barrage_dummy : public SpellScriptLoader
{
    public:
        iron_docks_lava_barrage_dummy() : SpellScriptLoader("iron_docks_lava_barrage_dummy") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->IsAIEnabled)
                        return;

                    for (uint8 l_I = 0; l_I < 3; l_I++)
                    {
                        if (Unit* target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            l_Caster->CastSpell(target, eSpells::SpellLavaBarrageAreaTrigger, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(iron_docks_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Burning Arrows - 164628
class iron_docks_area_trigger_burning_arrow : public AreaTriggerEntityScript
{
    public:
        iron_docks_area_trigger_burning_arrow() : AreaTriggerEntityScript("iron_docks_area_trigger_burning_arrow") { }

        uint32 m_Timer = 500;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;

                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 3.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(3.0f, l_Searcher);

                if (l_PlayerList.empty())
                    return;

                for (Player* l_Iter : l_PlayerList)
                {
                    if (!l_Iter->HasAura(eSpells::SpellBurningArrowDoT))
                    {
                        l_Iter->CastSpell(l_Iter, eSpells::SpellBurningArrowDoT, true);
                        m_Targets.push_back(l_Iter->GetGUID());
                    }
                }

                m_Timer = 500;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eSpells::SpellBurningArrowDoT))
                    l_Target->RemoveAura(eSpells::SpellBurningArrowDoT);
            }
        }

        iron_docks_area_trigger_burning_arrow* GetAI() const override
        {
            return new iron_docks_area_trigger_burning_arrow();
        }
};

/// Lava Barrage - 173484
class iron_docks_area_lava_barrage_effect : public AreaTriggerEntityScript
{
    public:
        iron_docks_area_lava_barrage_effect() : AreaTriggerEntityScript("iron_docks_area_lava_barrage_effect") { }

        uint32 m_Timer = 500;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;

                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 3.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(3.0f, l_Searcher);

                if (l_PlayerList.empty())
                    return;

                for (Player* l_Iter : l_PlayerList)
                {
                    if (!l_Iter->HasAura(eSpells::SpellLavaBarrageDoT))
                    {
                        l_Iter->CastSpell(l_Iter, eSpells::SpellLavaBarrageDoT);
                        m_Targets.push_back(l_Iter->GetGUID());
                    }
                }

                m_Timer = 500;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eSpells::SpellLavaBarrageDoT))
                    l_Target->RemoveAura(eSpells::SpellLavaBarrageDoT);
            }
        }

        iron_docks_area_lava_barrage_effect* GetAI() const override
        {
            return new iron_docks_area_lava_barrage_effect();
        }
};

/// Barbed Arrow Barrage - 164278
class iron_docks_area_trigger_barbed_arrow : public AreaTriggerEntityScript
{
    public:
        iron_docks_area_trigger_barbed_arrow() : AreaTriggerEntityScript("iron_docks_area_trigger_barbed_arrow") { }

        uint32 m_Timer = 500;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;

                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 3.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(3.0f, l_Searcher);

                if (l_PlayerList.empty())
                    return;

                for (Player* l_Iter : l_PlayerList)
                {
                    if (!l_Iter->HasAura(eSpells::SpellBarbedArrowDoT))
                    {
                        l_Iter->CastSpell(l_Iter, eSpells::SpellBarbedArrowDoT);
                        m_Targets.push_back(l_Iter->GetGUID());
                    }
                }

                m_Timer = 500;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eSpells::SpellBarbedArrowDoT))
                    l_Target->RemoveAura(eSpells::SpellBarbedArrowDoT);
            }
        }

        iron_docks_area_trigger_barbed_arrow* GetAI() const override
        {
            return new iron_docks_area_trigger_barbed_arrow();
        }
};

class iron_docks_area_trigger_oil_effect : public AreaTriggerEntityScript
{
    public:
        iron_docks_area_trigger_oil_effect() : AreaTriggerEntityScript("iron_docks_area_trigger_oil_effect") { }

        uint32 m_Timer = 1000;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;

                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 3.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(3.0f, l_Searcher);

                if (l_PlayerList.empty())
                    return;

                for (Player* l_Iter : l_PlayerList)
                {
                    if (!l_Iter->HasAura(eSpells::SpellGreaseVialEffect))
                    {
                        l_Iter->CastSpell(l_Iter, eSpells::SpellGreaseVialEffect);
                        m_Targets.push_back(l_Iter->GetGUID());
                    }
                }

                m_Timer = 1000;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eSpells::SpellGreaseVialEffect))
                    l_Target->RemoveAura(eSpells::SpellGreaseVialEffect);
            }
        }

        iron_docks_area_trigger_oil_effect* GetAI() const override
        {
            return new iron_docks_area_trigger_oil_effect();
        }
};

/// Jagget Caltrops - 173325
class iron_docks_area_trigger_jagged_caltrops : public AreaTriggerEntityScript
{
    public:
        iron_docks_area_trigger_jagged_caltrops() : AreaTriggerEntityScript("iron_docks_area_trigger_jagged_caltrops") { }

        uint32 m_Timer = 1500;
        std::list<uint64> m_Targets;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;

                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);

                if (l_PlayerList.empty())
                    return;

                for (Player* l_Iter : l_PlayerList)
                {
                    if (!l_Iter->HasAura(eSpells::SpellCultTrapsDamage))
                    {
                        l_Iter->CastSpell(l_Iter, eSpells::SpellCultTrapsDamage);
                        m_Targets.push_back(l_Iter->GetGUID());

                        if (l_Iter->HasAura(eSpells::SpellCultTrapsDamage))
                        {
                            if (AuraPtr l_Aura = l_Iter->GetAura(eSpells::SpellCultTrapsDamage))
                                l_Aura->SetDuration(3);
                        }
                    }
                }

                m_Timer = 1500;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eSpells::SpellCultTrapsDamage))
                    l_Target->RemoveAura(eSpells::SpellCultTrapsDamage);
            }
        }

        iron_docks_area_trigger_jagged_caltrops* GetAI() const override
        {
            return new iron_docks_area_trigger_jagged_caltrops();
        }
};

/// Instance Portal (Raid: Normal, Heroic, Mythic) - 232490
class go_iron_docks_instance_portal : public GameObjectScript
{
    public:
        go_iron_docks_instance_portal() : GameObjectScript("go_iron_docks_instance_portal") { }

        struct go_iron_docks_instance_portalAI : public GameObjectAI
        {
            go_iron_docks_instance_portalAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject)
            {
                m_CheckTimer = 1000;
            }

            uint32 m_CheckTimer;

            void UpdateAI(uint32 p_Diff) override
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= p_Diff)
                    {
                        m_CheckTimer = 1000;

                        std::list<Player*> l_PlayerList;
                        go->GetPlayerListInGrid(l_PlayerList, 5.0f);

                        for (Player* l_Player : l_PlayerList)
                            l_Player->TeleportTo(1116, g_OutTeleportPos);
                    }
                    else
                        m_CheckTimer -= p_Diff;
                }
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_iron_docks_instance_portalAI(p_GameObject);
        }
};

void AddSC_iron_docks_cpp()
{
    /// NPCs
    new iron_docks_mob_gromkar_battlemaster();
    new iron_docks_mob_gromkar_deadeye();
    new iron_docks_mob_gromkar_footsoldier();
    new iron_docks_mob_gromkar_incinerator();
    new iron_docks_mob_gromkar_technician();
    new iron_docks_siege_master_olugar();
    new iron_docks_mob_champion_darona();
    new iron_docks_gwarnok();
    new iron_docks_mob_iron_star();
    new iron_docks_mob_ogron();
    new iron_docks_mob_gromkar_deck_hand();
    new iron_docks_stunned_cheering_practicing_trigger_second_segement();
    new iron_docks_mob_gromkar_technician_deckhand_leader();
    new iron_docks_stunned_cheering_practicing_trigger();
    new iron_docks_stunned_soldiers_trigger();
    new iron_docks_trigger_stand_third_event();
    new iron_docks_trigger_cannon();
    new iron_docks_archery_target();
    new iron_docks_gatecrasher_trigger();
    new iron_docks_mob_thundering_wandler();
    new iron_docks_mob_rampaging_clefthoof();
    new iron_docks_mob_drake();

    /// Spells
    new iron_docks_spell_flurry_periodic();
    new iron_docks_spell_charge_forward();
    new iron_docks_spell_shoot_damage_decrease_on_dummies();
    new iron_docks_spell_barbed_arrow_aura();
    new iron_docks_spell_barbed_arrow_dummy();
    new iron_docks_spell_burning_arrow_aura();
    new iron_docks_spell_charging_slash_effect();
    new spell_barrage_targets_los_fix();
    new iron_docks_lava_barrage_dummy();

    /// AreaTriggers
    new iron_docks_area_trigger_burning_arrow();
    new iron_docks_area_lava_barrage_effect();
    new iron_docks_area_trigger_barbed_arrow();
    new iron_docks_area_trigger_oil_effect();
    new iron_docks_area_trigger_jagged_caltrops();

    /// GameObject
    new go_iron_docks_instance_portal();
}