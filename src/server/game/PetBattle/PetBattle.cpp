////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "PetBattle.h"
#include "PetBattleAbilityEffect.h"
#include "WildBattlePet.h"
#include "DB2Store.h"
#include "DatabaseEnv.h"
#include "AchievementMgr.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"

#define PETBATTLE_ABILITY_TRAP_FAMILY 0x99C00

/// Load
void BattlePet::Load(Field* p_Fields)
{
    JournalID        = MAKE_NEW_GUID(p_Fields[0].GetUInt64(), 0, HIGHGUID_BATTLE_PET);
    Slot             = p_Fields[1].GetInt32();
    Name             = p_Fields[2].GetString();
    NameTimeStamp    = p_Fields[3].GetUInt32();
    Species          = p_Fields[4].GetUInt32();
    Quality          = p_Fields[5].GetUInt32();
    Breed            = p_Fields[6].GetUInt32();
    Level            = p_Fields[7].GetUInt32();
    XP               = p_Fields[8].GetUInt32();
    DisplayModelID   = p_Fields[9].GetUInt32();
    Health           = p_Fields[10].GetInt32();
    Flags            = p_Fields[11].GetUInt32();
    InfoPower        = p_Fields[12].GetInt32();
    InfoMaxHealth    = p_Fields[13].GetInt32();
    InfoSpeed        = p_Fields[14].GetInt32();
    InfoGender       = p_Fields[15].GetInt32();
    AccountID        = p_Fields[16].GetInt32();
    DeclinedNames[0] = p_Fields[17].GetString();
    DeclinedNames[1] = p_Fields[18].GetString();
    DeclinedNames[2] = p_Fields[19].GetString();
    DeclinedNames[3] = p_Fields[20].GetString();
    DeclinedNames[4] = p_Fields[21].GetString();

    for (uint8 l_I = 0; l_I < MAX_PETBATTLE_ABILITIES; ++l_I)
        Abilities[l_I] = 0;

    for (uint32 l_SpeciesXAbilityId = 0; l_SpeciesXAbilityId < sBattlePetSpeciesXAbilityStore.GetNumRows(); ++l_SpeciesXAbilityId)
    {
        BattlePetSpeciesXAbilityEntry const* l_SpeciesXAbilityInfo = sBattlePetSpeciesXAbilityStore.LookupEntry(l_SpeciesXAbilityId);

        if (!l_SpeciesXAbilityInfo || l_SpeciesXAbilityInfo->speciesId != Species || l_SpeciesXAbilityInfo->level > Level)
            continue;

        if (l_SpeciesXAbilityInfo->level < 5)
            Abilities[l_SpeciesXAbilityInfo->tier] = l_SpeciesXAbilityInfo->abilityId;
        else
        {
            switch (l_SpeciesXAbilityInfo->tier)
            {
                case 0:
                    if (Flags & BATTLEPET_FLAG_ABILITY_1_SECOND)
                        Abilities[l_SpeciesXAbilityInfo->tier] = l_SpeciesXAbilityInfo->abilityId;
                    break;
                case 1:
                    if (Flags & BATTLEPET_FLAG_ABILITY_2_SECOND)
                        Abilities[l_SpeciesXAbilityInfo->tier] = l_SpeciesXAbilityInfo->abilityId;
                    break;
                case 2:
                    if (Flags & BATTLEPET_FLAG_ABILITY_3_SECOND)
                        Abilities[l_SpeciesXAbilityInfo->tier] = l_SpeciesXAbilityInfo->abilityId;
                    break;
                default:
                    break;
            }
        }
    }
}
/// Load
void BattlePet::CloneFrom(BattlePet::Ptr & p_BattlePet)
{
    JournalID        = p_BattlePet->JournalID;
    Slot             = p_BattlePet->Slot;
    Name             = p_BattlePet->Name;
    NameTimeStamp    = p_BattlePet->NameTimeStamp;
    Species          = p_BattlePet->Species;
    Quality          = p_BattlePet->Quality;
    Breed            = p_BattlePet->Breed;
    Level            = p_BattlePet->Level;
    XP               = p_BattlePet->XP;
    DisplayModelID   = p_BattlePet->DisplayModelID;
    Health           = p_BattlePet->Health;
    Flags            = p_BattlePet->Flags;
    InfoPower        = p_BattlePet->InfoPower;
    InfoMaxHealth    = p_BattlePet->InfoMaxHealth;
    InfoSpeed        = p_BattlePet->InfoSpeed;
    InfoGender       = p_BattlePet->InfoGender;
    AccountID        = p_BattlePet->AccountID;
    DeclinedNames[0] = p_BattlePet->DeclinedNames[0];
    DeclinedNames[1] = p_BattlePet->DeclinedNames[1];
    DeclinedNames[2] = p_BattlePet->DeclinedNames[2];
    DeclinedNames[3] = p_BattlePet->DeclinedNames[3];
    DeclinedNames[4] = p_BattlePet->DeclinedNames[4];

    for (uint8 l_I = 0; l_I < MAX_PETBATTLE_ABILITIES; ++l_I)
        Abilities[l_I] = p_BattlePet->Abilities[l_I];
}
/// Save
void BattlePet::Save(SQLTransaction& p_Transaction)
{
    PreparedStatement* l_Statement = LoginDatabase.GetPreparedStatement(LOGIN_REP_PETBATTLE);
    l_Statement->setUInt64(0, GUID_LOPART(JournalID));
    l_Statement->setInt32(1, Slot);
    l_Statement->setString(2, Name);
    l_Statement->setUInt32(3, NameTimeStamp);
    l_Statement->setUInt32(4, Species);
    l_Statement->setUInt32(5, Quality);
    l_Statement->setUInt32(6, Breed);
    l_Statement->setUInt32(7, Level);
    l_Statement->setUInt32(8, XP);
    l_Statement->setUInt32(9, DisplayModelID);
    l_Statement->setInt32(10, Health);
    l_Statement->setUInt32(11, Flags);
    l_Statement->setInt32(12, InfoPower);
    l_Statement->setInt32(13, InfoMaxHealth);
    l_Statement->setInt32(14, InfoSpeed);
    l_Statement->setInt32(15, InfoGender);
    l_Statement->setInt32(16, AccountID);
    l_Statement->setString(17, DeclinedNames[0]);
    l_Statement->setString(18, DeclinedNames[1]);
    l_Statement->setString(19, DeclinedNames[2]);
    l_Statement->setString(20, DeclinedNames[3]);
    l_Statement->setString(21, DeclinedNames[4]);
    p_Transaction->Append(l_Statement);
}

//////////////////////////////////////////////////////////////////////////

/// When a player earn a pet it will insert into his account
void BattlePet::AddToPlayer(Player* p_Player)
{
    PreparedStatement* l_Statement = LoginDatabase.GetPreparedStatement(LOGIN_INS_PETBATTLE);
    l_Statement->setInt32(0, Slot);
    l_Statement->setString(1, Name);
    l_Statement->setUInt32(2, NameTimeStamp);
    l_Statement->setUInt32(3, Species);
    l_Statement->setUInt32(4, Quality);
    l_Statement->setUInt32(5, Breed);
    l_Statement->setUInt32(6, Level);
    l_Statement->setUInt32(7, XP);
    l_Statement->setUInt32(8, DisplayModelID);
    l_Statement->setInt32(9, Health);
    l_Statement->setUInt32(10, Flags);
    l_Statement->setInt32(11, InfoPower);
    l_Statement->setInt32(12, InfoMaxHealth);
    l_Statement->setInt32(13, InfoSpeed);
    l_Statement->setInt32(14, InfoGender);
    l_Statement->setInt32(15, p_Player->GetSession()->GetAccountId());
    l_Statement->setString(16, DeclinedNames[0]);
    l_Statement->setString(17, DeclinedNames[1]);
    l_Statement->setString(18, DeclinedNames[2]);
    l_Statement->setString(19, DeclinedNames[3]);
    l_Statement->setString(20, DeclinedNames[4]);

    // We need to execute it sync to be sure we will have it at next async select
    LoginDatabase.Query(l_Statement);

    p_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_BATTLEPET, 1);
}

//////////////////////////////////////////////////////////////////////////

/// Update pet stats
void BattlePet::UpdateStats()
{
    int32* l_Stats = &InfoPower;
    InfoGender = 0;

    for (size_t l_I = 0; l_I < 3; ++l_I)
        l_Stats[l_I] = 1000;

    // Breed
    for (size_t l_I = 0; l_I < sBattlePetBreedStateStore.GetNumRows(); ++l_I)
    {
        BattlePetBreedStateEntry const* l_BreedStateInfo = sBattlePetBreedStateStore.LookupEntry(l_I);

        if (!l_BreedStateInfo || l_BreedStateInfo->breed != Breed)
            continue;

        if (l_BreedStateInfo->stateId == BATTLEPET_STATE_Stat_Power
            || l_BreedStateInfo->stateId == BATTLEPET_STATE_Stat_Stamina
            || l_BreedStateInfo->stateId == BATTLEPET_STATE_Stat_Speed)
            l_Stats[l_BreedStateInfo->stateId - BATTLEPET_STATE_Stat_Power] = l_BreedStateInfo->value;
        else if (l_BreedStateInfo->stateId == BATTLEPET_STATE_Stat_Gender)
            InfoGender = l_BreedStateInfo->value;
    }

    // Species
    for (size_t l_I = 0; l_I < sBattlePetSpeciesStateStore.GetNumRows(); ++l_I)
    {
        BattlePetSpeciesStateEntry const* l_SpeciesStateInfo = sBattlePetSpeciesStateStore.LookupEntry(l_I);

        if (!l_SpeciesStateInfo || l_SpeciesStateInfo->speciesId != Species)
            continue;

        if (l_SpeciesStateInfo->stateId == BATTLEPET_STATE_Stat_Power
            || l_SpeciesStateInfo->stateId == BATTLEPET_STATE_Stat_Stamina
            || l_SpeciesStateInfo->stateId == BATTLEPET_STATE_Stat_Speed)
            l_Stats[l_SpeciesStateInfo->stateId - BATTLEPET_STATE_Stat_Power] += l_SpeciesStateInfo->value;
    }

    // Quality
    float l_QualityFactor = 1.0f;

    BattlePetBreedQualityEntry const* l_QualityInfo = sBattlePetBreedQualityStore.LookupEntry(7 + Quality);
    if (l_QualityInfo)
        l_QualityFactor = l_QualityInfo->factor;

    // Level and Quality
    for (size_t l_I = 0; l_I < 3; ++l_I)
        l_Stats[l_I] = (l_Stats[l_I] * Level) * l_QualityFactor;

    InfoPower       = InfoPower / 100;
    InfoSpeed       = InfoSpeed / 100;
    InfoMaxHealth   = (10000 + (InfoMaxHealth * 5)) / 100;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
BattlePetInstance::BattlePetInstance()
{
    OriginalBattlePet = BattlePet::Ptr();
    OriginalCreature = 0;
}

//////////////////////////////////////////////////////////////////////////

/// Is alive ?
bool BattlePetInstance::IsAlive()
{
    return States[BATTLEPET_STATE_Is_Dead] == 0;
}

bool BattlePetInstance::CanAttack()
{
    return !States[BATTLEPET_STATE_turnLock] && !States[BATTLEPET_STATE_Mechanic_IsStunned];
}

//////////////////////////////////////////////////////////////////////////

/// Get max health
int32 BattlePetInstance::GetMaxHealth()
{
    return (10000 + (5 * States[BATTLEPET_STATE_Stat_Stamina]) + (States[BATTLEPET_STATE_maxHealthBonus] * States[BATTLEPET_STATE_Internal_InitialLevel])) / 100;
}
int32 BattlePetInstance::GetSpeed()
{
    int32 modPercent = States[BATTLEPET_STATE_Mod_SpeedPercent];

    // Passive: flying
    if (States[BATTLEPET_STATE_Passive_Flying])
        modPercent += 50;

    return (States[BATTLEPET_STATE_Stat_Speed] + CalculatePct(States[BATTLEPET_STATE_Stat_Speed], modPercent)) / 100;
}
/// Get max xp for current level
uint32 BattlePetInstance::GetMaxXPForCurrentLevel()
{
    uint32 l_It1 = 100 + (Level - 1);
    uint32 l_It2 = (Level - 1);

    if (l_It1 >= sGtBattlePetXPStore.GetNumRows() || l_It2 >= sGtBattlePetXPStore.GetNumRows())
        return 0;

    return sGtBattlePetXPStore.LookupEntry(l_It1)->value * sGtBattlePetXPStore.LookupEntry(l_It2)->value;
}
/// Get xp earn
uint32 BattlePetInstance::GetXPEarn(uint32 p_TargetPetID)
{
    std::shared_ptr<BattlePetInstance> l_TargetPet = PetBattleInstance->Pets[p_TargetPetID];

    int32 l_LevelDiff = l_TargetPet->Level - Level;

    if (l_LevelDiff < -4 || l_LevelDiff > 2)
        return 0;

    return (Level + 9) * (l_LevelDiff + 5);
}

//////////////////////////////////////////////////////////////////////////

/// Update original instance
void BattlePetInstance::UpdateOriginalInstance()
{
    if (!OriginalBattlePet)
        return;

    OriginalBattlePet->JournalID       = JournalID;
    OriginalBattlePet->Slot            = Slot;
    OriginalBattlePet->Species         = Species;
    OriginalBattlePet->Quality         = Quality;
    OriginalBattlePet->Breed           = Breed;
    OriginalBattlePet->Level           = Level;
    OriginalBattlePet->XP              = XP;
    OriginalBattlePet->DisplayModelID  = DisplayModelID;
    OriginalBattlePet->Health          = Health;
    OriginalBattlePet->Flags           = Flags;
    OriginalBattlePet->InfoPower       = InfoPower;
    OriginalBattlePet->InfoMaxHealth   = InfoMaxHealth;
    OriginalBattlePet->InfoSpeed       = InfoSpeed;
    OriginalBattlePet->InfoGender      = InfoGender;
    OriginalBattlePet->AccountID       = AccountID;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
PetBattleEventUpdate::PetBattleEventUpdate()
{
    UpdateType  = 0;
    TargetPetID = PETBATTLE_NULL_ID;
    Health      = 0;
    MaxHealth   = 0;
    TriggerAbilityId = 0;
    Speed       = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
PetBattleEvent::PetBattleEvent(uint32 p_EventType, int32 p_SourcePetID, uint32 p_Flags, uint32 p_AbilityEffectID, uint32 p_RoundTurn, uint32 p_BuffTurn, uint32 p_StackDepth)
{
    Flags           = p_Flags;
    SourcePetID     = p_SourcePetID;
    AbilityEffectID = p_AbilityEffectID;
    EventType       = p_EventType;
    BuffTurn        = p_BuffTurn;
    RoundTurn       = p_RoundTurn;
    StackDepth      = p_StackDepth;
}

//////////////////////////////////////////////////////////////////////////

/// Make an health update
PetBattleEvent& PetBattleEvent::UpdateHealth(int8 p_TargetPetID, int32 p_Health)
{
    PetBattleEventUpdate l_Update;
    l_Update.UpdateType     = PETBATTLE_EVENT_UPDATE_HEALTH;
    l_Update.TargetPetID    = p_TargetPetID;
    l_Update.Health         = p_Health;

    Updates.push_back(l_Update);

    return *this;
}
/// Make an max health update
PetBattleEvent& PetBattleEvent::UpdateMaxHealth(int8 p_TargetPetID, int32 p_MaxHealth)
{
    return UpdateSpeed(p_TargetPetID, p_MaxHealth);
}
/// Make an state update
PetBattleEvent& PetBattleEvent::UpdateState(int8 p_TargetPetID, uint32 p_StateID, int32 p_Value)
{
    PetBattleEventUpdate l_Update;
    l_Update.UpdateType     = PETBATTLE_EVENT_UPDATE_STATE;
    l_Update.TargetPetID    = p_TargetPetID;
    l_Update.State.ID       = p_StateID;
    l_Update.State.Value    = p_Value;

    Updates.push_back(l_Update);

    return *this;
}
/// Make an front pet change
PetBattleEvent& PetBattleEvent::UpdateFrontPet(int8 p_NewFrontPet)
{
    PetBattleEventUpdate l_Update;
    l_Update.UpdateType     = PETBATTLE_EVENT_UPDATE_FRONTPET;
    l_Update.TargetPetID    = p_NewFrontPet;

    Updates.push_back(l_Update);

    return *this;
}
/// Make an buff update
PetBattleEvent& PetBattleEvent::UpdateBuff(int8 p_TargetPetID, uint32 p_ID, uint32 p_AbilityID, int32 p_Duration, uint32 p_Turn)
{
    PetBattleEventUpdate l_Update;
    l_Update.UpdateType     = PETBATTLE_EVENT_UPDATE_BUFF;
    l_Update.TargetPetID    = p_TargetPetID;
    l_Update.Buff.AbilityID = p_AbilityID;
    l_Update.Buff.Duration  = p_Duration;
    l_Update.Buff.Turn      = p_Turn;
    l_Update.Buff.ID        = p_ID;

    Updates.push_back(l_Update);

    return *this;
}
/// Make an speed update
PetBattleEvent& PetBattleEvent::UpdateSpeed(int8 p_TargetPetID, int32 p_Speed)
{
    PetBattleEventUpdate l_Update;
    l_Update.UpdateType     = PETBATTLE_EVENT_UPDATE_SPEED;
    l_Update.TargetPetID    = p_TargetPetID;
    l_Update.Speed          = p_Speed;

    Updates.push_back(l_Update);

    return *this;
}
/// Make an trigger
PetBattleEvent& PetBattleEvent::Trigger(int8 p_TargetPetID, uint32 p_AbilityId)
{
    PetBattleEventUpdate l_Update;
    l_Update.UpdateType = PETBATTLE_EVENT_UPDATE_TRIGGER;
    l_Update.TargetPetID = p_TargetPetID;
    l_Update.TriggerAbilityId = p_AbilityId;

    Updates.push_back(l_Update);

    return *this;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Apply
void PetBattleAura::Apply(PetBattle* p_Battle)
{
    for (uint32 l_I = 0; l_I < sBattlePetAbilityStateStore.GetNumRows(); l_I++)
    {
        const BattlePetAbilityStateEntry * l_Entry = sBattlePetAbilityStateStore.LookupEntry(l_I);

        if (!l_Entry || l_Entry->abilityId != AbilityID)
            continue;

        uint32 l_Flags = 0;

        // Passive: elemental
        // TODO: add state flags for positive / negative
        /*FIXME: need more work; check weather ability, dont change state value on remove
        if (p_Battle->Pets[TargetPetID]->States[BATTLEPET_STATE_Passive_Elemental])
        {
            switch (l_Entry->stateId)
            {
            case BATTLEPET_STATE_Mod_HealingTakenPercent:
            case BATTLEPET_STATE_Stat_Accuracy:
                if (l_Entry->value < 0)
                    l_Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;
                break;

            case BATTLEPET_STATE_Mechanic_IsBlind:
            case BATTLEPET_STATE_Mechanic_IsChilled:
            case BATTLEPET_STATE_Mechanic_IsBurning:
            case BATTLEPET_STATE_swapOutLock:
                if (l_Entry->value > 0)
                    l_Flags |= PETBATTLE_EVENT_FLAG_IMMUNE;
                break;

            default:
                break;
            }
        }
        */

        int32 l_Value = p_Battle->Pets[TargetPetID]->States[l_Entry->stateId];
        if (!l_Flags)
            l_Value += l_Entry->value;

        p_Battle->SetPetState(CasterPetID, TargetPetID, 0, l_Entry->stateId, l_Value, false, l_Flags);
    }

    if (AbilityID == 577)   ///< Healthy http://www.wowhead.com/petability=576/perk-up
    {
        std::shared_ptr<BattlePetInstance> l_Pet = p_Battle->Pets[TargetPetID];
        int32 l_HealthBonus = 5 * l_Pet->Level;

        float l_HealthPct = float(l_Pet->Health) / float(l_Pet->GetMaxHealth() - l_HealthBonus);

        l_Pet->InfoMaxHealth = l_Pet->GetMaxHealth();
        l_Pet->Health = l_HealthPct * float(l_Pet->GetMaxHealth());

        {
            PetBattleEvent l_Event(PETBATTLE_EVENT_SET_MAX_HEALTH, CasterPetID, 0, 577, p_Battle->RoundTurn, 0, 1);
            l_Event.UpdateMaxHealth(TargetPetID, l_Pet->InfoMaxHealth);
            p_Battle->RoundEvents.push_back(l_Event);
        }
        {
            PetBattleEvent l_Event(PETBATTLE_EVENT_SET_HEALTH, CasterPetID, 0, 577, p_Battle->RoundTurn, 0, 1);
            l_Event.UpdateHealth(TargetPetID, l_Pet->Health);
            p_Battle->RoundEvents.push_back(l_Event);
        }
    }
}
/// Remove
void PetBattleAura::Remove(PetBattle* p_Battle)
{
    if (AbilityID == p_Battle->WeatherAbilityId)
        p_Battle->WeatherAbilityId = 0;

    for (uint32 l_I = 0; l_I < sBattlePetAbilityStateStore.GetNumRows(); l_I++)
    {
        const BattlePetAbilityStateEntry * l_Entry = sBattlePetAbilityStateStore.LookupEntry(l_I);

        if (!l_Entry || l_Entry->abilityId != AbilityID)
            continue;

        p_Battle->SetPetState(CasterPetID, TargetPetID, 0, l_Entry->stateId, p_Battle->Pets[TargetPetID]->States[l_Entry->stateId] - l_Entry->value);
    }

    if (AbilityID == 577)   ///< Healthy http://www.wowhead.com/petability=576/perk-up
    {
        std::shared_ptr<BattlePetInstance> l_Pet = p_Battle->Pets[TargetPetID];
        int32 l_HealthBonus = 5 * l_Pet->Level;

        float l_HealthPct = float(l_Pet->Health) / float(l_Pet->GetMaxHealth() + l_HealthBonus);

        l_Pet->InfoMaxHealth = l_Pet->GetMaxHealth();
        l_Pet->Health = l_HealthPct * float(l_Pet->GetMaxHealth());

        {
            PetBattleEvent l_Event(PETBATTLE_EVENT_SET_HEALTH, CasterPetID, 0, 577, p_Battle->RoundTurn, Turn, 1);
            l_Event.UpdateHealth(TargetPetID, l_Pet->Health);
            p_Battle->RoundEvents.push_back(l_Event);
        }
        {
            PetBattleEvent l_Event(PETBATTLE_EVENT_SET_MAX_HEALTH, CasterPetID, 0, 577, p_Battle->RoundTurn, Turn, 1);
            l_Event.UpdateMaxHealth(TargetPetID, l_Pet->InfoMaxHealth);
            p_Battle->RoundEvents.push_back(l_Event);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

/// Process all aura effects
void PetBattleAura::Process(PetBattle* p_Battle)
{
    if (Duration != -1 && Turn > MaxDuration && !Expired)
    {
        Expire(p_Battle);
        return;
    }

    BattlePetAbilityEntry const* l_AbilityInfo = sBattlePetAbilityStore.LookupEntry(AbilityID);
    if (l_AbilityInfo)
    {
        PetBattleAbilityTurn l_AbilityTurn;
        memset(&l_AbilityTurn, 0, sizeof(l_AbilityTurn));

        uint32 l_TurnCount = 0;
        uint32 l_MaxTurnID = 0;

        for (uint32 l_AbilityTurnId = 0; l_AbilityTurnId < sBattlePetAbilityTurnStore.GetNumRows(); ++l_AbilityTurnId)
        {
            BattlePetAbilityTurnEntry const* abilityTurnInfo = sBattlePetAbilityTurnStore.LookupEntry(l_AbilityTurnId);
            if (!abilityTurnInfo || abilityTurnInfo->abilityId != AbilityID)
                continue;

            l_TurnCount++;
            l_MaxTurnID = std::max(l_MaxTurnID, abilityTurnInfo->turn);
        }

        for (uint32 l_AbilityTurnId = 0; l_AbilityTurnId < sBattlePetAbilityTurnStore.GetNumRows(); ++l_AbilityTurnId)
        {
            BattlePetAbilityTurnEntry const* l_AbilityTurnInfo = sBattlePetAbilityTurnStore.LookupEntry(l_AbilityTurnId);
            if (!l_AbilityTurnInfo || l_AbilityTurnInfo->abilityId != AbilityID)
                continue;

            if (l_AbilityTurnInfo->turn != Turn && l_TurnCount != 1 && l_MaxTurnID != 1)
                continue;

            if (l_AbilityTurnInfo->hasProcType && l_AbilityTurnInfo->procType != PETBATTLE_ABILITY_TURN0_PROC_ON_TURN)
                continue;

            for (uint32 l_AbilityEffectId = 0; l_AbilityEffectId < sBattlePetAbilityEffectStore.GetNumRows(); ++l_AbilityEffectId)
            {
                BattlePetAbilityEffectEntry const* l_AbilityEffectInfo = sBattlePetAbilityEffectStore.LookupEntry(l_AbilityEffectId);

                if (!l_AbilityEffectInfo || l_AbilityEffectInfo->abilityTurnId != l_AbilityTurnInfo->id)
                    continue;

                PetBattleAbilityEffect l_Effect;
                l_Effect.EffectInfo         = l_AbilityEffectInfo;
                l_Effect.AbilityTurn        = &l_AbilityTurn;
                l_Effect.PetBattleInstance  = p_Battle;
                l_Effect.Caster             = CasterPetID;
                l_Effect.StopChain          = false;
                l_Effect.AbilityID          = AbilityID;
                l_Effect.IsTriggered        = l_AbilityInfo->flags & BATTLEPET_ABILITY_FLAG_TRIGGER;
                l_Effect.ReportFailAsImmune = false;
                l_Effect.Targets.push_back(TargetPetID);

                std::list<PetBattleEvent>::iterator l_It = p_Battle->RoundEvents.end();

                if (!l_Effect.Execute())
                    l_AbilityTurn.ChainFailure |= 1 << (l_AbilityEffectInfo->effectIndex - 1);

                /// Update "buff turn" on all event triggered by the current ability effect
                for (; l_It != p_Battle->RoundEvents.end(); ++l_It)
                    l_It->BuffTurn = Turn;

                if (l_Effect.StopChain)
                    break;
            }
        }
    }

    PetBattleEvent l_Event(PETBATTLE_EVENT_BUFF_CHANGE, CasterPetID);
    l_Event.UpdateBuff(TargetPetID, ID, AbilityID, Duration, Turn);

    p_Battle->RoundEvents.push_back(l_Event);

    Turn++;
    Duration--;
}
/// Make aura expire
void PetBattleAura::Expire(PetBattle* p_Battle)
{
    if (Expired)
        return;

    PetBattleEvent l_Event(PETBATTLE_EVENT_BUFF_CANCEL, CasterPetID, 0, 0, 1, 0, 1);
    l_Event.UpdateBuff(TargetPetID, ID, AbilityID, 0, Turn);

    p_Battle->RoundEvents.push_back(l_Event);

    Expired = true;

    p_Battle->Cast(CasterPetID, AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_AURA_REMOVED, PETBATTLE_CAST_TRIGGER_ALL);

    Remove(p_Battle);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update team
bool PetBattleTeam::Update()
{
    if (Ready)
        return false;

    if (PetBattleInstance->BattleStatus == PETBATTLE_STATUS_FINISHED)
    {
        Ready = false;
        return false;
    }
		
    std::shared_ptr<BattlePetInstance>  l_FrontPet          = PetBattleInstance->Pets[ActivePetID];
    bool                                l_IsFrontPetAlive   = l_FrontPet->IsAlive();
    uint32                              l_ThisTeamID        = PetBattleInstance->Teams[0] == this ? PETBATTLE_TEAM_1 : PETBATTLE_TEAM_2;

    if (!l_IsFrontPetAlive)
    {
        std::vector<uint32> l_AvailablesPets = GetAvailablesPets();

        if (!l_AvailablesPets.size())
        {
            PetBattleInstance->Finish(!l_ThisTeamID, false);
            return true;
        }
        if (l_AvailablesPets.size() == 1)
        {
            PetBattleInstance->SwapPet(l_ThisTeamID, l_AvailablesPets[0]);
            //TODO send ClientPetBattleReplacementsMade opcode

            if (PetBattleInstance->BattleType == PETBATTLE_TYPE_PVE && l_ThisTeamID == PETBATTLE_PVE_TEAM_ID)
                PetBattleInstance->Teams[PETBATTLE_TEAM_1]->Ready = true;
        }
        else if (PetBattleInstance->BattleType == PETBATTLE_TYPE_PVE && l_ThisTeamID == PETBATTLE_PVE_TEAM_ID)
        {
            PetBattleInstance->SwapPet(l_ThisTeamID, l_AvailablesPets[rand() % l_AvailablesPets.size()]);
            PetBattleInstance->Teams[PETBATTLE_TEAM_1]->Ready = true;
            Ready = true;
        }
    }
    else if (HasPendingMultiTurnCast() && l_IsFrontPetAlive)
        Ready = true;
    else if (l_IsFrontPetAlive)
    {
        if (PetBattleInstance->BattleType == PETBATTLE_TYPE_PVE && l_ThisTeamID == PETBATTLE_PVE_TEAM_ID)
        {
            std::vector<uint32> l_AvailablesAbilities;

            for (uint32 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
                if (l_FrontPet->Abilities[l_CurrentAbilitySlot] && PetBattleInstance->CanCast(l_ThisTeamID, l_FrontPet->Abilities[l_CurrentAbilitySlot]))
                    l_AvailablesAbilities.push_back(l_FrontPet->Abilities[l_CurrentAbilitySlot]);

            if (l_AvailablesAbilities.size())
            {
                uint32 l_NextAbilityID = l_AvailablesAbilities[rand() % l_AvailablesAbilities.size()];

                PetBattleInstance->PrepareCast(l_ThisTeamID, l_NextAbilityID);
            }
            else
                PetBattleInstance->SwapPet(l_ThisTeamID, ActivePetID);  ///< Skip turn

            Ready = true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////

/// Do casts for front pet
void PetBattleTeam::DoCasts(uint32 p_Turn0ProcCond)
{
    if (ActiveAbilityId && PetBattleInstance->Pets[ActivePetID]->IsAlive() && PetBattleInstance->Pets[ActivePetID]->CanAttack())
    {
        if (p_Turn0ProcCond == PETBATTLE_ABILITY_TURN0_PROC_ON_NONE && PetBattleInstance->RoundDeadPets.size() == 0)
            PetBattleInstance->Cast(ActivePetID, ActiveAbilityId, activeAbilityTurn, p_Turn0ProcCond, PETBATTLE_CAST_TRIGGER_NONE);
        else
            PetBattleInstance->Cast(ActivePetID, ActiveAbilityId, 0, p_Turn0ProcCond, PETBATTLE_CAST_TRIGGER_ALL);

        for (PetBattleAuraList::iterator l_It = PetBattleInstance->PetAuras.begin(); l_It != PetBattleInstance->PetAuras.end(); ++l_It)
            if (!(*l_It)->Expired && (*l_It)->CasterPetID == ActivePetID)
                PetBattleInstance->Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_ABILITY, PETBATTLE_CAST_TRIGGER_ALL);
    }
}

//////////////////////////////////////////////////////////////////////////

/// Has pending multi turn cast
bool PetBattleTeam::HasPendingMultiTurnCast()
{
    if (ActiveAbilityId && activeAbilityTurn <= activeAbilityTurnMax)
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////

/// Can cast any ability
bool PetBattleTeam::CanCastAny()
{
    if (HasPendingMultiTurnCast())
        return false;

    if (PetBattleInstance->Pets[ActivePetID]->States[BATTLEPET_STATE_turnLock])
        return false;

    if (PetBattleInstance->Pets[ActivePetID]->States[BATTLEPET_STATE_Mechanic_IsStunned])
        return false;

    return true;
}
/// Can swap
bool PetBattleTeam::CanSwap(int8 p_ReplacementPet)
{
    if (HasPendingMultiTurnCast())
        return false;

    if (PetBattleInstance->Pets[ActivePetID]->IsAlive() 
        &&  (  PetBattleInstance->Pets[ActivePetID]->States[BATTLEPET_STATE_swapOutLock]
            || PetBattleInstance->Pets[ActivePetID]->States[BATTLEPET_STATE_Mechanic_IsWebbed]))
        return false;

    if (p_ReplacementPet != PETBATTLE_NULL_ID)
    {
        if (!PetBattleInstance->Pets[p_ReplacementPet]->IsAlive())
            return false;

        if (PetBattleInstance->Pets[p_ReplacementPet]->States[BATTLEPET_STATE_swapInLock])
            return false;
    }

    return true;
}
/// Can catch opponent pet
uint8 PetBattleTeam::CanCatchOpponentTeamFrontPet()
{
    if (PetBattleInstance->BattleType != PETBATTLE_TYPE_PVE)
        return 0;

    if (!GetCatchAbilityID())
        return 0;

    uint32 l_ThisTeamID = PetBattleInstance->Teams[PETBATTLE_TEAM_1] == this ? PETBATTLE_TEAM_1 : PETBATTLE_TEAM_2;

    if (PetBattleInstance->BattleType != PETBATTLE_TYPE_PVE || (PetBattleInstance->BattleType == PETBATTLE_TYPE_PVE && PetBattleInstance->PveBattleType == PVE_PETBATTLE_TRAINER))
        return 0;

    if (l_ThisTeamID == PETBATTLE_PVE_TEAM_ID)
        return 0;

    if (CapturedPet != PETBATTLE_NULL_ID)
        return PETBATTLE_TEAM_CATCH_FLAG_ONE_CATCH_PER_FIGHT;

    if (PetBattleInstance->Teams[!l_ThisTeamID]->ActivePetID == PETBATTLE_NULL_ID)
        return 0;

    std::shared_ptr<BattlePetInstance> l_TargetPet = PetBattleInstance->Pets[PetBattleInstance->Teams[!l_ThisTeamID]->ActivePetID];

    if (!l_TargetPet || !l_TargetPet->IsAlive())
        return 0;

    if (BattlePetSpeciesEntry const* l_Entry = sBattlePetSpeciesStore.LookupEntry(l_TargetPet->Species))
    {
        if ((l_Entry->flags & BATTLEPET_SPECIES_FLAG_UNTAMEABLE) != 0)
            return 0;
    }

    float l_TargetHealth    = l_TargetPet->Health;
    float l_TargetMaxHealth = l_TargetPet->InfoMaxHealth;
    float l_TargetHealthPct = (l_TargetHealth / l_TargetMaxHealth) * 100;

    if (l_TargetHealthPct >= 35)
        return PETBATTLE_TEAM_CATCH_FLAG_TOO_MUCH_HP;

    /// Max 3 same pet for player
    if (CapturedSpeciesCount.find(l_TargetPet->Species) != CapturedSpeciesCount.end())
    {
        if (CapturedSpeciesCount[l_TargetPet->Species] > 2)
            return 0;
    }

    /// todo more check

    return PETBATTLE_TEAM_CATCH_FLAG_ENABLE_TRAP;
}

//////////////////////////////////////////////////////////////////////////

/// Get team flags 1
uint32 PetBattleTeam::GetTeamInputFlags()
{
    uint32 l_Flags = 0;

    if (PetBattleInstance->BattleType != PETBATTLE_TYPE_PVE && PetBattleInstance->Teams[PETBATTLE_TEAM_1]->ActivePetID == PETBATTLE_NULL_ID && PetBattleInstance->Teams[PETBATTLE_TEAM_2]->ActivePetID == PETBATTLE_NULL_ID)
        l_Flags |= PETBATTLE_TEAM_INPUT_FLAG_LOCK_ABILITIES_2 | PETBATTLE_TEAM_INPUT_FLAG_SELECT_NEW_PET;;

    if (!CanCastAny())
        l_Flags |= (PETBATTLE_TEAM_INPUT_FLAG_LOCK_ABILITIES_1 | PETBATTLE_TEAM_INPUT_FLAG_LOCK_ABILITIES_2);

    if (!CanSwap())
        l_Flags |= PETBATTLE_TEAM_INPUT_FLAG_LOCK_PET_SWAP;

    if (!PetBattleInstance->Pets[ActivePetID]->IsAlive())
    {
        if (GetAvailablesPets().size() > 1)
            l_Flags |= PETBATTLE_TEAM_INPUT_FLAG_LOCK_ABILITIES_2 | PETBATTLE_TEAM_INPUT_FLAG_SELECT_NEW_PET;
        else
            l_Flags |= PETBATTLE_TEAM_INPUT_FLAG_LOCK_ABILITIES_2 | PETBATTLE_TEAM_INPUT_FLAG_LOCK_PET_SWAP;
    }
    
    uint32 l_ThisTeamID = PetBattleInstance->Pets[ActivePetID]->TeamID;
    if (PetBattleInstance->Teams[!l_ThisTeamID]->ActivePetID != PETBATTLE_NULL_ID
        && PetBattleInstance->Pets[PetBattleInstance->Teams[!l_ThisTeamID]->ActivePetID]
        && !PetBattleInstance->Pets[PetBattleInstance->Teams[!l_ThisTeamID]->ActivePetID]->IsAlive())
        l_Flags |= PETBATTLE_TEAM_INPUT_FLAG_LOCK_ABILITIES_2 | PETBATTLE_TEAM_INPUT_FLAG_LOCK_PET_SWAP;

    return l_Flags;
}
/// Get team flags 2
uint32 PetBattleTeam::GetTeamTrapFlags()
{
    uint32 l_Flags = CanCatchOpponentTeamFrontPet();

    /// @TODO PETBATTLE_TEAM_CATCH_FLAG_NEED_LVL3_PET

    return l_Flags;
}
/// Get available pet for combat
std::vector<uint32> PetBattleTeam::GetAvailablesPets()
{
    std::vector<uint32> l_AvailablesPets;
    uint32              l_ThisTeamID        = PetBattleInstance->Teams[PETBATTLE_TEAM_1] == this ? PETBATTLE_TEAM_1 : PETBATTLE_TEAM_2;

    for (uint32 l_CurrentPetSlot = 0; l_CurrentPetSlot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetSlot++)
        if (PetBattleInstance->Pets[l_CurrentPetSlot] && PetBattleInstance->Pets[l_CurrentPetSlot]->IsAlive() && PetBattleInstance->Pets[l_CurrentPetSlot]->TeamID == l_ThisTeamID
            && CanSwap(l_CurrentPetSlot))
            l_AvailablesPets.push_back(l_CurrentPetSlot);

    return l_AvailablesPets;
}

//////////////////////////////////////////////////////////////////////////

/// Get catch ability ID
uint32 PetBattleTeam::GetCatchAbilityID()
{
    uint32 l_ThisTeamID = PetBattleInstance->Teams[PETBATTLE_TEAM_1] == this ? PETBATTLE_TEAM_1 : PETBATTLE_TEAM_2;

    if (PetBattleInstance->BattleType == PETBATTLE_TYPE_PVE && l_ThisTeamID == PETBATTLE_PVE_TEAM_ID)
        return 0;

    Player* l_Player = ObjectAccessor::GetObjectInWorld(PlayerGuid, (Player*)NULL);

    if (l_Player)
    {
        /// LevelUpDisplay.lua => LEVEL_UP_TRAP_LEVELS
        static const uint32 l_PetBattleCatchAbilityID[] = { 427, 77, 135 };

        return l_PetBattleCatchAbilityID[l_Player->GetBattlePetTrapLevel() - 1];
    }

    return 427; ///< Pet trap
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
PetBattle::PetBattle()
{
    BattleStatus    = PETBATTLE_STATUS_CREATION;
    RoundStatus     = PETBATTLE_ROUND_FINISHED;
    RoundResult     = PETBATTLE_ROUND_RESULT_NONE;
    TotalPetCount   = 0;
    WeatherAbilityId = 0;
    
    for (uint32 l_CurrentPetID = 0; l_CurrentPetID < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetID++)
        Pets[l_CurrentPetID] = 0;

    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        Teams[l_CurrentTeamID] = new PetBattleTeam();
        Teams[l_CurrentTeamID]->PetBattleInstance       = this;
        Teams[l_CurrentTeamID]->TeamPetCount            = 0;
        Teams[l_CurrentTeamID]->OwnerGuid               = 0;
        Teams[l_CurrentTeamID]->PlayerGuid              = 0;
        Teams[l_CurrentTeamID]->ActiveAbilityId         = 0;
        Teams[l_CurrentTeamID]->activeAbilityTurn       = 0;
        Teams[l_CurrentTeamID]->activeAbilityTurnMax    = 0;
        Teams[l_CurrentTeamID]->CapturedPet             = PETBATTLE_NULL_ID;
        Teams[l_CurrentTeamID]->Ready                   = false;

        for (uint32 l_CurrentPetID = 0; l_CurrentPetID < MAX_PETBATTLE_SLOTS; l_CurrentPetID++)
            Teams[l_CurrentTeamID]->TeamPets[l_CurrentPetID] = 0;
    }

    WinnerTeamId = -1;
    CatchedPetId = PETBATTLE_NULL_ID;

    m_UpdateTimer.SetInterval(PETBATTLE_UPDATE_INTERVAL);
}
/// Destructor
PetBattle::~PetBattle()
{
    /// Delete teams
    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if ((BattleType == PETBATTLE_TYPE_PVE && l_CurrentTeamID != PETBATTLE_PVE_TEAM_ID) || BattleType != PETBATTLE_TYPE_PVE)
            for (uint32 l_CurrentPetID = 0; l_CurrentPetID < MAX_PETBATTLE_SLOTS; l_CurrentPetID++)
                if (Pets[l_CurrentPetID])
                    Pets[l_CurrentPetID] = BattlePetInstance::Ptr();

        delete Teams[l_CurrentTeamID];
    }

    for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
        delete (*l_It);
}

//////////////////////////////////////////////////////////////////////////

/// Add pet to the battle
void PetBattle::AddPet(uint32 p_TeamID, std::shared_ptr<BattlePetInstance> p_Pet)
{
    if (p_Pet == 0 || p_Pet->Slot < 0 || p_Pet->Slot >= MAX_PETBATTLE_SLOTS)
        return;

    assert(p_TeamID < MAX_PETBATTLE_TEAM);
    assert(Teams[p_TeamID]->TeamPetCount < MAX_PETBATTLE_SLOTS);

    Teams[p_TeamID]->TeamPets[Teams[p_TeamID]->TeamPetCount] = p_Pet;

    p_Pet->ID                   = (p_TeamID == PETBATTLE_TEAM_2 ? MAX_PETBATTLE_SLOTS : 0) + Teams[p_TeamID]->TeamPetCount;
    p_Pet->TeamID               = p_TeamID;
    p_Pet->PetBattleInstance    = this;
    p_Pet->OldLevel             = p_Pet->Level;
    p_Pet->OldXP                = p_Pet->OldXP;

    TotalPetCount++;
    Teams[p_TeamID]->TeamPetCount++;

    if (Teams[p_TeamID]->TeamPetCount > MAX_PETBATTLE_SLOTS)
    {
        ACE_Stack_Trace l_StackTrace;
        sLog->outAshran("PetBattle::AddPet TeamPetCount overflow (%u)", Teams[p_TeamID]->TeamPetCount);
        sLog->outAshran(l_StackTrace.c_str());
    }

    Pets[p_Pet->ID] = p_Pet;

    memset(p_Pet->States, 0, sizeof(p_Pet->States));
    p_Pet->States[0] = 1;

    for (uint32 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
    {
        p_Pet->Cooldowns[l_CurrentAbilitySlot] = -1;
        p_Pet->Lockdowns[l_CurrentAbilitySlot] = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    /// State calculation part

    // Breed
    for (size_t l_I = 0; l_I < sBattlePetBreedStateStore.GetNumRows(); ++l_I)
    {
        BattlePetBreedStateEntry const* l_BreedStateInfo = sBattlePetBreedStateStore.LookupEntry(l_I);
        if (!l_BreedStateInfo || l_BreedStateInfo->breed != p_Pet->Breed)
            continue;

        p_Pet->States[l_BreedStateInfo->stateId] += l_BreedStateInfo->value;
    }

    // Species
    for (size_t l_I = 0; l_I < sBattlePetSpeciesStateStore.GetNumRows(); ++l_I)
    {
        BattlePetSpeciesStateEntry const* l_SpeciesStateInfo = sBattlePetSpeciesStateStore.LookupEntry(l_I);
        if (!l_SpeciesStateInfo || l_SpeciesStateInfo->speciesId != p_Pet->Species)
            continue;

        if (l_SpeciesStateInfo->stateId > NUM_BATTLEPET_STATES)
            continue;

        p_Pet->States[l_SpeciesStateInfo->stateId] += l_SpeciesStateInfo->value;
    }

    // Primary stats is scaled with level and quality
    BattlePetBreedQualityEntry const* l_QualityInfo = sBattlePetBreedQualityStore.LookupEntry(7 + p_Pet->Quality);
    if (l_QualityInfo)
        for (size_t l_I = 0; l_I < 3; ++l_I)
            p_Pet->States[BATTLEPET_STATE_Stat_Power + l_I] *= p_Pet->Level * l_QualityInfo->factor;

    // Populate infos
    p_Pet->InfoPower        = p_Pet->States[BATTLEPET_STATE_Stat_Power] / 100;
    p_Pet->InfoSpeed        = p_Pet->States[BATTLEPET_STATE_Stat_Speed] / 100;
    p_Pet->InfoMaxHealth    = (10000 + (p_Pet->States[BATTLEPET_STATE_Stat_Stamina] * 5)) / 100;
    p_Pet->InfoGender       = p_Pet->States[BATTLEPET_STATE_Stat_Gender];

    if (p_Pet->Health > p_Pet->InfoMaxHealth)
        p_Pet->Health = p_Pet->InfoMaxHealth;
}

//////////////////////////////////////////////////////////////////////////

/// Start the battle
void PetBattle::Begin()
{
    RoundStatus = PETBATTLE_ROUND_RUNNING;
    RoundResult = PETBATTLE_ROUND_RESULT_NORMAL;
    Turn = 0;

    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        Teams[l_CurrentTeamID]->ActiveAbilityId = 0;
        Teams[l_CurrentTeamID]->ActivePetID     = l_CurrentTeamID == PETBATTLE_TEAM_2 ? MAX_PETBATTLE_SLOTS : 0;

        Player* l_Player = sObjectAccessor->FindPlayer(Teams[l_CurrentTeamID]->OwnerGuid);

        if (l_Player)
            l_Player->UnsummonCurrentBattlePetIfAny(true);
    }

    /// In PVE battle the front pets (slot 0) are auto lock
    if (BattleType == PETBATTLE_TYPE_PVE)
    {
        /// Team 1 pet selection
        SwapPet(PETBATTLE_TEAM_1, Teams[PETBATTLE_TEAM_1]->ActivePetID, true);

        /// Team 2 pet selection
        SwapPet(PETBATTLE_TEAM_2, Teams[PETBATTLE_TEAM_2]->ActivePetID, true);

        Player* l_Player = sObjectAccessor->FindPlayer(Teams[PETBATTLE_TEAM_1]->OwnerGuid);

        if (l_Player)
        {
            l_Player->PetBattleCountBattleSpecies();

            l_Player->GetSession()->SendPetBattleInitialUpdate(this);
            l_Player->GetSession()->SendPetBattleFirstRound(this); 
        }

        RoundEvents.clear();
        RoundDeadPets.clear();
        RoundPetSpeedUpdate.clear();
    }
    else if (BattleType == PETBATTLE_TYPE_PVP_DUEL || BattleType == PETBATTLE_TYPE_PVP_MATCHMAKING)
    {
        Player* l_PlayerA = sObjectAccessor->FindPlayer(Teams[PETBATTLE_TEAM_1]->OwnerGuid);
        Player* l_PlayerB = sObjectAccessor->FindPlayer(Teams[PETBATTLE_TEAM_2]->OwnerGuid);

        if (l_PlayerA && l_PlayerB)
        {
            l_PlayerA->GetSession()->SendPetBattleInitialUpdate(this);
            l_PlayerB->GetSession()->SendPetBattleInitialUpdate(this);

            l_PlayerA->GetSession()->SendPetBattleFirstRound(this);
            l_PlayerB->GetSession()->SendPetBattleFirstRound(this);
        }

        RoundEvents.clear();
        RoundDeadPets.clear();
        RoundPetSpeedUpdate.clear();
    }

    Turn++;

    // Initialize pet health and auras
    for (uint32 l_PetId = 0; l_PetId < MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS; ++l_PetId)
        Cast(l_PetId, 291, 1, 0, PETBATTLE_CAST_TRIGGER_NONE);

    RoundEvents.clear();
    RoundDeadPets.clear();
    RoundPetSpeedUpdate.clear();

    RoundResult = PETBATTLE_ROUND_RESULT_NONE;
    RoundStatus = PETBATTLE_ROUND_FINISHED;
    BattleStatus = PETBATTLE_STATUS_RUNNING;
}
/// Proceed round battle
void PetBattle::ProceedRound()
{
    if (!Teams[PETBATTLE_TEAM_1]->Ready || !Teams[PETBATTLE_TEAM_2]->Ready)
        return;

    RoundStatus = PETBATTLE_ROUND_RUNNING;
    RoundTurn = 1;

    for (uint32 l_PetId = 0; l_PetId < MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS; ++l_PetId)
    {
        if (Pets[l_PetId])
        {
            Pets[l_PetId]->States[BATTLEPET_STATE_Condition_DidDamageThisRound] = 0;
            Pets[l_PetId]->States[BATTLEPET_STATE_Condition_WasDamagedThisTurn] = 0;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //Cast abilities (on round start)

    RoundFirstTeamCasting = GetFirstAttackingTeam();

    uint32 l_FirstTeam = RoundFirstTeamCasting;

    Teams[l_FirstTeam]->DoCasts(PETBATTLE_ABILITY_TURN0_PROC_ON_ROUND_START);
    Teams[!l_FirstTeam]->DoCasts(PETBATTLE_ABILITY_TURN0_PROC_ON_ROUND_START);

    //////////////////////////////////////////////////////////////////////////
    //Cast abilities

    l_FirstTeam = RoundFirstTeamCasting;

    Teams[l_FirstTeam]->DoCasts();
    Teams[!l_FirstTeam]->DoCasts();

    //////////////////////////////////////////////////////////////////////////
    {
        PetBattleEvent l_Event(PETBATTLE_EVENT_AURA_PROCESSING_BEGIN);
        l_Event.UpdateFrontPet();

        RoundEvents.push_back(l_Event);
    }

    //////////////////////////////////////////////////////////////////////////
    /// Process all auras

    l_FirstTeam = RoundFirstTeamCasting;
    for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
        if (!(*l_It)->Expired && Pets[(*l_It)->CasterPetID]->TeamID == l_FirstTeam)
            (*l_It)->Process(this);

    for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
        if (!(*l_It)->Expired && Pets[(*l_It)->CasterPetID]->TeamID == !l_FirstTeam)
            (*l_It)->Process(this);

    //////////////////////////////////////////////////////////////////////////
    {
        PetBattleEvent l_Event(PETBATTLE_EVENT_AURA_PROCESSING_END);
        l_Event.UpdateFrontPet();

        RoundEvents.push_back(l_Event);
    }
    //////////////////////////////////////////////////////////////////////////

    /// here seed some state (speed, power)

    for (uint32 l_I = 0; l_I < RoundPetSpeedUpdate.size(); l_I++)
    {
        PetBattleEvent l_Event(PETBATTLE_EVENT_SET_SPEED, RoundPetSpeedUpdate[l_I].first, 0, RoundPetSpeedUpdate[l_I].second, RoundTurn++, 0, 1);
        l_Event.UpdateSpeed(RoundPetSpeedUpdate[l_I].first, Pets[RoundPetSpeedUpdate[l_I].first]->GetSpeed());

        RoundEvents.push_back(l_Event);
    }
   
    //////////////////////////////////////////////////////////////////////////
    /// Ability proc on round end

    l_FirstTeam = RoundFirstTeamCasting;
    Teams[l_FirstTeam]->DoCasts(PETBATTLE_ABILITY_TURN0_PROC_ON_ROUND_END);
    Teams[!l_FirstTeam]->DoCasts(PETBATTLE_ABILITY_TURN0_PROC_ON_ROUND_END);

    // Passive: Humanoid
    for (uint32 l_PetId = 0; l_PetId < MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS; ++l_PetId)
        if (Pets[l_PetId] && Pets[l_PetId]->States[BATTLEPET_STATE_Passive_Humanoid] && Pets[l_PetId]->States[BATTLEPET_STATE_Condition_DidDamageThisRound])
            Cast(l_PetId, 726, 1, PETBATTLE_ABILITY_TURN0_PROC_ON_NONE, PETBATTLE_CAST_TRIGGER_NONE);

    // Passive: Dragonkin
    if (Pets[Teams[0]->ActivePetID]->IsAlive() && Pets[Teams[1]->ActivePetID]->IsAlive())
        for (size_t l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; ++l_TeamID)
            if (Pets[Teams[l_TeamID]->ActivePetID]->States[BATTLEPET_STATE_Passive_Dragon])
                if (Pets[Teams[!l_TeamID]->ActivePetID]->Health * 100 / Pets[Teams[!l_TeamID]->ActivePetID]->GetMaxHealth() < 50)
                    AddAura(Teams[l_TeamID]->ActivePetID, Teams[l_TeamID]->ActivePetID, 245, 1, 0, 0, 0);

    //////////////////////////////////////////////////////////////////////////
    /// clear expired auras
    PetBattleAuraList l_AurasToRemove;

    for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
        if ((*l_It)->Expired)
            l_AurasToRemove.push_back(*l_It);

    for (PetBattleAuraList::iterator l_It = l_AurasToRemove.begin(); l_It != l_AurasToRemove.end(); ++l_It)
    {
        PetAuras.remove((*l_It));
        delete (*l_It);
    }

    //////////////////////////////////////////////////////////////////////////
    /// Update abilities turns

    for (uint32 l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; l_CurrentTeamID++)
    {
        Teams[l_CurrentTeamID]->activeAbilityTurn++;
     
        if (Teams[l_CurrentTeamID]->activeAbilityTurn > Teams[l_CurrentTeamID]->activeAbilityTurnMax)
        {
            Teams[l_CurrentTeamID]->ActiveAbilityId = 0;
            Teams[l_CurrentTeamID]->activeAbilityTurn = 0;
            Teams[l_CurrentTeamID]->activeAbilityTurnMax = 0;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    /// Update abilities cooldown

    for (uint32 l_CurrentPetSlot = 0; l_CurrentPetSlot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetSlot++)
    {
        for (uint32 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
            if (Pets[l_CurrentPetSlot] && Pets[l_CurrentPetSlot]->Cooldowns[l_CurrentAbilitySlot] != -1)
                Pets[l_CurrentPetSlot]->Cooldowns[l_CurrentAbilitySlot]--;
    }

    //////////////////////////////////////////////////////////////////////////
    // Send round result

    if (RoundResult == PETBATTLE_ROUND_RESULT_NONE)
        RoundResult = PETBATTLE_ROUND_RESULT_NORMAL;

    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if (!Teams[l_CurrentTeamID]->PlayerGuid)
            continue;

        Player* l_Player = ObjectAccessor::GetObjectInWorld(Teams[l_CurrentTeamID]->PlayerGuid, (Player*)NULL);

        if (!l_Player)
            continue;

        l_Player->GetSession()->SendPetBattleRoundResult(this);
    }

    //////////////////////////////////////////////////////////////////////////
    // To next turn

    ++Turn;
    RoundEvents.clear();
    RoundDeadPets.clear();
    RoundPetSpeedUpdate.clear();

    RoundResult = PETBATTLE_ROUND_RESULT_NONE;
    RoundStatus = PETBATTLE_ROUND_FINISHED;
}
/// Finish the battle
void PetBattle::Finish(uint32 p_WinnerTeamID, bool p_Aborted)
{
    if (p_Aborted)
    {
        /// Webbed pet can't forfeit or swap out
        if (Pets[Teams[p_WinnerTeamID]->ActivePetID]
            && Pets[Teams[p_WinnerTeamID]->ActivePetID]->IsAlive()
            && Pets[Teams[p_WinnerTeamID]->ActivePetID]->States[BATTLEPET_STATE_Mechanic_IsWebbed])
            return;
    }

    RoundStatus     = PETBATTLE_ROUND_FINISHED;
    WinnerTeamId    = p_WinnerTeamID;

    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if (Teams[l_CurrentTeamID]->PlayerGuid)
        {
            Player* l_Player = ObjectAccessor::GetObjectInWorld(Teams[l_CurrentTeamID]->PlayerGuid, (Player*)NULL);

            if (!l_Player)
                continue;

            uint32 l_AvailablePetCount = Teams[l_CurrentTeamID]->GetAvailablesPets().size();

            for (size_t l_CurrentPetID = 0; l_CurrentPetID < Teams[l_CurrentTeamID]->TeamPetCount; ++l_CurrentPetID)
            {
                BattlePetInstance::Ptr l_CurrentPet = Teams[l_CurrentTeamID]->TeamPets[l_CurrentPetID];

                if (!l_CurrentPet)
                    continue;

                if (l_CurrentPet->Health < 0)
                    l_CurrentPet->Health = 0;

                if (p_Aborted && p_WinnerTeamID != l_CurrentTeamID)
                    l_CurrentPet->Health = AddPct(l_CurrentPet->Health, -GetForfeitHealthPenalityPct());    ///< 10% health loose on forfeit

                if (p_WinnerTeamID == l_CurrentTeamID && l_AvailablePetCount && BattleType == PETBATTLE_TYPE_PVE && l_CurrentPet->IsAlive()
                    && l_CurrentPet->Level < BATTLEPET_MAX_LEVEL && FightedPets.find(l_CurrentPet->ID) != FightedPets.end())
                {
                    uint32  l_MyTeamPetCount = Teams[l_CurrentTeamID]->TeamPetCount; ///< l_MyTeamPetCount is never read 01/18/16
                    uint32  l_XpEarn = 0;
                    float   l_XpMod[] = { 1.0f, 1.0f, 0.5f };

                    for (uint32 l_OpponentTeamCurrentPet = 0; l_OpponentTeamCurrentPet < Teams[PETBATTLE_PVE_TEAM_ID]->TeamPetCount; l_OpponentTeamCurrentPet++)
                    {
                        if (!Teams[PETBATTLE_PVE_TEAM_ID]->TeamPets[l_OpponentTeamCurrentPet] || FightedPets.find(Teams[PETBATTLE_PVE_TEAM_ID]->TeamPets[l_OpponentTeamCurrentPet]->ID) == FightedPets.end())
                            continue;

                        l_XpEarn += (float(l_CurrentPet->GetXPEarn(Teams[PETBATTLE_PVE_TEAM_ID]->TeamPets[l_OpponentTeamCurrentPet]->ID)) * l_XpMod[l_OpponentTeamCurrentPet]) / l_AvailablePetCount;
                    }

                    AddPct(l_XpEarn, l_Player->GetTotalAuraModifier(SPELL_AURA_MOD_BATTLE_PET_XP_PCT));
                    uint32 l_XpToNextLevel = l_CurrentPet->GetMaxXPForCurrentLevel();

                    if ((l_CurrentPet->XP + l_XpEarn) >= l_XpToNextLevel)
                    {
                        l_CurrentPet->XP = (l_CurrentPet->XP + l_XpEarn) - l_XpToNextLevel;
                        l_CurrentPet->Level++;
                        l_CurrentPet->UpdateStats();
                        l_CurrentPet->Health = l_CurrentPet->InfoMaxHealth;

                        if (l_CurrentPet->Level == BATTLEPET_MAX_LEVEL)
                            l_CurrentPet->XP = 0;

                        BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(l_CurrentPet->Species);
                        if (l_SpeciesInfo)
                            l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET, l_CurrentPet->Level, l_SpeciesInfo->type);

                        if (l_CurrentPet->Level >= 3)
                        {
                            /// Quest progress for 12 x Level Up!
                            l_Player->KilledMonsterCredit(65876);
                        }
                    }
                    else
                        l_CurrentPet->XP += l_XpEarn;

                    if (p_WinnerTeamID == l_CurrentTeamID)
                        l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_PETBATTLE, 1);
                }

                l_CurrentPet->UpdateOriginalInstance();
            }

            if (BattleType == PETBATTLE_TYPE_PVE && PveBattleType == PVE_PETBATTLE_WILD)
            {
                /// Quest progress for 12 x Learning the Ropes
                l_Player->KilledMonsterCredit(65355);
            }

            if (BattleType == PETBATTLE_TYPE_PVE && l_CurrentTeamID == p_WinnerTeamID && Teams[l_CurrentTeamID]->CapturedPet != PETBATTLE_NULL_ID)
            {
                if (Pets[Teams[l_CurrentTeamID]->CapturedPet]->Level >= 16 && Pets[Teams[l_CurrentTeamID]->CapturedPet]->Level <= 20)
                    Pets[Teams[l_CurrentTeamID]->CapturedPet]->Level -= 1;
                else if (Pets[Teams[l_CurrentTeamID]->CapturedPet]->Level >= 21 && Pets[Teams[l_CurrentTeamID]->CapturedPet]->Level <= 25)
                    Pets[Teams[l_CurrentTeamID]->CapturedPet]->Level -= 2;

                if (Pets[Teams[l_CurrentTeamID]->CapturedPet]->Health <= CalculatePct(Pets[Teams[l_CurrentTeamID]->CapturedPet]->InfoMaxHealth, 5))
                {
                    if (AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievement(6571))
                        l_Player->CompletedAchievement(l_Achievement);
                }

                Pets[Teams[l_CurrentTeamID]->CapturedPet]->UpdateStats();
                Pets[Teams[l_CurrentTeamID]->CapturedPet]->Health = Pets[Teams[l_CurrentTeamID]->CapturedPet]->InfoMaxHealth;
                Pets[Teams[l_CurrentTeamID]->CapturedPet]->Slot = PETBATTLE_NULL_SLOT;
                Pets[Teams[l_CurrentTeamID]->CapturedPet]->AddToPlayer(l_Player);

                BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(Pets[Teams[l_CurrentTeamID]->CapturedPet]->Species);
                if (l_SpeciesInfo)
                {
                    l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET, l_SpeciesInfo->entry);
                    l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_SPECIFIC_BATTLEPET, l_SpeciesInfo->id);
                    l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_COMBAT, 1, Pets[Teams[l_CurrentTeamID]->CapturedPet]->Quality);
                }

                /// Quest progress for 12 x Got One!
                l_Player->KilledMonsterCredit(65356);
            }

            if (BattleType == PETBATTLE_TYPE_PVE && PveBattleType == PVE_PETBATTLE_TRAINER && l_CurrentTeamID == p_WinnerTeamID)
            {
                Creature* l_Trainer = ObjectAccessor::GetObjectInOrOutOfWorld(Teams[PETBATTLE_PVE_TEAM_ID]->OwnerGuid, (Creature*)nullptr);

                if (l_Trainer)
                    l_Player->QuestObjectiveSatisfy(l_Trainer->GetEntry(), 1, QUEST_OBJECTIVE_TYPE_PET_BATTLE_TAMER, l_Trainer->GetGUID());
            }
        }
    }

    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if (Teams[l_CurrentTeamID]->PlayerGuid)
        {
            Player* l_Player = ObjectAccessor::GetObjectInWorld(Teams[l_CurrentTeamID]->PlayerGuid, (Player*)NULL);

            if (!l_Player)
                continue;

            //TODO: update achievement criteria
            l_Player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);

            //////////////////////////////////////////////////////////////////////////
            // Send battle result
            CombatResult = p_Aborted ? PETBATTLE_RESULT_ABANDON : (l_CurrentTeamID == p_WinnerTeamID ? PETBATTLE_RESULT_WON : PETBATTLE_RESULT_LOOSE);

            l_Player->GetSession()->SendPetBattleFinalRound(this);
            l_Player->SetControlled(false, UNIT_STATE_ROOT);
            l_Player->SummonLastSummonedBattlePet();
            l_Player->ReloadPetBattles();
            l_Player->_petBattleId = 0;

            if (BattleType == PETBATTLE_TYPE_PVP_MATCHMAKING)
                l_Player->TeleportTo(l_Player->m_recallMap, l_Player->m_recallX, l_Player->m_recallY, l_Player->m_recallZ, l_Player->m_recallO);
        }
        else
        {
            if (BattleType == PETBATTLE_TYPE_PVE && PveBattleType == PVE_PETBATTLE_WILD)
            {
                for (size_t l_CurrentPetID = 0; l_CurrentPetID < Teams[l_CurrentTeamID]->TeamPetCount; ++l_CurrentPetID)
                {
                    BattlePetInstance::Ptr l_CurrentPet = Teams[l_CurrentTeamID]->TeamPets[l_CurrentPetID];

                    if (!l_CurrentPet || l_CurrentPet->OriginalCreature)
                        continue;

                    Creature* l_WildPet = ObjectAccessor::GetObjectInOrOutOfWorld(l_CurrentPet->OriginalCreature, (Creature*)NULL);

                    if (!l_WildPet)
                        continue;

                    l_WildPet->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
                    l_WildPet->SetControlled(false, UNIT_STATE_ROOT);
                    l_WildPet->_petBattleId = 0;

                    sWildBattlePetMgr->LeaveBattle(l_WildPet, p_WinnerTeamID != PETBATTLE_PVE_TEAM_ID);
                }
            }
        }
    }

    BattleStatus = PETBATTLE_STATUS_FINISHED;
}

//////////////////////////////////////////////////////////////////////////

/// Update the pet battle
void PetBattle::Update(uint32 p_TimeDiff)
{
    if (BattleStatus != PETBATTLE_STATUS_RUNNING)
        return;

    if (RoundStatus == PETBATTLE_ROUND_RUNNING)
        return;

    m_UpdateTimer.Update(p_TimeDiff);

    if (m_UpdateTimer.Passed())
    {
        m_UpdateTimer.Reset();

        Teams[PETBATTLE_TEAM_1]->Update();
        bool l_BattleIsFinish = Teams[PETBATTLE_TEAM_2]->Update();

        // if the first update return battle finish state, it's handle in the second update
        if (l_BattleIsFinish)
        {
            Teams[PETBATTLE_TEAM_1]->Ready = false;
            Teams[PETBATTLE_TEAM_2]->Ready = false;
            return;
        }

        if (!Teams[PETBATTLE_TEAM_1]->Ready && Teams[PETBATTLE_TEAM_1]->ActivePetID != PETBATTLE_NULL_ID && !Pets[Teams[PETBATTLE_TEAM_1]->ActivePetID]->CanAttack())
        {
            Teams[PETBATTLE_TEAM_1]->Ready = true;
            Teams[PETBATTLE_TEAM_1]->ActiveAbilityId = 0;
            Teams[PETBATTLE_TEAM_1]->activeAbilityTurn = 0;
            Teams[PETBATTLE_TEAM_1]->activeAbilityTurnMax = 0;
        }

        if (!Teams[PETBATTLE_TEAM_2]->Ready && Teams[PETBATTLE_TEAM_2]->ActivePetID != PETBATTLE_NULL_ID && !Pets[Teams[PETBATTLE_TEAM_2]->ActivePetID]->CanAttack())
        {
            Teams[PETBATTLE_TEAM_2]->Ready = true;
            Teams[PETBATTLE_TEAM_2]->ActiveAbilityId = 0;
            Teams[PETBATTLE_TEAM_2]->activeAbilityTurn = 0;
            Teams[PETBATTLE_TEAM_2]->activeAbilityTurnMax = 0;
        }

        if (Teams[PETBATTLE_TEAM_1]->Ready && Teams[PETBATTLE_TEAM_2]->Ready)
        {
            ProceedRound();

            Teams[PETBATTLE_TEAM_1]->Ready = false;
            Teams[PETBATTLE_TEAM_2]->Ready = false;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

/// Swap pet
void PetBattle::SwapPet(uint32 p_TeamID, int32 p_NewFrontPetID, bool p_Initial)
{
    assert(p_TeamID < MAX_PETBATTLE_TEAM);
    
    if (p_NewFrontPetID >= (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS))
        return;

    if (!Pets[p_NewFrontPetID])
        return;

    if (!Teams[p_TeamID]->CanSwap())
        return;

    int32 l_OldTeamFrontPetID = Teams[p_TeamID]->ActivePetID;
    bool l_PetChanged = p_NewFrontPetID != l_OldTeamFrontPetID;

    if (l_PetChanged)
    {
        for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
            if (!(*l_It)->Expired)
                Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_SWAP_OUT, PETBATTLE_CAST_TRIGGER_ALL);
    }

    uint32 l_EventFlags = 0;

    if (!l_PetChanged && BattleStatus != PETBATTLE_STATUS_CREATION)
        l_EventFlags |= PETBATTLE_EVENT_FLAG_SKIP_TURN;

    Teams[p_TeamID]->ActivePetID = p_NewFrontPetID;

    PetBattleEvent l_Event(PETBATTLE_EVENT_PET_SWAP, p_NewFrontPetID, l_EventFlags);
    l_Event.UpdateFrontPet((!p_Initial && p_NewFrontPetID == l_OldTeamFrontPetID) ? PETBATTLE_NULL_ID : p_NewFrontPetID);

    RoundEvents.push_back(l_Event);

    if (l_PetChanged)
    {
        for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
            if (!(*l_It)->Expired)
                Cast((*l_It)->CasterPetID, (*l_It)->AbilityID, 0, PETBATTLE_ABILITY_TURN0_PROC_ON_SWAP_IN, PETBATTLE_CAST_TRIGGER_ALL);
    }

    Teams[p_TeamID]->ActiveAbilityId        = 0;
    Teams[p_TeamID]->activeAbilityTurn      = 0;
    Teams[p_TeamID]->activeAbilityTurnMax   = 0;

    if (!p_Initial)
        Teams[p_TeamID]->Ready = true;

    FightedPets[p_NewFrontPetID] = true;
}

//////////////////////////////////////////////////////////////////////////

/// Check can cast
bool PetBattle::CanCast(uint32 p_TeamID, uint32 p_AbilityID)
{
    assert(p_TeamID < MAX_PETBATTLE_TEAM);

    PetBattleTeam* l_Team = Teams[p_TeamID];

    if (!l_Team->CanCastAny())
        return false;

    if (!sBattlePetAbilityStore.LookupEntry(p_AbilityID))
        return false;

    bool l_HaveAbility = false;

    for (uint32 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
        if (Pets[l_Team->ActivePetID]->Abilities[l_CurrentAbilitySlot] == p_AbilityID && Pets[l_Team->ActivePetID]->Cooldowns[l_CurrentAbilitySlot] <= 0)
            l_HaveAbility = true;

    return l_HaveAbility;
}
/// Prepare cast
void PetBattle::PrepareCast(uint32 p_TeamID, uint32 p_AbilityID)
{
    BattlePetAbilityEntry const* l_AbilityInfo = sBattlePetAbilityStore.LookupEntry(p_AbilityID); ///< l_AbilityInfo is never read 01/18/16

    PetBattleAbilityTurn l_AbilityTurn;
    memset(&l_AbilityTurn, 0, sizeof(l_AbilityTurn));

    uint32 l_MaxTurnID = 0;

    for (uint32 l_AbilityTurnId = 0; l_AbilityTurnId < sBattlePetAbilityTurnStore.GetNumRows(); ++l_AbilityTurnId)
    {
        BattlePetAbilityTurnEntry const* abilityTurnInfo = sBattlePetAbilityTurnStore.LookupEntry(l_AbilityTurnId);
        if (!abilityTurnInfo || abilityTurnInfo->abilityId != p_AbilityID)
            continue;

        l_MaxTurnID = std::max(l_MaxTurnID, abilityTurnInfo->turn);
    }

    Teams[p_TeamID]->ActiveAbilityId        = p_AbilityID;
    Teams[p_TeamID]->activeAbilityTurn      = 1;
    Teams[p_TeamID]->activeAbilityTurnMax   = l_MaxTurnID;

    Teams[p_TeamID]->Ready = true;
}
/// Cast an ability
PetBattleCastResult PetBattle::Cast(uint32 p_CasterPetID, uint32 p_AbilityID, uint32 p_Turn, uint32 p_Turn0ProcCondition, uint32 p_TriggerFlag)
{
    if (!Pets[p_CasterPetID])
        return PETBATTLE_CAST_INTERNAL_ERROR;

    if (RoundResult == PETBATTLE_ROUND_RESULT_CATCH_OR_KILL)
        return PETBATTLE_CAST_OK;

    /// check if the ability exist
    BattlePetAbilityEntry const* l_AbilityInfo = sBattlePetAbilityStore.LookupEntry(p_AbilityID);

    if (!l_AbilityInfo)
        return PETBATTLE_CAST_INVALID_ID;

    // States
    if (!p_Turn && p_TriggerFlag == PETBATTLE_CAST_TRIGGER_NONE)
    {
        for (uint32 l_AbilityStateId = 0; l_AbilityStateId != sBattlePetAbilityStateStore.GetNumRows(); ++l_AbilityStateId)
        {
            BattlePetAbilityStateEntry const* l_AbilityStateInfo = sBattlePetAbilityStateStore.LookupEntry(l_AbilityStateId);
            if (!l_AbilityStateInfo || l_AbilityStateInfo->abilityId != p_AbilityID)
                continue;

            SetPetState(p_CasterPetID, p_CasterPetID, 0, l_AbilityStateInfo->stateId, Pets[p_CasterPetID]->States[l_AbilityStateInfo->stateId] + l_AbilityStateInfo->value);
        }
    }

    PetBattleAbilityTurn l_AbilityTurn;
    memset(&l_AbilityTurn, 0, sizeof(l_AbilityTurn));

    for (uint32 l_AbilityTurnId = 0; l_AbilityTurnId < sBattlePetAbilityTurnStore.GetNumRows(); ++l_AbilityTurnId)
    {
        BattlePetAbilityTurnEntry const* l_AbilityTurnInfo = sBattlePetAbilityTurnStore.LookupEntry(l_AbilityTurnId);

        if (!l_AbilityTurnInfo || l_AbilityTurnInfo->abilityId != p_AbilityID || l_AbilityTurnInfo->turn != p_Turn)
            continue;

        if (l_AbilityTurnInfo->turn == 0 && l_AbilityTurnInfo->procType != p_Turn0ProcCondition)
            continue;

        for (uint32 l_AbilityEffectId = 0; l_AbilityEffectId < sBattlePetAbilityEffectStore.GetNumRows(); ++l_AbilityEffectId)
        {
            BattlePetAbilityEffectEntry const* l_AbilityEffectInfo = sBattlePetAbilityEffectStore.LookupEntry(l_AbilityEffectId);

            if (!l_AbilityEffectInfo || l_AbilityEffectInfo->abilityTurnId != l_AbilityTurnInfo->id)
                continue;

            PetBattleAbilityEffect l_AbilityEffect;
            l_AbilityEffect.EffectInfo          = l_AbilityEffectInfo;
            l_AbilityEffect.AbilityTurn         = &l_AbilityTurn;
            l_AbilityEffect.PetBattleInstance   = this;
            l_AbilityEffect.Caster              = p_CasterPetID;
            l_AbilityEffect.StopChain           = false;
            l_AbilityEffect.AbilityID           = p_AbilityID;
            l_AbilityEffect.IsTriggered         = l_AbilityInfo->flags & BATTLEPET_ABILITY_FLAG_TRIGGER;
            l_AbilityEffect.ReportFailAsImmune  = false;
            l_AbilityEffect.SelectTargets();

            if (!l_AbilityEffect.Execute())
                l_AbilityTurn.ChainFailure |= 1 << (l_AbilityEffectInfo->effectIndex - 1);

            if (l_AbilityEffect.StopChain)
                break;

            if (RoundResult == PETBATTLE_ROUND_RESULT_CATCH_OR_KILL)
                return PETBATTLE_CAST_OK;
        }
    }

    if (!(p_TriggerFlag & PETBATTLE_CAST_TRIGGER_IGNORE_COOLDOWN))
    {
        int32 l_AbilitySlot = -1;

        for (uint32 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
        {
            if (Pets[p_CasterPetID]->Abilities[l_CurrentAbilitySlot] == p_AbilityID)
            {
                l_AbilitySlot = l_CurrentAbilitySlot;
                break;
            }
        }

        if (l_AbilitySlot != -1)
            Pets[p_CasterPetID]->Cooldowns[l_AbilitySlot] = l_AbilityInfo->cooldown;
    }

    return PETBATTLE_CAST_OK;
}

//////////////////////////////////////////////////////////////////////////

/// Add aura
bool PetBattle::AddAura(uint32 p_CasterPetID, uint32 p_TargetPetID, uint32 p_AbilityID, int32 p_Duration, int32 p_MaxAllowed, uint32 p_FromAbilityEffectID, uint32 p_Flags)
{
    PetBattleAura*  l_OldAura = NULL;
    uint32          l_Slot = 0;
    int32           l_AuraCount = 0;

    if (!p_MaxAllowed)
        p_MaxAllowed = 1;

    // Search same auras
    for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
    {
        l_Slot = (*l_It)->ID;

        if (!(*l_It)->Expired && (*l_It)->AbilityID == p_AbilityID && (*l_It)->TargetPetID == p_TargetPetID)
        {
            ++l_AuraCount;
            if (!l_OldAura)
            {
                l_Slot = (*l_It)->ID;
                l_OldAura = (*l_It);
            }
        }
        else if (!l_AuraCount && (*l_It)->ID > l_Slot)
            l_Slot = (*l_It)->ID;
    }

    if (!l_AuraCount)
        ++l_Slot;

    // Event
    PetBattleEvent l_Event(l_AuraCount ? PETBATTLE_EVENT_BUFF_CHANGE : PETBATTLE_EVENT_BUFF_APPLY, p_CasterPetID, p_Flags, p_FromAbilityEffectID, RoundTurn++, 0, 1);
    l_Event.UpdateBuff(p_TargetPetID, l_Slot, p_AbilityID, p_Duration, 0);
    RoundEvents.push_back(l_Event);

    if (p_Flags)
        return false;

    // Remove old aura if at max allowed count
    if (!p_Flags && p_MaxAllowed && l_AuraCount >= p_MaxAllowed)
    {
        int l_RemainingAuraToRemove = 1 + l_AuraCount - p_MaxAllowed;
        for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end() && l_RemainingAuraToRemove > 0; ++l_It)
        {
            if (!(*l_It)->Expired && (*l_It)->AbilityID == p_AbilityID && (*l_It)->TargetPetID == p_TargetPetID)
            {
                (*l_It)->Expire(this);
                --l_RemainingAuraToRemove;
            }
        }
    }

    // Create aura
    PetBattleAura*  l_Aura = new PetBattleAura();
    l_Aura->AbilityID   = p_AbilityID;
    l_Aura->TriggerId   = p_FromAbilityEffectID;
    l_Aura->CasterPetID = p_CasterPetID;
    l_Aura->Duration    = p_Duration;
    l_Aura->MaxDuration = p_Duration;
    l_Aura->Turn        = 1;
    l_Aura->TargetPetID = p_TargetPetID;
    l_Aura->ID          = l_Slot;
    l_Aura->Expired     = false;

    PetAuras.push_back(l_Aura);
    l_Aura->Apply(this);

    return true;
}
/// Set pet state
void PetBattle::SetPetState(uint32 p_SourcePetID, uint32 p_TargetPetID, uint32 p_FromAbilityEffectID, uint32 p_State, int32 p_Value, bool p_FromCapture, uint32 p_Flags)
{
    if (p_State >= NUM_BATTLEPET_STATES)
    {
        sLog->outSlack("#jarvis", "danger", true, "PetBattle::SetPetState %u %u %u", p_FromAbilityEffectID, p_State, p_Value);
        return;
    }

    if (Pets[p_TargetPetID]->States[p_State] == p_Value)
        return;

    Pets[p_TargetPetID]->States[p_State] = p_Value;

    BattlePetStateEntry const* l_StateInfo = sBattlePetStateStore.LookupEntry(p_State);
    if (l_StateInfo && l_StateInfo->flags && !p_FromCapture)
    {
        PetBattleEvent l_Event(PETBATTLE_EVENT_SET_STATE, p_SourcePetID, p_Flags, p_FromAbilityEffectID, RoundTurn++, 0, 1);
        l_Event.UpdateState(p_TargetPetID, p_State, p_Value);
        RoundEvents.push_back(l_Event);
    }
}
/// kill
void PetBattle::Kill(int8 p_Killer, int8 p_Target, uint32 p_KillerAbibilityEffectID, bool p_FromCapture, uint32 p_Flags)
{
    if (Teams[Pets[p_Target]->TeamID]->ActivePetID == p_Target)
    {
        Teams[Pets[p_Target]->TeamID]->ActiveAbilityId = 0;
        Teams[Pets[p_Target]->TeamID]->activeAbilityTurn = 0;
        Teams[Pets[p_Target]->TeamID]->activeAbilityTurnMax = 0;
    }

    for (PetBattleAuraList::iterator l_It = PetAuras.begin(); l_It != PetAuras.end(); ++l_It)
        if ((*l_It)->TargetPetID == p_Target)
            (*l_It)->Expire(this);

    SetPetState(p_Killer, p_Target, p_KillerAbibilityEffectID, BATTLEPET_STATE_Is_Dead, 1, p_FromCapture, p_Flags);

    RoundResult = PETBATTLE_ROUND_RESULT_CATCH_OR_KILL;
}
/// Catch
void PetBattle::Catch(int8 p_Catcher, int8 p_CatchedTarget, uint32 p_FromAbilityEffectID)
{
    Kill(p_Catcher, p_CatchedTarget, p_FromAbilityEffectID);

    Teams[Pets[p_Catcher]->TeamID]->CapturedPet = p_CatchedTarget;
    CatchedPetId = p_CatchedTarget;

    RoundDeadPets.push_back(p_CatchedTarget);

    RoundResult = PETBATTLE_ROUND_RESULT_CATCH_OR_KILL;
}

//////////////////////////////////////////////////////////////////////////

/// Get first attacking team
uint32 PetBattle::GetFirstAttackingTeam()
{
    std::shared_ptr<BattlePetInstance> l_ActivePets[2];
    l_ActivePets[0] = Pets[Teams[0]->ActivePetID];
    l_ActivePets[1] = Pets[Teams[1]->ActivePetID];

    //////////////////////////////////////////////////////////////////////////
    /// Deduce the first caster (based on front pets speed)
    int32 l_Pet0Speed = l_ActivePets[0]->GetSpeed();
    int32 l_Pet1Speed = l_ActivePets[1]->GetSpeed();

    return (l_Pet0Speed == l_Pet1Speed) ? rand() & 1 : l_Pet1Speed > l_Pet0Speed;
}

//////////////////////////////////////////////////////////////////////////

/// Get forfeit health penalty pct
int32 PetBattle::GetForfeitHealthPenalityPct()
{
    if (BattleType != PETBATTLE_TYPE_PVE)
        return 0;

    // TODO
    return 10;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
PetBattleSystem::PetBattleSystem()
{
    m_MaxPetBattleID = 1;
    m_DeleteUpdateTimer.SetInterval(PETBATTLE_DELETE_INTERVAL);
    m_LFBAvgWaitTime = 0;
    m_LFBNumWaitTimeAvg = 0;
    m_LFBRequestsUpdateTimer.SetInterval(PETBATTLE_LFB_INTERVAL);
}
/// Destructor
PetBattleSystem::~PetBattleSystem()
{
    /// Clean all battles
    for (std::map<uint64, PetBattle*>::iterator l_Iterator = m_PetBattles.begin(); l_Iterator != m_PetBattles.end(); l_Iterator++)
        delete l_Iterator->second;

    /// Clear all requests
    for (std::map<uint64, PetBattleRequest*>::iterator l_Iterator = m_Requests.begin(); l_Iterator != m_Requests.end(); l_Iterator++)
        delete l_Iterator->second;
}

//////////////////////////////////////////////////////////////////////////

/// Create a new battle with an unique auto incremented ID
PetBattle* PetBattleSystem::CreateBattle()
{
    uint64 l_BattleID = ++m_MaxPetBattleID;

    m_PetBattles[l_BattleID] = new PetBattle();
    m_PetBattles[l_BattleID]->ID = l_BattleID;

    return m_PetBattles[l_BattleID];
}
/// Create a new pet battle request (actually we use requested guid (player guid) as request id)
PetBattleRequest* PetBattleSystem::CreateRequest(uint64 p_RequesterGuid)
{
    m_Requests[p_RequesterGuid] = new PetBattleRequest();
    m_Requests[p_RequesterGuid]->RequesterGuid = p_RequesterGuid;

    return m_Requests[p_RequesterGuid];
}

//////////////////////////////////////////////////////////////////////////

/// Get a battle by his unique id
PetBattle* PetBattleSystem::GetBattle(uint64 p_BattleID)
{
    std::map<uint64, PetBattle*>::iterator l_Iterator = m_PetBattles.find(p_BattleID);

    if (l_Iterator == m_PetBattles.end())
        return 0;

    return l_Iterator->second;
}
/// Get a request by his requested guid (player guid)
PetBattleRequest* PetBattleSystem::GetRequest(uint64 p_RequesterGuid)
{
    std::map<uint64, PetBattleRequest*>::iterator l_Iterator = m_Requests.find(p_RequesterGuid);

    if (l_Iterator == m_Requests.end())
        return 0;

    return l_Iterator->second;
}

//////////////////////////////////////////////////////////////////////////

/// Remove an battle and delete it
void PetBattleSystem::RemoveBattle(uint64 p_BattleID)
{
    PetBattle* l_Battle = GetBattle(p_BattleID);

    if (l_Battle)
    {
        delete l_Battle;

        m_PetBattles[p_BattleID] = 0;
        m_PetBattles.erase(p_BattleID);
    }
}
/// Remove an request and delete it
void PetBattleSystem::RemoveRequest(uint64 p_RequesterGuid)
{
    PetBattleRequest* l_Request = GetRequest(p_RequesterGuid);

    if (l_Request)
    {
        delete l_Request;
        l_Request = NULL;

        m_Requests[p_RequesterGuid] = 0;
        m_Requests.erase(p_RequesterGuid);
    }
}

//////////////////////////////////////////////////////////////////////////

/// Join queue for pvp matchmaking
void PetBattleSystem::JoinQueue(Player* p_Player)
{
    /// Pandaren case
    if (p_Player->GetTeamId() == TEAM_NEUTRAL)
    {
        p_Player->GetSession()->SendPetBattleQueueStatus(0, 1, LFBUpdateStatus::LFB_CANT_JOIN_DUE_TO_UNSELECTED_FACTION, 0);
        return;
    }

    // Load player pets
    BattlePet::Ptr * l_PetSlots = p_Player->GetBattlePetCombatTeam();
    uint32 l_Weight = 0;

    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        if (!l_PetSlots[l_CurrentPetSlot])
            continue;

        l_Weight += l_PetSlots[l_CurrentPetSlot]->Level;
    }

    std::lock_guard<std::mutex> l_Lock(m_LFBRequestsMutex);

    if (m_LFBRequests[p_Player->GetGUID()] != nullptr)
        return;

    LFBTicket* l_Ticket = new LFBTicket();
    l_Ticket->State             = LFBState::LFB_STATE_QUEUED;
    l_Ticket->JoinTime          = time(nullptr);
    l_Ticket->TicketID          = 1;
    l_Ticket->MatchingOpponent  = nullptr;
    l_Ticket->ProposalAnswer    = LFBAnswer::LFB_ANSWER_PENDING;
    l_Ticket->Weight            = l_Weight;
    l_Ticket->RequesterGUID     = p_Player->GetGUID();
    l_Ticket->TeamID            = p_Player->GetTeamId();

    p_Player->GetSession()->SendBattlePetJournalLockAcquired();
    p_Player->GetSession()->SendPetBattleQueueStatus(l_Ticket->JoinTime, l_Ticket->TicketID, LFBUpdateStatus::LFB_JOIN_QUEUE, m_LFBAvgWaitTime);

    m_LFBRequests[p_Player->GetGUID()] = l_Ticket;
}

/// On proposal response
void PetBattleSystem::ProposalResponse(Player* p_Player, bool p_Accept)
{
    std::lock_guard<std::mutex> l_Lock(m_LFBRequestsMutex);

    if (m_LFBRequests[p_Player->GetGUID()] == nullptr)
        return;

    LFBTicket* l_Ticket = m_LFBRequests[p_Player->GetGUID()];

    if (l_Ticket->State != LFBState::LFB_STATE_PROPOSAL)
        return;

    l_Ticket->ProposalAnswer = p_Accept ? LFBAnswer::LFB_ANSWER_AGREE : LFBAnswer::LFB_ANSWER_DENY;
}

/// Leave queue for pvp matchmaking
void PetBattleSystem::LeaveQueue(Player* p_Player)
{
    std::lock_guard<std::mutex> l_Lock(m_LFBRequestsMutex);

    if (m_LFBRequests[p_Player->GetGUID()] == nullptr)
        return;

    LFBTicket* l_Ticket = m_LFBRequests[p_Player->GetGUID()];

    switch (l_Ticket->State)
    {
        case LFBState::LFB_STATE_PROPOSAL:
        {
            if (l_Ticket->MatchingOpponent)
            {
                Player* l_Opponent = HashMapHolder<Player>::Find(l_Ticket->MatchingOpponent->RequesterGUID);

                l_Ticket->MatchingOpponent->MatchingOpponent = nullptr;
                l_Ticket->MatchingOpponent->State = LFBState::LFB_STATE_QUEUED;

                if (l_Opponent)
                    l_Opponent->GetSession()->SendPetBattleQueueStatus(l_Ticket->MatchingOpponent->JoinTime, l_Ticket->MatchingOpponent->TicketID, LFBUpdateStatus::LFB_JOIN_QUEUE, m_LFBAvgWaitTime);
            }

            /// Continue to other case handlers
        }
        case LFBState::LFB_STATE_FINISHED:
        case LFBState::LFB_STATE_IN_COMBAT:
        case LFBState::LFB_STATE_QUEUED:
        {
            p_Player->GetSession()->SendPetBattleQueueStatus(l_Ticket->JoinTime, l_Ticket->TicketID, LFBUpdateStatus::LFB_LEAVE_QUEUE, m_LFBAvgWaitTime);
            p_Player->GetSession()->SendBattlePetJournalLockDenied();

            delete m_LFBRequests[p_Player->GetGUID()];
            m_LFBRequests[p_Player->GetGUID()] = nullptr;

            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

/// Update the whole pet battle system (request and battles)
void PetBattleSystem::Update(uint32 p_TimeDiff)
{
    m_DeleteUpdateTimer.Update(p_TimeDiff);
    m_LFBRequestsUpdateTimer.Update(p_TimeDiff);

    if (m_DeleteUpdateTimer.Passed())
    {
        m_DeleteUpdateTimer.Reset();

        while (!m_PetBbattlesDeleteQueue.empty())
        {
            std::pair<uint64, PetBattle*> & l_Front = m_PetBbattlesDeleteQueue.front();
            RemoveBattle(l_Front.first);

            m_PetBbattlesDeleteQueue.pop();
        }
    }

    for (std::map<uint64, PetBattle*>::iterator l_Iterator = m_PetBattles.begin(); l_Iterator != m_PetBattles.end(); l_Iterator++)
    {
        PetBattle* l_Battle = l_Iterator->second;

        if (!l_Battle)
            continue;

        if (l_Battle->BattleStatus == PETBATTLE_STATUS_RUNNING)
            l_Battle->Update(p_TimeDiff);
        else if (l_Battle->BattleStatus == PETBATTLE_STATUS_FINISHED)
        {
            l_Battle->BattleStatus = PETBATTLE_STATUS_PENDING_DELETE;
            m_PetBbattlesDeleteQueue.push(std::pair<uint64, PetBattle*>(l_Iterator->first, l_Iterator->second));
        }
    }

    if (m_LFBRequestsUpdateTimer.Passed())
    {
        m_LFBRequestsUpdateTimer.Reset();

        std::lock_guard<std::mutex> l_Lock(m_LFBRequestsMutex);
        std::vector<uint64> l_TicketsToRemove;

        for (auto l_Pair : m_LFBRequests)
        {
            LFBTicket* l_Ticket = l_Pair.second;

            if (!l_Ticket)
            {
                l_TicketsToRemove.push_back(l_Pair.first);
                continue;
            }

            auto l_Count = std::count_if(l_TicketsToRemove.begin(), l_TicketsToRemove.end(), [l_Ticket](uint64 const& p_Ticket) -> bool
            {
                return p_Ticket == l_Ticket->RequesterGUID;
            });

            /// Ticket is in deletion queue don't update him so
            if (l_Count > 0)
                continue;

            uint32 l_QueuedTime = uint32(time(nullptr) - l_Ticket->JoinTime);
            uint32 l_OldNumber = m_LFBNumWaitTimeAvg++;
            m_LFBAvgWaitTime = int32((m_LFBAvgWaitTime * l_OldNumber + l_QueuedTime) / m_LFBNumWaitTimeAvg);

            switch (l_Ticket->State)
            {
                case LFBState::LFB_STATE_QUEUED:
                {
                    Player* l_Player = HashMapHolder<Player>::Find(l_Ticket->RequesterGUID);

                    if (l_Player)
                    {
                        l_Player->GetSession()->SendPetBattleQueueStatus(l_Ticket->JoinTime, l_Ticket->TicketID, LFBUpdateStatus::LFB_UPDATE_STATUS, m_LFBAvgWaitTime);

                        std::vector<LFBTicket*> l_PossibleOpponent;
                        for (auto l_Pair : m_LFBRequests)
                        {
                            LFBTicket* l_SecondTicket = l_Pair.second;

                            if (!l_SecondTicket || l_SecondTicket->State != LFBState::LFB_STATE_QUEUED || l_SecondTicket->TeamID != l_Ticket->TeamID || abs((int)(l_SecondTicket->Weight - l_Ticket->Weight)) > 5)
                                continue;

                            if (l_SecondTicket->RequesterGUID == l_Ticket->RequesterGUID)
                                continue;
                            
                            if (HashMapHolder<Player>::Find(l_SecondTicket->RequesterGUID) == nullptr)
                                continue;

                            l_PossibleOpponent.push_back(l_SecondTicket);
                        }

                        std::sort(l_PossibleOpponent.begin(), l_PossibleOpponent.end(), [this](LFBTicket const* p_A, LFBTicket const* p_B)
                        {
                            return p_A->Weight >= p_B->Weight;
                        });

                        if (!l_PossibleOpponent.empty())
                        {
                            LFBTicket* l_Left = l_Ticket;
                            LFBTicket* l_Right = l_PossibleOpponent[0];

                            Player* l_LeftPlayer = HashMapHolder<Player>::Find(l_Left->RequesterGUID);
                            Player* l_RightPlayer = HashMapHolder<Player>::Find(l_Right->RequesterGUID);

                            l_Left->MatchingOpponent = l_Right;
                            l_Right->MatchingOpponent = l_Left;

                            l_Left->State = LFBState::LFB_STATE_PROPOSAL;
                            l_Right->State = LFBState::LFB_STATE_PROPOSAL;

                            l_Left->ProposalTime = time(nullptr);
                            l_Right->ProposalTime = time(nullptr);

                            l_LeftPlayer->GetSession()->SendPetBattleQueueStatus(l_Left->JoinTime, l_Left->TicketID, LFBUpdateStatus::LFB_PROPOSAL_BEGIN, m_LFBAvgWaitTime);
                            l_RightPlayer->GetSession()->SendPetBattleQueueStatus(l_Right->JoinTime, l_Right->TicketID, LFBUpdateStatus::LFB_PROPOSAL_BEGIN, m_LFBAvgWaitTime);

                            l_LeftPlayer->GetSession()->SendPetBattleQueueProposeMatch();
                            l_RightPlayer->GetSession()->SendPetBattleQueueProposeMatch();
                        }
                    }
                    else
                        l_TicketsToRemove.push_back(l_Ticket->RequesterGUID);
                    break;
                }
                case LFBState::LFB_STATE_PROPOSAL:
                {
                    LFBTicket* l_Left = l_Ticket;
                    LFBTicket* l_Right = l_Ticket->MatchingOpponent;

                    Player* l_LeftPlayer = HashMapHolder<Player>::Find(l_Left->RequesterGUID);
                    Player* l_RightPlayer = HashMapHolder<Player>::Find(l_Right->RequesterGUID);

                    /// Enter in combat
                    if (l_Left->ProposalAnswer == LFBAnswer::LFB_ANSWER_AGREE && l_Right->ProposalAnswer == LFBAnswer::LFB_ANSWER_AGREE)
                    {
                        if (l_LeftPlayer && l_RightPlayer)
                        {
                            l_LeftPlayer->GetSession()->SendPetBattleQueueStatus(l_Left->JoinTime, l_Left->TicketID, LFBUpdateStatus::LFB_PET_BATTLE_IS_STARTED, m_LFBAvgWaitTime);
                            l_RightPlayer->GetSession()->SendPetBattleQueueStatus(l_Right->JoinTime, l_Right->TicketID, LFBUpdateStatus::LFB_PET_BATTLE_IS_STARTED, m_LFBAvgWaitTime);

                            l_Left->State   = LFBState::LFB_STATE_IN_COMBAT;
                            l_Right->State  = LFBState::LFB_STATE_IN_COMBAT;

                            std::vector<PetBattleMembersPositions> l_Positions;
                            for (PetBattleMembersPositions const& l_Data : gPetBattlePositions)
                            {
                                if (l_Data.Team != l_Left->TeamID)
                                    continue;

                                l_Positions.push_back(l_Data);
                            }

                            PetBattleMembersPositions l_Location = l_Positions[urand(0, l_Positions.size() - 1)];

                            BattlePetInstance::Ptr  l_PlayerPets[MAX_PETBATTLE_SLOTS];
                            BattlePetInstance::Ptr  l_PlayerOpposantPets[MAX_PETBATTLE_SLOTS];
                            size_t                  l_PlayerPetCount = 0;
                            size_t                  l_PlayerOpposantPetCount = 0;
                            PetBattle*              l_Battle;

                            l_Battle = sPetBattleSystem->CreateBattle();
                            l_Battle->PvPMatchMakingRequest.LocationResult                    = 0;
                            l_Battle->PvPMatchMakingRequest.TeamPosition[PETBATTLE_TEAM_1][0] = l_Location.Positions[0].x;
                            l_Battle->PvPMatchMakingRequest.TeamPosition[PETBATTLE_TEAM_1][1] = l_Location.Positions[0].y;
                            l_Battle->PvPMatchMakingRequest.TeamPosition[PETBATTLE_TEAM_1][2] = l_Location.Positions[0].z;
                            l_Battle->PvPMatchMakingRequest.TeamPosition[PETBATTLE_TEAM_2][0] = l_Location.Positions[1].x;
                            l_Battle->PvPMatchMakingRequest.TeamPosition[PETBATTLE_TEAM_2][1] = l_Location.Positions[1].y;
                            l_Battle->PvPMatchMakingRequest.TeamPosition[PETBATTLE_TEAM_2][2] = l_Location.Positions[1].z;

                            Position l_BattleCenterPosition;
                            {
                                l_BattleCenterPosition.m_positionX = (l_Location.Positions[0].x + l_Location.Positions[1].x) / 2;
                                l_BattleCenterPosition.m_positionY = (l_Location.Positions[0].y + l_Location.Positions[1].y) / 2;
                                l_BattleCenterPosition.m_positionZ = (l_Location.Positions[0].z + l_Location.Positions[1].z) / 2;
                            }

                            l_Battle->PvPMatchMakingRequest.PetBattleCenterPosition[0] = l_BattleCenterPosition.m_positionX;
                            l_Battle->PvPMatchMakingRequest.PetBattleCenterPosition[1] = l_BattleCenterPosition.m_positionY;
                            l_Battle->PvPMatchMakingRequest.PetBattleCenterPosition[2] = l_BattleCenterPosition.m_positionZ;

                            G3D::Vector3 const& l_One = l_Location.Positions[PETBATTLE_TEAM_1];
                            G3D::Vector3 const& l_Second = l_Location.Positions[PETBATTLE_TEAM_2];

                            float l_Dx = l_Second.x - l_One.x;
                            float l_Dy = l_Second.y - l_One.y;

                            float l_Angle = atan2(l_Dy, l_Dx);
                            l_Battle->PvPMatchMakingRequest.BattleFacing = (l_Angle >= 0) ? l_Angle : 2 * M_PI + l_Angle;

                            // Temporary pet buffer
                            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                            {
                                l_PlayerPets[l_CurrentPetSlot] = 0;
                                l_PlayerOpposantPets[l_CurrentPetSlot] = 0;
                            }

                            // Load player pets
                            BattlePet::Ptr * l_PetSlots = l_LeftPlayer->GetBattlePetCombatTeam();

                            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                            {
                                if (!l_PetSlots[l_CurrentPetSlot])
                                    continue;

                                if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= l_LeftPlayer->GetUnlockedPetBattleSlot())
                                    break;

                                l_PlayerPets[l_PlayerPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
                                l_PlayerPets[l_PlayerPetCount]->CloneFrom(l_PetSlots[l_CurrentPetSlot]);
                                l_PlayerPets[l_PlayerPetCount]->Slot = l_PlayerPetCount;
                                l_PlayerPets[l_PlayerPetCount]->OriginalBattlePet = l_PetSlots[l_CurrentPetSlot];

                                ++l_PlayerPetCount;
                            }

                            BattlePet::Ptr * l_PetOpposantSlots = l_RightPlayer->GetBattlePetCombatTeam();

                            for (size_t l_CurrentOpposantPetSlot = 0; l_CurrentOpposantPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentOpposantPetSlot)
                            {
                                if (!l_PetOpposantSlots[l_CurrentOpposantPetSlot])
                                    continue;

                                if (l_PlayerOpposantPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerOpposantPetCount >= l_RightPlayer->GetUnlockedPetBattleSlot())
                                    break;

                                l_PlayerOpposantPets[l_PlayerOpposantPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
                                l_PlayerOpposantPets[l_PlayerOpposantPetCount]->CloneFrom(l_PetOpposantSlots[l_CurrentOpposantPetSlot]);
                                l_PlayerOpposantPets[l_PlayerOpposantPetCount]->Slot = l_PlayerOpposantPetCount;
                                l_PlayerOpposantPets[l_PlayerOpposantPetCount]->OriginalBattlePet = l_PetOpposantSlots[l_CurrentOpposantPetSlot];

                                ++l_PlayerOpposantPetCount;
                            }

                            // Add player pets
                            l_Battle->Teams[PETBATTLE_TEAM_1]->OwnerGuid    = l_LeftPlayer->GetGUID();
                            l_Battle->Teams[PETBATTLE_TEAM_1]->PlayerGuid   = l_LeftPlayer->GetGUID();
                            l_Battle->Teams[PETBATTLE_TEAM_2]->OwnerGuid    = l_RightPlayer->GetGUID();
                            l_Battle->Teams[PETBATTLE_TEAM_2]->PlayerGuid   = l_RightPlayer->GetGUID();

                            for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
                            {
                                if (l_PlayerPets[l_CurrentPetSlot])
                                    l_Battle->AddPet(PETBATTLE_TEAM_1, l_PlayerPets[l_CurrentPetSlot]);

                                if (l_PlayerOpposantPets[l_CurrentPetSlot])
                                    l_Battle->AddPet(PETBATTLE_TEAM_2, l_PlayerOpposantPets[l_CurrentPetSlot]);
                            }

                            l_Battle->BattleType = PETBATTLE_TYPE_PVP_MATCHMAKING;
                            l_Battle->PvPMatchMakingRequest.IsPvPReady[PETBATTLE_TEAM_1] = false;
                            l_Battle->PvPMatchMakingRequest.IsPvPReady[PETBATTLE_TEAM_2] = false;

                            // Launch battle
                            l_LeftPlayer->_petBattleId = l_Battle->ID;
                            l_RightPlayer->_petBattleId = l_Battle->ID;

                            l_LeftPlayer->SetBattlegroundEntryPoint();
                            l_LeftPlayer->ScheduleDelayedOperation(DELAYED_PET_BATTLE_INITIAL);
                            l_LeftPlayer->SaveRecallPosition();
                            l_LeftPlayer->TeleportTo(l_Location.MapID, l_Location.Positions[PETBATTLE_TEAM_1].x + 0.01f, l_Location.Positions[PETBATTLE_TEAM_1].y + 0.01f, l_Location.Positions[PETBATTLE_TEAM_1].z + 0.01f, l_RightPlayer->GetOrientation() - M_PI);

                            l_RightPlayer->SetBattlegroundEntryPoint();
                            l_RightPlayer->ScheduleDelayedOperation(DELAYED_PET_BATTLE_INITIAL);
                            l_RightPlayer->SaveRecallPosition();
                            l_RightPlayer->TeleportTo(l_Location.MapID, l_Location.Positions[PETBATTLE_TEAM_2].x + 0.01f, l_Location.Positions[PETBATTLE_TEAM_2].y + 0.01f, l_Location.Positions[PETBATTLE_TEAM_2].z + 0.01f, l_LeftPlayer->GetOrientation() - M_PI);

                            l_TicketsToRemove.push_back(l_Left->RequesterGUID);
                            l_TicketsToRemove.push_back(l_Right->RequesterGUID);
                        }
                    }
                    /// Someone declined
                    else if (l_Left->ProposalAnswer == LFBAnswer::LFB_ANSWER_DENY || l_Right->ProposalAnswer == LFBAnswer::LFB_ANSWER_DENY)
                    {
                        bool p_LeftRemoved = false;
                        bool p_RightRemoved = false;

                        if (l_LeftPlayer && l_RightPlayer)
                        {
                            if (l_Left->ProposalAnswer == LFBAnswer::LFB_ANSWER_DENY)
                            {
                                l_RightPlayer->GetSession()->SendPetBattleQueueStatus(l_Left->JoinTime, l_Left->TicketID, LFBUpdateStatus::LFB_OPPONENT_PROPOSAL_DECLINED, m_LFBAvgWaitTime);
                                l_TicketsToRemove.push_back(l_Left->RequesterGUID);
                                p_LeftRemoved = true;
                            }

                            if (l_Right->ProposalAnswer == LFBAnswer::LFB_ANSWER_DENY)
                            {
                                l_LeftPlayer->GetSession()->SendPetBattleQueueStatus(l_Left->JoinTime, l_Left->TicketID, LFBUpdateStatus::LFB_OPPONENT_PROPOSAL_DECLINED, m_LFBAvgWaitTime);
                                l_TicketsToRemove.push_back(l_Right->RequesterGUID);
                                p_RightRemoved = true;
                            }

                            if (!p_LeftRemoved)
                            {
                                l_Left->State = LFBState::LFB_STATE_QUEUED;
                                l_LeftPlayer->GetSession()->SendPetBattleQueueStatus(l_Left->JoinTime, l_Left->TicketID, LFBUpdateStatus::LFB_JOIN_QUEUE, m_LFBAvgWaitTime);
                            }
                            if (!p_RightRemoved)
                            {
                                l_Right->State = LFBState::LFB_STATE_QUEUED;
                                l_RightPlayer->GetSession()->SendPetBattleQueueStatus(l_Right->JoinTime, l_Right->TicketID, LFBUpdateStatus::LFB_JOIN_QUEUE, m_LFBAvgWaitTime);
                            }
                        }
                    }
                    /// Proposal expired
                    if ((time(nullptr) - l_Ticket->ProposalTime) > PETBATTLE_LFB_PROPOSAL_TIMEOUT)
                    {
                        l_Left->MatchingOpponent = nullptr;
                        l_Right->MatchingOpponent = nullptr;

                        l_TicketsToRemove.push_back(l_Left->RequesterGUID);
                        l_TicketsToRemove.push_back(l_Right->RequesterGUID);
                        continue;
                    }

                    break;
                }
                default:
                    break;
            }
        }

        for (uint64& l_Guid : l_TicketsToRemove)
        {
            LFBTicket* l_Ticket = m_LFBRequests[l_Guid];

            if (!l_Ticket)
            {
                auto l_Itr = m_LFBRequests.find(l_Guid);

                if (l_Itr != m_LFBRequests.end())
                    m_LFBRequests.erase(l_Itr);

                continue;
            }

            switch (l_Ticket->State)
            {
                case LFBState::LFB_STATE_PROPOSAL:
                {
                    if (l_Ticket->MatchingOpponent)
                    {
                        Player* l_Opponent = HashMapHolder<Player>::Find(l_Ticket->MatchingOpponent->RequesterGUID);

                        l_Ticket->MatchingOpponent->MatchingOpponent = nullptr;
                        l_Ticket->MatchingOpponent->State            = LFBState::LFB_STATE_QUEUED;

                        if (l_Opponent)
                            l_Opponent->GetSession()->SendPetBattleQueueStatus(l_Ticket->MatchingOpponent->JoinTime, l_Ticket->MatchingOpponent->TicketID, LFBUpdateStatus::LFB_JOIN_QUEUE, m_LFBAvgWaitTime);
                    }

                    /// Continue to other case handlers
                }
                case LFBState::LFB_STATE_FINISHED:
                case LFBState::LFB_STATE_IN_COMBAT:
                case LFBState::LFB_STATE_QUEUED:
                {
                    Player* l_Player = HashMapHolder<Player>::Find(l_Ticket->RequesterGUID);

                    if (l_Player)
                    {
                        l_Player->GetSession()->SendPetBattleQueueStatus(l_Ticket->JoinTime, l_Ticket->TicketID, LFBUpdateStatus::LFB_LEAVE_QUEUE, m_LFBAvgWaitTime);
                        l_Player->GetSession()->SendBattlePetJournalLockDenied();
                    }

                    delete m_LFBRequests[l_Guid];
                    m_LFBRequests[l_Guid] = nullptr;
                    break;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

/// Forfeit an battle
void PetBattleSystem::ForfeitBattle(uint64 p_BattleID, uint64 p_ForfeiterGuid)
{
    PetBattle* l_Battle = GetBattle(p_BattleID);

    if (!l_Battle)
        return;

    uint32 l_ForfeiterTeamID;

    for (l_ForfeiterTeamID = 0; l_ForfeiterTeamID < MAX_PETBATTLE_TEAM; ++l_ForfeiterTeamID)
    {
        if (l_Battle->Teams[l_ForfeiterTeamID]->OwnerGuid == p_ForfeiterGuid)
            break;
    }

    if (l_ForfeiterTeamID == MAX_PETBATTLE_TEAM)
        return;

    l_Battle->Finish(!l_ForfeiterTeamID, true);
}

//////////////////////////////////////////////////////////////////////////

/// Can player enter in a pet battle
eBattlePetRequests PetBattleSystem::CanPlayerEnterInPetBattle(Player* p_Player, PetBattleRequest* p_Request)
{
    if (!p_Player || !p_Player->IsInWorld())
        return PETBATTLE_REQUEST_CREATE_FAILED;

    if (p_Player->isDead())
        return PETBATTLE_REQUEST_NOT_WHILE_DEAD;

    // Player can't be already in battle
    if (p_Player->_petBattleId)
        return PETBATTLE_REQUEST_IN_BATTLE;

    if (IS_PLAYER_GUID(p_Request->OpponentGuid))
    {
        if (Player* l_Player = HashMapHolder<Player>::Find(p_Request->OpponentGuid))
        {
            if (l_Player->_petBattleId)
                return PETBATTLE_REQUEST_IN_BATTLE;

            if (!p_Player->IsWithinDist3d(l_Player, INTERACTION_DISTANCE))
                return PETBATTLE_REQUEST_TARGET_OUT_OF_RANGE;

            if (!p_Player->IsWithinLOSInMap(l_Player))
                return PETBATTLE_REQUEST_NOT_HERE_OBSTRUCTED;
        }
    }
    else if (IS_CREATURE_GUID(p_Request->OpponentGuid))
    {
        if (!p_Player->GetNPCIfCanInteractWith(p_Request->OpponentGuid, 0))
            return PETBATTLE_REQUEST_TARGET_INVALID;

        Creature* l_Creature = sObjectAccessor->GetCreature(*p_Player, p_Request->OpponentGuid);

        if (l_Creature->_petBattleId != 0)
            return PETBATTLE_REQUEST_WILD_PET_TAPPED;

        if (!p_Player->IsWithinDist3d(l_Creature, INTERACTION_DISTANCE))
            return PETBATTLE_REQUEST_TARGET_OUT_OF_RANGE;

        if (!p_Player->IsWithinLOSInMap(l_Creature))
            return PETBATTLE_REQUEST_NOT_HERE_OBSTRUCTED;
    }

    // Player can't be in combat
    if (p_Player->isInCombat())
        return PETBATTLE_REQUEST_NOT_WHILE_IN_COMBAT;

    // Check positions
    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if (p_Player->GetMap()->getObjectHitPos(p_Player->GetPhaseMask(), p_Request->PetBattleCenterPosition[0], p_Request->PetBattleCenterPosition[1], p_Request->PetBattleCenterPosition[2],
                                                                          p_Request->TeamPosition[l_CurrentTeamID][0], p_Request->TeamPosition[l_CurrentTeamID][1], p_Request->TeamPosition[l_CurrentTeamID][2],
                                                                          p_Request->TeamPosition[l_CurrentTeamID][0], p_Request->TeamPosition[l_CurrentTeamID][1], p_Request->TeamPosition[l_CurrentTeamID][2], 0.0f))
        {
            return PETBATTLE_REQUEST_NOT_HERE_UNEVEN_GROUND;
        }
    }

    uint32 l_OpponentTeamID = PETBATTLE_PVE_TEAM_ID;

    if (p_Request->RequestType != PETBATTLE_TYPE_PVE)
        l_OpponentTeamID = (p_Request->OpponentGuid == p_Player->GetGUID()) ? PETBATTLE_TEAM_1 : PETBATTLE_TEAM_2;

    // Load player pets
    BattlePet::Ptr * l_PetSlots = p_Player->GetBattlePetCombatTeam();
    size_t l_PlayerPetCount = 0;
    size_t l_PlayerDeadPetCount = 0;

    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        if (!l_PetSlots[l_CurrentPetSlot])
            continue;

        if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= p_Player->GetUnlockedPetBattleSlot())
            break;

        if (l_PetSlots[l_CurrentPetSlot]->Health < 1)
            l_PlayerDeadPetCount++;

        ++l_PlayerPetCount;
    }

    if (!l_PlayerPetCount)
        return PETBATTLE_REQUEST_NO_PETS_IN_SLOT;

    if (l_PlayerPetCount == l_PlayerDeadPetCount)
        return PETBATTLE_REQUEST_ALL_PETS_DEAD;

    return PETBATTLE_REQUEST_OK;
}

