////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ScriptedCreature.h"

#include "Containers.h"
#include "GridNotifiersImpl.h"

/// Prismatic Crystal - 76933
class spell_npc_mage_prismatic_crystal : public CreatureScript
{
    public:
        spell_npc_mage_prismatic_crystal() : CreatureScript("npc_mage_prismatic_crystal") { }

        struct spell_npc_mage_prismatic_crystalAI : public ScriptedAI
        {
            spell_npc_mage_prismatic_crystalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                PrismaticCrystalAura    = 155153,
                PrismaticCrystalDamage  = 155152
            };

            enum eDatas
            {
                FactionHostile = 14,
                FactionFriend  = 35
            };

            uint64 m_Owner = 0;

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Owner = p_Summoner->GetGUID();

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                if (Player* l_Player = p_Summoner->ToPlayer())
                {
                    if (AuraPtr l_Aura = me->AddAura(eSpells::PrismaticCrystalAura, me))
                    {
                        if (AuraEffectPtr l_DamageTaken = l_Aura->GetEffect(SpellEffIndex::EFFECT_0))
                        {
                            if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SpecIndex::SPEC_MAGE_FROST)
                                l_DamageTaken->ChangeAmount(10);    ///< BasePoint = 30, but only for Arcane and Fire spec
                        }
                    }
                }

                me->setFaction(eDatas::FactionHostile);
                me->ForceValuesUpdateAtIndex(EUnitFields::UNIT_FIELD_FACTION_TEMPLATE);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (p_Attacker->GetGUID() != m_Owner)
                {
                    p_Damage = 0;
                    return;
                }

                if (p_SpellInfo && p_SpellInfo->Id != eSpells::PrismaticCrystalDamage)
                {
                    if (Unit* l_Owner = sObjectAccessor->FindUnit(m_Owner))
                    {
                        int32 l_BasePoint = p_Damage;
                        l_Owner->CastCustomSpell(me, eSpells::PrismaticCrystalDamage, nullptr, &l_BasePoint, nullptr, true);
                    }
                }
            }

            void OnSendFactionTemplate(uint32& p_FactionID, Player* p_Target)
            {
                if (m_Owner == p_Target->GetGUID())
                    p_FactionID = eDatas::FactionHostile;
                else
                    p_FactionID = eDatas::FactionFriend;
            }

            void UpdateAI(uint32 const p_Diff) { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_mage_prismatic_crystalAI(p_Creature);
        }
};

/// Shadow Reflection - 77726
class spell_npc_rogue_shadow_reflection : public CreatureScript
{
    public:
        spell_npc_rogue_shadow_reflection() : CreatureScript("npc_rogue_shadow_reflection") { }

        struct spell_npc_rogue_shadow_reflectionAI : public ScriptedAI
        {
            spell_npc_rogue_shadow_reflectionAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Queuing(true) { }

            enum eSpells
            {
                SpellShadowReflectionAura = 156744,
                ShadowReflectionClone     = 168691
            };

            enum eDatas
            {
                AddSpellToQueue = 0,
                FinishFirstPhase
            };

            struct SpellData
            {
                SpellData(uint32 p_ID, uint32 p_Time)
                {
                    ID = p_ID;
                    Time = p_Time;
                }

                uint32 ID;
                uint32 Time;
            };

            bool m_Queuing;

            /// Automatically stored in good time order
            std::queue<SpellData> m_SpellQueue;

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->CastSpell(me, eSpells::SpellShadowReflectionAura, true);
            }

            void SetGUID(uint64 p_Data, int32 p_ID)
            {
                switch (p_ID)
                {
                    case eDatas::AddSpellToQueue:
                    {
                        if (!m_Queuing)
                            break;
                        uint32 l_SpellID = p_Data & 0xFFFFFFFF;
                        uint32 l_Time = p_Data >> 32;
                        m_SpellQueue.push(SpellData(l_SpellID, l_Time));
                        break;
                    }
                    case eDatas::FinishFirstPhase:
                    {
                        if (!m_Queuing || me->ToTempSummon() == nullptr)
                            break;

                        m_Queuing = false;
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                        if (Unit* l_Owner = me->ToTempSummon()->GetOwner())
                        {
                            Unit* l_OwnerTarget = nullptr;
                            if (Player* l_Player = l_Owner->ToPlayer())
                                l_OwnerTarget = l_Player->GetSelectedUnit();
                            else
                                l_OwnerTarget = l_Owner->getVictim();

                            if (l_OwnerTarget)
                                AttackStart(l_OwnerTarget);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::ShadowReflectionClone && p_Caster != nullptr && p_Caster->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    for (uint8 l_AttType = 0; l_AttType < WeaponAttackType::MaxAttack; ++l_AttType)
                    {
                        me->SetBaseWeaponDamage((WeaponAttackType)l_AttType, MAXDAMAGE, p_Caster->GetWeaponDamageRange((WeaponAttackType)l_AttType, MAXDAMAGE));
                        me->SetBaseWeaponDamage((WeaponAttackType)l_AttType, MINDAMAGE, p_Caster->GetWeaponDamageRange((WeaponAttackType)l_AttType, MINDAMAGE));
                    }

                    me->UpdateAttackPowerAndDamage();

                    if (Item* l_MH = p_Caster->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                    {
                        if (uint32 l_Transmo = l_MH->GetDynamicValue(ITEM_DYNAMIC_FIELD_MODIFIERS, 0))
                            me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, l_Transmo);
                        else
                            me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID, l_MH->GetTemplate()->DisplayInfoID);
                    }

                    if (Item* l_OH = p_Caster->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                    {
                        if (uint32 l_Transmo = l_OH->GetDynamicValue(ITEM_DYNAMIC_FIELD_MODIFIERS, 0))
                            me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID + 1, l_Transmo);
                        else
                            me->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEM_ID + 1, l_OH->GetTemplate()->DisplayInfoID);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_Queuing)
                    return;

                if (!UpdateVictim())
                    return;

                if (SpellData* l_SpellData = GetNextSpellData())
                {
                    if (l_SpellData->Time <= p_Diff)
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, l_SpellData->ID, true);

                        m_SpellQueue.pop();
                    }
                    else
                        l_SpellData->Time -= p_Diff;
                }

                DoMeleeAttackIfReady();
            }

            SpellData* GetNextSpellData()
            {
                if (m_SpellQueue.empty())
                    return nullptr;

                return &m_SpellQueue.front();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_rogue_shadow_reflectionAI(p_Creature);
        }
};

/// Storm Elemental - 77936
class spell_npc_sha_storm_elemental : public CreatureScript
{
public:
    spell_npc_sha_storm_elemental() : CreatureScript("npc_storm_elemental") { }

    struct spell_npc_sha_storm_elementalAI : public ScriptedAI
    {
        spell_npc_sha_storm_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eSpells
        {
            SpellWindGust       = 157333,
            SpellCallLightning  = 157348
        };

        enum eEvents
        {
            EventWindGust = 1,
            EventCallLightning
        };

        EventMap m_Events;

        void Reset()
        {
            m_Events.Reset();
        }

        void EnterCombat(Unit* p_Attacker)
        {
            m_Events.ScheduleEvent(eEvents::EventWindGust, 2000);
            m_Events.ScheduleEvent(eEvents::EventCallLightning, 8000);
        }

        void UpdateAI(uint32 const p_Diff)
        {
            if (!UpdateVictim())
            {
                if (Unit* l_Owner = me->GetOwner())
                {
                    Unit* l_OwnerTarget = nullptr;
                    if (Player* l_Player = l_Owner->ToPlayer())
                        l_OwnerTarget = l_Player->GetSelectedUnit();
                    else
                        l_OwnerTarget = l_Owner->getVictim();

                    if (l_OwnerTarget)
                        AttackStart(l_OwnerTarget);
                }

                return;
            }

            m_Events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (m_Events.ExecuteEvent())
            {
                case eEvents::EventWindGust:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSpells::SpellWindGust, false);
                    m_Events.ScheduleEvent(eEvents::EventWindGust, 9000);
                    break;
                case eEvents::EventCallLightning:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSpells::SpellCallLightning, false);
                    m_Events.ScheduleEvent(eEvents::EventCallLightning, 15000);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const
    {
        return new spell_npc_sha_storm_elementalAI(p_Creature);
    }
};

/// Ravager - 76168
class spell_npc_warr_ravager : public CreatureScript
{
    public:
        spell_npc_warr_ravager() : CreatureScript("npc_warrior_ravager") { }

        struct spell_npc_warr_ravagerAI : public ScriptedAI
        {
            spell_npc_warr_ravagerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                RavagerAura = 153709
            };

            void IsSummonedBy(Unit* p_Summoner)
            {
                me->CastSpell(me, eSpells::RavagerAura, true);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                if (p_Summoner == nullptr || p_Summoner->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (Player* l_Player = p_Summoner->ToPlayer())
                {
                    if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND))
                    {
                        /// Display Transmogrifications on player's clone
                        if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->GetDynamicValue(ItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS, 0)))
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID, l_Proto->ItemId);
                        else
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID, l_Item->GetTemplate()->ItemId);
                    }

                    if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND))
                    {
                        /// Display Transmogrifications on player's clone
                        if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->GetDynamicValue(ItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS, 0)))
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID + 1, l_Proto->ItemId);
                        else
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID + 1, l_Item->GetTemplate()->ItemId);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warr_ravagerAI(p_Creature);
        }
};

void AddSC_npc_spell_scripts()
{
    /// Mage NPC
    new spell_npc_mage_prismatic_crystal();

    /// Rogue NPC
    new spell_npc_rogue_shadow_reflection();

    /// Shaman NPC
    new spell_npc_sha_storm_elemental();

    /// Warrior NPC
    new spell_npc_warr_ravager();
}
