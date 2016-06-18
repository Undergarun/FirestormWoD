////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __BATTLEGROUNDNA_H
#define __BATTLEGROUNDNA_H

class Battleground;

enum BattlegroundNAObjectTypes
{
    BG_NA_OBJECT_DOOR_1         = 0,
    BG_NA_OBJECT_DOOR_2         = 1,
    BG_NA_OBJECT_DOOR_3         = 2,
    BG_NA_OBJECT_DOOR_4         = 3,
    BG_NA_OBJECT_BUFF_1         = 4,
    BG_NA_OBJECT_BUFF_2         = 5,
    BG_NA_OBJECT_MAX            = 6
};

enum BattlegroundNAObjects
{
    BG_NA_OBJECT_TYPE_DOOR_1    = 183978,
    BG_NA_OBJECT_TYPE_DOOR_2    = 183980,
    BG_NA_OBJECT_TYPE_DOOR_3    = 183977,
    BG_NA_OBJECT_TYPE_DOOR_4    = 183979,
    BG_NA_OBJECT_TYPE_BUFF_1    = 184663,
    BG_NA_OBJECT_TYPE_BUFF_2    = 184664
};

class BattlegroundNAScore : public BattlegroundScore
{
    public:
        BattlegroundNAScore() {};
        virtual ~BattlegroundNAScore() {};
        //TODO fix me
};

class BattlegroundNA : public Battleground
{
    public:
        BattlegroundNA();
        ~BattlegroundNA();

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player* player);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();

        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);
        bool SetupBattleground();
        virtual void Reset();
        virtual void FillInitialWorldStates(ByteBuffer &d);
        void HandleKillPlayer(Player* player, Player* killer);
        bool HandlePlayerUnderMap(Player* player);
#ifdef CROSS
        uint32 GetZoneId() const { return BATTLEGROUND_NA_ZONEID; }
#endif /* CROSS */
};
#endif
