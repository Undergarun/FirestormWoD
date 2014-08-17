/// Credit merydwin@gmail.com
#include "Module.hpp"
#include <algorithm>

namespace BNet2 {

    /// Constructor
    Module::Module(const std::string p_Hash, ModuleType p_Type, Platforms p_Platform, uint32_t p_ModuleID)
        : m_ModuleHash(p_Hash), m_Type(p_Type), m_Platform(p_Platform), m_ID(p_ModuleID)
    {
        std::transform(m_ModuleHash.begin(), m_ModuleHash.end(), m_ModuleHash.begin(), ::tolower);

        char * l_Str = const_cast<char*>(p_Hash.c_str());

        for (unsigned int l_I = 0; l_I < p_Hash.size(); l_I += 2)
        {
            char l_Buffer[3] = { *(l_Str + l_I), *(l_Str + l_I + 1), '\0' };
            m_ModuleHashData[l_I / 2] = strtol(l_Buffer, NULL, 16);
        }
    }

    //////////////////////////////////////////////////////////////////////////

    /// Get module ID
    uint32_t Module::GetID()
    {
        return m_ID;
    }
    /// Get module hash
    std::string Module::GetHash()
    {
        return m_ModuleHash;
    }
    /// Get module hash data
    uint8_t * Module::GetHashData()
    {
        return m_ModuleHashData;
    }
    /// Get module hash data size
    uint32_t Module::GetHashDataSize()
    {
        return SHA256_DIGEST_LENGTH;
    }
    /// Platform
    Platforms Module::GetPlatform()
    {
        return m_Platform;
    }
    /// Get type
    ModuleType Module::GetType()
    {
        return m_Type;
    }
    /// Get type str
    std::string Module::GetTypeStr()
    {
        switch (m_Type)
        {
            case BNet2::BATTLENET2_MODULE_TYPE_AUTH:
                return "auth";
                break;

            default:
                break;
        }

        return "unkn";
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// static instance (aka singleton)
    ModuleManager ModuleManager::m_Instance = ModuleManager();

    //////////////////////////////////////////////////////////////////////////

    /// Return unique instance of AuthComponentManager
    ModuleManager * ModuleManager::GetSingleton()
    {
        return &m_Instance;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Has module
    bool ModuleManager::HasModule(Module::Ptr p_Module)
    {
        for (std::list<Module::Ptr>::iterator l_It = m_Modules.begin(); l_It != m_Modules.end(); ++l_It)
        {
            if ((*l_It)->GetHash() == p_Module->GetHash())
                return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Register module
    void ModuleManager::RegisterModule(Module::Ptr p_Module)
    {
        if (HasModule(p_Module))
            return;

        m_Modules.push_back(p_Module);
    }

    //////////////////////////////////////////////////////////////////////////

    /// Get modules for a specific platform
    std::list<Module::Ptr> ModuleManager::GetPlatformModules(BNet2::Platforms p_Platform)
    {
        std::list<Module::Ptr> l_Result;

        for (std::list<Module::Ptr>::iterator l_It = m_Modules.begin(); l_It != m_Modules.end(); ++l_It)
        {
            if ((*l_It)->GetPlatform() == p_Platform)
                l_Result.push_back(*l_It);
        }

        return l_Result;
    }

}
