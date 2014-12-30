#ifndef REPORT_WORKER_HPP
# define REPORT_WORKER_HPP

# include <curl/curl.h>

namespace MS
{
    namespace Reporting
    {
        class ReportWorker
        {
        public:
            ReportWorker(std::string const&& p_Address);
            ~ReportWorker();

            bool ProcessReporting();

        private:
            std::string m_Address;
        };
    }
}

#endif /* !REPORT_WORKER_HPP */