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

#ifndef TRINITY_LOOTMGR_H
#define TRINITY_LOOTMGR_H

#include "ItemEnchantmentMgr.h"
#include "ByteBuffer.h"
#include "RefManager.h"
#include "SharedDefines.h"
#include "ConditionMgr.h"
#include "Object.h"
#include "ItemPrototype.h"

#include <map>
#include <vector>

enum RollType
{
    ROLL_PASS         = 0,
    ROLL_NEED         = 1,
    ROLL_GREED        = 2,
    ROLL_DISENCHANT   = 3,
    MAX_ROLL_TYPE     = 4
};

enum RollMask
{
    ROLL_FLAG_TYPE_PASS         = 0x01,
    ROLL_FLAG_TYPE_NEED         = 0x02,
    ROLL_FLAG_TYPE_GREED        = 0x04,
    ROLL_FLAG_TYPE_DISENCHANT   = 0x08,

    ROLL_ALL_TYPE_NO_DISENCHANT = 0x07,
    ROLL_ALL_TYPE_MASK          = 0x0F
};

#define MAX_NR_LOOT_ITEMS 16
// note: the client cannot show more than 16 items total
#define MAX_NR_QUEST_ITEMS 32
// unrelated to the number of quest items shown, just for reserve

enum LootMethod
{
    FREE_FOR_ALL      = 0,
    ROUND_ROBIN       = 1,
    MASTER_LOOT       = 2,
    GROUP_LOOT        = 3,
    NEED_BEFORE_GREED = 4
};

enum PermissionTypes
{
    ALL_PERMISSION              = 0,
    GROUP_PERMISSION            = 1,
    MASTER_PERMISSION           = 2,
    ROUND_ROBIN_PERMISSION      = 3,
    OWNER_PERMISSION            = 4,
    NONE_PERMISSION             = 5,
};

enum LootItemType
{
    LOOT_ITEM_TYPE_CURRENCY = 0, // Guessed ????
    LOOT_ITEM_TYPE_ITEM     = 2,
};

enum LootType
{
    LOOT_CORPSE                 = 1,
    LOOT_PICKPOCKETING          = 2,
    LOOT_FISHING                = 3,
    LOOT_DISENCHANTING          = 4,
                                                            // ignored always by client
    LOOT_SKINNING               = 6,
    LOOT_PROSPECTING            = 7,
    LOOT_MILLING                = 8,

    LOOT_FISHINGHOLE            = 20,                       // unsupported by client, sending LOOT_FISHING instead
    LOOT_INSIGNIA               = 21                        // unsupported by client, sending LOOT_CORPSE instead
};

enum LootListItemType
{
    LOOT_LIST_ITEM                      = 0,
    LOOT_LIST_CURRENCY                  = 1,
    LOOT_LIST_TRACKING_QUEST            = 2,
};

// type of Loot Item in Loot View
enum LootSlotType
{
    LOOT_SLOT_TYPE_ALLOW_LOOT   = 4,                        // player can loot the item.
    LOOT_SLOT_TYPE_ROLL_ONGOING = 7,                        // roll is ongoing. player cannot loot.
    LOOT_SLOT_TYPE_MASTER       = 6,                        // item can only be distributed by group loot master.
    LOOT_SLOT_TYPE_LOCKED       = 3,                        // item is shown in red. player cannot loot.
    LOOT_SLOT_TYPE_OWNER        = 5                         // ignore binding confirmation and etc, for single player looting
};

class Player;
class LootStore;
class ConditionMgr;

struct LootStoreItem
{
    uint32  itemid;                                         // id of the item
    uint8   type;                                           // 0 = item, 1 = currency
    float   chance;                                         // always positive, chance to drop for both quest and non-quest items, chance to be used for refs
    int32   mincountOrRef;                                  // mincount for drop items (positive) or minus referenced TemplateleId (negative)
    uint16  lootmode;
    uint8   group       :7;
    bool    needs_quest :1;                                 // quest drop (negative ChanceOrQuestChance in DB)
    uint32   maxcount;                                      // max drop count for the item (mincountOrRef positive) or Ref multiplicator (mincountOrRef negative)
    std::vector<uint32> itemBonuses;                        // item bonuses >= WoD
    ConditionContainer conditions;                               // additional loot condition

    // Constructor, converting ChanceOrQuestChance -> (chance, needs_quest)
    // displayid is filled in IsValid() which must be called after
    LootStoreItem(uint32 _itemid, uint8 _type, float _chanceOrQuestChance, uint16 _lootmode, uint8 _group, int32 _mincountOrRef, uint32 _maxcount, std::vector<uint32> _itemBonuses)
        : itemid(_itemid), type(_type), chance(fabs(_chanceOrQuestChance)), mincountOrRef(_mincountOrRef), lootmode(_lootmode),
        group(_group), needs_quest(_chanceOrQuestChance < 0), maxcount(_maxcount), itemBonuses(_itemBonuses)
         {}

    bool Roll(bool rate, Player const* Player) const;                             // Checks if the entry takes it's chance (at loot generation)
    bool IsValid(LootStore const& store, uint32 entry) const;
                                                            // Checks correctness of values
};

typedef std::set<uint32> AllowedLooterSet;

struct Loot;

struct LootItem
{
    Loot*   currentLoot;
    uint32  itemid;
    uint8   type;                                           // 0 = item, 1 = currency
    uint32  randomSuffix;
    int32   randomPropertyId;
    ConditionContainer conditions;                       // additional loot condition
    std::vector<uint32> itemBonuses;
    AllowedLooterSet allowedGUIDs;
    uint32   count;
    uint64  PersonalLooter;
    bool    currency          : 1;
    bool    is_looted         : 1;
    bool    is_blocked        : 1;
    bool    freeforall        : 1;                          // free for all
    bool    is_underthreshold : 1;
    bool    is_counted        : 1;
    bool    needs_quest       : 1;                          // quest drop
    bool    follow_loot_rules : 1;
    bool    alreadyAskedForRoll;

    // Constructor, copies most fields from LootStoreItem, generates random count and random suffixes/properties
    // Should be called for non-reference LootStoreItem entries only (mincountOrRef > 0)
    explicit LootItem(LootStoreItem const& p_LootItem, ItemContext p_Context, Loot* p_Loot);

    // Basic checks for player/item compatibility - if false no chance to see the item in the loot
    bool AllowedForPlayer(Player const* player) const;

    void AddAllowedLooter(Player const* player);
    const AllowedLooterSet & GetAllowedLooters() const { return allowedGUIDs; }

    void SetPersonalLooter(Player const* p_Player);
    bool IsPersonalLootFor(Player const* p_Player) const;
};

struct QuestItem
{
    uint8   index;                                          // position in quest_items;
    bool    is_looted;

    QuestItem()
        : index(0), is_looted(false) {}

    QuestItem(uint8 _index, bool _islooted = false)
        : index(_index), is_looted(_islooted) {}
};

class LootTemplate;

typedef std::vector<QuestItem> QuestItemList;
typedef std::vector<LootItem> LootItemList;
typedef std::map<uint32, QuestItemList*> QuestItemMap;
typedef std::vector<LootStoreItem> LootStoreItemList;
typedef UNORDERED_MAP<uint32, LootTemplate*> LootTemplateMap;

typedef std::set<uint32> LootIdSet;

class LootStore
{
    public:
        explicit LootStore(char const* name, char const* entryName, bool ratesAllowed)
            : m_name(name), m_entryName(entryName), m_ratesAllowed(ratesAllowed) {}

        virtual ~LootStore() { Clear(); }

        void Verify() const;

        uint32 LoadAndCollectLootIds(LootIdSet& ids_set);
        void CheckLootRefs(LootIdSet* ref_set = NULL) const; // check existence reference and remove it from ref_set
        void ReportUnusedIds(LootIdSet const& ids_set) const;
        void ReportNotExistedId(uint32 id) const;

        bool HaveLootFor(uint32 loot_id) const { return m_LootTemplates.find(loot_id) != m_LootTemplates.end(); }
        bool HaveQuestLootFor(uint32 loot_id) const;
        bool HaveQuestLootForPlayer(uint32 loot_id, Player* player) const;

        LootTemplate const* GetLootFor(uint32 loot_id) const;
        void ResetConditions();
        LootTemplate* GetLootForConditionFill(uint32 loot_id);

        char const* GetName() const { return m_name; }
        char const* GetEntryName() const { return m_entryName; }
        bool IsRatesAllowed() const { return m_ratesAllowed; }
    protected:
        uint32 LoadLootTable();
        void Clear();
    private:
        LootTemplateMap m_LootTemplates;
        char const* m_name;
        char const* m_entryName;
        bool m_ratesAllowed;
};

class LootTemplate
{
    class LootGroup;                                       // A set of loot definitions for items (refs are not allowed inside)
    typedef std::vector<LootGroup> LootGroups;

    public:
        // Adds an entry to the group (at loading stage)
        void AddEntry(LootStoreItem& item);
        // Rolls for every item in the template and adds the rolled items the the loot
        void Process(Loot& loot, bool rate, uint16 lootMode, Player const* p_Player, uint8 groupId = 0) const;
        void CopyConditions(ConditionContainer conditions);
        void FillAutoAssignationLoot(std::list<const ItemTemplate*>& p_ItemList, Player* p_Player = nullptr, bool p_IsBGReward = false) const;

        // True if template includes at least 1 quest drop entry
        bool HasQuestDrop(LootTemplateMap const& store, uint8 groupId = 0) const;
        // True if template includes at least 1 quest drop for an active quest of the player
        bool HasQuestDropForPlayer(LootTemplateMap const& store, Player const* player, uint8 groupId = 0) const;

        // Checks integrity of the template
        void Verify(LootStore const& store, uint32 Id) const;
        void CheckLootRefs(LootTemplateMap const& store, LootIdSet* ref_set) const;
        bool addConditionItem(Condition* cond);
        bool isReference(uint32 id);

    private:
        LootStoreItemList Entries;                          // not grouped only
        LootGroups        Groups;                           // groups have own (optimised) processing, grouped entries go there
};

//=====================================================

class LootValidatorRef :  public Reference<Loot, LootValidatorRef>
{
    public:
        LootValidatorRef() {}
        void targetObjectDestroyLink() {}
        void sourceObjectDestroyLink() {}
};

//=====================================================

class LootValidatorRefManager : public RefManager<Loot, LootValidatorRef>
{
    public:
        typedef LinkedListHead::Iterator< LootValidatorRef > iterator;

        LootValidatorRef* getFirst() { return (LootValidatorRef*)RefManager<Loot, LootValidatorRef>::getFirst(); }
        LootValidatorRef* getLast() { return (LootValidatorRef*)RefManager<Loot, LootValidatorRef>::getLast(); }

        iterator begin() { return iterator(getFirst()); }
        iterator end() { return iterator(NULL); }
        iterator rbegin() { return iterator(getLast()); }
        iterator rend() { return iterator(NULL); }
};

//=====================================================
struct LootView;

ByteBuffer& operator<<(ByteBuffer& b, LootItem const& li);
ByteBuffer& operator<<(ByteBuffer& b, LootView const& lv);

struct LinkedLootInfo
{
    uint64 creatureGUID;
    uint32 slot;
    PermissionTypes permission;
};

struct InstanceLooters
{
    public:

        InstanceLooters() : m_isEnabled(false) {}

        void SetEnabled(bool value) { m_isEnabled = value; }
        bool IsEnabled() const { return m_isEnabled; }

        void ClearGuids() { playerGuids.clear(); }
        void AddPlayerGuid(uint64 guid) { playerGuids.insert(guid); }
        bool HasPlayerGuid(uint64 guid) const { return (playerGuids.find(guid) != playerGuids.end()); }

    private:

        bool m_isEnabled;
        std::set<uint64> playerGuids;
};

struct Loot
{
    friend ByteBuffer& operator<<(ByteBuffer& b, LootView const& lv);

    QuestItemMap const& GetPlayerCurrencies() const { return PlayerCurrencies; }
    QuestItemMap const& GetPlayerQuestItems() const { return PlayerQuestItems; }
    QuestItemMap const& GetPlayerFFAItems() const { return PlayerFFAItems; }
    QuestItemMap const& GetPlayerNonQuestNonFFAConditionalItems() const { return PlayerNonQuestNonFFAConditionalItems; }

    std::map<uint32, LinkedLootInfo> LinkedLoot;
    std::vector<LootItem>            Items;
    std::vector<LootItem>            QuestItems;

    LootType loot_type;                                     ///< required for achievement system
    uint64 source;                                          ///< Source guid of loot (gameobject, creature) 
    LootType Type;                                          ///< required for achievement system
    uint64 RoundRobinPlayer;                                ///< GUID of the player having the Round-Robin ownership for the loot. If 0, round robin owner has released.
    uint32 MaxLinkedSlot;
    uint32 AdditionalLinkedGold;
    uint32 Gold;
    ItemContext Context;                                    ///< Used to find item bonus to apply in dungeon / raid
    uint8 UnlootedCount;
    bool  m_IsAoELoot;

    Loot(uint32 _gold = 0) : Type(LOOT_CORPSE), RoundRobinPlayer(0), MaxLinkedSlot(0), AdditionalLinkedGold(0), Gold(_gold), Context(ItemContext::None), UnlootedCount(0), m_IsAoELoot(false) {}
    ~Loot() { clear(); }

    void SetSource(uint64 p_Source) { source = p_Source; }

    // if loot becomes invalid this reference is used to inform the listener
    void addLootValidatorRef(LootValidatorRef* pLootValidatorRef)
    {
        i_LootValidatorRefManager.insertFirst(pLootValidatorRef);
    }

    // void clear();
    void clear()
    {
        for (QuestItemMap::const_iterator itr = PlayerCurrencies.begin(); itr != PlayerCurrencies.end(); ++itr)
            delete itr->second;
        PlayerCurrencies.clear();

        for (QuestItemMap::const_iterator itr = PlayerQuestItems.begin(); itr != PlayerQuestItems.end(); ++itr)
            delete itr->second;
        PlayerQuestItems.clear();

        for (QuestItemMap::const_iterator itr = PlayerFFAItems.begin(); itr != PlayerFFAItems.end(); ++itr)
            delete itr->second;
        PlayerFFAItems.clear();

        for (QuestItemMap::const_iterator itr = PlayerNonQuestNonFFAConditionalItems.begin(); itr != PlayerNonQuestNonFFAConditionalItems.end(); ++itr)
            delete itr->second;
        PlayerNonQuestNonFFAConditionalItems.clear();

        PlayersLooting.clear();
        Items.clear();
        QuestItems.clear();
        Gold = 0;
        UnlootedCount = 0;
        RoundRobinPlayer = 0;
        AdditionalLinkedGold = 0;
        i_LootValidatorRefManager.clearReferences();
        m_IsAoELoot = false;
    }

    void addLinkedLoot(uint32 slot, uint64 linkedCreature, uint32 linkedSlot, PermissionTypes perm)
    {
        LinkedLoot[slot].creatureGUID = linkedCreature;
        LinkedLoot[slot].slot = linkedSlot;
        LinkedLoot[slot].permission = perm;

        if (MaxLinkedSlot < slot)
            MaxLinkedSlot = slot;
    }

    bool isLinkedLoot(uint32 slot)
    {
        if (LinkedLoot.find(slot) != LinkedLoot.end())
            return true;

        return false;
    }

    // Must used only AFTER isLinkedLoot check
    LinkedLootInfo& getLinkedLoot(uint32 slot)
    {
        return LinkedLoot[slot];
    }

    bool empty() const { return Items.empty() && Gold == 0; }
    bool isLooted() const { return Gold == 0 && UnlootedCount == 0; }

    void NotifyItemRemoved(uint8 lootIndex, uint64 p_PersonalLooter = 0);
    void NotifyQuestItemRemoved(uint8 questIndex);
    void NotifyMoneyRemoved();
    void AddLooter(uint64 GUID) { PlayersLooting.insert(GUID); }
    void RemoveLooter(uint64 GUID) { PlayersLooting.erase(GUID); }
    bool IsLooter(uint64 GUID) { return PlayersLooting.find(GUID) != PlayersLooting.end(); }

    void generateMoneyLoot(uint32 minAmount, uint32 maxAmount);
    bool FillLoot(uint32 lootId, LootStore const& store, Player* lootOwner, bool personal, bool noEmptyError = false, uint16 lootMode = LOOT_MODE_DEFAULT);
    void FillCurrencyLoot(Player* player);
    void FillFFALoot(Player* player);
    void FillNotNormalLootFor(Player* player, bool presentAtLooting);
    void FillQuestLoot(Player* player);
    void FillNonQuestNonFFAConditionalLoot(Player* player, bool presentAtLooting);

    // Inserts the item into the loot (called by LootTemplate processors)
    void AddItem(LootStoreItem const & item);

    LootItem* LootItemInSlot(uint32 lootslot, Player* player, QuestItem** qitem = NULL, QuestItem** ffaitem = NULL, QuestItem** conditem = NULL, QuestItem* *currency = NULL);
    uint32 GetMaxSlotInLootFor(Player* player) const;
    bool hasItemFor(Player* player) const;
    bool hasOverThresholdItem() const;

    // there are players that killed the mob (instance only)
    InstanceLooters AllowedPlayers;

    private:
        std::set<uint64> PlayersLooting;
        QuestItemMap PlayerCurrencies;
        QuestItemMap PlayerQuestItems;
        QuestItemMap PlayerFFAItems;
        QuestItemMap PlayerNonQuestNonFFAConditionalItems;

        // All rolls are registered here. They need to know, when the loot is not valid anymore
        LootValidatorRefManager i_LootValidatorRefManager;
};

struct LootView
{
    Loot &loot;
    Player* viewer;
    uint8 _loot_type;
    ObjectGuid _guid;
    PermissionTypes permission;
    LootView(Loot &_loot, Player* _viewer, uint8 loot_type, uint64 guid, PermissionTypes _permission = ALL_PERMISSION)
        : loot(_loot), viewer(_viewer), _loot_type(loot_type), _guid(ObjectGuid(guid)), permission(_permission) {}
};

extern LootStore LootTemplates_Creature;
extern LootStore LootTemplates_Fishing;
extern LootStore LootTemplates_Gameobject;
extern LootStore LootTemplates_Item;
extern LootStore LootTemplates_Mail;
extern LootStore LootTemplates_Milling;
extern LootStore LootTemplates_Pickpocketing;
extern LootStore LootTemplates_Reference;
extern LootStore LootTemplates_Skinning;
extern LootStore LootTemplates_Disenchant;
extern LootStore LootTemplates_Prospecting;
extern LootStore LootTemplates_Spell;

void LoadLootTemplates_Creature();
void LoadLootTemplates_Fishing();
void LoadLootTemplates_Gameobject();
void LoadLootTemplates_Item();
void LoadLootTemplates_Mail();
void LoadLootTemplates_Milling();
void LoadLootTemplates_Pickpocketing();
void LoadLootTemplates_Skinning();
void LoadLootTemplates_Disenchant();
void LoadLootTemplates_Prospecting();

void LoadLootTemplates_Spell();
void LoadLootTemplates_Reference();

inline void LoadLootTables()
{
    LoadLootTemplates_Creature();
    LoadLootTemplates_Fishing();
    LoadLootTemplates_Gameobject();
    LoadLootTemplates_Item();
    LoadLootTemplates_Mail();
    LoadLootTemplates_Milling();
    LoadLootTemplates_Pickpocketing();
    LoadLootTemplates_Skinning();
    LoadLootTemplates_Disenchant();
    LoadLootTemplates_Prospecting();
    LoadLootTemplates_Spell();

    LoadLootTemplates_Reference();
}

#endif