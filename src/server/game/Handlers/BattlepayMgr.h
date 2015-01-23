////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# ifndef _BATTLEPAY_MGR_HPP
# define _BATTLEPAY_MGR_HPP

#include "Common.h"

namespace Battlepay
{
    /// All kind of currency we can use in Battlepay
    enum ShopCurrency
    {
        Unknow = 0,
        Usd    = 1,
        Gbp    = 2,
        Krw    = 3,
        Eur    = 4,
        Rub    = 5,
        Ars    = 8,
        CLP    = 9,
        Mxn    = 10,
        Brl    = 11,
        Aud    = 12,
        Cpt    = 14,
        Tpt    = 15,
        Beta   = 16
    };

    /// Client error enum
    /// See Blizzard_StoreUISecure.lua
    /// Last update : 6.0.3 19116
    enum Error
    {
        InvalidPaymentMethod       = 25,
        PaymentFailed              = 2,
        WrongCurrency              = 12,
        BattlepayDisabled          = 13,
        InsufficientBalance        = 28,
        Other                      = 0,
        //AlreadyOwned             = 0,     ///< This error is client-side only, can't be sended by the server
        ParentalControlsNoPurchase = 34,
        Denied                     = 1
    };

    struct ProductGroup
    {
        uint32          GroupID;
        std::string     Name;
        int32           IconFileDataID;
        uint8           DisplayType;
        int32           Ordering;
    };

    struct DisplayInfo
    {
        uint32          CreatureDisplayInfoID;
        uint32          FileDataID;
        std::string     Name1;
        std::string     Name2;
        std::string     Name3;
        uint32          Flags;
    };

    struct ProductItem
    {
        uint32          ID;
        uint32          ItemID;
        uint32          Quantity;
        uint32          DisplayInfoID;
        bool            HasPet;
        uint8           PetResult;
    };

    struct Product
    {
        uint32                      ProductID;
        uint64                      NormalPriceFixedPoint;
        uint64                      CurrentPriceFixedPoint;
        std::vector<ProductItem>    Items;
        uint8                       Type;
        uint8                       ChoiceType;
        uint32                      Flags;
        uint32                      DisplayInfoID;
    };

    struct ShopEntry
    {
        uint32          EntryID;
        uint32          GroupID;
        uint32          ProductID;
        int32           Ordering;
        uint32          Flags;
        uint8           BannerType;
        uint32          DisplayInfoID;
    };

    struct Purchase
    {
        Purchase()
        {
            memset(this, 0, sizeof(Purchase));
        }

        uint64 PurchaseID;
        uint32 ClientToken;
        uint32 ServerToken;
        uint32 ProductID;
        uint64 CurrentPrice;
        uint64 TargetCharacter;
        uint8  Status;
    };

    class Manager
    {
        public:

            Manager()
            {
                m_WalletName = "Ashran points";     ///< @TODO: Take it from server conf
                m_PurchaseIDCount = 0;
            }

            ~Manager() {};

            /*
            * Load all battlepay data from database
            */
            void LoadFromDatabase();

            /*
            * Return the current currency we use for battlepay
            */
            ShopCurrency GetShopCurrency() const;

            /*
            * Return if the battlepay is available for player or not
            */
            bool IsAvailable() const;

            std::vector<ProductGroup> const& GetProductGroups() const { return m_ProductGroups; }
            std::vector<ShopEntry> const& GetShopEntries() const { return m_ShopEntries; }
            std::map<uint32, Product> const& GetProducts() const { return m_Products; }

            bool ProductExist(uint32 p_ProductID) const
            {
                if (m_Products.find(p_ProductID) == m_Products.end())
                    return false;
                return true;
            }

            Battlepay::Product const& GetProduct(uint32 p_ProductID) const
            {
                return m_Products.at(p_ProductID);
            }

            DisplayInfo const* GetDisplayInfo(uint32 p_Id) const
            { 
                if (m_DisplayInfos.find(p_Id) == m_DisplayInfos.end())
                    return nullptr;

                return &m_DisplayInfos.at(p_Id);
            }

            void RegisterStartPurchase(uint32 p_AccountId, Battlepay::Purchase p_Purchase)
            {
                m_ActualTransactions[p_AccountId] = p_Purchase;
            }

            uint64 GenerateNewPurchaseID()
            {
                return uint64(0x1E77800000000000 | ++m_PurchaseIDCount);
            }

            Battlepay::Purchase const* GetPurchase(uint64 p_PlayerGUID) const
            {
                if (m_ActualTransactions.find(p_PlayerGUID) == m_ActualTransactions.end())
                    return nullptr;

                return &m_ActualTransactions.at(p_PlayerGUID);
            }

            std::string const& GetDefaultWalletName() const { return m_WalletName; }

            /// @TODO: Bruteforce to make proper server enum
            uint32 ConvertServerErrorToClientError(uint32 p_ServerError)
            {
                if (p_ServerError <= 14)
                {
                    if (p_ServerError >= 13)
                        return 3;
                    int v1 = p_ServerError - 1;
                    if (!v1)
                        return 8;
                    int v2 = v1 - 1;
                    if (!v2)
                        return 1;
                    if (v2 == 10)
                        return 2;
                    return 5;
                }

                if (p_ServerError != 25)
                {
                    if (p_ServerError > 27)
                    {
                        if (p_ServerError <= 29)
                            return 4;
                        if (p_ServerError == 34)
                            return 7;
                    }
                    return 5;
                }
                return 0;
            }

        private:

            /*
            * Load product groups of Battlepay in database
            */
            void LoadProductGroups();

            /*
            * Load products of Battlepay in database
            */
            void LoadProduct();

            /*
            * Load shop entries of Battlepay in database
            */
            void LoadShopEntires();

            /*
            * Load display info of Battlepay in database
            */
            void LoadDisplayInfos();

            std::vector<ProductGroup>     m_ProductGroups;
            std::vector<ShopEntry>        m_ShopEntries;
            std::map<uint32, Product>     m_Products;
            std::map<uint32, DisplayInfo> m_DisplayInfos;
            std::map<uint32, Purchase>    m_ActualTransactions;

            uint64       m_PurchaseIDCount;
            std::string  m_WalletName;

    };
}

#define sBattlepayMgr ACE_Singleton<Battlepay::Manager, ACE_Null_Mutex>::instance()
#endif