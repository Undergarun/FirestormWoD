#ifndef REPORTS_HPP
# define REPORTS_HPP

# include "ByteBuffer.h"

namespace MS
{
    namespace Reporting
    {
        enum ReportType
        {
            CharacterCreation,
        };

        template <uint32_t ReportType> 
        class MakeReport
        {
        public:
            template <typename... Args>
            static ByteBuffer Craft(Args... p_Args)
            {
                return ByteBuffer();
            }
        };

        //// Craft a "CharacterCreation" report
        /// @p_Arg0 : Account ID (expect uint32_t)
        /// @p_Arg1 : TimeStamp (expect uint32_t)
        template <>
        template <typename... Args>
        static ByteBuffer MakeReport<ReportType::CharacterCreation>::Craft(Args... p_Args)
        {
            static_assert(sizeof... (p_Args) == 2);
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            ByteBuffer l_Buffer;
            l_Buffer << static_cast<uint32_t>(std::get<0>(l_DeveloppedArgs)); // AccountId.
            l_Buffer << static_cast<uint32_t>(std::get<1>(l_DeveloppedArgs)); // TimeStamp.

            return l_Buffer;
        }
    }
}

#endif /* !REPORTS_HPP */