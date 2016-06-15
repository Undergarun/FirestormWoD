////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "GameObjectAI.h"
#include "ScriptMgr.h"

/// Train Set - 193963
class go_toy_train_set : public GameObjectScript
{
    public:
        go_toy_train_set() : GameObjectScript("go_toy_train_set") { }

        enum eSpell
        {
            ToyTrainSetPulse = 61551
        };

        struct go_toy_train_setAI : public GameObjectAI
        {
            go_toy_train_setAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject), m_PulseTimer(3 * TimeConstants::IN_MILLISECONDS) { }

            uint32 m_PulseTimer;

            void UpdateAI(uint32 p_Diff) override
            {
                if (p_Diff < m_PulseTimer)
                    m_PulseTimer -= p_Diff;
                else
                {
                    go->CastSpell(nullptr, eSpell::ToyTrainSetPulse);
                    m_PulseTimer = 6 * TimeConstants::IN_MILLISECONDS;
                }
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_toy_train_setAI(p_GameObject);
        }
};

#ifndef __clang_analyzer__
void AddSC_go_toy()
{
    new go_toy_train_set();
}
#endif
