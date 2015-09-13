////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonGO.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison 
{
    /// Constructor
    go_garrison_cache::go_garrison_cache()
        : GameObjectScript("go_garrison_cache")
    { 

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_GameObject : Target GameObject instance
    bool go_garrison_cache::OnGossipHello(Player * p_Player, GameObject * p_GameObject)
    {
        if (p_Player->GetGarrison())
        {
            p_Player->GetGarrison()->RewardGarrisonCache();

            /// Alliance
            if (p_Player->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER) && p_GameObject)
                p_Player->QuestObjectiveSatisfy(41327, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_GameObject->GetGUID());

            /// Horde
            if (p_Player->HasQuest(Quests::QUEST_WHAT_WE_GOT) && p_GameObject)
            {
                p_Player->QuestObjectiveSatisfy(41325, 2, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, p_GameObject->GetGUID());

                Creature * l_Creature = p_Player->FindNearestCreature(NPCs::NPC_LADY_SENA, 15);

                if (l_Creature)
                    l_Creature->AI()->Talk(0);
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    go_garrison_outhouse::go_garrison_outhouse()
        : GameObjectScript("go_garrison_outhouse")
    { 

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_GameObject : Target GameObject instance
    bool go_garrison_outhouse::OnGossipHello(Player * p_Player, GameObject * p_GameObject)
    {
        p_Player->CastSpell(p_Player, MS::Garrison::Spells::SPELL_RELIEVED);

        if (!p_Player->GetAchievementMgr().HasAchieved(Achievements::ACHIEVEMENT_STAYING_REGULAR))
            p_Player->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(Achievements::ACHIEVEMENT_STAYING_REGULAR), nullptr);

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    go_garrison_shipment_container::go_garrison_shipment_container()
        : GameObjectScript("go_garrison_shipment_container")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a GameObjectAI object is needed for the GameObject.
    /// @p_GameObject : GameObject instance
    GameObjectAI* go_garrison_shipment_container::GetAI(GameObject * p_GameObject) const
    {
        return new go_garrison_shipment_containerAI(p_GameObject);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    go_garrison_shipment_container::go_garrison_shipment_containerAI::go_garrison_shipment_containerAI(GameObject * p_GameObject)
        : GameObjectAI(p_GameObject)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    bool go_garrison_shipment_container::go_garrison_shipment_containerAI::GossipHello(Player * p_Player)
    {
        if (!p_Player || !p_Player->GetGarrison())
            return false;

        Garrison::Manager            * l_Garrison   = p_Player->GetGarrison();
        std::vector<GarrisonWorkOrder> l_WorkOrders = l_Garrison->GetWorkOrders();

        uint32 l_ThisGobPlotInstanceID = l_Garrison->GetGameObjectPlotInstanceID(go->GetGUID());

        if (!l_ThisGobPlotInstanceID)
            return false;

        std::map<uint32, bool> l_ToastStatus;
        for (uint32 l_I = 0; l_I < l_WorkOrders.size(); ++l_I)
        {
            if (l_WorkOrders[l_I].PlotInstanceID != l_ThisGobPlotInstanceID)
                continue;

            if (l_WorkOrders[l_I].CompleteTime > time(nullptr))
                continue;

            CharShipmentEntry const* l_ShipmentEntry = sCharShipmentStore.LookupEntry(l_WorkOrders[l_I].ShipmentID);

            if (!l_ShipmentEntry)
                continue;

            uint32 l_RewardItemID = l_ShipmentEntry->ResultItemID;

            /// Adding items
            uint32 l_NoSpaceForCount = 0;

            /// check space and find places
            ItemPosCountVec l_Destination;
            InventoryResult l_Message = p_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, l_RewardItemID, 1, &l_NoSpaceForCount);

            if (l_Message == EQUIP_ERR_OK)
            {
                p_Player->StoreNewItem(l_Destination, l_RewardItemID, true, Item::GenerateItemRandomPropertyId(l_RewardItemID));

                if (l_ToastStatus[l_RewardItemID] == false)
                {
                    p_Player->SendDisplayToast(l_RewardItemID, 1, DISPLAY_TOAST_METHOD_LOOT, TOAST_TYPE_NEW_ITEM, false, false);
                    l_ToastStatus[l_RewardItemID] = true;
                }

                l_Garrison->DeleteWorkOrder(l_WorkOrders[l_I].DatabaseID);
            }
            else
                p_Player->SendEquipError(l_Message, nullptr, nullptr, l_RewardItemID);
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    go_garrison_herb::go_garrison_herb()
        : GameObjectScript("go_garrison_herb")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_GameObject : Target GameObject instance
    bool go_garrison_herb::OnGossipHello(Player* p_Player, GameObject* p_GameObject)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)p_GameObject->GetInstanceScript();

        if (!l_GarrisonSite)
            return true;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (l_Owner != p_Player)
            return true;

        float l_BaseX = p_GameObject->GetPositionX();
        float l_BaseY = p_GameObject->GetPositionY();

        uint64 l_PackedValue = 0;
        l_PackedValue |= ((uint64)*(uint32*)(&l_BaseX)) << 0;
        l_PackedValue |= ((uint64)*(uint32*)(&l_BaseY)) << 32;

        std::vector<uint64> l_Creatures = l_Owner->GetGarrison()->GetBuildingCreaturesByBuildingType(BuildingType::Farm);

        for (uint64 l_CreatureGUID : l_Creatures)
        {
            Creature* l_Creature = HashMapHolder<Creature>::Find(l_CreatureGUID);

            if (l_Creature)
                l_Creature->AI()->SetGUID(l_PackedValue, CreatureAIDataIDs::GatheredPos);
        }

        return false;
    }

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_GO()
{
    new MS::Garrison::go_garrison_cache;
    new MS::Garrison::go_garrison_outhouse;
    new MS::Garrison::go_garrison_shipment_container;
    new MS::Garrison::go_garrison_herb;
}