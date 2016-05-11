////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _OBJECT_H
#define _OBJECT_H

#include "Common.h"
#include "UpdateFields.h"
#include "UpdateData.h"
#include "GridReference.h"
#include "ObjectDefines.h"
#include "GridDefines.h"
#include "Map.h"
#include "UpdateMask.h"


#define CONTACT_DISTANCE            0.5f
#define INTERACTION_DISTANCE        5.0f
#define ATTACK_DISTANCE             5.0f
#define MAX_VISIBILITY_DISTANCE     SIZE_OF_GRIDS           // max distance for visible objects
#define SIGHT_RANGE_UNIT            50.0f
#define DEFAULT_VISIBILITY_DISTANCE 90.0f                   // default visible distance, 90 yards on continents
#define DEFAULT_VISIBILITY_INSTANCE 170.0f                  // default visible distance in instances, 170 yards
#define DEFAULT_VISIBILITY_BGARENAS 533.0f                  // default visible distance in BG/Arenas, roughly 533 yards

#define DEFAULT_WORLD_OBJECT_SIZE   0.388999998569489f      // player size, also currently used (correctly?) for any non Unit world objects
#define DEFAULT_COMBAT_REACH        1.5f
#define MIN_MELEE_REACH             2.0f
#define NOMINAL_MELEE_RANGE         5.0f
#define MELEE_RANGE                 (NOMINAL_MELEE_RANGE - MIN_MELEE_REACH * 2) //center to center for players

enum TypeMask
{
    TYPEMASK_OBJECT         = 0x0001,
    TYPEMASK_ITEM           = 0x0002,
    TYPEMASK_CONTAINER      = 0x0006,                       // TYPEMASK_ITEM | 0x0004
    TYPEMASK_UNIT           = 0x0008, // creature
    TYPEMASK_PLAYER         = 0x0010,
    TYPEMASK_GAMEOBJECT     = 0x0020,
    TYPEMASK_DYNAMICOBJECT  = 0x0040,
    TYPEMASK_CORPSE         = 0x0080,
    TYPEMASK_AREATRIGGER    = 0x0100,
    TYPEMASK_SCENEOBJECT    = 0x0200,
    TYPEMASK_SEER           = TYPEMASK_UNIT|TYPEMASK_DYNAMICOBJECT|TYPEMASK_DYNAMICOBJECT
};

enum TypeID
{
    TYPEID_OBJECT        = 0,
    TYPEID_ITEM          = 1,
    TYPEID_CONTAINER     = 2,
    TYPEID_UNIT          = 3,
    TYPEID_PLAYER        = 4,
    TYPEID_GAMEOBJECT    = 5,
    TYPEID_DYNAMICOBJECT = 6,
    TYPEID_CORPSE        = 7,
    TYPEID_AREATRIGGER   = 8,
    TYPEID_SCENEOBJECT   = 9
};

#define NUM_CLIENT_OBJECT_TYPES             10

uint32 GuidHigh2TypeId(uint32 guid_hi);

enum TempSummonType
{
    TEMPSUMMON_TIMED_OR_DEAD_DESPAWN       = 1,             // despawns after a specified time OR when the creature disappears
    TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN     = 2,             // despawns after a specified time OR when the creature dies
    TEMPSUMMON_TIMED_DESPAWN               = 3,             // despawns after a specified time
    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT = 4,             // despawns after a specified time after the creature is out of combat
    TEMPSUMMON_CORPSE_DESPAWN              = 5,             // despawns instantly after death
    TEMPSUMMON_CORPSE_TIMED_DESPAWN        = 6,             // despawns after a specified time after death
    TEMPSUMMON_DEAD_DESPAWN                = 7,             // despawns when the creature disappears
    TEMPSUMMON_MANUAL_DESPAWN              = 8              // despawns when UnSummon() is called
};

enum PhaseMasks
{
    PHASEMASK_NORMAL   = 0x00000001,
    PHASEMASK_ANYWHERE = 0xFFFFFFFF
};

enum NotifyFlags
{
    NOTIFY_NONE                     = 0x00,
    NOTIFY_AI_RELOCATION            = 0x01,
    NOTIFY_VISIBILITY_CHANGED       = 0x02,
    NOTIFY_ALL                      = 0xFF
};

class WorldPacket;
class UpdateData;
class ByteBuffer;
class WorldSession;
class Creature;
class Player;
class UpdateMask;
class InstanceScript;
class GameObject;
class TempSummon;
class Vehicle;
class CreatureAI;
class ZoneScript;
class Unit;
class Transport;

typedef std::unordered_set<uint64> GuidUnorderedSet;
typedef std::unordered_map<Player*, UpdateData> UpdateDataMapType;

class DynamicFields
{
public:
    DynamicFields()
    {
        _dynamicValues = new uint32[Count];
        _dynamicChangedFields = new bool[Count];

        Clear();
    }

    ~DynamicFields()
    {
        delete[] _dynamicValues;
        _dynamicValues = NULL;
        delete[] _dynamicChangedFields;
        _dynamicChangedFields = NULL;
    }

    void Clear()
    {
        memset(_dynamicValues, 0, sizeof(uint32)*Count);
        memset(_dynamicChangedFields, 0, sizeof(bool)*Count);
    }

    void ClearMask()
    {
        memset(_dynamicChangedFields, 0, sizeof(bool)*Count);
    }

    uint32* _dynamicValues;
    bool* _dynamicChangedFields;

    const static uint32 Count = sizeof(uint32)*8;
};

class Object
{
    public:
        virtual ~Object();

        bool IsInWorld() const { return m_inWorld; }

        virtual void AddToWorld();
        virtual void RemoveFromWorld();

        uint64 GetGUID() const { return GetGuidValue(0); }
        uint32 GetGUIDLow() const { return GUID_LOPART(GetGuidValue(0)); }
        uint32 GetGUIDMid() const { return GUID_ENPART(GetGuidValue(0)); }
        uint32 GetGUIDHigh() const { return GUID_HIPART(GetGuidValue(0)); }
        ObjectGuid GetObjectGuid() const { return ObjectGuid(GetGuidValue(0)); }
        const ByteBuffer& GetPackGUID() const { return m_PackGUID; }
        uint32 GetEntry() const { return GetUInt32Value(OBJECT_FIELD_ENTRY_ID); }
        void SetEntry(uint32 entry) { SetUInt32Value(OBJECT_FIELD_ENTRY_ID, entry); }

        void SetObjectScale(float scale) { SetFloatValue(OBJECT_FIELD_SCALE, scale); }

        TypeID GetTypeId() const { return m_objectTypeId; }
        bool isType(uint16 mask) const { return (mask & m_objectType); }

        virtual void BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const;
        void SendUpdateToPlayer(Player* player);

        void BuildValuesUpdateBlockForPlayer(UpdateData* data, Player* target) const;
        void BuildOutOfRangeUpdateBlock(UpdateData* data) const;

        virtual void DestroyForPlayer(Player* target, bool onDeath = false) const;

        // GUIDS 128
        bool AddGuidValue(uint16 index, uint64 value);
        bool RemoveGuidValue(uint16 index, uint64 value);
        void SetGuidValue(uint16 index, uint64 value);
        uint64 GetGuidValue(uint16 index) const
        {
            if (!m_uint32Values)
                return uint64(0);

            return Guid128To64(Guid128(*((uint64*)&(m_uint32Values[index])), *((uint64*)&(m_uint32Values[index + 2]))));
        }

        int32 GetInt32Value(uint16 index) const
        {
            if (!m_int32Values)
                return int32(0);

            return m_int32Values[index];
        }

        uint32 GetUInt32Value(uint16 index) const
        {
            if (!m_uint32Values)
                return uint32(0);

            return m_uint32Values[index];
        }

        uint64 GetUInt64Value(uint16 index) const
        {
            if (!m_uint32Values)
                return uint64(0);

            return *((uint64*)&(m_uint32Values[index]));
        }

        float GetFloatValue(uint16 index) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            return m_floatValues[index];
        }

        uint8 GetByteValue(uint16 index, uint8 offset) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            ASSERT(offset < 4);
            return *(((uint8*)&m_uint32Values[index])+offset);
        }

        uint16 GetUInt16Value(uint16 index, uint8 offset) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            ASSERT(offset < 2);
            return *(((uint16*)&m_uint32Values[index])+offset);
        }

        void SetInt32Value(uint16 index, int32 value);
        void SetUInt32Value(uint16 index, uint32 value);
        void UpdateUInt32Value(uint16 index, uint32 value);
        void SetUInt64Value(uint16 index, uint64 value);
        void SetFloatValue(uint16 index, float value);
        void SetByteValue(uint16 index, uint8 offset, uint8 value);
        void SetUInt16Value(uint16 index, uint8 offset, uint16 value);
        void SetInt16Value(uint16 index, uint8 offset, int16 value) { SetUInt16Value(index, offset, (uint16)value); }
        void SetStatFloatValue(uint16 index, float value);
        void SetStatInt32Value(uint16 index, int32 value);

        bool AddUInt64Value(uint16 index, uint64 value);
        bool RemoveUInt64Value(uint16 index, uint64 value);

        void ApplyModUInt32Value(uint16 index, int32 val, bool apply);
        void ApplyModInt32Value(uint16 index, int32 val, bool apply);
        void ApplyModUInt64Value(uint16 index, int32 val, bool apply);
        void ApplyModPositiveFloatValue(uint16 index, float val, bool apply);
        void ApplyModSignedFloatValue(uint16 index, float val, bool apply);

        void ApplyPercentModFloatValue(uint16 index, float val, bool apply)
        {
            float value = GetFloatValue(index);
            ApplyPercentModFloatVar(value, val, apply);
            SetFloatValue(index, value);
        }

        void SetFlag(uint16 index, uint32 newFlag);
        void RemoveFlag(uint16 index, uint32 oldFlag);

        void ToggleFlag(uint16 index, uint32 flag)
        {
            if (HasFlag(index, flag))
                RemoveFlag(index, flag);
            else
                SetFlag(index, flag);
        }

        bool HasFlag(uint16 index, uint32 flag) const
        {
            if (index >= m_valuesCount && !PrintIndexError(index, false))
                return false;

            return (m_uint32Values[index] & flag) != 0;
        }

        void SetByteFlag(uint16 index, uint8 offset, uint8 newFlag);
        void RemoveByteFlag(uint16 index, uint8 offset, uint8 newFlag);

        void ToggleFlag(uint16 index, uint8 offset, uint8 flag)
        {
            if (HasByteFlag(index, offset, flag))
                RemoveByteFlag(index, offset, flag);
            else
                SetByteFlag(index, offset, flag);
        }

        bool HasByteFlag(uint16 index, uint8 offset, uint8 flag) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            ASSERT(offset < 4);
            return (((uint8*)&m_uint32Values[index])[offset] & flag) != 0;
        }

        void ApplyModFlag(uint16 index, uint32 flag, bool apply)
        {
            if (apply) SetFlag(index, flag); else RemoveFlag(index, flag);
        }

        void SetFlag64(uint16 index, uint64 newFlag)
        {
            uint64 oldval = GetUInt64Value(index);
            uint64 newval = oldval | newFlag;
            SetUInt64Value(index, newval);
        }

        void RemoveFlag64(uint16 index, uint64 oldFlag)
        {
            uint64 oldval = GetUInt64Value(index);
            uint64 newval = oldval & ~oldFlag;
            SetUInt64Value(index, newval);
        }

        void ToggleFlag64(uint16 index, uint64 flag)
        {
            if (HasFlag64(index, flag))
                RemoveFlag64(index, flag);
            else
                SetFlag64(index, flag);
        }

        bool HasFlag64(uint16 index, uint64 flag) const
        {
            ASSERT(index < m_valuesCount || PrintIndexError(index, false));
            return (GetUInt64Value(index) & flag) != 0;
        }

        void ApplyModFlag64(uint16 index, uint64 flag, bool apply)
        {
            if (apply) SetFlag64(index, flag); else RemoveFlag64(index, flag);
        }

        void ClearUpdateMask(bool remove);

        uint16 GetValuesCount() const { return m_valuesCount; }

        // Dynamic Field function
        std::vector<uint32> const& GetDynamicValues(uint16 index) const;
        uint32 GetDynamicValue(uint16 index, uint16 secondIndex) const;
        void AddDynamicValue(uint16 index, uint32 value);
        void RemoveDynamicValue(uint16 index, uint32 value);
        void ClearDynamicValue(uint16 index);
        void SetDynamicValue(uint16 index, uint8 offset, uint32 value);

        virtual bool hasQuest(uint32 /* quest_id */) const { return false; }
        virtual bool hasInvolvedQuest(uint32 /* quest_id */) const { return false; }
        virtual void BuildUpdate(UpdateDataMapType&) {}
        void BuildFieldsUpdate(Player*, UpdateDataMapType &) const;

        void SetFieldNotifyFlag(uint16 flag) { _fieldNotifyFlags |= flag; }
        void RemoveFieldNotifyFlag(uint16 flag) { _fieldNotifyFlags &= ~flag; }

        // FG: some hacky helpers
        void ForceValuesUpdateAtIndex(uint32);

        Player* ToPlayer() { if (IsPlayer()) return reinterpret_cast<Player*>(this); else return NULL; }
        Player const* ToPlayer() const { if (IsPlayer()) return reinterpret_cast<Player const*>(this); else return NULL; }
        bool IsPlayer() const { return m_objectTypeId == TYPEID_PLAYER; }

        Creature* ToCreature() { if (GetTypeId() == TYPEID_UNIT) return reinterpret_cast<Creature*>(this); else return NULL; }
        Creature const* ToCreature() const { if (GetTypeId() == TYPEID_UNIT) return reinterpret_cast<Creature const*>(this); else return NULL; }

        Unit* ToUnit() { if (isType(TYPEMASK_UNIT)) return reinterpret_cast<Unit*>(this); else return NULL; }
        Unit const* ToUnit() const { if (isType(TYPEMASK_UNIT)) return reinterpret_cast<Unit const*>(this); else return NULL; }

        GameObject* ToGameObject() { if (GetTypeId() == TYPEID_GAMEOBJECT) return reinterpret_cast<GameObject*>(this); else return NULL; }
        GameObject const* ToGameObject() const { if (GetTypeId() == TYPEID_GAMEOBJECT) return reinterpret_cast<GameObject const*>(this); else return NULL; }

        Corpse* ToCorpse() { if (GetTypeId() == TYPEID_CORPSE) return reinterpret_cast<Corpse*>(this); else return NULL; }
        Corpse const* ToCorpse() const { if (GetTypeId() == TYPEID_CORPSE) return reinterpret_cast<Corpse const*>(this); else return NULL; }

        DynamicObject* ToDynObject() { if (GetTypeId() == TYPEID_DYNAMICOBJECT) return reinterpret_cast<DynamicObject*>(this); else return NULL; }
        DynamicObject const* ToDynObject() const { if (GetTypeId() == TYPEID_DYNAMICOBJECT) return reinterpret_cast<DynamicObject const*>(this); else return NULL; }

        AreaTrigger* ToAreaTrigger() { if (GetTypeId() == TYPEID_AREATRIGGER) return reinterpret_cast<AreaTrigger*>(this); else return NULL; }
        AreaTrigger const* ToAreaTrigger() const { if (GetTypeId() == TYPEID_AREATRIGGER) return reinterpret_cast<AreaTrigger const*>(this); else return NULL; }

    protected:
        Object();

        void _InitValues();
        void _Create(uint32 guidlow, uint32 entry, HighGuid guidhigh);
        std::string _ConcatFields(uint16 startIndex, uint16 size) const;
        void _LoadIntoDataField(const char* p_Data, uint32 p_StartOffset, uint32 p_Count, bool p_Force);

        uint32 GetUpdateFieldData(Player const* target, uint32*& flags) const;
        uint32 GetDynamicUpdateFieldData(Player const* target, uint32*& flags) const;

        bool IsUpdateFieldVisible(uint32 flags, bool isSelf, bool isOwner, bool isItemOwner, bool isPartyMember) const;

        void BuildMovementUpdate(ByteBuffer * data, uint32 flags) const;
        virtual void BuildValuesUpdate(uint8 updatetype, ByteBuffer* data, Player* target) const;
        virtual void BuildDynamicValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const;

        uint16 m_objectType;

        TypeID m_objectTypeId;
        uint32 m_updateFlag;

        union
        {
            int32  *m_int32Values;
            uint32 *m_uint32Values;
            float  *m_floatValues;
        };

        uint16 m_valuesCount;

        uint16 _fieldNotifyFlags;

        bool m_objectUpdated;

        std::vector<uint32>* _dynamicValues;
        uint32 _dynamicValuesCount;
        UpdateMask _changesMask;
        UpdateMask _dynamicChangesMask;
        UpdateMask* _dynamicChangesArrayMask;

    private:
        bool m_inWorld;

        ByteBuffer m_PackGUID;

        // for output helpfull error messages from asserts
        bool PrintIndexError(uint32 index, bool set) const;
        Object(const Object&);                              // prevent generation copy constructor
        Object& operator=(Object const&);                   // prevent generation assigment operator
};

struct Position
{
    struct PositionXYZStreamer
    {
        explicit PositionXYZStreamer(Position& pos) : m_pos(&pos) {}
        Position* m_pos;
    };

    struct PositionXYZOStreamer
    {
        explicit PositionXYZOStreamer(Position& pos) : m_pos(&pos) {}
        Position* m_pos;
    };

    float m_positionX;
    float m_positionY;
    float m_positionZ;
// Better to limit access to m_orientation field, but this will be hard to achieve with many scripts using array initialization for this structure
//private:
    float m_orientation;
//public:

    bool operator==(Position const &a) const;

    inline bool operator!=(Position const &a) const
    {
        return !(operator==(a));
    }

    Position operator-(Position const& p_Rhs) const
    {
        Position l_Pos;
        l_Pos.m_positionX = m_positionX - p_Rhs.m_positionX;
        l_Pos.m_positionY = m_positionY - p_Rhs.m_positionY;
        l_Pos.m_positionZ = m_positionZ - p_Rhs.m_positionZ;

        return l_Pos;
    }

    Position operator+(Position const& p_Rhs) const
    {
        Position l_Pos;
        l_Pos.m_positionX = m_positionX + p_Rhs.m_positionX;
        l_Pos.m_positionY = m_positionY + p_Rhs.m_positionY;
        l_Pos.m_positionZ = m_positionZ + p_Rhs.m_positionZ;

        return l_Pos;
    }

    Position operator*(float p_Rhs) const
    {
        Position l_Pos;
        l_Pos.m_positionX = m_positionX * p_Rhs;
        l_Pos.m_positionY = m_positionY * p_Rhs;
        l_Pos.m_positionZ = m_positionZ * p_Rhs;

        return l_Pos;
    }

    Position operator/(float p_Rhs) const
    {
        Position l_Pos;
        l_Pos.m_positionX = m_positionX / p_Rhs;
        l_Pos.m_positionY = m_positionY / p_Rhs;
        l_Pos.m_positionZ = m_positionZ / p_Rhs;

        return l_Pos;
    }

    void Relocate(float x, float y)
        { m_positionX = x; m_positionY = y;}
    void Relocate(float x, float y, float z)
        { m_positionX = x; m_positionY = y; m_positionZ = z; }
    void Relocate(float x, float y, float z, float orientation)
        { m_positionX = x; m_positionY = y; m_positionZ = z; SetOrientation(orientation); }
    void Relocate(const Position &pos)
        { m_positionX = pos.m_positionX; m_positionY = pos.m_positionY; m_positionZ = pos.m_positionZ; SetOrientation(pos.m_orientation); }
    void Relocate(const Position* pos)
        { m_positionX = pos->m_positionX; m_positionY = pos->m_positionY; m_positionZ = pos->m_positionZ; SetOrientation(pos->m_orientation); }
    void RelocateOffset(const Position &offset);
    void SetOrientation(float orientation)
    { m_orientation = NormalizeOrientation(orientation); }

    float GetPositionX() const { return m_positionX; }
    float GetPositionY() const { return m_positionY; }
    float GetPositionZ() const { return m_positionZ; }
    float GetOrientation() const { return m_orientation; }

    void GetPosition(float &x, float &y) const
        { x = m_positionX; y = m_positionY; }
    void GetPosition(float &x, float &y, float &z) const
        { x = m_positionX; y = m_positionY; z = m_positionZ; }
    void GetPosition(float &x, float &y, float &z, float &o) const
        { x = m_positionX; y = m_positionY; z = m_positionZ; o = m_orientation; }
    void GetPosition(Position* pos) const
    {
        if (pos)
            pos->Relocate(m_positionX, m_positionY, m_positionZ, m_orientation);
    }

    Position::PositionXYZStreamer PositionXYZStream()
    {
        return PositionXYZStreamer(*this);
    }
    Position::PositionXYZOStreamer PositionXYZOStream()
    {
        return PositionXYZOStreamer(*this);
    }

    bool IsPositionValid() const;

    float GetExactDist2dSq(float x, float y) const
        { float dx = m_positionX - x; float dy = m_positionY - y; return dx*dx + dy*dy; }
    float GetExactDist2d(const float x, const float y) const
        { return sqrt(GetExactDist2dSq(x, y)); }
    float GetExactDist2dSq(const Position* pos) const
        { float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; return dx*dx + dy*dy; }
    float GetExactDist2d(const Position* pos) const
        { return sqrt(GetExactDist2dSq(pos)); }
    float GetExactDistSq(float x, float y, float z) const
        { float dz = m_positionZ - z; return GetExactDist2dSq(x, y) + dz*dz; }
    float GetExactDist(float x, float y, float z) const
        { return sqrt(GetExactDistSq(x, y, z)); }
    float GetExactDistSq(const Position* pos) const
        { float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; float dz = m_positionZ - pos->m_positionZ; return dx*dx + dy*dy + dz*dz; }
    float GetExactDist(const Position* pos) const
        { return sqrt(GetExactDistSq(pos)); }

    void GetPositionOffsetTo(const Position & endPos, Position & retOffset) const;

    float GetAngle(const Position* pos) const;
    float GetAngle(float x, float y) const;
    float GetRelativeAngle(const Position* pos) const
        { return GetAngle(pos) - m_orientation; }
    float GetRelativeAngle(float x, float y) const { return GetAngle(x, y) - m_orientation; }
    void GetSinCos(float x, float y, float &vsin, float &vcos) const;

    bool IsInDist2d(float x, float y, float dist) const
        { return GetExactDist2dSq(x, y) < dist * dist; }
    bool IsInDist2d(const Position* pos, float dist) const
        { return GetExactDist2dSq(pos) < dist * dist; }
    bool IsInDist(float x, float y, float z, float dist) const
        { return GetExactDistSq(x, y, z) < dist * dist; }
    bool IsInDist(const Position* pos, float dist) const
        { return GetExactDistSq(pos) < dist * dist; }
    bool HasInArc(float arcangle, const Position* pos) const;
    bool HasInLine(WorldObject const* target, float width) const;
    std::string ToString() const;

    // modulos a radian orientation to the range of 0..2PI
    static float NormalizeOrientation(float o)
    {
        if (o >= 0.0f && o < 6.2831864f)
            return o;

        // fmod only supports positive numbers. Thus we have
        // to emulate negative numbers
        if (o < 0)
        {
            float mod = -o;
            mod = fmod(mod, 2.0f * static_cast<float>(M_PI));
            mod = -mod + 2.0f * static_cast<float>(M_PI);
            return mod;
        }
        return fmod(o, 2.0f * static_cast<float>(M_PI));
    }

    static float NormalizePitch(float o)
    {
        if (o > -M_PI && o < M_PI)
            return o;

        o = NormalizeOrientation(o + M_PI) - M_PI;

        return o;
    }

    bool IsNearPosition(Position const* p_CheckPos, float p_Range) const
    {
        float l_PosX = GetPositionX();
        float l_PosY = GetPositionY();
        float l_PosZ = GetPositionZ();

        if ((l_PosX <= (p_CheckPos->m_positionX + p_Range) && l_PosX >= (p_CheckPos->m_positionX - p_Range)) &&
            (l_PosY <= (p_CheckPos->m_positionY + p_Range) && l_PosY >= (p_CheckPos->m_positionY - p_Range)) &&
            (l_PosZ <= (p_CheckPos->m_positionZ + p_Range) && l_PosZ >= (p_CheckPos->m_positionZ - p_Range)))
            return true;

        return false;
    }
};

static float dotProductXY(Position const& p_Pos1, Position const& p_Pos2)
{
    return p_Pos1.m_positionX * p_Pos2.m_positionX + p_Pos1.m_positionY * p_Pos2.m_positionY;
}

inline Position& normalizeXY(Position& p_Pos)
{
    float l_Norme = std::sqrt(dotProductXY(p_Pos, p_Pos));
    p_Pos.m_positionX /= l_Norme;
    p_Pos.m_positionY /= l_Norme;

    return p_Pos;
}

inline float DistanceFromLine(Position const& p_PointLine1, Position const& p_PointLine2, Position const& p_Point3)
{
    float l_x1 = p_PointLine1.GetPositionX();
    float l_x2 = p_PointLine2.GetPositionX();
    float l_y1 = p_PointLine1.GetPositionY();
    float l_y2 = p_PointLine2.GetPositionY();

    float l_dx = l_x2 - l_x1;
    float l_dy = l_y2 - l_y1;

    return std::abs(l_dy * p_Point3.GetPositionX() - l_dx * p_Point3.GetPositionY() - l_x1 * l_y2 + l_x2 * l_y1) / std::sqrt(l_dx * l_dx + l_dy * l_dy);
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer);
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer);

struct MovementInfo
{
    // common
    uint64 guid;
    uint32 flags;
    uint16 flags2;
    Position pos;
    uint32 time;
    // transport
    uint64 t_guid;
    Position t_pos;
    int8 t_seat;
    uint32 t_time;
    uint32 PrevMoveTime;
    uint32 VehicleRecID;
    // swimming/flying
    union
    {
        float pitch;
        uint32 HavePitch;
    };
    // falling
    uint32 fallTime;
    // jumping
    float JumpVelocity, j_cosAngle, j_sinAngle, j_xyspeed;
    // spline
    union
    {
        float splineElevation;
        uint32 HaveSplineElevation;
    };
    // BitClientData
    bool HasFallData;
    bool hasFallDirection;
    uint32 Alive32;

    MovementInfo()
    {
        pos.Relocate(0, 0, 0, 0);
        guid = 0;
        flags = 0;
        flags2 = 0;
        time = t_time = PrevMoveTime = VehicleRecID = fallTime = 0;
        splineElevation = 0.0f;
        pitch = JumpVelocity = j_sinAngle = j_cosAngle = j_xyspeed = 0.0f;
        t_guid = 0;
        t_pos.Relocate(0, 0, 0, 0);
        t_seat = -1;
        HasFallData = false;
        hasFallDirection = false;
        Alive32 = 0;
    }

    uint32 GetMovementFlags() const { return flags; }
    void SetMovementFlags(uint32 flag) { flags = flag; }
    void AddMovementFlag(uint32 flag) { flags |= flag; }
    void RemoveMovementFlag(uint32 flag) { flags &= ~flag; }
    bool HasMovementFlag(uint32 flag) const { return flags & flag; }

    uint16 GetExtraMovementFlags() const { return flags2; }
    void AddExtraMovementFlag(uint16 flag) { flags2 |= flag; }
    bool HasExtraMovementFlag(uint16 flag) const { return flags2 & flag; }

    void SetFallTime(uint32 time) { fallTime = time; }

    void OutDebug();
    void Normalize();

    void ResetJump()
    {
        fallTime = 0;
        JumpVelocity = j_cosAngle = j_sinAngle = j_xyspeed = 0.0f;
    }
};

#define MAPID_INVALID 0xFFFFFFFF

class WorldLocation : public Position
{
    public:
        explicit WorldLocation(uint32 _mapid = MAPID_INVALID, float _x = 0, float _y = 0, float _z = 0, float _o = 0)
            : m_mapId(_mapid) { Relocate(_x, _y, _z, _o); }
        WorldLocation(const WorldLocation &loc) { WorldRelocate(loc); }

        void WorldRelocate(const WorldLocation &loc)
            { m_mapId = loc.GetMapId(); Relocate(loc); }
        uint32 GetMapId() const { return m_mapId; }

        uint32 m_mapId;
};

template<class T>
class GridObject
{
    public:
        bool IsInGrid() const { return _gridRef.isValid(); }
        void AddToGrid(GridRefManager<T>& m) { ASSERT(!IsInGrid()); _gridRef.link(&m, (T*)this); }
        void RemoveFromGrid() { ASSERT(IsInGrid()); _gridRef.unlink(); }
    private:
        GridReference<T> _gridRef;
};

template <class T_VALUES, class T_FLAGS, class FLAG_TYPE, uint8 ARRAY_SIZE>
class FlaggedValuesArray32
{
    public:
        FlaggedValuesArray32()
        {
            memset(&m_values, 0x00, sizeof(T_VALUES) * ARRAY_SIZE);
            m_flags = 0;
        }

        T_FLAGS  GetFlags() const { return m_flags; }
        bool     HasFlag(FLAG_TYPE flag) const { return m_flags & (1 << flag); }
        void     AddFlag(FLAG_TYPE flag) { m_flags |= (1 << flag); }
        void     DelFlag(FLAG_TYPE flag) { m_flags &= ~(1 << flag); }

        T_VALUES GetValue(FLAG_TYPE flag) const { return m_values[flag]; }
        void     SetValue(FLAG_TYPE flag, T_VALUES value) { m_values[flag] = value; }
        void     AddValue(FLAG_TYPE flag, T_VALUES value) { m_values[flag] += value; }

    private:
        T_VALUES m_values[ARRAY_SIZE];
        T_FLAGS m_flags;
};

enum MapObjectCellMoveState
{
    MAP_OBJECT_CELL_MOVE_NONE, //not in move list
    MAP_OBJECT_CELL_MOVE_ACTIVE, //in move list
    MAP_OBJECT_CELL_MOVE_INACTIVE //in move list but should not move
};

class MapObject
{
        friend class Map; //map for moving creatures
        friend class ObjectGridLoader; //grid loader for loading creatures

    protected:
        MapObject() : _moveState(MAP_OBJECT_CELL_MOVE_NONE) {}

    private:
        Cell _currentCell;
        Cell const& GetCurrentCell() const { return _currentCell; }
        void SetCurrentCell(Cell const& cell) { _currentCell = cell; }

        MapObjectCellMoveState _moveState;
        Position _newPosition;
        void SetNewCellPosition(float x, float y, float z, float o)
        {
            _moveState = MAP_OBJECT_CELL_MOVE_ACTIVE;
            _newPosition.Relocate(x, y, z, o);
        }
};

class WorldObject : public Object, public WorldLocation
{
    protected:
        explicit WorldObject(bool isWorldObject); //note: here it means if it is in grid object list or world object list
    public:
        virtual ~WorldObject();

        virtual void Update (uint32 /*time_diff*/) { }

        void _Create(uint32 guidlow, HighGuid guidhigh, uint32 phaseMask);

        virtual void RemoveFromWorld()
        {
            if (!IsInWorld())
                return;

            DestroyForNearbyPlayers();

            Object::RemoveFromWorld();
        }

        void GetNearPoint2D(float &x, float &y, float distance, float absAngle) const;
        void GetNearPoint(WorldObject const* p_Searcher, float &p_InOutX, float &p_InOutY, float &p_InOutZ, float p_SearcherSize, float p_Distance2D, float p_AbsAngle) const;
        void GetClosePoint(float &x, float &y, float &z, float size, float distance2d = 0, float angle = 0) const
        {
            // angle calculated from current orientation
            GetNearPoint(NULL, x, y, z, size, distance2d, GetOrientation() + angle);
        }
        void MovePosition(Position &pos, float dist, float angle);
        void GetNearPosition(Position &pos, float dist, float angle)
        {
            GetPosition(&pos);
            MovePosition(pos, dist, angle);
        }
        void MovePositionToFirstCollision(Position &pos, float dist, float angle);
        void GetFirstCollisionPosition(Position &pos, float dist, float angle)
        {
            GetPosition(&pos);
            MovePositionToFirstCollision(pos, dist, angle);
        }
        void MovePositionToCollisionBetween(Position &pos, float distMin, float distMax, float angle);
        void GetCollisionPositionBetween(Position &pos, float distMin, float distMax, float angle)
        {
            GetPosition(&pos);
            MovePositionToCollisionBetween(pos, distMin, distMax, angle);
        }
        void GetRandomNearPosition(Position &pos, float radius)
        {
            GetPosition(&pos);
            MovePosition(pos, radius * (float)rand_norm(), (float)rand_norm() * static_cast<float>(2 * M_PI));
        }

        void GetContactPoint(const WorldObject* obj, float &x, float &y, float &z, float distance2d = CONTACT_DISTANCE) const
        {
            // angle to face `obj` to `this` using distance includes size of `obj`
            GetNearPoint(obj, x, y, z, obj->GetObjectSize(), distance2d, GetAngle(obj));
        }

        float GetObjectSize() const;

        void UpdateGroundPositionZ(float x, float y, float &z) const;
        void UpdateAllowedPositionZ(float x, float y, float &z) const;

        void GetRandomPoint(const Position &srcPos, float distance, float &rand_x, float &rand_y, float &rand_z) const;
        void GetRandomPoint(const Position &srcPos, float distance, Position &pos) const
        {
            float x, y, z;
            GetRandomPoint(srcPos, distance, x, y, z);
            pos.Relocate(x, y, z, GetOrientation());
        }

        uint32 GetInstanceId() const { return m_InstanceId; }

        virtual void SetPhaseMask(uint32 newPhaseMask, bool update);
        uint32 GetPhaseMask() const { return m_phaseMask; }
        bool InSamePhase(WorldObject const* obj) const { return InSamePhase(obj->GetPhaseMask()); }
        bool InSamePhase(uint32 phasemask) const { return (GetPhaseMask() & phasemask); }

        virtual uint32 GetZoneId(bool forceRecalc = false) const;
        virtual uint32 GetAreaId(bool forceRecalc = false) const;
        virtual void GetZoneAndAreaId(uint32& zoneid, uint32& areaid, bool forceRecalc = false) const;

        InstanceScript* GetInstanceScript();

        const char* GetName() const { return m_name.c_str(); }
        size_t GetNameLength() const { return m_name.length(); }
        void SetName(const std::string& newname) { m_name=newname; }

        virtual const char* GetNameForLocaleIdx(LocaleConstant /*locale_idx*/) const { return GetName(); }

        float GetDistance(const WorldObject* obj) const
        {
            float d = GetExactDist(obj) - GetObjectSize() - obj->GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance(const Position &pos) const
        {
            float d = GetExactDist(&pos) - GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance(float x, float y, float z) const
        {
            float d = GetExactDist(x, y, z) - GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance2d(const WorldObject* obj) const
        {
            float d = GetExactDist2d(obj) - GetObjectSize() - obj->GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistance2d(float x, float y) const
        {
            float d = GetExactDist2d(x, y) - GetObjectSize();
            return d > 0.0f ? d : 0.0f;
        }
        float GetDistanceZ(const WorldObject* obj) const;

        bool IsSelfOrInSameMap(const WorldObject* obj) const
        {
            if (this == obj)
                return true;
            return IsInMap(obj);
        }
        bool IsInMap(const WorldObject* obj) const
        {
            if (obj)
                return IsInWorld() && obj->IsInWorld() && (GetMap() == obj->GetMap());
            return false;
        }
        bool IsWithinDist3d(float x, float y, float z, float dist) const
            { return IsInDist(x, y, z, dist + GetObjectSize()); }
        bool IsWithinDist3d(const Position* pos, float dist) const
            { return IsInDist(pos, dist + GetObjectSize()); }
        bool IsWithinDist3d(Position const* p_Pos, float p_Dist, WorldObject const* p_Target = nullptr)
        {
            return IsInDist(p_Pos, p_Dist + (p_Target != nullptr ? (GetObjectSize() + p_Target->GetObjectSize()) : GetObjectSize()));
        }
        bool IsWithinDist2d(float x, float y, float dist) const
            { return IsInDist2d(x, y, dist + GetObjectSize()); }
        bool IsWithinDist2d(const Position* pos, float dist) const
            { return IsInDist2d(pos, dist + GetObjectSize()); }
        // use only if you will sure about placing both object at same map
        bool IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D = true) const
        {
            return obj && _IsWithinDist(obj, dist2compare, is3D);
        }
        bool IsWithinDistInMap(WorldObject const* obj, float dist2compare, bool is3D = true) const
        {
            return obj && IsInMap(obj) && InSamePhase(obj) && _IsWithinDist(obj, dist2compare, is3D);
        }
        bool IsWithinLOS(float x, float y, float z) const;
        bool IsWithinLOSInMap(const WorldObject* obj) const;
        bool GetDistanceOrder(WorldObject const* obj1, WorldObject const* obj2, bool is3D = true) const;
        bool IsInRange(WorldObject const* obj, float minRange, float maxRange, bool is3D = true, bool useSizeFactor = true) const;
        bool IsInRange2d(float x, float y, float minRange, float maxRange) const;
        bool IsInRange3d(float x, float y, float z, float minRange, float maxRange) const;
        bool isInFront(WorldObject const* target, float arc = M_PI) const;
        bool isInBack(WorldObject const* target, float arc = M_PI) const;

        bool IsInBetween(const WorldObject* obj1, const WorldObject* obj2, float size = 0) const;
        bool IsInAxe(const WorldObject* obj1, const WorldObject* obj2, float size = 0) const;
        bool IsInAxe(WorldObject const* p_Object, float p_Width, float p_Range) const;
        bool IsInElipse(const WorldObject* p_Obj1, const WorldObject* p_Obj2, float p_With, float p_Thickness) const;

        virtual void CleanupsBeforeDelete(bool finalCleanup = true);  // used in destructor or explicitly before mass creature delete to remove cross-references to already deleted units

        virtual void SendMessageToSet(WorldPacket* data, bool self, const GuidUnorderedSet& p_IgnoredList = GuidUnorderedSet());
        virtual void SendMessageToSetInRange(WorldPacket* data, float dist, bool self, const GuidUnorderedSet& p_IgnoredList = GuidUnorderedSet());
        virtual void SendMessageToSet(WorldPacket* data, Player const* skipped_rcvr, const GuidUnorderedSet& p_IgnoredList = GuidUnorderedSet());

        virtual uint8 getLevelForTarget(WorldObject const* /*target*/) const { return 1; }

        void MonsterSay(const char* text, uint32 language, uint64 TargetGuid);
        void MonsterYell(const char* text, uint32 language, uint64 TargetGuid);
        void MonsterTextEmote(const char* text, uint64 TargetGuid, bool IsBossEmote = false);
        void MonsterWhisper(const char* text, uint64 receiver, bool IsBossWhisper = false);
        void MonsterSay(int32 textId, uint32 language, uint64 TargetGuid);
        void MonsterYell(int32 textId, uint32 language, uint64 TargetGuid);
        void MonsterTextEmote(int32 textId, uint64 TargetGuid, bool IsBossEmote = false);
        void MonsterWhisper(int32 textId, uint64 receiver, bool IsBossWhisper = false);
        void MonsterYellToZone(int32 textId, uint32 language, uint64 TargetGuid);
        void BuildMonsterChat(WorldPacket* data, uint8 msgtype, char const* text, uint32 language, char const* name, uint64 TargetGuid) const;

        void PlayDistanceSound(WorldObject * p_SourceObject, uint32 p_SoundKitID, WorldObject * p_TargetObject = NULL, float p_SourceX = 0.0f, float p_SourceY = 0.0f, float p_SourceZ = 0.0f);
        void PlayDirectSound(uint32 sound_id, Player* target = NULL);

        void SendObjectDeSpawnAnim(uint64 guid);

        virtual void SaveRespawnTime() {}
        void AddObjectToRemoveList();

        float GetGridActivationRange() const;
        virtual float GetVisibilityRange() const;
        float GetSightRange(const WorldObject* target = NULL) const;
        bool canSeeOrDetect(WorldObject const* obj, bool ignoreStealth = false, bool distanceCheck = false) const;

        FlaggedValuesArray32<int32, uint32, StealthType, TOTAL_STEALTH_TYPES> m_stealth;
        FlaggedValuesArray32<int32, uint32, StealthType, TOTAL_STEALTH_TYPES> m_stealthDetect;

        FlaggedValuesArray32<int32, uint32, InvisibilityType, TOTAL_INVISIBILITY_TYPES> m_invisibility;
        FlaggedValuesArray32<int32, uint32, InvisibilityType, TOTAL_INVISIBILITY_TYPES> m_invisibilityDetect;

        FlaggedValuesArray32<int32, uint32, ServerSideVisibilityType, TOTAL_SERVERSIDE_VISIBILITY_TYPES> m_serverSideVisibility;
        FlaggedValuesArray32<int32, uint32, ServerSideVisibilityType, TOTAL_SERVERSIDE_VISIBILITY_TYPES> m_serverSideVisibilityDetect;

        // Low Level Packets
        void SendPlaySound(uint32 Sound, bool OnlySelf);

        virtual void SetMap(Map* map);
        virtual void ResetMap();
        Map* GetMap() const  { return m_currMap; }
        Map* FindMap() const { return m_currMap; }
        //used to check all object's GetMap() calls when object is not in world!

        //this function should be removed in nearest time...
        Map const* GetBaseMap() const;

        void SetZoneScript();
        ZoneScript* GetZoneScript() const { return m_zoneScript; }

        TempSummon* SummonCreature(uint32 id, const Position &pos, TempSummonType spwtype = TEMPSUMMON_MANUAL_DESPAWN, uint32 despwtime = 0, uint32 vehId = 0, uint64 viewerGuid = 0, std::list<uint64>* viewersList = NULL) const;
        TempSummon* SummonCreature(uint32 id, float x, float y, float z, float ang = 0, TempSummonType spwtype = TEMPSUMMON_MANUAL_DESPAWN, uint32 despwtime = 0, uint64 viewerGuid = 0, std::list<uint64>* viewersList = NULL)
        {
            if (!x && !y && !z)
            {
                GetClosePoint(x, y, z, GetObjectSize());
                ang = GetOrientation();
            }
            Position pos;
            pos.Relocate(x, y, z, ang);
            return SummonCreature(id, pos, spwtype, despwtime, 0, viewerGuid, viewersList);
        }
        GameObject* SummonGameObject(uint32 entry, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime, uint64 viewerGuid = 0, std::list<uint64>* viewersList = NULL, uint32 p_AnimProgress = 100, uint32 p_GoHealth = 0, bool p_GarrisonPlotObject = false, bool p_Active = false);
        GameObject* SummonGameObject(uint32 p_Entry, Position const& p_Pos, float p_Rot0, float p_Rot1, float p_Rot2, float p_Rot3, uint32 p_RespTime, uint64 p_ViewerGuid = 0, std::list<uint64>* p_ViewerList = nullptr, uint32 p_AnimProgress = 100, uint32 p_GoHealth = 0, bool p_GarrisonPlotObject = false, bool p_Active = false);
        Creature*   SummonTrigger(float x, float y, float z, float ang, uint32 dur, CreatureAI* (*GetAI)(Creature*) = NULL);
        void SummonCreatureGroup(uint8 group, std::list<TempSummon*>& list);

        Creature*   FindNearestCreature(uint32 entry, float range, bool alive = true) const;
        GameObject* FindNearestGameObject(uint32 entry, float range) const;
        GameObject* FindNearestGameObject(float p_Range) const;
        GameObject* FindNearestGameObjectOfType(GameobjectTypes type, float range) const;
        Player*     FindNearestPlayer(float range, bool alive = true);
        AreaTrigger* FindNearestAreaTrigger(uint32 p_SpellID, float p_Range) const;

        void GetGameObjectListWithEntryInGrid(std::list<GameObject*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetCreatureListWithEntryInGrid(std::list<Creature*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetCreatureListInGrid(std::list<Creature*>& lList, float fMaxSearchRange) const;
        void GetPlayerListInGrid(std::list<Player*>& lList, float fMaxSearchRange, bool p_Self = false) const;

        void GetGameObjectListWithEntryInGridAppend(std::list<GameObject*>& lList, uint32 uiEntry, float fMaxSearchRange) const;
        void GetCreatureListWithEntryInGridAppend(std::list<Creature*>& lList, uint32 uiEntry, float fMaxSearchRange) const;

        void DestroyForNearbyPlayers();
        virtual void UpdateObjectVisibility(bool forced = true);
        void BuildUpdate(UpdateDataMapType&);

        bool isActiveObject() const { return m_isActive; }
        void setActive(bool isActiveObject);
        void SetWorldObject(bool apply);
        bool IsPermanentWorldObject() const { return m_isWorldObject; }
        bool IsWorldObject() const;

        template<class NOTIFIER> void VisitNearbyObject(const float &radius, NOTIFIER &notifier, bool loadGrids = false) const { if (IsInWorld()) GetMap()->VisitAll(GetPositionX(), GetPositionY(), radius, notifier, loadGrids); }
        template<class NOTIFIER> void VisitNearbyGridObject(const float &radius, NOTIFIER &notifier, bool loadGrids = false) const { if (IsInWorld()) GetMap()->VisitGrid(GetPositionX(), GetPositionY(), radius, notifier, loadGrids); }
        template<class NOTIFIER> void VisitNearbyWorldObject(const float &radius, NOTIFIER &notifier, bool loadGrids = false) const { if (IsInWorld()) GetMap()->VisitWorld(GetPositionX(), GetPositionY(), radius, notifier, loadGrids); }
#ifdef MAP_BASED_RAND_GEN
        int32 irand(int32 min, int32 max) const     { return int32 (GetMap()->mtRand.randInt(max - min)) + min; }
        uint32 urand(uint32 min, uint32 max) const  { return GetMap()->mtRand.randInt(max - min) + min;}
        int32 rand32() const                        { return GetMap()->mtRand.randInt();}
        double rand_norm() const                    { return GetMap()->mtRand.randExc();}
        double rand_chance() const                  { return GetMap()->mtRand.randExc(100.0);}
#endif

        uint32  LastUsedScriptID;

        // Transports
        Transport* GetTransport() const { return m_transport; }
        virtual uint64 GetTransGUID()   const;
        void SetTransport(Transport* t) { m_transport = t; }

        MovementInfo m_movementInfo;
        uint32 m_movementInfoLastTime;

        float GetTransOffsetX() const { return m_movementInfo.t_pos.GetPositionX(); }
        float GetTransOffsetY() const { return m_movementInfo.t_pos.GetPositionY(); }
        float GetTransOffsetZ() const { return m_movementInfo.t_pos.GetPositionZ(); }
        float GetTransOffsetO() const { return m_movementInfo.t_pos.GetOrientation(); }
        uint32 GetTransTime()   const { return m_movementInfo.t_time; }
        int8 GetTransSeat()     const { return m_movementInfo.t_seat; }

        virtual float GetStationaryX() const { return GetPositionX(); }
        virtual float GetStationaryY() const { return GetPositionY(); }
        virtual float GetStationaryZ() const { return GetPositionZ(); }
        virtual float GetStationaryO() const { return GetOrientation(); }

        uint16 GetAIAnimKitId() const { return m_AIAnimKitId; }
        void SetAIAnimKitId(uint16 animKitId, bool p_Packet = true);
        uint16 GetMovementAnimKitId() const { return m_MovementAnimKitId; }
        void SetMovementAnimKitId(uint16 animKitId);
        uint16 GetMeleeAnimKitId() const { return m_MeleeAnimKitId; }
        void SetMeleeAnimKitId(uint16 animKitId);

        void SetAnimTier(uint32 p_Tier);
        void SetPlayerHoverAnim(bool p_Apply);

        // Personal visibility system
        bool MustBeVisibleOnlyForSomePlayers() const { return !_visibilityPlayerList.empty(); }
        void GetMustBeVisibleForPlayersList(std::list<uint64/* guid*/>& playerList) { playerList = _visibilityPlayerList; }

        bool IsPlayerInPersonnalVisibilityList(uint64 guid) const;
        void AddPlayerInPersonnalVisibilityList(uint64 guid) { if (IS_PLAYER_GUID(guid)) _visibilityPlayerList.push_back(guid); }
        void AddPlayersInPersonnalVisibilityList(std::list<uint64> viewerList);
        void RemovePlayerFromPersonnalVisibilityList(uint64 guid) { if (IS_PLAYER_GUID(guid)) _visibilityPlayerList.remove(guid); }

        mutable uint32 m_lastEntrySummon;
        mutable uint32 m_summonCounter;

        ZoneScript* m_zoneScript;
    protected:
        std::string m_name;
        bool m_isActive;
        const bool m_isWorldObject;

        // transports
        Transport* m_transport;

        //these functions are used mostly for Relocate() and Corpse/Player specific stuff...
        //use them ONLY in LoadFromDB()/Create() funcs and nowhere else!
        //mapId/instanceId should be set in SetMap() function!
        void SetLocationMapId(uint32 _mapId) { m_mapId = _mapId; }
        void SetLocationInstanceId(uint32 _instanceId) { m_InstanceId = _instanceId; }

        virtual bool IsNeverVisible() const { return !IsInWorld(); }
        virtual bool IsAlwaysVisibleFor(WorldObject const* /*seer*/) const { return false; }
        virtual bool IsInvisibleDueToDespawn() const { return false; }
        //difference from IsAlwaysVisibleFor: 1. after distance check; 2. use owner or charmer as seer
        virtual bool IsAlwaysDetectableFor(WorldObject const* /*seer*/) const { return false; }

    private:
        Map* m_currMap;                                    //current object's Map location

        //uint32 m_mapId;                                     // object at map with map_id
        uint32 m_InstanceId;                                // in map copy with instance id
        uint32 m_phaseMask;                                 // in area phase state

        std::list<uint64/* guid*/> _visibilityPlayerList;

        virtual bool _IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D) const;

        bool CanNeverSee(WorldObject const* obj) const { return GetMap() != obj->GetMap() || !InSamePhase(obj); }
        virtual bool CanAlwaysSee(WorldObject const* /*obj*/) const { return false; }
        bool CanDetect(WorldObject const* obj, bool ignoreStealth) const;
        bool CanDetectInvisibilityOf(WorldObject const* obj) const;
        bool CanDetectStealthOf(WorldObject const* obj) const;

        uint16 m_AIAnimKitId;
        uint16 m_MovementAnimKitId;
        uint16 m_MeleeAnimKitId;
};

namespace JadeCore
{
    template<class T>
    void RandomResizeList(std::list<T> &_list, uint32 _size)
    {
        while (_list.size() > _size)
        {
            typename std::list<T>::iterator itr = _list.begin();
            advance(itr, urand(0, _list.size() - 1));
            _list.erase(itr);
        }
    }
    template<class T, class Predicate>
    void RandomResizeList(std::list<T> &list, Predicate& predicate, uint32 size)
    {
        //! First use predicate filter
        std::list<T> listCopy;
        for (typename std::list<T>::iterator itr = list.begin(); itr != list.end(); ++itr)
            if (predicate(*itr))
                listCopy.push_back(*itr);

        if (size)
            RandomResizeList(listCopy, size);

        list = listCopy;
    }

    // Binary predicate to sort WorldObjects based on the distance to a reference WorldObject
    class ObjectDistanceOrderPred
    {
        public:
            ObjectDistanceOrderPred(const WorldObject* pRefObj, bool ascending = true) : m_refObj(pRefObj), m_ascending(ascending) {}
            bool operator()(const WorldObject* pLeft, const WorldObject* pRight) const
            {
                return m_ascending ? m_refObj->GetDistanceOrder(pLeft, pRight) : !m_refObj->GetDistanceOrder(pLeft, pRight);
            }
        private:
            const WorldObject* m_refObj;
            const bool m_ascending;
    };

    /// Binary predicate to sort WorldObjects based on the distance to a reference Position
    class PositionDistanceOrderPred
    {
        public:
            PositionDistanceOrderPred(Position const p_RefPsos, bool p_Ascending = true) : m_RefPos(p_RefPsos), m_Ascending(p_Ascending) { }

            bool operator()(WorldObject const* pLeft, WorldObject const* pRight) const
            {
                float l_LeftDist = pLeft->GetDistance(m_RefPos);
                float l_RightDist = pRight->GetDistance(m_RefPos);

                return m_Ascending ? l_LeftDist < l_RightDist : l_LeftDist > l_RightDist;
            }

        private:
            Position const m_RefPos;
            bool const m_Ascending;
    };
}

#endif
