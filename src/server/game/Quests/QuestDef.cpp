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

#include "QuestDef.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"

Quest::Quest(Field* questRecord)
{
    uint8 index = 0;

    Id                      = questRecord[index++].GetUInt32();
    Method                  = questRecord[index++].GetUInt8();
    Level                   = questRecord[index++].GetInt16();
    MinLevel                = uint32(questRecord[index++].GetInt16());
    MaxLevel                = uint32(questRecord[index++].GetInt16());
    ZoneOrSort              = questRecord[index++].GetInt16();
    Type                    = questRecord[index++].GetUInt16();
    SuggestedPlayers        = questRecord[index++].GetUInt8();
    LimitTime               = questRecord[index++].GetUInt32();
    RequiredTeam            = questRecord[index++].GetInt8();
    RequiredClasses         = questRecord[index++].GetInt32();
    RequiredRaces           = questRecord[index++].GetInt32();
    RequiredSkillId         = questRecord[index++].GetUInt16();
    RequiredSkillPoints     = questRecord[index++].GetUInt16();
    RequiredMinRepFaction   = questRecord[index++].GetUInt16();
    RequiredMaxRepFaction   = questRecord[index++].GetUInt16();
    RequiredMinRepValue     = questRecord[index++].GetInt32();
    RequiredMaxRepValue     = questRecord[index++].GetInt32();
    PrevQuestId             = questRecord[index++].GetInt32();
    NextQuestId             = questRecord[index++].GetInt32();
    ExclusiveGroup          = questRecord[index++].GetInt32();
    NextQuestIdChain        = questRecord[index++].GetUInt32();
    RewardXPId              = questRecord[index++].GetUInt8();
    RewardMoney             = questRecord[index++].GetInt32();
    RewardMoneyMaxLevel     = questRecord[index++].GetUInt32();
    RewardSpell             = questRecord[index++].GetUInt32();
    RewardSpellCast         = questRecord[index++].GetInt32();
    RewardHonor             = questRecord[index++].GetUInt32();
    RewardHonorMultiplier   = questRecord[index++].GetFloat();
    RewardMailTemplateId    = questRecord[index++].GetUInt32();
    RewardMailDelay         = questRecord[index++].GetUInt32();
    SourceItemId            = questRecord[index++].GetUInt32();
    SourceSpellid           = questRecord[index++].GetUInt32();
    Flags                   = questRecord[index++].GetUInt32();
    Flags2                  = questRecord[index++].GetUInt32();
    SpecialFlags            = questRecord[index++].GetUInt8();
    MinimapTargetMark       = questRecord[index++].GetUInt32();
    RewardTitleId           = questRecord[index++].GetUInt8();
    RewardTalents           = questRecord[index++].GetUInt8();
    RewardArenaPoints       = questRecord[index++].GetUInt16();
    RewardSkillId           = questRecord[index++].GetUInt16();
    RewardSkillPoints       = questRecord[index++].GetUInt8();
    RewardReputationMask    = questRecord[index++].GetUInt8();
    QuestGiverPortrait      = questRecord[index++].GetUInt32();
    QuestTurnInPortrait     = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemId[i] = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemIdCount[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemId[i] = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemCount[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionId[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueId[i] = questRecord[index++].GetInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueIdOverride[i] = questRecord[index++].GetInt32();

    PointMapId              = questRecord[index++].GetUInt16();
    PointX                  = questRecord[index++].GetFloat();
    PointY                  = questRecord[index++].GetFloat();
    PointOption             = questRecord[index++].GetUInt32();
    Title                   = questRecord[index++].GetString();
    Objectives              = questRecord[index++].GetString();
    Details                 = questRecord[index++].GetString();
    EndText                 = questRecord[index++].GetString();
    CompletedText           = questRecord[index++].GetString();
    OfferRewardText         = questRecord[index++].GetString();
    RequestItemsText        = questRecord[index++].GetString();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemId[i] = questRecord[index++].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemCount[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyId[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyCount[i] = questRecord[index++].GetUInt16();

    QuestGiverTextWindow    = questRecord[index++].GetString();
    QuestGiverTargetName    = questRecord[index++].GetString();
    QuestTurnTextWindow     = questRecord[index++].GetString();
    QuestTurnTargetName     = questRecord[index++].GetString();
    SoundAccept             = questRecord[index++].GetUInt16();
    SoundTurnIn             = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[index++].GetUInt32();

    EmoteOnIncomplete       = questRecord[index++].GetUInt16();
    EmoteOnComplete         = questRecord[index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[index++].GetInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[index++].GetInt32();

    StartScript             = questRecord[index++].GetUInt32();
    CompleteScript          = questRecord[index++].GetUInt32();

    // int32 WDBVerified = questRecord[index++].GetInt32();

    if (SpecialFlags & QUEST_SPECIAL_FLAGS_AUTO_ACCEPT)
        Flags |= QUEST_FLAGS_AUTO_ACCEPT;

    m_rewItemsCount = 0;
    m_rewChoiceItemsCount = 0;
    m_rewCurrencyCount = 0;

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        if (RewardItemId[i])
            ++m_rewItemsCount;

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        if (RewardChoiceItemId[i])
            ++m_rewChoiceItemsCount;

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (RewardCurrencyId[i])
            ++m_rewCurrencyCount;

    for (QuestPackageItemEntry const* l_QuestPackageItem : sQuestPackageItemsByGroup[ExclusiveGroup])
        DynamicRewards.push_back(l_QuestPackageItem);

    memset(QuestObjecitveTypeCount, 0, sizeof(QuestObjecitveTypeCount));
}

uint32 Quest::XPValue(Player* player) const
{
    if (player)
    {
        int32 quest_level = (Level == -1 ? player->getLevel() : Level);
        const QuestXPEntry* xpentry = sQuestXPStore.LookupEntry(quest_level);
        if (!xpentry)
            return 0;

        int32 diffFactor = 2 * (quest_level - player->getLevel()) + 20;
        if (diffFactor < 1)
            diffFactor = 1;
        else if (diffFactor > 10)
            diffFactor = 10;

        uint32 xp = diffFactor * xpentry->Exp[RewardXPId] / 10;
        if (xp <= 100)
            xp = 5 * ((xp + 2) / 5);
        else if (xp <= 500)
            xp = 10 * ((xp + 5) / 10);
        else if (xp <= 1000)
            xp = 25 * ((xp + 12) / 25);
        else
            xp = 50 * ((xp + 25) / 50);

        return xp;
    }

    return 0;
}

int32 Quest::GetRewMoney() const
{
    return int32(RewardMoney * sWorld->getRate(RATE_DROP_MONEY));
}

QuestObjective const * Quest::GetQuestObjective(uint32 objectiveId) const
{
    for (uint32 l_I = 0; l_I < QuestObjectives.size(); ++l_I)
        if (QuestObjectives[l_I].ID == objectiveId)
            return &QuestObjectives[l_I];

    return nullptr;
}

uint32 Quest::GetQuestObjectiveId(uint32 p_QuestId, uint8 p_ObjIndex) const
{
    const Quest * l_Quest = sObjectMgr->GetQuestTemplate(p_QuestId);

    if (!l_Quest)
        return 0;

    if (uint32 l_ObjectiveId = QuestObjectives[p_ObjIndex].ID)
        return l_ObjectiveId;

    return 0;
}

QuestObjective const * Quest::GetQuestObjectiveXIndex(uint8 index) const
{
    for (uint32 l_I = 0; l_I < QuestObjectives.size(); ++l_I)
        if (QuestObjectives[l_I].Index == index)
            return &QuestObjectives[l_I];

    return nullptr;
}

QuestObjective const * Quest::GetQuestObjectiveXObjectId(uint32 objectId) const
{
    for (uint32 l_I = 0; l_I < QuestObjectives.size(); ++l_I)
        if (QuestObjectives[l_I].ObjectID == objectId)
            return &QuestObjectives[l_I];

    return nullptr;
}

uint8 Quest::GetQuestObjectiveCountType(uint8 type) const
{
    if (!type || type >= QUEST_OBJECTIVE_TYPE_END)
        return 0;

    return QuestObjecitveTypeCount[type];
}

bool Quest::IsAutoAccept() const
{
    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_ACCEPT) ? false : (Flags & QUEST_FLAGS_AUTO_ACCEPT);
}

bool Quest::IsAutoComplete() const
{
    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_AUTO_COMPLETE) ? false : (Method == 0 || HasFlag(QUEST_FLAGS_AUTOCOMPLETE));
}

bool Quest::IsAllowedInRaid() const
{
    if (IsRaidQuest())
        return true;

    return sWorld->getBoolConfig(CONFIG_QUEST_IGNORE_RAID);
}

uint32 Quest::CalculateHonorGain(uint8 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    return honor;
}
