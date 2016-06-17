////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

enum eIronMaidensActions
{
    ActionIntro,
    ActionAfterTrashesIntro
};

static void RespawnMaidens(InstanceScript* p_Instance, Creature* p_Source)
{
    if (p_Instance == nullptr || p_Source == nullptr)
        return;

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = Creature::GetCreature(*p_Source, p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            l_Maiden->Respawn();
            l_Maiden->GetMotionMaster()->MoveTargetedHome();
        }
    }
}

static bool StartMaidens(InstanceScript* p_Instance, Creature* p_Source, Unit* p_Target)
{
    if (p_Instance == nullptr || p_Source == nullptr || p_Target == nullptr)
        return false;

    if (p_Instance->GetBossState(eFoundryDatas::DataIronMaidens) == EncounterState::IN_PROGRESS)
        return false; ///< Prevent recursive calls

    p_Instance->SetBossState(eFoundryDatas::DataIronMaidens, EncounterState::IN_PROGRESS);

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = Creature::GetCreature(*p_Source, p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, l_Maiden);

            if (l_Maiden->IsAIEnabled)
                l_Maiden->AI()->AttackStart(p_Target);

            l_Maiden->SetInCombatWith(p_Target);
        }
    }

    return true;
}

static void WipeMaidens(InstanceScript* p_Instance)
{
    if (p_Instance == nullptr)
        return;

    if (p_Instance->GetBossState(eFoundryDatas::DataIronMaidens) != EncounterState::IN_PROGRESS)
        return; ///< Prevent recursive calls

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = p_Instance->instance->GetCreature(p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            if (l_Maiden->IsAIEnabled)
                l_Maiden->AI()->EnterEvadeMode();

            p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, l_Maiden);
        }
    }

    p_Instance->SetBossState(eFoundryDatas::DataIronMaidens, EncounterState::FAIL);
}
