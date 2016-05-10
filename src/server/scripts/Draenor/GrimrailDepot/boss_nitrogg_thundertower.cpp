////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "grimrail_depot.hpp"

enum eNitroggThundertowerTalks
{
    TalkAggro = 1,          ///< Ready!!! [46109]
    TalkDeath,              ///< i didn't ... even ... try .. [46110]
    TalkCommanderEvent01,   ///< Let'em have it boys! [46111]
    TalkCommanderEvent02,   ///< Give'em all you got! [46112]
    TalkCommanderEvent03,   ///< I've got my sights on you! [46113]
    TalkCommanderEvent04,   ///< RAPID FIRE!!! [46114]
    TalkCommanderEvent05,   ///< Hah! Let's take this down to your level [46115]
    TalkIntro01,            ///< Who dares challenge me? [46116]
    TalkIntro02,            ///< They are here get up! [46117]
    TalkIntro03,            ///< Formation! now!! [46118]
    TalkKill01,             ///<  Ha ha ha Another contender down! [46120]
    TalkKill02              ///<  So weak! [46121]
};

enum eNitroggThundertowerActions
{

};

enum eNitroggThundertowerMomvents
{

};

/// Nitrogg Thunder Tower - 79545
class boss_nitrogg_thundertower : public CreatureScript
{
public:

    boss_nitrogg_thundertower() : CreatureScript("boss_nitrogg_thundertower") { }

    struct boss_nitrogg_thundertowerAI : public BossAI
    {
        boss_nitrogg_thundertowerAI(Creature* p_Creature) : BossAI(p_Creature, GrimrailDepotData::DataNitroggThundertower)
        {
            m_Instance = me->GetInstanceScript();
            m_First = false;
        }

        enum eNitroggThundertowerSpells
        {
            SpellBlackrockGrenadeTriggerMissileA = 163539,
            SpellBlackrockGrenadeTriggerMissileB = 161150,
            SpellBlackrockGrenadeDamage          = 161153,
            SpellBlackrockMortarDummy            = 163550,
            SpellBlackrockMortarTriggerMissile   = 163541,
            SpellBlackrockMortarDamage           = 163741,
            SpellBlackrockMunitionsDummy         = 167709,
            SpellFlametongueDummy                = 176031,
            SpellFlametongueTriggerMissile       = 176032,
            SpellFlametongueAura                 = 176033,
            SpellFlametongue                     = 176039,
            SpellLavaWreath                      = 176027,
            SpellLavaWreathDummy                 = 176025,
            SpellShrapnelBlast                   = 160943
        };

        enum eNitroggThundertowerEvents
        {
            EventBlackrockGrenade = 1,
            EventBlackrockMortar,
            EventFlametongue,
            EventLavaWreath,
            EventSharpnelBlast 
        };

        InstanceScript* m_Instance;
        bool m_First;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            if (m_Instance != nullptr)
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

            _EnterCombat();
            Talk(eNitroggThundertowerTalks::TalkAggro);
            events.ScheduleEvent(eNitroggThundertowerEvents::EventBlackrockGrenade, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNitroggThundertowerEvents::EventBlackrockMortar, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNitroggThundertowerEvents::EventFlametongue, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNitroggThundertowerEvents::EventLavaWreath, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNitroggThundertowerEvents::EventSharpnelBlast, 0 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eNitroggThundertowerTalks::TalkKill01);
                else
                    Talk(eNitroggThundertowerTalks::TalkKill02);
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_nitrogg_thundertowerAI(p_Creature);
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_nitrogg_thundertower()
{
    new boss_nitrogg_thundertower();
}
#endif
