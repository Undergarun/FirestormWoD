////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "QuestDef.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"

Quest::Quest(Field* p_QuestRecord)
{
    uint8 l_Index = 0;

    Id                      = p_QuestRecord[l_Index++].GetUInt32();
    Method                  = p_QuestRecord[l_Index++].GetUInt8();
    Level                   = p_QuestRecord[l_Index++].GetInt16();
    MinLevel                = uint32(p_QuestRecord[l_Index++].GetInt16());
    MaxLevel                = uint32(p_QuestRecord[l_Index++].GetInt16());
    PackageID               = p_QuestRecord[l_Index++].GetUInt32();
    ZoneOrSort              = p_QuestRecord[l_Index++].GetInt16();
    Type                    = p_QuestRecord[l_Index++].GetUInt16();
    SuggestedPlayers        = p_QuestRecord[l_Index++].GetUInt8();
    LimitTime               = p_QuestRecord[l_Index++].GetUInt32();
    RequiredTeam            = p_QuestRecord[l_Index++].GetInt8();
    RequiredClasses         = p_QuestRecord[l_Index++].GetInt32();
    RequiredRaces           = p_QuestRecord[l_Index++].GetInt32();
    RequiredSkillId         = p_QuestRecord[l_Index++].GetUInt16();
    RequiredSkillPoints     = p_QuestRecord[l_Index++].GetUInt16();
    RequiredMinRepFaction   = p_QuestRecord[l_Index++].GetUInt16();
    RequiredMaxRepFaction   = p_QuestRecord[l_Index++].GetUInt16();
    RequiredMinRepValue     = p_QuestRecord[l_Index++].GetInt32();
    RequiredMaxRepValue     = p_QuestRecord[l_Index++].GetInt32();
    PrevQuestId             = p_QuestRecord[l_Index++].GetInt32();
    NextQuestId             = p_QuestRecord[l_Index++].GetInt32();
    ExclusiveGroup          = p_QuestRecord[l_Index++].GetInt32();
    NextQuestIdChain        = p_QuestRecord[l_Index++].GetUInt32();
    RewardXPId              = p_QuestRecord[l_Index++].GetUInt8();
    RewardMoney             = p_QuestRecord[l_Index++].GetInt32();
    RewardMoneyMaxLevel     = p_QuestRecord[l_Index++].GetUInt32();
    RewardSpell             = p_QuestRecord[l_Index++].GetUInt32();
    RewardSpellCast         = p_QuestRecord[l_Index++].GetInt32();
    RewardHonor             = p_QuestRecord[l_Index++].GetUInt32();
    RewardHonorMultiplier   = p_QuestRecord[l_Index++].GetFloat();
    RewardMailTemplateId    = p_QuestRecord[l_Index++].GetUInt32();
    RewardMailDelay         = p_QuestRecord[l_Index++].GetUInt32();
    SourceItemId            = p_QuestRecord[l_Index++].GetUInt32();
    SourceSpellid           = p_QuestRecord[l_Index++].GetUInt32();
    Flags                   = p_QuestRecord[l_Index++].GetUInt32();
    Flags2                  = p_QuestRecord[l_Index++].GetUInt32();
    SpecialFlags            = p_QuestRecord[l_Index++].GetUInt8();
    MinimapTargetMark       = p_QuestRecord[l_Index++].GetUInt32();
    RewardTitleId           = p_QuestRecord[l_Index++].GetUInt8();
    RewardTalents           = p_QuestRecord[l_Index++].GetUInt8();
    RewardArenaPoints       = p_QuestRecord[l_Index++].GetUInt16();
    RewardSkillId           = p_QuestRecord[l_Index++].GetUInt16();
    RewardSkillPoints       = p_QuestRecord[l_Index++].GetUInt8();
    RewardReputationMask    = p_QuestRecord[l_Index++].GetUInt8();
    QuestGiverPortrait      = p_QuestRecord[l_Index++].GetUInt32();
    QuestTurnInPortrait     = p_QuestRecord[l_Index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemId[i] = p_QuestRecord[l_Index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewardItemIdCount[i] = p_QuestRecord[l_Index++].GetUInt16();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemId[i] = p_QuestRecord[l_Index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewardChoiceItemCount[i] = p_QuestRecord[l_Index++].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionId[i] = p_QuestRecord[l_Index++].GetUInt16();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueId[i] = p_QuestRecord[l_Index++].GetInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewardFactionValueIdOverride[i] = p_QuestRecord[l_Index++].GetInt32();

    PointMapId              = p_QuestRecord[l_Index++].GetUInt16();
    PointX                  = p_QuestRecord[l_Index++].GetFloat();
    PointY                  = p_QuestRecord[l_Index++].GetFloat();
    PointOption             = p_QuestRecord[l_Index++].GetUInt32();
    Title                   = p_QuestRecord[l_Index++].GetString();
    Objectives              = p_QuestRecord[l_Index++].GetString();
    Details                 = p_QuestRecord[l_Index++].GetString();
    EndText                 = p_QuestRecord[l_Index++].GetString();
    CompletedText           = p_QuestRecord[l_Index++].GetString();
    OfferRewardText         = p_QuestRecord[l_Index++].GetString();
    RequestItemsText        = p_QuestRecord[l_Index++].GetString();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemId[i] = p_QuestRecord[l_Index++].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        RequiredSourceItemCount[i] = p_QuestRecord[l_Index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyId[i] = p_QuestRecord[l_Index++].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        RewardCurrencyCount[i] = p_QuestRecord[l_Index++].GetUInt32();

    QuestGiverTextWindow    = p_QuestRecord[l_Index++].GetString();
    QuestGiverTargetName    = p_QuestRecord[l_Index++].GetString();
    QuestTurnTextWindow     = p_QuestRecord[l_Index++].GetString();
    QuestTurnTargetName     = p_QuestRecord[l_Index++].GetString();
    SoundAccept             = p_QuestRecord[l_Index++].GetUInt16();
    SoundTurnIn             = p_QuestRecord[l_Index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmote[i] = p_QuestRecord[l_Index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = p_QuestRecord[l_Index++].GetUInt32();

    EmoteOnIncomplete       = p_QuestRecord[l_Index++].GetUInt16();
    EmoteOnComplete         = p_QuestRecord[l_Index++].GetUInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = p_QuestRecord[l_Index++].GetInt16();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = p_QuestRecord[l_Index++].GetInt32();

    StartScript             = p_QuestRecord[l_Index++].GetUInt32();
    CompleteScript          = p_QuestRecord[l_Index++].GetUInt32();

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

    for (QuestPackageItemEntry const* l_QuestPackageItem : sQuestPackageItemsByGroup[PackageID])
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

/// remove this it's legacy
uint32 Quest::CalculateHonorGain(uint8 level) const
{
    if (level > GT_MAX_LEVEL)
        level = GT_MAX_LEVEL;

    uint32 honor = 0;

    return honor;
}
