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

const uint8_t jam_dispatch_check_data_win32[]    = { 0xEB, 0x1D };     ///< jmp to bypass socket check
const uint8_t jam_quest_check_data_win32[]       = { 0x90, 0x90 };     ///< jmp to bypass socket check
const uint8_t client_check_data_win32[]          = { 0xBB, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x22, 0x01, 0x00, 0x00 };     ///< jmp to bypass socket check

const uint8_t load_custom_data1[]                = { 0x00 };
const uint8_t load_custom_data2_win32[]          = { 0x75 };
const uint8_t load_custom_data3_win32[]          = { 0x75 };
const uint8_t load_custom_data4[]                = { 0x04 };
const uint8_t load_custom_data5[]                = { 0x75 };
const uint8_t addon[]                            = { 0x6A, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
const uint8_t addon2[]                           = { 0x02 };
const uint8_t addon3[]                           = { 0x00 };

const uint8_t navigatehome_to_navigateto_win32[] = { 0x83, 0xC4, 0x0C, 0x56, 0x8B, 0xCF, 0xE8, 0xE3, 0xFE, 0xFF, 0xFF, 0x90, 0xEB, 0x37 };

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
    { 0x000123B3, sizeof(load_custom_data1),                     load_custom_data1,                     },        ///< CASC - check local file
    { 0x000110B6, sizeof(load_custom_data2_win32),               load_custom_data2_win32,               },        ///< CASC - check local file part 2
    { 0x000123C0, sizeof(load_custom_data3_win32),               load_custom_data3_win32,               },        ///< CASC - check local file part 3
    { 0x0086CA12, sizeof(load_custom_data4),                     load_custom_data4,                     },        ///< Bypass signature check
    { 0x0039AE8E, sizeof(load_custom_data5),                     load_custom_data5,                     },        ///< Bypass .old  rename
    { 0x0003EE92, sizeof(navigatehome_to_navigateto_win32),      navigatehome_to_navigateto_win32,      },        ///< Replace lua funtion navigatehome(string index) to navigateto(string url)
    { 0x0039C5FF, sizeof(addon),                                 addon                                  },
    { 0x0039E0DE, sizeof(addon2),                                addon2                                 },
    { 0x0039E3EF, sizeof(addon2),                                addon2                                 },
    { 0x0039E41E, sizeof(addon2),                                addon2                                 },
    { 0x00407E6E, sizeof(addon3),                                addon3                                 },
    { 0x0007FEDE, sizeof(addon3),                                addon3                                 }   

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

const uint8_t load_custom_data2_win64[] =
{
    0x0F, 0x85                          ///< JNZ loc_14001AE3A
};

const uint8_t load_custom_data3_win64[] =
{
    0x0F, 0x85                          ///< JNZ loc_14001CDF2
};

const uint8_t navigatehome_to_navigateto_win64[]
{
    0x40, 0x53,                                  ///< push rbx
    0x41, 0x57,                                  ///< push r15
    0x57,                                        ///< push rdi
    0x48, 0x83, 0xEC, 0x20,                      ///< sub rsp,20
    0x48, 0x8B, 0xD9,                            ///< mov rbx,rcx
    0x4C, 0x8B, 0x9B, 0x60, 0x03, 0x00, 0x00,    ///< mov r11,[rbx+00000360]
    0x4D, 0x85, 0xDB,                            ///< test r11,r11
    0x74, 0x71,                                  ///< je "ashran64 - navigate.exe"+5F7E9
    0x4C, 0x8B, 0xFA,                            ///< mov r15,rdx
    0xFC,                                        ///< cld
    0x48, 0x83, 0xC9, 0xFF,                      ///< or rcx,-01
    0x31, 0xC0,                                  ///< xor eax,eax
    0x48, 0x8B, 0xFA,                            ///< mov rdi,rdx
    0xF2, 0xAE,                                  ///< repne scasb
    0x48, 0xF7, 0xD1,                            ///< not rcx
    0x48, 0xFF, 0xC9,                            ///< dec rcx
    0x4C, 0x8B, 0xC1,                            ///< mov r8,rcx
    0x49, 0x8B, 0xCB,                            ///< mov rcx,r11
    0xE8, 0xD8, 0x82, 0x18, 0x00,                ///< call "ashran64 - navigate.exe"+1E7A70
    0x49, 0x8B, 0xD7,                            ///< mov rdx,r15
    0x48, 0x83, 0xC9, 0xFF,                      ///< or rcx,-01
    0x31, 0xC0,                                  ///< xor eax,eax
    0x48, 0x8B, 0xFA,                            ///< mov rdi,rdx
    0xF2, 0xAE,                                  ///< repne scasb
    0x48, 0xF7, 0xD1,                            ///< not rcx
    0x48, 0xFF, 0xC9,                            ///< dec rcx
    0x4C, 0x8B, 0xC1,                            ///< mov r8,rcx
    0x48, 0x8D, 0x8B, 0x30, 0x03, 0x00, 0x00,    ///< lea rcx,[rbx+00000330]
    0xE8, 0x35, 0x83, 0xFA, 0xFF,                ///< call "ashran64 - navigate.exe"+7AF0
    0x48, 0x83, 0xC4, 0x20,                      ///< add rsp,20
    0x5F,                                        ///< pop rdi
    0x41, 0x5F,                                  ///< pop r15
    0x5B,                                        ///< pop rbx
    0xC3                                         ///< ret
};

const uint8_t navigatehome_2_win64[] =
{
    0x8B, 0xD0, 0x48, 0x8B, 0xCB, 0xE8, 0x36, 0xFE, 0xFF, 0xFF, 0xEB, 0x44
};

const uint8_t addon_1_win64[] = { 0x41, 0xb1, 0x02, 0x90 };
const uint8_t addon_2_win64[] = { 0x02 };
const uint8_t addon_3_win64[] = { 0x00 };
const uint8_t addon_4_win64[] = { 0x41, 0xb1, 0x00, 0x90 };
const uint8_t addon_5_win64[] = { 0x41, 0xb1, 0x00 };


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
    { 0x0001C0AE, sizeof(load_custom_data1),                     load_custom_data1,                     },
    { 0x0001A1A5, sizeof(load_custom_data2_win64),               load_custom_data2_win64                },
    { 0x0001C0AF, sizeof(load_custom_data3_win64),               load_custom_data3_win64,               },
    { 0x00D5FADB, sizeof(load_custom_data4),                     load_custom_data4,                     },        ///< Bypass signature check
    { 0x0060540D, sizeof(load_custom_data5),                     load_custom_data5,                     },        ///< Bypass .old  rename
    { 0x0005EB60, sizeof(navigatehome_to_navigateto_win64),      navigatehome_to_navigateto_win64       },
    { 0x0005ED20, sizeof(navigatehome_2_win64),                  navigatehome_2_win64                   },
    { 0x000418D2, sizeof(addon_1_win64),                         addon_1_win64                          },
    { 0x0060B49B, sizeof(addon_2_win64),                         addon_2_win64                          },
    { 0x0060BA93, sizeof(addon_2_win64),                         addon_2_win64                          },
    { 0x0060BAD4, sizeof(addon_2_win64),                         addon_2_win64                          },
    { 0x006FBF26, sizeof(addon_3_win64),                         addon_3_win64                          },
    { 0x0003AAFB, sizeof(addon_4_win64),                         addon_4_win64                          },
    { 0x0003DF39, sizeof(addon_4_win64),                         addon_4_win64                          },
    { 0x000C1069, sizeof(addon_5_win64),                         addon_5_win64                          }
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

const struct SystemItem gSystems[] =
{
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