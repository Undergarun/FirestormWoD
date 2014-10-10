#include "ScriptMgr.h"
#include "PetBattle.h"
#include "PetBattleAbilityEffect.h"
#include "DB2Stores.h"

//class pba_magma_wave : public PetBattleAbilityScript
//{
//public:
//    pba_magma_wave() : PetBattleAbilityScript(319) {}
//
//    virtual bool SelectTargets(PetBattleAbilityEffect* effect) override
//    {
//        if (effect->EffectInfo->id <= 3) // 1, 2, 3
//        {
//            BattlePetInstance* caster = effect->PetBattleInstance->Pets[effect->Caster];
//            BattlePetInstance* target = effect->PetBattleInstance->Teams[!caster->TeamID]->TeamPets[effect->EffectInfo->id];
//            if (target)
//                effect->AddTarget(target->ID);
//
//            return true;
//        }
//
//        return false;
//    }
//};
//
//void AddSC_petbattle_abilities()
//{
//    sScriptMgr->AddScript(new pba_magma_wave());
//}
