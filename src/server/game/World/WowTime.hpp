////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Define.h"

#pragma once
#ifndef WOWTIME_HPP_INCLUDED
#define WOWTIME_HPP_INCLUDED

namespace MS { namespace Utilities
{
    namespace Globals
    {
        namespace InSeconds
        {
            enum
            {
                Seconds = 1,
                Minute  = 60 * InSeconds::Seconds,
                Hour    = 60 * InSeconds::Minute,
                Day     = 24 * InSeconds::Hour
            };

        }   ///< namespace InSeconds

        namespace InMinutes
        {
            enum
            {
                Minute  = 1,
                Hour    = 60 * InMinutes::Minute,
                Day     = 24 * InMinutes::Hour
            };

        }   ///< namespace InMinutes

    }   ///< namespace Globals

    /// WoWTime client class
    class WowTime
    {
        public:
            /// Constructor
            WowTime();

            /// Encode the time
            uint32 Encode();
            /// Encode the time
            static uint32 Encode(time_t p_Time);
            /// Decode a encoded time
            void Decode(uint32 p_EncodedTime);
            /// Make a WowTime instance from a packed time
            static WowTime FromEncodedTime(uint32 p_EncodedTime);

            /// Add days to this timestamp
            void AddDays(int32 p_Count, bool p_KeepHoursAndMinutes);
            /// Add minutes to this timestamp
            void AddMinutes(int32 p_Count);
            /// Add holiday duration
            void AddHolidayDuration(int32 p_Duration);

            /// Get day count since epoch
            uint32 GetDaysSinceEpoch();
            /// Get POSIX time from UTC timestamp
            time_t GetPosixTimeFromUTC();
            /// Get time
            time_t GetPosixTime() const;
            /// Get hours + minutes in minutes
            time_t GetHourAndMinutes();

            /// Set UTC time from POSIX timestamp
            void SetUTCTimeFromPosixTime(time_t p_PosixTime);
            /// Set hours + minutes
            void SetHourAndMinutes(uint32 p_Minutes);
            /// Set hours + minutes
            void SetHourAndMinutes(uint32 p_Hours, uint32 p_Minutes);
            /// Set date [Month_0..11], [MonthDay_0..31]
            bool SetDate(uint32 p_Month, uint32 p_MonthDay, uint32 p_Year);

            /// Diff 2 WoWTime
            time_t DiffTime(WowTime const& p_Other);
            /// Is the same day
            bool IsSameDay(WowTime const& p_Other);
            /// In range
            bool InRange(WowTime const& p_Left, WowTime const& p_Right);
            /// Compute region time
            void ComputeRegionTime(WowTime& p_Other);

            /// Compare
            bool operator==(WowTime const& p_Other) const;
            /// Compare
            bool operator!=(WowTime const& p_Other) const;
            /// Compare
            bool operator<(WowTime const& p_Other) const;
            /// Compare
            bool operator<=(WowTime const& p_Other) const;
            /// Compare
            bool operator>(WowTime const& p_Other) const;
            /// Compare
            bool operator>=(WowTime const& p_Other) const;

        public:
            int32 Minute;
            int32 Hour;
            int32 WeekDay;
            int32 MonthDay;
            int32 Month;
            int32 Year;
            int32 Unk1;
            int32 YearDay;

    };

}   ///< namespace Utilities
}   ///< namespace MS

#endif  ///< WOWTIME_HPP_INCLUDED
