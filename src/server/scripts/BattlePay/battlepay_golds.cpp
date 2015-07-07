////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////


namespace BattlePay
{
    enum Type : int64
    {
        Gold10k     = 100000000U,
        Gold50k     = 500000000U,
        Gold200k    = 2000000000U,
        Gold500k    = 5000000000U,
    };

    enum String
    {
        AtGoldLimit    = 14089,
        NeedToBeInGame = 14090
    };
}

template<int64 t_Gold> class BattlePay_Gold : BattlePayProductScript
{
    public:
        BattlePay_Gold(std::string p_ScriptName) : BattlePayProductScript(p_ScriptName) {}

        void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& p_Product) override
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
                return;

            l_Player->ModifyMoney(t_Gold);
            l_Player->SaveToDB();
        }

        bool CanBuy(WorldSession* p_Session, Battlepay::Product const& p_Product, std::string& p_Reason) override
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
            {
                p_Reason = sObjectMgr->GetTrinityString(BattlePay::String::NeedToBeInGame, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            if (uint64(l_Player->GetMoney() + t_Gold) > MAX_MONEY_AMOUNT)
            {
                p_Reason = sObjectMgr->GetTrinityString(BattlePay::AtGoldLimit, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            return true;
        }
};

void AddSC_BattlePay_Golds()
{
    new BattlePay_Gold<BattlePay::Gold10k>("battlepay_gold_10k");
    new BattlePay_Gold<BattlePay::Gold50k>("battlepay_gold_50k");
    new BattlePay_Gold<BattlePay::Gold200k>("battlepay_gold_200k");
    new BattlePay_Gold<BattlePay::Gold500k>("battlepay_gold_500k");
}