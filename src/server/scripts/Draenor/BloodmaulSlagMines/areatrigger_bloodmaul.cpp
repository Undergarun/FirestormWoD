////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "instance_bloodmaul.hpp"
#include "Common.h"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {

            // AreaTriggers for spells: 151582
            class AreaTrigger_SuppresionField : public AreaTriggerEntityScript
            {
                enum class Spells : uint32
                {
                    SuppresionFieldEffect = 151638,
                };

                std::forward_list<uint64> m_Targets;

            public:
                AreaTrigger_SuppresionField()
                    : AreaTriggerEntityScript("AreaTrigger_SuppresionField"),
                    m_Targets()
                {
                }

                AreaTriggerEntityScript* GetAI() const
                {
                    return new AreaTrigger_SuppresionField();
                }

                void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
                {
                    // If We are on the last tick.
                    if (p_AreaTrigger->GetDuration() < 100)
                    {

                        for (auto l_Guid : m_Targets)
                        {
                            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                            if (l_Target)
                                l_Target->RemoveAura(uint32(Spells::SuppresionFieldEffect));
                        }
                    }
                }

                void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
                {
                    std::list<Unit*> l_TargetList;
                    float l_Radius = 5.0f;

                    JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
                    JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                    p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                    std::forward_list<uint64> l_ToRemove; // We need to do it in two phase, otherwise it will break iterators.
                    for (auto l_Guid : m_Targets)
                    {
                        Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                        if (l_Target && l_Target->GetExactDist2d(p_AreaTrigger) > l_Radius)
                        {
                            if (l_Target->HasAura(uint32(Spells::SuppresionFieldEffect)))
                            {
                                l_ToRemove.emplace_front(l_Guid);
                                l_Target->RemoveAura(uint32(Spells::SuppresionFieldEffect));
                            }
                        }
                    }

                    for (auto l_Guid : l_ToRemove)
                    {
                        m_Targets.remove(l_Guid);
                    }

                    for (Unit* l_Unit : l_TargetList)
                    {
                        if (!l_Unit || l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius || l_Unit->HasAura(uint32(Spells::SuppresionFieldEffect)))
                            continue;

                        p_AreaTrigger->GetCaster()->CastSpell(l_Unit, uint32(Spells::SuppresionFieldEffect), true);
                        m_Targets.emplace_front(l_Unit->GetGUID());
                    }
                }
            };
        }
    }
}
#ifndef __clang_analyzer__
void AddSC_areatrigger_Bloodmaul()
{
    new MS::Instances::Bloodmaul::AreaTrigger_SuppresionField();
}
#endif
