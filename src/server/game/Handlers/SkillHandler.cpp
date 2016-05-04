////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectAccessor.h"
#include "UpdateMask.h"

void WorldSession::HandleSetSpecialization(WorldPacket& p_Packet)
{
    uint32  l_TabIndex  = p_Packet.read<uint32>();
    uint8   l_ClassID   = m_Player->getClass();

    // Avoid cheat - hack
    if (m_Player->GetSpecializationId(m_Player->GetActiveSpec()))
        return;

    uint32 l_SpecializationID       = 0;
    uint32 l_SpecializationSpell    = 0;

    for (uint32 l_I = 0; l_I < sChrSpecializationsStore.GetNumRows(); l_I++)
    {
        ChrSpecializationsEntry const * l_Specialization = sChrSpecializationsStore.LookupEntry(l_I);

        if (!l_Specialization)
            continue;

        if (l_Specialization->ClassID == l_ClassID && l_Specialization->OrderIndex == l_TabIndex)
        {
            l_SpecializationID      = l_Specialization->ID;
            l_SpecializationSpell   = l_Specialization->MasterySpellID;

            break;
        }
    }

    if (l_SpecializationID)
    {
        m_Player->SetSpecializationId(m_Player->GetActiveSpec(), l_SpecializationID);
        m_Player->SendTalentsInfoData(false);

        for (uint8 l_I = POWER_MANA; l_I < MAX_POWERS; ++l_I)
        {
            m_Player->SetMaxPower(Powers(l_I), m_Player->GetCreatePowers(Powers(l_I)));
            m_Player->SetPower(Powers(l_I), 0);
        }

        if (l_SpecializationSpell)
            m_Player->learnSpell(l_SpecializationSpell, false);

        m_Player->InitSpellForLevel();
        m_Player->UpdateMasteryPercentage();
    }
}

void WorldSession::HandleLearnTalents(WorldPacket& p_RecvPacket)
{
    uint32 l_TalentCount = 0; 
    
    p_RecvPacket >> l_TalentCount;

    if (l_TalentCount == 0 || l_TalentCount > MAX_TALENT_SPELLS)
        return;

    if (l_TalentCount > m_Player->GetFreeTalentPoints())
        return;

    for (uint32 l_I = 0; l_I < l_TalentCount; l_I++)
    {
        uint16 l_TalentID = p_RecvPacket.read<uint16>();
        m_Player->LearnTalent(l_TalentID);
    }

    m_Player->SaveToDB();
    m_Player->SendTalentsInfoData(false);
}

void WorldSession::HandleTalentWipeConfirmOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    /// 0 - Talents, 1 - Specialization
    uint8 l_RespecType = p_RecvData.read<uint8>();

    Creature* l_Trainer = m_Player->GetNPCIfCanInteractWith(l_Guid, NPCFlags::UNIT_NPC_FLAG_TRAINER);
    if (l_Trainer == nullptr)
        return;

    if (!l_Trainer->isCanTrainingAndResetTalentsOf(m_Player))
        return;

    /// Remove fake death
    if (m_Player->HasUnitState(UnitState::UNIT_STATE_DIED))
        m_Player->RemoveAurasByType(AuraType::SPELL_AURA_FEIGN_DEATH);

    /// Talents
    if (!l_RespecType)
    {
        if (!m_Player->ResetTalents())
        {
            /// You don't have any talents
            m_Player->SendTalentWipeConfirm(0, false);
            return;
        }
    }
    /// Specialization
    else
    {
        if (m_Player->GetSpecializationId(m_Player->GetActiveSpec()) != SpecIndex::SPEC_NONE)
            m_Player->ResetSpec();
        else
        {
            /// You don't have any class specialization
            m_Player->SendTalentWipeConfirm(0, true);
            return;
        }
    }

    m_Player->SendTalentsInfoData(false);

    /// Spell: "Untalent Visual Effect"
    l_Trainer->CastSpell(m_Player, 14867, true);
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recvData)
{
    uint32 skillId;
    recvData >> skillId;

    if (!IsProfessionSkill(skillId))
        return;

    GetPlayer()->SetSkill(skillId, 0, 0, 0);
}

void WorldSession::HandleArcheologyRequestHistory(WorldPacket& /*p_RecvData*/) ///< p_RecvData is unused
{
    WorldPacket l_Data(SMSG_SETUP_RESEARCH_HISTORY, 2048);

    MS::Skill::Archaeology::CompletedProjectMap& l_Projects = GetPlayer()->GetArchaeologyMgr().GetCompletedProjects();
    uint32 l_Count = l_Projects.size();

    l_Data << uint32(l_Count);

    if (l_Count > 0)
    {
        for (MS::Skill::Archaeology::CompletedProjectMap::iterator l_Iter = l_Projects.begin(); l_Iter != l_Projects.end(); ++l_Iter)
        {
            if (ResearchProjectEntry const* l_Project = sResearchProjectStore.LookupEntry((*l_Iter).first)) ///< l_Project is unused
            {
                l_Data << uint32((*l_Iter).first);
                l_Data << uint32((*l_Iter).second.FirstCompletedDate);
                l_Data << uint32((*l_Iter).second.CompletionCount);
            }
            else
                l_Data << uint32(0) << uint32(0) << uint32(0);
        }
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleShowTradeSkillOpcode(WorldPacket& p_RecvPacket)
{
    uint32 l_SpellID = 0;
    uint32 l_SkillID = 0;
    uint64 l_PlayerGuid = 0;

    p_RecvPacket.readPackGUID(l_PlayerGuid);
    p_RecvPacket >> l_SpellID;
    p_RecvPacket >> l_SkillID;

    Player* l_Player = ObjectAccessor::FindPlayer(l_PlayerGuid);

    if (!l_Player)
        return;

    if (!l_Player->HasSkill(l_SkillID))
        return;

    SkillLineEntry const* l_SkillEntry = sSkillLineStore.LookupEntry(l_SkillID);
    if (!l_SkillEntry || l_SkillEntry->categoryId != SKILL_CATEGORY_PROFESSION)
        return;

    std::vector<uint32> l_Recipes;

    for (uint32 l_J = 0; l_J < sSkillLineAbilityStore.GetNumRows(); ++l_J)
    {
        if (SkillLineAbilityEntry const* l_Ability = sSkillLineAbilityStore.LookupEntry(l_J))
        {
            if (l_Ability->skillId == l_SkillID && l_Player->HasSpell(l_Ability->spellId))
                l_Recipes.push_back(l_Ability->spellId);
        }
    }

    WorldPacket l_Data(SMSG_SHOW_TRADE_SKILL_RESPONSE, 20 * 1024);
    l_Data.appendPackGUID(l_PlayerGuid);        ///< PlayerGUID
    l_Data << uint32(l_SpellID);                ///< SpellID
    l_Data << uint32(1);                        ///< SkillLineIDs count
    l_Data << uint32(1);                        ///< SkillRanks count
    l_Data << uint32(1);                        ///< SkillMaxRanks count
    l_Data << uint32(l_Recipes.size());         ///< KnownAbilitySpellIDs count

    l_Data << uint32(l_SkillID);                                ///< SkillLineIDs
    l_Data << uint32(l_Player->GetSkillValue(l_SkillID));       ///< SkillRanks
    l_Data << uint32(l_Player->GetMaxSkillValue(l_SkillID));    ///< SkillMaxRanks

    for (size_t l_I = 0; l_I < l_Recipes.size(); ++l_I)         ///< KnownAbilitySpellIDs
        l_Data << uint32(l_Recipes[l_I]);

    SendPacket(&l_Data);
}