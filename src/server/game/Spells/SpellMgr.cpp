/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ObjectMgr.h"
#include "SpellAuras.h"
#include "SpellAuraDefines.h"
#include "SharedDefines.h"
#include "DBCStores.h"
#include "World.h"
#include "Chat.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "CreatureAI.h"
#include "MapManager.h"
#include "BattlegroundIC.h"
#include "BattlefieldWG.h"
#include "BattlefieldMgr.h"
#include "InstanceScript.h"

bool IsPrimaryProfessionSkill(uint32 skill)
{
    SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(skill);
    if (!pSkill)
        return false;

    if (pSkill->categoryId != SKILL_CATEGORY_PROFESSION)
        return false;

    return true;
}

bool IsPartOfSkillLine(uint32 skillId, uint32 spellId)
{
    SkillLineAbilityMapBounds skillBounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
    for (SkillLineAbilityMap::const_iterator itr = skillBounds.first; itr != skillBounds.second; ++itr)
        if (itr->second->skillId == skillId)
            return true;

    return false;
}

DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellInfo const* spellproto, bool triggered)
{
    if (spellproto->IsPositive())
        return DIMINISHING_NONE;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellproto->Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
            return DIMINISHING_TAUNT;
    }

    // Explicit Diminishing Groups
    switch (spellproto->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (spellproto->Id)
            {
                case 20549:  // War Stomp
                case 47481:  // Gnaw
                case 96201:  // Web Wrap
                    return DIMINISHING_CONTROLLED_STUN;
                case 25046:  // Arcane Torrent (Energy)
                case 28730:  // Arcane Torrent (Mana)
                case 50613:  // Arcane Torrent (Death Knight)
                case 69179:  // Arcane Torrent (Warrior)
                case 80483:  // Arcane Torrent (Hunter)
                case 129597: // Arcane Torrent (Monk)
                    return DIMINISHING_SILENCE;
                case 107079: // Quaking Palm
                    return DIMINISHING_DISORIENT;
            }

            // Pet charge effects (Infernal Awakening, Demon Charge)
            if (spellproto->SpellVisual[0] == 2816 && spellproto->SpellIconID == 15)
                return DIMINISHING_CONTROLLED_STUN;
            // Frost Tomb
            else if (spellproto->Id == 48400)
                return DIMINISHING_NONE;
            // Earthquake (Trash, Ulduar)
            else if (spellproto->Id == 64697)
                return DIMINISHING_NONE;
            // Summoning Disorientation
            else if (spellproto->Id == 32752)
                return DIMINISHING_NONE;
            else if (spellproto->SpellVisual[0] == 14153)
                return DIMINISHING_NONE;
            // Silence, Asira Dawnslayer, Hour of Twilight
            else if (spellproto->Id == 103587)
                return DIMINISHING_NONE;
            // Glyph of Intimidating Shout
            else if (spellproto->Id == 95199)
                return DIMINISHING_LIMITONLY;
            break;
        }
        // Event spells
        case SPELLFAMILY_UNK1:
            return DIMINISHING_NONE;
        case SPELLFAMILY_MAGE:
        {
            switch (spellproto->Id)
            {
                case 122:    // Frost Nova
                case 33395:  // Freeze
                    return DIMINISHING_CONTROLLED_ROOT;
                case 44572:  // Deep Freeze
                case 118271: // Combustion Impact
                    return DIMINISHING_CONTROLLED_STUN;
                case 118:    // Polymorph
                case 82691:  // Ring of Frost
                    return DIMINISHING_DISORIENT;
                case 55021:  // Improved Counter Spell
                case 102051: // Frostjaw
                    return DIMINISHING_SILENCE;
                case 111340: // Ice Ward
                    return DIMINISHING_ICE_WARD;
                case 31589:  // Slow
                    return DIMINISHING_LIMITONLY;
            }

            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            switch (spellproto->Id)
            {
                case 107566: // Staggering Shout
                case 105771: // Warbringer
                    return DIMINISHING_CONTROLLED_ROOT;
                case 132168: // Shockwave
                case 132169: // Storm Bolt
                    return DIMINISHING_CONTROLLED_STUN;
                case 676:    // Disarm
                    return DIMINISHING_DISARM;
                case 5246:   // Intimidating Shout
                    return DIMINISHING_FEAR;
                case 118895: // Dragon Roar
                    return DIMINISHING_RANDOM_STUN;
                case 6552:   // Pummel
                case 18498:  // Glyph of Gag Order
                    return DIMINISHING_SILENCE;
            }

            // Piercing Howl - limit duration to 6s in PvP
            if (spellproto->Id == 12323)
                return DIMINISHING_LIMITONLY;
            // Hamstring - limit duration to 10s in PvP
            if (spellproto->SpellFamilyFlags[0] & 0x2)
                return DIMINISHING_LIMITONLY;
            // Charge Stun (own diminishing)
            else if (spellproto->SpellFamilyFlags[0] & 0x01000000)
                return DIMINISHING_CHARGE;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            switch (spellproto->Id)
            {
                case 22703:  // Infernal Awakening
                case 30283:  // Shadowfury
                case 89766:  // Axe Toss
                    return DIMINISHING_CONTROLLED_STUN;
                case 118093: // Voidwalker: Disarm
                    return DIMINISHING_DISARM;
                case 6358:   // Seduction
                case 115268: // Mesmerize
                case 132412: // Seduction
                    return DIMINISHING_DISORIENT;
                case 118699: // Fear
                case 5484:   // Howl of Terror
                case 104045: // Sleep
                    return DIMINISHING_FEAR;
                case 6789:   // Mortal Coil
                    return DIMINISHING_HORROR;
                case 24259:  // Spell Lock
                case 115782: // Optical Blast
                    return DIMINISHING_SILENCE;
                case 710:    // Banish
                    return DIMINISHING_BANISH;
                case 31117:  // Unstable Affliction
                    return DIMINISHING_LIMITONLY;
            }

            // Curses/etc
            if ((spellproto->SpellFamilyFlags[0] & 0x80000000) || (spellproto->SpellFamilyFlags[1] & 0x200))
                return DIMINISHING_LIMITONLY;
            // Curse of Exhaustion
            else if (spellproto->Id == 18223)
                return DIMINISHING_LIMITONLY;

            break;
        }
        case SPELLFAMILY_DRUID:
        {
            switch (spellproto->Id)
            {
                case 9005:  // Pounce
                case 22570: // Maim
                case 102456:// Pounce (Incarnation)
                case 5211:  // Mighty Bash
                case 102795:// Bear Hug
                case 113801:// Bash
                    return DIMINISHING_CONTROLLED_STUN;
                case 33786: // Cyclone
                case 113506:// Cyclone (Symbiosis)
                    return DIMINISHING_CYCLONE;
                case 339:   // Entangling Roots
                case 19975: // Entangling Roots (Nature's Grasp)
                case 102359:// Mass Entanglement
                    return DIMINISHING_CONTROLLED_ROOT;
                case 81261: // Solar Beam
                    return DIMINISHING_SILENCE;
                case 770:   // Faerie Fire
                case 102355:// Faerie Swarm
                    return DIMINISHING_LIMITONLY;
                case 45334: // Feral Charge: Bear Effect
                    return DIMINISHING_RANDOM_ROOT;
                case 2637:  // Hibernate
                    return DIMINISHING_DISORIENT;
                default:
                    break;
            }

            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            switch (spellproto->Id)
            {
                case 115197: // Partial Paralysis
                    return DIMINISHING_CONTROLLED_ROOT;
                case 1833:   // Cheap Shot
                case 408:    // Kidney Shot
                    return DIMINISHING_CONTROLLED_STUN;
                case 51722:  // Dismantle
                    return DIMINISHING_DISARM;
                case 1776:   // Gouge
                case 6770:   // Sap
                    return DIMINISHING_DISORIENT;
                case 2094:   // Blind
                    return DIMINISHING_FEAR;
                case 113953: // Paralytic Poison
                    return DIMINISHING_RANDOM_STUN;
                case 1330:   // Garrote
                    return DIMINISHING_SILENCE;
            }

            // Crippling poison - Limit to 10 seconds in PvP (No SpellFamilyFlags)
            if (spellproto->SpellIconID == 163)
                return DIMINISHING_LIMITONLY;

            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            switch (spellproto->Id)
            {
                case 4167:   // Web
                case 50245:  // Pin
                case 53148:  // Charge
                case 54706:  // Venom Web Spray
                case 90327:  // Lock Jaw
                case 136634: // Narrow Escape
                    return DIMINISHING_CONTROLLED_ROOT;
                case 24394:  // Intimidation
                case 50519:  // Sonic Blast
                case 56626:  // Sting
                case 90337:  // Bad Manner
                case 117526: // Binding Shot
                    return DIMINISHING_CONTROLLED_STUN;
                case 50541:  // Clench
                case 91644:  // Snatch
                    return DIMINISHING_DISARM;
                case 19386:  // Wyvern Sting
                case 3355:   // Freezing Trap
                    return DIMINISHING_DISORIENT;
                case 1513:   // Scare Beast
                    return DIMINISHING_FEAR;
                case 34490:  // Silencing Shot
                    return DIMINISHING_SILENCE;
                case 64803:  // Entrapment
                    return DIMINISHING_ENTRAPMENT;
            }

            // Hunter's Mark
            if ((spellproto->SpellFamilyFlags[0] & 0x400) && spellproto->SpellIconID == 538)
                return DIMINISHING_LIMITONLY;

            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            switch (spellproto->Id)
            {
                case 853:    // Hammer of Justice
                case 105593: // Fist of Justice
                case 115752: // Blinding Light (Glyphed)
                case 119072: // Holy Wrath
                    return DIMINISHING_CONTROLLED_STUN;
                case 20066:  // Repentance
                    return DIMINISHING_DISORIENT;
                case 10326:  // Turn Evil
                case 145067: // Turn Evil
                case 105421: // Blinding Light
                    return DIMINISHING_FEAR;
                case 31935:  // Avenger's Shield
                    return DIMINISHING_SILENCE;
            }

            // Judgement of Justice - limit duration to 10s in PvP
            if (spellproto->SpellFamilyFlags[0] & 0x100000)
                return DIMINISHING_LIMITONLY;

            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            switch (spellproto->Id)
            {
                case 96294:  // Chains of Ice
                    return DIMINISHING_CONTROLLED_ROOT;
                case 91797:  // Monstrous Blow
                case 91800:  // Gnaw
                case 108194: // Asphyxiate
                case 115001: // Remorseless Winter
                    return DIMINISHING_CONTROLLED_STUN;
                case 47476:  // Strangulate
                    return DIMINISHING_SILENCE;
            }

            // Hungering Cold (no flags)
            if (spellproto->SpellIconID == 2797)
                return DIMINISHING_DISORIENT;
            // Mark of Blood
            else if ((spellproto->SpellFamilyFlags[0] & 0x10000000) && spellproto->SpellIconID == 2285)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_MONK:
        {
            switch (spellproto->Id)
            {
                case 116706: // Disable
                    return DIMINISHING_CONTROLLED_ROOT;
                case 120086: // Fists of Fury
                case 119381: // Leg Sweep
                case 119392: // Charging Ox Wave
                case 122242: // Clash
                    return DIMINISHING_CONTROLLED_STUN;
                case 117368: // Grapple Weapon
                    return DIMINISHING_DISARM;
                case 115078: // Disable
                    return DIMINISHING_DISORIENT;
                case 116709: // Spear Hand Strike
                    return DIMINISHING_SILENCE;
                case 116095: // Disable (reduce movement speed)
                    return DIMINISHING_LIMITONLY;
            }

            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            switch (spellproto->Id)
            {
                case 87194:  // Glyph of Mind Blast
                case 114404: // Void Tendril's Grasp
                    return DIMINISHING_CONTROLLED_ROOT;
                case 64058:  // Psychic Horror
                    return DIMINISHING_DISARM;
                case 9484:   // Shackle Undead
                    return DIMINISHING_DISORIENT;
                case 8122:   // Psychic Scream
                case 113792: // Psychic Terror
                    return DIMINISHING_FEAR;
                case 64044:  // Psychic Horror
                case 87204:  // Sin and Punishment
                    return DIMINISHING_HORROR;
                case 15487:  // Silence
                    return DIMINISHING_SILENCE;
                case 605:    // Dominate Mind
                    return DIMINISHING_DOMINATE_MIND;
                case 88625:  // Holy Word: Chastise
                    return DIMINISHING_NONE;
            }

            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            switch (spellproto->Id)
            {
                case 63685:  // Freeze
                case 64695:  // Earthgrab
                    return DIMINISHING_CONTROLLED_ROOT;
                case 118905: // Static Charge
                    return DIMINISHING_CONTROLLED_STUN;
                case 51514:  // Hex
                    return DIMINISHING_DISORIENT;
                case 77505:  // Earthquake
                    return DIMINISHING_RANDOM_STUN;
                case 76780:  // Bind Elemental
                    return DIMINISHING_BIND_ELEMENTAL;
            }

            break;
        }
        default:
            break;
    }

    // Lastly - Set diminishing depending on mechanic
    uint32 mechanic = spellproto->GetAllEffectsMechanicMask();
    if (mechanic & (1 << MECHANIC_CHARM))
        return DIMINISHING_MIND_CONTROL;
    if (mechanic & (1 << MECHANIC_SILENCE))
        return DIMINISHING_SILENCE;
    if (mechanic & (1 << MECHANIC_SLEEP))
        return DIMINISHING_SLEEP;
    if (mechanic & ((1 << MECHANIC_SAPPED) | (1 << MECHANIC_POLYMORPH) | (1 << MECHANIC_SHACKLE)))
        return DIMINISHING_DISORIENT;
    // Mechanic Knockout, except Blast Wave
    if (mechanic & (1 << MECHANIC_KNOCKOUT) && spellproto->SpellIconID != 292)
        return DIMINISHING_DISORIENT;
    if (mechanic & (1 << MECHANIC_DISARM))
        return DIMINISHING_DISARM;
    if (mechanic & (1 << MECHANIC_FEAR))
        return DIMINISHING_FEAR;
    if (mechanic & (1 << MECHANIC_STUN))
        return triggered ? DIMINISHING_RANDOM_STUN : DIMINISHING_CONTROLLED_STUN;
    if (mechanic & (1 << MECHANIC_BANISH))
        return DIMINISHING_BANISH;
    if (mechanic & (1 << MECHANIC_ROOT))
        return triggered ? DIMINISHING_RANDOM_ROOT : DIMINISHING_CONTROLLED_ROOT;
    if (mechanic & (1 << MECHANIC_HORROR))
        return DIMINISHING_HORROR;

    return DIMINISHING_NONE;
}

DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_RANDOM_STUN:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_CHARGE:
            return DRTYPE_ALL;
        case DIMINISHING_LIMITONLY:
        case DIMINISHING_NONE:
            return DRTYPE_NONE;
        default:
            return DRTYPE_PLAYER;
    }
}

DiminishingLevels GetDiminishingReturnsMaxLevel(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
            return DIMINISHING_LEVEL_TAUNT_IMMUNE;
        default:
            return DIMINISHING_LEVEL_IMMUNE;
    }
}

int32 GetDiminishingReturnsLimitDuration(DiminishingGroup group, SpellInfo const* spellproto)
{
    if (!IsDiminishingReturnsGroupDurationLimited(group))
        return 0;

    // Explicit diminishing duration
    switch (spellproto->SpellFamilyName)
    {
        case SPELLFAMILY_DRUID:
        {
            switch (spellproto->Id)
            {
                case 770:   // Faerie Fire - limit to 20 seconds in PvP (5.4)
                    return 20 * IN_MILLISECONDS;
                case 102355:// Faerie Swarm - limit to 40 seconds in PvP (3.1)
                    return 40 * IN_MILLISECONDS;
                default:
                    break;
            }

            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            // Piercing Howl
            if (spellproto->Id == 12323)
                return 6 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Wyvern Sting
            if (spellproto->SpellFamilyFlags[1] & 0x1000)
                return 6 * IN_MILLISECONDS;
            // Hunter's Mark
            if (spellproto->SpellFamilyFlags[0] & 0x400)
                return 30 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Repentance - limit to 6 seconds in PvP
            if (spellproto->SpellFamilyFlags[0] & 0x4)
                return 6 * IN_MILLISECONDS;
             // Turn Evil - limit to 8 seconds in PvP
            else if (spellproto->Id == 145067 || spellproto->Id == 10326)
                 return 8 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Banish - limit to 6 seconds in PvP
            if (spellproto->SpellFamilyFlags[1] & 0x8000000)
                return 6 * IN_MILLISECONDS;
            // Curse of Tongues - limit to 12 seconds in PvP
            else if (spellproto->SpellFamilyFlags[2] & 0x800)
                return 12 * IN_MILLISECONDS;
            // Curse of Elements - limit to 120 seconds in PvP
            else if (spellproto->SpellFamilyFlags[1] & 0x200)
                return 120 * IN_MILLISECONDS;
            // Curse of Exhaustion
            else if (spellproto->Id == 18223)
                return 10 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_MONK:
        {
            // Disable (reduce movement speed) - limit to 8 seconds in PvP
            if (spellproto->Id == 116095)
                return 8 * IN_MILLISECONDS;
            // Disable (root) - limit to 4 seconds in PvP
            else if (spellproto->Id == 116706)
                return 4 * IN_MILLISECONDS;
            break;
        }
        default:
            break;
    }

    return 8 * IN_MILLISECONDS;
}

bool IsDiminishingReturnsGroupDurationLimited(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_BANISH:
        case DIMINISHING_CONTROLLED_STUN:
        case DIMINISHING_CONTROLLED_ROOT:
        case DIMINISHING_CYCLONE:
        case DIMINISHING_DISORIENT:
        case DIMINISHING_ENTRAPMENT:
        case DIMINISHING_FEAR:
        case DIMINISHING_HORROR:
        case DIMINISHING_MIND_CONTROL:
        case DIMINISHING_OPENING_STUN:
        case DIMINISHING_RANDOM_ROOT:
        case DIMINISHING_RANDOM_STUN:
        case DIMINISHING_SLEEP:
        case DIMINISHING_LIMITONLY:
        case DIMINISHING_BIND_ELEMENTAL:
        case DIMINISHING_DOMINATE_MIND:
        case DIMINISHING_ICE_WARD:
        case DIMINISHING_PARALYTIC_POISON:
        case DIMINISHING_RING_OF_FROST:
        case DIMINISHING_DEEP_FREEZE:
        case DIMINISHING_DRAGONS_BREATH:
            return true;
        default:
            return false;
    }
}

SpellMgr::SpellMgr()
{
}

SpellMgr::~SpellMgr()
{
    UnloadSpellInfoStore();
}

/// Some checks for spells, to prevent adding deprecated/broken spells for trainers, spell book, etc
bool SpellMgr::IsSpellValid(SpellInfo const* spellInfo, Player* player, bool msg)
{
    // not exist
    if (!spellInfo)
        return false;

    bool need_check_reagents = false;

    // check effects
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        switch (spellInfo->Effects[i].Effect)
        {
        case 0:
            continue;

            // craft spell for crafting non-existed item (break client recipes list show)
        case SPELL_EFFECT_CREATE_ITEM:
        case SPELL_EFFECT_CREATE_ITEM_2:
            {
                if (spellInfo->Effects[i].ItemType == 0)
                {
                    // skip auto-loot crafting spells, its not need explicit item info (but have special fake items sometime)
                    if (!spellInfo->IsLootCrafting())
                    {
                        if (msg)
                        {
                            if (player)
                                ChatHandler(player).PSendSysMessage("Craft spell %u not have create item entry.", spellInfo->Id);
                            else
                                sLog->outError(LOG_FILTER_SQL, "Craft spell %u not have create item entry.", spellInfo->Id);
                        }
                        return false;
                    }

                }
                // also possible IsLootCrafting case but fake item must exist anyway
                else if (!sObjectMgr->GetItemTemplate(spellInfo->Effects[i].ItemType))
                {
                    if (msg)
                    {
                        if (player)
                            ChatHandler(player).PSendSysMessage("Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Effects[i].ItemType);
                        else
                            sLog->outError(LOG_FILTER_SQL, "Craft spell %u create not-exist in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Effects[i].ItemType);
                    }
                    return false;
                }

                need_check_reagents = true;
                break;
            }
        case SPELL_EFFECT_LEARN_SPELL:
            {
                SpellInfo const* spellInfo2 = sSpellMgr->GetSpellInfo(spellInfo->Effects[i].TriggerSpell);
                if (!IsSpellValid(spellInfo2, player, msg))
                {
                    if (msg)
                    {
                        if (player)
                            ChatHandler(player).PSendSysMessage("Spell %u learn to broken spell %u, and then...", spellInfo->Id, spellInfo->Effects[i].TriggerSpell);
                        else
                            sLog->outError(LOG_FILTER_SQL, "Spell %u learn to invalid spell %u, and then...", spellInfo->Id, spellInfo->Effects[i].TriggerSpell);
                    }
                    return false;
                }
                break;
            }
        }
    }

    if (need_check_reagents)
    {
        for (uint8 j = 0; j < MAX_SPELL_REAGENTS; ++j)
        {
            if (spellInfo->Reagent[j] > 0 && !sObjectMgr->GetItemTemplate(spellInfo->Reagent[j]))
            {
                if (msg)
                {
                    if (player)
                        ChatHandler(player).PSendSysMessage("Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagent[j]);
                    else
                        sLog->outError(LOG_FILTER_SQL, "Craft spell %u have not-exist reagent in DB item (Entry: %u) and then...", spellInfo->Id, spellInfo->Reagent[j]);
                }
                return false;
            }
        }
    }

    return true;
}

bool SpellMgr::IsSpellForbidden(uint32 spellid)
{
    std::list<uint32>::iterator Itr;

    for (Itr = mForbiddenSpells.begin(); Itr != mForbiddenSpells.end(); Itr++)
        if ((*Itr) == spellid)
            return true;

    return false;
}

uint32 SpellMgr::GetSpellDifficultyId(uint32 spellId) const
{
    SpellDifficultySearcherMap::const_iterator i = mSpellDifficultySearcherMap.find(spellId);
    return i == mSpellDifficultySearcherMap.end() ? 0 : (*i).second;
}

void SpellMgr::SetSpellDifficultyId(uint32 spellId, uint32 id)
{
    mSpellDifficultySearcherMap[spellId] = id;
}

uint32 SpellMgr::GetSpellIdForDifficulty(uint32 spellId, Unit const* caster) const
{
    // Dbc supprimée au passage a MoP
    return spellId;
    /*if (!GetSpellInfo(spellId))
    return spellId;

    if (!caster || !caster->GetMap() || !caster->GetMap()->IsDungeon())
    return spellId;

    uint32 mode = uint32(caster->GetMap()->GetSpawnMode());
    if (mode >= MAX_DIFFICULTY)
    {
    sLog->outError(LOG_FILTER_SPELLS_AURAS, "SpellMgr::GetSpellIdForDifficulty: Incorrect Difficulty for spell %u.", spellId);
    return spellId; //return source spell
    }

    uint32 difficultyId = GetSpellDifficultyId(spellId);
    if (!difficultyId)
    return spellId; //return source spell, it has only REGULAR_DIFFICULTY

    SpellDifficultyEntry const* difficultyEntry = sSpellDifficultyStore.LookupEntry(difficultyId);
    if (!difficultyEntry)
    {
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "SpellMgr::GetSpellIdForDifficulty: SpellDifficultyEntry not found for spell %u. This should never happen.", spellId);
    return spellId; //return source spell
    }

    if (difficultyEntry->SpellID[mode] <= 0 && mode > HEROIC_DIFFICULTY)
    {
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "SpellMgr::GetSpellIdForDifficulty: spell %u mode %u spell is NULL, using mode %u", spellId, mode, mode - 2);
    mode -= 2;
    }

    if (difficultyEntry->SpellID[mode] <= 0)
    {
    sLog->outError(LOG_FILTER_SQL, "SpellMgr::GetSpellIdForDifficulty: spell %u mode %u spell is 0. Check spelldifficulty_dbc!", spellId, mode);
    return spellId;
    }

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "SpellMgr::GetSpellIdForDifficulty: spellid for spell %u in mode %u is %d", spellId, mode, difficultyEntry->SpellID[mode]);
    return uint32(difficultyEntry->SpellID[mode]);*/
}

SpellInfo const* SpellMgr::GetSpellForDifficultyFromSpell(SpellInfo const* spell, Unit const* caster) const
{
    if (!spell)
        return NULL;

    if (!caster || !caster->GetMap() || !caster->GetMap()->IsDungeon())
        return spell;

    uint32 mode = uint32(caster->GetMap()->GetSpawnMode());
    return GetSpellInfo(spell->Id, (Difficulty)mode);
}

SpellChainNode const* SpellMgr::GetSpellChainNode(uint32 spell_id) const
{
    SpellChainMap::const_iterator itr = mSpellChains.find(spell_id);
    if (itr == mSpellChains.end())
        return NULL;

    return &itr->second;
}

uint32 SpellMgr::GetFirstSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->first->Id;

    return spell_id;
}

uint32 SpellMgr::GetLastSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->last->Id;

    return spell_id;
}

uint32 SpellMgr::GetNextSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        if (node->next)
            return node->next->Id;

    return 0;
}

uint32 SpellMgr::GetPrevSpellInChain(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        if (node->prev)
            return node->prev->Id;

    return 0;
}

uint8 SpellMgr::GetSpellRank(uint32 spell_id) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
        return node->rank;

    return 0;
}

uint32 SpellMgr::GetSpellWithRank(uint32 spell_id, uint32 rank, bool strict) const
{
    if (SpellChainNode const* node = GetSpellChainNode(spell_id))
    {
        if (rank != node->rank)
            return GetSpellWithRank(node->rank < rank ? node->next->Id : node->prev->Id, rank, strict);
    }
    else if (strict && rank > 1)
        return 0;
    return spell_id;
}

SpellRequiredMapBounds SpellMgr::GetSpellsRequiredForSpellBounds(uint32 spell_id) const
{
    return SpellRequiredMapBounds(mSpellReq.lower_bound(spell_id), mSpellReq.upper_bound(spell_id));
}

SpellsRequiringSpellMapBounds SpellMgr::GetSpellsRequiringSpellBounds(uint32 spell_id) const
{
    return SpellsRequiringSpellMapBounds(mSpellsReqSpell.lower_bound(spell_id), mSpellsReqSpell.upper_bound(spell_id));
}

bool SpellMgr::IsSpellRequiringSpell(uint32 spellid, uint32 req_spellid) const
{
    SpellsRequiringSpellMapBounds spellsRequiringSpell = GetSpellsRequiringSpellBounds(req_spellid);
    for (SpellsRequiringSpellMap::const_iterator itr = spellsRequiringSpell.first; itr != spellsRequiringSpell.second; ++itr)
    {
        if (itr->second == spellid)
            return true;
    }
    return false;
}

const SpellsRequiringSpellMap SpellMgr::GetSpellsRequiringSpell()
{
    return this->mSpellsReqSpell;
}

uint32 SpellMgr::GetSpellRequired(uint32 spell_id) const
{
    SpellRequiredMap::const_iterator itr = mSpellReq.find(spell_id);

    if (itr == mSpellReq.end())
        return 0;

    return itr->second;
}

SpellLearnSkillNode const* SpellMgr::GetSpellLearnSkill(uint32 spell_id) const
{
    SpellLearnSkillMap::const_iterator itr = mSpellLearnSkills.find(spell_id);
    if (itr != mSpellLearnSkills.end())
        return &itr->second;
    else
        return NULL;
}

SpellLearnSpellMapBounds SpellMgr::GetSpellLearnSpellMapBounds(uint32 spell_id) const
{
    return SpellLearnSpellMapBounds(mSpellLearnSpells.lower_bound(spell_id), mSpellLearnSpells.upper_bound(spell_id));
}

bool SpellMgr::IsSpellLearnSpell(uint32 spell_id) const
{
    return mSpellLearnSpells.find(spell_id) != mSpellLearnSpells.end();
}

bool SpellMgr::IsSpellLearnToSpell(uint32 spell_id1, uint32 spell_id2) const
{
    SpellLearnSpellMapBounds bounds = GetSpellLearnSpellMapBounds(spell_id1);
    for (SpellLearnSpellMap::const_iterator i = bounds.first; i != bounds.second; ++i)
        if (i->second.spell == spell_id2)
            return true;
    return false;
}

SpellTargetPosition const* SpellMgr::GetSpellTargetPosition(uint32 spell_id, SpellEffIndex effIndex) const
{
    SpellTargetPositionMap::const_iterator itr = mSpellTargetPositions.find(std::make_pair(spell_id, effIndex));
    if (itr != mSpellTargetPositions.end())
        return &itr->second;
    return NULL;
}

SpellSpellGroupMapBounds SpellMgr::GetSpellSpellGroupMapBounds(uint32 spell_id) const
{
    spell_id = GetFirstSpellInChain(spell_id);
    return SpellSpellGroupMapBounds(mSpellSpellGroup.lower_bound(spell_id), mSpellSpellGroup.upper_bound(spell_id));
}

uint32 SpellMgr::IsSpellMemberOfSpellGroup(uint32 spellid, SpellGroup groupid) const
{
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellid);
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        if (itr->second == groupid)
            return true;
    }
    return false;
}

SpellGroupSpellMapBounds SpellMgr::GetSpellGroupSpellMapBounds(SpellGroup group_id) const
{
    return SpellGroupSpellMapBounds(mSpellGroupSpell.lower_bound(group_id), mSpellGroupSpell.upper_bound(group_id));
}

void SpellMgr::GetSetOfSpellsInSpellGroup(SpellGroup group_id, std::set<uint32>& foundSpells) const
{
    std::set<SpellGroup> usedGroups;
    GetSetOfSpellsInSpellGroup(group_id, foundSpells, usedGroups);
}

void SpellMgr::GetSetOfSpellsInSpellGroup(SpellGroup group_id, std::set<uint32>& foundSpells, std::set<SpellGroup>& usedGroups) const
{
    if (usedGroups.find(group_id) != usedGroups.end())
        return;
    usedGroups.insert(group_id);

    SpellGroupSpellMapBounds groupSpell = GetSpellGroupSpellMapBounds(group_id);
    for (SpellGroupSpellMap::const_iterator itr = groupSpell.first; itr != groupSpell.second; ++itr)
    {
        if (itr->second < 0)
        {
            SpellGroup currGroup = (SpellGroup)abs(itr->second);
            GetSetOfSpellsInSpellGroup(currGroup, foundSpells, usedGroups);
        }
        else
        {
            foundSpells.insert(itr->second);
        }
    }
}

bool SpellMgr::AddSameEffectStackRuleSpellGroups(SpellInfo const* spellInfo, int32 amount, std::map<SpellGroup, int32>& groups) const
{
    uint32 spellId = spellInfo->GetFirstRankSpell()->Id;
    SpellSpellGroupMapBounds spellGroup = GetSpellSpellGroupMapBounds(spellId);
    // Find group with SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT if it belongs to one
    for (SpellSpellGroupMap::const_iterator itr = spellGroup.first; itr != spellGroup.second; ++itr)
    {
        SpellGroup group = itr->second;
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(group);
        if (found != mSpellGroupStack.end())
        {
            if (found->second == SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT)
            {
                // Put the highest amount in the map
                if (groups.find(group) == groups.end())
                    groups[group] = amount;
                else
                {
                    int32 curr_amount = groups[group];
                    // Take absolute value because this also counts for the highest negative aura
                    if (abs(curr_amount) < abs(amount))
                        groups[group] = amount;
                }
                // return because a spell should be in only one SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT group
                return true;
            }
        }
    }
    // Not in a SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT group, so return false
    return false;
}

SpellGroupStackRule SpellMgr::CheckSpellGroupStackRules(SpellInfo const* spellInfo1, SpellInfo const* spellInfo2) const
{
    uint32 spellid_1 = spellInfo1->GetFirstRankSpell()->Id;
    uint32 spellid_2 = spellInfo2->GetFirstRankSpell()->Id;
    if (spellid_1 == spellid_2)
        return SPELL_GROUP_STACK_RULE_DEFAULT;
    // find SpellGroups which are common for both spells
    SpellSpellGroupMapBounds spellGroup1 = GetSpellSpellGroupMapBounds(spellid_1);
    std::set<SpellGroup> groups;
    for (SpellSpellGroupMap::const_iterator itr = spellGroup1.first; itr != spellGroup1.second; ++itr)
    {
        if (IsSpellMemberOfSpellGroup(spellid_2, itr->second))
        {
            bool add = true;
            SpellGroupSpellMapBounds groupSpell = GetSpellGroupSpellMapBounds(itr->second);
            for (SpellGroupSpellMap::const_iterator itr2 = groupSpell.first; itr2 != groupSpell.second; ++itr2)
            {
                if (itr2->second < 0)
                {
                    SpellGroup currGroup = (SpellGroup)abs(itr2->second);
                    if (IsSpellMemberOfSpellGroup(spellid_1, currGroup) && IsSpellMemberOfSpellGroup(spellid_2, currGroup))
                    {
                        add = false;
                        break;
                    }
                }
            }
            if (add)
                groups.insert(itr->second);
        }
    }

    SpellGroupStackRule rule = SPELL_GROUP_STACK_RULE_DEFAULT;

    for (std::set<SpellGroup>::iterator itr = groups.begin(); itr!= groups.end(); ++itr)
    {
        SpellGroupStackMap::const_iterator found = mSpellGroupStack.find(*itr);
        if (found != mSpellGroupStack.end())
            rule = found->second;
        if (rule)
            break;
    }
    return rule;
}

SpellProcEventEntry const* SpellMgr::GetSpellProcEvent(uint32 spellId) const
{
    SpellProcEventMap::const_iterator itr = mSpellProcEventMap.find(spellId);
    if (itr != mSpellProcEventMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::IsSpellProcEventCanTriggeredBy(SpellProcEventEntry const* spellProcEvent, uint32 EventProcFlag, SpellInfo const* procSpell, uint32 procFlags, uint32 procExtra, bool active)
{
    // No extra req need
    uint32 procEvent_procEx = PROC_EX_NONE;

    // check prockFlags for condition
    if ((procFlags & EventProcFlag) == 0)
        return false;

    bool hasFamilyMask = false;

    /* Check Periodic Auras

    *Dots can trigger if spell has no PROC_FLAG_SUCCESSFUL_NEGATIVE_MAGIC_SPELL
    nor PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL

    *Only Hots can trigger if spell has PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL

    *Only dots can trigger if spell has both positivity flags or PROC_FLAG_SUCCESSFUL_NEGATIVE_MAGIC_SPELL

    *Aura has to have PROC_FLAG_TAKEN_POSITIVE_MAGIC_SPELL or spellfamily specified to trigger from Hot

    */

    if (procFlags & PROC_FLAG_DONE_PERIODIC)
    {
        if (EventProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG)
        {
            if (!(procExtra & PROC_EX_INTERNAL_DOT))
                return false;
        }
        else if (procExtra & PROC_EX_INTERNAL_HOT)
            procExtra |= PROC_EX_INTERNAL_REQ_FAMILY;
        else if (EventProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS)
            return false;
    }

    if (procFlags & PROC_FLAG_TAKEN_PERIODIC)
    {
        if (EventProcFlag & PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS)
        {
            if (!(procExtra & PROC_EX_INTERNAL_DOT))
                return false;
        }
        else if (procExtra & PROC_EX_INTERNAL_HOT)
            procExtra |= PROC_EX_INTERNAL_REQ_FAMILY;
        else if (EventProcFlag & PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS)
            return false;
    }
    // Trap casts are active by default
    if (procFlags & PROC_FLAG_DONE_TRAP_ACTIVATION)
        active = true;

    // Always trigger for this
    if (procFlags & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    if (spellProcEvent)     // Exist event data
    {
        // Store extra req
        procEvent_procEx = spellProcEvent->procEx;

        // For melee triggers
        if (procSpell == NULL)
        {
            // Check (if set) for school (melee attack have Normal school)
            if (spellProcEvent->schoolMask && (spellProcEvent->schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0)
                return false;
        }
        else // For spells need check school/spell family/family mask
        {
            // Check (if set) for school
            if (spellProcEvent->schoolMask && (spellProcEvent->schoolMask & procSpell->SchoolMask) == 0)
                return false;

            // Check (if set) for spellFamilyName
            if (spellProcEvent->spellFamilyName && (spellProcEvent->spellFamilyName != procSpell->SpellFamilyName))
                return false;

            // spellFamilyName is Ok need check for spellFamilyMask if present
            if (spellProcEvent->spellFamilyMask)
            {
                if (!(spellProcEvent->spellFamilyMask & procSpell->SpellFamilyFlags))
                    return false;
                hasFamilyMask = true;
                // Some spells are not considered as active even with have spellfamilyflags
                if (!(procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL))
                    active = true;
            }
        }
    }

    if (procExtra & (PROC_EX_INTERNAL_REQ_FAMILY))
    {
        if (!hasFamilyMask)
            return false;
    }

    // Check for extra req (if none) and hit/crit
    if (procEvent_procEx == PROC_EX_NONE)
    {
        // No extra req, so can trigger only for hit/crit - spell has to be active
        if ((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) && active)
            return true;
    }
    else // Passive spells hits here only if resist/reflect/immune/evade
    {
        if (procExtra & AURA_SPELL_PROC_EX_MASK)
        {
            // if spell marked as procing only from not active spells
            if (active && procEvent_procEx & PROC_EX_NOT_ACTIVE_SPELL)
                return false;
            // if spell marked as procing only from active spells
            if (!active && procEvent_procEx & PROC_EX_ONLY_ACTIVE_SPELL)
                return false;
            // Exist req for PROC_EX_EX_TRIGGER_ALWAYS
            if (procEvent_procEx & PROC_EX_EX_TRIGGER_ALWAYS)
                return true;
            // PROC_EX_NOT_ACTIVE_SPELL and PROC_EX_ONLY_ACTIVE_SPELL flags handle: if passed checks before
            if ((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) && ((procEvent_procEx & (AURA_SPELL_PROC_EX_MASK)) == 0))
                return true;
        }
        // Check Extra Requirement like (hit/crit/miss/resist/parry/dodge/block/immune/reflect/absorb and other)
        if (procEvent_procEx & procExtra)
            return true;
    }
    return false;
}

SpellProcEntry const* SpellMgr::GetSpellProcEntry(uint32 spellId) const
{
    SpellProcMap::const_iterator itr = mSpellProcMap.find(spellId);
    if (itr != mSpellProcMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::CanSpellTriggerProcOnEvent(SpellProcEntry const& procEntry, ProcEventInfo& eventInfo)
{
    // proc type doesn't match
    if (!(eventInfo.GetTypeMask() & procEntry.typeMask))
        return false;

    // check XP or honor target requirement
    if (procEntry.attributesMask & PROC_ATTR_REQ_EXP_OR_HONOR)
        if (Player* actor = eventInfo.GetActor()->ToPlayer())
            if (eventInfo.GetActionTarget() && !actor->isHonorOrXPTarget(eventInfo.GetActionTarget()))
                return false;

    // always trigger for these types
    if (eventInfo.GetTypeMask() & (PROC_FLAG_KILLED | PROC_FLAG_KILL | PROC_FLAG_DEATH))
        return true;

    // check school mask (if set) for other trigger types
    if (procEntry.schoolMask && !(eventInfo.GetSchoolMask() & procEntry.schoolMask))
        return false;

    // check spell family name/flags (if set) for spells
    if (eventInfo.GetTypeMask() & (PERIODIC_PROC_FLAG_MASK | SPELL_PROC_FLAG_MASK | PROC_FLAG_DONE_TRAP_ACTIVATION))
    {
        if (procEntry.spellFamilyName && (procEntry.spellFamilyName != eventInfo.GetSpellInfo()->SpellFamilyName))
            return false;

        if (procEntry.spellFamilyMask && !(procEntry.spellFamilyMask & eventInfo.GetSpellInfo()->SpellFamilyFlags))
            return false;
    }

    // check spell type mask (if set)
    if (eventInfo.GetTypeMask() & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK))
    {
        if (procEntry.spellTypeMask && !(eventInfo.GetSpellTypeMask() & procEntry.spellTypeMask))
            return false;
    }

    // check spell phase mask
    if (eventInfo.GetTypeMask() & REQ_SPELL_PHASE_PROC_FLAG_MASK)
    {
        if (!(eventInfo.GetSpellPhaseMask() & procEntry.spellPhaseMask))
            return false;
    }

    // check hit mask (on taken hit or on done hit, but not on spell cast phase)
    if ((eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK) || ((eventInfo.GetTypeMask() & DONE_HIT_PROC_FLAG_MASK) && !(eventInfo.GetSpellPhaseMask() & PROC_SPELL_PHASE_CAST)))
    {
        uint32 hitMask = procEntry.hitMask;
        // get default values if hit mask not set
        if (!hitMask)
        {
            // for taken procs allow normal + critical hits by default
            if (eventInfo.GetTypeMask() & TAKEN_HIT_PROC_FLAG_MASK)
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL;
            // for done procs allow normal + critical + absorbs by default
            else
                hitMask |= PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB;
        }
        if (!(eventInfo.GetHitMask() & hitMask))
            return false;
    }

    return true;
}

SpellBonusEntry const* SpellMgr::GetSpellBonusData(uint32 spellId) const
{
    // Lookup data
    SpellBonusMap::const_iterator itr = mSpellBonusMap.find(spellId);
    if (itr != mSpellBonusMap.end())
        return &itr->second;
    // Not found, try lookup for 1 spell rank if exist
    if (uint32 rank_1 = GetFirstSpellInChain(spellId))
    {
        SpellBonusMap::const_iterator itr2 = mSpellBonusMap.find(rank_1);
        if (itr2 != mSpellBonusMap.end())
            return &itr2->second;
    }
    return NULL;
}

SpellThreatEntry const* SpellMgr::GetSpellThreatEntry(uint32 spellID) const
{
    SpellThreatMap::const_iterator itr = mSpellThreatMap.find(spellID);
    if (itr != mSpellThreatMap.end())
        return &itr->second;
    else
    {
        uint32 firstSpell = GetFirstSpellInChain(spellID);
        itr = mSpellThreatMap.find(firstSpell);
        if (itr != mSpellThreatMap.end())
            return &itr->second;
    }
    return NULL;
}

SkillLineAbilityMapBounds SpellMgr::GetSkillLineAbilityMapBounds(uint32 spell_id) const
{
    return SkillLineAbilityMapBounds(mSkillLineAbilityMap.lower_bound(spell_id), mSkillLineAbilityMap.upper_bound(spell_id));
}

PetAura const* SpellMgr::GetPetAura(uint32 spell_id, uint8 eff)
{
    SpellPetAuraMap::const_iterator itr = mSpellPetAuraMap.find((spell_id<<8) + eff);
    if (itr != mSpellPetAuraMap.end())
        return &itr->second;
    else
        return NULL;
}

SpellEnchantProcEntry const* SpellMgr::GetSpellEnchantProcEvent(uint32 enchId) const
{
    SpellEnchantProcEventMap::const_iterator itr = mSpellEnchantProcEventMap.find(enchId);
    if (itr != mSpellEnchantProcEventMap.end())
        return &itr->second;
    return NULL;
}

bool SpellMgr::IsArenaAllowedEnchancment(uint32 ench_id) const
{
    return mEnchantCustomAttr[ench_id];
}

const std::vector<int32>* SpellMgr::GetSpellLinked(int32 spell_id) const
{
    SpellLinkedMap::const_iterator itr = mSpellLinkedMap.find(spell_id);
    return itr != mSpellLinkedMap.end() ? &(itr->second) : NULL;
}

PetLevelupSpellSet const* SpellMgr::GetPetLevelupSpellList(uint32 petFamily) const
{
    PetLevelupSpellMap::const_iterator itr = mPetLevelupSpellMap.find(petFamily);
    if (itr != mPetLevelupSpellMap.end())
        return &itr->second;
    else
        return NULL;
}

PetDefaultSpellsEntry const* SpellMgr::GetPetDefaultSpellsEntry(int32 id) const
{
    PetDefaultSpellsMap::const_iterator itr = mPetDefaultSpellsMap.find(id);
    if (itr != mPetDefaultSpellsMap.end())
        return &itr->second;
    return NULL;
}

SpellAreaMapBounds SpellMgr::GetSpellAreaMapBounds(uint32 spell_id) const
{
    return mSpellAreaMap.equal_range(spell_id);
}

SpellAreaForQuestMapBounds SpellMgr::GetSpellAreaForQuestMapBounds(uint32 quest_id) const
{
    return mSpellAreaForQuestMap.equal_range(quest_id);
}

SpellAreaForQuestMapBounds SpellMgr::GetSpellAreaForQuestEndMapBounds(uint32 quest_id) const
{
    return mSpellAreaForQuestEndMap.equal_range(quest_id);
}

SpellAreaForAuraMapBounds SpellMgr::GetSpellAreaForAuraMapBounds(uint32 spell_id) const
{
    return mSpellAreaForAuraMap.equal_range(spell_id);
}

SpellAreaForAreaMapBounds SpellMgr::GetSpellAreaForAreaMapBounds(uint32 area_id) const
{
    return mSpellAreaForAreaMap.equal_range(area_id);
}

bool SpellArea::IsFitToRequirements(Player const* player, uint32 newZone, uint32 newArea) const
{
    if (gender != GENDER_NONE)                   // not in expected gender
        if (!player || gender != player->getGender())
            return false;

    if (raceMask)                                // not in expected race
        if (!player || !(raceMask & player->getRaceMask()))
            return false;

    if (areaId)                                  // not in expected zone
        if (newZone != areaId && newArea != areaId)
            return false;

    if (questStart)                              // not in expected required quest state
        if (!player || ((questStartStatus & (1 << player->GetQuestStatus(questStart))) == 0))
            return false;

    if (questEnd)                                // not in expected forbidden quest state
        if (!player || (questEndStatus & (1 << player->GetQuestStatus(questEnd))))
            return false;

    if (auraSpell)                               // not have expected aura
        if (!player || (auraSpell > 0 && !player->HasAura(auraSpell)) || (auraSpell < 0 && player->HasAura(-auraSpell)))
            return false;

    // Extra conditions -- leaving the possibility add extra conditions...
    switch (spellId)
    {
        case 58600: // No fly Zone - Dalaran
        {
            if (!player)
                return false;

            AreaTableEntry const* pArea = GetAreaEntryByAreaID(player->GetAreaId());
            if (!(pArea && pArea->flags & AREA_FLAG_NO_FLY_ZONE))
                return false;
            if (!player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !player->HasAuraType(SPELL_AURA_FLY))
                return false;
            break;
        }
        case 58730: // No fly Zone - Wintergrasp
        case 91604: // No fly Zone - Wintergrasp
        {
            if (!player)
                return false;

            Battlefield* Bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());
            if (!Bf || Bf->CanFlyIn() || (!player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) && !player->HasAuraType(SPELL_AURA_FLY)))
                return false;
            break;
        }
        case 68719: // Oil Refinery - Isle of Conquest.
        case 68720: // Quarry - Isle of Conquest.
        {
            if (!player || player->GetBattlegroundTypeId() != BATTLEGROUND_IC || !player->GetBattleground())
                return false;

            uint8 nodeType = spellId == 68719 ? NODE_TYPE_REFINERY : NODE_TYPE_QUARRY;
            uint8 nodeState = player->GetTeamId() == TEAM_ALLIANCE ? NODE_STATE_CONTROLLED_A : NODE_STATE_CONTROLLED_H;

            BattlegroundIC* pIC = static_cast<BattlegroundIC*>(player->GetBattleground());
            if (pIC->GetNodeState(nodeType) == nodeState)
                return true;

            return false;
        }
        case 56618: // Horde Controls Factory Phase Shift
        case 56617: // Alliance Controls Factory Phase Shift
        {
            if (!player)
                return false;

            Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId());

            if (!bf || bf->GetTypeId() != BATTLEFIELD_WG)
                return false;

            // team that controls the workshop in the specified area
            uint32 team = bf->GetData(newArea);

            if (team == TEAM_HORDE)
                return spellId == 56618;
            else if (team == TEAM_ALLIANCE)
                return spellId == 56617;
            break;
        }
        case 57940: // Essence of Wintergrasp - Northrend
        case 58045: // Essence of Wintergrasp - Wintergrasp
        {
            if (!player)
                return false;

            if (Battlefield* battlefieldWG = sBattlefieldMgr->GetBattlefieldByBattleId(BATTLEFIELD_BATTLEID_WG))
                return battlefieldWG->IsEnabled() && (player->GetTeamId() == battlefieldWG->GetDefenderTeam()) && !battlefieldWG->IsWarTime();
            break;
        }
        case 74411: // Battleground - Dampening
        {
            if (!player)
                return false;

            if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(player->GetZoneId()))
                return bf->IsWarTime();
            break;
        }
        case 73822: // Hellscream's Warsong
        case 73828: // Strength of Wrynn
        {
            if (!player)
                return false;

            InstanceScript* instanceScript = ((Player*)player)->GetInstanceScript();
            if (!instanceScript)
                return false;

            if (instanceScript->GetData(41) == 3) // 41 - DATA_BUFF_REMOVED
                return false;
            break;
        }
    }

    return true;
}

void SpellMgr::LoadSpellRanks()
{
    uint32 oldMSTime = getMSTime();

    // cleanup core data before reload - remove reference to ChainNode from SpellInfo
    for (SpellChainMap::iterator itr = mSpellChains.begin(); itr != mSpellChains.end(); ++itr)
    {
        for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
        {
            if (mSpellInfoMap[difficulty][itr->first])
                mSpellInfoMap[difficulty][itr->first]->ChainEntry = NULL;
        }
    }
    mSpellChains.clear();
    //                                                     0             1      2
    QueryResult result = WorldDatabase.Query("SELECT first_spell_id, spell_id, rank from spell_ranks ORDER BY first_spell_id, rank");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell rank records. DB table `spell_ranks` is empty.");

        return;
    }

    uint32 count = 0;
    bool finished = false;

    do
    {
        // spellid, rank
        std::list < std::pair < int32, int32 > > rankChain;
        int32 currentSpell = -1;
        int32 lastSpell = -1;

        // fill one chain
        while (currentSpell == lastSpell && !finished)
        {
            Field* fields = result->Fetch();

            currentSpell = fields[0].GetUInt32();
            if (lastSpell == -1)
                lastSpell = currentSpell;
            uint32 spell_id = fields[1].GetUInt32();
            uint32 rank = fields[2].GetUInt8();

            // don't drop the row if we're moving to the next rank
            if (currentSpell == lastSpell)
            {
                rankChain.push_back(std::make_pair(spell_id, rank));
                if (!result->NextRow())
                    finished = true;
            }
            else
                break;
        }
        // check if chain is made with valid first spell
        SpellInfo const* first = GetSpellInfo(lastSpell);
        if (!first)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell rank identifier(first_spell_id) %u listed in `spell_ranks` does not exist!", lastSpell);
            continue;
        }
        // check if chain is long enough
        if (rankChain.size() < 2)
        {
            sLog->outError(LOG_FILTER_SQL, "There is only 1 spell rank for identifier(first_spell_id) %u in `spell_ranks`, entry is not needed!", lastSpell);
            continue;
        }
        int32 curRank = 0;
        bool valid = true;
        // check spells in chain
        for (std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin(); itr!= rankChain.end(); ++itr)
        {
            SpellInfo const* spell = GetSpellInfo(itr->first);
            if (!spell)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u (rank %u) listed in `spell_ranks` for chain %u does not exist!", itr->first, itr->second, lastSpell);
                valid = false;
                break;
            }
            ++curRank;
            if (itr->second != curRank)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u (rank %u) listed in `spell_ranks` for chain %u does not have proper rank value(should be %u)!", itr->first, itr->second, lastSpell, curRank);
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;
        int32 prevRank = 0;
        // insert the chain
        std::list<std::pair<int32, int32> >::iterator itr = rankChain.begin();
        do
        {
            ++count;
            int32 addedSpell = itr->first;
            mSpellChains[addedSpell].first = GetSpellInfo(lastSpell);
            mSpellChains[addedSpell].last = GetSpellInfo(rankChain.back().first);
            mSpellChains[addedSpell].rank = itr->second;
            mSpellChains[addedSpell].prev = GetSpellInfo(prevRank);
            for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
                if (mSpellInfoMap[difficulty][addedSpell])
                    mSpellInfoMap[difficulty][addedSpell]->ChainEntry = &mSpellChains[addedSpell];
            prevRank = addedSpell;
            ++itr;
            if (itr == rankChain.end())
            {
                mSpellChains[addedSpell].next = NULL;
                break;
            }
            else
                mSpellChains[addedSpell].next = GetSpellInfo(itr->first);
        }
        while (true);
    }
    while
        (!finished);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell rank records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellRequired()
{
    uint32 oldMSTime = getMSTime();

    mSpellsReqSpell.clear();                                   // need for reload case
    mSpellReq.clear();                                         // need for reload case

    //                                                   0        1
    QueryResult result = WorldDatabase.Query("SELECT spell_id, req_spell from spell_required");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell required records. DB table `spell_required` is empty.");

        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();
        uint32 spell_req = fields[1].GetUInt32();

        // check if chain is made with valid first spell
        SpellInfo const* spell = GetSpellInfo(spell_id);
        if (!spell)
        {
            sLog->outError(LOG_FILTER_SQL, "spell_id %u in `spell_required` table is not found in dbcs, skipped", spell_id);
            continue;
        }

        SpellInfo const* req_spell = GetSpellInfo(spell_req);
        if (!req_spell)
        {
            sLog->outError(LOG_FILTER_SQL, "req_spell %u in `spell_required` table is not found in dbcs, skipped", spell_req);
            continue;
        }

        if (GetFirstSpellInChain(spell_id) == GetFirstSpellInChain(spell_req))
        {
            sLog->outError(LOG_FILTER_SQL, "req_spell %u and spell_id %u in `spell_required` table are ranks of the same spell, entry not needed, skipped", spell_req, spell_id);
            continue;
        }

        if (IsSpellRequiringSpell(spell_id, spell_req))
        {
            sLog->outError(LOG_FILTER_SQL, "duplicated entry of req_spell %u and spell_id %u in `spell_required`, skipped", spell_req, spell_id);
            continue;
        }

        mSpellReq.insert (std::pair<uint32, uint32>(spell_id, spell_req));
        mSpellsReqSpell.insert (std::pair<uint32, uint32>(spell_req, spell_id));
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell required records in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellLearnSkills()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSkills.clear();                              // need for reload case

    // search auto-learned skills and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);

        if (!entry)
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_SKILL)
            {
                SpellLearnSkillNode dbc_node;
                dbc_node.skill = entry->Effects[i].MiscValue;
                dbc_node.step  = entry->Effects[i].CalcValue();
                if (dbc_node.skill != SKILL_RIDING)
                    dbc_node.value = 1;
                else
                    dbc_node.value = dbc_node.step * 75;
                dbc_node.maxvalue = dbc_node.step * 75;
                mSpellLearnSkills[spell] = dbc_node;
                ++dbc_count;
                break;
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Spell Learn Skills from DBC in %u ms", dbc_count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellLearnSpells()
{
    uint32 oldMSTime = getMSTime();

    mSpellLearnSpells.clear();                              // need for reload case

    //                                                  0      1        2
    QueryResult result = WorldDatabase.Query("SELECT entry, SpellID, Active FROM spell_learn_spell");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell learn spells. DB table `spell_learn_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell_id = fields[0].GetUInt32();

        SpellLearnSpellNode node;
        node.spell       = fields[1].GetUInt32();
        node.active      = fields[2].GetBool();
        node.autoLearned = false;

        if (!GetSpellInfo(spell_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_learn_spell` does not exist", spell_id);
            continue;
        }

        if (!GetSpellInfo(node.spell))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_learn_spell` learning not existed spell %u", spell_id, node.spell);
            continue;
        }

        /*if (GetTalentSpellCost(node.spell))
        {
        sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_learn_spell` attempt learning talent spell %u, skipped", spell_id, node.spell);
        continue;
        }*/

        mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell_id, node));

        ++count;
    }
    while (result->NextRow());

    // search auto-learned spells and add its to map also for use in unlearn spells/talents
    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);

        if (!entry)
            continue;

        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL)
            {
                SpellLearnSpellNode dbc_node;
                dbc_node.spell = entry->Effects[i].TriggerSpell;
                dbc_node.active = true;                     // all dbc based learned spells is active (show in spell book or hide by client itself)

                // ignore learning not existed spells (broken/outdated/or generic learnig spell 483
                if (!GetSpellInfo(dbc_node.spell))
                    continue;

                // talent or passive spells or skill-step spells auto-casted and not need dependent learning,
                // pet teaching spells must not be dependent learning (casted)
                // other required explicit dependent learning
                dbc_node.autoLearned = entry->Effects[i].TargetA.GetTarget() == TARGET_UNIT_PET || /*GetTalentSpellCost(spell) > 0 ||*/ entry->IsPassive() || entry->HasEffect(SPELL_EFFECT_SKILL_STEP);

                SpellLearnSpellMapBounds db_node_bounds = GetSpellLearnSpellMapBounds(spell);

                bool found = false;
                for (SpellLearnSpellMap::const_iterator itr = db_node_bounds.first; itr != db_node_bounds.second; ++itr)
                {
                    if (itr->second.spell == dbc_node.spell)
                    {
                        sLog->outError(LOG_FILTER_SQL, "Spell %u auto-learn spell %u in spell.dbc then the record in `spell_learn_spell` is redundant, please fix DB.",
                            spell, dbc_node.spell);
                        found = true;
                        break;
                    }
                }

                if (!found)                                  // add new spell-spell pair if not found
                {
                    mSpellLearnSpells.insert(SpellLearnSpellMap::value_type(spell, dbc_node));
                    ++dbc_count;
                }
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell learn spells + %u found in DBC in %u ms", count, dbc_count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellTargetPositions()
{
    uint32 oldMSTime = getMSTime();

    mSpellTargetPositions.clear(); // need for reload case

    //                                                0    1         2           3                  4                     5                  6
    QueryResult result = WorldDatabase.Query("SELECT id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM spell_target_position");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell target coordinates. DB table `spell_target_position` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 Spell_ID = fields[0].GetUInt32();
        SpellEffIndex effIndex = SpellEffIndex(fields[1].GetUInt8());

        SpellTargetPosition st;

        st.target_mapId = fields[2].GetUInt16();
        st.target_X = fields[3].GetFloat();
        st.target_Y = fields[4].GetFloat();
        st.target_Z = fields[5].GetFloat();
        st.target_Orientation = fields[6].GetFloat();

        MapEntry const* mapEntry = sMapStore.LookupEntry(st.target_mapId);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (Id: %u, effIndex: %u) target map (ID: %u) does not exist in `Map.dbc`.", Spell_ID, effIndex, st.target_mapId);
            continue;
        }

        if (st.target_X == 0 && st.target_Y == 0 && st.target_Z == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (Id: %u, effIndex: %u) target coordinates not provided.", Spell_ID, effIndex);
            continue;
        }

        SpellInfo const* spellInfo = GetSpellInfo(Spell_ID);
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (ID:%u) listed in `spell_target_position` does not exist.", Spell_ID);
            continue;
        }

        if (spellInfo->Effects[effIndex].TargetA.GetTarget() == TARGET_DEST_DB || spellInfo->Effects[effIndex].TargetB.GetTarget() == TARGET_DEST_DB)
        {
            std::pair<uint32, SpellEffIndex> key = std::make_pair(Spell_ID, effIndex);
            mSpellTargetPositions[key] = st;
            ++count;
        }
        else
        {
            sLog->outError(LOG_FILTER_SQL, "Spell (Id: %u, effIndex: %u) listed in `spell_target_position` does not have target TARGET_DEST_DB (17).", Spell_ID, effIndex);
            continue;
        }

    } while (result->NextRow());

    /*
    // Check all spells
    for (uint32 i = 1; i < GetSpellInfoStoreSize; ++i)
    {
    SpellInfo const* spellInfo = GetSpellInfo(i);
    if (!spellInfo)
    continue;

    bool found = false;
    for (int j = 0; j < MAX_SPELL_EFFECTS; ++j)
    {
    switch (spellInfo->Effects[j].TargetA)
    {
    case TARGET_DEST_DB:
    found = true;
    break;
    }
    if (found)
    break;
    switch (spellInfo->Effects[j].TargetB)
    {
    case TARGET_DEST_DB:
    found = true;
    break;
    }
    if (found)
    break;
    }
    if (found)
    {
    if (!sSpellMgr->GetSpellTargetPosition(i))
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Spell (ID: %u) does not have record in `spell_target_position`", i);
    }
    }*/

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell teleport coordinates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellGroups()
{
    uint32 oldMSTime = getMSTime();

    mSpellSpellGroup.clear();                                  // need for reload case
    mSpellGroupSpell.clear();

    //                                                0     1
    QueryResult result = WorldDatabase.Query("SELECT id, spell_id FROM spell_group");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell group definitions. DB table `spell_group` is empty.");
        return;
    }

    std::set<uint32> groups;
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        if (group_id <= SPELL_GROUP_DB_RANGE_MIN && group_id >= SPELL_GROUP_CORE_RANGE_MAX)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellGroup id %u listed in `spell_group` is in core range, but is not defined in core!", group_id);
            continue;
        }
        int32 spell_id = fields[1].GetInt32();

        groups.insert(std::set<uint32>::value_type(group_id));
        mSpellGroupSpell.insert(SpellGroupSpellMap::value_type((SpellGroup)group_id, spell_id));

    }
    while (result->NextRow());

    for (SpellGroupSpellMap::iterator itr = mSpellGroupSpell.begin(); itr!= mSpellGroupSpell.end();)
    {
        if (itr->second < 0)
        {
            if (groups.find(abs(itr->second)) == groups.end())
            {
                sLog->outError(LOG_FILTER_SQL, "SpellGroup id %u listed in `spell_group` does not exist", abs(itr->second));
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
        else
        {
            SpellInfo const* spellInfo = GetSpellInfo(itr->second);

            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_group` does not exist", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else if (spellInfo->GetRank() > 1)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_group` is not first rank of spell", itr->second);
                mSpellGroupSpell.erase(itr++);
            }
            else
                ++itr;
        }
    }

    for (std::set<uint32>::iterator groupItr = groups.begin(); groupItr != groups.end(); ++groupItr)
    {
        std::set<uint32> spells;
        GetSetOfSpellsInSpellGroup(SpellGroup(*groupItr), spells);

        for (std::set<uint32>::iterator spellItr = spells.begin(); spellItr != spells.end(); ++spellItr)
        {
            ++count;
            mSpellSpellGroup.insert(SpellSpellGroupMap::value_type(*spellItr, SpellGroup(*groupItr)));
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell group definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellGroupStackRules()
{
    uint32 oldMSTime = getMSTime();

    mSpellGroupStack.clear();                                  // need for reload case

    //                                                       0         1
    QueryResult result = WorldDatabase.Query("SELECT group_id, stack_rule FROM spell_group_stack_rules");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell group stack rules. DB table `spell_group_stack_rules` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 group_id = fields[0].GetUInt32();
        uint8 stack_rule = fields[1].GetInt8();
        if (stack_rule >= SPELL_GROUP_STACK_RULE_MAX)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellGroupStackRule %u listed in `spell_group_stack_rules` does not exist", stack_rule);
            continue;
        }

        SpellGroupSpellMapBounds spellGroup = GetSpellGroupSpellMapBounds((SpellGroup)group_id);

        if (spellGroup.first == spellGroup.second)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellGroup id %u listed in `spell_group_stack_rules` does not exist", group_id);
            continue;
        }

        mSpellGroupStack[(SpellGroup)group_id] = (SpellGroupStackRule)stack_rule;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell group stack rules in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadForbiddenSpells()
{
    uint32 oldMSTime = getMSTime();

    mForbiddenSpells.clear();

    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT spell_id FROM spell_forbidden");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell group definitions", count);
        return;
    }

    do
    {
        Field *fields = result->Fetch();
    
        mForbiddenSpells.push_back(fields[0].GetUInt32());

        ++count;
    }
    while (result->NextRow());


    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u forbidden spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::InitializeItemUpgradeDatas()
{
    uint32 oldMSTime = getMSTime();

    uint16 spTable[71][2] =
    {
        {458, 4914}, {463, 5152}, {466, 5293}, {470, 5497}, {471, 5552}, {474, 5704}, {476, 5812}, {478, 5920}, {480, 6037}, {483, 6206},
        {484, 6262}, {487, 6445}, {489, 6564}, {490, 6628}, {491, 6684}, {493, 6810}, {494, 6874}, {496, 7007}, {497, 7070}, {498, 7140},
        {500, 7272}, {501, 7337}, {502, 7410}, {503, 7478}, {504, 7548}, {505, 7619}, {506, 7690}, {507, 7759}, {508, 7836}, {509, 7907},
        {510, 7982}, {511, 8054}, {512, 8132}, {513, 8209}, {514, 8286}, {515, 8364}, {516, 8441}, {517, 8521}, {518, 8603}, {519, 8680},
        {520, 8764}, {521, 8841}, {522, 8925}, {524, 9093}, {525, 9179}, {526, 9265}, {528, 9440}, {530, 9618}, {532, 9797}, {535, 10078},
        {536, 10169}, {539, 10458}, {540, 10557}, {541, 10655}, {543, 10859}, {544, 10957}, {545, 11060}, {548, 11372}, {549, 11479}, {553, 11916},
        {557, 12370}, {559, 12602}, {561, 12841}, {563, 13079}, {566, 13452}, {567, 13578}, {570, 13961}, {572, 14225}, {574, 14492}, {576, 14766}, {580, 15321}
    };

    for (uint8 i = 0; i < 71; ++i)
        mItemUpgradeDatas.insert(std::make_pair(spTable[i][0], spTable[i][1]));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 71 item upgrade datas in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellProcEvents()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcEventMap.clear();                             // need for reload case

    //                                                0      1           2                3                 4                 5                 6                   7           8        9         10         11
    QueryResult result = WorldDatabase.Query("SELECT entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, spellFamilyMask3, procFlags, procEx, ppmRate, CustomChance, Cooldown FROM spell_proc_event");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell proc event conditions. DB table `spell_proc_event` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 customProc = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        SpellInfo const* spell = GetSpellInfo(entry);
        if (!spell)
        {
            WorldDatabase.PExecute("DELETE FROM spell_proc_event WHERE entry = %u;", entry);
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc_event` does not exist", entry);
            continue;
        }

        SpellProcEventEntry spe;

        spe.schoolMask          = fields[1].GetInt8();
        spe.spellFamilyName     = fields[2].GetUInt16();
        spe.spellFamilyMask[0]  = fields[3].GetUInt32();
        spe.spellFamilyMask[1]  = fields[4].GetUInt32();
        spe.spellFamilyMask[2]  = fields[5].GetUInt32();
        spe.spellFamilyMask[3]  = fields[6].GetUInt32();
        spe.procFlags           = fields[7].GetUInt32();
        spe.procEx              = fields[8].GetUInt32();
        spe.ppmRate             = fields[9].GetFloat();
        spe.customChance        = fields[10].GetFloat();
        spe.cooldown            = fields[11].GetUInt32();

        mSpellProcEventMap[entry] = spe;

        if (spell->ProcFlags == 0)
        {
            if (spe.procFlags == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc_event` probally not triggered spell", entry);
                continue;
            }
            customProc++;
        }
        ++count;
    }
    while (result->NextRow());

    if (customProc)
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u extra and %u custom spell proc event conditions in %u ms",  count, customProc, GetMSTimeDiffToNow(oldMSTime));
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u extra spell proc event conditions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void SpellMgr::LoadSpellProcs()
{
    uint32 oldMSTime = getMSTime();

    mSpellProcMap.clear();                             // need for reload case

    //                                                 0        1           2                3                 4                 5                 6         7              8               9        10              11             12      13        14
    QueryResult result = WorldDatabase.Query("SELECT spellId, schoolMask, spellFamilyName, spellFamilyMask0, spellFamilyMask1, spellFamilyMask2, typeMask, spellTypeMask, spellPhaseMask, hitMask, attributesMask, ratePerMinute, chance, cooldown, charges FROM spell_proc");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell proc conditions and data. DB table `spell_proc` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 spellId = fields[0].GetInt32();

        bool allRanks = false;
        if (spellId <=0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellEntry = GetSpellInfo(spellId);
        if (!spellEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc` does not exist", spellId);
            continue;
        }

        if (allRanks)
        {
            if (GetFirstSpellInChain(spellId) != uint32(spellId))
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc` is not first rank of spell.", fields[0].GetInt32());
                continue;
            }
        }

        SpellProcEntry baseProcEntry;

        baseProcEntry.schoolMask      = fields[1].GetInt8();
        baseProcEntry.spellFamilyName = fields[2].GetUInt16();
        baseProcEntry.spellFamilyMask[0] = fields[3].GetUInt32();
        baseProcEntry.spellFamilyMask[1] = fields[4].GetUInt32();
        baseProcEntry.spellFamilyMask[2] = fields[5].GetUInt32();
        baseProcEntry.typeMask        = fields[6].GetUInt32();
        baseProcEntry.spellTypeMask   = fields[7].GetUInt32();
        baseProcEntry.spellPhaseMask  = fields[8].GetUInt32();
        baseProcEntry.hitMask         = fields[9].GetUInt32();
        baseProcEntry.attributesMask  = fields[10].GetUInt32();
        baseProcEntry.ratePerMinute   = fields[11].GetFloat();
        baseProcEntry.chance          = fields[12].GetFloat();
        float cooldown                = fields[13].GetFloat();
        baseProcEntry.cooldown        = uint32(cooldown);
        baseProcEntry.charges         = fields[14].GetUInt32();

        while (true)
        {
            if (mSpellProcMap.find(spellId) != mSpellProcMap.end())
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_proc` has duplicate entry in the table", spellId);
                break;
            }
            SpellProcEntry procEntry = SpellProcEntry(baseProcEntry);

            // take defaults from dbcs
            if (!procEntry.typeMask)
                procEntry.typeMask = spellEntry->ProcFlags;
            if (!procEntry.charges)
                procEntry.charges = spellEntry->ProcCharges;
            if (!procEntry.chance && !procEntry.ratePerMinute)
                procEntry.chance = float(spellEntry->ProcChance);

            // validate data
            if (procEntry.schoolMask & ~SPELL_SCHOOL_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `schoolMask` set: %u", spellId, procEntry.schoolMask);
            if (procEntry.spellFamilyName && (procEntry.spellFamilyName < 3 || procEntry.spellFamilyName > 17 || procEntry.spellFamilyName == 14 || procEntry.spellFamilyName == 16))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `spellFamilyName` set: %u", spellId, procEntry.spellFamilyName);
            if (procEntry.chance < 0)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has negative value in `chance` field", spellId);
                procEntry.chance = 0;
            }
            if (procEntry.ratePerMinute < 0)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has negative value in `ratePerMinute` field", spellId);
                procEntry.ratePerMinute = 0;
            }
            if (cooldown < 0)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has negative value in `cooldown` field", spellId);
                procEntry.cooldown = 0;
            }
            if (procEntry.chance == 0 && procEntry.ratePerMinute == 0)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u doesn't have `chance` and `ratePerMinute` values defined, proc will not be triggered", spellId);
            if (procEntry.charges > 99)
            {
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has too big value in `charges` field", spellId);
                procEntry.charges = 99;
            }
            if (!procEntry.typeMask)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u doesn't have `typeMask` value defined, proc will not be triggered", spellId);
            if (procEntry.spellTypeMask & ~PROC_SPELL_PHASE_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `spellTypeMask` set: %u", spellId, procEntry.spellTypeMask);
            if (procEntry.spellTypeMask && !(procEntry.typeMask & (SPELL_PROC_FLAG_MASK | PERIODIC_PROC_FLAG_MASK)))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has `spellTypeMask` value defined, but it won't be used for defined `typeMask` value", spellId);
            if (!procEntry.spellPhaseMask && procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u doesn't have `spellPhaseMask` value defined, but it's required for defined `typeMask` value, proc will not be triggered", spellId);
            if (procEntry.spellPhaseMask & ~PROC_SPELL_PHASE_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `spellPhaseMask` set: %u", spellId, procEntry.spellPhaseMask);
            if (procEntry.spellPhaseMask && !(procEntry.typeMask & REQ_SPELL_PHASE_PROC_FLAG_MASK))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has `spellPhaseMask` value defined, but it won't be used for defined `typeMask` value", spellId);
            if (procEntry.hitMask & ~PROC_HIT_MASK_ALL)
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has wrong `hitMask` set: %u", spellId, procEntry.hitMask);
            if (procEntry.hitMask && !(procEntry.typeMask & TAKEN_HIT_PROC_FLAG_MASK || (procEntry.typeMask & DONE_HIT_PROC_FLAG_MASK && (!procEntry.spellPhaseMask || procEntry.spellPhaseMask & (PROC_SPELL_PHASE_HIT | PROC_SPELL_PHASE_FINISH)))))
                sLog->outError(LOG_FILTER_SQL, "`spell_proc` table entry for spellId %u has `hitMask` value defined, but it won't be used for defined `typeMask` and `spellPhaseMask` values", spellId);

            mSpellProcMap[spellId] = procEntry;

            if (allRanks)
            {
                spellId = GetNextSpellInChain(spellId);
                spellEntry = GetSpellInfo(spellId);
            }
            else
                break;
        }
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell proc conditions and data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellBonusess()
{
    uint32 oldMSTime = getMSTime();

    mSpellBonusMap.clear();                             // need for reload case

    //                                                0      1             2          3         4
    QueryResult result = WorldDatabase.Query("SELECT entry, direct_bonus, dot_bonus, ap_bonus, ap_dot_bonus FROM spell_bonus_data");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell bonus data. DB table `spell_bonus_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();

        SpellInfo const* spell = GetSpellInfo(entry);
        if (!spell)
        {
            WorldDatabase.PExecute("DELETE FROM spell_bonus_data WHERE entry = %u;", entry);
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_bonus_data` does not exist", entry);
            continue;
        }

        SpellBonusEntry& sbe = mSpellBonusMap[entry];
        sbe.direct_damage = fields[1].GetFloat();
        sbe.dot_damage    = fields[2].GetFloat();
        sbe.ap_bonus      = fields[3].GetFloat();
        sbe.ap_dot_bonus   = fields[4].GetFloat();

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u extra spell bonus data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellThreats()
{
    uint32 oldMSTime = getMSTime();

    mSpellThreatMap.clear();                                // need for reload case

    //                                                0      1        2       3
    QueryResult result = WorldDatabase.Query("SELECT entry, flatMod, pctMod, apPctMod FROM spell_threat");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 aggro generating spells. DB table `spell_threat` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        if (!GetSpellInfo(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_threat` does not exist", entry);
            continue;
        }

        SpellThreatEntry ste;
        ste.flatMod  = fields[1].GetInt32();
        ste.pctMod   = fields[2].GetFloat();
        ste.apPctMod = fields[3].GetFloat();

        mSpellThreatMap[entry] = ste;
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u SpellThreatEntries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSkillLineAbilityMap()
{
    uint32 oldMSTime = getMSTime();

    mSkillLineAbilityMap.clear();

    uint32 count = 0;

    for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
    {
        SkillLineAbilityEntry const* SkillInfo = sSkillLineAbilityStore.LookupEntry(i);
        if (!SkillInfo)
            continue;

        mSkillLineAbilityMap.insert(SkillLineAbilityMap::value_type(SkillInfo->spellId, SkillInfo));
        ++count;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u SkillLineAbility MultiMap Data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellPetAuras()
{
    uint32 oldMSTime = getMSTime();

    mSpellPetAuraMap.clear();                                  // need for reload case

    //                                                  0       1       2    3
    QueryResult result = WorldDatabase.Query("SELECT spell, effectId, pet, aura FROM spell_pet_auras");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell pet auras. DB table `spell_pet_auras` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        uint8 eff = fields[1].GetUInt8();
        uint32 pet = fields[2].GetUInt32();
        uint32 aura = fields[3].GetUInt32();

        SpellPetAuraMap::iterator itr = mSpellPetAuraMap.find((spell<<8) + eff);
        if (itr != mSpellPetAuraMap.end())
            itr->second.AddAura(pet, aura);
        else
        {
            SpellInfo const* spellInfo = GetSpellInfo(spell);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_pet_auras` does not exist", spell);
                continue;
            }
            if (spellInfo->Effects[eff].Effect != SPELL_EFFECT_DUMMY &&
                (spellInfo->Effects[eff].Effect != SPELL_EFFECT_APPLY_AURA ||
                spellInfo->Effects[eff].ApplyAuraName != SPELL_AURA_DUMMY))
            {
                sLog->outError(LOG_FILTER_SPELLS_AURAS, "Spell %u listed in `spell_pet_auras` does not have dummy aura or dummy effect", spell);
                continue;
            }

            SpellInfo const* spellInfo2 = GetSpellInfo(aura);
            if (!spellInfo2)
            {
                sLog->outError(LOG_FILTER_SQL, "Aura %u listed in `spell_pet_auras` does not exist", aura);
                continue;
            }

            PetAura pa(pet, aura, spellInfo->Effects[eff].TargetA.GetTarget() == TARGET_UNIT_PET, spellInfo->Effects[eff].CalcValue());
            mSpellPetAuraMap[(spell<<8) + eff] = pa;
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell pet auras in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

// Fill custom data about enchancments
void SpellMgr::LoadEnchantCustomAttr()
{
    uint32 oldMSTime = getMSTime();

    uint32 size = sSpellItemEnchantmentStore.GetNumRows();
    mEnchantCustomAttr.resize(size);

    for (uint32 i = 0; i < size; ++i)
        mEnchantCustomAttr[i] = 0;

    uint32 count = 0;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = GetSpellInfo(i);
        if (!spellInfo)
            continue;

        // TODO: find a better check
        if (!(spellInfo->AttributesEx2 & SPELL_ATTR2_PRESERVE_ENCHANT_IN_ARENA) || !(spellInfo->Attributes & SPELL_ATTR0_NOT_SHAPESHIFT))
            continue;

        for (uint32 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (spellInfo->Effects[j].Effect == SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY)
            {
                uint32 enchId = spellInfo->Effects[j].MiscValue;
                SpellItemEnchantmentEntry const* ench = sSpellItemEnchantmentStore.LookupEntry(enchId);
                if (!ench)
                    continue;
                mEnchantCustomAttr[enchId] = true;
                ++count;
                break;
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u custom enchant attributes in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellEnchantProcData()
{
    uint32 oldMSTime = getMSTime();

    mSpellEnchantProcEventMap.clear();                             // need for reload case

    //                                                  0         1           2         3
    QueryResult result = WorldDatabase.Query("SELECT entry, customChance, PPMChance, procEx FROM spell_enchant_proc_data");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell enchant proc event conditions. DB table `spell_enchant_proc_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 enchantId = fields[0].GetUInt32();

        SpellItemEnchantmentEntry const* ench = sSpellItemEnchantmentStore.LookupEntry(enchantId);
        if (!ench)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchancment %u listed in `spell_enchant_proc_data` does not exist", enchantId);
            continue;
        }

        SpellEnchantProcEntry spe;

        spe.customChance = fields[1].GetUInt32();
        spe.PPMChance = fields[2].GetFloat();
        spe.procEx = fields[3].GetUInt32();

        mSpellEnchantProcEventMap[enchantId] = spe;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u enchant proc data definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellLinked()
{
    uint32 oldMSTime = getMSTime();

    mSpellLinkedMap.clear();    // need for reload case

    //                                                0              1             2
    QueryResult result = WorldDatabase.Query("SELECT spell_trigger, spell_effect, type FROM spell_linked_spell");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 linked spells. DB table `spell_linked_spell` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 trigger = fields[0].GetInt32();
        int32 effect = fields[1].GetInt32();
        int32 type = fields[2].GetUInt8();

        SpellInfo const* spellInfo = GetSpellInfo(abs(trigger));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_linked_spell` does not exist", abs(trigger));
            continue;
        }
        spellInfo = GetSpellInfo(abs(effect));
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_linked_spell` does not exist", abs(effect));
            continue;
        }

        if (type) //we will find a better way when more types are needed
        {
            if (trigger > 0)
                trigger += SPELL_LINKED_MAX_SPELLS * type;
            else
                trigger -= SPELL_LINKED_MAX_SPELLS * type;
        }
        mSpellLinkedMap[trigger].push_back(effect);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u linked spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadPetLevelupSpellMap()
{
    uint32 oldMSTime = getMSTime();

    mPetLevelupSpellMap.clear();                                   // need for reload case

    uint32 count = 0;
    uint32 family_count = 0;

    for (uint32 i = 0; i < sCreatureFamilyStore.GetNumRows(); ++i)
    {
        CreatureFamilyEntry const* creatureFamily = sCreatureFamilyStore.LookupEntry(i);
        if (!creatureFamily)                                     // not exist
            continue;

        for (uint8 j = 0; j < 2; ++j)
        {
            if (!creatureFamily->skillLine[j])
                continue;

            for (uint32 k = 0; k < sSkillLineAbilityStore.GetNumRows(); ++k)
            {
                SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(k);
                if (!skillLine)
                    continue;

                //if (skillLine->skillId != creatureFamily->skillLine[0] &&
                //    (!creatureFamily->skillLine[1] || skillLine->skillId != creatureFamily->skillLine[1]))
                //    continue;

                if (skillLine->skillId != creatureFamily->skillLine[j])
                    continue;

                if (skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                SpellInfo const* spell = GetSpellInfo(skillLine->spellId);
                if (!spell) // not exist or triggered or talent
                    continue;

                if (!spell->SpellLevel)
                    continue;

                PetLevelupSpellSet& spellSet = mPetLevelupSpellMap[creatureFamily->ID];
                if (spellSet.empty())
                    ++family_count;

                spellSet.insert(PetLevelupSpellSet::value_type(spell->SpellLevel, spell->Id));
                ++count;
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pet levelup and default spells for %u families in %u ms", count, family_count, GetMSTimeDiffToNow(oldMSTime));
}

bool LoadPetDefaultSpells_helper(CreatureTemplate const* cInfo, PetDefaultSpellsEntry& petDefSpells)
{
    // skip empty list;
    bool have_spell = false;
    for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if (petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }
    if (!have_spell)
        return false;

    // remove duplicates with levelupSpells if any
    if (PetLevelupSpellSet const* levelupSpells = cInfo->family ? sSpellMgr->GetPetLevelupSpellList(cInfo->family) : NULL)
    {
        for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
        {
            if (!petDefSpells.spellid[j])
                continue;

            for (PetLevelupSpellSet::const_iterator itr = levelupSpells->begin(); itr != levelupSpells->end(); ++itr)
            {
                if (itr->second == petDefSpells.spellid[j])
                {
                    petDefSpells.spellid[j] = 0;
                    break;
                }
            }
        }
    }

    // skip empty list;
    have_spell = false;
    for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
    {
        if (petDefSpells.spellid[j])
        {
            have_spell = true;
            break;
        }
    }

    return have_spell;
}

void SpellMgr::LoadPetDefaultSpells()
{
    uint32 oldMSTime = getMSTime();

    mPetDefaultSpellsMap.clear();

    uint32 countCreature = 0;
    uint32 countData = 0;

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {

        if (!itr->second.PetSpellDataId)
            continue;

        // for creature with PetSpellDataId get default pet spells from dbc
        CreatureSpellDataEntry const* spellDataEntry = sCreatureSpellDataStore.LookupEntry(itr->second.PetSpellDataId);
        if (!spellDataEntry)
            continue;

        int32 petSpellsId = -int32(itr->second.PetSpellDataId);
        PetDefaultSpellsEntry petDefSpells;
        for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
            petDefSpells.spellid[j] = spellDataEntry->spellId[j];

        if (LoadPetDefaultSpells_helper(&itr->second, petDefSpells))
        {
            mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
            ++countData;
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded addition spells for %u pet spell data entries in %u ms", countData, GetMSTimeDiffToNow(oldMSTime));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading summonable creature templates...");
    oldMSTime = getMSTime();

    // different summon spells
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellEntry = GetSpellInfo(i);
        if (!spellEntry)
            continue;

        for (uint8 k = 0; k < MAX_SPELL_EFFECTS; ++k)
        {
            if (spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON || spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON_PET)
            {
                uint32 creature_id = spellEntry->Effects[k].MiscValue;
                CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature_id);
                if (!cInfo)
                    continue;

                // already loaded
                if (cInfo->PetSpellDataId)
                    continue;

                // for creature without PetSpellDataId get default pet spells from creature_template
                int32 petSpellsId = cInfo->Entry;
                if (mPetDefaultSpellsMap.find(cInfo->Entry) != mPetDefaultSpellsMap.end())
                    continue;

                PetDefaultSpellsEntry petDefSpells;
                for (uint8 j = 0; j < MAX_CREATURE_SPELL_DATA_SLOT; ++j)
                    petDefSpells.spellid[j] = cInfo->spells[j];

                if (LoadPetDefaultSpells_helper(cInfo, petDefSpells))
                {
                    mPetDefaultSpellsMap[petSpellsId] = petDefSpells;
                    ++countCreature;
                }
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u summonable creature templates in %u ms", countCreature, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadSpellAreas()
{
    uint32 oldMSTime = getMSTime();

    mSpellAreaMap.clear();                                  // need for reload case
    mSpellAreaForQuestMap.clear();
    mSpellAreaForActiveQuestMap.clear();
    mSpellAreaForQuestEndMap.clear();
    mSpellAreaForAuraMap.clear();

    //                                                  0     1         2              3               4                 5          6          7       8         9
    QueryResult result = WorldDatabase.Query("SELECT spell, area, quest_start, quest_start_status, quest_end_status, quest_end, aura_spell, racemask, gender, autocast FROM spell_area");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell area requirements. DB table `spell_area` is empty.");

        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 spell = fields[0].GetUInt32();
        SpellArea spellArea;
        spellArea.spellId             = spell;
        spellArea.areaId              = fields[1].GetUInt32();
        spellArea.questStart          = fields[2].GetUInt32();
        spellArea.questStartStatus    = fields[3].GetUInt32();
        spellArea.questEndStatus      = fields[4].GetUInt32();
        spellArea.questEnd            = fields[5].GetUInt32();
        spellArea.auraSpell           = fields[6].GetInt32();
        spellArea.raceMask            = fields[7].GetUInt32();
        spellArea.gender              = Gender(fields[8].GetUInt8());
        spellArea.autocast            = fields[9].GetBool();

        if (SpellInfo const* spellInfo = GetSpellInfo(spell))
        {
            if (spellArea.autocast)
                const_cast<SpellInfo*>(spellInfo)->Attributes |= SPELL_ATTR0_CANT_CANCEL;
        }
        else
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` does not exist", spell);
            continue;
        }

        {
            bool ok = true;
            SpellAreaMapBounds sa_bounds = GetSpellAreaMapBounds(spellArea.spellId);
            for (SpellAreaMap::const_iterator itr = sa_bounds.first; itr != sa_bounds.second; ++itr)
            {
                if (spellArea.spellId != itr->second.spellId)
                    continue;
                if (spellArea.areaId != itr->second.areaId)
                    continue;
                if (spellArea.questStart != itr->second.questStart)
                    continue;
                if (spellArea.auraSpell != itr->second.auraSpell)
                    continue;
                if ((spellArea.raceMask & itr->second.raceMask) == 0)
                    continue;
                if (spellArea.gender != itr->second.gender)
                    continue;

                // duplicate by requirements
                ok =false;
                break;
            }

            if (!ok)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` already listed with similar requirements.", spell);
                continue;
            }
        }

        if (spellArea.areaId && !GetAreaEntryByAreaID(spellArea.areaId))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong area (%u) requirement", spell, spellArea.areaId);
            continue;
        }

        if (spellArea.questStart && !sObjectMgr->GetQuestTemplate(spellArea.questStart))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong start quest (%u) requirement", spell, spellArea.questStart);
            continue;
        }

        if (spellArea.questEnd)
        {
            if (!sObjectMgr->GetQuestTemplate(spellArea.questEnd))
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong end quest (%u) requirement", spell, spellArea.questEnd);
                continue;
            }
        }

        if (spellArea.auraSpell)
        {
            SpellInfo const* spellInfo = GetSpellInfo(abs(spellArea.auraSpell));
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong aura spell (%u) requirement", spell, abs(spellArea.auraSpell));
                continue;
            }

            if (uint32(abs(spellArea.auraSpell)) == spellArea.spellId)
            {
                sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have aura spell (%u) requirement for itself", spell, abs(spellArea.auraSpell));
                continue;
            }

            // not allow autocast chains by auraSpell field (but allow use as alternative if not present)
            if (spellArea.autocast && spellArea.auraSpell > 0)
            {
                bool chain = false;
                SpellAreaForAuraMapBounds saBound = GetSpellAreaForAuraMapBounds(spellArea.spellId);
                for (SpellAreaForAuraMap::const_iterator itr = saBound.first; itr != saBound.second; ++itr)
                {
                    if (itr->second->autocast && itr->second->auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if (chain)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }

                SpellAreaMapBounds saBound2 = GetSpellAreaMapBounds(spellArea.auraSpell);
                for (SpellAreaMap::const_iterator itr2 = saBound2.first; itr2 != saBound2.second; ++itr2)
                {
                    if (itr2->second.autocast && itr2->second.auraSpell > 0)
                    {
                        chain = true;
                        break;
                    }
                }

                if (chain)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have aura spell (%u) requirement that itself autocast from aura", spell, spellArea.auraSpell);
                    continue;
                }
            }
        }

        if (spellArea.raceMask && (spellArea.raceMask & RACEMASK_ALL_PLAYABLE) == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong race mask (%u) requirement", spell, spellArea.raceMask);
            continue;
        }

        if (spellArea.gender != GENDER_NONE && spellArea.gender != GENDER_FEMALE && spellArea.gender != GENDER_MALE)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u listed in `spell_area` have wrong gender (%u) requirement", spell, spellArea.gender);
            continue;
        }

        SpellArea const* sa = &mSpellAreaMap.insert(SpellAreaMap::value_type(spell, spellArea))->second;

        // for search by current zone/subzone at zone/subzone change
        if (spellArea.areaId)
            mSpellAreaForAreaMap.insert(SpellAreaForAreaMap::value_type(spellArea.areaId, sa));

        // for search at quest start/reward
        if (spellArea.questStart)
            mSpellAreaForQuestMap.insert(SpellAreaForQuestMap::value_type(spellArea.questStart, sa));

        // for search at quest start/reward
        if (spellArea.questEnd)
            mSpellAreaForQuestEndMap.insert(SpellAreaForQuestMap::value_type(spellArea.questEnd, sa));

        // for search at aura apply
        if (spellArea.auraSpell)
            mSpellAreaForAuraMap.insert(SpellAreaForAuraMap::value_type(abs(spellArea.auraSpell), sa));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell area requirements in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

static const uint32 SkillClass[MAX_CLASSES] = {0, 840, 800, 795, 921, 804, 796, 924, 904, 849, 829, 798};

void SpellMgr::LoadSpellClassInfo()
{
    mSpellClassInfo.resize(MAX_CLASSES);
    for (int ClassID = 0; ClassID < MAX_CLASSES; ClassID++)
    {
        ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(ClassID);
        if (!classEntry)
            continue;

        // Player damage reduction (72% base resilience)
        mSpellClassInfo[ClassID].insert(115043);
        mSpellClassInfo[ClassID].insert(142689);
        // Player mastery activation
        mSpellClassInfo[ClassID].insert(114585);
        // Battle Fatigue
        mSpellClassInfo[ClassID].insert(134732);

        // All Rune for DK
        if (ClassID == CLASS_DEATH_KNIGHT)
        {
            mSpellClassInfo[ClassID].insert(53323);
            mSpellClassInfo[ClassID].insert(54447);
            mSpellClassInfo[ClassID].insert(53342);
            mSpellClassInfo[ClassID].insert(53331);
            mSpellClassInfo[ClassID].insert(54446);
            mSpellClassInfo[ClassID].insert(53323);
            mSpellClassInfo[ClassID].insert(53344);
            mSpellClassInfo[ClassID].insert(70164);
            mSpellClassInfo[ClassID].insert(62158);
        }

        // Swift Flight Form
        if (ClassID == CLASS_DRUID)
            mSpellClassInfo[ClassID].insert(40120);

        // Dark Soul
        if (ClassID == CLASS_WARLOCK)
            mSpellClassInfo[ClassID].insert(77801);

        // All portals and teleports for mages and Mana Attunement
        if (ClassID == CLASS_MAGE)
        {
            mSpellClassInfo[ClassID].insert(3561);
            mSpellClassInfo[ClassID].insert(3562);
            mSpellClassInfo[ClassID].insert(3563);
            mSpellClassInfo[ClassID].insert(3565);
            mSpellClassInfo[ClassID].insert(3566);
            mSpellClassInfo[ClassID].insert(3567);
            mSpellClassInfo[ClassID].insert(32271);
            mSpellClassInfo[ClassID].insert(32272);
            mSpellClassInfo[ClassID].insert(49359);
            mSpellClassInfo[ClassID].insert(49360);
            mSpellClassInfo[ClassID].insert(32266);
            mSpellClassInfo[ClassID].insert(32267);
            mSpellClassInfo[ClassID].insert(10059);
            mSpellClassInfo[ClassID].insert(11416);
            mSpellClassInfo[ClassID].insert(11417);
            mSpellClassInfo[ClassID].insert(11418);
            mSpellClassInfo[ClassID].insert(11419);
            mSpellClassInfo[ClassID].insert(11420);
            mSpellClassInfo[ClassID].insert(49358);
            mSpellClassInfo[ClassID].insert(49361);
            mSpellClassInfo[ClassID].insert(35715);
            mSpellClassInfo[ClassID].insert(33690);
            mSpellClassInfo[ClassID].insert(33691);
            mSpellClassInfo[ClassID].insert(35717);
            mSpellClassInfo[ClassID].insert(53140);
            mSpellClassInfo[ClassID].insert(53142);
            mSpellClassInfo[ClassID].insert(88342);
            mSpellClassInfo[ClassID].insert(88344);
            mSpellClassInfo[ClassID].insert(88345);
            mSpellClassInfo[ClassID].insert(88346);
            mSpellClassInfo[ClassID].insert(121039);
            mSpellClassInfo[ClassID].insert(132626);
            mSpellClassInfo[ClassID].insert(132627);
            mSpellClassInfo[ClassID].insert(132621);
            mSpellClassInfo[ClassID].insert(132620);
        }

        // Ancestral Focus
        if (ClassID == CLASS_SHAMAN)
            mSpellClassInfo[ClassID].insert(89920);

        // Plate Mail skill
        if (ClassID == CLASS_PALADIN || ClassID == CLASS_WARRIOR)
            mSpellClassInfo[ClassID].insert(750);

        // Mail skill
        if (ClassID == CLASS_SHAMAN || ClassID == CLASS_HUNTER)
            mSpellClassInfo[ClassID].insert(8737);

        // Dual Wield
        if (ClassID == CLASS_WARRIOR || ClassID == CLASS_HUNTER || ClassID == CLASS_ROGUE || ClassID == CLASS_DEATH_KNIGHT || ClassID == CLASS_MONK)
            mSpellClassInfo[ClassID].insert(674);

        // Natural Insight druid
        if (ClassID == CLASS_DRUID)
            mSpellClassInfo[ClassID].insert(112857);
        
        // Sinister Strike Enabler
        if (ClassID == CLASS_ROGUE)
            mSpellClassInfo[ClassID].insert(79327);

        // Opening gameobject
        if (ClassID == CLASS_MONK)
        {
            mSpellClassInfo[ClassID].insert(3365);
            mSpellClassInfo[ClassID].insert(6247);
            mSpellClassInfo[ClassID].insert(6477);
            mSpellClassInfo[ClassID].insert(6478);
            mSpellClassInfo[ClassID].insert(21651);
            mSpellClassInfo[ClassID].insert(22810);
            mSpellClassInfo[ClassID].insert(61437);
            mSpellClassInfo[ClassID].insert(68398);
            mSpellClassInfo[ClassID].insert(96220);
        }

        for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
        {
            SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(i);
            if (!skillLine)
                continue;

            SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(skillLine->spellId);
            if (!spellEntry)
                continue;

            if (spellEntry->SpellLevel == 0)
                continue;

            if (skillLine->skillId !=  SkillClass[ClassID] || skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                continue;

            // See CGSpellBook::InitFutureSpells in client
            if (spellEntry->Attributes & SPELL_ATTR0_TRADESPELL || spellEntry->Attributes & SPELL_ATTR0_HIDDEN_CLIENTSIDE
                || spellEntry->AttributesEx8 & SPELL_ATTR8_UNK13 || spellEntry->AttributesEx4 & SPELL_ATTR4_UNK15)
                continue;

            if (sSpellMgr->IsTalent(spellEntry->Id))
                continue;

            mSpellClassInfo[ClassID].insert(spellEntry->Id);
        }

        for (uint32 i = 0; i < sSpecializationSpellStore.GetNumRows(); ++i)
        {
            SpecializationSpellEntry const* specializationInfo = sSpecializationSpellStore.LookupEntry(i);
            if (!specializationInfo)
                continue;

            ChrSpecializationsEntry const* chrSpec = sChrSpecializationsStore.LookupEntry(specializationInfo->SpecializationEntry);
            if (!chrSpec)
                continue;

            mSpellClassInfo[chrSpec->classId].insert(specializationInfo->LearnSpell);
        }
    }
}

struct spellDifficultyLoadInfo
{
    uint32 id;
    std::list<uint32> difficultyList;
};

void SpellMgr::LoadSpellInfoStore()
{
    uint32 oldMSTime = getMSTime();

    std::map<uint32, std::set<uint32> > spellDifficultyList;

    for (uint32 i = 0; i < sSpellEffectStore.GetNumRows(); ++i)
        if (SpellEffectEntry const* spellEffect = sSpellEffectStore.LookupEntry(i))
            spellDifficultyList[spellEffect->EffectSpellId].insert(spellEffect->EffectDifficulty);


    UnloadSpellInfoStore();
    for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
        mSpellInfoMap[difficulty].resize(sSpellStore.GetNumRows(), NULL);

    for (uint32 i = 0; i < sSpellStore.GetNumRows(); ++i)
    {
        if (SpellEntry const* spellEntry = sSpellStore.LookupEntry(i))
        {
            std::set<uint32> difficultyInfo = spellDifficultyList[i];
            for (std::set<uint32>::iterator itr = difficultyInfo.begin(); itr != difficultyInfo.end(); itr++)
                mSpellInfoMap[(*itr)][i] = new SpellInfo(spellEntry, (*itr));
        }
    }

    std::set<uint32> alreadySet;
    for (uint32 i = 0; i < sSpellPowerStore.GetNumRows(); i++)
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(i);
        if (!spellPower)
            continue;

        if (alreadySet.find(spellPower->SpellId) != alreadySet.end())
            continue;

        for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
        {
            SpellInfo* spell = mSpellInfoMap[difficulty][spellPower->SpellId];
            if (!spell)
                continue;

            spell->ManaCost = spellPower->manaCost;
            spell->ManaCostPercentage = spellPower->ManaCostPercentage;
            spell->ManaPerSecond = spellPower->manaPerSecond;
            spell->PowerType = spellPower->powerType;

            spell->spellPower->manaCost = spellPower->manaCost;
            spell->spellPower->ManaCostPercentage = spellPower->ManaCostPercentage;
            spell->spellPower->manaPerSecond = spellPower->manaPerSecond;
            spell->spellPower->powerType = spellPower->powerType;
        }

        alreadySet.insert(spellPower->SpellId);
    }

    for (uint32 i = 0; i < sTalentStore.GetNumRows(); i++)
    {
        TalentEntry const* talentInfo = sTalentStore.LookupEntry(i);
        if (!talentInfo)
            continue;

        SpellInfo * spellEntry = mSpellInfoMap[NONE_DIFFICULTY][talentInfo->spellId];
        if (spellEntry)
            spellEntry->talentId = talentInfo->Id;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded spell info store in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::UnloadSpellInfoStore()
{
    for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
    {
        for (uint32 i = 0; i < mSpellInfoMap[difficulty].size(); ++i)
        {
            if (mSpellInfoMap[difficulty][i])
                delete mSpellInfoMap[difficulty][i];
        }
        mSpellInfoMap[difficulty].clear();
    }
}

void SpellMgr::UnloadSpellInfoImplicitTargetConditionLists()
{
    for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
    {
        for (uint32 i = 0; i < mSpellInfoMap[difficulty].size(); ++i)
        {
            if (mSpellInfoMap[difficulty][i])
                mSpellInfoMap[difficulty][i]->_UnloadImplicitTargetConditionLists();
        }
    }
}

void SpellMgr::LoadSpellCustomAttr()
{
    uint32 oldMSTime = getMSTime();

    SpellInfo* spellInfo = NULL;
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        for (int difficulty = 0; difficulty < MAX_DIFFICULTY; difficulty++)
        {
            spellInfo = mSpellInfoMap[difficulty][i];
            if (!spellInfo)
                continue;

            for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
            {
                switch (spellInfo->Effects[j].ApplyAuraName)
                {
                    case SPELL_AURA_MOD_POSSESS:
                    case SPELL_AURA_MOD_CONFUSE:
                    case SPELL_AURA_MOD_CHARM:
                    case SPELL_AURA_AOE_CHARM:
                    case SPELL_AURA_MOD_FEAR:
                    case SPELL_AURA_MOD_FEAR_2:
                    case SPELL_AURA_MOD_STUN:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                        break;
                    case SPELL_AURA_PERIODIC_HEAL:
                    case SPELL_AURA_PERIODIC_DAMAGE:
                    case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
                    case SPELL_AURA_PERIODIC_LEECH:
                    case SPELL_AURA_PERIODIC_MANA_LEECH:
                    case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                    case SPELL_AURA_PERIODIC_ENERGIZE:
                    case SPELL_AURA_OBS_MOD_HEALTH:
                    case SPELL_AURA_OBS_MOD_POWER:
                    case SPELL_AURA_POWER_BURN:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                        break;
                }

                switch (spellInfo->Effects[j].Effect)
                {
                    case SPELL_EFFECT_SCHOOL_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE:
                    case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
                    case SPELL_EFFECT_NORMALIZED_WEAPON_DMG:
                    case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
                    case SPELL_EFFECT_HEAL:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_DIRECT_DAMAGE;
                        break;
                    case SPELL_EFFECT_POWER_DRAIN:
                    case SPELL_EFFECT_POWER_BURN:
                    case SPELL_EFFECT_HEAL_MAX_HEALTH:
                    case SPELL_EFFECT_HEALTH_LEECH:
                    case SPELL_EFFECT_HEAL_PCT:
                    case SPELL_EFFECT_ENERGIZE_PCT:
                    case SPELL_EFFECT_ENERGIZE:
                    case SPELL_EFFECT_HEAL_MECHANICAL:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_NO_INITIAL_THREAT;
                        break;
                    case SPELL_EFFECT_CHARGE:
                    case SPELL_EFFECT_CHARGE_DEST:
                    case SPELL_EFFECT_JUMP:
                    case SPELL_EFFECT_JUMP_DEST:
                    case SPELL_EFFECT_LEAP_BACK:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_CHARGE;
                        break;
                    case SPELL_EFFECT_PICKPOCKET:
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_PICKPOCKET;
                        break;
                    case SPELL_EFFECT_CREATE_ITEM:
                    case SPELL_EFFECT_CREATE_ITEM_2:
                        mSpellCreateItemList.push_back(i);
                        break;
                    case SPELL_EFFECT_ENCHANT_ITEM:
                    case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
                    case SPELL_EFFECT_ENCHANT_ITEM_PRISMATIC:
                    case SPELL_EFFECT_ENCHANT_HELD_ITEM:
                    {
                        // only enchanting profession enchantments procs can stack
                        if (IsPartOfSkillLine(SKILL_ENCHANTING, i))
                        {
                            uint32 enchantId = spellInfo->Effects[j].MiscValue;
                            SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchantId);
                            for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                            {
                                if (enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                                    continue;

                                SpellInfo* procInfo = (SpellInfo*)GetSpellInfo(enchant->spellid[s]);
                                if (!procInfo)
                                    continue;

                                // if proced directly from enchantment, not via proc aura
                                // NOTE: Enchant Weapon - Blade Ward also has proc aura spell and is proced directly
                                // however its not expected to stack so this check is good
                                if (procInfo->HasAura(SPELL_AURA_PROC_TRIGGER_SPELL))
                                    continue;

                            procInfo->AttributesCu |= SPELL_ATTR0_CU_ENCHANT_STACK;
                        }
                    }
                    else if (IsPartOfSkillLine(SKILL_RUNEFORGING, i))
                    {
                        uint32 enchantId = spellInfo->Effects[j].MiscValue;
                        SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(enchantId);
                        for (uint8 s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                        {
                            SpellInfo* procInfo = (SpellInfo*)GetSpellInfo(enchant->spellid[s]);
                            if (!procInfo)
                                continue;

                            switch (procInfo->Id)
                            {
                                case 53365: // Rune of the Fallen Crusader
                                case 51714: // Rune of Razorice
                                    continue;
                            }

                            procInfo->AttributesCu |= SPELL_ATTR0_CU_ENCHANT_STACK;
                        }
                    }
                    break;
                }
            }
        }

        if (!spellInfo->_IsPositiveEffect(EFFECT_0, false))
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;

        if (!spellInfo->_IsPositiveEffect(EFFECT_1, false))
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF1;

        if (!spellInfo->_IsPositiveEffect(EFFECT_2, false))
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF2;

        if (spellInfo->SpellVisual[0] == 3879 || spellInfo->Id == 74117)
            spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_BACK;

        ////////////////////////////////////
        ///      DEFINE BINARY SPELLS   ////
        ////////////////////////////////////
        for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            switch (spellInfo->Effects[j].Effect)
            {
                case SPELL_EFFECT_DISPEL:
                case SPELL_EFFECT_STEAL_BENEFICIAL_BUFF:
                case SPELL_EFFECT_POWER_DRAIN:
                case SPELL_EFFECT_POWER_BURN:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_BINARY;
                    break;
            }

            switch (spellInfo->Effects[j].Mechanic)
            {
                case MECHANIC_FEAR:
                case MECHANIC_CHARM:
                case MECHANIC_SNARE:
                    // Frost Bolt is not binary
                    if (spellInfo->Id == 116)
                        break;
                case MECHANIC_FREEZE:
                case MECHANIC_BANISH:
                case MECHANIC_POLYMORPH:
                case MECHANIC_ROOT:
                case MECHANIC_INTERRUPT:
                case MECHANIC_SILENCE:
                case MECHANIC_HORROR:
                    spellInfo->AttributesCu |= SPELL_ATTR0_CU_BINARY;
                    break;
             }
        }

        switch (spellInfo->Mechanic)
        {
            case MECHANIC_FEAR:
            case MECHANIC_CHARM:
            case MECHANIC_SNARE:
            case MECHANIC_FREEZE:
            case MECHANIC_BANISH:
            case MECHANIC_POLYMORPH:
            case MECHANIC_ROOT:
            case MECHANIC_INTERRUPT:
            case MECHANIC_SILENCE:
            case MECHANIC_HORROR:
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_BINARY;
                break;
        }

        ///////////////////////////
        //////   END BINARY  //////
        ///////////////////////////

        switch (spellInfo->Id)
        {
            case 48505: // Starfall
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[1].BasePoints = 20;
                break;
            case 32409: // Shadow Word: Death (triggered)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                spellInfo->Effects[0].ApplyAuraName = 0;
                break;
            case 88611: // Smoke Bomb (triggered)
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 145109:// Ysera's Gift (caster)
            case 145110:// Ysera's Gift (ally)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_HEAL_PCT;
                spellInfo->Effects[0].BasePoints = 5;
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 128997:// Spirit Beast Blessing
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 127830:// Spirit Beast Blessing (Mastery Rating)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AREA_AURA_RAID;
                break;
            case 32546: // Binding Heal
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 15286: // Vampiric Embrace
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 29858: // Soulshatter
                spellInfo->OverrideSpellList.push_back(97827); // Add Taunt (Metamorphosis)
                break;
            case 119403:// Glyph of Explosive Trap
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 149575:// Explosive Trap (knock back)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[1].TargetA = TARGET_DEST_DEST;
                break;
            case 108287:// Totemic Projection
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 115176:// Zen Meditation
                spellInfo->ProcCharges = 5;
                spellInfo->Effects[0].BasePoints = 5;
                break;
            case 137639:// Storm, Earth and Fire
                spellInfo->AttributesCu &= ~(SPELL_ATTR0_CU_NEGATIVE_EFF1|SPELL_ATTR0_CU_NEGATIVE_EFF0);
                break;
            case 138130:// Storm, Earth and Fire (for spirits)
                spellInfo->Effects[0].Effect = 0;
                break;
            case 5740:  // Rain of Fire
            case 104232:// Rain of Fire (Aftermath)
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 130392:// Blink Strikes
                spellInfo->Effects[0].TargetA = TARGET_UNIT_PET;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_PET;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_PET;
                break;
            case 130393:// Blink Strike
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); // 30y
                break;
            case 135299:// Ice Trap (snare)
            case 140023:// Ring of Peace (dummy)
            case 81782: // Power Word: Barrier (buff)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(285); // 1s
                break;
            case 134735:// Battle Fatigue : Harcoded Basepoint for Season 13
                spellInfo->Effects[0].BasePoints = -30;
                spellInfo->Effects[1].BasePoints = -30;
                break;
            case 103965:// Metamorphosis (override auras)
                spellInfo->Effects[2].SpellClassMask[0] = 64;
                spellInfo->Effects[7].SpellClassMask[1] = 0x400;
                break;
            case 145518:// Genesis
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 119890:// Death Blossom
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(139); // 40y
                break;
            case 120629:// Huddle in Terror
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 119956:// Dread Spray (visual)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 119958:// Dread Spray
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(285); // 1s
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_1;
                spellInfo->Effects[0].Effect = 0;
                break;
            case 119414:// Breath of Fear
                spellInfo->Effects[2].Effect = 0;
                break;
            case 107145:// Wall of Light activation
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                break;
            case 119312:// Conjure Terror Spawn (01)
            case 119370:// Conjure Terror Spawn (02)
            case 119371:// Conjure Terror Spawn (03)
            case 119372:// Conjure Terror Spawn (04)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 117865:// Wall of Light
                spellInfo->Effects[0].Effect = 0;
                break;
            case 126848:// Ritual of Purification
                spellInfo->Effects[0].BasePoints = -10;
                spellInfo->Effects[1].BasePoints = -10;
                break;
            case 125706:// Channeling Corruption
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 125713:// Channeling Corruption (triggered)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 125736:// Night Terrors (missile)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 117230:// Overpowered
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                break;
            case 117988:// Defiled Ground
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[1].Mechanic = MECHANIC_NONE;
                break;
            case 117052:// Corruption Sha
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetB = 0;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[2].TargetB = 0;
                break;
            case 127731:// Corruption Sha (triggered)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 123244:// Hide
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[2].Effect = 0;
                break;
            case 82366:
            case 110704:
            case 116467:
                spellInfo->Effects[2].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 124009:
            case 130793:
            case 123996:
                spellInfo->StartRecoveryTime = 1500;
                spellInfo->StartRecoveryCategory = 133;
                spellInfo->Effects[3].Effect = 0;
                break;
            case 127424:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 45257: // Using Steam Tonk Controller
            case 45440: // Steam Tonk Controller
            case 49352: // Crashin' Thrashin' Racer Controller
            case 75111: // Blue Crashin' Thrashin' Racer Controller
            case 60256: // Collect Sample
                //Crashes client on pressing ESC (Maybe because of ReqSpellFocus and GameObject)
                spellInfo->AttributesEx4 &= ~SPELL_ATTR4_TRIGGERED;
                break;
            case 8629:
            case 11285:
            case 11286:
            case 12540:
            case 13579:
            case 24698:
            case 28456:
            case 29425:
            case 34940:
            case 36862:
            case 38764:
            case 38863:
            case 52743: // Head Smack
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER;
                break;
            case 53: // Backstab
            case 2589:
            case 2590:
            case 2591:
            case 8721:
            case 11279:
            case 11280:
            case 11281:
            case 25300:
            case 26863:
            case 48656:
            case 48657:
            case 8631:
            case 8632:
            case 8633:
            case 11289:
            case 11290:
            case 26839:
            case 26884:
            case 48675:
            case 48676:
            case 5221: // Shred
            case 6800:
            case 8992:
            case 9829:
            case 9830:
            case 27001:
            case 27002:
            case 48571:
            case 48572:
            case 8676: // Ambush
            case 8724:
            case 8725:
            case 11267:
            case 11268:
            case 11269:
            case 27441:
            case 48689:
            case 48690:
            case 48691:
            case 6787:
            case 9866:
            case 9867:
            case 27005:
            case 48578:
            case 48579:
            case 21987: // Lash of Pain
            case 23959: // Test Stab R50
            case 24825: // Test Backstab
            case 58563: // Assassinate Restless Lookout
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET;
                break;
            // Pandashan's Dragon Gun
            case 120751:
            case 120876:
            case 120964:
            case 124347:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                break;
            case 26029: // Dark Glare
            case 37433: // Spout
            case 43140: // Flame Breath
            case 43215: // Flame Breath
            case 70461: // Coldflame Trap
            case 72133: // Pain and Suffering
            case 73788: // Pain and Suffering
            case 73789: // Pain and Suffering
            case 73790: // Pain and Suffering
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_LINE;
                break;
            case 24340: // Meteor
            case 26558: // Meteor
            case 28884: // Meteor
            case 36837: // Meteor
            case 38903: // Meteor
            case 41276: // Meteor
            case 57467: // Meteor
            case 26789: // Shard of the Fallen Star
            case 31436: // Malevolent Cleave
            case 35181: // Dive Bomb
            case 40810: // Saber Lash
            case 43267: // Saber Lash
            case 43268: // Saber Lash
            case 42384: // Brutal Swipe
            case 45150: // Meteor Slash
            case 64688: // Sonic Screech
            case 72373: // Shared Suffering
            case 71904: // Chaos Bane
            case 70492: // Ooze Eruption
            case 72505: // Ooze Eruption
            case 72624: // Ooze Eruption
            case 72625: // Ooze Eruption
            case 88942: // Meteor Slash
            case 95172: // Meteor Slash
            case 82935: // Caustic Slime
            case 88915: // Caustic Slime
            case 88916: // Caustic Slime
            case 88917: // Caustic Slime
            case 86825: // Blackout
            case 92879: // Blackout
            case 92880: // Blackout
            case 92881: // Blackout
            case 77679: // Scorching Blast
            case 92968: // Scorching Blast
            case 92969: // Scorching Blast
            case 92970: // Scorching Blast
            case 98474: // Flame Scythe
            case 100212: // Flame Scythe
            case 100213: // Flame Scythe
            case 100214: // Flame Scythe
            case 105069: // Seething Hate
            case 108094: // Seething Hate
            case 103414: // Stomp
            case 108571: // Stomp
            case 109033: // Stomp
            case 109034: // Stomp
            case 103527: // Void Diffusion dmg
            case 104605: // Void Diffusion dmg
            case 108345: // Void Diffusion dmg
            case 108346: // Void Diffusion dmg
            case 106375: // Unstable Twilight
            case 109182: // Unstable Twilight
            case 109183: // Unstable Twilight
            case 109184: // Unstable Twilight
            case 107439: // Twilight Barrage
            case 109203: // Twilight Barrage
            case 109204: // Twilight Barrage
            case 109205: // Twilight Barrage
            case 106401: // Twilight Onslaught
            case 108862: // Twilight Onslaught
            case 109226: // Twilight Onslaught
            case 109227: // Twilight Onslaught
                // ONLY SPELLS WITH SPELLFAMILY_GENERIC and EFFECT_SCHOOL_DAMAGE
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 18500: // Wing Buffet
            case 33086: // Wild Bite
            case 49749: // Piercing Blow
            case 52890: // Penetrating Strike
            case 53454: // Impale
            case 59446: // Impale
            case 64777: // Machine Gun
            case 65239: // Machine Gun
            case 65919: // Impale
            case 67858: // Impale
            case 67859: // Impale
            case 67860: // Impale
            case 69293: // Wing Buffet
            case 74439: // Machine Gun
            case 63278: // Mark of the Faceless (General Vezax)
            case 62544: // Thrust (Argent Tournament)
            case 64588: // Thrust (Argent Tournament)
            case 66479: // Thrust (Argent Tournament)
            case 68505: // Thrust (Argent Tournament)
            case 62709: // Counterattack! (Argent Tournament)
            case 62626: // Break-Shield (Argent Tournament, Player)
            case 64590: // Break-Shield (Argent Tournament, Player)
            case 64342: // Break-Shield (Argent Tournament, NPC)
            case 64686: // Break-Shield (Argent Tournament, NPC)
            case 65147: // Break-Shield (Argent Tournament, NPC)
            case 68504: // Break-Shield (Argent Tournament, NPC)
            case 62874: // Charge (Argent Tournament, Player)
            case 68498: // Charge (Argent Tournament, Player)
            case 64591: // Charge (Argent Tournament, Player)
            case 63003: // Charge (Argent Tournament, NPC)
            case 63010: // Charge (Argent Tournament, NPC)
            case 68321: // Charge (Argent Tournament, NPC)
            case 72255: // Mark of the Fallen Champion (Deathbringer Saurfang)
            case 72444: // Mark of the Fallen Champion (Deathbringer Saurfang)
            case 72445: // Mark of the Fallen Champion (Deathbringer Saurfang)
            case 72446: // Mark of the Fallen Champion (Deathbringer Saurfang)
            case 62775: // Tympanic Tantrum (XT-002 encounter)
            case 102598: // Void Strike, Infinite Warden
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 64422: // Sonic Screech (Auriaya)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 72293: // Mark of the Fallen Champion (Deathbringer Saurfang)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                break;
            // Custom MoP Script
            // FENG
            case 116711:// Draw Flame
                spellInfo->ChannelInterruptFlags = 0x0;
                spellInfo->AuraInterruptFlags = 0x0;
                break;
            // GARAJAL
            case 118443:// Instantly Death
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            // ELEGON
            case 116989:// Overloaded Missile
            case 117220:// Overloaded Triggered
            case 118430:// Core Beam
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 129724:// Grasping Energy Tendrils
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 117960:// Celestial Breathe
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            case 118018:// Draw Power (Summons)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[3].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[4].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[5].TargetA = TARGET_UNIT_CASTER;
                break;
            case 116661:// Draw Power (lightning damage for activated focus)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            // SPIRIT KINGS
            case 117558:
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); // 10 yards
                break;
            case 117829:// Cowardice (DoT)
            case 117006:// Devastating Arc
            case 119775:// Reaching Attack
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 117708:// Maddening Shout
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                break;
            case 118047:// Pillage
                spellInfo->Effects[0].Effect = SPELL_EFFECT_CHARGE;
                spellInfo->Effects[0].TriggerSpell = 0;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            // SPELLS
            case 688:   // Summon Imp
                spellInfo->OverrideSpellList.push_back(112866); // Summon Fel Imp
                break;
            case 697:   // Summon Voidwalker
                spellInfo->OverrideSpellList.push_back(112867); // Summon Voidlord
                break;
            case 712:   // Summon Succubus
                spellInfo->OverrideSpellList.push_back(112868); // Summon Shivarra
                break;
            case 691:   // Summon Felhunter
                spellInfo->OverrideSpellList.push_back(112869); // Summon Observer
                break;
            case 30146: // Summon Felguard
                spellInfo->OverrideSpellList.push_back(112870); // Summon Wrathguard
                break;
            case 1122:  // Summon Infernal
                spellInfo->OverrideSpellList.push_back(112921); // Summon Abyssal
                break;
            case 18540: // Summon Doomguard
                spellInfo->OverrideSpellList.push_back(112927); // Summon Terrorguard
                break;
            case 120517:// Halo (Holy)
            case 120644:// Halo (shadow)
                spellInfo->AttributesCu &= ~SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 80240: // Havoc
                spellInfo->ProcCharges = 3;
                break;
            case 129020:// Avatar
                spellInfo->AttributesEx &= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 121129:// Daybreak (heal)
                spellInfo->Effects[1].TargetA = TARGET_SRC_CASTER;
                break;
            case 73981: // Redirect
            case 110730:// Redirect
            case 51699: // Honor Among Thieves (triggered)
            case 57934: // Tricks of the Trade
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                break;
            case 57723: // Exhaustion
            case 57724: // Sated
            case 80354: // Temporal Displacement
            case 95809: // Insanity
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 105770: // Item - Druid T13 Restoration 4P Bonus (Rejuvenation)
                spellInfo->Effects[0].SpellClassMask = flag128(0x00000050, 0, 0);
                break;
            case 105786: // Temporal Ruin Warlock 4P T13 bonus
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            // Item - Druid T12 Feral 4P Bonus
            case 99009:
                spellInfo->ProcChance = 100;
                spellInfo->ProcFlags = 16;
                break;
            // Item - Shaman T12 Enhancement 2P Bonus
            case 99209:
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            // Item - Shaman T12 Enhancement 4P Bonus
            case 99213:
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->ProcChance = 100;
                spellInfo->ProcFlags = 16;
                break;
            // Stormfire, Item - Shaman T12 Enhancement 4P Bonus
            case 99212:
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            // Item - Shaman T12 Elemental 4P Bonus
            case 99206:
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            // Item - Rogue T12 4P Bonus
            case 99175:
                spellInfo->ProcChance = 0;
                spellInfo->ProcFlags = 0;
                break;
            // Item - Warrior T12 DPS 2P Bonus
            case 99234:
                spellInfo->ProcChance = 0;
                spellInfo->ProcFlags = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                break;
            case 12654: // Ignite
            case 31803: // Censure
            case 77661: // Searing Flame
            case 77489: // Echo of Light
            case 99002: // Fiery Claws, Item - Druid T12 Feral 2P Bonus
            case 99132: // Divine Fire, Item - Priest T12 Healer 2P Bonus
            case 99173: // Burning Wounds, Item - Rogue T12 2P Bonus
            case 56161: // Glyph of Prayer of Healing
            case 55078: // Blood Plague
            case 55095: // Frost Fever
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                break;
            case 81662: // Will of the Necropolis
                spellInfo->Effects[0].BasePoints = 25;
                break;
            case 127538:// Savage Roar (Glyphed)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(636); // 12s / 0s / 42s
                break;
            case 146512:// Fortitude - hotfix 5.4.2
                spellInfo->Effects[0].BasePoints = 2600;
                break;
            case 15473: // Shadowform - hotfix 5.4.2
                spellInfo->Effects[6].BasePoints = 100;
                spellInfo->SchoolMask = SPELL_SCHOOL_MASK_SHADOW;
                break;
            case 49020: // Obliterate - hotfix 5.4.2
                spellInfo->Effects[1].BasePoints = 250;
                break;
            case 91107: // Unholy Might
                spellInfo->Effects[0].BasePoints = 35;
                spellInfo->OverrideSpellList.push_back(109260); // Add Aspect of the Iron Hack to override spell list of Aspect of the Hawk
                break;
            case 24858: // Moonkin form - hotfix 5.4.2
                spellInfo->Effects[2].BasePoints = 100;
                break;
            case 50887: // Icy Talons - hotfix 5.4.2
                spellInfo->Effects[0].BasePoints = 45;
                break;
            case 12294: // Mortal strike - hotfix 5.4.2
                spellInfo->Effects[2].BasePoints = 215;
                break;
            case 774:  // Rejuvenation - hotfix 5.4.2 (idk why they have 2 healing effects, so 2 ticks when must be one)
                spellInfo->Effects[2].Effect = 0;
                break;
            case 109260:// Aspect of the Iron Hawk - hotfix 5.4.2
                spellInfo->Effects[0].BasePoints = 35;
                break;
            case 48181: // Haunt - hotfix 5.4.2
                spellInfo->Effects[3].BasePoints = 35;
                break;
            case 16246: // Clearcasting - hotfix 5.4.2
                spellInfo->Effects[1].BasePoints = 20;
                break;
            case 105284:// Ancestral Vigor - fix 5.4.1
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(9); // 30s
                break;
            case 78976: // Solar Beam
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 982:   // Revive Pet - hotfix 5.4.2
                spellInfo->CastTimeEntry = sSpellCastTimesStore.LookupEntry(5); // 2s
                break;
            case 125676:// Glyph of Afterlife
                spellInfo->Effects[0].BasePoints = 50;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
                spellInfo->Effects[0].ValueMultiplier = 50;
                break;
            case 88852: // Tower of Radiance
                spellInfo->Effects[0].Effect = 0;
                break;
            case 24275: // Hammer of Wrath
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 53592: // Guarded by the Light
                spellInfo->Attributes |= SPELL_ATTR0_ABILITY;
                spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;
                spellInfo->Attributes |= SPELL_ATTR0_HIDE_IN_COMBAT_LOG;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 29144: // Unwavering Sentinel
                spellInfo->Attributes |= SPELL_ATTR0_ABILITY;
                spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 5246:  // Intimidating Shout
                spellInfo->MaxAffectedTargets |= 5;
                break;
            case 32379: // Shadow Word : Death - Glyph of Shadow Word: Death
                spellInfo->OverrideSpellList.push_back(129176);
                break;
            case 48045: // Mind Sear
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            case 91724: // Spell Warding
                spellInfo->Effects[0].BasePoints = 6;
                break;
            case 116481:// Tiger's lust
                spellInfo->Attributes |= SPELL_ATTR0_ABILITY;
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_HASTE_AFFECT_DURATION;
                break;
            case 879:   // Exorcism
                spellInfo->AttributesEx2 |= SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 20271: // Judgement
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 86704: // Ancient Fury
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 77613: // Grace
                spellInfo->Dispel = DISPEL_NONE;
                spellInfo->SpellIconID = 2819;
                break;
            case 2818:  // Deadly Poison
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REDIRECTED;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 108212:// Burst of Speed
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 31224: // Cloak of Shadows
                spellInfo->Effects[0].BasePoints = -200;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE;
                spellInfo->Effects[0].ValueMultiplier = -200;
                break;
            case 51690: // Killing Spree
                spellInfo->Effects[3].Effect = SPELL_EFFECT_FORCE_DESELECT;
                break;
            case 137619:// Marked for Death
                spellInfo->AttributesEx |= SPELL_ATTR1_NO_THREAT;
                break;
            case 8680:  // Wound Poison
            case 3409:  // Crippling Poison
            case 5760:  // Mind-numbing Poison
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REDIRECTED;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 115098:// Chi Wave - Add Aura for bounce counting
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[1].BasePoints = 1;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetB = 0;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1); // 10s
                spellInfo->Speed = 100.0f;
                break;
            case 118864:// Combo Breaker (tiger palm)
            case 116768:// Combo Breaker (blackout kick)
                spellInfo->ProcFlags = 0;
                break;
            case 117050:// Glaive Toss (talent)
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 120755:// Glaive Toss (Glaive right)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 94528: // Flare
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                break;
            case 108482:// Unbound Will
                // Crashes client on pressing ESC
                spellInfo->AttributesEx4 &= ~SPELL_ATTR4_TRIGGERED;
                break;
            case 66:    // Invisibility
                spellInfo->OverrideSpellList.push_back(110959); // Greater Invisibility
                spellInfo->Dispel = DISPEL_INVISIBILITY;
                break;
            case 110959:// Greater Invisibility
                spellInfo->Dispel = DISPEL_INVISIBILITY;
                break;
            case 47476: // Strangulate
                spellInfo->OverrideSpellList.push_back(108194); // Asphyxiate
                break;
            case 56131: // Glyph of Purify
                spellInfo->Effects[0].Effect = SPELL_EFFECT_HEAL_PCT;
                break;
            case 108945:// Angelic Bulwark
                spellInfo->Effects[0].TriggerSpell = 114214;
                break;
            case 34428: // Victory Rush
                spellInfo->OverrideSpellList.push_back(103840); // Impending Victory
                break;
            case 91342: // Shadow Infusion
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 27285: // Seed of Corruption
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); // 10 Yards
                break;
            case 104220:// Soulburn : Health Funnel
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 87385: // Soulburn : Seed of Corruption - damage
                spellInfo->Effects[0].TargetA = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); // 10 Yards
                break;
            case 86664: // Soulburn: Seed of Corruption
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(85);
                break;
            case 89523: // Glyph of Grounding Totem
                spellInfo->SpellFamilyName = SPELLFAMILY_SHAMAN;
                break;
            case 1856:  // Vanish
                spellInfo->Effects[1].TriggerSpell = 131368;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SANCTUARY;
                break;
            case 131369:// Vanish - Improved Stealth
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(27); // 3s
                break;
            case 116784:// Wildfire Spark - Boss Feng
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 67801: // Deterrence
                spellInfo->ProcCharges = 100;
                break;
            case 114234:// Glyph of Shred
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 120954:// Fortifying Brew
                spellInfo->Effects[4].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 114871:// Holy Prism (Allies)
            case 114852:// Holy Prism (ennemies)
                spellInfo->Effects[2].Effect = 0;
                break;
            case 123154:// Fists of Fury Visual Target
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(35); // 4s
                break;
            case 53479: // Last Stand (Tenacity)
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 104316:// Imp Swarm
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 114736:// Disrupted Nether
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(4); // 120s
                break;
            case 126309:// Still Water
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 126311:// Surface Trot
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AREA_AURA_PET;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 88767: // Fulmination (triggered)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 51514: // Hex
            case 130616:// Glyph of Fear effect
            case 117961:// Impervious Shield
            case 117837:// Delirious
            case 117697:// Shield of Darkness
                spellInfo->Dispel = DISPEL_MAGIC;
                break;
            case 113792:// Pyschic Horror - Psyfiend
                spellInfo->MaxAffectedTargets = 1;
                spellInfo->CastTimeEntry = sSpellCastTimesStore.LookupEntry(216); // 1.9s
                spellInfo->AttributesEx2 &= ~SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                spellInfo->AttributesEx6 &= ~SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                break;
            case 125972:// Felin Grace
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_SAFE_FALL;
                break;
            case 83968: // Mass Resurrect
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_DEAD;
                break;
            case 108507:// Kil'Jaeden's Cunning (passive)
                spellInfo->Effects[1].SpellClassMask[0] = 0x7C01487D;
                spellInfo->Effects[1].SpellClassMask[1] = 0x48022DF0;
                spellInfo->Effects[1].SpellClassMask[2] = 0x04000010;
                break;
            case 119049:// Kil'Jaeden's Cunning
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 121039:// Mana Attunement (400%)
                spellInfo->Effects[0].BasePoints = 50;
                break;
            case 116833:// Cosmetic Spirit Totem - Gara'Jal
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 130774:// Amethyst Pool - Periodic damage
                spellInfo->Attributes |= SPELL_ATTR0_CANT_CANCEL;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
            case 116782:// Titan Gas
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                spellInfo->Effects[0].BasePoints = 17000;
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(113); // radius 1000
                break;
            case 122336:
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(16); // radius 1.0 instead of 6.0
                spellInfo->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(16);
            case 123811:// Pheromones of Zeal
                spellInfo->Effects[0].TargetA = TARGET_UNIT_DEST_AREA_ENTRY;
                break;
            case 122706:// Noise Cancelling
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].BasePoints = 60;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].BasePoints = 75;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 122707:// Noise Cancelling
                spellInfo->Effects[0].BasePoints = 10;
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 122731:// Create Noise Cancelling AreaTrigger
                spellInfo->Effects[0].BasePoints = 351;
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
/*            case 122853:// Tempest Slash
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                spellInfo->Effects[0].BasePoints = urand(190000, 210000);
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_KNOCK_BACK;
                spellInfo->Effects[1].BasePoints = 75;
                spellInfo->Effects[1].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[1].MiscValue = 150;
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[2].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;*/
            case 122842:// Tempest Slash (summoning tornado)
                spellInfo->Effects[0].TargetB = TARGET_DEST_DEST_RIGHT;
                break;
            case 123633:
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].BasePoints = 0;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[0].Amplitude = 500;
                break;
            case 108503:// Grimoire of Sacrifice
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 119905:// Cauterize (Command Demon)
            case 119907:// Disarm (Command Demon)
            case 119909:// Whiplash (Command Demon)
            case 119910:// Spell Lock (Command Demon)
            case 119911:// Optical Blast (Command Demon)
            case 119913:// Fellash (Command Demon)
            case 119914:// Felstorm (Command Demon)
            case 119915:// Wrathstorm (Command Demon)
                spellInfo->AttributesEx6 &= ~SPELL_ATTR6_DONT_DISPLAY_COOLDOWN;
                break;
            case 119904:// Supplant Command Demon
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].Effect = 0;
                break;
            case 24259: // Spell Lock (Felhunter)
            case 115782:// Optical Blast (Observer)
                spellInfo->Speed = 80;
                break;
            case 108199:// Gorefiend's Grasp
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 49206: // Summon Gargoyle
                spellInfo->AttributesEx5 |= SPELL_ATTR5_SINGLE_TARGET_SPELL;
                break;
            case 45529: // Blood Tap
                spellInfo->Effects[0].Effect = SPELL_EFFECT_ACTIVATE_RUNE;
                spellInfo->Effects[0].MiscValueB = 3;
                break;
            case 82691: // Ring of Frost
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                break;
            case 91021: // Find Weakness
                spellInfo->Effects[0].BasePoints = 100;
                break;
            case 76577: // Smoke Bomb
                spellInfo->SpellVisual[1] = 20733;
                break;
            case 118699:// Fear Effect
                spellInfo->Dispel = DISPEL_MAGIC;
                spellInfo->AttributesEx3 &= ~SPELL_ATTR3_IGNORE_HIT_RESULT;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_FEAR;
                break;
            case 124991:// Nature's Vigil (Damage)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 124988:// Nature's Vigil (Heal)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 122355:// Molten Core
                spellInfo->StackAmount = 255;
                break;
            case 6203:  // Soulstone
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_DEAD;
                break;
            case 106707:// Faerie Swarm (talent)
                spellInfo->AttributesEx8 |= SPELL_ATTR8_AURA_SEND_AMOUNT;
                break;
            case 118291:// Greater Fire Elemental
            case 118323:// Greater Earth Elemental
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SUMMON_PET;
                break;
            case 114942:// Healing Tide
                spellInfo->MaxAffectedTargets = 5;
                break;
            case 108283:// Echo of the Elements
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 62099: // Shamanism
                spellInfo->Effects[0].SpellClassMask[2] |= 0x8000;
                break;
            case 116943:// Earthgrab
                spellInfo->AttributesEx5 |= SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 6474:  // Earthbind Totem
            case 8145:  // Tremor Totem effect
                spellInfo->AttributesEx5 |= SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            case 2484:  // Earthbind Totem
                spellInfo->OverrideSpellList.push_back(51485);
                break;
            case 6544:  // Heroic Leap
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[2].TargetB = 0;
                spellInfo->Effects[2].BasePoints = 0;
                break;
            case 116198:// Enfeeblement Aura
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[1].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[2].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[4].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[4].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 116202:// Elements Aura
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[2].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[2].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 853:   // Hammer of Justice
                spellInfo->OverrideSpellList.push_back(105593); // Replace Hammer of Justice by Fist of Justice
                break;
            case 94339: // Fungal Area
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(18); // 20s
                break;
            case 81282: // Fungal Growth
                spellInfo->Effects[0].BasePoints = 100;
                break;
            case 6785:  // Ravage
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET;
                spellInfo->OverrideSpellList.push_back(102545); // Replace Ravage by Ravage (Incarnation)
                break;
            case 5212:  // Prowl
                spellInfo->OverrideSpellList.push_back(102547); // Replace Prowl by Prowl (Incarnation)
                break;
            case 16914: // Hurricane
                spellInfo->OverrideSpellList.push_back(106996); // Replace Hurricane by Astral Storm
                break;
            case 755:   // Health Funnel
                spellInfo->OverrideSpellList.push_back(108415); // Replace Health Funnel by Soul Link
                break;
            case 85673: // Word of Glory
                spellInfo->OverrideSpellList.push_back(114163); // Replace World of glory by Eternal Flame
                break;
            case 114163:// Eternal Flame
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 974:   // Earth Shield
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_HEALING_RECEIVED;
                break;
            case 86529: // Mail Specialization (Shaman)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 61999: // Raise Ally
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ALLY;
                break;
            case 31935: // Avenger's Shield
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MAGIC;
                break;
            case 498:   // Divine Protection
            case 30823: // Shamanistic Rage
            case 51490: // Thunderstorm
            case 108416:// Sacrificial Pact
            case 137562:// Nimble Brew
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                break;
            case 115611:// Temporal Ripples
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
            case 115610:// Temporal Shield
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                break;
            case 81333: // Might of the Frozen Wastes
                spellInfo->Effects[1].BasePoints = 30;
                break;
            case 44203: // Tranquility (triggered)
                spellInfo->MaxAffectedTargets = 5;
                break;
            case 121118:// Dire Beast summons
            case 122802:
            case 122804:
            case 122806:
            case 122807:
            case 122809:
            case 122811:
            case 126213:
            case 126214:
            case 126215:
            case 126216:
            case 132764:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 19574: // Bestial Wrath
                spellInfo->Effects[3].Effect = 0;
                spellInfo->Effects[3].ApplyAuraName = 0;
                break;
            case 87935: // Serpent Spread
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(21); // -1s
                break;
            case 53257: // Cobra Strikes
                spellInfo->Effects[0].BasePoints = 0;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_CRIT_PCT;
                break;
            case 21562: // Power Word : Fortitude
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 1126:  // Mark of the Wild
            case 19740: // Blessing of Might
            case 20217: // Blessing of Kings
            case 24907: // Moonkin Aura
            case 109773:// Dark Intent
            case 116781:// Legacy of the White Tiger
            case 1459:  // Arcane Illumination
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                break;
            case 61316: // Dalaran Illumination
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_CASTER_AREA_RAID;
                break;
            case 11958: // Cold Snap
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_CONFUSED;
                break;
            case 112023:// Sparring (dummy)
                spellInfo->AuraInterruptFlags = 0x0;
                spellInfo->ChannelInterruptFlags = 0x0;
                spellInfo->InterruptFlags = 0x0;
                break;
            case 116033:// Sparring (stacks)
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 115073:// Spinning Fire Blossom
                spellInfo->Effects[0].TargetA = 0;
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[0].BasePoints = 0;
                spellInfo->OverrideSpellList.push_back(123408); // Add Spinning Fire Blossom (Overrided by Glyph) to override spell list of Spinning Fire Blossom
                break;
            case 86698: // Guardian of Ancient Kings
                spellInfo->Effects[0].TargetA = TARGET_CHECK_ENTRY;
                break;
            case 86674: // Ancient Healer
                spellInfo->ProcCharges = 5;
                break;
            case 86657: // Ancient Guardian
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                break;
            case 5782:  // Fear
                spellInfo->Dispel = DISPEL_MAGIC;
                spellInfo->Mechanic = 0;
                spellInfo->Effects[0].Mechanic = MECHANIC_NONE;
                spellInfo->OverrideSpellList.push_back(104045); // Add Sleep (Metamorphosis)
                break;
            case 51460: // Runic Corruption
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_POWER_REGEN_PERCENT;
                spellInfo->Effects[EFFECT_0].MiscValue = 5;
                spellInfo->Effects[EFFECT_0].MiscValueB = NUM_RUNE_TYPES;
                break;
            case 45204: // Mirror Image - Clone Me!
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 11371: // Arthas's Gift
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 41055: // Copy Weapon Spells
            case 45206:
            case 63416:
            case 69891:
            case 69892:
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Mechanic = 0;
                break;
            case 20066: // Repentence
            case 115175:// Soothing Mists
            case 116694:// Surging Mists
            case 117952:// Crackling Jade Lightning
                spellInfo->PreventionType = SPELL_PREVENTION_TYPE_SILENCE;
                break;
            case 117833:// Crazy Thought
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                spellInfo->PreventionType = SPELL_PREVENTION_TYPE_SILENCE;
                break;
            case 102793:
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_DECREASE_SPEED;
                break;
            case 172:   // Corruption
            case 30108: // Unstable Affliction
                spellInfo->Effects[2].Effect = 0;
                spellInfo->Effects[2].ApplyAuraName = 0;
                spellInfo->Effects[2].BasePoints = 0;
                break;
            case 34433: // Shadowfiend
                spellInfo->Effects[EFFECT_0].MiscValueB = 1561;
                spellInfo->OverrideSpellList.push_back(123040); // Add Mindbender to override spell list of Shadowfiend
                break;
            case 64904: // Hymn of Hope
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT;
                break;
            case 59907: // Lightwell Charges
                spellInfo->Effects[0].BasePoints = 15;
                spellInfo->ProcCharges = 15;
                break;
            case 126135:// Lightwell
                spellInfo->OverrideSpellList.push_back(724); // Add old Lightwell to override list
                break;
            case 81751: // Atonement
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 108201:// Desecrated Ground
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_CONFUSED;
                spellInfo->AttributesEx |= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 9005:  // Pounce
                spellInfo->OverrideSpellList.push_back(102546); // Replace Pounce by Pounce (Incarnation)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(35); // 4s
                break;
            case 102359:// Mass Entanglement
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                break;
            case 33763: // Lifebloom
            case 102355:// Faerie Swarm
                spellInfo->AttributesEx5 |= SPELL_ATTR5_SINGLE_TARGET_SPELL;
                break;
            case 3411:  // Intervene
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                spellInfo->AttributesEx7 |= SPELL_ATTR7_HAS_CHARGE_EFFECT;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_RAID;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_CHARGE;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_ADD_CASTER_HIT_TRIGGER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_RAID;
                spellInfo->Effects[1].TargetB = 0;
                spellInfo->OverrideSpellList.push_back(114029); // Add Safeguard to override spell list of Intervene
                break;
            case 1784:  // Stealth
                spellInfo->OverrideSpellList.push_back(115191); // Add Stealth (talent) to override spell list of Stealth
                break;
            case 115191:// Subterfuge
                spellInfo->AttributesEx |= SPELL_ATTR0_DISABLED_WHILE_ACTIVE;
                spellInfo->AttributesEx8 |= SPELL_ATTR8_AURA_SEND_AMOUNT;
                spellInfo->ProcFlags = 0x00A22A8;   // 1784 ProcsFlags
                break;
            case 115192:// Subterfuge
                spellInfo->Attributes |= SPELL_ATTR0_DONT_AFFECT_SHEATH_STATE;
                spellInfo->Attributes |= SPELL_ATTR0_NOT_SHAPESHIFT;
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 84745: // Shallow Insight
            case 84746: // Moderate Insight
            case 84747: // Deep Insight
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
                spellInfo->Effects[0].MiscValue = SPELL_SCHOOL_MASK_ALL;
                break;
            case 121733:// Throw
                spellInfo->OverrideSpellList.push_back(114014); // Add Shuriken Toss to override spell list of Throw
                break;
            case 105361:// Seal of Command
                spellInfo->OverrideSpellList.push_back(31801); // Replace Seal of Command with Seal of Truth
                break;
            case 117895:// Eminence (statue)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 115294:// Mana Tea
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); // 1s
                break;
            case 44457: // Living Bomb
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 12051: // Evocation
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[1].ApplyAuraName = 0;
                break;
            case 23691: // Berzerker Rage Effect
                spellInfo->Effects[0].BasePoints = 100;
                break;
            case 114205:// Demoralizing Banner
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(8); // 15s
                break;
            case 127630:// Cascade - damage trigger
            case 120786:// Cascade - heal trigger
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 122128:// Divine Star (shadow)
            case 110745:// Divine Star (other)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(29);
                spellInfo->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(29);
                break;
            case 12975:// Last Stand
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 122507:// Rallying Cry
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1);
                break;
            case 106922:// Might of Ursoc
            case 113072:// Might of Ursoc (Symbiosis)
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 117828:// Backdraft
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->Effects[0].MiscValue = SPELLMOD_CASTING_TIME;
                spellInfo->Effects[0].BasePoints = -30;
                spellInfo->Effects[0].SpellClassMask[1] |= 0x2000;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->Effects[1].MiscValue = SPELLMOD_COST;
                spellInfo->Effects[1].BasePoints = -30;
                spellInfo->Effects[1].SpellClassMask[1] |= 0x80;
                spellInfo->Effects[1].SpellClassMask[0] |= 0x1000;
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->Effects[2].MiscValue = SPELLMOD_CASTING_TIME;
                spellInfo->Effects[2].BasePoints = -30;
                spellInfo->Effects[2].SpellClassMask[1] |= 0x80;
                spellInfo->Effects[2].SpellClassMask[0] |= 0x1000;
                break;
            case 109259:// Powershot
                spellInfo->Effects[1].BasePoints = 70;
                spellInfo->Effects[2].BasePoints = 90;
                spellInfo->Effects[2].Effect = SPELL_EFFECT_WEAPON_PERCENT_DAMAGE;
                break;
            case 82926: // Fire ! (for Master Marksman)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->Effects[0].MiscValue = SPELLMOD_CASTING_TIME;
                spellInfo->Effects[0].BasePoints = -100;
                spellInfo->Effects[0].SpellClassMask[0] |= 0x20000;
                break;
            case 7384: // Overpower
                spellInfo->AttributesEx |= SPELL_ATTR1_ENABLE_AT_DODGE;
                break;
            case 114695:// Pursuit of Justice
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 6262:  // Healthstone
                spellInfo->Effects[0].Effect = SPELL_EFFECT_HEAL_PCT;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_OBS_MOD_HEALTH;
                spellInfo->Effects[1].BasePoints = 4;
                break;
            case 56224: // Glyph of Healthstone
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 34299: // Leader of the pack - healing
            case 81280: // Blood Burst
                spellInfo->Effects[0].Effect = SPELL_EFFECT_HEAL_PCT;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 90259: // Glyph of Frost Pillar (Root Aura)
                spellInfo->Effects[0].MiscValue = 0;
                spellInfo->Effects[0].MiscValueB = 0;
                break;
            case 49821: // Mind Sear
                spellInfo->Effects[0].TargetA = TARGET_DEST_CHANNEL_TARGET;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 10326: // Turn Evil
                spellInfo->Mechanic = MECHANIC_TURN;
                spellInfo->OverrideSpellList.push_back(145067); // Evil is a point of view
                break;
            case 113656:// Fists of Fury
                spellInfo->PreventionType = SPELL_PREVENTION_TYPE_SILENCE;
                break;
            case 115315:// Summon Black Ox Statue
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 13165: // Aspect of the Hawk
                spellInfo->OverrideSpellList.push_back(109260); // Add Aspect of the Iron Hack to override spell list of Aspect of the Hawk
                spellInfo->Effects[0].BasePoints = 35;
                break;
            case 6346:  // Fear Ward
            case 110717:// Fear Ward (Symbiosis)
            case 48108: // Hot Streak
            case 57761: // Brain Freeze
            case 132158:// Nature's Swiftness
            case 74434: // Soul Burn
            case 34936: // Backlash
            case 23920: // Spell Reflection
            case 113002:// Spell Reflection (Symbiosis)
            case 124430:// Divine Insight (Shadow)
            case 81292: // Glyph of Mind Spike
            case 114250:// Selfless Healer
            case 90174: // Divine Purpose
            case 131567:// Holy Spark
            case 69369: // Predator Swiftness
            case 113853:// Blazing Speed aurastate
            case 114028:// Mass Spell Reflection
            case 145151:// Dream of Cenarius (Balance)
                spellInfo->ProcCharges = 1;
                break;
            case 110600:// Ice Trap (Symbiosis)
                spellInfo->Effects[0].MiscValue = 164639;
                break;
            case 110588:// Misdirection (Symbiosis)
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_MOD_SCALE;
                spellInfo->Effects[2].BasePoints = 30;
                break;
            case 111546:
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_NONE;
                break;
            case 113890:
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 113886:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 122292:// Intervene (Symbiosis)
                spellInfo->Effects[1].BasePoints = 100;
                break;
            case 6358:  // Seduce (succubus)
            case 115268:// Mesmerize
                spellInfo->SpellFamilyName = SPELLFAMILY_WARLOCK;
                break;
            case 980:   // Agony
                spellInfo->StackAmount = 10;
                break;
            case 131740:// Corruption (Malefic Grasp)
            case 131736:// Unstable Affliction (Malefic Grasp)
            case 132566:// Seed of Corruption (Malefic Grasp)
            case 131737:// Agony (Malefic Grasp)
            case 42463: // Seal of Truth
            case 124464:// Shadow Word: Pain (Mastery instant damage)
            case 124465:// Vampiric Touch (Mastery instant damage)
            case 124467:// Devouring Plague (Mastery instant damage)
            case 124468:// Mind Flay (Mastery instant damage)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 131116:// Allow to use Raging Blow
                spellInfo->StackAmount = 2;
                break;
            case 44544: // Fingers of Frost
            case 126084:// Fingers of Frost - visual
                spellInfo->StackAmount = 2;
                break;
            case 85222: // Light of Dawn
                spellInfo->MaxAffectedTargets = 6;
                break;
            case 2641:  // Dismiss Pet
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_DEAD;
                break;
            case 52042: // Healing Stream - Totem
                spellInfo->Effects[0].Effect = SPELL_EFFECT_HEAL;
                spellInfo->Effects[0].BasePoints = 31;
                break;
            case 324:   // Lightning Shield
                spellInfo->ProcCharges = 0;
                break;
            case 116740:// Tigereye Brew
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1);
                break;
            case 117993:// Chi Torpedo : Heal
            case 124040:// Chi Torpedo : Damage
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(9);
                break;
            case 80325: // Camouflage
            case 119450:// Glyph of Camouflage
                spellInfo->Effects[1].Effect = 0;
                break;
            case 115008:// Chi Torpedo
                spellInfo->OverrideSpellList.push_back(121828); // Override List with Chi Torpedo - Talent
                break;
            case 109132:// Roll
                spellInfo->OverrideSpellList.push_back(115008); // Override List with Chi Torpedo
                spellInfo->OverrideSpellList.push_back(121827); // Override List with Roll - Talent
                break;
            case 115295:// Guard
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_HEALING_DONE_PERCENT;
                spellInfo->Effects[1].BasePoints = 30;
                break;
            case 124273:// Heavy Stagger
            case 124274:// Moderate Stagger
            case 124275:// Light Stagger
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_PERIODIC_DAMAGE;
                spellInfo->Effects[0].Amplitude = 1000;
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 108937:// Baby Elephant Takes a Bath
                spellInfo->Effects[1].BasePoints = 40;
                spellInfo->Effects[1].MiscValue = 100;
                break;
            case 126451:// Clash - Impact
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(29);
                break;
            case 121253:// Keg Smash
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 115308:// Elusive Brew
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1);
                break;
            case 115129:// Expel Harm - Damage to a nearby ennemy within 10 yards
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_NEARBY_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                break;
            case 126892:// Zen Pilgrimage
            case 126895:// Zen Pilgrimage : Return
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 130320:// Rising Sun Kick - Monks abilities deal 10% more damage
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);
                break;
            case 101546:// Spinning Crane Kick - Decrease Speed
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_DECREASE_SPEED;
                spellInfo->Effects[1].BasePoints = -30;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);
                spellInfo->OverrideSpellList.push_back(116847);
                break;
            case 125084:// Charging Ox Wave
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); // radius 30
                break;
            case 107270:// Spinning Crane Kick - Radius
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(14);
                break;
            case 107223:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 106909:
                {
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(113379);
                    if (!spell)
                        break;
                    spellInfo->DurationEntry = spell->DurationEntry;
                }
                break;
            case 113315:
                {
                    SpellInfo const* spell = sSpellMgr->GetSpellInfo(113379);
                    if (!spell)
                        break;
                    spellInfo->DurationEntry = spell->DurationEntry;
                }
                break;
            case 106736:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 106113:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 119922: //Shockwave
            case 119929:
            case 119930:
            case 119931:
            case 119932:
            case 119933:
                spellInfo->Speed = 5.f;
                break;
            case 106112:
                {
                    const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(22);
                    if (!radius)
                        break;

                    spellInfo->Effects[0].RadiusEntry = radius; //200yards.
                }
                break;
            case 106847:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                // Wise Mari Hydrolance damage
            case 106267:
                //spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 106334:// Wash Away
                spellInfo->AttributesEx3 &= ~ SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                break;
            case 120552:// Mantid Munition Explosion
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(16);
                break;
            case 119684:// Ground Slam
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 106853:// Fists of Fury
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 112060:// Apparitions
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 118685:// Battering Headbutt
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(5);
                break;
            case 60670: // Malygos Enrage
                spellInfo->Effects[1].TriggerSpell = 0;
                spellInfo->Effects[2].TriggerSpell = 0;
                break;
            case 114746:// Snatch Master's Flame
                spellInfo->Effects[2].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[2].TargetB = 0;
                break;
            case 104855:// Overpacked Firework
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            // Add Server-Side dummy spell for Fishing
            // TODO : Add more generic system to load server-side spell
            case 7733:  // Apprentice Fishing
            case 7734:  // Journeyman Fishing
            case 18249: // Artisan Fishing
            case 54083: // Expert Fishing
            case 54084: // Master Fishing
            case 51293: // Grand Master Fishing
            case 88869: // Illustrious Grand Master Fishing
            case 110412:// Zen Master Fishing
            {
                SpellInfo* fishingDummy = new SpellInfo(sSpellStore.LookupEntry(131474), difficulty);
                fishingDummy->Id = spellInfo->Effects[0].TriggerSpell;
                mSpellInfoMap[difficulty][spellInfo->Effects[0].TriggerSpell] = fishingDummy;
                break;
            }
            // Mogu'shan Vault
            case 116000:// Voodoo Dolls
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 116161:// Crossed Over
                spellInfo->Effects[EFFECT_1].MiscValue = 2; // Set Phase to 2
                spellInfo->Effects[EFFECT_3].Effect    = 0; // No need to summon
                break;
            case 116272:// Banishment
                spellInfo->Effects[EFFECT_0].MiscValue = 2; // Set Phase to 2
                break;
            case 116606:// Troll Rush
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 118303:// Fixate
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 15850: // Chilled
            case 16927: // Chilled
            case 20005: // Chilled
                spellInfo->Mechanic = MECHANIC_SNARE;
                break;
            case 89640: // Water Spirit
                spellInfo->Effects[EFFECT_0].BasePoints = 5000;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 130649:// Life Spirit
                spellInfo->Effects[EFFECT_0].BasePoints = 20000;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 127362:
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            // Isle of Conquest Gunship Portal
            case 66630:
            case 66637:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 66550: // teleports outside (Isle of Conquest)
            case 66551: // teleports inside (Isle of Conquest)
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            // Tree of Life
            case 65139:
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[1].ApplyAuraName = 0;
                break;
            case 60206: // Ram
                spellInfo->Effects[2].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                break;
            case 70890: // Scourge Strike triggered part
            case 96172: // Hand of Light
            case 101085: // Wrath of Tarecgosa
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                break;
            case 20711: // Spirit of Redemption
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 89485: // Inner Focus
                spellInfo->ProcChance = 100;
                spellInfo->ProcCharges = 1;
                spellInfo->ProcFlags = PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;
                spellInfo->Effects[1].SpellClassMask = spellInfo->Effects[0].SpellClassMask;
                break;
            case 54785: // Demonic Leap
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_NONE;
                spellInfo->Attributes |= SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(43);
                break;
            case 5176:  // Wrath
            case 2912:  // Starfire
            case 78674: // Starsurge
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[EFFECT_1].Mechanic = MECHANIC_NONE;
                break;
            case 33891:  // Tree form
            case 114282: // Tree form
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(9);
                break;
            // Chain Lightning Elemental Overload proc
            case 45297:
                spellInfo->MaxLevel = spellInfo->SpellLevel;
                break;
            case 34490: // Silencing Shot
            case 147362:// Counter Shot
                spellInfo->Speed = 0;
                break;
            case 132626: // Alliance Portal - Mage
                spellInfo->AttributesEx7 |= SPELL_ATTR7_ALLIANCE_ONLY;
                break;
            case 132620: // Horde Portal - Mage
                spellInfo->AttributesEx7 |= SPELL_ATTR7_HORDE_ONLY;
                break;
            // Magic Barrier, Lady Vashj
            case 38112:
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 36819: // Pyroblast (Kael'thas)
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            // Snowball
            case 25677:
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 54402: // Land Mine Knockback
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_DEST_AREA_ALLY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_DEST_AREA_ALLY;
                break;
            // Improved Frost Presence (Rank 1/2)
            case 50384:
            case 50385:
                spellInfo->Effects[EFFECT_1].SpellClassMask = 0;
                break;
            case 81262: // Efflorensence
            case 88685: // Holy World: Sanctuary
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            // Detonate Mana, Tyrande's Favorite Doll
            case 92601:
                spellInfo->CasterAuraSpell = 92596;
                break;
            // Shard of the Defiler - Echo of Archimonde
            case 21079:
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_2].Effect = 0;
                spellInfo->Effects[EFFECT_2].ApplyAuraName = 0;
                break;
            // Burn Hay, Braizer Torch
            case 88646:
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].TargetA = 0;
                spellInfo->Effects[EFFECT_1].TargetB = 0;
                break;
            // Cauterizing Flame
            case 99152:
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            // Moonwell Chalice
            case 100403:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            // Arest, Sturdy Manacles, Seize the Ambassador
            case 78628:
                spellInfo->Effects[EFFECT_0].Effect = 0;
                break;
            // Flamebreaker, Flameseer's Staff, Flamebreaker quest
            case 75206:
                spellInfo->Effects[EFFECT_0].Effect = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                break;
            // Summon Unbound Flamesparks, Flameseer's Staff, Flamebreaker quest
            case 74723:
                spellInfo->Effects[EFFECT_0].MiscValue = 40065;
                break;
            // Inflate Air Balloon, Undersea Inflation quest
            case 75346:
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_KILL_CREDIT;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_0].MiscValue = 40399;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                break;
            // Hercular's Rod
            case 89821:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            // Fortune Cookie
            case 87604:
                spellInfo->Effects[EFFECT_2].Effect = SPELL_EFFECT_SCRIPT_EFFECT;
                break;
            // Heart's Judgment, Heart of Ignacious trinket
            case 91041:
                spellInfo->CasterAuraSpell = 91027;
                break;
            // Heart's Judgment, Heart of Ignacious trinket (heroic)
            case 92328:
                spellInfo->CasterAuraSpell = 92325;
                break;
            case 53182:
            case 53183:
            case 53184:
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                break;
            // Glyph of Fear
            case 56244:
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER;
                spellInfo->Effects[EFFECT_0].BasePoints = 5000;
                spellInfo->Effects[EFFECT_0].MiscValue = SPELLMOD_COOLDOWN;
                break;
            // Eclipse markers
            case 67484:
            case 67483:
            case 48517:
            case 48518:
                spellInfo->AuraInterruptFlags = 0;
                spellInfo->Attributes |= SPELL_ATTR0_CANT_CANCEL;
                break;
            // Cheat Death
            case 45182:
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                break;
            //
            // RUBY SANCTUM SPELLS
            //
            case 74769: // Twilight Cutter
            case 77844: // Twilight Cutter
            case 77845: // Twilight Cutter
            case 77846: // Twilight Cutter
                {
                    const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(12);
                    if (!radius)
                        break;

                    spellInfo->Effects[0].RadiusEntry = radius; //200yards.
                }
                break;
            case 75509: // Twilight Mending
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 75888: // Awaken Flames
            case 75889: // Awaken Flames
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            case 74799: // Soul Consumption
                {
                    const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(32);
                    if (!radius)
                        break;

                    spellInfo->Effects[1].RadiusEntry = radius; //200yards.
                }
                break;
            case 74802: // Consumption (10man normal)
            case 75874: // Consumption (25man normal)
            case 74630: // Combustion (10man normal)
            case 75882: // Combustion (25man normal)
                {
                    const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(22);
                    if (!radius)
                        break;

                    spellInfo->Effects[0].RadiusEntry = radius; //200yards.
                }
                break;
            case 75875: // Consumption (10man heroic)
            case 75876: // Consumption (25man heroic)
            case 75883: // Combustion (10man heroic)
            case 75884: // Combustion (25man heroic)
                {
                    const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(22);
                    if (!radius)
                        break;

                    spellInfo->Effects[0].RadiusEntry = radius; //200yards.
                    spellInfo->Effects[1].RadiusEntry = radius; //200yards.
                }
                break;
            break;
            case 74562: // Fiery Combustion
            case 74792: // Soul Consumption
                spellInfo->AttributesEx |= (SPELL_ATTR1_CANT_BE_REFLECTED|SPELL_ATTR1_CANT_BE_REDIRECTED);
                break;
            // Bad Manner
            case 90337:
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            // ENDOF RUBY SANCTUM SPELLS
            // EYE OF ETERNITY SPELLS
            // All spells below work even without these changes. The LOS attribute is due to problem
            // from collision between maps & gos with active destroyed state.
            case 57473: // Arcane Storm bonus explicit visual spell
            case 57431: // Summon Static Field
            case 56091: // Flame Spike (Wyrmrest Skytalon)
            case 56092: // Engulf in Flames (Wyrmrest Skytalon)
            case 57090: // Revivify (Wyrmrest Skytalon)
            case 57143: // Life Burst (Wyrmrest Skytalon)
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            // This would never crit on retail and it has attribute for SPELL_ATTR3_NO_DONE_BONUS because is handled from player, 
            // until someone figures how to make scions not critting without hack and without making them main casters this should stay here. 
            case 63934: // Arcane Barrage (casted by players and NONMELEEDAMAGELOG with caster Scion of Eternity (original caster)). 
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT; 
                break;
            // ENDOF EYE OF ETERNITY SPELLS
            //
            // ICECROWN CITADEL SPELLS
            //
            // THESE SPELLS ARE WORKING CORRECTLY EVEN WITHOUT THIS HACK
            // THE ONLY REASON ITS HERE IS THAT CURRENT GRID SYSTEM
            // DOES NOT ALLOW FAR OBJECT SELECTION (dist > 333)
            case 70781: // Light's Hammer Teleport
            case 70856: // Oratory of the Damned Teleport
            case 70857: // Rampart of Skulls Teleport
            case 70858: // Deathbringer's Rise Teleport
            case 70859: // Upper Spire Teleport
            case 70860: // Frozen Throne Teleport
            case 70861: // Sindragosa's Lair Teleport
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DB;
                break;
            case 69055: // Saber Lash (Lord Marrowgar)
            case 70814: // Saber Lash (Lord Marrowgar)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_5_YARDS); // 5yd
                break;
            case 69075: // Bone Storm (Lord Marrowgar)
            case 70834: // Bone Storm (Lord Marrowgar)
            case 70835: // Bone Storm (Lord Marrowgar)
            case 70836: // Bone Storm (Lord Marrowgar)
            case 72864: // Death Plague (Rotting Frost Giant)
            case 71160: // Plague Stench (Stinky)
            case 71161: // Plague Stench (Stinky)
            case 71123: // Decimate (Stinky & Precious)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_100_YARDS); // 100yd
                break;
            case 72378: // Blood Nova (Deathbringer Saurfang)
            case 73058: // Blood Nova (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                break;
            case 72385: // Boiling Blood (Deathbringer Saurfang)
            case 72441: // Boiling Blood (Deathbringer Saurfang)
            case 72442: // Boiling Blood (Deathbringer Saurfang)
            case 72443: // Boiling Blood (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                break;
            case 72769: // Scent of Blood (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                // no break
            case 72771: // Scent of Blood (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                break;
            case 72723: // Resistant Skin (Deathbringer Saurfang adds)
                // this spell initially granted Shadow damage immunity, however it was removed but the data was left in client
                spellInfo->Effects[EFFECT_2].Effect = 0;
                break;
            case 70460: // Coldflame Jets (Traps after Saurfang)
                spellInfo->SetDurationIndex(1); // 10 seconds
                break;
            case 71412: // Green Ooze Summon (Professor Putricide)
            case 71415: // Orange Ooze Summon (Professor Putricide)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 71159: // Awaken Plagued Zombies
                spellInfo->SetDurationIndex(21);
                break;
            case 71127: // Mortal Wound  
                spellInfo->AttributesEx3 |= SPELL_ATTR3_IGNORE_HIT_RESULT;
                break;
            case 70530: // Volatile Ooze Beam Protection (Professor Putricide)
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA; // for an unknown reason this was SPELL_EFFECT_APPLY_AREA_AURA_RAID
                break;
            case 69508: // Slime Spray
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            // THIS IS HERE BECAUSE COOLDOWN ON CREATURE PROCS IS NOT IMPLEMENTED
            case 71604: // Mutated Strength (Professor Putricide)
            case 72673: // Mutated Strength (Professor Putricide)
            case 72674: // Mutated Strength (Professor Putricide)
            case 72675: // Mutated Strength (Professor Putricide)
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 72454: // Mutated Plague (Professor Putricide)
            case 72464: // Mutated Plague (Professor Putricide)
            case 72506: // Mutated Plague (Professor Putricide)
            case 72507: // Mutated Plague (Professor Putricide)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                break;
            case 70911: // Unbound Plague (Professor Putricide) (needs target selection script)
            case 72854: // Unbound Plague (Professor Putricide) (needs target selection script)
            case 72855: // Unbound Plague (Professor Putricide) (needs target selection script)
            case 72856: // Unbound Plague (Professor Putricide) (needs target selection script)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 71518: // Unholy Infusion Quest Credit (Professor Putricide)
            case 72934: // Blood Infusion Quest Credit (Blood-Queen Lana'thel)
            case 72289: // Frost Infusion Quest Credit (Sindragosa)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // another missing radius
                break;
            case 70232: // Empowered Blood
            case 70320: // Empowered Blood
                spellInfo->Effects[EFFECT_0].MiscValue = 127;
                spellInfo->Effects[EFFECT_0].MiscValueB = 127;
                break;
            case 71708: // Empowered Flare (Blood Prince Council)
            case 72785: // Empowered Flare (Blood Prince Council)
            case 72786: // Empowered Flare (Blood Prince Council)
            case 72787: // Empowered Flare (Blood Prince Council)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 71266: // Swarming Shadows
            case 72890: // Swarming Shadows
                spellInfo->AreaGroupId = 0; // originally, these require area 4522, which is... outside of Icecrown Citadel
                break;
            case 26022: // Pursuit of Justice (Rank 1)
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].BasePoints = 7;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 26023: // Pursuit of Justice (Rank 1)
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].BasePoints = 14;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 70602: // Corruption
            case 48278: // Paralyze
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 70715: // Column of Frost (visual marker)
                spellInfo->SetDurationIndex(32); // 6 seconds (missing)
                break;
            case 71085: // Mana Void (periodic aura)
                spellInfo->SetDurationIndex(9); // 30 seconds (missing)
                break;
            case 72015: // Frostbolt Volley (only heroic)
            case 72016: // Frostbolt Volley (only heroic)
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(EFFECT_RADIUS_40_YARDS);
                break;
            case 70936: // Summon Suppressor (needs target selection script)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 72706: // Achievement Check (Valithria Dreamwalker)
            case 71357: // Order Whelp
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);   // 200yd
                break;
            case 70598: // Sindragosa's Fury
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 69846: // Frost Bomb
                spellInfo->Speed = 0.0f;    // This spell's summon happens instantly
                break;
            case 71614: // Ice Lock
                spellInfo->Mechanic = MECHANIC_STUN;
                break;
            case 72762: // Defile
                spellInfo->SetDurationIndex(559); // 53 seconds
                break;
            case 72743: // Defile
                spellInfo->SetDurationIndex(22); // 45 seconds
                break;
            case 72754: // Defile
            case 73708: // Defile
            case 73709: // Defile
            case 73710: // Defile
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                break;
            case 69030: // Val'kyr Target Search
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                break;
            case 69198: // Raging Spirit Visual
                spellInfo->SetRangeIndex(13); // 50000yd
                break;
            case 73654: // Harvest Souls
            case 74295: // Harvest Souls
            case 74296: // Harvest Souls
            case 74297: // Harvest Souls
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                break;
            case 72546: // Harvest Soul
            case 72597: // Harvest Soul
            case 72608: // Harvest Soul
                spellInfo->AttributesEx7 |= SPELL_ATTR7_ZONE_TELEPORT;
                break;
            case 73655: // Harvest Soul
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->AttributesEx7 |= SPELL_ATTR7_ZONE_TELEPORT;
                break;
            case 73540: // Summon Shadow Trap
                spellInfo->SetDurationIndex(23); // 90 seconds
                break;
            case 73530: // Shadow Trap (visual)
                spellInfo->SetDurationIndex(28); // 5 seconds
                break;
            case 73529: // Shadow Trap
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_10_YARDS); // 10yd
                break;
            case 74282: // Shadow Trap (searcher)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_3_YARDS); // 3yd
                break;
            case 72595: // Restore Soul
            case 73650: // Restore Soul
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                break;
            case 74086: // Destroy Soul
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                break;
            case 74302: // Summon Spirit Bomb
            case 74342: // Summon Spirit Bomb
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 74341: // Summon Spirit Bomb
            case 74343: // Summon Spirit Bomb
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 73579: // Summon Spirit Bomb
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_25_YARDS); // 25yd
                break;
            case 72350: // Fury of Frostmourne
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_INSTAKILL;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_0].Amplitude = 50000;
                break;
            case 75127: // Kill Frostmourne Players
            case 72351: // Fury of Frostmourne
            case 72431: // Jump (removes Fury of Frostmourne debuff)
            case 72429: // Mass Resurrection
            case 73159: // Play Movie
            case 73582: // Trigger Vile Spirit (Inside, Heroic)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                break;
            case 72376: // Raise Dead
                spellInfo->MaxAffectedTargets = 3;
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); // 50000yd
                break;
            case 71809: // Jump
                spellInfo->SetRangeIndex(3); // 20yd
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_25_YARDS); // 25yd
                break;
            case 72405: // Broken Frostmourne
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); // 200yd
                break;
            case 68872: // Soulstorm (Bronjahm Encounter)
                spellInfo->InterruptFlags = 0;
                break;
            // ENDOF ICECROWN CITADEL SPELLS
            //
            // SHADOWFANG KEEP SPELLS
            // Baron Ashbury
            case 93720: // Wracking Pain
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                break;
            case 93810: // Calamity dmg
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                break;
            case 93423: // Asphyxiate
            case 93710:
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(28);
                break;
            case 93468: // Stay of Execution
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                break;
            case 93706: // Stay of Execution t
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                break;
            // Baron Silverlaine
            case 93956: // Cursed Veil
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(18);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(18);
                break;
            // Commander Springvale
            case 93722: // Shield of Perfidious dmg
            case 93737:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                break;
            // Lord Valden
            case 93697: // Conjure Poisonous Mixture
            case 93704:
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(18);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(18);
                break;
            case 93505: // Conjure Frost Mixture
            case 93702:
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(18);
                break;
            case 93617: // Toxic Coagulant dmg
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(12);
                break;
            case 93689: // Toxic Catalyst dmg
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(12);
                break;
            // Lord Godfrey
            case 96344: // Pistol Barrage sum
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(12);
                break;
            // ENDOF SHADOWFANG SPELLS
            //
            // DEADMINES SPELLS
            // Admiral Ripsnarl
            case 88736: // Taste for Blood
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                spellInfo->ProcChance = 0;
                spellInfo->ProcFlags = 0;
                break;
            case 95647: // Ripsnarl Achievement Aura
                spellInfo->AttributesEx3 = SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                break;
            // Captain Cookie
            case 89250: // Summon Cauldron
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 89268: // Throw Food Targeting
            case 89740: 
            case 90561:
            case 90562:
            case 90582:
            case 90583:
            case 90563:
            case 90584:
            case 90564:
            case 90585:
            case 90565:
            case 90586:
                spellInfo->MaxAffectedTargets = 1;
                spellInfo->AttributesEx3 = SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                break;
            // Vanessa Vancleef
            case 92620: // Backslash targeting
                spellInfo->MaxAffectedTargets =1;
                break;
            // ENDOF DEADMINES
            //
            // ULDUAR SPELLS
            //
            case 64014: // Expedition Base Camp Teleport
            case 64032: // Formation Grounds Teleport
            case 64028: // Colossal Forge Teleport
            case 64031: // Scrapyard Teleport
            case 64030: // Antechamber Teleport
            case 64029: // Shattered Walkway Teleport
            case 64024: // Conservatory Teleport
            case 64025: // Halls of Invention Teleport
            case 64027: // Descent into Madness Teleport
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DB;
                break;
            case 62374: // Pursued (Flame Leviathan)
                spellInfo->Effects[0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS);   // 50000yd
                break;
            case 62383: // Shatter (Ignis)
                spellInfo->SpellVisual[0] = 12639;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 63342: // Focused Eyebeam Summon Trigger (Kologarn)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 62716: // Growth of Nature (Freya)
            case 65584: // Growth of Nature (Freya)
            case 64381: // Strength of the Pack (Auriaya)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 63018: // Searing Light (XT-002)
            case 65121: // Searing Light (25m) (XT-002)
            case 63024: // Gravity Bomb (XT-002)
            case 64234: // Gravity Bomb (25m) (XT-002)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 62834: // Boom (XT-002)
            // This hack is here because we suspect our implementation of spell effect execution on targets
            // is done in the wrong order. We suspect that EFFECT_0 needs to be applied on all targets,
            // then EFFECT_1, etc - instead of applying each effect on target1, then target2, etc.
            // The above situation causes the visual for this spell to be bugged, so we remove the instakill
            // effect and implement a script hack for that.
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 64386: // Terrifying Screech (Auriaya)
            case 64389: // Sentinel Blast (Auriaya)
            case 64678: // Sentinel Blast (Auriaya)
                spellInfo->SetDurationIndex(28); // 5 seconds, wrong DBC data?
                break;
            case 64321: // Potent Pheromones (Freya)
                // spell should dispel area aura, but doesn't have the attribute
                // may be db data bug, or blizz may keep reapplying area auras every update with checking immunity
                // that will be clear if we get more spells with problem like this
                spellInfo->AttributesEx |= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 62301: // Cosmic Smash (Algalon the Observer)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 64598: // Cosmic Smash (Algalon the Observer)
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 62293: // Cosmic Smash (Algalon the Observer)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_DEST_CASTER;
                break;
            case 62311: // Cosmic Smash (Algalon the Observer)
            case 64596: // Cosmic Smash (Algalon the Observer)
                spellInfo->SetRangeIndex(6);  // 100yd
                break;
            // ENDOF ULDUAR SPELLS
            //
            case 54934: // Glyph of Exorcism
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 87426: // Shadow Apparition
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 73685: // Unleash Life 
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MAGIC;
                break;
            case 49560: // Death Grip
            case 49576:
                spellInfo->SchoolMask = SPELL_SCHOOL_MASK_SHADOW;
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MAGIC;
                spellInfo->Mechanic = MECHANIC_NONE;
                spellInfo->Effects[0].Mechanic = MECHANIC_NONE;
                break;
            case 114255:// Surge of Light (proc)
                spellInfo->StackAmount = 2;
                break;
            case 1543:  // Flare
                spellInfo->Effects[0].TriggerSpell = 94528;
                break;
            // Player Damage Reduction Level 90, we have S13, so we need to decrease to 65% of base resilience
            // @TODO: Remove this hack when we out S14
            case 142689:
                spellInfo->Effects[0].BasePoints = -2500;
                break;
            // Terrorize Player (tsulong spell)
            case 123011:
                spellInfo->MaxAffectedTargets = 1;
                break;
            default:
                break;
        }

        switch (spellInfo->Id)
        {
            case 19386: // Wyvern Sting
            case 2637: // Hibernate
            case 339: // Entangling Roots
            case 335: // Freezing Trap
            case 118: // Polymorph
            case 20066: // Repentance
            case 9484: // Shackle Undead
            case 2094: // Blind
            case 51514: // Hex
            case 76780: // Shackle Elemental
            case 710: // Banish
            case 6358: // Seduction
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                break;
            
            default:
                break;
        }

            switch (spellInfo->SpellFamilyName)
            {
                case SPELLFAMILY_SHAMAN:
                    if (spellInfo->Id == 60103)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                case SPELLFAMILY_WARRIOR:
                    // Shout
                    if (spellInfo->SpellFamilyFlags[0] & 0x20000 || spellInfo->SpellFamilyFlags[1] & 0x20)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                    // Dragon Roar
                    if (spellInfo->Id == 118000)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                case SPELLFAMILY_DRUID:
                    // Roar
                    if (spellInfo->SpellFamilyFlags[0] & 0x8)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                    // Rake
                    if (spellInfo->Id == 1822)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                default:
                    break;
            }

            // This must be re-done if targets changed since the spellinfo load
            spellInfo->ExplicitTargetMask = spellInfo->_GetExplicitTargetMask();

            switch (spellInfo->Id)
            {
                case 73680: // Unleash Elements
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_UNIT_ALLY;
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_UNIT_ENEMY;
                    break;
                case 107223:
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                   break;
                case 106736:
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                    break;
                case 106112:
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_DEST_LOCATION;
                    break;
                case 106113:
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                    break;
            }
        }
    }

    CreatureAI::FillAISpellInfo();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded spell custom attributes in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadAreaTriggerVisuals()
{
    uint32 oldMSTime = getMSTime();

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_AREATRIGGER_VISUAL);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Areatrigger Visuals. DB table `areatrigger_visual` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        mAreaTriggerVisuals.insert(std::make_pair(fields[0].GetUInt32(), fields[1].GetFloat()));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u AreaTrigger visuals in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadTalentSpellInfo()
{
    for (uint32 i = 0; i < sTalentStore.GetNumRows(); i++)
    {
        TalentEntry const* talent = sTalentStore.LookupEntry(i);
        if (!talent)
            continue;

        mTalentSpellInfo.insert(talent->spellId);
    }
}

const SpellInfo* SpellMgr::GetSpellInfo(uint32 spellId, Difficulty difficulty) const
{
    if (spellId < GetSpellInfoStoreSize())
    {
        if (mSpellInfoMap[difficulty][spellId])
            return mSpellInfoMap[difficulty][spellId];

        return mSpellInfoMap[NONE_DIFFICULTY][spellId];
    }

    return NULL;
}

void SpellMgr::LoadSpellPowerInfo()
{
    mSpellPowerInfo.resize(sSpellStore.GetNumRows());
    for (uint32 i = 0; i < sSpellPowerStore.GetNumRows(); i++)
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(i);
        if (!spellPower)
            continue;

        mSpellPowerInfo[spellPower->SpellId].push_back(spellPower->Id);
    }
}

SpellPowerEntry const* SpellMgr::GetSpellPowerEntryByIdAndPower(uint32 id, Powers power) const
{
    for (auto itr : GetSpellPowerList(id))
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(itr);
        if (!spellPower)
            continue;

        if (spellPower->powerType == power)
            return spellPower;
    }

    SpellInfo const* spell = sSpellMgr->GetSpellInfo(id);
    return spell->spellPower;
}
