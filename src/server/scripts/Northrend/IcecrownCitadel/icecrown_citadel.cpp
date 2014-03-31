/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "SpellAuraEffects.h"
#include "SmartAI.h"
#include "icecrown_citadel.h"

// Weekly quest support
// * Deprogramming                (DONE)
// * Securing the Ramparts        (DONE)
// * Residue Rendezvous           (DONE)
// * Blood Quickening             (DONE)
// * Respite for a Tormented Soul

enum Texts
{
    // Highlord Tirion Fordring (at Light's Hammer)
    SAY_TIRION_INTRO_1              = 0,
    SAY_TIRION_INTRO_2              = 1,
    SAY_TIRION_INTRO_3              = 2,
    SAY_TIRION_INTRO_4              = 3,
    SAY_TIRION_INTRO_H_5            = 4,
    SAY_TIRION_INTRO_A_5            = 5,

    // The Lich King (at Light's Hammer)
    SAY_LK_INTRO_1                  = 0,
    SAY_LK_INTRO_2                  = 1,
    SAY_LK_INTRO_3                  = 2,
    SAY_LK_INTRO_4                  = 3,
    SAY_LK_INTRO_5                  = 4,

    // Highlord Bolvar Fordragon (at Light's Hammer)
    SAY_BOLVAR_INTRO_1              = 0,

    // High Overlord Saurfang (at Light's Hammer)
    SAY_SAURFANG_INTRO_1            = 15,
    SAY_SAURFANG_INTRO_2            = 16,
    SAY_SAURFANG_INTRO_3            = 17,
    SAY_SAURFANG_INTRO_4            = 18,

    // Muradin Bronzebeard (at Light's Hammer)
    SAY_MURADIN_INTRO_1             = 13,
    SAY_MURADIN_INTRO_2             = 14,
    SAY_MURADIN_INTRO_3             = 15,

    // Deathbound Ward
    SAY_TRAP_ACTIVATE               = 0,

    // Rotting Frost Giant
    EMOTE_DEATH_PLAGUE_WARNING      = 0,

    // Sister Svalna
    SAY_SVALNA_KILL_CAPTAIN         = 1, // happens when she kills a captain
    SAY_SVALNA_KILL                 = 4,
    SAY_SVALNA_CAPTAIN_DEATH        = 5, // happens when a captain resurrected by her dies
    SAY_SVALNA_DEATH                = 6,
    EMOTE_SVALNA_IMPALE             = 7,
    EMOTE_SVALNA_BROKEN_SHIELD      = 8,

    SAY_CROK_INTRO_1                = 0, // Ready your arms, my Argent Brothers. The Vrykul will protect the Frost Queen with their lives.
    SAY_ARNATH_INTRO_2              = 5, // Even dying here beats spending another day collecting reagents for that madman, Finklestein.
    SAY_CROK_INTRO_3                = 1, // Enough idle banter! Our champions have arrived - support them as we push our way through the hall!
    SAY_SVALNA_EVENT_START          = 0, // You may have once fought beside me, Crok, but now you are nothing more than a traitor. Come, your second death approaches!
    SAY_CROK_COMBAT_WP_0            = 2, // Draw them back to us, and we'll assist you.
    SAY_CROK_COMBAT_WP_1            = 3, // Quickly, push on!
    SAY_CROK_FINAL_WP               = 4, // Her reinforcements will arrive shortly, we must bring her down quickly!
    SAY_SVALNA_RESURRECT_CAPTAINS   = 2, // Foolish Crok. You brought my reinforcements with you. Arise, Argent Champions, and serve the Lich King in death!
    SAY_CROK_COMBAT_SVALNA          = 5, // I'll draw her attacks. Return our brothers to their graves, then help me bring her down!
    SAY_SVALNA_AGGRO                = 3, // Come, Scourgebane. I'll show the master which of us is truly worthy of the title of "Champion"!
    SAY_CAPTAIN_DEATH               = 0,
    SAY_CAPTAIN_RESURRECTED         = 1,
    SAY_CAPTAIN_KILL                = 2,
    SAY_CAPTAIN_SECOND_DEATH        = 3,
    SAY_CAPTAIN_SURVIVE_TALK        = 4,
    SAY_CROK_WEAKENING_GAUNTLET     = 6,
    SAY_CROK_WEAKENING_SVALNA       = 7,
    SAY_CROK_DEATH                  = 8,
};

enum Spells
{
    // Rotting Frost Giant
    SPELL_DEATH_PLAGUE              = 72879,
    SPELL_DEATH_PLAGUE_AURA         = 72865,
    SPELL_RECENTLY_INFECTED         = 72884,
    SPELL_DEATH_PLAGUE_KILL         = 72867,
    SPELL_STOMP                     = 64652,
    SPELL_ARCTIC_BREATH             = 72848,

    // Frost Freeze Trap
    SPELL_COLDFLAME_JETS            = 70460,

    // Alchemist Adrianna
    SPELL_HARVEST_BLIGHT_SPECIMEN   = 72155,
    SPELL_HARVEST_BLIGHT_SPECIMEN25 = 72162,

    // Crok Scourgebane
    SPELL_ICEBOUND_ARMOR            = 70714,
    SPELL_SCOURGE_STRIKE            = 71488,
    SPELL_DEATH_STRIKE              = 71489,

    // Sister Svalna
    SPELL_CARESS_OF_DEATH           = 70078,
    SPELL_IMPALING_SPEAR_KILL       = 70196,
    SPELL_REVIVE_CHAMPION           = 70053,
    SPELL_UNDEATH                   = 70089,
    SPELL_IMPALING_SPEAR            = 71443,
    SPELL_AETHER_SHIELD             = 71463,
    SPELL_DIVINE_SURGE              = 71465,
    SPELL_AETHER_BURST_10           = 71468,
    SPELL_AETHER_BURST_25           = 71469,
    SPELL_HURL_SPEAR                = 71466,

    // Captain Arnath
    SPELL_DOMINATE_MIND             = 14515,
    SPELL_FLASH_HEAL_NORMAL         = 71595,
    SPELL_POWER_WORD_SHIELD_NORMAL  = 71548,
    SPELL_SMITE_NORMAL              = 71546,
    SPELL_FLASH_HEAL_UNDEAD         = 71782,
    SPELL_POWER_WORD_SHIELD_UNDEAD  = 71780,
    SPELL_SMITE_UNDEAD              = 71778,

    // Captain Brandon
    SPELL_CRUSADER_STRIKE           = 71549,
    SPELL_DIVINE_SHIELD             = 71550,
    SPELL_JUDGEMENT_OF_COMMAND      = 71551,
    SPELL_HAMMER_OF_BETRAYAL        = 71784,

    // Captain Grondel
    SPELL_CHARGE                    = 71553,
    SPELL_MORTAL_STRIKE             = 71552,
    SPELL_SUNDER_ARMOR              = 71554,
    SPELL_CONFLAGRATION             = 71785,

    // Captain Rupert
    SPELL_FEL_IRON_BOMB_NORMAL      = 71592,
    SPELL_MACHINE_GUN_NORMAL        = 71594,
    SPELL_ROCKET_LAUNCH_NORMAL      = 71590,
    SPELL_FEL_IRON_BOMB_UNDEAD      = 71787,
    SPELL_MACHINE_GUN_UNDEAD        = 71788,
    SPELL_ROCKET_LAUNCH_UNDEAD      = 71786,

    // Invisible Stalker (Float, Uninteractible, LargeAOI)
    SPELL_SOUL_MISSILE              = 72585,

    //----------- PLAGUE WORKS -----------//
    // Blighted Abomination
    SPELL_CLEAVE                    = 40504,
    SPELL_PLAGUE_CLOUD              = 71150,
    SPELL_SCOURGE_HOOK              = 71140,

    // Vengeful Fleshreaper
    SPELL_LEAPING_FACE_MAUL         = 71164,
    SPELL_DEVOUR_HUMANOID           = 71164,

    // Plague Scientist
    SPELL_PLAGUE_BLAST              = 73079,
    SPELL_PLAGUE_STREAM             = 69871,
    SPELL_COMBOBULATING_SPRAY       = 71103,

    // Pustulating Horror
    SPELL_BLIGHT_BOMB               = 71088,
    SPELL_BUBBLING_PUS_10N          = 71089,
    SPELL_BUBBLING_PUS_25N          = 71090,

    // Decaying Colossus
    SPELL_MASSIVE_STOMP_10N         = 71114,
    SPELL_MASSIVE_STOMP_25N         = 71115,

    //-------- SANCTUARY OF BLOOD --------//
    // Darkfallen Archmage
    SPELL_AMPLIFY_MAGIC_10N         = 70408,
    SPELL_AMPLIFY_MAGIC_25N         = 72336,
    SPELL_BLAST_WAVE_10N            = 70407,
    SPELL_BLAST_WAVE_25N            = 71151,
    SPELL_FIREBALL_10N              = 70409,
    SPELL_FIREBALL_25N              = 71153,
    SPELL_POLYMORPH_SPIDER          = 70410,
    SPELL_SIPHON_ESSENCE            = 70299,

    // Darkfallen Blood Knight
    SPELL_VAMPIRIC_AURA             = 71736,
    SPELL_UNHOLY_STRIKE             = 70437,
    SPELL_BLOOD_MIRROR_DAMAGE       = 70445,
    SPELL_BLOOD_MIRROR_BUFF         = 70451,
    SPELL_BLOOD_MIRROR_DUMMY        = 70450,

    // Darkfallen Noble
    SPELL_CHAINS_OF_SHADOW          = 70645,
    SPELL_SHADOW_BOLT_10N           = 72960,
    SPELL_SHADOW_BOLT_25N           = 72961,

    // Vampiric Fiend
    SPELL_DISEASE_CLOUD             = 41290,
    SPELL_LEECHING_ROT              = 70671,

    // Darkfallen Advisor
    SPELL_LICH_SLAP_10N             = 72057,
    SPELL_LICH_SLAP_25N             = 72421,
    SPELL_SHROUD_OF_PROTECTION      = 72065,
    SPELL_SHROUD_OF_SPELL_WARDING   = 72066,

    // Darkfallen Commander
    SPELL_VAMPIRE_RUSH_10N          = 70449,
    SPELL_VAMPIRE_RUSH_25N          = 71155,
    SPELL_BATTLE_SHOUT              = 70750,

    // Darkfallen Lieutenant
    SPELL_VAMPIRIC_CURSE            = 70423,
    SPELL_REND_FLESH_10N            = 70435,
    SPELL_REND_FLESH_25N            = 71154,

    // Darkfallen Tactician
    SPELL_SHADOWSTEP                = 70431,
    SPELL_BLOOD_SAP                 = 70432,

    //------------ FROST WING ------------//
    // Ymirjar Deathbringer
    SPELL_BANISH                    = 71298,
    SPELL_DEATHS_EMBRACE_10N        = 71299,
    SPELL_DEATHS_EMBRACE_25N        = 71300,
    SPELL_SHADOW_BOLT_YMIRJAR_10N   = 71296,
    SPELL_SHADOW_BOLT_YMIRJAR_25N   = 71297,
    SPELL_SUMMON_YMIRJAR            = 71303,

    // Ymirjar Frostbinder
    SPELL_ARCTIC_CHILL              = 71270,
    SPELL_FROZEN_ORB                = 71274,
    SPELL_TWISTED_WINDS             = 71306,
    SPELL_SPIRIT_STREAM             = 69929,

    // Ymirjar Battle-Maiden
    SPELL_ADRENALINE_RUSH           = 71258,
    SPELL_BARBARIC_STRIKE           = 71257,

    // Ymirjar Huntress
    SPELL_ICE_TRAP                  = 71249,
    SPELL_RAPID_SHOT                = 71251,
    SPELL_SHOOT                     = 71253,
    SPELL_VOLLEY                    = 71252,
    SPELL_SUMMON_WARHAWK            = 71705,

    //Ymirjar Warlord
    SPELL_WHIRLWIND                 = 41056,
};

// Helper defines
// Captain Arnath
#define SPELL_FLASH_HEAL        (IsUndead ? SPELL_FLASH_HEAL_UNDEAD : SPELL_FLASH_HEAL_NORMAL)
#define SPELL_POWER_WORD_SHIELD (IsUndead ? SPELL_POWER_WORD_SHIELD_UNDEAD : SPELL_POWER_WORD_SHIELD_NORMAL)
#define SPELL_SMITE             (IsUndead ? SPELL_SMITE_UNDEAD : SPELL_SMITE_NORMAL)

// Captain Rupert
#define SPELL_FEL_IRON_BOMB     (IsUndead ? SPELL_FEL_IRON_BOMB_UNDEAD : SPELL_FEL_IRON_BOMB_NORMAL)
#define SPELL_MACHINE_GUN       (IsUndead ? SPELL_MACHINE_GUN_UNDEAD : SPELL_MACHINE_GUN_NORMAL)
#define SPELL_ROCKET_LAUNCH     (IsUndead ? SPELL_ROCKET_LAUNCH_UNDEAD : SPELL_ROCKET_LAUNCH_NORMAL)

enum EventTypes
{
    // Highlord Tirion Fordring (at Light's Hammer)
    // The Lich King (at Light's Hammer)
    // Highlord Bolvar Fordragon (at Light's Hammer)
    // High Overlord Saurfang (at Light's Hammer)
    // Muradin Bronzebeard (at Light's Hammer)
    EVENT_TIRION_INTRO_2                = 1,
    EVENT_TIRION_INTRO_3                = 2,
    EVENT_TIRION_INTRO_4                = 3,
    EVENT_TIRION_INTRO_5                = 4,
    EVENT_LK_INTRO_1                    = 5,
    EVENT_TIRION_INTRO_6                = 6,
    EVENT_LK_INTRO_2                    = 7,
    EVENT_LK_INTRO_3                    = 8,
    EVENT_LK_INTRO_4                    = 9,
    EVENT_BOLVAR_INTRO_1                = 10,
    EVENT_LK_INTRO_5                    = 11,
    EVENT_SAURFANG_INTRO_1              = 12,
    EVENT_TIRION_INTRO_H_7              = 13,
    EVENT_SAURFANG_INTRO_2              = 14,
    EVENT_SAURFANG_INTRO_3              = 15,
    EVENT_SAURFANG_INTRO_4              = 16,
    EVENT_SAURFANG_RUN                  = 17,
    EVENT_MURADIN_INTRO_1               = 18,
    EVENT_MURADIN_INTRO_2               = 19,
    EVENT_MURADIN_INTRO_3               = 20,
    EVENT_TIRION_INTRO_A_7              = 21,
    EVENT_MURADIN_INTRO_4               = 22,
    EVENT_MURADIN_INTRO_5               = 23,
    EVENT_MURADIN_RUN                   = 24,

    // Rotting Frost Giant
    EVENT_DEATH_PLAGUE                  = 25,
    EVENT_STOMP                         = 26,
    EVENT_ARCTIC_BREATH                 = 27,

    // Frost Freeze Trap
    EVENT_ACTIVATE_TRAP                 = 28,

    // Crok Scourgebane
    EVENT_SCOURGE_STRIKE                = 29,
    EVENT_DEATH_STRIKE                  = 30,
    EVENT_HEALTH_CHECK                  = 31,
    EVENT_CROK_INTRO_3                  = 32,
    EVENT_START_PATHING                 = 33,

    // Sister Svalna
    EVENT_ARNATH_INTRO_2                = 34,
    EVENT_SVALNA_START                  = 35,
    EVENT_SVALNA_RESURRECT              = 36,
    EVENT_SVALNA_COMBAT                 = 37,
    EVENT_IMPALING_SPEAR                = 38,
    EVENT_AETHER_SHIELD                 = 39,

    // Captain Arnath
    EVENT_ARNATH_FLASH_HEAL             = 40,
    EVENT_ARNATH_PW_SHIELD              = 41,
    EVENT_ARNATH_SMITE                  = 42,
    EVENT_ARNATH_DOMINATE_MIND          = 43,

    // Captain Brandon
    EVENT_BRANDON_CRUSADER_STRIKE       = 44,
    EVENT_BRANDON_DIVINE_SHIELD         = 45,
    EVENT_BRANDON_JUDGEMENT_OF_COMMAND  = 46,
    EVENT_BRANDON_HAMMER_OF_BETRAYAL    = 47,

    // Captain Grondel
    EVENT_GRONDEL_CHARGE_CHECK          = 48,
    EVENT_GRONDEL_MORTAL_STRIKE         = 49,
    EVENT_GRONDEL_SUNDER_ARMOR          = 50,
    EVENT_GRONDEL_CONFLAGRATION         = 51,

    // Captain Rupert
    EVENT_RUPERT_FEL_IRON_BOMB          = 52,
    EVENT_RUPERT_MACHINE_GUN            = 53,
    EVENT_RUPERT_ROCKET_LAUNCH          = 54,

    // Sindragosas Ward
    EVENT_SUB_WAVE_1                    = 55,
    EVENT_SUB_WAVE_2                    = 56,
    EVENT_UPDATE_CHECK                  = 57,

    //-------- SANCTUARY OF BLOOD --------//
    // Darkfallen Archmage
    EVENT_AMPLIFY                       = 58,
    EVENT_BLAST                         = 59,
    EVENT_FIREBALL                      = 60,
    EVENT_POLYMORPH                     = 61,

    // Darkfallen Blood Knight
    EVENT_AURA                          = 62,
    EVENT_STRIKE                        = 63,
    EVENT_MIRROR                        = 64,

    // Darkfallen Noble
    EVENT_CHAINS                        = 65,
    EVENT_BOLT                          = 66,
    EVENT_FIEND                         = 67,

    // Vampiric Fiend
    EVENT_LEECHING                      = 68,

    // Darkfallen Advisor
    EVENT_LICH                          = 69,
    EVENT_PROTECTION                    = 70,

    // Darkfallen Commander
    EVENT_RUSH                          = 71,
    EVENT_SHOUT                         = 72,

    // Darkfallen Lieutenant
    EVENT_REND                          = 73,
    EVENT_CURSE                         = 74,

    // Darkfallen Tactician
    EVENT_SHADOW_STEP                   = 75,
    EVENT_SAP                           = 76,
    EVENT_STRIKER                       = 77,

    //----------- PLAGUE WORKS -----------//
    // Blighted Abomination
    EVENT_CLEAVE                        = 78,
    EVENT_CLOUD                         = 79,
    EVENT_HOOK                          = 80,

    // Vengeful Fleshreaper
    EVENT_JUMP_1                          = 81,
    EVENT_DEVOUR                        = 82,

    // Plague Scientist
    EVENT_STREAM                        = 83,
    EVENT_STREAM_OUT                    = 84,
    EVENT_SPRAY                         = 85,
    EVENT_BLAST_PL                      = 86,

    // Pustulating Horror
    EVENT_PUS                           = 87,

    // Decaying Colossus
    EVENT_STOMP_PL                      = 88,

    //------------- FROST WING -------------//
    // Ymirjar Deathbringer
    EVENT_BANISH                        = 89,
    EVENT_EMBRACE                       = 90,
    EVENT_BOLT_FW                       = 91,

    // Ymirjar Frostbinder
    EVENT_ORB                           = 92,

    // Ymirjar Battle-Maiden
    EVENT_RUSH_FW                       = 93,
    EVENT_STRIKE_FW                     = 94,

    // Ymirjar Huntress
    EVENT_TRAP                          = 95,
    EVENT_SHOT                          = 96,
    EVENT_RSHOT                         = 97,
    EVENT_VOLLEY                        = 98,

    //Ymirjar Warlord
    EVENT_WHIRLWIND                     = 99,

    // Invisible Stalker (Float, Uninteractible, LargeAOI)
    EVENT_SOUL_MISSILE                  = 100,
};

enum DataTypesICC
{
    DATA_DAMNED_KILLS       = 1,
};

enum Actions
{
    // Sister Svalna
    ACTION_KILL_CAPTAIN         = 1,
    ACTION_START_GAUNTLET       = 2,
    ACTION_RESURRECT_CAPTAINS   = 3,
    ACTION_CAPTAIN_DIES         = 4,
    ACTION_RESET_EVENT          = 5,
};

enum EventIds
{
    EVENT_AWAKEN_WARD_1 = 22900,
    EVENT_AWAKEN_WARD_2 = 22907,
    EVENT_AWAKEN_WARD_3 = 22908,
    EVENT_AWAKEN_WARD_4 = 22909,
};

enum MovementPoints
{
    POINT_LAND  = 1,
};

const Position SindragosaGauntletSpawn[12] =
{
    { 4130.71f, 2484.10f, 211.033f, 0 },
    { 4137.93f, 2505.52f, 211.033f, 0 },
    { 4160.64f, 2528.13f, 211.033f, 0 },
    { 4180.81f, 2533.88f, 211.033f, 0 },
    { 4200.92f, 2527.18f, 211.033f, 0 },
    { 4222.23f, 2503.56f, 211.033f, 0 },
    { 4229.40f, 2484.63f, 211.033f, 0 },
    { 4222.01f, 2464.93f, 211.033f, 0 },
    { 4201.55f, 2441.03f, 211.033f, 0 },
    { 4181.29f, 2433.38f, 211.033f, 0 },
    { 4161.86f, 2441.94f, 211.033f, 0 },
    { 4138.78f, 2463.95f, 211.033f, 0 },
};

class FrostwingVrykulSearcher
{
    public:
        FrostwingVrykulSearcher(Creature const* source, float range) : _source(source), _range(range) {}

        bool operator()(WorldObject* object) const
        {
            if (!object->ToUnit()->isAlive())
                return false;

            switch (object->ToUnit()->GetEntry())
            {
                case NPC_YMIRJAR_BATTLE_MAIDEN:
                case NPC_YMIRJAR_DEATHBRINGER:
                case NPC_YMIRJAR_FROSTBINDER:
                case NPC_YMIRJAR_HUNTRESS:
                case NPC_YMIRJAR_WARLORD:
                    break;
                default:
                    return false;
            }

            if (!object->IsWithinDist(_source, _range, false))
                return false;

            return true;
        }

    private:
        Creature const* _source;
        float _range;
};

class FrostwingGauntletRespawner
{
    public:
        void operator()(Creature* creature)
        {
            switch (creature->GetOriginalEntry())
            {
                case NPC_YMIRJAR_BATTLE_MAIDEN:
                case NPC_YMIRJAR_DEATHBRINGER:
                case NPC_YMIRJAR_FROSTBINDER:
                case NPC_YMIRJAR_HUNTRESS:
                case NPC_YMIRJAR_WARLORD:
                    break;
                case NPC_CROK_SCOURGEBANE:
                case NPC_CAPTAIN_ARNATH:
                case NPC_CAPTAIN_BRANDON:
                case NPC_CAPTAIN_GRONDEL:
                case NPC_CAPTAIN_RUPERT:
                    creature->AI()->DoAction(ACTION_RESET_EVENT);
                    break;
                case NPC_SISTER_SVALNA:
                    creature->AI()->DoAction(ACTION_RESET_EVENT);
                    // return, this creature is never dead if event is reset
                    return;
                default:
                    return;
            }

            uint32 corpseDelay = creature->GetCorpseDelay();
            uint32 respawnDelay = creature->GetRespawnDelay();
            creature->SetCorpseDelay(1);
            creature->SetRespawnDelay(2);

            if (CreatureData const* data = creature->GetCreatureData())
                creature->SetPosition(data->posX, data->posY, data->posZ, data->orientation);
            creature->DespawnOrUnsummon();

            creature->SetCorpseDelay(corpseDelay);
            creature->SetRespawnDelay(respawnDelay);
        }
};

class CaptainSurviveTalk : public BasicEvent
{
    public:
        explicit CaptainSurviveTalk(Creature const& owner) : _owner(owner) { }

        bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
        {
            _owner.AI()->Talk(SAY_CAPTAIN_SURVIVE_TALK);
            return true;
        }

    private:
        Creature const& _owner;
};

enum AshenRingRecover
{
    FACTION_ASHEN_VEREDICT  = 1156,
    NPC_TEXT_ID             = 537006,
    MAX_RINGS_NUMBER        = 20,
};

class npc_icc_ring_recover : public CreatureScript
{
    public:
        npc_icc_ring_recover() : CreatureScript("npc_icc_ring_recover") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            for (int i = 0; i < MAX_RINGS_NUMBER; ++i)
            {
                if (player->HasItemCount(rings[i], 1, true)) // Check items also in bank
                {
                    creature->MonsterWhisper(sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_ALREADY_HAVE), player->GetGUID());
                    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_DELETE), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_CONFIRM), NULL, false);
                    player->SEND_GOSSIP_MENU(NPC_TEXT_ID, creature->GetGUID());
                    return true;
                }
            }

            switch (player->GetReputationRank(FACTION_ASHEN_VEREDICT))
            {
                case REP_FRIENDLY:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_CASTER_FRIENDLY), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_HEALER_FRIENDLY), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_STRENGTH_FRIENDLY), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_AGILITY_FRIENDLY), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_TANK_FRIENDLY), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                    break;
                case REP_HONORED:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_CASTER_HONORED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_HEALER_HONORED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_STRENGTH_HONORED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_AGILITY_HONORED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_TANK_HONORED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
                    break;
                case REP_REVERED:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_CASTER_REVERED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_HEALER_REVERED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_STRENGTH_REVERED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_AGILITY_REVERED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_TANK_REVERED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                    break;
                case REP_EXALTED:
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_CASTER_EXALTED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_HEALER_EXALTED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_STRENGTH_EXALTED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_MELEE_AGILITY_EXALTED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_TANK_EXALTED), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
                    break;
                default:
                    creature->MonsterWhisper(sObjectMgr->GetTrinityStringForDBCLocale(LANG_ICC_RING_NOT_ENOUGH_REP), player->GetGUID());
                    break;
            }

            player->SEND_GOSSIP_MENU(NPC_TEXT_ID, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 21)
            {
                for (int i = 0; i < MAX_RINGS_NUMBER; ++i)
                {
                    if (player->HasItemCount(rings[i], 1, true))
                        player->DestroyItemCount(rings[i], 1, true);
                }

                player->CastSpell(player, 71811, false); // visual effect
                player->CLOSE_GOSSIP_MENU();
                return true;
            }

            if (!player->HasEnoughMoney(int64(2000000)))   // 200 gold
                player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
            else
            {
                player->ModifyMoney(int64(-2000000));

                int id = action - (GOSSIP_ACTION_INFO_DEF + 1);
                if (id < 20)
                    player->AddItem(rings[id], 1);
                
                player->CastSpell(player, 66400, false); // visual effect
                creature->CastSpell(creature, 66402, false); // visual effect
            }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

    private:
        uint32 static const rings[20];
};

uint32 const npc_icc_ring_recover::rings[MAX_RINGS_NUMBER] = 
{
    50377, 50378, 52569, 50376, 50375, // Friendly
    50384, 50386, 52570, 50387, 50388, // Honored
    50397, 50399, 52571, 50401, 50403, // Revered
    50398, 50400, 52572, 50402, 50404  // Exalted
};

enum BuffRemoverEnum
{
    NPC_TEXT_ID_1   = 15602,
    NPC_TEXT_ID_2   = 15606
};

class npc_icc_buff_remover : public CreatureScript
{
    public:
        npc_icc_buff_remover() : CreatureScript("npc_icc_buff_remover") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            InstanceScript* _instance = creature->GetInstanceScript();
            if (!_instance)
                return false;

            if (_instance->GetData(DATA_BUFF_REMOVED) != DONE)
                if (player->GetGroup() && player->GetGroup()->IsLeader(player->GetGUID()))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, sObjectMgr->GetTrinityStringForDBCLocale(creature->GetEntry() == NPC_KING_VARIAN_WRYNN ? LANG_ICC_BUFF_REMOVE_A : LANG_ICC_BUFF_REMOVE_H), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            player->SEND_GOSSIP_MENU(creature->GetEntry() == NPC_KING_VARIAN_WRYNN ? NPC_TEXT_ID_1 : NPC_TEXT_ID_2, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            InstanceScript* _instance = creature->GetInstanceScript();
            if (!_instance)
                return false;

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                _instance->SetData(DATA_BUFF_REMOVED, DONE);
                _instance->DoRemoveAurasDueToSpellOnPlayers(creature->GetEntry() == NPC_KING_VARIAN_WRYNN ? SPELL_STRENGTH_OF_WRYNN : SPELL_HELLSCREAM_S_WARSONG);
            }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

// at Light's Hammer
class npc_highlord_tirion_fordring_lh : public CreatureScript
{
    public:
        npc_highlord_tirion_fordring_lh() : CreatureScript("npc_highlord_tirion_fordring_lh") { }

        struct npc_highlord_tirion_fordringAI : public ScriptedAI
        {
            npc_highlord_tirion_fordringAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
            {
            }

            void Reset()
            {
                _events.Reset();
                _theLichKing = 0;
                _bolvarFordragon = 0;
                _factionNPC = 0;
                _damnedKills = 0;
            }

            // IMPORTANT NOTE: This is triggered from per-GUID scripts
            // of The Damned SAI
            void SetData(uint32 type, uint32 data)
            {
                if (type == DATA_DAMNED_KILLS && data == 1)
                {
                    if (++_damnedKills == 2)
                    {
                        if (Creature* theLichKing = me->FindNearestCreature(NPC_THE_LICH_KING_LH, 150.0f))
                        {
                            if (Creature* bolvarFordragon = me->FindNearestCreature(NPC_HIGHLORD_BOLVAR_FORDRAGON_LH, 150.0f))
                            {
                                if (Creature* factionNPC = me->FindNearestCreature(_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? NPC_SE_HIGH_OVERLORD_SAURFANG : NPC_SE_MURADIN_BRONZEBEARD, 50.0f))
                                {
                                    me->setActive(true);
                                    _theLichKing = theLichKing->GetGUID();
                                    theLichKing->setActive(true);
                                    _bolvarFordragon = bolvarFordragon->GetGUID();
                                    bolvarFordragon->setActive(true);
                                    _factionNPC = factionNPC->GetGUID();
                                    factionNPC->setActive(true);
                                }
                            }
                        }

                        if (!_bolvarFordragon || !_theLichKing || !_factionNPC)
                            return;

                        Talk(SAY_TIRION_INTRO_1);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_2, 4000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_3, 14000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_4, 18000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_5, 31000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_1, 35000);
                        _events.ScheduleEvent(EVENT_TIRION_INTRO_6, 51000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_2, 58000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_3, 74000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_4, 86000);
                        _events.ScheduleEvent(EVENT_BOLVAR_INTRO_1, 100000);
                        _events.ScheduleEvent(EVENT_LK_INTRO_5, 108000);

                        if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                        {
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_1, 120000);
                            _events.ScheduleEvent(EVENT_TIRION_INTRO_H_7, 129000);
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_2, 139000);
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_3, 150000);
                            _events.ScheduleEvent(EVENT_SAURFANG_INTRO_4, 162000);
                            _events.ScheduleEvent(EVENT_SAURFANG_RUN, 170000);
                        }
                        else
                        {
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_1, 120000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_2, 124000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_3, 127000);
                            _events.ScheduleEvent(EVENT_TIRION_INTRO_A_7, 136000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_4, 144000);
                            _events.ScheduleEvent(EVENT_MURADIN_INTRO_5, 151000);
                            _events.ScheduleEvent(EVENT_MURADIN_RUN, 157000);
                        }
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (_damnedKills != 2)
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TIRION_INTRO_2:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_3:
                            Talk(SAY_TIRION_INTRO_2);
                            break;
                        case EVENT_TIRION_INTRO_4:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_5:
                            Talk(SAY_TIRION_INTRO_3);
                            break;
                        case EVENT_LK_INTRO_1:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NO_SHEATHE);
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_1);
                            break;
                        case EVENT_TIRION_INTRO_6:
                            Talk(SAY_TIRION_INTRO_4);
                            break;
                        case EVENT_LK_INTRO_2:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_2);
                            break;
                        case EVENT_LK_INTRO_3:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_3);
                            break;
                        case EVENT_LK_INTRO_4:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                                theLichKing->AI()->Talk(SAY_LK_INTRO_4);
                            break;
                        case EVENT_BOLVAR_INTRO_1:
                            if (Creature* bolvarFordragon = ObjectAccessor::GetCreature(*me, _bolvarFordragon))
                            {
                                bolvarFordragon->AI()->Talk(SAY_BOLVAR_INTRO_1);
                                bolvarFordragon->setActive(false);
                            }
                            break;
                        case EVENT_LK_INTRO_5:
                            if (Creature* theLichKing = ObjectAccessor::GetCreature(*me, _theLichKing))
                            {
                                theLichKing->AI()->Talk(SAY_LK_INTRO_5);
                                theLichKing->setActive(false);
                            }
                            break;
                        case EVENT_SAURFANG_INTRO_1:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_1);
                            break;
                        case EVENT_TIRION_INTRO_H_7:
                            Talk(SAY_TIRION_INTRO_H_5);
                            break;
                        case EVENT_SAURFANG_INTRO_2:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_2);
                            break;
                        case EVENT_SAURFANG_INTRO_3:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_3);
                            break;
                        case EVENT_SAURFANG_INTRO_4:
                            if (Creature* saurfang = ObjectAccessor::GetCreature(*me, _factionNPC))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_4);
                            break;
                        case EVENT_MURADIN_RUN:
                        case EVENT_SAURFANG_RUN:
                            if (Creature* factionNPC = ObjectAccessor::GetCreature(*me, _factionNPC))
                                factionNPC->GetMotionMaster()->MovePath(factionNPC->GetDBTableGUIDLow()*10, false);
                            me->setActive(false);
                            _damnedKills = 3;
                            break;
                        case EVENT_MURADIN_INTRO_1:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_1);
                            break;
                        case EVENT_MURADIN_INTRO_2:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
                            break;
                        case EVENT_MURADIN_INTRO_3:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            break;
                        case EVENT_TIRION_INTRO_A_7:
                            Talk(SAY_TIRION_INTRO_A_5);
                            break;
                        case EVENT_MURADIN_INTRO_4:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_2);
                            break;
                        case EVENT_MURADIN_INTRO_5:
                            if (Creature* muradin = ObjectAccessor::GetCreature(*me, _factionNPC))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_3);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap _events;
            InstanceScript* const _instance;
            uint64 _theLichKing;
            uint64 _bolvarFordragon;
            uint64 _factionNPC;
            uint16 _damnedKills;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_highlord_tirion_fordringAI>(creature);
        }
};

class npc_rotting_frost_giant : public CreatureScript
{
    public:
        npc_rotting_frost_giant() : CreatureScript("npc_rotting_frost_giant") { }

        struct npc_rotting_frost_giantAI : public ScriptedAI
        {
            npc_rotting_frost_giantAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                _events.ScheduleEvent(EVENT_STOMP, urand(5000, 8000));
                _events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(10000, 15000));
            }

            void JustDied(Unit* /*killer*/)
            {
                _events.Reset();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEATH_PLAGUE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            {
                                Talk(EMOTE_DEATH_PLAGUE_WARNING, target->GetGUID());
                                DoCast(target, SPELL_DEATH_PLAGUE);
                            }
                            _events.ScheduleEvent(EVENT_DEATH_PLAGUE, 15000);
                            break;
                        case EVENT_STOMP:
                            DoCastVictim(SPELL_STOMP);
                            _events.ScheduleEvent(EVENT_STOMP, urand(15000, 18000));
                            break;
                        case EVENT_ARCTIC_BREATH:
                            DoCastVictim(SPELL_ARCTIC_BREATH);
                            _events.ScheduleEvent(EVENT_ARCTIC_BREATH, urand(26000, 33000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_rotting_frost_giantAI>(creature);
        }
};

class npc_frost_freeze_trap : public CreatureScript
{
    public:
        npc_frost_freeze_trap() : CreatureScript("npc_frost_freeze_trap") { }

        struct npc_frost_freeze_trapAI: public Scripted_NoMovementAI
        {
            npc_frost_freeze_trapAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case 1000:
                    case 11000:
                        _events.ScheduleEvent(EVENT_ACTIVATE_TRAP, uint32(action));
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_ACTIVATE_TRAP)
                {
                    DoCast(me, SPELL_COLDFLAME_JETS);
                    _events.ScheduleEvent(EVENT_ACTIVATE_TRAP, 22000);
                }
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_frost_freeze_trapAI>(creature);
        }
};

class npc_alchemist_adrianna : public CreatureScript
{
    public:
        npc_alchemist_adrianna() : CreatureScript("npc_alchemist_adrianna") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (!creature->FindCurrentSpellBySpellId(SPELL_HARVEST_BLIGHT_SPECIMEN) && !creature->FindCurrentSpellBySpellId(SPELL_HARVEST_BLIGHT_SPECIMEN25))
                if (player->HasAura(SPELL_ORANGE_BLIGHT_RESIDUE) && player->HasAura(SPELL_GREEN_BLIGHT_RESIDUE))
                    creature->CastSpell(creature, SPELL_HARVEST_BLIGHT_SPECIMEN, false);
            return false;
        }
};

class boss_sister_svalna : public CreatureScript
{
    public:
        boss_sister_svalna() : CreatureScript("boss_sister_svalna") { }

        struct boss_sister_svalnaAI : public BossAI
        {
            boss_sister_svalnaAI(Creature* creature) : BossAI(creature, DATA_SISTER_SVALNA),
                _isEventInProgress(false)
            {
            }

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                _Reset();
                me->SetReactState(REACT_DEFENSIVE);
                _isEventInProgress = false;
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_SVALNA_DEATH);

                uint64 delay = 1;
                for (uint32 i = 0; i < 4; ++i)
                {
                    if (Creature* crusader = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CAPTAIN_ARNATH + i)))
                    {
                        if (crusader->isAlive() && crusader->GetEntry() == crusader->GetCreatureData()->id)
                        {
                            crusader->m_Events.AddEvent(new CaptainSurviveTalk(*crusader), crusader->m_Events.CalculateTime(delay));
                            delay += 6000;
                        }
                    }
                }
            }

            void EnterCombat(Unit* /*attacker*/)
            {
                _EnterCombat();
                if (Creature* crok = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                    crok->AI()->Talk(SAY_CROK_COMBAT_SVALNA);
                events.ScheduleEvent(EVENT_SVALNA_COMBAT, 9000);
                events.ScheduleEvent(EVENT_IMPALING_SPEAR, urand(40000, 50000));
                events.ScheduleEvent(EVENT_AETHER_SHIELD, urand(100000, 110000));
            }

            void KilledUnit(Unit* victim)
            {
                switch (victim->GetTypeId())
                {
                    case TYPEID_PLAYER:
                        Talk(SAY_SVALNA_KILL);
                        break;
                    case TYPEID_UNIT:
                        switch (victim->GetEntry())
                        {
                            case NPC_CAPTAIN_ARNATH:
                            case NPC_CAPTAIN_BRANDON:
                            case NPC_CAPTAIN_GRONDEL:
                            case NPC_CAPTAIN_RUPERT:
                                Talk(SAY_SVALNA_KILL_CAPTAIN);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();
                me->SetReactState(REACT_PASSIVE);
                me->SetDisableGravity(false);
                me->SetHover(false);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_KILL_CAPTAIN:
                        me->CastCustomSpell(SPELL_CARESS_OF_DEATH, SPELLVALUE_MAX_TARGETS, 1, me, true);
                        break;
                    case ACTION_START_GAUNTLET:
                        me->setActive(true);
                        _isEventInProgress = true;
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        events.ScheduleEvent(EVENT_SVALNA_START, 25000);
                        break;
                    case ACTION_RESURRECT_CAPTAINS:
                        events.ScheduleEvent(EVENT_SVALNA_RESURRECT, 7000);
                        break;
                    case ACTION_CAPTAIN_DIES:
                        Talk(SAY_SVALNA_CAPTAIN_DEATH);
                        break;
                    case ACTION_RESET_EVENT:
                        me->setActive(false);
                        Reset();
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_HURL_SPEAR && me->HasAura(SPELL_AETHER_SHIELD))
                {
                    me->RemoveAurasDueToSpell(SPELL_AETHER_SHIELD);
                    me->RemoveAurasDueToSpell(SPELL_DIVINE_SURGE);
                    me->CastSpell(me, Is25ManRaid() ? SPELL_AETHER_BURST_25 : SPELL_AETHER_BURST_10, true);
                    Talk(EMOTE_SVALNA_BROKEN_SHIELD, caster->GetGUID());
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != EFFECT_MOTION_TYPE || id != POINT_LAND)
                    return;

                _isEventInProgress = false;
                me->setActive(false);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetDisableGravity(false);
                me->SetHover(false);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_IMPALING_SPEAR_KILL:
                        me->Kill(target);
                        break;
                    case SPELL_IMPALING_SPEAR:
                        if (TempSummon* summon = target->SummonCreature(NPC_IMPALING_SPEAR, *target))
                        {
                            Talk(EMOTE_SVALNA_IMPALE, target->GetGUID());
                            summon->CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, 1, target, false);
                            summon->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1 | UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() && !_isEventInProgress)
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SVALNA_START:
                            Talk(SAY_SVALNA_EVENT_START);
                            break;
                        case EVENT_SVALNA_RESURRECT:
                            Talk(SAY_SVALNA_RESURRECT_CAPTAINS);
                            me->CastSpell(me, SPELL_REVIVE_CHAMPION, false);
                            break;
                        case EVENT_SVALNA_COMBAT:
                            me->SetReactState(REACT_DEFENSIVE);
                            Talk(SAY_SVALNA_AGGRO);
                            break;
                        case EVENT_IMPALING_SPEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_IMPALING_SPEAR))
                            {
                                DoCast(me, SPELL_AETHER_SHIELD);
                                DoCast(me, SPELL_DIVINE_SURGE);
                                DoCast(target, SPELL_IMPALING_SPEAR);
                            }
                            events.ScheduleEvent(EVENT_IMPALING_SPEAR, urand(20000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool _isEventInProgress;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<boss_sister_svalnaAI>(creature);
        }
};

class npc_crok_scourgebane : public CreatureScript
{
    public:
        npc_crok_scourgebane() : CreatureScript("npc_crok_scourgebane") { }

        struct npc_crok_scourgebaneAI : public npc_escortAI
        {
            npc_crok_scourgebaneAI(Creature* creature) : npc_escortAI(creature),
                _instance(creature->GetInstanceScript()), _respawnTime(creature->GetRespawnDelay()),
                _corpseDelay(creature->GetCorpseDelay())
            {
                SetDespawnAtEnd(false);
                SetDespawnAtFar(false);
                _isEventActive = false;
                _isEventDone = _instance->GetBossState(DATA_SISTER_SVALNA) == DONE;
                _didUnderTenPercentText = false;
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_SCOURGE_STRIKE, urand(7500, 12500));
                _events.ScheduleEvent(EVENT_DEATH_STRIKE, urand(25000, 30000));
                me->SetReactState(REACT_DEFENSIVE);
                _didUnderTenPercentText = false;
                _wipeCheckTimer = 1000;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_START_GAUNTLET)
                {
                    if (_isEventDone || !me->isAlive())
                        return;

                    _isEventActive = true;
                    _isEventDone = true;
                    // Load Grid with Sister Svalna
                    me->GetMap()->LoadGrid(4356.71f, 2484.33f);
                    if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                        svalna->AI()->DoAction(ACTION_START_GAUNTLET);
                    Talk(SAY_CROK_INTRO_1);
                    _events.ScheduleEvent(EVENT_ARNATH_INTRO_2, 7000);
                    _events.ScheduleEvent(EVENT_CROK_INTRO_3, 14000);
                    _events.ScheduleEvent(EVENT_START_PATHING, 37000);
                    me->setActive(true);
                    for (uint32 i = 0; i < 4; ++i)
                        if (Creature* crusader = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_CAPTAIN_ARNATH + i)))
                            crusader->AI()->DoAction(ACTION_START_GAUNTLET);
                }
                else if (action == ACTION_RESET_EVENT)
                {
                    _isEventActive = false;
                    _isEventDone = _instance->GetBossState(DATA_SISTER_SVALNA) == DONE;
                    me->setActive(false);
                    _aliveTrash.clear();
                    _currentWPid = 0;
                }
            }

            void SetGUID(uint64 guid, int32 type/* = 0*/)
            {
                if (type == ACTION_VRYKUL_DEATH)
                {
                    _aliveTrash.erase(guid);
                    if (_aliveTrash.empty())
                    {
                        SetEscortPaused(false);
                        if (_currentWPid == 4 && _isEventActive)
                        {
                            _isEventActive = false;
                            me->setActive(false);
                            Talk(SAY_CROK_FINAL_WP);
                            if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                                svalna->AI()->DoAction(ACTION_RESURRECT_CAPTAINS);
                        }
                    }
                }
            }

            void WaypointReached(uint32 waypointId)
            {
                switch (waypointId)
                {
                    // pause pathing until trash pack is cleared
                    case 0:
                        Talk(SAY_CROK_COMBAT_WP_0);
                        if (!_aliveTrash.empty())
                            SetEscortPaused(true);
                        break;
                    case 1:
                        Talk(SAY_CROK_COMBAT_WP_1);
                        if (!_aliveTrash.empty())
                            SetEscortPaused(true);
                        break;
                    case 4:
                        if (_aliveTrash.empty() && _isEventActive)
                        {
                            _isEventActive = false;
                            me->setActive(false);
                            Talk(SAY_CROK_FINAL_WP);
                            if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                                svalna->AI()->DoAction(ACTION_RESURRECT_CAPTAINS);
                        }
                        break;
                    default:
                        break;
                }
            }

            void WaypointStart(uint32 waypointId)
            {
                _currentWPid = waypointId;
                switch (waypointId)
                {
                    case 0:
                    case 1:
                    case 4:
                    {
                        // get spawns by home position
                        float minY = 2600.0f;
                        float maxY = 2650.0f;
                        if (waypointId == 1)
                        {
                            minY -= 50.0f;
                            maxY -= 50.0f;
                            // at waypoints 1 and 2 she kills one captain
                            if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                                svalna->AI()->DoAction(ACTION_KILL_CAPTAIN);
                        }
                        else if (waypointId == 4)
                        {
                            minY -= 100.0f;
                            maxY -= 100.0f;
                        }

                        // get all nearby vrykul
                        std::list<Creature*> temp;
                        FrostwingVrykulSearcher check(me, 80.0f);
                        JadeCore::CreatureListSearcher<FrostwingVrykulSearcher> searcher(me, temp, check);
                        me->VisitNearbyGridObject(80.0f, searcher);

                        _aliveTrash.clear();
                        for (std::list<Creature*>::iterator itr = temp.begin(); itr != temp.end(); ++itr)
                            if ((*itr)->GetHomePosition().GetPositionY() < maxY && (*itr)->GetHomePosition().GetPositionY() > minY)
                                _aliveTrash.insert((*itr)->GetGUID());
                        break;
                    }
                    // at waypoints 1 and 2 she kills one captain
                    case 2:
                        if (Creature* svalna = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_SISTER_SVALNA)))
                            svalna->AI()->DoAction(ACTION_KILL_CAPTAIN);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                // check wipe
                if (!_wipeCheckTimer)
                {
                    _wipeCheckTimer = 1000;
                    Player* player = NULL;
                    JadeCore::AnyPlayerInObjectRangeCheck check(me, 60.0f);
                    JadeCore::PlayerSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, player, check);
                    me->VisitNearbyWorldObject(60.0f, searcher);
                    // wipe
                    if (!player)
                    {
                        damage *= 100;
                        if (damage >= me->GetHealth())
                        {
                            FrostwingGauntletRespawner respawner;
                            JadeCore::CreatureWorker<FrostwingGauntletRespawner> worker(me, respawner);
                            me->VisitNearbyGridObject(333.0f, worker);
                            Talk(SAY_CROK_DEATH);
                        }
                        return;
                    }
                }

                if (HealthBelowPct(10))
                {
                    if (!_didUnderTenPercentText)
                    {
                        _didUnderTenPercentText = true;
                        if (_isEventActive)
                            Talk(SAY_CROK_WEAKENING_GAUNTLET);
                        else
                            Talk(SAY_CROK_WEAKENING_SVALNA);
                    }

                    damage = 0;
                    DoCast(me, SPELL_ICEBOUND_ARMOR);
                    _events.ScheduleEvent(EVENT_HEALTH_CHECK, 1000);
                }
            }

            void UpdateEscortAI(uint32 const diff)
            {
                if (_wipeCheckTimer <= diff)
                    _wipeCheckTimer = 0;
                else
                    _wipeCheckTimer -= diff;

                if (!UpdateVictim() && !_isEventActive)
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ARNATH_INTRO_2:
                            if (Creature* arnath = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_CAPTAIN_ARNATH)))
                                arnath->AI()->Talk(SAY_ARNATH_INTRO_2);
                            break;
                        case EVENT_CROK_INTRO_3:
                            Talk(SAY_CROK_INTRO_3);
                            break;
                        case EVENT_START_PATHING:
                            Start(true, true);
                            break;
                        case EVENT_SCOURGE_STRIKE:
                            DoCastVictim(SPELL_SCOURGE_STRIKE);
                            _events.ScheduleEvent(EVENT_SCOURGE_STRIKE, urand(10000, 14000));
                            break;
                        case EVENT_DEATH_STRIKE:
                            if (HealthBelowPct(20))
                                DoCastVictim(SPELL_DEATH_STRIKE);
                            _events.ScheduleEvent(EVENT_DEATH_STRIKE, urand(5000, 10000));
                            break;
                        case EVENT_HEALTH_CHECK:
                            if (HealthAbovePct(15))
                            {
                                me->RemoveAurasDueToSpell(SPELL_ICEBOUND_ARMOR);
                                _didUnderTenPercentText = false;
                            }
                            else
                            {
                                me->DealHeal(me, me->CountPctFromMaxHealth(5));
                                _events.ScheduleEvent(EVENT_HEALTH_CHECK, 1000);
                            }
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            bool CanAIAttack(Unit const* target) const
            {
                // do not see targets inside Frostwing Halls when we are not there
                return (me->GetPositionY() > 2660.0f) == (target->GetPositionY() > 2660.0f);
            }

        private:
            EventMap _events;
            std::set<uint64> _aliveTrash;
            InstanceScript* _instance;
            uint32 _currentWPid;
            uint32 _wipeCheckTimer;
            uint32 const _respawnTime;
            uint32 const _corpseDelay;
            bool _isEventActive;
            bool _isEventDone;
            bool _didUnderTenPercentText;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_crok_scourgebaneAI>(creature);
        }
};

struct npc_argent_captainAI : public ScriptedAI
{
    public:
        npc_argent_captainAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript()), _firstDeath(true)
        {
            FollowAngle = PET_FOLLOW_ANGLE;
            FollowDist = PET_FOLLOW_DIST;
            IsUndead = false;
        }

        void JustDied(Unit* /*killer*/)
        {
            if (_firstDeath)
            {
                _firstDeath = false;
                Talk(SAY_CAPTAIN_DEATH);
            }
            else
                Talk(SAY_CAPTAIN_SECOND_DEATH);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_CAPTAIN_KILL);
        }

        void DoAction(int32 const action)
        {
            if (action == ACTION_START_GAUNTLET)
            {
                if (Creature* crok = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                {
                    me->SetReactState(REACT_DEFENSIVE);
                    FollowAngle = me->GetAngle(crok) + me->GetOrientation();
                    FollowDist = me->GetDistance2d(crok);
                    me->GetMotionMaster()->MoveFollow(crok, FollowDist, FollowAngle, MOTION_SLOT_IDLE);
                }

                me->setActive(true);
            }
            else if (action == ACTION_RESET_EVENT)
            {
                _firstDeath = true;
            }
        }

        void EnterCombat(Unit* /*target*/)
        {
            me->SetHomePosition(*me);
            if (IsUndead)
                DoZoneInCombat();
        }

        bool CanAIAttack(Unit const* target) const
        {
            // do not see targets inside Frostwing Halls when we are not there
            return (me->GetPositionY() > 2660.0f) == (target->GetPositionY() > 2660.0f);
        }

        void EnterEvadeMode()
        {
            // not yet following
            if (me->GetMotionMaster()->GetMotionSlotType(MOTION_SLOT_IDLE) != CHASE_MOTION_TYPE || IsUndead)
            {
                ScriptedAI::EnterEvadeMode();
                return;
            }

            if (!_EnterEvadeMode())
                return;

            if (!me->GetVehicle())
            {
                me->GetMotionMaster()->Clear(false);
                if (Creature* crok = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                    me->GetMotionMaster()->MoveFollow(crok, FollowDist, FollowAngle, MOTION_SLOT_IDLE);
            }

            Reset();
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_REVIVE_CHAMPION && !IsUndead)
            {
                IsUndead = true;
                me->setDeathState(JUST_RESPAWNED);
                uint32 newEntry = 0;
                switch (me->GetEntry())
                {
                    case NPC_CAPTAIN_ARNATH:
                        newEntry = NPC_CAPTAIN_ARNATH_UNDEAD;
                        break;
                    case NPC_CAPTAIN_BRANDON:
                        newEntry = NPC_CAPTAIN_BRANDON_UNDEAD;
                        break;
                    case NPC_CAPTAIN_GRONDEL:
                        newEntry = NPC_CAPTAIN_GRONDEL_UNDEAD;
                        break;
                    case NPC_CAPTAIN_RUPERT:
                        newEntry = NPC_CAPTAIN_RUPERT_UNDEAD;
                        break;
                    default:
                        return;
                }

                Talk(SAY_CAPTAIN_RESURRECTED);
                me->UpdateEntry(newEntry, instance->GetData(DATA_TEAM_IN_INSTANCE), me->GetCreatureData());
                DoCast(me, SPELL_UNDEATH, true);
                DoZoneInCombat();
            }
        }

    protected:
        EventMap Events;
        InstanceScript* instance;
        float FollowAngle;
        float FollowDist;
        bool IsUndead;

    private:
        bool _firstDeath;
};

class npc_captain_arnath : public CreatureScript
{
    public:
        npc_captain_arnath() : CreatureScript("npc_captain_arnath") { }

        struct npc_captain_arnathAI : public npc_argent_captainAI
        {
            npc_captain_arnathAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_ARNATH_FLASH_HEAL, urand(4000, 7000));
                Events.ScheduleEvent(EVENT_ARNATH_PW_SHIELD, urand(8000, 14000));
                Events.ScheduleEvent(EVENT_ARNATH_SMITE, urand(3000, 6000));
                if (Is25ManRaid() && IsUndead)
                    Events.ScheduleEvent(EVENT_ARNATH_DOMINATE_MIND, urand(22000, 27000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ARNATH_FLASH_HEAL:
                            if (Creature* target = FindFriendlyCreature())
                                DoCast(target, SPELL_FLASH_HEAL);
                            Events.ScheduleEvent(EVENT_ARNATH_FLASH_HEAL, urand(6000, 9000));
                            break;
                        case EVENT_ARNATH_PW_SHIELD:
                        {
                            std::list<Creature*> targets = DoFindFriendlyMissingBuff(40.0f, SPELL_POWER_WORD_SHIELD);
                            DoCast(JadeCore::Containers::SelectRandomContainerElement(targets), SPELL_POWER_WORD_SHIELD);
                            Events.ScheduleEvent(EVENT_ARNATH_PW_SHIELD, urand(15000, 20000));
                            break;
                        }
                        case EVENT_ARNATH_SMITE:
                            DoCastVictim(SPELL_SMITE);
                            Events.ScheduleEvent(EVENT_ARNATH_SMITE, urand(4000, 7000));
                            break;
                        case EVENT_ARNATH_DOMINATE_MIND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_DOMINATE_MIND);
                            Events.ScheduleEvent(EVENT_ARNATH_DOMINATE_MIND, urand(28000, 37000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            Creature* FindFriendlyCreature() const
            {
                Creature* target = NULL;
                JadeCore::MostHPMissingInRange u_check(me, 60.0f, 0);
                JadeCore::CreatureLastSearcher<JadeCore::MostHPMissingInRange> searcher(me, target, u_check);
                me->VisitNearbyGridObject(60.0f, searcher);
                return target;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_arnathAI>(creature);
        }
};

class npc_captain_brandon : public CreatureScript
{
    public:
        npc_captain_brandon() : CreatureScript("npc_captain_brandon") { }

        struct npc_captain_brandonAI : public npc_argent_captainAI
        {
            npc_captain_brandonAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_BRANDON_CRUSADER_STRIKE, urand(6000, 10000));
                Events.ScheduleEvent(EVENT_BRANDON_DIVINE_SHIELD, 500);
                Events.ScheduleEvent(EVENT_BRANDON_JUDGEMENT_OF_COMMAND, urand(8000, 13000));
                if (IsUndead)
                    Events.ScheduleEvent(EVENT_BRANDON_HAMMER_OF_BETRAYAL, urand(25000, 30000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BRANDON_CRUSADER_STRIKE:
                            DoCastVictim(SPELL_CRUSADER_STRIKE);
                            Events.ScheduleEvent(EVENT_BRANDON_CRUSADER_STRIKE, urand(6000, 12000));
                            break;
                        case EVENT_BRANDON_DIVINE_SHIELD:
                            if (HealthBelowPct(20))
                                DoCast(me, SPELL_DIVINE_SHIELD);
                            Events.ScheduleEvent(EVENT_BRANDON_DIVINE_SHIELD, 500);
                            break;
                        case EVENT_BRANDON_JUDGEMENT_OF_COMMAND:
                            DoCastVictim(SPELL_JUDGEMENT_OF_COMMAND);
                            Events.ScheduleEvent(EVENT_BRANDON_JUDGEMENT_OF_COMMAND, urand(8000, 13000));
                            break;
                        case EVENT_BRANDON_HAMMER_OF_BETRAYAL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_HAMMER_OF_BETRAYAL);
                            Events.ScheduleEvent(EVENT_BRANDON_HAMMER_OF_BETRAYAL, urand(45000, 60000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_brandonAI>(creature);
        }
};

class npc_captain_grondel : public CreatureScript
{
    public:
        npc_captain_grondel() : CreatureScript("npc_captain_grondel") { }

        struct npc_captain_grondelAI : public npc_argent_captainAI
        {
            npc_captain_grondelAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_GRONDEL_CHARGE_CHECK, 500);
                Events.ScheduleEvent(EVENT_GRONDEL_MORTAL_STRIKE, urand(8000, 14000));
                Events.ScheduleEvent(EVENT_GRONDEL_SUNDER_ARMOR, urand(3000, 12000));
                if (IsUndead)
                    Events.ScheduleEvent(EVENT_GRONDEL_CONFLAGRATION, urand(12000, 17000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GRONDEL_CHARGE_CHECK:
                            DoCastVictim(SPELL_CHARGE);
                            Events.ScheduleEvent(EVENT_GRONDEL_CHARGE_CHECK, 500);
                            break;
                        case EVENT_GRONDEL_MORTAL_STRIKE:
                            DoCastVictim(SPELL_MORTAL_STRIKE);
                            Events.ScheduleEvent(EVENT_GRONDEL_MORTAL_STRIKE, urand(10000, 15000));
                            break;
                        case EVENT_GRONDEL_SUNDER_ARMOR:
                            DoCastVictim(SPELL_SUNDER_ARMOR);
                            Events.ScheduleEvent(EVENT_GRONDEL_SUNDER_ARMOR, urand(5000, 17000));
                            break;
                        case EVENT_GRONDEL_CONFLAGRATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_CONFLAGRATION);
                            Events.ScheduleEvent(EVENT_GRONDEL_CONFLAGRATION, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_grondelAI>(creature);
        }
};

class npc_captain_rupert : public CreatureScript
{
    public:
        npc_captain_rupert() : CreatureScript("npc_captain_rupert") { }

        struct npc_captain_rupertAI : public npc_argent_captainAI
        {
            npc_captain_rupertAI(Creature* creature) : npc_argent_captainAI(creature)
            {
            }

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_RUPERT_FEL_IRON_BOMB, urand(15000, 20000));
                Events.ScheduleEvent(EVENT_RUPERT_MACHINE_GUN, urand(25000, 30000));
                Events.ScheduleEvent(EVENT_RUPERT_ROCKET_LAUNCH, urand(10000, 15000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RUPERT_FEL_IRON_BOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_FEL_IRON_BOMB);
                            Events.ScheduleEvent(EVENT_RUPERT_FEL_IRON_BOMB, urand(15000, 20000));
                            break;
                        case EVENT_RUPERT_MACHINE_GUN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_MACHINE_GUN);
                            Events.ScheduleEvent(EVENT_RUPERT_MACHINE_GUN, urand(25000, 30000));
                            break;
                        case EVENT_RUPERT_ROCKET_LAUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_ROCKET_LAUNCH);
                            Events.ScheduleEvent(EVENT_RUPERT_ROCKET_LAUNCH, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_captain_rupertAI>(creature);
        }
};

class npc_impaling_spear : public CreatureScript
{
    public:
        npc_impaling_spear() : CreatureScript("npc_impaling_spear") { }

        struct npc_impaling_spearAI : public CreatureAI
        {
            npc_impaling_spearAI(Creature* creature) : CreatureAI(creature)
            {
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                _vehicleCheckTimer = 500;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_vehicleCheckTimer <= diff)
                {
                    _vehicleCheckTimer = 500;
                    if (!me->GetVehicle())
                        me->DespawnOrUnsummon(100);
                }
                else
                    _vehicleCheckTimer -= diff;
            }

            uint32 _vehicleCheckTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_impaling_spearAI(creature);
        }
};

class npc_arthas_teleport_visual : public CreatureScript
{
    public:
        npc_arthas_teleport_visual() : CreatureScript("npc_arthas_teleport_visual") { }

        struct npc_arthas_teleport_visualAI : public NullCreatureAI
        {
            npc_arthas_teleport_visualAI(Creature* creature) : NullCreatureAI(creature), _instance(creature->GetInstanceScript())
            {
            }

            void Reset()
            {
                _events.Reset();
                if (_instance->GetBossState(DATA_PROFESSOR_PUTRICIDE) == DONE &&
                    _instance->GetBossState(DATA_BLOOD_QUEEN_LANA_THEL) == DONE &&
                    _instance->GetBossState(DATA_SINDRAGOSA) == DONE)
                    _events.ScheduleEvent(EVENT_SOUL_MISSILE, urand(1000, 6000));
            }

            void UpdateAI(uint32 const diff)
            {
                if (_events.Empty())
                    return;

                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_SOUL_MISSILE)
                {
                    DoCastAOE(SPELL_SOUL_MISSILE);
                    _events.ScheduleEvent(EVENT_SOUL_MISSILE, urand(5000, 7000));
                }
            }

        private:
            InstanceScript* _instance;
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            // Distance from the center of the spire
            if (creature->GetExactDist2d(4357.052f, 2769.421f) < 100.0f && creature->GetHomePosition().GetPositionZ() < 315.0f)
                return GetIcecrownCitadelAI<npc_arthas_teleport_visualAI>(creature);

            // Default to no script
            return NULL;
        }
};

class spell_icc_stoneform : public SpellScriptLoader
{
    public:
        spell_icc_stoneform() : SpellScriptLoader("spell_icc_stoneform") { }

        class spell_icc_stoneform_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_icc_stoneform_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* target = GetTarget()->ToCreature())
                {
                    target->SetReactState(REACT_PASSIVE);
                    target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    target->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_CUSTOM_SPELL_02);
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* target = GetTarget()->ToCreature())
                {
                    target->SetReactState(REACT_AGGRESSIVE);
                    target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    target->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_icc_stoneform_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_icc_stoneform_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_icc_stoneform_AuraScript();
        }
};

class spell_icc_sprit_alarm : public SpellScriptLoader
{
    public:
        spell_icc_sprit_alarm() : SpellScriptLoader("spell_icc_sprit_alarm") { }

        class spell_icc_sprit_alarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_sprit_alarm_SpellScript);

            void HandleEvent(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                uint32 trapId = 0;
                switch (GetSpellInfo()->Effects[effIndex].MiscValue)
                {
                    case EVENT_AWAKEN_WARD_1:
                        trapId = GO_SPIRIT_ALARM_1;
                        break;
                    case EVENT_AWAKEN_WARD_2:
                        trapId = GO_SPIRIT_ALARM_2;
                        break;
                    case EVENT_AWAKEN_WARD_3:
                        trapId = GO_SPIRIT_ALARM_3;
                        break;
                    case EVENT_AWAKEN_WARD_4:
                        trapId = GO_SPIRIT_ALARM_4;
                        break;
                    default:
                        return;
                }

                if (GameObject* trap = GetCaster()->FindNearestGameObject(trapId, 5.0f))
                    trap->SetRespawnTime(trap->GetGOInfo()->trap.autoCloseTime);

                std::list<Creature*> wards;
                GetCaster()->GetCreatureListWithEntryInGrid(wards, NPC_DEATHBOUND_WARD, 150.0f);
                wards.sort(JadeCore::ObjectDistanceOrderPred(GetCaster()));
                for (std::list<Creature*>::iterator itr = wards.begin(); itr != wards.end(); ++itr)
                {
                    if ((*itr)->isAlive() && (*itr)->HasAura(SPELL_STONEFORM))
                    {
                        (*itr)->AI()->Talk(SAY_TRAP_ACTIVATE);
                        (*itr)->RemoveAurasDueToSpell(SPELL_STONEFORM);
                        if (Unit* target = (*itr)->SelectNearestTarget(150.0f))
                            (*itr)->AI()->AttackStart(target);
                        break;
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_icc_sprit_alarm_SpellScript::HandleEvent, EFFECT_2, SPELL_EFFECT_SEND_EVENT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_sprit_alarm_SpellScript();
        }
};

class DeathPlagueTargetSelector
{
    public:
        explicit DeathPlagueTargetSelector(Unit* caster) : _caster(caster) {}

        bool operator()(WorldObject* object) const
        {
            if (object == _caster)
                return true;

            if (object->GetTypeId() != TYPEID_PLAYER)
                return true;

            if (object->ToUnit()->HasAura(SPELL_RECENTLY_INFECTED) || object->ToUnit()->HasAura(SPELL_DEATH_PLAGUE_AURA))
                return true;

            return false;
        }

    private:
        Unit* _caster;
};

class spell_frost_giant_death_plague : public SpellScriptLoader
{
    public:
        spell_frost_giant_death_plague() : SpellScriptLoader("spell_frost_giant_death_plague") { }

        class spell_frost_giant_death_plague_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_frost_giant_death_plague_SpellScript);

            bool Load()
            {
                _failed = false;
                return true;
            }

            // First effect
            void CountTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetCaster());
                _failed = targets.empty();
            }

            // Second effect
            void FilterTargets(std::list<WorldObject*>& targets)
            {
                // Select valid targets for jump
                targets.remove_if(DeathPlagueTargetSelector(GetCaster()));
                if (!targets.empty())
                {
                    WorldObject* target = JadeCore::Containers::SelectRandomContainerElement(targets);
                    targets.clear();
                    targets.push_back(target);
                }

                targets.push_back(GetCaster());
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (GetHitUnit() != GetCaster())
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_DEATH_PLAGUE_AURA, true);
                else if (_failed)
                    GetCaster()->CastSpell(GetCaster(), SPELL_DEATH_PLAGUE_KILL, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_frost_giant_death_plague_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_frost_giant_death_plague_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_frost_giant_death_plague_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }

            bool _failed;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_frost_giant_death_plague_SpellScript();
        }
};

class spell_icc_harvest_blight_specimen : public SpellScriptLoader
{
    public:
        spell_icc_harvest_blight_specimen() : SpellScriptLoader("spell_icc_harvest_blight_specimen") { }

        class spell_icc_harvest_blight_specimen_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_harvest_blight_specimen_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
            }

            void HandleQuestComplete(SpellEffIndex /*effIndex*/)
            {
                GetHitUnit()->RemoveAurasDueToSpell(uint32(GetEffectValue()));
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_icc_harvest_blight_specimen_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnEffectHitTarget += SpellEffectFn(spell_icc_harvest_blight_specimen_SpellScript::HandleQuestComplete, EFFECT_1, SPELL_EFFECT_QUEST_COMPLETE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_harvest_blight_specimen_SpellScript();
        }
};

class AliveCheck
{
    public:
        bool operator()(WorldObject* object) const
        {
            if (Unit* unit = object->ToUnit())
                return unit->isAlive();
            return true;
        }
};

class spell_svalna_revive_champion : public SpellScriptLoader
{
    public:
        spell_svalna_revive_champion() : SpellScriptLoader("spell_svalna_revive_champion") { }

        class spell_svalna_revive_champion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_svalna_revive_champion_SpellScript);

            void RemoveAliveTarget(std::list<WorldObject*>& targets)
            {
                targets.remove_if(AliveCheck());
                JadeCore::RandomResizeList(targets, 2);
            }

            void Land(SpellEffIndex /*effIndex*/)
            {
                Creature* caster = GetCaster()->ToCreature();
                if (!caster)
                    return;

                Position pos;
                caster->GetPosition(&pos);
                caster->GetNearPosition(pos, 5.0f, 0.0f);
                //pos.m_positionZ = caster->GetBaseMap()->GetHeight(caster->GetPhaseMask(), pos.GetPositionX(), pos.GetPositionY(), caster->GetPositionZ(), true, 50.0f);
                //pos.m_positionZ += 0.05f;
                caster->SetHomePosition(pos);
                caster->GetMotionMaster()->MoveLand(POINT_LAND, pos);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_svalna_revive_champion_SpellScript::RemoveAliveTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
                OnEffectHit += SpellEffectFn(spell_svalna_revive_champion_SpellScript::Land, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_svalna_revive_champion_SpellScript();
        }
};

class spell_svalna_remove_spear : public SpellScriptLoader
{
    public:
        spell_svalna_remove_spear() : SpellScriptLoader("spell_svalna_remove_spear") { }

        class spell_svalna_remove_spear_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_svalna_remove_spear_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Creature* target = GetHitCreature())
                {
                    if (Unit* vehicle = target->GetVehicleBase())
                        vehicle->RemoveAurasDueToSpell(SPELL_IMPALING_SPEAR);
                    target->DespawnOrUnsummon(1);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_svalna_remove_spear_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_svalna_remove_spear_SpellScript();
        }
};

class spell_icc_soul_missile : public SpellScriptLoader
{
    public:
        spell_icc_soul_missile() : SpellScriptLoader("spell_icc_soul_missile") { }

        class spell_icc_soul_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_soul_missile_SpellScript);

            void RelocateDest()
            {
                static Position const offset = {0.0f, 0.0f, 200.0f, 0.0f};
                const_cast<WorldLocation*>(GetExplTargetDest())->RelocateOffset(offset);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_icc_soul_missile_SpellScript::RelocateDest);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_soul_missile_SpellScript();
        }
};

class at_icc_saurfang_portal : public AreaTriggerScript
{
    public:
        at_icc_saurfang_portal() : AreaTriggerScript("at_icc_saurfang_portal") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (!instance || instance->GetBossState(DATA_DEATHBRINGER_SAURFANG) != DONE)
                return true;

            player->TeleportTo(631, 4126.35f, 2769.23f, 350.963f, 0.0f);

            if (instance->GetData(DATA_COLDFLAME_JETS) == NOT_STARTED)
            {
                // Process relocation now, to preload the grid and initialize traps
                player->GetMap()->PlayerRelocation(player, 4126.35f, 2769.23f, 350.963f, 0.0f);

                instance->SetData(DATA_COLDFLAME_JETS, IN_PROGRESS);
                std::list<Creature*> traps;
                GetCreatureListWithEntryInGrid(traps, player, NPC_FROST_FREEZE_TRAP, 120.0f);
                traps.sort(JadeCore::ObjectDistanceOrderPred(player));
                bool instant = false;
                for (std::list<Creature*>::iterator itr = traps.begin(); itr != traps.end(); ++itr)
                {
                    (*itr)->AI()->DoAction(instant ? 1000 : 11000);
                    instant = !instant;
                }
            }

            return true;
        }
};

class at_icc_shutdown_traps : public AreaTriggerScript
{
    public:
        at_icc_shutdown_traps() : AreaTriggerScript("at_icc_shutdown_traps") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                instance->SetData(DATA_COLDFLAME_JETS, DONE);
            return true;
        }
};

class at_icc_start_blood_quickening : public AreaTriggerScript
{
    public:
        at_icc_start_blood_quickening() : AreaTriggerScript("at_icc_start_blood_quickening") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetData(DATA_BLOOD_QUICKENING_STATE) == NOT_STARTED)
                    instance->SetData(DATA_BLOOD_QUICKENING_STATE, IN_PROGRESS);
            return true;
        }
};

class at_icc_start_frostwing_gauntlet : public AreaTriggerScript
{
    public:
        at_icc_start_frostwing_gauntlet() : AreaTriggerScript("at_icc_start_frostwing_gauntlet") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* crok = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_CROK_SCOURGEBANE)))
                    crok->AI()->DoAction(ACTION_START_GAUNTLET);
            return true;
        }
};

enum WardEnums
{
    SAY_AGGRO   = 0
};

class npc_sindragosas_ward : public CreatureScript
{
    public:
        npc_sindragosas_ward() : CreatureScript("npc_sindragosas_ward") { }

        struct npc_sindragosas_wardAI : public ScriptedAI
        {
            npc_sindragosas_wardAI(Creature* creature) : ScriptedAI(creature), _summons(me)
            {
                _instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                _events.Reset();
                _isEventInProgressOrDone = false;
                _spawnCountToBeSummonedInWave = 0;
                _waveNumber = 0;
                _summons.DespawnAll();

                if (_instance)
                    _instance->SetData(DATA_SINDRAGOSA_GAUNTLET, NOT_STARTED);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_START_GAUNTLET)
                    if (!_isEventInProgressOrDone)
                        if (!IsAnyPlayerOutOfRange())
                            DoZoneInCombat(me, 150.0f);
            }

            void EnterCombat(Unit* /*attacker*/)
            {
                Talk(SAY_AGGRO);
                _isEventInProgressOrDone = true;
                _spawnCountToBeSummonedInWave = 32;
                _waveNumber = 1;
                DoSummonWave(_waveNumber);
                _events.ScheduleEvent(EVENT_SUB_WAVE_1, 10000);
                _events.ScheduleEvent(EVENT_SUB_WAVE_2, 25000);
                _events.ScheduleEvent(EVENT_UPDATE_CHECK, 5000);
            }

            void DoSummonWave(uint8 number)
            {
                switch (number)
                {
                    case 1:
                    case 3:
                        me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[1], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[4], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[7], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_WEBWEAVER, SindragosaGauntletSpawn[10], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[2], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[5], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[8], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_NERUBAR_CHAMPION, SindragosaGauntletSpawn[11], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        break;
                    case 2:
                        me->SummonCreature(NPC_FROSTWARDEN_SORCERESS, SindragosaGauntletSpawn[3].GetPositionX() + 5, SindragosaGauntletSpawn[3].GetPositionY() - 5, SindragosaGauntletSpawn[3].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_FROSTWARDEN_SORCERESS, SindragosaGauntletSpawn[9].GetPositionX() - 5, SindragosaGauntletSpawn[9].GetPositionY() + 5, SindragosaGauntletSpawn[9].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletSpawn[3].GetPositionX() - 5, SindragosaGauntletSpawn[3].GetPositionY() + 5, SindragosaGauntletSpawn[3].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletSpawn[9].GetPositionX() + 5, SindragosaGauntletSpawn[9].GetPositionY() - 5, SindragosaGauntletSpawn[9].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletSpawn[3].GetPositionX(), SindragosaGauntletSpawn[3].GetPositionY(), SindragosaGauntletSpawn[3].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        me->SummonCreature(NPC_FROSTWARDEN_WARRIOR, SindragosaGauntletSpawn[9].GetPositionX(), SindragosaGauntletSpawn[9].GetPositionY(), SindragosaGauntletSpawn[9].GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        break;
                    case EVENT_SUB_WAVE_1:
                    case EVENT_SUB_WAVE_2:
                        for (uint8 i = 0; i < 12; i++)
                            me->SummonCreature(NPC_NERUBAR_BROODLING, SindragosaGauntletSpawn[i], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        break;
                    default:
                        break;
                }
            }

            bool IsAnyPlayerOutOfRange()
            {
                if (!me->GetMap())
                    return true;

                Map::PlayerList const& playerList = me->GetMap()->GetPlayers();

                if (playerList.isEmpty())
                    return true;

                for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                    {
                        if (player->isGameMaster())
                            continue;

                        if (player->isAlive() && me->GetDistance(player) > 125.0f)
                            return true;
                    }
                }

                return false;
            }

            void JustSummoned(Creature* summon)
            {
                _summons.Summon(summon);
                DoZoneInCombat(summon, 150.0f);
            }

            void JustDied(Unit* /*killer*/)
            {
                _summons.DespawnAll();
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*who*/)
            {
                _spawnCountToBeSummonedInWave--;
                summon->DespawnOrUnsummon(30000);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                // This one should never happen, if summoned creature despawns alive, reset!
                if (summon->isAlive())
                {
                    EnterEvadeMode();
                    return;
                }

                _summons.Despawn(summon);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || !_isEventInProgressOrDone)
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUB_WAVE_1:
                            DoSummonWave(EVENT_SUB_WAVE_1);
                            break;
                        case EVENT_SUB_WAVE_2:
                            DoSummonWave(EVENT_SUB_WAVE_2);
                            break;
                        case EVENT_UPDATE_CHECK:
                        {
                            if (_spawnCountToBeSummonedInWave <= 5)
                            {
                                if (_summons.size() < _spawnCountToBeSummonedInWave)
                                    _spawnCountToBeSummonedInWave = _summons.size();

                                if (!_spawnCountToBeSummonedInWave)
                                {
                                    switch (_waveNumber)
                                    {
                                        case 1:
                                            _spawnCountToBeSummonedInWave += 30;
                                            break;
                                        case 2:
                                            _spawnCountToBeSummonedInWave += 32;
                                            break;
                                        case 3:
                                            me->Kill(me);
                                            if (_instance)
                                                _instance->SetData(DATA_SINDRAGOSA_GAUNTLET, DONE);
                                            return;
                                    }

                                    _waveNumber++;
                                    DoSummonWave(_waveNumber);
                                    _events.ScheduleEvent(EVENT_SUB_WAVE_1, 10000);
                                    _events.ScheduleEvent(EVENT_SUB_WAVE_2, 25000);
                                }
                            }

                            if (IsAnyPlayerOutOfRange())
                            {
                                EnterEvadeMode();
                                return;
                            }

                            _events.ScheduleEvent(EVENT_UPDATE_CHECK, 5000);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

        private:
            bool _isEventInProgressOrDone;
            uint32 _spawnCountToBeSummonedInWave;
            uint8 _waveNumber;
            InstanceScript* _instance;
            EventMap _events;
            SummonList _summons;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_sindragosas_wardAI>(creature);
        }
};

class at_icc_start_sindragosa_gauntlet : public AreaTriggerScript
{
    public:
        at_icc_start_sindragosa_gauntlet() : AreaTriggerScript("at_icc_start_sindragosa_gauntlet") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* ward = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_SINDRAGOSA_GAUNTLET)))
                    ward->AI()->DoAction(ACTION_START_GAUNTLET);
            return true;
        }
};

class npc_blighted_abomination : public CreatureScript
{
    public:
        npc_blighted_abomination() : CreatureScript("npc_blighted_abomination") { }

        struct npc_blighted_abominationAI : public ScriptedAI
        {
            npc_blighted_abominationAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_CLOUD, 10000);
                Events.ScheduleEvent(EVENT_CLEAVE, 4000);
                Events.ScheduleEvent(EVENT_HOOK, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLOUD:
                            DoCastAOE(SPELL_PLAGUE_CLOUD);
                            Events.ScheduleEvent(EVENT_CLOUD, 20000);
                            break;
                        case EVENT_HOOK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_SCOURGE_HOOK);
                            Events.ScheduleEvent(EVENT_HOOK, urand(12000, 20000));
                            break;
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            Events.ScheduleEvent(EVENT_CLEAVE, 6000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_blighted_abominationAI(creature);
        }
};

class npc_vengeful_fleshreapert : public CreatureScript
{
    public:
        npc_vengeful_fleshreapert() : CreatureScript("npc_vengeful_fleshreapert") { }

        struct npc_vengeful_fleshreapertAI : public ScriptedAI
        {
            npc_vengeful_fleshreapertAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_JUMP_1, urand(3000, 20000));
                Events.ScheduleEvent(EVENT_DEVOUR, urand(3000, 5000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_JUMP_1:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_LEAPING_FACE_MAUL);
                            Events.ScheduleEvent(EVENT_JUMP_1, urand(20000,40000));
                            break;
                        case EVENT_DEVOUR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_DEVOUR_HUMANOID);
                            Events.ScheduleEvent(EVENT_DEVOUR, urand(5000,8000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_vengeful_fleshreapertAI(creature);
        }
};

class npc_plague_scientist : public CreatureScript
{
    public:
        npc_plague_scientist() : CreatureScript("npc_plague_scientist") { }

        struct npc_plague_scientistAI : public ScriptedAI
        {
            npc_plague_scientistAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_BLAST_PL, 2000);
                Events.ScheduleEvent(EVENT_STREAM, urand(8000, 12000));
                Events.ScheduleEvent(EVENT_SPRAY, urand(5000, 8000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STREAM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_PLAGUE_STREAM);
                            Events.ScheduleEvent(EVENT_STREAM_OUT, 24000);
                            break;
                        case EVENT_STREAM_OUT:
                            me->CastStop(SPELL_PLAGUE_STREAM);
                            Events.ScheduleEvent(EVENT_STREAM, 10000);
                            break;
                        case EVENT_SPRAY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_COMBOBULATING_SPRAY);
                            Events.ScheduleEvent(EVENT_STREAM, urand(8000,12000));
                            break;
                        case EVENT_BLAST_PL:
                            DoCastVictim(SPELL_PLAGUE_BLAST);
                            Events.ScheduleEvent(EVENT_BLAST_PL, 4000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_plague_scientistAI(creature);
        }
};

class npc_pustulating_horror : public CreatureScript
{
    public:
        npc_pustulating_horror() : CreatureScript("npc_pustulating_horror") { }

        struct npc_pustulating_horrorAI : public ScriptedAI
        {
            npc_pustulating_horrorAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_PUS, 2000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PUS:
                        {
                            if (HealthBelowPct(15))
                            {
                                DoCastAOE(SPELL_BLIGHT_BOMB);
                                Events.ScheduleEvent(EVENT_PUS, 5000);
                                break;
                            }
                            else
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0))
                                    DoCast(target, RAID_MODE(SPELL_BUBBLING_PUS_10N, SPELL_BUBBLING_PUS_25N, SPELL_BUBBLING_PUS_10N, SPELL_BUBBLING_PUS_25N));
                                Events.ScheduleEvent(EVENT_PUS, urand(15000, 25000));
                                break;
                            }
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_pustulating_horrorAI(creature);
        }
};

class npc_decaying_colossus : public CreatureScript
{
    public:
        npc_decaying_colossus() : CreatureScript("npc_decaying_colossus") { }

        struct npc_decaying_colossusAI : public ScriptedAI
        {
            npc_decaying_colossusAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_STOMP_PL, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STOMP_PL:
                            DoCastVictim(RAID_MODE(SPELL_MASSIVE_STOMP_10N, SPELL_MASSIVE_STOMP_25N, SPELL_MASSIVE_STOMP_10N, SPELL_MASSIVE_STOMP_25N));
                            Events.ScheduleEvent(EVENT_STOMP_PL, urand(15000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_decaying_colossusAI(creature);
        }
};

class npc_darkfallen_archmage : public CreatureScript
{
    public:
        npc_darkfallen_archmage() : CreatureScript("npc_darkfallen_archmage") { }

        struct npc_darkfallen_archmageAI : public ScriptedAI
        {
            npc_darkfallen_archmageAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_AMPLIFY, urand(10000, 15000));
                Events.ScheduleEvent(EVENT_BLAST, urand(8000, 10000));
                Events.ScheduleEvent(EVENT_FIREBALL, 2000);
                Events.ScheduleEvent(EVENT_POLYMORPH, urand(9000, 12000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_AMPLIFY:
                            DoCastVictim(RAID_MODE(SPELL_AMPLIFY_MAGIC_10N, SPELL_AMPLIFY_MAGIC_25N, SPELL_AMPLIFY_MAGIC_10N, SPELL_AMPLIFY_MAGIC_25N));
                            Events.ScheduleEvent(EVENT_AMPLIFY, urand(15000, 20000));
                            break;
                        case EVENT_POLYMORPH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_POLYMORPH_SPIDER);
                            Events.ScheduleEvent(EVENT_POLYMORPH, urand(15000, 20000));
                            break;
                        case EVENT_FIREBALL:
                            DoCastVictim(RAID_MODE(SPELL_FIREBALL_10N, SPELL_FIREBALL_25N, SPELL_FIREBALL_10N, SPELL_FIREBALL_25N));
                            Events.ScheduleEvent(EVENT_FIREBALL, urand(3000, 4000));
                            break;
                        case EVENT_BLAST:
                            DoCastAOE(RAID_MODE(SPELL_BLAST_WAVE_10N, SPELL_BLAST_WAVE_25N, SPELL_BLAST_WAVE_10N, SPELL_BLAST_WAVE_25N));
                            Events.ScheduleEvent(EVENT_BLAST, urand(10000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_archmageAI(creature);
        }
};

class npc_darkfallen_blood_knight : public CreatureScript
{
    public:
        npc_darkfallen_blood_knight() : CreatureScript("npc_darkfallen_blood_knight") { }

        struct npc_darkfallen_blood_knightAI : public ScriptedAI
        {
            npc_darkfallen_blood_knightAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_AURA, urand(12000, 15000));
                Events.ScheduleEvent(EVENT_STRIKE, urand(2000, 3000));
                Events.ScheduleEvent(EVENT_MIRROR, urand(4000, 5000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STRIKE:
                            DoCastVictim(SPELL_UNHOLY_STRIKE);
                            Events.ScheduleEvent(EVENT_STRIKE, urand(3000, 4000));
                            break;
                        case EVENT_AURA:
                            DoCast(me, SPELL_VAMPIRIC_AURA);
                            Events.ScheduleEvent(EVENT_AURA, urand(12000, 15000));
                        case EVENT_MIRROR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            {
                                DoCastVictim(SPELL_BLOOD_MIRROR_DUMMY);
                                me->getVictim()->CastSpell(target, SPELL_BLOOD_MIRROR_DAMAGE, true);
                                me->CastSpell(me->getVictim(), SPELL_BLOOD_MIRROR_BUFF, true);
                            }
                            Events.ScheduleEvent(EVENT_MIRROR, urand(32000, 37000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_blood_knightAI(creature);
        }
};

enum Noblemisc
{
    NPC_VAMPIRIC_FIEND = 37901,
};

class npc_darkfallen_noble : public CreatureScript
{
    public:
        npc_darkfallen_noble() : CreatureScript("npc_darkfallen_noble") { }

        struct npc_darkfallen_nobleAI : public ScriptedAI
        {
            npc_darkfallen_nobleAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_CHAINS, urand(2000, 4000));
                Events.ScheduleEvent(EVENT_BOLT, urand(3000, 5000));
                Events.ScheduleEvent(EVENT_FIEND, 15000);
            }

            void JustSummoned(Creature* summon)
            {
                DoZoneInCombat(summon, 150.0f);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOLT:
                            DoCastVictim(RAID_MODE(SPELL_SHADOW_BOLT_10N, SPELL_SHADOW_BOLT_25N, SPELL_SHADOW_BOLT_10N, SPELL_SHADOW_BOLT_25N));
                            Events.ScheduleEvent(EVENT_BOLT, urand(3000, 4000));
                            break;
                        case EVENT_CHAINS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_CHAINS_OF_SHADOW);
                            Events.ScheduleEvent(EVENT_CHAINS, urand(20000, 25000));
                            break;
                        case EVENT_FIEND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                me->SummonCreature(NPC_VAMPIRIC_FIEND, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                            Events.ScheduleEvent(EVENT_FIEND, 60000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_nobleAI(creature);
        }
};

class npc_vampiric_fiend : public CreatureScript
{
    public:
        npc_vampiric_fiend() : CreatureScript("npc_vampiric_fiend") { }

        struct npc_vampiric_fiendAI : public ScriptedAI
        {
            npc_vampiric_fiendAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_LEECHING, 10000);
            }

            void EnterCombat(Unit* /*target*/)
            {
                DoCast(me, SPELL_DISEASE_CLOUD);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LEECHING:
                            DoCastAOE(SPELL_LEECHING_ROT);
                            me->DespawnOrUnsummon(3000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_vampiric_fiendAI(creature);
        }
};

class npc_darkfallen_advisor : public CreatureScript
{
    public:
        npc_darkfallen_advisor() : CreatureScript("npc_darkfallen_advisor") { }

        struct npc_darkfallen_advisorAI : public ScriptedAI
        {
            npc_darkfallen_advisorAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_LICH, urand(2000, 5000));
                Events.ScheduleEvent(EVENT_PROTECTION, urand(10000, 15000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LICH:
                            DoCastVictim(RAID_MODE(SPELL_LICH_SLAP_10N, SPELL_LICH_SLAP_25N, SPELL_LICH_SLAP_10N, SPELL_LICH_SLAP_25N));
                            Events.ScheduleEvent(EVENT_LICH, 10000);
                            break;
                        case EVENT_PROTECTION:
                            if (Unit* target = urand(0, 1) ? SelectTarget(SELECT_TARGET_RANDOM, 0) : DoSelectLowestHpFriendly(40.0f))
                            {
                                DoCast(target, SPELL_SHROUD_OF_PROTECTION);
                                DoCast(target, SPELL_SHROUD_OF_SPELL_WARDING);
                            }
                            Events.ScheduleEvent(EVENT_PROTECTION, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_advisorAI(creature);
        }
};

class npc_darkfallen_commander : public CreatureScript
{
    public:
        npc_darkfallen_commander() : CreatureScript("npc_darkfallen_commander") { }

        struct npc_darkfallen_commanderAI : public ScriptedAI
        {
            npc_darkfallen_commanderAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_RUSH, urand(4000, 8000));
                Events.ScheduleEvent(EVENT_SHOUT, urand(8000, 10000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHOUT:
                            DoCast(me, SPELL_BATTLE_SHOUT);
                            Events.ScheduleEvent(EVENT_SHOUT, urand(15000, 20000));
                            break;
                        case EVENT_RUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, RAID_MODE(SPELL_VAMPIRE_RUSH_10N, SPELL_VAMPIRE_RUSH_25N, SPELL_VAMPIRE_RUSH_10N, SPELL_VAMPIRE_RUSH_25N));
                            Events.ScheduleEvent(EVENT_RUSH, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_commanderAI(creature);
        }
};

class npc_darkfallen_lieutenant : public CreatureScript
{
    public:
        npc_darkfallen_lieutenant() : CreatureScript("npc_darkfallen_lieutenant") { }

        struct npc_darkfallen_lieutenantAI : public ScriptedAI
        {
            npc_darkfallen_lieutenantAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_REND, urand(1000, 2000));
                Events.ScheduleEvent(EVENT_CURSE, urand(8000, 15000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_REND:
                            DoCastVictim(RAID_MODE(SPELL_REND_FLESH_10N, SPELL_REND_FLESH_25N, SPELL_REND_FLESH_10N, SPELL_REND_FLESH_25N));
                            Events.ScheduleEvent(EVENT_REND, 25000);
                            break;
                        case EVENT_CURSE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_VAMPIRIC_CURSE);
                            Events.ScheduleEvent(EVENT_REND, urand(10000,20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_lieutenantAI(creature);
        }
};

class npc_darkfallen_tactician : public CreatureScript
{
    public:
        npc_darkfallen_tactician() : CreatureScript("npc_darkfallen_tactician") { }

        struct npc_darkfallen_tacticianAI : public ScriptedAI
        {
            npc_darkfallen_tacticianAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 m_uiSHADOWSTEP_Timer;
            uint32 m_uiSAP_Timer;
            uint32 m_uiSTRIKE_Timer;

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_SHADOW_STEP, urand(1000, 2000));
                Events.ScheduleEvent(EVENT_SAP, urand(5000, 15000));
                Events.ScheduleEvent(EVENT_STRIKER, urand(2000, 3000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STRIKER:
                            DoCastVictim(SPELL_UNHOLY_STRIKE);
                            Events.ScheduleEvent(EVENT_STRIKER, 6000);
                            break;
                        case EVENT_SAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_BLOOD_SAP);
                            Events.ScheduleEvent(EVENT_SAP, urand(15000, 25000));
                            break;
                        case EVENT_SHADOW_STEP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_SHADOWSTEP);
                            Events.ScheduleEvent(EVENT_SHADOW_STEP, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_darkfallen_tacticianAI(creature);
        }
};

class npc_ymirjar_deathbringer : public CreatureScript
{
    public:
        npc_ymirjar_deathbringer() : CreatureScript("npc_ymirjar_deathbringer") { }

        struct npc_ymirjar_deathbringerAI : public ScriptedAI
        {
            npc_ymirjar_deathbringerAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_BANISH, urand(5000, 10000));
                Events.ScheduleEvent(EVENT_EMBRACE, urand(10000, 15000));
                Events.ScheduleEvent(EVENT_BOLT_FW, urand(1000, 2000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EMBRACE:
                            DoCast(me, RAID_MODE(SPELL_DEATHS_EMBRACE_10N, SPELL_DEATHS_EMBRACE_25N, SPELL_DEATHS_EMBRACE_10N, SPELL_DEATHS_EMBRACE_25N));
                            Events.ScheduleEvent(EVENT_EMBRACE, urand(15000, 25000));
                            break;
                        case EVENT_BANISH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_BANISH);
                            Events.ScheduleEvent(EVENT_BANISH, urand(15000, 25000));
                            break;
                        case EVENT_BOLT_FW:
                            DoCastVictim(RAID_MODE(SPELL_SHADOW_BOLT_YMIRJAR_10N, SPELL_SHADOW_BOLT_YMIRJAR_25N, SPELL_SHADOW_BOLT_YMIRJAR_10N, SPELL_SHADOW_BOLT_YMIRJAR_25N));
                            Events.ScheduleEvent(EVENT_BOLT_FW, 2000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_ymirjar_deathbringerAI(creature);
        }
};

class npc_ymirjar_frostbinder : public CreatureScript
{
    public:
        npc_ymirjar_frostbinder() : CreatureScript("npc_ymirjar_frostbinder") { }

        struct npc_ymirjar_frostbinderAI : public ScriptedAI
        {
            npc_ymirjar_frostbinderAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            uint32 m_uiORB_Timer;

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_ORB, 2000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ORB:
                            DoCastAOE(SPELL_FROZEN_ORB);
                            Events.ScheduleEvent(EVENT_ORB, urand(3000, 5000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_ymirjar_frostbinderAI(creature);
        }
};

class npc_ymirjar_battlemaiden : public CreatureScript
{
    public:
        npc_ymirjar_battlemaiden() : CreatureScript("npc_ymirjar_battlemaiden") { }

        struct npc_ymirjar_battlemaidenAI : public ScriptedAI
        {
            npc_ymirjar_battlemaidenAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_RUSH_FW, urand(10000, 15000));
                Events.ScheduleEvent(EVENT_STRIKE_FW, urand(1000, 5000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STRIKE_FW:
                            DoCastVictim(SPELL_BARBARIC_STRIKE);
                            Events.ScheduleEvent(EVENT_STRIKE_FW, urand(2000, 3000));
                            break;
                        case EVENT_RUSH_FW:
                            DoCast(me, SPELL_ADRENALINE_RUSH);
                            Events.ScheduleEvent(EVENT_RUSH_FW, urand(15000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_ymirjar_battlemaidenAI(creature);
        }
};

class npc_ymirjar_huntress : public CreatureScript
{
    public:
        npc_ymirjar_huntress() : CreatureScript("npc_ymirjar_huntress") { }

        struct npc_ymirjar_huntressAI : public ScriptedAI
        {
            npc_ymirjar_huntressAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_TRAP, urand(5000, 15000));
                Events.ScheduleEvent(EVENT_RSHOT, urand(10000, 15000));
                Events.ScheduleEvent(EVENT_SHOT, urand(1000, 2000));
                Events.ScheduleEvent(EVENT_VOLLEY, urand(5000, 15000));
            }

            void EnterCombat(Unit* /*target*/)
            {
                if (Is25ManRaid())
                    DoCast(SPELL_SUMMON_WARHAWK);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RSHOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_RAPID_SHOT);
                            Events.ScheduleEvent(EVENT_RSHOT, urand(15000, 25000));
                            break;
                        case EVENT_VOLLEY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_VOLLEY);
                            Events.ScheduleEvent(EVENT_VOLLEY, urand(10000, 15000));
                            break;
                        case EVENT_TRAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_ICE_TRAP);
                            Events.ScheduleEvent(EVENT_TRAP, urand(30000, 35000));
                            break;
                        case EVENT_SHOT:
                            DoCastVictim(SPELL_SHOOT);
                            break;
                        default:
                            break;
                    }
                }

                if (me->isAttackReady())
                {
                    if (me->IsWithinMeleeRange(me->getVictim()))
                        DoMeleeAttackIfReady();
                    else
                        Events.ScheduleEvent(EVENT_SHOT, 1000);
                }
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_ymirjar_huntressAI(creature);
        }
};

class npc_ymirjar_warlord : public CreatureScript
{
    public:
        npc_ymirjar_warlord() : CreatureScript("npc_ymirjar_warlord") { }

        struct npc_ymirjar_warlordAI : public ScriptedAI
        {
            npc_ymirjar_warlordAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            uint32 m_uiWHIRLWIND_Timer;

            void Reset()
            {
                Events.Reset();
                Events.ScheduleEvent(EVENT_WHIRLWIND, urand(5000, 15000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WHIRLWIND:
                            DoCastAOE(SPELL_WHIRLWIND);
                            Events.ScheduleEvent(EVENT_WHIRLWIND, urand(12000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        protected:
            EventMap Events;
        };

        CreatureAI *GetAI(Creature *creature) const
        {
            return new npc_ymirjar_warlordAI(creature);
        }
};

void AddSC_icecrown_citadel()
{
    new npc_icc_ring_recover();
    new npc_icc_buff_remover();
    new npc_highlord_tirion_fordring_lh();
    new npc_rotting_frost_giant();
    new npc_frost_freeze_trap();
    new npc_alchemist_adrianna();
    new boss_sister_svalna();
    new npc_crok_scourgebane();
    new npc_captain_arnath();
    new npc_captain_brandon();
    new npc_captain_grondel();
    new npc_captain_rupert();
    new npc_impaling_spear();
    new npc_arthas_teleport_visual();
    new spell_icc_stoneform();
    new spell_icc_sprit_alarm();
    new spell_frost_giant_death_plague();
    new spell_icc_harvest_blight_specimen();
    new spell_trigger_spell_from_caster("spell_svalna_caress_of_death", SPELL_IMPALING_SPEAR_KILL);
    new spell_svalna_revive_champion();
    new spell_svalna_remove_spear();
    new spell_icc_soul_missile();
    new at_icc_saurfang_portal();
    new at_icc_shutdown_traps();
    new at_icc_start_blood_quickening();
    new at_icc_start_frostwing_gauntlet();
    new npc_sindragosas_ward();
    new at_icc_start_sindragosa_gauntlet();
    new npc_blighted_abomination();
    new npc_vengeful_fleshreapert();
    new npc_plague_scientist();
    new npc_pustulating_horror();
    new npc_decaying_colossus();
    new npc_darkfallen_archmage();
    new npc_darkfallen_blood_knight();
    new npc_darkfallen_noble();
    new npc_vampiric_fiend();
    new npc_darkfallen_advisor();
    new npc_darkfallen_commander();
    new npc_darkfallen_lieutenant();
    new npc_darkfallen_tactician();
    new npc_ymirjar_deathbringer();
    new npc_ymirjar_frostbinder();
    new npc_ymirjar_battlemaiden();
    new npc_ymirjar_huntress();
    new npc_ymirjar_warlord();
}
