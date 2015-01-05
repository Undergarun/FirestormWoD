#ifndef REPORTS_HPP
# define REPORTS_HPP

# include <iostream>
# include <EasyJSon.hpp>

namespace MS
{
    namespace Reporting
    {
        template <std::uint32_t ReportType> 
        class MakeReport
        {
        public:
            template <typename... Args>
            static std::string Craft(Args... p_Args)
            {
                return std::string();
            }
        };

# include "Reports.hxx"
    }
}

#endif /* !REPORTS_HPP */