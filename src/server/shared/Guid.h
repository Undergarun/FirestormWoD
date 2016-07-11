////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_GUID_H
# define TRINITY_GUID_H

# include "Common.h"

enum HighGuid
{
    HIGHGUID_ITEM               = 0x400,                        // blizz 4000
    HIGHGUID_CONTAINER          = 0x400,                        // blizz 4000
    HIGHGUID_PLAYER             = 0x018,                        // blizz 0018
    HIGHGUID_GAMEOBJECT         = 0xF11,                        // blizz F110
    HIGHGUID_TRANSPORT          = 0xF12,                        // blizz F120 (for GAMEOBJECT_TYPE_TRANSPORT)
    HIGHGUID_UNIT               = 0xF13,                        // blizz F130
    HIGHGUID_PET                = 0xF14,                        // blizz F140
    HIGHGUID_VEHICLE            = 0xF15,                        // blizz F550
    HIGHGUID_DYNAMICOBJECT      = 0xF10,                        // blizz F100
    HIGHGUID_CORPSE             = 0xF101,                       // blizz F100
    HIGHGUID_AREATRIGGER        = 0xF1B,                        // blizz F100
    HIGHGUID_TYPE_BATTLEGROUND  = 0x1F1,                        // new 4.x
    HIGHGUID_MO_TRANSPORT       = 0x1FC,                        // blizz 1FC0 (for GAMEOBJECT_TYPE_MO_TRANSPORT)
    HIGHGUID_GROUP              = 0x1F5,
    HIGHGUID_GUILD              = 0x1FF,                        // new 4.x
    HIGHGUID_INSTANCE_SAVE      = 0x104,                        // new 5.x
    HIGHGUID_LOOT               = 0xF19,                        // new 5.4.x
    HIGHGUID_SCENEOBJECT        = 0x18F,
    HIGHGUID_BNET_ACCOUNT       = 0xE4E,
    HIGHGUID_WOW_ACCOUNT        = 0xD4E,
    HIGHGUID_VIGNETTE           = 0x200,
    HIGHGUID_BATTLE_PET         = 0x201,
    HIGHGUID_CONVERSATION       = 0x202,
    HIGHGUID_MAIL               = 0x800,                        // Custom, used for cross <-> local realm sync
    HIGHGUID_PET_NUMBER         = 0x900,                        // Custom, used for cross <-> local realm sync
    HIGHGUID_EQUIPMENT_SET      = 0x700,
};

# define IS_EMPTY_GUID(Guid)          (Guid == 0)

# define IS_CREATURE_GUID(Guid)       (GUID_HIPART(Guid) == HIGHGUID_UNIT)
# define IS_PET_GUID(Guid)            (GUID_HIPART(Guid) == HIGHGUID_PET)
# define IS_VEHICLE_GUID(Guid)        (GUID_HIPART(Guid) == HIGHGUID_VEHICLE)
# define IS_CRE_OR_VEH_GUID(Guid)     (IS_CREATURE_GUID(Guid) || IS_VEHICLE_GUID(Guid))
# define IS_CRE_OR_VEH_OR_PET_GUID(Guid)(IS_CRE_OR_VEH_GUID(Guid) || IS_PET_GUID(Guid))
# define IS_PLAYER_GUID(Guid)         (GUID_HIPART(Guid) == HIGHGUID_PLAYER && Guid != 0)
# define IS_GUILD_GUID(Guid)          (GUID_HIPART(Guid) == HIGHGUID_GUILD && Guid != 0)
# define IS_UNIT_GUID(Guid)           (IS_CRE_OR_VEH_OR_PET_GUID(Guid) || IS_PLAYER_GUID(Guid))
// special case for empty guid need check
# define IS_ITEM_GUID(Guid)           (GUID_HIPART(Guid) == HIGHGUID_ITEM)
# define IS_GAMEOBJECT_GUID(Guid)     (GUID_HIPART(Guid) == HIGHGUID_GAMEOBJECT)
# define IS_DYNAMICOBJECT_GUID(Guid)  (GUID_HIPART(Guid) == HIGHGUID_DYNAMICOBJECT)
# define IS_CORPSE_GUID(Guid)         (GUID_HIPART(Guid) == HIGHGUID_CORPSE)
# define IS_TRANSPORT(Guid)           (GUID_HIPART(Guid) == HIGHGUID_TRANSPORT)
# define IS_MO_TRANSPORT(Guid)        (GUID_HIPART(Guid) == HIGHGUID_MO_TRANSPORT)
# define IS_GROUP(Guid)               (GUID_HIPART(Guid) == HIGHGUID_GROUP)
# define IS_GUILD(Guid)               (GUID_HIPART(Guid) == HIGHGUID_GUILD)
# define IS_AREATRIGGER(Guid)         (GUID_HIPART(Guid) == HIGHGUID_AREATRIGGER)
# define IS_CONVERSATION(Guid)        (GUID_HIPART(Guid) == HIGHGUID_CONVERSATION)

// l - OBJECT_FIELD_GUID
// e - OBJECT_FIELD_ENTRY for GO (except GAMEOBJECT_TYPE_MO_TRANSPORT) and creatures or UNIT_FIELD_PETNUMBER for pets
// h - OBJECT_FIELD_GUID + 1
inline uint64 MAKE_NEW_GUID(uint64 l, uint64 e, uint64 h)
{
    if (!l)
        return 0;

    return uint64(uint64(l) | (uint64(e) << 32) | (uint64(h) << ((h == HIGHGUID_CORPSE || h == HIGHGUID_AREATRIGGER) ? 48 : 52)));
}
//#define MAKE_NEW_GUID(l, e, h)   uint64(uint64(l) | (uint64(e) << 32) | (uint64(h) << ((h == HIGHGUID_GUILD || h == HIGHGUID_CORPSE) ? 48 : 52)))

//#define GUID_HIPART(x)   (uint32)((uint64(x) >> 52)) & 0x0000FFFF)
inline uint32 GUID_HIPART(uint64 guid)
{
    uint32 t = ((uint64(guid) >> 48) & 0x0000FFFF);
    return (t == HIGHGUID_CORPSE || t == HIGHGUID_AREATRIGGER) ? t : ((uint32(t) >> 4) & 0x00000FFF);
}

// We have different low and middle part size for different guid types
# define _GUID_ENPART_2(x) 0
# define _GUID_ENPART_3(x) (uint32)((uint64(x) >> 32) & UI64LIT(0x00000000000FFFFF))
# define _GUID_LOPART_2(x) (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))
# define _GUID_LOPART_3(x) (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))

inline bool IsGuidHaveEnPart(uint64 guid)
{
    switch (GUID_HIPART(guid))
    {
        case HIGHGUID_ITEM:
        case HIGHGUID_PLAYER:
        case HIGHGUID_DYNAMICOBJECT:
        case HIGHGUID_CORPSE:
        case HIGHGUID_GROUP:
        case HIGHGUID_GUILD:
        case HIGHGUID_CONVERSATION:
            return false;

        case HIGHGUID_GAMEOBJECT:
        case HIGHGUID_TRANSPORT:
        case HIGHGUID_UNIT:
        case HIGHGUID_PET:
        case HIGHGUID_VEHICLE:
        case HIGHGUID_MO_TRANSPORT:
        case HIGHGUID_AREATRIGGER:
        default:
            return true;
    }
}

# define GUID_ENPART(x) (IsGuidHaveEnPart(x) ? _GUID_ENPART_3(x) : _GUID_ENPART_2(x))
# define GUID_LOPART(x) (IsGuidHaveEnPart(x) ? _GUID_LOPART_3(x) : _GUID_LOPART_2(x))

inline char const* GetLogNameForGuid(uint64 guid)
{
    switch (GUID_HIPART(guid))
    {
        case HIGHGUID_ITEM:         return "item";
        case HIGHGUID_PLAYER:       return guid ? "player" : "none";
        case HIGHGUID_GAMEOBJECT:   return "gameobject";
        case HIGHGUID_TRANSPORT:    return "transport";
        case HIGHGUID_UNIT:         return "creature";
        case HIGHGUID_PET:          return "pet";
        case HIGHGUID_VEHICLE:      return "vehicle";
        case HIGHGUID_DYNAMICOBJECT:return "dynobject";
        case HIGHGUID_CORPSE:       return "corpse";
        case HIGHGUID_MO_TRANSPORT: return "mo_transport";
        case HIGHGUID_GROUP:        return "group";
        case HIGHGUID_GUILD:        return "guild";
        case HIGHGUID_AREATRIGGER:  return "areatrigger";
        case HIGHGUID_VIGNETTE:     return "vignette";
        case HIGHGUID_CONVERSATION: return "conversation";
        default:
            return "<unknown>";
    }
}
//////////////////////////////////////////////////////////////////////////

enum Guid128Type
{
    GUID_TYPE_NONE                  = 0x00,
    GUID_TYPE_PLAYER                = 0x02, ///< 6.0.1 18663
    GUID_TYPE_ITEM                  = 0x03, ///< 6.0.1 18663
    GUID_TYPE_CONTAINER             = 0x03, ///< 6.0.1 18663
    GUID_TYPE_STATIC_DOOR           = 0x04, ///< 6.0.1 18663
    GUID_TYPE_MO_TRANSPORT          = 0x05, ///< 6.0.1 18663
    GUID_TYPE_CONVERSATION          = 0x06, ///< 6.0.1 18663
    GUID_TYPE_UNIT                  = 0x07, ///< 6.0.1 18663
    GUID_TYPE_VEHICLE               = 0x08, ///< 6.0.1 18663
    GUID_TYPE_PET                   = 0x09, ///< 6.0.1 18663
    GUID_TYPE_GAMEOBJECT            = 0x0A, ///< 6.0.1 18663
    GUID_TYPE_DYNAMIC_OBJECT        = 0x0B, ///< 6.0.1 18663
    GUID_TYPE_AREATRIGGER           = 0x0C, ///< 6.0.1 18663
    GUID_TYPE_CORPSE                = 0x0D, ///< 6.0.1 18663
    GUID_TYPE_LOOT                  = 0x0E, ///< 6.0.1 18663
    GUID_TYPE_SCENE_OBJECT          = 0x0F, ///< 6.0.1 18663
    GUID_TYPE_SCENARIO              = 0x10, ///< 6.0.1 18663
    GUID_TYPE_AI_GROUP              = 0x11, ///< 6.0.1 18663
    GUID_TYPE_DYNAMIC_DOOR          = 0x12, ///< 6.0.1 18663
    GUID_TYPE_CLIENT_ACTOR          = 0x13, ///< 6.0.1 18663
    GUID_TYPE_VIGNETTE              = 0x14, ///< 6.0.1 18663
    GUID_TYPE_CALL_FOR_HELP         = 0x15, ///< 6.0.1 18663
    GUID_TYPE_AI_RESOURCE           = 0x16, ///< 6.0.1 18663
    GUID_TYPE_AI_LOCK               = 0x17, ///< 6.0.1 18663
    GUID_TYPE_AI_LOCK_TICKET        = 0x18, ///< 6.0.1 18663
    GUID_TYPE_CHAT_CHANNEL          = 0x19, ///< 6.0.1 18663
    GUID_TYPE_GROUP                 = 0x1A, ///< 6.0.1 18663
    GUID_TYPE_GUILD                 = 0x1B, ///< 6.0.1 18663
    GUID_TYPE_WOW_ACC               = 0x1C, ///< 6.0.1 18663
    GUID_TYPE_BNET_ACC              = 0x1D, ///< 6.0.1 18663
    GUID_TYPE_GM_TASK               = 0x1E, ///< 6.0.1 18663
    GUID_TYPE_MOBILE_SESSION        = 0x1F, ///< 6.0.1 18663
    GUID_TYPE_RAID_GROUP            = 0x20, ///< 6.0.1 18663
    GUID_TYPE_SPELL                 = 0x21, ///< 6.0.1 18663
    GUID_TYPE_MAIL                  = 0x22, ///< 6.0.1 18663
    GUID_TYPE_WEB_OBJ               = 0x23, ///< 6.0.1 18663
    GUID_TYPE_LFG_OBJECT            = 0x24, ///< 6.0.1 18663
    GUID_TYPE_LFG_LIST              = 0x25, ///< 6.0.1 18663
    GUID_TYPE_USER_ROUTER           = 0x26, ///< 6.0.1 18663
    GUID_TYPE_PVP_QUEUE_GROUP       = 0x27, ///< 6.0.1 18663
    GUID_TYPE_USER_CLIENT           = 0x28, ///< 6.0.1 18663
    GUID_TYPE_PET_BATTLE            = 0x29, ///< 6.0.1 18663
    GUID_TYPE_UNIQUE_USER_CLIENT    = 0x2A, ///< 6.0.1 18663
    GUID_TYPE_BATTLE_PET            = 0x2B, ///< 6.0.1 18663

    GUID_TYPE_TRANSPORT             = 0x2C, ///< unk
    GUID_TYPE_BATTLEGROUND          = 0x2D, ///< unk
    GUID_TYPE_INSTANCE_SAVE         = 0x2E  ///< unk
};

struct Guid128
{
public:

    Guid128()
    {
        m_Data[0] = 0;
        m_Data[1] = 0;
    }
    Guid128(const Guid128 &  p_Other)
    {
        m_Data[0] = p_Other.m_Data[0];
        m_Data[1] = p_Other.m_Data[1];
    }
    Guid128(uint64 p_Low, uint64 p_Hight)
    {
# if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
        m_Data[1] = p_Hight;
        m_Data[0] = p_Low;
#  else
        m_Data[1] = p_Low;
        m_Data[0] = p_Hight;
# endif
    }
    Guid128(Guid128Type p_Type, uint32 p_RealmID, uint32 p_Entry, uint64 p_ID)
    {
        Make(p_Type, p_RealmID, p_Entry, p_ID);
    }

    bool IsValid() const
    {
        return GetType() != 0;
    }
    bool IsNull() const
    {
        return (m_Data[0] == 0 && m_Data[1] == 0) || (IsValid() && GetLow() == 0);
    }
    uint64 GetHi() const
    {
# if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
        return m_Data[1];
#  else
        return m_Data[0];
# endif
    }
    uint64 GetLow() const
    {
# if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
        return m_Data[0];
#  else
        return m_Data[1];
# endif
    }
    uint8 GetType() const
    {
        return (GetHi() >> 58) & 0x3F;
    }
    uint32 GetMid() const
    {
        return (GetHi() >> 6) & 0x7FFFFF;
    }
    void Make(Guid128Type p_Type, uint32 p_RealmID, uint32 p_Entry, uint64 p_ID)
    {
# if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
        m_Data[1] = (uint64(p_Type) << 58) | (uint64(p_RealmID & 0xFFFF) << 42) | (uint64(p_Entry & 0x7FFFFF) << 6);
        m_Data[0] = p_ID;
#  else
        m_Data[1] = p_ID;
        m_Data[0] = (uint64(p_Type) << 58) | (uint64(p_RealmID & 0xFFFF) << 42) | (uint64(p_Entry & 0x7FFFFF) << 6);
# endif
    }

    inline bool operator==(const Guid128 & p_Other) const
    {
        return m_Data[0] == p_Other.m_Data[0] && m_Data[1] == p_Other.m_Data[1];
    }
    inline  bool operator!=(const Guid128 & p_Other) const
    {
        return m_Data[0] != p_Other.m_Data[0] || m_Data[1] != p_Other.m_Data[1];
    }

    inline bool operator> (const Guid128 & p_B) const
    {
        if (GetHi() == p_B.GetHi())
            return GetLow() > p_B.GetLow();

        return GetHi() > p_B.GetHi();
    }
    inline bool operator<(const Guid128 & p_B) const
    {
        return !(*this > p_B);
    }

    inline bool IsType(Guid128Type p_Type) const
    {
        return GetType() == p_Type;
    }

    Guid128 & operator=(const Guid128 & p_Other)
    {
        m_Data[0] = p_Other.m_Data[0];
        m_Data[1] = p_Other.m_Data[1];

        return *this;
    }

private:
    uint64 m_Data[2];

};

Guid128 Guid64To128(const uint64 & p_64Guid);
uint64 Guid128To64(const Guid128 & p_128Guid);

#endif
