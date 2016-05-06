////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_STABLES_ALLIANCE_HPP_GARRISON
#define GARRISON_STABLES_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    static const uint32 g_FannyQuestgiverEntry  = 86974;
    static const uint32 g_KeeganQuestgiverEntry = 86973;

    //////////////////////////////////////////////////////////////////////////
    /// 86974 - Fanny Firebeard                                            ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_FannyFirebeard : public CreatureScript
    {
        public:
            /// Constructor
            npc_FannyFirebeard();

            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            bool CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList);

            void ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_FannyFirebeardAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_FannyFirebeardAI(Creature* p_Creature);

            std::vector<uint32> m_SummonsEntries;

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnPlotInstanceUnload() override;
    };

    //////////////////////////////////////////////////////////////////////////
    /// 86973 - Keegan Firebeard                                           ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_KeeganFirebeard : public CreatureScript
    {
        public:
            /// Constructor
            npc_KeeganFirebeard();

            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            bool CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList);

            void ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID);
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_STABLES_ALLIANCE_HPP_GARRISON