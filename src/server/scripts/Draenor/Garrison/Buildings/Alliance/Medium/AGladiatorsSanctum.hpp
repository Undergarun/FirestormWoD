////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_GLADIATORS_SANCTUM_ALLIANCE_GARRISON
#define GARRISON_GLADIATORS_SANCTUM_ALLIANCE_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "../../MultiFactionaProfessionlBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 86639 - Altar of Bones                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AltarOfBonesAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    class npc_AltarOfBones : public CreatureScript
    {
        public:
            /// Constructor
            npc_AltarOfBones();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_AltarOfBonesAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_AltarOfBonesAI(Creature* p_Creature);

                /// When the building ID is set
                /// @p_BuildingID : Set building ID
                virtual void OnSetBuildingID(uint32 p_BuildingID) override
                {
                    m_OnPointReached.clear();

                    GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

                    if (!l_BuildingEntry)
                        return;

                    switch (l_BuildingEntry->Level)
                    {
                        case 1:
                            (npc_AltarOfBonesAIData::FnLevel1)(this, me);
                            break;

                        case 2:
                            (npc_AltarOfBonesAIData::FnLevel2)(this, me);
                            break;

                        case 3:
                            (npc_AltarOfBonesAIData::FnLevel3)(this, me);
                            break;

                        default:
                            break;
                    }
                }
            };

    };
    //////////////////////////////////////////////////////////////////////////
    /// 86677 - Kuros                                                      ///
    //////////////////////////////////////////////////////////////////////////

    class npc_Kuros_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_Kuros_Garr();

            bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_Kuros_GarrAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_Kuros_GarrAI(Creature* p_Creature);
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GLADIATORS_SANCTUM_ALLIANCE_GARRISON
#endif
