////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_NPC_HPP_GARRISON
#define GARRISON_NPC_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"
#include <map>

#include "ScriptedCosmeticAI.hpp"

namespace MS { namespace Garrison 
{
    /// Sequence position structure
    struct SequencePosition
    {
        /// Position
        float X, Y, Z, O;
    };

    class GarrisonNPCAI : public AI::CosmeticAI
    {
        public:
            /// Constructor
            GarrisonNPCAI(Creature * p_Creature);

            /// Set to relative position from building
            /// @p_X : Relative X
            /// @p_Y : Relative Y
            /// @p_Z : Relative Z
            void MoveBuildingRelative(uint32 p_PointID, float p_X, float p_Y, float p_Z);
            /// Set facing to relative angle from the building
            /// @p_O : Relative angle
            void SetFacingBuildingRelative(float p_O);

            /// Show shipment crafter UI
            void SendShipmentCrafterUI(Player * p_Player);

            /// Get building ID
            uint32 GetBuildingID();

            /// Setup action sequence
            /// @p_CoordTable       : Coordinates table
            /// @p_SequenceTable    : Sequence table
            /// @p_SequenceSize     : Size of sequence table,
            /// @p_FirstMovePointID : First move point ID
            void SetupActionSequence(SequencePosition * p_CoordTable, uint8 * p_SequenceTable, uint32 p_SequenceSize, uint32 p_FirstMovePointID);
            /// Do next sequence element
            virtual void DoNextSequenceAction();

        public:
            /// When the building ID is set
            /// @p_BuildingID : Set building ID
            virtual void OnSetBuildingID(uint32 p_BuildingID);

        public:
            /// Set UInt32 value
            /// @p_ID    : Value ID
            /// @p_Value : Value
            virtual void SetData(uint32 p_ID, uint32 p_Value) override;

        protected:
            GarrisonPlotInstanceInfoLocation const* m_PlotInstanceLocation; ///< This creature plot
            G3D::Vector3 m_NonRotatedPlotPosition;                          ///< Cache for coord transformation
            uint32 m_BuildingID;                                            ///< This creature building ID

        private:
            SequencePosition * m_CoordTable;
            uint8 * m_SequenceTable;
            uint32 m_SequenceSize;
            uint32 m_FirstMovePointID;
            uint8 m_SequencePosition;

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Sequence initializer function
    using InitSequenceFunction = std::function<void(GarrisonNPCAI*, Creature*)>;

    /// Simple sequence cosmetic script, Helper for small cosmetic script
    /// @t_ScriptName  : Script name
    /// @t_SetupLevel1 : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2 : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3 : Function pour initializing sequence for level 3 building
    template<const char * t_ScriptName, InitSequenceFunction * t_SetupLevel1, InitSequenceFunction * t_SetupLevel2, InitSequenceFunction * t_SetupLevel3>
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

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Garrison Ford
    class npc_GarrisonFord : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarrisonFord();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// 87518 - 87519 - 80258
    class npc_CallToArms : public CreatureScript
    {
        public:
            /// Constructor
            npc_CallToArms();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI * GetAI(Creature * p_Creature) const override;

            /// AI Script
            struct npc_CallToArmsAI : public CreatureAI
            {
                /// Constructor
                npc_CallToArmsAI(Creature * p_Creature);

                /// On reset
                virtual void Reset() override;
                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                Player * m_Owner;
                bool m_Ranged;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Garrison ROPE owner
    class npc_GarrisonCartRope : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarrisonCartRope();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI * GetAI(Creature * p_Creature) const override;

            /// AI Script
            struct npc_GarrisonCartRopeAI : public CreatureAI
            {
                /// Constructor
                npc_GarrisonCartRopeAI(Creature * p_Creature);

                /// On reset
                virtual void Reset() override;
                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                bool HasRope;   ///< Has rope
            };

    };

    /// Garrison assistant brightstone
    class npc_AssistantBrightstone : public CreatureScript
    {
        public:
            /// Constructor
            npc_AssistantBrightstone();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Garrison vindicator maraad
    class npc_VindicatorMaraad : public CreatureScript
    {
        public:
            /// Constructor
            npc_VindicatorMaraad();

            /// Called when a player accepts a quest from the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Accepted quest
            virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;
            /// Called when a player completes a quest with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Completed quest
            virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;

    };

    /// Garrison shelly hamby
    class npc_ShellyHamby : public CreatureScript
    {
        public:
            /// Constructor
            npc_ShellyHamby();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Garrison baros alexston
    class npc_BarosAlexsom : public CreatureScript
    {
        public:
            /// Constructor
            npc_BarosAlexsom();

            /// Called when a player accepts a quest from the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Accepted quest
            virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;
            /// Called when a player completes a quest with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Completed quest
            virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;

    };

    /// Lunarfall Laborer
    class npc_LunarfallLaborer : public CreatureScript
    {
        public:
            /// Constructor
            npc_LunarfallLaborer();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_LunarfallLaborerAI : public CreatureAI
            {
                /// Constructor
                npc_LunarfallLaborerAI(Creature * p_Creature);

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;
                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;

            };

    };

    namespace GussofForgefire
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Nothing     = 100,
                Stairs1     = 101,
                Stairs2     = 102,
                Stairs3     = 103,
                Stairs4     = 104,
                Anvil       = 105,
                Canon       = 106
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Nothing     = 50 * IN_MILLISECONDS,
                Stairs4     = 40 * IN_MILLISECONDS,
                Anvil       = 30 * IN_MILLISECONDS,
                Canon       = 30 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Nothing,
            MovePointIDs::Stairs1,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs4,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs1,
            MovePointIDs::Anvil,
            MovePointIDs::Canon,
            MovePointIDs::Anvil,
            MovePointIDs::Canon,
            MovePointIDs::Nothing,
            MovePointIDs::Canon,
            MovePointIDs::Anvil
        };

        static float MovePointLoc[][4] =
        {
            {  -6.5769f, -5.7238f, 0.3151f, 0.9118f },   ///< MovePointIDs::Nothing
            {  -5.2637f,  7.8586f, 0.4547f, 2.5800f },   ///< MovePointIDs::Stairs1
            {  -7.5862f,  9.2789f, 1.7972f, 2.9489f },   ///< MovePointIDs::Stairs2
            { -10.3781f,  8.7117f, 3.1357f, 3.7304f },   ///< MovePointIDs::Stairs3
            { -13.3514f,  9.4382f, 3.2421f, 2.2028f },   ///< MovePointIDs::Stairs4
            {  -0.5560f, -9.9787f, 0.3217f, 4.9244f },   ///< MovePointIDs::Anvil
            {  -8.5564f, -9.4336f, 0.3063f, 4.3707f }    ///< MovePointIDs::Canon
        };
    }

    /// Gussof Forgefire
    class npc_GussofForgefire : public CreatureScript
    {
        public:
            /// Constructor
            npc_GussofForgefire();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_GussofForgefireAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_GussofForgefireAI(Creature * p_Creature);

                /// Do next sequence element
                void DoNextSequenceAction();

                uint8 m_SequencePosition;
            };

    };

    namespace KristenStoneforge
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Table       = 100,
                Stairs1     = 101,
                Stairs2     = 102,
                Stairs3     = 103,
                Stairs4     = 104,
                Chest       = 105,
                UpTable     = 106,
                CanonBalls  = 107
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Table       = 10 * IN_MILLISECONDS,
                Chest       = 10 * IN_MILLISECONDS,
                UpTable     = 20 * IN_MILLISECONDS,
                CanonBalls  =  5 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Table,
            MovePointIDs::Stairs1,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs4,
            MovePointIDs::Chest,
            MovePointIDs::UpTable,
            MovePointIDs::Chest,
            MovePointIDs::Stairs4,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs1,
            MovePointIDs::Table,
            MovePointIDs::Stairs1,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs4,
            MovePointIDs::UpTable,
            MovePointIDs::Stairs4,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs1,
            MovePointIDs::Table,
            MovePointIDs::CanonBalls
        };

        static float MovePointLoc[][4] =
        {
            {  -7.0744f,  0.6789f, 0.3219f, 2.8745f },   ///< MovePointIDs::Table
            {  -4.2971f,  7.9247f, 0.3240f, 2.2305f },   ///< MovePointIDs::Stairs1
            {  -6.6770f, 10.7132f, 1.8032f, 2.5906f },   ///< MovePointIDs::Stairs2
            {  -9.1449f, 10.8494f, 1.8467f, 3.6941f },   ///< MovePointIDs::Stairs3
            { -11.2239f,  9.5250f, 3.1786f, 3.9061f },   ///< MovePointIDs::Stairs4
            { -16.2015f, -2.1603f, 3.2165f, 3.5134f },   ///< MovePointIDs::Chest
            { -16.5532f,  2.0093f, 3.2329f, 2.9236f },   ///< MovePointIDs::UpTable
            {   1.6793f,  7.7725f, 0.3264f, 0.0019f }    ///< MovePointIDs::CanonBalls
        };
    }

    /// Grun'lek
    class npc_KristenStoneforge : public CreatureScript
    {
        public:
            /// Constructor
            npc_KristenStoneforge();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_KristenStoneforgAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_KristenStoneforgAI(Creature * p_Creature);

                /// Do next sequence element
                void DoNextSequenceAction();

                uint8 m_SequencePosition;
            };

    };

    namespace JonathanStephens
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Table       = 100,
                Chair       = 101,
                Armory      = 102,
                Middle      = 103,
                Middle2     = 104,
                BackBed     = 105,
                Shields     = 106
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Table   = 35 * IN_MILLISECONDS,
                Chair   = 35 * IN_MILLISECONDS,
                Armory  = 35 * IN_MILLISECONDS,
                Middle  = 35 * IN_MILLISECONDS,
                Middle2 = 35 * IN_MILLISECONDS,
                BackBed = 35 * IN_MILLISECONDS,
                Shields = 35 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Table,
            MovePointIDs::Chair,
            MovePointIDs::Armory,
            MovePointIDs::Middle,
            MovePointIDs::Chair,
            MovePointIDs::Middle2,
            MovePointIDs::BackBed,
            MovePointIDs::Shields,
            MovePointIDs::Middle2
        };

        static float MovePointLoc[][4] =
        {
            {  16.1678f, -2.8809f, 0.3448f, 5.6532f },   ///< MovePointIDs::Table
            {  16.2702f, -4.5576f, 0.3113f, 5.6924f },   ///< MovePointIDs::Chair
            {  14.7902f, -5.4950f, 0.3528f, 4.6047f },   ///< MovePointIDs::Armory
            {  11.9206f, -0.0325f, 0.3153f, 0.0446f },   ///< MovePointIDs::Middle
            {  13.8977f,  0.1359f, 0.3319f, 0.1153f },   ///< MovePointIDs::Middle2
            {   6.0455f,  2.9463f, 0.3333f, 0.0132f },   ///< MovePointIDs::BackBed
            {  14.3000f,  3.3470f, 0.3473f, 5.7820f }    ///< MovePointIDs::Shields
        };
    }

    /// Grun'lek
    class npc_JonathanStephens : public CreatureScript
    {
        public:
            /// Constructor
            npc_JonathanStephens();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_JonathanStephensAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_JonathanStephensAI(Creature * p_Creature);

                /// Do next sequence element
                void DoNextSequenceAction();

                uint8 m_SequencePosition;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Garrison shelly hamby
    class npc_Skaggit : public CreatureScript
    {
        public:
            /// Constructor
            npc_Skaggit();

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Lady sena
    class npc_LadySena : public CreatureScript
    {
        public:
            /// Constructor
            npc_LadySena();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

    };

    /// Sergeant Grimjaw
    class npc_SergeantGrimjaw : public CreatureScript
    {
        public:
            /// Constructor
            npc_SergeantGrimjaw();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

    };

    /// Senior Peon II
    class npc_SeniorPeonII : public CreatureScript
    {
        public:
            /// Constructor
            npc_SeniorPeonII();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_SeniorPeonIIAI : public CreatureAI
            {
                /// Constructor
                npc_SeniorPeonIIAI(Creature * p_Creature);

                /// Called at waypoint reached or point movement finished
                /// @p_Type : Movement Type
                /// @p_ID   : Misc ID
                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                std::queue<std::function<void()>> m_DelayedOperations;  ///< Delayed operations
            };

    };

    /// Garlowe
    class npc_Gazlowe : public CreatureScript
    {
        public:
            /// Constructor
            npc_Gazlowe();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_GazloweAI : public CreatureAI
            {
                /// Constructor
                npc_GazloweAI(Creature * p_Creature);

                /// Called at waypoint reached or point movement finished
                /// @p_Type : Movement Type
                /// @p_ID   : Misc ID
                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                std::queue<std::function<void()>> m_DelayedOperations;  ///< Delayed operations
            };

    };

    namespace GrunLek
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Out     = 100,
                Carpet  = 101,
                Chest   = 102,
                Table   = 103
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Out     = 10 * IN_MILLISECONDS,
                Carpet  = 7  * IN_MILLISECONDS,
                Chest   = 5  * IN_MILLISECONDS,
                Table   = 20 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Out,
            MovePointIDs::Carpet,
            MovePointIDs::Chest,
            MovePointIDs::Carpet,
            MovePointIDs::Chest,
            MovePointIDs::Table
        };

        static float MovePointLoc[][4] =
        {
            { 17.9442f, -1.0791f, 0.3122f, 6.0833f },   ///< MovePointIDs::Out
            {  3.2029f, -0.1679f, 1.1216f, 0.1070f },   ///< MovePointIDs::Carpet
            { -0.3188f,  0.1821f, 1.1216f, 3.2722f },   ///< MovePointIDs::Chest
            {  4.3978f,  1.8508f, 1.1218f, 2.2001f }    ///< MovePointIDs::Table
        };
    }

    /// Grun'lek
    class npc_GrunLek : public CreatureScript
    {
        public:
            /// Constructor
            npc_GrunLek();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_GrunLekAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_GrunLekAI(Creature * p_Creature);

                /// Do next sequence element
                void DoNextSequenceAction();

                uint8 m_SequencePosition;
            };

    };

    /// Frostwall Grunt
    class npc_FrostWallGrunt : public CreatureScript
    {
        public:
            /// Constructor
            npc_FrostWallGrunt();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_FrostWallGruntAI : public CreatureAI
            {
                /// Constructor
                npc_FrostWallGruntAI(Creature * p_Creature);

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;
            };

    };

    /// Frostwall Smith
    class npc_FrostWallSmith : public CreatureScript
    {
        public:
            /// Constructor
            npc_FrostWallSmith();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_FrostWallSmithAI : public CreatureAI
            {
                /// Constructor
                npc_FrostWallSmithAI(Creature * p_Creature);

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;
            };

    };

    namespace OrgekIronhand
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Anvil  = 100,
                Front  = 101,
                Forge1 = 102,
                Forge2 = 103,
                Chest  = 104,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Anvil  = 35 * IN_MILLISECONDS,
                Front  = 15 * IN_MILLISECONDS,
                Forge1 = 10 * IN_MILLISECONDS,
                Forge2 = 10 * IN_MILLISECONDS,
                Chest  =  5 * IN_MILLISECONDS,
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Anvil,
            MovePointIDs::Front,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
            MovePointIDs::Chest,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
        };

        static float MovePointLoc[][4] =
        {
            {  1.1273f, -2.3209f,  0.6197f, 5.4307f },   ///< MovePointIDs::Anvil
            {  4.9331f,  1.2398f,  0.6199f, 0.2502f },   ///< MovePointIDs::Front
            { -0.3698f,  3.0644f,  0.6210f, 1.5305f },   ///< MovePointIDs::Forge1
            {  0.0264f,  2.1578f,  0.6197f, 1.6915f },   ///< MovePointIDs::Forge2
            { 10.0623f, -5.8536f, -0.1225f, 3.9730f },   ///< MovePointIDs::Chest
        };
    }

    /// 79867 - Orgek Ironhand
    class npc_OrgekIronhand : public CreatureScript
    {
        public:
            /// Constructor
            npc_OrgekIronhand();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_OrgekIronhandAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_OrgekIronhandAI(Creature * p_Creature);

                /// Do next sequence element
                void DoNextSequenceAction();

                uint8 m_SequencePosition;
            };

    };

    namespace Kinja
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Anvil  = 100,
                Front  = 101,
                Forge1 = 102,
                Forge2 = 103,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Anvil  = 60 * IN_MILLISECONDS,
                Front  = 40 * IN_MILLISECONDS,
                Forge1 = 25 * IN_MILLISECONDS,
                Forge2 = 20 * IN_MILLISECONDS,
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Front,
            MovePointIDs::Anvil,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
        };

        static float MovePointLoc[][4] =
        {
            {  1.1273f, -2.3209f,  0.6197f, 5.4307f },   ///< MovePointIDs::Anvil
            {  4.9331f,  1.2398f,  0.6199f, 0.2502f },   ///< MovePointIDs::Front
            { -0.3698f,  3.0644f,  0.6210f, 1.5305f },   ///< MovePointIDs::Forge1
            {  0.0264f,  2.1578f,  0.6197f, 1.6915f },   ///< MovePointIDs::Forge2
        };
    }

    /// 79817 - Kinja
    class npc_Kinja : public CreatureScript
    {
        public:
            /// Constructor
            npc_Kinja();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_KinjaAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_KinjaAI(Creature * p_Creature);

                /// Do next sequence element
                void DoNextSequenceAction();

                uint8 m_SequencePosition;
            };

    };

    /// Frostwall Peon
    class npc_FrostwallPeon : public CreatureScript
    {
        public:
            /// Constructor
            /// @p_ScriptName : Script name
            npc_FrostwallPeon(const std::string & p_ScriptName);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_FrostwallPeonAI : public CreatureAI
            {
                /// Constructor
                npc_FrostwallPeonAI(Creature * p_Creature);

                /// Called at waypoint reached or point movement finished
                /// @p_Type : Movement Type
                /// @p_ID   : Misc ID
                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;
                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;

                /// Select a random tree ID
                uint32 SelectRandomTree();

                uint32 m_LastTalkTimer;             ///< Last talk timer
                uint32 m_TalkInterval;              ///< Talk interval
                uint32 m_Phase;                     ///< Current phase ID
                int32 m_ThreeID;                    ///< Current Tree ID

                int32 m_WoodCuttingRemainingTime;   ///< Wood cutting remaining animation time
                int32 m_WoodCuttingCycleCount;      ///< Wood cutting cycle count

                float m_SpawnX;                     ///< Spawn position X
                float m_SpawnY;                     ///< Spawn position Y
                float m_SpawnZ;                     ///< Spawn position Z
                float m_SpawnO;                     ///< Spawn position O

                bool m_BackToSpawnPos;              ///< He is back to spawn position
                bool m_IsDynamicScript;             ///< Is this an dynamic script instance
                bool m_Initialised;                 ///< Script is initialized

                std::queue<std::function<void()>> m_DelayedOperations;  ///< Delayed operations
            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_NPC_HPP_GARRISON