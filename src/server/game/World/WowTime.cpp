////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "WowTime.hpp"
#include <ace/OS_NS_time.h>

namespace MS { namespace Utilities
{
    /// Constructor
    WowTime::WowTime()
    {
        Minute      = -1;
        Hour        = -1;
        WeekDay     = -1;
        MonthDay    = -1;
        Month       = -1;
        Year        = -1;
        Unk1        = 0;
        YearDay     = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Encode the time
    uint32 WowTime::Encode()
    {
        uint32 l_Result = (this->Minute & 63) | ((this->Hour & 31) << 6) | ((this->WeekDay & 7) << 11) | ((this->MonthDay & 63) << 14) | ((this->Month & 15) << 20) | ((this->Year & 31) << 24) | ((this->Unk1 & 3) << 29);
        return l_Result;
    }

    /// Encode the time
    uint32 WowTime::Encode(time_t p_Time)
    {
        WowTime l_Time;
        l_Time.SetUTCTimeFromPosixTime(p_Time);

        return l_Time.Encode();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Decode a encoded time
    void WowTime::Decode(uint32 p_EncodedTime)
    {
        int l_Temp = p_EncodedTime & 0x3F;
        if (l_Temp == 63)
            this->Minute = -1;
        else
            this->Minute = l_Temp;

        l_Temp = (p_EncodedTime >> 6) & 0x1F;
        if (l_Temp == 31)
            this->Hour = -1;
        else
            this->Hour = l_Temp;

        l_Temp = (p_EncodedTime >> 11) & 7;
        if (l_Temp == 7)
            this->WeekDay = -1;
        else
            this->WeekDay = l_Temp;

        l_Temp = (p_EncodedTime >> 14) & 0x3F;
        if (l_Temp == 63)
            this->MonthDay = -1;
        else
            this->MonthDay = l_Temp;

        l_Temp = (p_EncodedTime >> 20) & 0xF;
        if (l_Temp == 15)
            this->Month = -1;
        else
            this->Month = l_Temp;

        l_Temp = (p_EncodedTime >> 24) & 0x1F;
        if (l_Temp == 31)
            this->Year = -1;
        else
            this->Year = l_Temp;

        l_Temp = (p_EncodedTime >> 29) & 3;
        if (l_Temp == 3)
            this->Unk1 = -1;
        else
            this->Unk1 = l_Temp;
    }

    /// Make a WowTime instance from a packed time
    WowTime WowTime::FromEncodedTime(uint32 p_EncodedTime)
    {
        WowTime l_Time;
        l_Time.Decode(p_EncodedTime);

        return l_Time;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add days to this timestamp
    void WowTime::AddDays(int32 p_Count, bool p_KeepHoursAndMinutes)
    {
        if (this->Year >= 0 && this->Month >= 0 && this->MonthDay >= 0)
        {
            struct tm l_TimeInfo;
            l_TimeInfo.tm_sec   = 0;
            l_TimeInfo.tm_year  = this->Year + 100;
            l_TimeInfo.tm_mon   = this->Month;
            l_TimeInfo.tm_mday  = this->MonthDay + 1;
            l_TimeInfo.tm_isdst = -1;

            time_t l_PosixTime = 0;
            if (p_KeepHoursAndMinutes)
            {
                l_TimeInfo.tm_hour  = this->Hour;
                l_TimeInfo.tm_min   = this->Minute;

                l_PosixTime = mktime(&l_TimeInfo) + (Globals::InSeconds::Day * p_Count);
            }
            else
                l_PosixTime = mktime(&l_TimeInfo) + (Globals::InSeconds::Day * p_Count) + Globals::InSeconds::Hour;

            ACE_OS::localtime_r(&l_PosixTime, &l_TimeInfo);

            this->Year      = l_TimeInfo.tm_year - 100;
            this->Month     = l_TimeInfo.tm_mon;
            this->MonthDay  = l_TimeInfo.tm_mday - 1;
            this->Hour      = l_TimeInfo.tm_hour;
            this->Minute    = l_TimeInfo.tm_min;
            this->WeekDay   = l_TimeInfo.tm_wday;
        }
    }

    /// Add minutes to this timestamp
    void WowTime::AddMinutes(int32 p_Count)
    {
        int l_TotalMinutes = 0;
        if (this->Hour >= 0 && this->Minute >= 0)
            l_TotalMinutes = (Globals::InMinutes::Hour * this->Hour) + this->Minute;

        signed int l_RemainMinutes = l_TotalMinutes + p_Count;
        if ((l_TotalMinutes + p_Count + 1439) >= 2879)
        {
            int l_DayCount = l_RemainMinutes / Globals::InMinutes::Day;
            l_RemainMinutes %= Globals::InMinutes::Day;

            if (this->Year >= 0 && this->Month >= 0 && this->MonthDay >= 0)
            {
                struct tm l_TimeInfo;
                l_TimeInfo.tm_sec   = 0LL;
                l_TimeInfo.tm_year  = this->Year + 100;
                l_TimeInfo.tm_mon   = this->Month;
                l_TimeInfo.tm_mday  = this->MonthDay + 1;
                l_TimeInfo.tm_isdst = -1;

                time_t l_Time = (Globals::InSeconds::Day * l_DayCount) + mktime(&l_TimeInfo) + Globals::InSeconds::Hour;
                ACE_OS::localtime_r(&l_Time, &l_TimeInfo);

                this->Year      = l_TimeInfo.tm_year - 100;
                this->Month     = l_TimeInfo.tm_mon;
                this->MonthDay  = l_TimeInfo.tm_mday - 1;
                this->WeekDay   = l_TimeInfo.tm_wday;
            }
        }
        if (l_RemainMinutes < 0)
        {
            if (this->Year >= 0 && this->Month >= 0 && this->MonthDay >= 0)
            {
                struct tm l_TimeInfo;
                l_TimeInfo.tm_sec   = 0LL;
                l_TimeInfo.tm_year  = this->Year + 100;
                l_TimeInfo.tm_mon   = this->Month;
                l_TimeInfo.tm_mday  = this->MonthDay + 1;
                l_TimeInfo.tm_isdst = -1;

                time_t l_Time = mktime(&l_TimeInfo) - (23 * Globals::InSeconds::Hour);
                ACE_OS::localtime_r(&l_Time, &l_TimeInfo);

                this->Year      = l_TimeInfo.tm_year - 100;
                this->Month     = l_TimeInfo.tm_mon;
                this->MonthDay  = l_TimeInfo.tm_mday - 1;
                this->WeekDay   = l_TimeInfo.tm_wday;
            }

            l_RemainMinutes += Globals::InMinutes::Day;
        }

        this->Hour      = l_RemainMinutes / Globals::InMinutes::Hour;
        this->Minute    = l_RemainMinutes % Globals::InMinutes::Hour;
    }

    /// Add holiday duration
    void WowTime::AddHolidayDuration(int32 p_Duration)
    {
        if (p_Duration >= Globals::InMinutes::Day)
            AddDays(p_Duration / Globals::InMinutes::Day, true);

        AddMinutes(p_Duration % Globals::InMinutes::Day);

        int l_OldTotalMinutes = 0;
        if (this->Hour >= 0 && this->Minute >= 0)
            l_OldTotalMinutes = (Globals::InMinutes::Hour * this->Hour) + this->Minute;

        int64 l_Duration        = (p_Duration % Globals::InMinutes::Day) + this->Minute + (Globals::InMinutes::Hour * this->Hour);
        int64 l_NewTotalMinute  = l_Duration - Globals::InMinutes::Day * (((((-1240768329 * l_Duration) >> 32) + l_Duration) >> 31) + ((((-1240768329 * l_Duration) >> 32) + l_Duration) >> 10));

        if (l_NewTotalMinute != l_OldTotalMinutes)
        {
            signed int l_Unk = Globals::InMinutes::Day;

            if (this->Hour >= 0 && this->Minute >= 0)
                l_Unk = (Globals::InMinutes::Hour * this->Hour) + this->Minute + Globals::InMinutes::Day;

            if ((l_Unk - l_NewTotalMinute) % Globals::InMinutes::Day == Globals::InMinutes::Hour)
            {
                if (this->Hour < 0 || this->Minute < 0 || ((Globals::InMinutes::Hour * this->Hour) + this->Minute) <= 59)
                {
                    if (this->Year >= 0 && this->Month >= 0 && this->MonthDay >= 0)
                    {
                        struct tm l_TimeInfo;
                        l_TimeInfo.tm_sec   = 0LL;
                        l_TimeInfo.tm_year  = this->Year + 100;
                        l_TimeInfo.tm_mon   = this->Month;
                        l_TimeInfo.tm_mday  = this->MonthDay + 1;
                        l_TimeInfo.tm_isdst = -1;

                        time_t l_Time = mktime(&l_TimeInfo) - (23 * Globals::InSeconds::Hour);
                        ACE_OS::localtime_r(&l_Time, &l_TimeInfo);

                        this->Year      = l_TimeInfo.tm_year - 100;
                        this->Month     = l_TimeInfo.tm_mon;
                        this->MonthDay  = l_TimeInfo.tm_mday - 1;
                        this->WeekDay   = l_TimeInfo.tm_wday;
                    }
                }

                this->Hour      = l_NewTotalMinute / Globals::InMinutes::Hour;
                this->Minute    = l_NewTotalMinute % Globals::InMinutes::Hour;
            }
            else
            {
                AddMinutes(Globals::InMinutes::Hour);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get day count since epoch
    uint32 WowTime::GetDaysSinceEpoch()
    {
        time_t l_PosixTime = GetPosixTimeFromUTC();

        if (!l_PosixTime)
            return 0;

        return l_PosixTime / Globals::InSeconds::Day;
    }

    /// Get POSIX time from UTC timestamp
    time_t WowTime::GetPosixTimeFromUTC()
    {
        if (this->Year >= 0 && this->Month >= 0 && this->MonthDay >= 0)
        {
            struct tm l_TimeInfo;
            l_TimeInfo.tm_sec   = 0;
            l_TimeInfo.tm_year  = this->Year + 100;
            l_TimeInfo.tm_mon   = this->Month;
            l_TimeInfo.tm_mday  = this->MonthDay + 1;

            if (this->Hour >= 0)
            {
                l_TimeInfo.tm_hour = this->Hour;

                if (this->Minute >= 0)
                    l_TimeInfo.tm_min = this->Minute;
            }

            l_TimeInfo.tm_isdst = -1;
            l_TimeInfo.tm_wday  = this->WeekDay;
            time_t l_Time = 0;

#ifdef _MSC_VER
            l_Time = _mkgmtime(&l_TimeInfo);
#else
            l_Time = timegm(&l_TimeInfo);
#endif
            return l_Time;
        }

        return 0;
    }

    /// Get time
    time_t WowTime::GetPosixTime() const
    {
        struct tm l_TimeInfo;

        l_TimeInfo.tm_sec   = 0LL;
        l_TimeInfo.tm_year  = this->Year + 100;
        l_TimeInfo.tm_mon   = this->Month;
        l_TimeInfo.tm_mday  = this->MonthDay + 1;
        l_TimeInfo.tm_hour  = this->Hour;
        l_TimeInfo.tm_min   = this->Minute;
        l_TimeInfo.tm_isdst = -1;

        return mktime(&l_TimeInfo);
    }

    /// Get hours + minutes in minutes
    time_t WowTime::GetHourAndMinutes()
    {
        return (this->Hour * Globals::InMinutes::Hour) + this->Minute;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UTC time from POSIX timestamp
    void WowTime::SetUTCTimeFromPosixTime(time_t p_PosixTime)
    {
        struct tm l_TimeInfo;
        ACE_OS::gmtime_r(&p_PosixTime, &l_TimeInfo);

        this->Year      = l_TimeInfo.tm_year - 100;
        this->Month     = l_TimeInfo.tm_mon;
        this->MonthDay  = l_TimeInfo.tm_mday - 1;
        this->Hour      = l_TimeInfo.tm_hour;
        this->Minute    = l_TimeInfo.tm_min;
        this->WeekDay   = l_TimeInfo.tm_wday;
    }

    /// Set hours + minutes
    void WowTime::SetHourAndMinutes(uint32 p_Minutes)
    {
        this->Hour      = p_Minutes / Globals::InMinutes::Hour;
        this->Minute    = p_Minutes % Globals::InMinutes::Hour;
    }

    /// Set hours + minutes
    void WowTime::SetHourAndMinutes(uint32 p_Hours, uint32 p_Minutes)
    {
        if (p_Hours <= 23 && p_Minutes <= 59)
        {
            this->Hour      = p_Hours;
            this->Minute    = p_Minutes;
        }
    }

    /// Set date [Month_0..11], [MonthDay_0..31]
    bool WowTime::SetDate(uint32 p_Month, uint32 p_MonthDay, uint32 p_Year)
    {
        if (p_Month <= 11 && p_MonthDay <= 31)
        {
            unsigned int l_Year = p_Year - 2000;
            if (p_Year <= 1999)
                l_Year = p_Year;

            if (l_Year <= 31)
            {
                this->Month     = p_Month;
                this->MonthDay  = p_MonthDay;
                this->Year      = p_Year;
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Diff 2 WoWTime
    time_t WowTime::DiffTime(WowTime const& p_Other)
    {
        return p_Other.GetPosixTime() - GetPosixTime();
    }

    /// Is the same day
    bool WowTime::IsSameDay(WowTime const& p_Other)
    {
        if (p_Other.Year != this->Year || p_Other.Month != this->Month || p_Other.MonthDay != this->MonthDay)
            return false;

        return true;
    }

    /// In range
    bool WowTime::InRange(WowTime const& p_Left, WowTime const& p_Right)
    {
        bool l_Cond1 = p_Left <= p_Right;
        bool l_Cond2 = *this >= p_Left;

        if (!l_Cond1)
        {
            if (l_Cond2)
                return true;

            return (*this < p_Right);
        }
        if (l_Cond2)
            return (*this < p_Right);

        return false;
    }

    /// Compute region time
    void WowTime::ComputeRegionTime(WowTime& p_Other)
    {
        static int32 s_holidayOffsetSeconds = 0;

        auto l_DiffTimeFromRegionToUTC = []() -> time_t
        {
            time_t l_LocalCurrentTime = time(nullptr);

            struct tm *l_UTCTimeInfo = gmtime(&l_LocalCurrentTime);
            l_UTCTimeInfo->tm_isdst = -1;

            return mktime(l_UTCTimeInfo) - l_LocalCurrentTime;
        };

        if (this->YearDay)
        {
            struct tm l_TimeInfo;

            if (s_holidayOffsetSeconds || (s_holidayOffsetSeconds = (Globals::InSeconds::Hour * this->YearDay) - l_DiffTimeFromRegionToUTC()))
            {
                l_TimeInfo.tm_sec   = 0LL;
                l_TimeInfo.tm_year  = p_Other.Year + 100;
                l_TimeInfo.tm_mon   = p_Other.Month;
                l_TimeInfo.tm_mday  = p_Other.MonthDay + 1;
                l_TimeInfo.tm_isdst = -1;
                l_TimeInfo.tm_hour  = p_Other.Hour;
                l_TimeInfo.tm_min   = p_Other.Minute;

                time_t l_Time = mktime(&l_TimeInfo) + s_holidayOffsetSeconds;
                ACE_OS::localtime_r(&l_Time, &l_TimeInfo);

                p_Other.Year        = l_TimeInfo.tm_year - 100;
                p_Other.Month       = l_TimeInfo.tm_mon;
                p_Other.MonthDay    = l_TimeInfo.tm_mday - 1;
                p_Other.Hour        = l_TimeInfo.tm_hour;
                p_Other.Minute      = l_TimeInfo.tm_min;
                p_Other.WeekDay     = -1;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Compare
    bool WowTime::operator==(WowTime const& p_Other) const
    {
        if (   (p_Other.Year        < 0 || this->Year       < 0 || p_Other.Year     == this->Year)
            && (p_Other.Month       < 0 || this->Month      < 0 || p_Other.Month    == this->Month)
            && (p_Other.MonthDay    < 0 || this->MonthDay   < 0 || p_Other.MonthDay == this->MonthDay)
            && (p_Other.WeekDay     < 0 || this->WeekDay    < 0 || p_Other.WeekDay  == this->WeekDay)
            && (p_Other.Hour        < 0 || this->Hour       < 0 || p_Other.Hour     == this->Hour)
            && (p_Other.Minute      < 0 || this->Minute     < 0 || p_Other.Minute   == this->Minute))
            return true;

        return false;
    }
    /// Compare
    bool WowTime::operator!=(WowTime const& p_Other) const
    {
        return !(*this == p_Other);
    }
    /// Compare
    bool WowTime::operator<(WowTime const& p_Other) const
    {
        if (*this == p_Other)
            return false;

        if (p_Other.Year >= 0 && this->Year >= 0)
        {
            if (this->Year > p_Other.Year)
                return false;

            if (this->Year < p_Other.Year)
                return true;
        }

        if (p_Other.Month >= 0 && this->Month >= 0)
        {
            if (this->Month > p_Other.Month)
                return false;

            if (this->Month < p_Other.Month)
                return true;
        }

        if (p_Other.MonthDay >= 0 && this->MonthDay >= 0)
        {
            if (this->MonthDay > p_Other.MonthDay)
                return false;

            if (this->MonthDay < p_Other.MonthDay)
                return true;
        }

        if (p_Other.WeekDay >= 0 && this->WeekDay >= 0)
        {
            if (this->WeekDay > p_Other.WeekDay)
                return false;

            if (this->WeekDay < p_Other.WeekDay)
                return true;
        }

        if (p_Other.Hour >= 0 && this->Hour >= 0)
        {
            if (this->Hour > p_Other.Hour)
                return false;

            if (this->Hour < p_Other.Hour)
                return true;
        }

        if (p_Other.Minute < 0)
            return false;
        else
        {
            if (this->Minute >= 0)
                return this->Minute < p_Other.Minute;
        }

        return false;
    }
    /// Compare
    bool WowTime::operator<=(WowTime const& p_Other) const
    {
        if (*this == p_Other)
            return true;

        return (*this < p_Other);
    }
    /// Compare
    bool WowTime::operator>(WowTime const& p_Other) const
    {
        if (*this == p_Other)
            return false;

        return !(*this <= p_Other);
    }
    /// Compare
    bool WowTime::operator>=(WowTime const& p_Other) const
    {
        if (*this == p_Other)
            return true;

        return !(*this < p_Other);
    }

}   ///< namespace Utilities
}   ///< namespace MS
