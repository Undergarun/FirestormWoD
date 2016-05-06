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
    namespace ProfessionBookSpells
    {
        enum
        {
            Alchemy         = 156614,
            Blacksmithing   = 169923,
            Enchanting      = 161788,
            Engineering     = 161787,
            Inscription     = 161789,
            JewelCrafting   = 169926,
            LeatherWorking  = 169925,
            Tailoring       = 169924,
            FirstAid        = 160329,
            Cooking         = 160360,
            Herbalism       = 158745,
            Mining          = 158754,
            Skinning        = 158756,
            Archaeology     = 158762,
            Fishing         = 160326
        };
    }

    namespace ProfessionAdditionalSpells
    {
        enum
        {
            Prospecting = 31252
        };
    }

    static std::map<uint32, uint32> g_SkillLearningSpells =
    {
        { SkillType::SKILL_ALCHEMY,        ProfessionBookSpells::Alchemy        },
        { SkillType::SKILL_BLACKSMITHING,  ProfessionBookSpells::Blacksmithing  },
        { SkillType::SKILL_ENCHANTING,     ProfessionBookSpells::Enchanting     },
        { SkillType::SKILL_ENGINEERING,    ProfessionBookSpells::Engineering    },
        { SkillType::SKILL_INSCRIPTION,    ProfessionBookSpells::Inscription    },
        { SkillType::SKILL_JEWELCRAFTING,  ProfessionBookSpells::JewelCrafting  },
        { SkillType::SKILL_LEATHERWORKING, ProfessionBookSpells::LeatherWorking },
        { SkillType::SKILL_TAILORING,      ProfessionBookSpells::Tailoring      },
        { SkillType::SKILL_FIRST_AID,      ProfessionBookSpells::FirstAid       },
        { SkillType::SKILL_COOKING,        ProfessionBookSpells::Cooking        },
        { SkillType::SKILL_HERBALISM,      ProfessionBookSpells::Herbalism      },
        { SkillType::SKILL_MINING,         ProfessionBookSpells::Mining         },
        { SkillType::SKILL_SKINNING,       ProfessionBookSpells::Skinning       },
        { SkillType::SKILL_ARCHAEOLOGY,    ProfessionBookSpells::Archaeology    },
        { SkillType::SKILL_FISHING,        ProfessionBookSpells::Fishing        }
    };
}

template<uint32 t_SkillID, uint32 t_Value> class BattlePay_Profession : BattlePayProductScript
{
    public:
        BattlePay_Profession(std::string p_ScriptName) : BattlePayProductScript(p_ScriptName) {}

        void OnProductDelivery(WorldSession* p_Session, Battlepay::Product const& /*p_Product*/)
        {
            Player* l_Player = p_Session->GetPlayer();
            if (l_Player == nullptr)
                return;

            uint32 l_SpellID = BattlePay::g_SkillLearningSpells[t_SkillID];

            l_Player->CastSpell(l_Player, l_SpellID, true);

            l_Player->SetSkill(t_SkillID, l_Player->GetSkillStep(t_SkillID), t_Value, t_Value);

            std::list<SkillLineAbilityEntry const*> l_Abilities = sSpellMgr->GetTradeSpellFromSkill(t_SkillID);
            for (auto l_Abilitie : l_Abilities)
            {
                if (l_Abilitie->spellId > 155748)   ///< last 5.4.7 spellid
                    continue;

                l_Player->learnSpell(l_Abilitie->spellId, false);
            }

            if (t_SkillID == SkillType::SKILL_JEWELCRAFTING)
                l_Player->learnSpell(BattlePay::ProfessionAdditionalSpells::Prospecting, false);             ///< Prospecting

            /// We also need to learn it for herbalism
            if (t_SkillID == SkillType::SKILL_HERBALISM || t_SkillID == SkillType::SKILL_MINING)
                l_Player->learnSpell(l_SpellID, false);

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

            /// Internal error, no message, can't happen on live
            if (BattlePay::g_SkillLearningSpells.find(t_SkillID) == BattlePay::g_SkillLearningSpells.end())
                return false;

            if (l_Player->getLevel() < 90)
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::Level90Required, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            if (IsPrimaryProfessionSkill(t_SkillID) && !l_Player->HasSkill(t_SkillID) && l_Player->GetFreePrimaryProfessionPoints() == 0)
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::ReachPrimaryProfessionLimit, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            if (l_Player->HasSkill(t_SkillID) && l_Player->GetSkillValue(t_SkillID) == t_Value)
            {
                p_Reason = sObjectMgr->GetTrinityString(Battlepay::String::YouAlreadyOwnThat, p_Session->GetSessionDbLocaleIndex());
                return false;
            }

            return true;
        }
};
#ifndef __clang_analyzer__
void AddSC_BattlePay_Professions()
{
    new BattlePay_Profession<SkillType::SKILL_ALCHEMY,        700>("battlepay_profession_alchemy");
    new BattlePay_Profession<SkillType::SKILL_BLACKSMITHING,  700>("battlepay_profession_blacksmithing");
    new BattlePay_Profession<SkillType::SKILL_ENCHANTING,     700>("battlepay_profession_enchanting");
    new BattlePay_Profession<SkillType::SKILL_ENGINEERING,    700>("battlepay_profession_engineering");
    new BattlePay_Profession<SkillType::SKILL_INSCRIPTION,    700>("battlepay_profession_inscription");
    new BattlePay_Profession<SkillType::SKILL_JEWELCRAFTING,  700>("battlepay_profession_jewelcrafting");
    new BattlePay_Profession<SkillType::SKILL_LEATHERWORKING, 700>("battlepay_profession_leatherworking");
    new BattlePay_Profession<SkillType::SKILL_TAILORING,      700>("battlepay_profession_tailoring");
    new BattlePay_Profession<SkillType::SKILL_FIRST_AID,      700>("battlepay_profession_first_aid");
    new BattlePay_Profession<SkillType::SKILL_COOKING,        700>("battlepay_profession_cooking");
    new BattlePay_Profession<SkillType::SKILL_HERBALISM,      700>("battlepay_profession_herbalism");
    new BattlePay_Profession<SkillType::SKILL_MINING,         700>("battlepay_profession_mining");
    new BattlePay_Profession<SkillType::SKILL_SKINNING,       700>("battlepay_profession_skinning");
    new BattlePay_Profession<SkillType::SKILL_ARCHAEOLOGY,    700>("battlepay_profession_archaeology");
    new BattlePay_Profession<SkillType::SKILL_FISHING,        700>("battlepay_profession_fishing");
}
#endif