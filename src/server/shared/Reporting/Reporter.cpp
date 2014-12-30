#include "Reporter.hpp"
#include "ScopedMutex.hpp"

namespace MS
{
    namespace Reporting
    {
        Reporter::Reporter()
            : m_ReportQueue(),
            m_Mutex()
        {
        }

        void Reporter::Report(report_type const& p_Query)
        {
            // We lock in order to access the queue.
            Threading::ScopedMutex l_ScopedMutex(m_Mutex);

            m_ReportQueue.emplace(std::move(std::make_unique<report_type>(p_Query)));
        }

        bool Reporter::HasReports() const
        {
            // We lock in order to access the queue.
            Threading::ScopedMutex l_ScopedMutex(m_Mutex);

            return !m_ReportQueue.empty();
        }

        Reporter::report_pointer const&& Reporter::Next()
        {
            // We lock in order to access the queue.
            Threading::ScopedMutex l_ScopedMutex(m_Mutex);

            auto const&& l_Report = std::move(m_ReportQueue.front());
            m_ReportQueue.pop();

            return std::move(l_Report);
        }
    }
}