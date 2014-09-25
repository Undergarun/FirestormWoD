#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
                               
const uint8_t config_data[] = "Garrosh.wtf";
const uint8_t builddate_data[] = " [Garrosh]";
const uint8_t bnet_portal_data[] = "garrosh";

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
    const char* Name;
    uint32_t ClientType;
    size_t PatchCount;
    const struct RewriteItem* Patchs;
};

enum ClientType
{
    CLIENT_TYPE_NONE  = 0000000000,
    CLIENT_TYPE_WIN32 = 0x0000014C,
    CLIENT_TYPE_WIN64 = 0x00008664,
    CLIENT_TYPE_MAC32 = 0xFEEDFACE,
    CLIENT_TYPE_MAC64 = 0xFEEDFACF
};

const uint8_t win32_packet_disable_filter_jam_dispatch[] =  {   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


const uint8_t win32_packet_sending_routage[] = { 0xBB, 0x00, 0x00, 0x00, 0x00 };                           ///< mov     ebx, 0

const struct RewriteItem Win32Patchs[] = {
    { 0x0000000, sizeof(win32_packet_disable_filter_jam_dispatch),  win32_packet_disable_filter_jam_dispatch    },   ///< Disable packet filter
   // { 0x038BC49, sizeof(win32_packet_sending_routage),              win32_packet_sending_routage                },   ///< Don't use special routage for packets
    { 0x0A30C98, sizeof(config_data),                               config_data                                 },   ///< Change default configuration file
    { 0x0A54DAC, sizeof(builddate_data),                            builddate_data                              },   ///< Change build date
    { 0x0AF427C, sizeof(bnet_portal_data),                          bnet_portal_data                            },   ///< Change bnet portal
};

const uint8_t mac64_packet_disable_filter_date[]  = {0x8B, 0x55, 0x0C, 0xE9, 0x41, 0x01, 0x00, 0x00}; // mov edx, [ebp+arg_4];  jmp 141h
const uint8_t mac64_packet_routage_data[]         = {0x33, 0xFF, 0xEB, 0x7A, 0x90, 0x90};             // xor edi, edi;  jmp short 7Ah

const struct RewriteItem Mac64Patchs[] = {
    { 0x0000000, sizeof(mac64_packet_disable_filter_date),          mac64_packet_disable_filter_date            },   ///< Disable packet filter
    { 0x0000000, sizeof(mac64_packet_routage_data),                 mac64_packet_routage_data                   },   ///< Don't use special routage for packets
    { 0x0000000, sizeof(config_data),                               config_data                                 },   ///< Change default configuration file
    { 0x0000000, sizeof(builddate_data),                            builddate_data                              },   ///< Change build date
    { 0x0000000, sizeof(bnet_portal_data),                          bnet_portal_data                            },   ///< Change bnet portal
};

const struct SystemItem gSystems[] = {
    {"win32",   CLIENT_TYPE_WIN32, _countof(Win32Patchs),  Win32Patchs},
    {"mac",     CLIENT_TYPE_MAC64, _countof(Mac64Patchs),  Mac64Patchs},
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
