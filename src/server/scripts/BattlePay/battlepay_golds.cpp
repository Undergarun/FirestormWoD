////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BattlepayMgr.h"

namespace BattlePay
{
    enum Type : int64
    {
        Gold10k     = 100000000U,
        Gold50k     = 500000000U,
        Gold100k    = 1000000000U,
        Gold300k    = 3000000000U,
    };
}

template<int64 t_Gold> class BattlePay_Gold : BattlePayProductScript
{
    public:
        BattlePay_Gold(std::string p_ScriptName) : BattlePayProductScript(p_ScriptName) {}

        void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/)
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
                return;

            l_Player->ModifyMoney(t_Gold);
            l_Player->SaveToDB();
        }

        bool CanBuy(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/, std::string& p_Reason)
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::NeedToBeInGame, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            if (uint64(l_Player->GetMoney() + t_Gold) > MAX_MONEY_AMOUNT)
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::AtGoldLimit, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_BattlePay_Golds()
{
    new BattlePay_Gold<BattlePay::Gold10k>("battlepay_gold_10k");
    new BattlePay_Gold<BattlePay::Gold50k>("battlepay_gold_50k");
    new BattlePay_Gold<BattlePay::Gold100k>("battlepay_gold_100k");
    new BattlePay_Gold<BattlePay::Gold300k>("battlepay_gold_300k");
}
#endif