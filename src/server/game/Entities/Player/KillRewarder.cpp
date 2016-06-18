////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "KillRewarder.h"
#include "SpellAuraEffects.h"
#include "Creature.h"
#include "Formulas.h"
#include "Group.h"
#ifndef CROSS
#include "Guild.h"
#endif /* not CROSS */
#include "GuildMgr.h"
#include "InstanceScript.h"
#include "Pet.h"
#include "Player.h"

// == KillRewarder ====================================================
// KillRewarder incapsulates logic of rewarding player upon kill with:
// * XP;
// * honor;
// * reputation;
// * kill credit (for quest objectives).
// Rewarding is initiated in two cases: when player kills unit in Unit::Kill()
// and on battlegrounds in Battleground::RewardXPAtKill().
//
// Rewarding algorithm is:
// 1. Initialize internal variables to default values.
// 2. In case when player is in group, initialize variables necessary for group calculations:
// 2.1. _count - number of alive group members within reward distance;
// 2.2. _sumLevel - sum of levels of alive group members within reward distance;
// 2.3. _maxLevel - maximum level of alive group member within reward distance;
// 2.4. _maxNotGrayMember - maximum level of alive group member within reward distance,
//      for whom victim is not gray;
// 2.5. _isFullXP - flag identifying that for all group members victim is not gray,
//      so 100% XP will be rewarded (50% otherwise).
// 3. Reward killer (and group, if necessary).
// 3.1. If killer is in group, reward group.
// 3.1.1. Initialize initial XP amount based on maximum level of group member,
//        for whom victim is not gray.
// 3.1.2. Alter group rate if group is in raid (not for battlegrounds).
// 3.1.3. Reward each group member (even dead) within reward distance (see 4. for more details).
// 3.2. Reward single killer (not group case).
// 3.2.1. Initialize initial XP amount based on killer's level.
// 3.2.2. Reward killer (see 4. for more details).
// 4. Reward player.
// 4.1. Give honor (player must be alive and not on BG).
// 4.2. Give XP.
// 4.2.1. If player is in group, adjust XP:
//        * set to 0 if player's level is more than maximum level of not gray member;
//        * cut XP in half if _isFullXP is false.
// 4.2.2. Apply auras modifying rewarded XP.
// 4.2.3. Give XP to player.
// 4.2.4. If player has pet, reward pet with XP (100% for single player, 50% for group case).
// 4.3. Give reputation (player must not be on BG).
// 4.4. Give kill credit (player must not be in group, or he must be alive or without corpse).
// 5. Credit instance encounter.
// 6. Update guild achievements.

KillRewarder::KillRewarder(Player* killer, Unit* victim, bool isBattleGround) :
// 1. Initialize internal variables to default values.
_killer(killer), _victim(victim), _group(killer->GetGroup()),
_groupRate(1.0f), _maxNotGrayMember(NULL), _count(0), _sumLevel(0), _xp(0),
_isFullXP(false), _maxLevel(0), _isBattleGround(isBattleGround), _isPvP(false)
{
    // mark the credit as pvp if victim is player
    if (victim->IsPlayer())
        _isPvP = true;
    // or if its owned by player and its not a vehicle
    else if (IS_PLAYER_GUID(victim->GetCharmerOrOwnerGUID()))
        _isPvP = !victim->IsVehicle();
    
    _InitGroupData();
}

inline void KillRewarder::_InitGroupData()
{
    if (_group)
    {
        // 2. In case when player is in group, initialize variables necessary for group calculations:
        for (GroupReference* itr = _group->GetFirstMember(); itr != NULL; itr = itr->next())
            if (Player* member = itr->getSource())
                if (member->isAlive() && member->IsAtGroupRewardDistance(_victim))
                {
                    const uint8 lvl = member->getLevel();
                    // 2.1. _count - number of alive group members within reward distance;
                    ++_count;
                    // 2.2. _sumLevel - sum of levels of alive group members within reward distance;
                    _sumLevel += lvl;
                    // 2.3. _maxLevel - maximum level of alive group member within reward distance;
                    if (_maxLevel < lvl)
                        _maxLevel = lvl;
                    // 2.4. _maxNotGrayMember - maximum level of alive group member within reward distance,
                    //      for whom victim is not gray;
                    uint32 grayLevel = JadeCore::XP::GetGrayLevel(lvl);
                    if (_victim->getLevel() > grayLevel && (!_maxNotGrayMember || _maxNotGrayMember->getLevel() < lvl))
                        _maxNotGrayMember = member;
                }
        // 2.5. _isFullXP - flag identifying that for all group members victim is not gray,
        //      so 100% XP will be rewarded (50% otherwise).
        _isFullXP = _maxNotGrayMember && (_maxLevel == _maxNotGrayMember->getLevel());
    }
    else
        _count = 1;
}

inline void KillRewarder::_InitXP(Player* player)
{
    // Get initial value of XP for kill.
    // XP is given:
    // * on battlegrounds;
    // * otherwise, not in PvP;
    // * not if killer is on vehicle.
    if (_isBattleGround || (!_isPvP && !_killer->GetVehicle()))
        _xp = JadeCore::XP::Gain(player, _victim);
}

inline void KillRewarder::_RewardHonor(Player* player)
{
    // Rewarded player must be alive.
    if (player->isAlive())
        player->RewardHonor(_victim, _count, -1, true);
}

inline void KillRewarder::_RewardXP(Player* p_Player, float p_Rate)
{
    uint32 l_Xp(_xp);
    if (_group)
    {
        // 4.2.1. If player is in group, adjust XP:
        //        * set to 0 if player's level is more than maximum level of not gray member;
        //        * cut XP in half if _isFullXP is false.
        if (_maxNotGrayMember && p_Player->isAlive() &&
            _maxNotGrayMember->getLevel() >= p_Player->getLevel())
            l_Xp = _isFullXP ?
            uint32(l_Xp * p_Rate) :             // Reward FULL XP if all group members are not gray.
            uint32(l_Xp * p_Rate / 2) + 1;      // Reward only HALF of XP if some of group members are gray.
        else
            l_Xp = 0;
    }
    if (l_Xp)
    {
        // 4.2.2. Apply auras modifying rewarded XP (SPELL_AURA_MOD_XP_PCT).
        Unit::AuraEffectList const& l_Auras = p_Player->GetAuraEffectsByType(SPELL_AURA_MOD_XP_PCT);
        for (Unit::AuraEffectList::const_iterator i = l_Auras.begin(); i != l_Auras.end(); ++i)
            AddPct(l_Xp, (*i)->GetAmount());
        
        // 4.2.3. Calculate expansion penalty
        if (_victim->GetTypeId() == TYPEID_UNIT && p_Player->getLevel() >= GetMaxLevelForExpansion(_victim->ToCreature()->GetCreatureTemplate()->expansion))
            l_Xp = CalculatePct(l_Xp, 10); // Players get only 10% xp for killing creatures of lower expansion levels than himself
        
        // 4.2.4. Give XP to player.
        p_Player->GiveXP(l_Xp, _victim, _groupRate);
        if (Pet* pet = p_Player->GetPet())
            // 4.2.5. If player has pet, reward pet with XP (100% for single player, 50% for group case).
            pet->GivePetXP(_group ? l_Xp / 2 : l_Xp);
        
        // Modificate xp for racial aura of trolls (+20% if beast)
        Unit::AuraEffectList const& l_AurasXpPct = p_Player->GetAuraEffectsByType(SPELL_AURA_MOD_XP_FROM_CREATURE_TYPE);
        for (Unit::AuraEffectList::const_iterator i = l_AurasXpPct.begin(); i != l_AurasXpPct.end(); ++i)
        {
            if (_victim->ToCreature() && _victim->ToCreature()->isType((*i)->GetMiscValue()))
                AddPct(l_Xp, (*i)->GetAmount());
        }
    }
}

inline void KillRewarder::_RewardReputation(Player* player, float rate)
{
    // 4.3. Give reputation (player must not be on BG).
    // Even dead players and corpses are rewarded.
    player->RewardReputation(_victim, rate);
}

inline void KillRewarder::_RewardKillCredit(Player* player)
{
    // 4.4. Give kill credit (player must not be in group, or he must be alive or without corpse).
    if (!_group || player->isAlive() || !player->GetCorpse())
        if (_victim->GetTypeId() == TYPEID_UNIT)
            player->KilledMonster(_victim->ToCreature()->GetCreatureTemplate(), _victim->GetGUID());
}

void KillRewarder::_RewardPlayer(Player* player, bool isDungeon)
{
    // 4. Reward player.
    if (!_isBattleGround)
    {
        // 4.1. Give honor (player must be alive and not on BG).
        _RewardHonor(player);
        // 4.1.1 Send player killcredit for quests with PlayerSlain
        if (_victim->IsPlayer())
            player->KilledPlayerCredit();
    }
    // Give XP only in PvE or in battlegrounds.
    // Give reputation and kill credit only in PvE.
    if (!_isPvP || _isBattleGround)
    {
        const float rate = _group ?
        _groupRate * float(player->getLevel()) / _sumLevel : // Group rate depends on summary level.
        1.0f;                                                // Personal rate is 100%.
        if (_xp)
        {
            // 4.2. Give XP.
            if (!(_victim->getLevel() > player->getLevel() && (_victim->getLevel() - player->getLevel()) > 6 && _group))
                _RewardXP(player, rate);
        }
        if (!_isBattleGround)
        {
            // If killer is in dungeon then all members receive full reputation at kill.
            _RewardReputation(player, isDungeon ? 1.0f : rate);
            _RewardKillCredit(player);
        }
    }
}

void KillRewarder::_RewardGroup()
{
    if (_maxLevel)
    {
        if (_maxNotGrayMember)
            // 3.1.1. Initialize initial XP amount based on maximum level of group member,
            //        for whom victim is not gray.
            _InitXP(_maxNotGrayMember);
        // To avoid unnecessary calculations and calls,
        // proceed only if XP is not ZERO or player is not on battleground
        // (battleground rewards only XP, that's why).
        if (!_isBattleGround || _xp)
        {
            const bool isDungeon = !_isPvP && sMapStore.LookupEntry(_killer->GetMapId())->IsDungeon();
            if (!_isBattleGround)
            {
                // 3.1.2. Alter group rate if group is in raid (not for battlegrounds).
                const bool isRaid = !_isPvP && sMapStore.LookupEntry(_killer->GetMapId())->IsRaid() && _group->isRaidGroup();
                _groupRate = JadeCore::XP::xp_in_group_rate(_count, isRaid);
            }
            
            // 3.1.3. Reward each group member (even dead or corpse) within reward distance.
            for (GroupReference* itr = _group->GetFirstMember(); itr != NULL; itr = itr->next())
                if (Player* member = itr->getSource())
                    if (member->IsAtGroupRewardDistance(_victim))
                        _RewardPlayer(member, isDungeon);
        }
    }
}

void KillRewarder::Reward()
{
    // 3. Reward killer (and group, if necessary).
    if (_group)
        // 3.1. If killer is in group, reward group.
        _RewardGroup();
    else
    {
        // 3.2. Reward single killer (not group case).
        // 3.2.1. Initialize initial XP amount based on killer's level.
        _InitXP(_killer);
        // To avoid unnecessary calculations and calls,
        // proceed only if XP is not ZERO or player is not on battleground
        // (battleground rewards only XP, that's why).
        if (!_isBattleGround || _xp)
            // 3.2.2. Reward killer.
            _RewardPlayer(_killer, false);
    }
    
    
    // 5. Credit instance encounter.
    // 6. Update guild achievements.
    if (Creature* victim = _victim->ToCreature())
    {
        if (InstanceScript* instance = _victim->GetInstanceScript())
            instance->UpdateEncounterState(ENCOUNTER_CREDIT_KILL_CREATURE, _victim->GetEntry(), _victim);
        
        if (uint32 guildId = victim->GetMap()->GetOwnerGuildId())
        {
#ifndef CROSS
            if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, victim->GetEntry(), 1, 0, victim, _killer);
#else /* CROSS */
            //if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                //guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, victim->GetEntry(), 1, 0, victim, _killer);
#endif /* CROSS */
        }
    }
}
