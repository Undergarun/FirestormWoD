////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <ScriptPCH.h>
#include <ScriptMgr.h>

class CharacterRenderer : public PlayerScript
{
    private:
        ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

    public:
        CharacterRenderer() : PlayerScript("CharacterRenderer")
        {
        }

        void OnLogout(Player* p_Player) override
        {
            PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_STATS);
            uint32 l_Idx = 0;

            std::ostringstream l_Damage;
            l_Damage << p_Player->GetFloatValue(UNIT_FIELD_MIN_DAMAGE) << " - " << p_Player->GetFloatValue(UNIT_FIELD_MAX_DAMAGE);

            l_Stmt->setUInt32(l_Idx++, p_Player->GetGUIDLow());
            l_Stmt->setUInt32(l_Idx++, p_Player->GetStat(STAT_STRENGTH));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetStat(STAT_AGILITY));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetStat(STAT_STAMINA));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetStat(STAT_INTELLECT));
            l_Stmt->setFloat(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_CRIT_PERCENTAGE));
            l_Stmt->setFloat(l_Idx++, ((1.0f / p_Player->GetFloatValue(UNIT_FIELD_MOD_HASTE)) - 1.0f)*100.0f);
            l_Stmt->setUInt32(l_Idx++, p_Player->GetMasteryCache());
            l_Stmt->setUInt32(l_Idx++, p_Player->GetStat(STAT_SPIRIT));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetUInt32Value(UNIT_FIELD_MOD_BONUS_ARMOR));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_MULTISTRIKE));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_LIFESTEAL));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_VERSATILITY) + p_Player->GetFloatValue(PLAYER_FIELD_VERSATILITY_BONUS));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_AVOIDANCE));
            l_Stmt->setString(l_Idx++, l_Damage.str().c_str());
            l_Stmt->setUInt32(l_Idx++, p_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack));
            l_Stmt->setFloat(l_Idx++, p_Player->GetAttackTime(WeaponAttackType::BaseAttack)/1000.f);
            l_Stmt->setUInt32(l_Idx++, p_Player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_NONE));
            l_Stmt->setUInt32(l_Idx++, p_Player->GetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER) * 5.0f);
            l_Stmt->setUInt32(l_Idx++, p_Player->GetArmor());
            l_Stmt->setFloat(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_DODGE_PERCENTAGE));
            l_Stmt->setFloat(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_PARRY_PERCENTAGE));
            l_Stmt->setFloat(l_Idx++, p_Player->GetFloatValue(PLAYER_FIELD_BLOCK_PERCENTAGE));

            CharacterDatabase.Execute(l_Stmt);
        }
};

#ifndef __clang_analyzer__
void AddSC_Character_Renderer()
{
    new CharacterRenderer();
};
#endif
