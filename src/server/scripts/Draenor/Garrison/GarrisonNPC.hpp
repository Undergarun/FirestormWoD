////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_NPC_HPP_GARRISON
# define GARRISON_NPC_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"
#include "../../../game/AI/ScriptedAI/ScriptedEscortAI.h"
#include "Common.h"
#include "ScriptedCosmeticAI.hpp"
#include "GarrisonNPCAI.hpp"
#include "Vehicle.h"
#include "CombatAI.h"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Sequence initializer function
    using InitSequenceFunction = std::function<void(GarrisonNPCAI*, Creature*)>;

    /// Creature AI
    /// @t_SetupLevel1 : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2 : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3 : Function pour initializing sequence for level 3 building
    template<InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3>
    struct SimpleSequenceCosmeticScriptAI : public GarrisonNPCAI
    {
        /// Constructor
        SimpleSequenceCosmeticScriptAI(Creature* p_Creature)
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

            switch (l_BuildingEntry->Level)
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

    /// Simple sequence cosmetic script, Helper for small cosmetic script
    /// @t_ScriptName  : Script name
    /// @t_SetupLevel1 : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2 : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3 : Function pour initializing sequence for level 3 building
    template<const char* t_ScriptName, InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3>
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
            CreatureAI* GetAI(Creature* p_Creature) const override
            {
                return new SimpleSequenceCosmeticScriptAI<t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>(p_Creature);
            }

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
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// 87518 - 87519 - 80258
    class npc_CallToArms : public CreatureScript
    {
        public:
            /// Constructor
            npc_CallToArms();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// AI Script
            struct npc_CallToArmsAI : public CreatureAI
            {
                /// Constructor
                npc_CallToArmsAI(Creature* p_Creature);

                /// On reset
                virtual void Reset() override;
                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                Player* m_Owner;
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
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// AI Script
            struct npc_GarrisonCartRopeAI : public CreatureAI
            {
                /// Constructor
                npc_GarrisonCartRopeAI(Creature* p_Creature);

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
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

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
            virtual bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override;
            /// Called when a player completes a quest with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Completed quest
            virtual bool OnQuestComplete(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override;

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
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

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
            virtual bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override;
            /// Called when a player completes a quest with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Completed quest
            virtual bool OnQuestComplete(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override;

    };

    /// Lunarfall Laborer
    class npc_LunarfallLaborer : public CreatureScript
    {
        public:
            /// Constructor
            npc_LunarfallLaborer();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_LunarfallLaborerAI : public CreatureAI
            {
                /// Constructor
                npc_LunarfallLaborerAI(Creature* p_Creature);

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;
                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;

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
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

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
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

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
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

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
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_SeniorPeonIIAI : public CreatureAI
            {
                /// Constructor
                npc_SeniorPeonIIAI(Creature* p_Creature);

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
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_GazloweAI : public CreatureAI
            {
                /// Constructor
                npc_GazloweAI(Creature* p_Creature);

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

    /// Frostwall Peon
    class npc_FrostwallPeon : public CreatureScript
    {
        public:
            /// Constructor
            /// @p_ScriptName : Script name
            npc_FrostwallPeon(const std::string & p_ScriptName);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_FrostwallPeonAI : public CreatureAI
            {
                /// Constructor
                npc_FrostwallPeonAI(Creature* p_Creature);

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

    /// Atheeru Palestar
    class npc_garrison_atheeru_palestar : public CreatureScript
    {
        public:
            /// Constructor
            npc_garrison_atheeru_palestar();

            enum MiscDatas
            {
                NpcTextID = 92007
            };

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target creature instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_garrison_atheeru_palestarAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_garrison_atheeru_palestarAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            void SpawnAssemblies();

            void DoAction(int32 const p_Action) override;

            void OnPlotInstanceUnload() override;
    };

    /// Amperial Construct
    class npc_garrison_amperial_construct : public CreatureScript
    {
        public:
            /// Constructor
            npc_garrison_amperial_construct();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_garrison_amperial_constructAI : public GarrisonNPCAI
            {
                uint64 m_OwnerGuid;
                uint64 m_CheckTimer;

                /// Constructor
                npc_garrison_amperial_constructAI(Creature* p_Creature);

                virtual void OnSpellClick(Unit* p_Clicker) override;

                virtual void PassengerBoarded(Unit* p_Passenger, int8 p_SeatID, bool p_Apply) override;

                virtual void SetGUID(uint64 p_Guid, int32 p_Id) override;

                virtual void UpdateAI(const uint32 p_Diff) override;
            };
    };

    /// Amperial Construct
    class npc_GarrisonStablesCreatures : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarrisonStablesCreatures();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_GarrisonStablesCreaturesAI : public npc_escortAI
            {
                /// Constructor
                npc_GarrisonStablesCreaturesAI(Creature* p_Creature);

                enum eCreaturesEntries
                {
                    NpcIcehoof       = 86847,
                    NpcRocktusk      = 86850,
                    NpcSilverpelt    = 86801,
                    NpcMeadowstomper = 86852,
                    NpcRiverwallow   = 86848,
                    NpcSnarler       = 86851
                };

                enum CreatureJumps
                {
                    MeadowstomperFirstJump = 1,
                    MeadowstomperSecondJump,
                    MeadowstomperThirdJump,
                    SnarlerFirstJump,
                    SnarlerSecondJump,
                    SnarlerThirdJump,
                    SnarlerFourthJump,
                    SnarlerFifthJump,
                    SnarlerSixthJump,
                    SnarlerSeventhJump,
                    SilverpeltFirstJump,
                    SilverpeltSecondJump
                };

                virtual void Reset() override;

                virtual void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo) override;

                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                virtual void WaypointReached(uint32 p_PointId) override;

                virtual void UpdateAI(uint32 const p_Diff) override;

                void StopEscortEvent(uint32 p_KillCredit, uint32 p_SpellID);
            };
    };

    // Fleet Command Talbe
    class npc_FleetCommandTable : public CreatureScript
    {
        public:
            npc_FleetCommandTable();
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;
    };

    /// Amperial Construct
    class npc_AncientTradingMechanism_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_AncientTradingMechanism_Garr();

            bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Followers Script
    class npc_follower_generic_script : public CreatureScript
    {
        public:
            /// Constructor
            npc_follower_generic_script();

            enum eSpells
            {
                SpellSongOfTheAnvil  = 176458,
                SpellSolaceOfTheForge = 176644
            };

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_follower_generic_scriptAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_follower_generic_scriptAI(Creature* p_Creature);

                uint32 m_UpdateTimer;
                uint64 m_OwnerGUID;

                virtual void Reset() override;

                virtual void UpdateAI(uint32 const p_Diff) override;

                virtual void SetGUID(uint64 p_Guid, int32 p_Id) override;
            };
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Fearsome Battle Standard (87594 / 86734)
    class npc_FearsomeBattleStandard : public CreatureScript
    {
        public:
            /// Constructor
            npc_FearsomeBattleStandard();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_FearsomeBattleStandardAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_FearsomeBattleStandardAI(Creature* p_Creature);

                enum eEvents
                {
                    EventCheckPlayers = 1
                };

                enum eSpells
                {
                    SpellAuraFearsomeBattleStandardPeriodicDmg = 176302
                };

                EventMap m_Events;

                virtual void Reset() override;

                virtual void UpdateAI(uint32 const p_Diff) override;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Inspiring Battle Standard (88277 / 88010)
    class npc_InspiringBattleStandard : public CreatureScript
    {
        public:
            /// Constructor
            npc_InspiringBattleStandard();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_InspiringBattleStandardAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_InspiringBattleStandardAI(Creature* p_Creature);

                enum eEvents
                {
                    EventCheckPlayers = 1
                };

                enum eSpells
                {
                    SpellAuraInspiringBattleStandardPeriodicDmg = 176252
                };

                EventMap m_Events;

                virtual void Reset() override;

                virtual void UpdateAI(uint32 const p_Diff) override;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Tent (86327, 86333, 86334, 86335, 86336, 86337, 86338, 86339, 86340, 86341, 86342, 86343, 86344, 86345, 86346, 86347, 86348, 86349, 86350, 86351, 86352, 86353, 86354
    class npc_LeatherWorkingTent_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_LeatherWorkingTent_Garr();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_LeatherWorkingTent_GarrAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_LeatherWorkingTent_GarrAI(Creature* p_Creature);

                enum eEvents
                {
                    EventCheckPlayers = 1
                };

                enum eSpells
                {
                    SpellAuraWellRestedTrackingAura = 172424
                };

                uint64 m_SummonerGuid;
                EventMap m_Events;

                virtual void Reset() override;

                virtual void IsSummonedBy(Unit* p_Summoner) override;

                virtual void UpdateAI(uint32 const p_Diff) override;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_StablesTrainingMounts_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_StablesTrainingMounts_Garr() : CreatureScript("npc_StablesTrainingMounts_Garr")
            {
            }

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_StablesTrainingMounts_GarrAI : public VehicleAI
            {
                /// Constructor
                npc_StablesTrainingMounts_GarrAI(Creature* creature) : VehicleAI(creature)
                {
                    m_SummonerGUID = 0;
                }

                enum eSpells
                {
                    SpellAuraRideVehicle       = 178807,
                    SpellAuraBlackClawOfSethe  = 174822,
                    SpellAuraGarnToothNecklace = 174823
                };

                enum eItems
                {
                    ItemGarnToothNecklace = 118470,
                    ItemBlackClawOfSethe  = 118469
                };

                uint64 m_SummonerGUID;

                virtual void IsSummonedBy(Unit* p_Summoner) override;

                virtual void PassengerBoarded(Unit* p_Passenger, int8 p_SeatID, bool p_Apply) override;

                virtual void JustDied(Unit* p_Killer) override;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_robot_rooster : public CreatureScript
    {
        public:
            /// Constructor
            npc_robot_rooster() : CreatureScript("npc_StablesTrainpc_robot_roosterningMounts_Garr")
            {
            }

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;

            /// Creature AI
            struct npc_robot_roosterAI : public ScriptedAI
            {
                /// Constructor
                npc_robot_roosterAI(Creature* creature) : ScriptedAI(creature)
                {
                    m_Events.Reset();
                }

                enum eDatas
                {
                    EventBerserk     = 1,
                    EventNitroBoosts = 2,
                    SpellBerserk     = 168458,
                    SpellNitroBoosts = 133022
                };

                EventMap m_Events;

                virtual void EnterCombat(Unit* p_Attacker) override;

                virtual void EnterEvadeMode() override;

                virtual void UpdateAI(uint32 const p_Diff) override;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_GarrisonWalter : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarrisonWalter() : CreatureScript("npc_GarrisonWalter")
            {
            }

            enum eData
            {
                DataOwnerGUID = 1
            };

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_GarrisonWalterAI : public VehicleAI
            {
                /// Constructor
                npc_GarrisonWalterAI(Creature* creature) : VehicleAI(creature)
                {
                    m_SummonerGUID = 0;
                }

                uint64 m_SummonerGUID;

                virtual void IsSummonedBy(Unit* p_Summoner) override;

                virtual uint64 GetGUID(int32 p_ID) override;
            };

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_GearshopWorkshopTurret_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_GearshopWorkshopTurret_Garr() : CreatureScript("npc_GearshopWorkshopTurret_Garr")
            {
            }

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_GearshopWorkshopTurret_GarrAI : public VehicleAI
            {
                /// Constructor
                npc_GearshopWorkshopTurret_GarrAI(Creature* creature) : VehicleAI(creature)
                {
                    m_SummonerGUID = 0;
                    m_AttackTimer  = 0;
                }

                enum eSpells
                {
                    SpellTurretFire = 168517
                };

                uint64 m_SummonerGUID;
                uint32 m_AttackTimer;

                virtual void Reset() override;

                virtual void EnterCombat(Unit* p_Attacker) override;

                virtual void IsSummonedBy(Unit* p_Summoner) override;

                virtual void UpdateAI(uint32 const p_Diff) override;
            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_NPC_HPP_GARRISON
