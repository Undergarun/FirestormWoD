////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "DatabaseEnv.h"
#include "Mail.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Language.h"
#include "DBCStores.h"
#include "Item.h"
#include "AccountMgr.h"

void WorldSession::HandleSendMail(WorldPacket& p_Packet)
{
    uint64 l_Mailbox = 0;
    uint64 l_SendMoney, l_COD;
    std::string l_Target, l_Subject, l_Body;
    uint32 l_BodyLength, l_SubjectLength, l_TargetLenght;
    uint32 l_StationeryID;
    uint8 l_AttachmentsCount;

    uint64 itemGUIDs[MAX_MAIL_ITEMS];

    uint32 l_OldMSTime = getMSTime();

    p_Packet.readPackGUID(l_Mailbox);

    p_Packet >> l_StationeryID;
    p_Packet >> l_SendMoney;
    p_Packet >> l_COD;

    l_TargetLenght      = p_Packet.ReadBits(9);
    l_SubjectLength     = p_Packet.ReadBits(9);
    l_BodyLength        = p_Packet.ReadBits(11);
    l_AttachmentsCount  = p_Packet.ReadBits(5);

    if (l_AttachmentsCount > MAX_MAIL_ITEMS)                        // client limit
    {
        GetPlayer()->SendMailResult(0, MAIL_SEND, MAIL_ERR_TOO_MANY_ATTACHMENTS);
        p_Packet.rfinish();                                         // set to end to avoid warnings spam
        return;
    }

    l_Target    = p_Packet.ReadString(l_TargetLenght);
    l_Subject   = p_Packet.ReadString(l_SubjectLength);
    l_Body      = p_Packet.ReadString(l_BodyLength);

    for (uint8 l_I = 0; l_I < l_AttachmentsCount; ++l_I)
    {
        p_Packet.read_skip<uint8>();           // item slot in mail, not used
        p_Packet.readPackGUID(itemGUIDs[l_I]);
    }

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    if (l_Target.empty())
        return;

    if (m_Player->getLevel() < sWorld->getIntConfig(CONFIG_MAIL_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_MAIL_SENDER_REQ), sWorld->getIntConfig(CONFIG_MAIL_LEVEL_REQ));
        return;
    }

    uint64 rc = 0;
    if (normalizePlayerName(l_Target))
        rc = sWorld->GetCharacterGuidByName(l_Target);

    if (!rc)
    {
        m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_RECIPIENT_NOT_FOUND);
        return;
    }

    if (m_Player->GetGUID() == rc)
    {
        m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_CANNOT_SEND_TO_SELF);
        return;
    }

    uint32 cost = l_AttachmentsCount ? 30 * l_AttachmentsCount : 30;  // price hardcoded in client

    uint64 reqmoney = cost + l_SendMoney;
    if (!m_Player->HasEnoughMoney(reqmoney) && !m_Player->isGameMaster())
    {
        m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_NOT_ENOUGH_MONEY);
        return;
    }

    Player* receive = ObjectAccessor::FindPlayer(rc);

    uint32 rc_team = 0;
    uint8 mails_count = 0;                                  //do not allow to send to one player more than 100 mails

    if (receive)
    {
        rc_team = receive->GetTeam(); ///< rc_team is never read 01/18/16
        mails_count = receive->GetMailSize();
    }

    //do not allow to have more than 100 mails in mailbox.. mails count is in opcode uint8!!! - so max can be 255..
    if (mails_count > 100)
    {
        m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_RECIPIENT_CAP_REACHED);
        return;
    }

    // test the receiver's Faction... or all items are account bound
    bool accountBound = l_AttachmentsCount ? true : false; ///< accoubntbound is never read 01/18/16
    for (uint8 l_I = 0; l_I < l_AttachmentsCount; ++l_I)
    {
        Item* item = m_Player->GetItemByGuid(itemGUIDs[l_I]);
        if (item)
        {
            ItemTemplate const* itemProto = item->GetTemplate();
            if (!itemProto || !(itemProto->Flags & ITEM_FLAG_BIND_TO_ACCOUNT))
            {
                accountBound = false; ///< accountbound is never read 01/18/16
                break;
            }
        }
    }

    uint32 rc_account = receive
        ? receive->GetSession()->GetAccountId()
        : sObjectMgr->GetPlayerAccountIdByGUID(rc);

    Item* items[MAX_MAIL_ITEMS];

    for (uint8 l_I = 0; l_I < l_AttachmentsCount; ++l_I)
    {
        if (!itemGUIDs[l_I])
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return;
        }

        Item* item = m_Player->GetItemByGuid(itemGUIDs[l_I]);

        // prevent sending bag with items (cheat: can be placed in bag after adding equipped empty bag to mail)
        if (!item)
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
            return;
        }

        if (!item->CanBeTraded(true))
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_MAIL_BOUND_ITEM);
            return;
        }

        if (item->IsBoundAccountWide() && item->IsSoulBound() && m_Player->GetSession()->GetAccountId() != rc_account)
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_NOT_SAME_ACCOUNT);
            return;
        }

        if (item->GetTemplate()->Flags & ITEM_FLAG_CONJURED || item->GetUInt32Value(ITEM_FIELD_EXPIRATION))
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_MAIL_BOUND_ITEM);
            return;
        }

        if (l_COD && item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_WRAPPED))
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_CANT_SEND_WRAPPED_COD);
            return;
        }

        if (item->IsNotEmptyBag())
        {
            m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_EQUIP_ERROR, EQUIP_ERR_DESTROY_NONEMPTY_BAG);
            return;
        }

        items[l_I] = item;
    }

    // Check for spamming
    if (!UpdateAntispamCount())
    {
        m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_INTERNAL_ERROR);
        SendNotification(GetTrinityString(LANG_ANTISPAM_ERROR)); ///> Format string is not a string literal (potentially insecure)
        return;
    }

    // Check for special symbols
    if (!checkMailText(l_Subject) ||  !checkMailText(l_Body))
    {
        m_Player->SendMailResult(0, MAIL_SEND, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    m_Player->SendMailResult(0, MAIL_SEND, MAIL_OK);

    m_Player->ModifyMoney(-int64(reqmoney));
    m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL, cost);

    bool needItemDelay = false;

    MailDraft draft(l_Subject, l_Body);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    if (l_AttachmentsCount > 0 || l_SendMoney > 0)
    {
        if (l_AttachmentsCount > 0)
        {
            for (uint8 i = 0; i < l_AttachmentsCount; ++i)
            {
                Item* item = items[i];
                bool l_MustLog = !AccountMgr::IsPlayerAccount(GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE);
                if (l_MustLog && GetAccountId() != rc_account) ///< It's useless to log self
                {
                    sLog->outCommand(GetAccountId(), "", GetPlayer()->GetGUIDLow(), GetPlayer()->GetName(),
                                    rc_account, "", 0, l_Target.c_str(),
                                    "GM %s (Account: %u) mail item: %s (Entry: %u Count: %u) to player: %s (Account: %u)",
                                    GetPlayerName().c_str(), GetAccountId(), item->GetTemplate()->Name1->Get(sWorld->GetDefaultDbcLocale()), item->GetEntry(), item->GetCount(), l_Target.c_str(), rc_account);
                }

                item->SetNotRefundable(GetPlayer()); // makes the item no longer refundable
                m_Player->MoveItemFromInventory(items[i]->GetBagSlot(), item->GetSlot(), true);

                item->DeleteFromInventoryDB(trans);     // deletes item from character's inventory
                item->SetOwnerGUID(rc);
                item->SaveToDB(trans);                  // recursive and not have transaction guard into self, item not in inventory and can be save standalone

                draft.AddItem(item);
            }

            // if item send to character at another account, then apply item delivery delay
            needItemDelay = GetAccountId() != rc_account;
        }

        bool l_MustLog = !AccountMgr::IsPlayerAccount(GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE);
        if (l_SendMoney > 0 && l_MustLog && GetAccountId() != rc_account) ///< It's useless to log self
        {
            //TODO: character guid
            sLog->outCommand(GetAccountId(), "", GetPlayer()->GetGUIDLow(), GetPlayer()->GetName(),
                            rc_account, "", 0, l_Target.c_str(),
                            "GM %s (Account: %u) mail money: " UI64FMTD " to player: %s (Account: %u)",
                            GetPlayerName().c_str(), GetAccountId(), l_SendMoney, l_Target.c_str(), rc_account);
        }
    }

    // If theres is an item, there is a one hour delivery delay if sent to another account's character.
    uint32 deliver_delay = needItemDelay ? sWorld->getIntConfig(CONFIG_MAIL_DELIVERY_DELAY) : 0;

    // Guild Mail
    if (receive && receive->GetGuildId() && m_Player->GetGuildId())
    {
        if (m_Player->HasAura(83951) && (m_Player->GetGuildId() == receive->GetGuildId()))
            deliver_delay = 0;
    }

    // VIP Accounts receive mails instantly
    if (receive && receive->GetSession()->IsPremium())
        deliver_delay = 0;

    // will delete item or place to receiver mail list
    draft
        .AddMoney(l_SendMoney)
        .AddCOD(l_COD)
        .SendMailTo(trans, MailReceiver(receive, GUID_LOPART(rc)), MailSender(m_Player), l_Body.empty() ? MAIL_CHECK_MASK_COPIED : MAIL_CHECK_MASK_HAS_BODY, deliver_delay);

    m_Player->SaveInventoryAndGoldToDB(trans);
    CharacterDatabase.CommitTransaction(trans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Time diff on mail send %u ms", GetMSTimeDiffToNow(l_OldMSTime));
}

// Called when mail is read
void WorldSession::HandleMailMarkAsRead(WorldPacket& p_Packet)
{
    uint64 l_Mailbox;
    uint32 l_MailID;

    p_Packet.readPackGUID(l_Mailbox);
    p_Packet >> l_MailID;

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Mail * l_Mail = m_Player->GetMail(l_MailID);

    if (l_Mail)
    {
        if (m_Player->unReadMails)
            --m_Player->unReadMails;

        l_Mail->checked = l_Mail->checked | MAIL_CHECK_MASK_READ;
        m_Player->m_mailsUpdated = true;
        l_Mail->state = MAIL_STATE_CHANGED;
    }
}

// Called when client deletes mail
void WorldSession::HandleMailDelete(WorldPacket & p_Packet)
{
    uint32 l_MailID;

    p_Packet >> l_MailID;                       // mailTemplateId
    p_Packet.read_skip<uint32>();

    Mail * l_Mail = m_Player->GetMail(l_MailID);

    m_Player->m_mailsUpdated = true;

    if (l_Mail)
    {
        // delete shouldn't show up for COD mails
        if (l_Mail->COD)
        {
            m_Player->SendMailResult(l_MailID, MAIL_DELETED, MAIL_ERR_INTERNAL_ERROR);
            return;
        }

        l_Mail->state = MAIL_STATE_DELETED;
    }
    m_Player->SendMailResult(l_MailID, MAIL_DELETED, MAIL_OK);
}

void WorldSession::HandleMailReturnToSender(WorldPacket & p_Packet)
{
    uint64 l_Owner  = 0;
    uint32 l_MailID = 0;

    p_Packet >> l_MailID;
    p_Packet.readPackGUID(l_Owner);

    //if (!GetPlayer()->GetGameObjectIfCanInteractWith(mailbox, GAMEOBJECT_TYPE_MAILBOX))
    //    return;

    Mail * l_Mail = m_Player->GetMail(l_MailID);
    if (!l_Mail || l_Mail->state == MAIL_STATE_DELETED || l_Mail->deliver_time > time(NULL))
    {
        m_Player->SendMailResult(l_MailID, MAIL_RETURNED_TO_SENDER, MAIL_ERR_INTERNAL_ERROR);
        return;
    }
    //we can return mail now
    //so firstly delete the old one
    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
    l_Stmt->setUInt32(0, l_MailID);
    l_Transaction->Append(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_ITEM_BY_ID);
    l_Stmt->setUInt32(0, l_MailID);
    l_Transaction->Append(l_Stmt);

    m_Player->RemoveMail(l_MailID);

    // only return mail if the player exists (and delete if not existing)
    if (l_Mail->messageType == MAIL_NORMAL && l_Mail->sender)
    {
        MailDraft l_Draft(l_Mail->subject, l_Mail->body);
        if (l_Mail->mailTemplateId)
            l_Draft = MailDraft(l_Mail->mailTemplateId, false);     // items already included

        if (l_Mail->HasItems())
        {
            for (MailItemInfoVec::iterator itr2 = l_Mail->items.begin(); itr2 != l_Mail->items.end(); ++itr2)
            {
                Item* l_Item = m_Player->GetMItem(itr2->item_guid);
                if (l_Item)
                    l_Draft.AddItem(l_Item);
                else
                {
                    //WTF?
                }

                m_Player->RemoveMItem(itr2->item_guid);
            }
        }
        l_Draft.AddMoney(l_Mail->money).SendReturnToSender(GetAccountId(), l_Mail->receiver, l_Mail->sender, l_Transaction);
    }

    CharacterDatabase.CommitTransaction(l_Transaction);

    delete l_Mail;                                               //we can deallocate old mail
    m_Player->SendMailResult(l_MailID, MAIL_RETURNED_TO_SENDER, MAIL_OK);
}

// Called when player takes item attached in mail
void WorldSession::HandleMailTakeItem(WorldPacket& p_Packet)
{
    uint64 l_Mailbox;
    uint32 l_MailID;
    uint32 l_ItemID;

    p_Packet.readPackGUID(l_Mailbox);
    p_Packet >> l_MailID;
    p_Packet >> l_ItemID;

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Mail * l_Mail = m_Player->GetMail(l_MailID);
    if (!l_Mail || l_Mail->state == MAIL_STATE_DELETED || l_Mail->deliver_time > time(NULL))
    {
        m_Player->SendMailResult(l_MailID, MAIL_ITEM_TAKEN, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    // prevent cheating with skip client money check
    if (!m_Player->HasEnoughMoney(uint64(l_Mail->COD)))
    {
        m_Player->SendMailResult(l_MailID, MAIL_ITEM_TAKEN, MAIL_ERR_NOT_ENOUGH_MONEY);
        return;
    }

    Item * l_Item = m_Player->GetMItem(l_ItemID);

    ItemPosCountVec l_Dest;
    uint8 l_Msg = m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, l_Dest, l_Item, false);

    if (l_Msg == EQUIP_ERR_OK)
    {
        SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
        l_Mail->RemoveItem(l_ItemID);
        l_Mail->removedItems.push_back(l_ItemID);

        if (l_Mail->COD > 0)                                     //if there is COD, take COD money from player and send them to sender by mail
        {
            uint64 sender_guid = MAKE_NEW_GUID(l_Mail->sender, 0, HIGHGUID_PLAYER);
            Player* receive = ObjectAccessor::FindPlayer(sender_guid);

            uint32 sender_accId = 0;

            if (!AccountMgr::IsPlayerAccount(GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
            {
                std::string sender_name;
                if (receive)
                {
                    sender_accId = receive->GetSession()->GetAccountId();
                    sender_name = receive->GetName();
                }
                else
                {
                    // can be calculated early
                    sender_accId = sObjectMgr->GetPlayerAccountIdByGUID(sender_guid);

                    if (!sObjectMgr->GetPlayerNameByGUID(sender_guid, sender_name))
                        sender_name = sObjectMgr->GetTrinityStringForDBCLocale(LANG_UNKNOWN);
                }
                sLog->outCommand(GetAccountId(), "", GetPlayer()->GetGUIDLow(), GetPlayer()->GetName(),
                                sender_accId, "", sender_guid, sender_name.c_str(),
                                "GM %s (Account: %u) receive mail item: %s (Entry: %u Count: %u) and send COD money: %lu to player: %s (Account: %u)",
                                GetPlayerName().c_str(), GetAccountId(), l_Item->GetTemplate()->Name1->Get(sWorld->GetDefaultDbcLocale()), l_Item->GetEntry(), l_Item->GetCount(), l_Mail->COD, sender_name.c_str(), sender_accId);
            }
            else if (!receive)
                sender_accId = sObjectMgr->GetPlayerAccountIdByGUID(sender_guid);

            // check player existence
            if (receive || sender_accId)
            {
                MailDraft(l_Mail->subject, "")
                    .AddMoney(l_Mail->COD)
                    .SendMailTo(l_Transaction, MailReceiver(receive, l_Mail->sender), MailSender(MAIL_NORMAL, l_Mail->receiver), MAIL_CHECK_MASK_COD_PAYMENT);
            }

            m_Player->ModifyMoney(-int32(l_Mail->COD));
        }
        l_Mail->COD = 0;
        l_Mail->state = MAIL_STATE_CHANGED;
        m_Player->m_mailsUpdated = true;
        m_Player->RemoveMItem(l_Item->GetGUIDLow());

        uint32 count = l_Item->GetCount();                      // save counts before store and possible merge with deleting
        l_Item->SetState(ITEM_UNCHANGED);                       // need to set this state, otherwise item cannot be removed later, if neccessary
        m_Player->MoveItemToInventory(l_Dest, l_Item, true);

        m_Player->SaveInventoryAndGoldToDB(l_Transaction);
        m_Player->_SaveMail(l_Transaction);
        CharacterDatabase.CommitTransaction(l_Transaction);

        m_Player->SendMailResult(l_MailID, MAIL_ITEM_TAKEN, MAIL_OK, 0, l_ItemID, count);
    }
    else
        m_Player->SendMailResult(l_MailID, MAIL_ITEM_TAKEN, MAIL_ERR_EQUIP_ERROR, l_Msg);
}

void WorldSession::HandleMailTakeMoney(WorldPacket& p_Packet)
{
    uint64 l_MailBox = 0;
    uint64 l_Money;
    uint32 l_MailID;

    p_Packet.readPackGUID(l_MailBox);
    p_Packet >> l_MailID;
    p_Packet >> l_Money;

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_MailBox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Mail * l_Mail = m_Player->GetMail(l_MailID);

    if ((!l_Mail || l_Mail->state == MAIL_STATE_DELETED || l_Mail->deliver_time > time(NULL)) ||
        (l_Money > 0 && l_Mail->money != l_Money))
    {
        m_Player->SendMailResult(l_MailID, MAIL_MONEY_TAKEN, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    // Don't take money if it exceed the max amount
    if (m_Player->GetMoney() + l_Money > MAX_MONEY_AMOUNT)
    {
        m_Player->SendMailResult(l_MailID, MAIL_MONEY_TAKEN, MAIL_ERR_EQUIP_ERROR);
        return;
    }

    m_Player->SendMailResult(l_MailID, MAIL_MONEY_TAKEN, MAIL_OK);

    m_Player->ModifyMoney(l_Money);
    l_Mail->money = 0;
    l_Mail->state = MAIL_STATE_CHANGED;
    m_Player->m_mailsUpdated = true;

    // save money and mail to prevent cheating
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    m_Player->SaveGoldToDB(trans);
    m_Player->_SaveMail(trans);
    CharacterDatabase.CommitTransaction(trans);
}

// Called when player lists his received mails
void WorldSession::HandleGetMailList(WorldPacket& p_Packet)
{
    uint64 l_MailBox;

    p_Packet.readPackGUID(l_MailBox);

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_MailBox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Player* player = m_Player;

    // client can't work with packets > max int16 value
    const uint32 maxPacketSize = 32767;

    uint32 mailsCount = 0;                                 // real send to client mails amount
    uint32 realCount  = 0;                                 // real mails amount

    WorldPacket l_Data(SMSG_MAIL_LIST_RESULT, 35 * 1024);
    ByteBuffer l_MailsBuffer;

    time_t cur_time = time(NULL);
    for (PlayerMails::iterator itr = player->GetMailBegin(); itr != player->GetMailEnd(); ++itr)
    {
        // Only first 50 mails are displayed
        if (mailsCount >= 50)
        {
            realCount += 1;
            continue;
        }

        // skip deleted or not delivered (deliver delay not expired) mails
        if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
            continue;

        uint8 item_count = (*itr)->items.size();            // max count is MAX_MAIL_ITEMS (12)

        size_t next_mail_size = 2+4+1+((*itr)->messageType == MAIL_NORMAL ? 8 : 4)+4*8+((*itr)->subject.size()+1)+((*itr)->body.size()+1)+1+item_count*(1+4+4+MAX_INSPECTED_ENCHANTMENT_SLOT*3*4+4+4+4+4+4+4+1);

        if (l_MailsBuffer.wpos() + next_mail_size > maxPacketSize)
        {
            realCount += 1;
            continue;
        }

        ++realCount;
        ++mailsCount;
    }

    l_Data << uint32(mailsCount);

    mailsCount = 0;

    for (PlayerMails::iterator itr = player->GetMailBegin(); itr != player->GetMailEnd(); ++itr)
    {
        // Only first 50 mails are displayed
        if (mailsCount >= 50)
            continue;

        // skip deleted or not delivered (deliver delay not expired) mails
        if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
            continue;

        uint8 item_count = (*itr)->items.size();                    // max count is MAX_MAIL_ITEMS (12)

        size_t next_mail_size = 2+4+1+((*itr)->messageType == MAIL_NORMAL ? 8 : 4)+4*8+((*itr)->subject.size()+1)+((*itr)->body.size()+1)+1+item_count*(1+4+4+MAX_INSPECTED_ENCHANTMENT_SLOT*3*4+4+4+4+4+4+4+1);

        if (l_MailsBuffer.wpos() + next_mail_size > maxPacketSize)
            continue;

        l_MailsBuffer << uint32((*itr)->messageID);                         // Message ID
        l_MailsBuffer << uint8((*itr)->messageType);                        // Message Type
        l_MailsBuffer << uint64((*itr)->COD);                               // COD
        l_MailsBuffer << uint32((*itr)->stationery);                        // stationery (Stationery.dbc)
        l_MailsBuffer << uint64((*itr)->money);                             // Gold
        l_MailsBuffer << uint32((*itr)->checked);                           // flags
        l_MailsBuffer << float(((*itr)->expire_time - time(NULL)) / DAY);   // Time
        l_MailsBuffer << uint32((*itr)->mailTemplateId);                    //

        l_MailsBuffer << uint32(item_count);

        for (uint8 l_I = 0; l_I < item_count; ++l_I)
        {
            Item* l_Item = player->GetMItem((*itr)->items[l_I].item_guid);

            l_MailsBuffer << uint8(l_I);
            l_MailsBuffer << uint32(l_Item ? l_Item->GetGUIDLow() : 0);

            Item::BuildDynamicItemDatas(l_MailsBuffer, l_Item);

            for (uint8 l_J = 0; l_J < MAX_INSPECTED_ENCHANTMENT_SLOT; ++l_J)
            {
                l_MailsBuffer << uint32(l_Item ? l_Item->GetEnchantmentId((EnchantmentSlot)l_J) : 0);
                l_MailsBuffer << uint32(l_Item ? l_Item->GetEnchantmentDuration((EnchantmentSlot)l_J) : 0);
                l_MailsBuffer << uint32(l_Item ? l_Item->GetEnchantmentCharges((EnchantmentSlot)l_J) : 0);
            }

            l_MailsBuffer << uint32(l_Item ? l_Item->GetCount() : 0);
            l_MailsBuffer << uint32(l_Item ? l_Item->GetSpellCharges() : 0);
            l_MailsBuffer << uint32(l_Item ? l_Item->GetUInt32Value(ITEM_FIELD_MAX_DURABILITY) : 0);
            l_MailsBuffer << uint32(l_Item ? l_Item->GetUInt32Value(ITEM_FIELD_DURABILITY) : 0);

            l_MailsBuffer.WriteBit(false);
            l_MailsBuffer.FlushBits();
        }

        l_MailsBuffer.WriteBit((*itr)->messageType == MAIL_NORMAL);            ///< Has Sender Character

        if ((*itr)->messageType == MAIL_CREATURE || (*itr)->messageType == MAIL_GAMEOBJECT || (*itr)->messageType == MAIL_AUCTION)
            l_MailsBuffer.WriteBit(1);                                         // hasSenderEntry
        else
            l_MailsBuffer.WriteBit(0);

        l_MailsBuffer.WriteBits((*itr)->subject.size(), 8);
        l_MailsBuffer.WriteBits((*itr)->body.size(), 13);
        l_MailsBuffer.FlushBits();

        if ((*itr)->messageType == MAIL_NORMAL)
            l_MailsBuffer.appendPackGUID(MAKE_NEW_GUID((*itr)->sender, 0, HIGHGUID_PLAYER));

        if ((*itr)->messageType == MAIL_CREATURE || (*itr)->messageType == MAIL_GAMEOBJECT || (*itr)->messageType == MAIL_AUCTION)
            l_MailsBuffer << uint32((*itr)->sender);                   // creature/gameobject entry, auction id

        l_MailsBuffer.WriteString((*itr)->subject);
        l_MailsBuffer.WriteString((*itr)->body);

        ++mailsCount;
    }

    l_Data << uint32(realCount);
    l_Data.append(l_MailsBuffer);

    SendPacket(&l_Data);

    // recalculate m_nextMailDelivereTime and unReadMails
    m_Player->UpdateNextMailTimeAndUnreads();
}

// Used when player copies mail body to his inventory
void WorldSession::HandleMailCreateTextItem(WorldPacket & p_Packet)
{
    uint64 l_Mailbox;
    uint32 l_MailID;

    p_Packet.readPackGUID(l_Mailbox);
    p_Packet >> l_MailID;

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_Mailbox, GAMEOBJECT_TYPE_MAILBOX))
        return;

    Mail * l_Mail = m_Player->GetMail(l_MailID);
    if (!l_Mail || (l_Mail->body.empty() && !l_Mail->mailTemplateId) || l_Mail->state == MAIL_STATE_DELETED || l_Mail->deliver_time > time(NULL))
    {
        m_Player->SendMailResult(l_MailID, MAIL_MADE_PERMANENT, MAIL_ERR_INTERNAL_ERROR);
        return;
    }

    Item * l_BodyItem = new Item;                              // This is not bag and then can be used new Item.
    if (!l_BodyItem->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), MAIL_BODY_ITEM_TEMPLATE, m_Player))
    {
        delete l_BodyItem;
        return;
    }

    // in mail template case we need create new item text
    if (l_Mail->mailTemplateId)
    {
        MailTemplateEntry const* mailTemplateEntry = sMailTemplateStore.LookupEntry(l_Mail->mailTemplateId);
        if (!mailTemplateEntry)
        {
            m_Player->SendMailResult(l_MailID, MAIL_MADE_PERMANENT, MAIL_ERR_INTERNAL_ERROR);
            return;
        }

        l_BodyItem->SetText(mailTemplateEntry->content);
    }
    else
        l_BodyItem->SetText(l_Mail->body);

    l_BodyItem->SetGuidValue(ITEM_FIELD_CREATOR, MAKE_NEW_GUID(l_Mail->sender, 0, HIGHGUID_PLAYER));
    l_BodyItem->SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_MAIL_TEXT_MASK);

    ItemPosCountVec l_Destinatiopn;
    uint8 l_Msg = m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, l_Destinatiopn, l_BodyItem, false);

    if (l_Msg == EQUIP_ERR_OK)
    {
        l_Mail->checked = l_Mail->checked | MAIL_CHECK_MASK_COPIED;
        l_Mail->state = MAIL_STATE_CHANGED;
        m_Player->m_mailsUpdated = true;

        m_Player->StoreItem(l_Destinatiopn, l_BodyItem, true);
        m_Player->SendMailResult(l_MailID, MAIL_MADE_PERMANENT, MAIL_OK);
    }
    else
    {
        m_Player->SendMailResult(l_MailID, MAIL_MADE_PERMANENT, MAIL_ERR_EQUIP_ERROR, l_Msg);
        delete l_BodyItem;
    }
}

// @TODO Fix me! ... this void has probably bad condition, but good data are sent
void WorldSession::HandleQueryNextMailTime(WorldPacket & /*recvData*/)
{
    WorldPacket l_Data(SMSG_MAIL_QUERY_NEXT_TIME_RESULT, 400);

    if (m_Player->unReadMails > 0)
    {
        l_Data << float(0);                                                             ///< NextMailTime
        l_Data << uint32(0);                                                            ///< Next

        uint32 l_Count = 0;
        time_t l_NowTime = time(NULL);
        std::set<uint32> l_SentSenders;

        for (PlayerMails::iterator itr = m_Player->GetMailBegin(); itr != m_Player->GetMailEnd(); ++itr)
        {
            Mail * l_Mail = (*itr);
            // must be not checked yet
            if (l_Mail->checked & MAIL_CHECK_MASK_READ)
                continue;

            // and already delivered
            if (l_NowTime < l_Mail->deliver_time)
                continue;

            // only send each mail sender once
            if (l_SentSenders.count(l_Mail->sender))
                continue;

            uint64 l_Guid = MAKE_NEW_GUID(l_Mail->sender, 0, HIGHGUID_PLAYER);
            l_Data.appendPackGUID(l_Mail->messageType == MAIL_NORMAL ? l_Guid : 0);     ///< SenderGUID

            l_Data << float(l_Mail->deliver_time - l_NowTime);                          ///< TimeLeft
            l_Data << uint32(l_Mail->messageType != MAIL_NORMAL ? l_Mail->sender : 0);  ///< AltSenderID
            l_Data << uint8(l_Mail->messageType);                                       ///< AltSenderType
            l_Data << uint32(l_Mail->stationery);                                       ///< StationeryID

            l_SentSenders.insert(l_Mail->sender);
            ++l_Count;

            if (l_Count == 2)                                  // do not display more than 2 mails
                break;
        }

        l_Data.put<uint32>(4, l_Count);
    }
    else
    {
        l_Data << float(-DAY);
        l_Data << uint32(0);
    }

    SendPacket(&l_Data);
}
#endif