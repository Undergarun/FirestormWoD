////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "grimrail_depot.hpp"

enum eSkylordTorvaTalks
{
	TalkAggro = 1,        ///< Rakun, lets give'em hell! [45878]
	TalkDeath,            ///< You ... Can't ... [45879]
	TalkGeneralEvent01,   ///< Another one! [45880]
	TalkGeneralEvent02,   ///< Light'em up!  [45881]
	TalkGeneralEvenet03,  ///< Show them what you're made of! [45882]
	TalkIntro,            ///< Nobody stops the iron Horde, Nobody [45883]
	TalkKill01,           ///< Hell shockin' [45884]
	TalkKill02            ///< Haaa [458845]
};

enum eSkylordTorvaActions
{
	
};

enum eSkylordTorvaCreatures
{
	
};

enum eSkylordTorvaMomvents
{
};

/// SkyLord Torva - 80005
class boss_skylord_torva : public CreatureScript
{
public:

	boss_skylord_torva() : CreatureScript("boss_skylord_torva") { }

	struct boss_skylord_torvaAI : public BossAI
	{
		boss_skylord_torvaAI(Creature* p_Creature) : BossAI(p_Creature, GrimrailDepotData::DataSkyLordTovra)
		{
			m_Instance = me->GetInstanceScript();
			m_First = false;
		}

		enum eSkylordTorvaSpells
		{
			SpellDiffusedEnergyDummy             = 161558,
			SpellDiffusedEnergyAura              = 161588,
			SpellDiffusedEnergyAreaTrigger       = 161589,
			SpellFreezingSnareAreaTrigger        = 162059,
			SpellFreezingSnareDummy              = 162062,
			SpellFreezingSnareDamage             = 162065,
			SpellFreezingSnare                   = 162066,
			SpellFreezingSnareTriggerMissile     = 162080,
			SpellSpinningSpearDummy              = 162055,
			SpellSpinningSpearDamage             = 162057,
			SpellSpinningSpearAreaTrigger        = 162058,
			SpellFarSight                        = 166364,
			SpellHealingRain                     = 166387,
			SpellHealingRainDummy                = 166388,
			SpellStormShieldAura                 = 166335,
			SpellStormShieldDamage               = 166336,
			SpellThunderZoneAura                 = 166340,
			SpellThunderZoneDummy                = 166346,
			SpellArcaneBlitzAura                 = 166397,
			SpellArcaneBlitzTriggerMissileA      = 166398,
			SpellArcaneBlitzTriggerMissileB      = 166399,
			SpellArcaneBlitzDamage               = 166404,
			SpellScoutingAHead                   = 166350
		};

		enum eSkylordTorvaEvents
		{
			EventDiffusedEnergy = 1,
			EventFreezingSnare,
			EventSpinningSpear,
			EventArcaneBlitz,
			EventScoutingAHead,
			EventFarSight,
			EventHealingRain,
			EventStormShield,
			EventThunderZone 
		};

        InstanceScript* m_Instance;
        bool m_First;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (m_Instance != nullptr)
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

            _EnterCombat();
            Talk(eSkylordTorvaTalks::TalkAggro);
            events.ScheduleEvent(eSkylordTorvaEvents::EventDiffusedEnergy, 5 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSkylordTorvaEvents::EventFreezingSnare, 5 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSkylordTorvaEvents::EventSpinningSpear, 5* TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eSkylordTorvaTalks::TalkKill01);
                else
                    Talk(eSkylordTorvaTalks::TalkKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eSkylordTorvaTalks::TalkDeath);
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
        return new boss_skylord_torvaAI(p_Creature);
    }
};

void AddSC_boss_skylord_tovra()
{
    new boss_skylord_torva();
}