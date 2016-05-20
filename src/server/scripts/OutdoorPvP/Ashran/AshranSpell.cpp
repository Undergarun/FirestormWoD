////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Blade Twister - 178795
class spell_ashran_blade_twister: public SpellScriptLoader
{
    public:
        spell_ashran_blade_twister() : SpellScriptLoader("spell_ashran_blade_twister") { }

        class spell_ashran_blade_twister_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_blade_twister_AuraScript);

            enum eSpells
            {
                BladeTwisterDamage = 177167
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    std::list<Creature*> l_Blades;
                    l_Target->GetCreatureListWithEntryInGrid(l_Blades, eCreatures::BladeTwisterTrigger, 50.0f);

                    if (l_Blades.empty())
                    {
                        p_AurEff->GetBase()->Remove();
                        return;
                    }

                    l_Blades.remove_if([this, l_Target](Creature* p_Creature) -> bool
                    {
                        if (!p_Creature->GetOwner())
                            return true;

                        if (p_Creature->GetOwner() != l_Target)
                            return true;

                        return false;
                    });

                    for (Creature* l_Creature : l_Blades)
                        l_Target->CastSpell(l_Creature, eSpells::BladeTwisterDamage, true);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashran_blade_twister_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_blade_twister_AuraScript();
        }
};

/// Living Bomb - 176670
class spell_ashran_emberfall_living_bomb: public SpellScriptLoader
{
    public:
        spell_ashran_emberfall_living_bomb() : SpellScriptLoader("spell_ashran_emberfall_living_bomb") { }

        class spell_ashran_emberfall_living_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_emberfall_living_bomb_AuraScript);

            enum eSpells
            {
                LivingBombTriggered = 176673
            };

            void AfterRemove(AuraEffect const*, AuraEffectHandleModes)
            {
                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode != AuraRemoveMode::AURA_REMOVE_BY_DEATH && removeMode != AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, eSpells::LivingBombTriggered, true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_ashran_emberfall_living_bomb_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_emberfall_living_bomb_AuraScript();
        }
};

/// Alliance Reward - 178531
/// Horde Reward - 178533
class spell_ashran_faction_rewards : public SpellScriptLoader
{
    public:
        spell_ashran_faction_rewards() : SpellScriptLoader("spell_ashran_faction_rewards") { }

        class spell_ashran_faction_rewards_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_faction_rewards_SpellScript);

            enum Items
            {
                StrongboxHorde      = 120151,
                StrongboxAlliance   = 118065
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return true;

                    Player* l_Player = p_Object->ToPlayer();
                    if (l_Player == nullptr)
                        return true;

                    /// Only one strongbox per day
                    if (!l_Player->CanHaveDailyLootForItem(Items::StrongboxAlliance) ||
                        !l_Player->CanHaveDailyLootForItem(Items::StrongboxHorde))
                        return true;

                    return false;
                });
            }

            void HandleOnHit()
            {
                if (GetHitUnit() == nullptr)
                    return;

                if (Player* l_Player = GetHitUnit()->ToPlayer())
                {
                    if (GetSpellInfo()->Id == eAshranSpells::SpellAllianceReward)
                        l_Player->AddDailyLootCooldown(Items::StrongboxHorde);
                    else
                        l_Player->AddDailyLootCooldown(Items::StrongboxAlliance);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_faction_rewards_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnHit += SpellHitFn(spell_ashran_faction_rewards_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_faction_rewards_SpellScript();
        }
};

/// Booming Shout - 177150
class spell_ashran_booming_shout : public SpellScriptLoader
{
    public:
        spell_ashran_booming_shout() : SpellScriptLoader("spell_ashran_booming_shout") { }

        class spell_ashran_booming_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_booming_shout_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (p_Object->GetEntry() == eCreatures::GaulDunFirok ||
                        p_Object->GetEntry() == eCreatures::MukmarRaz ||
                        p_Object->GetEntry() == eCreatures::KorlokTheOgreKing)
                        return true;

                    return false;
                });
            }

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        float l_MaxRadius = GetSpellInfo()->Effects[p_EffIndex].CalcRadius(l_Caster, GetSpell());
                        float l_Dist = l_Caster->GetDistance(l_Target);
                        float l_Pct = l_Dist / l_MaxRadius;
                        SetHitDamage(int32(GetHitDamage() * l_Pct));
                    }
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_booming_shout_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_booming_shout_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_ashran_booming_shout_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_booming_shout_SpellScript();
        }
};

/// Curse of Krong - 165134
class spell_ashran_curse_of_krong : public SpellScriptLoader
{
    public:
        spell_ashran_curse_of_krong() : SpellScriptLoader("spell_ashran_curse_of_krong") { }

        class spell_ashran_curse_of_krong_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_curse_of_krong_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->DealDamage(l_Target, l_Target->CountPctFromMaxHealth(p_AurEff->GetAmount()), nullptr, DOT);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashran_curse_of_krong_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_curse_of_krong_AuraScript();
        }
};

/// Artifacts Collected - 177393
class spell_ashran_artifacts_collected : public SpellScriptLoader
{
    public:
        spell_ashran_artifacts_collected() : SpellScriptLoader("spell_ashran_artifacts_collected") { }

        class spell_ashran_artifacts_collected_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_artifacts_collected_AuraScript);

            void UpdateCurrentCount(uint32, AuraEffect* p_AurEff)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Creature* l_Caster = GetCaster()->ToCreature())
                {
                    ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetZoneScript(l_Caster->GetZoneId());
                    if (l_ZoneScript == nullptr)
                        return;

                    uint8 l_Type = eArtifactsDatas::CountForMage;
                    uint8 l_TeamID = TeamId::TEAM_ALLIANCE;
                    switch (l_Caster->GetEntry())
                    {
                        case eCreatures::Nisstyr:
                            l_Type = eArtifactsDatas::CountForWarlock;
                            l_TeamID = TeamId::TEAM_HORDE;
                            break;
                        case eCreatures::Marketa:
                            l_Type = eArtifactsDatas::CountForWarlock;
                            break;
                        case eCreatures::Fura:
                            l_TeamID = TeamId::TEAM_HORDE;
                            break;
                        case eCreatures::Kalgan:
                            l_Type = eArtifactsDatas::CountForWarriorPaladin;
                            l_TeamID = TeamId::TEAM_HORDE;
                            break;
                        case eCreatures::ValantBrightsworn:
                            l_Type = eArtifactsDatas::CountForWarriorPaladin;
                            break;
                        case eCreatures::Atomik:
                            l_Type = eArtifactsDatas::CountForDruidShaman;
                            l_TeamID = TeamId::TEAM_HORDE;
                            break;
                        case eCreatures::Anenga:
                            l_Type = eArtifactsDatas::CountForDruidShaman;
                            break;
                        case eCreatures::Ecilam:
                        default:
                            break;
                    }

                    if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    {
                        uint32 l_Count = l_Ashran->GetArtifactCollected(l_TeamID, l_Type);
                        p_AurEff->ChangeAmount(l_Count);
                    }
                }
            }

            void SetMaxCount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Creature* l_Caster = GetCaster()->ToCreature())
                {
                    switch (l_Caster->GetEntry())
                    {
                        case eCreatures::Marketa:
                        case eCreatures::Nisstyr:
                            p_Amount = eArtifactsDatas::MaxCountForWarlock;
                            break;
                        case eCreatures::Ecilam:
                        case eCreatures::Fura:
                            p_Amount = eArtifactsDatas::MaxCountForMage;
                            break;
                        case eCreatures::Kalgan:
                        case eCreatures::ValantBrightsworn:
                            p_Amount = eArtifactsDatas::MaxCountForWarriorPaladin;
                            break;
                        case eCreatures::Atomik:
                        case eCreatures::Anenga:
                            p_Amount = eArtifactsDatas::MaxCountForDruidShaman;
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register() override
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_ashran_artifacts_collected_AuraScript::UpdateCurrentCount, EFFECT_0, SPELL_AURA_DUMMY);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ashran_artifacts_collected_AuraScript::SetMaxCount, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_artifacts_collected_AuraScript();
        }
};

/// Stone Empowerment - 170896
class spell_ashran_stone_empowerment : public SpellScriptLoader
{
    public:
        spell_ashran_stone_empowerment() : SpellScriptLoader("spell_ashran_stone_empowerment") { }

        enum eSpell
        {
            StoneEmpowermentProc = 170897
        };

        class spell_ashran_stone_empowerment_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_stone_empowerment_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                /// Caster is Kronus, target are enemies
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (p_EventInfo.GetActionTarget() == l_Caster)
                    return;

                l_Caster->CastSpell(l_Caster, eSpell::StoneEmpowermentProc, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_ashran_stone_empowerment_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_stone_empowerment_AuraScript();
        }
};

/// Summon Disposable Pocket Flying Machine - 168232 (Alliance)
/// Summon Disposable Pocket Flying Machine - 170407 (Horde)
class spell_ashran_pocket_flying_machine : public SpellScriptLoader
{
    public:
        spell_ashran_pocket_flying_machine() : SpellScriptLoader("spell_ashran_pocket_flying_machine") { }

        class spell_ashran_pocket_flying_machine_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_pocket_flying_machine_SpellScript);

            enum eSpells
            {
                DistrubutionAlliance    = 168232,
                DistrubitionHorde       = 170407
            };

            void HandleScriptEffect()
            {
                if (GetHitUnit() == nullptr)
                    return;

                if (Player* l_Player = GetHitUnit()->ToPlayer())
                {
                    ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(l_Player->GetZoneId());
                    if (l_ZoneScript == nullptr)
                        return;

                    if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    {
                        uint32 l_ArtifactCount = l_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                        l_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                        uint32 l_Modulo = l_ArtifactCount % eArtifactsDatas::MaxArtifactCounts;
                        uint32 l_Count = l_ArtifactCount / eArtifactsDatas::MaxArtifactCounts;
                        uint8 l_Rand = urand(0, eArtifactsDatas::MaxArtifactCounts - 1);

                        uint32 l_SpellID = GetSpellInfo()->Id;
                        for (uint8 l_I = 0; l_I < eArtifactsDatas::MaxArtifactCounts; ++l_I)
                        {
                            if (l_SpellID == eSpells::DistrubutionAlliance)
                            {
                                if (l_I == l_Rand)
                                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, l_I, l_Count + l_Modulo);
                                else
                                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, l_I, l_Count);
                            }
                            else
                            {
                                if (l_I == l_Rand)
                                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, l_I, l_Count + l_Modulo);
                                else
                                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, l_I, l_Count);
                            }
                        }

                        l_Ashran->RewardHonorAndReputation(l_ArtifactCount, l_Player);
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_ashran_pocket_flying_machine_SpellScript::HandleScriptEffect);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_pocket_flying_machine_SpellScript();
        }
};

/// Vile Blood - 162908
class spell_ashran_vile_blood : public SpellScriptLoader
{
    public:
        spell_ashran_vile_blood() : SpellScriptLoader("spell_ashran_vile_blood") { }

        enum eSpell
        {
            VileBloodProc = 167129
        };

        class spell_ashran_vile_blood_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_vile_blood_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                /// Caster is Kronus, target are enemies
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (p_EventInfo.GetActionTarget() == l_Caster)
                    return;

                uint32 l_ID = p_EventInfo.GetDamageInfo()->GetSpellInfo() ? p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id : 0;
                if (l_ID == eSpell::VileBloodProc)
                    return;

                l_Caster->CastSpell(l_Caster, eSpell::VileBloodProc, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_ashran_vile_blood_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_vile_blood_AuraScript();
        }
};

/// Splitting Breath - 161520
class spell_ashran_splitting_breath : public SpellScriptLoader
{
    public:
        spell_ashran_splitting_breath() : SpellScriptLoader("spell_ashran_splitting_breath") { }

        class spell_ashran_splitting_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_splitting_breath_SpellScript);

            enum eSpell
            {
                TargetRestrict = 21544
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_splitting_breath_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_splitting_breath_SpellScript();
        }
};

/// Shadow Claws - 176542
class spell_ashran_shadow_claws : public SpellScriptLoader
{
    public:
        spell_ashran_shadow_claws() : SpellScriptLoader("spell_ashran_shadow_claws") { }

        class spell_ashran_shadow_claws_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_shadow_claws_SpellScript);

            enum eSpell
            {
                TargetRestrict = 24395
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_shadow_claws_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_shadow_claws_SpellScript();
        }
};

/// Darkness Within - 158830
class spell_ashran_darkness_within : public SpellScriptLoader
{
    public:
        spell_ashran_darkness_within() : SpellScriptLoader("spell_ashran_darkness_within") { }

        class spell_ashran_darkness_within_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_darkness_within_AuraScript);

            enum eSpell
            {
                SpellDarknessWithinSearcher = 158844
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpell::SpellDarknessWithinSearcher, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ashran_darkness_within_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_darkness_within_AuraScript();
        }
};

/// Earth Smash - 176187
class spell_ashran_earth_smash : public SpellScriptLoader
{
    public:
        spell_ashran_earth_smash() : SpellScriptLoader("spell_ashran_earth_smash") { }

        class spell_ashran_earth_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_earth_smash_SpellScript);

            enum eSpell
            {
                TargetRestrict = 24334
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Radius = GetSpellInfo()->Effects[0].CalcRadius(l_Caster);
                p_Targets.remove_if([l_Radius, l_Caster, l_Restriction](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->IsInAxe(l_Caster, l_Restriction->Width, l_Radius))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_earth_smash_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_129);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_earth_smash_SpellScript();
        }
};

/// Preserved Discombobulator Ray - 168224
class spell_ashran_preserved_discombobulator_ray : public SpellScriptLoader
{
    public:
        spell_ashran_preserved_discombobulator_ray() : SpellScriptLoader("spell_ashran_preserved_discombobulator_ray") { }

        class spell_ashran_preserved_discombobulator_ray_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_preserved_discombobulator_ray_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->Dismount();
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_ashran_preserved_discombobulator_ray_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_preserved_discombobulator_ray_AuraScript();
        }
};

/// Shockwave - 164092
class spell_ashran_shockwave : public SpellScriptLoader
{
    public:
        spell_ashran_shockwave() : SpellScriptLoader("spell_ashran_shockwave") { }

        class spell_ashran_shockwave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ashran_shockwave_SpellScript);

            enum eSpell
            {
                TargetRestrict = 21000
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_shockwave_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ashran_shockwave_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ashran_shockwave_SpellScript();
        }
};

/// Ancient Artifact - 168506
class spell_ashran_ancient_artifact : public SpellScriptLoader
{
    public:
        spell_ashran_ancient_artifact() : SpellScriptLoader("spell_ashran_ancient_artifact") { }

        class spell_ashran_ancient_artifact_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_ancient_artifact_AuraScript);

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (Unit* l_Target = GetTarget())
                {
                    OutdoorPvP* l_Outdoor = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(l_Target->GetZoneId());
                    if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_Outdoor)
                        l_Ashran->HandleArtifactDrop(l_Target, l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_EXPIRE ? 0 : GetDuration());
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_ashran_ancient_artifact_AuraScript::OnRemove, EFFECT_4, SPELL_AURA_398, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_ancient_artifact_AuraScript();
        }
};

/// Horde Racer - 166819
/// Alliance Racer - 166784
class spell_ashran_horde_and_alliance_racer : public SpellScriptLoader
{
    public:
        spell_ashran_horde_and_alliance_racer() : SpellScriptLoader("spell_ashran_horde_and_alliance_racer") { }

        class spell_ashran_horde_and_alliance_racer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ashran_horde_and_alliance_racer_AuraScript);

            enum eSpells
            {
                HordeRider      = 166819,
                AllianceRider   = 166784
            };

            enum eDatas
            {
                SpeedPCTPerPlayer   = 30,
                MaxSpeedPCT         = 500
            };

            uint32 m_CheckTimer;

            bool Load() override
            {
                m_CheckTimer = 200;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (m_CheckTimer)
                    {
                        if (m_CheckTimer <= p_Diff)
                        {
                            m_CheckTimer = 200;

                            std::list<Player*> l_PlayerList;
                            l_Caster->GetPlayerListInGrid(l_PlayerList, 10.0f);

                            if (l_PlayerList.empty())
                            {
                                if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                                    l_AurEff->ChangeAmount(0);

                                return;
                            }

                            l_PlayerList.remove_if([this](Player* p_Player) -> bool
                            {
                                if (p_Player == nullptr)
                                    return true;

                                if (GetSpellInfo()->Id == eSpells::AllianceRider && p_Player->GetTeamId() != TeamId::TEAM_ALLIANCE)
                                    return true;

                                if (GetSpellInfo()->Id == eSpells::HordeRider && p_Player->GetTeamId() != TeamId::TEAM_HORDE)
                                    return true;

                                return false;
                            });

                            if (l_PlayerList.empty())
                            {
                                if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                                    l_AurEff->ChangeAmount(0);

                                return;
                            }

                            /// The riders move very slowly, but for each player of their faction within their vicinity their speed is increased by 30%,
                            /// allowing them to outstrip their competitor.
                            if (AuraEffect* l_AurEff = l_Caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                                l_AurEff->ChangeAmount(std::min(((int32)l_PlayerList.size() * eDatas::SpeedPCTPerPlayer), (int32)eDatas::MaxSpeedPCT));
                        }
                        else
                            m_CheckTimer -= p_Diff;
                    }
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_ashran_horde_and_alliance_racer_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ashran_horde_and_alliance_racer_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_AshranSpells()
{
    new spell_ashran_blade_twister();
    new spell_ashran_emberfall_living_bomb();
    new spell_ashran_faction_rewards();
    new spell_ashran_booming_shout();
    new spell_ashran_curse_of_krong();
    new spell_ashran_artifacts_collected();
    new spell_ashran_stone_empowerment();
    new spell_ashran_pocket_flying_machine();
    new spell_ashran_vile_blood();
    new spell_ashran_splitting_breath();
    new spell_ashran_shadow_claws();
    new spell_ashran_darkness_within();
    new spell_ashran_earth_smash();
    new spell_ashran_preserved_discombobulator_ray();
    new spell_ashran_shockwave();
    new spell_ashran_ancient_artifact();
    new spell_ashran_horde_and_alliance_racer();
}
#endif
