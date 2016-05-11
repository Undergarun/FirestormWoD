////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Splitting Breath - 161519
class areatrigger_ashran_splitting_breath : public AreaTriggerEntityScript
{
    public:
        areatrigger_ashran_splitting_breath() : AreaTriggerEntityScript("areatrigger_ashran_splitting_breath") { }

        enum eSpells
        {
            SplittingBreath = 161517
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 3.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, eSpells::SplittingBreath, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_ashran_splitting_breath();
        }
};

/// Volcano - 176144
class areatrigger_ashran_volcano : public AreaTriggerEntityScript
{
    public:
        areatrigger_ashran_volcano() : AreaTriggerEntityScript("areatrigger_ashran_volcano") { }

        enum eSpell
        {
            VolcanicFallout = 176146
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 4.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, eSpell::VolcanicFallout, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_ashran_volcano();
        }
};
#ifndef __clang_analyzer__
void AddSC_AshranAreaTrigger()
{
    new areatrigger_ashran_splitting_breath();
    new areatrigger_ashran_volcano();
}
#endif
