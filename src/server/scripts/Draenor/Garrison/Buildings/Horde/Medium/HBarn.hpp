////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_BARN_HORDE_HPP_GARRISON
#define GARRISON_BARN_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 85048 - Farmer Lok'Lub                                             ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_FarmerLokLubAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_FarmerLokLub : public CreatureScript
    {
        public:
            /// Constructor
            npc_FarmerLokLub();

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

    class npc_FarmerLokLubAI : public SimpleSequenceCosmeticScriptAI<&npc_FarmerLokLubAIData::FnLevel1, &npc_FarmerLokLubAIData::FnLevel2, &npc_FarmerLokLubAIData::FnLevel3>
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_FarmerLokLubAI(Creature* p_Creature);

            uint8 m_ProductionChosen = 0;

            virtual void SetData(uint32 p_ID, uint32 p_Value) override;

            /// Called when at HandleGarrisonGetShipmentInfoOpcode() is received
            virtual int OnShipmentIDRequest(Player* p_Player) override;
    };

    //////////////////////////////////////////////////////////////////////////
    /// 83925 - Iron Trap                                                  ///
    //////////////////////////////////////////////////////////////////////////

    class npc_IronTrap : public CreatureScript
    {
        public:
            /// Constructor
            npc_IronTrap();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_IronTrapAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_IronTrapAI(Creature* p_Creature);

            uint64 m_SummonerGuid;
            bool m_FoundEntry;

            /// Called when the creature is summoned
            /// @p_Summoner : Unit that summoned the creature
            virtual void IsSummonedBy(Unit* p_Summoner) override;

            /// Called when any unit movement is detected near of the creature
            /// @p_Who : Unit that moves around the creature
            virtual void MoveInLineOfSight(Unit* p_Who) override;
    };

    //////////////////////////////////////////////////////////////////////////
    /// 85093 - Farmer Lok lub                                             ///
    //////////////////////////////////////////////////////////////////////////

    class npc_FarmerLokLub_Trap : public CreatureScript
    {
        public:
            /// Constructor
            npc_FarmerLokLub_Trap();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_FarmerLokLub_TrapAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_FarmerLokLub_TrapAI(Creature* p_Creature);

            virtual void Reset() override;
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BARN_HORDE_HPP_GARRISON
