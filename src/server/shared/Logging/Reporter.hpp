////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2014-2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef REPORTER_HPP
# define REPORTER_HPP

# include <ace/Singleton.h>
# include <EasyJSon.hpp>

# include "ByteBuffer.h"

static const std::string k_Base64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

class Reporter
{
    public:
        friend class ACE_Singleton<Reporter, ACE_Thread_Mutex>;

        Reporter() { }

        void SetAddress(std::string const p_Address) { m_Address = p_Address; }
        void SetIndex(std::string const p_Index) { m_Index = p_Index; }

        bool HasReports() { return !m_ReportQueue.empty(); }

        void ScheduleNextReport();

        void EnqueueReport(std::string p_Datas);

        static inline bool IsBase64(uint8 p_Char)
        {
            return (isalnum(p_Char) || (p_Char == '+') || (p_Char == '/'));
        }

        std::string base64_encode(char const* p_BytesToEncode, uint32 p_Len)
        {
            std::string l_Ret;
            int32 l_I = 0;
            int32 l_J = 0;
            char l_CharArray3[3];
            char l_CharArray4[4];

            while (p_Len--)
            {
                l_CharArray3[l_I++] = *(p_BytesToEncode++);

                if (l_I == 3)
                {
                    l_CharArray4[0] = (l_CharArray3[0] & 0xfc) >> 2;
                    l_CharArray4[1] = ((l_CharArray3[0] & 0x03) << 4) + ((l_CharArray3[1] & 0xf0) >> 4);
                    l_CharArray4[2] = ((l_CharArray3[1] & 0x0f) << 2) + ((l_CharArray3[2] & 0xc0) >> 6);
                    l_CharArray4[3] = l_CharArray3[2] & 0x3f;

                    for (l_I = 0; (l_I <4); l_I++)
                        l_Ret += k_Base64Chars[l_CharArray4[l_I]];

                    l_I = 0;
                }
            }

            if (l_I)
            {
                for (l_J = l_I; l_J < 3; l_J++)
                    l_CharArray3[l_J] = '\0';

                l_CharArray4[0] = (l_CharArray3[0] & 0xfc) >> 2;
                l_CharArray4[1] = ((l_CharArray3[0] & 0x03) << 4) + ((l_CharArray3[1] & 0xf0) >> 4);
                l_CharArray4[2] = ((l_CharArray3[1] & 0x0f) << 2) + ((l_CharArray3[2] & 0xc0) >> 6);
                l_CharArray4[3] = l_CharArray3[2] & 0x3f;

                for (l_J = 0; (l_J < l_I + 1); l_J++)
                    l_Ret += k_Base64Chars[l_CharArray4[l_J]];

                while ((l_I++ < 3))
                    l_Ret += '=';
            }

            return l_Ret;
        }

    private:
        ACE_Based::LockedQueue<std::string, ACE_Thread_Mutex> m_ReportQueue;

        std::string m_Address;
        std::string m_Index;
};

# define sReporter ACE_Singleton<Reporter, ACE_Thread_Mutex>::instance()

#endif ///< REPORTER_HPP