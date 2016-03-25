////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# ifndef _BATTLEPAY_PACKET_FACTORY_HPP
# define _BATTLEPAY_PACKET_FACTORY_HPP

#include "Common.h"
#include "BattlepayMgr.h"

namespace Battlepay
{
    namespace PacketFactory
    {
        const float g_CurrencyPrecision = 10000.0f;

        /// Client error enum
        /// See Blizzard_StoreUISecure.lua
        /// Last update : 6.2.3 20779
        enum Error
        {
            InvalidPaymentMethod       = 25,
            PaymentFailed              = 2,
            WrongCurrency              = 12,
            BattlepayDisabled          = 13,    ///< Also 14
            InsufficientBalance        = 28,    ///< Also 29
            Other                      = 3,
            //AlreadyOwned               = ,
            ParentalControlsNoPurchase = 34,
            Denied                     = 1,
            ConsumableTokenOwned       = 46,
            TooManyTokens              = 47,
            Ok                         = 0
            /// ItemUnavailable
        };

        /// Result of SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE
        /// see @BattlePay::PacketFactory::SendProductList
        namespace ProductListResult
        {
            enum
            {
                Available    = 0,       ///< The shop is available
                LockUnk1     = 1,       ///< The shop is locked, unknow reason (i've see nothing in client for case "1", all result > 0 lock the shop anyway)
                LockUnk2     = 2,       ///< The shop is locked, unknow reason (i've see nothing in client for case "1", all result > 0 lock the shop anyway)
                RegionLocked = 3        ///< The shop is locked because the region of the player is locked
            };
        }

        /// Update status of SMSG_BATTLE_PAY_PURCHASE_UPDATE
        /// see @Battlepay::PacketFactory::SendPurchaseUpdate
        namespace UpdateStatus
        {
            enum
            {
                Loading = 2,
                Ready   = 3,
                Finish  = 6
            };
        }

        namespace CustomMessage
        {
            enum
            {
                AshranStoreBalance,
                AshranAccountId,
                AshranStoreBuyFailed
            };

            static const char* g_CustomMessage[] =
            {
                "ASHRAN_STORE_BALANCE",
                "ASHRAN_ACCOUNT_ID",
                "ASHRAN_STORE_BUY_FAILED"
            };

            static const char* GetCustomMessage(uint8 p_ID)
            {
                return g_CustomMessage[p_ID];
            }
        }

        /*
        * Write display info into packet bytebuffer
        * @param p_DisplayInfoID: Id of the display info to write
        * @param p_Packet: The packet where we write display info data
        */
        void WriteDisplayInfo(uint32 p_DisplayInfoID, WorldPacket& p_Packet, LocaleConstant p_LocaleIdx, std::string p_Description = "");

        /*
        * Build & send a battlepay start purchase response packet to the client
        * @param p_Session  : Packet will be sent to that WorldSession
        * @param p_Purchase : Purchase we want send the start response of
        * @param p_Error    : Error / Result of the start purchase, see @Battlepay::PacketFactory::Error enum
        */
        void SendStartPurchaseResponse(WorldSession* p_Session, Battlepay::Purchase const& p_Purchase, Battlepay::PacketFactory::Error const& p_Error);

        /*
        * Build & send a battlepay purchase update
        * @param p_Session  : Packet will be sent to that WorldSession
        * @param p_Purchase : Purchase we want made a update of
        * @param p_Result   : Result of the update, need to be reverse
        */
        void SendPurchaseUpdate(WorldSession* p_Session, Battlepay::Purchase const& p_Purchase, uint32 p_Result);

        /*
        * Build & send a battlepay purchase confirm
        * @param p_Session  : Packet will be sent to that WorldSession
        * @param p_Purchase : Purchase we want made a confirm notification of
        */
        void SendConfirmPurchase(WorldSession* p_Session, Battlepay::Purchase const& p_Purchase);

        /*
        * Build & send the battlepay product list
        * @param p_Session  : Packet will be sent to that WorldSession
        */
        void SendProductList(WorldSession* p_Session);

        /*
        * NIY, send dummy packet (client need it to show store)
        * Build & send the battlepay purchase list
        * @param p_Session  : Packet will be sent to that WorldSession
        */
        void SendPurchaseList(WorldSession* p_Session);

        /*
        * NIY, send dummy packet (client need it to show store)
        * Build & send the battlepay distribution list
        * @param p_Session  : Packet will be sent to that WorldSession
        */
        void SendDistributionList(WorldSession* p_Session);

        /*
        * Send points balance to client
        * Because it's custom, we use addon message system
        * @param p_Session  : Packet will be sent to that WorldSession
        */
        void SendPointsBalance(WorldSession* p_Session);
    }
}

# endif