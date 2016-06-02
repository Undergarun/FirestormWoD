////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

enum eThogarMiscDatas : uint8
{
    /// Tracks handling
    FirstTrack,
    SecondTrack,
    ThirdTrack,
    FourthTrack,
    MaxTrainTracks,
    /// Trains handling
    /// Trains are large solid objects that block line of sight and that deal lethal damage to any players who are hit by them.
    /// There are 4 types of train and they can come in two forms:
    /// full-length train (train that is as long as a track) and half-length train (train that is half as long as a track).
    NonAddTrain = 0,
    SiegeTrain,
    HalfLengthAddTrains,
    FullLengthAddTrains,
    MaxTrainType,
    MaxFightTrains = 15,
    /// Adds datas
    MaxIronRaiders = 8
};

enum eThogarCreatures
{
    TrainEngine         = 77181,
    TrainWheels         = 76907,
    Train1              = 82107,
    Train2              = 81226,
    TroopTransport1     = 78842,
    TroopTransport2     = 81752,
    TroopTransport3     = 77158,
    WeaponsTransport1   = 81768,
    WeaponsTransport2   = 89077,
    SuppliesTransport   = 81778,
    WoodTransport       = 89074,
    SlagTank            = 81177,
    Gunner              = 77260,
    Lieutnant           = 77243,
    SiegeEngine1        = 81316,
    SiegeEngine2        = 78982,
    IronGunnerySergeant = 81318,
    GrimrailLoader      = 89055,
    OgronHauler         = 89062,
    GromkarGrenadier    = 89064,
    ManAtArmsIntro      = 77687,
    GromkarGunner       = 89068
};

enum eThogarGameObjects
{
    TrainAndCarCollisionBox1    = 232213,
    TrainAndCarCollisionBox2    = 227739,
    EngineCollisionBox          = 237588
};

enum eThogarSpells
{
    SlagTankAura            = 178189,
    SuppliesTransportAura   = 178232,
    MovingFrontAura         = 164263,
    BreakingFrontAura       = 164265,
    StoppedFrontAura        = 164266,
    MovingBackAura          = 167632,
    BreakingBackAura        = 167633,
    StoppedBackAura         = 167634,
    TroopTransportOpen      = 164319,
    TroopTransportClosed    = 164320,
    MovingTrain             = 156553,
    WoodTransportAura       = 178261
};

enum eThogarTalks
{
    TalkTrack1,
    TalkTrack2,
    TalkTrack3,
    TalkTrack4,
    TalkExpress,
    TalkIntro,
    TalkAggro,
    TalkReinforcements,
    TalkIronGunnerySergeant,
    TalkGromkarManAtArms,
    TalkTripleTrain,    ///< Mythic only
    TalkDeforester,     ///< Mythic only
    TalkSlay,
    TalkBerserk,
    TalkWipe,
    TalkDeath
};

enum eThogarActions
{
    ActionNone,
    /// Intro: Part1 - Iron Raiders
    IntroBegin,
    IntroEnd,
    TrainMoveEnd,
    /// Intro: Part2 - Iron Gunnery Sergeants
    IntroBeginPart2,
    TrainMoveEndPart2,
    /// Intro: Part3 - Wood transport, before intro text
    IntroBeginPart3,
    TrainMoveEndPart3
};

struct TrackDoors
{
    uint32 RightDoor;
    uint32 LeftDoor;
};

static std::vector<TrackDoors> g_TrackDoors =
{
    { eFoundryGameObjects::MassiveDoorTrack1Right, eFoundryGameObjects::MassiveDoorTrack1Left },
    { eFoundryGameObjects::MassiveDoorTrack2Right, eFoundryGameObjects::MassiveDoorTrack2Left },
    { eFoundryGameObjects::MassiveDoorTrack3Right, eFoundryGameObjects::MassiveDoorTrack3Left },
    { eFoundryGameObjects::MassiveDoorTrack4Right, eFoundryGameObjects::MassiveDoorTrack4Left }
};

////////////////////////////////////////////////////////////////////////////////
/// This table delineates the timing and track location of all of the significant events in the first six and a half minutes of the encounter.
/// This is not a complete list of  Moving Train locations and timers.
////////////////////////////////////////////////////////////////////////////////
///             Significant Events & Track Locations
///             Time    Track #     Notes
/// 0           0:40    1           4 CS, 12 R
/// 1           1:00    4           1 GS
/// 2           1:30    3           2 CS, 2 FM, 1 MaA
/// 3 / 4       2:10    2 / 3       4 CS, 12 R / 4 CS, 12 R
/// 5 / 6       2:50    1 / 4       Adds from Tracks 2&3 must be dead before these trains arrive
/// 7           3:00    1           1 GS
/// 8           3:25    4           4 CS, 12 R
/// 9 / 10      4:20    2 / 4       2 CS, 2 FM, 1 MaA / 1 GS
/// 11 / 12     5:15    1 / 4       1 GS / 1 GS
/// 13 / 14     6:20    2 / 3       4 CS, 12 R / 2 CS, 2 FM, 1 MaA
/// CS = Iron Crack-Shot, R = Iron Raider, FM = Grom'kar Firemender,
/// MaA = Grom'kar Man-at-Arms, GS = Iron Gunnery Sergeant
/// Times are approximate
////////////////////////////////////////////////////////////////////////////////

struct WaggonDatas
{
    uint32 Entry;
    uint32 VisualAura;

    std::vector<uint32> Passengers;
};

struct TrainDatas
{
    /// Entries
    uint32 EngineEntry;

    std::vector<WaggonDatas> WaggonsDatas;
};

enum eThogarTrains
{
    /// Intro trains
    IntroWoodTrain,
    IntroSiegeTrain,
    IntroTroopsTrain,
    /// Fight Trains
    FightTrainBeginning,
    FightTrainEnd = eThogarTrains::FightTrainBeginning + eThogarMiscDatas::MaxFightTrains,
    /// End
    MaxTrains = eThogarTrains::FightTrainEnd
};

static std::vector<TrainDatas> const g_TrainDatas =
{
    /// Wood train for intro
    {
        eThogarCreatures::TrainEngine,
        {
            { eThogarCreatures::WoodTransport, eThogarSpells::WoodTransportAura, { } },
            { eThogarCreatures::TroopTransport2, 0, { eThogarCreatures::GromkarGunner, eThogarCreatures::GromkarGunner, eThogarCreatures::GrimrailLoader, eThogarCreatures::GrimrailLoader } }
        }
    },
    /// Siege engine train for intro
    {
        eThogarCreatures::TrainEngine,
        {
            { eThogarCreatures::Train2, 0, { eThogarCreatures::SiegeEngine1 } },
            { eThogarCreatures::Train1, 0, { eThogarCreatures::SiegeEngine1, eThogarCreatures::ManAtArmsIntro } },
            { eThogarCreatures::SuppliesTransport, eThogarSpells::SuppliesTransportAura, { } }
        }
    },
    /// Iron raider train for intro
    {
        eThogarCreatures::TrainEngine,
        {
            {
                eThogarCreatures::TroopTransport1, 0,
                {
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider
                }
            },
            { eThogarCreatures::SlagTank, eThogarSpells::SlagTankAura, { } },
            {
                eThogarCreatures::TroopTransport1, 0,
                {
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider,
                    eFoundryCreatures::IronRaider
                }
            }
        }
    }
};

static bool const g_RightToLeftTrains[eThogarTrains::MaxTrains] =
{
    true,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
};

static uint32 const g_TrainTrackIDs[eThogarTrains::MaxTrains] =
{
    eThogarMiscDatas::FirstTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::FirstTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::ThirdTrack,
    eThogarMiscDatas::SecondTrack,
    eThogarMiscDatas::ThirdTrack,
    eThogarMiscDatas::FirstTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::FirstTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::SecondTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::FirstTrack,
    eThogarMiscDatas::FourthTrack,
    eThogarMiscDatas::SecondTrack,
    eThogarMiscDatas::ThirdTrack
};

static uint32 const g_TrainTimers[eThogarMiscDatas::MaxFightTrains] =
{
    40 * TimeConstants::IN_MILLISECONDS,
    20 * TimeConstants::IN_MILLISECONDS,
    30 * TimeConstants::IN_MILLISECONDS,
    40 * TimeConstants::IN_MILLISECONDS,
    0,  ///< At the same time than previous one
    40 * TimeConstants::IN_MILLISECONDS,
    0,  ///< At the same time than previous one
    10 * TimeConstants::IN_MILLISECONDS,
    25 * TimeConstants::IN_MILLISECONDS,
    55 * TimeConstants::IN_MILLISECONDS,
    0,  ///< At the same time than previous one
    55 * TimeConstants::IN_MILLISECONDS,
    0,  ///< At the same time than previous one
    55 * TimeConstants::IN_MILLISECONDS,
    0,  ///< At the same time than previous one
};

/// At Thogar's feet, left
Position const g_UnkPos1 = { 589.925f, 3296.323f, 299.4833f, 1.570796f };

Position const g_TrainTrackSpawnPos[eThogarMiscDatas::MaxTrainTracks] =
{
    { 589.925f, 2913.5f, 299.4833f, 1.570796f },
    { 565.775f, 2913.5f, 299.4833f, 1.570796f },
    { 541.625f, 2913.5f, 299.4833f, 1.570796f },
    { 517.475f, 2913.5f, 299.4833f, 1.570796f }
};

Position const g_TrainTrackEndPos[eThogarMiscDatas::MaxTrainTracks] =
{
    { 589.925f, 3713.5f, 299.4833f, 1.570796f },
    { 565.775f, 3713.5f, 299.4833f, 1.570796f },
    { 541.625f, 3713.5f, 299.4833f, 1.570796f },
    { 517.475f, 3713.5f, 299.4833f, 1.570796f }
};

Position const g_TrainTrackIntroEndPos = { 517.475f, 3330.412f, 299.4833f, 1.570796f };
Position const g_TrainTrackIntroSiegeEndPos = { 517.475f, 3313.283f, 299.4833f, 1.570796f };
Position const g_TrainTrackIntroWoodEndPos = { 589.925f, 3296.323f, 299.4833f, 1.570796f };

Position const g_IronRaiderLeftExitPos[eThogarMiscDatas::MaxIronRaiders] =
{
    { 493.975f, 3355.412f, 305.8985f, 1.570796f },
    { 500.975f, 3355.412f, 305.9013f, 1.570796f },
    { 493.975f, 3350.412f, 305.9027f, 1.570796f },
    { 500.975f, 3350.412f, 305.9000f, 1.570796f },
    { 493.975f, 3344.412f, 305.9012f, 1.570796f },
    { 500.975f, 3344.412f, 305.8985f, 1.570796f },
    { 493.975f, 3339.412f, 305.8999f, 1.570796f },
    { 500.975f, 3339.412f, 305.8972f, 1.570796f }
};

Position const g_IronRaiderRightExitPos[eThogarMiscDatas::MaxIronRaiders] =
{
    { 493.975f, 3287.412f, 305.8896f, 1.570796f },
    { 500.975f, 3287.412f, 305.8854f, 1.570796f },
    { 493.975f, 3282.412f, 305.8895f, 1.570796f },
    { 500.975f, 3282.412f, 305.8854f, 1.570796f },
    { 493.975f, 3276.412f, 305.8917f, 1.570796f },
    { 500.975f, 3276.412f, 305.8898f, 1.570796f },
    { 493.975f, 3271.412f, 305.8883f, 1.570796f },
    { 500.975f, 3271.412f, 305.8863f, 1.570796f }
};

Position const g_ManAtArmsExitPos = { 482.475f, 3312.533f, 302.3777f, 1.570796f };

Position const g_ThogarJumpPos = { 596.063f, 3312.91f, 299.145f, M_PI };

eThogarTalks const g_TrackTalks[eThogarMiscDatas::MaxTrainTracks] =
{
    eThogarTalks::TalkTrack1,
    eThogarTalks::TalkTrack2,
    eThogarTalks::TalkTrack3,
    eThogarTalks::TalkTrack4
};

static void ApplyPassengerFlags(Creature* p_Passenger, bool p_IsTrain = true)
{
    p_Passenger->SetReactState(ReactStates::REACT_PASSIVE);
    p_Passenger->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

    p_Passenger->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);

    if (p_IsTrain)
        p_Passenger->CastSpell(p_Passenger, eThogarSpells::StoppedFrontAura, true);
}

static void SummonTrain(Creature* p_Summoner, uint8 p_TrackID, uint8 p_TrainType)
{
    if (p_Summoner == nullptr || p_TrackID >= eThogarMiscDatas::MaxTrainTracks || p_TrainType >= eThogarMiscDatas::MaxTrainType)
        return;

    InstanceScript* l_InstanceScript = p_Summoner->GetInstanceScript();
    if (l_InstanceScript == nullptr)
        return;

    Creature* l_Thogar = Creature::GetCreature(*p_Summoner, l_InstanceScript->GetData64(eFoundryCreatures::BossOperatorThogar));
    if (l_Thogar == nullptr)
        return;

    Position const l_Pos = g_TrainTrackSpawnPos[p_TrackID];

    if (Creature* l_Wheels = p_Summoner->SummonCreature(eThogarCreatures::TrainWheels, l_Pos))
    {
        if (Creature* l_Engine = p_Summoner->SummonCreature(eThogarCreatures::TrainEngine, l_Pos))
            l_Engine->EnterVehicle(l_Wheels, 0);

        switch (p_TrainType)
        {
            case eThogarMiscDatas::NonAddTrain:
            {
                break;
            }
            case eThogarMiscDatas::SiegeTrain:
            {
                if (p_TrackID != eThogarMiscDatas::FirstTrack && p_TrackID != eThogarMiscDatas::FourthTrack)
                    break;

                if (Creature* l_Train = p_Summoner->SummonCreature(urand(0, 1) ? eThogarCreatures::Train1 : eThogarCreatures::Train2, l_Pos))
                {
                    l_Train->EnterVehicle(l_Wheels, 1);

                    if (Creature* l_SiegeEngine = p_Summoner->SummonCreature(eThogarCreatures::SiegeEngine1, l_Pos))
                    {
                        l_SiegeEngine->EnterVehicle(l_Train, 0);

                        if (Creature* l_GunnerySergeant = p_Summoner->SummonCreature(eThogarCreatures::IronGunnerySergeant, l_Pos))
                            l_GunnerySergeant->EnterVehicle(l_SiegeEngine, 0);
                    }
                }

                break;
            }
            case eThogarMiscDatas::HalfLengthAddTrains:
            {
                std::vector<uint32> l_Entries = { eThogarCreatures::TroopTransport1, eThogarCreatures::TroopTransport2, eThogarCreatures::TroopTransport3 };

                if (Creature* l_Train = p_Summoner->SummonCreature(l_Entries[urand(0, l_Entries.size() - 1)], l_Pos))
                    l_Train->EnterVehicle(l_Wheels, 1);

                break;
            }
            case eThogarMiscDatas::FullLengthAddTrains:
            {
                if (p_TrackID != eThogarMiscDatas::SecondTrack && p_TrackID != eThogarMiscDatas::ThirdTrack)
                    break;

                std::vector<uint32> l_Entries = { eThogarCreatures::TroopTransport1, eThogarCreatures::TroopTransport2, eThogarCreatures::TroopTransport3 };

                if (Creature* l_Train = p_Summoner->SummonCreature(l_Entries[urand(0, l_Entries.size() - 1)], l_Pos))
                    l_Train->EnterVehicle(l_Wheels, 1);

                break;
            }
            default:
                break;
        }
    }

    if (l_Thogar->IsAIEnabled)
        l_Thogar->AI()->Talk(g_TrackTalks[p_TrackID]);
}

static void SummonTrain(Creature* p_Summoner, uint8 p_TrainID, eThogarActions p_Action = eThogarActions::ActionNone, bool p_Talk = false)
{
    if (p_Summoner == nullptr || p_TrainID >= eThogarTrains::MaxTrains)
        return;

    uint8 l_TrackID = g_TrainTrackIDs[p_TrainID];
    if (l_TrackID >= eThogarMiscDatas::MaxTrainTracks)
        return;

    InstanceScript* l_InstanceScript = p_Summoner->GetInstanceScript();
    if (l_InstanceScript == nullptr)
        return;

    Creature* l_Thogar = Creature::GetCreature(*p_Summoner, l_InstanceScript->GetData64(eFoundryCreatures::BossOperatorThogar));
    if (l_Thogar == nullptr)
        return;

    bool l_IsRightToLeft = g_RightToLeftTrains[p_TrainID];

    Position const l_Pos = l_IsRightToLeft ? g_TrainTrackSpawnPos[l_TrackID] : g_TrainTrackEndPos[l_TrackID];

    if (Creature* l_Wheels = p_Summoner->SummonCreature(eThogarCreatures::TrainWheels, l_Pos))
    {
        ApplyPassengerFlags(l_Wheels, false);

        TrainDatas l_TrainDatas = g_TrainDatas[p_TrainID];

        int8 l_SeatID = 0;
        if (Creature* l_Engine = p_Summoner->SummonCreature(l_TrainDatas.EngineEntry, l_Pos))
        {
            ApplyPassengerFlags(l_Engine);
            l_Engine->EnterVehicle(l_Wheels, l_SeatID++);
        }

        for (WaggonDatas l_WaggonData : l_TrainDatas.WaggonsDatas)
        {
            if (Creature* l_Waggon = p_Summoner->SummonCreature(l_WaggonData.Entry, l_Pos))
            {
                ApplyPassengerFlags(l_Waggon);

                /// Apply visual aura if needed
                if (uint32 l_AuraID = l_WaggonData.VisualAura)
                    l_Waggon->CastSpell(l_Waggon, l_AuraID, true);

                l_Waggon->EnterVehicle(l_Wheels, l_SeatID++);

                int8 l_OtherSeatID = 0;
                for (uint32 l_Entry : l_WaggonData.Passengers)
                {
                    if (Creature* l_Passenger = p_Summoner->SummonCreature(l_Entry, l_Pos))
                    {
                        ApplyPassengerFlags(l_Passenger, false);

                        l_Passenger->EnterVehicle(l_Waggon, l_OtherSeatID++);
                    }
                }
            }
        }

        if (l_Wheels->IsAIEnabled)
        {
            l_Wheels->AI()->SetGUID(p_Summoner->GetGUID());
            l_Wheels->AI()->SetData(0, l_TrackID);
            l_Wheels->AI()->DoAction(p_Action);
        }
    }

    if (p_Talk && l_Thogar->IsAIEnabled)
        l_Thogar->AI()->Talk(g_TrackTalks[l_TrackID]);
}
