#include "ReportWorker.hpp"
#include "Reporter.hpp"

#include <iostream>

namespace MS
{
    namespace Reporting
    {
        namespace
        {
            static const std::string base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";


            static inline bool is_base64(unsigned char c) {
                return (isalnum(c) || (c == '+') || (c == '/'));
            }

            std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
                std::string ret;
                int i = 0;
                int j = 0;
                unsigned char char_array_3[3];
                unsigned char char_array_4[4];

                while (in_len--) {
                    char_array_3[i++] = *(bytes_to_encode++);
                    if (i == 3) {
                        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                        char_array_4[3] = char_array_3[2] & 0x3f;

                        for (i = 0; (i <4); i++)
                            ret += base64_chars[char_array_4[i]];
                        i = 0;
                    }
                }

                if (i)
                {
                    for (j = i; j < 3; j++)
                        char_array_3[j] = '\0';

                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for (j = 0; (j < i + 1); j++)
                        ret += base64_chars[char_array_4[j]];

                    while ((i++ < 3))
                        ret += '=';

                }

                return ret;

            }
        }

        ReportWorker::ReportWorker(std::string const&& p_Address)
            : m_Address(p_Address)
        {
        }

        ReportWorker::~ReportWorker()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        bool ReportWorker::ProcessReporting()
        {
            if (!sReporter->HasReports())
                return true;

            auto l_ReportData = sReporter->Next();
            auto l_Size = l_ReportData->size();

            CURLcode l_Res = CURLE_OK;
            CURL* l_Curl = curl_easy_init();
            if (l_Curl)
            {
                curl_easy_setopt(l_Curl, CURLOPT_URL, m_Address.c_str());

                /// If there is a redirection, the request will follow it.
                curl_easy_setopt(l_Curl, CURLOPT_FOLLOWLOCATION, 1L);

                std::string l_b64 = base64_encode(l_ReportData->contents(), l_Size);
                curl_easy_setopt(l_Curl, CURLOPT_POSTFIELDS, l_b64.c_str());
                curl_easy_setopt(l_Curl, CURLOPT_POSTFIELDSIZE, l_b64.size());

                /// Perform the request, res will get the return code.
                l_Res = curl_easy_perform(l_Curl);
                // Check for errors.
                if (l_Res != CURLE_OK)
                {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(l_Res));
                    sReporter->Fallback(std::move(l_ReportData));
                }

                /// Always cleanup.
                curl_easy_cleanup(l_Curl);
                return true;
            }

            return false;
        }
    }
}