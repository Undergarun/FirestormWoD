////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_BARN_ALLIANCE_HPP_GARRISON
#define GARRISON_BARN_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace HomerStonefield_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Start       = 100,
                Way1        = 101,
                Way2        = 102,
                Way3        = 103,
                Way4        = 104,
                SecondPause = 105
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Start,
            MovePointIDs::Way1,
            MovePointIDs::Way2,
            MovePointIDs::Way3,
            MovePointIDs::Way4,        ///< Emote 8 secs
            MovePointIDs::Way3,
            MovePointIDs::Way2,
            MovePointIDs::Way1,
            MovePointIDs::SecondPause, ///< Emote 8 secs
            MovePointIDs::Way1
        };

        static SequencePosition MovePointLoc[] =
        {
            { 10.0619f,   2.0853f,  0.3176f, 1.6661f },   ///< MovePointIDs::Start
            { 6.3820f,   -0.2748f,  0.3174f, 4.6415f },   ///< MovePointIDs::Way1
            { 5.6731f,  -13.3014f,  0.0005f, 4.6769f },   ///< MovePointIDs::Way2
            { -2.0149f, -12.5370f,  0.0005f, 1.5510f },   ///< MovePointIDs::Way3
            { -2.0733f,   4.1613f,  0.0011f, 6.2646f },   ///< MovePointIDs::Way4
            { 4.0922f,   -3.7361f,  0.3176f, 4.7252f },   ///< MovePointIDs::SecondPause
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 85048 - Farmer Lok'Lub                                             ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_HomerStonefieldAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_HomerStonefield_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_HomerStonefield_Garr();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target creature instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when player asks from a creature gossip to create a shipment.
            /// @p_Player       : Player that created the shipment
            /// @p_Creature     : NPC that got the gossip
            virtual void OnShipmentCreated(Player* p_Player, Creature* p_Creature, uint32 p_BuildingID) override;

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            virtual bool HandleGossipActions(Player* p_Player, Creature* p_Creature, uint32 p_QuestID, uint32 p_Action);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_HomerStonefieldAI : public SimpleSequenceCosmeticScriptAI<&npc_HomerStonefieldAIData::FnLevel1, &npc_HomerStonefieldAIData::FnLevel2, &npc_HomerStonefieldAIData::FnLevel3>
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_HomerStonefieldAI(Creature* p_Creature);

            uint8 m_ProductionChosen = 0;

            virtual void SetData(uint32 p_ID, uint32 p_Value) override;

            /// Called when at HandleGarrisonGetShipmentInfoOpcode() is received
            virtual int OnShipmentIDRequest(Player* p_Player) override;
    };

    //////////////////////////////////////////////////////////////////////////
    /// 83967 - Homer Stonefield (escort)                                  ///
    //////////////////////////////////////////////////////////////////////////

    class npc_HomerStonefield_Garr_Trap : public CreatureScript
    {
        public:
            /// Constructor
            npc_HomerStonefield_Garr_Trap();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_HomerStonefield_Garr_TrapAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_HomerStonefield_Garr_TrapAI(Creature* p_Creature);
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BARN_ALLIANCE_HPP_GARRISON