////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#ifndef BATTLEGROUND_MGR_HPP
# define BATTLEGROUND_MGR_HPP

# include "Common.h"
# include "DBCEnums.h"
# include "Battleground.h"
# include "Object.h"
# include "ObjectAccessor.h"
# include <ace/Singleton.h>
# include <array>

# include "BattlegroundScheduler.hpp"
# include "BattlegroundInvitationsMgr.hpp"

namespace MS
{
    namespace Battlegrounds
    {
        using BattlegroundList = std::list<std::pair<uint32, Battleground*>>;
        using BattleMastersMap = UNORDERED_MAP<uint32, BattlegroundTypeId>;
        using BattlegroundSelectionWeightMap = std::map<BattlegroundTypeId, uint8>; ///< TypeId and its selectionWeight.

        namespace WsCurrency
        {
            enum
            {
                ResetTime = 30000 ///< Custom worldstate.
            };
        } ///< namespace WsCurrency.

        struct CreateBattlegroundData
        {
            BattlegroundTypeId bgTypeId;
            bool IsArena;
            uint32 MinPlayersPerTeam;
            uint32 MaxPlayersPerTeam;
            uint32 LevelMin;
            uint32 LevelMax;
            char* BattlegroundName;
            uint32 MapID;
            float Team1StartLocX;
            float Team1StartLocY;
            float Team1StartLocZ;
            float Team1StartLocO;
            float Team2StartLocX;
            float Team2StartLocY;
            float Team2StartLocZ;
            float Team2StartLocO;
            float StartMaxDist;
            uint32 holiday;
            uint32 scriptId;
        };

        struct QueueSchedulerItem
        {
            QueueSchedulerItem(uint32 arenaMMRating,
                uint8 arenaType,
                BattlegroundType::Type bgQueueTypeId,
                BattlegroundTypeId bgTypeId,
                Bracket::Id bracketid)
                : _arenaMMRating(arenaMMRating),
                _arenaType(arenaType),
                _bgQueueTypeId(bgQueueTypeId),
                _bgTypeId(bgTypeId),
                _bracket_id(bracketid)
            {
            }

            const uint32 _arenaMMRating;
            const uint8 _arenaType;
            const BattlegroundType::Type _bgQueueTypeId;
            const BattlegroundTypeId _bgTypeId;
            const Bracket::Id _bracket_id;
        };

        class BattlegroundMgr
        {
            friend class ACE_Singleton<BattlegroundMgr, ACE_Null_Mutex>;

        private:
            /// Constructor.
            BattlegroundMgr();

            /// Destructor.
            ~BattlegroundMgr();

        public:

            /// Updates the manager.
            /// @p_Diff : The time diff with the previous update.
            void Update(uint32 diff);

            /// Gets the battleground associated to the instance id and the bg type.
            /// Returns the instance of the battleground.
            /// @p_InstanceId   : The id of the instance.
            /// @p_BgType       : The type.
            Battleground* GetBattleground(uint32 p_InstanceId, BattlegroundType::Type p_BgType) const;

            /// Returns the template of the battleground type.
            /// @p_BgTypeId : The type.
            Battleground* GetBattlegroundTemplate(BattlegroundType::Type p_BgTypeId) const;

            /// Creates a new battleground from the type, the bracket entry, the arena type and the skirmish boolean.
            /// Returns the instance of battleground created.
            /// @p_BgTypeId     : The type.
            /// @p_BracketEntry : The entry of the bracket.
            /// @p_ArenaType    : The type of the arena if it is an arena.
            /// @p_IsSkirmish   : True if the type is a not rated arena (default: false).
            /// @p_IsWargame    : True if the battleground is a wargame (default: false)
            /// @p_IsSkirmish   : True if the battleground is a wargame and use tournament rules (default: false).
            /// @p_RatedBg      : True if the battleground is rated (default: false).

            Battleground* CreateNewBattleground(BattlegroundType::Type p_BgTypeId, Bracket const* p_BracketEntry, uint8 p_ArenaType,
                bool p_IsSkirmish = false, bool p_IsWargame = false, bool p_UseTournamentRules = false, bool p_RatedBg = false);

            /// Creates the template of the battleground associated to the data given.
            /// Returns the type id.
            /// @p_Data : The data associated to the template we are looking for.
            uint32 CreateBattlegroundTemplate(CreateBattlegroundData& p_Data);

            /// Removes the instance of the battleground associated to the instance id and the type.
            /// @p_InstanceId   : The instance id.
            /// @p_BgTypeId     : The type of the battleground.
            void RemoveBattleground(uint32 p_InstanceId, BattlegroundType::Type p_BgTypeId)
            {
                auto l_Brackets = m_InstanceId2Brackets.find(p_InstanceId);
                if (l_Brackets == std::end(m_InstanceId2Brackets))
                    return;

                m_Battlegrounds[l_Brackets->second][p_BgTypeId].remove_if([p_InstanceId](std::pair<uint32, Battleground*> const& p_Pair){
                    return p_Pair.first == p_InstanceId;
                });
            }

            /// Creates and returns special instance id just for the clients.
            /// @p_BgTypeId     : The type of the battleground.
            /// @p_BracketId    : The id of the bracket.
            uint32 CreateClientVisibleInstanceId(BattlegroundType::Type p_BgTypeId, Bracket::Id p_BracketId);

            /// Loads the templates battlegrounds from the database and creates them.
            void CreateInitialBattlegrounds();

            /// Deletes and cleans all the instances of battlegrounds.
            void DeleteAllBattlegrounds();

            /// Teleport the player to the battleground associated to the instance id and type.
            /// @p_Player       : The player.
            /// @p_InstanceId   : The instance id.
            /// @p_BgType       : The type.
            void TeleportToBattleground(Player* p_Player, uint32 p_InstanceId, BattlegroundType::Type p_BgType);

            /// Returns the scheduler associated to the battleground manager.
            inline MS::Battlegrounds::BattlegroundScheduler& GetScheduler() { return m_Scheduler; }

            /// Returns the invitation manager associated to the battleground manager.
            inline MS::Battlegrounds::BattlegroundInvitationsMgr& GetInvitationsMgr() { return m_InvitationsMgr; }

            /// Returns the list of battlegrounds according to the BracketID and the type.
            /// @p_BracketId    : The id of the bracket.
            /// @p_Type         : The type.
            BattlegroundList& GetBattlegroundList(Bracket::Id p_BracketId, BattlegroundType::Type p_Type) { return m_Battlegrounds[p_BracketId][p_Type]; }

            /// Remove the player from the invitations manager and the scheduler.
            /// @p_Guid                 : The GUID of the player.
            /// @p_DecreaseInvitedCount : A boolean which indicate if it decrease the count of invitations to make.
            /// @p_Type : The type of battleground the player wants to go.
            void RemovePlayer(uint64 p_Guid, bool p_DecreaseInvitedCount, BattlegroundType::Type p_Type)
            {
                if (m_InvitationsMgr.IsOwningPlayer(p_Guid))
                    m_InvitationsMgr.RemovePlayer(p_Guid, p_DecreaseInvitedCount, p_Type);
                else
                    m_Scheduler.RemovePlayer(p_Guid, p_Type);
            }

            /// Returns a copy of the set of battleground client ids associated to the type and the bracket id.
            /// @p_BgType       : The type.
            /// @p_BracketId    : The bracket id.
            std::set<uint32> GetClientBattlegroundIds(uint32 p_BgTypeId, uint32 p_BracketId) { return m_ClientBattlegroundIds[p_BgTypeId][p_BracketId]; }

            /// Returns the max rating difference for rated battlegrounds.
            uint32 GetMaxRatingDifference() const;

            /// Returns the discard timer for rated battlegrounds.
            uint32 GetRatingDiscardTimer()  const;

            /// Returns the premature finish time.
            uint32 GetPrematureFinishTime() const;

            /// Toggles the arena testing flag (DEPRECATED).
            void ToggleArenaTesting();

            /// Toggles the battleground testing flag.
            void ToggleTesting();

            /// Activate the holiday flag for the battlegrounds who are in the p_ActivateHolidayId.
            /// @p_ActivateHolidayId : The list of id that activate holidays in battlegrounds.
            void SetHolidayWeekends(std::list<uint32> const& activeHolidayId);

            /// Loads the battle masters entry from the database and cache them in memory.
            void LoadBattleMastersEntry();

            /// Returns the TypeId of the battle master entry.
            /// @p_Entry : The entry of the battle master.
            BattlegroundTypeId GetBattleMasterBG(uint32 p_Entry) const
            {
                auto itr = m_BattleMastersMap.find(p_Entry);
                if (itr != std::end(m_BattleMastersMap))
                    return itr->second;

                return BATTLEGROUND_WS;
            }

            /// Returns the value of the arena testing flag.
            bool isArenaTesting() const { return m_ArenaTesting; }

            /// Returns the value of the battleground testing flag.
            bool isTesting() const { return m_Testing; }

            /// Converts a TypeId into a HolidayIds.
            /// @p_TypeId : The type id to convert.
            static HolidayIds BGTypeToWeekendHolidayId(BattlegroundTypeId p_TypeId);

            /// Converts a HolidayIds to a BattlegroundTypeId.
            /// @p_Holiday : The holiday ids.
            static BattlegroundTypeId WeekendHolidayIdToBGType(HolidayIds p_Holiday);

            /// Returns true if the BattlegroundTypeId is a BattlegroundWeekend.
            /// @p_BgTypeId : The type id.
            static bool IsBGWeekend(BattlegroundTypeId p_BgTypeId);

        private:

            BattleMastersMap m_BattleMastersMap;                                                ///< The battle masters map.
            BattlegroundList m_Battlegrounds[Brackets::Count][BattlegroundType::Max];           ///< The battlegrounds owned by the manager.
            std::array<Battleground*, BattlegroundType::Total> m_BattlegroundTemplates;         ///< The templates of the battlegrounds in database.
            BattlegroundSelectionWeightMap m_ArenaSelectionWeights;                             ///< The arena selection weights, not used.
            BattlegroundSelectionWeightMap m_BGSelectionWeights;                                ///< The battleground selection weights, not used.
            BattlegroundSelectionWeightMap m_RatedBGSelectionWeights;                           ///< The rated battleground selection weights, not used.
            std::set<uint32> m_ClientBattlegroundIds[BattlegroundType::Total][Brackets::Count]; ///< The instance ids just visible for the client
            bool m_ArenaTesting;                                                                ///< The arena testing flag, not used.
            bool m_Testing;                                                                     ///< The battleground testing flag.
            BattlegroundScheduler m_Scheduler;                                                  ///< The battleground scheduler.
            BattlegroundInvitationsMgr m_InvitationsMgr;                                        ///< The battleground invitations manager.
            std::map<uint32, Bracket::Id> m_InstanceId2Brackets;                                ///< A map that keep binded instance ids and bracket ids.
        };
    } ///< namespace Battlegrounds.
} ///< namespace MS.

# define sBattlegroundMgr ACE_Singleton<MS::Battlegrounds::BattlegroundMgr, ACE_Null_Mutex>::instance() ///< The singleton of the battleground manager.

#endif ///< BATTLEGROUND_MGR_HPP
