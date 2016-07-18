////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_STABLES_HORDE_HPP_GARRISON
#define GARRISON_STABLES_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    static const uint32 g_TormakQuestgiverEntry     = 86979;
    static const uint32 g_SagePalunaQuestgiverEntry = 87242;

    //////////////////////////////////////////////////////////////////////////
    /// 86979 - Tormak                                                     ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_Tormak : public CreatureScript
    {
        public:
            /// Constructor
            npc_Tormak();

            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            virtual bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override;

            bool CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList);

            uint32 ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_TormakAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_TormakAI(Creature* p_Creature);

            std::vector<uint32> m_SummonsEntries;

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnPlotInstanceUnload() override;

            void ProcessSummonPlotCreatures(int l_Index);
    };

    //////////////////////////////////////////////////////////////////////////
    /// 87242 - Sage Paluna                                                ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_SagePaluna : public CreatureScript
    {
        public:
            /// Constructor
            npc_SagePaluna();

            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            bool CheckRewardQuest(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestList);

            uint32 ProceedQuestSelection(Player* p_Player, Creature* p_Creature, std::vector<uint32> p_QuestsList, uint32 p_NextListQuestID, uint32 p_FirstQuestID);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_SagePalunaAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_SagePalunaAI(Creature* p_Creature);
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_STABLES_HORDE_HPP_GARRISON
#endif
