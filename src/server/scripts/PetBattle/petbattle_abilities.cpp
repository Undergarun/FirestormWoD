#include "ScriptMgr.h"
#include "PetBattle.h"
#include "PetBattleAbilityEffect.h"
#include "DB2Stores.h"

class PlayerScript_Battle_Pet_Training_Spell: public PlayerScript
{
public:
    PlayerScript_Battle_Pet_Training_Spell() :PlayerScript("PlayerScript_Battle_Pet_Training_Spell") {}

    void OnSpellLearned(Player* p_Player, uint32 p_SpellId)
    {
        switch (p_SpellId)
        {
            case 119467:
            case 122026:
            case 125439:
            case 125610:
            {
                if (!p_Player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HAS_BATTLE_PET_TRAINING))
                {
                    p_Player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HAS_BATTLE_PET_TRAINING);
                    p_Player->GetSession()->SendBattlePetJournal();
                    p_Player->GetSession()->SendBattlePetTrapLevel();
                    p_Player->GetSession()->SendBattlePetLicenseChanged();
                }
                break;
            }
        }
    }
};

#ifndef __clang_analyzer__
void AddSC_petbattle_abilities()
{
    new PlayerScript_Battle_Pet_Training_Spell();
}
#endif