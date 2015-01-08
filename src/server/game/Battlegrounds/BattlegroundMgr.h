/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __BATTLEGROUNDMGR_H
# define __BATTLEGROUNDMGR_H

# include "Common.h"
# include "DBCEnums.h"
# include "Battleground.h"
# include "Object.h"
# include <ace/Singleton.h>
# include <array>

# include "BattlegroundScheduler.hpp"
# include "BattlegroundInvitationsMgr.hpp"

namespace MS
{
    namespace Battlegrounds
    {
        typedef std::list<std::pair<uint32, Battleground*>> BattlegroundList;

        typedef UNORDERED_MAP<uint32, BattlegroundTypeId> BattleMastersMap;

#define WS_CURRENCY_RESET_TIME 30000                    // Custom worldstate

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
            using BGFreeSlotQueueType = std::list < Battleground* > ;

            friend class ACE_Singleton < BattlegroundMgr, ACE_Null_Mutex > ;

        private:
            BattlegroundMgr();
            ~BattlegroundMgr();

        public:
            void Update(uint32 diff);

            /* Battlegrounds */
            Battleground* GetBattleground(uint32 p_InstanceId, BattlegroundType::Type p_BgType) const;
            Battleground* GetBattlegroundTemplate(BattlegroundType::Type p_BgTypeId) const;

            Battleground* CreateNewBattleground(BattlegroundType::Type p_BgTypeId, Bracket const* p_BracketEntry, uint8 p_ArenaType, bool p_IsSkirmish = false);

            uint32 CreateBattlegroundTemplate(CreateBattlegroundData& p_Data);

            void AddBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId, Battleground* p_Bg)
            {
                m_Battlegrounds[p_Bg->GetBracketId()][bgTypeId].push_back(std::make_pair(InstanceID, p_Bg));
            }

            void RemoveBattleground(uint32 instanceID, BattlegroundTypeId bgTypeId)
            {
                auto l_Brackets = m_InstanceId2Brackets.find(instanceID);
                if (l_Brackets == std::end(m_InstanceId2Brackets))
                    return;

                m_Battlegrounds[l_Brackets->second][bgTypeId].remove_if([instanceID](const std::pair<uint32, Battleground*>& p_Pig){
                    return p_Pig.first == instanceID;
                });
            }
            uint32 CreateClientVisibleInstanceId(BattlegroundType::Type bgTypeId, Bracket::Id bracket_id);

            void CreateInitialBattlegrounds();
            void InitAutomaticArenaPointDistribution();
            void DeleteAllBattlegrounds();

            void TeleportToBattleground(Player* player, uint32 InstanceID, BattlegroundType::Type bgTypeId);

            inline MS::Battlegrounds::BattlegroundScheduler& GetScheduler() { return m_Scheduler; }
            inline MS::Battlegrounds::BattlegroundInvitationsMgr& GetInvitationsMgr() { return m_InvitationsMgr; }

            void RemovePlayer(uint64 p_Guid, bool p_DecreaseInvitedCount)
            {
                if (m_InvitationsMgr.IsOwningPlayer(p_Guid))
                    m_InvitationsMgr.RemovePlayer(p_Guid, p_DecreaseInvitedCount);
                else
                    m_Scheduler.RemovePlayer(p_Guid);
            }

            std::set<uint32> GetClientBattlegroundIds(uint32 p_BgTypeId, uint32 p_BracketId)
            {
                return m_ClientBattlegroundIds[p_BgTypeId][p_BracketId];
            }

            BGFreeSlotQueueType BGFreeSlotQueue[MAX_BATTLEGROUND_TYPE_ID];

            //void ScheduleQueueUpdate(uint32 arenaMatchmakerRating, uint8 arenaType, BattlegroundType bgQueueTypeId, BattlegroundTypeId bgTypeId, Bracket::Id bracket_id);
            uint32 GetMaxRatingDifference() const;
            uint32 GetRatingDiscardTimer()  const;
            uint32 GetPrematureFinishTime() const;

            void ToggleArenaTesting();
            void ToggleTesting();

            void SetHolidayWeekends(std::list<uint32> activeHolidayId);
            void LoadBattleMastersEntry();
            BattlegroundTypeId GetBattleMasterBG(uint32 entry) const
            {
                BattleMastersMap::const_iterator itr = m_BattleMastersMap.find(entry);
                if (itr != m_BattleMastersMap.end())
                    return itr->second;
                return BATTLEGROUND_WS;
            }

            bool isArenaTesting() const { return m_ArenaTesting; }
            bool isTesting() const { return m_Testing; }

            static bool IsBattlegroundType(BattlegroundType::Type p_BgType) { return !IsArenaType(p_BgType); }
            static bool IsSkirmishQueue(BattlegroundType::Type p_BgType) { return (p_BgType == MS::Battlegrounds::BattlegroundType::ArenaSkirmish2v2 || p_BgType == MS::Battlegrounds::BattlegroundType::ArenaSkirmish3v3); }

            static HolidayIds BGTypeToWeekendHolidayId(BattlegroundTypeId bgTypeId);
            static BattlegroundTypeId WeekendHolidayIdToBGType(HolidayIds holiday);
            static bool IsBGWeekend(BattlegroundTypeId bgTypeId);
        private:
            typedef std::map<BattlegroundTypeId, uint8> BattlegroundSelectionWeightMap; // TypeId and its selectionWeight

            BattleMastersMap    m_BattleMastersMap;

            /* Battlegrounds */
            BattlegroundList m_Battlegrounds[Brackets::Count][BattlegroundType::Max];
            std::array<Battleground*, BattlegroundType::Total> m_BattlegroundTemplates;

            BattlegroundSelectionWeightMap m_ArenaSelectionWeights;
            BattlegroundSelectionWeightMap m_BGSelectionWeights;
            BattlegroundSelectionWeightMap m_RatedBGSelectionWeights;

            std::vector<QueueSchedulerItem*> m_QueueUpdateScheduler;
            std::set<uint32> m_ClientBattlegroundIds[BattlegroundType::Total][Brackets::Count]; //the instanceids just visible for the client
            uint32 m_NextRatedArenaUpdate;
            bool   m_ArenaTesting;
            bool   m_Testing;

            BattlegroundScheduler m_Scheduler;
            BattlegroundInvitationsMgr m_InvitationsMgr;

            std::map<uint32, Bracket::Id> m_InstanceId2Brackets;
        };
    }
}

# define sBattlegroundMgr ACE_Singleton<MS::Battlegrounds::BattlegroundMgr, ACE_Null_Mutex>::instance()
#endif

