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
    TrainEngine     = 77181,
    TrainWheels     = 76907,
    Train1          = 82107,
    Train2          = 81226,
    TroopTransport1 = 78842,
    TroopTransport2 = 81752,
    TroopTransport3 = 77158
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

void SummonTrain(Creature* p_Summoner, uint8 p_TrackID, uint8 p_TrainType)
{
    if (p_Summoner == nullptr || p_TrackID >= eThogarMiscDatas::MaxTrainTracks || p_TrainType >= eThogarMiscDatas::MaxTrainType)
        return;

    Position const l_Pos = g_TrainTrackSpawnPos[p_TrackID];

    if (Creature* l_Wheels = p_Summoner->SummonCreature(eThogarCreatures::TrainWheels, l_Pos))
    {
        if (Creature* l_Engine = p_Summoner->SummonCreature(eThogarCreatures::TrainEngine, l_Pos))
            l_Engine->EnterVehicle(l_Wheels);

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

                if (Creature* l_Train = p_Summoner->SummonCreature(urand(eThogarCreatures::Train1, eThogarCreatures::Train2), l_Pos))
                    l_Train->EnterVehicle(l_Wheels);

                break;
            }
            case eThogarMiscDatas::HalfLengthAddTrains:
            {
                std::vector<uint32> l_Entries = { eThogarCreatures::TroopTransport1, eThogarCreatures::TroopTransport2, eThogarCreatures::TroopTransport3 };

                if (Creature* l_Train = p_Summoner->SummonCreature(l_Entries[urand(0, l_Entries.size() - 1)], l_Pos))
                    l_Train->EnterVehicle(l_Wheels);

                break;
            }
            case eThogarMiscDatas::FullLengthAddTrains:
            {
                if (p_TrackID != eThogarMiscDatas::SecondTrack && p_TrackID != eThogarMiscDatas::ThirdTrack)
                    break;

                std::vector<uint32> l_Entries = { eThogarCreatures::TroopTransport1, eThogarCreatures::TroopTransport2, eThogarCreatures::TroopTransport3 };

                if (Creature* l_Train = p_Summoner->SummonCreature(l_Entries[urand(0, l_Entries.size() - 1)], l_Pos))
                    l_Train->EnterVehicle(l_Wheels);

                break;
            }
            default:
                break;
        }
    }
}
