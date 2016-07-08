////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "ScriptedCreature.h"

namespace MS { namespace Skill
{
    namespace SpellIDs
    {
        enum
        {
            /// Cooking
            SaberfishBroth   = 161001,
            GrilledSaberfish = 161002,

            /// Inscription
            ResearchWarbinderInkItem = 167948,
            ResearchMoonglowInk      = 165564,
            ResearchMidnightInk      = 165304,
            ResearchLionInk          = 165456,
            ResearchJadefireInk      = 165460,
            ResearchCelestialInk     = 165461,
            ResearchShimmeringInk    = 165463,
            ResearchEtherealInk      = 165464,
            ResearchInkOfTheSea      = 165465,
            ResearchBlackfallowInk   = 165466,
            ResearchInkOfDreams      = 165467,

            /// Reset secondary properties
            InscribedCrystal         = 166430,
            DidiDelicateAssembly     = 168121,
            HexweaveEmbroidery       = 168864,
            TaladiteRecrystalizer    = 173016,
            TruesteelGrinder         = 173347,
            TruesteelReshaper        = 173353,
            LeatherRefurbishingKit   = 173362,
            TaladiteRecrystalizer2   = 178351,
            TaladiteRecrystalizer3   = 178381,
            TaladiteRecrystalizer4   = 178382,
            TaladiteRecrystalizer5   = 178383
        };
    }

    namespace ItemIDs
    {
        enum
        {
            /// Cooking rewards for SpellIDs::SaberfishBroth && SpellIDs::GrilledSaberfish
            BlackrockHam                = 118311,
            ClefthoofSausages           = 118315,
            FatSleeperCakes             = 118319,
            FieryCalamari               = 118320,
            GrilledGulper               = 118317,
            PanSearedTalbuk             = 118312,
            RylakCrepes                 = 118314,
            SkulkerChowder              = 118321,
            SteamedScorpion             = 118316,
            SturgeonStew                = 118318,

            /// Blacksmithing
            TruesteelIngot              = 108257,

            /// Enchantment
            FracturedTemporalCrystal    = 115504,
            TemporalCrystal             = 113588,

            /// Inscription
            WarPaints                   = 112377,

            /// Tailoring
            HexweaveCloth               = 111556,

            /// First Aid
            AntisepticBandage           = 111603,

            /// Leatherworking
            BurnishedLeather            = 110611,

            /// Jewelcrafting
            TaladiteCrystal             = 115524,

            /// Engineering
            GearspringParts             = 111366,

            /// Alchemy
            AlchemicalCatalyst          = 108996,

            /// Additional rewards for daily crafting
            SorcerousAir                = 113264,
            SorcerousEarth              = 113263,
            SorcerousWater              = 113262,
            SorcerousFire               = 113261
        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// Generic spell script Draenor cooking rewards
    //////////////////////////////////////////////////////////////////////////
    class spell_Cooking_DraenorRecipesRewards : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_Cooking_DraenorRecipesRewards()
                : SpellScriptLoader("spell_Cooking_DraenorRecipesRewards")
            {

            }

            class spell_Cooking_DraenorRecipesRewards_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_Cooking_DraenorRecipesRewards_SpellScript);

                void HandleAfterCast()
                {
                    Player * l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;

                    if (!l_Player)
                        return;

                    uint32 l_SpellID = GetSpellInfo()->Id;

                    std::vector<uint32> l_RewardItems;

                    if (l_SpellID == SpellIDs::SaberfishBroth || l_SpellID == SpellIDs::GrilledSaberfish)
                    {
                        l_RewardItems.push_back(ItemIDs::BlackrockHam);
                        l_RewardItems.push_back(ItemIDs::ClefthoofSausages);
                        l_RewardItems.push_back(ItemIDs::FatSleeperCakes);
                        l_RewardItems.push_back(ItemIDs::FieryCalamari);
                        l_RewardItems.push_back(ItemIDs::GrilledGulper);
                        l_RewardItems.push_back(ItemIDs::PanSearedTalbuk);
                        l_RewardItems.push_back(ItemIDs::RylakCrepes);
                        l_RewardItems.push_back(ItemIDs::SkulkerChowder);
                        l_RewardItems.push_back(ItemIDs::SteamedScorpion);
                        l_RewardItems.push_back(ItemIDs::SturgeonStew);
                    }

                    /// Can have a reward (Guessed value)
                    if (!roll_chance_i(60))
                        return;

                    std::vector<uint32> l_Candidates;

                    for (uint32 l_ItemID : l_RewardItems)
                    {
                        ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_ItemID);

                        if (!l_ItemTemplate)
                            return;

                        if (l_ItemTemplate->Spells[0].SpellId == 483 /* Learning */)
                        {
                            if (!l_Player->HasSpell(l_ItemTemplate->Spells[1].SpellId))
                                l_Candidates.push_back(l_ItemID);
                        }
                    }

                    if (!l_Candidates.size())
                        return;


                    auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                    std::shuffle(l_Candidates.begin(), l_Candidates.end(), std::default_random_engine(l_Seed));

                    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(l_Candidates.at(0));

                    if (!pProto)
                        return;

                    ItemPosCountVec l_Dest;
                    uint32 l_NoSpace = 0;

                    if (l_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Dest, pProto->ItemId, 1, &l_NoSpace) == EQUIP_ERR_OK)
                    {
                        Item* pItem = l_Player->StoreNewItem(l_Dest, pProto->ItemId, true, Item::GenerateItemRandomPropertyId(pProto->ItemId));
                        l_Player->SendNewItem(pItem, 1, true, false);
                    }
                }

                void Register() override
                {
                    AfterCast += SpellCastFn(spell_Cooking_DraenorRecipesRewards_SpellScript::HandleAfterCast);
                }

            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript* GetSpellScript() const override
            {
                return new spell_Cooking_DraenorRecipesRewards_SpellScript();
            }

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// Inscription research spells
    //////////////////////////////////////////////////////////////////////////
    class spell_Inscription_Research : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_Inscription_Research()
                : SpellScriptLoader("spell_Inscription_Research")
            {

            }

            class spell_Inscription_Research_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_Inscription_Research_SpellScript);

                void HandleAfterCast()
                {
                    Player* l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;

                    if (!l_Player)
                        return;

                    uint32 l_SpellID = GetSpellInfo()->Id;
                    uint32 l_CurrentSkillValue = l_Player->GetSkillValue(SKILL_INSCRIPTION);

                    if (l_SpellID != SpellIDs::ResearchWarbinderInkItem)
                    {
                        for (uint32 l_I = 0; l_I < sSkillLineAbilityStore.GetNumRows(); ++l_I)
                        {
                            SkillLineAbilityEntry const* l_Entry = sSkillLineAbilityStore.LookupEntry(l_I);

                            if (!l_Entry || l_Entry->skillId != SKILL_INSCRIPTION || l_Entry->spellId != l_SpellID)
                                continue;

                            if (l_CurrentSkillValue < l_Entry->req_skill_value || l_CurrentSkillValue > l_Entry->max_value)
                                continue;

                            uint32 l_NumSkillUp = l_Entry->skill_gain;

                            if (l_CurrentSkillValue >= l_Entry->min_value)
                                l_NumSkillUp = 1;

                            l_Player->UpdateSkill(SKILL_INSCRIPTION, l_NumSkillUp);
                            break;
                        }
                    }

                    std::vector<uint32> l_RewardSpells;

                    switch (l_SpellID)
                    {
                        case SpellIDs::ResearchWarbinderInkItem:
                            /// http://www.wowhead.com/spell=167950/research-warbinders-ink#teaches-spell
                            l_RewardSpells = {
                                162805, 162840, 162813, 162818, 162877, 162841,
                                162869, 162847, 162858, 162842, 162829, 162878,
                                162856, 162808, 162815, 162855, 162857, 162859,
                                162831, 162884, 162832, 162833, 162846, 162837,
                                162860, 162861, 162848, 162844, 162879, 162806,
                                162807, 162814, 162851, 162834, 162862, 162843,
                                162871, 162820, 162872, 162850, 162880, 162822,
                                162819, 162817, 162824, 162826, 162881, 162810,
                                162882, 162863, 162830, 162835, 162849, 162811,
                                162812, 162821, 162854, 162873, 162864, 162883,
                                162865, 162852, 162827, 162838, 162874, 162853,
                                162867, 162866, 162876, 175186, 178448, 162823,
                                162839, 57160,  57152,  57190
                            };
                            break;
                        case SpellIDs::ResearchMoonglowInk:
                            /// http://www.wowhead.com/spell=165564/research-moonglow-ink#teaches-spell
                            l_RewardSpells = {
                                64253, 124460, 64276,  57224,  57251, 112437,
                                57261,  57277, 57120,  64270,  56983, 112454,
                                57221,  57185, 57222, 148283, 148260,  58314,
                                57003,  64302, 71102, 126687, 112265,  58298,
                                64285,  56989, 56965, 148278,  64255,  58332
                            };
                            break;
                        case SpellIDs::ResearchMidnightInk:
                            /// http://www.wowhead.com/spell=165304/research-midnight-ink#teaches-spell
                            l_RewardSpells = {
                                 58297, 148272,  71101,  64266,  57215, 148259,
                                 57122,  57242, 112450, 148282, 124459,  58326,
                                 94405,  57271,  56980,  57009, 112264, 148266,
                                112444,  57240,  58312,  57250,  57258,  57202,
                                 56958,  57014,  56975,  57247,  57191,  95825
                            };
                            break;
                        case SpellIDs::ResearchLionInk:
                            /// http://www.wowhead.com/spell=165456/research-lions-ink#teaches-spell
                            l_RewardSpells = {
                                58323,  58322,  57161,  57001, 148271,  57238,
                                57037, 135561,  58289,  57027, 148280, 112442,
                                57007, 148269, 112440,  57183, 148257,  58299,
                                57132, 148281,  57225, 123781,  57209, 124457,
                                57223,  57181,  56999,  57237,  59561,  56954,
                                57127
                            };
                            break;
                        case SpellIDs::ResearchJadefireInk:
                            /// http://www.wowhead.com/spell=165460/research-jadefire-ink#teaches-spell
                            l_RewardSpells = {
                                 57036, 112429, 57000,  57200,  57022,  57119,
                                 58302, 124449, 58345, 148279, 127625,  58347,
                                148261, 182157, 58341, 112468, 148255, 148268,
                                148270, 124451
                            };
                            break;
                        case SpellIDs::ResearchCelestialInk:
                            /// http://www.wowhead.com/spell=165461/research-celestial-ink#teaches-spell
                            l_RewardSpells = {
                                56950,  56994,  57012,  57034,  57114,  57126,
                                57157,  57172,  57219,  57220,  57235,  57246,
                                57252,  57253,  57276,  58308,  58316,  58340,
                                59339,  64259,  64273,  64283,  64307,  92579,
                                94000, 107907, 112463, 124456, 148273, 148292,
                                57153
                            };
                            break;
                        case SpellIDs::ResearchShimmeringInk:
                            /// http://www.wowhead.com/spell=165463/research-shimmering-ink#teaches-spell
                            l_RewardSpells = {
                                 57002,  57023,  57032,  57129,  57168,  57198,
                                 57210,  57236,  58320,  58325,  58333,  58336,
                                 58346,  95710, 112466, 119481, 126801, 131152,
                                148284, 148291, 148489,  57211,  57124,  56946,
                                 57267,  57232, 132167
                            };
                            break;
                        case SpellIDs::ResearchEtherealInk:
                            /// http://www.wowhead.com/spell=165464/research-ethereal-ink#teaches-spell
                            l_RewardSpells = {
                                 56978,  56990,  56991,  57005,  57033,  57125,
                                 57265,  57274,  58286,  58315,  58318,  58327,
                                 58330,  58343,  59340,  95215, 112465, 124452,
                                124455, 148276, 148288, 148290, 148487, 182154,
                                 57207,  57115,  57263,  57164,  57234,  57195
                            };
                            break;
                        case SpellIDs::ResearchInkOfTheSea:
                            /// http://www.wowhead.com/spell=165465/research-ink-of-the-sea#teaches-spell
                            l_RewardSpells = {
                                 56948,  56952,  56972,  56995,  57229, 57230,
                                 57270,  58311,  58317,  58328,  58329, 58342,
                                 94404, 112457, 112464, 112469, 124442, 126153,
                                148275, 148287, 148289, 182156,  57159, 57233,
                                 57019,  56988,  57130,  57260,  57193
                            };
                            break;
                        case SpellIDs::ResearchBlackfallowInk:
                            /// http://www.wowhead.com/spell=165466/research-blackfallow-ink#teaches-spell
                            l_RewardSpells = {
                                 57031,  57154,  57196,  57217,  57228,  57249,
                                 57257,  58287,  58296,  58324,  58337,  58339,
                                 64260,  64262,  68166, 112430, 112461, 112462,
                                122030, 124466, 126800, 148286,  56986,  55691
                            };
                            break;
                        case SpellIDs::ResearchInkOfDreams:
                            /// http://www.wowhead.com/spell=165467/research-ink-of-dreams#teaches-spell
                            l_RewardSpells = {
                                 56959,  56979,  56981,  57123,  57133,  57194,
                                 57226,  57227,  57269,  58288,  58301,  58306,
                                 59326,  64258,  64261,  64312, 112266, 112458,
                                112460, 124461, 126696, 148274, 148285, 182155,
                                182158,  57189,  56998,  58375
                            };
                            break;
                    }

                    /// Can have a reward (Guessed value)
                    if (l_SpellID != SpellIDs::ResearchWarbinderInkItem && !roll_chance_i(80))
                        return;

                    std::vector<uint32> l_Candidates;
                    for (uint32 l_SpellID : l_RewardSpells)
                    {
                        if (!l_Player->HasSpell(l_SpellID))
                            l_Candidates.push_back(l_SpellID);
                    }

                    if (!l_Candidates.size())
                    {
                        auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                        std::shuffle(l_RewardSpells.begin(), l_RewardSpells.end(), std::default_random_engine(l_Seed));

                        uint32 l_SpellID = l_RewardSpells.at(0);
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellID);

                        if (!l_SpellInfo || l_SpellInfo->Effects[EFFECT_0].Effect != SPELL_EFFECT_CREATE_ITEM)
                            return;

                        uint32 l_ItemID = l_SpellInfo->Effects[EFFECT_0].ItemType;

                        if (!sObjectMgr->GetItemTemplate(l_ItemID))
                            return;

                        l_Player->AddItem(l_ItemID, 1);
                        return;
                    }

                    auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                    std::shuffle(l_Candidates.begin(), l_Candidates.end(), std::default_random_engine(l_Seed));

                    l_Player->learnSpell(l_Candidates.at(0), false);
                }

                void Register() override
                {
                    AfterCast += SpellCastFn(spell_Inscription_Research_SpellScript::HandleAfterCast);
                }

            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript* GetSpellScript() const override
            {
                return new spell_Inscription_Research_SpellScript();
            }
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// Reset secondary properties spells
    //////////////////////////////////////////////////////////////////////////
    class spell_Skill_ResetSecondaryProperties : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_Skill_ResetSecondaryProperties()
                : SpellScriptLoader("spell_Skill_ResetSecondaryProperties")
            {
            }

            class spell_Skill_ResetSecondaryProperties_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_Skill_ResetSecondaryProperties_SpellScript);

                bool HaveSameItemSourceSkill(Item* p_Item1, Item* p_Item2) const
                {
                    if (p_Item1 == nullptr
                        || p_Item2 == nullptr)
                        return false;

                    auto l_SourceSkill1 = sSpellMgr->GetItemSourceSkills(p_Item1->GetEntry());
                    auto l_SourceSkill2 = sSpellMgr->GetItemSourceSkills(p_Item2->GetEntry());

                    if (l_SourceSkill1 == nullptr
                        || l_SourceSkill2 == nullptr)
                        return false;

                    for (auto l_Skill1 : *l_SourceSkill1)
                    {
                        for (auto l_Skill2 : *l_SourceSkill2)
                        {
                            if (l_Skill1 == l_Skill2)
                                return true;
                        }
                    }

                    return false;
                }

                bool HaveRequireIlevel(Item* p_ItemTarget)
                {
                    uint32 l_RequireIlevel = 0;

                    switch (GetSpellInfo()->Id)
                    {
                        case SpellIDs::InscribedCrystal:
                        case SpellIDs::DidiDelicateAssembly:
                        case SpellIDs::TruesteelGrinder:
                            l_RequireIlevel = 630;
                            break;
                        case SpellIDs::HexweaveEmbroidery:
                        case SpellIDs::TaladiteRecrystalizer:
                        case SpellIDs::TaladiteRecrystalizer2:
                        case SpellIDs::TaladiteRecrystalizer3:
                        case SpellIDs::TaladiteRecrystalizer4:
                        case SpellIDs::TaladiteRecrystalizer5:
                        case SpellIDs::TruesteelReshaper:
                        case SpellIDs::LeatherRefurbishingKit:
                            l_RequireIlevel = 640;
                            break;
                        default:
                            return false;
                    }

                    if ((p_ItemTarget->GetTemplate()->ItemLevel + p_ItemTarget->GetItemLevelBonusFromItemBonuses()) < l_RequireIlevel)
                        return false;

                    return true;
                }

                SpellCastResult CheckCast()
                {
                    if (GetCaster()->GetTypeId() != TypeID::TYPEID_PLAYER)
                        return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                    Item* l_ItemModifier = GetSpell()->m_CastItem;
                    Item* l_ItemTarget   = GetSpell()->m_targets.GetItemTarget();

                    if (!HaveSameItemSourceSkill(l_ItemModifier, l_ItemTarget))
                        return SpellCastResult::SPELL_FAILED_CANT_REPLACE_ITEM_BONUS;

                    if (!HaveRequireIlevel(l_ItemTarget))
                        return SpellCastResult::SPELL_FAILED_CANT_REPLACE_ITEM_BONUS;

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void ResetSecondaryProperties()
                {
                    Item* l_ItemModifier = GetSpell()->m_CastItem;
                    Item* l_ItemTarget   = GetSpell()->m_targets.GetItemTarget();

                    if (l_ItemModifier == nullptr
                        || l_ItemTarget == nullptr)
                        return;


                    /// Remove all old random bonus
                    for (auto l_ItemBonusGroupID : l_ItemTarget->GetTemplate()->m_ItemBonusGroups)
                    {
                        auto l_ItemBonusGroup = sObjectMgr->GetItemBonusGroup(l_ItemBonusGroupID);
                        if (l_ItemBonusGroup == nullptr)
                            continue;

                        for (auto l_Bonus : *l_ItemBonusGroup)
                            l_ItemTarget->RemoveItemBonus(l_Bonus);
                    }

                    /// Apply new random bonus
                    for (auto l_ItemBonusGroupID : l_ItemTarget->GetTemplate()->m_ItemBonusGroups)
                    {
                        auto l_ItemBonusGroup = sObjectMgr->GetItemBonusGroup(l_ItemBonusGroupID);
                        if (l_ItemBonusGroup == nullptr)
                            continue;

                        std::vector<uint32> l_BonusList(*l_ItemBonusGroup);

                        auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                        std::shuffle(l_BonusList.begin(), l_BonusList.end(), std::default_random_engine(l_Seed));

                        l_ItemTarget->AddItemBonus(l_BonusList.at(0));
                    }

                    l_ItemTarget->SetState(ITEM_CHANGED, GetCaster()->ToPlayer());
                }

                void Register() override
                {
                    OnHit       += SpellHitFn(spell_Skill_ResetSecondaryProperties_SpellScript::ResetSecondaryProperties);
                    OnCheckCast += SpellCheckCastFn(spell_Skill_ResetSecondaryProperties_SpellScript::CheckCast);
                }

            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript* GetSpellScript() const override
            {
                return new spell_Skill_ResetSecondaryProperties_SpellScript();
            }
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    namespace DailyMajorSkills
    {
        //////////////////////////////////////////////////////////////////////////
        /// GENERIC : Recipe who the created items grow with skill level
        //////////////////////////////////////////////////////////////////////////
        template<char const* t_Name, int t_SkillID, int t_ItemID, int t_BaseCount> class spell_Skill_GrowFromSkillLevel : public SpellScriptLoader
        {
            public:
                /// Constructor
                spell_Skill_GrowFromSkillLevel() : SpellScriptLoader(t_Name) { }

                class spell_Skill_GrowFromSkillLevel_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_Skill_GrowFromSkillLevel_SpellScript);

                    uint32 GetItemCount(Player* p_Player)
                    {
                        uint32 l_SkillValue = p_Player->GetSkillValue(t_SkillID);
                        uint32 l_RollCount = t_BaseCount;

                        if (l_SkillValue > 600)
                            l_RollCount += 1 + ((l_SkillValue - 600) / 20);

                        return l_RollCount;
                    }

                    SpellCastResult CheckCast()
                    {
                        Player* l_Caster = GetCaster() ? GetCaster()->ToPlayer() : nullptr;

                        if (!l_Caster)
                            return SPELL_FAILED_ERROR;

                        uint32 l_RollCount = GetItemCount(l_Caster);

                        ItemPosCountVec l_Destination;
                        InventoryResult l_Message = l_Caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, t_ItemID, l_RollCount);

                        if (l_Message != EQUIP_ERR_OK)
                        {
                            l_Caster->SendEquipError(EQUIP_ERR_INV_FULL, nullptr);
                            return SPELL_FAILED_DONT_REPORT;
                        }

                        return SPELL_CAST_OK;
                    }

                    void HandleCreateItem(SpellEffIndex p_EffIndex)
                    {
                        PreventHitDefaultEffect(p_EffIndex);

                        Player* l_Caster = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
                        if (!l_Caster)
                            return;

                        uint32 l_RollCount = GetItemCount(l_Caster);

                        ItemPosCountVec l_Destination;
                        InventoryResult l_Message = l_Caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, t_ItemID, l_RollCount);

                        if (l_Message != EQUIP_ERR_OK)
                            return;

                        l_Caster->UpdateCraftSkill(GetSpellInfo()->Id);

                        if (Item* l_Item = l_Caster->StoreNewItem(l_Destination, t_ItemID, true, Item::GenerateItemRandomPropertyId(t_ItemID)))
                            l_Caster->SendNewItem(l_Item, l_RollCount, false, true);

                        HandleAdditionalReagents(l_Caster);
                    }

                    void HandleAdditionalReagents(Player* p_Player)
                    {
                        uint32 l_ItemIDs[2] = { 0,              0           };
                        uint32 l_Counts[2]  = { urand(0, 5),    urand(0, 5) };

                        switch (t_ItemID)
                        {
                            /// May also create Sorcerous Air and Fire.
                            case ItemIDs::GearspringParts:
                                l_ItemIDs[0] = ItemIDs::SorcerousAir;
                                l_ItemIDs[1] = ItemIDs::SorcerousFire;
                                break;
                            /// May also create Sorcerous Fire and Water.
                            case ItemIDs::AlchemicalCatalyst:
                                l_ItemIDs[0] = ItemIDs::SorcerousWater;
                                l_ItemIDs[1] = ItemIDs::SorcerousFire;
                                break;
                            /// May also create Sorcerous Fire and Earth.
                            case ItemIDs::TruesteelIngot:
                                l_ItemIDs[0] = ItemIDs::SorcerousEarth;
                                l_ItemIDs[1] = ItemIDs::SorcerousFire;
                                break;
                            /// May also create Sorcerous Water and Earth.
                            case ItemIDs::WarPaints:
                            case ItemIDs::BurnishedLeather:
                                l_ItemIDs[0] = ItemIDs::SorcerousWater;
                                l_ItemIDs[1] = ItemIDs::SorcerousEarth;
                                break;
                            /// May also create Sorcerous Earth and Air.
                            case ItemIDs::TaladiteCrystal:
                                l_ItemIDs[0] = ItemIDs::SorcerousAir;
                                l_ItemIDs[1] = ItemIDs::SorcerousEarth;
                                break;
                            /// May also create Sorcerous Water and Air.
                            case ItemIDs::HexweaveCloth:
                                l_ItemIDs[0] = ItemIDs::SorcerousWater;
                                l_ItemIDs[1] = ItemIDs::SorcerousAir;
                                break;
                            default:
                                return;
                        }

                        for (uint8 l_I = 0; l_I < 2; ++l_I)
                        {
                            if (l_ItemIDs[l_I] && l_Counts[l_I])
                            {
                                ItemPosCountVec l_Destination;
                                InventoryResult l_Message = p_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Destination, l_ItemIDs[l_I], l_Counts[l_I]);

                                if (l_Message != InventoryResult::EQUIP_ERR_OK)
                                    return;

                                if (Item* l_Item = p_Player->StoreNewItem(l_Destination, l_ItemIDs[l_I], true, Item::GenerateItemRandomPropertyId(l_ItemIDs[l_I])))
                                    p_Player->SendNewItem(l_Item, l_Counts[l_I], false, true);
                            }
                        }
                    }

                    void Register() override
                    {
                        OnCheckCast += SpellCheckCastFn(spell_Skill_GrowFromSkillLevel_SpellScript::CheckCast);
                        OnEffectHitTarget += SpellEffectFn(spell_Skill_GrowFromSkillLevel_SpellScript::HandleCreateItem, EFFECT_0, SPELL_EFFECT_CREATE_ITEM_2);
                    }
                };

                /// Should return a fully valid SpellScript pointer.
                SpellScript* GetSpellScript() const override
                {
                    return new spell_Skill_GrowFromSkillLevel_SpellScript();
                }
        };

        //////////////////////////////////////////////////////////////////////////
        /// 171690 - Truesteel Ingot
        //////////////////////////////////////////////////////////////////////////
        char  spell_Skill_BlackSmithing_TruesteelIngot_Name[]       = "spell_Skill_BlackSmithing_TruesteelIngot";
        using spell_Skill_BlackSmithing_TruesteelIngot              = spell_Skill_GrowFromSkillLevel<spell_Skill_BlackSmithing_TruesteelIngot_Name, SKILL_BLACKSMITHING, ItemIDs::TruesteelIngot, 8>;

        //////////////////////////////////////////////////////////////////////////
        /// 169081 - War Paints
        //////////////////////////////////////////////////////////////////////////
        char  spell_Skill_Inscription_WarPaints_Name[]              = "spell_Skill_Inscription_WarPaints";
        using spell_Skill_Inscription_WarPaints                     = spell_Skill_GrowFromSkillLevel<spell_Skill_Inscription_WarPaints_Name, SKILL_INSCRIPTION, ItemIDs::WarPaints, 4>;

        //////////////////////////////////////////////////////////////////////////
        /// 168835 - Hexweave Cloth
        //////////////////////////////////////////////////////////////////////////
        char  spell_Skill_Tailoring_HexweaveCloth_Name[]            = "spell_Skill_Tailoring_HexweaveCloth";
        using spell_Skill_Tailoring_HexweaveCloth                   = spell_Skill_GrowFromSkillLevel<spell_Skill_Tailoring_HexweaveCloth_Name, SKILL_TAILORING, ItemIDs::HexweaveCloth, 8>;

        //////////////////////////////////////////////////////////////////////////
        /// 172539 - Antiseptic Bandage
        //////////////////////////////////////////////////////////////////////////
        char  spell_Skill_FirstAid_AntisepticBandage_Name[]         = "spell_Skill_FirstAid_AntisepticBandage";
        using spell_Skill_FirstAid_AntisepticBandage                = spell_Skill_GrowFromSkillLevel<spell_Skill_FirstAid_AntisepticBandage_Name, SKILL_FIRST_AID, ItemIDs::AntisepticBandage, 4>;

        //////////////////////////////////////////////////////////////////////////
        /// 171391 - Burnished Leather
        //////////////////////////////////////////////////////////////////////////
        char  spell_Skill_Leatherworking_BurnishedLeather_Name[]    = "spell_Skill_Leatherworking_BurnishedLeather";
        using spell_Skill_Leatherworking_BurnishedLeather           = spell_Skill_GrowFromSkillLevel<spell_Skill_Leatherworking_BurnishedLeather_Name, SKILL_LEATHERWORKING, ItemIDs::BurnishedLeather, 8>;

        //////////////////////////////////////////////////////////////////////////
        /// 170700 - Taladite Crystal
        //////////////////////////////////////////////////////////////////////////
        char  spell_Skill_Jewelcrafting_TaladiteCrystal_Name[]      = "spell_Skill_Jewelcrafting_TaladiteCrystal";
        using spell_Skill_Jewelcrafting_TaladiteCrystal             = spell_Skill_GrowFromSkillLevel<spell_Skill_Jewelcrafting_TaladiteCrystal_Name, SKILL_JEWELCRAFTING, ItemIDs::TaladiteCrystal, 8>;

        //////////////////////////////////////////////////////////////////////////
        /// 169080 - Gearspring Parts
        //////////////////////////////////////////////////////////////////////////
        char  spell_skill_engineering_gearspring_parts_name[]       = "spell_skill_engineering_gearspring_parts";
        using spell_skill_engineering_gearspring_parts              = spell_Skill_GrowFromSkillLevel<spell_skill_engineering_gearspring_parts_name, SKILL_ENGINEERING, ItemIDs::GearspringParts, 4>;

        //////////////////////////////////////////////////////////////////////////
        /// 156587 - Alchemical Catalyst
        //////////////////////////////////////////////////////////////////////////
        char  spell_skill_alchemy_alchemical_catalyst_name[]        = "spell_skill_alchemy_alchemical_catalyst";
        using spell_skill_alchemy_alchemical_catalyst               = spell_Skill_GrowFromSkillLevel<spell_skill_alchemy_alchemical_catalyst_name, SKILL_ALCHEMY, ItemIDs::AlchemicalCatalyst, 8>;

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        /// 169092 - Temporal Crystal
        //////////////////////////////////////////////////////////////////////////
        class spell_Skill_Enchantment_TemporalCrystal : public SpellScriptLoader
        {
            public:
                /// Constructor
                spell_Skill_Enchantment_TemporalCrystal() : SpellScriptLoader("spell_Skill_Enchantment_TemporalCrystal") { }

                class spell_Skill_Enchantment_TemporalCrystal_SpellScript : public SpellScript
                {
                    PrepareSpellScript(spell_Skill_Enchantment_TemporalCrystal_SpellScript);

                    uint32 m_Count;
                    uint32 m_ItemID;

                    bool Load() override
                    {
                        m_ItemID    = ItemIDs::FracturedTemporalCrystal;
                        m_Count     = urand(3, 6);
                        return true;
                    }

                    SpellCastResult CheckCast()
                    {
                        Player* l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
                        if (!l_Player)
                            return SpellCastResult::SPELL_FAILED_ERROR;

                        uint32 l_SkillValue = l_Player->GetSkillValue(SkillType::SKILL_ENCHANTING);

                        /// Low Enchanting skill may fracture the crystal when created.
                        if (l_SkillValue < 600)
                        {
                            int32 l_Chance = l_SkillValue * 100 / 600;
                            if (roll_chance_i(l_Chance))
                            {
                                m_Count     = 1;
                                m_ItemID    = ItemIDs::TemporalCrystal;
                            }
                        }
                        else
                        {
                            m_ItemID = ItemIDs::TemporalCrystal;
                            m_Count = 1;
                        }

                        ItemPosCountVec l_Destination;
                        InventoryResult l_Message = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Destination, m_ItemID, m_Count);

                        if (l_Message != InventoryResult::EQUIP_ERR_OK)
                        {
                            l_Player->SendEquipError(InventoryResult::EQUIP_ERR_INV_FULL, nullptr);
                            return SpellCastResult::SPELL_FAILED_DONT_REPORT;
                        }

                        return SpellCastResult::SPELL_CAST_OK;
                    }

                    void HandleCreateItem(SpellEffIndex p_EffIndex)
                    {
                        PreventHitDefaultEffect(p_EffIndex);

                        Player* l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
                        if (!l_Player)
                            return;

                        ItemPosCountVec l_Destination;
                        InventoryResult l_Message = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Destination, m_ItemID, m_Count);

                        if (l_Message != InventoryResult::EQUIP_ERR_OK)
                            return;

                        l_Player->UpdateCraftSkill(GetSpellInfo()->Id);

                        if (Item* l_Item = l_Player->StoreNewItem(l_Destination, m_ItemID, true, Item::GenerateItemRandomPropertyId(m_ItemID)))
                            l_Player->SendNewItem(l_Item, m_Count, false, true);

                        HandleAdditionalReagents(l_Player);
                    }

                    void HandleAdditionalReagents(Player* p_Player)
                    {
                        uint32 l_ItemIDs[2] = { ItemIDs::SorcerousAir,  ItemIDs::SorcerousFire  };
                        uint32 l_Counts[2]  = { urand(0, 5),            urand(0, 5)             };

                        for (uint8 l_I = 0; l_I < 2; ++l_I)
                        {
                            if (l_ItemIDs[l_I] && l_Counts[l_I])
                            {
                                ItemPosCountVec l_Destination;
                                InventoryResult l_Message = p_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Destination, l_ItemIDs[l_I], l_Counts[l_I]);

                                if (l_Message != InventoryResult::EQUIP_ERR_OK)
                                    return;

                                if (Item* l_Item = p_Player->StoreNewItem(l_Destination, l_ItemIDs[l_I], true, Item::GenerateItemRandomPropertyId(l_ItemIDs[l_I])))
                                    p_Player->SendNewItem(l_Item, l_Counts[l_I], false, true);
                            }
                        }
                    }

                    void Register() override
                    {
                        OnCheckCast += SpellCheckCastFn(spell_Skill_Enchantment_TemporalCrystal_SpellScript::CheckCast);
                        OnEffectHitTarget += SpellEffectFn(spell_Skill_Enchantment_TemporalCrystal_SpellScript::HandleCreateItem, EFFECT_0, SPELL_EFFECT_CREATE_ITEM_2);
                    }
                };

                /// Should return a fully valid SpellScript pointer.
                SpellScript* GetSpellScript() const override
                {
                    return new spell_Skill_Enchantment_TemporalCrystal_SpellScript();
                }
        };
    }   ///< namespace DailyMajorSkills

    /// Darkmoon Card of Draenor - 163294
    class spell_Skill_Darkmoon_Card_Of_Draenor : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_Skill_Darkmoon_Card_Of_Draenor() : SpellScriptLoader("spell_Skill_Darkmoon_Card_Of_Draenor") { }

            class spell_Skill_Darkmoon_Card_Of_Draenor_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_Skill_Darkmoon_Card_Of_Draenor_SpellScript);

                void HandleOnCast()
                {
                    std::vector<uint32> l_RewardCard = {
                        112278, 112277, 112276, 112274, 112275, 112273, 112272, 112271, 113135,
                        112302, 112301, 112300, 112299, 112298, 112297, 112296, 112295, 113142,
                        112286, 112285, 112284, 112283, 112282, 112281, 112280, 112279, 113139,
                        112294, 112293, 112292, 112291, 112290, 112289, 112288, 112287, 113140
                    };

                    auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                    std::shuffle(l_RewardCard.begin(), l_RewardCard.end(), std::default_random_engine(l_Seed));

                    if (Player* l_Caster = GetCaster()->ToPlayer())
                        l_Caster->AddItem(l_RewardCard.at(0), 1);
                }

                void Register()
                {
                    OnCast += SpellCastFn(spell_Skill_Darkmoon_Card_Of_Draenor_SpellScript::HandleOnCast);
                }

            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_Skill_Darkmoon_Card_Of_Draenor_SpellScript();
            }
    };

    /// Upgrade Armor - Taladite Amplifier - 187551
    class spell_skill_taladite_amplifier_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_taladite_amplifier_upgrade() : SpellScriptLoader("spell_skill_taladite_amplifier_upgrade") { }

            class spell_skill_taladite_amplifier_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_taladite_amplifier_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 173014,
                    SecondUpgrade   = 173023,
                    ThirdUpgrade    = 181410
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 670)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 655)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 640)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_taladite_amplifier_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_taladite_amplifier_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_taladite_amplifier_upgrade_SpellScript();
            }
    };

    /// Upgrade Armor - Burnished Essence - 187541
    class spell_skill_burnished_essence_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_burnished_essence_upgrade() : SpellScriptLoader("spell_skill_burnished_essence_upgrade") { }

            class spell_skill_burnished_essence_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_burnished_essence_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 171223,
                    SecondUpgrade   = 171224,
                    ThirdUpgrade    = 181406
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 670)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 655)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 640)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_burnished_essence_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_burnished_essence_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_burnished_essence_upgrade_SpellScript();
            }
    };

    /// Upgrade Weapon - Steelforged Essence - 187550
    class spell_skill_steelforged_essence_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_steelforged_essence_upgrade() : SpellScriptLoader("spell_skill_steelforged_essence_upgrade") { }

            class spell_skill_steelforged_essence_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_steelforged_essence_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 171652,
                    SecondUpgrade   = 171653,
                    ThirdUpgrade    = 181407
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 660)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 645)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 630)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_steelforged_essence_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_steelforged_essence_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_steelforged_essence_upgrade_SpellScript();
            }
    };

    /// Upgrade Armor - Truesteel Essence - 187546
    class spell_skill_truesteel_essence_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_truesteel_essence_upgrade() : SpellScriptLoader("spell_skill_truesteel_essence_upgrade") { }

            class spell_skill_truesteel_essence_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_truesteel_essence_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 171640,
                    SecondUpgrade   = 171649,
                    ThirdUpgrade    = 181408
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 670)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 655)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 640)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_truesteel_essence_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_truesteel_essence_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_truesteel_essence_upgrade_SpellScript();
            }
    };

    /// Upgrade Weapon - True Iron Trigger - 187552
    class spell_skill_true_iron_trigger_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_true_iron_trigger_upgrade() : SpellScriptLoader("spell_skill_true_iron_trigger_upgrade") { }

            class spell_skill_true_iron_trigger_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_true_iron_trigger_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 177361,
                    SecondUpgrade   = 177362,
                    ThirdUpgrade    = 181413
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 660)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 645)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 630)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_true_iron_trigger_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_true_iron_trigger_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_true_iron_trigger_upgrade_SpellScript();
            }
    };

    /// Upgrade Armor - Linkgrease Locksprocket - 187538
    class spell_skill_linkgrease_locksprocket_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_linkgrease_locksprocket_upgrade() : SpellScriptLoader("spell_skill_linkgrease_locksprocket_upgrade") { }

            class spell_skill_linkgrease_locksprocket_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_linkgrease_locksprocket_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 168095,
                    SecondUpgrade   = 168120,
                    ThirdUpgrade    = 181414
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 670)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 655)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 640)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_linkgrease_locksprocket_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_linkgrease_locksprocket_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_linkgrease_locksprocket_upgrade_SpellScript();
            }
    };

    /// Upgrade Weapon - Weapon Crystal - 187535
    class spell_skill_weapon_crystal_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_weapon_crystal_upgrade() : SpellScriptLoader("spell_skill_weapon_crystal_upgrade") { }

            class spell_skill_weapon_crystal_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_weapon_crystal_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 165808,
                    SecondUpgrade   = 178250,
                    ThirdUpgrade    = 181411
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 660)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 645)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 630)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_weapon_crystal_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_weapon_crystal_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_weapon_crystal_upgrade_SpellScript();
            }
    };

    /// Upgrade Armor - Hexweave Essence - 187539
    class spell_skill_hexweave_essence_upgrade : public SpellScriptLoader
    {
        public:
            spell_skill_hexweave_essence_upgrade() : SpellScriptLoader("spell_skill_hexweave_essence_upgrade") { }

            class spell_skill_hexweave_essence_upgrade_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_skill_hexweave_essence_upgrade_SpellScript);

                enum eUpgradeSpells
                {
                    FirstUpgrade    = 168865,
                    SecondUpgrade   = 168867,
                    ThirdUpgrade    = 181409
                };

                SpellCastResult CheckCast()
                {
                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            if (l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget) >= (uint32)GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                                return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                        }
                    }

                    return SpellCastResult::SPELL_CAST_OK;
                }

                void HandleChangeBonuses(SpellEffIndex p_EffIndex)
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    if (Item* l_ItemTarget = GetExplTargetItem())
                    {
                        if (Player* l_Player = GetCaster()->ToPlayer())
                        {
                            uint32 l_Triggered = 0;
                            uint32 l_ItemLevel = l_Player->GetEquipItemLevelFor(l_ItemTarget->GetTemplate(), l_ItemTarget);

                            if (l_ItemLevel >= 670)
                                l_Triggered = eUpgradeSpells::ThirdUpgrade;
                            else if (l_ItemLevel >= 655)
                                l_Triggered = eUpgradeSpells::SecondUpgrade;
                            else if (l_ItemLevel >= 640)
                                l_Triggered = eUpgradeSpells::FirstUpgrade;

                            if (l_Triggered)
                                l_Player->CastSpell(l_ItemTarget, l_Triggered, true);
                        }
                    }
                }

                void Register() override
                {
                    OnCheckCast += SpellCheckCastFn(spell_skill_hexweave_essence_upgrade_SpellScript::CheckCast);
                    OnEffectHitTarget += SpellEffectFn(spell_skill_hexweave_essence_upgrade_SpellScript::HandleChangeBonuses, EFFECT_0, SPELL_EFFECT_CHANGE_ITEM_BONUSES);
                }
            };

            SpellScript* GetSpellScript() const override
            {
                return new spell_skill_hexweave_essence_upgrade_SpellScript();
            }
    };

    /// Last Update 6.2.3
    class PlayerScript_skill_learn_spec : public PlayerScript
    {
    public:
        PlayerScript_skill_learn_spec() : PlayerScript("PlayerScript_skill_learn_spec") { }

        enum eSpells
        {
            Engineering_Gnomish_Engineer = 20219,
            Engineering_Goblin_Engineer = 20222,
            Alchimist_Potion_Master = 28675,
            Alchimist_Elixir_Master = 28677,
            Alchimist_Transmutation_Master = 28672
        };

        void OnSpellLearned(Player* p_Player, uint32 p_SpellId)
        {
            switch (p_SpellId)
            {
            case eSpells::Engineering_Gnomish_Engineer:
                p_Player->removeSpell(eSpells::Engineering_Goblin_Engineer);
                break;
            case eSpells::Engineering_Goblin_Engineer:
                p_Player->removeSpell(eSpells::Engineering_Gnomish_Engineer);
                break;
            case eSpells::Alchimist_Potion_Master:
                p_Player->removeSpell(eSpells::Alchimist_Elixir_Master);
                p_Player->removeSpell(eSpells::Alchimist_Transmutation_Master);
                break;
            case eSpells::Alchimist_Elixir_Master:
                p_Player->removeSpell(eSpells::Alchimist_Potion_Master);
                p_Player->removeSpell(eSpells::Alchimist_Transmutation_Master);
                break;
            case eSpells::Alchimist_Transmutation_Master:
                p_Player->removeSpell(eSpells::Alchimist_Elixir_Master);
                p_Player->removeSpell(eSpells::Alchimist_Potion_Master);
                break;
            }
        }

    };

}   ///< namespace Skill
}   ///< namespace MS

#ifndef __clang_analyzer__
void AddSC_spell_skill()
{
    new MS::Skill::spell_Cooking_DraenorRecipesRewards();
    new MS::Skill::spell_Inscription_Research();
    new MS::Skill::spell_Skill_ResetSecondaryProperties();
    new MS::Skill::spell_Skill_Darkmoon_Card_Of_Draenor();

    /// Daily major skills
    new MS::Skill::DailyMajorSkills::spell_Skill_BlackSmithing_TruesteelIngot();
    new MS::Skill::DailyMajorSkills::spell_Skill_Inscription_WarPaints();
    new MS::Skill::DailyMajorSkills::spell_Skill_Tailoring_HexweaveCloth();
    new MS::Skill::DailyMajorSkills::spell_Skill_FirstAid_AntisepticBandage();
    new MS::Skill::DailyMajorSkills::spell_Skill_Leatherworking_BurnishedLeather();
    new MS::Skill::DailyMajorSkills::spell_Skill_Jewelcrafting_TaladiteCrystal();
    new MS::Skill::DailyMajorSkills::spell_Skill_Enchantment_TemporalCrystal();
    new MS::Skill::DailyMajorSkills::spell_skill_engineering_gearspring_parts();
    new MS::Skill::DailyMajorSkills::spell_skill_alchemy_alchemical_catalyst();

    new MS::Skill::spell_skill_taladite_amplifier_upgrade();
    new MS::Skill::spell_skill_burnished_essence_upgrade();
    new MS::Skill::spell_skill_steelforged_essence_upgrade();
    new MS::Skill::spell_skill_truesteel_essence_upgrade();
    new MS::Skill::spell_skill_true_iron_trigger_upgrade();
    new MS::Skill::spell_skill_linkgrease_locksprocket_upgrade();
    new MS::Skill::spell_skill_weapon_crystal_upgrade();
    new MS::Skill::spell_skill_hexweave_essence_upgrade();

    new MS::Skill::PlayerScript_skill_learn_spec();
}
#endif
