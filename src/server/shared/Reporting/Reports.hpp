#ifndef REPORTS_HPP
# define REPORTS_HPP

# include "ByteBuffer.h"

namespace MS
{
    namespace Reporting
    {
        namespace ReportType
        {
            enum ReportType
            {
                CharacterCreation,
            };
        }

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

        template <>
        template <typename... Args>
        static ByteBuffer MakeReport<ReportType::CharacterCreation>::Craft(Args... p_Args)
        {
            auto&& l_DeveloppedArgs = std::forward_as_tuple(p_Args...);

            ByteBuffer l_Buffer;
            l_Buffer << std::get<0>(l_DeveloppedArgs); // AccountId.
            l_Buffer << std::get<1>(l_DeveloppedArgs); // Time.

            return l_Buffer;
        }

        /*template <int, typename... Args>
        static ByteBuffer MakeReport<int, Args>(Args... p_Args)
        {
            ByteBuffer l_Buffer;
            l_Buffer << std::get<0>(p_Args); // AccountId.
            l_Buffer << std::get<1>(p_Args); // Time.

            return l_Buffer;
        }*/
    }
}

#endif /* !REPORTS_HPP */