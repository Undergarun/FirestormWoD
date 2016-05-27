////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

////////////////////////////////////////////////////////////////////////////////
/// This table delineates the timing and track location of all of the significant events in the first six and a half minutes of the encounter.
/// This is not a complete list of  Moving Train locations and timers.
////////////////////////////////////////////////////////////////////////////////
/// Significant Events & Track Locations
/// Time    Track #     Notes
/// 0:40    1           4 CS, 12 R
/// 1:00    4           1 GS
/// 1:30    3           2 CS, 2 FM, 1 MaA
/// 2:10    2 / 3       4 CS, 12 R / 4 CS, 12 R
/// 2:50    1 / 4       Adds from Tracks 2&3 must be dead before these trains arrive
/// 3:00    1           1 GS
/// 3:25    4           4 CS, 12 R
/// 4:20    2 / 4       2 CS, 2 FM, 1 MaA / 1 GS
/// 5:15    1 / 4       1 GS / 1 GS
/// 6:20    2 / 3       4 CS, 12 R / 2 CS, 2 FM, 1 MaA
/// CS = Iron Crack-Shot, R = Iron Raider, FM = Grom'kar Firemender,
/// MaA = Grom'kar Man-at-Arms, GS = Iron Gunnery Sergeant
/// Times are approximate
////////////////////////////////////////////////////////////////////////////////

enum eThogarMiscDatas
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
    MaxTrainType
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
    GromkarGrenadier    = 89064
};

enum eThogarSpells
{
    SlagTankAura        = 178189,
    MovingFrontAura     = 164263,
    BreakingFrontAura   = 164265,
    StoppedFrontAura    = 164266,
    MovingBackAura      = 167632,
    BreakingBackAura    = 167633,
    StoppedBackAura     = 167634,
    TroopTransportOpen  = 164319,
    MovingTrain         = 156553
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

eThogarTalks const g_TrackTalks[eThogarMiscDatas::MaxTrainTracks] =
{
    eThogarTalks::TalkTrack1,
    eThogarTalks::TalkTrack2,
    eThogarTalks::TalkTrack3,
    eThogarTalks::TalkTrack4
};

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

static void SummonIntroTrain(Creature* p_Summoner, uint8 p_TrackID)
{
    if (p_Summoner == nullptr || p_TrackID >= eThogarMiscDatas::MaxTrainTracks)
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
        {
            l_Engine->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
            l_Engine->CastSpell(l_Engine, eThogarSpells::StoppedFrontAura, true);
            l_Engine->EnterVehicle(l_Wheels, 0);
        }

        if (Creature* l_Train = p_Summoner->SummonCreature(eThogarCreatures::TroopTransport1, l_Pos))
        {
            l_Train->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
            l_Train->CastSpell(l_Train, eThogarSpells::StoppedFrontAura, true);
            l_Train->EnterVehicle(l_Wheels, 1);

            for (uint8 l_I = 0; l_I < 8; ++l_I)
            {
                if (Creature* l_Raider = p_Summoner->SummonCreature(eFoundryCreatures::IronRaider, l_Pos))
                {
                    l_Raider->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
                    l_Raider->EnterVehicle(l_Train, l_I);
                }
            }
        }

        if (Creature* l_Train = p_Summoner->SummonCreature(eThogarCreatures::SlagTank, l_Pos))
        {
            l_Train->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);

            l_Train->CastSpell(l_Train, eThogarSpells::StoppedFrontAura, true);
            l_Train->CastSpell(l_Train, eThogarSpells::SlagTankAura, true);

            l_Train->EnterVehicle(l_Wheels, 2);

            for (uint8 l_I = 0; l_I < 2; ++l_I)
            {
                if (Creature* l_Loader = p_Summoner->SummonCreature(eThogarCreatures::GrimrailLoader, l_Pos))
                {
                    l_Loader->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
                    l_Loader->EnterVehicle(l_Train, l_I * 2);
                }
            }

            if (Creature* l_Hauler = p_Summoner->SummonCreature(eThogarCreatures::OgronHauler, l_Pos))
            {
                l_Hauler->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
                l_Hauler->EnterVehicle(l_Train, 1);
            }

            if (Creature* l_Grenadier = p_Summoner->SummonCreature(eThogarCreatures::GromkarGrenadier, l_Pos))
            {
                l_Grenadier->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
                l_Grenadier->EnterVehicle(l_Train, 3);
            }
        }

        if (Creature* l_Train = p_Summoner->SummonCreature(eThogarCreatures::TroopTransport1, l_Pos))
        {
            l_Train->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
            l_Train->CastSpell(l_Train, eThogarSpells::StoppedFrontAura, true);
            l_Train->EnterVehicle(l_Wheels, 3);

            for (uint8 l_I = 0; l_I < 8; ++l_I)
            {
                if (Creature* l_Raider = p_Summoner->SummonCreature(eFoundryCreatures::IronRaider, l_Pos))
                {
                    l_Raider->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
                    l_Raider->EnterVehicle(l_Train, l_I);
                }
            }
        }

        if (l_Wheels->IsAIEnabled)
        {
            l_Wheels->AI()->DoAction(0);
            l_Wheels->AI()->SetGUID(p_Summoner->GetGUID());
        }
    }

    if (l_Thogar->IsAIEnabled)
        l_Thogar->AI()->Talk(g_TrackTalks[p_TrackID]);
}
