////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TRINITY_VEHICLE_H
#define __TRINITY_VEHICLE_H

#include "ObjectDefines.h"
#include "VehicleDefines.h"

struct VehicleEntry;
class Unit;

typedef std::set<uint64> GuidSet;

class Vehicle : public TransportBase
{
    public:
        explicit Vehicle(Unit* unit, VehicleEntry const* vehInfo, uint32 creatureEntry);
        virtual ~Vehicle();

        void Install();
        void Uninstall(bool dismount = false);
        void Reset(bool evading = false);
        void InstallAllAccessories(bool evading);
        void ApplyAllImmunities();
        void InstallAccessory(uint32 entry, int8 seatId, bool minion, uint8 type, uint32 summonTime);   //! May be called from scripts

        Unit* GetBase() const { return _me; }
        VehicleEntry const* GetVehicleInfo() const { return _vehicleInfo; }
        uint32 GetCreatureEntry() const { return _creatureEntry; }

        bool HasEmptySeat(int8 seatId) const;
        Unit* GetPassenger(int8 seatId) const;
        int8 GetNextEmptySeat(int8 seatId, bool next) const;
        uint8 GetAvailableSeatCount() const;

        bool CheckCustomCanEnter();
        bool AddPassenger(Unit* passenger, int8 seatId = -1);
        void RemovePassenger(Unit* passenger);
        void RelocatePassengers();
        void RemoveAllPassengers(bool dismount = false);
        void Dismiss();
        bool IsVehicleInUse() const;

        inline bool ArePassengersSpawnedByAI() const { return _passengersSpawnedByAI; }
        void SetPassengersSpawnedByAI(bool passengersSpawnedByAI) { _passengersSpawnedByAI = passengersSpawnedByAI; }

        inline bool CanBeCastedByPassengers() const { return _canBeCastedByPassengers; }
        void SetCanBeCastedByPassengers(bool canBeCastedByPassengers) { _canBeCastedByPassengers = canBeCastedByPassengers; }

        SeatMap Seats;

        VehicleSeatEntry const* GetSeatForPassenger(Unit const* passenger);

    private:
        enum Status
        {
            STATUS_NONE,
            STATUS_INSTALLED,
            STATUS_UNINSTALLING
        };

        SeatMap::iterator GetSeatIteratorForPassenger(Unit* passenger);
        void InitMovementInfoForBase();

        /// This method transforms supplied transport offsets into global coordinates
        void CalculatePassengerPosition(float& x, float& y, float& z, float& o);

        /// This method transforms supplied global coordinates into local offsets
        void CalculatePassengerOffset(float& x, float& y, float& z, float& o);

        Unit* _me;
        VehicleEntry const* _vehicleInfo;
        GuidSet vehiclePlayers;
        uint32 _usableSeatNum;         // Number of seats that match VehicleSeatEntry::UsableByPlayer, used for proper display flags
        uint32 _creatureEntry;         // Can be different than me->GetBase()->GetEntry() in case of players
        Status _status;

        bool _passengersSpawnedByAI;
        bool _canBeCastedByPassengers;
};
#endif
