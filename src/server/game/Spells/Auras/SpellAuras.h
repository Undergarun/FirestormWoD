////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_SPELLAURAS_H
#define TRINITY_SPELLAURAS_H

#include "SpellAuraDefines.h"
#include "SpellInfo.h"
#include "Unit.h"

class Unit;
class SpellInfo;
struct SpellModifier;
struct ProcTriggerSpell;
struct SpellProcEntry;

// forward decl
class AuraEffect;
class Aura;
class DynamicObject;
class AuraScript;
class ProcInfo;

// update aura target map every 500 ms instead of every update - reduce amount of grid searcher calls
#define UPDATE_TARGET_MAP_INTERVAL 500

class AuraApplication
{
    friend void Unit::_ApplyAura(AuraApplication * aurApp, uint32 effMask);
    friend void Unit::_UnapplyAura(AuraApplicationMap::iterator &i, AuraRemoveMode removeMode);
    friend void Unit::_ApplyAuraEffect(Aura* aura, uint32 effIndex);
    friend void Unit::RemoveAura(AuraApplication * aurApp, AuraRemoveMode mode);
    friend AuraApplication * Unit::_CreateAuraApplication(Aura* aura, uint32 effMask);
    private:
        Unit* const _target;
        Aura const* _base;
        AuraRemoveMode _removeMode:8;                  // Store info for know remove aura reason
        uint8 m_Slot;                                  // Aura slot on unit
        uint8 _flags;                                  // Aura info flag
        uint32 _effectMask;
        uint32 _effectsToApply;                        // Used only at spell hit to determine which effect should be applied
        bool _needClientUpdate:1;
        uint8 m_EffectCount;

        explicit AuraApplication(Unit* target, Unit* caster, Aura* base, uint32 effMask);
        void _Remove();
    private:
        void _InitFlags(Unit* caster, uint32 effMask);
        void _HandleEffect(uint8 effIndex, bool apply);
    public:
        Unit* GetTarget() const { return _target; }
        Aura* GetBase() const { return(Aura*) _base; }

        uint8 GetSlot() const { return m_Slot; }
        uint8 GetFlags() const { return _flags; }
        uint32 GetEffectMask() const { return _effectMask; }
        bool HasEffect(uint8 effect) const { ASSERT(effect < SpellEffIndex::MAX_EFFECTS);  return _effectMask & (1 << effect); }
        uint8 GetEffectCount() const { return m_EffectCount; }
        bool IsPositive() const { return _flags & AFLAG_POSITIVE; }
        bool IsSelfcasted() const { return _flags & AFLAG_CASTER; }
        uint32 GetEffectsToApply() const { return _effectsToApply; }

        void SetRemoveMode(AuraRemoveMode mode) { _removeMode = mode; }
        AuraRemoveMode GetRemoveMode() const {return _removeMode;}

        void SetNeedClientUpdate() { _needClientUpdate = true;}
        bool IsNeedClientUpdate() const { return _needClientUpdate;}
        void BuildUpdatePacket(ByteBuffer& data, bool remove, uint32 overrideSpell = 0) const;
        void ClientUpdate(bool remove = false);
        void SendFakeAuraUpdate(uint32 auraId, bool remove);
};

class Aura
{
    friend Aura* Unit::_TryStackingOrRefreshingExistingAura(SpellInfo const* newAura, uint32 effMask, Unit* caster, int32 *baseAmount, Item* castItem, uint64 casterGUID, int32 castItemLevel);
    public:
        typedef std::map<uint64, AuraApplication *> ApplicationMap;

        static uint32 BuildEffectMaskForOwner(SpellInfo const* spellProto, uint32 avalibleEffectMask, WorldObject* owner);
        static Aura* TryRefreshStackOrCreate(SpellInfo const* spellproto, uint32 tryEffMask, WorldObject* owner, Unit* caster, int32* baseAmount = NULL, Item* castItem = NULL, uint64 casterGUID = 0, bool* refresh = NULL, int32 castItemLevel = -1);
        static Aura* TryCreate(SpellInfo const* spellproto, uint32 effMask, WorldObject* owner, Unit* caster, int32 *baseAmount = NULL, Item* castItem = NULL, uint64 casterGUID = 0, int32 castItemLevel = -1);
        static Aura* Create(SpellInfo const* spellproto, uint32 effMask, WorldObject* owner, Unit* caster, int32* baseAmount, Item* castItem, uint64 casterGUID, int32 castItemLevel);
        explicit Aura(SpellInfo const* spellproto, WorldObject* owner, Unit* caster, Item* castItem, uint64 casterGUID, int32 castItemLevel);
        void _InitEffects(uint32 effMask, Unit* caster, int32 *baseAmount);
        virtual ~Aura();

        SpellInfo const* GetSpellInfo() const { return m_spellInfo; }
        uint32 GetId() const { return m_spellInfo->Id; }

        void CopyAura(Unit* p_AuraCaster, Unit* p_Caster);

        void FillMechanicAndControlTypes(Mechanics& p_Mechanics, LossOfControlType& p_Type, SpellEffIndex& p_EffIndex);

        uint64 GetCastItemGUID() const { return m_castItemGuid; }
        int32 GetCastItemLevel() const { return m_castItemLevel; }
        uint64 GetCasterGUID() const { return m_casterGuid; }
        Unit* GetCaster() const;
        WorldObject* GetOwner() const { return m_owner; }
        Unit* GetUnitOwner() const { return (GetType() == UNIT_AURA_TYPE) ? (Unit*)m_owner : nullptr; }
        DynamicObject* GetDynobjOwner() const { return (GetType() == DYNOBJ_AURA_TYPE) ? (DynamicObject*)m_owner : nullptr; }

        AuraObjectType GetType() const;

        virtual void _ApplyForTarget(Unit* target, Unit* caster, AuraApplication * auraApp);
        virtual void _UnapplyForTarget(Unit* target, Unit* caster, AuraApplication * auraApp);
        void _Remove(AuraRemoveMode removeMode);
        virtual void Remove(AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT) = 0;

        virtual void FillTargetMap(std::map<Unit*, uint32> & targets, Unit* caster) = 0;
        void UpdateTargetMap(Unit* caster, bool apply = true);

        void _RegisterForTargets() {Unit* caster = GetCaster(); UpdateTargetMap(caster, false);}
        void ApplyForTargets() {Unit* caster = GetCaster(); UpdateTargetMap(caster, true);}
        void _ApplyEffectForTargets(uint8 effIndex);

        void UpdateOwner(uint32 diff, WorldObject* owner);
        void Update(uint32 diff, Unit* caster);

        time_t GetApplyTime() const { return m_applyTime; }
        int32 GetMaxDuration() const { return m_maxDuration; }
        void SetMaxDuration(int32 duration) { m_maxDuration = duration; }
        int32 CalcMaxDuration() const { return CalcMaxDuration(GetCaster()); }
        int32 CalcMaxDuration(Unit* caster) const;
        int32 GetDuration() const { return m_duration; }
        void SetDuration(int32 duration, bool withMods = false);
        void SetAuraTimer(int32 newTime, uint64 guid = 0);
        void RefreshDuration(bool withMods = false);
        void RefreshTimers();
        bool IsExpired() const { return !GetDuration();}
        bool IsPermanent() const { return GetMaxDuration() == -1; }

        uint8 GetCharges() const { return m_procCharges; }
        void SetCharges(uint8 charges);
        void SetModCharges(int16 charges);
        uint8 CalcMaxCharges(Unit* caster) const;
        uint8 CalcMaxCharges() const { return CalcMaxCharges(GetCaster()); }
        bool ModCharges(int32 num, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        bool DropCharge(AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT) { return ModCharges(-1, removeMode); }

        uint8 GetStackAmount() const { return m_stackAmount; }
        void SetStackAmount(uint8 num);
        bool ModStackAmount(int32 num, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        bool DropStack(AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT) { return ModStackAmount(-1, removeMode); }

        void RefreshSpellMods();

        uint16 GetCasterLevel() const { return m_casterLevel; }

        bool IsArea() const;
        bool IsPassive() const;
        bool IsDeathPersistent() const;
        bool IsRemovedOnShapeLost(Unit* target) const { return (GetCasterGUID() == target->GetGUID() && m_spellInfo->Stances && !(m_spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_NEED_SHAPESHIFT) && !(m_spellInfo->Attributes & SPELL_ATTR0_NOT_SHAPESHIFT)); }
        bool CanBeSaved() const;
        bool IsRemoved() const { return m_isRemoved; }
        bool CanBeSentToClient() const;
        // Single cast aura helpers
        bool IsSingleTarget() const {return m_isSingleTarget;}
        void SetIsSingleTarget(bool val) { m_isSingleTarget = val;}
        void UnregisterSingleTarget();
        int32 CalcDispelChance(Unit* auraTarget, bool offensive) const;

        void SetLoadedState(int32 maxduration, int32 duration, int32 charges, uint8 stackamount, uint32 recalculateMask, int32 * amount);

        // helpers for aura effects
        bool HasEffect(uint8 effIndex) const { return GetEffect(effIndex) != nullptr; }
        bool HasEffectType(AuraType type) const;
        uint8 GetEffectIndexByType(AuraType type) const;
        AuraEffect* GetEffect(uint8 effIndex) const { ASSERT(effIndex < SpellEffIndex::MAX_EFFECTS); return m_effects[effIndex]; }
        uint8 GetEffectCount() const { return m_EffectCount; }
        uint32 GetEffectMask() const { uint32 effMask = 0; for (uint8 i = 0; i < SpellEffIndex::MAX_EFFECTS; ++i) if (m_effects[i]) effMask |= 1 << i; return effMask; }
        void RecalculateAmountOfEffects(bool p_Force = false);
        void HandleAllEffects(AuraApplication * aurApp, uint8 mode, bool apply);

        // Helpers for targets
        ApplicationMap const & GetApplicationMap() {return m_applications;}
        void GetApplicationList(std::list<AuraApplication*> & applicationList) const;
        const AuraApplication * GetApplicationOfTarget (uint64 guid) const { ApplicationMap::const_iterator itr = m_applications.find(guid); if (itr != m_applications.end()) return itr->second; return NULL; }
        AuraApplication * GetApplicationOfTarget (uint64 guid) { ApplicationMap::iterator itr = m_applications.find(guid); if (itr != m_applications.end()) return itr->second; return NULL; }
        bool IsAppliedOnTarget(uint64 guid) const { return m_applications.find(guid) != m_applications.end(); }

        void SetNeedClientUpdateForTargets() const;
        void HandleAuraSpecificMods(AuraApplication const* aurApp, Unit* caster, bool apply, bool onReapply);
        void HandleAuraSpecificPeriodics(AuraApplication const* aurApp, Unit* caster);
        bool CanBeAppliedOn(Unit* target);
        bool CheckAreaTarget(Unit* target);
        bool CanStackWith(Aura const* existingAura) const;
        bool IsSameRaidBuff(Aura const* existingAura) const;

        // Proc system
        // this subsystem is not yet in use - the core of it is functional, but still some research has to be done
        // and some dependant problems fixed before it can replace old proc system (for example cooldown handling)
        // currently proc system functionality is implemented in Unit::ProcDamageAndSpell
        bool IsUsingCharges() const { return m_isUsingCharges; }
        void SetUsingCharges(bool val) { m_isUsingCharges = val; }
        void PrepareProcToTrigger(AuraApplication* aurApp, ProcEventInfo& eventInfo);
        bool IsProcTriggeredOnEvent(AuraApplication* aurApp, ProcEventInfo& eventInfo) const;
        float CalcProcChance(SpellProcEntry const& procEntry, ProcEventInfo& eventInfo) const;
        void TriggerProcOnEvent(AuraApplication* aurApp, ProcEventInfo& eventInfo);
        SpellPowerEntry const* GetSpellPowerData() const { return m_spellPowerData; }
        float CalcPPMProcChance(float procsPerMinute, Unit* actor) const;
        void SetLastProcAttemptTime(uint32 lastProcAttemptTime) { m_lastProcAttemptTime = lastProcAttemptTime; }
        void SetLastProcSuccessTime(uint32 lastProcSuccessTime) { m_lastProcSuccessTime = lastProcSuccessTime; }

        void Delink();

        // AuraScript
        void LoadScripts();
        bool CallScriptCheckAreaTargetHandlers(Unit* target);
        void CallScriptDispel(DispelInfo* dispelInfo);
        void CallScriptAfterDispel(DispelInfo* dispelInfo);
        bool CallScriptEffectApplyHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes mode);
        bool CallScriptEffectRemoveHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes mode);
        void CallScriptAfterEffectApplyHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes mode);
        void CallScriptAfterEffectRemoveHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp, AuraEffectHandleModes mode);
        bool CallScriptEffectPeriodicHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp);
        void CallScriptAfterEffectPeriodicHandlers(AuraEffect const* p_AurEff, AuraApplication const* p_AurApp);
        void CallScriptAuraUpdateHandlers(uint32 diff);
        void CallScriptEffectUpdateHandlers(uint32 diff, AuraEffect* aurEff);
        void CallScriptEffectUpdatePeriodicHandlers(AuraEffect* aurEff);
        void CallScriptEffectCalcAmountHandlers(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated);
        void CallScriptEffectCalcPeriodicHandlers(AuraEffect const* aurEff, bool & isPeriodic, int32 & amplitude);
        void CallScriptEffectCalcSpellModHandlers(AuraEffect const* aurEff, SpellModifier* & spellMod);
        void CallScriptEffectAbsorbHandlers(AuraEffect* aurEff, AuraApplication const* aurApp, DamageInfo & dmgInfo, uint32 & absorbAmount, bool & defaultPrevented);
        void CallScriptEffectAfterAbsorbHandlers(AuraEffect* aurEff, AuraApplication const* aurApp, DamageInfo & dmgInfo, uint32 & absorbAmount);
        void CallScriptEffectManaShieldHandlers(AuraEffect* aurEff, AuraApplication const* aurApp, DamageInfo & dmgInfo, uint32 & absorbAmount, bool & defaultPrevented);
        void CallScriptEffectAfterManaShieldHandlers(AuraEffect* aurEff, AuraApplication const* aurApp, DamageInfo & dmgInfo, uint32 & absorbAmount);
        void SetScriptData(uint32 type, uint32 data);
        void SetScriptGuid(uint32 type, uint64 data);
        // Spell Proc Hooks
        bool CallScriptCheckProcHandlers(AuraApplication const* aurApp, ProcEventInfo& eventInfo);
        bool CallScriptPrepareProcHandlers(AuraApplication const* aurApp, ProcEventInfo& eventInfo);
        void CallScriptProcHandlers(AuraApplication const* aurApp, ProcEventInfo& eventInfo);
        void CallScriptAfterProcHandlers(AuraApplication const* aurApp, ProcEventInfo& eventInfo);
        bool CallScriptEffectProcHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp, ProcEventInfo& eventInfo);
        void CallScriptAfterEffectProcHandlers(AuraEffect const* aurEff, AuraApplication const* aurApp, ProcEventInfo& eventInfo);
        bool CallScriptCanRrefreshProcHandlers();

        std::list<AuraScript*> m_loadedScripts;
    private:
        void _DeleteRemovedApplications();
    protected:
        SpellInfo const* const m_spellInfo;
        uint64 const m_casterGuid;
        uint64 const m_castItemGuid;                        // it is NOT safe to keep a pointer to the item because it may get deleted
        int32 m_castItemLevel;
        time_t const m_applyTime;
        WorldObject* const m_owner;                        //

        int32 m_maxDuration;                                // Max aura duration
        int32 m_duration;                                   // Current time
        int32 m_timeCla;                                    // Timer for power per sec calcultion
        int32 m_updateTargetMapInterval;                    // Timer for UpdateTargetMapOfEffect

        uint16 m_casterLevel;                               // Aura level (store caster level for correct show level dep amount)
        uint8 m_procCharges;                                // Aura charges (0 for infinite)
        uint8 m_stackAmount;                                // Aura stack amount

        uint8 m_EffectCount;
        AuraEffect* m_effects[SpellEffIndex::MAX_EFFECTS];
        ApplicationMap m_applications;

        bool m_isRemoved;
        bool m_isSingleTarget;                        // true if it's a single target spell and registered at caster - can change at spell steal for example
        bool m_isUsingCharges;

        uint32 m_lastProcAttemptTime;
        uint32 m_lastProcSuccessTime;

        SpellPowerEntry const* m_spellPowerData;
    private:
        Unit::AuraApplicationList m_removedApplications;
};

class UnitAura : public Aura
{
    friend Aura* Aura::Create(SpellInfo const* spellproto, uint32 effMask, WorldObject* owner, Unit* caster, int32 *baseAmount, Item* castItem, uint64 casterGUID, int32 castItemLevel);
    protected:
        explicit UnitAura(SpellInfo const* spellproto, uint32 effMask, WorldObject* owner, Unit* caster, int32 *baseAmount, Item* castItem, uint64 casterGUID, int32 castItemLevel);
    public:
        void _ApplyForTarget(Unit* target, Unit* caster, AuraApplication * aurApp);
        void _UnapplyForTarget(Unit* target, Unit* caster, AuraApplication * aurApp);

        void Remove(AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);

        void FillTargetMap(std::map<Unit*, uint32> & targets, Unit* caster);

        // Allow Apply Aura Handler to modify and access m_AuraDRGroup
        void SetDiminishGroup(DiminishingGroup group) { m_AuraDRGroup = group; }
        DiminishingGroup GetDiminishGroup() const { return m_AuraDRGroup; }

    private:
        DiminishingGroup m_AuraDRGroup:8;               // Diminishing
};

class DynObjAura : public Aura
{
    friend Aura* Aura::Create(SpellInfo const* spellproto, uint32 effMask, WorldObject* owner, Unit* caster, int32 *baseAmount, Item* castItem, uint64 casterGUID, int32 castItemLevel);
    protected:
        explicit DynObjAura(SpellInfo const* spellproto, uint32 effMask, WorldObject* owner, Unit* caster, int32 *baseAmount, Item* castItem, uint64 casterGUID, int32 castItemLevel);
    public:
        void Remove(AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);

        void FillTargetMap(std::map<Unit*, uint32> & targets, Unit* caster);
};
#endif
