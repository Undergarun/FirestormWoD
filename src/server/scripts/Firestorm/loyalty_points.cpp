////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <ScriptPCH.h>
#include <ScriptMgr.h>

#ifndef CROSS
class LoyaltyEvents : public PlayerScript
{
    public:
        LoyaltyEvents() : PlayerScript("LoyaltyEvents")
        {
        }

        void OnAchievementEarned(Player* p_Player, AchievementEntry const* p_Achievement, bool& p_SendAchievement, bool p_After) override
        {
            if (p_After && p_Player)
                p_Player->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::Achievement);
        }

        void OnCreatureKill(Player* p_Killer, Creature* p_Killed) override
        {
            if (p_Killed->IsDungeonBoss() || p_Killed->isWorldBoss() || p_Killed->GetCreatureTemplate()->rank == CREATURE_ELITE_WORLDBOSS
                || p_Killed->GetCreatureTemplate()->rank == CREATURE_ELITE_RARE || p_Killed->GetCreatureTemplate()->rank == CREATURE_ELITE_RAREELITE)
                p_Killer->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::KillRareOrBossCreature);
        }

        void OnPetBattleFinish(Player* p_Player) override
        {
            p_Player->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::BattlePet);
        }

        void OnDungeonFinderFinish(Player* p_Player) override
        {
            p_Player->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::DungeonFinder);
        }

        void OnCraftItem(Player* p_Player, Item* p_Item) override
        {
            if (p_Item->GetTemplate()->Quality == ItemQualities::ITEM_QUALITY_EPIC)
                p_Player->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::EpicCraft);
            else
                p_Player->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::Craft);
        }

        void OnGrabRessource(Player* p_Player, GameObject* p_GameObject) override
        {
            p_Player->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::GrabRessource);
        }

        void OnDuelEnd(Player* p_Player1, Player* p_Player2, DuelCompleteType p_CompletionType) override
        {
            if (p_CompletionType != DuelCompleteType::DUEL_WON)
                return;

            if (p_Player1)
                p_Player1->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::Duel);

            if (p_Player2)
                p_Player2->GetSession()->CompleteLoyaltyEvent(LoyaltyEvent::Duel);
        }
};

#ifndef __clang_analyzer__
void AddSC_Loyalty_Points()
{
    new LoyaltyEvents();
};
#endif

#endif