////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef OUTDOOR_PVP_H_
#define OUTDOOR_PVP_H_

#include "Utilities/Util.h"
#include "SharedDefines.h"
#include "ZoneScript.h"
#include "ObjectAccessor.h"

class GameObject;

enum OutdoorPvPTypes
{
    OUTDOOR_PVP_HP      = 1,
    OUTDOOR_PVP_NA      = 2,
    OUTDOOR_PVP_TF      = 3,
    OUTDOOR_PVP_ZM      = 4,
    OUTDOOR_PVP_SI      = 5,
    OUTDOOR_PVP_ASHRAN  = 6
};

#define MAX_OUTDOORPVP_TYPES 7

enum ObjectiveStates
{
    OBJECTIVESTATE_NEUTRAL = 0,
    OBJECTIVESTATE_ALLIANCE,
    OBJECTIVESTATE_HORDE,
    OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE,
    OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE,
    OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE,
    OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE
};

#define OTHER_TEAM(a) (a == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE)

// struct for go spawning
struct go_type
{
    uint32 entry;
    uint32 map;
    float x;
    float y;
    float z;
    float o;
    float rot0;
    float rot1;
    float rot2;
    float rot3;
};

// struct for creature spawning
struct creature_type
{
    uint32 entry;
    uint32 teamval;
    uint32 map;
    float x;
    float y;
    float z;
    float o;
};

// some class predefs
class Player;
class GameObject;
class WorldPacket;
class Creature;
class Unit;
struct GossipMenuItems;
class OutdoorPvP;
class OutdoorGraveyard;

typedef std::set<Player*> PlayerSet;
typedef std::vector<OutdoorGraveyard*> GraveyardVector;
typedef std::set<uint64> GuidSet;

class OPvPCapturePoint
{
    public:

        OPvPCapturePoint(OutdoorPvP* pvp);

        virtual ~OPvPCapturePoint() {}

        virtual void FillInitialWorldStates(ByteBuffer & /*data*/) {}

        // send world state update to all players present
        void SendUpdateWorldState(uint32 field, uint32 value);

        // send kill notify to players in the controlling faction
        void SendObjectiveComplete(uint32 id, uint64 guid);

        // used when player is activated/inactivated in the area
        virtual bool HandlePlayerEnter(Player* player);
        virtual void HandlePlayerLeave(Player* player);

        // checks if player is in range of a capture credit marker
        bool IsInsideObjective(Player* player) const;

        virtual bool HandleCustomSpell(Player* player, uint32 spellId, GameObject* go);

        virtual int32 HandleOpenGo(Player* player, uint64 guid);

        // returns true if the state of the objective has changed, in this case, the OutdoorPvP must send a world state ui update.
        virtual bool Update(uint32 diff);

        virtual void ChangeState() = 0;

        virtual void ChangeTeam(TeamId /*oldTeam*/) {}

        virtual void SendChangePhase();

        virtual bool HandleGossipOption(Player* player, uint64 guid, uint32 gossipid);

        virtual bool CanTalkTo(Player* player, Creature* c, GossipMenuItems const& gso);

        virtual bool HandleDropFlag(Player* player, uint32 spellId);

        virtual void DeleteSpawns();

        uint64 m_capturePointGUID;

        GameObject* m_capturePoint;

        void AddGO(uint32 type, uint32 guid, uint32 entry = 0);
        void AddCre(uint32 type, uint32 guid, uint32 entry = 0);

        bool SetCapturePointData(go_type p_Data)
        {
            return SetCapturePointData(p_Data.entry, p_Data.map, p_Data.x, p_Data.y, p_Data.z, p_Data.o, p_Data.rot0, p_Data.rot1, p_Data.rot2, p_Data.rot3);
        }
        bool SetCapturePointData(uint32 entry, uint32 map, float x, float y, float z, float o = 0,
            float rotation0 = 0, float rotation1 = 0, float rotation2 = 0, float rotation3 = 0);

        void SetState(ObjectiveStates p_State) { m_State = p_State; }
        void SetValue(float p_Value) { m_value = p_Value; }

    protected:

        bool AddObject(uint32 p_Type, go_type p_Data)
        {
            return AddObject(p_Type, p_Data.entry, p_Data.map, p_Data.x, p_Data.y, p_Data.z, p_Data.o, p_Data.rot0, p_Data.rot1, p_Data.rot2, p_Data.rot3);
        }
        bool AddObject(uint32 type, uint32 entry, uint32 map, float x, float y, float z, float o,
            float rotation0, float rotation1, float rotation2, float rotation3);
        bool AddCreature(uint32 p_Type, creature_type p_Data, uint32 p_SpawnTime = 0)
        {
            return AddCreature(p_Type, p_Data.entry, p_Data.teamval, p_Data.map, p_Data.x, p_Data.y, p_Data.z, p_Data.o, p_SpawnTime);
        }
        bool AddCreature(uint32 type, uint32 entry, uint32 teamval, uint32 map, float x, float y, float z, float o, uint32 spawntimedelay = 0);

        bool DelCreature(uint32 type);
        bool DelObject(uint32 type);

        bool DelCapturePoint();

    protected:

        // active players in the area of the objective, 0 - alliance, 1 - horde
        GuidSet m_activePlayers[2];

        // total shift needed to capture the objective
        float m_maxValue;
        float m_minValue;

        // maximum speed of capture
        float m_maxSpeed;

        // the status of the objective
        float m_value;

        TeamId m_team;

        // objective states
        ObjectiveStates m_OldState;
        ObjectiveStates m_State;

        // neutral value on capture bar
        uint32 m_neutralValuePct;

        // pointer to the OutdoorPvP this objective belongs to
        OutdoorPvP* m_PvP;

        // map to store the various gameobjects and creatures spawned by the objective
        //        type, guid
        std::map<uint32, uint64> m_Objects;
        std::map<uint32, uint64> m_Creatures;
        std::map<uint64, uint32> m_ObjectTypes;
        std::map<uint64, uint32> m_CreatureTypes;
};

// base class for specific outdoor pvp handlers
class OutdoorPvP : public ZoneScript
{
    friend class OutdoorPvPMgr;

    public:

        // ctor
        OutdoorPvP();

        // dtor
        virtual ~OutdoorPvP();

        // deletes all gos/creatures spawned by the pvp
        void DeleteSpawns();

        typedef std::map<uint64, OPvPCapturePoint*> OPvPCapturePointMap;

        virtual void FillInitialWorldStates(ByteBuffer & /*data*/) {}

        // called when a player triggers an areatrigger
        virtual bool HandleAreaTrigger(Player* player, uint32 trigger);

        // called on custom spell
        virtual bool HandleCustomSpell(Player* player, uint32 spellId, GameObject* go);

        // called on go use
        virtual bool HandleOpenGo(Player* player, uint64 guid);

        // setup stuff
        virtual bool SetupOutdoorPvP() {return true;}

        void OnGameObjectCreate(GameObject* p_GameObject);
        void OnGameObjectRemove(GameObject* p_GameObject);
        void OnCreatureCreate(Creature* /*p_Creature*/) { }
        void OnCreatureRemove(Creature* /*p_Creature*/) { }

        // send world state update to all players present
        void SendUpdateWorldState(uint32 field, uint32 value);

        // called by OutdoorPvPMgr, updates the objectives and if needed, sends new worldstateui information
        virtual bool Update(uint32 diff);

        // handle npc/player kill
        virtual void HandleKill(Player* killer, Unit* killed);
        virtual void HandleKillImpl(Player* /*killer*/, Unit* /*killed*/) { }
        virtual void HandlePlayerKilled(Player* /*p_Player*/) { }

        /// Handle some custom PvP loots
        virtual void FillCustomPvPLoots(Player* /*p_Looter*/, Loot& /*p_Loot*/, uint64 /*p_Container*/) { }

        // checks if player is in range of a capture credit marker
        bool IsInsideObjective(Player* player) const;

        // awards rewards for player kill
        virtual void AwardKillBonus(Player* /*player*/) {}

        uint32 GetTypeId() {return m_TypeId;}

        virtual bool HandleDropFlag(Player* player, uint32 spellId);

        virtual bool HandleGossipOption(Player* player, uint64 guid, uint32 gossipid);

        virtual bool CanTalkTo(Player* player, Creature* c, GossipMenuItems const& gso);

        void TeamApplyBuff(TeamId team, uint32 spellId, uint32 spellId2 = 0);

        virtual void HandleBFMGREntryInviteResponse(bool /*p_Accepted*/, Player* /*p_Player*/) { }

        virtual WorldSafeLocsEntry const* GetClosestGraveyard(Player* p_Player);

        void SendAreaSpiritHealerQueryOpcode(Player* p_Player, uint64 const& p_Guid);
        void AddPlayerToResurrectQueue(uint64 p_SpiritGuid, uint64 p_PlayerGuid);
        OutdoorGraveyard* GetGraveyardById(uint32 p_ID);
        void SetGraveyardNumber(uint32 p_Count) { m_GraveyardList.resize(p_Count); }

        uint64 GetCreature(uint32 p_Type);

    protected:

        // the map of the objectives belonging to this outdoorpvp
        OPvPCapturePointMap m_capturePoints;

        GuidSet m_Players[MS::Battlegrounds::TeamsCount::Value];

        uint32 m_TypeId;

        // map to store the various gameobjects and creatures spawned by the objective
        //       type,   guid
        std::map<uint32, uint64> m_Creatures;
        std::map<uint64, uint32> m_CreatureTypes;
        std::map<uint32, uint64> m_Objects;
        std::map<uint64, uint32> m_ObjectTypes;

        bool m_sendUpdate;

        /// Graveyard variables
        GraveyardVector m_GraveyardList;    ///< Vector which contain the different Graveyards of the battle
        uint32 m_LastResurectTimer;         ///< Timer for resurrect players every 30s

        // world state stuff
        virtual void SendRemoveWorldStates(Player* /*p_Player*/) {}

        void BroadcastPacket(WorldPacket& data) const;

        virtual void HandlePlayerEnterZone(Player* p_Player, uint32 p_ZoneID);
        virtual void HandlePlayerLeaveZone(Player* p_Player, uint32 p_ZoneID);

        virtual void HandlePlayerEnterMap(Player* /*p_Player*/, uint32 /*p_MapID*/) { }
        virtual void HandlePlayerLeaveMap(Player* /*p_Player*/, uint32 /*p_MapID*/) { }

        virtual void HandlePlayerEnterArea(Player* /*p_Player*/, uint32 /*p_AreaID*/) { }
        virtual void HandlePlayerLeaveArea(Player* /*p_Player*/, uint32 /*p_AreaID*/) { }

        virtual void HandlePlayerResurrects(Player* /*p_Player*/, uint32 /*p_ZoneID*/) { }

        void AddCapturePoint(OPvPCapturePoint* cp)
        {
            m_capturePoints[cp->m_capturePointGUID] = cp;

            // Must do that again, in case of we tried to add capture point before...
            // ...we tried to add capture point itself
            if (GameObject* l_Go = HashMapHolder<GameObject>::Find(cp->m_capturePointGUID))
                cp->m_capturePoint = l_Go;
        }

        OPvPCapturePoint * GetCapturePoint(uint64 p_Guid) const
        {
            OutdoorPvP::OPvPCapturePointMap::const_iterator itr = m_capturePoints.find(p_Guid);
            if (itr != m_capturePoints.end())
                return itr->second;
            return NULL;
        }

        void RegisterZone(uint32 zoneid);

        bool HasPlayer(Player* player) const;

        void TeamCastSpell(TeamId team, int32 spellId);

        void AddAreaTrigger(uint32 p_Entry, uint32 p_PhaseMask, uint32 p_SpellVisualID, Position const& p_Pos, uint32 p_Duration, Map* p_Map);

        bool AddCreature(uint32 p_Type, creature_type p_Data, uint32 p_SpawnTime = 0)
        {
            return AddCreature(p_Type, p_Data.entry, p_Data.teamval, p_Data.map, p_Data.x, p_Data.y, p_Data.z, p_Data.o, p_SpawnTime);
        }
        bool AddCreature(uint32 p_Type, uint32 p_Entry, uint32 p_Team, uint32 p_MapID, float p_X, float p_Y, float p_Z, float p_O, uint32 p_SpawnTime = 0);
        bool DelCreature(uint32 p_Type);

        bool AddObject(uint32 p_Type, go_type p_Data)
        {
            return AddObject(p_Type, p_Data.entry, p_Data.map, p_Data.x, p_Data.y, p_Data.z, p_Data.o, p_Data.rot0, p_Data.rot1, p_Data.rot2, p_Data.rot3);
        }
        bool AddObject(uint32 p_Type, uint32 p_Entry, uint32 p_Map, float p_X, float p_Y, float p_Z, float p_O,
            float p_Rot0, float p_Rot1, float p_Rot2, float p_Rot3);
        bool DelObject(uint32 p_Type);
};

class OutdoorGraveyard
{
    public:
        OutdoorGraveyard(OutdoorPvP* p_OutdoorPvP);

        /// Method to changing who controls the graveyard
        void GiveControlTo(TeamId p_Team);
        TeamId GetControlTeamId() { return m_ControlTeam; }

        /// Find the nearest graveyard to a player
        float GetDistance(Player* p_Player) const;

        /// Initialize the graveyard
        void Initialize(TeamId p_Team, uint32 p_Graveyard);

        /// Set spirit service for the graveyard
        void SetSpirit(Creature* p_Spirit, TeamId p_Team);

        /// Add a player to the graveyard
        void AddPlayer(uint64 p_Guid);

        /// Remove a player from the graveyard
        void RemovePlayer(uint64 p_Guid);

        /// Resurrect players
        void Resurrect();

        /// Move players waiting to that graveyard on the nearest one
        void RelocateDeadPlayers();

        /// Check if this graveyard has a spirit guide
        bool HasNpc(uint64 p_Guid)
        {
            if (!m_SpiritGuide[0] && !m_SpiritGuide[1])
                return false;

            if (!sObjectAccessor->FindUnit(m_SpiritGuide[0]) &&
                !sObjectAccessor->FindUnit(m_SpiritGuide[1]))
                return false;

            return (m_SpiritGuide[0] == p_Guid || m_SpiritGuide[1] == p_Guid);
        }

        /// Check if a player is in this graveyard's resurrect queue
        bool HasPlayer(uint64 p_Guid)
        {
            return m_ResurrectQueue.find(p_Guid) != m_ResurrectQueue.end();
        }

        /// Get the graveyard's ID.
        uint32 GetGraveyardId() { return m_GraveyardId; }

    protected:
        TeamId m_ControlTeam;
        uint32 m_GraveyardId;
        uint64 m_SpiritGuide[2];
        GuidSet m_ResurrectQueue;
        OutdoorPvP* m_OutdoorPvP;
};

#endif /*OUTDOOR_PVP_H_*/
