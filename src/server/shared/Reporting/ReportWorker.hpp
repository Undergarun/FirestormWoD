#ifndef REPORT_WORKER_HPP
# define REPORT_WORKER_HPP

# define CURL_STATICLIB
# include <curl/curl.h>

#include <iostream>

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
