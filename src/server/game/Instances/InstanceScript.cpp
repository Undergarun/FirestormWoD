////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "DatabaseEnv.h"
#include "Map.h"
#include "Player.h"
#include "Group.h"
#include "GameObject.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Log.h"
#include "LFGMgr.h"
#include "Guild.h"
#include "WowTime.hpp"

InstanceScript::InstanceScript(Map* p_Map)
{
    instance                    = p_Map;
    m_CompletedEncounters       = 0;
    m_ChallengeStarted          = false;
    m_ConditionCompleted        = false;
    m_CreatureKilled            = 0;
    m_StartChallengeTime        = 0;
    m_ChallengeDoorGuid         = 0;
    m_ChallengeOrbGuid          = 0;
    m_ChallengeTime             = 0;
    m_MedalType                 = eChallengeMedals::MedalTypeNone;

    m_InstanceGuid              = MAKE_NEW_GUID(p_Map->GetId(), 0, HIGHGUID_INSTANCE_SAVE);
    m_BeginningTime             = 0;
    m_ScenarioID                = 0;
    m_ScenarioStep              = 0;
    m_EncounterTime             = 0;
    m_DisabledMask              = 0;

    m_InCombatResCount          = 0;
    m_MaxInCombatResCount       = 0;
    m_CombatResChargeTime       = 0;
    m_NextCombatResChargeTime   = 0;

    m_EncounterDatas            = EncounterDatas();
}

void InstanceScript::SaveToDB()
{
    std::string data = GetSaveData();
    if (data.empty())
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_INSTANCE_DATA);
    stmt->setUInt32(0, GetCompletedEncounterMask());
    stmt->setString(1, data);
    stmt->setUInt32(2, instance->GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

void InstanceScript::HandleGameObject(uint64 GUID, bool open, GameObject* go)
{
    if (!go)
        go = instance->GetGameObject(GUID);
    if (go)
        go->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
    else
        sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript: HandleGameObject failed");
}

void InstanceScript::UpdateOperations(uint32 const p_Diff)
{
    for (auto l_It = m_TimedDelayedOperations.begin(); l_It != m_TimedDelayedOperations.end(); l_It++)
    {
        l_It->first -= p_Diff;

        if (l_It->first < 0)
        {
            l_It->second();
            l_It->second = nullptr;
        }
    }

    uint32 l_TimedDelayedOperationCountToRemove = (uint32)std::count_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](std::pair<int32, std::function<void()>> const& p_Pair) -> bool
    {
        return p_Pair.second == nullptr;
    });

    for (uint32 l_I = 0; l_I < l_TimedDelayedOperationCountToRemove; l_I++)
    {
        auto l_It = std::find_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](std::pair<int32, std::function<void()>> const& p_Pair) -> bool
        {
            return p_Pair.second == nullptr;
        });

        if (l_It != std::end(m_TimedDelayedOperations))
            m_TimedDelayedOperations.erase(l_It);
    }

    if (m_TimedDelayedOperations.empty() && !m_EmptyWarned)
    {
        m_EmptyWarned = true;
        LastOperationCalled();
    }
}

bool InstanceScript::IsEncounterInProgress() const
{
    for (std::vector<BossInfo>::const_iterator itr = m_Bosses.begin(); itr != m_Bosses.end(); ++itr)
        if (itr->state == IN_PROGRESS)
            return true;

    return false;
}

void InstanceScript::OnPlayerEnter(Player* p_Player)
{
    SendScenarioState(ScenarioData(m_ScenarioID, m_ScenarioStep), p_Player);
    UpdateCriteriasAfterLoading();

    /// In challenge mode, item set bonuses and gem bonuses are disabled
    /// Disable them
    if (instance->IsChallengeMode())
    {
        HandleItemSetBonusesOnPlayers(false);
        HandleGemBonusesOnPlayers(false);
    }
}

void InstanceScript::OnPlayerExit(Player* p_Player)
{
    p_Player->RemoveAura(eInstanceSpells::SpellDetermination);

    /// In challenge mode, item set bonuses and gem bonuses are disabled
    /// Re enable them
    if (instance->IsChallengeMode())
    {
        HandleItemSetBonusesOnPlayers(true);
        HandleGemBonusesOnPlayers(true);
    }
}

void InstanceScript::LoadMinionData(const MinionData* data)
{
    while (data->entry)
    {
        if (data->bossId < m_Bosses.size())
            minions.insert(std::make_pair(data->entry, MinionInfo(&m_Bosses[data->bossId])));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadMinionData: " UI64FMTD " minions loaded.", uint64(minions.size()));
}

void InstanceScript::SetBossNumber(uint32 p_Number)
{
    m_Bosses.resize(p_Number);
    m_BossesScenarios.resize(p_Number);
}

void InstanceScript::LoadDoorData(const DoorData* data)
{
    while (data->entry)
    {
        if (data->bossId < m_Bosses.size())
            doors.insert(std::make_pair(data->entry, DoorInfo(&m_Bosses[data->bossId], data->type, BoundaryType(data->boundary))));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadDoorData: " UI64FMTD " doors loaded.", uint64(doors.size()));
}

void InstanceScript::LoadScenariosInfos(BossScenarios const* p_Scenarios, uint32 p_ScenarioID)
{
    while (p_Scenarios->m_ScenarioID)
    {
        if (p_Scenarios->m_BossID < m_Bosses.size())
            m_BossesScenarios[p_Scenarios->m_BossID] = BossScenarios(p_Scenarios->m_BossID, p_Scenarios->m_ScenarioID);

        ++p_Scenarios;
    }

    m_ScenarioID = p_ScenarioID;
}

void InstanceScript::UpdateMinionState(Creature* minion, EncounterState state)
{
    switch (state)
    {
        case NOT_STARTED:
            if (!minion->isAlive())
                minion->Respawn();
            else if (minion->isInCombat())
                minion->AI()->EnterEvadeMode();
            break;
        case IN_PROGRESS:
            if (!minion->isAlive())
                minion->Respawn();
            else if (!minion->getVictim())
                minion->AI()->DoZoneInCombat();
            break;
        default:
            break;
    }
}

void InstanceScript::UpdateDoorState(GameObject* door, bool p_WithDelay /*= false*/)
{
    if (!door)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());
    if (lower == upper)
        return;

    bool open = true;
    for (DoorInfoMap::iterator itr = lower; itr != upper && open; ++itr)
    {
        switch (itr->second.type)
        {
            case DOOR_TYPE_ROOM:
                open = (itr->second.bossInfo->state != IN_PROGRESS);
                break;
            case DOOR_TYPE_PASSAGE:
                open = (itr->second.bossInfo->state == DONE);
                break;
            case DOOR_TYPE_SPAWN_HOLE:
                open = (itr->second.bossInfo->state == IN_PROGRESS);
                break;
            default:
                break;
        }
    }

    /// Delay Door closing, like retail
    if (!open)
    {
        if (!p_WithDelay)
        {
            door->SetGoState(GOState::GO_STATE_READY);
            return;
        }

        uint64 l_DoorGuid = door->GetGUID();
        AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this, l_DoorGuid]() -> void
        {
            if (GameObject* l_Door = sObjectAccessor->FindGameObject(l_DoorGuid))
                UpdateDoorState(l_Door);
        });
    }
    else
        door->SetGoState(GOState::GO_STATE_ACTIVE);
}

void InstanceScript::AddDoor(GameObject* door, bool add)
{
    if (!door)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());

    if (lower == upper)
        return;

    for (DoorInfoMap::iterator itr = lower; itr != upper; ++itr)
    {
        DoorInfo const& data = itr->second;

        if (add)
        {
            data.bossInfo->door[data.type].insert(door);
            switch (data.boundary)
            {
                default:
                case BOUNDARY_NONE:
                    break;
                case BOUNDARY_N:
                case BOUNDARY_S:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX();
                    break;
                case BOUNDARY_E:
                case BOUNDARY_W:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionY();
                    break;
                case BOUNDARY_NW:
                case BOUNDARY_SE:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() + door->GetPositionY();
                    break;
                case BOUNDARY_NE:
                case BOUNDARY_SW:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() - door->GetPositionY();
                    break;
            }
        }
        else
            data.bossInfo->door[data.type].erase(door);
    }

    if (add)
        UpdateDoorState(door);
}

void InstanceScript::OnGameObjectRemove(GameObject* p_Go)
{
    /// - If gameobject is door, remove it from DoorInfoMap
    {
        DoorInfoMap::iterator l_Lower = doors.lower_bound(p_Go->GetEntry());
        DoorInfoMap::iterator l_Upper = doors.upper_bound(p_Go->GetEntry());

        if (l_Lower != l_Upper)
        {
            for (DoorInfoMap::iterator l_Iterator = l_Lower; l_Iterator != l_Upper; ++l_Iterator)
            {
                DoorInfo const& l_DoorInfo = l_Iterator->second;
                l_DoorInfo.bossInfo->door[l_DoorInfo.type].erase(p_Go);
            }
        }
    }

    ZoneScript::OnGameObjectRemove(p_Go);
}

void InstanceScript::AddMinion(Creature* minion, bool add)
{
    MinionInfoMap::iterator itr = minions.find(minion->GetEntry());
    if (itr == minions.end())
        return;

    if (add)
        itr->second.bossInfo->minion.insert(minion);
    else
        itr->second.bossInfo->minion.erase(minion);
}

bool InstanceScript::SetBossState(uint32 p_ID, EncounterState p_State)
{
    if (p_ID < m_Bosses.size())
    {
        BossInfo* l_BossInfos = &m_Bosses[p_ID];
        BossScenarios* l_BossScenario = &m_BossesScenarios[p_ID];

        if (l_BossInfos->state == EncounterState::TO_BE_DECIDED) // loading
        {
            l_BossInfos->state = p_State;

            if (p_State == EncounterState::DONE)
                SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, uint32(time(nullptr)), m_BeginningTime, 0));

            for (uint32 l_Type = 0; l_Type < DoorType::MAX_DOOR_TYPES; ++l_Type)
            {
                for (DoorSet::iterator l_Iter = l_BossInfos->door[l_Type].begin(); l_Iter != l_BossInfos->door[l_Type].end(); ++l_Iter)
                    UpdateDoorState(*l_Iter, true);
            }

            return false;
        }
        else
        {
            if (l_BossInfos->state == p_State)
                return false;

            switch (p_State)
            {
                case EncounterState::DONE:
                {
                    for (MinionSet::iterator l_Iter = l_BossInfos->minion.begin(); l_Iter != l_BossInfos->minion.end(); ++l_Iter)
                    {
                        if ((*l_Iter)->isWorldBoss() && (*l_Iter)->isAlive())
                            return false;
                    }

                    SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, uint32(time(nullptr)), m_BeginningTime, 0));

                    /// This buff disappears immediately after killing the boss
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::SpellDetermination);

                    /// End of challenge
                    if (p_ID == (m_Bosses.size() - 1))
                    {
                        if (instance->IsChallengeMode() && m_ChallengeStarted && m_ConditionCompleted)
                        {
                            m_ChallengeStarted = false;

                            SendChallengeNewPlayerRecord();
                            SendChallengeModeComplete(RewardChallengers());
                            SendChallengeStopElapsedTimer(1);

                            SaveChallengeDatasIfNeeded();

                            DoUpdateAchievementCriteria(AchievementCriteriaTypes::ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON, instance->GetId(), m_MedalType);
                        }

                        SendScenarioState(ScenarioData(m_ScenarioID, ++m_ScenarioStep));
                    }

                    /// Handle Guild challenges
                    {
                        InstanceMap::PlayerList const& l_PlayersMap = instance->GetPlayers();
                        for (InstanceMap::PlayerList::const_iterator l_Itr = l_PlayersMap.begin(); l_Itr != l_PlayersMap.end(); ++l_Itr)
                        {
                            if (Player* l_Player = l_Itr->getSource())
                            {
                                if (l_Player->GetGroup() && l_Player->GetGroup()->IsGuildGroup(0, true, true))
                                {
                                    if (Guild* l_Guild = l_Player->GetGuild())
                                    {
                                        if (instance->IsRaid())
                                            l_Guild->CompleteGuildChallenge(GuildChallengeType::ChallengeRaid);
                                        else if (instance->IsChallengeMode())
                                            l_Guild->CompleteGuildChallenge(GuildChallengeType::ChallengeDungeonChallenge);
                                        else if (instance->IsDungeon())
                                            l_Guild->CompleteGuildChallenge(GuildChallengeType::ChallengeDungeon);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    m_EncounterTime = 0;

                    ResetCombatResurrection();

                    /// Bloodlust, Heroism, Temporal Displacement and Insanity debuffs are removed at the end of an encounter
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::HunterInsanity);
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::MageTemporalDisplacement);
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::ShamanExhaustion);
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::ShamanSated);
                    break;
                }
                case EncounterState::IN_PROGRESS:
                {
                    m_EncounterTime = uint32(time(nullptr));
                    StartCombatResurrection();
                    break;
                }
                case EncounterState::FAIL:
                {
                    /// Now you have to fight for at least 3mins to get a stack.
                    /// It was nerfed due to people intentionally reseting the boss to gain max stack to kill the boss faster.
                    if (m_EncounterTime && instance->IsLFR() && (time(nullptr) - m_EncounterTime) >= 3 * TimeConstants::MINUTE)
                        DoCastSpellOnPlayers(eInstanceSpells::SpellDetermination);

                    /// Upon reseting a boss, all combat bloodlust spells will have their cooldowns reset
                    for (uint8 l_I = 0; l_I < eInstanceSpells::MaxBloodlustSpells; ++l_I)
                        DoRemoveSpellCooldownOnPlayers(g_BloodlustSpells[l_I]);

                    break;
                }
                default:
                    break;
            }

            l_BossInfos->state = p_State;
            SaveToDB();
        }

        for (uint32 l_Type = 0; l_Type < DoorType::MAX_DOOR_TYPES; ++l_Type)
        {
            for (DoorSet::iterator l_Iter = l_BossInfos->door[l_Type].begin(); l_Iter != l_BossInfos->door[l_Type].end(); ++l_Iter)
                UpdateDoorState(*l_Iter, true);
        }

        for (MinionSet::iterator l_Iter = l_BossInfos->minion.begin(); l_Iter != l_BossInfos->minion.end(); ++l_Iter)
            UpdateMinionState(*l_Iter, p_State);

        /// Send encounters for Bosses
        if (instance->IsRaid())
        {
            if (p_State == EncounterState::IN_PROGRESS)
                SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_START);
            else if (p_State == EncounterState::DONE || p_State == EncounterState::FAIL)
                SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_END);
        }

        return true;
    }

    return false;
}

std::string InstanceScript::LoadBossState(const char * data)
{
    if (!data)
        return NULL;
    std::istringstream loadStream(data);
    uint32 buff;
    uint32 bossId = 0;
    for (std::vector<BossInfo>::iterator i = m_Bosses.begin(); i != m_Bosses.end(); ++i, ++bossId)
    {
        loadStream >> buff;
        if (buff < TO_BE_DECIDED)
            SetBossState(bossId, (EncounterState)buff);
    }
    return loadStream.str();
}

std::string InstanceScript::GetBossSaveData()
{
    std::ostringstream saveStream;
    for (std::vector<BossInfo>::iterator i = m_Bosses.begin(); i != m_Bosses.end(); ++i)
        saveStream << (uint32)i->state << ' ';
    return saveStream.str();
}

void InstanceScript::DoUseDoorOrButton(uint64 uiGuid, uint32 uiWithRestoreTime, bool bUseAlternativeState)
{
    if (!uiGuid)
        return;

    GameObject* go = instance->GetGameObject(uiGuid);

    if (go)
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR || go->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (go->getLootState() == GO_READY)
                go->UseDoorOrButton(uiWithRestoreTime, bUseAlternativeState);
            else if (go->getLootState() == GO_ACTIVATED)
                go->ResetDoorOrButton();
        }
        else
            sLog->outError(LOG_FILTER_GENERAL, "SD2: Script call DoUseDoorOrButton, but gameobject entry %u is type %u.", go->GetEntry(), go->GetGoType());
    }
}

void InstanceScript::DoRespawnGameObject(uint64 uiGuid, uint32 uiTimeToDespawn)
{
    if (GameObject* go = instance->GetGameObject(uiGuid))
    {
        //not expect any of these should ever be handled
        if (go->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE || go->GetGoType() == GAMEOBJECT_TYPE_DOOR ||
            go->GetGoType() == GAMEOBJECT_TYPE_BUTTON || go->GetGoType() == GAMEOBJECT_TYPE_TRAP)
            return;

        if (go->isSpawned())
            return;

        go->SetRespawnTime(uiTimeToDespawn);
        go->UpdateObjectVisibility();
    }
}

void InstanceScript::DoUpdateWorldState(uint32 uiStateId, uint32 uiStateData)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            if (Player* player = itr->getSource())
                player->SendUpdateWorldState(uiStateId, uiStateData);
    }
    else
        sLog->outDebug(LOG_FILTER_TSCR, "DoUpdateWorldState attempt send data but no players in map.");
}

// Send Notify to all players in instance
void InstanceScript::DoSendNotifyToInstance(char const* format, ...)
{
    InstanceMap::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        va_list ap;
        va_start(ap, format);
        char buff[1024];
        vsnprintf(buff, 1024, format, ap);
        va_end(ap);
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            if (Player* player = i->getSource())
                if (WorldSession* session = player->GetSession())
                    session->SendNotification("%s", buff);
    }
}

// Reset Achievement Criteria for all players in instance
void InstanceScript::DoResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 /*= 0*/, uint64 miscValue2 /*= 0*/, bool evenIfCriteriaComplete /*= false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->GetAchievementMgr().ResetAchievementCriteria(type, miscValue1, miscValue2, evenIfCriteriaComplete);
}

// Complete Achievement for all players in instance
void InstanceScript::DoCompleteAchievement(uint32 achievement)
{
  AchievementEntry const* pAE = sAchievementStore.LookupEntry(achievement);
  Map::PlayerList const &plrList = instance->GetPlayers();
  if (!pAE)
      return;

  if (!plrList.isEmpty())
      for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
          if (Player *pPlayer = i->getSource())
              pPlayer->CompletedAchievement(pAE);
}

// Update Achievement Criteria for all players in instance
void InstanceScript::DoUpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, uint32 miscValue3 /*=0*/, Unit* unit /*= NULL*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3, unit);
}

// Start timed achievement for all players in instance
void InstanceScript::DoStartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().StartTimedAchievement(type, entry);
}

// Stop timed achievement for all players in instance
void InstanceScript::DoStopTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().RemoveTimedAchievement(type, entry);
}

// Remove Auras due to Spell on all players in instance
void InstanceScript::DoRemoveAurasDueToSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    if (!PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            if (Player* player = itr->getSource())
            {
                player->RemoveAurasDueToSpell(spell);
                if (Pet* pet = player->GetPet())
                    pet->RemoveAurasDueToSpell(spell);
            }
        }
    }
}

void InstanceScript::DoRemoveForcedMovementsOnPlayers(uint64 p_Source)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (!l_PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
            {
                if (l_Player->HasMovementForce(p_Source))
                    l_Player->SendApplyMovementForce(p_Source, false, Position());
            }
        }
    }
}

// Cast spell on all players in instance
void InstanceScript::DoCastSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->CastSpell(player, spell, true);
}

void InstanceScript::DoSetAlternatePowerOnPlayers(int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->SetPower(POWER_ALTERNATE_POWER, value);
}

void InstanceScript::DoModifyPlayerCurrencies(uint32 id, int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->ModifyCurrency(id, value);
}

void InstanceScript::DoNearTeleportPlayers(const Position pos, bool casting /*=false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), casting);
}

void InstanceScript::DoKilledMonsterKredit(uint32 questId, uint32 entry, uint64 guid/* =0*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                if (pPlayer->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->KilledMonsterCredit(entry, guid);
}

// Add aura on all players in instance
void InstanceScript::DoAddAuraOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->AddAura(spell, player);
}

void InstanceScript::DoRemoveSpellCooldownOnPlayers(uint32 p_SpellID)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();

    if (!l_PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
            {
                if (l_Player->HasSpellCooldown(p_SpellID))
                    l_Player->RemoveSpellCooldown(p_SpellID, true);
            }
        }
    }
}

void InstanceScript::DoRemoveSpellCooldownWithTimeOnPlayers(uint32 p_MinRecoveryTime)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (l_PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->RemoveSpellCooldownsWithTime(p_MinRecoveryTime);
    }
}

void InstanceScript::HandleItemSetBonusesOnPlayers(bool p_Apply)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (l_PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->HandleItemSetBonuses(p_Apply);
    }
}

void InstanceScript::HandleGemBonusesOnPlayers(bool p_Apply)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (l_PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->HandleGemBonuses(p_Apply);
    }
}

void InstanceScript::DoCombatStopOnPlayers()
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (l_PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (!l_Player->isInCombat())
                continue;

            l_Player->CombatStop();
        }
    }
}

void InstanceScript::SetCriteriaProgressOnPlayers(CriteriaEntry const* p_Criteria, uint64 p_ChangeValue, ProgressType p_Type)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (l_PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            l_Player->GetAchievementMgr().SetCriteriaProgress(p_Criteria, p_ChangeValue, l_Player, p_Type);
            l_Player->GetAchievementMgr().SetCompletedAchievementsIfNeeded(p_Criteria, l_Player);
        }
    }
}

void InstanceScript::RepopPlayersAtGraveyard(bool p_ForceGraveyard /*= false*/)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    if (l_PlayerList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->RepopAtGraveyard(p_ForceGraveyard);
    }
}

void InstanceScript::RespawnCreature(uint64 p_Guid /*= 0*/)
{
    auto l_WorldObjects = instance->GetAllWorldObjectOnMap();
    for (auto l_WorldObject = l_WorldObjects->begin(); l_WorldObject != l_WorldObjects->end(); l_WorldObject++)
    {
        if ((*l_WorldObject)->GetTypeId() != TypeID::TYPEID_UNIT)
            continue;

        Creature* l_Creature = (*l_WorldObject)->ToCreature();

        if (p_Guid && l_Creature->GetGUID() != p_Guid)
            continue;

        if (l_Creature->isAlive() && l_Creature->isInCombat() && l_Creature->IsAIEnabled)
            l_Creature->AI()->EnterEvadeMode();
        else if (l_Creature->isDead())
        {
            l_Creature->Respawn();

            uint64 l_Guid = l_Creature->GetGUID();
            AddTimedDelayedOperation(100, [this, l_Guid]() -> void
            {
                if (Creature* l_Creature = instance->GetCreature(l_Guid))
                {
                    l_Creature->GetMotionMaster()->Clear();
                    l_Creature->GetMotionMaster()->MoveTargetedHome();
                }
            });
        }
    }
}

bool InstanceScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/ /*= NULL*/, uint64 /*miscvalue1*/ /*= 0*/)
{
    sLog->outError(LOG_FILTER_GENERAL, "Achievement system call InstanceScript::CheckAchievementCriteriaMeet but instance script for map %u not have implementation for achievement criteria %u",
        instance->GetId(), criteria_id);
    return false;
}

bool InstanceScript::CheckRequiredBosses(uint32 p_ID, Player const* p_Player) const
{
    /// Disable case (for LFR)
    if (m_DisabledMask & (1 << p_ID))
        return false;

    /// Disable boss until challenge mode starts
    if (instance->IsChallengeMode() && !IsChallengeModeStarted())
        return false;

    if (p_Player && p_Player->isGameMaster())
        return true;

    if (instance->GetPlayersCountExceptGMs() > instance->ToInstanceMap()->GetMaxPlayers())
        return false;

    return true;
}

void InstanceScript::SendEncounterUnit(uint32 p_Type, Unit* p_Unit /*= NULL*/, uint8 p_Param1 /*= 0*/, uint8 /*p_Param2*/ /*= 0*/)
{
    WorldPacket l_Data;

    switch (p_Type)
    {
        case EncounterFrameType::ENCOUNTER_FRAME_ENGAGE:
            if (!p_Unit)
                return;
            p_Unit->BuildEncounterFrameData(&l_Data, true, p_Param1);
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE:
            if (!p_Unit)
                return;
            p_Unit->BuildEncounterFrameData(&l_Data, false);
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_UPDATE_PRIORITY:
            if (!p_Unit)
                return;
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_CHANGE_PRIORITY, 8 + 1);
            l_Data.append(p_Unit->GetPackGUID());
            l_Data << uint8(p_Param1);  // TargetFramePriority
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_START_TIMER:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_TIMER_START, 4);
            l_Data << int32(0);         // TimeRemaining
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_START_OBJECTIVE:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_START, 4);
            l_Data << int32(0);         // ObjectiveID
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_COMPLETE_OBJECTIVE:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_COMPLETE, 4);
            l_Data << int32(0);         // ObjectiveID
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_START:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_START, 4 * 4);
            l_Data << uint32(m_InCombatResCount);
            l_Data << int32(m_MaxInCombatResCount);
            l_Data << int32(m_CombatResChargeTime);
            l_Data << uint32(m_NextCombatResChargeTime);
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_UPDATE_OBJECTIVE:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_UPDATE, 4 * 2);
            l_Data << int32(0);         // ProgressAmount
            l_Data << int32(0);         // ObjectiveID
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_END:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_END, 0);
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_IN_COMBAT_RESURRECTION:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_IN_COMBAT_RESURRECTION, 0);
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_PHASE_SHIFT_CHANGED:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_PHASE_SHIFT_CHANGED, 0);
            break;
        case EncounterFrameType::ENCOUNTER_FRAME_GAIN_COMBAT_RESURRECTION_CHARGE:
            l_Data.Initialize(Opcodes::SMSG_INSTANCE_ENCOUNTER_GAIN_COMBAT_RESURRECTION_CHARGE, 4 * 2);
            l_Data << int32(m_InCombatResCount);
            l_Data << uint32(m_CombatResChargeTime);
            break;
        default:
            break;
    }

    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendScenarioState(ScenarioData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(SMSG_SCENARIO_STATE);

    l_Data << int32(p_Data.m_ScenarioID);
    l_Data << int32(p_Data.m_StepID);
    l_Data << uint32(instance->GetDifficultyID());
    l_Data << uint32(p_Data.m_WaveCurrent);
    l_Data << uint32(p_Data.m_WaveMax);
    l_Data << uint32(p_Data.m_TimerDuration);

    l_Data << uint32(p_Data.m_CriteriaCount);
    l_Data << uint32(p_Data.m_BonusCount);

    for (CriteriaProgressData l_ProgressData : p_Data.m_CriteriaProgress)
        BuildCriteriaProgressPacket(&l_Data, l_ProgressData);

    for (BonusObjectiveData l_BonusObjective : p_Data.m_BonusObjectives)
    {
        l_Data << int32(l_BonusObjective.m_ObjectiveID);
        l_Data.WriteBit(l_BonusObjective.m_ObjectiveComplete);
        l_Data.FlushBits();
    }

    l_Data.WriteBit(p_Data.m_ScenarioComplete);
    l_Data.FlushBits();

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(SMSG_SCENARIO_PROGRESS_UPDATE, 39);
    BuildCriteriaProgressPacket(&l_Data, p_Data);

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::BuildCriteriaProgressPacket(WorldPacket* p_Data, CriteriaProgressData p_CriteriaProgress)
{
    *p_Data << int32(p_CriteriaProgress.m_ID);
    *p_Data << uint64(p_CriteriaProgress.m_Quantity);
    p_Data->appendPackGUID(p_CriteriaProgress.m_Guid);
    *p_Data << uint32(MS::Utilities::WowTime::Encode(p_CriteriaProgress.m_Date));
    *p_Data << int32(p_CriteriaProgress.m_TimeFromStart);
    *p_Data << int32(p_CriteriaProgress.m_TimeFromCreate);

    p_Data->WriteBits(p_CriteriaProgress.m_Flags, 4);
    p_Data->FlushBits();
}

void InstanceScript::UpdateCriteriasAfterLoading()
{
    for (uint8 l_I = 0; l_I < m_Bosses.size(); ++l_I)
    {
        BossInfo* bossInfo = &m_Bosses[l_I];
        BossScenarios* l_BossScenario = &m_BossesScenarios[l_I];

        if (bossInfo->state == DONE)
            SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, uint32(time(nullptr)), m_BeginningTime, 0));
    }
}

//////////////////////////////////////////////////////////////////////////
/// ChallengesMode
void InstanceScript::SendChallengeModeStart()
{
    WorldPacket l_Data(SMSG_CHALLENGE_MODE_START, 4);
    l_Data << uint32(instance->GetId());
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendChallengeStartTimer(uint32 p_Time)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->SendStartTimer(p_Time, p_Time, CHALLENGE_TIMER);
    }
}

void InstanceScript::SendChallengeStartElapsedTimer(uint32 p_TimerID, uint32 p_Time)
{
    WorldPacket l_Data(SMSG_START_ELAPSED_TIMER, 8);
    l_Data << uint32(p_TimerID);
    l_Data << uint32(p_Time);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendChallengeStopElapsedTimer(uint32 p_TimerID, bool p_KeepTimer /*= false*/)
{
    WorldPacket l_Data(SMSG_STOP_ELAPSED_TIMER, 5);
    l_Data << uint32(p_TimerID);
    l_Data.WriteBit(p_KeepTimer);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SetChallengeModeStarted()
{
    if (m_ChallengeStarted || !instance->IsChallengeMode())
        return;

    m_ChallengeStarted = true;

    ///< Challenge mode start timer is 5 seconds
    SendChallengeStartTimer(5);
    m_StartChallengeTime = 5000;
}

void InstanceScript::ScheduleChallengeStartup(uint32 p_Diff)
{
    if (m_StartChallengeTime)
    {
        if (m_StartChallengeTime <= p_Diff)
        {
            m_StartChallengeTime = 0;

            if (GameObject* l_ChallengeDoor = sObjectAccessor->FindGameObject(m_ChallengeDoorGuid))
                l_ChallengeDoor->SetGoState(GO_STATE_ACTIVE);

            SendChallengeStartElapsedTimer(1, 0);
            SendChallengeModeStart();

            m_MedalType = eChallengeMedals::MedalTypeGold;
        }
        else
            m_StartChallengeTime -= p_Diff;
    }
}

void InstanceScript::ScheduleChallengeTimeUpdate(uint32 p_Diff)
{
    if (!m_ChallengeStarted || m_StartChallengeTime)
        return;

    m_ChallengeTime += p_Diff;

    MapChallengeModeEntry const* l_ChallengeEntry = nullptr;
    for (uint32 l_I = 0; l_I < sMapChallengeModeStore.GetNumRows(); ++l_I)
    {
        if (MapChallengeModeEntry const* l_Challenge = sMapChallengeModeStore.LookupEntry(l_I))
        {
            if (l_Challenge->MapID == instance->GetId())
            {
                l_ChallengeEntry = l_Challenge;
                break;
            }
        }
    }

    /// Should not happens
    if (l_ChallengeEntry == nullptr)
        return;

    uint32 l_Times[eChallengeMedals::MedalTypeGold];
    l_Times[eChallengeMedals::MedalTypeBronze - 1]  = l_ChallengeEntry->BronzeTime * TimeConstants::IN_MILLISECONDS;
    l_Times[eChallengeMedals::MedalTypeSilver - 1]  = l_ChallengeEntry->SilverTime * TimeConstants::IN_MILLISECONDS;
    l_Times[eChallengeMedals::MedalTypeGold - 1]    = l_ChallengeEntry->GoldTime * TimeConstants::IN_MILLISECONDS;

    /// Downgrade Medal if needed
    switch (m_MedalType)
    {
        case eChallengeMedals::MedalTypeGold:
            if (m_ChallengeTime > l_Times[eChallengeMedals::MedalTypeGold - 1])
                --m_MedalType;
            break;
        case eChallengeMedals::MedalTypeSilver:
            if (m_ChallengeTime > l_Times[eChallengeMedals::MedalTypeSilver - 1])
                --m_MedalType;
            break;
        case eChallengeMedals::MedalTypeBronze:
            if (m_ChallengeTime > l_Times[eChallengeMedals::MedalTypeBronze - 1])
                --m_MedalType;
            break;
        default:
            break;
    }
}

void InstanceScript::ScheduleBeginningTimeUpdate(uint32 p_Diff)
{
    m_BeginningTime += p_Diff;
}

void InstanceScript::SendChallengeNewPlayerRecord()
{
    uint32 l_MapID = instance->GetId();

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (l_Player->HasChallengeCompleted(l_MapID))
            {
                CompletedChallenge* l_Challenge = l_Player->GetCompletedChallenge(l_MapID);
                /// Should not happens
                if (l_Challenge == nullptr)
                    continue;

                bool l_NewBestTime = m_ChallengeTime < l_Challenge->m_BestTime;
                bool l_NewBestMedal = m_MedalType > l_Challenge->m_BestMedal;

                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_COMPLETED_CHALLENGE);
                l_Statement->setUInt32(0, l_NewBestTime ? m_ChallengeTime : l_Challenge->m_BestTime);
                l_Statement->setUInt32(1, m_ChallengeTime);
                l_Statement->setUInt8(2, l_NewBestMedal ? m_MedalType : l_Challenge->m_BestMedal);
                l_Statement->setUInt32(3, l_NewBestMedal ? uint32(time(nullptr)) : l_Challenge->m_BestMedalDate);
                l_Statement->setUInt32(4, l_Player->GetGUIDLow());
                l_Statement->setUInt32(5, l_MapID);
                CharacterDatabase.Execute(l_Statement);

                if (l_NewBestMedal)
                {
                    l_Challenge->m_BestMedal = m_MedalType;
                    l_Challenge->m_BestMedalDate = uint32(time(nullptr));
                }

                if (l_NewBestTime)
                    l_Challenge->m_BestTime = m_ChallengeTime;

                l_Challenge->m_LastTime = m_ChallengeTime;

                /// Send new record only for new best time
                if (!l_NewBestTime)
                    continue;
            }
            else
            {
                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_COMPLETED_CHALLENGE);
                l_Statement->setUInt32(0, l_Player->GetGUIDLow());
                l_Statement->setUInt32(1, l_MapID);
                l_Statement->setUInt32(2, m_ChallengeTime);
                l_Statement->setUInt32(3, m_ChallengeTime);
                l_Statement->setUInt8(4, m_MedalType);
                l_Statement->setUInt32(5, uint32(time(nullptr)));
                CharacterDatabase.Execute(l_Statement);

                CompletedChallenge l_Challenge;
                l_Challenge.m_BestMedal = m_MedalType;
                l_Challenge.m_BestMedalDate = uint32(time(nullptr));
                l_Challenge.m_BestTime = m_ChallengeTime;
                l_Challenge.m_LastTime = m_ChallengeTime;

                l_Player->AddCompletedChallenge(l_MapID, l_Challenge);
                l_Player->GetSession()->SendChallengeModeMapStatsUpdate(l_MapID);
            }

            WorldPacket l_Data(SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD, 12);
            l_Data << uint32(l_MapID);
            l_Data << uint32(m_ChallengeTime);
            l_Data << uint32(m_MedalType);
            l_Player->SendDirectMessage(&l_Data);
        }
    }
}

void InstanceScript::SendChallengeModeComplete(uint32 p_Money)
{
    uint32 l_ItemRewards = 0;
    uint32 l_CurrencyRewards = 0;
    WorldPacket l_Data(SMSG_CHALLENGE_MODE_COMPLETE, 4 * 4 + (l_ItemRewards * 22) + (l_CurrencyRewards * 8));

    ///< Reward
    {
        l_Data << uint32(l_ItemRewards);
        l_Data << uint32(l_CurrencyRewards);
        l_Data << uint32(p_Money);

        ///< ItemReward
        for (uint32 l_I = 0; l_I < l_ItemRewards; ++l_I)
        {
            Item::BuildDynamicItemDatas(l_Data, nullptr);

            l_Data << uint32(0);    ///< Quantity
        }

        ///< CurrencyReward
        for (uint32 l_I = 0; l_I < l_CurrencyRewards; ++l_I)
        {
            l_Data << uint32(0);    ///< CurrencyID
            l_Data << uint32(0);    ///< Quantity
        }
    }

    l_Data << uint32(m_ChallengeTime);
    l_Data << uint32(instance->GetId());
    l_Data << uint32(m_MedalType);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SaveChallengeDatasIfNeeded()
{
    uint32 l_MapID = instance->GetId();
    RealmCompletedChallenge* l_GroupChallenge = sObjectMgr->GetGroupCompletedChallengeForMap(l_MapID);
    RealmCompletedChallenge* l_GuildChallenge = sObjectMgr->GetGuildCompletedChallengeForMap(l_MapID);

    SaveNewGroupChallenge();

    /// Delete old group record if it's a new realm-best time (or if it's the first), and reward titles/achievements
    if (l_GuildChallenge == nullptr || (l_GroupChallenge != nullptr && l_GroupChallenge->m_CompletionTime > m_ChallengeTime))
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_CHALLENGE);
        l_Statement->setUInt32(0, l_MapID);
        CharacterDatabase.Execute(l_Statement);

        RewardNewRealmRecord(l_GroupChallenge);
    }

    bool l_GuildGroup = false;
    uint32 l_GuildID = 0;
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (Group* l_Group = l_Player->GetGroup())
            {
                if (!l_Group->IsGuildGroup(l_Player->GetGuildId(), true, true))
                    continue;

                l_GuildGroup = true;
                l_GuildID = l_Player->GetGuildId();
            }
        }
    }

    /// New best time for the guild
    if (l_GuildGroup)
    {
        SaveNewGroupChallenge(l_GuildID);

        /// Delete old guild record if it's a new realm-best time
        if (l_GuildChallenge != nullptr && l_GuildChallenge->m_CompletionTime > m_ChallengeTime)
        {
            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_CHALLENGE);
            l_Statement->setUInt32(0, l_MapID);
            l_Statement->setUInt32(1, l_GuildID);
            CharacterDatabase.Execute(l_Statement);
        }
    }
}

void InstanceScript::SaveNewGroupChallenge(uint32 p_GuildID /*= 0*/)
{
    uint32 l_Index = 0;
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(p_GuildID ? CHAR_INS_GUILD_CHALLENGE : CHAR_INS_GROUP_CHALLENGE);
    l_Statement->setUInt32(l_Index++, instance->GetId());

    if (p_GuildID)
        l_Statement->setUInt32(l_Index++, p_GuildID);

    l_Statement->setUInt32(l_Index++, 0);   ///< AttemptID
    l_Statement->setUInt32(l_Index++, m_ChallengeTime);
    l_Statement->setUInt32(l_Index++, uint32(time(nullptr)));
    l_Statement->setUInt8(l_Index++, m_MedalType);

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    uint32 l_Count = 0;
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            ++l_Count;
    }

    l_Statement->setUInt8(l_Index++, l_Count);

    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            l_Statement->setUInt32(l_Index++, l_Player->GetGUIDLow());
            l_Statement->setUInt32(l_Index++, l_Player->GetSpecializationId(l_Player->GetActiveSpec()));
        }
    }

    if (l_Count < 5)
    {
        for (uint8 l_I = 0; l_I < (5 - l_Count); ++l_I)
        {
            l_Statement->setUInt32(l_Index++, 0);
            l_Statement->setUInt32(l_Index++, 0);
        }
    }

    CharacterDatabase.Execute(l_Statement);
}

uint32 InstanceScript::RewardChallengers()
{
    ChallengeReward* l_Reward = sObjectMgr->GetChallengeRewardsForMap(instance->GetId());
    if (l_Reward == nullptr)
        return 0;

    if (m_MedalType < eChallengeMedals::MaxMedalType)
    {
        uint32 l_Money = l_Reward->MoneyReward[m_MedalType];

        Map::PlayerList const& l_PlayerList = instance->GetPlayers();
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
                l_Player->ModifyMoney(int64(l_Money));
        }

        return l_Money;
    }

    return 0;
}

void InstanceScript::RewardNewRealmRecord(RealmCompletedChallenge* p_OldChallenge /*= nullptr*/)
{
    ChallengeReward* l_Reward = sObjectMgr->GetChallengeRewardsForMap(instance->GetId());
    if (l_Reward == nullptr)
        return;

    CharTitlesEntry const* l_Title = sCharTitlesStore.LookupEntry(l_Reward->TitleID);
    if (l_Title == nullptr)
        return;

    AchievementEntry const* l_Achievement = sAchievementStore.LookupEntry(l_Reward->AchievementID);
    if (l_Achievement == nullptr)
        return;

    /// Remove title to previous challengers - Achievement will stay
    if (p_OldChallenge != nullptr)
    {
        for (uint8 l_I = 0; l_I < 5; ++l_I)
        {
            /// Check if player is online, then remove the title
            if (Player* l_Player = HashMapHolder<Player>::Find(p_OldChallenge->m_Members[l_I].m_Guid))
                l_Player->SetTitle(l_Title, true);
            else
            {
                uint32 l_Index = l_Title->MaskID / 32;
                uint32 l_Flag = 1 << (l_Title->MaskID % 32);
                uint32 l_LowGuid = GUID_LOPART(p_OldChallenge->m_Members[l_I].m_Guid);

                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_TITLES);
                l_Statement->setUInt32(0, l_LowGuid);

                PreparedQueryResult l_Result = CharacterDatabase.AsyncQuery(l_Statement, [l_Index, l_Flag, l_LowGuid](PreparedQueryResult const& p_Result) -> void
                {
                    if (!p_Result)
                        return;

                    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

                    Field* l_Fields = p_Result->Fetch();
                    char const* l_KnownTitlesStr = l_Fields[0].GetCString();

                    /// Title removal
                    if (l_KnownTitlesStr)
                    {
                        uint32 const l_TitleSize = KNOWN_TITLES_SIZE;
                        uint32 l_KnownTitles[l_TitleSize];
                        Tokenizer l_Tokens(l_KnownTitlesStr, ' ', l_TitleSize);

                        uint32 l_ActualSize = l_Tokens.size();

                        for (uint32 l_J = 0; l_J < l_TitleSize; ++l_J)
                        {
                            if (l_J < l_ActualSize)
                                l_KnownTitles[l_J] = atol(l_Tokens[l_J]);
                            else
                                l_KnownTitles[l_J] = 0;
                        }

                        l_KnownTitles[l_Index] &= ~l_Flag;

                        std::ostringstream l_Stream;
                        for (uint32 l_J = 0; l_J < l_TitleSize; ++l_J)
                            l_Stream << l_KnownTitles[l_J] << ' ';

                        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TITLES_FACTION_CHANGE);
                        l_Statement->setString(0, l_Stream.str().c_str());
                        l_Statement->setUInt32(1, l_LowGuid);
                        l_Transaction->Append(l_Statement);

                        /// Unset any currently chosen title
                        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_RES_CHAR_TITLES_FACTION_CHANGE);
                        l_Statement->setUInt32(0, l_LowGuid);
                        l_Transaction->Append(l_Statement);
                    }

                    CharacterDatabase.CommitTransaction(l_Transaction);
                });
            }
        }
    }

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            l_Player->SetTitle(l_Title);
            l_Player->CompletedAchievement(l_Achievement);
        }
    }
}

void InstanceScript::ResetChallengeMode()
{
    /// Reset internal datas
    m_ChallengeStarted      = false;
    m_ConditionCompleted    = false;
    m_CreatureKilled        = 0;
    m_StartChallengeTime    = 0;
    m_ChallengeTime         = 0;
    m_MedalType             = eChallengeMedals::MedalTypeNone;
    m_BeginningTime         = 0;
    m_ScenarioStep          = 0;

    /// Reset challenge door
    if (GameObject* l_ChallengeDoor = instance->GetGameObject(m_ChallengeDoorGuid))
        l_ChallengeDoor->SetGoState(GOState::GO_STATE_READY);

    /// Reset challenge orb
    if (GameObject* l_ChallengeOrb = instance->GetGameObject(m_ChallengeOrbGuid))
    {
        l_ChallengeOrb->SetGoState(GOState::GO_STATE_READY);
        l_ChallengeOrb->RemoveFlag(EGameObjectFields::GAMEOBJECT_FIELD_FLAGS, GameObjectFlags::GO_FLAG_NODESPAWN);
    }

    /// Reset challenge timer
    SendChallengeStopElapsedTimer(1);

    /// Reset scenario datas
    SendScenarioState(ScenarioData(m_ScenarioID, m_ScenarioStep));

    /// Reset all cooldowns of 3min or more
    DoRemoveSpellCooldownWithTimeOnPlayers(TimeConstants::MINUTE * 3 * TimeConstants::IN_MILLISECONDS);

    /// Teleport players to entrance
    RepopPlayersAtGraveyard();

    /// Reset all creatures
    RespawnCreature();
}
//////////////////////////////////////////////////////////////////////////

bool InstanceScript::IsWipe()
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();

    if (PlayerList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
    {
        Player* player = Itr->getSource();

        if (!player)
            continue;

        if (player->isAlive() && !player->isGameMaster())
            return false;
    }

    return true;
}

void InstanceScript::UpdateEncounterState(EncounterCreditType p_Type, uint32 p_CreditEntry, Unit* p_Source)
{
    DungeonEncounterList const* l_Encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), instance->GetDifficultyID());
    if (!l_Encounters || l_Encounters->empty())
        return;

    int32 l_MaxIndex = -100000;
    for (DungeonEncounterList::const_iterator l_Iter = l_Encounters->begin(); l_Iter != l_Encounters->end(); ++l_Iter)
    {
        if ((*l_Iter)->dbcEntry->OrderIndex > l_MaxIndex)
            l_MaxIndex = (*l_Iter)->dbcEntry->OrderIndex;
    }

    for (DungeonEncounterList::const_iterator l_Iter = l_Encounters->begin(); l_Iter != l_Encounters->end(); ++l_Iter)
    {
        if ((p_Source != nullptr && ((*l_Iter)->dbcEntry->CreatureDisplayID == p_Source->GetNativeDisplayId())) || ((*l_Iter)->creditType == p_Type && (*l_Iter)->creditEntry == p_CreditEntry))
        {
            m_CompletedEncounters |= 1 << (*l_Iter)->dbcEntry->Bit;

            if ((*l_Iter)->dbcEntry->OrderIndex == l_MaxIndex)
            {
                Map::PlayerList const& l_PlayerList = instance->GetPlayers();
                if (l_PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                    {
                        uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                        if (l_Player->IsAtGroupRewardDistance(p_Source))
                            sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                    }
                }
            }

            SendEncounterEnd((*l_Iter)->dbcEntry->ID, true);

            if (p_Source != nullptr && p_Source->GetTypeId() == TypeID::TYPEID_UNIT)
                SaveEncounterLogs(p_Source->ToCreature(), (*l_Iter)->dbcEntry->ID);

            if (p_Source != nullptr)
                SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_END, p_Source);

            WorldPacket l_Data(Opcodes::SMSG_BOSS_KILL_CREDIT, 4);
            l_Data << int32((*l_Iter)->dbcEntry->ID);
            instance->SendToPlayers(&l_Data);

            if (!sObjectMgr->IsDisabledEncounter((*l_Iter)->dbcEntry->ID, instance->GetDifficultyID()))
                DoUpdateAchievementCriteria(AchievementCriteriaTypes::ACHIEVEMENT_CRITERIA_TYPE_DEFEAT_ENCOUNTER, (*l_Iter)->dbcEntry->ID);

            return;
        }
    }
}

void InstanceScript::SendEncounterStart(uint32 p_EncounterID)
{
    if (!p_EncounterID)
        return;

    WorldPacket l_Data(Opcodes::SMSG_ENCOUNTER_START);
    l_Data << uint32(p_EncounterID);
    l_Data << uint32(instance->GetDifficultyID());
    l_Data << uint32(instance->GetPlayers().getSize());
    instance->SendToPlayers(&l_Data);

    if (sObjectMgr->IsDisabledEncounter(p_EncounterID, instance->GetDifficultyID()))
        return;

    /// Reset datas before each attempt
    m_EncounterDatas = EncounterDatas();

    m_EncounterDatas.Expansion = instance->GetEntry()->ExpansionID;

    /// Register encounter datas for further logs
    if (instance->IsRaid() && m_EncounterDatas.Expansion == Expansion::EXPANSION_WARLORDS_OF_DRAENOR)
    {
        m_EncounterDatas.RealmID        = g_RealmID;
        m_EncounterDatas.EncounterID    = p_EncounterID;

        Map::PlayerList const& l_PlayerList = instance->GetPlayers();
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
            {
                if (Group* l_Group = l_Player->GetGroup())
                {
                    if (l_Player->GetGuild() == nullptr || !l_Group->IsGuildGroup(l_Player->GetGuildId(), true, true))
                        continue;

                    m_EncounterDatas.GuildID        = l_Player->GetGuildId();
                    m_EncounterDatas.GuildFaction   = l_Player->GetTeamId();
                    m_EncounterDatas.GuildName      = l_Player->GetGuildName();
                    break;
                }
            }
        }

        m_EncounterDatas.MapID          = instance->GetId();
        m_EncounterDatas.DifficultyID   = instance->GetDifficultyID();
        m_EncounterDatas.StartTime      = uint32(time(nullptr));
    }
}

void InstanceScript::SendEncounterEnd(uint32 p_EncounterID, bool p_Success)
{
    if (!p_EncounterID)
        return;

    WorldPacket l_Data(Opcodes::SMSG_ENCOUNTER_END);
    l_Data << uint32(p_EncounterID);
    l_Data << uint32(instance->GetDifficultyID());
    l_Data << uint32(instance->GetPlayers().getSize());
    l_Data.WriteBit(p_Success);
    l_Data.FlushBits();
    instance->SendToPlayers(&l_Data);

    if (sObjectMgr->IsDisabledEncounter(p_EncounterID, instance->GetDifficultyID()))
        return;

    m_EncounterDatas.CombatDuration = uint32(time(nullptr)) - m_EncounterDatas.StartTime;
    m_EncounterDatas.EndTime        = uint32(time(nullptr));
    m_EncounterDatas.Success        = p_Success;

    if (m_EncounterDatas.GuildID || instance->IsLFR())
    {
        Map::PlayerList const& l_PlayerList = instance->GetPlayers();
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
            {
                RosterData l_Data;

                l_Data.GuidLow      = l_Player->GetGUIDLow();
                l_Data.Name         = l_Player->GetName();
                l_Data.Level        = l_Player->getLevel();
                l_Data.Class        = l_Player->getClass();
                l_Data.SpecID       = l_Player->GetSpecializationId();
                l_Data.Role         = l_Player->GetRoleForGroup();
                l_Data.ItemLevel    = l_Player->GetAverageItemLevelEquipped();

                m_EncounterDatas.RosterDatas.push_back(l_Data);
            }
        }

        sScriptMgr->OnEncounterEnd(&m_EncounterDatas);
    }

    /// Reset datas after each attempt
    m_EncounterDatas = EncounterDatas();
}

void InstanceScript::SaveEncounterLogs(Creature* p_Creature, uint32 p_EncounterID)
{
    if ((p_Creature->GetNativeTemplate()->flags_extra & CREATURE_FLAG_EXTRA_LOG_GROUP_DMG) == 0)
        return;

    CreatureDamageLogList const& l_DamageLogs  = p_Creature->GetDamageLogs();
    GroupDumpList const& l_GroupDumps          = p_Creature->GetGroupDumps();

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    for (auto l_Log : l_DamageLogs)
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_ENCOUNTER_DAMAGE_LOG);
        l_Statement->setUInt32(0, p_EncounterID);
        l_Statement->setUInt64(1, m_EncounterDatas.StartTime);
        l_Statement->setUInt64(2, l_Log.Time);
        l_Statement->setUInt32(3, l_Log.AttackerGuid);
        l_Statement->setUInt32(4, l_Log.Damage);
        l_Statement->setUInt32(5, l_Log.Spell);
        l_Transaction->Append(l_Statement);
    }

    for (auto l_Dump : l_GroupDumps)
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_ENCOUNTER_GROUP_DUMP);
        l_Statement->setUInt32(0, p_EncounterID);
        l_Statement->setUInt64(1, m_EncounterDatas.StartTime);
        l_Statement->setUInt64(2, l_Dump.Time);
        l_Statement->setString(3, l_Dump.Dump);
        l_Transaction->Append(l_Statement);
    }

    CharacterDatabase.CommitTransaction(l_Transaction);

    p_Creature->ClearDamageLog();
    p_Creature->ClearGroupDumps();
}

uint32 InstanceScript::GetEncounterIDForBoss(Creature* p_Boss) const
{
    DungeonEncounterList const* l_Encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), instance->GetDifficultyID());
    if (!l_Encounters || l_Encounters->empty() || p_Boss == nullptr)
        return 0;

    for (DungeonEncounterList::const_iterator l_Iter = l_Encounters->begin(); l_Iter != l_Encounters->end(); ++l_Iter)
    {
        if (((*l_Iter)->dbcEntry->CreatureDisplayID == p_Boss->GetNativeDisplayId()) || ((*l_Iter)->creditEntry == p_Boss->GetEntry()))
            return (*l_Iter)->dbcEntry->ID;
    }

    return 0;
}

void InstanceScript::UpdatePhasing()
{
    PhaseUpdateData phaseUdateData;
    phaseUdateData.AddConditionType(CONDITION_INSTANCE_DATA);

    Map::PlayerList const& players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->getSource())
            player->GetPhaseMgr().NotifyConditionChanged(phaseUdateData);
}

void InstanceScript::UpdateCreatureGroupSizeStats()
{
    auto l_WorldObjects = instance->GetAllWorldObjectOnMap();
    for (auto l_WorldObject = l_WorldObjects->begin(); l_WorldObject != l_WorldObjects->end(); l_WorldObject++)
    {
        if ((*l_WorldObject)->GetTypeId() != TYPEID_UNIT)
            continue;

        Creature* l_Creature = (*l_WorldObject)->ToCreature();
        l_Creature->UpdateGroupSizeStats();
    }
}

//////////////////////////////////////////////////////////////////////////
/// Combat Resurrection - http://wow.gamepedia.com/Resurrect#Combat_resurrection
void InstanceScript::ResetCombatResurrection()
{
    if (!instance->IsRaid())
        return;

    m_InCombatResCount = 0;
    m_MaxInCombatResCount = 0;
    m_CombatResChargeTime = 0;
    m_NextCombatResChargeTime = 0;
}

void InstanceScript::StartCombatResurrection()
{
    if (!instance->IsRaid())
        return;

    /// Upon engaging a boss, all combat resurrection spells will have their cooldowns reset and begin with 1 charge.
    for (uint8 l_I = 0; l_I < eInstanceSpells::MaxBattleResSpells; ++l_I)
        DoRemoveSpellCooldownOnPlayers(g_BattleResSpells[l_I]);

    m_InCombatResCount = 1;

    /// Charges will accumulate at a rate of 1 per (90/RaidSize) minutes.
    /// Example 1: A 10-player raid will accumulate 1 charge every 9 minutes (90/10 = 9).
    /// Example 2: A 20-player raid will accumulate 1 charge every 4.5 minutes (90/20 = 4.5).
    uint32 l_PlayerCount = instance->GetPlayers().getSize();
    if (!l_PlayerCount)
        return;

    float l_Value = 9000.0f / (float)l_PlayerCount;
    uint32 l_Timer = uint32(l_Value / 100.0f);

    l_Value -= (float)l_Timer * 100.0f;
    l_Timer *= TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS;
    l_Value *= TimeConstants::MINUTE / 100.0f * TimeConstants::IN_MILLISECONDS;
    l_Timer += uint32(l_Value);

    m_MaxInCombatResCount = 9;
    m_CombatResChargeTime = l_Timer;
    m_NextCombatResChargeTime = l_Timer;
}

void InstanceScript::UpdateCombatResurrection(uint32 const p_Diff)
{
    if (!m_NextCombatResChargeTime)
        return;

    /// Add a charge
    if (m_NextCombatResChargeTime <= p_Diff)
    {
        m_NextCombatResChargeTime = m_CombatResChargeTime;
        m_InCombatResCount = std::min(++m_InCombatResCount, m_MaxInCombatResCount);

        SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_GAIN_COMBAT_RESURRECTION_CHARGE);
    }
    else
        m_NextCombatResChargeTime -= p_Diff;
}

bool InstanceScript::CanUseCombatResurrection() const
{
    if (!instance->IsRaid())
        return true;

    if (!IsEncounterInProgress())
        return true;

    if (m_InCombatResCount <= 0)
        return false;

    return true;
}

void InstanceScript::ConsumeCombatResurrectionCharge()
{
    /// Shouldn't happen
    if (m_InCombatResCount <= 0)
        return;

    --m_InCombatResCount;
    SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_IN_COMBAT_RESURRECTION);
}
//////////////////////////////////////////////////////////////////////////

class EncounterScript_Global : public EncounterScript
{
    public:
        EncounterScript_Global() : EncounterScript("EncounterScript_PvE_Logs") { }

        void OnEncounterEnd(EncounterDatas const* p_EncounterDatas) override
        {
            EasyJSon::Node<std::string> l_Node;

            l_Node["Expansion"]        = p_EncounterDatas->Expansion;
            l_Node["RealmID"]          = p_EncounterDatas->RealmID;
            l_Node["GuildID"]          = p_EncounterDatas->GuildID;
            l_Node["GuildFaction"]     = p_EncounterDatas->GuildFaction;
            l_Node["GuildName"]        = p_EncounterDatas->GuildName;
            l_Node["MapID"]            = p_EncounterDatas->MapID;
            l_Node["EncounterID"]      = p_EncounterDatas->EncounterID;
            l_Node["DifficultyID"]     = p_EncounterDatas->DifficultyID;
            l_Node["StartTime"]        = p_EncounterDatas->StartTime;
            l_Node["CombatDuration"]   = p_EncounterDatas->CombatDuration;
            l_Node["EndTime"]          = p_EncounterDatas->EndTime;
            l_Node["Success"]          = p_EncounterDatas->Success;

            for (std::size_t l_I = 0; l_I < p_EncounterDatas->RosterDatas.size(); ++l_I)
            {
                RosterData const& l_Data = p_EncounterDatas->RosterDatas[l_I];

                l_Node["RosterDatas"][l_I]["Name"]      = l_Data.Name;
                l_Node["RosterDatas"][l_I]["GuidLow"]   = l_Data.GuidLow;
                l_Node["RosterDatas"][l_I]["Level"]     = l_Data.Level;
                l_Node["RosterDatas"][l_I]["Class"]     = l_Data.Class;
                l_Node["RosterDatas"][l_I]["SpecID"]    = l_Data.SpecID;
                l_Node["RosterDatas"][l_I]["Role"]      = l_Data.Role;
                l_Node["RosterDatas"][l_I]["ItemLevel"] = l_Data.ItemLevel;
            }

            l_Node["EncounterHealth"]  = p_EncounterDatas->EncounterHealth;
            l_Node["DeadCount"]        = p_EncounterDatas->DeadCount;

            sReporter->EnqueueReport(l_Node.Serialize<std::ostringstream>(true));
        }
};

#ifndef __clang_analyzer__
void AddSC_EncounterScripts()
{
    new EncounterScript_Global();
}
#endif
