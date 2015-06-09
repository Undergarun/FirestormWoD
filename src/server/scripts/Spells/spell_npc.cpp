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
#include "CellImpl.h"

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

class spell_npc_sha_capacitor_totem : public CreatureScript
{
    public:
        spell_npc_sha_capacitor_totem() : CreatureScript("npc_capacitor_totem") { }

        struct spell_npc_sha_capacitor_totemAI : public ScriptedAI
        {
            enum eSpells
            {
                StaticCharge = 118905
            };

            bool m_HasBeenCasted;

            spell_npc_sha_capacitor_totemAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_HasBeenCasted = false;
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (!me->HasAura(eSpells::StaticCharge) && !m_HasBeenCasted)
                {
                    me->CastSpell(me, eSpells::StaticCharge, false);
                    m_HasBeenCasted = true;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_capacitor_totemAI(p_Creature);
        }
};

class spell_npc_sha_spirit_link_totem : public CreatureScript
{
    public:
        spell_npc_sha_spirit_link_totem() : CreatureScript("npc_spirit_link_totem") { }

        struct spell_npc_sha_spirit_link_totemAI : public ScriptedAI
        {
            uint32 CastTimer;

            spell_npc_sha_spirit_link_totemAI(Creature* creature) : ScriptedAI(creature)
            {
                CastTimer = 1000;

                if (creature->GetOwner() && creature->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                {
                    if (creature->GetEntry() == 53006)
                    {
                        creature->CastSpell(creature, 98007, false);
                        creature->CastSpell(creature, 98017, true);
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (CastTimer >= diff)
                {
                    if (me->GetOwner() && me->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                    {
                        if (me->GetEntry() == 53006)
                        {
                            me->CastSpell(me, 98007, false);
                            me->CastSpell(me, 98017, true);
                        }
                    }
                }

                CastTimer = 0;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_sha_spirit_link_totemAI(creature);
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
                SpellWindGust       = 157331,
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

class spell_npc_sha_fire_elemental : public CreatureScript
{
    public:
        spell_npc_sha_fire_elemental() : CreatureScript("npc_fire_elemental") { }

        struct spell_npc_sha_fire_elementalAI : public ScriptedAI
        {
            spell_npc_sha_fire_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) {}

            enum fireEvents
            {
                EVENT_FIRE_NOVA     = 1,
                EVENT_FIRE_BLAST    = 2,
                EVENT_FIRE_SHIELD   = 3
            };

            enum fireSpells
            {
                SPELL_SHAMAN_FIRE_BLAST     = 57984,
                SPELL_SHAMAN_FIRE_NOVA      = 12470,
                SPELL_SHAMAN_FIRE_SHIELD    = 13376
            };

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_FIRE_NOVA, urand(5000, 20000));
                events.ScheduleEvent(EVENT_FIRE_BLAST, urand(5000, 20000));
                events.ScheduleEvent(EVENT_FIRE_SHIELD, 0);
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (Unit* owner = me->GetOwner())
                    {
                        Unit* ownerTarget = NULL;
                        if (Player* plr = owner->ToPlayer())
                            ownerTarget = plr->GetSelectedUnit();
                        else
                            ownerTarget = owner->getVictim();

                        if (ownerTarget)
                            AttackStart(ownerTarget);
                    }

                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FIRE_NOVA:
                        DoCastVictim(SPELL_SHAMAN_FIRE_NOVA);
                        events.ScheduleEvent(EVENT_FIRE_NOVA, urand(5000, 20000));
                        break;
                    case EVENT_FIRE_BLAST:
                        DoCastVictim(SPELL_SHAMAN_FIRE_BLAST);
                        events.ScheduleEvent(EVENT_FIRE_BLAST, urand(5000, 20000));
                        break;
                    case EVENT_FIRE_SHIELD:
                        DoCastVictim(SPELL_SHAMAN_FIRE_SHIELD);
                        events.ScheduleEvent(EVENT_FIRE_SHIELD, 4000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI *GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_fire_elementalAI(p_Creature);
        }
};


class spell_npc_sha_earth_elemental : public CreatureScript
{
    public:
        spell_npc_sha_earth_elemental() : CreatureScript("npc_earth_elemental") { }

        struct spell_npc_sha_earth_elementalAI : public ScriptedAI
        {
            spell_npc_sha_earth_elementalAI(Creature* p_Creature) : ScriptedAI(p_Creature) {}

            enum eSpells
            {
                AngeredEarth = 36213
            };

            uint32 AngeredEarth_Timer;

            void Reset()
            {
                AngeredEarth_Timer = 0;
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (Unit* owner = me->GetOwner())
                    {
                        Unit* ownerTarget = NULL;
                        if (Player* plr = owner->ToPlayer())
                            ownerTarget = plr->GetSelectedUnit();
                        else
                            ownerTarget = owner->getVictim();

                        if (ownerTarget)
                            AttackStart(ownerTarget);
                    }

                    return;
                }

                if (AngeredEarth_Timer <= diff)
                {
                    DoCast(me->getVictim(), eSpells::AngeredEarth);
                    AngeredEarth_Timer = 5000 + rand() % 15000; // 5-20 sec cd
                }
                else
                    AngeredEarth_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_earth_elementalAI(p_Creature);
        }
};


class spell_npc_sha_feral_spirit : public CreatureScript
{
    public:
        spell_npc_sha_feral_spirit() : CreatureScript("npc_feral_spirit") { }

        enum eSpells
        {
            SpiritLeap           = 58867,
            SpiritWalk           = 58875,
            SpiritHunt           = 58877,
            GlyphOfSpiritRaptors = 147783,
            RaptorTranform       = 147908
        };

        struct spell_npc_sha_feral_spiritAI : public ScriptedAI
        {
            spell_npc_sha_feral_spiritAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                Unit* l_Owner = me->GetOwner();
                if (l_Owner && l_Owner->HasAura(eSpells::GlyphOfSpiritRaptors))
                    me->CastSpell(me, eSpells::RaptorTranform, true);

                me->CastSpell(me, eSpells::SpiritWalk, true);
                me->CastSpell(me, eSpells::SpiritHunt, true);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                me->CastSpell(p_Attacker, eSpells::SpiritLeap, true);
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

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_sha_feral_spiritAI(p_Creature);
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

class spell_npc_warl_wild_imp : public CreatureScript
{
    public:
        spell_npc_warl_wild_imp() : CreatureScript("npc_wild_imp") { }

        enum eSpells
        {
            Firebolt = 104318,
            MoltenCore = 122351,
            MoltenCoreAura = 122355
        };

        struct spell_npc_warl_wild_impAI : public ScriptedAI
        {
            uint32 m_Charges;

            spell_npc_warl_wild_impAI(Creature *creature) : ScriptedAI(creature)
            {
                m_Charges = 10;
                me->SetReactState(REACT_HELPER);
            }

            void Reset()
            {
                me->SetReactState(REACT_HELPER);

                if (me->GetOwner())
                    if (me->GetOwner()->getVictim())
                        AttackStart(me->GetOwner()->getVictim());
            }

            void UpdateAI(const uint32 p_Diff)
            {
                Unit* l_Owner = me->GetOwner();
                if (!l_Owner)
                    return;

                if (!UpdateVictim())
                {
                    Unit* l_OwnerTarget = nullptr;
                    if (Player* l_Player = l_Owner->ToPlayer())
                        l_OwnerTarget = l_Player->GetSelectedUnit();
                    else
                        l_OwnerTarget = l_Owner->getVictim();

                    if (l_OwnerTarget)
                        AttackStart(l_OwnerTarget);

                    return;
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (m_Charges == 0)
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                me->CastSpell(me->getVictim(), eSpells::Firebolt, false);
                m_Charges--;

                l_Owner->EnergizeBySpell(l_Owner, eSpells::Firebolt, 5 * l_Owner->GetPowerCoeff(POWER_DEMONIC_FURY), POWER_DEMONIC_FURY);

                if (AuraEffectPtr l_MoltenCore = l_Owner->GetAuraEffect(eSpells::MoltenCore, EFFECT_0))
                    if (roll_chance_i(l_MoltenCore->GetAmount()))
                        l_Owner->CastSpell(l_Owner, eSpells::MoltenCoreAura, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new spell_npc_warl_wild_impAI(creature);
        }
};

enum eGatewaySpells
{
    PortalVisual = 113900,
    GatewayInteract = 113902,
    CooldownMarker = 113942
};

enum eGatewayNpc
{
    GreenGate = 59262,
    PurpleGate = 59271
};

/// npc_demonic_gateway_purple - 59271
class spell_npc_warl_demonic_gateway_purple : public CreatureScript
{
    public:
        spell_npc_warl_demonic_gateway_purple() : CreatureScript("npc_demonic_gateway_purple") { }

        struct spell_npc_warl_demonic_gateway_purpleAI : public ScriptedAI
        {
            spell_npc_warl_demonic_gateway_purpleAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, eGatewaySpells::PortalVisual, true);

                me->SetFlag(UNIT_FIELD_INTERACT_SPELL_ID, eGatewaySpells::GatewayInteract);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void OnSpellClick(Unit* p_Clicker)
            {
                if (p_Clicker->GetTypeId() != TYPEID_PLAYER)
                    return;

                // Demonic Gateway cooldown marker
                if (p_Clicker->HasAura(eGatewaySpells::CooldownMarker))
                    return;

                Unit* l_Owner = me->GetOwner();
                if (!l_Owner || !l_Owner->ToPlayer())
                    return;

                if (Group* l_Group = p_Clicker->ToPlayer()->GetGroup())
                {
                    if (l_Owner->ToPlayer()->GetGroup() != l_Group)
                        return;
                }
                else if (l_Owner != p_Clicker)
                    return;

                std::list<Creature*> l_GreenGates;
                me->GetCreatureListWithEntryInGrid(l_GreenGates, eGatewayNpc::GreenGate, 75.0f);

                if (l_GreenGates.empty())
                    return;

                l_GreenGates.sort(JadeCore::DistanceCompareOrderPred(me));
                for (auto itr : l_GreenGates)
                {
                    p_Clicker->CastSpell(p_Clicker, eGatewaySpells::CooldownMarker, true);

                    // Init dest coordinates
                    float x, y, z;
                    itr->GetPosition(x, y, z);

                    float speedXY;
                    float speedZ = 5;

                    speedXY = p_Clicker->GetExactDist2d(x, y) * 10.0f / speedZ;
                    p_Clicker->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warl_demonic_gateway_purpleAI(p_Creature);
        }
};

/// npc_demonic_gateway_green - 59262
class spell_npc_warl_demonic_gateway_green : public CreatureScript
{
    public:
        spell_npc_warl_demonic_gateway_green() : CreatureScript("npc_demonic_gateway_green") { }

        struct spell_npc_warl_demonic_gateway_greenAI : public ScriptedAI
        {
            spell_npc_warl_demonic_gateway_greenAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, eGatewaySpells::PortalVisual, true);

                me->SetFlag(UNIT_FIELD_INTERACT_SPELL_ID, eGatewaySpells::GatewayInteract);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void OnSpellClick(Unit* p_Clicker)
            {
                if (p_Clicker->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_Clicker->HasAura(eGatewaySpells::CooldownMarker))
                    return;

                Unit* l_Owner = me->GetOwner();
                if (!l_Owner || !l_Owner->ToPlayer())
                    return;

                if (Group* l_Group = p_Clicker->ToPlayer()->GetGroup())
                {
                    if (l_Owner->ToPlayer()->GetGroup() != l_Group)
                        return;
                }
                else if (l_Owner != p_Clicker)
                    return;

                std::list<Creature*> l_PurpleGates;
                me->GetCreatureListWithEntryInGrid(l_PurpleGates, eGatewayNpc::PurpleGate, 75.0f);

                if (l_PurpleGates.empty())
                    return;

                l_PurpleGates.sort(JadeCore::DistanceCompareOrderPred(me));
                for (auto itr : l_PurpleGates)
                {
                    p_Clicker->CastSpell(p_Clicker, eGatewaySpells::CooldownMarker, true);

                    // Init dest coordinates
                    float x, y, z;
                    itr->GetPosition(x, y, z);

                    float speedXY;
                    float speedZ = 5;

                    speedXY = p_Clicker->GetExactDist2d(x, y) * 10.0f / speedZ;
                    p_Clicker->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new spell_npc_warl_demonic_gateway_greenAI(p_Creature);
        }
};

void AddSC_npc_spell_scripts()
{
    /// Mage NPC
    new spell_npc_mage_prismatic_crystal();

    /// Rogue NPC
    new spell_npc_rogue_shadow_reflection();

    /// Shaman NPC
    new spell_npc_sha_capacitor_totem();
    new spell_npc_sha_spirit_link_totem();
    new spell_npc_sha_storm_elemental();
    new spell_npc_sha_fire_elemental();
    new spell_npc_sha_earth_elemental();
    new spell_npc_sha_feral_spirit();

    /// Warrior NPC
    new spell_npc_warr_ravager();

    /// Warlock NPC
    new spell_npc_warl_wild_imp();
    new spell_npc_warl_demonic_gateway_purple();
    new spell_npc_warl_demonic_gateway_green();
}
