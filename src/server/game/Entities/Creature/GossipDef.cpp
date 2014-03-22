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
            if (GossipMenuItemsLocale const* no = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, menuItemId)))
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
    ObjectGuid guid = objectGUID;

    WorldPacket data(SMSG_GOSSIP_MESSAGE, 100);         // guess size

    if (titleTextId == DEFAULT_GOSSIP_MESSAGE && !_gossipMenu.GetMenuId())
        data << uint32(DEFAULT_GREETINGS_GOSSIP);           // default greeting ID
    else
        data << uint32(_gossipMenu.GetMenuId());            // new 2.4.0

    data << uint32(0);                                  // Friendship faction
    data << uint32(titleTextId);

    data.WriteBit(guid[0]);
    data.WriteBit(guid[1]);
    data.WriteBits(_gossipMenu.GetMenuItems().size(), 20);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);

    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        GossipMenuItem const& item = itr->second;
        data.WriteBits(item.Message.size(), 12);
        data.WriteBits(item.BoxMessage.size(), 12);
    }

    data.WriteBit(guid[4]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[2]);
    data.WriteBits(_questMenu.GetMenuItemCount(), 19);

    for (uint32 iI = 0; iI < _questMenu.GetMenuItemCount(); ++iI)
    {
        QuestMenuItem const& item = _questMenu.GetItem(iI);
        uint32 questID = item.QuestId;
        Quest const* quest = sObjectMgr->GetQuestTemplate(questID);
        std::string title = quest->GetTitle();

        int locale = _session->GetSessionDbLocaleIndex();
        if (locale >= 0)
            if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                ObjectMgr::GetLocaleString(localeData->Title, locale, title);

        data.WriteBits(title.size(), 9);
        data.WriteBit(quest->IsRepeatable());                   // 3.3.3 changes icon: blue question or yellow exclamation, is repeatable
    }

    data.WriteBit(guid[5]);

    for (uint32 iI = 0; iI < _questMenu.GetMenuItemCount(); ++iI)
    {
        QuestMenuItem const& item = _questMenu.GetItem(iI);
        uint32 questID = item.QuestId;
        Quest const* quest = sObjectMgr->GetQuestTemplate(questID);
        std::string title = quest->GetTitle();

        int locale = _session->GetSessionDbLocaleIndex();
        if (locale >= 0)
            if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                ObjectMgr::GetLocaleString(localeData->Title, locale, title);

        Player* plr = _session->GetPlayer();

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

        data << uint32(0);                              // quest flags 2
        data << uint32(questStat);                      // quest icon
        if (title.size() > 0)
            data.append(title.c_str(), title.size());       // quest title
        data << uint32(quest->GetFlags());              // quest flags
        data << int32(quest->GetQuestLevel());          // quest level
        data << uint32(questID);
    }

    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        GossipMenuItem const& item = itr->second;
        data << uint32(item.BoxMoney);                                  // money required to open menu, 2.0.3
        data << uint32(itr->first);
        if (item.BoxMessage.size() > 0)
            data.append(item.BoxMessage.c_str(), item.BoxMessage.size());
        data << uint8(item.IsCoded);                                    // makes pop up box password
        if (item.Message.size() > 0)
            data.append(item.Message.c_str(), item.Message.size());
        data << uint8(item.MenuItemIcon);
    }

    uint8 byteOrder[8] = { 3, 4, 7, 2, 1, 6, 0, 5 };
    data.WriteBytesSeq(guid, byteOrder);

    _session->SendPacket(&data);
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

    WorldPacket data(SMSG_GOSSIP_POI, 4 + 4 + 4 + 4 + 4 + 10);  // guess size
    data << uint32(poi->flags);
    data << float(poi->x);
    data << float(poi->y);
    data << uint32(poi->icon);
    data << uint32(poi->data);
    data << iconText;

    _session->SendPacket(&data);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(16);                                   // can be set for max from most often sizes to speedup push_back and less memory use
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
    ObjectGuid guid = npcGUID;
    WorldPacket data(SMSG_QUESTGIVER_QUEST_LIST, 100);    // guess size
    data.WriteBits(Title.size(), 11);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[7]);
    data.WriteBits(_questMenu.GetMenuItemCount(), 19);

    uint32 count = 0;
    for (; count < _questMenu.GetMenuItemCount(); ++count)
    {
        QuestMenuItem const& qmi = _questMenu.GetItem(count);

        uint32 questID = qmi.QuestId;

        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questID))
        {
            std::string title = quest->GetTitle();

            int loc_idx = _session->GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (QuestLocale const* ql = sObjectMgr->GetQuestLocale(questID))
                    ObjectMgr::GetLocaleString(ql->Title, loc_idx, title);

            uint8 wrongLen = title.size() % 2;
            data.WriteBits((title.size() - wrongLen) / 2, 8);
            data.WriteBit(wrongLen != 0);
            data.WriteBit(quest->IsRepeatable());
        }
    }

    data.WriteBit(guid[5]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[4]);

    count = 0;
    for (; count < _questMenu.GetMenuItemCount(); ++count)
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

            data << uint32(questStat);                      // quest icon
            data << uint32(quest->GetFlags());              // quest flags
            data << uint32(questID);
            data << uint32(0);                              // quest flags 2
            data << int32(quest->GetQuestLevel());          // quest level
            if (title.size())
                data.append(title.c_str(), title.size());       // quest title
        }
    }

    data << uint32(eEmote._Delay);                         // player emote
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);

    if (Title.size())
        data.append(Title.c_str(), Title.size());

    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[4]);
    data << uint32(eEmote._Emote);                         // NPC emote
    data.WriteByteSeq(guid[0]);

    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST NPC Guid=%u", GUID_LOPART(npcGUID));
}

void PlayerMenu::SendQuestGiverStatus(uint32 questStatus, uint64 npcGUID) const
{
    ObjectGuid guid = npcGUID;
    WorldPacket data(SMSG_QUESTGIVER_STATUS);
    uint8 bitOrder[8] = {6, 5, 0, 1, 2, 4, 3, 7};
    data.WriteBitInOrder(guid, bitOrder);
    data.WriteByteSeq(guid[3]);
    data << uint32(questStatus);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[6]);

    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_STATUS NPC Guid=%u, status=%u", GUID_LOPART(npcGUID), questStatus);
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const* quest, uint64 npcGUID, bool activateAccept) const
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
    ObjectGuid guid = npcGUID;
    ObjectGuid guid2 = npcGUID;
    bool hiddenRewardItem = quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS);

    WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS);

    data << uint32(quest->GetRewardReputationMask());
    data << uint32(quest->GetRewardSkillPoints());

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
    {
        data << uint32(quest->RewardFactionId[i]);
        data << uint32(quest->RewardFactionValueIdOverride[i]);
        data << uint32(quest->RewardFactionValueId[i]);        
    }

    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[3]);
    data << uint32(0); // Unk 0
    data << uint32(hiddenRewardItem ? 0 : quest->GetRewChoiceItemsCount());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[3]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[2]);

    float QuestXpRate = 1;
    if (_session->GetPlayer()->GetPersonnalXpRate())
        QuestXpRate = _session->GetPlayer()->GetPersonnalXpRate();
    else
        QuestXpRate = sWorld->getRate(RATE_XP_QUEST);

    data << uint32(quest->XPValue(_session->GetPlayer()) * QuestXpRate);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[3]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[1]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[4]);
    data << uint32(quest->GetQuestId());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[3]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[2]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[1]);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[1]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(quest->GetQuestTurnInPortrait());
    data << uint32(quest->GetRewSpellCast());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[1]);
    data << uint32(quest->GetQuestGiverPortrait());

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[2]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(hiddenRewardItem ? 0 : quest->GetRewItemsCount());

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[1]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(quest->GetRewMoneyMaxLevel());
    data << uint32(quest->GetFlags());
    data << uint32(quest->GetRewSpell());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[0]);

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyId[i]);
        data << uint32(quest->RewardCurrencyCount[i]);
    }

    data << uint32(quest->GetRewardSkillId());
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[5]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemId[1]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[0]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[4]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[2]);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[0]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[2]);
    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemId[3]);
    data << uint32(0); //unk 0
    data << uint32(hiddenRewardItem ? 0 : quest->RewardChoiceItemCount[5]);
    data << uint32(0); //unk 0

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[0]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[4]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(hiddenRewardItem ? 0 : quest->RewardItemIdCount[0]);
    data << uint32(quest->GetBonusTalents());

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[2]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[3]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(quest->GetSuggestedPlayers());

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardChoiceItemId[5]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(quest->RewardItemId[0]);

    data.WriteBit(guid[4]);
    data.WriteBit(guid2[0]);
    data.WriteBit(guid2[3]);

    data.WriteBits(questGiverTextWindow.size(), 10);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[1]);
    data.WriteBits(questTurnTextWindow.size(), 10);
    data.WriteBits(questDetails.size(), 12);
    data.WriteBit(guid[5]);
    data.WriteBit(0);                               // IsFinished? value is sent back to server in quest accept packet
    data.WriteBit(guid[0]);
    data.WriteBits(QUEST_EMOTE_COUNT, 21);

    data.WriteBits(0, 22);                          // unk counter3 22bits

    data.WriteBits(questObjectives.size(), 12);

    data.WriteBits((quest->GetReqItemsCount() + quest->GetReqCreatureOrGOcount() + quest->GetReqCurrencyCount()), 20);
    data.WriteBits(0, 8);                           // unk counter 8bits

    data.WriteBit(guid2[5]);
    data.WriteBit(guid2[4]);
    data.WriteBit(guid2[2]);
    data.WriteBit(guid2[7]);
    data.WriteBits(questGiverTargetName.size(), 8);
    
    /*
            var unkStringLenght = packet.ReadBits("unk 8 bits", 8);
            unkStringLenght *= 2;
            unkStringLenght = unkStringLenght | packet.ReadBit("bit4093");

    */

    uint8 wrongLen = questTitle.size() % 2;
    data.WriteBits((questTitle.size() - wrongLen) / 2, 8);
    data.WriteBit(wrongLen != 0);

    data.WriteBit(guid2[6]);
    data.WriteBit(1);                                       // ActiveAccept//AutoFinish
    data.WriteBit(guid[3]);
    data.WriteBit(guid2[1]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[6]);
    data.WriteBit(0);                                       // 4.x FIXME: Starts at AreaTrigger

    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        data << uint32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }

    data.WriteByteSeq(guid2[4]);
    data.WriteByteSeq(guid2[5]);

    if (questTurnTargetName.size())
        data.append(questTurnTargetName.c_str(), questTurnTargetName.size());

    if (questGiverTextWindow.size())
        data.append(questGiverTextWindow.c_str(), questGiverTextWindow.size());

    data.WriteByteSeq(guid2[1]);
    data.WriteByteSeq(guid[0]);

    for (uint32 i = 0; i < quest->GetReqItemsCount(); i++) // requiredItem
    {
        data.WriteBits(1, 8);                                   // ("type (1 = item), (2 = gob), (3 = creature), (4 = currency)", i);
        data << uint32(quest->RequiredItemCount[i]);
        data << uint32(0);                                      // unk32
        data << uint32(quest->RequiredItemId[i]);             
    }

    for (uint32 i = 0; i < quest->GetReqCreatureOrGOcount(); i++) 
    {
        if (quest->RequiredNpcOrGo[i] > 0)
            data.WriteBits(3, 8);                                   // ("type (1 = item), (2 = gob), (3 = creature), (4 = currency)", i);
        else
            data.WriteBits(2, 8);

        data << uint32(quest->RequiredNpcOrGoCount[i]);
        data << uint32(0);                                      // unk32
        data << uint32(quest->RequiredNpcOrGo[i]);             
    }

    for (uint32 i = 0; i < quest->GetReqCurrencyCount(); i++)  
    {
        data.WriteBits(4, 8);                                   // ("type (1 = item), (2 = gob), (3 = creature), (4 = currency)", i);
        data << uint32(quest->RequiredCurrencyCount[i]);
        data << uint32(0);                                      // unk32
        data << uint32(quest->RequiredCurrencyId[i]);             
    }

    data.WriteByteSeq(guid2[0]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[3]);

    if (questTurnTargetName.size())
        data.append(questTurnTargetName.c_str(), questTurnTargetName.size());

    if (questGiverTargetName.size())
        data.append(questGiverTargetName.c_str(), questGiverTargetName.size());

    data.WriteByteSeq(guid2[2]);

    /* for (int i = 0; i < count3; i++)                     // unk counter3 22bits
                packet.ReadUInt32("unk uint32", i);
    */
    
    data.WriteByteSeq(guid2[6]);

    if (questTitle.size())
        data.append(questTitle.c_str(), questTitle.size());

    data.WriteByteSeq(guid[7]);
    
    if (questObjectives.size())
        data.append(questObjectives.c_str(), questObjectives.size());

    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid2[3]);
    data.WriteByteSeq(guid[6]);

    if (questDetails.size())
        data.append(questDetails.c_str(), questDetails.size());

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid2[7]);
    data.WriteByteSeq(guid[1]);

    _session->SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS NPCGuid=%u, questid=%u", GUID_LOPART(npcGUID), quest->GetQuestId());
}

void PlayerMenu::SendQuestQueryResponse(Quest const* quest) const
{
    std::string questTitle = quest->GetTitle();
    std::string questDetails = quest->GetDetails();
    std::string questObjectives = quest->GetObjectives();
    std::string questEndText = quest->GetEndText();
    std::string questCompletedText = quest->GetCompletedText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName = quest->GetQuestTurnTargetName();

    std::string questObjectiveText[QUEST_OBJECTIVES_COUNT];
    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        questObjectiveText[i] = quest->ObjectiveText[i];

    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->Details, locale, questDetails);
            ObjectMgr::GetLocaleString(localeData->Objectives, locale, questObjectives);
            ObjectMgr::GetLocaleString(localeData->EndText, locale, questEndText);
            ObjectMgr::GetLocaleString(localeData->CompletedText, locale, questCompletedText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, questTurnTargetName);

            for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
                ObjectMgr::GetLocaleString(localeData->ObjectiveText[i], locale, questObjectiveText[i]);
        }
    }

    uint8 count = 0;
    bool hiddenItemReward = quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_DYNAMIC_ITEM_REWARD) || quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS);

    WorldPacket data(SMSG_QUEST_QUERY_RESPONSE, 100);       // guess size
    data.WriteBit(1);                                       // has data
    data.WriteBits(questDetails.size(), 12);

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        if (quest->RequiredNpcOrGo[i]!= 0)
            ++count;

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        if (quest->RequiredItemId[i] != 0)
            ++count;

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        if (quest->RequiredCurrencyId[i] != 0)
            ++count;

    if (quest->GetRepObjectiveFaction() != 0)
        ++count;

    if (quest->GetRepObjectiveFaction2() != 0)
        ++count;

    data.WriteBits(count, 19);

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredNpcOrGo[i]!= 0)
        {
            data.WriteBits(questObjectiveText[i].size(), 8);
            data.WriteBits(0, 22);
        }
    }
    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredItemId[i] != 0)
        {
            data.WriteBits(questObjectiveText[i].size(), 8);
            data.WriteBits(0, 22);
        }
    }
    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredCurrencyId[i] != 0)
        {
            data.WriteBits(questObjectiveText[i].size(), 8);
            data.WriteBits(0, 22);
        }
    }
    if (quest->GetRepObjectiveFaction() != 0)
    {
        data.WriteBits(0, 8);
        data.WriteBits(0, 22);
    }
    if (quest->GetRepObjectiveFaction2() != 0)
    {
        data.WriteBits(0, 8);
        data.WriteBits(0, 22);
    }

    data.WriteBits(questCompletedText.size(), 11);
    data.WriteBits(questGiverTargetName.size(), 8);
    data.WriteBits(questTurnTextWindow.size(), 10);
    data.WriteBits(questObjectives.size(), 12);

    uint8 wrongLen = questEndText.size() % 2;
    data.WriteBits(( questEndText.size() - wrongLen) / 2, 8);
    data.WriteBit(wrongLen != 0);

    data.WriteBits(questTurnTargetName.size(), 8);
    data.WriteBits(questGiverTextWindow.size(), 10);
    
    wrongLen = questTitle.size() % 2;
    data.WriteBits(( questTitle.size() - wrongLen) / 2, 8);
    data.WriteBit(wrongLen != 0);

    data << uint32(hiddenItemReward ? 0 : quest->RewardItemIdCount[2]);
    data << int32(quest->GetRewSpellCast());
    data << float(quest->GetRewHonorMultiplier());

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredNpcOrGo[i]!= 0)
        {
            data << uint32(0); //unk
            if (quest->RequiredNpcOrGo[i] < 0)
            {           
                data << uint32((quest->RequiredNpcOrGo[i] * (-1)));
                data << uint32(0);
                data << uint32(quest->RequiredNpcOrGoCount[i]);
                if (questObjectiveText[i].size())
                    data.append(questObjectiveText[i].c_str(), questObjectiveText[i].size());
                data << uint8(i); // objective index
                data << uint8(2);
            }
            else
            {
                data << uint32(quest->RequiredNpcOrGo[i]);
                data << uint32(0);
                data << uint32(quest->RequiredNpcOrGoCount[i]);
                if (questObjectiveText[i].size())
                    data.append(questObjectiveText[i].c_str(), questObjectiveText[i].size());
                data << uint8(i); // objective index
                data << uint8(0);
            }
        }
    }

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredItemId[i] != 0)
        {
            data << uint32(0); //unk
            data << uint32(quest->RequiredItemId[i]);
            data << uint32(0);
            data << uint32(quest->RequiredItemCount[i]);
            if (questObjectiveText[i].size())
                data.append(questObjectiveText[i].c_str(), questObjectiveText[i].size());
            data << uint8(0);
            data << uint8(1);
        }
    }

    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (quest->RequiredCurrencyId[i] != 0)
        {
            data << uint32(0); //unk   
            data << uint32(quest->RequiredCurrencyId[i]);
            data << uint32(0);
            data << uint32(quest->RequiredCurrencyCount[i]);       
            if (questObjectiveText[i].size())
                data.append(questObjectiveText[i].c_str(), questObjectiveText[i].size());
            data << uint8(255);
            data << uint8(4);
        }
    }

    if (quest->GetRepObjectiveFaction() != 0)
    {
        data << uint32(0); //unk
        data << uint32(quest->GetRepObjectiveFaction());
        data << uint32(0);
        data << uint32(quest->GetRepObjectiveValue());
        data << uint8(255);
        data << uint8(6);
    }

    if (quest->GetRepObjectiveFaction2() != 0)
    {
        data << uint32(0); //unk
        data << uint32(quest->GetRepObjectiveFaction2());
        data << uint32(0);
        data << uint32(quest->GetRepObjectiveValue2());
        data << uint8(255);
        data << uint8(6);
    }

    data << uint32(quest->GetSrcItemId());
    data << uint32(0); // 2965, itemSourceReq related
    data << float(quest->GetPointY());
    if (questTurnTargetName.size())
        data.append(questTurnTargetName.c_str(), questTurnTargetName.size());
    data << uint32(quest->GetNextQuestInChain());
    if (questEndText.size())
        data.append(questEndText.c_str(), questEndText.size());
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemId[2]);

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyCount[i]);
        data << uint32(quest->RewardCurrencyId[i]);
    }

    data << uint32(quest->GetMinLevel());
    data << float(quest->GetPointX());
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemId[3]);

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i) 
    {
        data << uint32(quest->RewardFactionValueId[i]);
        data << uint32(quest->RewardFactionValueIdOverride[i]);
        data << uint32(quest->RewardFactionId[i]);
    }

    data << uint32(quest->GetRewArenaPoints());
    data << uint32(0);                                      // RewardChoiceItemUnk[2]
    data << uint32(quest->GetQuestGiverPortrait());
    data << uint32(quest->GetQuestTurnInPortrait());
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemCount[4]);
    data << uint32(0);                                      // 2962
    if (questTitle.size())
        data.append(questTitle.c_str(), questTitle.size());
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemIdCount[0]);
    data << uint32(0);                                      // 2964
    data << uint32(0);                                      // RewardChoiceItemUnk[0]
    data << uint32(quest->GetRewardSkillId());
    data << uint32(quest->GetMinimapTargetMark());
    data << uint32(0);                                      // RewardChoiceItemUnk[3]
    data << uint32(0);                                      // RewardChoiceItemUnk[4]
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemCount[1]);
    data << uint32(quest->GetQuestId());
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemId[1]);
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemIdCount[1]);
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemId[2]);
    data << uint32(quest->GetRewardSkillPoints());
    data << uint32(0);                                      // 2963
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemId[1]);
    data << uint32(quest->GetExclusiveGroup());
    data << uint32(quest->GetRewardReputationMask());
    if (questTurnTextWindow.size())
        data.append(questTurnTextWindow.c_str(), questTurnTextWindow.size());
    data << uint32(quest->GetSoundTurnIn());
    data << uint32(quest->GetXPId());
    data << uint32(quest->GetFlags() & 0xFFFF);
    if (questCompletedText.size())
        data.append(questCompletedText.c_str(), questCompletedText.size());
    data << uint32(quest->GetQuestLevel());
    data << uint32(0);                                      // 2966
    data << uint32(quest->GetSuggestedPlayers());
    data << uint32(quest->GetPointMapId());
    data << uint32(quest->GetRewMoneyMaxLevel());
    data << uint32(quest->GetPlayersSlain());
    data << uint32(0);                                      // 2961
    data << uint32(0);                                      // RewardChoiceItemUnk[5]
    data << uint32(quest->GetZoneOrSort());
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemId[0]);
    data << uint32(quest->GetXPId());
    if (questDetails.size())
        data.append(questDetails.c_str(), questDetails.size());
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemCount[0]);
    if (questGiverTargetName.size())
        data.append(questGiverTargetName.c_str(), questGiverTargetName.size());
    data << uint32(0);                                      // RewardChoiceItemUnk[1]
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemId[0]);
    data << uint32(quest->GetQuestMethod());
    data << uint32(quest->GetRewHonorAddition());
    data << uint32(quest->GetBonusTalents());
    if (questGiverTextWindow.size())
        data.append(questGiverTextWindow.c_str(), questGiverTextWindow.size());
    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
        data << uint32(0);                                  // Hide money rewarded
    else
        data << uint32(quest->GetRewOrReqMoney());          // reward money (below max lvl)
    data << uint32(quest->GetType());
    if (questObjectives.size())
        data.append(questObjectives.c_str(), questObjectives.size());
    data << uint32(quest->GetRewSpell());
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemId[3]);
    data << uint32(0);                                  // 2960
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemCount[5]);
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemCount[3]);
    data << uint32(hiddenItemReward ? 0 : quest->RewardItemIdCount[3]);
    data << uint32(quest->GetSoundAccept());
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemId[5]);
    data << uint32(0);                                  // 2959
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemCount[2]);
    data << uint32(quest->GetPointOpt());
    data << uint32(hiddenItemReward ? 0 : quest->RewardChoiceItemId[4]);
    data << uint32(quest->GetQuestId());

    /*
    data << uint32(quest->GetQuestId());                    // quest id
    data << uint32(quest->GetQuestMethod());                // Accepted values: 0, 1 or 2. 0 == IsAutoComplete() (skip objectives/details)
    data << uint32(quest->GetQuestLevel());                 // may be -1, static data, in other cases must be used dynamic level: Player::GetQuestLevel (0 is not known, but assuming this is no longer valid for quest intended for client)
    data << uint32(quest->GetExclusiveGroup());
    data << uint32(quest->GetMinLevel());                   // min level
    data << uint32(quest->GetZoneOrSort());                 // zone or sort to display in quest log

    data << uint32(quest->GetType());                       // quest type

    data << uint32(quest->GetSuggestedPlayers());           // suggested players count
    data << uint32(quest->GetNextQuestInChain());           // client will request this quest from NPC, if not 0
    data << uint32(quest->GetXPId());                       // used for calculating rewarded experience

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
        data << uint32(0);                                  // Hide money rewarded
    else
        data << uint32(quest->GetRewOrReqMoney());          // reward money (below max lvl)

    data << uint32(quest->GetRewMoneyMaxLevel());           // used in XP calculation at client
    data << uint32(quest->GetRewSpell());                   // reward spell, this spell will display (icon) (casted if RewSpellCast == 0)
    data << int32(quest->GetRewSpellCast());                // casted spell

    // rewarded honor points
    data << uint32(quest->GetRewHonorAddition());
    data << float(quest->GetRewHonorMultiplier());

    data << uint32(quest->GetSrcItemId());                  // source item id
    data << uint32(quest->GetFlags() & 0xFFFF);             // quest flags
    data << uint32(quest->GetMinimapTargetMark());          // minimap target mark (skull, etc. missing enum)
    data << uint32(quest->GetPlayersSlain());               // players slain
    data << uint32(quest->GetBonusTalents());               // bonus talents
    data << uint32(quest->GetRewArenaPoints());             // bonus arena points FIXME: arena points were removed, right?
    data << uint32(quest->GetRewardSkillId());              // reward skill id
    data << uint32(quest->GetRewardSkillPoints());          // reward skill points
    data << uint32(quest->GetRewardReputationMask());       // rep mask (unsure on what it does)
    data << uint32(quest->GetQuestGiverPortrait());         // quest giver entry ?
    data << uint32(quest->GetQuestTurnInPortrait());        // quest turnin entry ?

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
    {
        for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
            data << uint32(0) << uint32(0);
        for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
            data << uint32(0) << uint32(0);
    }
    else
    {
        for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        {
            data << uint32(quest->RewardItemId[i]);
            data << uint32(quest->RewardItemIdCount[i]);
        }
        for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        {
            data << uint32(quest->RewardChoiceItemId[i]);
            data << uint32(quest->RewardChoiceItemCount[i]);
        }
    }

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)        // reward factions ids
        data << uint32(quest->RewardFactionId[i]);

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)        // columnid+1 QuestFactionReward.dbc?
        data << int32(quest->RewardFactionValueId[i]);

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)           // unknown usage
        data << int32(quest->RewardFactionValueIdOverride[i]);

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyId[i]);
        data << uint32(quest->RewardCurrencyCount[i]);
    }

    data << quest->GetPointMapId();
    data << quest->GetPointX();
    data << quest->GetPointY();
    data << quest->GetPointOpt();

    data << questTitle;
    data << questObjectives;
    data << questDetails;
    data << questEndText;
    data << questCompletedText;
    data << questGiverTextWindow;
    data << questGiverTargetName;
    data << questTurnTextWindow;
    data << questTurnTargetName;
    data << uint32(quest->GetSoundAccept());
    data << uint32(quest->GetSoundTurnIn());
    for (uint32 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        data << uint32(quest->RequiredSourceItemId[i]);
        data << uint32(quest->RequiredSourceItemCount[i]);
    }*/


    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUEST_QUERY_RESPONSE questid=%u", quest->GetQuestId());
}

// @TODO: Replace unknow uint32 by missing RewardChoiceItemId/RewardChoiceItemCount tab (5 & 6) in packet
void PlayerMenu::SendQuestGiverOfferReward(Quest const* quest, uint64 npcGUID, bool enableNext) const
{
    std::string questTitle = quest->GetTitle();
    std::string questOfferRewardText = quest->GetOfferRewardText();
    std::string questGiverTextWindow = quest->GetQuestGiverTextWindow();
    std::string questGiverTargetName = quest->GetQuestGiverTargetName();
    std::string questTurnTextWindow = quest->GetQuestTurnTextWindow();
    std::string questTurnTargetName = quest->GetQuestTurnTargetName();

    int locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            ObjectMgr::GetLocaleString(localeData->Title, locale, questTitle);
            ObjectMgr::GetLocaleString(localeData->OfferRewardText, locale, questOfferRewardText);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTextWindow, locale, questGiverTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestGiverTargetName, locale, questGiverTargetName);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTextWindow, locale, questTurnTextWindow);
            ObjectMgr::GetLocaleString(localeData->QuestTurnTargetName, locale, questTurnTargetName);
        }
    }

    uint32 RewardChoiceItemId[QUEST_REWARD_CHOICES_COUNT] ;
    uint32 RewardChoiceItemCount[QUEST_REWARD_CHOICES_COUNT];
    if (quest->HasSpecialFlags(QUEST_SPECIAL_FLAGS_DYNAMIC_ITEM_REWARD))
    {
        Player* plr = _session->GetPlayer();
        uint32 index = 0;
        for (auto dynamicReward : quest->DynamicRewards)
        {
            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(dynamicReward.itemID);
            if (!itemTemplate)
                continue;

            // @TODO: Check if we really need to check specialisation id or just player's class
            // (if player doesn't have choosen spec, he doesn't have reward ??)
            //if (itemTemplate->HasSpec() && !itemTemplate->HasSpec(plr->GetSpecializationId(plr->GetActiveSpec())))
            //    continue;

            if (itemTemplate->HasSpec() && !itemTemplate->HasClassSpec(plr->getClass()))
                continue;

            if (index >= QUEST_REWARD_CHOICES_COUNT)
                continue;

            RewardChoiceItemId[index] = dynamicReward.itemID;
            RewardChoiceItemCount[index] = dynamicReward.count;
            index++;
        }
    }
    else
    {
        for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; i++)
        {
            RewardChoiceItemId[i] = quest->RewardChoiceItemId[i];
            RewardChoiceItemCount[i] = quest->RewardChoiceItemCount[i];
        }
    }

    WorldPacket data(SMSG_QUESTGIVER_OFFER_REWARD, 50);     // guess size
    
    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[2]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);
    
    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[0]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);
    
    data << uint32(quest->RewardItemIdCount[0]);

    for (uint32 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
    {
        data << uint32(quest->RewardCurrencyId[i]);
        data << uint32(quest->RewardCurrencyCount[i]);
    }

    data << uint32(RewardChoiceItemCount[0]);

    data << uint32(0); // 78
    data << uint32(0); // 144

    data << uint32(0);                       // unk
    
    data << uint32(0); // 83
    data << uint32(0); // 98
    data << uint32(0); // 99
    
    data << uint32(RewardChoiceItemCount[3]);

    for (uint32 i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
    {
        data << uint32(quest->RewardFactionId[i]);
        data << uint32(quest->RewardFactionValueIdOverride[i]);
        data << uint32(quest->RewardFactionValueId[i]);        
    }
    
    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[3]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[1]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);
    
    data << uint32(quest->RewardItemId[3]);
    
    data << uint32(0); // 75

    float QuestXpRate = 1;
    if (_session->GetPlayer()->GetPersonnalXpRate())
        QuestXpRate = _session->GetPlayer()->GetPersonnalXpRate();
    else
        QuestXpRate = sWorld->getRate(RATE_XP_QUEST);

    data << uint32(quest->XPValue(_session->GetPlayer()) * QuestXpRate);
    
    data << uint32(quest->RewardItemIdCount[2]);
    data << uint32(RewardChoiceItemCount[3]);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[0]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(0); // 101
    data << uint32(0); // 79
    data << uint32(quest->RewardItemIdCount[3]);
    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[1]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);
    
    data << uint32(RewardChoiceItemId[3]);
    data << uint32(0); // 84
    data << uint32(quest->GetFlags());
    data << uint32(0); // 100


    data << uint32(0); // 594
    data << uint32(0); // 70

    data << uint32(RewardChoiceItemCount[1]);

    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(RewardChoiceItemId[3]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);

    data << uint32(0); // 82

    data << uint32(quest->RewardItemId[1]);

    data << uint32(0); // 97
    data << uint32(0); // 145

    data << uint32(quest->RewardItemId[2]);
    if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewardItemId[2]))
        data << uint32(itemTemplate->DisplayInfoID);
    else
        data << uint32(0);
    data << uint32(0);  // 102
    data << uint32(quest->GetRewChoiceItemsCount());
    data << uint32(quest->GetSuggestedPlayers());
    data << uint32(RewardChoiceItemId[2]);
    data << uint32(RewardChoiceItemId[1]);
    data << uint32(quest->GetRewOrReqMoney()); // 76
    data << uint32(RewardChoiceItemId[0]);

    data << uint32(quest->RewardItemId[0]);

    data << uint32(0); // 80

    data << uint32(quest->RewardItemIdCount[1]);

    data << uint32(0); // 81

    data << uint32(quest->GetQuestId()); 


    ObjectGuid guid = npcGUID;

    data.WriteBit(guid[4]);

    uint8 wrongLen = questTitle.size() % 2;
    data.WriteBits(( questTitle.size() - wrongLen) / 2, 8);
    data.WriteBit(wrongLen != 0);

    data.WriteBits(questGiverTextWindow.size(), 10); // unk size 1
    data.WriteBits(0, 10); // unk size 2
    data.WriteBits(QUEST_EMOTE_COUNT, 21); // Close on cancel ??? Look like quest_emote_count ...

    data.WriteBit(0); // unk bit 288

    data.WriteBit(guid[6]);

    data.WriteBits(0, 8); // unk size 3
    data.WriteBits(0, 8); // unk size 4

    data.WriteBit(guid[0]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[7]);

    data.WriteBits(questOfferRewardText.size(), 12); // unk size 5

    data.WriteBit(guid[3]);
    data.FlushBits();

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);

    if (questGiverTextWindow.size())
        data.append(questGiverTextWindow.c_str(), questGiverTextWindow.size());

    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[1]);

    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        data << uint32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }

    if (questTitle.size())
        data.append(questTitle.c_str(), questTitle.size());

    data.WriteByteSeq(guid[6]);

    //if (unknow.size())
    //    data.append(unknow.c_str(), unknow.size());
    //if (unknow.size())
    //    data.append(unknow.c_str(), unknow.size());
    if (questOfferRewardText.size())
        data.append(questOfferRewardText.c_str(), questOfferRewardText.size());

    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[7]);

    //if (unknow.size())
        //data.append(unknow.c_str(), unknow.size());
    /*
    data << uint64(npcGUID);
    data << uint32(quest->GetQuestId());
    data << uint32(0);
    data << questTitle;
    data << questOfferRewardText;

    data << questGiverTextWindow;
    data << questGiverTargetName;
    data << questTurnTextWindow;
    data << questTurnTargetName;
    data << uint32(quest->GetQuestGiverPortrait());
    data << uint32(quest->GetQuestTurnInPortrait());

    data << uint8(enableNext ? 1 : 0);                      // Auto Finish
    data << uint32(quest->GetFlags());                      // 3.3.3 questFlags
    data << uint32(quest->GetSuggestedPlayers());           // SuggestedGroupNum
    data << uint32(0);

    uint32 emoteCount = 0;
    for (uint8 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        if (quest->OfferRewardEmote[i] <= 0)
            break;
        ++emoteCount;
    }

    data << emoteCount;                                     // Emote Count
    for (uint8 i = 0; i < emoteCount; ++i)
    {
        data << uint32(quest->OfferRewardEmoteDelay[i]);    // Delay Emote
        data << uint32(quest->OfferRewardEmote[i]);
    }
    */
    // quest->BuildExtraQuestInfo(data, _session->GetPlayer());

    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD NPCGuid=%u, questid=%u", GUID_LOPART(npcGUID), quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const* quest, uint64 npcGUID, bool canComplete, bool closeOnCancel) const
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

    if (!quest->GetReqItemsCount() && canComplete)
    {
        SendQuestGiverOfferReward(quest, npcGUID, true);
        return;
    }

    ObjectGuid guid = npcGUID;
    WorldPacket data(SMSG_QUESTGIVER_REQUEST_ITEMS, 50);    // guess size
    data << uint32(0);
    data << uint32(0);
    data << uint32(quest->GetFlags());
    data << uint32(quest->GetQuestId());
    data << uint32(GUID_ENPART(npcGUID));
    data << uint32(0);
    data << uint32(0);
    data << uint32(95);
    data << uint32(0);
    data.WriteBit(closeOnCancel);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[0]);
    data.WriteBits(requestItemsText.size(), 12);
    data.WriteBit(guid[4]);

    uint32 currencyCount = 0;
    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    {
        if (!quest->RequiredCurrencyId[i])
            continue;

        currencyCount++;
    }
    
    data.WriteBits(currencyCount, 21);

    uint32 itemCount = 0;
    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->RequiredItemId[i])
            continue;

        itemCount++;
    }
    data.WriteBits(itemCount, 20);

    data.WriteBit(guid[3]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);

    uint8 wrongLen = questTitle.size() % 2;
    data.WriteBits(( questTitle.size() - wrongLen) / 2, 8);
    data.WriteBit(wrongLen != 0);

    data.WriteByteSeq(guid[6]);

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->RequiredItemId[i])
            continue;

        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RequiredItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
        
        data << uint32(quest->RequiredItemCount[i]);
        data << uint32(quest->RequiredItemId[i]);
    }

    if (questTitle.size())
        data.append(questTitle.c_str(), questTitle.size());

    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    {
        if (!quest->RequiredCurrencyId[i])
            continue;

        data << uint32(quest->RequiredCurrencyId[i]);
        data << uint32(quest->RequiredCurrencyCount[i]);
    }

    data.WriteByteSeq(guid[4]);
    if (requestItemsText.size())
        data.append(requestItemsText.c_str(), requestItemsText.size());
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[3]);
    /*

    data << uint64(npcGUID);
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << requestItemsText;



    data << uint32(0);                                   // unknown

    if (canComplete)
        data << quest->GetCompleteEmote();
    else
        data << quest->GetIncompleteEmote();

    // Close Window after cancel
    data << uint32(closeOnCancel);

    data << uint32(quest->GetFlags());                      // 3.3.3 questFlags
    data << uint32(quest->GetSuggestedPlayers());           // SuggestedGroupNum

    // Required Money
    data << uint32(quest->GetRewOrReqMoney() < 0 ? -quest->GetRewOrReqMoney() : 0);
    data << uint32(0);
    data << uint32(quest->GetReqItemsCount());
    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->RequiredItemId[i])
            continue;

        data << uint32(quest->RequiredItemId[i]);
        data << uint32(quest->RequiredItemCount[i]);

        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RequiredItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << uint32(quest->GetReqCurrencyCount());
    for (int i = 0; i < QUEST_REQUIRED_CURRENCY_COUNT; ++i)
    {
        if (!quest->RequiredCurrencyId[i])
            continue;

        data << uint32(quest->RequiredCurrencyId[i]);
        data << uint32(quest->RequiredCurrencyCount[i]);
    }

    if (!canComplete)            // Experimental; there are 6 similar flags, if any of them
        data << uint32(0x00);    // of them is 0 player can't complete quest (still unknown meaning)
    else
        data << uint32(0x02);

    data << uint32(0x04);
    data << uint32(0x08);
    data << uint32(0x10);
    data << uint32(0x40);*/

    _session->SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS NPCGuid=%u, questid=%u", GUID_LOPART(npcGUID), quest->GetQuestId());
}
