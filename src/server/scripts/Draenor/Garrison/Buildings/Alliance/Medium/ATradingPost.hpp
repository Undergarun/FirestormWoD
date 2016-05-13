////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_TRADING_POST_ALLIANCE_HPP_GARRISON
#define GARRISON_TRADING_POST_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 87212 - Trader Joseph                                              ///
    //////////////////////////////////////////////////////////////////////////
    class npc_TraderJoseph : public CreatureScript
    {
        public:
            /// Constructor
            npc_TraderJoseph();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_TraderJosephAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_TraderJosephAI(Creature* p_Creature);

                virtual void OnPlotInstanceUnload() override;

                /// When the PlotInstance ID is set
                /// @p_BuildingID : Set plot instance ID
                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

                virtual void OnDailyDataReset() override;
            };
    };

    /// Handle Reputation Restrictions
    class playerScript_Garrison_TradingPost : public PlayerScript
    {
        public:
            playerScript_Garrison_TradingPost() : PlayerScript("playerScript_Garrison_TradingPost") { }

            void OnReputationChange(Player* p_Player, uint32 p_FactionID, int32& p_Standing, bool p_Incremential) override;

            bool CheckRequirements(Player* p_Player);
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_TRADING_POST_ALLIANCE_HPP_GARRISON