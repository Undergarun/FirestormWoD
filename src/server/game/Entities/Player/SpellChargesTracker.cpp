#include "SpellChargesTracker.h"

namespace JadeCore
{
    void SpellChargesTracker::consume(std::uint32_t spellId, std::uint32_t regenTimer)
    {
        auto const res = spellChargesMap_.insert(std::make_pair(spellId, ChargeData()));
        auto &chargeData = res.first->second;

        ++chargeData.m_ConsumedCharges;

        if (res.second)
        {
            chargeData.m_CurrentRegenTime = regenTimer;
            chargeData.m_BaseRegenTime = regenTimer;
        }
    }

    void SpellChargesTracker::update(std::uint32_t diff)
    {
        for (auto i = spellChargesMap_.begin(); i != spellChargesMap_.end();)
        {
            auto &chargeData = i->second;

            // First, wait till it's time to replenish consumed charge
            if (chargeData.m_CurrentRegenTime > diff)
            {
                chargeData.m_CurrentRegenTime -= diff;
                ++i;
                continue;
            }

            // If we replenish last consumed charge, simply erase charge data
            if (--chargeData.m_ConsumedCharges == 0)
            {
                i = spellChargesMap_.erase(i);
                continue;
            }

            // Otherwise, we have to start countdown again
            std::uint32_t const rem = diff - chargeData.m_CurrentRegenTime;
            chargeData.m_CurrentRegenTime = chargeData.m_BaseRegenTime - rem;
            ++i;
        }
    }
} // namespace JadeCore