/// Credit merydwin@gmail.com
#pragma once

#include "AuthResult.hpp"

#include <string>
#include <inttypes.h>
#include <list>
#include <vector>

namespace BNet2 {

    /// Authentication platforms
    enum Platforms
    {
        BATTLENET2_PLATFORM_BASE     = 1 << 0,  ///< Base for authentication protocol
        BATTLENET2_PLATFORM_WIN      = 1 << 1,  ///< Windows x86
        BATTLENET2_PLATFORM_WIN64    = 1 << 2,  ///< Windows x64
        BATTLENET2_PLATFORM_MAC64    = 1 << 4,  ///< Mac OSX x64

        BATTLENET2_PLATFORM_ALL_WINDOWS     = BATTLENET2_PLATFORM_WIN | BATTLENET2_PLATFORM_WIN64,
        BATTLENET2_PLATFORM_ALL_MACINTOSH   = BATTLENET2_PLATFORM_MAC64,
        BATTLENET2_PLATFORM_ALL             = 0xFFFFFFFF
    };

    /// Authentication programs
    enum Programs
    {
        BATTLENET2_PROGRAM_BNET     = 1 << 0,   ///< BNet authentication modules
        BATTLENET2_PROGRAM_WOW      = 1 << 1,   ///< WoW release client
        BATTLENET2_PROGRAM_WOWB     = 1 << 2,   ///< WoW beta client
        BATTLENET2_PROGRAM_WOWT     = 1 << 3,   ///< WoW test client
        BATTLENET2_PROGRAM_WOWTR    = 1 << 4,   ///< WoW test release client

        BATTLENET2_PROGRAM_ALL_CLIENTS  = BATTLENET2_PROGRAM_WOW | BATTLENET2_PROGRAM_WOWB | BATTLENET2_PROGRAM_WOWT | BATTLENET2_PROGRAM_WOWTR,
        BATTLENET2_PROGRAM_ALL          = 0xFFFFFFFF
    };

    /// Client locales
    enum Locales
    {
        BATTLENET2_LOCALE_FR = 1 << 0,  ///< frFR
        BATTLENET2_LOCALE_US = 1 << 1,  ///< enUS
        BATTLENET2_LOCALE_GB = 1 << 2,  ///< enGB
        BATTLENET2_LOCALE_ES = 1 << 3,  ///< esES
        BATTLENET2_LOCALE_DE = 1 << 4,  ///< deDE

        BATTLENET2_LOCALE_ALL = 0xFFFFFFFF,
        BATTLENET2_LOCALE_NONE = 0
    };

    /// Authentication component
    struct AuthComponent
    {
        std::string Program;    ///< Component programs (WoW, BNet, ...)
        std::string Platform;   ///< Component platform (Win, Win64, Base, ...)
        uint32_t    Build;      ///< Component build

        /// Other component is equal
        bool operator== (const AuthComponent & p_Right) const
        {
            return Program == p_Right.Program && Platform == p_Right.Platform && Build == p_Right.Build;
        }
    };

    /// Authentication component manager
    class AuthComponentManager
    {
        /// static instance (aka singleton)
        static AuthComponentManager m_Instance;

        public:
            /// Return unique instance of AuthComponentManager
            static AuthComponentManager * GetSingleton();

            /// Allow for a specific build programs / platforms / locales
            void Allow(uint32_t p_Build, uint32_t p_Programs, uint32_t p_Platforms, uint32_t p_Locales);

            /// Check component
            AuthResult Check(AuthComponent & p_Component);

            /// Has component
            bool HasComponent(AuthComponent & p_Component);
            /// Specific check
            bool HasPlatform(const std::string & p_Platform);
            /// Specific check
            bool HasProgram(const std::string & p_Program);
            /// Specific check
            bool HasLocal(const std::string & p_Local);
            /// Specific check
            bool HasBuild(uint32_t p_Build);

            /// Flags to string arrays
            std::vector<std::string> GetPrograms(uint32_t p_Flag);
            /// Flags to string arrays
            std::vector<std::string> GetPlatforms(uint32_t p_Flag);
            /// Flags to string arrays
            std::vector<std::string> GetLocales(uint32_t p_Flag);

        private:
            std::list<AuthComponent> m_Components;  ///< Registered component

    };

}