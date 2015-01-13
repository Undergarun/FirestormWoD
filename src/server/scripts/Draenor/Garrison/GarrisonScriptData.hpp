/// Copyright Ashran 2014-2015

#ifndef GARRISON_SCRIPT_DATA_HPP_GARRISON
#define GARRISON_SCRIPT_DATA_HPP_GARRISON

namespace MS { namespace Garrison {

    /// Instance data common IDs
    enum InstanceDataIDs
    {
        GARRISON_INSTANCE_DATA_PEON_ENABLED = 99,
        GARRISON_INSTANCE_DATA_THREE_FIRST  = 100,
        GARRISON_INSTANCE_DATA_THREE_END    = 110,
    };

    /// Garrison common spells
    enum Spells
    {
        SPELL_GARRISON_HEARTHSTONE             = 171253,
        SPELL_RELIEVED                         = 162410,
        SPELL_LEFT_ROPE                        = 164400,
        SPELL_RIGHT_ROPE                       = 164422,
        SPELL_COMESTIC_SLEEP                   = 162907,
        SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER = 161329
    };

    /// Garrison Quests
    enum Quests
    {
        /// Alliance
        QUEST_ETABLISH_YOUR_GARRISON_A  = 34586,
        QUEST_KEEPING_IT_TOGETHER       = 35176,
        QUEST_SHIP_SALVAGE              = 35166,
        QUEST_PALE_MOONLIGHT            = 35174,

        /// Horde
        QUEST_ETABLISH_YOUR_GARRISON_H  = 34378,
        QUEST_WHAT_WE_GOT               = 34824,
    };

    /// Garrison achievement related
    enum Achievements
    {
        ACHIEVEMENT_STAYING_REGULAR = 8933
    };

    /// Garrison NPCs
    enum NPCs
    {
        NPC_LADY_SENA                                   = 80223,
        NPC_GARRISON_ALLIANCE_CART                      = 81627,
        NPC_GARRISON_ALLIANCE_HORSE                     = 81633,
    };

    /// NPC texts id
    enum NPCTexts
    {
        NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_1        = 85492,
        NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_2        = 85490,
        NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_3        = 85493,
        NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST    = 83682,
        NPC_TEXT_SHELLY_HAMBY_TEXT_QUEST             = 83505,
    };

    /// Creature text index
    enum CreatureTexts
    {
        CREATURE_TEXT_BAROS_ETABLISH_YOUR_GARRISON_CHAT           = 0,
        CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_START_CHAT        = 1,
        CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_END_CHAT          = 2,
        CREATURE_TEXT_BAROS_SHIP_SALVAGE_START_CHAT               = 3,
        CREATURE_TEXT_BAROS_SHIP_SALVAGE_END_CHAT                 = 4,
        CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT = 0,
        CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT   = 1,
    };

    /// Items
    enum Items
    {
        ITEM_SHELLY_HAMBY_REPORT = 112730,
    };

    /// Garrison creation coords
    float gGarrisonCreationCoords[][4] =
    {
        { 1766.761475f,  191.2846830f,  72.115326f, 0.4649370f },   ///< TEAM_ALLIANCE
        { 5698.020020f, 4512.1635574f, 127.401695f, 2.8622720f }    ///< TEAM_HORDE
    };

    /// Garrison hearthstone teleport coords
    float gGarrisonHearthStoneTeleportCoord[][4] =
    {
        /// Horde
        { 5561.15f,  4508.785f, 132.70f,    3.406f   }, ///< Horde Level 1
        {    0.f,       0.f,      0.f,      0.000f   }, ///< Horde Level 2
        { 5563.70f,  4599.920f, 141.71100f, 0.000f   }, ///< Horde Level 3
        /// Alliance
        { 1850.71f,   254.430f,  78.08300f, 1.76f    }, ///< Alliance Level 1
        { 1947.08f,   324.883f,  90.28076f, 0.00f    }, ///< Alliance Level 2
        {    0.f,       0.f,      0.f,      0.00f    }, ///< Alliance Level 3
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SCRIPT_DATA_HPP_GARRISON