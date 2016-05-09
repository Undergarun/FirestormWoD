////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Transport.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Path.h"
#include "ScriptMgr.h"
#include "WorldPacket.h"
#include "DBCStores.h"
#include "World.h"
#include "GameObjectAI.h"
#include "Vehicle.h"
#include "MapReference.h"
#include "Player.h"
#include "CellImpl.h"

Transport::Transport() : GameObject(),
    _transportInfo(NULL), _isMoving(true), _pendingStop(false), _delayedAddModel(false)
{
    m_updateFlag = UPDATEFLAG_HAS_SERVER_TIME | UPDATEFLAG_HAS_POSITION | UPDATEFLAG_HAS_ROTATION;
}

Transport::~Transport()
{
}

bool Transport::Create(uint32 guidlow, uint32 entry, uint32 mapid, float x, float y, float z, float ang, uint32 animprogress)
{
    Relocate(x, y, z, ang);

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_TRANSPORTS, "Transport (GUID: %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)",
            guidlow, x, y);
        return false;
    }

    Object::_Create(guidlow, 0, HIGHGUID_MO_TRANSPORT);

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);

    if (!goinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Transport not created: entry in `gameobject_template` not found, guidlow: %u map: %u  (X: %f Y: %f Z: %f) ang: %f", guidlow, mapid, x, y, z, ang);
        return false;
    }

    m_goInfo = goinfo;

    TransportTemplate const* tInfo = sTransportMgr->GetTransportTemplate(entry);
    if (!tInfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Transport %u (name: %s) will not be created, missing `transport_template` entry.", entry, goinfo->name.c_str());
        return false;
    }

    if (tInfo->pathTime == 0)
        return false;

    _transportInfo = tInfo;

    // initialize waypoints
    _nextFrame = tInfo->keyFrames.begin();
    _currentFrame = _nextFrame++;
    _triggeredArrivalEvent = false;
    _triggeredDepartureEvent = false;

    m_goValue->Transport.PathProgress = 0;
    SetFloatValue(OBJECT_FIELD_SCALE, goinfo->size);
    SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, goinfo->faction);
    SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, goinfo->flags);
    SetPeriod(tInfo->pathTime);
    SetEntry(goinfo->entry);
    SetDisplayId(goinfo->displayId);
    SetGoState(GO_STATE_READY);
    _pendingStop = goinfo->moTransport.allowstopping != 0;
    SetGoType(GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT);
    SetGoAnimProgress(animprogress);
    SetName(goinfo->name);
    if (GetCustomFlags() & eGoBCustomFlags::CustomFlagUseQuaternion)
        SetRotationAngles(0.0f, float(M_PI / 2.0f), 0.0f);
    else
        UpdateRotationFields(0.0f, 1.0f);
    return true;
}

void Transport::Update(uint32 diff)
{
    uint32 const positionUpdateDelay = 200;

    if (AI())
        AI()->UpdateAI(diff);
    else if (!AIM_Initialize())
        sLog->outDebug(LOG_FILTER_TRANSPORTS, "Could not initialize GameObjectAI for Transport");

    if (GetKeyFrames().size() <= 1)
        return;

    m_goValue->Transport.PathProgress += diff;

    uint32 timer = m_goValue->Transport.PathProgress % GetPeriod();

    // Set current waypoint
    // Desired outcome: _currentFrame->DepartureTime < timer < _nextFrame->ArriveTime
    // ... arrive | ... delay ... | departure
    //      event /         event /
    for (;;)
    {
        if (timer >= _currentFrame->ArriveTime)
        {
            if (!_triggeredArrivalEvent)
            {
                DoEventIfAny(*_currentFrame, false);
                _triggeredArrivalEvent = true;
            }

            if (timer < _currentFrame->DepartureTime)
            {
                SetMoving(false);
                if (_pendingStop)
                    SetGoState(GO_STATE_READY);
                break;  // its a stop frame and we are waiting
            }
        }

        if (_pendingStop && timer >= _currentFrame->DepartureTime && GetGoState() == GO_STATE_READY)
        {
            m_goValue->Transport.PathProgress = (m_goValue->Transport.PathProgress / GetPeriod());
            m_goValue->Transport.PathProgress *= GetPeriod();
            m_goValue->Transport.PathProgress += _currentFrame->ArriveTime;
            break;
        }

        if (timer >= _currentFrame->DepartureTime && !_triggeredDepartureEvent)
        {
            DoEventIfAny(*_currentFrame, true); // departure event
            _triggeredDepartureEvent = true;
        }

        if (timer >= _currentFrame->DepartureTime && timer < _currentFrame->NextArriveTime)
            break;  // found current waypoint

        MoveToNextWaypoint();

        // not waiting anymore
        SetMoving(true);

        // Enable movement
        if (GetGOInfo()->moTransport.allowstopping)
            SetGoState(GO_STATE_ACTIVE);

        // Departure event
        if (_currentFrame->IsTeleportFrame())
            TeleportTransport(_nextFrame->Node->MapID, _nextFrame->Node->x, _nextFrame->Node->y, _nextFrame->Node->z);

        sScriptMgr->OnRelocate(this, _currentFrame->Node->NodeIndex, _currentFrame->Node->MapID, _currentFrame->Node->x, _currentFrame->Node->y, _currentFrame->Node->z);

        sLog->outDebug(LOG_FILTER_TRANSPORTS, "Transport %u (%s) moved to node %u %u %f %f %f", GetEntry(), GetName(), _currentFrame->Node->NodeIndex, _currentFrame->Node->MapID, _currentFrame->Node->x, _currentFrame->Node->y, _currentFrame->Node->z);
    }

    // Add model to map after we are fully done with moving maps
    if (_delayedAddModel)
    {
        _delayedAddModel = false;
        if (m_model)
            GetMap()->InsertGameObjectModel(*m_model);
    }

    // Set position
    _positionChangeTimer.Update(diff);
    if (_positionChangeTimer.Passed())
    {
        _positionChangeTimer.Reset(positionUpdateDelay);
        if (IsMoving())
        {
            float t = CalculateSegmentPos(float(timer) * 0.001f);
            G3D::Vector3 pos, dir;
            _currentFrame->Spline->evaluate_percent(_currentFrame->Index, t, pos);
            //_currentFrame->Spline->evaluate_derivative(_currentFrame->Index, t, dir);
            UpdatePosition(pos.x, pos.y, pos.z, 0.0f/*atan2(dir.x, dir.y)*/);
        }
    }

    sScriptMgr->OnTransportUpdate(this, diff);
}

void Transport::AddPassenger(WorldObject* passenger)
{
    if (_passengers.insert(passenger).second)
        sLog->outDebug(LOG_FILTER_TRANSPORTS, "Object %s boarded transport %s.", passenger->GetName(), GetName());

    if (Player* plr = passenger->ToPlayer())
        sScriptMgr->OnAddPassenger(this, plr);
}

void Transport::RemovePassenger(WorldObject* passenger)
{
    if (_passengers.erase(passenger) || _staticPassengers.erase(passenger)) // static passenger can remove itself in case of grid unload
        sLog->outDebug(LOG_FILTER_TRANSPORTS, "Object %s removed from transport %s.", passenger->GetName(), GetName());


    if (Player* plr = passenger->ToPlayer())
        sScriptMgr->OnRemovePassenger(this, plr);
}

Creature* Transport::CreateNPCPassenger(uint32 guid, CreatureData const* data)
{
    Map* map = GetMap();
    Creature* creature = new Creature();

    if (!creature->LoadCreatureFromDB(guid, map, false))
    {
        delete creature;
        return NULL;
    }

    float x = data->posX;
    float y = data->posY;
    float z = data->posZ;
    float o = data->orientation;

    creature->SetTransport(this);
    creature->m_movementInfo.t_guid = GetGUID();
    creature->m_movementInfo.t_pos.Relocate(x, y, z, o);
    CalculatePassengerPosition(x, y, z, o);
    creature->Relocate(x, y, z, o);
    creature->SetHomePosition(creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation());
    creature->SetTransportHomePosition(creature->m_movementInfo.t_pos);

    /// @HACK - transport models are not added to map's dynamic LoS calculations
    ///         because the current GameObjectModel cannot be moved without recreating
    creature->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);

    if (!creature->IsPositionValid())
    {
        sLog->outDebug(LOG_FILTER_TRANSPORTS, "Creature (guidlow %d, entry %d) not created. Suggested coordinates aren't valid (X: %f Y: %f)", creature->GetGUIDLow(), creature->GetEntry(), creature->GetPositionX(), creature->GetPositionY());
        delete creature;
        return NULL;
    }

    map->AddToMap(creature);
    _staticPassengers.insert(creature);

    sScriptMgr->OnAddCreaturePassenger(this, creature);
    return creature;
}

GameObject* Transport::CreateGOPassenger(uint32 guid, GameObjectData const* data)
{
    Map* map = GetMap();
    GameObject* go = new GameObject();

    if (!go->LoadGameObjectFromDB(guid, map, false))
    {
        delete go;
        return NULL;
    }

    float x = data->posX;
    float y = data->posY;
    float z = data->posZ;
    float o = data->orientation;

    go->SetTransport(this);
    go->m_movementInfo.t_guid = GetGUID();
    go->m_movementInfo.t_pos.Relocate(x, y, z, o);
    CalculatePassengerPosition(x, y, z, o);
    go->Relocate(x, y, z, o);

    if (!go->IsPositionValid())
    {
        sLog->outDebug(LOG_FILTER_TRANSPORTS, "GameObject (guidlow %d, entry %d) not created. Suggested coordinates aren't valid (X: %f Y: %f)", go->GetGUIDLow(), go->GetEntry(), go->GetPositionX(), go->GetPositionY());
        delete go;
        return NULL;
    }

    map->AddToMap(go);
    _staticPassengers.insert(go);

    //sScriptMgr->OnAddCreaturePassenger(this, go);
    return go;
}

void Transport::CalculatePassengerPosition(float& x, float& y, float& z, float& o) 
{
    float inx = x, iny = y, inz = z;
    if (o)
        o = Position::NormalizeOrientation(GetOrientation() + o);

    x = GetPositionX() + inx * std::cos(GetOrientation()) - iny * std::sin(GetOrientation());
    y = GetPositionY() + iny * std::cos(GetOrientation()) + inx * std::sin(GetOrientation());
    z = GetPositionZ() + inz;
}

void Transport::CalculatePassengerOffset(float& x, float& y, float& z, float& o)
{
    if (o)
        o = Position::NormalizeOrientation(o - GetOrientation());

    z -= GetPositionZ();
    y -= GetPositionY();    // y = searchedY * std::cos(o) + searchedX * std::sin(o)
    x -= GetPositionX();    // x = searchedX * std::cos(o) + searchedY * std::sin(o + pi)
    float inx = x, iny = y;
    y = (iny - inx * std::tan(GetOrientation())) / (std::cos(GetOrientation()) + std::sin(GetOrientation()) * std::tan(GetOrientation()));
    x = (inx + iny * std::tan(GetOrientation())) / (std::cos(GetOrientation()) + std::sin(GetOrientation()) * std::tan(GetOrientation()));
}

void Transport::UpdatePosition(float x, float y, float z, float o)
{
    bool newActive = GetMap()->IsGridLoaded(x, y);

    Relocate(x, y, z, o);

    UpdatePassengerPositions(_passengers);

    /* There are four possible scenarios that trigger loading/unloading passengers:
      1. transport moves from inactive to active grid
      2. the grid that transport is currently in becomes active
      3. transport moves from active to inactive grid
      4. the grid that transport is currently in unloads
    */
    if (_staticPassengers.empty() && newActive) // 1. and 2.
        LoadStaticPassengers();
    else if (!_staticPassengers.empty() && !newActive && Cell(x, y).DiffGrid(Cell(GetPositionX(), GetPositionY()))) // 3.
        UnloadStaticPassengers();
    else
        UpdatePassengerPositions(_staticPassengers);
    // 4. is handed by grid unload
}

void Transport::LoadStaticPassengers()
{
    if (uint32 mapId = GetGOInfo()->moTransport.mapID)
    {
        CellObjectGuidsMap const& cells = sObjectMgr->GetMapObjectGuids(mapId, GetMap()->GetSpawnMode());
        CellGuidSet::const_iterator guidEnd;
        for (CellObjectGuidsMap::const_iterator cellItr = cells.begin(); cellItr != cells.end(); ++cellItr)
        {
            // Creatures on transport
            guidEnd = cellItr->second.creatures.end();
            for (CellGuidSet::const_iterator guidItr = cellItr->second.creatures.begin(); guidItr != guidEnd; ++guidItr)
                CreateNPCPassenger(*guidItr, sObjectMgr->GetCreatureData(*guidItr));

            // GameObjects on transport
            guidEnd = cellItr->second.gameobjects.end();
            for (CellGuidSet::const_iterator guidItr = cellItr->second.gameobjects.begin(); guidItr != guidEnd; ++guidItr)
                CreateGOPassenger(*guidItr, sObjectMgr->GetGOData(*guidItr));
        }
    }
}

void Transport::UnloadStaticPassengers()
{
    while (!_staticPassengers.empty())
    {
        WorldObject* obj = *_staticPassengers.begin();
        obj->AddObjectToRemoveList();   // also removes from _staticPassengers
    }
}

void Transport::EnableMovement(bool enabled)
{
    if (!GetGOInfo()->moTransport.allowstopping)
        return;

    _pendingStop = !enabled;
}

void Transport::MoveToNextWaypoint()
{
    // Clear events flagging
    _triggeredArrivalEvent = false;
    _triggeredDepartureEvent = false;

    // Set frames
    _currentFrame = _nextFrame++;
    if (_nextFrame == GetKeyFrames().end())
        _nextFrame = GetKeyFrames().begin();
}

float Transport::CalculateSegmentPos(float now)
{
    KeyFrame const& frame = *_currentFrame;
    const float speed = float(m_goInfo->moTransport.moveSpeed);
    const float accel = float(m_goInfo->moTransport.accelRate);
    float timeSinceStop = frame.TimeFrom + (now - (1.0f/IN_MILLISECONDS) * frame.DepartureTime);
    float timeUntilStop = frame.TimeTo - (now - (1.0f/IN_MILLISECONDS) * frame.DepartureTime);
    float segmentPos, dist;
    float accelTime = _transportInfo->accelTime;
    float accelDist = _transportInfo->accelDist;
    // calculate from nearest stop, less confusing calculation...
    if (timeSinceStop < timeUntilStop)
    {
        if (timeSinceStop < accelTime)
            dist = 0.5f * accel * timeSinceStop * timeSinceStop;
        else
            dist = accelDist + (timeSinceStop - accelTime) * speed;
        segmentPos = dist - frame.DistSinceStop;
    }
    else
    {
        if (timeUntilStop < _transportInfo->accelTime)
            dist = 0.5f * accel * timeUntilStop * timeUntilStop;
        else
            dist = accelDist + (timeUntilStop - accelTime) * speed;
        segmentPos = frame.DistUntilStop - dist;
    }

    return segmentPos / frame.NextDistFromPrev;
}

void Transport::TeleportTransport(uint32 newMapid, float x, float y, float z)
{
    Map const* oldMap = GetMap();

    if (oldMap->GetId() != newMapid)
    {
        Map* newMap = sMapMgr->CreateBaseMap(newMapid);
        Map::PlayerList const& oldPlayers = GetMap()->GetPlayers();
        if (!oldPlayers.isEmpty())
        {
            UpdateData data(oldMap->GetId());
            BuildOutOfRangeUpdateBlock(&data);
            WorldPacket packet;
            data.BuildPacket(&packet);
            for (Map::PlayerList::const_iterator itr = oldPlayers.begin(); itr != oldPlayers.end(); ++itr)
                if (itr->getSource()->GetTransport() != this)
                    itr->getSource()->SendDirectMessage(&packet);
        }

        UnloadStaticPassengers();
        GetMap()->RemoveFromMap<Transport>(this, false);
        SetMap(newMap);

        Map::PlayerList const& newPlayers = GetMap()->GetPlayers();
        if (!newPlayers.isEmpty())
        {
            for (Map::PlayerList::const_iterator itr = newPlayers.begin(); itr != newPlayers.end(); ++itr)
            {
                if (itr->getSource()->GetTransport() != this)
                {
                    UpdateData data(GetMap()->GetId());
                    BuildCreateUpdateBlockForPlayer(&data, itr->getSource());
                    WorldPacket packet;
                    data.BuildPacket(&packet);
                    itr->getSource()->SendDirectMessage(&packet);
                }
            }
        }

        // Teleport passengers after everyone on destination map are sent create packet
        // but before transport itself is registered there and begins updating
        for (std::set<WorldObject*>::iterator itr = _staticPassengers.begin(); itr != _staticPassengers.end(); ++itr)
        {
            switch ((*itr)->GetTypeId())
            {
                case TYPEID_UNIT:
                    (*itr)->ToCreature()->FarTeleportTo(newMap, x, y, z, (*itr)->GetOrientation());
                    break;
                case TYPEID_GAMEOBJECT:
                {
                    GameObject* go = (*itr)->ToGameObject();
                    go->GetMap()->RemoveFromMap(go, false);
                    Relocate(x, y, z, go->GetOrientation());
                    SetMap(newMap);
                    newMap->AddToMap(go);
                    break;
                }
                default:
                    return;
            }
        }

        for (std::set<WorldObject*>::iterator itr = _passengers.begin(); itr != _passengers.end(); ++itr)
        {
            switch ((*itr)->GetTypeId())
            {
                case TYPEID_UNIT:
                    if (!IS_PLAYER_GUID((*itr)->ToUnit()->GetOwnerGUID()))  // pets should be teleported with player
                        (*itr)->ToCreature()->FarTeleportTo(newMap, x, y, z, (*itr)->GetOrientation());
                    break;
                case TYPEID_GAMEOBJECT:
                {
                    GameObject* go = (*itr)->ToGameObject();
                    go->GetMap()->RemoveFromMap(go, false);
                    Relocate(x, y, z, go->GetOrientation());
                    SetMap(newMap);
                    newMap->AddToMap(go);
                    break;
                }
                case TYPEID_PLAYER:
                    (*itr)->ToPlayer()->TeleportTo(newMapid, x, y, z, (*itr)->GetOrientation(), TELE_TO_NOT_LEAVE_TRANSPORT);
                    break;
                default:
                    return;
            }
            
        }

        GetMap()->AddToMap<Transport>(this);
    }
    else
    {
        /// That will be better to teleport it server-side too, but since 6.2.3 when we send SMSG_MOVE_TELEPORT with transport guid the client teleport the player to invalid position
        /// The client will do the teleporation client-side and send the new position to the server anyway

        /// Teleport players, they need to know it
        /*for (std::set<WorldObject*>::iterator itr = _passengers.begin(); itr != _passengers.end(); ++itr)
            if ((*itr)->IsPlayer())
                (*itr)->ToUnit()->NearTeleportTo(x, y, z, GetOrientation());*/
    }

    UpdatePosition(x, y, z, GetOrientation());
}

void Transport::UpdatePassengerPositions(std::set<WorldObject*>& passengers)
{
    for (std::set<WorldObject*>::iterator itr = passengers.begin(); itr != passengers.end(); ++itr)
    {
        WorldObject* passenger = *itr;
        // transport teleported but passenger not yet (can happen for players)
        if (passenger->GetMap() != GetMap())
            continue;

        // if passenger is on vehicle we have to assume the vehicle is also on transport
        // and its the vehicle that will be updating its passengers
        if (Unit* unit = passenger->ToUnit())
            if (unit->GetVehicle())
                continue;

        // Do not use Unit::UpdatePosition here, we don't want to remove auras
        // as if regular movement occurred
        float x, y, z, o;
        passenger->m_movementInfo.t_pos.GetPosition(x, y, z, o);
        CalculatePassengerPosition(x, y, z, o);
        switch (passenger->GetTypeId())
        {
            case TYPEID_UNIT:
            {
                Creature* creature = passenger->ToCreature();
                GetMap()->CreatureRelocation(creature, x, y, z, o, false);
                creature->GetTransportHomePosition(x, y, z, o);
                CalculatePassengerPosition(x, y, z, o);
                creature->SetHomePosition(x, y, z, o);
                break;
            }
            case TYPEID_PLAYER:
                GetMap()->PlayerRelocation(passenger->ToPlayer(), x, y, z, o);
                break;
            case TYPEID_GAMEOBJECT:
                GetMap()->GameObjectRelocation(passenger->ToGameObject(), x, y, z, o, false);
                break;
            default:
                return;
        }

        if (Unit* unit = passenger->ToUnit())
            if (Vehicle* vehicle = unit->GetVehicleKit())
                vehicle->RelocatePassengers();
    }
}

void Transport::DoEventIfAny(KeyFrame const& node, bool departure)
{
    if (uint32 eventid = departure ? node.Node->DepartureEventID : node.Node->ArrivalEventID)
    {
        sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "Taxi %s event %u of node %u of %s path", departure ? "departure" : "arrival", eventid, node.Node->NodeIndex, GetName());
        GetMap()->ScriptsStart(sEventScripts, eventid, this, this);
        EventInform(eventid);
    }
}

void Transport::BuildUpdate(UpdateDataMapType& data_map)
{
    Map::PlayerList const& players = GetMap()->GetPlayers();
    if (players.isEmpty())
        return;

    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        BuildFieldsUpdate(itr->getSource(), data_map);

    ClearUpdateMask(true);
}
