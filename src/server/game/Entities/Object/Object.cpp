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

#include "Common.h"
#include "SharedDefines.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "Object.h"
#include "Creature.h"
#include "Player.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "UpdateData.h"
#include "UpdateMask.h"
#include "Util.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "Log.h"
#include "Transport.h"
#include "TargetedMovementGenerator.h"
#include "WaypointMovementGenerator.h"
#include "VMapFactory.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "SpellAuraEffects.h"
#include "UpdateFieldFlags.h"
#include "TemporarySummon.h"
#include "Totem.h"
#include "OutdoorPvPMgr.h"
#include "MovementPacketBuilder.h"
#include "DynamicTree.h"
#include "Unit.h"
#include "Group.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "MoveSpline.h"

uint32 GuidHigh2TypeId(uint32 guid_hi)
{
    switch (guid_hi)
    {
        case HIGHGUID_ITEM:         return TYPEID_ITEM;
        //case HIGHGUID_CONTAINER:    return TYPEID_CONTAINER; HIGHGUID_CONTAINER == HIGHGUID_ITEM currently
        case HIGHGUID_UNIT:         return TYPEID_UNIT;
        case HIGHGUID_PET:          return TYPEID_UNIT;
        case HIGHGUID_PLAYER:       return TYPEID_PLAYER;
        case HIGHGUID_GAMEOBJECT:   return TYPEID_GAMEOBJECT;
        case HIGHGUID_DYNAMICOBJECT:return TYPEID_DYNAMICOBJECT;
        case HIGHGUID_CORPSE:       return TYPEID_CORPSE;
        case HIGHGUID_AREATRIGGER:  return TYPEID_AREATRIGGER;
        case HIGHGUID_MO_TRANSPORT: return TYPEID_GAMEOBJECT;
        case HIGHGUID_VEHICLE:      return TYPEID_UNIT;
    }
    return NUM_CLIENT_OBJECT_TYPES;                         // unknown
}

Object::Object() : m_PackGUID(sizeof(uint64)+1)
{
    m_objectTypeId      = TYPEID_OBJECT;
    m_objectType        = TYPEMASK_OBJECT;

    m_uint32Values      = NULL;
    _changedFields      = NULL;
    _dynamicFields      = NULL;
    m_valuesCount       = 0;
    _dynamicTabCount    = 0;
    _fieldNotifyFlags   = UF_FLAG_DYNAMIC;

    m_inWorld           = false;
    m_objectUpdated     = false;

    m_PackGUID.appendPackGUID(0);

    if (sWorld->deleteUnits.find(this) != sWorld->deleteUnits.end())
       sWorld->deleteUnits[this] = false;
}

WorldObject::~WorldObject()
{
    // this may happen because there are many !create/delete
    if (IsWorldObject() && m_currMap)
    {
        if (GetTypeId() == TYPEID_CORPSE)
        {
            sLog->outFatal(LOG_FILTER_GENERAL, "Object::~Object Corpse guid=" UI64FMTD ", type=%d, entry=%u deleted but still in map!!", GetGUID(), ((Corpse*)this)->GetType(), GetEntry());
            ASSERT(false);
        }
        ResetMap();
    }
}

Object::~Object()
{
    if (sWorld->deleteUnits.find(this) != sWorld->deleteUnits.end())
        sWorld->deleteUnits.insert(std::make_pair(this, true));
    else
        sWorld->deleteUnits[this] = true;

    if (IsInWorld())
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "Object::~Object - guid=" UI64FMTD ", typeid=%d, entry=%u deleted but still in world!!", GetGUID(), GetTypeId(), GetEntry());
        if (isType(TYPEMASK_ITEM))
            sLog->outFatal(LOG_FILTER_GENERAL, "Item slot %u", ((Item*)this)->GetSlot());
        //ASSERT(false);
        RemoveFromWorld();
    }

    if (m_objectUpdated)
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "Object::~Object - guid=" UI64FMTD ", typeid=%d, entry=%u deleted but still in update list!!", GetGUID(), GetTypeId(), GetEntry());
        //ASSERT(false);
        sObjectAccessor->RemoveUpdateObject(this);
    }

    delete [] m_uint32Values;
    delete [] _changedFields;
    delete [] _dynamicFields;
}

void Object::_InitValues()
{
    m_uint32Values = new uint32[m_valuesCount];
    memset(m_uint32Values, 0, m_valuesCount*sizeof(uint32));

    _changedFields = new bool[m_valuesCount];
    memset(_changedFields, 0, m_valuesCount*sizeof(bool));

    _dynamicFields = new DynamicFields[_dynamicTabCount];

    m_objectUpdated = false;
}

void Object::_Create(uint32 guidlow, uint32 entry, HighGuid guidhigh)
{
    if (!m_uint32Values || !_dynamicFields)
        _InitValues();

    uint64 guid = MAKE_NEW_GUID(guidlow, entry, guidhigh);
    SetUInt64Value(OBJECT_FIELD_GUID, guid);
    SetUInt16Value(OBJECT_FIELD_TYPE, 0, m_objectType);
    m_PackGUID.clear();
    m_PackGUID.appendPackGUID(GetGUID());
}

std::string Object::_ConcatFields(uint16 startIndex, uint16 size) const
{
    std::ostringstream ss;
    for (uint16 index = 0; index < size; ++index)
        ss << GetUInt32Value(index + startIndex) << ' ';
    return ss.str();
}

void Object::AddToWorld()
{
    if (m_inWorld)
        return;

    ASSERT(m_uint32Values);

    m_inWorld = true;

    // synchronize values mirror with values array (changes will send in updatecreate opcode any way
    ClearUpdateMask(true);
}

void Object::RemoveFromWorld()
{
    if (!m_inWorld)
        return;

    m_inWorld = false;

    // if we remove from world then sending changes not required
    ClearUpdateMask(true);
}

void Object::BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    if (!target)
        return;

    uint8  updateType = UPDATETYPE_CREATE_OBJECT;
    uint16 flags      = m_updateFlag;

    uint32 valCount = m_valuesCount;

    /** lower flag1 **/
    if (target == this)                                      // building packet for yourself
        flags |= UPDATEFLAG_SELF;
    //else if (GetTypeId() == TYPEID_PLAYER && target != this)
        //valCount = PLAYER_END_NOT_SELF;

    switch (GetGUIDHigh())
    {
        case HIGHGUID_PLAYER:
        case HIGHGUID_PET:
        case HIGHGUID_CORPSE:
        case HIGHGUID_DYNAMICOBJECT:
        case HIGHGUID_AREATRIGGER:
            updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
        case HIGHGUID_UNIT:
            if (ToUnit()->ToTempSummon() && IS_PLAYER_GUID(ToUnit()->ToTempSummon()->GetSummonerGUID()))
                updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
        case HIGHGUID_GAMEOBJECT:
            if (IS_PLAYER_GUID(ToGameObject()->GetOwnerGUID()))
                updateType = UPDATETYPE_CREATE_OBJECT2;
            break;
    }

    if (flags & UPDATEFLAG_STATIONARY_POSITION)
    {
        // UPDATETYPE_CREATE_OBJECT2 for some gameobject types...
        if (isType(TYPEMASK_GAMEOBJECT))
        {
            switch (((GameObject*)this)->GetGoType())
            {
                case GAMEOBJECT_TYPE_TRAP:
                case GAMEOBJECT_TYPE_DUEL_ARBITER:
                case GAMEOBJECT_TYPE_FLAGSTAND:
                case GAMEOBJECT_TYPE_FLAGDROP:
                    updateType = UPDATETYPE_CREATE_OBJECT2;
                    break;
                case GAMEOBJECT_TYPE_TRANSPORT:
                    flags |= UPDATEFLAG_TRANSPORT;
                    break;
                default:
                    break;
            }
        }
    }

    if (ToUnit() && ToUnit()->getVictim())
        flags |= UPDATEFLAG_HAS_TARGET;

    ByteBuffer buf(500);
    buf << uint8(updateType);
    buf.append(GetPackGUID());
    buf << uint8(m_objectTypeId);

    BuildMovementUpdate(&buf, flags);
    BuildValuesUpdate(updateType, &buf, target);
    BuildDynamicValuesUpdate(&buf);

    data->AddUpdateBlock(buf);
}

void Object::SendUpdateToPlayer(Player* player)
{
    // send create update to player
    UpdateData upd(player->GetMapId());
    WorldPacket packet;

    BuildCreateUpdateBlockForPlayer(&upd, player);
    if (upd.BuildPacket(&packet))
        player->GetSession()->SendPacket(&packet);
}

void Object::BuildValuesUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    ByteBuffer buf(500);

    buf << uint8(UPDATETYPE_VALUES);
    buf.append(GetPackGUID());

    if (sWorld->isDelete((Object*)this))
    {
        ACE_Stack_Trace trace;
        sLog->OutPandashan("BuildValuesUpdateBlockForPlayer this delete !!!");
    }

    if (sWorld->isDelete(target))
    {
        ACE_Stack_Trace trace;
        sLog->OutPandashan("BuildValuesUpdateBlockForPlayer target delete !!!");
    }

    BuildValuesUpdate(UPDATETYPE_VALUES, &buf, target);
    BuildDynamicValuesUpdate(&buf);

    data->AddUpdateBlock(buf);
}

void Object::BuildOutOfRangeUpdateBlock(UpdateData* data) const
{
    data->AddOutOfRangeGUID(GetGUID());
}

void Object::DestroyForPlayer(Player* target, bool onDeath) const
{
    ASSERT(target);

    WorldPacket data(SMSG_DESTROY_OBJECT, 8 + 1);

    ObjectGuid guid = GetGUID();

    data.WriteBit(guid[3]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);

    //! If the following bool is true, the client will call "void CGUnit_C::OnDeath()" for this object.
    //! OnDeath() does for eg trigger death animation and interrupts certain spells/missiles/auras/sounds...
    data.WriteBit(onDeath);
    data.WriteBit(guid[1]);

    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[6]);

    target->GetSession()->SendPacket(&data);
}

void Object::BuildMovementUpdate(ByteBuffer* data, uint16 flags) const
{
    const Player* player = ToPlayer();
    const Unit* unit = ToUnit();
    const GameObject* go = ToGameObject();
    const DynamicObject* dob = ToDynObject();
    const AreaTrigger *atr = ToAreaTrigger();

    const WorldObject* wo =
        player ? (const WorldObject*)player : (
        unit ? (const WorldObject*)unit : (
        go ? (const WorldObject*)go : dob ? (const WorldObject*)dob : (
        atr ? (const WorldObject*)atr : (const WorldObject*)ToCorpse())));

    bool isTransport = false;
    if (go)
    {
        if (GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(go->GetEntry()))
            if (goinfo->type == GAMEOBJECT_TYPE_TRANSPORT && goinfo->transport.pause)
                isTransport = false;
    }

    uint32 bitCounter2 = 0;
    bool hasAreaTriggerData = isType(TYPEMASK_AREATRIGGER) && ((AreaTrigger*)this)->GetVisualRadius() != 0.0f;
    bool isSceneObject = false;

    data->WriteBit(hasAreaTriggerData);                         // isAreaTrigger
    data->WriteBit(isSceneObject);                              // isScenario
    data->WriteBit(0);                                          // unk bit 676
    data->WriteBit(flags & UPDATEFLAG_TRANSPORT);               // isTransport
    data->WriteBit(0);                                          // unk bit 810
    data->WriteBit(flags & UPDATEFLAG_ANIMKITS);                // HasAnimKits
    data->WriteBit(flags & UPDATEFLAG_SELF);                    // unk bit 680
    data->WriteBit(0);                                          // unk bit 1044
    data->WriteBits(uint32(isTransport ? 1 : 0), 22);
    data->WriteBit(0);                                          // unk bit
    data->WriteBit(0);                                          // unk bit
    data->WriteBit((flags & UPDATEFLAG_LIVING) && unit);        // isAlive
    data->WriteBit((flags & UPDATEFLAG_STATIONARY_POSITION) && wo);     // hasStationaryPosition
    data->WriteBit((flags & UPDATEFLAG_GO_TRANSPORT_POSITION) && wo);   // hasGameObjectData
    data->WriteBit((flags & UPDATEFLAG_ROTATION) && go);                // hasRotation
    data->WriteBit((flags & UPDATEFLAG_HAS_TARGET) && unit && unit->getVictim());  // hasTarget
    data->WriteBit(0);                                          // unk bit 1064
    data->WriteBit(0);                                          // unk bit
    data->WriteBit(0);                                          // unk bit
    data->WriteBit(0);                                          // isSelf
    data->WriteBit((flags & UPDATEFLAG_VEHICLE) && unit);       // hasVehicleData

    if (unit)
    {
        const_cast<Unit*>(unit)->m_movementInfo.Normalize();
    }

    if ((flags & UPDATEFLAG_GO_TRANSPORT_POSITION) && wo)
    {
        ObjectGuid transGuid = wo->m_movementInfo.t_guid;

        data->WriteBit(transGuid[5]);
        data->WriteBit(transGuid[0]);
        data->WriteBit(transGuid[6]);
        data->WriteBit(wo->m_movementInfo.t_time2 != 0);              // HasTransportTime2
        data->WriteBit(transGuid[7]);
        data->WriteBit(transGuid[3]);
        data->WriteBit(transGuid[2]);
        data->WriteBit(wo->m_movementInfo.t_time3 != 0);             // HasTransportTime3
        data->WriteBit(transGuid[4]);
        data->WriteBit(transGuid[1]);
    }

    if (isSceneObject)
    {
        /*
        @TODO: Scenarios
        */
    }

    if (hasAreaTriggerData)
    {
        data->WriteBit(true);   // scale
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);   // unk, always true on retail sniff
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
        data->WriteBit(false);
    }

    if ((flags & UPDATEFLAG_LIVING) && unit)
    {
        ObjectGuid guid = GetGUID();

        uint32 movementFlags = unit->m_movementInfo.GetMovementFlags();
        uint16 movementFlagsExtra = unit->m_movementInfo.GetExtraMovementFlags();

        if (GetTypeId() == TYPEID_UNIT)
            movementFlags &= MOVEMENTFLAG_MASK_CREATURE_ALLOWED;

        bool isSplineEnabled = unit->movespline->Initialized() && !unit->movespline->Finalized();
        data->WriteBit(guid[5]);
        data->WriteBit(isSplineEnabled);
        data->WriteBit(0);                                          // unk
        data->WriteBit(guid[1]);
        data->WriteBit(guid[3]);
        data->WriteBit(G3D::fuzzyEq(unit->GetOrientation(), 0.0f)); // Has Orientation bit
        data->WriteBit(0);                                          // unk

        if (isSplineEnabled)
            Movement::PacketBuilder::WriteCreateBits(*unit->movespline, *data);

        data->WriteBits(0, 22);
        data->WriteBit(guid[6]);
        data->WriteBit(guid[7]);
        data->WriteBit(unit->m_movementInfo.t_guid != 0LL);         // Has transport data
        data->WriteBit(!unit->m_movementInfo.HavePitch);

        if (unit->m_movementInfo.t_guid)
        {
            ObjectGuid transGuid = unit->m_movementInfo.t_guid;

            data->WriteBit(unit->m_movementInfo.t_time3);                                                  // Has transport time 3
            data->WriteBit(transGuid[6]);
            data->WriteBit(transGuid[2]);
            data->WriteBit(transGuid[1]);
            data->WriteBit(transGuid[0]);
            data->WriteBit(transGuid[5]);
            data->WriteBit(unit->m_movementInfo.t_time2);                                                  // Has transport time 2
            data->WriteBit(transGuid[3]);
            data->WriteBit(transGuid[7]);
            data->WriteBit(transGuid[4]);
        }

        data->WriteBits(0, 19);
        data->WriteBit(!movementFlagsExtra);
        data->WriteBit(guid[2]);
        data->WriteBit(!movementFlags);
        data->WriteBit(!unit->m_movementInfo.time);

        if (movementFlags)
            data->WriteBits(movementFlags, 30);

        data->WriteBit(unit->m_movementInfo.hasFallData); // IsInterpolated ?

        if (movementFlagsExtra)
            data->WriteBits(movementFlagsExtra, 13);

        data->WriteBit(guid[4]);

        if (unit->m_movementInfo.hasFallData)
            data->WriteBit(unit->m_movementInfo.hasFallDirection);

        data->WriteBit(guid[0]);
        data->WriteBit(0); // Unk
        data->WriteBit(1); // unk
        data->WriteBit(!unit->m_movementInfo.HaveSplineElevation);
    }

    if ((flags & UPDATEFLAG_HAS_TARGET) && unit && unit->getVictim())
    {
        ObjectGuid victimGuid = unit->getVictim()->GetGUID();   // checked in BuildCreateUpdateBlockForPlayer
    
        uint8 bitOrder[8] = {6, 4, 5, 2, 3, 7, 0, 1};
        data->WriteBitInOrder(victimGuid, bitOrder);
    }

    if (flags & UPDATEFLAG_ANIMKITS)
    {
        data->WriteBit(1);                                                      // Missing AnimKit1
        data->WriteBit(1);                                                      // Missing AnimKit2
        data->WriteBit(1);                                                      // Missing AnimKit3
    }

    data->FlushBits();

    if (isTransport)
        if (GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(go->GetEntry()))
            if (goinfo->type == GAMEOBJECT_TYPE_TRANSPORT)
                (*data) << (uint32)goinfo->transport.pause;

    if (hasAreaTriggerData)
    {
        *data << uint32(8);                                         // ObjectType AreaTrigger
        *data << float(((AreaTrigger*)this)->GetVisualRadius());    // scale
        *data << float(((AreaTrigger*)this)->GetVisualRadius());    // scale
    }

    if ((flags & UPDATEFLAG_LIVING) && unit)
    {
        ObjectGuid guid = GetGUID();

        if (unit->movespline->Initialized() && !unit->movespline->Finalized())
            Movement::PacketBuilder::WriteCreateData(*unit->movespline, *data, (Unit*)unit);

        if (unit->m_movementInfo.t_guid != 0LL)
        {
            ObjectGuid transGuid = unit->m_movementInfo.t_guid;

            *data << float(unit->GetTransOffsetX());
            if (unit->m_movementInfo.t_time2)
            {
                *data << uint32(unit->m_movementInfo.t_time2);
            }
            *data << uint32(unit->GetTransTime());
            *data << float(unit->GetTransOffsetZ());
            data->WriteByteSeq(transGuid[4]);
            data->WriteByteSeq(transGuid[3]);
            *data << int8(unit->GetTransSeat());
            *data << float(unit->GetTransOffsetY());
            *data << float(unit->GetTransOffsetO());
            data->WriteByteSeq(transGuid[0]);
            data->WriteByteSeq(transGuid[7]);
            data->WriteByteSeq(transGuid[6]);
            data->WriteByteSeq(transGuid[5]);
            data->WriteByteSeq(transGuid[1]);
            data->WriteByteSeq(transGuid[2]);
            if (unit->m_movementInfo.t_time3)
            {
                *data << uint32(unit->m_movementInfo.t_time3);
            }
        }

        data->WriteByteSeq(guid[2]);
        data->WriteByteSeq(guid[6]);
        data->WriteByteSeq(guid[0]);

        if (unit->m_movementInfo.hasFallData)
        {
            if (unit->m_movementInfo.hasFallDirection)
            {
                *data << float(unit->m_movementInfo.j_cosAngle);
                *data << float(unit->m_movementInfo.j_sinAngle);
                *data << float(unit->m_movementInfo.j_xyspeed);
            }

            *data << float(unit->m_movementInfo.j_zspeed);
            *data << uint32(unit->m_movementInfo.fallTime);
        }

        *data << float(unit->GetPositionY());

        if (unit->m_movementInfo.time)
            *data << uint32(unit->m_movementInfo.time);

        *data << unit->GetSpeed(MOVE_PITCH_RATE);

        data->WriteByteSeq(guid[7]);

        if (!G3D::fuzzyEq(unit->GetOrientation(), 0.0f))
            *data << float(unit->GetOrientation());

        *data << unit->GetSpeed(MOVE_FLIGHT_BACK);
        *data << unit->GetSpeed(MOVE_SWIM);

        data->WriteByteSeq(guid[1]);

        *data << float(unit->GetPositionZMinusOffset());

        if (unit->m_movementInfo.HavePitch)
            *data << float(unit->m_movementInfo.pitch);

        data->WriteByteSeq(guid[4]);

        *data << unit->GetSpeed(MOVE_FLIGHT);
        *data << unit->GetSpeed(MOVE_RUN);
        *data << unit->GetSpeed(MOVE_RUN_BACK);
        *data << unit->GetSpeed(MOVE_TURN_RATE);
        *data << unit->GetSpeed(MOVE_SWIM_BACK);

        *data << float(unit->GetPositionX());

        data->WriteByteSeq(guid[5]);
        data->WriteByteSeq(guid[3]);

        if (unit->m_movementInfo.HaveSplineElevation)
            *data << float(unit->m_movementInfo.splineElevation);

        *data << unit->GetSpeed(MOVE_WALK);
    }

    if ((flags & UPDATEFLAG_GO_TRANSPORT_POSITION) && wo)
    {
        ObjectGuid transGuid = wo->m_movementInfo.t_guid;

        *data << uint32(wo->GetTransTime());
        *data << float(wo->GetTransOffsetY());

        data->WriteByteSeq(transGuid[0]);
        data->WriteByteSeq(transGuid[5]);

        if (wo->m_movementInfo.t_time3)
            *data << uint32(wo->m_movementInfo.t_time3);

        data->WriteByteSeq(transGuid[7]);
        data->WriteByteSeq(transGuid[4]);

        *data << float(wo->GetTransOffsetO());
        *data << float(wo->GetTransOffsetX());

        data->WriteByteSeq(transGuid[1]);
        data->WriteByteSeq(transGuid[3]);
        data->WriteByteSeq(transGuid[6]);

        *data << int8(wo->GetTransSeat());

        if (wo->m_movementInfo.t_time2)
            *data << uint32(wo->m_movementInfo.t_time2);

        data->WriteByteSeq(transGuid[2]);

        *data << float(wo->GetTransOffsetZ());
    }

    if ((flags & UPDATEFLAG_STATIONARY_POSITION) && wo)
    {
        *data << float(wo->GetOrientation());

        if (unit)
            *data << float(unit->GetPositionZMinusOffset());
        else
            *data << float(wo->GetPositionZ());
        *data << float(wo->GetPositionX());
        *data << float(wo->GetPositionY());
    }

    if ((flags & UPDATEFLAG_HAS_TARGET) && unit && unit->getVictim())
    {
        ObjectGuid victimGuid = unit->getVictim()->GetGUID();   // checked in BuildCreateUpdateBlockForPlayer

        data->WriteByteSeq(victimGuid[1]);
        data->WriteByteSeq(victimGuid[5]);
        data->WriteByteSeq(victimGuid[4]);
        data->WriteByteSeq(victimGuid[7]);
        data->WriteByteSeq(victimGuid[6]);
        data->WriteByteSeq(victimGuid[2]);
        data->WriteByteSeq(victimGuid[0]);
        data->WriteByteSeq(victimGuid[3]);
    }

    if (flags & UPDATEFLAG_TRANSPORT)
        *data << uint32(getMSTime());

    if ((flags & UPDATEFLAG_ROTATION) && go)
        *data << uint64(go->GetRotation());

    if ((flags & UPDATEFLAG_VEHICLE) && unit)
    {
        *data << float(unit->GetOrientation());
        *data << uint32(unit->GetVehicleKit()->GetVehicleInfo()->m_ID);
    }

    if (flags & UPDATEFLAG_ANIMKITS)
    {
        *data << uint16(1);                                                      // Missing AnimKit1
        *data << uint16(1);                                                      // Missing AnimKit2
        *data << uint16(1);                                                      // Missing AnimKit3
    }

    if ((flags & UPDATEFLAG_LIVING) && unit && unit->movespline->Initialized() && !unit->movespline->Finalized())
    {
        Movement::PacketBuilder::WriteCreateGuid(*unit->movespline, *data);
    }

    /*
    data->WriteBits(bitCounter2, 21);               //BitCounter2
    data->WriteBit(flags & UPDATEFLAG_TRANSPORT);   //isTransport
    data->WriteBit(hasAreaTriggerData);             //HasAreaTriggerInfo
    data->WriteBit(0);                              //Bit1
    data->WriteBit(0);                              //HasUnknown2
    data->WriteBit(0);                              //Bit2
    data->WriteBit(0);                              //Bit3
    data->WriteBit(0);                              //HasUnknown4

    // Transport time related
    if (bitCounter2)
    {
        /*
        for (uint32 i = 0; i < bitCounter2; i++)
        todo
        */
    /*}

    // HasAreaTriggerInfo
    if (hasAreaTriggerData)
    {
        data->WriteBit(0);
        data->WriteBit(0);
        data->WriteBit(0);
        data->WriteBit(0);
        data->WriteBit(1); //scale
        data->WriteBit(0);
        data->WriteBit(0);
    }

    // If (HasUnknown2 )
    // readSomeBits, TODO check via IDA debug
    //We know have to realign the bits so as to put bytes.
    data->FlushBits();
    for (uint32 i = 0; i < bitCounter2; i++)
    {
        //unk32
        //unkfloat
        //unkfloat
        //unk32
        //unkfloat
        //unkfloatd
    }

    // HasAreaTriggerInfo
    if (hasAreaTriggerData)
    {
        *data << float(((AreaTrigger*)this)->GetVisualRadius()); // scale
        *data << float(((AreaTrigger*)this)->GetVisualRadius()); // scale
        *data << uint32(8); // unk ID
        *data << float(1); // unk, always 1 in sniff
        *data << float(1); // unk, always 1 in sniff
    }*/
}

void Object::BuildValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    if (!target)
        return;

    ByteBuffer fieldBuffer;
    UpdateMask updateMask;
    updateMask.SetCount(m_valuesCount);

    uint32* flags = NULL;
    uint32 visibleFlag = GetUpdateFieldData(target, flags);

    int sendedCount = 0;

    for (uint16 index = 0; index < m_valuesCount; ++index)
    {
        if (_fieldNotifyFlags & flags[index] ||
            ((updateType == UPDATETYPE_VALUES ? _changedFields[index] : m_uint32Values[index]) && (flags[index] & visibleFlag)))
        {
            updateMask.SetBit(index);
            fieldBuffer << m_uint32Values[index];

            ++sendedCount;
        }
    }

    for (uint16 index = 0; index < m_valuesCount; ++index)
    {
        if (updateMask.GetBit(index))
            --sendedCount;
    }

    ASSERT(sendedCount == 0);

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
}

void Object::BuildDynamicValuesUpdate(ByteBuffer* data) const
{
    // Crashfix, prevent use of bag with dynamic field
    Item* temp = ((Item*)this);
    if (GetTypeId() == TYPEID_ITEM && temp && temp->ToBag())
    {
        *data << uint8(0);
        return;
    }

    if (_dynamicTabCount == 0)
    {
        *data << uint8(0);
        return;
    }

    uint32 dynamicTabMask = 0;
    std::vector<uint32> dynamicFieldsMask;
    dynamicFieldsMask.resize(_dynamicTabCount);

    for (uint32 i = 0; i < _dynamicTabCount; ++i)
    {
        dynamicFieldsMask[i] = 0;

        for (int index = 0; index < DynamicFields::Count; ++index)
        {
            if (!_dynamicFields[i]._dynamicChangedFields[index])
                continue;

            dynamicTabMask |= 1 << i;
            dynamicFieldsMask[i] |= 1 << index;
        }
    }

    *data << uint8(bool(dynamicTabMask));
    if (dynamicTabMask)
    {
        *data << uint32(dynamicTabMask);

        for (uint32 i = 0; i < _dynamicTabCount; ++i)
        {
            if (dynamicTabMask & (1 << i)) //if ( (1 << (v16 & 31)) & *(&dest + (v16 >> 5)) )
            {
                *data << uint8(1);
                *data << uint32(dynamicFieldsMask[i]);

                for (int index = 0; index < 32; index++)
                {
                    if (dynamicFieldsMask[i] & (1 << index))
                        *data << uint32(_dynamicFields[i]._dynamicValues[index]);
                }
            }
        }
    }
}

void Object::ClearUpdateMask(bool remove)
{
    memset(_changedFields, 0, m_valuesCount*sizeof(bool));

    if (m_objectUpdated)
    {
        for (uint32 i = 0; i < _dynamicTabCount; ++i)
        {
            _dynamicFields[i].ClearMask();
        }

        if (remove)
            sObjectAccessor->RemoveUpdateObject(this);
        m_objectUpdated = false;
    }
}

void Object::BuildFieldsUpdate(Player* player, UpdateDataMapType& data_map) const
{
    UpdateDataMapType::iterator iter = data_map.find(player);

    if (iter == data_map.end())
    {
        std::pair<UpdateDataMapType::iterator, bool> p = data_map.insert(UpdateDataMapType::value_type(player, UpdateData(player->GetMapId())));
        ASSERT(p.second);
        iter = p.first;
    }

    BuildValuesUpdateBlockForPlayer(&iter->second, iter->first);
}

void Object::_LoadIntoDataField(char const* data, uint32 startOffset, uint32 count)
{
    if (!data)
        return;

    Tokenizer tokens(data, ' ', count);

    if (tokens.size() != count)
        return;

    for (uint32 index = 0; index < count; ++index)
    {
        m_uint32Values[startOffset + index] = atol(tokens[index]);
        _changedFields[startOffset + index] = true;
    }
}

uint32 Object::GetUpdateFieldData(Player const* target, uint32*& flags) const
{
    uint32 visibleFlag = UF_FLAG_PUBLIC;
    if (target == this)
        visibleFlag |= UF_FLAG_PRIVATE;

    switch (GetTypeId())
    {
        case TYPEID_ITEM:
        case TYPEID_CONTAINER:
            flags = ItemUpdateFieldFlags;
            if (((Item*)this)->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER | UF_FLAG_ITEM_OWNER;
            break;
        case TYPEID_UNIT:
        case TYPEID_PLAYER:
        {
            Player* plr = ToUnit()->GetCharmerOrOwnerPlayerOrPlayerItself();
            flags = UnitUpdateFieldFlags;
            if (ToUnit()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;

            if (HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_SPECIALINFO))
                if (ToUnit()->HasAuraTypeWithCaster(SPELL_AURA_EMPATHY, target->GetGUID()))
                    visibleFlag |= UF_FLAG_SPECIAL_INFO;

            if (plr && plr->IsInSameRaidWith(target))
                visibleFlag |= UF_FLAG_PARTY_MEMBER;
            break;
        }
        case TYPEID_GAMEOBJECT:
            flags = GameObjectUpdateFieldFlags;
            if (ToGameObject()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_DYNAMICOBJECT:
            flags = DynamicObjectUpdateFieldFlags;
            if (((DynamicObject*)this)->GetCasterGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_CORPSE:
            flags = CorpseUpdateFieldFlags;
            if (ToCorpse()->GetOwnerGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_AREATRIGGER:
            flags = AreaTriggerUpdateFieldFlags;
            if (((AreaTrigger*)this)->GetCasterGUID() == target->GetGUID())
                visibleFlag |= UF_FLAG_OWNER;
            break;
        case TYPEID_OBJECT:
            break;
    }

    return visibleFlag;
}

void Object::SetInt32Value(uint16 index, int32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_int32Values[index] != value)
    {
        m_int32Values[index] = value;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetUInt32Value(uint16 index, uint32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_uint32Values[index] != value)
    {
        m_uint32Values[index] = value;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::UpdateUInt32Value(uint16 index, uint32 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    m_uint32Values[index] = value;
    _changedFields[index] = true;
}

void Object::SetUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (*((uint64*)&(m_uint32Values[index])) != value)
    {
        m_uint32Values[index] = PAIR64_LOPART(value);
        m_uint32Values[index + 1] = PAIR64_HIPART(value);
        _changedFields[index] = true;
        _changedFields[index + 1] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

bool Object::AddUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (value && !*((uint64*)&(m_uint32Values[index])))
    {
        m_uint32Values[index] = PAIR64_LOPART(value);
        m_uint32Values[index + 1] = PAIR64_HIPART(value);
        _changedFields[index] = true;
        _changedFields[index + 1] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }

        return true;
    }

    return false;
}

bool Object::RemoveUInt64Value(uint16 index, uint64 value)
{
    ASSERT(index + 1 < m_valuesCount || PrintIndexError(index, true));
    if (value && *((uint64*)&(m_uint32Values[index])) == value)
    {
        m_uint32Values[index] = 0;
        m_uint32Values[index + 1] = 0;
        _changedFields[index] = true;
        _changedFields[index + 1] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }

        return true;
    }

    return false;
}

void Object::SetFloatValue(uint16 index, float value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (m_floatValues[index] != value)
    {
        m_floatValues[index] = value;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetByteValue(uint16 index, uint8 offset, uint8 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::SetByteValue: wrong offset %u", offset);
        return;
    }

    if (uint8(m_uint32Values[index] >> (offset * 8)) != value)
    {
        m_uint32Values[index] &= ~uint32(uint32(0xFF) << (offset * 8));
        m_uint32Values[index] |= uint32(uint32(value) << (offset * 8));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetUInt16Value(uint16 index, uint8 offset, uint16 value)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 2)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::SetUInt16Value: wrong offset %u", offset);
        return;
    }

    if (uint16(m_uint32Values[index] >> (offset * 16)) != value)
    {
        m_uint32Values[index] &= ~uint32(uint32(0xFFFF) << (offset * 16));
        m_uint32Values[index] |= uint32(uint32(value) << (offset * 16));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetStatFloatValue(uint16 index, float value)
{
    if (value < 0)
        value = 0.0f;

    SetFloatValue(index, value);
}

void Object::SetStatInt32Value(uint16 index, int32 value)
{
    if (value < 0)
        value = 0;

    SetUInt32Value(index, uint32(value));
}

void Object::ApplyModUInt32Value(uint16 index, int32 val, bool apply)
{
    int32 cur = GetUInt32Value(index);
    cur += (apply ? val : -val);
    if (cur < 0)
        cur = 0;
    SetUInt32Value(index, cur);
}

void Object::ApplyModInt32Value(uint16 index, int32 val, bool apply)
{
    int32 cur = GetInt32Value(index);
    cur += (apply ? val : -val);
    SetInt32Value(index, cur);
}

void Object::ApplyModSignedFloatValue(uint16 index, float  val, bool apply)
{
    float cur = GetFloatValue(index);
    cur += (apply ? val : -val);
    SetFloatValue(index, cur);
}

void Object::ApplyModPositiveFloatValue(uint16 index, float  val, bool apply)
{
    float cur = GetFloatValue(index);
    cur += (apply ? val : -val);
    if (cur < 0)
        cur = 0;
    SetFloatValue(index, cur);
}

void Object::SetFlag(uint16 index, uint32 newFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));
    uint32 oldval = m_uint32Values[index];
    uint32 newval = oldval | newFlag;

    if (oldval != newval)
    {
        m_uint32Values[index] = newval;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::RemoveFlag(uint16 index, uint32 oldFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));
    ASSERT(m_uint32Values);

    uint32 oldval = m_uint32Values[index];
    uint32 newval = oldval & ~oldFlag;

    if (oldval != newval)
    {
        m_uint32Values[index] = newval;
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetByteFlag(uint16 index, uint8 offset, uint8 newFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::SetByteFlag: wrong offset %u", offset);
        return;
    }

    if (!(uint8(m_uint32Values[index] >> (offset * 8)) & newFlag))
    {
        m_uint32Values[index] |= uint32(uint32(newFlag) << (offset * 8));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::RemoveByteFlag(uint16 index, uint8 offset, uint8 oldFlag)
{
    ASSERT(index < m_valuesCount || PrintIndexError(index, true));

    if (offset > 4)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object::RemoveByteFlag: wrong offset %u", offset);
        return;
    }

    if (uint8(m_uint32Values[index] >> (offset * 8)) & oldFlag)
    {
        m_uint32Values[index] &= ~uint32(uint32(oldFlag) << (offset * 8));
        _changedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

void Object::SetDynamicUInt32Value(uint32 tab, uint16 index, uint32 value)
{
    ASSERT(tab < _dynamicTabCount);
    ASSERT(index < DynamicFields::Count);

    if (_dynamicFields[tab]._dynamicValues[index] != value)
    {
        _dynamicFields[tab]._dynamicValues[index] = value;
        _dynamicFields[tab]._dynamicChangedFields[index] = true;

        if (m_inWorld && !m_objectUpdated)
        {
            sObjectAccessor->AddUpdateObject(this);
            m_objectUpdated = true;
        }
    }
}

bool Object::PrintIndexError(uint32 index, bool set) const
{
    sLog->outError(LOG_FILTER_GENERAL, "Attempt %s non-existed value field: %u (count: %u) for object typeid: %u type mask: %u", (set ? "set value to" : "get value from"), index, m_valuesCount, GetTypeId(), m_objectType);

    // ASSERT must fail after function call
    return false;
}

bool Position::HasInLine(WorldObject const* target, float width) const
{
    if (!HasInArc(M_PI, target))
        return false;
    width += target->GetObjectSize();
    float angle = GetRelativeAngle(target);
    return fabs(sin(angle)) * GetExactDist2d(target->GetPositionX(), target->GetPositionY()) < width;
}

std::string Position::ToString() const
{
    std::stringstream sstr;
    sstr << "X: " << m_positionX << " Y: " << m_positionY << " Z: " << m_positionZ << " O: " << m_orientation;
    return sstr.str();
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    float x, y, z, o;
    buf >> x >> y >> z >> o;
    streamer.m_pos->Relocate(x, y, z, o);
    return buf;
}
ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    streamer.m_pos->GetPosition(x, y, z);
    buf << x << y << z;
    return buf;
}

ByteBuffer& operator>>(ByteBuffer& buf, Position::PositionXYZStreamer const& streamer)
{
    float x, y, z;
    buf >> x >> y >> z;
    streamer.m_pos->Relocate(x, y, z);
    return buf;
}

ByteBuffer& operator<<(ByteBuffer& buf, Position::PositionXYZOStreamer const& streamer)
{
    float x, y, z, o;
    streamer.m_pos->GetPosition(x, y, z, o);
    buf << x << y << z << o;
    return buf;
}

void MovementInfo::OutDebug()
{
    sLog->outInfo(LOG_FILTER_GENERAL, "MOVEMENT INFO");
    sLog->outInfo(LOG_FILTER_GENERAL, "guid " UI64FMTD, guid);
    sLog->outInfo(LOG_FILTER_GENERAL, "flags %u", flags);
    sLog->outInfo(LOG_FILTER_GENERAL, "flags2 %u", flags2);
    sLog->outInfo(LOG_FILTER_GENERAL, "time %u current time " UI64FMTD "", flags2, uint64(::time(NULL)));
    sLog->outInfo(LOG_FILTER_GENERAL, "position: `%s`", pos.ToString().c_str());
    if (t_guid)
    {
        sLog->outInfo(LOG_FILTER_GENERAL, "TRANSPORT:");
        sLog->outInfo(LOG_FILTER_GENERAL, "guid: " UI64FMTD, t_guid);
        sLog->outInfo(LOG_FILTER_GENERAL, "position: `%s`", t_pos.ToString().c_str());
        sLog->outInfo(LOG_FILTER_GENERAL, "seat: %i", t_seat);
        sLog->outInfo(LOG_FILTER_GENERAL, "time: %u", t_time);
        if (flags2 & MOVEMENTFLAG2_INTERPOLATED_MOVEMENT)
            sLog->outInfo(LOG_FILTER_GENERAL, "time2: %u", t_time2);
    }

    if ((flags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) || (flags2 & MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING))
        sLog->outInfo(LOG_FILTER_GENERAL, "pitch: %f", pitch);

    sLog->outInfo(LOG_FILTER_GENERAL, "fallTime: %u", fallTime);
    if (flags & MOVEMENTFLAG_FALLING)
        sLog->outInfo(LOG_FILTER_GENERAL, "j_zspeed: %f j_sinAngle: %f j_cosAngle: %f j_xyspeed: %f", j_zspeed, j_sinAngle, j_cosAngle, j_xyspeed);

    if (flags & MOVEMENTFLAG_SPLINE_ELEVATION)
        sLog->outInfo(LOG_FILTER_GENERAL, "splineElevation: %f", splineElevation);
}

void MovementInfo::Normalize()
{
    pos.m_orientation = Position::NormalizeOrientation(pos.m_orientation);
    t_pos.m_orientation = Position::NormalizeOrientation(t_pos.m_orientation);

    pitch = Position::NormalizePitch(pitch);

    if (hasFallDirection && !hasFallData)
        hasFallDirection = false;
}

WorldObject::WorldObject(bool isWorldObject): WorldLocation(),
m_name(""), m_isActive(false), m_isWorldObject(isWorldObject), m_zoneScript(NULL),
m_transport(NULL), m_currMap(NULL), m_InstanceId(0),
m_phaseMask(PHASEMASK_NORMAL)
{
    m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE | GHOST_VISIBILITY_GHOST);
    m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);

    m_lastEntrySummon = 0;
    m_summonCounter = 0;
}

void WorldObject::SetWorldObject(bool on)
{
    if (!IsInWorld())
        return;

    GetMap()->AddObjectToSwitchList(this, on);
}

bool WorldObject::IsWorldObject() const
{
    if (m_isWorldObject)
        return true;

    if (ToCreature() && ToCreature()->m_isTempWorldObject)
        return true;

    return false;
}

void WorldObject::setActive(bool on)
{
    if (m_isActive == on)
        return;

    if (GetTypeId() == TYPEID_PLAYER)
        return;

    m_isActive = on;

    if (!IsInWorld())
        return;

    Map* map = FindMap();
    if (!map)
        return;

    if (on)
    {
        if (GetTypeId() == TYPEID_UNIT)
            map->AddToActive(this->ToCreature());
        else if (GetTypeId() == TYPEID_DYNAMICOBJECT)
            map->AddToActive((DynamicObject*)this);
    }
    else
    {
        if (GetTypeId() == TYPEID_UNIT)
            map->RemoveFromActive(this->ToCreature());
        else if (GetTypeId() == TYPEID_DYNAMICOBJECT)
            map->RemoveFromActive((DynamicObject*)this);
    }
}

void WorldObject::CleanupsBeforeDelete(bool /*finalCleanup*/)
{
    if (IsInWorld())
        RemoveFromWorld();
}

void WorldObject::_Create(uint32 guidlow, HighGuid guidhigh, uint32 phaseMask)
{
    Object::_Create(guidlow, 0, guidhigh);
    m_phaseMask = phaseMask;
}

uint32 WorldObject::GetZoneId() const
{
    return GetBaseMap()->GetZoneId(m_positionX, m_positionY, m_positionZ);
}

uint32 WorldObject::GetAreaId() const
{
    return GetBaseMap()->GetAreaId(m_positionX, m_positionY, m_positionZ);
}

void WorldObject::GetZoneAndAreaId(uint32& zoneid, uint32& areaid) const
{
    GetBaseMap()->GetZoneAndAreaId(zoneid, areaid, m_positionX, m_positionY, m_positionZ);
}

InstanceScript* WorldObject::GetInstanceScript()
{
    Map* map = GetMap();
    return map->IsDungeon() ? ((InstanceMap*)map)->GetInstanceScript() : NULL;
}

float WorldObject::GetDistanceZ(const WorldObject* obj) const
{
    float dz = fabs(GetPositionZ() - obj->GetPositionZ());
    float sizefactor = GetObjectSize() + obj->GetObjectSize();
    float dist = dz - sizefactor;
    return (dist > 0 ? dist : 0);
}

bool WorldObject::_IsWithinDist(WorldObject const* obj, float dist2compare, bool is3D) const
{
    float sizefactor = GetObjectSize() + obj->GetObjectSize();
    float maxdist = dist2compare + sizefactor;

    if (m_transport && obj->GetTransport() &&  obj->GetTransport()->GetGUIDLow() == m_transport->GetGUIDLow())
    {
        float dtx = m_movementInfo.t_pos.m_positionX - obj->m_movementInfo.t_pos.m_positionX;
        float dty = m_movementInfo.t_pos.m_positionY - obj->m_movementInfo.t_pos.m_positionY;
        float disttsq = dtx * dtx + dty * dty;
        if (is3D)
        {
            float dtz = m_movementInfo.t_pos.m_positionZ - obj->m_movementInfo.t_pos.m_positionZ;
            disttsq += dtz * dtz;
        }
        return disttsq < (maxdist * maxdist);
    }

    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float distsq = dx*dx + dy*dy;
    if (is3D)
    {
        float dz = GetPositionZ() - obj->GetPositionZ();
        distsq += dz*dz;
    }

    return distsq < maxdist * maxdist;
}

bool WorldObject::IsWithinLOSInMap(const WorldObject* obj) const
{
    if (!IsInMap(obj))
        return false;

    float ox, oy, oz;
    obj->GetPosition(ox, oy, oz);

    // Hack fix for Ice Tombs (Sindragosa encounter)
    if (obj->GetTypeId() == TYPEID_UNIT)
        if (obj->GetEntry() == 36980 || obj->GetEntry() == 38320 || obj->GetEntry() == 38321 || obj->GetEntry() == 38322)
            return true;

    // Hack fix for Alysrazor
    if (GetMapId() == 720 && GetAreaId() == 5766)
        if ((GetTypeId() == TYPEID_PLAYER) || (obj->GetTypeId() == TYPEID_PLAYER))
            return true;

    // AoE spells
    if (GetTypeId() == TYPEID_UNIT)
        if (GetEntry() == 36980 || GetEntry() == 38320 || GetEntry() == 38321 || GetEntry() == 38322)
            return true;

    return IsWithinLOS(ox, oy, oz);
}

bool WorldObject::IsWithinLOS(float ox, float oy, float oz) const
{
    /*float x, y, z;
    GetPosition(x, y, z);
    VMAP::IVMapManager* vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
    return vMapManager->isInLineOfSight(GetMapId(), x, y, z+2.0f, ox, oy, oz+2.0f);*/
    if (IsInWorld())
        return GetMap()->isInLineOfSight(GetPositionX(), GetPositionY(), GetPositionZ()+2.f, ox, oy, oz+2.f, GetPhaseMask());

    return true;
}

bool WorldObject::GetDistanceOrder(WorldObject const* obj1, WorldObject const* obj2, bool is3D /* = true */) const
{
    float dx1 = GetPositionX() - obj1->GetPositionX();
    float dy1 = GetPositionY() - obj1->GetPositionY();
    float distsq1 = dx1*dx1 + dy1*dy1;
    if (is3D)
    {
        float dz1 = GetPositionZ() - obj1->GetPositionZ();
        distsq1 += dz1*dz1;
    }

    float dx2 = GetPositionX() - obj2->GetPositionX();
    float dy2 = GetPositionY() - obj2->GetPositionY();
    float distsq2 = dx2*dx2 + dy2*dy2;
    if (is3D)
    {
        float dz2 = GetPositionZ() - obj2->GetPositionZ();
        distsq2 += dz2*dz2;
    }

    return distsq1 < distsq2;
}

bool WorldObject::IsInRange(WorldObject const* obj, float minRange, float maxRange, bool is3D /* = true */, bool useSizeFactor /* = true */) const
{
    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float distsq = dx*dx + dy*dy;
    if (is3D)
    {
        float dz = GetPositionZ() - obj->GetPositionZ();
        distsq += dz*dz;
    }

    float sizefactor = useSizeFactor ? (GetObjectSize() + obj->GetObjectSize()) : 0.0f;

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

bool WorldObject::IsInRange2d(float x, float y, float minRange, float maxRange) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;
    float distsq = dx*dx + dy*dy;

    float sizefactor = GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

bool WorldObject::IsInRange3d(float x, float y, float z, float minRange, float maxRange) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;
    float dz = GetPositionZ() - z;
    float distsq = dx*dx + dy*dy + dz*dz;

    float sizefactor = GetObjectSize();

    // check only for real range
    if (minRange > 0.0f)
    {
        float mindist = minRange + sizefactor;
        if (distsq < mindist * mindist)
            return false;
    }

    float maxdist = maxRange + sizefactor;
    return distsq < maxdist * maxdist;
}

void Position::RelocateOffset(const Position & offset)
{
    m_positionX = GetPositionX() + (offset.GetPositionX() * std::cos(GetOrientation()) + offset.GetPositionY() * std::sin(GetOrientation() + M_PI));
    m_positionY = GetPositionY() + (offset.GetPositionY() * std::cos(GetOrientation()) + offset.GetPositionX() * std::sin(GetOrientation()));
    m_positionZ = GetPositionZ() + offset.GetPositionZ();
    SetOrientation(GetOrientation() + offset.GetOrientation());
}

void Position::GetPositionOffsetTo(const Position & endPos, Position & retOffset) const
{
    float dx = endPos.GetPositionX() - GetPositionX();
    float dy = endPos.GetPositionY() - GetPositionY();

    retOffset.m_positionX = dx * std::cos(GetOrientation()) + dy * std::sin(GetOrientation());
    retOffset.m_positionY = dy * std::cos(GetOrientation()) - dx * std::sin(GetOrientation());
    retOffset.m_positionZ = endPos.GetPositionZ() - GetPositionZ();
    retOffset.SetOrientation(endPos.GetOrientation() - GetOrientation());
}

float Position::GetAngle(const Position* obj) const
{
    if (!obj)
        return 0;

    return GetAngle(obj->GetPositionX(), obj->GetPositionY());
}

// Return angle in range 0..2*pi
float Position::GetAngle(const float x, const float y) const
{
    float dx = x - GetPositionX();
    float dy = y - GetPositionY();

    float ang = atan2(dy, dx);
    ang = (ang >= 0) ? ang : 2 * M_PI + ang;
    return ang;
}

void Position::GetSinCos(const float x, const float y, float &vsin, float &vcos) const
{
    float dx = GetPositionX() - x;
    float dy = GetPositionY() - y;

    if (fabs(dx) < 0.001f && fabs(dy) < 0.001f)
    {
        float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
        vcos = std::cos(angle);
        vsin = std::sin(angle);
    }
    else
    {
        float dist = sqrt((dx*dx) + (dy*dy));
        vcos = dx / dist;
        vsin = dy / dist;
    }
}

bool Position::HasInArc(float arc, const Position* obj) const
{
    // always have self in arc
    if (obj == this)
        return true;

    // move arc to range 0.. 2*pi
    arc = NormalizeOrientation(arc);

    float angle = GetAngle(obj);
    angle -= m_orientation;

    // move angle to range -pi ... +pi
    angle = NormalizeOrientation(angle);
    if (angle > M_PI)
        angle -= 2.0f*M_PI;

    float lborder = -1 * (arc/2.0f);                        // in range -pi..0
    float rborder = (arc/2.0f);                             // in range 0..pi
    return ((angle >= lborder) && (angle <= rborder));
}

bool WorldObject::IsInBetween(const WorldObject* obj1, const WorldObject* obj2, float size) const
{
    if (!obj1 || !obj2)
        return false;

    float dist = GetExactDist2d(obj1->GetPositionX(), obj1->GetPositionY());

    // not using sqrt() for performance
    if ((dist * dist) >= obj1->GetExactDist2dSq(obj2->GetPositionX(), obj2->GetPositionY()))
        return false;

    if (!size)
        size = GetObjectSize() / 2;

    float angle = obj1->GetAngle(obj2);

    // not using sqrt() for performance
    return (size * size) >= GetExactDist2dSq(obj1->GetPositionX() + cos(angle) * dist, obj1->GetPositionY() + sin(angle) * dist);
}

bool WorldObject::IsInAxe(const WorldObject* obj1, const WorldObject* obj2, float size) const
{
    if (!obj1 || !obj2)
        return false;

    float dist = GetExactDist2d(obj1->GetPositionX(), obj1->GetPositionY());

    if (!size)
        size = GetObjectSize() / 2;

    float angle = obj1->GetAngle(obj2);

    // not using sqrt() for performance
    return (size * size) >= GetExactDist2dSq(obj1->GetPositionX() + cos(angle) * dist, obj1->GetPositionY() + sin(angle) * dist);
}

bool WorldObject::isInFront(WorldObject const* target,  float arc) const
{
    return HasInArc(arc, target);
}

bool WorldObject::isInBack(WorldObject const* target, float arc) const
{
    return !HasInArc(2 * M_PI - arc, target);
}

void WorldObject::GetRandomPoint(const Position &pos, float distance, float &rand_x, float &rand_y, float &rand_z) const
{
    if (!distance)
    {
        pos.GetPosition(rand_x, rand_y, rand_z);
        return;
    }

    // angle to face `obj` to `this`
    float angle = (float)rand_norm()*static_cast<float>(2*M_PI);
    float new_dist = (float)rand_norm()*static_cast<float>(distance);

    rand_x = pos.m_positionX + new_dist * std::cos(angle);
    rand_y = pos.m_positionY + new_dist * std::sin(angle);
    rand_z = pos.m_positionZ;

    JadeCore::NormalizeMapCoord(rand_x);
    JadeCore::NormalizeMapCoord(rand_y);
    UpdateGroundPositionZ(rand_x, rand_y, rand_z);            // update to LOS height if available
}

void WorldObject::UpdateGroundPositionZ(float x, float y, float &z) const
{
    float new_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
    if (new_z > INVALID_HEIGHT)
        z = new_z+ 0.05f;                                   // just to be sure that we are not a few pixel under the surface
}

void WorldObject::UpdateAllowedPositionZ(float x, float y, float &z) const
{
    switch (GetTypeId())
    {
        case TYPEID_UNIT:
        {
            // non fly unit don't must be in air
            // non swim unit must be at ground (mostly speedup, because it don't must be in water and water level check less fast
            if (!ToCreature()->CanFly())
            {
                bool canSwim = ToCreature()->isPet() ? true : ToCreature()->canSwim();
                float ground_z = z;
                float max_z = canSwim
                    ? GetBaseMap()->GetWaterOrGroundLevel(x, y, z, &ground_z, !ToUnit()->HasAuraType(SPELL_AURA_WATER_WALK))
                    : ((ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true)));
                if (max_z > INVALID_HEIGHT)
                {
                    if (z > max_z)
                        z = max_z;
                    else if (z < ground_z)
                        z = ground_z;
                }
            }
            else
            {
                float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
                if (z < ground_z)
                    z = ground_z;
            }
            break;
        }
        case TYPEID_PLAYER:
        {
            // for server controlled moves playr work same as creature (but it can always swim)
            if (!ToPlayer()->CanFly())
            {
                float ground_z = z;
                float max_z = GetBaseMap()->GetWaterOrGroundLevel(x, y, z, &ground_z, !ToUnit()->HasAuraType(SPELL_AURA_WATER_WALK));
                if (max_z > INVALID_HEIGHT)
                {
                    if (z > max_z)
                        z = max_z;
                    else if (z < ground_z)
                        z = ground_z;
                }
            }
            else
            {
                float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
                if (z < ground_z)
                    z = ground_z;
            }
            break;
        }
        default:
        {
            float ground_z = GetBaseMap()->GetHeight(GetPhaseMask(), x, y, z, true);
            if (ground_z > INVALID_HEIGHT)
                z = ground_z;
            break;
        }
    }
}

bool Position::IsPositionValid() const
{
    return JadeCore::IsValidMapCoord(m_positionX, m_positionY, m_positionZ, m_orientation);
}

float WorldObject::GetGridActivationRange() const
{
    if (ToPlayer())
        return GetMap()->GetVisibilityRange();
    else if (ToCreature())
        return ToCreature()->m_SightDistance;
    else
        return 0.0f;
}

float WorldObject::GetVisibilityRange() const
{
    if (isActiveObject() && !ToPlayer())
        return MAX_VISIBILITY_DISTANCE;
    else
        if (GetMap())
            return GetMap()->GetVisibilityRange();

    return MAX_VISIBILITY_DISTANCE;
}

float WorldObject::GetSightRange(const WorldObject* target) const
{
    if (ToUnit())
    {
        if (ToPlayer())
        {
            if (target && target->isActiveObject() && !target->ToPlayer())
                return MAX_VISIBILITY_DISTANCE;
            else
            {
                if (Map* map = GetMap())
                    return map->GetVisibilityRange();
                else
                    return MAX_VISIBILITY_DISTANCE;
            }
        }
        else if (ToCreature())
            return ToCreature()->m_SightDistance;
        else
            return SIGHT_RANGE_UNIT;
    }

    return 0.0f;
}

bool WorldObject::canSeeOrDetect(WorldObject const* obj, bool ignoreStealth, bool distanceCheck) const
{
    if (this == obj)
        return true;

    if (obj->MustBeVisibleOnlyForSomePlayers())
    {
        Player const* thisPlayer = ToPlayer();

        if (!thisPlayer)
            return false;

        if (!obj->IsPlayerInPersonnalVisibilityList(thisPlayer->GetGUID()))
            return false;
    }

    if (obj->IsNeverVisible() || CanNeverSee(obj))
        return false;

    if (obj->IsAlwaysVisibleFor(this) || CanAlwaysSee(obj))
        return true;

    bool corpseVisibility = false;
    if (distanceCheck)
    {
        bool corpseCheck = false;
        if (Player const* thisPlayer = ToPlayer())
        {
            if (thisPlayer->isDead() && thisPlayer->GetHealth() > 0 && // Cheap way to check for ghost state
                !(obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & GHOST_VISIBILITY_GHOST))
            {
                if (Corpse* corpse = thisPlayer->GetCorpse())
                {
                    corpseCheck = true;
                    if (corpse->IsWithinDist(thisPlayer, GetSightRange(obj), false))
                        if (corpse->IsWithinDist(obj, GetSightRange(obj), false))
                            corpseVisibility = true;
                }
            }
        }

        WorldObject const* viewpoint = this;
        if (Player const* player = this->ToPlayer())
            viewpoint = player->GetViewpoint();

        if (!viewpoint)
            viewpoint = this;

        if (!corpseCheck && !viewpoint->IsWithinDist(obj, GetSightRange(obj), false))
            return false;
    }

    // GM visibility off or hidden NPC
    if (!obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM))
    {
        // Stop checking other things for GMs
        if (m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GM))
            return true;
    }
    else
        return m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GM) >= obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM);

    // Ghost players, Spirit Healers, and some other NPCs
    if (!corpseVisibility && !(obj->m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GHOST) & m_serverSideVisibilityDetect.GetValue(SERVERSIDE_VISIBILITY_GHOST)))
    {
        // Alive players can see dead players in some cases, but other objects can't do that
        if (Player const* thisPlayer = ToPlayer())
        {
            if (Player const* objPlayer = obj->ToPlayer())
            {
                if (thisPlayer->GetTeam() != objPlayer->GetTeam() || !thisPlayer->IsGroupVisibleFor(objPlayer))
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }

    if (obj->IsInvisibleDueToDespawn())
        return false;

    if (!CanDetect(obj, ignoreStealth))
        return false;

    return true;
}

bool WorldObject::CanDetect(WorldObject const* obj, bool ignoreStealth) const
{
    const WorldObject* seer = this;

    // Pets don't have detection, they use the detection of their masters
    if (const Unit* thisUnit = ToUnit())
        if (Unit* controller = thisUnit->GetCharmerOrOwner())
            seer = controller;

    if (obj->IsAlwaysDetectableFor(seer))
        return true;

    if (!ignoreStealth && !seer->CanDetectInvisibilityOf(obj))
        return false;

    if (!ignoreStealth && !seer->CanDetectStealthOf(obj))
        return false;

    return true;
}

bool WorldObject::CanDetectInvisibilityOf(WorldObject const* obj) const
{
    // pets are invisible if caster is invisible
    if (obj->ToUnit() && obj->ToUnit()->GetOwner())
        return this->CanDetectInvisibilityOf(obj->ToUnit()->GetOwner());

    uint32 mask = obj->m_invisibility.GetFlags() & m_invisibilityDetect.GetFlags();

    // Check for not detected types
    if (mask != obj->m_invisibility.GetFlags())
        return false;

    for (uint32 i = 0; i < TOTAL_INVISIBILITY_TYPES; ++i)
    {
        if (!(mask & (1 << i)))
            continue;

        int32 objInvisibilityValue = obj->m_invisibility.GetValue(InvisibilityType(i));
        int32 ownInvisibilityDetectValue = m_invisibilityDetect.GetValue(InvisibilityType(i));

        // Too low value to detect
        if (ownInvisibilityDetectValue < objInvisibilityValue)
            return false;
    }

    return true;
}

bool WorldObject::CanDetectStealthOf(WorldObject const* obj) const
{
    // Combat reach is the minimal distance (both in front and behind),
    //   and it is also used in the range calculation.
    // One stealth point increases the visibility range by 0.3 yard.

    if (!obj->m_stealth.GetFlags())
        return true;

    float distance = GetExactDist(obj);

    // stealth detection of traps = invisibility detection, calculate from compare detection and stealth values
    if (obj->m_stealth.HasFlag(STEALTH_TRAP))
    {
        if (!HasInArc(M_PI, obj))
            return false;

        // rogue class - detect traps limit to 20 yards
        float maxDetectDistance = 20.0f;
        if (distance > maxDetectDistance)
            return false;

        int32 objTrapStealthValue = obj->m_stealth.GetValue(STEALTH_TRAP);
        int32 ownTrapStealthDetectValue = m_stealthDetect.GetValue(STEALTH_TRAP);

        if (ownTrapStealthDetectValue < objTrapStealthValue)
            // not rogue class - detect traps limit to melee distance
            if (distance > 4.0f)
                return false;
    }
    else
    {
        float combatReach = 0.0f;

        if (isType(TYPEMASK_UNIT))
        {
            combatReach = ((Unit*)this)->GetCombatReach();
            if (distance < combatReach)
                return true;

            if (((Unit*)this)->HasAuraType(SPELL_AURA_DETECT_STEALTH))
                return true;
        }

        if (!HasInArc(M_PI, obj))
            return false;

        // Starting points
        int32 detectionValue = 30;

        // Level difference: 5 point / level, starting from level 1.
        // There may be spells for this and the starting points too, but
        // not in the DBCs of the client.
        detectionValue += int32(getLevelForTarget(obj) - 1) * 5;

        // Apply modifiers
        detectionValue += m_stealthDetect.GetValue(STEALTH_GENERAL);
        if (obj->isType(TYPEMASK_GAMEOBJECT))
            if (Unit* owner = ((GameObject*)obj)->GetOwner())
                detectionValue -= int32(owner->getLevelForTarget(this) - 1) * 5;

        detectionValue -= obj->m_stealth.GetValue(STEALTH_GENERAL);

        // Calculate max distance
        float visibilityRange = float(detectionValue) * 0.3f + combatReach;

        if (visibilityRange > MAX_PLAYER_STEALTH_DETECT_RANGE)
            visibilityRange = MAX_PLAYER_STEALTH_DETECT_RANGE;

        if (distance > visibilityRange)
            return false;
    }

    return true;
}

bool WorldObject::IsPlayerInPersonnalVisibilityList(uint64 guid) const
{
    if (!IS_PLAYER_GUID(guid))
        return false;

    for (auto Itr: _visibilityPlayerList)
        if (Itr == guid)
            return true;

    return false;
}

void WorldObject::AddPlayersInPersonnalVisibilityList(std::list<uint64> viewerList)
{
    for (auto guid: viewerList)
    {
        if (!IS_PLAYER_GUID(guid))
            continue;

        _visibilityPlayerList.push_back(guid);
    }
}

void WorldObject::SendPlaySound(uint32 Sound, bool OnlySelf)
{
    uint8 bitsOrder[8] = { 6, 7, 5, 2, 1, 4, 0, 3 };
    uint8 bytesOrder[8] = { 7, 0, 5, 4, 3, 1, 2, 6 };

    ObjectGuid guid = GetGUID();

    WorldPacket data(SMSG_PLAY_SOUND, 12);
    data.WriteBitInOrder(guid, bitsOrder);
    data.WriteBytesSeq(guid, bytesOrder);
    data << Sound;

    if (OnlySelf && GetTypeId() == TYPEID_PLAYER)
        this->ToPlayer()->GetSession()->SendPacket(&data);
    else
        SendMessageToSet(&data, true); // ToSelf ignored in this case
}

void Object::ForceValuesUpdateAtIndex(uint32 i)
{
    _changedFields[i] = true;
    if (m_inWorld && !m_objectUpdated)
    {
        sObjectAccessor->AddUpdateObject(this);
        m_objectUpdated = true;
    }
}

namespace JadeCore
{
    class MonsterChatBuilder
    {
        public:
            MonsterChatBuilder(WorldObject const& obj, ChatMsg msgtype, int32 textId, uint32 language, uint64 targetGUID)
                : i_object(obj), i_msgtype(msgtype), i_textId(textId), i_language(language), i_targetGUID(targetGUID) {}
            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(i_textId, loc_idx);

                // TODO: i_object.GetName() also must be localized?
                i_object.BuildMonsterChat(&data, i_msgtype, text, i_language, i_object.GetNameForLocaleIdx(loc_idx), i_targetGUID);
            }

        private:
            WorldObject const& i_object;
            ChatMsg i_msgtype;
            int32 i_textId;
            uint32 i_language;
            uint64 i_targetGUID;
    };

    class MonsterCustomChatBuilder
    {
        public:
            MonsterCustomChatBuilder(WorldObject const& obj, ChatMsg msgtype, const char* text, uint32 language, uint64 targetGUID)
                : i_object(obj), i_msgtype(msgtype), i_text(text), i_language(language), i_targetGUID(targetGUID) {}
            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                // TODO: i_object.GetName() also must be localized?
                i_object.BuildMonsterChat(&data, i_msgtype, i_text, i_language, i_object.GetNameForLocaleIdx(loc_idx), i_targetGUID);
            }

        private:
            WorldObject const& i_object;
            ChatMsg i_msgtype;
            const char* i_text;
            uint32 i_language;
            uint64 i_targetGUID;
    };
}                                                           // namespace JadeCore

void WorldObject::MonsterSay(const char* text, uint32 language, uint64 TargetGuid)
{
    CellCoord p = JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    JadeCore::MonsterCustomChatBuilder say_build(*this, CHAT_MSG_MONSTER_SAY, text, language, TargetGuid);
    JadeCore::LocalizedPacketDo<JadeCore::MonsterCustomChatBuilder> say_do(say_build);
    JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterCustomChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), say_do);
    TypeContainerVisitor<JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterCustomChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY));
}

void WorldObject::MonsterSay(int32 textId, uint32 language, uint64 TargetGuid)
{
    CellCoord p = JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    JadeCore::MonsterChatBuilder say_build(*this, CHAT_MSG_MONSTER_SAY, textId, language, TargetGuid);
    JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> say_do(say_build);
    JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), say_do);
    TypeContainerVisitor<JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY));
}

void WorldObject::MonsterYell(const char* text, uint32 language, uint64 TargetGuid)
{
    CellCoord p = JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    JadeCore::MonsterCustomChatBuilder say_build(*this, CHAT_MSG_MONSTER_YELL, text, language, TargetGuid);
    JadeCore::LocalizedPacketDo<JadeCore::MonsterCustomChatBuilder> say_do(say_build);
    JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterCustomChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL), say_do);
    TypeContainerVisitor<JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterCustomChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL));
}

void WorldObject::MonsterYell(int32 textId, uint32 language, uint64 TargetGuid)
{
    CellCoord p = JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    JadeCore::MonsterChatBuilder say_build(*this, CHAT_MSG_MONSTER_YELL, textId, language, TargetGuid);
    JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> say_do(say_build);
    JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL), say_do);
    TypeContainerVisitor<JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_YELL));
}

void WorldObject::MonsterYellToZone(int32 textId, uint32 language, uint64 TargetGuid)
{
    JadeCore::MonsterChatBuilder say_build(*this, CHAT_MSG_MONSTER_YELL, textId, language, TargetGuid);
    JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> say_do(say_build);

    uint32 zoneid = GetZoneId();

    Map::PlayerList const& pList = GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator itr = pList.begin(); itr != pList.end(); ++itr)
        if (itr->getSource()->GetZoneId() == zoneid)
            say_do(itr->getSource());
}

void WorldObject::MonsterTextEmote(const char* text, uint64 TargetGuid, bool IsBossEmote)
{
    WorldPacket data;
    BuildMonsterChat(&data, IsBossEmote ? CHAT_MSG_RAID_BOSS_EMOTE : CHAT_MSG_MONSTER_EMOTE, text, LANG_UNIVERSAL, GetName(), TargetGuid);
    SendMessageToSetInRange(&data, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), true);
}

void WorldObject::MonsterTextEmote(int32 textId, uint64 TargetGuid, bool IsBossEmote)
{
    CellCoord p = JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY());

    Cell cell(p);
    cell.SetNoCreate();

    JadeCore::MonsterChatBuilder say_build(*this, IsBossEmote ? CHAT_MSG_RAID_BOSS_EMOTE : CHAT_MSG_MONSTER_EMOTE, textId, LANG_UNIVERSAL, TargetGuid);
    JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> say_do(say_build);
    JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> > say_worker(this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE), say_do);
    TypeContainerVisitor<JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::MonsterChatBuilder> >, WorldTypeMapContainer > message(say_worker);
    cell.Visit(p, message, *GetMap(), *this, sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE));
}

void WorldObject::MonsterWhisper(const char* text, uint64 receiver, bool IsBossWhisper)
{
    Player* player = ObjectAccessor::FindPlayer(receiver);
    if (!player || !player->GetSession())
        return;

    LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

    WorldPacket data;
    BuildMonsterChat(&data, IsBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_MONSTER_WHISPER, text, LANG_UNIVERSAL, GetNameForLocaleIdx(loc_idx), receiver);

    player->GetSession()->SendPacket(&data);
}

void WorldObject::MonsterWhisper(int32 textId, uint64 receiver, bool IsBossWhisper)
{
    Player* player = ObjectAccessor::FindPlayer(receiver);
    if (!player || !player->GetSession())
        return;

    LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
    char const* text = sObjectMgr->GetTrinityString(textId, loc_idx);

    WorldPacket data;
    BuildMonsterChat(&data, IsBossWhisper ? CHAT_MSG_RAID_BOSS_WHISPER : CHAT_MSG_MONSTER_WHISPER, text, LANG_UNIVERSAL, GetNameForLocaleIdx(loc_idx), receiver);

    player->GetSession()->SendPacket(&data);
}

void WorldObject::BuildMonsterChat(WorldPacket* data, uint8 msgtype, char const* text, uint32 language, char const* name, uint64 targetGuid) const
{
    uint32 messageLength = text ? strlen(text) : 0;
    uint32 speakerNameLength = name ? strlen(name) + 1 : 0;
    uint32 prefixeLength = 0;
    Unit* target = ObjectAccessor::FindUnit(targetGuid);
    uint32 receiverLength = target ? strlen(target->GetName()) : 0;
    uint32 channelLength = 0;
    std::string channel = ""; // no channel

    ObjectGuid senderGuid = GetGUID();
    ObjectGuid groupGuid = 0;
    ObjectGuid receiverGuid = targetGuid;
    ObjectGuid guildGuid = 0;

    bool unkBit = false;
    bool bit5256 = false;
    bool bit5264 = false;

    data->Initialize(SMSG_MESSAGE_CHAT, 200);
    data->WriteBit(0);                                          // Unk bit 5269
    data->WriteBit(messageLength ? 0 : 1);
    data->WriteBit(!bit5256);                                   // !Unk bit 5256
    data->WriteBit(0);                                          // !has sender
    data->WriteBit(1);                                          // has sender GUID

    uint8 bitsOrder[8] = { 2, 4, 0, 6, 1, 3, 5, 7 };
    data->WriteBitInOrder(senderGuid, bitsOrder);

    data->WriteBit(0);                                          // has group GUID

    uint8 bitsOrder2[8] = { 6, 0, 4, 1, 2, 3, 7, 5 };
    data->WriteBitInOrder(groupGuid, bitsOrder2);

    data->WriteBit(1);                                          // !has prefix
    data->WriteBit(0);                                          // Unk bit 5268
    data->WriteBit(!unkBit);                                    // !unk bit
    data->WriteBit(!bit5264);                                   // !unk bit 5264

    data->WriteBits(speakerNameLength, 11);

    data->WriteBit(receiverGuid ? 1 : 0);                       // has receiver GUID

    uint8 bitsOrder3[8] = { 4, 0, 6, 7, 5, 1, 3, 2 };
    data->WriteBitInOrder(receiverGuid, bitsOrder3);

    // Never happens for creature
    if (prefixeLength)
        data->WriteBits(prefixeLength, 5);

    data->WriteBit(receiverGuid ? 0 : 1);                       // !has receiver
    data->WriteBit(0);                                          // !has chat tag

    if (messageLength)
        data->WriteBits(messageLength, 12);

    data->WriteBit(0);                                          // !hasLanguage
    data->WriteBits(0, 9);                                      // chat Tag, empty for creatures
    data->WriteBit(0);                                          // has guild GUID

    if (receiverLength)
        data->WriteBits(receiverLength, 11);

    uint8 bitsOrder4[8] = { 0, 2, 1, 4, 6, 7, 5, 3 };
    data->WriteBitInOrder(guildGuid, bitsOrder4);

    data->WriteBit(1);                                          // !has channel
    data->WriteBits(channelLength, 7);

    if (channelLength)
        data->WriteString(channel);

    if (speakerNameLength)
        data->WriteString(name);

    uint8 byteOrder[8] = { 6, 7, 1, 2, 4, 3, 0, 5 };
    data->WriteBytesSeq(groupGuid, byteOrder);

    uint8 byteOrder1[8] = { 0, 4, 1, 3, 5, 7, 2, 6 };
    data->WriteBytesSeq(receiverGuid, byteOrder1);

    *data << uint8(msgtype);

    uint8 byteOrder2[8] = { 7, 6, 5, 4, 0, 2, 1, 3 };
    data->WriteBytesSeq(senderGuid, byteOrder2);

    // Never happens for creatures
    if (prefixeLength)
        data->WriteString("");

    if (unkBit)
        *data << uint32(0);                                         // unk uint32

    uint8 byteOrder3[8] = { 1, 0, 3, 7, 6, 5, 2, 4 };
    data->WriteBytesSeq(guildGuid, byteOrder3);

    if (receiverLength)
        data->WriteString("");

    if (bit5256)
        *data << uint32(0);                                         // unk uint32

    *data << uint8(language);

    if (messageLength)
        data->WriteString(text);

    if (bit5264)
        *data << uint32(0);                                         // unk uint32
}

void Unit::BuildHeartBeatMsg(WorldPacket* data) const
{
    data->Initialize(SMSG_MOVE_UPDATE);
    WriteMovementUpdate(*data);
}

void WorldObject::SendMessageToSet(WorldPacket* data, bool self)
{
    if (IsInWorld())
        SendMessageToSetInRange(data, GetVisibilityRange(), self);
}

void WorldObject::SendMessageToSetInRange(WorldPacket* data, float dist, bool /*self*/)
{
    JadeCore::MessageDistDeliverer notifier(this, data, dist);
    VisitNearbyWorldObject(dist, notifier);
}

void WorldObject::SendMessageToSet(WorldPacket* data, Player const* skipped_rcvr)
{
    JadeCore::MessageDistDeliverer notifier(this, data, GetVisibilityRange(), false, skipped_rcvr);
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

void WorldObject::SendObjectDeSpawnAnim(uint64 guid)
{
    WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
    data << uint64(guid);
    SendMessageToSet(&data, true);
}

void WorldObject::SetMap(Map* map)
{
    ASSERT(map);
    ASSERT(!IsInWorld() || GetTypeId() == TYPEID_CORPSE);
    if (m_currMap == map) // command add npc: first create, than loadfromdb
        return;
    if (m_currMap)
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::SetMap: obj %u new map %u %u, old map %u %u", (uint32)GetTypeId(), map->GetId(), map->GetInstanceId(), m_currMap->GetId(), m_currMap->GetInstanceId());
        ASSERT(false);
    }
    m_currMap = map;
    m_mapId = map->GetId();
    m_InstanceId = map->GetInstanceId();
    if (IsWorldObject())
        m_currMap->AddWorldObject(this);
}

void WorldObject::ResetMap()
{
    ASSERT(m_currMap);
    ASSERT(!IsInWorld());
    if (IsWorldObject())
        m_currMap->RemoveWorldObject(this);
    m_currMap = NULL;
    //maybe not for corpse
    //m_mapId = 0;
    //m_InstanceId = 0;
}

Map const* WorldObject::GetBaseMap() const
{
    ASSERT(m_currMap);
    return m_currMap->GetParent();
}

void WorldObject::AddObjectToRemoveList()
{
    ASSERT(m_uint32Values);

    Map* map = FindMap();
    if (!map)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Object (TypeId: %u Entry: %u GUID: %u) at attempt add to move list not have valid map (Id: %u).", GetTypeId(), GetEntry(), GetGUIDLow(), GetMapId());
        return;
    }

    map->AddObjectToRemoveList(this);
}

TempSummon* Map::SummonCreature(uint32 entry, Position const& pos, SummonPropertiesEntry const* properties /*= NULL*/, uint32 duration /*= 0*/, Unit* summoner /*= NULL*/, uint32 spellId /*= 0*/, uint32 vehId /*= 0*/, uint64 viewerGuid /*= 0*/, std::list<uint64>* viewersList /*= NULL*/)
{
    uint32 mask = UNIT_MASK_SUMMON;
    if (properties)
    {
        switch (properties->Category)
        {
            case SUMMON_CATEGORY_PET:
                mask = UNIT_MASK_GUARDIAN;
                break;
            case SUMMON_CATEGORY_PUPPET:
                mask = UNIT_MASK_PUPPET;
                break;
            case SUMMON_CATEGORY_VEHICLE:
                mask = UNIT_MASK_MINION;
                break;
            case SUMMON_CATEGORY_WILD:
            case SUMMON_CATEGORY_ALLY:
            case SUMMON_CATEGORY_UNK:
            {
                switch (properties->Type)
                {
                    case SUMMON_TYPE_MINION:
                    case SUMMON_TYPE_GUARDIAN:
                    case SUMMON_TYPE_GUARDIAN2:
                        mask = UNIT_MASK_GUARDIAN;
                        break;
                    case SUMMON_TYPE_TOTEM:
                        mask = UNIT_MASK_TOTEM;
                        break;
                    case SUMMON_TYPE_VEHICLE:
                    case SUMMON_TYPE_VEHICLE2:
                        mask = UNIT_MASK_SUMMON;
                        break;
                    case SUMMON_TYPE_MINIPET:
                        mask = UNIT_MASK_MINION;
                        break;
                    default:
                        if (properties->Flags & 512) // Mirror Image, Summon Gargoyle
                            mask = UNIT_MASK_GUARDIAN;
                        break;
                }
                break;
            }
            default:
                return NULL;
        }
    }

    switch (spellId)
    {
        case 114192:// Mocking Banner
        case 114203:// Demoralizing Banner
        case 114207:// Skull Banner
            mask = UNIT_MASK_GUARDIAN;
            break;
        default:
            break;
    }

    uint32 phase = PHASEMASK_NORMAL;
    uint32 team = 0;
    if (summoner)
    {
        phase = summoner->GetPhaseMask();
        if (summoner->GetTypeId() == TYPEID_PLAYER)
            team = summoner->ToPlayer()->GetTeam();
    }

    // Fix Serpent Jade Statue and Sturdy Ox Statue - is Guardian
    if (entry == 60849 || entry == 61146)
        mask = UNIT_MASK_GUARDIAN;

    TempSummon* summon = NULL;
    switch (mask)
    {
        case UNIT_MASK_SUMMON:
            summon = new TempSummon(properties, summoner, false);
            break;
        case UNIT_MASK_GUARDIAN:
            summon = new Guardian(properties, summoner, false);
            break;
        case UNIT_MASK_PUPPET:
            summon = new Puppet(properties, summoner);
            break;
        case UNIT_MASK_TOTEM:
            summon = new Totem(properties, summoner);
            break;
        case UNIT_MASK_MINION:
            summon = new Minion(properties, summoner, false);
            break;
    }

    if (!summon->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), this, phase, entry, vehId, team, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation()))
    {
        delete summon;
        return NULL;
    }

    summon->SetUInt32Value(UNIT_CREATED_BY_SPELL, spellId);

    summon->SetHomePosition(pos);

    summon->InitStats(duration);

    if (viewerGuid)
        summon->AddPlayerInPersonnalVisibilityList(viewerGuid);

    if (viewersList)
        summon->AddPlayersInPersonnalVisibilityList(*viewersList);

    AddToMap(summon->ToCreature());
    summon->InitSummon();

    //ObjectAccessor::UpdateObjectVisibility(summon);

    return summon;
}

/**
* Summons group of creatures.
*
* @param group Id of group to summon.
* @param list  List to store pointers to summoned creatures.
*/

void Map::SummonCreatureGroup(uint8 group, std::list<TempSummon*>& list)
{
    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetId(), SUMMONER_TYPE_MAP, group);
    if (!data)
        return;

    for (std::vector<TempSummonData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
        if (TempSummon* summon = SummonCreature(itr->entry, itr->pos, NULL, itr->time))
            list.push_back(summon);
}

void WorldObject::SetZoneScript()
{
    if (Map* map = FindMap())
    {
        if (map->IsDungeon())
            m_zoneScript = (ZoneScript*)((InstanceMap*)map)->GetInstanceScript();
        else if (!map->IsBattlegroundOrArena())
        {
            if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(GetZoneId()))
                m_zoneScript = bf;
            else
            {
                if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(GetZoneId()))
                    m_zoneScript = bf;
                else
                    m_zoneScript = sOutdoorPvPMgr->GetZoneScript(GetZoneId());
            }
        }
    }
}

TempSummon* WorldObject::SummonCreature(uint32 entry, const Position &pos, TempSummonType spwtype, uint32 duration, uint32 /*vehId*/, uint64 viewerGuid, std::list<uint64>* viewersList) const
{
    if (m_lastEntrySummon != entry)
    {
        m_lastEntrySummon = entry;
        m_summonCounter = 1;
    }
    else
    {
        m_summonCounter++;
        if (m_summonCounter > 20 && isType(TYPEMASK_PLAYER))
            sLog->OutPandashan("Player %u spam summon of creature %u [counter %u]", GetGUIDLow(), entry, m_summonCounter);
    }

    if (Map* map = FindMap())
    {
        if (TempSummon* summon = map->SummonCreature(entry, pos, NULL, duration, isType(TYPEMASK_UNIT) ? (Unit*)this : NULL, 0, 0, viewerGuid, viewersList))
        {
            summon->SetTempSummonType(spwtype);
            return summon;
        }
    }

    return NULL;
}

Pet* Player::SummonPet(uint32 entry, float x, float y, float z, float ang, PetType petType, uint32 duration, PetSlot slotID, bool stampeded)
{
    Pet* pet = new Pet(this, petType);

    bool currentPet = (slotID != PET_SLOT_UNK_SLOT);
    if (pet->GetOwner() && pet->GetOwner()->getClass() != CLASS_HUNTER)
        currentPet = false;

    //summoned pets always non-curent!
    if (petType == SUMMON_PET && pet->LoadPetFromDB(this, entry, 0, currentPet, slotID, stampeded))
    {
        if (pet->GetOwner() && pet->GetOwner()->getClass() == CLASS_WARLOCK)
        {
            if (pet->GetOwner()->HasAura(108503))
                pet->GetOwner()->RemoveAura(108503);

            // Supplant Command Demon
            if (pet->GetOwner()->getLevel() >= 56)
            {
                int32 bp = 0;

                pet->GetOwner()->RemoveAura(119904);

                switch (pet->GetEntry())
                {
                    case ENTRY_IMP:
                    case ENTRY_FEL_IMP:
                        bp = 119905;// Cauterize Master
                        break;
                    case ENTRY_VOIDWALKER:
                    case ENTRY_VOIDLORD:
                        bp = 119907;// Disarm
                        break;
                    case ENTRY_SUCCUBUS:
                        bp = 119909;// Whilplash
                        break;
                    case ENTRY_SHIVARRA:
                        bp = 119913;// Fellash
                        break;
                    case ENTRY_FELHUNTER:
                        bp = 119910;// Spell Lock
                        break;
                    case ENTRY_OBSERVER:
                        bp = 119911;// Optical Blast
                        break;
                    case ENTRY_FELGUARD:
                        bp = 119914;// Felstorm
                        break;
                    case ENTRY_WRATHGUARD:
                        bp = 119915;// Wrathstorm
                        break;
                    default:
                        break;
                }

                if (bp)
                    pet->GetOwner()->CastCustomSpell(pet->GetOwner(), 119904, &bp, NULL, NULL, true);
            }
        }

        if (pet->IsPetGhoul())
            pet->setPowerType(POWER_ENERGY);

        if (duration > 0)
            pet->SetDuration(duration);

        return pet;
    }

    if (stampeded)
        petType = HUNTER_PET;

    // petentry == 0 for hunter "call pet" (current pet summoned if any)
    if (!entry)
    {
        delete pet;
        return NULL;
    }

    pet->Relocate(x, y, z, ang);
    if (!pet->IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Pet (guidlow %d, entry %d) not summoned. Suggested coordinates isn't valid (X: %f Y: %f)", pet->GetGUIDLow(), pet->GetEntry(), pet->GetPositionX(), pet->GetPositionY());
        delete pet;
        return NULL;
    }

    Map* map = GetMap();
    uint32 pet_number = sObjectMgr->GeneratePetNumber();
    if (!pet->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_PET), map, GetPhaseMask(), entry, pet_number))
    {
        sLog->outError(LOG_FILTER_GENERAL, "no such creature entry %u", entry);
        delete pet;
        return NULL;
    }

    pet->SetCreatorGUID(GetGUID());
    pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, getFaction());

    pet->setPowerType(POWER_MANA);
    pet->SetUInt32Value(UNIT_NPC_FLAGS, 0);
    pet->SetUInt32Value(UNIT_NPC_FLAGS + 1, 0);
    pet->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    pet->InitStatsForLevel(getLevel());

    // Only slot 100, as it's not hunter pet.
    SetMinion(pet, true, PET_SLOT_OTHER_PET);

    switch (petType)
    {
        case SUMMON_PET:
            // this enables pet details window (Shift+P)
            pet->GetCharmInfo()->SetPetNumber(pet_number, true);
            pet->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
            pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
            pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
            pet->SetFullHealth();
            pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));
            pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped in this case
            break;
        default:
            break;
    }

    map->AddToMap(pet->ToCreature());

    switch (petType)
    {
        case SUMMON_PET:
            pet->InitPetCreateSpells();
            pet->SavePetToDB(PET_SLOT_ACTUAL_PET_SLOT);
            PetSpellInitialize();
            break;
        default:
            break;
    }

    if (pet->GetOwner() && pet->GetOwner()->getClass() == CLASS_WARLOCK)
    {
        if (pet->GetOwner()->HasAura(108503))
            pet->GetOwner()->RemoveAura(108503);

        // Supplant Command Demon
        if (pet->GetOwner()->getLevel() >= 56)
        {
            int32 bp = 0;

            pet->GetOwner()->RemoveAura(119904);

            switch (pet->GetEntry())
            {
                case ENTRY_IMP:
                case ENTRY_FEL_IMP:
                    bp = 119905;// Cauterize Master
                    break;
                case ENTRY_VOIDWALKER:
                case ENTRY_VOIDLORD:
                    bp = 119907;// Disarm
                    break;
                case ENTRY_SUCCUBUS:
                    bp = 119909; // Whiplash
                    break;
                case ENTRY_SHIVARRA:
                    bp = 119913;// Fellash
                    break;
                case ENTRY_FELHUNTER:
                    bp = 119910;// Spell Lock
                    break;
                case ENTRY_OBSERVER:
                    bp = 119911;// Optical Blast
                    break;
                case ENTRY_FELGUARD:
                    bp = 119914;// Felstorm
                    break;
                case ENTRY_WRATHGUARD:
                    bp = 119915;// Wrathstorm
                    break;
                default:
                    break;
            }

            if (bp)
                pet->GetOwner()->CastCustomSpell(pet->GetOwner(), 119904, &bp, NULL, NULL, true);
        }
    }

    if (duration > 0)
        pet->SetDuration(duration);

    return pet;
}

void Player::SendBattlegroundTimer(uint32 currentTime, uint32 maxTime)
{
    WorldPacket data(SMSG_START_TIMER, 12);
    data << uint32(maxTime);
    data << uint32(currentTime);
    data << uint32(PVP_TIMER);
    SendDirectMessage(&data);
}

GameObject* WorldObject::SummonGameObject(uint32 entry, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime, uint64 viewerGuid, std::list<uint64>* viewersList)
{
    if (!IsInWorld())
        return NULL;

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);
    if (!goinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject template %u not found in database!", entry);
        return NULL;
    }
    Map* map = GetMap();
    GameObject* go = new GameObject();
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, map, GetPhaseMask(), x, y, z, ang, rotation0, rotation1, rotation2, rotation3, 100, GO_STATE_READY))
    {
        delete go;
        return NULL;
    }
    go->SetRespawnTime(respawnTime);
    if (GetTypeId() == TYPEID_PLAYER || GetTypeId() == TYPEID_UNIT) //not sure how to handle this
        ToUnit()->AddGameObject(go);
    else
        go->SetSpawnedByDefault(false);

    if (viewerGuid)
        go->AddPlayerInPersonnalVisibilityList(viewerGuid);

    if (viewersList)
        go->AddPlayersInPersonnalVisibilityList(*viewersList);

    map->AddToMap(go);

    return go;
}

Creature* WorldObject::SummonTrigger(float x, float y, float z, float ang, uint32 duration, CreatureAI* (*GetAI)(Creature*))
{
    TempSummonType summonType = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN;
    Creature* summon = SummonCreature(WORLD_TRIGGER, x, y, z, ang, summonType, duration);
    if (!summon)
        return NULL;

    //summon->SetName(GetName());
    if (GetTypeId() == TYPEID_PLAYER || GetTypeId() == TYPEID_UNIT)
    {
        summon->setFaction(((Unit*)this)->getFaction());
        summon->SetLevel(((Unit*)this)->getLevel());
    }

    if (GetAI)
        summon->AIM_Initialize(GetAI(summon));
    return summon;
}

/**
* Summons group of creatures. Should be called only by instances of Creature and GameObject classes.
*
* @param group Id of group to summon.
* @param list  List to store pointers to summoned creatures.
*/
void WorldObject::SummonCreatureGroup(uint8 group, std::list<TempSummon*>& list)
{
    ASSERT((GetTypeId() == TYPEID_GAMEOBJECT || GetTypeId() == TYPEID_UNIT) && "Only GOs and creatures can summon npc groups!");

    std::vector<TempSummonData> const* data = sObjectMgr->GetSummonGroup(GetEntry(), GetTypeId() == TYPEID_GAMEOBJECT ? SUMMONER_TYPE_GAMEOBJECT : SUMMONER_TYPE_CREATURE, group);
    if (!data)
        return;

    for (std::vector<TempSummonData>::const_iterator itr = data->begin(); itr != data->end(); ++itr)
        if (TempSummon* summon = SummonCreature(itr->entry, itr->pos, itr->type, itr->time))
            list.push_back(summon);
}

Creature* WorldObject::FindNearestCreature(uint32 entry, float range, bool alive) const
{
    Creature* creature = NULL;
    JadeCore::NearestCreatureEntryWithLiveStateInObjectRangeCheck checker(*this, entry, alive, range);
    JadeCore::CreatureLastSearcher<JadeCore::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(this, creature, checker);
    VisitNearbyObject(range, searcher);
    return creature;
}

GameObject* WorldObject::FindNearestGameObject(uint32 entry, float range) const
{
    GameObject* go = NULL;
    JadeCore::NearestGameObjectEntryInObjectRangeCheck checker(*this, entry, range);
    JadeCore::GameObjectLastSearcher<JadeCore::NearestGameObjectEntryInObjectRangeCheck> searcher(this, go, checker);
    VisitNearbyGridObject(range, searcher);
    return go;
}

GameObject* WorldObject::FindNearestGameObjectOfType(GameobjectTypes type, float range) const
{ 
    GameObject* go = NULL;
    JadeCore::NearestGameObjectTypeInObjectRangeCheck checker(*this, type, range);
    JadeCore::GameObjectLastSearcher<JadeCore::NearestGameObjectTypeInObjectRangeCheck> searcher(this, go, checker);
    VisitNearbyGridObject(range, searcher);
    return go;
}

Player* WorldObject::FindNearestPlayer(float range, bool alive)
{
    Player* player = NULL;
    JadeCore::AnyPlayerInObjectRangeCheck check(this, range);
    JadeCore::PlayerSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(this, player, check);
    VisitNearbyWorldObject(range, searcher);
    return player;
}

void WorldObject::GetGameObjectListWithEntryInGrid(std::list<GameObject*>& gameobjectList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(JadeCore::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    JadeCore::AllGameObjectsWithEntryInRange check(this, entry, maxSearchRange);
    JadeCore::GameObjectListSearcher<JadeCore::AllGameObjectsWithEntryInRange> searcher(this, gameobjectList, check);
    TypeContainerVisitor<JadeCore::GameObjectListSearcher<JadeCore::AllGameObjectsWithEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetCreatureListWithEntryInGrid(std::list<Creature*>& creatureList, uint32 entry, float maxSearchRange) const
{
    CellCoord pair(JadeCore::ComputeCellCoord(this->GetPositionX(), this->GetPositionY()));
    Cell cell(pair);
    cell.SetNoCreate();

    JadeCore::AllCreaturesOfEntryInRange check(this, entry, maxSearchRange);
    JadeCore::CreatureListSearcher<JadeCore::AllCreaturesOfEntryInRange> searcher(this, creatureList, check);
    TypeContainerVisitor<JadeCore::CreatureListSearcher<JadeCore::AllCreaturesOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

    cell.Visit(pair, visitor, *(this->GetMap()), *this, maxSearchRange);
}

void WorldObject::GetPlayerListInGrid(std::list<Player*>& playerList, float maxSearchRange) const
{    
    JadeCore::AnyPlayerInObjectRangeCheck checker(this, maxSearchRange);
    JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(this, playerList, checker);
    this->VisitNearbyWorldObject(maxSearchRange, searcher);
}

void WorldObject::GetGameObjectListWithEntryInGridAppend(std::list<GameObject*>& gameobjectList, uint32 entry, float maxSearchRange) const
{
    std::list<GameObject*> tempList;
    GetGameObjectListWithEntryInGrid(tempList, entry, maxSearchRange);
    gameobjectList.sort();
    tempList.sort();
    gameobjectList.merge(tempList);
}

void WorldObject::GetCreatureListWithEntryInGridAppend(std::list<Creature*>& creatureList, uint32 entry, float maxSearchRange) const
{
    std::list<Creature*> tempList;
    GetCreatureListWithEntryInGrid(tempList, entry, maxSearchRange);
    creatureList.sort();
    tempList.sort();
    creatureList.merge(tempList);
}

/*
namespace JadeCore
{
    class NearUsedPosDo
    {
        public:
            NearUsedPosDo(WorldObject const& obj, WorldObject const* searcher, float angle, ObjectPosSelector& selector)
                : i_object(obj), i_searcher(searcher), i_angle(angle), i_selector(selector) {}

            void operator()(Corpse*) const {}
            void operator()(DynamicObject*) const {}

            void operator()(Creature* c) const
            {
                // skip self or target
                if (c == i_searcher || c == &i_object)
                    return;

                float x, y, z;

                if (!c->isAlive() || c->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED) ||
                    !c->GetMotionMaster()->GetDestination(x, y, z))
                {
                    x = c->GetPositionX();
                    y = c->GetPositionY();
                }

                add(c, x, y);
            }

            template<class T>
                void operator()(T* u) const
            {
                // skip self or target
                if (u == i_searcher || u == &i_object)
                    return;

                float x, y;

                x = u->GetPositionX();
                y = u->GetPositionY();

                add(u, x, y);
            }

            // we must add used pos that can fill places around center
            void add(WorldObject* u, float x, float y) const
            {
                // u is too nearest/far away to i_object
                if (!i_object.IsInRange2d(x, y, i_selector.m_dist - i_selector.m_size, i_selector.m_dist + i_selector.m_size))
                    return;

                float angle = i_object.GetAngle(u)-i_angle;

                // move angle to range -pi ... +pi
                while (angle > M_PI)
                    angle -= 2.0f * M_PI;
                while (angle < -M_PI)
                    angle += 2.0f * M_PI;

                // dist include size of u
                float dist2d = i_object.GetDistance2d(x, y);
                i_selector.AddUsedPos(u->GetObjectSize(), angle, dist2d + i_object.GetObjectSize());
            }
        private:
            WorldObject const& i_object;
            WorldObject const* i_searcher;
            float              i_angle;
            ObjectPosSelector& i_selector;
    };
}                                                           // namespace JadeCore
*/

//===================================================================================================

void WorldObject::GetNearPoint2D(float &x, float &y, float distance2d, float absAngle) const
{
    x = GetPositionX() + (GetObjectSize() + distance2d) * std::cos(absAngle);
    y = GetPositionY() + (GetObjectSize() + distance2d) * std::sin(absAngle);

    JadeCore::NormalizeMapCoord(x);
    JadeCore::NormalizeMapCoord(y);
}

void WorldObject::GetNearPoint(WorldObject const* searcher, float &x, float &y, float &z, float searcher_size, float distance2d, float absAngle) const
{
    GetNearPoint2D(x, y, distance2d+searcher_size, absAngle);
    z = GetPositionZ();
    if (!searcher || !searcher->ToCreature() || !searcher->GetMap()->Instanceable())
        UpdateAllowedPositionZ(x, y, z);
    /*
    // if detection disabled, return first point
    if (!sWorld->getIntConfig(CONFIG_DETECT_POS_COLLISION))
    {
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available
        return;
    }

    // or remember first point
    float first_x = x;
    float first_y = y;
    bool first_los_conflict = false;                        // first point LOS problems

    // prepare selector for work
    ObjectPosSelector selector(GetPositionX(), GetPositionY(), GetObjectSize(), distance2d+searcher_size);

    // adding used positions around object
    {
        CellCoord p(JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY()));
        Cell cell(p);
        cell.SetNoCreate();

        JadeCore::NearUsedPosDo u_do(*this, searcher, absAngle, selector);
        JadeCore::WorldObjectWorker<JadeCore::NearUsedPosDo> worker(this, u_do);

        TypeContainerVisitor<JadeCore::WorldObjectWorker<JadeCore::NearUsedPosDo>, GridTypeMapContainer  > grid_obj_worker(worker);
        TypeContainerVisitor<JadeCore::WorldObjectWorker<JadeCore::NearUsedPosDo>, WorldTypeMapContainer > world_obj_worker(worker);

        CellLock<GridReadGuard> cell_lock(cell, p);
        cell_lock->Visit(cell_lock, grid_obj_worker,  *GetMap(), *this, distance2d);
        cell_lock->Visit(cell_lock, world_obj_worker, *GetMap(), *this, distance2d);
    }

    // maybe can just place in primary position
    if (selector.CheckOriginal())
    {
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;

        first_los_conflict = true;                          // first point have LOS problems
    }

    float angle;                                            // candidate of angle for free pos

    // special case when one from list empty and then empty side preferred
    if (selector.FirstAngle(angle))
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // set first used pos in lists
    selector.InitializeAngle();

    // select in positions after current nodes (selection one by one)
    while (selector.NextAngle(angle))                        // angle for free pos
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // BAD NEWS: not free pos (or used or have LOS problems)
    // Attempt find _used_ pos without LOS problem

    if (!first_los_conflict)
    {
        x = first_x;
        y = first_y;

        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available
        return;
    }

    // special case when one from list empty and then empty side preferred
    if (selector.IsNonBalanced())
    {
        if (!selector.FirstAngle(angle))                     // _used_ pos
        {
            GetNearPoint2D(x, y, distance2d, absAngle+angle);
            z = GetPositionZ();
            UpdateGroundPositionZ(x, y, z);                   // update to LOS height if available

            if (IsWithinLOS(x, y, z))
                return;
        }
    }

    // set first used pos in lists
    selector.InitializeAngle();

    // select in positions after current nodes (selection one by one)
    while (selector.NextUsedAngle(angle))                    // angle for used pos but maybe without LOS problem
    {
        GetNearPoint2D(x, y, distance2d, absAngle+angle);
        z = GetPositionZ();
        UpdateGroundPositionZ(x, y, z);                       // update to LOS height if available

        if (IsWithinLOS(x, y, z))
            return;
    }

    // BAD BAD NEWS: all found pos (free and used) have LOS problem :(
    x = first_x;
    y = first_y;

    UpdateGroundPositionZ(x, y, z);                           // update to LOS height if available
    */
}

void WorldObject::MovePosition(Position &pos, float dist, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, ground, floor;
    destx = pos.m_positionX + dist * std::cos(angle);
    desty = pos.m_positionY + dist * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!JadeCore::IsValidMapCoord(destx, desty))
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::MovePosition invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    float step = dist/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else if (IsWithinLOS(destx, desty, destz))
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    JadeCore::NormalizeMapCoord(pos.m_positionX);
    JadeCore::NormalizeMapCoord(pos.m_positionY);
    UpdateGroundPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::MovePositionToFirstCollision(Position &pos, float dist, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, ground, floor;
    pos.m_positionZ += 2.0f;
    destx = pos.m_positionX + dist * std::cos(angle);
    desty = pos.m_positionY + dist * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!JadeCore::IsValidMapCoord(destx, desty))
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::MovePositionToFirstCollision invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    bool col = VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(GetMapId(), pos.m_positionX, pos.m_positionY, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // collision occurred
    if (col)
    {
        // move back a bit
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        dist = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    // check dynamic collision
    col = GetMap()->getObjectHitPos(GetPhaseMask(), pos.m_positionX, pos.m_positionY, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // Collided with a gameobject
    if (col)
    {
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        dist = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    float step = dist/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    JadeCore::NormalizeMapCoord(pos.m_positionX);
    JadeCore::NormalizeMapCoord(pos.m_positionY);
    UpdateAllowedPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::MovePositionToCollisionBetween(Position &pos, float distMin, float distMax, float angle)
{
    angle += GetOrientation();
    float destx, desty, destz, tempDestx, tempDesty, ground, floor;
    pos.m_positionZ += 2.0f;

    tempDestx = pos.m_positionX + distMin * std::cos(angle);
    tempDesty = pos.m_positionY + distMin * std::sin(angle);

    destx = pos.m_positionX + distMax * std::cos(angle);
    desty = pos.m_positionY + distMax * std::sin(angle);

    // Prevent invalid coordinates here, position is unchanged
    if (!JadeCore::IsValidMapCoord(destx, desty))
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "WorldObject::MovePositionToFirstCollision invalid coordinates X: %f and Y: %f were passed!", destx, desty);
        return;
    }

    ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
    floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
    destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

    bool col = VMAP::VMapFactory::createOrGetVMapManager()->getObjectHitPos(GetMapId(), tempDestx, tempDesty, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // collision occurred
    if (col)
    {
        // move back a bit
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        distMax = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    // check dynamic collision
    col = GetMap()->getObjectHitPos(GetPhaseMask(), tempDestx, tempDesty, pos.m_positionZ+0.5f, destx, desty, destz+0.5f, destx, desty, destz, -0.5f);

    // Collided with a gameobject
    if (col)
    {
        destx -= CONTACT_DISTANCE * std::cos(angle);
        desty -= CONTACT_DISTANCE * std::sin(angle);
        distMax = sqrt((pos.m_positionX - destx)*(pos.m_positionX - destx) + (pos.m_positionY - desty)*(pos.m_positionY - desty));
    }

    float step = distMax/10.0f;

    for (uint8 j = 0; j < 10; ++j)
    {
        // do not allow too big z changes
        if (fabs(pos.m_positionZ - destz) > 6)
        {
            destx -= step * std::cos(angle);
            desty -= step * std::sin(angle);
            ground = GetMap()->GetHeight(GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
            floor = GetMap()->GetHeight(GetPhaseMask(), destx, desty, pos.m_positionZ, true);
            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;
        }
        // we have correct destz now
        else
        {
            pos.Relocate(destx, desty, destz);
            break;
        }
    }

    JadeCore::NormalizeMapCoord(pos.m_positionX);
    JadeCore::NormalizeMapCoord(pos.m_positionY);
    UpdateAllowedPositionZ(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
    pos.SetOrientation(GetOrientation());
}

void WorldObject::SetPhaseMask(uint32 newPhaseMask, bool update)
{
    m_phaseMask = newPhaseMask;

    if (update && IsInWorld())
        UpdateObjectVisibility();
}

void WorldObject::PlayDistanceSound(uint32 sound_id, Player* target /*= NULL*/)
{
    WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 4+8);
    ObjectGuid guid = GetGUID();

    data.WriteBit(guid[3]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[4]);

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[1]);

    data << uint32(sound_id);

    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[3]);
    if (target)
        target->SendDirectMessage(&data);
    else
        SendMessageToSet(&data, true);
}

void WorldObject::PlayDirectSound(uint32 sound_id, Player* target /*= NULL*/)
{
    uint8 bitsOrder[8] = { 6, 7, 5, 2, 1, 4, 0, 3 };
    uint8 bytesOrder[8] = { 7, 0, 5, 4, 3, 1, 2, 6 };

    ObjectGuid guid = GetGUID();

    WorldPacket data(SMSG_PLAY_SOUND, 12);
    data.WriteBitInOrder(guid, bitsOrder);
    data.WriteBytesSeq(guid, bytesOrder);
    data << sound_id;
    if (target)
        target->SendDirectMessage(&data);
    else
        SendMessageToSet(&data, true);
}

void WorldObject::DestroyForNearbyPlayers()
{
    if (!IsInWorld())
        return;

    std::list<Player*> targets;
    JadeCore::AnyPlayerInObjectRangeCheck check(this, GetVisibilityRange(), false);
    JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(this, targets, check);
    VisitNearbyWorldObject(GetVisibilityRange(), searcher);
    for (std::list<Player*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
    {
        Player* player = (*iter);

        if (player == this)
            continue;

        if (!player->HaveAtClient(this))
            continue;

        if (isType(TYPEMASK_UNIT) && ((Unit*)this)->GetCharmerGUID() == player->GetGUID()) // TODO: this is for puppet
            continue;

        DestroyForPlayer(player);
        player->m_clientGUIDs.erase(GetGUID());
    }
}

void WorldObject::UpdateObjectVisibility(bool /*forced*/)
{
    //updates object's visibility for nearby players
    JadeCore::VisibleChangesNotifier notifier(*this);
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

struct WorldObjectChangeAccumulator
{
    UpdateDataMapType& i_updateDatas;
    WorldObject& i_object;
    std::set<uint64> plr_list;
    WorldObjectChangeAccumulator(WorldObject &obj, UpdateDataMapType &d) : i_updateDatas(d), i_object(obj) {}
    void Visit(PlayerMapType &m)
    {
        Player* source = NULL;
        for (PlayerMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->getSource();

            BuildPacket(source);

            if (!source->GetSharedVisionList().empty())
            {
                SharedVisionList::const_iterator it = source->GetSharedVisionList().begin();
                for (; it != source->GetSharedVisionList().end(); ++it)
                    BuildPacket(*it);
            }
        }
    }

    void Visit(CreatureMapType &m)
    {
        Creature* source = NULL;
        for (CreatureMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->getSource();
            if (!source->GetSharedVisionList().empty())
            {
                SharedVisionList::const_iterator it = source->GetSharedVisionList().begin();
                for (; it != source->GetSharedVisionList().end(); ++it)
                    BuildPacket(*it);
            }
        }
    }

    void Visit(DynamicObjectMapType &m)
    {
        DynamicObject* source = NULL;
        for (DynamicObjectMapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            source = iter->getSource();
            uint64 guid = source->GetCasterGUID();

            if (IS_PLAYER_GUID(guid))
            {
                //Caster may be NULL if DynObj is in removelist
                if (Player* caster = ObjectAccessor::FindPlayer(guid))
                    if (caster->GetUInt64Value(PLAYER_FARSIGHT) == source->GetGUID())
                        BuildPacket(caster);
            }
        }
    }

    void BuildPacket(Player* player)
    {
        // Only send update once to a player
        if (plr_list.find(player->GetGUID()) == plr_list.end() && player->HaveAtClient(&i_object))
        {
            i_object.BuildFieldsUpdate(player, i_updateDatas);
            plr_list.insert(player->GetGUID());
        }
    }

    template<class SKIP> void Visit(GridRefManager<SKIP> &) {}
};

void WorldObject::BuildUpdate(UpdateDataMapType& data_map)
{
    CellCoord p = JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY());
    Cell cell(p);
    cell.SetNoCreate();
    WorldObjectChangeAccumulator notifier(*this, data_map);
    TypeContainerVisitor<WorldObjectChangeAccumulator, WorldTypeMapContainer > player_notifier(notifier);
    Map& map = *GetMap();
    //we must build packets for all visible players
    cell.Visit(p, player_notifier, map, *this, GetVisibilityRange());

    ClearUpdateMask(false);
}

uint64 WorldObject::GetTransGUID() const
{
    if (GetTransport())
        return GetTransport()->GetGUID();
    return 0;
}
