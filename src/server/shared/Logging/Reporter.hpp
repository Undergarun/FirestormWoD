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

# include "ByteBuffer.h"

class Reporter
{
    public:
        friend class ACE_Singleton<Reporter, ACE_Thread_Mutex>;

        Reporter() { }

        void SetAddress(std::string const p_Address) { m_Address = p_Address; }

        bool HasReports() { return !m_ReportQueue.empty(); }

        void ScheduleNextReport();

        void EnqueueReport(ByteBuffer p_Datas);

    private:
        ACE_Based::LockedQueue<ByteBuffer, ACE_Thread_Mutex> m_ReportQueue;

        std::string m_Address;
};

# define sReporter ACE_Singleton<Reporter, ACE_Thread_Mutex>::instance()

#endif ///< REPORTER_HPP