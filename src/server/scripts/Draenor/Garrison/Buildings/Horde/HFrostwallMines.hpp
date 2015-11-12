////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_FROSTWALL_MINES_HORDE_HPP_GARRISON
#define GARRISON_FROSTWALL_MINES_HORDE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    static const SequencePosition g_GorenHatchlingPositions[] =
    {
        { 3.911870f, -30.7051f, -30.9979f, 1.18066f },
        { 2.467050f, -36.6031f, -30.6423f, 1.18066f },
        { -1.88602f, -29.2526f, -30.6014f, 1.18066f },
        { 44.32350f, -43.5130f, -28.5026f, 4.61736f },
        { 47.58300f, -52.2979f, 31.58170f, 5.66530f },
        { 50.62890f, -49.2966f, -27.9263f, 4.61736f },
        { 44.66070f, -49.5836f, -28.1074f, 4.61736f },
        { 44.55930f, -84.9490f, -21.5312f, 5.66530f },
        { 41.83970f, -90.2661f, -19.7810f, 5.66530f },
        { 19.79770f, -109.887f, -3.70430f, 5.66530f },
        { 8.074030f, -109.751f, -0.29330f, 5.66530f },
        { -10.6161f, -85.3503f, 16.49370f, 5.66530f },
        { -3.00287f, -75.4234f, 21.77770f, 5.66530f },
        { 29.48580f, -64.5315f, 30.97370f, 5.66530f },
        { 47.58330f, -52.2983f, 31.08170f, 5.66530f },
        { 47.82500f, -76.8389f, 30.20970f, 5.66530f },
        { 54.77110f, -55.8200f, 30.39070f, 5.82629f }
    };

    /// Frostwall Goren Position
    static const SequencePosition g_GorenPositions[] =
    {
        { 47.8288f, -44.6977f, -28.049f, 1.68950f },
        { 39.2111f, -84.5936f, -21.295f, 1.45399f },
        { 21.8813f, -108.017f, -3.5393f, 3.08481f },
        { 15.5437f, -105.079f, -2.9113f, 0.78255f },
        { 4.30848f, -107.155f, 1.95270f, 3.07154f },
        { -10.909f, -77.7452f, 19.3737f, 3.56891f },
        { -1.9439f, -78.6121f, 21.8927f, 0.32559f },
        { 28.0191f, -62.4507f, 31.3217f, 2.55127f },
        { 29.7700f, -63.3582f, 31.1827f, 5.89228f },
        { 29.7700f, -63.3582f, 31.1827f, 5.89228f },
        { 38.6124f, -70.0625f, 30.3677f, 2.80451f },
        { 44.0328f, -68.1909f, 30.2497f, 6.09400f },
        { 49.0390f, -70.8915f, 29.6147f, 5.11862f },
        { 40.5404f, -85.6739f, 31.6447f, 6.17879f },
        { 28.7424f, -79.5823f, 30.6327f, 4.26206f },
        { 38.2381f, -45.2519f, 32.0107f, 6.09400f },
        { 55.9101f, -57.1221f, 30.1917f, 6.09400f },
        { -0.8578f, -33.2827f, -30.762f, 3.46918f },
        { 32.1305f, -44.8074f, -28.984f, 5.84822f }
    };

    static const SequencePosition g_MinersPositions[] =
    {
        { 11.1349f, 13.55580f, -34.9398f, 5.93412f },
        { -0.9937f, 8.681110f, -33.3508f, 5.93412f },
        { 6.28446f, -37.8387f, -30.8215f, 5.93412f },
        { -6.2846f, -23.7491f, -30.6559f, 5.93412f },
        { 7.70396f, -13.5887f, -29.9909f, 5.93412f },
        { 18.7029f, -29.3607f, -30.9258f, 5.93412f },
        { 30.6946f, -48.5749f, -28.1097f, 5.93412f },
        { 56.0952f, -50.0695f, -27.5936f, 5.93412f },
        { 55.2605f, -83.6182f, -22.6694f, 5.93412f },
        { 41.0277f, -83.3388f, -21.6817f, 5.93412f },
        { 10.6270f, -104.148f, -0.69441f, 5.93412f },
        { -16.515f, -107.353f, 11.42600f, 5.93412f },
        { -8.1533f, -92.1833f, 14.05050f, 5.93412f },
        { -8.8705f, -74.1551f, 20.77180f, 5.93412f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 81688 - Gorsol                                                     ///
    //////////////////////////////////////////////////////////////////////////
    class npc_Gorsol : public CreatureScript
    {
        public:
            /// Constructor
            npc_Gorsol();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_GorsolAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_GorsolAI(Creature* p_Creature);

                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
            };
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FROSTWALL_MINES_HORDE_HPP_GARRISON