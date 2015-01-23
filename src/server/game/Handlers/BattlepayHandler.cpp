////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "BattlepayMgr.h"
#include "BattlepayPacketFactory.h"

void WorldSession::HandleBattlepayGetPurchaseList(WorldPacket& p_RecvData)
{
    Battlepay::PacketFactory::SendPurchaseList(this);
}

void WorldSession::HandleBattlepayGetProductListQuery(WorldPacket& p_RecvData)
{
    if (!sBattlepayMgr->IsAvailable())
        return;

    Battlepay::PacketFactory::SendDistributionList(this);
    Battlepay::PacketFactory::SendProductList(this);
}

void WorldSession::HandleBattlePayStartPurchase(WorldPacket& p_RecvData)
{
    uint64 l_TargetCharacter;
    uint32 l_ProductID;
    uint32 l_ClientToken;

    p_RecvData >> l_ClientToken;
    p_RecvData >> l_ProductID;
    p_RecvData.readPackGUID(l_TargetCharacter);

    /// Create new purchase for the actual transaction
    Battlepay::Purchase l_Purchase;
    l_Purchase.ProductID       = l_ProductID;
    l_Purchase.ClientToken     = l_ClientToken;
    l_Purchase.TargetCharacter = l_TargetCharacter;
    l_Purchase.Status          = Battlepay::PacketFactory::UpdateStatus::Loading;

    /// @TODO: Handle price, already have it ...etc and generate a error code (result) if needed
    auto l_CharacterNameData = sWorld->GetCharacterNameData(GUID_LOPART(l_TargetCharacter));

    /// The TargetCharacter guid sended by the client doesn't exist
    if (l_CharacterNameData == nullptr)
    {
        Battlepay::PacketFactory::SendStartPurchaseResponse(this, l_Purchase, Battlepay::PacketFactory::Error::Denied);
        return;
    }

    /// The TargetCharacter guid sended by the client isn't owned by the current account
    if (l_CharacterNameData->m_AccountId != GetAccountId())
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

    Battlepay::Product const& l_Product = sBattlepayMgr->GetProduct(l_ProductID);

    /// Purchase can be done, let's generate purchase ID & Server Token
    l_Purchase.PurchaseID      = sBattlepayMgr->GenerateNewPurchaseID();
    l_Purchase.ServerToken     = urand(0, 0xFFFFFFF);
    l_Purchase.CurrentPrice    = l_Product.CurrentPriceFixedPoint;
    l_Purchase.Status          = Battlepay::PacketFactory::UpdateStatus::Ready;

    /// Store the purchase and link it to the current account, we will need it later
    sBattlepayMgr->RegisterStartPurchase(GetAccountId(), l_Purchase);

    /// Send neccesary packets to client to show the confirmation window
    Battlepay::PacketFactory::SendStartPurchaseResponse(this, l_Purchase, Battlepay::PacketFactory::Error::OtherOK);
    Battlepay::PacketFactory::SendPurchaseUpdate(this, l_Purchase, Battlepay::PacketFactory::Error::OtherOK);
    Battlepay::PacketFactory::SendConfirmPurchase(this, l_Purchase);
}

void WorldSession::HandleBattlePayConfirmPurchase(WorldPacket& p_RecvData)
{
    uint32 l_ServerToken;
    uint64 l_ClientCurrentPriceFixedPoint;
    bool   l_ConfirmPurchase;

    l_ConfirmPurchase = p_RecvData.ReadBit();
    p_RecvData.ResetBitReading();

    p_RecvData >> l_ServerToken;
    p_RecvData >> l_ClientCurrentPriceFixedPoint;

    Battlepay::Purchase const* l_Purchase = sBattlepayMgr->GetPurchase(GetAccountId());
    /// We can't handle that case because we havn't purchase data
    /// Anyway, the client is cheater if it send payConfirm opcode without starting purchase
    if (l_Purchase == nullptr)
        return;

    if (l_Purchase->ServerToken != l_ServerToken)
    {
        Battlepay::PacketFactory::SendPurchaseUpdate(this, *l_Purchase, Battlepay::PacketFactory::Error::Denied);
        return;
    }

    if (l_ConfirmPurchase == false)
    {
        Battlepay::PacketFactory::SendPurchaseUpdate(this, *l_Purchase, Battlepay::PacketFactory::Error::OtherCancelByUser);
        return;
    }

    if (l_Purchase->CurrentPrice != l_ClientCurrentPriceFixedPoint)
    {
        Battlepay::PacketFactory::SendPurchaseUpdate(this, *l_Purchase, Battlepay::PacketFactory::Error::InsufficientBalance);
        return;
    }
}