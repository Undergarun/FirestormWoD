////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
#include "BattlegroundMgr.hpp"
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

DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellInfo const* spellproto, Unit* p_Caster)
{
    if (spellproto->IsPositive())
        return DIMINISHING_NONE;

    for (uint8 i = 0; i < spellproto->EffectCount; ++i)
    {
        if (spellproto->Effects[i].ApplyAuraName == SPELL_AURA_MOD_TAUNT)
            return DIMINISHING_TAUNT;
    }

    // Explicit Diminishing Groups
    switch (spellproto->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            // Entrapment -- 135373
            if (spellproto->SpellIconID == 20 && spellproto->SpellVisual[0] == 39588)
                return DIMINISHING_ROOT;

            // Intimidation -- 24394
            if (spellproto->SpellIconID == 166 && spellproto->SpellVisual[0] == 2816)
                return DIMINISHING_STUN;
            // Pulverize (Primal Earth Elemental) -- 118345
            if (spellproto->SpellIconID == 4507 && spellproto->SpellVisual[0] == 39877)
                return DIMINISHING_STUN;
            // Static Charge (Capacitor Totem) -- 118905
            if (spellproto->SpellIconID == 54 && spellproto->SpellVisual[0] == 24442)
                return DIMINISHING_STUN;
            // Remorseless Winter -- 115001
            if (spellproto->SpellIconID == 5744 && spellproto->SpellVisual[0] == 23514)
                return DIMINISHING_STUN;
            // War Stomp (Tauren Racial) -- 20549
            if (spellproto->Id == 20549)
                return DIMINISHING_STUN;

            // Quaking Palm (Pandaren Racial) -- 107079
            if (spellproto->Id == 107079)
                return DIMINISHING_INCAPACITATE;

            // Arcane Torrent (Blood Elf Racial) -- 25046 & 28730 & 50613 & 69179 & 80483 & 129596 & 155145
            if (spellproto->IsArcaneTorrent())
                return DIMINISHING_SILENCE;

            // Gorefiend's Grasp -- 108199
            if (spellproto->SpellIconID == 5743 && spellproto->SpellVisual[0] == 28937)
                return DIMINISHING_AOE_KNOCKBACK;
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            // Frostjaw -- 102051
            if (spellproto->SpellFamilyFlags[2] & 0x40000)
                return DIMINISHING_ROOT;

            // Frost Nova -- 122
            if (spellproto->SpellFamilyFlags[0] & 0x40)
                return DIMINISHING_ROOT;
            // Ice Ward -- 111340
            if (spellproto->SpellFamilyFlags[0] & 0x80000 && spellproto->SpellFamilyFlags[2] & 0x20000)
                return DIMINISHING_ROOT;
            // Freeze (Water Elemental) -- 33395
            if (spellproto->SpellFamilyFlags[2] & 0x200)
                return DIMINISHING_ROOT;

            // Deep Freeze -- 44572
            if (spellproto->SpellFamilyFlags[1] & 0x100000)
                return DIMINISHING_STUN;

            /// Dragon's Breath -- 31661
            if (spellproto->SpellFamilyFlags[0] & 0x800000)
                return DIMINISHING_DISORIENT;
            /// Polymorph -- 118
            if (spellproto->SpellFamilyFlags[0] & 0x1000000)
                return DIMINISHING_INCAPACITATE;
            // Ring of Frost -- 82691
            if (spellproto->SpellFamilyFlags[2] & 0x40)
                return DIMINISHING_INCAPACITATE;
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            /// Shockwave -- 132168
            if (spellproto->Id == 132168)
                return DIMINISHING_STUN;
            /// Storm Bolt -- 132169
            if (spellproto->Id == 132169)
                return DIMINISHING_STUN;
            /// Warbringer -- 7922
            if (spellproto->Id == 7922)
                return DIMINISHING_LIMITONLY;

            /// Intimidating Shout -- 5246
            if (spellproto->SpellFamilyFlags[0] & 0x40000)
                return DIMINISHING_DISORIENT;

            /// Hamstring -- 1715, 8 seconds in PvP (6.0)
            if (spellproto->SpellFamilyFlags[0] & 0x2)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            // Mortal Coil -- 6789
            if (spellproto->SpellFamilyFlags[0] & 0x80000)
                return DIMINISHING_INCAPACITATE;
            // Banish -- 710
            if (spellproto->SpellFamilyFlags[1] & 0x8000000)
                return DIMINISHING_INCAPACITATE;
            // Blood Horror -- 137143, no flags (17986)
            if (spellproto->SpellIconID == 5819 && spellproto->SpellVisual[0] == 30694)
                return DIMINISHING_INCAPACITATE;

            // Fear -- 118699
            if (spellproto->Id == 118699 || spellproto->Id == 130616)
                return DIMINISHING_DISORIENT;
            // Howl of Terror -- 5484
            if (spellproto->SpellFamilyFlags[1] & 0x8)
                return DIMINISHING_DISORIENT;

            // Shadowfury -- 30283
            if (spellproto->SpellFamilyFlags[1] & 0x1000)
                return DIMINISHING_STUN;
            // Summon Infernal -- 22703
            if (spellproto->Id == 22703)
                return DIMINISHING_STUN;

            // Debilitate (Terrorguard pet) -- 170996
            if (spellproto->Id == 170996)
                return DIMINISHING_ROOT;

            /// Chaos Wave -- 124915, slow effect
            if ((spellproto->SpellFamilyFlags[0] & 0x201000) == 0x201000)
                return DIMINISHING_NONE;
            break;
        }
        case SPELLFAMILY_WARLOCK_PET:
        {
            // Fellash -- 115770
            // Whiplash -- 6360
            if (spellproto->SpellFamilyFlags[0] & 0x8000000)
                return DIMINISHING_AOE_KNOCKBACK;

            // Mesmerize (Shivarra pet) -- 115268
            // Seduction (Succubus pet) -- 6358
            if (spellproto->SpellFamilyFlags[0] & 0x2000000)
                return DIMINISHING_DISORIENT;

            // Axe Toss (Felguard pet and Wrathguard pet) -- 89766
            if (spellproto->SpellFamilyFlags[1] & 0x4)
                return DIMINISHING_STUN;
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            // Maim -- 22570
            if (spellproto->SpellFamilyFlags[1] & 0x80)
                return DIMINISHING_STUN;
            // Mighty Bash -- 5211
            if (spellproto->SpellFamilyFlags[0] & 0x2000)
                return DIMINISHING_STUN;
            // Rake -- 163505 -- no flags on the stun, 20490
            if (spellproto->SpellIconID == 494 && spellproto->SpellVisual[0] == 38283)
                return DIMINISHING_STUN;

            // Incapacitating Roar -- 99, no flags on the stun, 14
            if (spellproto->SpellIconID == 960 && spellproto->SpellVisual[0] == 38528)
                return DIMINISHING_INCAPACITATE;

            // Cyclone -- 33786
            if (spellproto->SpellFamilyFlags[1] & 0x20)
                return DIMINISHING_DISORIENT;

            // Glyph of Fae Silence -- 114238, no flags on the silence, 15035
            if (spellproto->SpellIconID == 957 && spellproto->SchoolMask == 8)
                return DIMINISHING_SILENCE;

            // Typhoon -- 61391
            if (spellproto->SpellFamilyFlags[1] & 0x1000000)
                return DIMINISHING_AOE_KNOCKBACK;
            // Ursol's Vortex -- 118283
            if (spellproto->Id == 118283)
                return DIMINISHING_AOE_KNOCKBACK;

            // Entangling Roots -- 339 
            if (spellproto->SpellFamilyFlags[0] & 0x200)
                return DIMINISHING_ROOT;
            // Mass Entanglement -- 102359, no flags on the root, 13535
            if (spellproto->SpellIconID == 5782 && spellproto->SpellVisual[0] == 38269)
                return DIMINISHING_ROOT;
            // Balance Force of Nature Treant Entangling roots -- 113770
            if (spellproto->Id == 113770)
                return DIMINISHING_ROOT;
            // Nature's Grasp
            if (spellproto->Id == 170855)
                return DIMINISHING_ROOT;

            // Solar Beam -- 78675
            if (spellproto->Id == 78675)
                return DIMINISHING_SILENCE;

            // Faerie Fire -- 770, Faerie Swarm -- 102355, 20 seconds in PvP (6.0)
            if (spellproto->SpellFamilyFlags[0] & 0x400 || spellproto->SpellFamilyFlags[0] & 0x100)
                return DIMINISHING_LIMITONLY;
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            // Cheap Shot -- 1833
            if (spellproto->SpellFamilyFlags[0] & 0x400)
                return DIMINISHING_STUN;
            // Kidney Shot -- 408
            if (spellproto->SpellFamilyFlags[0] & 0x200000)
                return DIMINISHING_STUN;

            // Gouge -- 1776
            if (spellproto->SpellFamilyFlags[0] & 0x8)
                return DIMINISHING_INCAPACITATE;
            // Sap -- 6770
            if (spellproto->SpellFamilyFlags[0] & 0x80)
                return DIMINISHING_INCAPACITATE;

            // Blind -- 2094
            if (spellproto->SpellFamilyFlags[0] & 0x1000000)
                return DIMINISHING_DISORIENT;

            // Garrote -- 1330
            if (spellproto->SpellFamilyFlags[1] & 0x20000000)
                return DIMINISHING_SILENCE;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Glyph of Explosive Trap -- 119403
            if (spellproto->Id == 13812 && p_Caster->HasAura(119403))
                return DIMINISHING_AOE_KNOCKBACK;

            /// Entrapment
            if (spellproto->Id == 64803)
                return DIMINISHING_ROOT;
            // Charge (Tenacity pet) -- 53148, no flags (5526)
            if (spellproto->SpellIconID == 1559 && spellproto->SpellVisual[0] == 39480)
                return DIMINISHING_ROOT;

            /// Narrow Escape -- 136634
            if (spellproto->Id == 136634)
                return DIMINISHING_ROOT;

            // Binding Shot -- 117526, no flags (15581)
            if (spellproto->SpellIconID == 4612 && spellproto->SpellVisual[0] == 6859)
                return DIMINISHING_STUN;

            // Freezing Trap -- 3355
            if (spellproto->SpellFamilyFlags[0] & 0x8)
                return DIMINISHING_INCAPACITATE;
            // Wyvern Sting -- 19386
            if (spellproto->SpellFamilyFlags[1] & 0x1000)
                return DIMINISHING_INCAPACITATE;
            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            // Repentance -- 20066
            if (spellproto->SpellFamilyFlags[0] & 0x4)
                return DIMINISHING_INCAPACITATE;

            // Turn Evil -- 10326
            if (spellproto->SpellFamilyFlags[1] & 0x800000)
                return DIMINISHING_DISORIENT;
            // Blinding Light -- 115750
            if (spellproto->Id == 115750)
                return DIMINISHING_DISORIENT;

            // Avenger's Shield -- 31935
            if (spellproto->SpellFamilyFlags[0] & 0x4000)
                return DIMINISHING_SILENCE;

            // Fist of Justice -- 105593
            // Hammer of Justice -- 853
            if (spellproto->SpellFamilyFlags[0] & 0x800)
                return DIMINISHING_STUN;
            // Holy Wrath -- 119072
            if (spellproto->SpellFamilyFlags[1] & 0x200000)
                return DIMINISHING_STUN;
            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            // Hex -- 51514
            if (spellproto->SpellFamilyFlags[1] & 0x8000)
                return DIMINISHING_INCAPACITATE;

            // Thunderstorm -- 51490
            if (spellproto->SpellFamilyFlags[1] & 0x2000)
                return DIMINISHING_AOE_KNOCKBACK;
            // Earthgrab Totem -- 64695
            if (spellproto->SpellFamilyFlags[2] & 0x4000)
                return DIMINISHING_ROOT;
            // Frost Shock (with Frozen Power) -- 63685, no flags (6918)
            if (spellproto->SpellIconID == 193 && spellproto->SpellVisual[0] == 39876)
                return DIMINISHING_ROOT;
            /// Earthquake -- 77505
            if (spellproto->Id == 77505)
                return DIMINISHING_STUN;

            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            /// Necrotic Plague
            if (spellproto->Id == 155159)
                return DIMINISHING_LIMITONLY;
            // Strangulate -- 47476
            if (spellproto->SpellFamilyFlags[0] & 0x200)
                return DIMINISHING_SILENCE;

            // Chains of Ice (with Chilblains) -- 96294, no flags (13020)
            if (spellproto->SpellIconID == 180 && spellproto->SpellVisual[0] == 20135)
                return DIMINISHING_ROOT;

            // Asphyxiate -- 108194
            if (spellproto->SpellFamilyFlags[2] & 0x100000)
                return DIMINISHING_STUN;
            // Gnaw (Ghoul) -- 91800, no flags (12511)
            if (spellproto->SpellIconID == 3010 && spellproto->SpellVisual[0] == 38760)
                return DIMINISHING_STUN;
            // Monstrous Blow (Ghoul w/ Dark Transformation active) -- 91797, no flags (12510)
            if (spellproto->SpellIconID == 15 && spellproto->SpellVisual[0] == 38761)
                return DIMINISHING_STUN;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Glyph of Mind Blast -- 87194, no flags (10092)
            if (spellproto->SpellIconID == 2114 && spellproto->SpellVisual[0] == 38927)
                return DIMINISHING_ROOT;
            // Void Tendrils -- 114404, no flags (15067)
            if (spellproto->SpellIconID == 5816 && spellproto->SpellVisual[0] == 25199)
                return DIMINISHING_ROOT;

            // Dominate Mind -- 605
            if (spellproto->SpellFamilyFlags[0] & 0x20000 && spellproto->SpellVisual[0] == 39068)
                return DIMINISHING_INCAPACITATE;
            // Holy Word: Chastise -- 88625
            if (spellproto->SpellFamilyFlags[2] & 0x20)
                return DIMINISHING_INCAPACITATE;
            // Psychic Horror -- 64044
            if (spellproto->SpellFamilyFlags[2] & 0x2000)
                return DIMINISHING_INCAPACITATE;
            /// Shackle Undead -- 9484
            if (spellproto->Id == 9484)
                return DIMINISHING_INCAPACITATE;

            /// Sin and Punishment -- 87204
            if (spellproto->Id == 87204)
                return DIMINISHING_DISORIENT;

            // Psychic Scream -- 8122
            if (spellproto->SpellFamilyFlags[0] & 0x10000)
                return DIMINISHING_DISORIENT;

            // Silence -- 15487
            if (spellproto->SpellFamilyFlags[1] & 0x200000 && spellproto->SchoolMask == 32)
                return DIMINISHING_SILENCE;
            break;
        }
        case SPELLFAMILY_MONK:
        {
            // Disable -- 116706, no flags (15483)
            if (spellproto->SpellIconID == 23 && spellproto->SpellVisual[0] == 39984)
                return DIMINISHING_ROOT;

            // Charging Ox Wave -- 119392
            if (spellproto->SpellFamilyFlags[1] & 0x10000)
                return DIMINISHING_STUN;
            // Fists of Fury -- 120086
            if (spellproto->SpellFamilyFlags[1] & 0x800000 && !(spellproto->SpellFamilyFlags[2] & 0x8))
                return DIMINISHING_STUN;
            // Leg Sweep -- 119381
            if (spellproto->SpellFamilyFlags[1] & 0x200)
                return DIMINISHING_STUN;

            // Glyph of Breath of Fire -- 123393, no flags (16504)
            if (spellproto->SpellIconID == 15 && spellproto->SpellVisual[0] == 25408)
                return DIMINISHING_INCAPACITATE;
            // Paralysis -- 115078
            if (spellproto->SpellFamilyFlags[2] & 0x800000)
                return DIMINISHING_INCAPACITATE;
            // Ring of Peace -- 137460, no flags (18006)
            if (spellproto->SpellIconID == 7195 && spellproto->SpellVisual[0] == 39999)
                return DIMINISHING_INCAPACITATE;
            break;
        }
        default:
            break;
    }

    return DIMINISHING_NONE;
}

DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group)
{
    switch (group)
    {
        case DIMINISHING_TAUNT:
        case DIMINISHING_STUN:
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
        case DIMINISHING_AOE_KNOCKBACK:
            return DIMINISHING_LEVEL_2;
        default:
            return DIMINISHING_LEVEL_IMMUNE;
    }
}

int32 GetDiminishingReturnsLimitDuration(SpellInfo const* spellproto)
{
    // Explicit diminishing duration
    switch (spellproto->SpellFamilyName)
    {
        case SPELLFAMILY_DEATHKNIGHT:
        {
            /// Necrotic Plague - 24 seconds in PvP
            if (spellproto->Id == 155159)
                return 24 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            /// Faerie Swarm - 8 seconds in PvP
            if (spellproto->SpellFamilyFlags[0] & 0x100)
                return 8 * IN_MILLISECONDS;
            /// Faerie Swarm (Decrease Speed) - 8 seconds in Pvp
            if (spellproto->Id == 102354)
                return 8 * IN_MILLISECONDS;
            /// Faerie Fire - 20 seconds in PvP (6.0)
            if (spellproto->SpellFamilyFlags[0] & 0x400)
                return 20 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            // Hunter's Mark - 30 seconds in PvP (6.0)
            if (spellproto->Id == 1130)
                return 30 * IN_MILLISECONDS;
            // Binding Shot - 3 seconds in PvP (6.0)
            if (spellproto->SpellIconID == 4612 && spellproto->SpellVisual[0] == 6859)
                return 3 * IN_MILLISECONDS;
            // Wyvern Sting - 6 seconds in PvP (6.0)
            if (spellproto->SpellFamilyFlags[1] & 0x1000)
                return 6 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_MONK:
        {
            // Paralysis - 4 seconds in PvP regardless of if they are facing you (6.0)
            if (spellproto->SpellFamilyFlags[2] & 0x800000)
                return 4 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            /// Psychic Scream - 6 seconds in PvP
            if (spellproto->Id == 8122)
                return 6 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            /// Howl of Terror - 6 seconds in PvP (6.0)
            if (spellproto->Id == 5484)
                return 6 * IN_MILLISECONDS;
            /// Fear - 6 seconds in PvP (6.0)
            if (spellproto->Id == 118699 || spellproto->Id == 130616)
                return 6 * IN_MILLISECONDS;
            /// Debilitate - 4 seconds in PvP
            if (spellproto->Id == 170996)
                return 4 * IN_MILLISECONDS;
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            /// Deep Freeze - 4 seconds in PvP (6.2.3)
            if (spellproto->Id == 44572)
                return 4 * IN_MILLISECONDS;
        }
        default:
            break;
    }

    return 8 * IN_MILLISECONDS;
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
    for (uint8 i = 0; i < spellInfo->EffectCount; ++i)
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

SpellInfo const* SpellMgr::GetSpellForDifficulty(uint32 p_SpellId, Difficulty p_Difficulty) const
{
    return GetSpellInfo(p_SpellId, p_Difficulty);
}

SpellInfo const* SpellMgr::GetSpellForDifficultyFromSpell(SpellInfo const* spell, Unit const* caster) const
{
    if (!spell)
        return nullptr;

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
        // No extra req, so can trigger only for hit/crit/absorb - spell has to be active
        if ((procExtra & (PROC_EX_NORMAL_HIT | PROC_EX_CRITICAL_HIT | PROC_EX_ABSORB)) && active)
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
            if ((procExtra & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT|PROC_EX_ABSORB)) && ((procEvent_procEx & (AURA_SPELL_PROC_EX_MASK)) == 0))
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

const std::set<MinorTalentEntry const*>* SpellMgr::GetSpecializationPerks(uint32 specializationId) const
{
    SpecializatioPerkMap::const_iterator iter = mSpecializationPerks.find(specializationId);
    return iter != mSpecializationPerks.end() ? &(iter->second) : nullptr;
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
            if (!(pArea && pArea->Flags & AREA_FLAG_NO_FLY_ZONE))
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

    std::map<uint32 /*spell*/, uint32 /*next*/> chains;
    std::set<uint32> hasPrev;
    for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
    {
        SkillLineAbilityEntry const* SkillInfo = sSkillLineAbilityStore.LookupEntry(i);
        if (!SkillInfo)
            continue;

        if (!SkillInfo->learnOnGetSkill)
            continue;

        if (!GetSpellInfo(SkillInfo->learnOnGetSkill) || !GetSpellInfo(SkillInfo->spellId))
            continue;

        chains[SkillInfo->learnOnGetSkill] = SkillInfo->spellId;
        hasPrev.insert(SkillInfo->spellId);
    }

    // each key in chains that isn't present in hasPrev is a first rank
    for (auto itr = chains.begin(); itr != chains.end(); ++itr)
    {
        if (hasPrev.count(itr->first))
            continue;

        SpellInfo const* first = GetSpellInfo(itr->first);
        SpellInfo const* next = GetSpellInfo(itr->second);

        mSpellChains[itr->first].first = first;
        mSpellChains[itr->first].prev = nullptr;
        mSpellChains[itr->first].next = next;
        mSpellChains[itr->first].last = next;
        mSpellChains[itr->first].rank = 1;
        for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
        {
            if (mSpellInfoMap[difficulty][itr->first])
                mSpellInfoMap[difficulty][itr->first]->ChainEntry = &mSpellChains[itr->first];
        }
        mSpellChains[itr->second].first = first;
        mSpellChains[itr->second].prev = first;
        mSpellChains[itr->second].next = nullptr;
        mSpellChains[itr->second].last = next;
        mSpellChains[itr->second].rank = 2;
        for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
        {
            if (mSpellInfoMap[difficulty][itr->second])
                mSpellInfoMap[difficulty][itr->second]->ChainEntry = &mSpellChains[itr->second];
        }

        uint8 rank = 3;
        auto nextItr = chains.find(itr->second);
        while (nextItr != chains.end())
        {
            SpellInfo const* prev = GetSpellInfo(nextItr->first); // already checked in previous iteration (or above, in case this is the first one)
            SpellInfo const* last = GetSpellInfo(nextItr->second);
            if (!last)
                break;

            mSpellChains[nextItr->first].next = last;

            mSpellChains[nextItr->second].first = first;
            mSpellChains[nextItr->second].prev = prev;
            mSpellChains[nextItr->second].next = nullptr;
            mSpellChains[nextItr->second].last = last;
            mSpellChains[nextItr->second].rank = rank++;
            for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
            {
                if (mSpellInfoMap[difficulty][nextItr->second])
                    mSpellInfoMap[difficulty][nextItr->second]->ChainEntry = &mSpellChains[nextItr->second];
            }

            // fill 'last'
            do
            {
                mSpellChains[prev->Id].last = last;
                prev = mSpellChains[prev->Id].prev;
            } while (prev);

            nextItr = chains.find(nextItr->second);
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell rank records in %u ms", uint32(mSpellChains.size()), GetMSTimeDiffToNow(oldMSTime));
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

    uint32 l_ProfessionSkillForStep[] { 0, 75, 150, 225, 300, 375, 450, 525, 600, 700};

    uint32 dbc_count = 0;
    for (uint32 spell = 0; spell < GetSpellInfoStoreSize(); ++spell)
    {
        SpellInfo const* entry = GetSpellInfo(spell);

        if (!entry)
            continue;

        for (uint8 i = 0; i < entry->EffectCount; ++i)
        {
            if (entry->Effects[i].Effect == SPELL_EFFECT_SKILL)
            {
                SpellLearnSkillNode dbc_node;
                dbc_node.skill = entry->Effects[i].MiscValue;
                dbc_node.step = entry->Effects[i].CalcValue();

                if (IsProfessionSkill(dbc_node.skill))
                {
                    uint16 l_Step = std::min(dbc_node.step, (uint16)((sizeof(l_ProfessionSkillForStep) / sizeof(uint32)) - 1));
                    dbc_node.value = 1;
                    dbc_node.maxvalue = l_ProfessionSkillForStep[l_Step];
                }
                else
                {
                    if (dbc_node.skill != SKILL_RIDING)
                        dbc_node.value = 1;
                    else
                        dbc_node.value = dbc_node.step * 75;
                    dbc_node.maxvalue = dbc_node.step * 75;
                }

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

        for (uint8 i = 0; i < entry->EffectCount; ++i)
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
    for (uint8 j = 0; j < spellInfo->EffectCount; ++j)
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
                (spellInfo->Effects[eff].Effect == SPELL_EFFECT_APPLY_AURA ||
                 spellInfo->Effects[eff].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET) &&
                spellInfo->Effects[eff].ApplyAuraName != SPELL_AURA_DUMMY)
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

bool IsCCSpell(SpellInfo const* p_SpellProto)
{
    if (p_SpellProto->SpellFamilyName == SPELLFAMILY_HUNTER ||
        p_SpellProto->SpellFamilyName == SPELLFAMILY_GENERIC)
        return false;

    for (uint8 l_EffectIndex = 0; l_EffectIndex < p_SpellProto->EffectCount; l_EffectIndex++)
    {
        switch (p_SpellProto->Effects[l_EffectIndex].ApplyAuraName)
        {
            case SPELL_AURA_MOD_CONFUSE:
            case SPELL_AURA_MOD_FEAR:
            case SPELL_AURA_MOD_STUN:
            case SPELL_AURA_MOD_ROOT:
            case SPELL_AURA_TRANSFORM:
                if (!p_SpellProto->IsPositiveEffect(l_EffectIndex))
                    return true;
                break;

            default:
                break;
        }
    }

    return false;
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

        for (uint8 j = 0; j < spellInfo->EffectCount; ++j)
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

void SpellMgr::LoadSpellPlaceHolder()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = GetSpellInfo(i);
        if (!spellInfo)
            continue;

        if (spellInfo->AttributesEx8 & SPELL_ATTR8_UNK13 && spellInfo->AttributesEx10 & SPELL_ATTR10_UNK15)
        {
            mPlaceHolderSpells.insert(spellInfo->Id);
            ++count;
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u talent place holder in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
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

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded addition spells for %u pet spell data entries in %u ms", countData, GetMSTimeDiffToNow(oldMSTime));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading summonable creature templates...");
    oldMSTime = getMSTime();

    // different summon spells
    for (uint32 i = 0; i < GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellEntry = GetSpellInfo(i);
        if (!spellEntry)
            continue;

        for (uint8 k = 0; k < spellEntry->EffectCount; ++k)
        {
            if (spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON || spellEntry->Effects[k].Effect == SPELL_EFFECT_SUMMON_PET)
            {
                uint32 creature_id = spellEntry->Effects[k].MiscValue;
                CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(creature_id);
                if (!cInfo)
                    continue;

                /// Get default pet spells from creature_template
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
    for (int l_ClassID = 0; l_ClassID < MAX_CLASSES; l_ClassID++)
    {
        ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(l_ClassID);
        if (!classEntry)
            continue;

        // Player mastery activation
        mSpellClassInfo[l_ClassID].insert(114585);
        // Battle Fatigue
        mSpellClassInfo[l_ClassID].insert(134732);

        // Opening gameobject
        if (l_ClassID == CLASS_MONK)
        {
            mSpellClassInfo[l_ClassID].insert(3365);
            mSpellClassInfo[l_ClassID].insert(6247);
            mSpellClassInfo[l_ClassID].insert(6477);
            mSpellClassInfo[l_ClassID].insert(6478);
            mSpellClassInfo[l_ClassID].insert(21651);
            mSpellClassInfo[l_ClassID].insert(22810);
            mSpellClassInfo[l_ClassID].insert(61437);
            mSpellClassInfo[l_ClassID].insert(68398);
            mSpellClassInfo[l_ClassID].insert(96220);
        }

        for (uint32 l_I = 0; l_I < sSkillLineAbilityStore.GetNumRows(); ++l_I)
        {
            SkillLineAbilityEntry const* l_SkillLine = sSkillLineAbilityStore.LookupEntry(l_I);
            if (!l_SkillLine)
                continue;

            SpellInfo const* l_SpellEntry = sSpellMgr->GetSpellInfo(l_SkillLine->spellId);
            if (!l_SpellEntry)
                continue;

            /*if (l_SpellEntry->SpellLevel == 0)
                continue;*/

            if (l_SkillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                continue;

            if (l_SkillLine->skillId != SkillClass[l_ClassID])
            {
                if ((l_SkillLine->classmask & 1 << (l_ClassID - 1)) == 0)
                    continue;
            }

            if (sSpellMgr->IsTalent(l_SpellEntry->Id))
                continue;

            mSpellClassInfo[l_ClassID].insert(l_SpellEntry->Id);
        }

        for (uint32 l_I = 0; l_I < sSpecializationSpellStore.GetNumRows(); ++l_I)
        {
            SpecializationSpellEntry const* l_SpecializationInfo = sSpecializationSpellStore.LookupEntry(l_I);
            if (!l_SpecializationInfo)
                continue;

            ChrSpecializationsEntry const* l_ChrSpec = sChrSpecializationsStore.LookupEntry(l_SpecializationInfo->SpecializationEntry);
            if (!l_ChrSpec)
                continue;

            mSpellClassInfo[l_ChrSpec->ClassID].insert(l_SpecializationInfo->LearnSpell);
        }

        for (uint32 l_I = 0; l_I < sMinorTalentStore.GetNumRows(); l_I++)
        {
            MinorTalentEntry const* l_MinorTalent = sMinorTalentStore.LookupEntry(l_I);

            if (!l_MinorTalent)
                continue;

            mSpecializationPerks[l_MinorTalent->specializationID].insert(l_MinorTalent);
        }
    }
}

void SpellMgr::InitializeSpellDifficulty()
{
    mAvaiableDifficultyBySpell.clear();

    /// SpellAuraOptions
    for (uint32 l_I = 0; l_I < sSpellAuraOptionsStore.GetNumRows(); ++l_I)
    {
        if (SpellAuraOptionsEntry const* l_SpellAuraOption = sSpellAuraOptionsStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellAuraOption->m_SpellID].insert(l_SpellAuraOption->m_DifficultyID);

            if (l_SpellAuraOption->m_DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellAuraOptionsStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellAuraOption->m_SpellID, l_SpellAuraOption->m_DifficultyID), l_SpellAuraOption->Id));
        }
    }

    /// SpellCategories
    for (uint32 l_I = 0; l_I < sSpellCategoriesStore.GetNumRows(); ++l_I)
    {
        if (SpellCategoriesEntry const* l_SpellCategories = sSpellCategoriesStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellCategories->SpellId].insert(l_SpellCategories->m_DifficultyID);

            if (l_SpellCategories->m_DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellCategoriesStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellCategories->SpellId, l_SpellCategories->m_DifficultyID), l_SpellCategories->Id));
        }
    }

    /// SpellCooldowns
    for (uint32 l_I = 0; l_I < sSpellCooldownsStore.GetNumRows(); ++l_I)
    {
        if (SpellCooldownsEntry const* l_SpellCooldown = sSpellCooldownsStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellCooldown->m_SpellID].insert(l_SpellCooldown->m_DifficultyID);

            if (l_SpellCooldown->m_DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellCooldownsStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellCooldown->m_SpellID, l_SpellCooldown->m_DifficultyID), l_SpellCooldown->Id));
        }
    }

    /// SpellEffect
    for (uint32 l_I = 0; l_I < sSpellEffectStore.GetNumRows(); ++l_I)
    {
        if (SpellEffectEntry const* l_SpellEffect = sSpellEffectStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellEffect->EffectSpellId].insert(l_SpellEffect->EffectDifficulty);

            if (l_SpellEffect->EffectDifficulty != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellEffectStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellEffect->EffectSpellId, l_SpellEffect->EffectDifficulty), l_SpellEffect->Id));
        }
    }

    /// SpellEquippedItems
    for (uint32 l_I = 0; l_I < sSpellEquippedItemsStore.GetNumRows(); ++l_I)
    {
        if (SpellEquippedItemsEntry const* l_SpellEquippedItem = sSpellEquippedItemsStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellEquippedItem->SpellID].insert(l_SpellEquippedItem->DifficultyID);

            if (l_SpellEquippedItem->DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellEquippedItemsStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellEquippedItem->SpellID, l_SpellEquippedItem->DifficultyID), l_SpellEquippedItem->Id));
        }
    }

    /// SpellInterrupts
    for (uint32 l_I = 0; l_I < sSpellInterruptsStore.GetNumRows(); ++l_I)
    {
        if (SpellInterruptsEntry const* l_SpellInterrupt = sSpellInterruptsStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellInterrupt->SpellID].insert(l_SpellInterrupt->DifficultyID);

            if (l_SpellInterrupt->DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellInterruptsStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellInterrupt->SpellID, l_SpellInterrupt->DifficultyID), l_SpellInterrupt->Id));

        }
    }

    /// SpellLevels
    for (uint32 l_I = 0; l_I < sSpellLevelsStore.GetNumRows(); ++l_I)
    {
        if (SpellLevelsEntry const* l_SpellLevel = sSpellLevelsStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellLevel->SpellID].insert(l_SpellLevel->DifficultyID);

            if (l_SpellLevel->DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellLevelsStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellLevel->SpellID, l_SpellLevel->DifficultyID), l_SpellLevel->Id));
        }
    }

    /// SpellTargetStriction
    for (uint32 l_I = 0; l_I < sSpellTargetRestrictionsStore.GetNumRows(); ++l_I)
    {
        if (SpellTargetRestrictionsEntry const* l_SpellTargetRestriction = sSpellTargetRestrictionsStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_SpellTargetRestriction->SpellId].insert(l_SpellTargetRestriction->DifficultyID);

            if (l_SpellTargetRestriction->DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellTargetRestrictionsStore.GetDbcFileName()].insert(std::make_pair(std::make_pair(l_SpellTargetRestriction->SpellId, l_SpellTargetRestriction->DifficultyID), l_SpellTargetRestriction->Id));
        }
    }

    /// SpellXSpellVisual
    for (uint32 l_I = 0; l_I < sSpellXSpellVisualStore.GetNumRows(); ++l_I)
    {
        if (SpellXSpellVisualEntry const* l_Visual = sSpellXSpellVisualStore.LookupEntry(l_I))
        {
            mAvaiableDifficultyBySpell[l_Visual->SpellId].insert(l_Visual->DifficultyID);

            if (l_Visual->DifficultyID != Difficulty::DifficultyNone)
                mDatastoreSpellDifficultyKey[sSpellXSpellVisualStore.GetDB2FileName()].insert(std::make_pair(std::make_pair(l_Visual->SpellId, l_Visual->DifficultyID), l_Visual->Id));
        }
    }
}

void SpellMgr::LoadSpellInfoStore()
{
    uint32 oldMSTime = getMSTime();

    UnloadSpellInfoStore();
    for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
        mSpellInfoMap[difficulty].resize(sSpellStore.GetNumRows(), nullptr);

    for (uint32 l_ID = 0; l_ID < sSpellXSpellVisualStore.GetNumRows(); ++l_ID)
    {
        SpellXSpellVisualEntry const* l_Entry = sSpellXSpellVisualStore.LookupEntry(l_ID);
        if (!l_Entry)
            continue;

        VisualsBySpellMap[l_Entry->SpellId][l_Entry->DifficultyID].push_back(l_Entry);
    }

    ParallelFor(0, sSpellStore.GetNumRows(), [this](uint32 l_I) -> void
    {
        if (SpellEntry const* spellEntry = sSpellStore.LookupEntry(l_I))
        {
            auto l_Itr = VisualsBySpellMap.find(l_I);
            SpellVisualMap emptyMap;
            SpellVisualMap& visualMap = (l_Itr == VisualsBySpellMap.end()) ? emptyMap : l_Itr->second;

            std::set<uint32> difficultyInfo = mAvaiableDifficultyBySpell[l_I];

            for (std::set<uint32>::iterator itr = difficultyInfo.begin(); itr != difficultyInfo.end(); itr++)
                mSpellInfoMap[(*itr)][l_I] = new SpellInfo(spellEntry, (*itr), std::move(visualMap));
        }
    });

    for (uint32 l_I = 0; l_I < sSpellPowerStore.GetNumRows(); l_I++)
    {
        SpellPowerEntry const* spellPower = sSpellPowerStore.LookupEntry(l_I);
        if (!spellPower)
            continue;

        for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
        {
            SpellInfo* spell = mSpellInfoMap[difficulty][spellPower->SpellId];
            if (!spell)
                continue;

            spell->SpellPowers.push_back(spellPower);
        }
    }

    for (uint32 l_I = 0; l_I < sTalentStore.GetNumRows(); l_I++)
    {
        TalentEntry const* l_TalentEntry = sTalentStore.LookupEntry(l_I);
        if (!l_TalentEntry)
            continue;

        SpellInfo* l_SpellInfo = mSpellInfoMap[DifficultyNone][l_TalentEntry->SpellID];
        if (l_SpellInfo)
            l_SpellInfo->m_TalentIDs.push_back(l_TalentEntry->Id);

        /// Load talents override spell
        if (l_TalentEntry->OverridesSpellID)
        {
            l_SpellInfo = (SpellInfo*)sSpellMgr->GetSpellInfo(l_TalentEntry->OverridesSpellID);
            if (l_SpellInfo)
                l_SpellInfo->OverrideSpellList.push_back(l_TalentEntry->SpellID);
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded spell info store in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::UnloadSpellInfoStore()
{
    for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
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
    for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
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
        for (int difficulty = 0; difficulty < Difficulty::MaxDifficulties; difficulty++)
        {
            spellInfo = mSpellInfoMap[difficulty][i];
            if (!spellInfo)
                continue;

            for (uint8 j = 0; j < spellInfo->EffectCount; ++j)
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
                    case SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL:
                    case SPELL_EFFECT_TEACH_FOLLOWER_ABILITY:
                        spellInfo->Effects[j].TargetA = TARGET_UNIT_CASTER;
                        spellInfo->Effects[j].TargetB = TARGET_UNIT_CASTER;

                        if (j == EFFECT_0 && spellInfo->Effects[EFFECT_1].Effect == SPELL_EFFECT_DUMMY)
                        {
                            spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CASTER;
                            spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_CASTER;
                        }
                        break;
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
                    {
                        mSpellCreateItemList.push_back(i);

                        SkillLineAbilityMapBounds l_SpellBounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellInfo->Id);
                        for (SkillLineAbilityMap::const_iterator l_SpellIdx = l_SpellBounds.first; l_SpellIdx != l_SpellBounds.second; ++l_SpellIdx)
                            m_ItemSourceSkills[spellInfo->Effects[j].ItemType].push_back(l_SpellIdx->second->skillId);

                        break;
                    }
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
                            for (uint8 s = 0; s < MAX_ENCHANTMENT_SPELLS; ++s)
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
                        for (uint8 s = 0; s < MAX_ENCHANTMENT_SPELLS; ++s)
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
        for (uint8 j = 0; j < spellInfo->EffectCount; ++j)
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

        if (spellInfo->Attributes & SPELL_ATTR0_TRADESPELL)
        {
            SkillLineAbilityMapBounds l_SpellBounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellInfo->Id);
            for (SkillLineAbilityMap::const_iterator l_SpellIdx = l_SpellBounds.first; l_SpellIdx != l_SpellBounds.second; ++l_SpellIdx)
                m_SkillTradeSpells[l_SpellIdx->second->skillId].push_back(l_SpellIdx->second);
        }

        ///////////////////////////
        //////   END BINARY  //////
        ///////////////////////////

        switch (spellInfo->Id)
        {
            ///////////////////////////////////////////////////////////////////////////////////
            /// Rush Orders
            ///////////////////////////////////////////////////////////////////////////////////
            case 181507: ///< Alchemy Lab
                spellInfo->Reagent[0]              = 122576;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181228;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181229;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181518: ///< The Tannery
                spellInfo->Reagent[0]              = 122596;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181240;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181241;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181519: ///< Scribes Quarters
                spellInfo->Reagent[0]              = 122593;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181236;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181237;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181520: ///< The Forge
                spellInfo->Reagent[0]              = 122595;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181230;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181231;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181521: ///< Tailoring Emporium
                spellInfo->Reagent[0]              = 122594;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181242;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181243;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181522: ///< Gem Boutique
                spellInfo->Reagent[0]              = 122592;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181238;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181239;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181524: ///< Enchanters Study
                spellInfo->Reagent[0]              = 122590;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181232;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181233;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            case 181525: ///< Engineering Works
                spellInfo->Reagent[0]              = 122591;
                spellInfo->ReagentCount[0]         = 1;
                spellInfo->Effects[0].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 181234;
                spellInfo->Effects[0].BasePoints   = 0; ///< TEAM_ALLIANCE
                spellInfo->Effects[1].Effect       = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[1].TriggerSpell = 181235;
                spellInfo->Effects[1].BasePoints   = 1; ///< TEAM_HORDE
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            case 1949: ///< Hellfire
                spellInfo->AttributesCu &= ~SPELL_ATTR0_CU_NEGATIVE;
                spellInfo->InterruptFlags &= ~SPELL_INTERRUPT_FLAG_MOVEMENT;
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            /// Engineering Works
            ///////////////////////////////////////////////////////////////////////////////////
            case 156756:
                spellInfo->Effects[0].MiscValue = 191605; ///< Missing data, take back entry from spell 54710
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            /// Scribe Quarters
            ///////////////////////////////////////////////////////////////////////////////////
            case 176513:    ///< Draenor Merchant Order
                spellInfo->Effects[0].Effect = SPELL_EFFECT_CREATE_RANDOM_ITEM;
                break;
            case 176791:    ///< Combine
                spellInfo->Effects[1].Effect = 0;
                /// No break needed here
            case 176482:    ///< Combine
            case 176483:    ///< Combine
            case 176484:    ///< Combine
            case 176485:    ///< Combine
            case 176486:    ///< Combine
            case 176487:    ///< Combine
            case 176488:    ///< Combine
            case 176489:    ///< Combine
            case 176490:    ///< Combine
            case 176491:    ///< Combine
            case 176934:    ///< Combine
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            case 61551:     ///< Toy Train Set (Pulse)
                spellInfo->TargetAuraSpell = 0;
                break;
            case 179244:    ///< Summon Chauffeur (Horde)
            case 179245:    ///< Summon Chauffeur (Alliance)
                spellInfo->Effects[EFFECT_0].MiscValueB = 284;  ///< This will allow level 1 mounting at 160% normal speed
                break;
            case 1843:      ///< Hack for disarm. Client sends the spell instead of gameobjectuse.
            case 161710:    ///< Garrison enchanter study
            case 160201:    ///< Garrison enchanter study
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_ALWAYS_ACTIVE;
                break;
            case 178444: ///< Create Armor Enhancement (garrison loot spell)
            case 178445: ///< Create Weapon Boost (garrison loot spell)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_CREATE_RANDOM_ITEM;
                break;
            case 167946: ///< Journeyman Logging (Lumber Mill)
                spellInfo->Effects[0].BasePoints = 1;
                break;
            case 168361: ///< Forward Thrust
                spellInfo->Effects[0].MiscValue = 500;
                break;
            case 105157: ///< See Quest Invis 14, Wandering Island spell
                spellInfo->AreaGroupId = 0;
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            /// Stables
            ///////////////////////////////////////////////////////////////////////////////////
            /*case 174216:    ///< Summon Snarler-In-Training
            case 174218:    ///< Summon Icehoof-In-Training - Potentially fixed, but causes player stuck
            case 174219:    ///< Summon Meadowstomper-In-Training
            case 174220:    ///< Summon Riverwallow-In-Training
            case 174221:    ///< Summon Rocktusk-In-Training
            case 174222:    ///< Summon Snarler-In-Training
                spellInfo->Effects[0].Effect        = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOUNTED;
                spellInfo->Effects[0].TargetA       = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].MiscValue     = 305;
                spellInfo->Effects[0].MiscValueB    = 230;
                break;*/
            case 173702: ///< Lasso Break
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 173686: ///< Stables Lasso
            case 174070: ///< Stables Lasso
                spellInfo->Effects[EFFECT_1].TriggerSpell = 0;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(5); ///< 300s
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            case 182464: ///< Portal to Garrison
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 179478: ///< Voidtalon of the Dark Star
                spellInfo->Effects[EFFECT_0].MiscValue = 89959;
                spellInfo->Effects[EFFECT_0].MiscValueB = 230;
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            /// Blackrock Foundry
            ///////////////////////////////////////////////////////////////////////////////////
            case 175609: ///< Unbind Flame
                spellInfo->Effects[EFFECT_0].MiscValueB = 64;
                break;
            case 175638: ///< Spinning Blade
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_1].TargetB = 0;
                break;
            case 175643: ///< Spinning Blade (DoT)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); ///< 2s
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 173192: ///< Cave In (Dot)
            case 159686: ///< Acidback Puddle (DoT)
            case 156203: ///< Retched Blackrock (Oregorger)
            case 155265: ///< Containment (Primal Elementalist)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 175091: ///< Animate Slag
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 170687: ///< Killing Spree
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_2;
                break;
            case 155077: ///< Overwhelming Blows (Gruul)
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                break;
            case 159632: ///< Insatiable Hunger
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 160665: ///< Rolling Box (Oregorger)
            case 160833: ///< Bust Loose (Heart of the Mountain)
            case 155738: ///< Slag Pool (Heart of the Mountain)
            case 155224: ///< Melt (Heart of the Mountain)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 114956: ///< Nether Tempest (launcher visual)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 155819: ///< Hunger Drive (Oregorger)
                spellInfo->Attributes &= ~SPELL_ATTR0_DEBUFF;
                break;
            case 155897: ///< Earthshaking Collision (Oregorger)
                spellInfo->Mechanic = MECHANIC_DISCOVERY;
                break;
            case 159958: ///< Earthshaking Stomp (Oregorger)
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 173461: ///< Blackrock Barrage (Oregorger)
                spellInfo->CasterAuraSpell = 0;
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 160382: ///< Defense (Security Guard)
            case 158246: ///< Hot Blooded (Foreman Feldspar)
            case 156932: ///< Rupture DoT (Foreman Feldspar)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); ///< 2s
                break;
            case 159558: ///< Bomb (Furnace Engineer)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_CAN_BE_CASTED_ON_ALLIES;
                break;
            case 155201: ///< Electrocution (Furnace Engineer)
                spellInfo->Effects[EFFECT_0].ChainTarget = 2;
                break;
            case 155196: ///< Fixate (Slag Elemental)
                spellInfo->MaxAffectedTargets = 1;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 159115: ///< Erupt (Firecaller)
                spellInfo->Attributes |= SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY;
                break;
            case 108977: //< Way of the Monk
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 178209: ///< Chest of Iron (T17)
            case 178210: ///< Legs of Iron (T17)
            case 178211: ///< Gauntlets of the Iron Conqueror (T17)
            case 178212: ///< Helm of Iron (T17)
            case 178213: ///< Shoulders of Iron (T17)
            case 178216: ///< Helm of Iron (Normal - T17 - Quest)
            case 178217: ///< Chest of Iron (Normal - T17 - Quest)
            case 178218: ///< Legs of Iron (Normal - T17 - Quest)
            case 178219: ///< Gauntlets of Iron (Normal - T17 - Quest)
            case 178220: ///< Shoulders of Iron (Normal - T17 - Quest)
            case 178221: ///< Legs of Iron (Heroic - T17 - Quest)
            case 178222: ///< Gauntlets of Iron (Heroic - T17 - Quest)
            case 178223: ///< Shoulders of Iron (Heroic - T17 - Quest)
            case 178224: ///< Helm of Iron (Heroic - T17 - Quest)
            case 178225: ///< Chest of Iron (Heroic - T17 - Quest)
            case 178226: ///< Helm of Iron (Mythic - T17 - Quest)
            case 178227: ///< Chest of Iron (Mythic - T17 - Quest)
            case 178228: ///< Legs of Iron (Mythic - T17 - Quest)
            case 178229: ///< Gauntlets of Iron (Mythic - T17 - Quest)
            case 178230: ///< Shoulders of Iron (Mythic - T17 - Quest)
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 155200: ///< Burn (Slag Elemental)
            case 155890: ///< Molten Torrent (Dummy visual - Molten Torrent Stalker)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 156220: ///< Tactical Retreat
            case 156883: ///< Tactical Retreat (Other)
            case 163636: ///< Firestorm V2 Missile (Firestorm Stalker)
            case 162757: ///< Ice Trap (Iron Marksman)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 155747: ///< Body Slam
            case 157923: ///< Jump Slam
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 30;
                break;
            case 156324: ///< Acid Torrent (AoE)
            case 155225: ///< Melt (Heart of the Mountain)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                break;
            case 177756: ///< Deafening Roar (Bellows Operator)
                spellInfo->EffectCount = 1;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].TriggerSpell = 0;
                break;
            case 177858: ///< Ember in the Wind (Mol'dana Two Blade)
                spellInfo->TargetAuraSpell = 177855;    ///< Ember in the Wind (aura)
                break;
            case 177891: ///< Rising Flame Kick (Mol'dana Two Blade)
            case 177855: ///< Ember in the Wind (aura - Mol'dana Two Blade)
            case 154932: ///< Molten Torrent (aura - Flamebender Ka'graz)
            case 161570: ///< Searing Plates (DoT - Franzok)
            case 159481: ///< Delayed Siege Bomb (Channel - Iron Gunnery Sergeant)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 156039: ///< Drop the Hammer (Aknor Steelbringer)
            case 155571: ///< Jump Out of Lava (Cinder Wolf)
            case 162285: ///< Rend and Tear (Beastlord Darmac)
            case 162279: ///< Rend and Tear (Beastlord Darmac)
            case 155567: ///< Rend and Tear (Cruelfang)
            case 155060: ///< Rend and Tear (Cruelfang)
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 50;
                break;
            case 174215: ///< Summon Armament (Flamebender Ka'graz)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_TARGET_BACK;
                break;
            case 163644: ///< Summon Enchanted Armament (Flamebender Ka'graz)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_NEARBY_ENTRY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(187);  ///< 300y
                break;
            case 174217: ///< Summon Enchanted Armament (Enchanted Armament)
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(187);  ///< 300y
                break;
            case 163153: ///< Enchant Armament (Jump - Enchanted Armament)
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 50;
                spellInfo->Effects[EFFECT_0].MiscValueB = 300;
                break;
            case 155074: ///< Charring Breath (Cinder Wolf)
            case 173790: ///< Spirit Bond (Stubborn Ironhoof)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 151271: ///< Collect Mask
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_NONE;
                break;
            case 155049: ///< Singe (Cinder Wolf)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 154938: ///< Molten Torrent (AoE Damage - 154938)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 162349: ///< Fists of Stone (Kromog)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_BLOCKABLE_SPELL;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 155745: ///< Charring Breath (Jump - Overheated Cinderwolf)
                spellInfo->Attributes |= SPELL_ATTR0_HIDDEN_CLIENTSIDE;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 154952: ///< Fixate (Cinder Wolf)
                spellInfo->MaxAffectedTargets = 1;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->InterruptFlags &= ~SPELL_INTERRUPT_FLAG_MOVEMENT;
                break;
            case 163633: ///< Magma Monsoon
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 161049: ///< Rippling Smash (Kromog)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 157060: ///< Rune of Grasping Earth - Cast (Kromog)
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
                break;
            case 157054: ///< Thundering Blows (Kromog)
                spellInfo->Effects[EFFECT_0].TriggerSpell = 162355;
                spellInfo->Effects[EFFECT_1].TriggerSpell = 157055;
                break;
            case 161923: ///< Rune of Crushing Earth - Damage (Stone Wall)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_129;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CONE_ENEMY_129;
                spellInfo->Effects[EFFECT_1].TargetB = 0;
                spellInfo->Effects[EFFECT_2].TargetA = TARGET_UNIT_CONE_ENEMY_129;
                spellInfo->Effects[EFFECT_2].TargetB = 0;
                break;
            case 154951: ///< Pin Down (Beastlord Darmac)
            case 163045: ///< Flame Vent Cosmetics (Flame Vent)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 155653: ///< Flame Infusion (Pack Beast)
                spellInfo->Attributes |= SPELL_ATTR0_CASTABLE_WHILE_DEAD;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 155198: ///< Savage Howl (Cruelfang)
                spellInfo->PreventionType = 0;
                break;
            case 155221: ///< Tantrum (Ironcrusher)
            case 155520: ///< Tantrum (Beastlord Darmac)
                spellInfo->AuraInterruptFlags = 0;
                spellInfo->ChannelInterruptFlags = 0;
                spellInfo->InterruptFlags = 0;
                break;
            case 156294: ///< Throw Grenade (Iron Raider)
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 160177: ///< Cautorizing Bolt (Grom'kar Firemender)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 159480: ///< Delayed Siege Bomb - Searcher (Iron Gunnery Sergeant)
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 159482: ///< Delayed Siege Bomb - Missile (Iron Gunnery Sergeant)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_TARGET_ENEMY;
                break;
            case 158084: ///< Delayed Siege Bomb - Damage (Iron Gunnery Sergeant)
            case 160050: ///< Delayed Siege Bomb - Damage (Operator Thogar)
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////
            /// Skills
            ///////////////////////////////////////////////////////////////////////////////////
            case 169092: ///< Temporal Crystal
                spellInfo->Effects[EFFECT_0].BasePoints = 1;
                spellInfo->Effects[EFFECT_0].DieSides = 0;
                break;
            ///////////////////////////////////////////////////////////////////////////////////
            case 113095: ///< Demon Hunter's Aspect
                spellInfo->Effects[EFFECT_0].MiscValue = 21178;
                break;
            case 163661: ///< Cho'gall Night
                spellInfo->EffectCount = 2;
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_PHASE;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_1].MiscValue = 2;
                break;
            case 51460:  ///< Runic Corruption
                spellInfo->EffectCount = 1;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_2].Effect = 0;
                spellInfo->Effects[EFFECT_2].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_3].Effect = 0;
                spellInfo->Effects[EFFECT_3].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_4].Effect = 0;
                spellInfo->Effects[EFFECT_4].ApplyAuraName = 0;
                break;
            case 168178: ///< Salvage (garrison loot spell)
            case 168179: ///< Salvage (garrison loot spell)
            case 168180: ///< Salvage (garrison loot spell)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_CREATE_RANDOM_ITEM;
                break;
            case 167650: ///< Loose Quills (Rukhmar)
            case 167630: ///< Blaze of Glory (Rukhmar)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_5_YARDS); ///< 5yd
                break;
            case 178851: ///< Rukhmar Bonus
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 128254: ///< Brew Finale Wheat Effect (Yan-Zhu - Stormstout Brewery)
            case 128256: ///< Brew Finale Medium Effect (Yan-Zhu - Stormstout Brewery)
            case 128258: ///< Brew Finale Dark Effect (Yan-Zhu - Stormstout Brewery)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_NEARBY_ENEMY;
                break;
            case 133601: ///< Durumu Debuff 2A (Durumu - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].BasePoints *= 35000;
                break;
            case 134169:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Attributes &= ~SPELL_ATTR0_HIDDEN_CLIENTSIDE;
                break;
            case 140016: ///< Drop Feathers (Ji Kun - Throne of Thunder) (ToT - #6 Ji Kun)
                spellInfo->Effects[EFFECT_0].MiscValue = 218543;
                break;
            case 134755: ///< Eye Sore (Durumu - Throne of Thunder)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 133740: ///< Bright Light (Durumu - Throne of Thunder)
                spellInfo->Effects[EFFECT_1].MiscValue = 0;
                spellInfo->AttributesEx8 &= ~SPELL_ATTR8_UNK27;
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNEL_TRACK_TARGET;
                spellInfo->Effects[EFFECT_3].TriggerSpell = 0;
                break;
            case 133795: ///< Drain Life (Durumu - Throne of Thunder)
                spellInfo->Effects[EFFECT_2].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 133798: ///< Drain Life (Durumu - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_TARGET_ANY;
                break;
            case 133796: ///< Drain Life (Durumu - Throne of Thunder)
            case 138908: ///< Transfusion (Dark Animus - Throne of Thunder)
            case 147234: ///< Dummy Nuke (Iron Qon - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 140495: ///< Lingering Gaze (Durumu - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].BasePoints = int32(spellInfo->Effects[EFFECT_0].BasePoints * 2.9f);
                break;
            case 136413: ///< Force of Will (Durumu - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_104;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 138378: ///< Transfusion (Dark Animus - Throne of Thunder)
                spellInfo->ExplicitTargetMask = 0;
                break;
            case 101257: ///< Wracking Pain dmg
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_100_YARDS);
                break;
            case 136954: ///< Anima Ring (Dark Animu - Throne of Thunder)
                for (uint8 l_Itr = 0; l_Itr < 12; ++l_Itr)
                    spellInfo->Effects[l_Itr].TriggerSpell = 0;
                break;
            case 136955: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 136956: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 136957: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 136958: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 136959: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 136960: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 138671: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 138672: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 138673: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 138674: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 138675: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
            case 138676: ///< Anima Ring (Triggered) (Dark Animus - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->ExplicitTargetMask = TARGET_FLAG_DEST_LOCATION;
                break;
            case 136962: ///< Anima Ring (Debuff) (Dark Animus - Throne of Thunder)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 138613: ///< Matter Swap (Dark Animus - Throne of Thunder)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetB = 0;
                break;
            case 139803: /// Triumphant Rush (ToT - #10 Iron Qon)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[EFFECT_2].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 167615: ///< Pierced Armor
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 97709:  ///< Altered Form (Racial)
                spellInfo->AttributesEx4 |= SPELL_ATTR4_UNK21;
                break;
            case 159407: ///< Combo Breaker: Chi Explosion
                spellInfo->ProcCharges = 0;
                spellInfo->ProcFlags = 0;
                spellInfo->ProcChance = 0;
                break;
            case 157607: ///< Instant Poison
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(29); ///< 12s
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 157675: ///< Chi Explosion
            case 182078: ///< Chi Explosion
                spellInfo->CastTimeEntry = 0;
                break;
                /// Shadowmoon Burial Grounds
            case 153068: ///< Void Devestation
                spellInfo->Effects[0].TargetA = TARGET_DEST_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_DEST_DEST_RADIUS;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); // 30y
                break;
            case 152962: ///< Soul Steal
                spellInfo->Effects[0].TriggerSpell = 0;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = NULL;
                break;
            case 152979: ///< Soul Shreads
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 164693: ///< Lunar Runes
            case 164695:
            case 164696:
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(4); ///< 120s
                break;
            case 154327: ///< Domination
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 153164: ///< Dark Communion
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = NULL;
                spellInfo->Effects[1].TargetB = NULL;
                spellInfo->Effects[2].TargetB = NULL;
                break;
            case 153153: ///< Dark Communion
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = NULL;
                break;
            case 153501: ///< Void Blast
            case 153070: ///< Void Devestation
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); // 2s
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 173073: ///< Ground Marker
                sSpellDurationStore.LookupEntry(1); ///< 10s
                break;
            case 153686: ///< Body Slam
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 154469: ///< Ritaul Of Bones - Darkness
            case 153692: ///< Necrotic Pitch Debuff
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); // 2s
                break;
            case 153236: ///< DaggerFall
                spellInfo->Effects[0].TargetA = 0;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 107079: ///< Quaking Palm
                spellInfo->SpellFamilyName = SPELLFAMILY_GENERIC;
                break;
            case 164685: ///< Dark Eclipse
                spellInfo->Effects[0].Amplitude = 600;
                break;
                /// Everbloom
            case 164643: ///< Rending Charge
            case 164886: ///< Dreadpetal Toxin
            case 169658: ///< Infected Wounds
            case 164965: ///< Choking Vines
            case 164834: ///< Barrage of Leaves
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 166491: ///< FireBloom - experimental, trying to produce triggers
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 169223: ///< Toxic Gas
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); // 1s
                break;
            case 167977: ///< Bramble Patch
            case 169495: ///< Living Leaves
            case 164294: ///< Unchecked Growth
            case 166726: ///< Frozen Rain
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); // 1s
                break;
            case 173080: ///< Fixate
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); // 1s
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 169322: ///< Descend Beam
            case 143569: ///< Sand Beam
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 169376: ///< Venomous Sting
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 164885: ///< Dreadpetal Toxin
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
                /// Iron Docks
            case 163705:  ///< Abrupt Restoration
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 178154:  ///< Acid Spit
            case 178155:  ///< Acid Spit Trigger Missile
            case 163689:  ///< Sanguine Sphere
            case 176287:  ///< Chain Drag
            case 172885:  ///< Charging Slash
            case 168348:  ///< Rapid Fire
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 168537:  ///< Cannon Barrage:
                spellInfo->Effects[0].Amplitude = 1000; // was 400
                break;
            case 164632:  ///< Burning Arrow
            case 164648:  ///< Barbed Arrow Dot
            case 173324:  ///< Jagged Caltrops Dot
            case 178156:  ///< Acid Splash
            case 172963:  ///  Gatecrasher
            case 173517:  /// Lava Blast
            case 164734:  /// Shredding Swipes
            case 173349:  /// Trampled
            case 173489:  /// Lava Barrage
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); // 1s
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 164504:  /// Initimidated
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 162424:  ///< Feeding Frenzy
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(563); // 20500ms
                break;
            case 163379:  ///< Big Boom
                spellInfo->InterruptFlags = 0;
                break;
            case 165152:  ///< Lava Sweep
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[0].Amplitude = 1000;
                break;
            case 172636:  ///< Grease Vial
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); // 2s
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 166923:  ///< Barbed Arrow Barrage
                spellInfo->Effects[0].BasePoints = 164370;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 163740: ///< Tainted Blood
                // case 163668: ///< Flaming Slash
                spellInfo->Effects[0].TargetB = TARGET_UNIT_NEARBY_ENEMY;
                break;
            case 169132: ///< Back Draft
                spellInfo->Effects[0].TargetB = TARGET_DEST_DEST;
                break;
                /// Auchindoun
            case 157505: ///< Arcane Bolt
                spellInfo->Effects[0].Amplitude = 1500;
                break;
            case 154340: ///< Sanctified Ground
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); // 1s
                break;
            case 156862: ///< Drain Soul Cosmetic
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 169682: ///< Azakkel visual pillar
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 153775: ///< Summon Imp
            case 164127: ///< Summon Pyromaniac
            case 164080: ///< Summon FelGuard
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(10); ///< from 15.0f (RangeEntry.ID 11) to 40.0f
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 153430: ///< Areatrigger Damage
                spellInfo->Effects[0].Amplitude = 2000;
            case 119975: ///< Conversion
                spellInfo->AttributesEx8 |= SPELL_ATTR8_AURA_SEND_AMOUNT;
                break;
            case 178533: ///< Horde Reward
            case 178531: ///< Alliance Reward
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(27);  ///< 50y
                break;
            case 115294: ///< Mana Tea
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_OBS_MOD_POWER;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(36); ///< 1s
                break;
            case 167625: ///< Blood feather
                spellInfo->Effects[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
                spellInfo->Effects[0].TriggerSpell = 167626; ///< Blood Feather
                break;
            case 167687: ///< Solar Breath
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                break;
            case 167629: ///< Blaze Of Glory
                spellInfo->Effects[1].TargetB = TARGET_UNIT_DEST_AREA_ENTRY;
                break;
            case 165907: ///< Earthrending Slam
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                break;
            case 124694: ///< Way of the Grill
            case 125584: ///< Way of the Wok
            case 125586: ///< Way of the Pot
            case 125587: ///< Way of the Steamer
            case 125588: ///< Way of the Oven
            case 125589: ///< Way of the Brew
                spellInfo->Effects[EFFECT_0].BasePoints = 8;
                break;
            case 161299: ///< Impact Spit
                spellInfo->Effects[0].TriggerSpell = 161304;
                break;
            case 166452: ///< Tormented Soul
                spellInfo->Effects[1].Effect = 0;
                break;
            case 164042: ///< Taste of Iron Game Aura
                spellInfo->Effects[2].Effect = 0;
                spellInfo->Effects[3].Effect = 0;
                break;
            case 133123: ///< Arcane Barrage
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(10); ///< from 15.0f (RangeEntry.ID 11) to 40.0f
                break;
            case 144757: /// Increased All Resist 05
                spellInfo->AttributesEx11 &= ~SPELL_ATTR11_SCALES_WITH_ITEM_LEVEL;
                break;
            case 170893:///< Kronus: Fracture
            case 177607:///< Fangraal: Entangling Roots
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_TARGET_ENEMY;
                break;
            case 165712:///< Stormshield Gladiator: Devotion Aura
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AREA_AURA_FRIEND;
                break;
            case 176172:///< Ancient Inferno: Molten Firestorm
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST_RANDOM;
                break;
            case 117032:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_DEST_DEST_RANDOM;
                spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(29); ///< 6 yards
                break;
            case 175093:///< Alliance Reward (Ashran events)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].BasePoints = 50;
                break;
            case 174094:///< Horde Reward (Ashran events)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].MiscValue = 1681;
                break;
            case 108683:///< Fire and Brimstone
                /// I guess spellmod type is failed here because of -75% damage
                spellInfo->Effects[EFFECT_5].MiscValue = SPELLMOD_DAMAGE;
                spellInfo->Effects[EFFECT_5].BasePoints = -65;
                /// Hack Fix, try to add visual effect for Chaos Bolt with Fire and Brimstone (157701) from Chaos Bolt (116858)
                /// TODO !!! Didn't help !!! Chaos Bolt with FaB doesn't have visual model !!!
                spellInfo->Effects[EFFECT_7].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_7].ApplyAuraName = SPELL_AURA_CHANGE_VISUAL_EFFECT;
                spellInfo->Effects[EFFECT_7].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_7].BasePoints = 116858;
                spellInfo->Effects[EFFECT_7].ValueMultiplier = 116858;
                spellInfo->Effects[EFFECT_7].MiscValue = 157701;
                spellInfo->Effects[EFFECT_7].MiscValueB = 116858;
                break;
            /// Second try to fix, didn't help too.
            case 157701:///< Chaos Bolt
                spellInfo->SpellVisual[0] = 45351; ///< Set a visual id from working Chaos Bolt.
                break;
            case 153626:///< Arcane Orb
                spellInfo->SpellVisual[0] = 42341;
                break;
            case 162472:///< Earth Breaker (Vul'gor)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 162304:///< Earth Breaker (Summon - Vul'gor)
                spellInfo->Effects[EFFECT_0].Effect = 0;
                break;
            case 159995:///< Chain Hurl (Kargath)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 30.0f;
                spellInfo->Effects[EFFECT_0].MiscValueB = 250;
                break;
            case 160061:///< Chain Hurl (Kargath)
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 30.0f;
                spellInfo->Effects[EFFECT_0].MiscValue = 250;
                spellInfo->Effects[EFFECT_0].MiscValueB = 250;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 151991:///< Chain Grip (Gorian Guardsman)
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 30.0f;
                spellInfo->Effects[EFFECT_0].MiscValue = 250;
                spellInfo->Effects[EFFECT_0].MiscValueB = 250;
                break;
            case 159113:///< Impale (Kargath)
                spellInfo->ChannelInterruptFlags = 0;
                break;
            case 159265:///< Blade Dance (charge visual - Kargath)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 158986:///< Berserker Rush (Kargath)
            case 162497:///< On the Hunt (Ravenous Bloodmaw - Kargath)
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 160953:///< Fire Bomb (Iron Bomber - Kargath)
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CASTABLE_WHILE_ON_VEHICLE;
                break;
            case 159414:///< Mauling Brew (Drunken Bileslinger - Kargath)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 159412:///< Mauling Brew (Drunken Bileslinger - Kargath)
                spellInfo->AttributesEx3 &= ~SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                break;
            case 161218:///< Maul (Ravenous Bloodmaw - Kargath)
                spellInfo->Effects[EFFECT_0].Effect = 0;
                spellInfo->Effects[EFFECT_2].Effect = 0;
                break;
            case 163408:///< Heckle (Kargath)
                spellInfo->CasterAuraSpell = 0;
                break;
            case 156127:///< Meat Hook (The Butcher)
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 100.0f;
                break;
            case 156160:///< Bounding Cleave (The Butcher)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_2].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_2].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 156171:///< Bounding Cleave (The Butcher)
            case 162398:///< Expel Magic: Arcane - Missile (Ko'ragh)
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 156172:///< Bounding Cleave (The Butcher)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 156157:///< Cleave (The Butcher)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 163042:///< Pale Vitriol
                spellInfo->Attributes |= SPELL_ATTR0_CASTABLE_WHILE_DEAD;
                break;
            case 166225:///< Boar's Rush (Krush)
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(23); ///< 40 yards
                break;
            case 174465:///< Unstoppable Charge (Iron Flame Technician)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 174473:///< Corrupted Blood (Iron Blood Mage)
                spellInfo->MaxAffectedTargets = 5;
                break;
            case 163113:///< Withering (Living Mushroom - Brackenspore)
            case 163124:///< Withering (Rejuvenating Mushroom - Brackenspore)
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PERIODIC_DAMAGE_PERCENT;
                break;
            case 159996:///< Infested Spores (Brackenspore)
            {
                for (auto l_Iter : spellInfo->SpellPowers)
                {
                    ((SpellPowerEntry*)l_Iter)->Cost = 500;
                    ((SpellPowerEntry*)l_Iter)->PowerType = POWER_RAGE;
                }

                break;
            }
            case 163242:///< Infested Spores - triggered (Brackenspore)
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_2;
                break;
            case 160446:///< Spore Shooter - summon (Brackenspore)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                break;
            case 162346:///< Crystalline Barrage (Tectus)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 162371:///< Crystalling Barrage - Summon (Tectus)
            case 163208:///< Fracture - Missile (Tectus)
            case 174856:///< Knockback (Ko'ragh)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 117624:///< Suicide No Blood No Logging (Tectus)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 162475:///< Tectonic Upheaval (Tectus)
                spellInfo->InterruptFlags = 0;
                spellInfo->ChannelInterruptFlags = 0;
                break;
            case 166185:///< Rending Slash (Highmaul Conscript)
            case 158026:///< Enfeebling Roar - Debuff (Phemos - Twin Ogron)
            case 163134:///< Nullification Barrier - Abosrb (Ko'ragh)
            case 161345:///< Suppression Field - DoT (Ko'ragh)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 166199:///< Arcane Volatility (Gorian Arcanist)
            case 158521:///< Double Slash (Phemos - Twin Ogron)
            case 175598:///< Devouring Leap (Night-Twisted Devout)
            case 172747:///< Expel Magic: Frost - AreaTrigger (Ko'ragh)
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 158419:///< Pulverize - Third AoE (Pol - Twin Ogron)
                spellInfo->CastTimeEntry = sSpellCastTimesStore.LookupEntry(4); ///< 1s
                break;
            case 158241:///< Blaze - DoT (Phemos - Twin Ogron)
            case 174405:///< Frozen Core - DoT (Breaker Ritualist - Frost)
            case 173827:///< Wild Flames - DoT (Breaker Ritualist - Fire)
            case 161242:///< Caustic Energy - DoT (Ko'ragh)
            case 172813:///< Expel Magic: Frost - Decrease Speed (Ko'ragh)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 172685:///< Expel Magic: Fire (Ko'ragh)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                break;
            case 162397:///< Expel Magic: Arcane (Ko'ragh)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_DEST_AREA_ALLY;
                break;
            case 161376:///< Volatile Anomalies - Missile 1 (Ko'ragh)
            case 161380:///< Volatile Anomalies - Missile 2 (Ko'ragh)
            case 161381:///< Volatile Anomalies - Missile 3 (Ko'ragh)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_DEST_CASTER_RANDOM;
                break;
            case 162595:///< Suppression Field - Silence (Ko'ragh)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); ///< 2s
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 110744:///< Divine Star - should be 2 sec -- WTF Blizz ?
            case 122121:
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(66);
                spellInfo->Effects[0].TargetA = SELECT_TARGET_SELF;
                spellInfo->ExplicitTargetMask = spellInfo->_GetExplicitTargetMask();
                break;
            case 165096:///< Ogreic Landing
                spellInfo->Effects[1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 164868:///< Ogreic Landing
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 164850:///< Nature Channeling (Cosmetic)
            case 175581:///< Void Touch
            case 175915:///< Acid Breath (Drov the Ruiner)
            case 139550:///< Torment
            case 138742:///< Chocking Sands
            case 99212: ///< Stormfire, Item - Shaman T12 Enhancement 4P Bonus
            case 116000:///< Voodoo Dolls
            case 38112: ///< Magic Barrier, Lady Vashj
            case 70602: ///< Corruption
            case 48278: ///< Paralyze
            case 65584: ///< Growth of Nature (Freya)
            case 64381: ///< Strength of the Pack (Auriaya)
            case 166289:///< Arcane Force (Gorian Arcanist)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                break;
            case 150055:///< Volcanic Tantrum
            case 149963:///< Shatter Earth
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(27); // 3 sec
                break;
            case 150004:///< Magma Barrage
                spellInfo->InterruptFlags = SPELL_INTERRUPT_FLAG_MOVEMENT;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 150306;
                break;
            case 102401:///< Wild Charge (Unform spell)
                spellInfo->Effects[EFFECT_1].ValueMultiplier = 25.0f;
                spellInfo->Effects[EFFECT_1].MiscValue = 50;
                break;
            case 94954: ///< Heroic Leap
                spellInfo->Effects[EFFECT_1].ValueMultiplier = 0;
                break;
            case 167718:///< Item - Monk T17 Mistweaver 4P Bonus
            case 31220:///< Sinister Calling
            case 17007:///< Leader of the Pack
            case 16864:///< Omen of Clarity
            case 16961:///< Primal Fury
            case 159232:///< Ursa Major
            case 159362:///< Blood Craze
            case 76672:///< Mastery: Hand of Light
            case 55447:///< Glyph of Flame Shock
                spellInfo->AttributesEx3 |= SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED;
                break;
            case 30814: ///< Mental Quickness
                spellInfo->Effects[EFFECT_4].Effect = 0;
                break;
            case 149959: // Earth Shatter
            case 149968:
            case 149969:
            case 149970:
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->ExplicitTargetMask = spellInfo->_GetExplicitTargetMask();
                break;
            case 175975:///< Genesis
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10);  ///< 30y
                break;
            case 77442: ///< Focus
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[0].ApplyAuraName = 0;
                break;
            case 162537: ///< Poisoned Ammo
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
                break;
            case 162543: ///< Poisoned Ammo (triggered)
                spellInfo->Speed = 0.0f;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                break;
            case 162546: ///< Frozen Ammo (triggered)
                spellInfo->Speed = 0.0f;
                break;
            case 121818: ///< Stampede
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[EFFECT_1].TargetB = 0;
                break;
            case 45470: ///< Death Strike (no heal bonus in SPELL_DAMAGE_CLASS_NONE)
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MELEE;
                break;
            case 117962: ///< Crackling Jade Shock
                spellInfo->Effects[0].Mechanic = MECHANIC_KNOCKOUT;
                break;
            case 154302: ///< Seal Conduit (first)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 154900: ///< Seal Conduit (second)
            case 160425: ///< Call of the Tides (Brackenspore)
                spellInfo->MaxAffectedTargets = 2;
                break;
            case 174627: ///< Fixate (Phantasmal Weapon)
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_1;
                break;
            case 157278: ///< Awaken Runestone (Imperator Mar'gok)
                spellInfo->InterruptFlags = 0;
                spellInfo->ChannelInterruptFlags = 0;
                spellInfo->AuraInterruptFlags = 0;
                break;
            case 157763: ///< Fixate (Imperator Mar'gok)
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                break;
            case 137143:
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                break;
            case 156734: ///< Destructive Resonance - Summon (Imperator Mar'gok)
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 157265: ///< Volatile Anomalies (Imperator Mar'gok)
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
                spellInfo->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
                break;
            case 158512: ///< Volatile Anomalies (Imperator Mar'gok)
            case 159158: ///< Volatile Anomalies (Imperator Mar'gok)
            case 159159: ///< Volatile Anomalies (Imperator Mar'gok)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(18);    ///< 15 yards
                break;
            case 156799: ///< Destructive Resonance (Other - Imperator Mar'gok)
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 158639: ///< Orbs of Chaos (1 - Imperator Mar'gok)
                for (uint8 l_I = SpellEffIndex::EFFECT_8; l_I < SpellEffIndex::MAX_EFFECTS; ++l_I)
                    spellInfo->Effects[l_I].Effect = 0;
                break;
            case 154901: ///< Seal Conduit (third)
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 157347: ///< Fiery Charge
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[1].Effect = 0;   ///< Need to be scripted
                break;
            case 77756: ///< Lava Surge
                spellInfo->Effects[EFFECT_0].TriggerSpell = 77762;
                break;
            case 77762: ///< Lava Surge
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1); ///< 10s
                break;
            case 8188:   ///< Magma Totem Passive
            case 5672:   ///< Healing Streams
            case 114941: ///< Healing Tide
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
                spellInfo->Effects[EFFECT_0].Amplitude = 2000;
                break;
            case 154150: ///< Energize
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 45477: ///< Icy touch
                spellInfo->Effects[EFFECT_0].AttackPowerMultiplier = 0.319f;
                break;
            case 65075:  ///< Tower of Flames
            case 65077:  ///< Tower of Frost
            case 64482:  ///< Tower of Life
            case 55076:  ///< Sun Scope
            case 60023:  ///< Scourge Banner Aura
            case 66118:  ///< Leeching Swarm
            case 137502: ///< Growing Fury
            case 58105:  ///< Power of Shadron
            case 61248:  ///< Power of Tenebron
            case 61251:  ///< Power of Vesperon
                spellInfo->AttributesEx4 |= SPELL_ATTR4_NOT_STEALABLE;
                break;
            case 132205: ///< Sha of Anger Bonus
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 137200: ///< Blessed Loa Spirit
                spellInfo->Effects[0].TargetA = TARGET_DEST_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_DEST_DEST_RIGHT;
                break;
            case 8936: ///< Regrowth
                spellInfo->Effects[0].BasePoints = 1;
                break;
            case 136339: ///< Lightning Tether
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 136350: ///< Lightning Tether (periodic)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 136340: ///< Storm Cloud
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 136345: ///< Storm Cloud (periodic)
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                break;
            case 121673: ///< Fire Shot
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 136050: ///< Malformed Blood
                spellInfo->Effects[1].BasePoints = int32(spellInfo->Effects[1].BasePoints * 2.85f);
                break;
            case 136521: ///< QuickSand
            case 136878: ///< QuickSand
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 137967: ///< Twisted Fate
                spellInfo->Effects[0].TargetA =  TARGET_UNIT_TARGET_ANY;
                break;
            case 134447: ///< Submerge
            case 122841: ///< Sha Active Sky
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(21); ///< Infinite
                break;
            case 134398: ///< Slime Trail (aura)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(27); ///< 3s
                break;
            case 134531: ///< Web Thread
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_1;
                break;
            case 132413: ///< Shadow Bulwark
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 152150:///< Death from Above (periodic dummy)
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->Effects[5].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->PreventionType = SpellPreventionMask::Pacify;
                break;
            case 178236:///< Death from Above (jump dest)
                spellInfo->Effects[0].TargetB = TARGET_DEST_TARGET_BACK;
                spellInfo->Effects[0].ValueMultiplier = 40.0f;
                break;
            case 139498: ///< Web Spray
                spellInfo->Effects[1].TriggerSpell = 0;
                break;
            case 138359: ///< Jump to Boss Platform
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 51128: ///< Killing Machine
                spellInfo->ProcChance = 0;
                break;
            case 137641: ///< Soul Fragment
                spellInfo->Effects[1].BasePoints = int32(spellInfo->Effects[1].BasePoints * 2.7f);
                break;
            case 174597:///< Ruthlessness (passive aura)
                spellInfo->Effects[0].Effect = 0;
                break;
            case 137650: ///< Shadowed Soul
                spellInfo->Effects[0].BasePoints = 3;
                spellInfo->Effects[1].BasePoints = 3;
                break;
            case 159234: ///< Thunderlord
            case 159675: ///< Warsong
            case 159676: ///< Frostwolf
            case 173322: ///< BleedingHollow
            case 159238: ///< Shattered Bleed
            case 118334: ///< Dancing Steel (agility)
            case 118335: ///< Dancing Steel (strength)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_ENCHANT_STACK;
                break;
            case 1329: ///< Mutilate
                spellInfo->AttributesEx3 &= ~SPELL_ATTR3_CANT_TRIGGER_PROC;
                break;
            case 113828: ///< Healing Touch (treant)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 142421: ///< Swiftmend (treant)
                spellInfo->Effects[1].TargetA = TARGET_DEST_TARGET_ANY;
                break;
            case 156910: ///< Beacon of Faith
            case 53563: ///< Beacon of Light
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 53651: ///< Beacon of Light
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(21); ///< -1ms
                break;
            case 129869: ///< Strike from the Heavens
                spellInfo->Effects[0].TriggerSpell = 129639;
                break;
            case 165376: ///< Enlightenment
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_CRIT_PCT;
                break;
            case 139834: ///< Cinders (summon)
                spellInfo->Effects[0].TargetA = TARGET_DEST_TARGET_ENEMY;
                break;
            case 140620: ///< Fungi Spores
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 140621: ///< Fungi Spores (damage)
            case 140619: ///< Crystal Barbs
            case 140598: ///< Fungal Explosion (damage)
            case 139850: ///< Acid Rain (damage)
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 137575: ///< Frostbite (aura)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 136990: ///< Frostbite
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 136917: ///< Biting Cold
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 136467: ///< Lingering Presence
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TriggerSpell = 0;
                break;
            case 137131: ///< Reckless Charge
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 137133: ///< Reckless Charge
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 137117: ///< Reckless Charge (Rolling)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 140138: ///< Nether Tear
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 163294: ///< Darkmoon Card of Draenor
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 138652: ///< Eruption
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 140946: ///< Dire Fixation
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 136644: ///< Venomous Effusion
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[2].Effect = 0;
                spellInfo->Effects[3].Effect = 0;
                break;
            case 59052: ///< Freezing Fog
                spellInfo->ProcChance = 45;
                break;
            case 136654: ///< Rending Charge
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(153); ///< 60 yards
                break;
            case 136740: ///< Double Swipe (back)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_BACK;
                break;
            case 136797: ///< Dino Mending
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 139900: ///< Stormcloud
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 137633: ///< Crystal Shell (damage absorb)
            case 137648: ///< Crystal Shell (heal absorb)
            case 134916: ///< Decapitate (debuff) (Lei Shen - Throne of Thunder)
                spellInfo->Attributes |= SPELL_ATTR0_CANT_CANCEL;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 134030: ///< Kick Shell
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                break;
            case 134476: ///< Rockfall (large damage)
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 135103: ///< Drain the Weak
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 137313: ///< Lightning Storm
                spellInfo->Effects[1].TriggerSpell = 0;
                break;
            case 157096: ///< Empowered Demons
                spellInfo->Effects[2].BasePoints = 0;
                spellInfo->Effects[3].BasePoints = 0;
                break;
            case 138732: ///< Ionization
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                break;
            case 137422: ///< Focused Lightning (eyes)
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 140555: ///< Lightning Storm (cosmetic - not sure)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(37); ///< 1ms
                break;
            case 137145: ///< Conductive Water (summon)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 139364: ///< Spirit Lantern
                spellInfo->Effects[1].Effect = 0;
                break;
            case 139461: ///< Spirit Light
                spellInfo->Effects[0].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[2].TargetA = TARGET_DEST_TARGET_ENEMY;
                spellInfo->Effects[2].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 137905: ///< Lightning Diffusion
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(7); ///< 2 yards
                break;
            case 138470: ///< Conductive Water (Damage taken)
            case 138006: ///< Electrified Waters (periodic damage)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30 yards
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 138002: ///< Conductive Water (Damage - Healing done)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30 yards
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30 yards
                spellInfo->Effects[1].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[2].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30 yards
                spellInfo->Effects[2].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[2].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[3].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30 yards
                spellInfo->Effects[3].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[3].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 137261: ///< Lightning Storm (damage)
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                break;
            case 137530: ///< Focused Lightning Conduction
            case 138133: ///< Lightning Fissure Conduction
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30 yards
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 137484: ///< Lightning Fissure (periodic)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 137485: ///< Lightning Fissure (damage)
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(8); ///< 5 yards
                break;
            case 137194: ///< Focused Lightning
                spellInfo->CastTimeEntry = sSpellCastTimesStore.LookupEntry(5); ///< 2.000
                break;
            case 139218: ///< Storm Weapon
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 113504: ///< Wind Sword
                spellInfo->Effects[0].BasePoints *= 10;
                break;
            case 119691: ///< Heavy Dynamite
                spellInfo->Effects[0].BasePoints *= 31;
                break;
            case 89909: ///< Water Spout
                spellInfo->Effects[0].BasePoints *= 22;
                break;
            case 118600: ///< Chi Torpedo
                spellInfo->Effects[0].BasePoints *= 10;
                break;
            case 118592: ///< Spinning Crane Kick
                spellInfo->Effects[0].BasePoints = urand(180, 230);
                break;
            case 48505: ///< Starfall
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[1].BasePoints = 20;
                break;
            case 119539: ///< Chi Torpedo
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 152261: ///< Holy Shield
                spellInfo->Effects[2].Effect = 0;
                break;
            case 139139: ///< Insanity
                spellInfo->ProcChance = 0;
                break;
            case 32409: ///< Shadow Word: Death (triggered)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                spellInfo->Effects[0].ApplyAuraName = 0;
                break;
            /// Curiously, these spells grants Intellect ...
            case 146046: ///< Expanded Mind
            case 148897: ///< Extravagant Visions
            case 148906: ///< Toxic Power
            case 148908: ///< Mark of Salvation
                spellInfo->Effects[0].BasePoints = 963;
                break;
            case 53390: ///< Tidal Waves
                spellInfo->Effects[0].BasePoints = -20;
                spellInfo->Effects[1].BasePoints = 30;
                break;
            case 146951: ///< Glyph of rapid rolling
            case 147364: ///< Glyph of rapid rolling
                spellInfo->ProcFlags = 0;
                break;
            case 164991: ///< Entangling Energy
            case 15286: ///< Vampiric Embrace
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 119403: ///< Glyph of Explosive Trap
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 149575: ///< Explosive Trap (knock back)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[1].TargetA = TARGET_DEST_DEST;
                break;
            case 13813:  ///< Explosive Trap
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(8); ///< 5 yards
                break;
            case 108287: ///< Totemic Projection
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 115176: ///< Zen Meditation
                spellInfo->ProcCharges = 5;
                spellInfo->Effects[0].BasePoints = 5;
                break;
            case 137639: ///< Storm, Earth and Fire
                spellInfo->AttributesCu &= ~(SPELL_ATTR0_CU_NEGATIVE_EFF1|SPELL_ATTR0_CU_NEGATIVE_EFF0);
                spellInfo->AttributesEx3 |= SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED;
                break;
            case 138130: ///< Storm, Earth and Fire (for spirits)
                spellInfo->Effects[0].Effect = 0;
                break;
            case 5740:   ///< Rain of Fire
            case 104232: ///< Rain of Fire (Aftermath)
                spellInfo->Effects[0].TargetB = 0;
                break;
            /*case 130393: ///< Blink Strike
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< 30y
                break;*/
            case 135299: ///< Ice Trap (snare)
            case 140023: ///< Ring of Peace (dummy)
            case 138234: ///< Lightning Storm (damage) (Lei Shen trash - Throne of Thunder)
            case 81782:  ///< Power Word: Barrier (buff)
            case 139485: ///< Dark Winds
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(285); ///< 1s
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                break;
            case 134821: ///< Discharged Energy (Lei Shen - Throne of Thunder)
            case 135153: ///< Crashing Thunder (DoT) (Lei Shen - Throne of Thunder)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(285); ///< 1s
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER | SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                break;
            case 137252: ///< Overcharge (Lei Shen - Throne of Thunder)
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(35); ///< Min: 0, Max: 35
                spellInfo->Effects[EFFECT_0].RadiusEntry = sSpellRadiusStore.LookupEntry(21); ///< 35
                break;
            case 103965: ///< Metamorphosis (override auras)
                ///< All this effects are Override with old spell id
                spellInfo->Effects[4].Effect = SPELL_EFFECT_NONE; ///< No more use (Void Ray : 115422)
                spellInfo->Effects[5].Effect = SPELL_EFFECT_NONE; ///< No more use (Aura of Enfeeblement : 116198)
                spellInfo->Effects[6].Effect = SPELL_EFFECT_NONE; ///< No more use (Aura of the Elements : 116202)
                spellInfo->Effects[7].Effect = SPELL_EFFECT_NONE; ///< No more use (Sleep : 104045)
                spellInfo->Effects[8].Effect = SPELL_EFFECT_NONE; ///< No more use (Provocation : 97827)
                spellInfo->Effects[9].Effect = SPELL_EFFECT_NONE;
                spellInfo->Effects[11].Effect = SPELL_EFFECT_NONE;
                spellInfo->Effects[14].Effect = SPELL_EFFECT_NONE; ///< No more use (Drain Life : 103990)
                spellInfo->Effects[15].Effect = SPELL_EFFECT_NONE;
                break;
            case 5857:   ///< Hellfire damage spell
                spellInfo->AttributesEx3 = 0;
                spellInfo->AttributesEx4 = 0;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
           case 77219: ///< Mastery: Master Demonologist
                spellInfo->Effects[1].SpellClassMask[0] |= 0x1040;
                spellInfo->Effects[1].SpellClassMask[3] |= 0x40000000;
                spellInfo->Effects[2].SpellClassMask[0] |= 0x1040;
                spellInfo->Effects[2].SpellClassMask[3] |= 0x40000000;
                break;
            case 145518: ///< Genesis
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 119890: ///< Death Blossom
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(139); ///< 40y
                break;
            case 120629: ///< Huddle in Terror
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 119956: ///< Dread Spray (visual)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 119958: ///< Dread Spray
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(285); ///< 1s
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_1;
                spellInfo->Effects[0].Effect = 0;
                break;
            case 119414: ///< Breath of Fear
                spellInfo->Effects[2].Effect = 0;
                break;
            case 107145: ///< Wall of Light activation
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                break;
            case 119312: ///< Conjure Terror Spawn (01)
            case 119370: ///< Conjure Terror Spawn (02)
            case 119371: ///< Conjure Terror Spawn (03)
            case 119372: ///< Conjure Terror Spawn (04)
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 117865: ///< Wall of Light
                spellInfo->Effects[0].Effect = 0;
                break;
            case 126848: ///< Ritual of Purification
                spellInfo->Effects[0].BasePoints = -10;
                spellInfo->Effects[1].BasePoints = -10;
                break;
            case 125706: ///< Channeling Corruption
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 125713: ///< Channeling Corruption (triggered)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 125736: ///< Night Terrors (missile)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 117230: ///< Overpowered
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                break;
            case 117988: ///< Defiled Ground
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[1].Mechanic = MECHANIC_NONE;
                break;
            case 117052: ///< Corruption Sha
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetB = 0;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[2].TargetB = 0;
                break;
            case 161209: ///< Vileblood Serum (missile)
                spellInfo->Effects[0].TargetA = TARGET_DEST_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 161199: ///< Debilitating Fixation (Kyrak)
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[1].ApplyAuraName = 0;
                spellInfo->ChannelInterruptFlags |= 0x1008;
                break;
            case 155498: ///< Rejuvenating Serum
            case 161203: ///< Rejuvenating Serum (Kyrak)
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MAGIC;
                break;
            case 152742:///< Fiery Boulder - AoE impact damage
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 152867:///< Heat Wave (Create AT)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 150759:///< Ferocious Yell
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 164616:///< Channel Flames (DoT)
            case 167739:///< Scorching Aura (Debuff)
            case 153227:///< Burning Slag (DoT)
            case 150784:///< Magma Eruption (DoT)
            case 161288:///< Vileblood Serum (DoT)
            case 161833:///< Noxious Spit (DoT)
            case 157420:///< Fiery Trail (DoT)
            case 155057:///< Magma Pool (DoT)
            case 166730:///< Burning Bridge (DoT)
            case 176037:///< Noxious Spit (DoT)
            case 161635:///< Molten Bomb
            case 159311:///< Flame Jet
            case 161517:///< Splitting Breath (DoT)
            case 176146:///< Volcanic Fallout
            case 159413:///< Mauling Brew
            case 175654:///< Rune of Disintegration
            case 163046:///< Pale Vitriol
            case 163140:///< Mind Fungus
            case 163590:///< Creeping Moss (damage)
            case 165494:///< Creeping Moss (healing)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); // 2s
                break;
            case 162370:///< Crystalline Barrage (DoT)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                break;
            case 163312: ///< Raving Assault
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT;
                break;
            case 154996: ///< Engulfing Fire (searcher)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 155721: ///< Black Iron Cyclone
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[0].ApplyAuraName = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->InterruptFlags &= ~SPELL_INTERRUPT_FLAG_MOVEMENT;
                break;
            case 127731: ///< Corruption Sha (triggered)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 123244: ///< Hide
                spellInfo->Effects[0].Effect = 0;
                spellInfo->Effects[2].Effect = 0;
                break;
            case 124009: ///< Tiger Lust
            case 130793: ///< Provoke
            case 123996: ///< Crackling Tiger Lightning
                spellInfo->StartRecoveryTime = 1500;
                spellInfo->StartRecoveryCategory = 133;
                spellInfo->Effects[3].Effect = 0;
                break;
            case 57761: ///< Brain Freeze
                spellInfo->Effects[0].BasePoints = -100;
                spellInfo->Effects[1].BasePoints = -100;
                break;
            case 127424: ///< Jade Fire
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 45257: ///< Using Steam Tonk Controller
            case 45440: ///< Steam Tonk Controller
            case 49352: ///< Crashin' Thrashin' Racer Controller
            case 75111: ///< Blue Crashin' Thrashin' Racer Controller
            case 60256: ///< Collect Sample
                /// Crashes client on pressing ESC (Maybe because of ReqSpellFocus and GameObject)
                spellInfo->AttributesEx4 &= ~SPELL_ATTR4_CAN_CAST_WHILE_CASTING;
                break;
            case 12540:
            case 13579:
            case 24698:
            case 28456:
            case 29425:
            case 34940:
            case 36862:
            case 38863:
            case 52743: ///< Head Smack
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER;
                break;
            case 21987: ///< Lash of Pain
            case 58563: ///< Assassinate Restless Lookout
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET;
                break;
            case 120751: ///< Pandashan's Dragon Gun
            case 120876:
            case 120964:
            case 124347:
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_54;
                break;
            case 26029: ///< Dark Glare
            case 37433: ///< Spout
            case 43140: ///< Flame Breath
            case 43215: ///< Flame Breath
            case 70461: ///< Coldflame Trap
            case 72133: ///< Pain and Suffering
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_CONE_LINE;
                break;
            case 24340:  ///< Meteor
            case 26558:  ///< Meteor
            case 28884:  ///< Meteor
            case 36837:  ///< Meteor
            case 38903:  ///< Meteor
            case 41276:  ///< Meteor
            case 57467:  ///< Meteor
            case 26789:  ///< Shard of the Fallen Star
            case 31436:  ///< Malevolent Cleave
            case 35181:  ///< Dive Bomb
            case 40810:  ///< Saber Lash
            case 43267:  ///< Saber Lash
            case 43268:  ///< Saber Lash
            case 42384:  ///< Brutal Swipe
            case 45150:  ///< Meteor Slash
            case 64688:  ///< Sonic Screech
            case 72373:  ///< Shared Suffering
            case 71904:  ///< Chaos Bane
            case 70492:  ///< Ooze Eruption
            case 88942:  ///< Meteor Slash
            case 82935:  ///< Caustic Slime
            case 86825:  ///< Blackout
            case 77679:  ///< Scorching Blast
            case 98474:  ///< Flame Scythe
            case 105069: ///< Seething Hate
            case 103414: ///< Stomp
            case 103527: ///< Void Diffusion dmg
            case 106375: ///< Unstable Twilight
            case 107439: ///< Twilight Barrage
            case 106401: ///< Twilight Onslaught
            case 155152: ///< Prismatic Crystal damage
            case 172073: ///< Meteoric Earthspire (Rokka & Lokk)
            case 135703: ///< Static Shock (Lei Shen - Throne of Thunder)
            case 136366: ///< Bounding Bolt (Lei Shen - Throne of Thunder)
                /// ONLY SPELLS WITH SPELLFAMILY_GENERIC and EFFECT_SCHOOL_DAMAGE
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                break;
            case 114093: ///< Windlash Off-Hand
            case 114089: ///< Windlash
            case 18500:  ///< Wing Buffet
            case 33086:  ///< Wild Bite
            case 49749:  ///< Piercing Blow
            case 52890:  ///< Penetrating Strike
            case 53454:  ///< Impale
            case 59446:  ///< Impale
            case 64777:  ///< Machine Gun
            case 65239:  ///< Machine Gun
            case 65919:  ///< Impale
            case 74439:  ///< Machine Gun
            case 63278:  ///< Mark of the Faceless (General Vezax)
            case 62544:  ///< Thrust (Argent Tournament)
            case 64588:  ///< Thrust (Argent Tournament)
            case 66479:  ///< Thrust (Argent Tournament)
            case 68505:  ///< Thrust (Argent Tournament)
            case 62709:  ///< Counterattack! (Argent Tournament)
            case 62626:  ///< Break-Shield (Argent Tournament, Player)
            case 64590:  ///< Break-Shield (Argent Tournament, Player)
            case 64342:  ///< Break-Shield (Argent Tournament, NPC)
            case 64686:  ///< Break-Shield (Argent Tournament, NPC)
            case 65147:  ///< Break-Shield (Argent Tournament, NPC)
            case 68504:  ///< Break-Shield (Argent Tournament, NPC)
            case 62874:  ///< Charge (Argent Tournament, Player)
            case 68498:  ///< Charge (Argent Tournament, Player)
            case 64591:  ///< Charge (Argent Tournament, Player)
            case 63003:  ///< Charge (Argent Tournament, NPC)
            case 63010:  ///< Charge (Argent Tournament, NPC)
            case 68321:  ///< Charge (Argent Tournament, NPC)
            case 72255:  ///< Mark of the Fallen Champion (Deathbringer Saurfang)
            case 62775:  ///< Tympanic Tantrum (XT-002 encounter)
            case 102598: ///< Void Strike, Infinite Warden
            case 154448: ///< Shrapnel Nova (Orebender Gor'Ashan)
            case 163047: ///< Paleobomb (Night-Twisted Cadaver - The Butcher)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 64422: ///< Sonic Screech (Auriaya)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_SHARE_DAMAGE;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 156791: ///< Call Adds
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(6);    ///< 100yards
                break;
            case 176544: ///< Fixate (Skyreach)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_TARGET_ANY;
                break;
            case 152973: ///< Protective Barrier (Skyreach)
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 156634: ///< Four winds (Skyreach)
            case 156636: ///< Four winds (Skyreach)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ANY;
                break;
            case 166623: ///< Four winds (Skyreach)
            case 166664: ///< Four winds (Skyreach)
            {
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ANY;
                SpellDurationEntry const* durationIndex = sSpellDurationStore.LookupEntry(8);
                if (!durationIndex)
                    break;
                spellInfo->DurationEntry = durationIndex;
                break;
            }
            case 81744: ///< Horde    (rated battleground faction override)
            case 81748: ///< Alliance (rated battleground faction override)
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_2].Effect = SPELL_EFFECT_APPLY_AURA;
                break;
            case 159226: ///< Solar storm (Skyreach)
            case 153759: ///< WindWalls (Skyreach)
            case 153139: ///< Four winds (Skyreach)
            case 158441: ///< Solar Zone (Skyreach)
            case 153907: ///< Dervish (Skyreach)
            case 156841: ///< Storm (Skyreach)
            case 72293:  ///< Mark of the Fallen Champion (Deathbringer Saurfang)
            case 159178: ///< Open Wounds (Kargath Bladefist)
            case 156152: ///< Gushing Wounds (The Butcher)
            case 158553: ///< Crush Armor (Imperator Mar'gok)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE_EFF0;
                break;
            case 151272: ///< Wheel of Pain Knockback
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 101184: ///< Leyara's Locket
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_2].Effect = 0;
                break;
            case 116711: ///< Draw Flame
                spellInfo->ChannelInterruptFlags = 0x0;
                spellInfo->AuraInterruptFlags = 0x0;
                break;
            case 118443: ///< Instantly Death
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 116989: ///< Overloaded Missile
            case 117220: ///< Overloaded Triggered
            case 118430: ///< Core Beam
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 129724: ///< Grasping Energy Tendrils
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 117960: ///< Celestial Breathe
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            case 118018: ///< Draw Power (Summons)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[3].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[4].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[5].TargetA = TARGET_UNIT_CASTER;
                break;
            case 116661: ///< Draw Power (lightning damage for activated focus)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 117558: ///< Coalescing Shadows
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); ///< 10 yards
                break;
            case 117829: ///< Cowardice (DoT)
            case 117006: ///< Devastating Arc
            case 119775: ///< Reaching Attack
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 117708: ///< Maddening Shout
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                break;
            case 17076: ///< Glyph of Rejuvenation
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 77495: ///< Mastery : Harmony
                spellInfo->Effects[0].SpellClassMask[0] |= 0x80;
                break;
            case 118047: ///< Pillage
                spellInfo->Effects[0].Effect = SPELL_EFFECT_CHARGE;
                spellInfo->Effects[0].TriggerSpell = 0;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 159623: ///< Leap of Faith (Glyph of Restored Faith)
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_RAID;
                break;
            case 114108: ///< Soul of the Forst (Resto)
                spellInfo->ProcFlags = 0;
                break;
            case 73325: ///< Leap of Faith
                spellInfo->OverrideSpellList.push_back(159623); ///< Leap of Faith (Glyph of Restored Faith)
                break;
            case 688: ///< Summon Imp
                spellInfo->OverrideSpellList.push_back(112866); ///< Summon Fel Imp
                break;
            case 697: ///< Summon Voidwalker
                spellInfo->OverrideSpellList.push_back(112867); ///< Summon Voidlord
                break;
            case 712: ///< Summon Succubus
                spellInfo->OverrideSpellList.push_back(112868); ///< Summon Shivarra
                break;
            case 691: ///< Summon Felhunter
                spellInfo->OverrideSpellList.push_back(112869); ///< Summon Observer
                break;
            case 30146: ///< Summon Felguard
                spellInfo->OverrideSpellList.push_back(112870); ///< Summon Wrathguard
                break;
            case 1122: ///< Summon Infernal
                spellInfo->OverrideSpellList.push_back(112921); ///< Summon Abyssal
                break;
            case 112921: ///< Summon Abyssal
                spellInfo->OverrideSpellList.push_back(157899); ///< Summon Abyssal
                break;
            case 18540: ///< Summon Doomguard
                spellInfo->OverrideSpellList.push_back(112927); ///< Summon Terrorguard
                break;
            case 105174: ///< Hand of Gul'dan
                spellInfo->OverrideSpellList.push_back(123194);
                break;
            case 120517: ///< Halo (Holy)
            case 120644: ///< Halo (shadow)
                spellInfo->AttributesCu &= ~SPELL_ATTR0_CU_NEGATIVE;
                break;
            case 121129: ///< Daybreak (heal)
                spellInfo->Effects[1].TargetA = TARGET_SRC_CASTER;
                break;
            case 114925: ///< Demonic Calling
                spellInfo->ProcFlags = 0;
                break;
            case 104317: ///< Wild Imp
                spellInfo->CasterAuraState = 0;
                break;
            case 51699:  ///< Honor Among Thieves (triggered)
            case 57934:  ///< Tricks of the Trade
            case 138275: ///< Cosmetic Visual (Lei Shen - Throne of Thunder)
            case 138274: ///< Cosmetic Visual (triggered - Lei Shen - Throne of Thunder)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                break;
            case 154294: ///< Power Conduit
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO | SPELL_ATTR3_STACK_FOR_DIFF_CASTERS;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                break;
            case 170493: ///< Honorbound
                spellInfo->Attributes |= SPELL_ATTR0_CANT_CANCEL;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                spellInfo->AttributesEx4 |= SPELL_ATTR4_NOT_STEALABLE;
                break;
            case 57723:  ///< Exhaustion
            case 57724:  ///< Sated
            case 80354:  ///< Temporal Displacement
            case 95809:  ///< Insanity
            case 160455: ///< Fatigued
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 186842: ///< Podling Disguise
                spellInfo->Effects[0].MiscValue = 85411;
                break;
            case 105770: ///< Item - Druid T13 Restoration 4P Bonus (Rejuvenation)
                spellInfo->Effects[0].SpellClassMask = flag128(0x00000050, 0, 0);
                break;
            case 99009: ///< Item - Druid T12 Feral 4P Bonus
                spellInfo->ProcChance = 100;
                spellInfo->ProcFlags = 16;
                break;
            case 99209: ///< Item - Shaman T12 Enhancement 2P Bonus
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 165440:///< Item - Paladin T17 Retribution 2P Bonus
            case 165439:///< Item - Paladin T17 Retribution 4P Bonus
            case 165446:///< Item - Paladin T17 Protection 2P Bonus
            case 167694:///< Item - Priest T17 Discipline 4P Bonus
            case 167684:///< Item - Priest T17 Holy 4P Bonus
            case 165629:///< Item - Priest T17 Shadow 4P Bonus
            case 167702:///< Item - Shaman T17 Restoration 4P Bonus
            case 165610:///< Item - Shaman T17 Enhancement 4P Bonus
            case 165525:///< Item - Hunter T17 Marksmanship 4P Bonus
            case 165544:///< Item - Hunter T17 Survival 2P Bonus
            case 165450:///< Item - Warlock T17 Demonology 2P Bonus
            case 165337:///< Item - Warrior T17 Fury 2P Bonus
            case 165349:///< Item - Warrior T17 Fury 4P Bonus
            case 165469:///< Item - Mage T17 Frost 4P Bonus
            case 165432:///< Item - Druid T17 Feral 4P Bonus
            case 165410:///< Item - Druid T17 Guardian 2P Bonus
            case 165478:///< Item - Rogue T17 Combat 4P Bonus
            case 165547:///< Item - Death Knight T17 Frost 2P Bonus
            case 165568:///< Item - Death Knight T17 Frost 4P Bonus
            case 165345:///< Item - Warrior T17 Arms 4P Bonus
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                break;
            case 167697:///< Item - Paladin T17 Holy 4P Bonus
                spellInfo->Effects[EFFECT_0].BasePoints = 20;
                break;
            case 181608:///< Inner Demon (for Warlock T17 Demonology 2P Bonus)
            case 166881:///< Shadow Strikes (for Rogue T17 Subtlety 4P Bonus)
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                break;
            case 165336:///< Item - Warrior T17 Arms 2P Bonus
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                break;
            case 165437:///< Item - Druid T17 Restoration 2P Bonus
                spellInfo->Effects[EFFECT_0].BasePoints = 2;
                break;
            case 170205:///< Item - Death Knight T17 Frost 4P Driver
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                spellInfo->Effects[EFFECT_1].TriggerSpell = 0;
                break;
            case 530:   ///< Charm (Possess)
                spellInfo->Effects[EFFECT_0].BasePoints = 102;
                break;
            case 36032: ///< Arcane Charge
                spellInfo->AttributesEx3 &= ~SPELL_ATTR3_CANT_TRIGGER_PROC;
                break;
            case 99213: ///< Item - Shaman T12 Enhancement 4P Bonus
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->ProcChance = 100;
                spellInfo->ProcFlags = 16;
                break;
            case 99206: ///< Item - Shaman T12 Elemental 4P Bonus
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 99175: ///< Item - Rogue T12 4P Bonus
                spellInfo->ProcChance = 0;
                spellInfo->ProcFlags = 0;
                break;
            case 99234: ///< Item - Warrior T12 DPS 2P Bonus
                spellInfo->ProcChance = 0;
                spellInfo->ProcFlags = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                break;
            case 111397: ///< Blood Horror
                spellInfo->ProcFlags = 0;
                break;
            case 114635:///< Ember Tap
                spellInfo->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_PERIODIC_HEAL;
            case 31803: ///< Censure
            case 77489: ///< Echo of Light
            case 99002: ///< Fiery Claws, Item - Druid T12 Feral 2P Bonus
            case 99132: ///< Divine Fire, Item - Priest T12 Healer 2P Bonus
            case 99173: ///< Burning Wounds, Item - Rogue T12 2P Bonus
            case 55078: ///< Blood Plague
            case 55095: ///< Frost Fever
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                break;
            case 81662: ///< Will of the Necropolis
                spellInfo->Effects[0].BasePoints = 25;
                break;
            case 91107: ///< Unholy Might
                spellInfo->OverrideSpellList.push_back(109260); ///< Add Aspect of the Iron Hack to override spell list of Aspect of the Hawk
                break;
            case 53490: ///< Bullheaded
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 109260: ///< Aspect of the Iron Hawk
                spellInfo->Effects[0].BasePoints = -10;
                break;
            case 109306: ///< Thrill of the Hunt
                spellInfo->ProcChance = 0;
                break;
            case 24529: ///< Glyph of Animal Bond
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA_ON_PET;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_HEALING_PCT;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].BasePoints = 10;
                spellInfo->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(12); ///< 100 yards
                spellInfo->Effects[1].MiscValue = 127;
                break;
            case 116014: ///< Rune of Power
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); ///< 2s
                break;
            case 88852: ///< Tower of Radiance
                spellInfo->Effects[0].Effect = 0;
                break;
            case 703: ///< Garrote
                spellInfo->ProcChance = 100;
                break;
            case 158392: ///< Hammer of Wrath (override)
            case 24275: ///< Hammer of Wrath
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 53592: ///< Guarded by the Light
                spellInfo->Attributes |= SPELL_ATTR0_ABILITY;
                spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;
                spellInfo->Attributes |= SPELL_ATTR0_HIDE_IN_COMBAT_LOG;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 29144: ///< Unwavering Sentinel
                spellInfo->Attributes |= SPELL_ATTR0_ABILITY;
                spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 5246: ///< Intimidating Shout
                spellInfo->MaxAffectedTargets |= 5;
                break;
            case 32379: ///< Shadow Word : Death - Glyph of Shadow Word: Death
                spellInfo->OverrideSpellList.push_back(129176);
                break;
            case 48045: ///< Mind Sear
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            case 91724: ///< Spell Warding
                spellInfo->Effects[0].BasePoints = 6;
                break;
            case 116481: ///< Tiger's lust
                spellInfo->Attributes |= SPELL_ATTR0_ABILITY;
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_HASTE_AFFECT_DURATION;
                break;
            case 879: ///< Exorcism
                spellInfo->AttributesEx2 |= SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 20271: ///< Judgement
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 86704: ///< Ancient Fury
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                break;
            case 155627: ///< Lunar Inspiration
                spellInfo->Attributes |= SPELL_ATTR0_PASSIVE;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            case 2818: ///< Deadly Poison
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REDIRECTED;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 118922: ///< Posthaste
            case 108212: ///< Burst of Speed
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 156297: ///< Acid Torrent
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                break;
            case 31224: ///< Cloak of Shadows
                spellInfo->Effects[0].BasePoints = -200;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE;
                spellInfo->Effects[0].ValueMultiplier = -200;
                break;
            case 137619: ///< Marked for Death
                spellInfo->AttributesEx |= SPELL_ATTR1_NO_THREAT;
                break;
            case 8680: ///< Wound Poison
            case 3409: ///< Crippling Poison
                spellInfo->Attributes |= SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REDIRECTED;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 115098: ///< Chi Wave - Add Aura for bounce counting
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[1].BasePoints = 1;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetB = 0;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1); ///< 10s
                spellInfo->Speed = 100.0f;
                break;
            case 117050: ///< Glaive Toss (talent)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_NONE;
                spellInfo->Effects[4].Effect = SPELL_EFFECT_NONE;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_NONE;
                break;
            case 120755: ///< Glaive Toss (Glaive right)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 108482: ///< Unbound Will
                /// Crashes client on pressing ESC
                spellInfo->AttributesEx4 &= ~SPELL_ATTR4_CAN_CAST_WHILE_CASTING;
                break;
            case 66: ///< Invisibility
                spellInfo->OverrideSpellList.push_back(110959); ///< Greater Invisibility
                spellInfo->Dispel = DISPEL_INVISIBILITY;
                break;
            case 110959: ///< Greater Invisibility
                spellInfo->Dispel = DISPEL_INVISIBILITY;
                break;
            case 47476: ///< Strangulate
                spellInfo->OverrideSpellList.push_back(108194); ///< Asphyxiate
                break;
            case 108945: ///< Angelic Bulwark
                spellInfo->Effects[0].TriggerSpell = 114214;
                break;
            case 114867: ///< Soul Reaper
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 34428: ///< Victory Rush
                spellInfo->OverrideSpellList.push_back(103840); ///< Impending Victory
                break;
            case 157633: ///< Improved Scorch
                spellInfo->SchoolMask = SPELL_SCHOOL_MASK_FIRE;
                break;
            case 91342: ///< Shadow Infusion
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 27285: ///< Seed of Corruption
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); ///< 10 Yards
                break;
            case 87385: ///< Soulburn : Seed of Corruption - damage
                spellInfo->Effects[0].TargetA = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13); ///< 10 Yards
                break;
            case 118779: ///< Victory Rush
            case 118340: ///< Impending Victory
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 89523: ///< Glyph of Grounding Totem
                spellInfo->SpellFamilyName = SPELLFAMILY_SHAMAN;
                break;
            case 1856:  ///< Vanish
                spellInfo->Effects[1].TriggerSpell = 131368;
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SANCTUARY;
                break;
            case 131368:  ///< Vanish (triggered)
                spellInfo->Effects[2].TriggerSpell = 131361;
                break;
            case 131361:///< Vanish - Improved Stealth
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(27); ///< 3s
                break;
            case 129197:///< Insanity
                spellInfo->Effects[0].BonusMultiplier *= 2.0f;
                break;
            case 116784:///< Wildfire Spark - Boss Feng
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 120954: ///< Fortifying Brew
                spellInfo->Effects[4].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 114871: ///< Holy Prism (Allies)
            case 114852: ///< Holy Prism (ennemies)
                spellInfo->Effects[2].Effect = 0;
                break;
            case 123154: ///< Fists of Fury Visual Target
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(35); ///< 4s
                break;
            case 31616: ///< Nature's Guardian
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                spellInfo->Effects[0].BasePoints = 25;
                break;
            case 53479: ///< Last Stand (Tenacity)
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 104316: ///< Imp Swarm
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 126309: ///< Still Water
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 126311: ///< Surface Trot
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AREA_AURA_PET;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 88767: ///< Fulmination (triggered)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 117837: ///< Delirious
                spellInfo->Dispel = DISPEL_MAGIC;
                break;
            case 125972: ///< Felin Grace
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_SAFE_FALL;
                break;
            case 83968: ///< Mass Resurrect
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_DEAD;
                break;
            case 116833: ///< Cosmetic Spirit Totem (MSV - #3 Gara'Jal)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 130774: ///< Amethyst Pool - Periodic damage (MSV - #1 Stone Guard)
                spellInfo->Attributes |= SPELL_ATTR0_CANT_CANCEL;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
            case 116782: ///< Titan Gas (MSV - #6 Will of Emperor)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SCHOOL_DAMAGE;
                spellInfo->Effects[0].BasePoints = 17000;
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(113); ///< radius 1000
                break;
            case 122336: ///< Sonic Ring (HoF - #1 Zor'lok)
            case 124673: ///< Sonic Pulse (HoF - #1 Zor'lok)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(16); ///< radius 1.0 instead of 6.0
                spellInfo->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(16);
            case 123811: ///< Pheromones of Zeal (HoF - #1 Zor'lok)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_DEST_AREA_ENTRY;
                break;
            case 178153: ///< Death from Above
                spellInfo->Effects[1].TargetA = TARGET_DEST_TARGET_FRONT;
                break;
            case 97817: ///< Leap of Faith
                spellInfo->Effects[0].TargetA = TARGET_DEST_TARGET_FRONT;
                spellInfo->Effects[0].MiscValue = 25;
                spellInfo->Effects[0].ValueMultiplier = 0;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(7); ///< radius 2.0 instead of 7.0
                break;
            case 122706: ///< Noise Cancelling (HoF - #1 Zor'lok)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].BasePoints = 60;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].BasePoints = 75;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 122707: ///< Noise Cancelling (HoF - #1 Zor'lok)
                spellInfo->Effects[0].BasePoints = 10;
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 122731: ///< Create Noise Cancelling AreaTrigger (HoF - #1 Zor'lok)
                spellInfo->Effects[0].BasePoints = 351;
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 122842: ///< Tempest Slash (summoning tornado) (HoF - #2 Ta'yak)
                spellInfo->Effects[0].TargetB = TARGET_DEST_DEST_RIGHT;
                break;
            case 125312: ///< Blade Tempest (HoF - #2 Ta'yak)
                spellInfo->AttributesEx3 = SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 45819: // Midsummer - Throw Torch
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_DEST_AREA_ENTRY;
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(15);
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 125327: ///< Blade Tempest (jump on Ta'yak) (HoF - #2 Ta'yak)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 123633: ///< Gale Winds (HoF - #2 Ta'yak)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].BasePoints = 0;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[0].Amplitude = 500;
                break;
            case 12043:
                spellInfo->Effects[1].Effect = 0;
                break;
            case 122786: ///< Broken leg (HoF - #3 Garalon)
                spellInfo->Effects[0].MiscValue = -15;
                spellInfo->Effects[1].BasePoints = 0; ///< Cancel damages, done by Garalon' script
                spellInfo->Effects[2].Effect = 3;
                break;
            case 121896: ///< Whirling Blade (HoF - #4 Mel'jarak)
                spellInfo->Effects[2].Effect = 0;
                spellInfo->Effects[3].Effect = 0;
                break;
            case 122370: ///< Reshape Life (HoF - #5 Un'sok)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CHANNEL_TARGET;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_FACTION;
                spellInfo->Effects[0].MiscValue = 2577;
                spellInfo->Effects[0].MiscValueB = 1;
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CHANNEL_TARGET;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_TRANSFORM;
                spellInfo->Effects[1].MiscValue = 62701;
                spellInfo->Effects[5].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[5].TargetA = TARGET_UNIT_CHANNEL_TARGET;
                spellInfo->Effects[5].ApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL_2;
                spellInfo->Effects[5].MiscValue = 58044;
                break;
            case 122457: ///< Rough Landing (HoF - #5 Un'sok)
                spellInfo->Effects[0].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                spellInfo->Effects[1].TargetB = TARGET_UNIT_SRC_AREA_ALLY;
                break;
            case 123713: ///< Servant of the Empress (HoF - #6 - Shek'zeer)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 123255: ///< Dissonance Field (HoF - #6 - Shek'zeer)
                spellInfo->Effects[1].Effect = SPELL_EFFECT_DUMMY; ///< Cancel triggering 123596 (knock back)
                break;
            case 126121: ///< Corrupted Dissonance Field (HoF - #6 - Shek'zeer)
                spellInfo->Effects[3].Effect = SPELL_EFFECT_DUMMY; ///< Same as previous spell (123255);
            case 124845: ///< Calamity (HoF - #6 - Shek'zeer)
                spellInfo->Effects[0].BasePoints = 50;
                spellInfo->Effects[1].BasePoints = 50;
                break;
            case 126125: ///< Corrupt Dissonance Field (HoF - #6 - Shek'zeer)
                spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ALLY;
                break;
            case 123638: ///< Heart of fear (HoF - #6 - Shek'zeer)
                spellInfo->Effects[1].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[1].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 125451: ///< Ultimate Corruption (HoF - #6 - Shek'zeer)
            case 139866: ///< Torrent of Ice (Megaera - Throne of Thunder) (ToT - #5 - Megaera)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 108503: ///< Grimoire of Sacrifice
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 136192: ///< Lightning Storm (Iron Qon - Throne of Thunder) (ToT - #10 Iron Qon)
            {
                for (uint8 l_Idx = 0; l_Idx < 5; ++l_Idx)
                    spellInfo->Effects[l_Idx].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            }
            case 136330: ///< Burning Cinders (ToT - #10 Iron Qon)
            case 136419: ///< Storm Cloud (ToT - #10 Iron Qon)
            case 136449: ///< Frozen Blood (ToT - #10 Iron Qon)
            case 137118: ///< Bloom Moon Lotus (ToT - #11 Twin Consorts)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ANY;
                break;
            case 137749: ///< Cosmic Barrage (ToT - #11 Twin Consorts)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                break;
            case 138318: ///< Crane Rush (ToT - #11 Twin Consorts)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_TARGET_ANY;
                break;
            case 143412: ///< Immerseus Swirl (Immerseus - Siege of Orgrimmar)
                spellInfo->Effects[EFFECT_0].Amplitude = 1000;
                break;
            case 143413:
                spellInfo->Effects[EFFECT_0].Amplitude = 3000;
                break;
            case 119905: ///< Cauterize (Command Demon)
            case 119907: ///< Disarm (Command Demon)
            case 119909: ///< Whiplash (Command Demon)
            case 119910: ///< Spell Lock (Command Demon)
            case 119911: ///< Optical Blast (Command Demon)
            case 119913: ///< Fellash (Command Demon)
            case 119914: ///< Felstorm (Command Demon)
            case 119915: ///< Wrathstorm (Command Demon)
                spellInfo->AttributesEx6 &= ~SPELL_ATTR6_DONT_DISPLAY_COOLDOWN;
                break;
            case 119904: ///< Supplant Command Demon
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].Effect = 0;
                break;
            case 108199: ///< Gorefiend's Grasp
                spellInfo->Mechanic = Mechanics::MECHANIC_GRIP;
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[2].Effect = 0;
                break;
            case 49206: ///< Summon Gargoyle
                spellInfo->AttributesEx5 |= SPELL_ATTR5_SINGLE_TARGET_SPELL;
                break;
            case 45529: ///< Blood Tap
                spellInfo->Effects[0].Effect = SPELL_EFFECT_ACTIVATE_RUNE;
                spellInfo->Effects[0].MiscValueB = 3;
                break;
            case 82691: ///< Ring of Frost
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                break;
            case 91021: ///< Find Weakness
                spellInfo->Effects[0].BasePoints = 100;
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 118699: ///< Fear Effect
                spellInfo->Dispel = DISPEL_MAGIC;
                spellInfo->AttributesEx3 &= ~SPELL_ATTR3_IGNORE_HIT_RESULT;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_FEAR;
                break;
            case 122355: ///< Molten Core
                spellInfo->StackAmount = 255;
                break;
            case 6203: ///< Soulstone
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_DEAD;
                break;
            case 106707: ///< Faerie Swarm (talent)
                spellInfo->AttributesEx8 |= SPELL_ATTR8_AURA_SEND_AMOUNT;
                break;
            case 118291: ///< Greater Fire Elemental
            case 118323: ///< Greater Earth Elemental
            case 157319: ///< Greater Storm Elemental
                spellInfo->Effects[0].Effect = SPELL_EFFECT_SUMMON_PET;
                break;
            case 62099: ///< Shamanism
                spellInfo->Effects[0].SpellClassMask[2] |= 0x8000;
                break;
            case 116943: ///< Earthgrab
                spellInfo->AttributesEx5 |= SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 6474:   ///< Earthbind Totem
            case 8145:   ///< Tremor Totem effect
            case 177096: ///< Forgemaster's Vigor
            case 177067: ///< Detonation
            case 177102: ///< Battering
            case 177086: ///< Sanitizing
            case 177081: ///< Molten Metal
                spellInfo->AttributesEx5 |= SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            case 2484: ///< Earthbind Totem
                spellInfo->OverrideSpellList.push_back(51485);
                break;
            case 33110: ///< Prayer of Mending
                spellInfo->Effects[0].BonusMultiplier = 0.0f;
                break;
            case 119611: ///< Renewing Mist
                spellInfo->Effects[0].BonusMultiplier = 0.109984f;
                break;
            case 109186: ///< Surge of light
                spellInfo->ProcFlags = PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS;
                break;
            case 853: ///< Hammer of Justice
                spellInfo->OverrideSpellList.push_back(105593); ///< Replace Hammer of Justice by Fist of Justice
                break;
            case 16914: ///< Hurricane
                spellInfo->OverrideSpellList.push_back(106996); ///< Replace Hurricane by Astral Storm
                break;
            case 85673: ///< Word of Glory
                spellInfo->OverrideSpellList.push_back(114163); ///< Replace World of glory by Eternal Flame
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 136494: ///< Word of Glory (overide by Glyph of Harsh Words
            case 130551: ///< Word of Glory (overide by Glyph of Harsh Words)
            case 20066:  ///< Repentance
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 114163: ///< Eternal Flame
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 974: ///< Earth Shield
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_HEALING_RECEIVED;
                break;
            case 86529: ///< Mail Specialization (Shaman)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 61999: ///< Raise Ally
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ALLY;
                break;
            case 31935: ///< Avenger's Shield
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MAGIC;
                break;
            case 498:    ///< Divine Protection
            case 30823:  ///< Shamanistic Rage
            case 51490:  ///< Thunderstorm
            case 108416: ///< Sacrificial Pact
            case 137562: ///< Nimble Brew
            case 134758: ///< Burning Cinders
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                break;
            case 159642 : ///< Glyph of Ghostly Speed
                spellInfo->Attributes &= ~SPELL_ATTR0_OUTDOORS_ONLY;
                break;
            case 173545: /// Chi Wave (healing bolt)
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 115611: ///< Temporal Ripples
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
            case 115610: ///< Temporal Shield
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                break;
            case 121118: ///< Dire Beast summons
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
            case 158624: ///< Feather
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENTRY;
                break;
            case 121536: ///< Feather
                spellInfo->Effects[0].TargetB = TARGET_DEST_DEST;
                break;
            case 177345: ///< Meteor
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); ///< 2 seconds
                break;
            case 19574: ///< Bestial Wrath
                spellInfo->Effects[3].Effect = 0;
                spellInfo->Effects[3].ApplyAuraName = 0;
                break;
            case 87935: ///< Serpent Spread
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(21); ///< -1s
                break;
            case 118253:
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                break;
            case 53257: ///< Cobra Strikes
                spellInfo->Effects[0].BasePoints = 0;
                spellInfo->Effects[1].BasePoints = 0;
                break;
            case 11958: ///< Cold Snap
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_CONFUSED;
                break;
            case 86674: ///< Ancient Healer
                spellInfo->ProcCharges = 5;
                break;
            case 86657: ///< Ancient Guardian
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                break;
            case 5782:  ///< Fear
                spellInfo->Dispel = DISPEL_MAGIC;
                spellInfo->Mechanic = 0;
                spellInfo->Effects[0].Mechanic = MECHANIC_NONE;
                spellInfo->OverrideSpellList.push_back(104045); ///< Add Sleep (Metamorphosis)
                break;
            case 45204: ///< Mirror Image - Clone Me!
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 11371: ///< Arthas's Gift
                spellInfo->Effects[0].TriggerSpell = 0;
                break;
            case 33891:  ///< Incarnation: Tree of Life
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(9);
                break;
            case 146950: ///< Glyph of Targeted Expulsion
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 115072: ///< Expel Harm
                spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ALLY;
                spellInfo->ExplicitTargetMask &= ~TARGET_FLAG_UNIT;
                break;
            case 117952: ///< Crackling Jade Lightning
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_HASTE_AFFECT_DURATION;
                break;
            case 117833: ///< Crazy Thought
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                break;
            case 172:   ///< Corruption
            case 30108: ///< Unstable Affliction
                spellInfo->Effects[2].Effect = 0;
                spellInfo->Effects[2].ApplyAuraName = 0;
                spellInfo->Effects[2].BasePoints = 0;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                break;
            case 34433: ///< Shadowfiend
                spellInfo->Effects[EFFECT_0].MiscValueB = 1561;
                spellInfo->OverrideSpellList.push_back(123040); ///< Add Mindbender to override spell list of Shadowfiend
                break;
            case 59907: ///< Lightwell Charges
                spellInfo->Effects[0].BasePoints = 15;
                spellInfo->ProcCharges = 15;
                break;
            case 53817: ///< Maelstrom Weapon
                spellInfo->Effects[EFFECT_2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->Effects[EFFECT_2].MiscValue = SPELLMOD_DAMAGE;
                spellInfo->Effects[EFFECT_4].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_4].ApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
                spellInfo->Effects[EFFECT_4].MiscValue = SPELLMOD_DAMAGE;
                spellInfo->ProcFlags = 0;
                break;
            case 126135: ///< Lightwell
                spellInfo->OverrideSpellList.push_back(724); ///< Add old Lightwell to override list
                break;
            case 81751: ///< Atonement
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 108201: ///< Desecrated Ground
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_CONFUSED;
                spellInfo->AttributesEx |= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 102359: ///< Mass Entanglement
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                break;
            case 64380: ///< Shattering Throw
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DISPEL_MECHANIC;
                spellInfo->Effects[0].MiscValue = 29;
                break;
            case 102355: ///< Faerie Swarm
                spellInfo->AttributesEx5 |= SPELL_ATTR5_SINGLE_TARGET_SPELL;
                break;
            case 3411:  ///< Intervene
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_RAID;
                spellInfo->AttributesEx6 |= SPELL_ATTR6_ASSIST_IGNORE_IMMUNE_FLAG;
                spellInfo->AttributesEx7 |= SPELL_ATTR7_HAS_CHARGE_EFFECT;
                spellInfo->OverrideSpellList.push_back(114029); ///< Add Safeguard to override spell list of Intervene
                break;
            case 157590: ///< Breath of the Serpent
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ALLY;
                break;
            case 114029: ///< Safeguard
                spellInfo->Effects[2].BasePoints = 100;
                break;
            case 1784:  ///< Stealth
                spellInfo->OverrideSpellList.push_back(115191); ///< Add Stealth (talent) to override spell list of Stealth
                break;
            case 115191: ///< Subterfuge
                spellInfo->AttributesEx |= SPELL_ATTR0_DISABLED_WHILE_ACTIVE;
                spellInfo->AttributesEx8 |= SPELL_ATTR8_AURA_SEND_AMOUNT;
                break;
            case 115192: ///< Subterfuge
                spellInfo->Attributes |= SPELL_ATTR0_DONT_AFFECT_SHEATH_STATE;
                spellInfo->Attributes |= SPELL_ATTR0_NOT_SHAPESHIFT;
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 20578: ///< Cannibalize
            case 20577: ///< Cannibalize
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 13812: ///< Explosive Trap
            case 3355: ///< Freezing Trap
            case 57879: ///< Snake Trap
                spellInfo->AttributesEx &= ~SPELL_ATTR1_NOT_BREAK_STEALTH;
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                break;
            case 84745: ///< Shallow Insight
            case 84746: ///< Moderate Insight
            case 84747: ///< Deep Insight
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
                break;
            case 157581: //< Empowered Bandit's Guile
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 121733: ///< Throw
                spellInfo->OverrideSpellList.push_back(114014); ///< Add Shuriken Toss to override spell list of Throw
                break;
            case 105361: ///< Seal of Command
                spellInfo->OverrideSpellList.push_back(31801); ///< Replace Seal of Command with Seal of Truth
                break;
            case 44457: ///< Living Bomb
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_SINGLE_TARGET_SPELL;
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 12051: ///< Evocation
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[1].ApplyAuraName = 0;
                break;
            case 23691: ///< Berzerker Rage Effect
                spellInfo->Effects[0].BasePoints = 100;
                break;
            case 127630: ///< Cascade - damage trigger
            case 120786: ///< Cascade - heal trigger
                spellInfo->Effects[1].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 122128: ///< Divine Star (shadow)
            case 110745: ///< Divine Star (other)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(29);
                spellInfo->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(29);
                break;
            case 12975: ///< Last Stand
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 117828: ///< Backdraft
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
            case 109259: ///< Powershot
                spellInfo->Effects[5].Effect = 0;
                spellInfo->Effects[2].BasePoints = 0;
                break;
            case 114695: ///< Pursuit of Justice
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 82938: ///< Explosive Trap (launcher)
            case 82940: ///< Ice Trap (launcher)
            case 60202: ///< Freezing Trap (launcher)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(7);
                break;
            case 56224: ///< Glyph of Healthstone
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 90259: ///< Glyph of Frost Pillar
                spellInfo->Effects[0].MiscValue = 0;
                spellInfo->Effects[0].MiscValueB = 0;
                spellInfo->Effects[1].BasePoints = -70;
                break;
            case 49821: ///< Mind Sear
                spellInfo->Effects[0].TargetA = TARGET_DEST_CHANNEL_TARGET;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
                break;
            case 10326: ///< Turn Evil
                spellInfo->Mechanic = MECHANIC_TURN;
                spellInfo->OverrideSpellList.push_back(145067); ///< Evil is a point of view
                break;
            case 113656: ///< Fists of Fury
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 137211: ///< Tremendous Fortitude
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
                break;
            case 115315: ///< Summon Black Ox Statue
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 114908: ///< Spirit Shell
            case 50273: ///< Arcane Barrage
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 153596:///< Comet Storm
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                spellInfo->Speed = 0.20f;
                break;
            case 12654: ///< Ignite
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                break;
            case 182287:/// Glyph of Ignite (effect)
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(28); ///< 5s
                break;
            case 153564:///< Meteor
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_FEARED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_CONFUSED;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 153561:///< Meteor (launch spell)
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 155158:///< Meteor (periodic damage)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_DONT_RESET_PERIODIC_TIMER;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_NEGATIVE;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                spellInfo->AttributesEx5 |= SPELL_ATTR5_HIDE_DURATION;
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(39); // 2s
                break;
            case 52610: ///< Savage Roar
                spellInfo->Effects[2].BasePoints = 40;
                break;
            case 29725: ///< Sudden Death
                spellInfo->ProcFlags = PROC_FLAG_DONE_MELEE_AUTO_ATTACK;
                break;
            case 8676:  ///< Ambush
                spellInfo->Effects[0].BonusMultiplier = 0;
                spellInfo->Effects[1].BonusMultiplier = 0;
                break;
            case 132411:///< Singe Magic
                spellInfo->AttributesEx6 |= SPELL_ATTR6_IGNORE_CASTER_AURAS;
                break;
            case 49376: ///< Wild Charge
                spellInfo->Effects[1].MiscValue = 25;
                spellInfo->Effects[1].ValueMultiplier = 0;
                break;
            case 169157:///< Demonic Leap (jump)
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(43);
                spellInfo->Effects[0].MiscValue = 35;
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_DEST_CASTER_FRONT;
                break;
            case 49575: ///< Death Grip (effect)
                spellInfo->Effects[0].ValueMultiplier = 0;
                break;
            case 56242: ///< Glyph of Imp Swarm
                spellInfo->Effects[1].BasePoints = 100;
                break;
            case 77535: ///< Blood Shield
            case 127802: ///< Touch of The Grave (trigger)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->AttributesEx6 |= SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS;
                break;
            case 53:    ///< Backstap
                spellInfo->Effects[0].BonusMultiplier = 0;
                break;
            case 86121: ///< Soul Swap
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 86213: ///< Soul Swap Exhale
                spellInfo->Speed = 0;
                break;
            case 157695:///< Demonbolt
                spellInfo->AttributesEx2 |= SPELL_ATTR2_NOT_NEED_SHAPESHIFT;
                spellInfo->AttributesEx4 |= SPELL_ATTR4_IGNORE_RESISTANCES;
                break;
            case 169686:///< Unyielding Strikes
                spellInfo->ProcCharges = 0;
                break;
            case 109167:///< Demonic Leap (jump)
                spellInfo->Effects[0].MiscValue = 300;
                break;
            case 781:   ///< Disengage
                spellInfo->Effects[0].TriggerSpell = 0; ///< Handled in Player::HandleFall()
                spellInfo->AttributesEx |= SPELL_ATTR1_NOT_BREAK_STEALTH;
                break;
            case 111400:///< Burning Rush
                spellInfo->AuraInterruptFlags = AURA_INTERRUPT_FLAG_NOT_ABOVEWATER + AURA_INTERRUPT_FLAG_NOT_UNDERWATER;
                break;
            case 96840: ///< Flame Patch for Glyph of the Blazing Trail
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(285); ///< 1s
                break;
            case 7922: ///< Warbringer stun effect
            case 103828: ///< Warbringer
            case 100:   ///< Charge
                spellInfo->AttributesEx3 |= SPELL_ATTR3_IGNORE_HIT_RESULT;
                break;
            case 47753: ///< Divine Aegis
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                spellInfo->Effects[0].BonusMultiplier = 0;
                break;
            case 45438:
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                break;
            case 170995:///< Cripple
                spellInfo->CastTimeEntry = sSpellCastTimesStore.LookupEntry(2); ///< 250ms - hack fix to imagine Seduction mechanic
                spellInfo->AttributesEx |= SPELL_ATTR1_NO_THREAT;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                break;
            case 118283:///< Ursol's Vortex
                spellInfo->Effects[0].ValueMultiplier = 60;
                break;
            case 6262:  ///< Healthstone
                spellInfo->AttributesEx2 &= ~SPELL_ATTR2_CANT_CRIT;
                break;
            case 95861: ///< Meditation
                spellInfo->Effects[1].Effect = 0;  ///< On retail priests don't have this bonus, also in tooltip nothing said about that
                break;
            case 157048:///< Final Verdict
                spellInfo->Effects[0].BasePoints = 259; ///< 6.2.3 hotfix: Final Verdict (Retribution) damage has been increased by 8%
                break;
            case 152107:///< Demonic Servitude
                spellInfo->Effects[3].BasePoints = 157899;
                spellInfo->Effects[3].MiscValue = 157899;
                spellInfo->Effects[3].SpellClassMask[0] = 0;
                spellInfo->Effects[3].SpellClassMask[1] |= 0x00100000;
                break;
            case 12723: ///< Sweeping Strikes (damage)
            case 94009: ///< Rend (final damage)
            case 174736:///< Enhanced Rend
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 642:   ///< Divine Shield
                spellInfo->AttributesEx5 |= SPELL_ATTR5_USABLE_WHILE_STUNNED;
                break;
            case 126890:///< Eminence
            case 117895:///< Eminence (statue)
                spellInfo->SpellLevel = 100;
                break;
            case 189999:///< Grove Warden
            case 171828:///< Solar Spikehawk
                spellInfo->Effects[0].MiscValue = 74410;
                spellInfo->Effects[0].MiscValueB = 248;
                break;
            case 81292:  ///< Glyph of Mind Spike (Shadow)
                spellInfo->ProcFlags = 0;
                break;
            case 187356: ///< Mysic Image (Magic Pet Mirror)
                spellInfo->Effects[0].TargetA = SpellImplicitTargetInfo();
                break;
            case 7268: ///< Arcane Missile
                spellInfo->AttributesEx &= ~SPELL_ATTR1_CHANNELED_1;
                break;
            case 174004:///< Spirit of Shinri
                spellInfo->Effects[1].MiscValue = 82415;
                break;
            /// All spells - BonusMultiplier = 0
            case 77758: ///< Thrash (bear)
            case 106830:///< Thrash (cat)
            case 22568: ///< Ferocious Bite
            case 5221:  ///< Shred
            case 22599: ///< Chromatic Mantle of the Dawn
            case 86273: ///< Illuminated Healing
            case 1752:  ///< Sinister Strike
                spellInfo->Effects[0].BonusMultiplier = 0;
                break;
            /// All spells - ProcFlags = 0
            case 58372:  ///< Glyph of Rude Interruption
            case 166061: ///< Item - Death Knight WoD PvP Unholy 4P Bonus
            case 170848: ///< Item - Druid WoD PvP Feral 2P Bonus
            case 170853: ///< Item - Druid WoD PvP Restoration 2P Bonus
            case 165691: ///< Item - Monk WoD PvP Windwalker/Brewmaster 2P Bonus
            case 165639: ///< Item - Warrior WoD PvP Fury 2P Bonus
            case 165636: ///< Item - Warrior WoD PvP Arms 2P Bonus
            case 165641: ///< Item - Warrior WoD PvP Protection 2P Bonus
            case 165995: ///< Item - Rogue WoD PvP 2P Bonus
            case 170877: ///< Item - Rogue WoD PvP Subtlety 4P Bonus
            case 182303: ///< Item - Rogue WoD PvP Combat 4P Bonus
            case 170883: ///< Item - Rogue WoD PvP Assassination 4P Bonus
            case 165886: ///< Item - Paladin WoD PvP Retribution 2P Bonus
			case 165905: ///< Item - Paladin WoD PvP Protection 2P Bonus
            case 166005: ///< Item - Hunter WoD PvP 2P Bonus
            case 166009: ///< Item - Hunter WoD PvP 2P Bonus
            case 165519: ///< Item - Hunter WoD PvP 2P Bonus
            case 171383: ///< Item - Warlock WoD PvP Destruction 2P Bonus
            case 171379: ///< Item - Warlock WoD PvP Affliction 4P Bonus
            case 165482: ///< Item - Rogue T17 Subtlety 2P Bonus
            case 56414: ///< Glyph of Dazing Shield
            case 162452: ///< Shadowy Insight
            case 87160:  ///< Surge of Darkness
            case 73685:  ///< Unleash Life (restoration)
            case 118864: ///< Combo Breaker (tiger palm)
            case 116768: ///< Combo Breaker (blackout kick)
            case 64803: ///< Entrapment
            case 135373: ///< Entrapment
            case 115174: ///< Unertie
                spellInfo->ProcFlags = 0;
                break;
            /// All spells - ProcCharges = 1
            case 6346:   ///< Fear Ward
            case 48108:  ///< Hot Streak
            case 132158: ///< Nature's Swiftness
            case 74434:  ///< Soul Burn
            case 23920:  ///< Spell Reflection
            case 124430: ///< Divine Insight (Shadow)
            case 114250: ///< Selfless Healer
            case 90174:  ///< Divine Purpose
            case 131567: ///< Holy Spark
            case 69369:  ///< Predator Swiftness
            case 114028: ///< Mass Spell Reflection
            case 122510: ///< Ultimatum
            case 34784:  ///< Intervene (triggered)
            case 73683:  ///< Unleash Flame
            case 52437:  ///< Sudden Death
            case 157174: ///< Elemental Fusion
                spellInfo->ProcCharges = 1;
                break;
            case 111546: ///< Chaotic Energy
                spellInfo->Effects[1].Effect = 0;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_NONE;
                break;
            case 111771: ///< Demonic Gateway (launch spell)
                spellInfo->Effects[1].Effect = 0;
                break;
            case 113890: ///< Demonic Gateway
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                break;
            case 113886: ///< Demonic Gateway
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 980: ///< Agony
                spellInfo->StackAmount = 10;
                break;
            case 131740: ///< Corruption (Malefic Grasp)
            case 131736: ///< Unstable Affliction (Malefic Grasp)
            case 132566: ///< Seed of Corruption (Malefic Grasp)
            case 131737: ///< Agony (Malefic Grasp)
            case 42463:  ///< Seal of Truth
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 131116: ///< Allow to use Raging Blow
                spellInfo->StackAmount = 2;
                break;
            case 44544:  ///< Fingers of Frost
            case 126084: ///< Fingers of Frost - visual
                spellInfo->StackAmount = 2;
                break;
            case 137017:
                spellInfo->Effects[EFFECT_1].SpellClassMask[1] &= ~0x80000000;
                break;
            case 116330: ///< Dizzying Haze
                spellInfo->Effects[EFFECT_1].BasePoints = 2000;
                break;
            case 85222: ///< Light of Dawn
                spellInfo->MaxAffectedTargets = 6;
                spellInfo->InterruptFlags |= SPELL_INTERRUPT_FLAG_INTERRUPT;
                break;
            case 2641: ///< Dismiss Pet
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_DEAD;
                break;
            case 52042: ///< Healing Stream - Totem
                spellInfo->Effects[0].Effect = SPELL_EFFECT_HEAL;
                spellInfo->Effects[0].BasePoints = 31;
                break;
            case 117993: ///< Chi Torpedo : Heal
            case 124040: ///< Chi Torpedo : Damage
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(9);
                break;
            case 80325:  ///< Camouflage
            case 119450: ///< Glyph of Camouflage
                spellInfo->Effects[1].Effect = 0;
                break;
            case 115008: ///< Chi Torpedo
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(596); // 900 ms
                spellInfo->OverrideSpellList.push_back(121828); ///< Override List with Chi Torpedo - Talent
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_MOD_SPEED_NOT_STACK;
                break;
            case 109132: ///< Roll
                spellInfo->OverrideSpellList.push_back(121827); ///< Override List with Roll - Talent
                break;
            case 115295: ///< Guard
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_HEALING_DONE_PERCENT;
                spellInfo->Effects[1].BasePoints = 30;
                break;
            case 126060: ///< Desperate Measures
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 124273: ///< Heavy Stagger
            case 124274: ///< Moderate Stagger
            case 124275: ///< Light Stagger
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_PERIODIC_DAMAGE;
                spellInfo->Effects[0].Amplitude = 1000;
                spellInfo->Effects[0].BasePoints = 0;
                break;
            case 108937: ///< Baby Elephant Takes a Bath
                spellInfo->Effects[1].BasePoints = 40;
                spellInfo->Effects[1].MiscValue = 100;
                break;
            case 115308: ///< Elusive Brew
                spellInfo->DurationEntry = sSpellDurationStore.LookupEntry(1);
                break;
            case 115129: ///< Expel Harm - Damage to a nearby ennemy within 10 yards
                spellInfo->Effects[0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_NEARBY_ENEMY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                break;
            case 126892: ///< Zen Pilgrimage
            case 126895: ///< Zen Pilgrimage : Return
                spellInfo->Effects[0].Effect = SPELL_EFFECT_DUMMY;
                break;
            case 126:
                spellInfo->Effects[EFFECT_2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[EFFECT_2].ApplyAuraName = SPELL_AURA_MOD_STEALTH_DETECT;
                spellInfo->Effects[EFFECT_2].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_2].BasePoints = 500;
                spellInfo->Effects[EFFECT_2].MiscValue = 1;
                break;
            case 125084: ///< Charging Ox Wave
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); ///< radius 30
                break;
            case 107223: ///< Sunfire Rays
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 106909: ///< Jade Soul
            {
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(113379);
                if (!spell)
                    break;
                spellInfo->DurationEntry = spell->DurationEntry;
                break;
            }
            case 113315: ///< Intensity
            {
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(113379);
                if (!spell)
                    break;
                spellInfo->DurationEntry = spell->DurationEntry;
                break;
            }
            case 106736: ///< Wither Will
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 106113: ///< Touch of Nothingness
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 119922: ///< Shockwave
            case 119929:
            case 119930:
            case 119931:
            case 119932:
            case 119933:
                spellInfo->Speed = 5.0f;
                break;
            case 106112: ///< Release Doubt
            {
                const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(22);
                if (!radius)
                    break;
                spellInfo->Effects[0].RadiusEntry = radius; ///< 200yards.
                break;
            }
            case 106847: ///< Barrel Toss
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 106334: ///< Wash Away
                spellInfo->AttributesEx3 &= ~ SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 124974: ///< Nature's Vigil
                spellInfo->AttributesEx7 &= ~SPELL_ATTR7_ALLIANCE_ONLY;
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(16);
                break;
            case 120552: ///< Mantid Munition Explosion
                spellInfo->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(16);
                break;
            case 119684: ///< Ground Slam
                spellInfo->Effects[0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                spellInfo->Effects[0].TargetB = 0;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                spellInfo->Effects[1].TargetB = 0;
                break;
            case 106853: ///< Fists of Fury
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ENEMY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 112060: ///< Apparitions
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 118685: ///< Battering Headbutt
                spellInfo->RangeEntry = sSpellRangeStore.LookupEntry(5);
                break;
            case 60670: ///< Malygos Enrage
                spellInfo->Effects[1].TriggerSpell = 0;
                spellInfo->Effects[2].TriggerSpell = 0;
                break;
            case 114746: ///< Snatch Master's Flame
                spellInfo->Effects[2].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[2].TargetB = 0;
                break;
            case 104855: ///< Overpacked Firework
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            /// Add Server-Side dummy spell for Fishing
            /// TODO : Add more generic system to load server-side spell
            case 7733:   ///< Apprentice Fishing
            case 7734:   ///< Journeyman Fishing
            case 18249:  ///< Artisan Fishing
            case 54083:  ///< Expert Fishing
            case 54084:  ///< Master Fishing
            case 51293:  ///< Grand Master Fishing
            case 88869:  ///< Illustrious Grand Master Fishing
            case 110412: ///< Zen Master Fishing
            {
                std::unordered_map<uint32, SpellVisualMap> l_VisualsBySpell;
                SpellInfo* fishingDummy = new SpellInfo(sSpellStore.LookupEntry(131474), difficulty, std::move(l_VisualsBySpell[spellInfo->Effects[0].TriggerSpell]));
                fishingDummy->Id = spellInfo->Effects[0].TriggerSpell;
                mSpellInfoMap[difficulty][spellInfo->Effects[0].TriggerSpell] = fishingDummy;
                break;
            }
            /// Mogu'shan Vault
            case 116161: ///< Crossed Over
                spellInfo->Effects[EFFECT_1].MiscValue = 2; ///< Set Phase to 2
                spellInfo->Effects[EFFECT_3].Effect    = 0; ///< No need to summon
                break;
            case 116272: ///< Banishment
                spellInfo->Effects[EFFECT_0].MiscValue = 2; ///< Set Phase to 2
                break;
            case 116606: ///< Troll Rush
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 118303: ///< Fixate
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                break;
            case 15850: ///< Chilled
            case 16927: ///< Chilled
            case 20005: ///< Chilled
                spellInfo->Mechanic = MECHANIC_SNARE;
                break;
            case 89640: ///< Water Spirit
                spellInfo->Effects[EFFECT_0].BasePoints = 5000;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 130649: ///< Life Spirit
                spellInfo->Effects[EFFECT_0].BasePoints = 20000;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 127362: ///< Grasping Energy Tendrils
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                break;
            case 66630: ///< Isle of Conquest Gunship Portal
            case 66637:
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                break;
            case 66550: ///< teleports outside (Isle of Conquest)
            case 66551: ///< teleports inside (Isle of Conquest)
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 60206: ///< Ram
                spellInfo->Effects[2].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
                break;
            case 96172:  ///< Hand of Light
            case 101085: ///< Wrath of Tarecgosa
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_IGNORE_HIT_RESULT;
                spellInfo->AttributesEx6 |= SPELL_ATTR6_IGNORE_CASTER_AURAS;
                spellInfo->AttributesEx6 |= SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS;
                break;
            case 147362: ///< Counter Shot
                spellInfo->Speed = 0;
                break;
            case 36819: ///< Pyroblast (Kael'thas)
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 25677: ///< Snowball
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 54402: ///< Land Mine Knockback
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_DEST_AREA_ALLY;
                spellInfo->Effects[EFFECT_1].TargetB = TARGET_UNIT_DEST_AREA_ALLY;
                break;
            case 81262: ///< Efflorensence
            case 88685: ///< Holy World: Sanctuary
                spellInfo->AttributesEx5 &= ~SPELL_ATTR5_START_PERIODIC_AT_APPLY;
                break;
            case 92601: ///< Detonate Mana, Tyrande's Favorite Doll
                spellInfo->CasterAuraSpell = 92596;
                break;
            case 21079: ///< Shard of the Defiler - Echo of Archimonde
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_2].Effect = 0;
                spellInfo->Effects[EFFECT_2].ApplyAuraName = 0;
                break;
            case 88646: ///< Burn Hay, Braizer Torch
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].TargetA = 0;
                spellInfo->Effects[EFFECT_1].TargetB = 0;
                break;
            case 99152: ///< Cauterizing Flame
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            case 100403: ///< Moonwell Chalice
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_1].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 78628: ///< Arest, Sturdy Manacles, Seize the Ambassador
                spellInfo->Effects[EFFECT_0].Effect = 0;
                break;
            case 75206: ///< Flamebreaker, Flameseer's Staff, Flamebreaker quest
                spellInfo->Effects[EFFECT_0].Effect = 0;
                spellInfo->Effects[EFFECT_0].ApplyAuraName = 0;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                break;
            case 74723: ///< Summon Unbound Flamesparks, Flameseer's Staff, Flamebreaker quest
                spellInfo->Effects[EFFECT_0].MiscValue = 40065;
                break;
            case 75346: ///< Inflate Air Balloon, Undersea Inflation quest
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_KILL_CREDIT;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[EFFECT_0].MiscValue = 40399;
                spellInfo->Effects[EFFECT_1].Effect = 0;
                spellInfo->Effects[EFFECT_1].ApplyAuraName = 0;
                break;
            case 89821: ///< Hercular's Rod
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 87604: ///< Fortune Cookie
                spellInfo->Effects[EFFECT_2].Effect = SPELL_EFFECT_SCRIPT_EFFECT;
                break;
            case 91041: ///< Heart's Judgment, Heart of Ignacious trinket
                spellInfo->CasterAuraSpell = 91027;
                break;
            case 92328: ///< Heart's Judgment, Heart of Ignacious trinket (heroic)
                spellInfo->CasterAuraSpell = 92325;
                break;
            case 45182: ///< Cheat Death
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
                break;
            ///
            /// RUBY SANCTUM SPELLS
            ///
            case 74769: ///< Twilight Cutter
            {
                const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(12);
                if (!radius)
                    break;
                spellInfo->Effects[0].RadiusEntry = radius; ///< 200yards.
                break;
            }
            case 75509: ///< Twilight Mending
                spellInfo->AttributesEx6 |= SPELL_ATTR6_CAN_TARGET_INVISIBLE;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 75888: ///< Awaken Flames
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_TARGET_SELF;
                break;
            case 74799: ///< Soul Consumption
            {
                const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(32);
                if (!radius)
                    break;
                spellInfo->Effects[1].RadiusEntry = radius; ///< 200yards.
                break;
            }
            case 74802: ///< Consumption (10man normal)
            case 74630: ///< Combustion (10man normal)
            {
                const SpellRadiusEntry* radius = sSpellRadiusStore.LookupEntry(22);
                if (!radius)
                    break;
                spellInfo->Effects[0].RadiusEntry = radius; ///< 200yards.
                break;
            }
            case 74562: ///< Fiery Combustion
            case 74792: ///< Soul Consumption
                spellInfo->AttributesEx |= (SPELL_ATTR1_CANT_BE_REFLECTED|SPELL_ATTR1_CANT_BE_REDIRECTED);
                break;
            /// ENDOF RUBY SANCTUM SPELLS
            /// EYE OF ETERNITY SPELLS
            /// All spells below work even without these changes. The LOS attribute is due to problem
            /// from collision between maps & gos with active destroyed state.
            case 57473: ///< Arcane Storm bonus explicit visual spell
            case 57431: ///< Summon Static Field
            case 56091: ///< Flame Spike (Wyrmrest Skytalon)
            case 56092: ///< Engulf in Flames (Wyrmrest Skytalon)
            case 57090: ///< Revivify (Wyrmrest Skytalon)
            case 57143: ///< Life Burst (Wyrmrest Skytalon)
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            /// This would never crit on retail and it has attribute for SPELL_ATTR3_NO_DONE_BONUS because is handled from player,
            /// until someone figures how to make scions not critting without hack and without making them main casters this should stay here.
            case 63934: ///< Arcane Barrage (casted by players and NONMELEEDAMAGELOG with caster Scion of Eternity (original caster)).
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                break;
            /// ENDOF EYE OF ETERNITY SPELLS
            ///
            /// ICECROWN CITADEL SPELLS
            ///
            /// THESE SPELLS ARE WORKING CORRECTLY EVEN WITHOUT THIS HACK
            /// THE ONLY REASON ITS HERE IS THAT CURRENT GRID SYSTEM
            /// DOES NOT ALLOW FAR OBJECT SELECTION (dist > 333)
            case 70781: ///< Light's Hammer Teleport
            case 70856: ///< Oratory of the Damned Teleport
            case 70857: ///< Rampart of Skulls Teleport
            case 70858: ///< Deathbringer's Rise Teleport
            case 70859: ///< Upper Spire Teleport
            case 70860: ///< Frozen Throne Teleport
            case 70861: ///< Sindragosa's Lair Teleport
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DB;
                break;
            case 69055: ///< Saber Lash (Lord Marrowgar)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_5_YARDS); ///< 5yd
                break;
            case 69075: ///< Bone Storm (Lord Marrowgar)
            case 72864: ///< Death Plague (Rotting Frost Giant)
            case 71160: ///< Plague Stench (Stinky)
            case 71123: ///< Decimate (Stinky & Precious)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_100_YARDS); ///< 100yd
                break;
            case 72378: ///< Blood Nova (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                break;
            case 72385: ///< Boiling Blood (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                break;
            case 72769: ///< Scent of Blood (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                /// no break
            case 72771: ///< Scent of Blood (Deathbringer Saurfang)
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);
                break;
            case 72723: ///< Resistant Skin (Deathbringer Saurfang adds)
                /// this spell initially granted Shadow damage immunity, however it was removed but the data was left in client
                spellInfo->Effects[EFFECT_2].Effect = 0;
                break;
            case 70460: ///< Coldflame Jets (Traps after Saurfang)
                spellInfo->SetDurationIndex(1); ///< 10 seconds
                break;
            case 71412: ///< Green Ooze Summon (Professor Putricide)
            case 71415: ///< Orange Ooze Summon (Professor Putricide)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            case 71159: ///< Awaken Plagued Zombies
                spellInfo->SetDurationIndex(21);
                break;
            case 71127: ///< Mortal Wound
                spellInfo->AttributesEx3 |= SPELL_ATTR3_IGNORE_HIT_RESULT;
                break;
            case 70530: ///< Volatile Ooze Beam Protection (Professor Putricide)
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_APPLY_AURA; ///< for an unknown reason this was SPELL_EFFECT_APPLY_AREA_AURA_RAID
                break;
            case 69508: ///< Slime Spray
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                break;
            /// THIS IS HERE BECAUSE COOLDOWN ON CREATURE PROCS IS NOT IMPLEMENTED
            case 71604: ///< Mutated Strength (Professor Putricide)
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 72454: ///< Mutated Plague (Professor Putricide)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                break;
            case 70911: ///< Unbound Plague (Professor Putricide) (needs target selection script)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_TARGET_ENEMY;
                break;
            case 71518: ///< Unholy Infusion Quest Credit (Professor Putricide)
            case 72934: ///< Blood Infusion Quest Credit (Blood-Queen Lana'thel)
            case 72289: ///< Frost Infusion Quest Credit (Sindragosa)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< another missing radius
                break;
            case 70232: ///< Empowered Blood
            case 70320: ///< Empowered Blood
                spellInfo->Effects[EFFECT_0].MiscValue = 127;
                spellInfo->Effects[EFFECT_0].MiscValueB = 127;
                break;
            case 71708: ///< Empowered Flare (Blood Prince Council)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 71266: ///< Swarming Shadows
                spellInfo->AreaGroupId = 0; ///< originally, these require area 4522, which is... outside of Icecrown Citadel
                break;
            case 26023: ///< Pursuit of Justice (Rank 1)
                spellInfo->Effects[1].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[1].BasePoints = 14;
                spellInfo->Effects[1].ApplyAuraName = SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK;
                spellInfo->Effects[1].TargetA = TARGET_UNIT_CASTER;
                break;
            case 70715: ///< Column of Frost (visual marker)
                spellInfo->SetDurationIndex(32); ///< 6 seconds (missing)
                break;
            case 71085: ///< Mana Void (periodic aura)
                spellInfo->SetDurationIndex(9); ///< 30 seconds (missing)
                break;
            case 70936: ///< Summon Suppressor (needs target selection script)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 72706: ///< Achievement Check (Valithria Dreamwalker)
            case 71357: ///< Order Whelp
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS);   ///< 200yd
                break;
            case 70598: ///< Sindragosa's Fury
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 69846: ///< Frost Bomb
                spellInfo->Speed = 0.0f;    ///< This spell's summon happens instantly
                break;
            case 71614: ///< Ice Lock
                spellInfo->Mechanic = MECHANIC_STUN;
                break;
            case 72762: ///< Defile
                spellInfo->SetDurationIndex(559); ///< 53 seconds
                break;
            case 72743: ///< Defile
                spellInfo->SetDurationIndex(22); ///< 45 seconds
                break;
            case 72754: ///< Defile
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                break;
            case 69030: ///< Val'kyr Target Search
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                break;
            case 69198: ///< Raging Spirit Visual
                spellInfo->SetRangeIndex(13); ///< 50000yd
                break;
            case 73654: ///< Harvest Souls
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                break;
            case 72546: ///< Harvest Soul
            case 72597: ///< Harvest Soul
            case 72608: ///< Harvest Soul
                spellInfo->AttributesEx7 |= SPELL_ATTR7_ZONE_TELEPORT;
                break;
            case 73655: ///< Harvest Soul
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->AttributesEx7 |= SPELL_ATTR7_ZONE_TELEPORT;
                break;
            case 73540: ///< Summon Shadow Trap
                spellInfo->SetDurationIndex(23); ///< 90 seconds
                break;
            case 73530: ///< Shadow Trap (visual)
                spellInfo->SetDurationIndex(28); ///< 5 seconds
                break;
            case 73529: ///< Shadow Trap
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_10_YARDS); ///< 10yd
                break;
            case 74282: ///< Shadow Trap (searcher)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_3_YARDS); ///< 3yd
                break;
            case 72595: ///< Restore Soul
            case 73650: ///< Restore Soul
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                break;
            case 74086: ///< Destroy Soul
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                break;
            case 74302: ///< Summon Spirit Bomb
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 73579: ///< Summon Spirit Bomb
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_25_YARDS); ///< 25yd
                break;
            case 72350: ///< Fury of Frostmourne
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                spellInfo->Effects[EFFECT_1].Effect = SPELL_EFFECT_INSTAKILL;
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_SRC_CASTER;
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_UNIT_SRC_AREA_ENEMY;
                spellInfo->Effects[EFFECT_0].Amplitude = 50000;
                break;
            case 75127: ///< Kill Frostmourne Players
            case 72351: ///< Fury of Frostmourne
            case 72431: ///< Jump (removes Fury of Frostmourne debuff)
            case 72429: ///< Mass Resurrection
            case 73159: ///< Play Movie
            case 73582: ///< Trigger Vile Spirit (Inside, Heroic)
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                break;
            case 72376: ///< Raise Dead
                spellInfo->MaxAffectedTargets = 3;
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS); ///< 50000yd
                break;
            case 71809: ///< Jump
                spellInfo->SetRangeIndex(3); ///< 20yd
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(EFFECT_RADIUS_25_YARDS); ///< 25yd
                break;
            case 72405: ///< Broken Frostmourne
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(EFFECT_RADIUS_200_YARDS); ///< 200yd
                break;
            case 68872: ///< Soulstorm (Bronjahm Encounter)
                spellInfo->InterruptFlags = 0;
                break;
            /// ENDOF ICECROWN CITADEL SPELLS
            ///
            /// SHADOWFANG KEEP SPELLS
            /// Baron Ashbury
            case 93720: ///< Wracking Pain
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                break;
            case 93810: ///< Calamity dmg
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                break;
            case 93423: ///< Asphyxiate
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(28);
                break;
            case 93468: ///< Stay of Execution
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                break;
            case 93706: ///< Stay of Execution t
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(28);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(28);
                break;
            /// Baron Silverlaine
            case 93956: ///< Cursed Veil
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(18);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(18);
                break;
            /// Commander Springvale
            case 93722: ///< Shield of Perfidious dmg
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_CONE_ENEMY_24;
                break;
            /// Lord Valden
            case 93697: ///< Conjure Poisonous Mixture
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(18);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(18);
                break;
            case 93505: ///< Conjure Frost Mixture
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(18);
                break;
            case 93617: ///< Toxic Coagulant dmg
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(12);
                break;
            case 93689: ///< Toxic Catalyst dmg
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_1].SetRadiusIndex(12);
                spellInfo->Effects[EFFECT_2].SetRadiusIndex(12);
                break;
            /// Lord Godfrey
            case 96344: ///< Pistol Barrage sum
                spellInfo->Effects[EFFECT_0].SetRadiusIndex(12);
                break;
            /// ENDOF SHADOWFANG SPELLS
            ///
            /// DEADMINES SPELLS
            /// Admiral Ripsnarl
            case 88736: ///< Taste for Blood
                spellInfo->Effects[EFFECT_0].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[EFFECT_0].TriggerSpell = 0;
                spellInfo->ProcChance = 0;
                spellInfo->ProcFlags = 0;
                break;
            case 95647: ///< Ripsnarl Achievement Aura
                spellInfo->AttributesEx3 = SPELL_ATTR3_ONLY_TARGET_PLAYERS;
                break;
            case 127722: ///< Serpent's Zeal
                spellInfo->ProcChance = 100;
                break;
            /// Captain Cookie
            case 89250: ///< Summon Cauldron
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DEST;
                break;
            case 89268: ///< Throw Food Targeting
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
            /// Vanessa Vancleef
            case 92620: ///< Backslash targeting
                spellInfo->MaxAffectedTargets =1;
                break;
            /// ENDOF DEADMINES
            ///
            /// ULDUAR SPELLS
            ///
            case 64014: ///< Expedition Base Camp Teleport
            case 64032: ///< Formation Grounds Teleport
            case 64028: ///< Colossal Forge Teleport
            case 64031: ///< Scrapyard Teleport
            case 64030: ///< Antechamber Teleport
            case 64029: ///< Shattered Walkway Teleport
            case 64024: ///< Conservatory Teleport
            case 64025: ///< Halls of Invention Teleport
            case 64027: ///< Descent into Madness Teleport
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_DEST_DB;
                break;
            case 62374: ///< Pursued (Flame Leviathan)
                spellInfo->Effects[0].SetRadiusIndex(EFFECT_RADIUS_50000_YARDS);   ///< 50000yd
                break;
            case 62383: ///< Shatter (Ignis)
                spellInfo->SpellVisual[0] = 12639;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
            case 63342: ///< Focused Eyebeam Summon Trigger (Kologarn)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 63018: ///< Searing Light (XT-002)
            case 65121: ///< Searing Light (25m) (XT-002)
            case 63024: ///< Gravity Bomb (XT-002)
            case 64234: ///< Gravity Bomb (25m) (XT-002)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 62834: ///< Boom (XT-002)
            /// This hack is here because we suspect our implementation of spell effect execution on targets
            /// is done in the wrong order. We suspect that EFFECT_0 needs to be applied on all targets,
            /// then EFFECT_1, etc - instead of applying each effect on target1, then target2, etc.
            /// The above situation causes the visual for this spell to be bugged, so we remove the instakill
            /// effect and implement a script hack for that.
                spellInfo->Effects[EFFECT_1].Effect = 0;
                break;
            case 64386: ///< Terrifying Screech (Auriaya)
            case 64389: ///< Sentinel Blast (Auriaya)
            case 64678: ///< Sentinel Blast (Auriaya)
                spellInfo->SetDurationIndex(28); ///< 5 seconds, wrong DBC data?
                break;
            case 64321: ///< Potent Pheromones (Freya)
                /// spell should dispel area aura, but doesn't have the attribute
                /// may be db data bug, or blizz may keep reapplying area auras every update with checking immunity
                /// that will be clear if we get more spells with problem like this
                spellInfo->AttributesEx |= SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY;
                break;
            case 62301: ///< Cosmic Smash (Algalon the Observer)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 64598: ///< Cosmic Smash (Algalon the Observer)
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 62293: ///< Cosmic Smash (Algalon the Observer)
                spellInfo->Effects[EFFECT_0].TargetB = TARGET_DEST_CASTER;
                break;
            case 62311: ///< Cosmic Smash (Algalon the Observer)
            case 64596: ///< Cosmic Smash (Algalon the Observer)
                spellInfo->SetRangeIndex(6);  ///< 100yd
                break;
            /// ENDOF ULDUAR SPELLS
            ///
            case 108194: ///< Asphixiate
                spellInfo->SchoolMask = (SPELL_SCHOOL_MASK_SHADOW | SPELL_SCHOOL_MASK_NORMAL);
                break;
            case 49560: ///< Death Grip
            case 49576:
                spellInfo->SchoolMask = SPELL_SCHOOL_MASK_SHADOW;
                spellInfo->DmgClass = SPELL_DAMAGE_CLASS_MAGIC;
                spellInfo->Mechanic = MECHANIC_GRIP;
                spellInfo->Effects[0].Mechanic = MECHANIC_GRIP;
                break;
            case 114255: ///< Surge of Light (proc)
                spellInfo->StackAmount = 2;
                break;
            case 1543: ///< Flare
                spellInfo->Effects[0].TriggerSpell = 132950;
                spellInfo->ProcChance = 100;
                break;
            case 109772: ///< Flare
                spellInfo->SetDurationIndex(18); ///< 20s Duration
                spellInfo->AttributesEx3 |= SPELL_ATTR3_IGNORE_HIT_RESULT;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 123011: ///< Terrorize Player (tsulong spell)
                spellInfo->MaxAffectedTargets = 1;
                break;
            case 158221: ///< Hurricane Strike (damage)
                spellInfo->SetDurationIndex(39); ///< 2 seconds
                spellInfo->MaxAffectedTargets = 3;
                break;
            case 152118: ///< Clarity of Will
                spellInfo->InterruptFlags = 0x0000000F;
                break;
            case 115399: ///< Chi Brew
                /// SPELL_ATTR4_CAN_CAST_WHILE_CASTING bypass charge check and players can usebug (the check is only doing client-side)
                /// Maybe SPELL_ATTR4_CAN_CAST_WHILE_CASTING don't bypass charge ?
                spellInfo->AttributesEx4 &= ~SPELL_ATTR4_CAN_CAST_WHILE_CASTING;
                break;
            case 44425: ///< Arcane barrage
                spellInfo->AttributesEx6 &= ~SPELL_ATTR6_CANT_TARGET_CROWD_CONTROLLED;
                break;
            case 46916: ///< Bloodsurge
                /// Blizzard use -300%, idk why
                spellInfo->Effects[EFFECT_1].BasePoints = -100;
                break;
            case 125050: ///< Fetch (Glyph)
                spellInfo->Effects[EFFECT_0].TargetA = TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                break;
            case 77472: ///< Healing Wave
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                break;
            case 145153: ///< Dream of Cenarius (Heal from Wrath)
                spellInfo->Effects[0].TargetA = TARGET_UNIT_TARGET_ALLY;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 30884: ///< Nature's Guardian
                spellInfo->ProcFlags |= PROC_FLAG_TAKEN_DAMAGE;
                break;
            case 55440: ///< Glyph of Healing Wave (Restoration)
                spellInfo->ProcFlags = 0;
                break;
            case 171253: ///< Garrison heartstone
                spellInfo->Effects[EFFECT_0].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[EFFECT_0].TargetB = 0;
                spellInfo->Effects[EFFECT_0].ItemType = 0;
                break;
            case 124280:///< Touch of Karma (DoT)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REDIRECTED;
                spellInfo->AttributesEx |= SPELL_ATTR1_CANT_BE_REFLECTED;
                break;
            case 49016: ///< Unholy Frenzy
            case 87023: ///< Cauterize
            case 113344:///< Bloodbath (DoT)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                break;
            case 148022:///< Icicle hit
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 110914:// Dark Bargain (DoT)
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_DONE_BONUS;
                spellInfo->AttributesEx2 |= SPELL_ATTR2_CANT_CRIT;
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_TRIGGERED_IGNORE_RESILENCE;
                break;
            case 51657:
                spellInfo->Effects[SpellEffIndex::EFFECT_0].TargetA = Targets::TARGET_DEST_DEST;
                spellInfo->Effects[SpellEffIndex::EFFECT_1].Effect = 0;
                break;
            case 81297: ///< Consecration Damages
            case 81298: ///< Consecration Visual
                spellInfo->Effects[SpellEffIndex::EFFECT_0].TargetA = Targets::TARGET_DEST_DEST;
                break;
            case 159556:
                spellInfo->Effects[0].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[0].ApplyAuraName = SPELL_AURA_PERIODIC_DUMMY;
                spellInfo->Effects[0].Amplitude = 1000;
            case 159740: ///< Glyph of Raging Blow
                spellInfo->Effects[SpellEffIndex::EFFECT_0].TriggerSpell = 159747;
                break;
            case 159747: ///< Glyph of Raging Blow (heal mod)
                spellInfo->ProcFlags = 0;
                spellInfo->ProcChance = 0;
                break;
            case 84721: ///< Frozen Orb damage
                spellInfo->AttributesEx2 |= SpellAttr2::SPELL_ATTR2_CAN_TARGET_NOT_IN_LOS;
                break;
            case 33619: ///< Glyph of Reflective Shield damage
                spellInfo->AttributesEx3 |= SpellAttr3::SPELL_ATTR3_NO_DONE_BONUS;
                break;
            case 159456: ///< Glyph of Travel
                spellInfo->Stances = 0;
                break;
            case 174556:
                spellInfo->Effects[0].TargetA = TARGET_DEST_DEST;
                spellInfo->Effects[0].TargetB = 0;
                break;
            case 91809: ///< Leap
                spellInfo->Effects[EFFECT_1].ValueMultiplier = 0;
                break;
            case 91802: ///< Shambling Rush
                spellInfo->Effects[EFFECT_0].ValueMultiplier = 0;
                break;
            case 143333: ///< Water walking aura
                spellInfo->AuraInterruptFlags |= AURA_INTERRUPT_FLAG_NOT_MOUNTED;
                break;
            case 157698: ///< Haunting Spirits
                spellInfo->AttributesEx8 |= SPELL_ATTR8_DONT_RESET_PERIODIC_TIMER;
                break;
            case 165462: ///< Unleash Flame
                spellInfo->ProcCharges = 1;
                spellInfo->Effects[EFFECT_1].SpellClassMask[0] |= 0x10000000;
                break;
            case 89792: ///< Flee
                spellInfo->Effects[SpellEffIndex::EFFECT_0].TargetB = Targets::TARGET_UNIT_SUMMONER;
                spellInfo->Effects[SpellEffIndex::EFFECT_1].TargetB = Targets::TARGET_UNIT_SUMMONER;
                break;
            case 110310: ///< Dampening
                spellInfo->Effects[SpellEffIndex::EFFECT_1].Amplitude = 10000;  ///< 10 secs
                break;
            case 47180:///< Glyph Of Cat Form
                spellInfo->Stances = 0;
                break;
            case 108415: ///< Soul Link
            case 108446:
                spellInfo->Attributes &= ~SPELL_ATTR0_NOT_SHAPESHIFT;
                spellInfo->AttributesEx3 &= ~SPELL_ATTR3_CANT_TRIGGER_PROC;
                spellInfo->AttributesEx3 |= SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED;
                break;
            case 56805: ///< Glyph Of Kick
                spellInfo->Effects[2].Effect = SPELL_EFFECT_APPLY_AURA;
                spellInfo->Effects[2].ApplyAuraName = SPELL_AURA_DUMMY;
                spellInfo->Effects[2].TargetA = TARGET_UNIT_CASTER;
                spellInfo->Effects[2].BasePoints = 0;
                break;
            case 172944:
                spellInfo->Effects[SpellEffIndex::EFFECT_3].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[SpellEffIndex::EFFECT_3].TargetA = Targets::TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[SpellEffIndex::EFFECT_3].TargetB = 0;
                break;
            case 174732:    ///< 174732 Punt Podling
                spellInfo->Effects[SpellEffIndex::EFFECT_0].TargetA = Targets::TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[SpellEffIndex::EFFECT_0].TargetB = 0;
                spellInfo->Effects[SpellEffIndex::EFFECT_1].Effect = SPELL_EFFECT_DUMMY;
                spellInfo->Effects[SpellEffIndex::EFFECT_1].TargetA = Targets::TARGET_UNIT_TARGET_ANY;
                spellInfo->Effects[SpellEffIndex::EFFECT_1].TargetB = 0;
                break;
            case 170494:    ///< Flask of Conquest
                spellInfo->Effects[SpellEffIndex::EFFECT_0].MiscValue = CURRENCY_TYPE_CONQUEST_META_ARENA_BG;
                spellInfo->Effects[SpellEffIndex::EFFECT_1].MiscValue = CURRENCY_TYPE_CONQUEST_META_ARENA_BG;
                break;
            case 127250:    ///< Ancient Knowledge
                spellInfo->AttributesEx3 |= SPELL_ATTR3_DEATH_PERSISTENT;
                break;
            default:
                break;
        }

        switch (spellInfo->Id)
        {
            case 19386: ///< Wyvern Sting
            case 339:   ///< Entangling Roots
            case 118:   ///< Polymorph
            case 20066: ///< Repentance
            case 9484:  ///< Shackle Undead
            case 2094:  ///< Blind
            case 51514: ///< Hex
            case 710:   ///< Banish
            case 6358:  ///< Seduction
                spellInfo->AttributesEx3 |= SPELL_ATTR3_NO_INITIAL_AGGRO;
                break;
            default:
                break;
        }

        /// Handle some specific spells for AoE avoidance
        /// Many of AreaTrigger DoTs are single target spells but must be considered as AoE spells
        switch (spellInfo->Id)
        {
            case 155223: ///< Melt (Blast Furnace)
            case 155743: ///< Slag Pool (Blast Furnace)
            case 156932: ///< Rupture (Blast Furnace)
            case 176133: ///< Slag Bomb (Blast Furnace)
            case 155080: ///< Inferno Slice (Gruul)
            case 155301: ///< Overhead Smash (Gruul)
            case 155078: ///< Overwhelming Blows (Gruul)
            case 155530: ///< Shatter (Gruul)
            case 173192: ///< Cave In (Gruul)
            case 156203: ///< Retched Blackrock (Oregorger)
            case 155897: ///< Earthshaking Collision (Oregorger)
            case 156388: ///< Explosive Shard - Missile (Oregorger)
            case 156374: ///< Explosive Shard - Explosion (Oregorger)
            case 155900: ///< Rolling Fury (Oregorger)
            case 155318: ///< Lava Slash - AoE missile (Flamebender Ka'graz)
            case 155314: ///< Lava Slash - DoT (Flamebender Ka'graz)
            case 162370: ///< Crystalline Barrage (Tectus)
            case 162510: ///< Tectonic Upheaval (Tectus)
            case 159311: ///< Flame Jet (Kargath Bladefist)
            case 159002: ///< Berserker Rush (Kargath Bladefist)
            case 159413: ///< Mauling Brew (Kargath Bladefist)
            case 158519: ///< Quake (Twin Ogron)
            case 158241: ///< Blaze (Twin Ogron)
            case 157944: ///< Whirlwind (Twin Ogron)
            case 158336: ///< Pulverize - First AoE damage (Twin Ogron)
            case 158417: ///< Pulverize - Second AoE damage (Twin Ogron)
            case 158420: ///< Pulverize - Third AoE damage (Twin Ogron)
            case 156844: ///< Stone Breath (Kromog)
            case 156704: ///< Slam (Kromog)
            case 162349: ///< Fists of Stone (Kromog)
            case 157055: ///< Thundering Blows (Kromog)
            case 161893: ///< Shattered Earth (Kromog)
            case 157659: ///< Rippling Smash (Kromog)
            case 161923: ///< Rune of Crushing Earth (Kromog)
            case 157247: ///< Reverberations (Kromog)
            case 162516: ///< Whirling Steel (Iron Assembly Warden)
            case 162672: ///< Goring Swipe (Stubborn Ironhoof & Ornery Ironhoof)
            case 162675: ///< Ground Slam (Stubborn Ironhoof & Ornery Ironhoof)
            case 162663: ///< Electrical Storm - DoT (Thunderlord Beast-Tender)
            case 162772: ///< Colossal Roar (Markog Aba'dir)
            case 154956: ///< Pin Down - Damage (Beastlord Darmac)
            case 154960: ///< Pinned Down - DoT (Beastlord Darmac)
            case 155222: ///< Tantrum - AoE (Ironcrusher)
            case 155249: ///< Stampede - 1st (Ironcrusher)
            case 155531: ///< Stampede - 2nd (Ironcrusher)
            case 155520: ///< Tantrum - AoE (Beastlord Darmac)
            case 155061: ///< Rend and Tear - AoE (Cruelfang)
            case 155198: ///< Savage Howl (Cruelfang)
            case 162283: ///< Rend and Tear - AoE (Beastlord Darmac)
            case 154989: ///< Inferno Breath (Dreadwing)
            case 156824: ///< Inferno Pyre - DoT (Dreadwing)
            case 156823: ///< Superheated Scrap - DoT (Beastlord Darmac)
            case 155657: ///< Flame Infusion - DoT (Pack Beast)
            case 163754: ///< Iron Bellow (Grom'kar Man-at-Arms)
            case 163752: ///< Reckless Slash (Grom'kar Man-at-Arms)
            case 156655: ///< Throw Grenade (Iron Raider)
            case 158084: ///< Delayed Siege Bomb - Damage (Iron Gunnery Sergeant)
            case 160050: ///< Delayed Siege Bomb - Damage (Operator Thogar)
            case 165195: ///< Prototype Pulse Grenade - DoT (Operator Thogar)
            case 156553: ///< Moving Train (Operator Thogar)
                spellInfo->AttributesCu |= SPELL_ATTR0_CU_IS_CUSTOM_AOE_SPELL;
                break;
            default:
                break;
        }

        /// Speed needs to be set for some reason, else delay won't apply
        if (IsCCSpell(spellInfo) && !spellInfo->Speed)
            spellInfo->Speed = 12345.0f;

            switch (spellInfo->SpellFamilyName)
            {
                case SPELLFAMILY_GENERIC:
                    /// Wind lash (auto-attack of Ascendance)
                    if (spellInfo->Id == 147051)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                break;
                case SPELLFAMILY_SHAMAN:
                    /// Lava Lash / Windstrike Main Hand and Off Hand - Overide Stormstrike (17364) with Ascendance (114051) /
                    if (spellInfo->Id == 60103 || spellInfo->Id == 115357 || spellInfo->Id == 115360)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                case SPELLFAMILY_WARRIOR:
                    /// Shout
                    if (spellInfo->SpellFamilyFlags[0] & 0x20000 || spellInfo->SpellFamilyFlags[1] & 0x20)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                    /// Dragon Roar
                    if (spellInfo->Id == 118000)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                case SPELLFAMILY_DRUID:
                    /// Roar
                    if (spellInfo->SpellFamilyFlags[0] & 0x8)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_AURA_CC;
                    /// Rake
                    if (spellInfo->Id == 1822)
                        spellInfo->AttributesCu |= SPELL_ATTR0_CU_IGNORE_ARMOR;
                    break;
                default:
                    break;
            }

            ///< This must be re-done if targets changed since the spellinfo load
            spellInfo->ExplicitTargetMask = spellInfo->Effects[0].Effect == SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL ? TARGET_FLAG_UNIT : spellInfo->_GetExplicitTargetMask();

            switch (spellInfo->Id)
            {
                case 119392: ///< Charging Ox Wave
                case 147490: ///< Healing Rain
                case 120644: ///< Halo (damage)
                case 120517: ///< Halo (heal)
                case 61882:  ///< Earthquake
                case 152280: ///< Defile
                case 109248: ///< Binding Shot
                case 173229: ///< Creeping Moss (Brackenspore)
                case 102793: ///< Ursol's Vortex
                case 123986: ///< Chi Butst
                case 155738: ///< Slag Pool (Heart of the Mountain)
                case 174556:
                    spellInfo->ExplicitTargetMask &= ~TARGET_FLAG_UNIT;
                    break;
                case 116011:///< Rune of Power
                    spellInfo->ExplicitTargetMask &= ~TARGET_FLAG_UNIT;
                    spellInfo->Effects[2].Effect = 0;   ///< No summon needed
                    break;
                case 73680: ///< Unleash Elements
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_UNIT_ALLY;
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_UNIT_ENEMY;
                    break;
                case 107223:///< Sunfire Rays
                case 106736:///< Wither Will
                case 106113:///< Touch of Nothingness
                    spellInfo->ExplicitTargetMask = TARGET_FLAG_UNIT_MASK;
                    break;
                case 78675:///< Solar beam
                case 106112:///< Release Doubt
                    spellInfo->ExplicitTargetMask |= TARGET_FLAG_DEST_LOCATION;
                    break;
                default:
                    break;
            }

            /// Our targetting system is weird as fuck - would need a full rewrite for this to work properly, do not touch - hours of debugging
            if (spellInfo->HasEffect(SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL) || spellInfo->HasEffect(SPELL_EFFECT_TEACH_FOLLOWER_ABILITY) || spellInfo->HasEffect(SPELL_EFFECT_RANDOMIZE_FOLLOWER_ABILITIES))
                spellInfo->ExplicitTargetMask = TARGET_FLAG_NONE;

            spellInfo->UpdateSpellEffectCount(); ///< Re-cache the maximum number of effects
        }
    }

    CreatureAI::FillAISpellInfo();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded spell custom attributes in %u ms", GetMSTimeDiffToNow(oldMSTime));
}

void SpellMgr::LoadTalentSpellInfo()
{
    for (uint32 i = 0; i < sTalentStore.GetNumRows(); i++)
    {
        TalentEntry const* talent = sTalentStore.LookupEntry(i);
        if (!talent)
            continue;

        mTalentSpellInfo.insert(talent->SpellID);
    }
}

const SpellInfo* SpellMgr::GetSpellInfo(uint32 p_SpellID, Difficulty p_Difficulty) const
{
    if (p_SpellID < GetSpellInfoStoreSize())
    {
        if (p_Difficulty != DifficultyNone)
        {
            /// If spell isn't available in difficulty we want, check fallback difficulty ...
            DifficultyEntry const* l_Difficulty = sDifficultyStore.LookupEntry(p_Difficulty);
            while (l_Difficulty != nullptr)
            {
                SpellInfo const* l_SpellInfo = mSpellInfoMap[l_Difficulty->ID][p_SpellID];
                if (l_SpellInfo != nullptr)
                    return l_SpellInfo;

                l_Difficulty = sDifficultyStore.LookupEntry(l_Difficulty->FallbackDifficultyID);
            }
        }

        return mSpellInfoMap[DifficultyNone][p_SpellID];
    }

    return nullptr;
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

        if (spellPower->PowerType == power)
            return spellPower;
    }

    return NULL;
}

void SpellMgr::LoadSpellUpgradeItemStage()
{
    QueryResult l_Result = WorldDatabase.PQuery("SELECT ItemBonusTreeCategory, ItemClass, ItemSubClassMask, InventoryTypeMask, MaxIlevel FROM spell_upgrade_item_stage");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell upgrade item stage. DB table `spell_upgrade_item_stage` is empty.");
        return;
    }

    uint32 l_Count = 0;

    do
    {
        Field* l_Fields = l_Result->Fetch();

        SpellUpgradeItemStage l_SpellUpgradeItemStage;

        uint32 l_ItemBonusTreeCategory            = l_Fields[0].GetUInt32();
        l_SpellUpgradeItemStage.ItemClass         = l_Fields[1].GetInt32();
        l_SpellUpgradeItemStage.ItemSubclassMask  = l_Fields[2].GetInt32();
        l_SpellUpgradeItemStage.InventoryTypeMask = l_Fields[3].GetInt32();
        l_SpellUpgradeItemStage.MaxIlevel         = l_Fields[4].GetInt32();

        m_SpellUpgradeItemStages[l_ItemBonusTreeCategory].push_back(l_SpellUpgradeItemStage);
        l_Count++;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell upgrade item stage.", l_Count);
}
