////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Config.h"

class PlayerScript_meta_achievement_midsummer : public PlayerScript
{
public:
    PlayerScript_meta_achievement_midsummer() : PlayerScript("PlayerScript_meta_achievement_midsummer") { }

    enum eDatas
    {
        LaDanseDuFeu                = 271,
        Jongleur                    = 272,
        LeRoiDeLaFeteDuFeu          = 1145,
        TelEstGlaceQuiCroyaitGeler  = 263,
        MaxAchievements             = 4,
        ProfanationDeLAlliance      = 1037,
        ProfanationDeLaHorde        = 1035,
        LesFeuxDAzerothX            = 1036,
        LesFeuxDAzerothY            = 1034,
        LeGardienDesFlammes         = 1038,
        LeGardeFlammes              = 1039
    };

    std::array<uint32, eDatas::MaxAchievements> m_Achievements =
    {
        eDatas::LaDanseDuFeu,
        eDatas::Jongleur,
        eDatas::LeRoiDeLaFeteDuFeu,
        eDatas::TelEstGlaceQuiCroyaitGeler
    };

    void OnAchivementEarned(Player* p_Player, AchievementEntry const* /*p_Achievement*/)
    {
        if (!p_Player)
            return;

        bool l_AllCompleted = true;
        
        for (uint32 l_AchievementID : m_Achievements)
        {
            if (!p_Player->GetAchievementMgr().HasAchieved(l_AchievementID))
                l_AllCompleted = false;
        }

        if (!p_Player->GetAchievementMgr().HasAchieved(eDatas::ProfanationDeLaHorde) && !p_Player->GetAchievementMgr().HasAchieved(eDatas::ProfanationDeLAlliance))
            l_AllCompleted = false;

        if (!p_Player->GetAchievementMgr().HasAchieved(eDatas::LesFeuxDAzerothY) && !p_Player->GetAchievementMgr().HasAchieved(eDatas::LesFeuxDAzerothX))
            l_AllCompleted = false;

        if (l_AllCompleted)
        {
            if (!p_Player->GetAchievementMgr().HasAchieved(eDatas::LeGardeFlammes))
                p_Player->CompletedAchievement(sAchievementStore.LookupEntry(eDatas::LeGardeFlammes));
            if (!p_Player->GetAchievementMgr().HasAchieved(eDatas::LeGardienDesFlammes))
                p_Player->CompletedAchievement(sAchievementStore.LookupEntry(eDatas::LeGardienDesFlammes));
        }
    }
};

#ifndef __clang_analyzer__
void AddSC_CustomPlayerscripts()
{
    new PlayerScript_meta_achievement_midsummer();
}
#endif
