////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_FISHING_HUT_ALLIANCE_HPP_GARRISON
#define GARRISON_FISHING_HUT_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

#include "FishingHut_Level1Data.hpp"

namespace MS { namespace Garrison 
{


    template<const char * t_ScriptName, std::function<void(GarrisonNPCAI*, Creature*)> * t_SetupLevel1, std::function<void(GarrisonNPCAI*, Creature*)> * t_SetupLevel2, std::function<void(GarrisonNPCAI*, Creature*)> * t_SetupLevel3>
    class SimpleSequenceCosmeticScript : public CreatureScript
    {
        public:
            /// Constructor
            SimpleSequenceCosmeticScript()
                : CreatureScript(t_ScriptName)
            {

            }

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const
            {
                return new SimpleSequenceCosmeticScriptAI(p_Creature);
            }

            /// Creature AI
            struct SimpleSequenceCosmeticScriptAI : public GarrisonNPCAI
            {
                /// Constructor
                SimpleSequenceCosmeticScriptAI(Creature * p_Creature)
                    : GarrisonNPCAI(p_Creature)
                {
                    SetAIObstacleManagerEnabled(true);
                }


                /// When the building ID is set
                /// @p_BuildingID : Set building ID
                virtual void OnSetBuildingID(uint32 p_BuildingID) override
                {
                    m_OnPointReached.clear();

                    GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

                    if (!l_BuildingEntry)
                        return;

                    switch (l_BuildingEntry->BuildingLevel)
                    {
                        case 1:
                            (*t_SetupLevel1)(this, me);
                            break;

                        case 2:
                            (*t_SetupLevel2)(this, me);
                            break;

                        case 3:
                            (*t_SetupLevel3)(this, me);
                            break;

                        default:
                            break;
                    }
                }

            };
    };





    namespace npc_TharisStrongcastData
    {
        extern std::function<void(GarrisonNPCAI*, Creature*)> FnLevel1;
        extern std::function<void(GarrisonNPCAI*, Creature*)> FnLevel2;
        extern std::function<void(GarrisonNPCAI*, Creature*)> FnLevel3;

        extern char gScriptName[];
    }

    using npc_TharisStrongcast = SimpleSequenceCosmeticScript<npc_TharisStrongcastData::gScriptName, &npc_TharisStrongcastData::FnLevel1, &npc_TharisStrongcastData::FnLevel2, &npc_TharisStrongcastData::FnLevel3>;

    /// Tharis Strongcast
//     class npc_TharisStrongcast : public CreatureScript
//     {
//         public:
//             /// Constructor
//             npc_TharisStrongcast();
// 
//             /// Called when a CreatureAI object is needed for the creature.
//             /// @p_Creature : Target creature instance
//             CreatureAI * GetAI(Creature * p_Creature) const;
// 
//             /// Creature AI
//             struct npc_TharisStrongcastAI : public GarrisonNPCAI
//             {
//                 /// Constructor
//                 npc_TharisStrongcastAI(Creature * p_Creature);
// 
//                 /// When the building ID is set
//                 /// @p_BuildingID : Set building ID
//                 virtual void OnSetBuildingID(uint32 p_BuildingID) override;
// 
//                 /// Setup AI for level 1
//                 void SetupLevel1();
//                 /// Setup AI for level 2
//                 void SetupLevel3();
//                 /// Setup AI for level 3
//                 void SetupLevel2();
// 
//             };
// 
//     };

    /// Segumi
    class npc_Segumi : public CreatureScript
    {
        public:
            /// Constructor
            npc_Segumi();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_SegumiAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_SegumiAI(Creature * p_Creature);

                /// When the building ID is set
                /// @p_BuildingID : Set building ID
                virtual void OnSetBuildingID(uint32 p_BuildingID) override;

                /// Setup AI for level 1
                void SetupLevel1();
                /// Setup AI for level 2
                void SetupLevel3();
                /// Setup AI for level 3
                void SetupLevel2();

            };

    };
    
    /// Ron ashton
    class npc_RonAshton : public CreatureScript
    {
        public:
            /// Constructor
            npc_RonAshton();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_RonAshtonAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_RonAshtonAI(Creature * p_Creature);

                /// When the building ID is set
                /// @p_BuildingID : Set building ID
                virtual void OnSetBuildingID(uint32 p_BuildingID) override;

                /// Setup AI for level 1
                void SetupLevel1();
                /// Setup AI for level 2
                void SetupLevel3();
                /// Setup AI for level 3
                void SetupLevel2();

            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FISHING_HUT_ALLIANCE_HPP_GARRISON