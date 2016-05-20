////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "PetBattle.h"
#include "Common.h"

class PlayerScriptPetBattle : public PlayerScript
{
    public:
        /// Constructor
        PlayerScriptPetBattle()
            : PlayerScript("PlayerScriptPetBattle")
        {

        }

        /// Called when a spline step is done
        /// @p_Player   : Player instance
        /// @p_MoveType : Movement type
        /// @p_ID       : Movement ID
        void OnMovementInform(Player * p_Player, uint32 p_MoveType, uint32 p_ID) override
        {
            if (p_Player && p_MoveType == POINT_MOTION_TYPE && p_ID == PETBATTLE_ENTER_MOVE_SPLINE_ID)
            {
                m_Mutex.lock();
                m_DelayedPetBattleStart[p_Player->GetGUID()] = getMSTime() + 1000;
                m_Mutex.unlock();
            }
        }

        /// Called when a update() of a player is done
        /// @p_Player : Player instance
        /// @p_Diff : diff time
        void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
        {
            m_Mutex.lock();

            if (m_DelayedPetBattleStart.find(p_Player->GetGUID()) != m_DelayedPetBattleStart.end())
            {
                if (m_DelayedPetBattleStart[p_Player->GetGUID()] > getMSTime())
                {
                    m_DelayedPetBattleStart.erase(m_DelayedPetBattleStart.find(p_Player->GetGUID()));

                    uint64 l_BattleID = p_Player->_petBattleId;
                    PetBattle * l_Battle = sPetBattleSystem->GetBattle(l_BattleID);

                    if (l_Battle)
                    {
                        p_Player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC); // Immuned only to NPC
                        p_Player->SetRooted(true);

                        l_Battle->Begin();
                    }
                }
            }

            m_Mutex.unlock();
        }

        std::map<uint64, uint32> m_DelayedPetBattleStart;
        std::mutex m_Mutex;
};

#ifndef __clang_analyzer__
void AddSC_PetBattlePlayerScript()
{
    new PlayerScriptPetBattle;
}
#endif
