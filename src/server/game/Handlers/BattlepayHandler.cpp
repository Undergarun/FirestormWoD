////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "BattlepayMgr.h"
#include "BattlepayPacketFactory.h"
#include "ScriptMgr.h"

void WorldSession::HandleBattlepayGetPurchaseList(WorldPacket& /*p_RecvData*/)
{
    Battlepay::PacketFactory::SendPurchaseList(this);
}

void WorldSession::HandleBattlepayGetProductListQuery(WorldPacket& /*p_RecvData*/)
{
    if (!sBattlepayMgr->IsAvailable(this))
        return;

    Battlepay::PacketFactory::SendDistributionList(this);
    Battlepay::PacketFactory::SendProductList(this);
    Battlepay::PacketFactory::SendPointsBalance(this);
}

void WorldSession::HandleBattlePayStartPurchase(WorldPacket& p_RecvData)
{
    if (!sWorld->getBoolConfig(CONFIG_WEB_DATABASE_ENABLE))
        return;

    uint64 l_TargetCharacter;
    uint32 l_ProductID;
    uint32 l_ClientToken;

    /// Read packet
    p_RecvData >> l_ClientToken;
    p_RecvData >> l_ProductID;
    p_RecvData.readPackGUID(l_TargetCharacter);

    /// Create new purchase for the actual transaction
    Battlepay::Purchase l_Purchase;
    l_Purchase.ProductID       = l_ProductID;
    l_Purchase.ClientToken     = l_ClientToken;
    l_Purchase.TargetCharacter = l_TargetCharacter;
    l_Purchase.Status          = Battlepay::PacketFactory::UpdateStatus::Loading;

    auto l_CharacterNameData = sWorld->GetCharacterInfo(GUID_LOPART(l_TargetCharacter));

    /// The TargetCharacter guid sended by the client doesn't exist
    if (l_CharacterNameData == nullptr)
    {
        Battlepay::PacketFactory::SendStartPurchaseResponse(this, l_Purchase, Battlepay::PacketFactory::Error::Denied);
        return;
    }

    /// The TargetCharacter guid sended by the client isn't owned by the current account
    if (l_CharacterNameData->AccountId != GetAccountId())
    {
        Battlepay::PacketFactory::SendStartPurchaseResponse(this, l_Purchase, Battlepay::PacketFactory::Error::Denied);
        return;
    }

    /// The ProductID sended by the client doesn't refer to any existing product
    if (!sBattlepayMgr->ProductExist(l_ProductID))
    {
        Battlepay::PacketFactory::SendStartPurchaseResponse(this, l_Purchase, Battlepay::PacketFactory::Error::Denied);
        return;
    }

    /// Update purchase price
    Battlepay::Product const& l_Product = sBattlepayMgr->GetProduct(l_Purchase.ProductID);
    l_Purchase.CurrentPrice = l_Product.CurrentPriceFixedPoint;

    /// Store the purchase and link it to the current account, we will need it later
    sBattlepayMgr->RegisterStartPurchase(GetAccountId(), l_Purchase);

    /// Query balance to database
    PreparedStatement* l_Statement = WebDatabase.GetPreparedStatement(WEB_SEL_ACCOUNT_POINTS);
    l_Statement->setUInt32(0, GetAccountId());
    l_Statement->setUInt32(1, GetAccountId());

    uint32 l_AccountId = GetAccountId();

    WebDatabase.AsyncQuery(l_Statement, [l_AccountId, l_Product](PreparedQueryResult p_Result) -> void
    {
        WorldSession* l_Session = sWorld->FindSession(l_AccountId);
        if (l_Session == nullptr)
            return;

        Battlepay::Purchase* l_Purchase = sBattlepayMgr->GetPurchase(l_Session->GetAccountId());

        /// Never buy points
        if (!p_Result)
        {
            Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::InsufficientBalance);
            return;
        }

        /// Check balance
        Field* l_Fields = p_Result->Fetch();
        if (std::atoi(l_Fields[0].GetCString()) < (int64)l_Purchase->CurrentPrice)
        {
            Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::InsufficientBalance);
            return;
        }

        /// Check bag free spaces (items)
        if (l_Product.Items.size())
        {
            Player* l_Player = l_Session->GetPlayer();
            if (l_Player != nullptr && l_Product.Items.size() > l_Player->GetBagsFreeSlots())
            {
                std::ostringstream l_Data;
                l_Data << sObjectMgr->GetTrinityString(Battlepay::String::NotEnoughFreeBagSlots, l_Session->GetSessionDbLocaleIndex());
                l_Player->SendCustomMessage(Battlepay::PacketFactory::CustomMessage::GetCustomMessage(Battlepay::PacketFactory::CustomMessage::AshranStoreBuyFailed), l_Data);
                Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
                return;
            }
        }

        /// Custom check (scripts)
        if (!l_Product.ScriptName.empty())
        {
            std::string l_Reason;
            bool l_CanBuy = sScriptMgr->BattlePayCanBuy(l_Session, l_Product, l_Reason);
            if (!l_CanBuy)
            {
                std::ostringstream l_Data;
                l_Data << l_Reason;
                l_Session->GetPlayer()->SendCustomMessage(Battlepay::PacketFactory::CustomMessage::GetCustomMessage(Battlepay::PacketFactory::CustomMessage::AshranStoreBuyFailed), l_Data);
                Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
                return;
            }
        }

        for (auto l_ItemProduct : l_Product.Items)
        {
            if (sBattlepayMgr->AlreadyOwnProduct(l_ItemProduct.ItemID, l_Session->GetPlayer()))
            {
                std::ostringstream l_Data;
                l_Data << sObjectMgr->GetTrinityString(Battlepay::String::YouAlreadyOwnThat, l_Session->GetSessionDbLocaleIndex());;
                l_Session->GetPlayer()->SendCustomMessage(Battlepay::PacketFactory::CustomMessage::GetCustomMessage(Battlepay::PacketFactory::CustomMessage::AshranStoreBuyFailed), l_Data);
                Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
                return;
            }
        }

        /// Purchase can be done, let's generate purchase ID & Server Token
        l_Purchase->PurchaseID = sBattlepayMgr->GenerateNewPurchaseID();
        l_Purchase->ServerToken = urand(0, 0xFFFFFFF);
        l_Purchase->Status = Battlepay::PacketFactory::UpdateStatus::Ready;

        /// Send neccesary packets to client to show the confirmation window
        Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Ok);
        Battlepay::PacketFactory::SendPurchaseUpdate(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Ok);
        Battlepay::PacketFactory::SendConfirmPurchase(l_Session, *l_Purchase);
    });
}

void WorldSession::HandleBattlePayConfirmPurchase(WorldPacket& p_RecvData)
{
    if (!sWorld->getBoolConfig(CONFIG_WEB_DATABASE_ENABLE))
        return;

    uint32 l_ServerToken;
    uint64 l_ClientCurrentPriceFixedPoint;
    bool   l_ConfirmPurchase;

    /// Read packet
    l_ConfirmPurchase = p_RecvData.ReadBit();
    p_RecvData.ResetBitReading();

    p_RecvData >> l_ServerToken;
    p_RecvData >> l_ClientCurrentPriceFixedPoint;

    /// Apply balance currency precision (thx @blizz)
    l_ClientCurrentPriceFixedPoint /= Battlepay::PacketFactory::g_CurrencyPrecision;

    Battlepay::Purchase* l_Purchase = sBattlepayMgr->GetPurchase(GetAccountId());
    /// We can't handle that case because we havn't purchase data
    /// Anyway, the client is cheater if it send payConfirm opcode without starting purchase
    if (l_Purchase == nullptr)
        return;

    /// Database query to process that purchase is already in progress.
    if (l_Purchase->Lock)
        return;

    /// Client refuse to buy or send invalid data
    if (l_Purchase->ServerToken != l_ServerToken
        || l_ConfirmPurchase == false
        || l_Purchase->CurrentPrice != l_ClientCurrentPriceFixedPoint)
    {
        Battlepay::PacketFactory::SendPurchaseUpdate(this, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
        return;
    }

    l_Purchase->Lock = true;

    /// Double balance check, because the balance can change since the user click on "Buy now"
    PreparedStatement* l_Statement = WebDatabase.GetPreparedStatement(WEB_SEL_ACCOUNT_POINTS);
    l_Statement->setUInt32(0, GetAccountId());
    l_Statement->setUInt32(1, GetAccountId());

    uint32 l_AccountId = GetAccountId();

    WebDatabase.AsyncQuery(l_Statement, [l_AccountId, l_Purchase](PreparedQueryResult p_Result) -> void
    {
        /// Never buy points
        if (!p_Result)
            return;

        WorldSession* l_Session = sWorld->FindSession(l_AccountId);
        if (l_Session == nullptr)
            return;

        /// Check balance
        Field* l_Fields = p_Result->Fetch();
        if (std::atoi(l_Fields[0].GetCString()) < (int64)l_Purchase->CurrentPrice)
            return;

        /// Custom check (scripts)
        Battlepay::Product const& l_Product = sBattlepayMgr->GetProduct(l_Purchase->ProductID);
        if (!l_Product.ScriptName.empty())
        {
            std::string l_Reason;
            bool l_CanBuy = sScriptMgr->BattlePayCanBuy(l_Session, l_Product, l_Reason);
            if (!l_CanBuy)
            {
                std::ostringstream l_Data;
                l_Data << l_Reason;
                l_Session->GetPlayer()->SendCustomMessage(Battlepay::PacketFactory::CustomMessage::GetCustomMessage(Battlepay::PacketFactory::CustomMessage::AshranStoreBuyFailed), l_Data);
                Battlepay::PacketFactory::SendPurchaseUpdate(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::PaymentFailed);
                return;
            }
        }

        /// Check bag free spaces (items)
        if (l_Product.Items.size())
        {
            Player* l_Player = l_Session->GetPlayer();
            if (l_Player != nullptr && l_Product.Items.size() > l_Player->GetBagsFreeSlots())
            {
                std::ostringstream l_Data;
                l_Data << sObjectMgr->GetTrinityString(Battlepay::String::NotEnoughFreeBagSlots, l_Session->GetSessionDbLocaleIndex());
                l_Player->SendCustomMessage(Battlepay::PacketFactory::CustomMessage::GetCustomMessage(Battlepay::PacketFactory::CustomMessage::AshranStoreBuyFailed), l_Data);
                Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
                return;
            }
        }

        for (auto l_ItemProduct : l_Product.Items)
        {
            if (sBattlepayMgr->AlreadyOwnProduct(l_ItemProduct.ItemID, l_Session->GetPlayer()))
            {
                std::ostringstream l_Data;
                l_Data << sObjectMgr->GetTrinityString(Battlepay::String::YouAlreadyOwnThat, l_Session->GetSessionDbLocaleIndex());;
                l_Session->GetPlayer()->SendCustomMessage(Battlepay::PacketFactory::CustomMessage::GetCustomMessage(Battlepay::PacketFactory::CustomMessage::AshranStoreBuyFailed), l_Data);
                Battlepay::PacketFactory::SendStartPurchaseResponse(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
                return;
            }
        }

        /// Display "Purchase delivered" frame
        Battlepay::PacketFactory::SendPurchaseUpdate(l_Session, *l_Purchase, Battlepay::PacketFactory::Error::Other);

        /// Save purchase in database (that decrease balance) & send balance update
        sBattlepayMgr->SavePurchase(l_Session, l_Purchase);

        /// Deliver purchases (items, gold, services ...etc)
        sBattlepayMgr->ProcessDelivery(l_Session, l_Purchase);
    });
}
