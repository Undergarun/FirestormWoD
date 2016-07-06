////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Pet.h"
#include "Formulas.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "CreatureAI.h"
#include "Unit.h"
#include "Util.h"
#include "Group.h"
#ifdef CROSS
#include "InterRealmMgr.h"
#endif /* CROSS */

#define PET_XP_FACTOR 0.05f

Pet::Pet(Player* owner, PetType type) : Guardian(NULL, owner, true),
m_removed(false), m_owner(owner),
m_petType(type), m_duration(0), m_auraRaidUpdateMask(0),
m_loading(false), m_specialization(0), m_declinedname(NULL)
{
    m_unitTypeMask |= UNIT_MASK_PET;
    if (type == HUNTER_PET)
        m_unitTypeMask |= UNIT_MASK_HUNTER_PET;

    if (!(m_unitTypeMask & UNIT_MASK_CONTROLABLE_GUARDIAN))
    {
        m_unitTypeMask |= UNIT_MASK_CONTROLABLE_GUARDIAN;
        InitCharmInfo();
    }

    m_name = "Pet";
    m_RegenPowerTimer = PET_FOCUS_REGEN_INTERVAL;
    m_Stampeded = false;
}

Pet::~Pet()
{
    delete m_declinedname;
}

void Pet::AddToWorld()
{
    ///- Register the pet for guid lookup
    if (!IsInWorld())
    {
        ///- Register the pet for guid lookup
        sObjectAccessor->AddObject(this);
        Unit::AddToWorld();
        AIM_Initialize();
    }

    // Prevent stuck pets when zoning. Pets default to "follow" when added to world
    // so we'll reset flags and let the AI handle things
    if (GetCharmInfo() && GetCharmInfo()->HasCommandState(COMMAND_FOLLOW))
    {
        GetCharmInfo()->SetIsCommandAttack(false);
        GetCharmInfo()->SetIsCommandFollow(false);
        GetCharmInfo()->SetIsAtStay(false);
        GetCharmInfo()->SetIsFollowing(false);
        GetCharmInfo()->SetIsReturning(false);
    }

    // Hack fix for Soul link, we need to buff pet and player if player has a talent
    if (m_owner && m_owner->IsPlayer() && m_owner->ToPlayer()->HasSpell(108415))
    {
        CastSpell(m_owner, 108446, true);
        m_owner->CastSpell(this, 108446, true);
    }

    /// Command Demon
    if (m_owner && m_owner->IsPlayer() && m_owner->ToPlayer()->getClass() == CLASS_WARLOCK)
    {
        if (m_owner->HasAura(108503))
            m_owner->RemoveAura(108503);

        /// Threatening Presence - special ability for some warlock pets
        if (GetEntry() == ENTRY_FELGUARD || GetEntry() == ENTRY_WRATHGUARD)
            learnSpell(134477);
        else if (GetEntry() == ENTRY_VOIDWALKER || GetEntry() == ENTRY_VOIDLORD)
            learnSpell(112042);

        // Supplant Command Demon
        if (m_owner->getLevel() >= 56)
        {
            int32 bp = 0;

            m_owner->RemoveAura(119904);

            switch (GetEntry())
            {
            case ENTRY_IMP:
            case ENTRY_FEL_IMP:
                bp = 119905;// Cauterize Master
                break;
            case ENTRY_VOIDWALKER:
            case ENTRY_VOIDLORD:
                bp = 119907;// Disarm Removed since 6.0.2 please clean me
                break;
            case ENTRY_SUCCUBUS:
                bp = 119909; // Whiplash
                break;
            case ENTRY_SHIVARRA:
                bp = 119913;// Fellash
                break;
            case ENTRY_FELHUNTER:
                bp = 119910;// Spell Lock
                break;
            case ENTRY_OBSERVER:
                bp = 119911;// Optical Blast
                break;
            case ENTRY_FELGUARD:
                bp = 119914;// Felstorm
                break;
            case ENTRY_WRATHGUARD:
                bp = 119915;// Wrathstorm
                break;
            default:
                break;
            }

            if (bp)
                m_owner->CastCustomSpell(m_owner, 119904, &bp, NULL, NULL, true);
        }
    }
}

void Pet::RemoveFromWorld()
{
    ///- Remove the pet from the accessor
    if (IsInWorld())
    {
        ///- Don't call the function for Creature, normal mobs + totems go in a different storage
        Unit::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

void Pet::LoadPetFromDB(Player* owner, uint32 petentry, uint32 /*petnumber*/, bool current, PetSlot slotID, bool stampeded, PetQueryHolder* holder, std::function<void(Pet*, bool)> p_Callback)
{
    m_loading = true;

    if (owner->IsPlayer())
    {
        /// Hack for water elemental
        /// Pet should be saved for all specs, but can be summoned only by frost mages
        if (owner->getClass() == CLASS_MAGE && owner->ToPlayer()->GetSpecializationId() != SPEC_MAGE_FROST)
        {
            m_loading = false;
            p_Callback(this, false);
            return;
        }
        /// Hack for Raise dead
        /// Pet should be saved for all specs, but can be summoned only by unholy dk
        if (owner->getClass() == CLASS_DEATH_KNIGHT && owner->ToPlayer()->GetSpecializationId() != SPEC_DK_UNHOLY)
        {
            m_loading = false;
            p_Callback(this, false);
            return;
        }
    }

    if (slotID == PET_SLOT_ACTUAL_PET_SLOT)
        slotID = owner->m_currentPetSlot;


    PreparedQueryResult result = holder->GetPetResult();

    if (!result)
    {
        m_loading = false;
        p_Callback(this, false);
        return;
    }

    Field* fields = result->Fetch();

    // update for case of current pet "slot = 0"
    petentry = fields[1].GetUInt32();
    if (!petentry)
    {
        p_Callback(this, false);
        return;
    }

    uint32 summon_spell_id = fields[14].GetUInt32();
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(summon_spell_id);

    bool is_temporary_summoned = spellInfo && spellInfo->GetDuration() > 0;

    // check temporary summoned pets like mage water elemental
    if (current && is_temporary_summoned)
    {
        m_loading = false;
        p_Callback(this, false);
        return;
    }

    PetType pet_type = PetType(fields[15].GetUInt8());
    if (pet_type == HUNTER_PET)
    {
        CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(petentry);
        if (!creatureInfo || !creatureInfo->isTameable(owner->CanTameExoticPets()))
        {
            p_Callback(this, false);
            return;
        }
    }

    uint32 pet_number = fields[0].GetUInt32();

    if (current && owner->IsPetNeedBeTemporaryUnsummoned())
    {
        owner->SetTemporaryUnsummonedPetNumber(pet_number);
        p_Callback(this, false);
        return;
    }

    Map* map = owner->GetMap();
    uint32 guid = sObjectMgr->GenerateLowGuid(HIGHGUID_PET);
    if (!Create(guid, map, owner->GetPhaseMask(), petentry))
    {
        p_Callback(this, false);
        return;
    }

    float px, py, pz;
    owner->GetClosePoint(px, py, pz, GetObjectSize(), PET_FOLLOW_DIST, GetFollowAngle());
    Relocate(px, py, pz, owner->GetOrientation());

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_PETS, "Pet (guidlow %d, entry %d) not loaded. Suggested coordinates isn't valid (X: %f Y: %f)",
            GetGUIDLow(), GetEntry(), GetPositionX(), GetPositionY());
        p_Callback(this, false);
        return;
    }

    setPetType(pet_type);
    setFaction(owner->getFaction());
    SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, summon_spell_id);

    CreatureTemplate const* cinfo = GetCreatureTemplate();
    if (cinfo->type == CREATURE_TYPE_CRITTER)
    {
        map->AddToMap(this->ToCreature());
        p_Callback(this, true);
        return;
    }

    m_charmInfo->SetPetNumber(pet_number, IsPermanentPetFor(owner));

    SetDisplayId(fields[3].GetUInt32());
    SetNativeDisplayId(fields[3].GetUInt32());
    uint32 petlevel = fields[4].GetUInt16();
    SetUInt32Value(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
    SetUInt32Value(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_NONE);
    SetName(fields[8].GetString());

    Powers powerType = POWER_MANA;
    switch (cinfo->unit_class)
    {
        case CLASS_WARRIOR:
            powerType = POWER_RAGE;
            break;
        case CLASS_ROGUE:
            powerType = POWER_ENERGY;
            break;
    }

    switch (getPetType())
    {
        case SUMMON_PET:
            petlevel = owner->getLevel();

            SetUInt32Value(UNIT_FIELD_SEX, 0x800); // class = mage
            SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
                                                            // this enables popup window (pet dismiss, cancel)
            if (owner && owner->getClass() == CLASS_WARLOCK)
            {
                SetUInt32Value(UNIT_FIELD_SEX, 0x400); // class = rogue
                setPowerType(POWER_ENERGY); // Warlock's pets have energy
            }

            if (cinfo && (cinfo->Entry == 17252 || cinfo->Entry == 58965))
            {
                if (owner && owner->IsPlayer() && owner->HasAura(56246))
                {
                    /// Get item template for Fel Guard weapon
                    if (ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(12784))
                    {
                        uint32 l_RandomItemId = owner->ToPlayer()->GetRandomWeaponFromPrimaryBag(l_ItemTemplate);
                        LoadSpecialEquipment(l_RandomItemId);
                    }
                }
            }
            break;
        case HUNTER_PET:
            SetUInt32Value(UNIT_FIELD_SEX, 0x02020300); // class = warrior, gender = none, power = focus
            SetSheath(SHEATH_STATE_MELEE);
            SetByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, fields[9].GetBool() ? UNIT_CAN_BE_ABANDONED : UNIT_CAN_BE_RENAMED | UNIT_CAN_BE_ABANDONED);

            SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
                                                            // this enables popup window (pet abandon, cancel)
            setPowerType(POWER_FOCUS);
            SetSpecializationId(fields[16].GetUInt32());
            break;
        default:
            if (!IsPetGhoul())
                sLog->outError(LOG_FILTER_PETS, "Pet have incorrect type (%u) for pet loading.", getPetType());
            break;
    }

    SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped here
    SetCreatorGUID(owner->GetGUID());

    InitStatsForLevel(petlevel);
    SetUInt32Value(UNIT_FIELD_PET_EXPERIENCE, fields[5].GetUInt32());

    SynchronizeLevelWithOwner();

    SetReactState(ReactStates(fields[6].GetUInt8()));
    SetCanModifyStats(true);

    if (getPetType() == SUMMON_PET && !current && owner && owner->getClass() != CLASS_WARLOCK)  //all (?) summon pets come with full health when called, but not when they are current
        SetPower(POWER_MANA, GetMaxPower(POWER_MANA));
    else
    {
        uint32 savedhealth = fields[10].GetUInt32();
        uint32 savedmana = fields[11].GetUInt32();
        if (!savedhealth && getPetType() == HUNTER_PET)
            setDeathState(JUST_DIED);
        else if (owner && owner->getClass() != CLASS_WARLOCK)
        {
            SetHealth(savedhealth > GetMaxHealth() ? GetMaxHealth() : savedhealth);
            SetPower(POWER_MANA, savedmana > uint32(GetMaxPower(POWER_MANA)) ? GetMaxPower(POWER_MANA) : savedmana);
        }
        else
        {
            if (savedhealth > GetMaxHealth() || savedhealth < 1)
                savedhealth = GetMaxHealth();
            SetHealth(savedhealth);
            SetMaxPower(POWER_ENERGY, GetCreatePowers(POWER_ENERGY));
            SetPower(POWER_ENERGY, GetCreatePowers(POWER_ENERGY));
        }
    }
    owner->SetMinion(this, true, slotID == PET_SLOT_UNK_SLOT ? PET_SLOT_OTHER_PET : slotID, stampeded);
    map->AddToMap(this->ToCreature());

    m_slot = fields[7].GetUInt8();

    uint32 timediff = uint32(time(NULL) - fields[13].GetUInt32());

    PreparedQueryResult auraResult          = holder->GetPreparedResult(PET_LOGIN_QUERY_LOADAURA);
    PreparedQueryResult auraEffectResult    = holder->GetPreparedResult(PET_LOGIN_QUERY_LOADAURAEFFECT);
    PreparedQueryResult spellResult         = holder->GetPreparedResult(PET_LOGIN_QUERY_LOADSPELL);
    PreparedQueryResult spellCooldownResult = holder->GetPreparedResult(PET_LOGIN_QUERY_LOADSPELLCOOLDOWN);

    _LoadAuras(auraResult, auraEffectResult, timediff);

    if (owner->IsPlayer() && owner->ToPlayer()->InArena())
        RemoveArenaAuras();

    // load action bar, if data broken will fill later by default spells.
    m_charmInfo->LoadPetActionBar(fields[12].GetString());

    _LoadSpells(spellResult);
    _LoadSpellCooldowns(spellCooldownResult);
    LearnPetPassives();
    InitLevelupSpellsForLevel();
    CastPetAuras(current);

    CleanupActionBar();                                     // remove unknown spells from action bar after load

    sLog->outDebug(LOG_FILTER_PETS, "New Pet has guid %u", GetGUIDLow());

    owner->PetSpellInitialize();

    if (owner->GetGroup())
        owner->SetGroupUpdateFlag(GROUP_UPDATE_PET);

    owner->SendTalentsInfoData(true);

    if (getPetType() == HUNTER_PET)
    {
        PreparedQueryResult result = holder->GetPreparedResult(PET_LOGIN_QUERY_DECLINED_NAME);

        if (result)
        {
            delete m_declinedname;
            m_declinedname = new DeclinedName;
            Field* fields2 = result->Fetch();
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                m_declinedname->name[i] = fields2[i].GetString();
        }
    }

    //set last used pet number (for use in BG's)
    if (owner->IsPlayer() && isControlled() && !isTemporarySummoned() && (getPetType() == SUMMON_PET || getPetType() == HUNTER_PET))
        owner->ToPlayer()->SetLastPetNumber(pet_number);

    m_loading = false;

    if (owner->IsPlayer() && isControlled() && !isTemporarySummoned() && getPetType() == HUNTER_PET)
    {
        uint32 l_SpecializationID = GetSpecializationId();
        if (owner->HasAuraType(AuraType::SPELL_AURA_OVERRIDE_PET_SPECS))
        {
            switch (l_SpecializationID)
            {
                case SpecIndex::SPEC_PET_FEROCITY:
                    l_SpecializationID = (uint32)SpecIndex::SPEC_ROGUE_FEROCITY_ADAPT;
                    break;
                case SpecIndex::SPEC_PET_CUNNING:
                    l_SpecializationID = (uint32)SpecIndex::SPEC_ROGUE_CUNNING_ADAPT;
                    break;
                case SpecIndex::SPEC_PET_TENACITY:
                    l_SpecializationID = (uint32)SpecIndex::SPEC_ROGUE_TENACIOUS_ADAPT;
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (l_SpecializationID)
            {
                case SpecIndex::SPEC_ROGUE_FEROCITY_ADAPT:
                    l_SpecializationID = (uint32)SpecIndex::SPEC_PET_FEROCITY;
                    break;
                case SpecIndex::SPEC_ROGUE_CUNNING_ADAPT:
                    l_SpecializationID = (uint32)SpecIndex::SPEC_PET_CUNNING;
                    break;
                case SpecIndex::SPEC_ROGUE_TENACIOUS_ADAPT:
                    l_SpecializationID = (uint32)SpecIndex::SPEC_PET_TENACITY;
                    break;
                default:
                    break;
            }
        }

        UnlearnSpecializationSpell();

        SetSpecializationId(l_SpecializationID);
        LearnSpecializationSpell();
        owner->ToPlayer()->SendTalentsInfoData(true);
    }

    p_Callback(this, true);
}

void Pet::SavePetToDB(PetSlot mode, bool stampeded)
{
    if (!GetEntry())
        return;

    // save only fully controlled creature
    if (!isControlled())
        return;

    if (stampeded)
        return;

    // not save not player pets
    if (!IS_PLAYER_GUID(GetOwnerGUID()))
        return;

    Player* owner = (Player*)GetOwner();
    if (!owner)
        return;

#ifdef CROSS
    if (owner->GetSession()->GetInterRealmClient() == nullptr)
        return;

    InterRealmDatabasePool* l_Database = GetOwner()->GetRealmDatabase();
#else
    auto l_Database = &CharacterDatabase;
#endif

    if (mode == PET_SLOT_ACTUAL_PET_SLOT)
        mode = owner->m_currentPetSlot;

    SetSlot(mode);

    // not save pet as current if another pet temporary unsummoned
    if (mode == owner->m_currentPetSlot && owner->GetTemporaryUnsummonedPetNumber() &&
        owner->GetTemporaryUnsummonedPetNumber() != m_charmInfo->GetRealmPetNumber())
    {
        // pet will lost anyway at restore temporary unsummoned
        if (getPetType() == HUNTER_PET)
            return;

        // for warlock case
        mode = PET_SLOT_OTHER_PET;
    }

    uint32 curhealth = GetHealth();
    uint32 curmana = GetPower(POWER_MANA);

    SQLTransaction trans = l_Database->BeginTransaction();

    // save auras before possibly removing them
    _SaveAuras(trans);

    // stable and not in slot saves
    if (mode > PET_SLOT_HUNTER_LAST && getPetType() == HUNTER_PET)
        RemoveAllAuras();

    _SaveSpells(trans);
    _SaveSpellCooldowns(trans);
    l_Database->CommitTransaction(trans);

    // current/stable/not_in_slot
    if (mode >= PET_SLOT_HUNTER_FIRST)
    {
        uint32 ownerLowGUID = GetOwner()->GetRealGUIDLow();
        std::string name = m_name;
        l_Database->EscapeString(name);
        trans = l_Database->BeginTransaction();

        // remove current data
        trans->PAppend("DELETE FROM character_pet WHERE owner = '%u' AND id = '%u'", ownerLowGUID, m_charmInfo->GetRealmPetNumber());

        // save pet
        std::ostringstream ss;
        ss  << "INSERT INTO character_pet (id, entry, owner, modelid, level, exp, Reactstate, slot, name, renamed, curhealth, curmana, abdata, savetime, CreatedBySpell, PetType, specialization) "
            << "VALUES ("
            << m_charmInfo->GetRealmPetNumber() << ','
            << GetEntry() << ','
            << ownerLowGUID << ','
            << GetNativeDisplayId() << ','
            << uint32(getLevel()) << ','
            << GetUInt32Value(UNIT_FIELD_PET_EXPERIENCE) << ','
            << uint32(GetReactState()) << ','
            << uint32(mode) << ", '"
            << name.c_str() << "', "
            << uint32(HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED) ? 0 : 1) << ','
            << curhealth << ','
            << curmana << ", '";

        for (uint32 i = ACTION_BAR_INDEX_START; i < ACTION_BAR_INDEX_END; ++i)
        {
            ss << uint32(m_charmInfo->GetActionBarEntry(i)->GetType()) << ' '
                << uint32(m_charmInfo->GetActionBarEntry(i)->GetAction()) << ' ';
        };

        ss  << "', "
            << time(NULL) << ','
            << GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL) << ','
            << uint32(getPetType()) << ','
            << GetSpecializationId()
            << ')';

        trans->Append(ss.str().c_str());

        uint64 l_PlayerGUID = owner->GetGUID();
        MS::Utilities::CallBackPtr l_CallBack = std::make_shared<MS::Utilities::Callback>([l_PlayerGUID](bool p_Success) -> void
        {
            Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGUID);
            if (!l_Player)
                return;

            l_Player->GetSession()->SendStablePet(0);
        });

        l_Database->CommitTransaction(trans, l_CallBack);
    }
    // delete
    else
    {
        if (owner->m_currentPetSlot >= PET_SLOT_HUNTER_FIRST && owner->m_currentPetSlot <= PET_SLOT_HUNTER_LAST)
            owner->setPetSlotUsed(owner->m_currentPetSlot, false);
        RemoveAllAuras();
        DeleteFromDB(m_charmInfo->GetRealmPetNumber(), owner->GetSession()->GetInterRealmNumber());
    }
}

void Pet::DeleteFromDB(uint32 guidlow, uint32 realmId)
{
#ifdef CROSS
    InterRealmDatabasePool* l_Database = sInterRealmMgr->GetClientByRealmNumber(realmId)->GetDatabase();
#else
    auto l_Database = &CharacterDatabase;
#endif

    SQLTransaction trans = l_Database->BeginTransaction();

    trans->PAppend("DELETE FROM character_pet WHERE id = '%u'", guidlow);
    trans->PAppend("DELETE FROM character_pet_declinedname WHERE id = '%u'", guidlow);
    trans->PAppend("DELETE FROM pet_aura WHERE guid = '%u'", guidlow);
    trans->PAppend("DELETE FROM pet_spell WHERE guid = '%u'", guidlow);
    trans->PAppend("DELETE FROM pet_spell_cooldown WHERE guid = '%u'", guidlow);

    l_Database->CommitTransaction(trans);
}

void Pet::setDeathState(DeathState s)                       // overwrite virtual Creature::setDeathState and Unit::setDeathState
{
    Creature::setDeathState(s);

    if (getDeathState() == CORPSE)
    {
        if (getPetType() == HUNTER_PET)
        {
            // pet corpse non lootable and non skinnable
            SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_NONE);
            RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
            //SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        }
    }
    else if (getDeathState() == ALIVE)
    {
        if (getPetType() == HUNTER_PET)
        {
            CastPetAuras(true);

            if (Unit* owner = GetOwner())
                if (Player* player = owner->ToPlayer())
                    player->StopCastingCharm();
        }
        else
        {
            //RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            CastPetAuras(true);
        }
    }
}

void Pet::Update(uint32 diff)
{
    if (m_removed)                                           // pet already removed, just wait in remove queue, no updates
        return;

    if (m_loading)
        return;

    if (getPetType() == HUNTER_PET) ///< Mend pet override (revive pet)
    {
        Player* l_Owner = GetOwner();
        if (l_Owner != nullptr)
        {
            if (getDeathState() == ALIVE && !l_Owner->HasAura(157863))
                l_Owner->CastSpell(l_Owner, 157863, true);
            else if (l_Owner->HasAura(157863) && getDeathState() != ALIVE)
                l_Owner->RemoveAura(157863);
        }
    }
    switch (m_deathState)
    {
        case CORPSE:
        {
            if (getPetType() != HUNTER_PET || m_corpseRemoveTime <= time(NULL))
            {
                Remove(PET_SLOT_ACTUAL_PET_SLOT, false, m_Stampeded);               //hunters' pets never get removed because of death, NEVER!
                return;
            }
            break;
        }
        case ALIVE:
        {
            // unsummon pet that lost owner
            Player* owner = GetOwner();
            if (!owner || (!IsWithinDistInMap(owner, GetMap()->GetVisibilityRange()) && !isPossessed()) || (isControlled() && !owner->GetPetGUID()))
            {
                Remove(PET_SLOT_ACTUAL_PET_SLOT, true, m_Stampeded);
                return;
            }

            if (isControlled())
            {
                if (owner->GetPetGUID() != GetGUID() && !m_Stampeded) // Stampede
                {
                    sLog->outError(LOG_FILTER_PETS, "Pet %u is not pet of owner %s, removed", GetEntry(), m_owner->GetName());
                    Remove(getPetType() == HUNTER_PET ? PET_SLOT_DELETED : PET_SLOT_ACTUAL_PET_SLOT, true, m_Stampeded);
                    return;
                }
            }

            if (m_duration > 0)
            {
                if (uint32(m_duration) > diff)
                    m_duration -= diff;
                else
                {
                    Remove(getPetType() != SUMMON_PET ? PET_SLOT_DELETED : PET_SLOT_ACTUAL_PET_SLOT, false, m_Stampeded);
                    return;
                }
            }

            if (Unit* l_Victim = getVictim())
            {
                if (!HasUnitState(UNIT_STATE_CASTING))
                {
                    GetMotionMaster()->MoveChase(l_Victim);
                }
            }

            //regenerate focus for hunter pets or energy for deathknight's ghoul
            if (m_RegenPowerTimer)
            {
                if (m_RegenPowerTimer > diff)
                    m_RegenPowerTimer -= diff;
                else
                {
                    switch (getPowerType())
                    {
                        case POWER_FOCUS:
                            Regenerate(POWER_FOCUS);
                            m_RegenPowerTimer += PET_FOCUS_REGEN_INTERVAL - diff;
                            if (!m_RegenPowerTimer)
                                ++m_RegenPowerTimer;

                            // Reset if large diff (lag) causes focus to get 'stuck'
                            if (m_RegenPowerTimer > PET_FOCUS_REGEN_INTERVAL)
                                m_RegenPowerTimer = PET_FOCUS_REGEN_INTERVAL;

                            break;
                        default:
                            m_RegenPowerTimer = 0;
                            break;
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    Creature::Update(diff);
}

void Creature::Regenerate(Powers power)
{
    uint32 curValue = GetPower(power);
    uint32 maxValue = GetMaxPower(power);

    if (curValue >= maxValue)
        return;

    float addvalue = 0.0f;

    switch (power)
    {
        case POWER_FOCUS:
        {
            // For hunter pets - Pets regen focus 125% more faster than owners
            if (GetOwner())
            {
                /// Calculate owners haste
                float l_OwnerHastePct = 1.0f / GetOwner()->GetFloatValue(UNIT_FIELD_MOD_HASTE);
                float l_OwnerBaseRegen = 4.0f;
                /// Calculate regenerate for 1 second and multiply for update interval
                addvalue += (l_OwnerBaseRegen * l_OwnerHastePct) * PET_FOCUS_REGEN_INTERVAL / IN_MILLISECONDS * sWorld->getRate(RATE_POWER_FOCUS);
                addvalue *= 1.25f;
            }
            break;
        }
        case POWER_ENERGY:
        {
            // For deathknight's ghoul and Warlock's pets
            addvalue = 20;
            break;
        }
        default:
            return;
    }

    // Apply modifiers (if any).
    AuraEffectList const& ModPowerRegenPCTAuras = GetAuraEffectsByType(SPELL_AURA_MOD_POWER_REGEN_PERCENT);
    for (AuraEffectList::const_iterator i = ModPowerRegenPCTAuras.begin(); i != ModPowerRegenPCTAuras.end(); ++i)
        if (Powers((*i)->GetMiscValue()) == power)
            AddPct(addvalue, (*i)->GetAmount());

    addvalue += GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, power) * (isHunterPet()? PET_FOCUS_REGEN_INTERVAL : CREATURE_REGEN_INTERVAL) / (5 * IN_MILLISECONDS);

    int32 intAddValue = int32(addvalue);

    if (IsAIEnabled)
        AI()->RegeneratePower(power, intAddValue);

    ModifyPower(power, intAddValue);
}

void Pet::Remove(PetSlot mode, bool returnreagent, bool stampeded)
{
    m_owner->RemovePet(this, mode, returnreagent, stampeded);
}

void Pet::GivePetXP(uint32 xp)
{
    if (getPetType() != HUNTER_PET)
        return;

    if (xp < 1)
        return;

    if (!isAlive())
        return;

    uint8 maxlevel = std::min((uint8)sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL), GetOwner()->getLevel());
    uint8 petlevel = getLevel();

    // If pet is detected to be at, or above(?) the players level, don't hand out XP
    if (petlevel >= maxlevel)
       return;

    uint32 nextLvlXP = GetUInt32Value(UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE);
    uint32 curXP = GetUInt32Value(UNIT_FIELD_PET_EXPERIENCE);
    uint32 newXP = curXP + xp;

    // Check how much XP the pet should receive, and hand off have any left from previous levelups
    while (newXP >= nextLvlXP && petlevel < maxlevel)
    {
        // Subtract newXP from amount needed for nextlevel, and give pet the level
        newXP -= nextLvlXP;
        ++petlevel;

        GivePetLevel(petlevel);

        nextLvlXP = GetUInt32Value(UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE);
    }
    // Not affected by special conditions - give it new XP
    SetUInt32Value(UNIT_FIELD_PET_EXPERIENCE, petlevel < maxlevel ? newXP : 0);
}

void Pet::GivePetLevel(uint8 level)
{
    if (!level || level == getLevel())
        return;

    if (getPetType()==HUNTER_PET)
    {
        SetUInt32Value(UNIT_FIELD_PET_EXPERIENCE, 0);
        SetUInt32Value(UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE, uint32(sObjectMgr->GetXPForLevel(level)*PET_XP_FACTOR));
    }

    InitStatsForLevel(level);
    InitLevelupSpellsForLevel();
}

bool Pet::CreateBaseAtCreature(Creature* creature)
{
    ASSERT(creature);

    if (!CreateBaseAtTamed(creature->GetCreatureTemplate(), creature->GetMap(), creature->GetPhaseMask()))
        return false;

    Relocate(creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation());

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_PETS, "Pet (guidlow %d, entry %d) not created base at creature. Suggested coordinates isn't valid (X: %f Y: %f)",
            GetGUIDLow(), GetEntry(), GetPositionX(), GetPositionY());
        return false;
    }

    CreatureTemplate const* cinfo = GetCreatureTemplate();
    if (!cinfo)
    {
        sLog->outError(LOG_FILTER_PETS, "CreateBaseAtCreature() failed, creatureInfo is missing!");
        return false;
    }

    SetDisplayId(creature->GetDisplayId());

    if (CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(cinfo->family))
        SetName(cFamily->Name);
    else
        SetName(creature->GetNameForLocaleIdx(sObjectMgr->GetDBCLocaleIndex()));

    return true;
}

bool Pet::CreateBaseAtCreatureInfo(CreatureTemplate const* cinfo, Unit* owner)
{
    if (!CreateBaseAtTamed(cinfo, owner->GetMap(), owner->GetPhaseMask()))
        return false;

    if (CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(cinfo->family))
        SetName(cFamily->Name);

    Relocate(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation());

    return true;
}

bool Pet::CreateBaseAtTamed(CreatureTemplate const* cinfo, Map* map, uint32 phaseMask)
{
    sLog->outDebug(LOG_FILTER_PETS, "Pet::CreateBaseForTamed");
    uint32 guid = sObjectMgr->GenerateLowGuid(HIGHGUID_PET);
    if (!Create(guid, map, phaseMask, cinfo->Entry))
        return false;

    setPowerType(POWER_FOCUS);
    SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, 0);
    SetUInt32Value(UNIT_FIELD_PET_EXPERIENCE, 0);
    SetUInt32Value(UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE, uint32(sObjectMgr->GetXPForLevel(getLevel()+1)*PET_XP_FACTOR));
    SetUInt32Value(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
    SetUInt32Value(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_NONE);

    if (cinfo->type == CREATURE_TYPE_BEAST)
    {
        SetUInt32Value(UNIT_FIELD_SEX, 0x02020300);
        setPowerType(POWER_FOCUS);
        SetSheath(SHEATH_STATE_MELEE);
        SetByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED | UNIT_CAN_BE_ABANDONED);
    }

    return true;
}

// TODO: Move stat mods code to pet passive auras
bool Guardian::InitStatsForLevel(uint8 p_PetLevel)
{
    CreatureTemplate const* l_CreatureTemplate = GetCreatureTemplate();
    ASSERT(l_CreatureTemplate);

    SetLevel(p_PetLevel);

    // Determine pet type
    PetType l_PetType = MAX_PET_TYPE;

    Unit* l_Owner = GetSummoner();
    if (l_Owner && isPet() && l_Owner->IsPlayer())
    {
        switch (l_Owner->getClass())
        {
            case CLASS_WARLOCK:
            case CLASS_MONK:
            case CLASS_SHAMAN:          // Fire Elemental
            case CLASS_PRIEST:          // Shadowfiend and Mindbender
            case CLASS_DEATH_KNIGHT:    // Risen Ghoul
            case CLASS_MAGE:            // Water Elemental
                l_PetType = SUMMON_PET;
                break;
            case CLASS_HUNTER:
                l_PetType = HUNTER_PET;
                m_unitTypeMask |= UNIT_MASK_HUNTER_PET;
                break;
            default:
                sLog->outError(LOG_FILTER_PETS, "Unknown type pet %u is summoned by player class %u", GetEntry(), l_Owner->getClass());
                break;
        }
    }

    uint32 l_PetStatEntry = l_CreatureTemplate->Entry;
    if (l_PetType == HUNTER_PET)
        l_PetStatEntry = 1;

    PetStatInfo const* l_PetStat = sObjectMgr->GetPetStatInfo(l_PetStatEntry);

    // Default stat
    if (l_PetStat == nullptr)
        l_PetStat = sObjectMgr->GetPetStatInfo(0);

    SetMeleeDamageSchool(SpellSchools(l_CreatureTemplate->dmgschool));

    SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, (l_Owner ? l_Owner->GetArmor() : 1) * l_PetStat->m_ArmorCoef);

    SetAttackTime(WeaponAttackType::BaseAttack,   l_PetStat->m_AttackSpeed * IN_MILLISECONDS);
    SetAttackTime(WeaponAttackType::OffAttack,    l_PetStat->m_AttackSpeed * IN_MILLISECONDS);
    SetAttackTime(WeaponAttackType::RangedAttack, l_PetStat->m_AttackSpeed * IN_MILLISECONDS);

    SetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED,                     1.0f);
    SetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE,                       1.0f);
    SetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER,                 1.0f);
    SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER,             0.0f);
    SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, 0.0f);

    if (l_PetType == HUNTER_PET)
    {
        if (sObjectMgr->GetCreatureModelInfo(GetDisplayId()) && l_Owner)
        {
            SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, l_Owner->GetFloatValue(UNIT_FIELD_BOUNDING_RADIUS));
            SetFloatValue(UNIT_FIELD_COMBAT_REACH, l_Owner->GetFloatValue(UNIT_FIELD_COMBAT_REACH));
        }
    }

    // Speed
    for (uint8 l_I = 0; l_I < MAX_MOVE_TYPE; ++l_I)
        SetSpeed(UnitMoveType(l_I), l_PetStat->m_Speed, true);

    // Resistance
    for (uint8 l_I = SPELL_SCHOOL_HOLY; l_I < MAX_SPELL_SCHOOL; ++l_I)
        SetModifierValue(UnitMods(UNIT_MOD_RESISTANCE_START + l_I), BASE_VALUE, float(l_CreatureTemplate->resistance[l_I]));

    Powers l_PetPower    = l_PetStat->m_Power;
    uint32 l_CreatePower = 0;

    if (l_PetStat->m_CreatePower == -1)
        l_CreatePower = GetCreatePowers(l_PetStat->m_Power);
    else
    {
        // Negative number, it's fix value
        if (l_PetStat->m_CreatePower < 0.0f)
            l_CreatePower = l_PetStat->m_CreatePower * -1;
        // Positive number, it's percentage of owner power
        else if (l_Owner != nullptr)
            l_CreatePower = float(l_Owner->GetMaxPower(l_Owner->getPowerType()) * l_PetStat->m_CreatePower);
    }

    SetCreateMana(l_PetPower == Powers::POWER_MANA ? l_CreatePower : 0);

    setPowerType(l_PetStat->m_Power);
    SetMaxPower(l_PetPower, l_CreatePower);
    SetPower(l_PetPower, l_CreatePower);

    // Base physical damage are 0-1 for every pet since WoD
    SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MINDAMAGE, 1);
    SetBaseWeaponDamage(WeaponAttackType::BaseAttack, MAXDAMAGE, 1);

    if (l_PetType == HUNTER_PET && !ToPet()->m_Stampeded)
        SetUInt32Value(UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE, uint32(sObjectMgr->GetXPForLevel(p_PetLevel) * PET_XP_FACTOR));

    if (l_Owner != nullptr)
    {
        SetCreateHealth(l_Owner->GetMaxHealth() * l_PetStat->m_HealthCoef);
        SetMaxHealth(l_Owner->GetMaxHealth() * l_PetStat->m_HealthCoef);
    }

    UpdateAllStats();

    SetFullHealth();

    if (IsWarlockPet())
        CastSpell(this, 123746, true);  ///< Fel Energy

    if (GetEntry() == ENTRY_GHOUL)
    {
        if (l_Owner && l_Owner->HasAura(58640))  ///< Glyph of the Geist
            CastSpell(this, 121916, true);
        if (l_Owner && l_Owner->HasAura(146652)) ///< Glyph of the Skeleton
            CastSpell(this, 147157, true);
    }

    if (l_PetType == HUNTER_PET)
    {
        if (l_Owner && l_Owner->HasAura(152244) && HasAura(20782))
        {
            RemoveAura(20782);
            CastSpell(this, 156843, true); ///<  Adaptation
        }
        CastSpell(this, 88680, true, nullptr, nullptr, l_Owner ? l_Owner->GetGUID() : 0); ///< Kindred Spirits
    }

    return true;
}

bool Pet::HaveInDiet(ItemTemplate const* item) const
{
    if (item->SubClass != ITEM_SUBCLASS_FOOD_DRINK)
        return false;

    CreatureTemplate const* cInfo = GetCreatureTemplate();
    if (!cInfo)
        return false;

    CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(cInfo->family);
    if (!cFamily)
        return false;

    uint32 diet = cFamily->petFoodMask;
    uint32 FoodMask = 1 << (item->FoodType-1);
    return diet & FoodMask;
}

uint32 Pet::GetCurrentFoodBenefitLevel(uint32 itemlevel)
{
    // -5 or greater food level
    if (getLevel() <= itemlevel + 5)                         //possible to feed level 60 pet with level 55 level food for full effect
        return 35000;
    // -10..-6
    else if (getLevel() <= itemlevel + 10)                   //pure guess, but sounds good
        return 17000;
    // -14..-11
    else if (getLevel() <= itemlevel + 14)                   //level 55 food gets green on 70, makes sense to me
        return 8000;
    // -15 or less
    else
        return 0;                                           //food too low level
}

void Pet::_LoadSpellCooldowns(PreparedQueryResult resultCooldown)
{
    m_CreatureSpellCooldowns.clear();
    m_CreatureCategoryCooldowns.clear();

    PreparedQueryResult result = resultCooldown;
    Player* owner = GetOwner() ? GetOwner()->ToPlayer() : NULL;

    if (result)
    {
        time_t curTime = time(NULL);
        ObjectGuid petGuid = GetGUID();

        do
        {
            Field* fields = result->Fetch();

            uint32 spell_id = fields[0].GetUInt32();
            time_t db_time  = time_t(fields[1].GetUInt32());

            if (!sSpellMgr->GetSpellInfo(spell_id))
            {
                sLog->outError(LOG_FILTER_PETS, "Pet %u have unknown spell %u in `pet_spell_cooldown`, skipping.", m_charmInfo->GetPetNumber(), spell_id);
                continue;
            }

            // skip outdated cooldown
            if (db_time <= curTime)
                continue;

            WorldPacket data(SMSG_SPELL_COOLDOWN, 16 + 2 + 1 + 4 + 4 + 4);
            data.appendPackGUID(petGuid);
            data << uint8(CooldownFlags::CooldownFlagNone);
            data << uint32(1);
            data << uint32(spell_id);
            data << uint32(uint32(db_time - curTime) * IN_MILLISECONDS);

            if (owner)
                owner->GetSession()->SendPacket(&data);

            _AddCreatureSpellCooldown(spell_id, db_time);

            sLog->outDebug(LOG_FILTER_PETS, "Pet (Number: %u) spell %u cooldown loaded (%u secs).", m_charmInfo->GetPetNumber(), spell_id, uint32(db_time-curTime));
        }
        while (result->NextRow());
    }
}

void Pet::_SaveSpellCooldowns(SQLTransaction& trans)
{
#ifdef CROSS
    if (!GetOwner())
        return;

    InterRealmDatabasePool* l_Database = GetOwner()->GetRealmDatabase();
#else
    auto l_Database = &CharacterDatabase;
#endif

    PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_PET_SPELL_COOLDOWNS);
    stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
    trans->Append(stmt);

    time_t curTime = time(NULL);

    // remove oudated and save active
    for (CreatureSpellCooldowns::iterator itr = m_CreatureSpellCooldowns.begin(); itr != m_CreatureSpellCooldowns.end();)
    {
        if (itr->second <= curTime)
            m_CreatureSpellCooldowns.erase(itr++);
        else
        {
            stmt = l_Database->GetPreparedStatement(CHAR_INS_PET_SPELL_COOLDOWN);
            stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
            stmt->setUInt32(1, itr->first);
            stmt->setUInt32(2, uint32(itr->second));
            trans->Append(stmt);

            ++itr;
        }
    }
}

void Pet::_LoadSpells(PreparedQueryResult resultSpell)
{
    PreparedQueryResult result = resultSpell;

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            addSpell(fields[0].GetUInt32(), ActiveStates(fields[1].GetUInt8()), PETSPELL_UNCHANGED);
        }
        while (result->NextRow());
    }
}

void Pet::_SaveSpells(SQLTransaction& trans)
{
#ifdef CROSS
    if (!GetOwner())
        return;

    InterRealmDatabasePool* l_Database = GetOwner()->GetRealmDatabase();
#else
    auto l_Database = &CharacterDatabase;
#endif

    for (PetSpellMap::iterator itr = m_spells.begin(), next = m_spells.begin(); itr != m_spells.end(); itr = next)
    {
        ++next;

        // prevent saving family passives to DB
        if (itr->second.type == PETSPELL_FAMILY)
            continue;

        PreparedStatement* stmt;

        switch (itr->second.state)
        {
            case PETSPELL_REMOVED:
                stmt = l_Database->GetPreparedStatement(CHAR_DEL_PET_SPELL_BY_SPELL);
                stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
                stmt->setUInt32(1, itr->first);
                trans->Append(stmt);

                itr = m_spells.erase(itr);
                continue;
            case PETSPELL_CHANGED:
                stmt = l_Database->GetPreparedStatement(CHAR_DEL_PET_SPELL_BY_SPELL);
                stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
                stmt->setUInt32(1, itr->first);
                trans->Append(stmt);

                stmt = l_Database->GetPreparedStatement(CHAR_INS_PET_SPELL);
                stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
                stmt->setUInt32(1, itr->first);
                stmt->setUInt8(2, itr->second.active);
                trans->Append(stmt);

                break;
            case PETSPELL_NEW:
                stmt = l_Database->GetPreparedStatement(CHAR_DEL_PET_SPELL_BY_SPELL);
                stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
                stmt->setUInt32(1, itr->first);
                trans->Append(stmt);

                stmt = l_Database->GetPreparedStatement(CHAR_INS_PET_SPELL);
                stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
                stmt->setUInt32(1, itr->first);
                stmt->setUInt8(2, itr->second.active);
                trans->Append(stmt);
                break;
            case PETSPELL_UNCHANGED:
                continue;
        }
        itr->second.state = PETSPELL_UNCHANGED;
    }
}

void Pet::_LoadAuras(PreparedQueryResult auraResult, PreparedQueryResult auraEffectResult, uint32 timediff)
{
    sLog->outDebug(LOG_FILTER_PETS, "Loading auras for pet %u", GetGUIDLow());

    PreparedQueryResult result = auraResult;
    PreparedQueryResult resultEffect = auraEffectResult;

    std::list<auraEffectData> auraEffectList;
    if (resultEffect)
    {
        do
        {
            Field* fields = resultEffect->Fetch();
            uint8 slot = fields[0].GetUInt8();
            uint8 effect = fields[1].GetUInt8();
            uint32 amount = fields[2].GetUInt32();
            uint32 baseamount = fields[3].GetUInt32();

            auraEffectList.push_back(auraEffectData(slot, effect, amount, baseamount));
        }
        while (resultEffect->NextRow());
    }

    if (result)
    {
        do
        {
            int32 damage[32];
            int32 baseDamage[32];
            Field* fields = result->Fetch();
            uint8 slot = fields[0].GetUInt8();
            uint64 caster_guid = fields[1].GetUInt64();
            // NULL guid stored - pet is the caster of the spell - see Pet::_SaveAuras
            if (!caster_guid)
                caster_guid = GetGUID();
            uint32 spellid = fields[2].GetUInt32();
            uint32 effmask = fields[3].GetUInt32();
            uint32 recalculatemask = fields[4].GetUInt32();
            uint8 stackcount = fields[5].GetUInt8();
            int32 maxduration = fields[6].GetInt32();
            int32 remaintime = fields[7].GetInt32();
            uint8 remaincharges = fields[8].GetUInt8();

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_PETS, "Unknown aura (spellid %u), ignore.", spellid);
                continue;
            }

            // Does not need to load Spirit Bond - reapplied at update
            if (spellInfo->Id == 118694)
                 continue;

            // negative effects should continue counting down after logout
            if (remaintime != -1 && !spellInfo->IsPositive())
            {
                if (remaintime/IN_MILLISECONDS <= int32(timediff))
                    continue;

                remaintime -= timediff*IN_MILLISECONDS;
            }

            // prevent wrong values of remaincharges
            if (spellInfo->ProcCharges)
            {
                if (remaincharges <= 0 || remaincharges > spellInfo->ProcCharges)
                    remaincharges = spellInfo->ProcCharges;
            }
            else
                remaincharges = 0;

            for (std::list<auraEffectData>::iterator itr = auraEffectList.begin(); itr != auraEffectList.end(); ++itr)
            {
                if (itr->_slot == slot)
                {
                    damage[itr->_effect] = itr->_amount;
                    baseDamage[itr->_effect] = itr->_baseamount;
                }
            }

            Aura* aura = Aura::TryCreate(spellInfo, effmask, this, NULL, &baseDamage[0], NULL, caster_guid);
            if (aura != nullptr)
            {
                if (!aura->CanBeSaved())
                {
                    aura->Remove();
                    continue;
                }
                aura->SetLoadedState(maxduration, remaintime, remaincharges, stackcount, recalculatemask, &damage[0]);
                aura->ApplyForTargets();
                sLog->outInfo(LOG_FILTER_PETS, "Added aura spellid %u, effectmask %u", spellInfo->Id, effmask);
            }
        }
        while (result->NextRow());
    }
}

void Pet::_SaveAuras(SQLTransaction& trans)
{
#ifdef CROSS
    if (!GetOwner())
        return;

    InterRealmDatabasePool* l_Database = GetOwner()->GetRealmDatabase();
#else
    auto l_Database = &CharacterDatabase;
#endif


    PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_PET_AURAS);
    stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
    trans->Append(stmt);

    stmt = l_Database->GetPreparedStatement(CHAR_DEL_PET_AURAS_EFFECTS);
    stmt->setUInt32(0, m_charmInfo->GetRealmPetNumber());
    trans->Append(stmt);

    for (AuraMap::const_iterator itr = m_ownedAuras.begin(); itr != m_ownedAuras.end(); ++itr)
    {
        // check if the aura has to be saved
        if (!itr->second->CanBeSaved() || IsPetAura(itr->second))
            continue;

        Aura* aura = itr->second;
        AuraApplication * foundAura = GetAuraApplication(aura->GetId(), aura->GetCasterGUID(), aura->GetCastItemGUID());

        if (!foundAura)
            continue;

        int32 damage[SpellEffIndex::MAX_EFFECTS];
        int32 baseDamage[SpellEffIndex::MAX_EFFECTS];
        uint32 effMask = 0;
        uint32 recalculateMask = 0;
        uint8 index = 0;
        for (uint8 i = 0; i < aura->GetEffectCount(); ++i)
        {
            if (aura->GetEffect(i))
            {
                index = 0;
                stmt = l_Database->GetPreparedStatement(CHAR_INS_PET_AURA_EFFECT);
                stmt->setUInt32(index++, m_charmInfo->GetRealmPetNumber());
                stmt->setUInt8(index++, foundAura->GetSlot());
                stmt->setUInt8(index++, i);
                stmt->setInt32(index++, aura->GetEffect(i)->GetBaseAmount());
                stmt->setInt32(index++, aura->GetEffect(i)->GetAmount());

                trans->Append(stmt);

                baseDamage[i] = aura->GetEffect(i)->GetBaseAmount();
                damage[i] = aura->GetEffect(i)->GetAmount();
                effMask |= (1<<i);
                if (aura->GetEffect(i)->CanBeRecalculated())
                    recalculateMask |= (1<<i);
            }
            else
            {
                baseDamage[i] = 0;
                damage[i] = 0;
            }
        }

        // don't save guid of caster in case we are caster of the spell - guid for pet is generated every pet load, so it won't match saved guid anyways
        uint64 casterGUID = (itr->second->GetCasterGUID() == GetGUID()) ? 0 : itr->second->GetCasterGUID();

        index = 0;
        PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_INS_PET_AURA);
        stmt->setUInt32(index++, m_charmInfo->GetRealmPetNumber());
        stmt->setUInt8(index++, foundAura->GetSlot());
        stmt->setUInt64(index++, casterGUID);
        stmt->setUInt32(index++, itr->second->GetId());
        stmt->setUInt8(index++, effMask);
        stmt->setUInt8(index++, recalculateMask);
        stmt->setUInt8(index++, itr->second->GetStackAmount());
        stmt->setInt32(index++, itr->second->GetMaxDuration());
        stmt->setInt32(index++, itr->second->GetDuration());
        stmt->setUInt8(index++, itr->second->GetCharges());

        trans->Append(stmt);
    }
}

bool Pet::addSpell(uint32 spellId, ActiveStates active /*= ACT_DECIDE*/, PetSpellState state /*= PETSPELL_NEW*/, PetSpellType type /*= PETSPELL_NORMAL*/)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return false;

    PetSpellMap::iterator itr = m_spells.find(spellId);
    if (itr != m_spells.end())
    {
        if (itr->second.state == PETSPELL_REMOVED)
        {
            m_spells.erase(itr);
            state = PETSPELL_CHANGED;
        }
        else if (state == PETSPELL_UNCHANGED && itr->second.state != PETSPELL_UNCHANGED)
        {
            // can be in case spell loading but learned at some previous spell loading
            itr->second.state = PETSPELL_UNCHANGED;

            if (active == ACT_ENABLED)
                ToggleAutocast(spellInfo, true);
            else if (active == ACT_DISABLED)
                ToggleAutocast(spellInfo, false);

            return false;
        }
        else
            return false;
    }

    PetSpell newspell;
    newspell.state = state;
    newspell.type = type;

    if (active == ACT_DECIDE)                               // active was not used before, so we save it's autocast/passive state here
    {
        if (spellInfo->IsAutocastable())
            newspell.active = ACT_DISABLED;
        else
            newspell.active = ACT_PASSIVE;
    }
    else
        newspell.active = active;

    if (spellInfo->IsRanked())
    {
        for (PetSpellMap::const_iterator itr2 = m_spells.begin(); itr2 != m_spells.end(); ++itr2)
        {
            if (itr2->second.state == PETSPELL_REMOVED)
                continue;

            SpellInfo const* oldRankSpellInfo = sSpellMgr->GetSpellInfo(itr2->first);

            if (!oldRankSpellInfo)
                continue;

            if (spellInfo->IsDifferentRankOf(oldRankSpellInfo))
            {
                // replace by new high rank
                if (spellInfo->IsHighRankOf(oldRankSpellInfo))
                {
                    newspell.active = itr2->second.active;

                    if (newspell.active == ACT_ENABLED)
                        ToggleAutocast(oldRankSpellInfo, false);

                    unlearnSpell(itr2->first, false, false);
                    break;
                }
                // ignore new lesser rank
                else
                    return false;
            }
        }
    }

    m_spells[spellId] = newspell;

    if (spellInfo->IsPassive() && (!spellInfo->CasterAuraState || HasAuraState(AuraStateType(spellInfo->CasterAuraState))))
        CastSpell(this, spellId, true);
    else
        m_charmInfo->AddSpellToActionBar(spellInfo);

    if (newspell.active == ACT_ENABLED)
        ToggleAutocast(spellInfo, true);

    return true;
}

bool Pet::learnSpell(uint32 p_SpellID)
{
    // prevent duplicated entires in spell book
    if (!addSpell(p_SpellID))
        return false;

    if (!m_loading)
    {
        WorldPacket l_Data(SMSG_PET_LEARNED_SPELLS, 4 + 4);
        l_Data << uint32(1);            ///< Count
        l_Data << uint32(p_SpellID);    ///< SpellID
        m_owner->GetSession()->SendPacket(&l_Data);
        m_owner->PetSpellInitialize();
    }
    return true;
}

void Pet::InitLevelupSpellsForLevel()
{
    uint8 level = getLevel();

    if (PetLevelupSpellSet const* levelupSpells = GetCreatureTemplate()->family ? sSpellMgr->GetPetLevelupSpellList(GetCreatureTemplate()->family) : NULL)
    {
        // PetLevelupSpellSet ordered by levels, process in reversed order
        for (PetLevelupSpellSet::const_reverse_iterator itr = levelupSpells->rbegin(); itr != levelupSpells->rend(); ++itr)
        {
            // will called first if level down
            if (itr->first > level)
                unlearnSpell(itr->second, true);                 // will learn prev rank if any
            // will called if level up
            else
                learnSpell(itr->second);                        // will unlearn prev rank if any
        }
    }

    // default spells (can be not learned if pet level (as owner level decrease result for example) less first possible in normal game)
    if (PetDefaultSpellsEntry const* defSpells = sSpellMgr->GetPetDefaultSpellsEntry(int32(GetEntry())))
    {
        for (uint8 i = 0; i < MAX_CREATURE_SPELL_DATA_SLOT; ++i)
        {
            SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(defSpells->spellid[i]);
            if (!spellEntry)
                continue;

            // This prevent to add spells with no cooldown - cheating !
            if (!spellEntry->RecoveryTime && !spellEntry->StartRecoveryCategory &&
                !spellEntry->StartRecoveryTime && !spellEntry->CategoryRecoveryTime)
                continue;

            // will called first if level down
            if (spellEntry->SpellLevel > level)
                unlearnSpell(spellEntry->Id, true);
            // will called if level up
            else
                learnSpell(spellEntry->Id);
        }
    }
}

bool Pet::unlearnSpell(uint32 p_SpellID, bool p_LearnPrev, bool p_ClearAb)
{
    if (removeSpell(p_SpellID, p_LearnPrev, p_ClearAb))
    {
        if (!m_loading)
        {
            WorldPacket l_Data(SMSG_PET_UNLEARNED_SPELLS, 4 + 4);
            l_Data << uint32(1);            ///< count
            l_Data << uint32(p_SpellID);    ///< SpellID
            m_owner->GetSession()->SendPacket(&l_Data);
        }
        return true;
    }
    return false;
}

bool Pet::removeSpell(uint32 spell_id, bool learn_prev, bool clear_ab)
{
    PetSpellMap::iterator itr = m_spells.find(spell_id);
    if (itr == m_spells.end())
        return false;

    if (itr->second.state == PETSPELL_REMOVED)
        return false;

    if (itr->second.state == PETSPELL_NEW)
        m_spells.erase(itr);
    else
        itr->second.state = PETSPELL_REMOVED;

    RemoveAurasDueToSpell(spell_id);

    if (learn_prev)
    {
        if (uint32 prev_id = sSpellMgr->GetPrevSpellInChain (spell_id))
            learnSpell(prev_id);
        else
            learn_prev = false;
    }

    // if remove last rank or non-ranked then update action bar at server and client if need
    if (clear_ab && !learn_prev && m_charmInfo->RemoveSpellFromActionBar(spell_id))
    {
        if (!m_loading)
        {
            // need update action bar for last removed rank
            if (Unit* owner = GetOwner())
                if (owner->IsPlayer())
                    owner->ToPlayer()->PetSpellInitialize();
        }
    }

    return true;
}

void Pet::CleanupActionBar()
{
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (UnitActionBarEntry const* ab = m_charmInfo->GetActionBarEntry(i))
        {
            if (ab->GetAction() && ab->IsActionBarForSpell())
            {
                if (!HasSpell(ab->GetAction()))
                    m_charmInfo->SetActionBar(i, 0, ACT_PASSIVE);
                else if (ab->GetType() == ACT_ENABLED)
                {
                    if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(ab->GetAction()))
                        ToggleAutocast(spellInfo, true);
                }
            }
        }
    }
}

void Pet::InitPetCreateSpells()
{
    m_charmInfo->InitPetActionBar();
    m_spells.clear();

    LearnPetPassives();
    InitLevelupSpellsForLevel();

    CastPetAuras(false);
}

void Pet::ToggleAutocast(SpellInfo const* spellInfo, bool apply)
{
    if (!spellInfo->IsAutocastable())
        return;

    uint32 spellid = spellInfo->Id;

    PetSpellMap::iterator itr = m_spells.find(spellid);
    if (itr == m_spells.end())
        return;

    uint32 i;

    if (apply)
    {
        for (i = 0; i < m_autospells.size() && m_autospells[i] != spellid; ++i)
            ;                                               // just search

        if (i == m_autospells.size())
        {
            m_autospells.push_back(spellid);

            if (itr->second.active != ACT_ENABLED)
            {
                itr->second.active = ACT_ENABLED;
                if (itr->second.state != PETSPELL_NEW)
                    itr->second.state = PETSPELL_CHANGED;
            }
        }
    }
    else
    {
        AutoSpellList::iterator itr2 = m_autospells.begin();
        for (i = 0; i < m_autospells.size() && m_autospells[i] != spellid; ++i, ++itr2)
            ;                                               // just search

        if (i < m_autospells.size())
        {
            m_autospells.erase(itr2);
            if (itr->second.active != ACT_DISABLED)
            {
                itr->second.active = ACT_DISABLED;
                if (itr->second.state != PETSPELL_NEW)
                    itr->second.state = PETSPELL_CHANGED;
            }
        }
    }
}

bool Pet::IsPermanentPetFor(Player* owner)
{
    switch (getPetType())
    {
        case SUMMON_PET:
            switch (owner->getClass())
            {
                case CLASS_WARLOCK:
                    return GetCreatureTemplate()->type == CREATURE_TYPE_DEMON;
                case CLASS_DEATH_KNIGHT:
                    return GetCreatureTemplate()->type == CREATURE_TYPE_UNDEAD;
                case CLASS_MAGE:
                    return GetCreatureTemplate()->type == CREATURE_TYPE_ELEMENTAL;
                default:
                    return false;
            }
        case HUNTER_PET:
            return true;
        default:
            return false;
    }
}

bool Pet::Create(uint32 guidlow, Map* map, uint32 phaseMask, uint32 Entry)
{
    ASSERT(map);
    SetMap(map);

    SetPhaseMask(phaseMask, false);
    Object::_Create(guidlow, Entry, HIGHGUID_PET);

    m_DBTableGuid = guidlow;
    m_originalEntry = Entry;

    if (!InitEntry(Entry))
        return false;

    // Force regen flag for player pets, just like we do for players themselves
    SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
    SetSheath(SHEATH_STATE_MELEE);

    return true;
}

bool Pet::HasSpell(uint32 spell) const
{
    PetSpellMap::const_iterator itr = m_spells.find(spell);
    return itr != m_spells.end() && itr->second.state != PETSPELL_REMOVED;
}

// Get all passive spells in our skill line
void Pet::LearnPetPassives()
{
    CreatureTemplate const* cInfo = GetCreatureTemplate();
    if (!cInfo)
        return;

    CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(cInfo->family);
    if (!cFamily)
        return;

    PetFamilySpellsStore::const_iterator petStore = sPetFamilySpellsStore.find(cFamily->ID);
    if (petStore != sPetFamilySpellsStore.end())
    {
        // For general hunter pets skill 270
        // Passive 01~10, Passive 00 (20782, not used), Ferocious Inspiration (34457)
        // Scale 01~03 (34902~34904, bonus from owner, not used)
        for (PetFamilySpellsSet::const_iterator petSet = petStore->second.begin(); petSet != petStore->second.end(); ++petSet)
            addSpell(*petSet, ACT_DECIDE, PETSPELL_NEW, PETSPELL_FAMILY);
    }
}

void Pet::CastPetAuras(bool current)
{
    Unit* owner = GetOwner();
    if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
        return;

    if (!IsPermanentPetFor(owner->ToPlayer()))
        return;

    for (PetAuraSet::const_iterator itr = owner->m_petAuras.begin(); itr != owner->m_petAuras.end();)
    {
        PetAura const* pa = *itr;
        ++itr;

        if (!current && pa->IsRemovedOnChangePet())
            owner->RemovePetAura(pa);
        else
            CastPetAura(pa);
    }
}

void Pet::CastPetAura(PetAura const* aura)
{
    uint32 auraId = aura->GetAura(GetEntry());
    if (!auraId)
        return;

    CastSpell(this, auraId, true);
}

bool Pet::IsPetAura(Aura const* aura)
{
    Unit* owner = GetOwner();

    if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
        return false;

    // if the owner has that pet aura, return true
    for (PetAuraSet::const_iterator itr = owner->m_petAuras.begin(); itr != owner->m_petAuras.end(); ++itr)
    {
        if ((*itr)->GetAura(GetEntry()) == aura->GetId())
            return true;
    }
    return false;
}

void Pet::learnSpellHighRank(uint32 spellid)
{
    learnSpell(spellid);

    if (uint32 next = sSpellMgr->GetNextSpellInChain(spellid))
        learnSpellHighRank(next);
}

void Pet::SynchronizeLevelWithOwner()
{
    Unit* owner = GetOwner();
    if (!owner || owner->GetTypeId() != TYPEID_PLAYER)
        return;

    switch (getPetType())
    {
        case SUMMON_PET:
        case HUNTER_PET:
            GivePetLevel(owner->getLevel());
            break;
        default:
            break;
    }
}

void Pet::LearnSpecializationSpell()
{
    for (uint32 i = 0; i < sSpecializationSpellStore.GetNumRows(); i++)
    {
        SpecializationSpellEntry const* specializationEntry = sSpecializationSpellStore.LookupEntry(i);
        if (!specializationEntry)
            continue;

        if (specializationEntry->SpecializationEntry != GetSpecializationId())
            continue;

        learnSpell(specializationEntry->LearnSpell);
    }
}

void Pet::UnlearnSpecializationSpell()
{
    for (uint32 i = 0; i < sSpecializationSpellStore.GetNumRows(); i++)
    {
        SpecializationSpellEntry const* specializationEntry = sSpecializationSpellStore.LookupEntry(i);
        if (!specializationEntry)
            continue;

        if (specializationEntry->SpecializationEntry != GetSpecializationId())
            continue;

        unlearnSpell(specializationEntry->LearnSpell, false);
    }
}

void Pet::ProhibitSpellSchool(SpellSchoolMask idSchoolMask, uint32 unTimeMs)
{
    Player* owner = GetOwner();
    time_t curTime = time(NULL);
    for (PetSpellMap::const_iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
    {
        if (itr->second.state == PETSPELL_REMOVED)
            continue;
        uint32 unSpellId = itr->first;
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(unSpellId);
        ASSERT(spellInfo);

        // Not send cooldown for this spells
        if (spellInfo->IsCooldownStartedOnEvent())
            continue;

        if ((spellInfo->PreventionType & (SpellPreventionMask::Silence)) == 0)
            continue;

        if ((idSchoolMask & spellInfo->GetSchoolMask()) && GetCreatureSpellCooldownDelay(unSpellId) < unTimeMs)
        {
            WorldPacket data(SMSG_SPELL_COOLDOWN, 16 + 2 + 1 + 4 + 4 + 4);
            data.appendPackGUID(GetGUID());
            data << uint8(CooldownFlags::CooldownFlagNone);
            data << uint32(1);
            data << uint32(unSpellId);
            data << uint32(uint32(unTimeMs));

            if (owner)
                owner->GetSession()->SendPacket(&data);

            _AddCreatureSpellCooldown(unSpellId, curTime + unTimeMs/IN_MILLISECONDS);
        }
    }
}
