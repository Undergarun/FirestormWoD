// ClientPatcher.cpp?: d?finit le point d'entr?e pour l'application console.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

const uint8_t config_data[]             = "Ashran.wtf";
const uint8_t build_date_data[]         = "[Ashran]";
const uint8_t bnet_portal_data[]        = "logon";
const uint8_t bnet_logon_data[]         = ".ashran.com";
const uint8_t bnet_agent_data[]         = "noagt.exe";
const uint8_t bnet_versions_data[]      = "http://%s-cdn612.ashran.com/%s/versions";
const uint8_t bnet_cdns_data[]          = "http://%s-cdn612.ashran.com/%s/cdns";

#ifndef _countof
#define _countof(V) (sizeof(V) / sizeof(V[0]))
#endif

struct RewriteItem
{
    intptr_t        Offset; ///< Patch offset 
    size_t          Size;   ///< Patch size
    const uint8_t*  Data;   ///< Patch bytes
};

struct SystemItem
{
    const char*                 Name;           ///< System name
    uint32_t                    ClientType;     ///< Client Type
    size_t                      PatchCount;     ///< Path count
    const struct RewriteItem*   Patchs;         ///< Patchs
};

enum ClientType
{
    CLIENT_TYPE_NONE    = 0000000000,
    CLIENT_TYPE_WIN32   = 0x0000014C,
    CLIENT_TYPE_WIN64   = 0x00008664,
    CLIENT_TYPE_MAC32   = 0xFEEDFACE,
    CLIENT_TYPE_MAC64   = 0xFEEDFACF
};
//#define BETA
#define LIVE

/// - 19116 beta
#ifdef BETA
const uint8_t jam_dispatch_check_data_win32[] = { 0xEB, 0x1D };     ///< jmp to bypass socket check
const uint8_t jam_quest_check_data_win32[]    = { 0xEB };           ///< jmp to bypass socket check
const uint8_t jam_spell_check_data_win32[]    = { 0xEB, 0x1D };     ///< jmp to bypass socket check
const uint8_t jam_guild_check_data_win32[]    = { 0xEB, 0x1B };     ///< jmp to bypass socket check

const struct RewriteItem Win32Patchs[] = 
{
    { 0x00A392C8, sizeof(config_data),                           config_data                            },        ///< Change default configuration file
    { 0x00A5D85C, sizeof(build_date_data),                       build_date_data                        },        ///< Change build date
    { 0x00AFD3C4, sizeof(bnet_portal_data),                      bnet_portal_data                       },        ///< Change bnet portal
    { 0x00B9DBE0, sizeof(bnet_logon_data),                       bnet_logon_data                        },        ///< Change bnet logon
    { 0x00A69234, sizeof(bnet_agent_data),                       bnet_agent_data                        },        ///< Change bnet agent
    { 0x00A38560, sizeof(bnet_versions_data),                    bnet_versions_data                     },        ///< Change bnet versions data
    { 0x00A3858C, sizeof(bnet_cdns_data),                        bnet_cdns_data                         },        ///< Change bnet cdns data
    { 0x002496D7, sizeof(jam_dispatch_check_data_win32),         jam_dispatch_check_data_win32          },        ///< Bypass jam dispatch socket check
    { 0x0023DB99, sizeof(jam_quest_check_data_win32),            jam_quest_check_data_win32             },        ///< bypass jam quest dispatch socket check
    { 0x0097C9D6, sizeof(jam_spell_check_data_win32),            jam_spell_check_data_win32             },        ///< bypass jam spell dispatch socket check
    { 0x0026C637, sizeof(jam_guild_check_data_win32),            jam_guild_check_data_win32             },        ///< bypass jam guild dispatch socket check
};

const uint8_t client_packets_check_data_mac64[] = { 0xB9, 0x00 };   ///< jmp to bypass socket check
const uint8_t jam_dispatch_check_data_mac64[]   = { 0xEB, 0x19 };   ///< jmp to bypass socket check
const uint8_t jam_quest_check_data_mac64[]      = { 0xEB, 0x53 };   ///< jmp to bypass socket check
const uint8_t jam_spell_check_data_mac64[]      = { 0xEB, 0x19 };   ///< jmp to bypass socket check
const uint8_t jam_guild_check_data_mac64[]      = { 0xEB, 0x58 };   ///< jmp to bypass socket check

const struct RewriteItem Mac64Patchs[] = 
{
    { 0x00000000, sizeof(config_data),                           config_data                       },             ///< Change default configuration file
    { 0x00000000, sizeof(build_date_data),                       build_date_data                   },             ///< Change build date
    { 0x00000000, sizeof(bnet_portal_data),                      bnet_portal_data                  },             ///< Change bnet portal
    { 0x00000000, sizeof(bnet_agent_data),                       bnet_agent_data                   },             ///< Change bnet agent
    { 0x00000000, sizeof(bnet_logon_data),                       bnet_logon_data                   },             ///< Change bnet logon
    { 0x00000000, sizeof(bnet_versions_data),                    bnet_versions_data                },             ///< Change bnet versions data
    { 0x00000000, sizeof(bnet_cdns_data),                        bnet_cdns_data                    },             ///< Change bnet cdns data
    { 0x00000000, sizeof(jam_dispatch_check_data_mac64),         jam_dispatch_check_data_mac64     },             ///< Bypass jam dispatch socket check
    { 0x00000000, sizeof(jam_quest_check_data_mac64),            jam_quest_check_data_mac64        },             ///< Bypass jam quest socket check
    { 0x00000000, sizeof(jam_spell_check_data_mac64),            jam_spell_check_data_mac64        },             ///< Bypass jam spell socket check
    { 0x00000000, sizeof(jam_guild_check_data_mac64),            jam_guild_check_data_mac64        },             ///< Bypass jam guild socket check
    { 0x00000000, sizeof(client_packets_check_data_mac64),       client_packets_check_data_mac64   },             ///< Bypass client send socket check
    { 0x00000000, sizeof(client_packets_check_data_mac64),       client_packets_check_data_mac64   },             ///< Bypass client send socket check
    { 0x00000000, sizeof(client_packets_check_data_mac64),       client_packets_check_data_mac64   },             ///< Bypass client send socket check
};

#endif

///-  19865 live
#ifdef LIVE

const uint8_t jam_dispatch_check_data_win32[] = { 0xEB, 0x1D };     ///< jmp to bypass socket check
const uint8_t jam_quest_check_data_win32[]    = { 0x90, 0x90 };     ///< jmp to bypass socket check
const uint8_t client_check_data_win32[]       = { 0xBB, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x22, 0x01, 0x00, 0x00 };     ///< jmp to bypass socket check
const uint8_t load_custom_data1_win32[]       = { 0x00 };
const uint8_t load_custom_data2_win32[]       = { 0x75 };
const uint8_t load_custom_data3_win32[]       = { 0x75 };
const uint8_t load_custom_data4_win32[]       = { 0x04 };
const uint8_t load_custom_data5_win32[]       = { 0x75 };

const uint8_t browser_account_id_win32[]      = { 0xEB, 0x0B};
const uint8_t browser_replace_url_win32[]     = { 0x02 };


const struct RewriteItem Win32Patchs[] =
{
    { 0x009A3128, sizeof(config_data),                           config_data                            },        ///< Change default configuration file
    { 0x009BA4F8, sizeof(build_date_data),                       build_date_data                        },        ///< Change build date
    { 0x00A4471C, sizeof(bnet_portal_data),                      bnet_portal_data                       },        ///< Change bnet portal
    { 0x00AD722C, sizeof(bnet_logon_data),                       bnet_logon_data                        },        ///< Change bnet logon
    { 0x009C2AF4, sizeof(bnet_agent_data),                       bnet_agent_data                        },        ///< Change bnet agent
    { 0x009A2470, sizeof(bnet_versions_data),                    bnet_versions_data                     },        ///< Change bnet versions data
    { 0x009A249C, sizeof(bnet_cdns_data),                        bnet_cdns_data                         },        ///< Change bnet cdns data
    { 0x0020D0A3, sizeof(jam_dispatch_check_data_win32),         jam_dispatch_check_data_win32          },        ///< Bypass jam dispatch socket check
    { 0x00202D1A, sizeof(jam_quest_check_data_win32),            jam_quest_check_data_win32             },        ///< Bypass jam quest dispatch socket check
    { 0x00264AEB, sizeof(client_check_data_win32),               client_check_data_win32                },        ///< Bypass client dispatch socket check
    { 0x000123B3, sizeof(load_custom_data1_win32),               load_custom_data1_win32,               },        ///< CASC - check local file
    { 0x000110B6, sizeof(load_custom_data2_win32),               load_custom_data2_win32,               },        ///< CASC - check local file part 2
    { 0x000123C0, sizeof(load_custom_data3_win32),               load_custom_data3_win32,               },        ///< CASC - check local file part 3
    { 0x0086CA12, sizeof(load_custom_data4_win32),               load_custom_data4_win32,               },        ///< Bypass signature check
    { 0x0039AE8E, sizeof(load_custom_data5_win32),               load_custom_data5_win32,               },        ///< Bypass .old  rename
    { 0x003A3023, sizeof(browser_account_id_win32),              browser_account_id_win32,              },        ///< Fill url with account id intead of account name
    { 0x0003EEA5, sizeof(browser_replace_url_win32),             browser_replace_url_win32,             },        ///< Use navigable url 2 instead 5 with account name fill
};

const uint8_t client_check_data_win64[] =
{
    0x41, 0xBE, 0x00, 0x00, 0x00, 0x00, ///< mov r14d, 0
    0xBF, 0x00, 0x00, 0x00, 0x00,       ///< mov edi, 0
    0xE9, 0x11, 0x01, 0x00, 0x00        ///< jmp loc_140408A3F
};

const uint8_t jam_quest_check_data_win64[] =
{
    0x90,                               ///< nop
    0x90                                ///< nop
};

const uint8_t jam_dispatch_check_data_win64[] =
{
    0xEB, 0x2B                          ///< jmp loc_14037F0E3
};

const struct RewriteItem Win64Patchs[] =
{
    { 0x00F09CC8, sizeof(config_data),                           config_data                            },        ///< Change default configuration file
    { 0x00F260E0, sizeof(build_date_data),                       build_date_data                        },        ///< Change build date
    { 0x00FC5BE0, sizeof(bnet_portal_data),                      bnet_portal_data                       },        ///< Change bnet portal
    { 0x01064728, sizeof(bnet_logon_data),                       bnet_logon_data                        },        ///< Change bnet logon
    { 0x00F2F210, sizeof(bnet_agent_data),                       bnet_agent_data                        },        ///< Change bnet agent
    { 0x00F08F68, sizeof(bnet_versions_data),                    bnet_versions_data                     },        ///< Change bnet versions data
    { 0x00F08F98, sizeof(bnet_cdns_data),                        bnet_cdns_data                         },        ///< Change bnet cdns data
    { 0x0037E4B6, sizeof(jam_dispatch_check_data_win64),         jam_dispatch_check_data_win64          },        ///< Bypass jam dispatch socket check
    { 0x0036E481, sizeof(jam_quest_check_data_win64),            jam_quest_check_data_win64             },        ///< bypass jam quest dispatch socket check
    { 0x00407D1E, sizeof(client_check_data_win64),               client_check_data_win64                },        ///< bypass client dispatch socket check
};

const uint8_t client_packets_check_data_mac64[] = { 0xBB, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x27, 0x01, 0x00, 0x00 };   ///< jmp to bypass socket check
const uint8_t jam_dispatch_check_data_mac64[]   = { 0xEB, 0x19 };                                                   ///< jmp to bypass socket check
const uint8_t jam_quest_check_data_mac64[]      = { 0xEB, 0x58 };                                                   ///< jmp to bypass socket check

const struct RewriteItem Mac64Patchs[] =
{
     { 0x014B276A, sizeof(config_data),                           config_data                       },             ///< Change default configuration file
     { 0x014727DB, sizeof(build_date_data),                       build_date_data                   },             ///< Change build date
     { 0x0149CC61, sizeof(bnet_portal_data),                      bnet_portal_data                  },             ///< Change bnet portal
     { 0x0149BEB7, sizeof(bnet_agent_data),                       bnet_agent_data                   },             ///< Change bnet agent
     { 0x015E93A0, sizeof(bnet_logon_data),                       bnet_logon_data                   },             ///< Change bnet logon
     { 0x014B1076, sizeof(bnet_versions_data),                    bnet_versions_data                },             ///< Change bnet versions data
     { 0x014B10A2, sizeof(bnet_cdns_data),                        bnet_cdns_data                    },             ///< Change bnet cdns data
     { 0x01094A26, sizeof(jam_dispatch_check_data_mac64),         jam_dispatch_check_data_mac64     },             ///< Bypass jam dispatch socket 
     { 0x010E8867, sizeof(jam_quest_check_data_mac64),            jam_quest_check_data_mac64        },             ///< Bypass jam dispatch socket check
     { 0x0064810C, sizeof(client_packets_check_data_mac64),       client_packets_check_data_mac64   },             ///< Bypass client send socket check
};
#endif

const struct SystemItem gSystems[] = {
    { "win32",  CLIENT_TYPE_WIN32,  _countof(Win32Patchs),  Win32Patchs },
    { "win64",  CLIENT_TYPE_WIN64,  _countof(Win64Patchs),  Win64Patchs },
    { "mac64",  CLIENT_TYPE_MAC64,  _countof(Mac64Patchs),  Mac64Patchs },
};

FILE* gFile = 0;

int main(int p_Argc, char* p_Argv[])
{
    const struct SystemItem     * l_SystemItems;
    const struct RewriteItem    * l_RewriteItem;

    gFile = fopen(p_Argv[1], "rb+");

    if (!gFile)
    {
        fprintf(stderr, "Can't open file.\n");
        return 1;
    }

    uint16_t l_MagicHeader = 0;

    if (!fread(&l_MagicHeader, sizeof(l_MagicHeader), 1, gFile))
    {
        fprintf(stderr, "Can't read file.\n");
        fclose(gFile);
        return 1;
    }

    uint32_t l_ClientType;

    if (l_MagicHeader == 0x5A4D)
    {
        uint32_t l_PEOffset = 0;
        uint32_t l_PEMagicHeader = 0;

        fseek(gFile, 0x3C, SEEK_SET);
        fread(&l_PEOffset, sizeof(l_PEOffset), 1, gFile);
        fseek(gFile, l_PEOffset, SEEK_SET);
        fread(&l_PEMagicHeader, sizeof(l_PEMagicHeader), 1, gFile);

        if (l_PEMagicHeader != 0x4550)
        {
            fprintf(stderr, "Not an valid windows executable.\n");
            fclose(gFile);
            return 1;
        }

        fread(&l_ClientType, sizeof(l_ClientType), 1, gFile);
        l_ClientType &= 0x0000FFFF;
    }
    else
    {
        fseek(gFile, 0, SEEK_SET);
        fread(&l_ClientType, sizeof(l_ClientType), 1, gFile);
    }

    for (l_SystemItems = gSystems; l_SystemItems < gSystems + _countof(gSystems); l_SystemItems += 1)
    {
        if (l_ClientType != l_SystemItems->ClientType)
            continue;

        printf("Convert %s executable.\n", l_SystemItems->Name);

        for (l_RewriteItem = l_SystemItems->Patchs; l_RewriteItem < l_SystemItems->Patchs + l_SystemItems->PatchCount; l_RewriteItem += 1)
        {
            if (l_RewriteItem->Offset == 0)
                continue;

            fseek(gFile, (long)l_RewriteItem->Offset, SEEK_SET);
            fwrite(l_RewriteItem->Data, 1, l_RewriteItem->Size, gFile);
        }

        break;
    }

    if (l_SystemItems == gSystems + _countof(gSystems))
    {
        fprintf(stderr, "Unknown signature %X.\n", l_MagicHeader);
        fclose(gFile);
        return 1;
    }

    fclose(gFile);

    printf("Executable patched with success !");

    return 0;
}