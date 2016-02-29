// Serves as a placeholder for other branches so we dont have to recmake every time

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"

#define DUEL_AREA_RANGE 200.f

enum Spells
{
    SPELL_FUN_QUICK_RECOVERY    = 176253
};

enum Gameobjects
{
    GAMEOBJECT_DUEL_AREA_OBJECT = 900000
};

/// Quick Recovery - 176253
class spell_fun_quick_recovery : public SpellScriptLoader
{
    public:
        spell_fun_quick_recovery() : SpellScriptLoader("spell_fun_quick_recovery") { }

        class spell_fun_quick_recovery_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fun_quick_recovery_AuraScript);

            bool Load()
            {
                m_Timer = 1000;
                return true;
            }

            void OnUpdate(uint32 p_Diff, AuraEffect* /* p_AurEff */)
            {
                Unit* l_Owner = GetUnitOwner();

                if (!l_Owner)
                    return;

                if (m_Timer <= p_Diff)
                {
                    m_Timer = 1000;

                    if (!l_Owner->FindNearestGameObject(GAMEOBJECT_DUEL_AREA_OBJECT, DUEL_AREA_RANGE))
                    {
                        Remove();

                        if (l_Owner->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_SANCTUARY))
                            l_Owner->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_SANCTUARY);

                        return;
                    }
                    else
                        if (!l_Owner->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_SANCTUARY))
                            l_Owner->SetByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_SANCTUARY);
                }
                else
                    m_Timer -= p_Diff;
            }

            void OnRemove(AuraEffect const* /* p_AurEff */, AuraEffectHandleModes /* p_Mode */)
            {
                Unit* l_Owner = GetUnitOwner();

                if (!l_Owner)
                    return;

                if (l_Owner->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_SANCTUARY))
                    l_Owner->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_SANCTUARY);
            }

            uint32 m_Timer;

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_fun_quick_recovery_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_RESET_COOLDOWNS_BEFORE_DUEL);
                OnEffectRemove += AuraEffectRemoveFn(spell_fun_quick_recovery_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_RESET_COOLDOWNS_BEFORE_DUEL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_fun_quick_recovery_AuraScript();
        }
};

class go_duel_area_flag : public GameObjectScript
{
    public:
        go_duel_area_flag() : GameObjectScript("go_duel_area_flag") { }

        struct go_duel_area_flagAI : public GameObjectAI
        {
            go_duel_area_flagAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject)
            {
                m_Timer = 1000;
            }

            void UpdateAI(uint32 p_Diff) override
            {
                if (m_Timer <= p_Diff)
                {
                    m_Timer = 1000;

                    std::list<Player*> l_Players;
                    JadeCore::AnyPlayerInObjectRangeCheck l_Checker(go, DUEL_AREA_RANGE - 10.f);
                    JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(go, l_Players, l_Checker);
                    go->VisitNearbyWorldObject(DUEL_AREA_RANGE - 10.f, l_Searcher); /// Prevent reapply spam with the -10.f

                    for (auto l_Iter : l_Players)
                        if (!l_Iter->HasAura(SPELL_FUN_QUICK_RECOVERY))
                            go->CastSpell(l_Iter, SPELL_FUN_QUICK_RECOVERY);
                }
                else
                    m_Timer -= p_Diff;
            }

            uint32 m_Timer;
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_duel_area_flagAI(p_GameObject);
        }
};

void AddSC_FunScripts()
{
    new spell_fun_quick_recovery();
    new go_duel_area_flag();
}
