////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
//#include "PacketStructures.h"

uint8 gExpansionPerRace[] =
{
    MAX_EXPANSION,                      ///< RACE_NONE
    EXPANSION_VANILLA,                  ///< RACE_HUMAN
    EXPANSION_VANILLA,                  ///< RACE_ORC
    EXPANSION_VANILLA,                  ///< RACE_DWARF
    EXPANSION_VANILLA,                  ///< RACE_NIGHTELF
    EXPANSION_VANILLA,                  ///< RACE_UNDEAD_PLAYER
    EXPANSION_VANILLA,                  ///< RACE_TAUREN
    EXPANSION_VANILLA,                  ///< RACE_GNOME
    EXPANSION_VANILLA,                  ///< RACE_TROLL
    EXPANSION_CATACLYSM,                ///< RACE_GOBLIN
    EXPANSION_THE_BURNING_CRUSADE,      ///< RACE_BLOODELF
    EXPANSION_THE_BURNING_CRUSADE,      ///< RACE_DRAENEI
    MAX_EXPANSION,                      ///< RACE_FEL_ORC
    MAX_EXPANSION,                      ///< RACE_NAGA
    MAX_EXPANSION,                      ///< RACE_BROKEN
    MAX_EXPANSION,                      ///< RACE_SKELETON
    MAX_EXPANSION,                      ///< RACE_VRYKUL
    MAX_EXPANSION,                      ///< RACE_TUSKARR
    MAX_EXPANSION,                      ///< RACE_FOREST_TROLL
    MAX_EXPANSION,                      ///< RACE_TAUNKA
    MAX_EXPANSION,                      ///< RACE_NORTHREND_SKELETON
    MAX_EXPANSION,                      ///< RACE_ICE_TROLL
    EXPANSION_CATACLYSM,                ///< RACE_WORGEN
    MAX_EXPANSION,                      ///< RACE_GILNEAN
    EXPANSION_MISTS_OF_PANDARIA,        ///< RACE_PANDAREN_NEUTRAL
    EXPANSION_MISTS_OF_PANDARIA,        ///< RACE_PANDAREN_ALLI
    EXPANSION_MISTS_OF_PANDARIA,        ///< RACE_PANDAREN_HORDE
};

uint8 gExpansionPerClass[] =
{
    MAX_EXPANSION,                      ///< CLASS_NONE
    EXPANSION_VANILLA,                  ///< CLASS_WARRIOR
    EXPANSION_VANILLA,                  ///< CLASS_PALADIN
    EXPANSION_VANILLA,                  ///< CLASS_HUNTER
    EXPANSION_VANILLA,                  ///< CLASS_ROGUE
    EXPANSION_VANILLA,                  ///< CLASS_PRIEST
    EXPANSION_WRATH_OF_THE_LICH_KING,   ///< CLASS_DEATH_KNIGHT
    EXPANSION_VANILLA,                  ///< CLASS_SHAMAN
    EXPANSION_VANILLA,                  ///< CLASS_MAGE
    EXPANSION_VANILLA,                  ///< CLASS_WARLOCK
    EXPANSION_MISTS_OF_PANDARIA,        ///< CLASS_MONK
    EXPANSION_VANILLA,                  ///< CLASS_DRUID
};

void WorldSession::SendAuthResponse(uint8 p_AuthResult, bool p_Queued, uint32 p_QueuePosition)
{
    WorldPacket l_Data(SMSG_AUTH_RESPONSE);

    CharacterTemplates const& l_CharacterTemplates = sObjectMgr->GetCharacterTemplates();
    bool l_PremadeAvailable = sWorld->getBoolConfig(CONFIG_TEMPLATES_ENABLED);

    uint32 l_PremadeCount               = l_PremadeAvailable ? l_CharacterTemplates.size() : 0;
    uint32 l_VirtualRealmsCount         = 1;
    uint32 l_TimeRemain                 = 0;
    uint32 l_TimeOptions                = 0;
    uint32 l_TimeRested                 = 0;
    uint32 l_RealmRaceCount             = 0;
    uint32 l_RealmClassCount            = 0;
    uint32 l_TimeSecondsUntilPCKick     = 0;
    uint32 l_CurrencyID                 = 0;

    uint16 l_NumPlayersHorde    = 0;
    uint16 l_NumPlayersAlliance = 0;

    bool l_IsExpansionTrial          = false;
    bool l_ForceCharacterTemplate    = false;
    bool l_IsVeteranTrial            = false;
    bool l_HasFreeCharacterMigration = false;

    for (uint32 l_I = 0; l_I < sizeof(gExpansionPerRace); ++l_I)
    {
        if (gExpansionPerRace[l_I] != MAX_EXPANSION)
            l_RealmRaceCount++;
    }

    for (uint32 l_I = 0; l_I < sizeof(gExpansionPerClass); ++l_I)
    {
        if (gExpansionPerClass[l_I] != MAX_EXPANSION)
            l_RealmClassCount++;
    }

    l_Data << uint8(p_AuthResult);                                          ///< Result
    l_Data.WriteBit(p_AuthResult == AUTH_OK);                               ///< Has Success Info
    l_Data.WriteBit(p_Queued);                                              ///< Has Wait Info
    l_Data.FlushBits();

    if (p_AuthResult == AUTH_OK)
    {
        l_Data << uint32(g_RealmID);                                        ///< Virtual Realm Address
        l_Data << uint32(l_VirtualRealmsCount);                             ///< Virtual Realms Count
        l_Data << uint32(l_TimeRemain);                                     ///< TimeRemain
        l_Data << uint32(l_TimeOptions);                                    ///< TimeOptions
        l_Data << uint32(l_TimeRested);                                     ///< TimeRested
        l_Data << uint8(Expansion());                                       ///< Active Expansion Level
        l_Data << uint8(Expansion());                                       ///< Account Expansion Level
        l_Data << uint32(l_TimeSecondsUntilPCKick);                         ///< Time Seconds Until PC Kick
        l_Data << uint32(l_RealmRaceCount);                                 ///< Available Classes
        l_Data << uint32(l_RealmClassCount);                                ///< Available Races
        l_Data << uint32(l_PremadeCount);                                   ///< Templates Count
        l_Data << uint32(l_CurrencyID);                                     ///< Currency ID

        for (uint32 l_I = 0; l_I < l_VirtualRealmsCount; ++l_I)
        {
            l_Data << uint32(g_RealmID);                                    ///< Realm Address

            l_Data.WriteBit(true);                                          ///< Is Locale
            l_Data.WriteBit(false);                                         ///< Is Internal
            l_Data.WriteBits(sWorld->GetRealmName().size(), 8);             ///< Realm Name Actual
#ifndef CROSS
            l_Data.WriteBits(sWorld->GetNormalizedRealmName().size(), 8);   ///< Realm Name Normalized
#else /* CROSS */
            l_Data.WriteBits(sWorld->GetRealmName().size(), 8);             ///< Realm Name Normalized
#endif /* CROSS */
            l_Data.FlushBits();

            l_Data.WriteString(sWorld->GetRealmName());                     ///< Realm Name Actual
#ifndef CROSS
            l_Data.WriteString(sWorld->GetNormalizedRealmName());           ///< Realm Name Normalized
#else /* CROSS */
            l_Data.WriteString(sWorld->GetRealmName());                     ///< Realm Name Normalized
#endif /* CROSS */
        }

        for (uint32 l_I = 0; l_I < sizeof(gExpansionPerRace); ++l_I)
        {
            if (gExpansionPerRace[l_I] != MAX_EXPANSION)
            {
                l_Data << uint8(l_I);                                       ///< Race Or Class ID
                l_Data << uint8(gExpansionPerRace[l_I]);                    ///< Required Expansion
            }
        }

        for (uint32 l_I = 0; l_I < sizeof(gExpansionPerClass); ++l_I)
        {
            if (gExpansionPerClass[l_I] != MAX_EXPANSION)
            {
                l_Data << uint8(l_I);                                       ///< Race Or Class ID
                l_Data << uint8(gExpansionPerClass[l_I]);                   ///< Required Expansion
            }
        }

        for (auto& l_Itr : l_CharacterTemplates)
        {
            if (!l_PremadeAvailable)
                break;

            CharacterTemplate const* l_Tempalte = l_Itr.second;
            l_Data << uint32(l_Tempalte->m_ID);                             ///< TemplateSetID
            l_Data << uint32(2);                                            ///< Classes Count

            // Loop here
            // 3/5 = ally/horde
            {
                l_Data << uint8(l_Tempalte->m_PlayerClass);                 ///< Class ID
                l_Data << uint8(3);                                         ///< Faction Group
                l_Data << uint8(l_Tempalte->m_PlayerClass);                 ///< Class ID
                l_Data << uint8(5);                                         ///< Faction Group
            }

            l_Data.WriteBits(l_Tempalte->m_Name.size(), 7);                 ///< Name
            l_Data.WriteBits(l_Tempalte->m_Description.size(), 10);         ///< Description
            l_Data.FlushBits();

            l_Data.WriteString(l_Tempalte->m_Name);                         ///< Name
            l_Data.WriteString(l_Tempalte->m_Description);                  ///< Description
        }

        l_Data.WriteBit(l_IsExpansionTrial);                                ///< Is Expansion Trial
        l_Data.WriteBit(l_ForceCharacterTemplate);                          ///< Force Character Template
        l_Data.WriteBit(l_NumPlayersHorde != 0);                            ///< Has Num Players Horde
        l_Data.WriteBit(l_NumPlayersAlliance != 0);                         ///< Has Num Players Alliance
        l_Data.WriteBit(l_IsVeteranTrial);                                  ///< Is Veteran Trial
        l_Data.FlushBits();

        if (l_NumPlayersHorde)
            l_Data << uint16(l_NumPlayersHorde);                            ///< Num Players Horde

        if (l_NumPlayersAlliance)
            l_Data << uint16(l_NumPlayersAlliance);                         ///< Num Players Alliance
    }

    if (p_Queued)
    {
        l_Data << uint32(p_QueuePosition);                                  ///< Wait Count
        l_Data.WriteBit(l_HasFreeCharacterMigration);                       ///< Has Free Character Migration
        l_Data.FlushBits();
    }

    SendPacket(&l_Data);
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}
