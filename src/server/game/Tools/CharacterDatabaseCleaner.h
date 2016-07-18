////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef CHARACTERDATABASECLEANER_H
#define CHARACTERDATABASECLEANER_H

namespace CharacterDatabaseCleaner
{
    enum CleaningFlags
    {
        CLEANING_FLAG_ACHIEVEMENT_PROGRESS  = 0x1,
        CLEANING_FLAG_SKILLS                = 0x2,
        CLEANING_FLAG_SPELLS                = 0x4,
        CLEANING_FLAG_TALENTS               = 0x8,
        CLEANING_FLAG_QUESTSTATUS           = 0x10
    };

    void CleanDatabase();

    void CheckUnique(const char* column, const char* table, bool (*check)(uint32));

    bool SkillCheck(uint32 skill);
    bool SpellCheck(uint32 spell_id);
    bool TalentCheck(uint32 talent_id);

    void CleanCharacterSkills();
    void CleanCharacterSpell();
    void CleanCharacterTalent();
    void CleanCharacterQuestStatus();
}

#endif
#endif
