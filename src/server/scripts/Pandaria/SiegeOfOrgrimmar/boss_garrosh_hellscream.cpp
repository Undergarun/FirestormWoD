/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSays
{
    /*
    Garrosh Hellscream yells: Enjoy the power of the arsenal of the true Horde.
    Garrosh Hellscream yells: Behold as I did the visions of fear , despair and doubt !
    Garrosh Hellscream yells: Okay, so you want to get a true measure orc warlord . As you wish .
    Garrosh Hellscream yells: Low.
    Garrosh Hellscream yells: Fool , my dark shamans have corrupt elements and tortured for miles around. They will not hear you. Once again , you show too weak and unable to act.
    Garrosh Hellscream yells: Anger . Hatred. Fear! These are weapons of war, tools warlord !
    Garrosh Hellscream yells: The real Horde WILL reality. I VIEW . HE HAS SHOWN ME . I'VE SEEN THE MOUNTAINS SKULLS AND THE RIVERS OF BLOOD . I WILL ... MY WORLD ... !
    Garrosh Hellscream yells: The heart will be your end !
    Garrosh Hellscream yells: The power flows in me!
    Garrosh Hellscream yells: The power to crush your world and everything in it !
    Garrosh Hellscream yells: The Very old calling you !
    Garrosh Hellscream yells: Long blind , heal our wounds !
    Garrosh Hellscream yells: Now , you know your place in my new world order.
    Garrosh Hellscream yells: I, Garrosh , son of Grom , I'll show you why I 'm Hellscream !
    Garrosh Hellscream yells: Die with honor !
    Garrosh Hellscream yells: No, it can not end ... like this. This is not ... This is not what I saw ...
    Garrosh Hellscream yells: We purify this world with fire and steel!
    Garrosh Hellscream yells: We are the orcish Horde , the true Horde. We die covered in blood and the gun in his hand on the battlefield , as the REAL orcs are supposed to die .
    Garrosh Hellscream yells: Yes ... Yes, I see it now ... I see the future of this world ... A world led by MY ... Horde Horde.
    Garrosh Hellscream yells: Pathetic.
    Garrosh Hellscream yells: For true Horde!
    Garrosh Hellscream yells: Blood and thunder !
    Garrosh Hellscream yells: You, you're not an orc and your word that you no longer agrees . You betrayed ours to seal fragile alliances , and I will take great pleasure to shatter .
    Garrosh Hellscream yells: You will be trapped for eternity!
    */
};

enum eSpells
{
    /*
    SPELL_ANNIHILAT                     = 144969, // Inflicts 3500000 Shadow damage to enemies within 0 yards in front of the caster and 351000 to 369000 Shadow damage to all enemies in the Realm of Y'Shaarj.
    SPELL_BERSERK_RAID_FINDER           = 64238,  //  Increases the caster's attack speed by 150% and all damage it deals by 900% for 30 min.
    SPELL_CALL_BOMBARDEMENT_25          = 147120, // 2 sec cast
    SPELL_CRUSHING_FEAR_10              = 147324, // Inflicts 390000 to 410000 Shadow damage and inflicts fear on enemies within 8 yards for 3 sec.
    SPELL_DESECRATE                     = 144748, // Garrosh throws the Desecrated Weapon at a random target.
    SPELL_EMPOWERED_DESECRATE           = 145829, // Garrosh has reached 75 energy and Desecrate is now Empowered!
    SPELL_EMPOWERED_DESECRATE           = 144749, // Garrosh throws the Empowered Desecrated Weapon at a random target.
    SPELL_EMPOWERED_DESECRATE           = 144816, // Inflicts 146250 to 153750 Shadow damage to enemies within 15 yards and creates a Empowered Desecrated Weapon at the location.
    SPELL_EMPOWERED_GRIPPING_DESPAIR    = 145195, // Inflicts 30000 Shadow damage every 1 sec for 10 sec. When Empowered Gripping Despair expires or is removed it causes Explosive Despair.
    SPELL_EMPOWERED_GRIPPING_DESPAIR    = 145831, // Garrosh has reached 100 energy and Gripping Despair is now Empowered!
    SPELL_EMPOWERED_TOUCH_OF_YSHAARJ    = 145171, // The Touch of Y'shaarj mind controls the target, causing them to seek out nearby allies and attempt to convert them as well. Dealing sufficient damage to the mind-controlled player will break the effect.
    SPELL_EMPOWERED_TOUCH_OF_YSHAARJ    = 145832, // Garrosh has reached 50 energy and Touch of Y'Shaarj is now Empowered!
    SPELL_EMPOWERED_WHIRLING_CORRUPTION = 145037, // Garrosh begins whirling, inflicting 102375 to 107625 Shadow damage to nearby enemies every 0.5 sec. The damage inflicted decreases the farther away an enemy is from Garrosh.
    SPELL_EMPOWERED_WHIRLING_CORRUPTION = 145833, // Garrosh has reached 25 energy and Whirling Corruption is now Empowered!
    SPELL_GRIPPING_DESPAIR              = 145183, // Inflicts 30000 Shadow damage every 1 sec for 10 sec. Stacks.
    SPELL_HELLSCREAMS_WARSONG           = 144821, // Hellscream's Warsong inspires his allies, increasing Physical damage dealt by 150% and increasing health by 150%.
    SPELL_MALEFICE_25	                = 147209, // Fires Malicious Blast at the closest 2 allies every 2 sec. Any Malicious Blast that is not absorbed will be fired at Garrosh instead, granting him 5 Energy.
    SPELL_MANIFEST_RAGE_25              = 147011, // 2 sec cast
    SPELL_REALM_OF_YSHAARJ 	            = 144954, // In the realm of Y'Shaarj! Stop Garrosh from Absorbing Power! Transported out of the Realm of Y'Shaarj after 1.02 min.
    SPELL_SUMMON_FADING_BREATH          = 147296, // Instant
    SPELL_TOUCH_OH_YSHAARJ              = 145065, // The Touch of Y'shaarj mind controls the target, causing them to seek out nearby allies and attempt to convert them as well. Dealing sufficient damage to the mind-controlled player will break the effect.
    SPELL_TOUCH_OH_YSHAARJ	            = 145071, // The Touch of Y'shaarj mind controls the target, causing them to seek out nearby allies and attempt to convert them as well. Dealing sufficient damage to the mind-controlled player will break the effect.
    SPELL_WEAK_MINDED	                = 145331, // Weak minded beings cannot enter the Realm of Y'shaarj.
    SPELL_WHIRLING_CORRUPTION           = 144985, // Garrosh begins whirling, inflicting 102375 to 107625 Shadow damage to nearby enemies every 0.5 sec. The damage inflicted decreases the farther away the enemy is from Garrosh.
    SPELL_SHAARJS_PROTECTION            = 144945  //Immune to damage.Defeating the Sha minions will dispel the protection.
    */
};

enum eEvents 
{
};

/*
       phase1
{
	SPELL_DESECRATE                     =144748,
	SPELL_HELLSCREAM'S_WARSONG          =144821,
	SPELL_KOR'_KRON_WARBRINGER          =71979,
	SPELL_WOLF_RIDER_FARSEER          =71983,
}

	    phase2

{
	SPELL__WHIRLING_CORRUPTION               =144985,
    SPELL_EMPOWERED_WHIRLING_CORRUPTION      =145037,
	SPELL_EMPOWERED_TOUCH_OF_Y'_SHAARJ       =145171,
	SPELL_EMPOWERED_TOUCH_OF_Y'_SHAARJ       =145832,
	SPELL_SPELL_DESECRATE                    =144748,
	SPELL_EMPOWERED_DESECRATE                =144816,
	SPELL_GRIPPING_DESPAIR                   =145183,
}

		phase3
{
   SPELL_EMPOWERED_WHIRLING_CORRUPTION =145037,
   SPELL_EMPOWERED_TOUCH_OF_Y'_SHAARJ  =145171,
   SPELL_EMPOWERED_DESECRATE           =144816,
   SPELL_GRIPPING_DESPAIR              =145183,
   SPELL_EXPOLSIVE_DESPAIR             =145199,
}

       phaseInterval 1-2
{
   SPELL_Y'SHAARJ_'S_PROTECTION  =144945,
   SPELL_ANNIHILATE              =144969,
   SPELL_CONSUMED_COURAGE        =149011,
   SPELL_CONSUMED_FAITH          =149033,
   SPELL_CONSUMED_HOPE           =149032,
}
*/

class boss_garrosh_hellscream : public CreatureScript
{
    public:
        boss_garrosh_hellscream() : CreatureScript("boss_garrosh_hellscream") { }

        struct boss_garrosh_hellscreamAI : public BossAI
        {
            boss_garrosh_hellscreamAI(Creature* creature) : BossAI(creature, DATA_GARROSH_HELLSCREAM)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_garrosh_hellscreamAI(creature);
        }
};

/*
				  //Start summoning adds
                    if (uiSpawnAddTimer <= diff) {
                        DoScriptText(SAY_Garrosh Hellscream yells: Enjoy the power of the arsenal of the true Horde., me);

				  // Spawn Warbringer
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_KOR'_KRON_WARBRINGER , LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_KOR'_KRON_WARBRINGER , LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_KOR'_KRON_WARBRINGER , UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_KOR'_KRON_WARBRINGER , UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }

						 //Start summoning adds
                    if (uiSpawnAddTimer <= diff) {
                        DoScriptText(SAY_Garrosh Hellscream yells: Behold as I did the visions of fear , despair and doubt !, me)

						 // Spawn FARSEER
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_WOLF_RIDER_FARSEER , LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_WOLF_RIDER_FARSEER , LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_WOLF_RIDER_FARSEER , UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_WOLF_RIDER_FARSEER R, UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }
*/

class mob_siege_engineer : public CreatureScript
{
    public:
        mob_siege_engineer() : CreatureScript("mob_siege_engineer") { }

        struct mob_siege_engineerAI : public ScriptedAI
        {
            mob_siege_engineerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_siege_engineerAI(creature);
        }
};

class mob_korkron_warbringer : public CreatureScript
{
    public:
        mob_korkron_warbringer() : CreatureScript("mob_korkron_warbringer") { }

        struct mob_korkron_warbringerAI : public ScriptedAI
        {
            mob_korkron_warbringerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korkron_warbringerAI(creature);
        }
};

class mob_wolf_rider_farseer : public CreatureScript
{
    public:
        mob_wolf_rider_farseer() : CreatureScript("mob_wolf_rider_farseer") { }

        struct mob_wolf_rider_farseerAI : public ScriptedAI
        {
            mob_wolf_rider_farseerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_wolf_rider_farseerAI(creature);
        }
};

void AddSC_Garrosh_Hellscream()
{
    new boss_garrosh_hellscream();
    new mob_siege_engineer();
    new mob_korkron_warbringer();
    new mob_wolf_rider_farseer();
}
