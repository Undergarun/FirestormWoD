#ifndef JadeCore_GAME_SPELL_CHARGES_TRACKER_H
#define JadeCore_GAME_SPELL_CHARGES_TRACKER_H

#include <cstdint>
#include <unordered_map>

namespace JadeCore
{
    class SpellChargesTracker final
    {
        struct ChargeData final
        {
            std::uint32_t consumedCharges;
            std::uint32_t currRegenTimer;
            std::uint32_t baseRegenTimer;
        };

        public:
            void consume(std::uint32_t spellId, std::uint32_t regenTimer);

            std::uint32_t consumedCount(std::uint32_t spellId) const;

            void update(std::uint32_t diff);

        private:
            std::unordered_map<std::uint32_t, ChargeData> spellChargesMap_;
    };

    inline std::uint32_t SpellChargesTracker::consumedCount(std::uint32_t spellId) const
    {
        auto const i = spellChargesMap_.find(spellId);
        return (i != spellChargesMap_.end()) ? i->second.consumedCharges : 0;
    }
} // namespace JadeCore

#endif // JadeCore_GAME_SPELL_CHARGES_TRACKER_H