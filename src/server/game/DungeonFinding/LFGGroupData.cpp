////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "LFG.h"
#include "LFGGroupData.h"

LfgGroupData::LfgGroupData():
m_State(LFG_STATE_NONE), m_OldState(LFG_STATE_NONE), m_Dungeon(0),
m_VotesNeeded(LFG_GROUP_KICK_VOTES_NEEDED), m_KicksLeft(LFG_GROUP_MAX_KICKS)
{
}

LfgGroupData::~LfgGroupData()
{
}

void LfgGroupData::SetState(LfgState state)
{
    switch (state)
    {
        case LFG_STATE_NONE:
        case LFG_STATE_DUNGEON:
        case LFG_STATE_FINISHED_DUNGEON:
            m_OldState = state;
                    // No break on purpose
        default:
            m_State = state;
    }
}

void LfgGroupData::RestoreState()
{
    m_State = m_OldState;
}

void LfgGroupData::SetDungeon(uint32 dungeon)
{
    m_Dungeon = dungeon;
}

void LfgGroupData::DecreaseKicksLeft()
{
    if (m_KicksLeft)
      --m_KicksLeft;
}

LfgState LfgGroupData::GetState() const
{
    return m_State;
}

uint32 LfgGroupData::GetDungeon(bool asId /* = true */) const
{
    if (asId)
        return (m_Dungeon & 0x00FFFFFF);
    else
        return m_Dungeon;
}

uint8 LfgGroupData::GetVotesNeeded() const
{
    return m_VotesNeeded;
}

uint8 LfgGroupData::GetKicksLeft() const
{
    return m_KicksLeft;
}
