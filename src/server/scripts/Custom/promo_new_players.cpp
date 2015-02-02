#include <ScriptPCH.h>
#include <ScriptMgr.h>

class PromotionNewPlayers : public PlayerScript
{
public:
    PromotionNewPlayers() :PlayerScript("PromotionNewPlayers") {}

    enum Values
    {
        February2015 = 1422777600,  ///< Timestamp of 1st of February 2015.
        SwiftNetherDrake = 37015,   ///< SpellId.
        ApprenticeRidingSkill = 33388
    };

    void OnLogin(Player* p_Player)
    {
        if (time(NULL) < Values::February2015 && p_Player->GetTotalPlayedTime() == 0)
        {
            p_Player->learnSpell(Values::ApprenticeRidingSkill, false);
            p_Player->learnSpell(Values::SwiftNetherDrake, false);
        }
    }
};

void AddSC_promotion_new_players()
{
    new PromotionNewPlayers();
};