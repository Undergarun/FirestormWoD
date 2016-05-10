////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef REPORTER_HPP
# define REPORTER_HPP

# include <ace/Singleton.h>
# include <EasyJSon.hpp>

# include "ByteBuffer.h"

class Reporter
{
    public:
        friend class ACE_Singleton<Reporter, ACE_Thread_Mutex>;

        Reporter() { }

        void SetActiveState(bool p_Apply) { m_Enabled = p_Apply; }
        void SetAddress(std::string const p_Address) { m_Address = p_Address; }
        void SetIndex(std::string const p_Index) { m_Index = p_Index; }

        bool HasReports() { return !m_ReportQueue.empty(); }

        void ScheduleNextReport();

        void EnqueueReport(std::string p_Datas);

    private:
        ACE_Based::LockedQueue<std::string, ACE_Thread_Mutex> m_ReportQueue;

        bool        m_Enabled;
        std::string m_Address;
        std::string m_Index;
};

# define sReporter ACE_Singleton<Reporter, ACE_Thread_Mutex>::instance()

#endif ///< REPORTER_HPP
