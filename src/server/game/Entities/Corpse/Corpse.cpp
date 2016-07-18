////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Corpse.h"
#include "Player.h"
#include "UpdateMask.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "GossipDef.h"
#include "World.h"

Corpse::Corpse(CorpseType type) : WorldObject(type != CORPSE_BONES), m_type(type)
{
    m_objectType |= TYPEMASK_CORPSE;
    m_objectTypeId = TYPEID_CORPSE;

    m_updateFlag = UPDATEFLAG_HAS_POSITION;

    m_valuesCount = CORPSE_END;
    _dynamicValuesCount = CORPSE_DYNAMIC_END;

    m_time = time(NULL);

    lootForBody = false;
    lootRecipient = NULL;
}

Corpse::~Corpse()
{
}

void Corpse::AddToWorld()
{
    ///- Register the corpse for guid lookup
    if (!IsInWorld())
        sObjectAccessor->AddObject(this);

    Object::AddToWorld();
}

void Corpse::RemoveFromWorld()
{
    ///- Remove the corpse from the accessor
    if (IsInWorld())
        sObjectAccessor->RemoveObject(this);

    Object::RemoveFromWorld();
}

bool Corpse::Create(uint32 guidlow, Map* map)
{
    SetMap(map);
    Object::_Create(guidlow, 0, HIGHGUID_CORPSE);

    loot.SetSource(GetGUID());
    loot.Context = map->GetLootItemContext();

    return true;
}

bool Corpse::Create(uint32 guidlow, Player* owner)
{
    ASSERT(owner);

    Relocate(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation());

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_PLAYER, "Corpse (guidlow %d, owner %s) not created. Suggested coordinates isn't valid (X: %f Y: %f)",
            guidlow, owner->GetName(), owner->GetPositionX(), owner->GetPositionY());
        return false;
    }

    //we need to assign owner's map for corpse
    //in other way we will get a crash in Corpse::SaveToDB()
    SetMap(owner->GetMap());

    WorldObject::_Create(guidlow, HIGHGUID_CORPSE, owner->GetPhaseMask());

    SetObjectScale(1);
    SetGuidValue(CORPSE_FIELD_OWNER, owner->GetGUID());

    loot.SetSource(GetGUID());

    _gridCoord = JadeCore::ComputeGridCoord(GetPositionX(), GetPositionY());

    loot.Context = GetMap()->GetLootItemContext();

    return true;
}

void Corpse::SaveToDB()
{
#ifndef CROSS
    // prevent DB data inconsistence problems and duplicates
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    DeleteFromDB(trans);

    uint16 index = 0;
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CORPSE);
    stmt->setUInt32(index++, GetGUIDLow());                                           // corpseGuid
    stmt->setUInt32(index++, GUID_LOPART(GetOwnerGUID()));                            // guid
    stmt->setFloat (index++, GetPositionX());                                         // posX
    stmt->setFloat (index++, GetPositionY());                                         // posY
    stmt->setFloat (index++, GetPositionZ());                                         // posZ
    stmt->setFloat (index++, GetOrientation());                                       // orientation
    stmt->setUInt16(index++, GetMapId());                                             // mapId
    stmt->setUInt32(index++, GetUInt32Value(CORPSE_FIELD_DISPLAY_ID));                // displayId
    stmt->setString(index++, _ConcatFields(CORPSE_FIELD_ITEMS, EQUIPMENT_SLOT_END));   // itemCache
    stmt->setUInt32(index++, GetUInt32Value(CORPSE_FIELD_SKIN_ID));                   // bytes1
    stmt->setUInt32(index++, GetUInt32Value(CORPSE_FIELD_FACIAL_HAIR_STYLE_ID));                   // bytes2
    stmt->setUInt8 (index++, GetUInt32Value(CORPSE_FIELD_FLAGS));                     // flags
    stmt->setUInt8 (index++, GetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS));             // dynFlags
    stmt->setUInt32(index++, uint32(m_time));                                         // time
    stmt->setUInt8 (index++, GetType());                                              // corpseType
    stmt->setUInt32(index++, GetInstanceId());                                        // instanceId
    stmt->setUInt16(index++, GetPhaseMask());                                         // phaseMask
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
#endif
}

void Corpse::DeleteBonesFromWorld()
{
    ASSERT(GetType() == CORPSE_BONES);
    Corpse* corpse = ObjectAccessor::GetCorpse(*this, GetGUID());

    if (!corpse)
    {
        sLog->outError(LOG_FILTER_PLAYER, "Bones %u not found in world.", GetGUIDLow());
        return;
    }

    AddObjectToRemoveList();
}

void Corpse::DeleteFromDB(SQLTransaction& trans)
{
#ifndef CROSS
    PreparedStatement* stmt = NULL;
    if (GetType() == CORPSE_BONES)
    {
        // Only specific bones
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CORPSE);
        stmt->setUInt32(0, GetGUIDLow());
    }
    else
    {
        // all corpses (not bones)
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_CORPSES);
        stmt->setUInt32(0, GUID_LOPART(GetOwnerGUID()));
    }
    trans->Append(stmt);
#endif
}

bool Corpse::LoadCorpseFromDB(uint32 guid, Field* fields)
{
    //        0     1     2     3            4      5          6          7       8       9      10        11    12          13          14          15         16
    // SELECT posX, posY, posZ, orientation, mapId, displayId, itemCache, bytes1, bytes2, flags, dynFlags, time, corpseType, instanceId, phaseMask, corpseGuid, guid FROM corpse WHERE corpseType <> 0

    uint32 ownerGuid = fields[16].GetUInt32();
    float posX   = fields[0].GetFloat();
    float posY   = fields[1].GetFloat();
    float posZ   = fields[2].GetFloat();
    float o      = fields[3].GetFloat();
    uint32 mapId = fields[4].GetUInt16();

    Object::_Create(guid, 0, HIGHGUID_CORPSE);

    SetUInt32Value(CORPSE_FIELD_DISPLAY_ID, fields[5].GetUInt32());
    _LoadIntoDataField(fields[6].GetCString(), CORPSE_FIELD_ITEMS, EQUIPMENT_SLOT_END, false);
    SetUInt32Value(CORPSE_FIELD_SKIN_ID, fields[7].GetUInt32());
    SetUInt32Value(CORPSE_FIELD_FACIAL_HAIR_STYLE_ID, fields[8].GetUInt32());
    SetUInt32Value(CORPSE_FIELD_FLAGS, fields[9].GetUInt8());
    SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, fields[10].GetUInt8());
    SetGuidValue(CORPSE_FIELD_OWNER, MAKE_NEW_GUID(ownerGuid, 0, HIGHGUID_PLAYER));

    m_time = time_t(fields[11].GetUInt32());

    uint32 instanceId  = fields[13].GetUInt32();
    uint32 phaseMask   = fields[14].GetUInt16();

    // place
    SetLocationInstanceId(instanceId);
    SetLocationMapId(mapId);
    SetPhaseMask(phaseMask, false);
    Relocate(posX, posY, posZ, o);

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_PLAYER, "Corpse (guid: %u, owner: %u) is not created, given coordinates are not valid (X: %f, Y: %f, Z: %f)",
            GetGUIDLow(), GUID_LOPART(GetOwnerGUID()), posX, posY, posZ);
        return false;
    }

    _gridCoord = JadeCore::ComputeGridCoord(GetPositionX(), GetPositionY());
    return true;
}

bool Corpse::IsExpired(time_t t) const
{
    if (m_type == CORPSE_BONES)
        return m_time < t - 5 * MINUTE;
    else
        return m_time < t - 3 * DAY;
}