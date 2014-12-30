#ifndef REPORTING_HPP
# define REPORTING_HPP

# include <ace/Singleton.h>

# include <queue>
# include <tuple>
# include <mutex>
# include <memory>

# include "Reports.hpp"

namespace MS
{
    namespace Reporting
    {
        class Reporter
        {
        public:
            typedef ByteBuffer report_type;
            typedef std::unique_ptr<report_type> report_pointer;
        public:
            friend class ACE_Singleton<Reporter, ACE_Thread_Mutex>;

            Reporter();

            void Report(report_type const& p_Query);
            bool HasReports() const;
            report_pointer const&& Next();

        private:
            std::queue<report_pointer> m_ReportQueue;
            mutable std::mutex m_Mutex;
        };
    }
}

# define sReporter ACE_Singleton<MS::Reporting::Reporter, ACE_Thread_Mutex>::instance()

#endif /* !REPORTING_HPP */