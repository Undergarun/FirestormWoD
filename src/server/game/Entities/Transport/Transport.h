////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRANSPORTS_H
#define TRANSPORTS_H

#include "GameObject.h"
#include "TransportMgr.h"
#include "VehicleDefines.h"

struct CreatureData;

class Transport : public GameObject, public TransportBase
{
        friend Transport* TransportMgr::CreateTransport(uint32, uint32, Map*);

        Transport();
    public:
        ~Transport();

        bool Create(uint32 guidlow, uint32 entry, uint32 mapid, float x, float y, float z, float ang, uint32 animprogress);
        void Update(uint32 diff);

        void BuildUpdate(UpdateDataMapType& data_map);

        void AddPassenger(WorldObject* passenger);
        void RemovePassenger(WorldObject* passenger);
        std::set<WorldObject*> const& GetPassengers() const { return _passengers; }

        Creature* CreateNPCPassenger(uint32 guid, CreatureData const* data);
        GameObject* CreateGOPassenger(uint32 guid, GameObjectData const* data);

        /// This method transforms supplied transport offsets into global coordinates
        void CalculatePassengerPosition(float& x, float& y, float& z, float& o);

        /// This method transforms supplied global coordinates into local offsets
        void CalculatePassengerOffset(float& x, float& y, float& z, float& o);

        uint32 GetPeriod() const { return GetUInt32Value(GAMEOBJECT_FIELD_LEVEL); }
        void SetPeriod(uint32 period) { SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, period); }
        uint32 GetTimer() const { return GetGOValue()->Transport.PathProgress; }

        KeyFrameVec const& GetKeyFrames() const { return _transportInfo->keyFrames; }

        void UpdatePosition(float x, float y, float z, float o);

        //! Needed when transport moves from inactive to active grid
        void LoadStaticPassengers();

        //! Needed when transport enters inactive grid
        void UnloadStaticPassengers();

        void EnableMovement(bool enabled);
        void SetDelayedAddModelToMap() { _delayedAddModel = true; }

    private:
        void MoveToNextWaypoint();
        float CalculateSegmentPos(float perc);
        void TeleportTransport(uint32 newMapid, float x, float y, float z);
        void UpdatePassengerPositions(std::set<WorldObject*>& passengers);
        void DoEventIfAny(KeyFrame const& node, bool departure);

        //! Helpers to know if stop frame was reached
        bool IsMoving() const { return _isMoving; }
        void SetMoving(bool val) { _isMoving = val; }

        TransportTemplate const* _transportInfo;

        KeyFrameVec::const_iterator _currentFrame;
        KeyFrameVec::const_iterator _nextFrame;
        uint32 _moveTimer;
        TimeTrackerSmall _positionChangeTimer;
        bool _isMoving;
        bool _pendingStop;

        //! These are needed to properly control events triggering only once for each frame
        bool _triggeredArrivalEvent;
        bool _triggeredDepartureEvent;

        std::set<WorldObject*> _passengers;
        std::set<WorldObject*> _staticPassengers;

        bool _delayedAddModel;
};

#endif
