////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonSpell.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    /// Constructor
    spell_garrison_hearthstone::spell_garrison_hearthstone()
        : SpellScriptLoader("spell_garrison_hearthstone")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Function called on server startup, if returns false script won't be used in core
    /// use for: dbc/template data presence/correctness checks
    bool spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::Validate(SpellInfo const* p_SpellEntry)
    {
        if (!sSpellMgr->GetSpellInfo(Spells::SPELL_GARRISON_HEARTHSTONE))
            return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell check cast
    SpellCastResult spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::HandleCheckCast()
    {
        Player* l_Player = GetCaster()->ToPlayer();

        if (GetCaster()->isInCombat() && l_Player)
        {
            l_Player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, GetSpell()->m_CastItem, NULL);
            return SPELL_FAILED_CUSTOM_ERROR;
        }

        if (l_Player && !l_Player->GetGarrison())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        return SPELL_CAST_OK;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell EFFECT_0 dummy proc
    /// @p_EffectIndex : Effect index
    void spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::HandleDummy(SpellEffIndex p_EffectIndex)
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (l_Player && l_Player->GetGarrison() && l_Player->GetGarrison()->GetGarrisonSiteLevelEntry())
        {
            Manager* l_Garrison = l_Player->GetGarrison();

            uint32 l_Index = (l_Garrison->GetGarrisonFactionIndex() * MS::Garrison::Globals::MaxLevel) + (l_Garrison->GetGarrisonSiteLevelEntry()->Level - 1);

            float l_X = gGarrisonHearthStoneTeleportCoord[l_Index][0];
            float l_Y = gGarrisonHearthStoneTeleportCoord[l_Index][1];
            float l_Z = gGarrisonHearthStoneTeleportCoord[l_Index][2];
            float l_O = gGarrisonHearthStoneTeleportCoord[l_Index][3];

            l_Player->TeleportTo(l_Garrison->GetGarrisonSiteLevelEntry()->MapID, l_X, l_Y, l_Z, l_O);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SpellScript/AuraScript interface base
    /// these functions are safe to override, see notes below for usage instructions
    ///
    /// Function in which handler functions are registered, must be implemented in script
    void spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::Register()
    {
        OnCheckCast += SpellCheckCastFn(spell_garrison_hearthstone_SpellScript::HandleCheckCast);
        OnEffectHit += SpellEffectFn(spell_garrison_hearthstone_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Should return a fully valid SpellScript pointer.
    SpellScript* spell_garrison_hearthstone::GetSpellScript() const
    {
        return new spell_garrison_hearthstone_SpellScript();
    }

    /// Constructor
    spell_garrison_portal::spell_garrison_portal()
        : SpellScriptLoader("spell_garrison_portal")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Function called on server startup, if returns false script won't be used in core
    /// use for: dbc/template data presence/correctness checks
    bool spell_garrison_portal::spell_garrison_portal_SpellScript::Validate(SpellInfo const* p_SpellEntry)
    {
        if (!sSpellMgr->GetSpellInfo(Spells::SpellGarrisonPortal))
            return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell check cast
    SpellCastResult spell_garrison_portal::spell_garrison_portal_SpellScript::HandleCheckCast()
    {
        Player* l_Player = GetCaster()->ToPlayer();

        if (l_Player && !l_Player->GetGarrison())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        return SPELL_CAST_OK;
    }

    /// Constructor
    spell_garrison_shipyard::spell_garrison_shipyard() : SpellScriptLoader("spell_garrison_shipyard")
    {
    }

    SpellCastResult spell_garrison_shipyard::spell_garrison_shipyard_SpellScript::HandleCheckCast()
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (!l_Player || l_Player->isInCombat())
        {
            l_Player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, GetSpell()->m_CastItem, NULL);
            return SPELL_FAILED_CUSTOM_ERROR;
        }

        Garrison::Manager* l_Garrison = l_Player->GetGarrison();

        if (!l_Player->GetGarrison())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        if (l_Garrison->HasShipyard())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        auto l_SiteLevelEntry = l_Garrison->GetGarrisonSiteLevelEntry();

        if (!l_SiteLevelEntry || l_SiteLevelEntry->Level != 3)
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        return SPELL_CAST_OK;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell EFFECT_0 dummy proc
    /// @p_EffectIndex : Effect index
    void spell_garrison_portal::spell_garrison_portal_SpellScript::HandleDummy(SpellEffIndex p_EffectIndex)
    {
        if (!GetCaster())
            return;

        if (Player* l_Player = GetCaster()->ToPlayer())
        {
            if (MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison())
            {
                switch (l_GarrisonMgr->GetGarrisonLevel())
                {
                    case 1:
                    {
                        if (l_Player->GetTeamId() == TEAM_ALLIANCE)
                            l_Player->TeleportTo(1158, 1847.2951f, 263.9954f, 78.3144f, 5.148287f); ///< Alliance Garrison Lvl 1
                        else
                            l_Player->TeleportTo(1152, 5561.7416f, 4509.7124f, 132.6979f, 3.5397f); ///< Horde Garrison Lvl 1
                        break;
                    }
                    case 2:
                    {
                        if (l_Player->GetTeamId() == TEAM_ALLIANCE)
                            l_Player->TeleportTo(1331, 1945.741f, 319.8971f, 90.4052f, 1.1545f); ///< Alliance Garrison Lvl 2
                        else
                            l_Player->TeleportTo(1330, 5568.2348f, 4590.6333f, 140.9079f, 2.2988f); ///< Horde Garrison Lvl 2
                        break;
                   }
                   case 3: ///< Garrison lvl 3 not released yet
                   default:
                        break;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SpellScript/AuraScript interface base
    /// these functions are safe to override, see notes below for usage instructions
    ///
    /// Function in which handler functions are registered, must be implemented in script
    void spell_garrison_portal::spell_garrison_portal_SpellScript::Register()
    {
        OnCheckCast += SpellCheckCastFn(spell_garrison_portal_SpellScript::HandleCheckCast);
        OnEffectHit += SpellEffectFn(spell_garrison_portal_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Should return a fully valid SpellScript pointer.
    SpellScript* spell_garrison_portal::GetSpellScript() const
    {
        return new spell_garrison_portal_SpellScript();
    }

    void spell_garrison_shipyard::spell_garrison_shipyard_SpellScript::HandleDummy(SpellEffIndex p_EffectIndex)
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (!l_Player)
            return;

        Garrison::Manager* l_Garrison = l_Player->GetGarrison();

        if (!l_Garrison)
            return;

        l_Garrison->CreateShipyardBySpell();
    }

    void spell_garrison_shipyard::spell_garrison_shipyard_SpellScript::Register()
    {
        OnCheckCast += SpellCheckCastFn(spell_garrison_shipyard_SpellScript::HandleCheckCast);
        OnEffectHit += SpellEffectFn(spell_garrison_shipyard_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
    }

    /// Should return a fully valid SpellScript pointer.
    SpellScript * spell_garrison_shipyard::GetSpellScript() const
    {
        return new spell_garrison_shipyard_SpellScript();
    }

    /// Stables Lassos - 173686/174070
    class spell_garrison_stables_lasso : public SpellScriptLoader
    {
        public:
            uint32 g_LassoBreakSpellID;
            spell_garrison_stables_lasso() : SpellScriptLoader("spell_garrison_stables_lasso") { g_LassoBreakSpellID = 173702; }

            class spell_garrison_stables_lasso_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_garrison_stables_lasso_AuraScript);

                void OnTick(AuraEffect const* p_AurEff)
                {
                    Unit* l_Caster = GetCaster();
                    WorldObject* l_Target = GetOwner();

                    if (l_Caster == nullptr || l_Target == nullptr)
                        return;

                    if (l_Caster->GetDistance2d(l_Target) >= 15.0f)
                    {
                        if (Creature* l_Creature = l_Target->ToCreature())
                            l_Creature->CastSpell(l_Creature, 173702, true);
                    }
                }

                void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
                {
                    WorldObject* l_Target = GetOwner();

                    if (l_Target == nullptr)
                        return;

                    if (Creature* l_Creature = l_Target->ToCreature())
                        l_Creature->DespawnOrUnsummon();
                }

                void Register() override
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_garrison_stables_lasso_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                    OnEffectRemove += AuraEffectRemoveFn(spell_garrison_stables_lasso_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_garrison_stables_lasso_AuraScript();
            }

            class spell_garrison_stables_lasso_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_garrison_stables_lasso_SpellScript);

                enum eEntries
                {
                    NpcSnarler       = 86851,
                    NpcIcehoof       = 86847,
                    NpcMeadowstomper = 86852,
                    NpcRiverwallow   = 86848,
                    NpcRocktusk      = 86850,
                    NpcSilverpelt    = 86801
                };

                void HandleBeforeCast()
                {
                    Unit* l_Target = GetExplTargetUnit();

                    if (Creature* l_Creature = l_Target->ToCreature())
                    {
                        switch (l_Creature->GetEntry())
                        {
                            case eEntries::NpcSnarler:
                            case eEntries::NpcIcehoof:
                            case eEntries::NpcMeadowstomper:
                            case eEntries::NpcRiverwallow:
                            case eEntries::NpcRocktusk:
                            case eEntries::NpcSilverpelt:
                                break;
                            default:
                                FinishCast(SpellCastResult::SPELL_FAILED_BAD_TARGETS);
                                break;
                        }
                    }
                    else
                        FinishCast(SpellCastResult::SPELL_FAILED_BAD_TARGETS);
                }

                void Register()
                {
                    BeforeCast += SpellCastFn(spell_garrison_stables_lasso_SpellScript::HandleBeforeCast);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_garrison_stables_lasso_SpellScript();
            }
    };

    /// 168655 / Sticky Grenade
    class spell_aura_sticky_grenade : public SpellScriptLoader
    {
        public:
            spell_aura_sticky_grenade() : SpellScriptLoader("spell_aura_sticky_grenade") { }

            class spell_aura_sticky_grenade_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_aura_sticky_grenade_AuraScript);

                enum eDatas
                {
                    SpellStickyGrenadeTargetDmg = 168659
                };

                void OnRemove(AuraEffect const* /*m_AurEff*/, AuraEffectHandleModes /*m_Mode*/)
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Caster, eDatas::SpellStickyGrenadeTargetDmg, true);
                }

                void Register()
                {
                    AfterEffectRemove += AuraEffectRemoveFn(spell_aura_sticky_grenade_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
                }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_aura_sticky_grenade_AuraScript();
        }
    };

    /// Pneumatic Power Gauntlet - 168555
    class spell_pneumatic_power_gauntlet : public SpellScriptLoader
    {
        public:
            spell_pneumatic_power_gauntlet() : SpellScriptLoader("spell_pneumatic_power_gauntlet") { }

            class spell_pneumatic_power_gauntlet_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_pneumatic_power_gauntlet_SpellScript);

                void OnSpellHit(SpellEffIndex)
                {
                    Unit* l_Caster = GetCaster();
                    Unit* l_Target = GetExplTargetUnit();

                    if (l_Caster == nullptr || l_Target == nullptr)
                        return;

                    l_Target->GetMotionMaster()->MoveJump(l_Caster->m_positionX, l_Caster->m_positionY, l_Caster->m_positionZ, 15.0f, 10.0f);
                }

                void Register()
                {
                    OnEffectHitTarget += SpellEffectFn(spell_pneumatic_power_gauntlet_SpellScript::OnSpellHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_pneumatic_power_gauntlet_SpellScript();
            }
    };

    class spell_GarrisonRouseTrader : public SpellScriptLoader
    {
        public:
            spell_GarrisonRouseTrader() : SpellScriptLoader("spell_GarrisonRouseTrader") { }

            class spell_GarrisonRouseTrader_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_GarrisonRouseTrader_SpellScript);

                SpellCastResult CheckCast()
                {
                    Unit* l_Target = GetExplTargetUnit();

                    if (Creature* l_Creature = l_Target->ToCreature())
                    {
                        if (!l_Creature->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR))
                            return SPELL_FAILED_BAD_TARGETS;
                    }
                    else
                        return SPELL_FAILED_BAD_TARGETS;

                    return SPELL_CAST_OK;
                }

                void OnSpellHit(SpellEffIndex /*p_EffIndex*/)
                {
                    Unit* l_Caster = GetCaster();

                    if (l_Caster->GetTypeId() == TYPEID_PLAYER)
                        l_Caster->ToPlayer()->KilledMonsterCredit(87254);
                }

                void Register()
                {
                    OnCheckCast += SpellCheckCastFn(spell_GarrisonRouseTrader_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_GarrisonRouseTrader_SpellScript::OnSpellHit, EFFECT_0, SPELL_EFFECT_DUMMY);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_GarrisonRouseTrader_SpellScript();
            }
    };

    /// Well-rested - 172425
    class spell_garrison_well_rested : public SpellScriptLoader
    {
        public:
            spell_garrison_well_rested() : SpellScriptLoader("spell_garrison_well_rested") { }

            class spell_garrison_well_rested_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_garrison_well_rested_AuraScript);

                enum eSpells
                {
                    SpellWellRestedTrackingAura = 172424
                };

                void OnAuraApply(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
                {
                    Unit* l_Owner = GetUnitOwner();

                    if (l_Owner == nullptr)
                        return;

                    if (l_Owner->HasAura(eSpells::SpellWellRestedTrackingAura))
                        l_Owner->RemoveAura(eSpells::SpellWellRestedTrackingAura);
                }

                void Register() override
                {
                    OnEffectApply += AuraEffectApplyFn(spell_garrison_well_rested_AuraScript::OnAuraApply, EFFECT_0, SPELL_AURA_MOD_PERCENT_STAT, AURA_EFFECT_HANDLE_REAL);
                }
            };

            AuraScript* GetAuraScript() const override
            {
                return new spell_garrison_well_rested_AuraScript();
            }
    };

    class spell_garrison_tent_spawn : public SpellScriptLoader
    {
        public:
            spell_garrison_tent_spawn() : SpellScriptLoader("spell_garrison_tent_spawn") { }

            class spell_garrison_tent_spawn_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_garrison_tent_spawn_SpellScript);

                SpellCastResult CheckCast()
                {
                    Unit* l_Caster = GetCaster();

                    /// Only in Draenor map or Garrison
                    if (l_Caster->GetMapId() != 1116 && (l_Caster->GetTypeId() == TYPEID_PLAYER && !l_Caster->ToPlayer()->IsInGarrison()))
                        return SPELL_FAILED_INCORRECT_AREA;

                    return SPELL_CAST_OK;
                }

                void Register()
                {
                    OnCheckCast += SpellCheckCastFn(spell_garrison_tent_spawn_SpellScript::CheckCast);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_garrison_tent_spawn_SpellScript();
            }
    };

    /// Well-rested - 172425
    class spell_aura_garrison_skyterror_falling : public SpellScriptLoader
    {
        public:
            spell_aura_garrison_skyterror_falling() : SpellScriptLoader("spell_aura_garrison_skyterror_falling") { }

            class spell_aura_garrison_skyterror_falling_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_aura_garrison_skyterror_falling_AuraScript);

                void OnUpdate(uint32 p_Diff)
                {
                    Unit* l_Owner = GetUnitOwner();

                    if (l_Owner == nullptr)
                        return;

                    /// Awaits for hardcpp's PR validation...
///                    if (!l_Owner->IsFalling())
///                        Remove();
                }

                void Register() override
                {
                    OnAuraUpdate += AuraUpdateFn(spell_aura_garrison_skyterror_falling_AuraScript::OnUpdate);
                }
            };

            AuraScript* GetAuraScript() const override
            {
                return new spell_aura_garrison_skyterror_falling_AuraScript();
            }
    };

    /// Pneumatic Power Gauntlet - 168555
    class spell_garrison_combine_scribe_items : public SpellScriptLoader
    {
        public:
            spell_garrison_combine_scribe_items() : SpellScriptLoader("spell_garrison_combine_scribe_items") { }

            class spell_garrison_combine_scribe_items_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_garrison_combine_scribe_items_SpellScript);

                void OnSpellHit(SpellEffIndex)
                {
                    Unit* l_Caster = GetCaster();

                    if (l_Caster == nullptr || l_Caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    uint32 l_RewardID = 119027;
                    uint32 l_Chance = urand(0, 99);

                    if (l_Chance <= 20)
                        l_RewardID = 119028;
                    else if (l_Chance > 20 && l_Chance <= 50)
                        l_RewardID = 119023;

                    /// check space and find places
                    uint32 l_NoSpaceForCount = 0;
                    ItemPosCountVec l_Destination;

                    InventoryResult l_Message = l_Caster->ToPlayer()->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, l_RewardID, 1, &l_NoSpaceForCount);

                    if (l_Message == EQUIP_ERR_OK)
                    {
                        if (Item* l_Item = l_Caster->ToPlayer()->StoreNewItem(l_Destination, l_RewardID, true, Item::GenerateItemRandomPropertyId(l_RewardID)))
                            l_Caster->ToPlayer()->SendNewItem(l_Item, 1, true, false, false);
                    }
                    else
                        l_Caster->ToPlayer()->SendEquipError(l_Message, nullptr, nullptr, l_RewardID);
                }

                void Register()
                {
                    OnEffectHitTarget += SpellEffectFn(spell_garrison_combine_scribe_items_SpellScript::OnSpellHit, EFFECT_0, SPELL_EFFECT_DUMMY);
                }
            };

            SpellScript* GetSpellScript() const
            {
                return new spell_garrison_combine_scribe_items_SpellScript();
            }
    };

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison()
{
    new MS::Garrison::spell_garrison_combine_scribe_items();
    new MS::Garrison::spell_aura_garrison_skyterror_falling();
    new MS::Garrison::spell_garrison_stables_lasso();
    new MS::Garrison::spell_garrison_hearthstone();
    new MS::Garrison::spell_garrison_portal();
    new MS::Garrison::spell_garrison_shipyard();
    new MS::Garrison::spell_aura_sticky_grenade();
    new MS::Garrison::spell_pneumatic_power_gauntlet();
    new MS::Garrison::spell_GarrisonRouseTrader();
    new MS::Garrison::spell_garrison_well_rested();
    new MS::Garrison::spell_garrison_tent_spawn();
}