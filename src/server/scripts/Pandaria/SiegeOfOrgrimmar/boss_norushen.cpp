////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    SPELL_BLIND_HATRED           = 145226,
    SPELL_DESPAIR                = 145725,
    SPELL_FOU_LINK               = 148974,
    SPELL_FRAYED                 = 146179,
    SPELL_FUSION                 = 145132,
    SPELL_SELF_DOUBT             = 146124,
    SPELL_UNCHECKED_CORRUPTION   = 145679,
    SPELL_UNLEASH_CORRUPTION     = 145769,
    SPELL_UNLEASHED_ANGER        = 145216
};

enum eEvents
{
};

class boss_norushen : public CreatureScript
{
    public:
        boss_norushen() : CreatureScript("boss_norushen") { }

        struct boss_norushenAI : public BossAI
        {
            boss_norushenAI(Creature* creature) : BossAI(creature, DATA_NORUSHEN)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_norushenAI(creature);
        }
};

class boss_amalgam_of_corruption : public CreatureScript
{
    public:
        boss_amalgam_of_corruption() : CreatureScript("boss_amalgam_of_corruption") { }

        struct boss_amalgam_of_corruptionAI : public BossAI
        {
            boss_amalgam_of_corruptionAI(Creature* creature) : BossAI(creature, DATA_NORUSHEN)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_amalgam_of_corruptionAI(creature);
        }
};

class mob_essence_of_corruption : public CreatureScript
{
    public:
        mob_essence_of_corruption() : CreatureScript("mob_essence_of_corruption") { }

        struct mob_essence_of_corruptionAI : public ScriptedAI
        {
            mob_essence_of_corruptionAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_essence_of_corruptionAI(creature);
        }
};

class mob_manifestation_of_corruption : public CreatureScript
{
    public:
        mob_manifestation_of_corruption() : CreatureScript("mob_manifestation_of_corruption") { }

        struct mob_manifestation_of_corruptionAI : public ScriptedAI
        {
            mob_manifestation_of_corruptionAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_manifestation_of_corruptionAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_norushen()
{
    new boss_norushen();
    new boss_amalgam_of_corruption();
    new mob_essence_of_corruption();
    new mob_manifestation_of_corruption();
}

#endif
/*
Upon entering:
Lorewalker Cho says: Astounding! Another chamber that looks to originate from these "Titans."
Lorewalker Cho says: It must have been hidden away long before recorded history. And with good reason!

Intro:
Lorewalker Cho says: In the chamber ahead, under our peaceful land, slept the heart of an Old God.
Norushen yells: Halt!
Lorewalker Cho says: Oh my! What is this? Hello! I am Lorewalker Cho!
Norushen yells: No further corruption will enter the heart chamber!
Lorewalker Cho says: Further corruption? No, we are here to stop the corruption, and save Pandaria!
Norushen yells: You wish to purge the corruption?
Lorewalker Cho says: Yes! Please let us pass!
Norushen yells: Should you pass this door, at this time, you would fail! You, all of you, are corrupted with the insidious plague known as Pride.
Norushen yells: You stand tall and proud atop your accomplishments, and this will be your downfall.
Norushen yells: Should you wish to defeat the corruption, you will first need to purify the corruption within yourselves.
Norushen yells: Speak to me again when you are prepared to face your inner demons.

Aggro:
Norushen yells: Very well, I will create a field to keep your corruption quarantined.
Norushen yells: Prove yourselves worthy and I will let you pass.
Norushen yells: The light cleanses; but it is not gentle. Gird yourselves and be purified!

Encourage:
Norushen says: Be purified in the light!
Norushen says: Face your inner demons!
Norushen says: Look inside yourself and cleanse the darkness within.
Norushen says: Stand in the light and be judged!

Player killed:
Norushen yells: Unworthy.
Norushen yells: Only the pure of heart may pass!

Wipe:
Norushen yells: The light will not suffer your taint.

Outro:
Norushen yells: You have been judged, and proven yourselves worthy. But be warned, what lies beyond will try your souls to their utmost. When you are fully prepared, you may enter the chamber.
*/

/*
-- Norushen
INSERT INTO creature_text VALUES
(71965, 0, 0, "Halt!", 14, 0, 100, 0, 0, 38883, 'NORUSHEN_INTRO_1'),
(71965, 0, 1, "No further corruption will enter the heart chamber!", 14, 0, 100, 0, 0, 38884, 'NORUSHEN_INTRO_2'),
(71965, 0, 2, "You wish to purge the corruption?", 14, 0, 100, 0, 0, 38885, 'NORUSHEN_INTRO_3'),
(71965, 0, 3, "Should you pass this door, at this time, you would fail! You, all of you, are corrupted with the insidious plague known as Pride.", 14, 0, 100, 0, 0, 38886, 'NORUSHEN_INTRO_4'),
(71965, 0, 4, "You stand tall and proud atop your accomplishements, and this will be your downfall.", 14, 0, 100, 0, 0, soundId, 'NORUSHEN_INTRO_5'),
(71965, 0, 5, "Should you wish to defeat the corruption, you will first need to purify the corruption within yourselves.", 14, 0, 100, 0, 0, soundId, 'NORUSHEN_INTRO_6'),
(71965, 0, 6, "Speak to me again when you are prepared to face your inner demons.", 14, 0, 100, 0, 0, 38887, 'NORUSHEN_INTRO_7'),
(71965, 1, 0, "Very well, I will create a field to keep your corruption quarantined.", 14, 0, 100, 0, 0, 38888, 'NORUSHEN_AGGRO_1'),
(71965, 1, 1, "Prove yourselves worthy and I will let you pass.", 14, 0, 100, 0, 0, 38889, 'NORUSHEN_AGGRO_2'),
(71965, 1, 2, "The light cleanses; but it is not gentle. Gird yourselves and be purified!", 14, 0, 100, 0, 0, 38880, 'NORUSHEN_AGGRO_3'),
(71965, 2, 0, "Be purified in the light!", 14, 0, 100, 0, 0, 38894, 'NORUSHEN_ENCOURAGE_1'),
(71965, 2, 1, "Face your inner demons!", 14, 0, 100, 0, 0, 38893, 'NORUSHEN_ENCOURAGE_2'),
(71965, 2, 2, "Look inside yourself and cleanse the darkness within.", 14, 0, 100, 0, 0, 38892, 'NORUSHEN_ENCOURAGE_3'),
(71965, 2, 3, "Stand in the light and be judged!", 14, 0, 100, 0, 0, 38895, 'NORUSHEN_ENCOURAGE_4'),
(71965, 3, 0, "Unworthy.", 14, 0, 100, 0, 0, 38890, 'NORUSHEN_SLAY_1'),
(71965, 3, 1, "Only the pure of heart may pass!", 14, 0, 100, 0, 0, 38891, 'NORUSHEN_SLAY_2'),
(71965, 4, 0, "The light will not suffer your taint.", 14, 0, 100, 0, 0, 38881, 'NORUSHEN_WIPE'),
(71965, 5, 0, "You have been judged, and proven yourselves worthy. But be warned, what lies beyond will try your souls to their utmost. When you are fully prepared, you may enter the chamber.", 14, 0, 100, 0, 0, 38882, 'NORUSHEN_OUTRO');
*/
