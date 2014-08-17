/// Credit merydwin@gmail.com
#pragma once

#include <string>
#include <inttypes.h>
#include <list>
#include <memory>

#include "Cryptography/SHA1.h"
#include "AuthComponent.hpp"

namespace BNet2 {

    /// Module type
    enum ModuleType
    {
        BATTLENET2_MODULE_TYPE_AUTH = 0x01,
    };

    class Session;
    class Packet;

    //// Module
    class Module
    {
        public:
            typedef std::shared_ptr<Module> Ptr;

        public:
            /// Constructor
            Module(const std::string p_Hash, ModuleType p_Type, Platforms p_Platform, uint32_t p_ModuleID);

            /// Get module ID
            uint32_t GetID();
            /// Get module hash
            std::string GetHash();
            /// Get module hash data
            uint8_t * GetHashData();
            /// Get module hash data size
            uint32_t GetHashDataSize();
            /// Platform
            Platforms GetPlatform();
            /// Get type
            ModuleType GetType();
            /// Get type str
            std::string GetTypeStr();
            /// Get module data
            virtual uint8_t * GetData() = 0;
            /// Get data size
            virtual uint32_t GetSize(Session * p_Session) = 0;
            /// Write module
            virtual void Write(Session * p_Session, Packet * p_Packet) = 0;

        private:
            std::string m_ModuleHash;                               ///< Module sha1 hash (using as id)
            uint8_t     m_ModuleHashData[SHA256_DIGEST_LENGTH];     ///< Module sha1 hash (using as id) data
            ModuleType  m_Type;                                     ///< Module type
            Platforms   m_Platform;                                 ///< Platform
            uint32_t    m_ID;                                       ///< Module common id for all platform

    };

    /// Module manager
    class ModuleManager
    {
        /// static instance (aka singleton)
        static ModuleManager m_Instance;

        public:
            /// Return unique instance of ModuleManager
            static ModuleManager * GetSingleton();

            /// Has module
            bool HasModule(Module::Ptr p_Module);

            /// Register module
            void RegisterModule(Module::Ptr p_Module);

            /// Get modules for a specific platform
            std::list<Module::Ptr> GetPlatformModules(BNet2::Platforms p_Platform);

        private:
            std::list<Module::Ptr> m_Modules;  ///< Registered modules

    };

}
