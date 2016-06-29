////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "BattlepayMgr.h"

template<uint32 t_Level> class BattlePay_Level : BattlePayProductScript
{
    public:
        BattlePay_Level(std::string p_ScriptName) : BattlePayProductScript(p_ScriptName) {}

        void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/)
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
                return;

            l_Player->GiveLevel(t_Level);
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

            if (t_Level <= l_Player->getLevel())
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::TooHighLevel, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            return true;
        }
};

template<AtLoginFlags t_AtLogin> class BattlePay_CharacterService : BattlePayProductScript
{
    public:
        BattlePay_CharacterService(std::string p_ScriptName) : BattlePayProductScript(p_ScriptName) {}

        void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/)
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
                return;

            l_Player->SetAtLoginFlag(t_AtLogin);
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

            /// Can't have two flags
            if (l_Player->HasAtLoginFlag((AtLoginFlags)0xFFFFFFFF))
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::YouAlreadyOwnThat, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            return true;
        }
};

template <uint32 t_AccountServiceFlag> class BattlePay_AccountService : BattlePayProductScript
{
    public:
        BattlePay_AccountService(std::string p_ScriptName) : BattlePayProductScript(p_ScriptName) {}

        void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/) override
        {
            p_Session->SetServiceFlags(t_AccountServiceFlag);
        }

        bool CanBuy(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/, std::string& p_Reason) override
        {
            if (p_Session->HasServiceFlags(t_AccountServiceFlag))
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::YouAlreadyOwnThat, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_BattlePay_Services()
{
    new BattlePay_Level<90>("battlepay_service_level90");
    new BattlePay_Level<100>("battlepay_service_level100");
    new BattlePay_CharacterService<AtLoginFlags::AT_LOGIN_RENAME>("battlepay_service_rename");
    new BattlePay_CharacterService<AtLoginFlags::AT_LOGIN_CHANGE_FACTION>("battlepay_service_change_faction");
    new BattlePay_CharacterService<AtLoginFlags::AT_LOGIN_CHANGE_RACE>("battlepay_service_change_race");
    new BattlePay_CharacterService<AtLoginFlags::AT_LOGIN_CUSTOMIZE>("battlepay_service_customize");
    //new BattlePay_AccountService<ServiceFlags::PremadePve>("battlepay_service_premade");
}
#endif
#endif
