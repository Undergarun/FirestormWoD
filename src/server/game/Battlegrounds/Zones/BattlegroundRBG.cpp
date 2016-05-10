#include "Player.h"
#include "Battleground.h"
#include "BattlegroundRBG.h"
#include "Language.h"

BattlegroundRBG::BattlegroundRBG()
{
    //TODO FIX ME!
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = 0;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_WS_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;
}

BattlegroundRBG::~BattlegroundRBG()
{

}

void BattlegroundRBG::StartingEventCloseDoors()
{
}

void BattlegroundRBG::StartingEventOpenDoors()
{
}

void BattlegroundRBG::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in constructor
    BattlegroundRBGScore* sc = new BattlegroundRBGScore;

    PlayerScores[player->GetGUID()] = sc;
}

void BattlegroundRBG::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/)
{
}

void BattlegroundRBG::HandleAreaTrigger(Player* /*Source*/, uint32 /*Trigger*/)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattlegroundRBG::UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType)
{
    std::map<uint64, BattlegroundScore*>::iterator itr = PlayerScores.find(p_Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    Battleground::UpdatePlayerScore(p_Source, p_Victim, p_Type, p_Value, p_DoAddHonor, p_RewardType);
}