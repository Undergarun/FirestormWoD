////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_FROSTWALL_MINES_HORDE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_FROSTWALL_MINES_HORDE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "../GatheringBuilding.hpp"
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

    static const SequencePosition g_MineLeftSideDeposits[] =
    {
        { -21.2715f,  -13.8611f, -59.2858f, 2.9218f },
        { 12.1809f,    24.8398f, -38.5510f, 4.4419f },
        { 9.6880f,     16.0787f, -36.2726f, 5.7378f },
        { 17.4994f,    15.7274f, -36.9667f, 1.9954f },
        { 22.5201f,    21.6839f, -38.5308f, 1.7127f },
        { 28.2479f,    26.9864f, -39.6607f, 1.7048f },
        { 39.2003f,    27.8017f, -42.6060f, 5.3648f },
        { 38.9916f,    18.4105f, -43.6418f, 4.1906f },
        { 33.5117f,    10.8657f, -42.5858f, 3.9079f },
        { 24.7082f,     7.4141f, -39.7118f, 4.0846f },
        { 20.5722f,     5.1232f, -38.0943f, 4.4246f },
        { 13.0901f,     4.3822f, -34.8394f, 3.4390f },
        { 8.9483f,     -1.6505f, -32.8574f, 5.3082f },
        { -4.8052f,    -8.5462f, -55.9181f, 2.9088f },
        { -12.2454f,   -4.4723f, -55.4827f, 3.1075f },
        { -19.4561f,   -3.1426f, -56.0133f, 4.1560f },
        { -30.3754f,   -2.0047f, -55.6653f, 4.4898f },
        { 28.3093f,   -36.2669f, -29.4803f, 0.6296f },
        { 36.7683f,   -34.9415f, -29.8132f, 1.2202f },
        { 44.8613f,   -31.3961f, -29.9007f, 6.1014f },
        { 49.2182f,   -44.6013f, -28.6273f, 4.9084f },
        { 59.0344f,   -57.2235f, -27.6171f, 5.6946f },
        { 59.9181f,   -63.6607f, -25.8761f, 4.9799f },
        { 60.4903f,   -72.1911f, -25.4847f, 5.2626f },
        { 59.4616f,   -79.9830f, -24.2250f, 4.2377f },
        { 54.8163f,   -86.7227f, -22.4267f, 4.8856f },
        { 22.2959f,  -117.3376f,  -3.6874f, 3.6997f },
        { 15.8191f,  -112.7528f,  -3.6108f, 3.4601f },
        { 8.4303f,   -113.2307f,  -0.5374f, 3.3502f },
        { -2.0537f,  -112.4389f,   4.5311f, 3.3580f },
        { -9.5442f,  -109.5809f,   8.9928f, 3.4130f },
        { -14.9442f, -108.5923f,  10.8239f, 4.2102f },
        { -17.3592f,  -98.1804f,  11.1821f, 1.7362f },
        { -17.6374f,  -89.5104f,  13.2508f, 1.3081f },
        { -15.7798f,  -82.9398f,  16.0685f, 1.2571f },
        { -11.6099f,  -77.1702f,  18.9313f, 1.3427f },
        { -5.5295f,   -70.6920f,  21.5120f, 0.7772f },
        { 2.5470f,    -70.1050f,  24.9173f, 0.4748f },
        { 12.3125f,   -69.5385f,  29.1250f, 0.1599f }
    };

    static const SequencePosition g_MineRightSideDeposits[] =
    {
        { 9.4030f,   -79.2071f,    27.6849f, 3.7272f },
        { 0.3343f,   -84.5890f,    20.9980f, 4.1160f },
        { -6.1773f,  -89.2095f,    15.7820f, 6.0481f },
        { -8.3468f,  -95.0270f,    11.9934f, 0.1183f },
        { -4.6753f,  -99.9309f,     8.3208f, 1.5242f },
        { 0.4708f,  -102.9879f,     4.3263f, 1.4456f },
        { 5.5319f,  -103.7457f,     1.8457f, 1.5045f },
        { 9.0834f,  -104.3205f,    -0.3793f, 1.0294f },
        { 15.1476f, -103.6681f,    -3.1618f, 1.8658f },
        { 21.4066f, -103.9364f,    -3.9799f, 1.2886f },
        { 23.3676f, -104.8962f,   -20.5796f, 2.9143f },
        { 38.3432f, -119.9020f,   -21.0272f, 5.6483f },
        { 43.6404f, -112.5980f,   -20.7993f, 2.6363f },
        { 32.1395f,  -89.8483f,   -20.5642f, 1.7959f },
        { 37.7903f,  -85.0931f,   -21.6949f, 1.3757f },
        { 43.0758f,  -75.4841f,   -24.3843f, 2.3300f },
        { 46.1104f,  -66.4390f,   -27.1028f, 2.1415f },
        { 40.3559f,  -57.4901f,   -27.5191f, 3.6141f },
        { 36.7049f,  -52.2187f,   -28.4205f, 2.8484f },
        { 28.5692f,  -47.1683f,   -28.9980f, 3.7987f },
        { 22.7923f,  -45.5326f,   -29.3045f, 3.4688f },
        { -32.0448f,   9.9907f,   -56.0018f, 1.2854f },
        { -21.6306f,  16.5841f,   -55.6365f, 1.3522f },
        { -12.6015f,  16.3066f,   -55.9601f, 0.5550f },
        { -6.1370f,   19.7570f,   -55.9856f, 0.3272f },
        { 2.8862f,    15.6458f,   -55.1730f, 5.8211f },
        { 8.5669f,    11.3826f,   -55.3300f, 0.0798f },
        { 6.7312f,     1.1474f,   -55.7901f, 0.0445f },
        { -14.2025f,   3.3835f,   -31.1417f, 2.2742f },
        { -5.3802f,    5.2938f,   -32.1254f, 0.8919f },
        { -0.1000f,   15.0299f,   -34.7130f, 2.7258f },
        { 0.5284f,    22.3085f,   -36.3094f, 1.3435f },
        { -23.7055f,  -0.7925f,   -56.8947f, 0.1179f },
        { -16.2427f,  -3.1806f,   -59.2778f, 6.2401f },
        { -12.5898f, -13.8761f,   -61.5426f, 4.7399f }
    };

    /// Herbs game object entries
    extern std::vector<uint32> g_DepositsEntry;

    /// FarmSimulator/GoatSimulator positions
    extern std::vector<GatheringPlotInfos> g_MineDeposits;

    class npc_GorsolAI : public GatheringBuildingMaster<&g_MineDeposits>
    {
        public:
            /// Constructor
            npc_GorsolAI(Creature* p_Creature);

            virtual void OnPlotInstanceUnload() override;

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            /// Select game object entry for a fresh gathering spawn
            /// @p_MiscData : Misc data
            virtual uint32 SelectGameObjectEntryForGatheringSpawn(uint32 p_MiscData) override;

            virtual void sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 p_Option) override;
    };

    //////////////////////////////////////////////////////////////////////////
    /// 81688 - Gorsol                                                     ///
    //////////////////////////////////////////////////////////////////////////
    class npc_Gorsol : public CreatureScript
    {
        public:
            /// Constructor
            npc_Gorsol();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature * p_Creature) const override;

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

            static std::vector<SequencePosition> CalculateDepositsPositions();

            static void SummonDeposits(std::vector<SequencePosition> p_SpawnPos, GarrisonNPCAI* l_AI);
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FROSTWALL_MINES_HORDE_HPP_GARRISON
#endif
