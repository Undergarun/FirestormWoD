/// Credit merydwin@gmail.com
#include "AuthComponent.hpp"

#include <vector>
#include <algorithm>

namespace BNet2 {

    /// static instance (aka singleton)
    AuthComponentManager AuthComponentManager::m_Instance = AuthComponentManager();

    //////////////////////////////////////////////////////////////////////////

    /// Return unique instance of AuthComponentManager
    AuthComponentManager * AuthComponentManager::GetSingleton()
    {
        return &m_Instance;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Allow for a specific build programs / platforms / locales
    void AuthComponentManager::Allow(uint32_t p_Build, uint32_t p_Programs, uint32_t p_Platforms, uint32_t p_Locales)
    {
        std::vector<std::string> l_Programs     = GetPrograms(p_Programs);
        std::vector<std::string> l_Platforms    = GetPlatforms(p_Platforms);
        std::vector<std::string> l_Locales      = GetLocales(p_Locales);

        for (uint32_t l_Program = 0; l_Program < l_Programs.size(); ++l_Program)
        {
            for (uint32_t l_Platform = 0; l_Platform < l_Platforms.size(); ++l_Platform)
            {
                AuthComponent l_Component;
                l_Component.Build       = p_Build;
                l_Component.Platform    = l_Platforms[l_Platform];
                l_Component.Program     = l_Programs[l_Program];

                if (!HasComponent(l_Component))
                    m_Components.push_back(l_Component);
            }

            for (uint32_t l_Locale = 0; l_Locale < l_Locales.size(); ++l_Locale)
            {
                AuthComponent l_Component;
                l_Component.Build       = p_Build;
                l_Component.Platform    = l_Locales[l_Locale];
                l_Component.Program     = l_Programs[l_Program];

                if (!HasComponent(l_Component))
                    m_Components.push_back(l_Component);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////

    /// Check component
    AuthResult AuthComponentManager::Check(AuthComponent & p_Component)
    {
        if (!HasComponent(p_Component))
        {
            if (!HasBuild(p_Component.Build))
                return BNet2::BATTLENET2_AUTH_INVALID_GAME_VERSION;

            if (!HasProgram(p_Component.Program))
                return BNet2::BATTLENET2_AUTH_INVALID_PROGRAM;

            if (!HasPlatform(p_Component.Platform))
                return BNet2::BATTLENET2_AUTH_INVALID_PLATFORM;

            if (!HasLocal(p_Component.Platform))
                return BNet2::BATTLENET2_AUTH_INVALID_LOCALE;
        }

        return BNet2::BATTLENET2_AUTH_OK;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Has component
    bool AuthComponentManager::HasComponent(AuthComponent & p_Component)
    {
        for (std::list<AuthComponent>::iterator l_It = m_Components.begin(); l_It != m_Components.end(); ++l_It)
        {
            if ((*l_It) == p_Component)
                return true;
        }

        return false;
    }
    /// Specific check
    bool AuthComponentManager::HasPlatform(const std::string & p_Platform)
    {
        for (std::list<AuthComponent>::iterator l_It = m_Components.begin(); l_It != m_Components.end(); ++l_It)
        {
            if ((*l_It).Platform == p_Platform)
                return true;
        }

        return false;
    }
    /// Specific check
    bool AuthComponentManager::HasProgram(const std::string & p_Program)
    {
        for (std::list<AuthComponent>::iterator l_It = m_Components.begin(); l_It != m_Components.end(); ++l_It)
        {
            if ((*l_It).Program == p_Program)
                return true;
        }

        return false;
    }
    /// Specific check
    bool AuthComponentManager::HasLocal(const std::string & p_Local)
    {
        return HasProgram(p_Local);
    }
    /// Specific check
    bool AuthComponentManager::HasBuild(uint32_t p_Build)
    {
        for (std::list<AuthComponent>::iterator l_It = m_Components.begin(); l_It != m_Components.end(); ++l_It)
        {
            if ((*l_It).Build == p_Build)
                return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Flags to string arrays
    std::vector<std::string> AuthComponentManager::GetPrograms(uint32_t p_Flag)
    {
        std::vector<std::string> l_Out;

        if (p_Flag & BNet2::BATTLENET2_PROGRAM_BNET)
            l_Out.push_back("Bnet");
        if (p_Flag & BNet2::BATTLENET2_PROGRAM_WOW)
            l_Out.push_back("WoW");
        if (p_Flag & BNet2::BATTLENET2_PROGRAM_WOWB)
            l_Out.push_back("WoWB");
        if (p_Flag & BNet2::BATTLENET2_PROGRAM_WOWT)
            l_Out.push_back("WoWT");
        if (p_Flag & BNet2::BATTLENET2_PROGRAM_WOWTR)
            l_Out.push_back("WoWTR");

        return l_Out;
    }
    /// Flags to string arrays
    std::vector<std::string> AuthComponentManager::GetPlatforms(uint32_t p_Flag)
    {
        std::vector<std::string> l_Out;

        if (p_Flag & BNet2::BATTLENET2_PLATFORM_BASE)
            l_Out.push_back("base");
        if (p_Flag & BNet2::BATTLENET2_PLATFORM_WIN)
            l_Out.push_back("Win");
        if (p_Flag & BNet2::BATTLENET2_PLATFORM_WIN64)
            l_Out.push_back("Wn64");
        if (p_Flag & BNet2::BATTLENET2_PLATFORM_MAC64)
            l_Out.push_back("Mc64");

        return l_Out;
    }
    /// Flags to string arrays
    std::vector<std::string> AuthComponentManager::GetLocales(uint32_t p_Flag)
    {
        std::vector<std::string> l_Out;

        if (p_Flag & BNet2::BATTLENET2_LOCALE_FR)
            l_Out.push_back("frFR");
        if (p_Flag & BNet2::BATTLENET2_LOCALE_US)
            l_Out.push_back("enUS");
        if (p_Flag & BNet2::BATTLENET2_LOCALE_GB)
            l_Out.push_back("enGB");
        if (p_Flag & BNet2::BATTLENET2_LOCALE_ES)
            l_Out.push_back("esES");
        if (p_Flag & BNet2::BATTLENET2_LOCALE_DE)
            l_Out.push_back("deDE");

        return l_Out;
    }

}