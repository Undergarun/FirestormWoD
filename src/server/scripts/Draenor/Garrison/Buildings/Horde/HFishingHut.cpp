////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "HFishingHut.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79892 - Mak'jin                                                   ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_MakJin_Garr::npc_MakJin_Garr()
        : CreatureScript("npc_MakJin_Garr")
    {
    }

    /// Constructor
    npc_MakJin_GarrAI::npc_MakJin_GarrAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    bool npc_MakJin_Garr::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        if (p_Quest->GetQuestId() == Quests::Horde_AnglinInOurGarrison)
        {
            if (p_Player && p_Creature)
            {
                if (MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison())
                {
                    CreatureAI* l_AI = p_Creature->AI();

                    if (l_AI == nullptr)
                        return true;

                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Quest_FishFight))
                    {
                        p_Player->AddQuest(l_Quest, p_Creature);
                        p_Player->CompleteQuest(l_Quest->GetQuestId());
                        p_Player->RewardQuest(l_Quest, 0, p_Creature, false);
                    }

                    if (GarrisonNPCAI* l_GarrisonAI = dynamic_cast<GarrisonNPCAI*>(l_AI))
                        l_GarrisonMgr->ActivateBuilding(l_GarrisonAI->GetPlotInstanceID());
                }
            }
        }

        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_MakJin_Garr::GetAI(Creature * p_Creature) const
    {
        return new npc_MakJin_GarrAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS