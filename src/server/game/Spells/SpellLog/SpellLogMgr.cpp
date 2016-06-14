////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "SpellLogMgr.h"
#include "Chat.h"

void SpellLogMgr::AddOrSetListener(Player* p_Player, uint32 p_SpellID)
{
    auto l_Listener = m_Listeners.find(p_Player);
    std::list<uint32>* l_List;

    if (l_Listener == m_Listeners.end())
    {
        auto l_Pair = m_Listeners.insert({p_Player, std::list<uint32> {}});
        l_List = &l_Pair.first->second;
    }
    else
        l_List = &l_Listener->second;

    if (!p_SpellID)
        return;

    l_List->push_back(p_SpellID);
}

void SpellLogMgr::RemoveListener(Player* p_Player)
{
    auto l_Listener = m_Listeners.find(p_Player);

    if (l_Listener != m_Listeners.end())
        m_Listeners.erase(l_Listener);
}

void SpellLogMgr::RemoveSpellFromListener(Player* p_Player, uint32 p_SpellID)
{
    auto l_Listener = m_Listeners.find(p_Player);

    if (l_Listener != m_Listeners.end())
    {
        auto l_It = std::find(l_Listener->second.begin(), l_Listener->second.end(), p_SpellID);
        if (l_It != l_Listener->second.end())
            l_Listener->second.erase(l_It);

        if (l_Listener->second.empty())
            m_Listeners.erase(l_Listener);
    }
}

void SpellLogMgr::LogSpell(Unit const* p_Caster, uint32 p_SpellID, const char* p_String, ...)
{
    if (m_Listeners.empty())
        return;

    char l_Buff[512];
    va_list l_Args;
    va_start(l_Args, p_String);
    vsprintf(l_Buff, p_String, l_Args);
    va_end(l_Args);

    uint32 l_Flags = sWorld->getIntConfig(CONFIG_SPELLOG_FLAGS);

    if (l_Flags & SPELLLOG_OUTPUT_FLAG_PLAYER)
    {
        for (auto& l_Listener : m_Listeners)
        {
            if (l_Listener.first == p_Caster)
            {
                if (std::find(l_Listener.second.begin(), l_Listener.second.end(), p_SpellID) != l_Listener.second.end())
                    ChatHandler(l_Listener.first).SendSysMessage(l_Buff);
            }
        }
    }

    if (l_Flags & SPELLLOG_OUTPUT_FLAG_CONSOLE)
    {
        bool l_Print = false;

        for (auto& l_Listener : m_Listeners)
        {
            for (auto& l_Entry : l_Listener.second)
            {
                if (l_Entry == p_SpellID)
                {
                    l_Print = true;
                    PrintToConsole(l_Buff);
                    break;
                }

                if (l_Print)
                    break;
            }

            if (l_Print)
                break;

        }
    }
}

void SpellLogMgr::LogSpell(Unit const* p_Caster, uint32 p_SpellID, uint32 p_SpellID2, const char* p_String, ...)
{
    if (m_Listeners.empty())
        return;

    char l_Buff[512];
    va_list l_Args;
    va_start(l_Args, p_String);
    vsprintf(l_Buff, p_String, l_Args);
    va_end(l_Args);

    uint32 l_Flags = sWorld->getIntConfig(CONFIG_SPELLOG_FLAGS);

    if (l_Flags & SPELLLOG_OUTPUT_FLAG_PLAYER)
    {
        for (auto& l_Listener : m_Listeners)
        {
            if (l_Listener.first == p_Caster)
            {
                if (std::find(l_Listener.second.begin(), l_Listener.second.end(), p_SpellID) != l_Listener.second.end() ||
                    std::find(l_Listener.second.begin(), l_Listener.second.end(), p_SpellID2) != l_Listener.second.end())
                    ChatHandler(l_Listener.first).SendSysMessage(l_Buff);
            }
        }
    }

    if (l_Flags & SPELLLOG_OUTPUT_FLAG_CONSOLE)
    {
        bool l_Print = false;

        for (auto& l_Listener : m_Listeners)
        {
            for (auto& l_Entry : l_Listener.second)
            {
                if (l_Entry == p_SpellID || l_Entry == p_SpellID2)
                {
                    l_Print = true;
                    PrintToConsole(l_Buff);
                    break;
                }

                if (l_Print)
                    break;
            }

            if (l_Print)
                break;
        }
    }
}

void SpellLogMgr::PrintToConsole(char* p_Str)
{
    printf(p_Str);
    printf("\n");
}
