////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "QuestDef.h"
#include "GossipDef.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Formulas.h"

GossipMenu::GossipMenu()
{
    _menuId = 0;
}

GossipMenu::~GossipMenu()
{
    ClearMenu();
}

void GossipMenu::AddMenuItem(int32 menuItemId, uint8 icon, std::string const& message, uint32 sender, uint32 action, std::string const& boxMessage, uint32 boxMoney, bool coded /*= false*/)
{
    ASSERT(_menuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    // Find a free new id - script case
    if (menuItemId == -1)
    {
        menuItemId = 0;
        if (!_menuItems.empty())
        {
            for (GossipMenuItemContainer::const_iterator itr = _menuItems.begin(); itr != _menuItems.end(); ++itr)
            {
                if (int32(itr->first) > menuItemId)
                    break;

                menuItemId = itr->first + 1;
            }
        }
    }

    GossipMenuItem& menuItem = _menuItems[menuItemId];

    menuItem.MenuItemIcon    = icon;
    menuItem.Message         = message;
    menuItem.IsCoded         = coded;
    menuItem.Sender          = sender;
    menuItem.OptionType      = action;
    menuItem.BoxMessage      = boxMessage;
    menuItem.BoxMoney        = boxMoney;
}

/**
 * @name AddMenuItem
 * @brief Adds a localized gossip menu item from db by menu id and menu item id.
 * @param menuId Gossip menu id.
 * @param menuItemId Gossip menu item id.
 * @param sender Identifier of the current menu.
 * @param action Custom action given to OnGossipHello.
 */
void GossipMenu::AddMenuItem(uint32 menuId, uint32 menuItemId, uint32 sender, uint32 action)
{
    /// Find items for given menu id.
    GossipMenuItemsMapBounds bounds = sObjectMgr->GetGossipMenuItemsMapBounds(menuId);
    /// Return if there are none.
    if (bounds.first == bounds.second)
        return;

    /// Iterate over each of them.
    for (GossipMenuItemsContainer::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
    {
        /// Find the one with the given menu item id.
        if (itr->second.OptionIndex != menuItemId)
            continue;

        /// Store texts for localization.
        std::string strOptionText = itr->second.OptionText;
        std::string strBoxText = itr->second.BoxText;

        /// Check need of localization.
        if (GetLocale() > LOCALE_enUS)
            /// Find localizations from database.
            if (GossipMenuItemsLocale const* no = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR64(menuId, menuItemId)))
            {
                /// Translate texts if there are any.
                ObjectMgr::GetLocaleString(no->OptionText, GetLocale(), strOptionText);
                ObjectMgr::GetLocaleString(no->BoxText, GetLocale(), strBoxText);
            }

        /// Add menu item with existing method. Menu item id -1 is also used in ADD_GOSSIP_ITEM macro.
        AddMenuItem(-1, itr->second.OptionIcon, strOptionText, sender, action, strBoxText, itr->second.BoxMoney, itr->second.BoxCoded);
    }
}

void GossipMenu::AddGossipMenuItemData(uint32 menuItemId, uint32 gossipActionMenuId, uint32 gossipActionPoi)
{
    GossipMenuItemData& itemData = _menuItemData[menuItemId];

    itemData.GossipActionMenuId  = gossipActionMenuId;
    itemData.GossipActionPoi     = gossipActionPoi;
}

uint32 GossipMenu::GetMenuItemSender(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.Sender;
}

uint32 GossipMenu::GetMenuItemAction(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.OptionType;
}

bool GossipMenu::IsMenuItemCoded(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return false;

    return itr->second.IsCoded;
}

void GossipMenu::ClearMenu()
{
    _menuItems.clear();
    _menuItemData.clear();
}

PlayerMenu::PlayerMenu(WorldSession* session) : _session(session)
{
    if (_session)
        _gossipMenu.SetLocale(_session->GetSessionDbLocaleIndex());
}

PlayerMenu::~PlayerMenu()
{
    ClearMenus();
}

void PlayerMenu::ClearMenus()
{
    _gossipMenu.ClearMenu();
    _questMenu.ClearMenu();
}

#define DEFAULT_GREETINGS_GOSSIP      68

void PlayerMenu::SendGossipMenu(uint32 titleTextId, uint64 objectGUID) const
{
    WorldPacket l_Data(SMSG_GOSSIP_MESSAGE, 2 * 1024);

    l_Data.appendPackGUID(objectGUID);

    if (titleTextId == DEFAULT_GOSSIP_MESSAGE && !_gossipMenu.GetMenuId())
        l_Data << uint32(DEFAULT_GREETINGS_GOSSIP);           // default greeting ID
    else
        l_Data << uint32(_gossipMenu.GetMenuId());            // new 2.4.0

    l_Data << uint32(0);                                      // Friendship faction
    l_Data << uint32(titleTextId);
    l_Data << uint32(_gossipMenu.GetMenuItems().size());
    l_Data << uint32(_questMenu.GetMenuItemCount());

    for (GossipMenuItemContainer::const_iterator l_It = _gossipMenu.GetMenuItems().begin(); l_It != _gossipMenu.GetMenuItems().end(); ++l_It)
    {
        GossipMenuItem const& l_Item = l_It->second;

        l_Data << uint32(l_It->first);
        l_Data << uint8(l_Item.MenuItemIcon);
        l_Data << uint8(l_Item.IsCoded);                                    // makes pop up box password
        l_Data << uint32(l_Item.BoxMoney);                                  // money required to open menu, 2.0.3

        l_Data.WriteBits(l_Item.Message.size(), 12);
        l_Data.WriteBits(l_Item.BoxMessage.size(), 12);
        l_Data.FlushBits();

        l_Data.WriteString(l_Item.Message);
        l_Data.WriteString(l_Item.BoxMessage);
    }

    for (uint32 l_It = 0; l_It < _questMenu.GetMenuItemCount(); ++l_It)
    {
        QuestMenuItem const& l_Item = _questMenu.GetItem(l_It);
        uint32 l_QuestID = l_Item.QuestId;
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestID);
        std::string l_Title = l_Quest->GetTitle();

        int l_Locale = _session->GetSessionDbLocaleIndex();
        if (l_Locale >= 0)
        {
            if (QuestLocale const* l_LocalData = sObjectMgr->GetQuestLocale(l_QuestID))
                ObjectMgr::GetLocaleString(l_LocalData->Title, l_Locale, l_Title);
        }

        Player * l_Player = _session->GetPlayer();

        uint32 l_QuestStat = l_Player ? l_Player->GetQuestStatus(l_QuestID) : 0;

        if (l_QuestStat == QUEST_STATUS_COMPLETE || l_QuestStat == QUEST_STATUS_INCOMPLETE)
        {
            if (l_Quest->IsRepeatable())
                l_QuestStat = 0;
            else
                l_QuestStat = 4;
        }
        else if (l_QuestStat == QUEST_STATE_NONE)
            l_QuestStat = 2;

        l_Data << uint32(l_QuestID);
        l_Data << uint32(l_QuestStat);                      // quest icon
        l_Data << int32(l_Quest->GetQuestLevel());          // quest level
        l_Data << uint32(l_Quest->GetFlags());              // quest flags
        l_Data << uint32(l_Quest->GetFlags2());             // quest flags 2

        l_Data.WriteBit(l_Quest->IsRepeatable());           // 3.3.3 changes icon: blue question or yellow exclamation, is repeatable
        l_Data.WriteBits(l_Title.size(), 9);
        l_Data.FlushBits();

        l_Data.WriteString(l_Title);
    }

    _session->SendPacket(&l_Data);
}

void PlayerMenu::SendCloseGossip() const
{
    WorldPacket data(SMSG_GOSSIP_COMPLETE, 0);
    _session->SendPacket(&data);
}

void PlayerMenu::SendPointOfInterest(uint32 poiId) const
{
    PointOfInterest const* poi = sObjectMgr->GetPointOfInterest(poiId);
    if (!poi)
    {
        sLog->outError(LOG_FILTER_SQL, "Request to send non-existing POI (Id: %u), ignored.", poiId);
        return;
    }

    std::string iconText = poi->icon_name;
    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
        if (PointOfInterestLocale const* localeData = sObjectMgr->GetPointOfInterestLocale(poiId))
            ObjectMgr::GetLocaleString(localeData->IconName, locale, iconText);

    WorldPacket data(SMSG_GOSSIP_POI, 3 + 4 + 4 + 4 + 4 + iconText.size());
    data.WriteBits(poi->flags, 14);
    data.WriteBits(iconText.length(), 6);
    data.FlushBits();
    data << float(poi->x);
    data << float(poi->y);
    data << uint32(poi->icon);
    data << uint32(poi->data);
    data.WriteString(iconText);

    _session->SendPacket(&data);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(32);                                   // can be set for max from most often sizes to speedup push_back and less memory use
}

QuestMenu::~QuestMenu()
{
    ClearMenu();
}

void QuestMenu::AddMenuItem(uint32 QuestId, uint8 Icon)
{
    if (!sObjectMgr->GetQuestTemplate(QuestId))
        return;

    ASSERT(_questMenuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    QuestMenuItem questMenuItem;

    questMenuItem.QuestId        = QuestId;
    questMenuItem.QuestIcon      = Icon;

    _questMenuItems.push_back(questMenuItem);
}

bool QuestMenu::HasItem(uint32 questId) const
{
    for (QuestMenuItemList::const_iterator i = _questMenuItems.begin(); i != _questMenuItems.end(); ++i)
        if (i->QuestId == questId)
            return true;

    return false;
}

void QuestMenu::ClearMenu()
{
    _questMenuItems.clear();
}

void PlayerMenu::SendQuestGiverQuestList(QEmote eEmote, const std::string& Title, uint64 npcGUID)
{
    WorldPacket data(SMSG_QUEST_GIVER_QUEST_LIST_MESSAGE, 1024);
    data.appendPackGUID(npcGUID);
    data << uint32(eEmote._Emote);                         // NPC emote
    data << uint32(eEmote._Delay);                         // player emote
    data << uint32(_questMenu.GetMenuItemCount());

    for (uint32 count = 0; count < _questMenu.GetMenuItemCount(); ++count)
    {
        QuestMenuItem const& qmi = _questMenu.GetItem(count);

        uint32 questID = qmi.QuestId;

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            Player* plr = _session->GetPlayer();
            std::string title = quest->GetTitle();

            int loc_idx = _session->GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
            if (QuestLocale const* ql = sObjectMgr->GetQuestLocale(questID))
                ObjectMgr::GetLocaleString(ql->Title, loc_idx, title);

            uint32 questStat = plr ? plr->GetQuestStatus(questID) : 0;

            if (questStat == QUEST_STATUS_COMPLETE || questStat == QUEST_STATUS_INCOMPLETE)
            {
                if (quest->IsRepeatable())
                    questStat = 0;
                else
                    questStat = 4;
            }
            else if (questStat == QUEST_STATE_NONE)
                questStat = 2;

            data << uint32(questID);
            data << uint32(questStat);                      // quest icon
            data << int32(quest->GetQuestLevel());          // quest level
            data << uint32(quest->GetFlags());              // quest flags
            data << uint32(quest->GetFlags2());             // quest flags 2

            data.WriteBit(quest->IsRepeatable());
            data.WriteBits(title.length(), 9);
            data.FlushBits();

            data.WriteString(title);
        }
    }

    data.WriteBits(Title.size(), 11);
    data.FlushBits();
    data.WriteString(Title);

    _session->SendPacket(&data);
}

void PlayerMenu::SendQuestGiverStatus(uint32 p_StatusFlags, uint64 p_QuestGiverGUID) const
{
    WorldPacket data(SMSG_QUEST_GIVER_STATUS);
    data.appendPackGUID(p_QuestGiverGUID);
    data << uint32(p_StatusFlags);

    _session->SendPacket(&data);
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const* quest, uint64 npcGUID) const
{
    std::string questTitle           = quest->GetTitle();
    std::string questDetails         = quest->GetDetails();
    std::string questObjectives      = quest->GetObjectives();
    std::string questEndText         = quest->GetEndText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow  = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName  = quest->GetQuestTurnTargetName();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->Details, locale, questDetails);
            ObjectMgr::GetLocaleString(localeData->Objectives, locale, questObjectives);
            ObjectMgr::GetLocaleString(localeData->EndText, locale, questEndText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, questTurnTargetName);
        }
    }

    ObjectGuid guid  = npcGUID;
    ObjectGuid guid2 = npcGUID;

    bool hiddenRewardItem = quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS);
    bool l_DisplayPopup   = false;
    bool l_StartCheat     = false;
    bool l_AutoLaunched   = false;


    uint32 l_LearnSpellCount = 0;

    float QuestXpRate = 1;
    if (_session->GetPlayer()->GetPersonnalXpRate())
        QuestXpRate = _session->GetPlayer()->GetPersonnalXpRate();
    else
        QuestXpRate = sWorld->getRate(RATE_XP_QUEST);

    WorldPacket data(SMSG_QUEST_GIVER_QUEST_DETAILS, 2 * 1024);

    data.appendPackGUID(guid);
    data.appendPackGUID(guid2);

    data << uint32(quest->GetQuestId());
    data << uint32(quest->GetQuestPackageID());
    data << uint32(quest->GetQuestGiverPortrait());
    data << uint32(quest->GetSuggestedPlayers());
    data << uint32(quest->GetFlags());
    data << uint32(quest->GetFlags2());
    data << uint32(quest->GetQuestTurnInPortrait());
    data << uint32(l_LearnSpellCount);

    data << uint32(hiddenRewardItem ? 0 : quest->GetRewChoiceItemsCount());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[0]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[0]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[1]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[1]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[2]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[2]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[3]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[3]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[4]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[4]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[5]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[5]);

    data << uint32(hiddenRewardItem ? 0 : quest->GetRewItemsCount());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[0]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[0]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[1]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[1]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[2]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[2]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[3]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[3]);

    data << uint32(quest->GetRewMoney());
    data << uint32(quest->XPValue(_session->GetPlayer()) * QuestXpRate);
    data << uint32(quest->GetCharTitleId());
    data << uint32(quest->GetBonusTalents());
    data << uint32(quest->GetRewardReputationMask());

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
    {
        data << uint32(quest->RewardFactionId[i]);
        data << uint32(quest->RewardFactionValueId[i]);
        data << uint32(quest->RewardFactionValueIdOverride[i]);
    }

    data << uint32(quest->GetRewSpell());                                                   ///< Spell Completion Display ID
    data << uint32(quest->GetRewSpellCast());                                               ///< Spell Completion ID

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyId[i]);
        data << uint32(quest->RewardCurrencyCount[i]);
    }

    data << uint32(quest->GetRewardSkillId());
    data << uint32(quest->GetRewardSkillPoints());

    data.WriteBit(false);                                                                   ///< Unk
    data.FlushBits();

    data << uint32(QUEST_EMOTE_COUNT);
    data << uint32(quest->QuestObjectives.size());

    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        data << uint32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }

    for (auto l_Objective : quest->QuestObjectives)
    {
        data << uint32(l_Objective.ID);
        data << uint32(l_Objective.ObjectID);
        data << uint32(l_Objective.Amount);
        data << uint8(l_Objective.Type);
    }

    data.WriteBits(questTitle.size(), 9);
    data.WriteBits(questDetails.size(), 12);
    data.WriteBits(questObjectives.size(), 12);
    data.WriteBits(questGiverTextWindow.size(), 10);
    data.WriteBits(questGiverTargetName.size(), 8);
    data.WriteBits(questTurnTextWindow.size(), 10);
    data.WriteBits(questTurnTargetName.size(), 8);

    data.WriteBit(l_DisplayPopup);
    data.WriteBit(l_StartCheat);
    data.WriteBit(l_AutoLaunched);
    data.FlushBits();

    data.WriteString(questTitle);
    data.WriteString(questDetails);
    data.WriteString(questObjectives);
    data.WriteString(questGiverTextWindow);
    data.WriteString(questGiverTargetName);
    data.WriteString(questTurnTextWindow);
    data.WriteString(questTurnTargetName);

    _session->SendPacket(&data);
}

void PlayerMenu::SendQuestQueryResponse(Quest const* p_Quest) const
{
    std::string l_Title = p_Quest->GetTitle();
    std::string l_Details = p_Quest->GetDetails();
    std::string l_ObjectivesText = p_Quest->GetObjectives();
    std::string l_EndText = p_Quest->GetEndText();
    std::string l_CompletedText = p_Quest->GetCompletedText();
    std::string l_GiverTextWindow = p_Quest->GetQuestGiverTextWindow();
    std::string l_GiverTargetName = p_Quest->GetQuestGiverTargetName();
    std::string l_TurnTextWindow = p_Quest->GetQuestTurnTextWindow();
    std::string l_TurnTargetName = p_Quest->GetQuestTurnTargetName();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(p_Quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, l_Title);
            ObjectMgr::GetLocaleString(localeData->Details, locale, l_Details);
            ObjectMgr::GetLocaleString(localeData->Objectives, locale, l_ObjectivesText);
            ObjectMgr::GetLocaleString(localeData->EndText, locale, l_EndText);
            ObjectMgr::GetLocaleString(localeData->CompletedText, locale, l_CompletedText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, l_GiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, l_GiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, l_TurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, l_TurnTargetName);
        }
    }

    bool l_HideItemReward = p_Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DYNAMIC_ITEM_REWARD) || p_Quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS);

    WorldPacket l_Data(SMSG_QUERY_QUEST_INFO_RESPONSE, 3 * 1024);
    l_Data << uint32(p_Quest->GetQuestId());
    l_Data.WriteBit(1);                                                                         ///< has data
    l_Data.FlushBits();
    
    l_Data << uint32(p_Quest->GetQuestId());                                                    ///< Quest ID
    l_Data << uint32(p_Quest->GetQuestMethod());                                                ///< Quest Method
    l_Data << uint32(p_Quest->GetQuestLevel());                                                 ///< Quest Level
    l_Data << uint32(p_Quest->GetQuestPackageID());                                             ///< Quest package ID
    l_Data << uint32(p_Quest->GetMinLevel());                                                   ///< Quest Min Level
    l_Data << uint32(p_Quest->GetZoneOrSort());                                                 ///< Quest sort ID
    l_Data << uint32(p_Quest->GetType());                                                       ///< Quest Type
    l_Data << uint32(p_Quest->GetSuggestedPlayers());                                           ///< Suggested Group Num
    l_Data << uint32(p_Quest->GetNextQuestInChain());                                           ///< Next Quest In Chain
    l_Data << uint32(p_Quest->GetXPId());                                                       ///< Reward XP Difficulty
    l_Data << float(0);                                                                         ///< XPMultiplier in TC
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->GetRewMoney());                            ///< Reward Money
    l_Data << uint32(p_Quest->GetRewMoneyMaxLevel());                                           ///< Reward Money Difficulty
    l_Data << float(0);                                                                         ///< RewardMoneyMultiplier in TC
    l_Data << uint32(0);                                                                        ///< RewardBonusMoney in TC
    l_Data << uint32(p_Quest->GetRewSpell());                                                   ///< Reward Display Spell, this spell will display (icon) (casted if RewSpellCast == 0)
    l_Data << int32(p_Quest->GetRewSpellCast());                                                ///< Reward Spell
    l_Data << uint32(p_Quest->GetRewHonorAddition());                                           ///< Reward Honor
    l_Data << float(p_Quest->GetRewHonorMultiplier());                                          ///< Reward Kill Honor
    l_Data << uint32(p_Quest->GetSrcItemId());                                                  ///< Start Item
    l_Data << uint32(p_Quest->GetFlags());                                                      ///< Flags
    l_Data << uint32(p_Quest->GetFlags2());                                                     ///< Flags EX

    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemId[0]);                          ///< Reward Items [0]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemIdCount[0]);                     ///< Reward Amount [0]
    l_Data << uint32(p_Quest->RequiredSourceItemId[0]);                                         ///< Item Drop [0]
    l_Data << uint32(p_Quest->RequiredSourceItemCount[0]);                                      ///< Item Drop Quantity [0]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemId[1]);                          ///< Reward Items [1]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemIdCount[1]);                     ///< Reward Amount [1]
    l_Data << uint32(p_Quest->RequiredSourceItemId[1]);                                         ///< Item Drop [1]
    l_Data << uint32(p_Quest->RequiredSourceItemCount[1]);                                      ///< Item Drop Quantity [1]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemId[2]);                          ///< Reward Items [2]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemIdCount[2]);                     ///< Reward Amount [2]
    l_Data << uint32(p_Quest->RequiredSourceItemId[2]);                                         ///< Item Drop [2]
    l_Data << uint32(p_Quest->RequiredSourceItemCount[2]);                                      ///< Item Drop Quantity [2]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemId[3]);                          ///< Reward Items [3]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardItemIdCount[3]);                     ///< Reward Amount [3]
    l_Data << uint32(p_Quest->RequiredSourceItemId[3]);                                         ///< Item Drop [3]
    l_Data << uint32(p_Quest->RequiredSourceItemCount[3]);                                      ///< Item Drop Quantity [3]

    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemId[0]);                    ///< Unfiltered Choice Items ID [0]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemCount[0]);                 ///< Unfiltered Choice Items Quantity [0]
    l_Data << uint32(0);                                                                        ///< Unfiltered Choice Items Display ID [0]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemId[1]);                    ///< Unfiltered Choice Items ID [1]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemCount[1]);                 ///< Unfiltered Choice Items Quantity [1]
    l_Data << uint32(0);                                                                        ///< Unfiltered Choice Items Display ID [1]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemId[2]);                    ///< Unfiltered Choice Items ID [2]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemCount[2]);                 ///< Unfiltered Choice Items Quantity [2]
    l_Data << uint32(0);                                                                        ///< Unfiltered Choice Items Display ID [2]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemId[3]);                    ///< Unfiltered Choice Items ID [3]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemCount[3]);                 ///< Unfiltered Choice Items Quantity [3]
    l_Data << uint32(0);                                                                        ///< Unfiltered Choice Items Display ID [3]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemId[4]);                    ///< Unfiltered Choice Items ID [4]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemCount[4]);                 ///< Unfiltered Choice Items Quantity [4]
    l_Data << uint32(0);                                                                        ///< Unfiltered Choice Items Display ID [4]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemId[5]);                    ///< Unfiltered Choice Items ID [5]
    l_Data << uint32(l_HideItemReward ? 0 : p_Quest->RewardChoiceItemCount[5]);                 ///< Unfiltered Choice Items Quantity [5]
    l_Data << uint32(0);                                                                        ///< Unfiltered Choice Items Display ID [5]

    l_Data << uint32(p_Quest->GetPointMapId());                                                 ///< POI Continent
    l_Data << float(p_Quest->GetPointX());                                                      ///< POI x
    l_Data << float(p_Quest->GetPointY());                                                      ///< POI y
    l_Data << uint32(p_Quest->GetPointOpt());                                                   ///< POI Priority
    l_Data << uint32(p_Quest->GetCharTitleId());                                                ///< Reward Title
    l_Data << uint32(p_Quest->GetRewArenaPoints());                                             ///< Reward Arena Points
    l_Data << uint32(0);                                                                        ///< RewardSkillLineID in TC
    l_Data << uint32(p_Quest->GetRewardSkillId());                                              ///< Reward Skill Line ID
    l_Data << uint32(p_Quest->GetRewardSkillPoints());                                          ///< Reward NumS kill Ups
    l_Data << uint32(p_Quest->GetQuestGiverPortrait());                                         ///< Portrait Giver
    l_Data << uint32(p_Quest->GetQuestTurnInPortrait());                                        ///< Portrait Turn In

    for (uint32 l_I = 0; l_I < QUEST_REPUTATIONS_COUNT; ++l_I)
    {
        l_Data << uint32(p_Quest->RewardFactionId[l_I]);                                        ///< Reward Faction ID
        l_Data << uint32(p_Quest->RewardFactionValueId[l_I]);                                   ///< Reward Faction Value
        l_Data << uint32(p_Quest->RewardFactionValueIdOverride[l_I]);                           ///< Reward Faction Override
    }

    l_Data << uint32(p_Quest->GetRewardReputationMask());                                       ///< Reward Faction Flags

    for (uint32 l_I = 0; l_I < QUEST_REWARD_CURRENCY_COUNT; ++l_I)
    {
        l_Data << uint32(p_Quest->RewardCurrencyId[l_I]);                                       ///< Reward Currency ID
        l_Data << uint32(p_Quest->RewardCurrencyCount[l_I]);                                    ///< Reward Currency Qty
    }

    l_Data << uint32(p_Quest->GetSoundAccept());                                                ///< Accepted Sound Kit ID
    l_Data << uint32(p_Quest->GetSoundTurnIn());                                                ///< Complete Sound Kit ID
    l_Data << uint32(0);                                                                        ///< AreaGroupID
    l_Data << uint32(p_Quest->GetLimitTime());                                                  ///< Time Allowed
    l_Data << uint32(p_Quest->QuestObjectives.size());                                          ///< Objective Count
    l_Data << uint32(0);                                                                        ///< AllowableRaces

    for (QuestObjective l_Objective : p_Quest->QuestObjectives)
    {
        std::string l_DescriptionText = l_Objective.Description;

        if (locale > 0)
        {
            if (QuestObjectiveLocale const* questObjectiveLocale = sObjectMgr->GetQuestObjectiveLocale(l_Objective.ID))
                ObjectMgr::GetLocaleString(questObjectiveLocale->Description, locale, l_DescriptionText);
        }

        l_Data << uint32(l_Objective.ID);                                                       ///< Id
        l_Data << uint8(l_Objective.Type);                                                      ///< Type
        l_Data << int8(l_Objective.Index);                                                      ///< Storage Index
        l_Data << uint32(l_Objective.ObjectID);                                                 ///< Object ID
        l_Data << uint32(l_Objective.Amount);                                                   ///< Amount
        l_Data << uint32(l_Objective.Flags);                                                    ///< Flags
        l_Data << float(l_Objective.UnkFloat);                                                  ///< Unk
        l_Data << uint32(l_Objective.VisualEffects.size());                                     ///< Visual Effects Count

        for (uint32 l_I = 0; l_I < l_Objective.VisualEffects.size(); ++l_I)
            l_Data << uint32(l_Objective.VisualEffects[l_I]);                                   ///< Visual Effects[l_I]

        l_Data.WriteBits(l_DescriptionText.size(), 8);                                          ///< Description
        l_Data.FlushBits();

        l_Data.WriteString(l_DescriptionText);                                                  ///< Description
    }

    l_Data.WriteBits(l_Title.size(), 9);
    l_Data.WriteBits(l_ObjectivesText.size(), 12);
    l_Data.WriteBits(l_Details.size(), 12);
    l_Data.WriteBits(l_EndText.size(), 9);
    l_Data.WriteBits(l_GiverTextWindow.size(), 10);
    l_Data.WriteBits(l_GiverTargetName.size(), 8);
    l_Data.WriteBits(l_TurnTextWindow.size(), 10);
    l_Data.WriteBits(l_TurnTargetName.size(), 8);
    l_Data.WriteBits(l_CompletedText.size(), 11);
    l_Data.FlushBits();

    l_Data.WriteString(l_Title);
    l_Data.WriteString(l_ObjectivesText);
    l_Data.WriteString(l_Details);
    l_Data.WriteString(l_EndText);
    l_Data.WriteString(l_GiverTextWindow);
    l_Data.WriteString(l_GiverTargetName);
    l_Data.WriteString(l_TurnTextWindow);
    l_Data.WriteString(l_TurnTargetName);
    l_Data.WriteString(l_CompletedText);

    _session->SendPacket(&l_Data);
}

void PlayerMenu::SendQuestGiverOfferReward(Quest const* p_Quest, uint64 p_NpcGUID, bool p_EnableNext) const
{
    std::string l_QuestTitle           = p_Quest->GetTitle();
    std::string l_QuestOfferRewardText = p_Quest->GetOfferRewardText();
    std::string l_QuestGiverTextWindow = p_Quest->GetQuestGiverTextWindow();
    std::string l_QuestGiverTargetName = p_Quest->GetQuestGiverTargetName();
    std::string l_QuestTurnTextWindow  = p_Quest->GetQuestTurnTextWindow();
    std::string l_QuestTurnTargetName  = p_Quest->GetQuestTurnTargetName();

    bool l_AutoLaunched = p_EnableNext;

    int l_Locale = _session->GetSessionDbLocaleIndex();
    if (l_Locale >= 0)
    {
        if (QuestLocale const* l_LocaleData = sObjectMgr->GetQuestLocale(p_Quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(l_LocaleData->Title, l_Locale, l_QuestTitle);
            ObjectMgr::GetLocaleString(l_LocaleData->OfferRewardText, l_Locale, l_QuestOfferRewardText);
            ObjectMgr::GetLocaleString(l_LocaleData->QuestGiverTextWindow, l_Locale, l_QuestGiverTextWindow);
            ObjectMgr::GetLocaleString(l_LocaleData->QuestGiverTargetName, l_Locale, l_QuestGiverTargetName);
            ObjectMgr::GetLocaleString(l_LocaleData->QuestTurnTextWindow, l_Locale, l_QuestTurnTextWindow);
            ObjectMgr::GetLocaleString(l_LocaleData->QuestTurnTargetName, l_Locale, l_QuestTurnTargetName);
        }
    }

    uint32 l_RewardChoiceItemId[QUEST_REWARD_CHOICES_COUNT] ;
    uint32 l_RewardChoiceItemCount[QUEST_REWARD_CHOICES_COUNT];
    for (int l_I = 0; l_I < QUEST_REWARD_CHOICES_COUNT; l_I++)
    {
        l_RewardChoiceItemId[l_I] = 0;
        l_RewardChoiceItemCount[l_I] = 0;
    }

    for (int l_I = 0; l_I < QUEST_REWARD_CHOICES_COUNT; l_I++)
    {
        l_RewardChoiceItemId[l_I]    = p_Quest->RewardChoiceItemId[l_I];
        l_RewardChoiceItemCount[l_I] = p_Quest->RewardChoiceItemCount[l_I];
    }

    float l_QuestXpRate = 1;
    if (_session->GetPlayer()->GetPersonnalXpRate())
        l_QuestXpRate = _session->GetPlayer()->GetPersonnalXpRate();
    else
        l_QuestXpRate = sWorld->getRate(RATE_XP_QUEST);

    WorldPacket l_Data(SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE, 2 * 1024);

    l_Data.appendPackGUID(p_NpcGUID);
    l_Data << uint32(GUID_ENPART(p_NpcGUID));
    l_Data << uint32(p_Quest->GetQuestId());
    l_Data << uint32(p_Quest->GetFlags());
    l_Data << uint32(p_Quest->GetFlags2());
    l_Data << uint32(p_Quest->GetSuggestedPlayers());

    l_Data << uint32(p_Quest->GetRewChoiceItemsCount());

    l_Data << uint32(l_RewardChoiceItemId[0]);
    l_Data << uint32(l_RewardChoiceItemCount[0]);
    l_Data << uint32(l_RewardChoiceItemId[1]);
    l_Data << uint32(l_RewardChoiceItemCount[1]);
    l_Data << uint32(l_RewardChoiceItemId[2]);
    l_Data << uint32(l_RewardChoiceItemCount[2]);
    l_Data << uint32(l_RewardChoiceItemId[3]);
    l_Data << uint32(l_RewardChoiceItemCount[3]);
    l_Data << uint32(l_RewardChoiceItemId[4]);
    l_Data << uint32(l_RewardChoiceItemCount[4]);
    l_Data << uint32(l_RewardChoiceItemId[5]);
    l_Data << uint32(l_RewardChoiceItemCount[5]);

    l_Data << uint32(p_Quest->GetRewItemsCount());

    l_Data << uint32(p_Quest->RewardItemId[0]);
    l_Data << uint32(p_Quest->RewardItemIdCount[0]);
    l_Data << uint32(p_Quest->RewardItemId[1]);
    l_Data << uint32(p_Quest->RewardItemIdCount[1]);
    l_Data << uint32(p_Quest->RewardItemId[2]);
    l_Data << uint32(p_Quest->RewardItemIdCount[2]);
    l_Data << uint32(p_Quest->RewardItemId[3]);
    l_Data << uint32(p_Quest->RewardItemIdCount[3]);

    l_Data << uint32(p_Quest->GetRewMoney());
    l_Data << uint32(p_Quest->XPValue(_session->GetPlayer()) * l_QuestXpRate);
    l_Data << uint32(p_Quest->GetCharTitleId());
    l_Data << uint32(p_Quest->GetBonusTalents());
    l_Data << uint32(p_Quest->GetRewardReputationMask());

    for (uint32 l_I = 0; l_I < QUEST_REPUTATIONS_COUNT; ++l_I)
    {
        l_Data << uint32(p_Quest->RewardFactionId[l_I]);
        l_Data << uint32(p_Quest->RewardFactionValueId[l_I]);
        l_Data << uint32(p_Quest->RewardFactionValueIdOverride[l_I]);
    }

    l_Data << uint32(p_Quest->GetRewSpell());                                                   ///< Spell Completion Display ID
    l_Data << uint32(p_Quest->GetRewSpellCast());                                               ///< Spell Completion ID

    for (uint32 l_I = 0; l_I < QUEST_REWARD_CURRENCY_COUNT; ++l_I)
    {
        l_Data << uint32(p_Quest->RewardCurrencyId[l_I]);
        l_Data << uint32(p_Quest->RewardCurrencyCount[l_I]);
    }

    l_Data << uint32(p_Quest->GetRewardSkillId());
    l_Data << uint32(p_Quest->GetRewardSkillPoints());

    l_Data.WriteBit(l_AutoLaunched);
    l_Data.FlushBits();

    l_Data << uint32(QUEST_EMOTE_COUNT);

    for (uint8 l_I = 0; l_I < QUEST_EMOTE_COUNT; ++l_I)
    {
        l_Data << uint32(p_Quest->DetailsEmote[l_I]);
        l_Data << uint32(p_Quest->DetailsEmoteDelay[l_I]);
    }

    l_Data.WriteBit(p_Quest->GetFlags() & QUEST_FLAGS_AUTO_ACCEPT);

    l_Data << uint32(p_Quest->GetQuestPackageID());
    l_Data << uint32(p_Quest->GetQuestTurnInPortrait());
    l_Data << uint32(p_Quest->GetQuestGiverPortrait());

    l_Data.WriteBits(l_QuestTitle.size(), 9);
    l_Data.WriteBits(l_QuestOfferRewardText.size(), 12);
    l_Data.WriteBits(l_QuestGiverTextWindow.size(), 10);
    l_Data.WriteBits(l_QuestGiverTargetName.size(), 8);
    l_Data.WriteBits(l_QuestTurnTextWindow.size(), 10);
    l_Data.WriteBits(l_QuestTurnTargetName.size(), 8);
    l_Data.FlushBits();

    l_Data.WriteString(l_QuestTitle);
    l_Data.WriteString(l_QuestOfferRewardText);
    l_Data.WriteString(l_QuestGiverTextWindow);
    l_Data.WriteString(l_QuestGiverTargetName);
    l_Data.WriteString(l_QuestTurnTextWindow);
    l_Data.WriteString(l_QuestTurnTargetName);

    _session->SendPacket(&l_Data);
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const* quest, uint64 npcGUID, bool canComplete, bool /*closeOnCancel*/) const
{
    // We can always call to RequestItems, but this packet only goes out if there are actually
    // items.  Otherwise, we'll skip straight to the OfferReward

    std::string questTitle = quest->GetTitle();
    std::string requestItemsText = quest->GetRequestItemsText();

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->RequestItemsText, locale, requestItemsText);
        }
    }

    uint32 itemCounter = quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM);
    if (itemCounter && canComplete)
    {
        SendQuestGiverOfferReward(quest, npcGUID);
        return;
    }

    ByteBuffer l_RequiredItemData;
    ByteBuffer l_RequiredCurrencyData;
    uint32 l_ItemCount = 0;
    uint32 l_CurrencyCount = 0;
    uint32 l_RequiredMoney = 0;

    for (auto l_Objective : quest->QuestObjectives)
    {
        if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_ITEM)
        {
            l_RequiredItemData << uint32(l_Objective.ObjectID);
            l_RequiredItemData << uint32(l_Objective.Amount);

            l_ItemCount++;
        }
        else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_CURRENCY)
        {
            l_RequiredCurrencyData << uint32(l_Objective.ObjectID);
            l_RequiredCurrencyData << uint32(l_Objective.Amount);

            l_CurrencyCount++;
        }
        else if (l_Objective.Type == QUEST_OBJECTIVE_TYPE_MONEY)
            l_RequiredMoney = l_Objective.Amount;
    }

    uint32 l_StatusFlag = canComplete ? 0xDE : 0;

    ObjectGuid guid = npcGUID;

    WorldPacket data(SMSG_QUEST_GIVER_REQUEST_ITEMS, 1024);
    data.appendPackGUID(guid);
    data << uint32(GUID_ENPART(guid));
    data << uint32(quest->GetQuestId());
    data << uint32(0);                                                                      ///< Emote delay
    data << uint32(canComplete ? quest->GetCompleteEmote() : quest->GetIncompleteEmote());
    data << uint32(quest->GetFlags());
    data << uint32(quest->GetFlags2());
    data << uint32(quest->GetSuggestedPlayers());
    data << uint32(l_RequiredMoney);
    data << uint32(l_ItemCount);
    data << uint32(l_CurrencyCount);
    data << uint32(l_StatusFlag);

    data.append(l_RequiredItemData);
    data.append(l_RequiredCurrencyData);

    data.WriteBit(quest->GetFlags() & QUEST_FLAGS_AUTO_ACCEPT);
    data.FlushBits();

    data.WriteBits(questTitle.length(), 9);
    data.WriteBits(requestItemsText.length(), 12);
    data.FlushBits();

    data.WriteString(questTitle);
    data.WriteString(requestItemsText);

    _session->SendPacket(&data);
}
