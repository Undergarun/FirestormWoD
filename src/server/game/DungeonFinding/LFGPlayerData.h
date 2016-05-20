////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LFGPLAYERDATA_H
#define _LFGPLAYERDATA_H

#include "Common.h"
#include "LFG.h"

/**
    Stores all lfg data needed about the player.
*/
class LfgPlayerData
{
    public:
        LfgPlayerData();
        ~LfgPlayerData();

        // General
        void SetState(LfgState state);
        void ClearState();
        void SetLockedDungeons(const LfgLockMap& lock);
        // Queue
        void SetRoles(uint8 roles);
        void SetComment(const std::string& comment);
        void SetSelectedDungeons(const LfgDungeonSet& dungeons);
        void ClearSelectedDungeons();

        // General
        LfgState GetState() const;
        const LfgLockMap& GetLockedDungeons() const;
        // Queue
        uint8 GetRoles() const;
        const std::string& GetComment() const;
        const LfgDungeonSet& GetSelectedDungeons() const;

    private:
        // General
        LfgState m_State;                                  ///< State if group in LFG
        LfgState m_OldState;                               ///< Old State
        // Player
        LfgLockMap m_LockedDungeons;                       ///< Dungeons player can't do and reason
        // Queue
        uint8 m_Roles;                                     ///< Roles the player selected when joined LFG
        std::string m_Comment;                             ///< Player comment used when joined LFG
        LfgDungeonSet m_SelectedDungeons;                  ///< Selected Dungeons when joined LFG
};

#endif
