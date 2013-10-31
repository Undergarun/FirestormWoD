using System;

namespace WowPacketParser.Enums
{
    [Flags]
    enum SpellHitType
    {
        SPELL_HIT_TYPE_UNK1     = 0x00000001,
        SPELL_HIT_TYPE_CRIT     = 0x00000002,
        SPELL_HIT_TYPE_UNK3     = 0x00000004,
        SPELL_HIT_TYPE_UNK4     = 0x00000008,
        SPELL_HIT_TYPE_UNK5     = 0x00000010,
        SPELL_HIT_TYPE_UNK6     = 0x00000020,
    };

    [Flags]
    enum SpellSchools
    {
        SPELL_SCHOOL_NORMAL = 0,
        SPELL_SCHOOL_HOLY   = 1,
        SPELL_SCHOOL_FIRE   = 2,
        SPELL_SCHOOL_NATURE = 3,
        SPELL_SCHOOL_FROST  = 4,
        SPELL_SCHOOL_SHADOW = 5,
        SPELL_SCHOOL_ARCANE = 6
    };

    [Flags]
    enum SpellSchoolMask
    {
        SPELL_SCHOOL_MASK_NONE = 0x00,                       // not exist
        SPELL_SCHOOL_MASK_NORMAL = (1 << 0), // PHYSICAL (Armor)
        SPELL_SCHOOL_MASK_HOLY = (1 << 1),
        SPELL_SCHOOL_MASK_FIRE = (1 << 2),
        SPELL_SCHOOL_MASK_NATURE = (1 << 3),
        SPELL_SCHOOL_MASK_FROST = (1 << 4),
        SPELL_SCHOOL_MASK_SHADOW = (1 << 5),
        SPELL_SCHOOL_MASK_ARCANE = (1 << 6),

        // unions

        // 124, not include normal and holy damage
        SPELL_SCHOOL_MASK_SPELL = (SPELL_SCHOOL_MASK_FIRE |
        SPELL_SCHOOL_MASK_NATURE | SPELL_SCHOOL_MASK_FROST |
        SPELL_SCHOOL_MASK_SHADOW | SPELL_SCHOOL_MASK_ARCANE),
        // 126
        SPELL_SCHOOL_MASK_MAGIC = (SPELL_SCHOOL_MASK_HOLY | SPELL_SCHOOL_MASK_SPELL),

        // 127
        SPELL_SCHOOL_MASK_ALL = (SPELL_SCHOOL_MASK_NORMAL | SPELL_SCHOOL_MASK_MAGIC)
    };
}
