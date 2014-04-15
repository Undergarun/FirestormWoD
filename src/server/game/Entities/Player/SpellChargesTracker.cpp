#include "SpellChargesTracker.h"

namespace JadeCore
{
    void SpellChargesTracker::consume(std::uint32_t spellId, std::uint32_t regenTimer)
    {
        auto const res = spellChargesMap_.insert(std::make_pair(spellId, ChargeData()));
        auto &chargeData = res.first->second;

        ++chargeData.consumedCharges;

        if (res.second)
        {
            chargeData.currRegenTimer = regenTimer;
            chargeData.baseRegenTimer = regenTimer;
        }
    }

    void SpellChargesTracker::update(std::uint32_t diff)
    {
        for (auto i = spellChargesMap_.begin(); i != spellChargesMap_.end();)
        {
            auto &chargeData = i->second;

            // First, wait till it's time to replenish consumed charge
            if (chargeData.currRegenTimer > diff)
            {
                chargeData.currRegenTimer -= diff;
                ++i;
                continue;
            }

            // If we replenish last consumed charge, simply erase charge data
            if (--chargeData.consumedCharges == 0)
            {
                i = spellChargesMap_.erase(i);
                continue;
            }

            // Otherwise, we have to start countdown again
            std::uint32_t const rem = diff - chargeData.currRegenTimer;
            chargeData.currRegenTimer = chargeData.baseRegenTimer - rem;
            ++i;
        }
    }
} // namespace JadeCore