#ifndef JadeCore_GAME_SPELL_CHARGES_TRACKER_H
#define JadeCore_GAME_SPELL_CHARGES_TRACKER_H

#include <cstdint>
#include <unordered_map>

struct ChargeData final
{
    std::uint32_t m_ConsumedCharges;
    std::uint32_t m_CurrentRegenTime;
    std::uint32_t m_BaseRegenTime;
};

typedef std::unordered_map<std::uint32_t, ChargeData> SpellChargesMap;

namespace JadeCore
{
    class SpellChargesTracker final
    {
        public:
            void consume(std::uint32_t spellId, std::uint32_t regenTimer);

            std::uint32_t consumedCount(std::uint32_t spellId) const;

            void update(std::uint32_t diff);

            SpellChargesMap GetSpellChargesMap() const { return spellChargesMap_; }

        private:
            SpellChargesMap spellChargesMap_;
    };

    inline std::uint32_t SpellChargesTracker::consumedCount(std::uint32_t spellId) const
    {
        auto const i = spellChargesMap_.find(spellId);
        return (i != spellChargesMap_.end()) ? i->second.m_ConsumedCharges : 0;
    }
} // namespace JadeCore

#endif // JadeCore_GAME_SPELL_CHARGES_TRACKER_H