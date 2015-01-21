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

    class Manager
    {
        public:

            Manager() {};
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
            std::map<uint32, Product> const& GetProducts();

            DisplayInfo const* GetDisplayInfo(uint32 p_Id) const
            { 
                if (m_DisplayInfos.find(p_Id) == m_DisplayInfos.end())
                    return nullptr;

                return &m_DisplayInfos.at(p_Id);
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
    };
}

#define sBattlepayMgr ACE_Singleton<Battlepay::Manager, ACE_Null_Mutex>::instance()
#endif