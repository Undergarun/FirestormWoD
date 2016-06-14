////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GOBLIN_WORKSHOP_HORDE_HPP_GARRISON
#define GARRISON_GOBLIN_WORKSHOP_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 85369 - Vee                                                        ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_VeeData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_Vee_Garrison : public CreatureScript
    {
        public:
            /// Constructor
            npc_Vee_Garrison();

            uint32 m_GobEntry = 0;

            /// Called when a player rewards the quest.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target creature instance
            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            /// Called when a player accepts the quest.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target creature instance
            virtual bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_Vee_GarrisonAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_Vee_GarrisonAI(Creature* p_Creature);

            uint64 m_OwnerGUID = 0;

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnDailyDataReset() override;

            virtual void SetGUID(uint64 p_Guid, int32 p_Id) override;
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GOBLIN_WORKSHOP_HORDE_HPP_GARRISON
