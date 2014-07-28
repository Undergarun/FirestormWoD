﻿namespace ClientPatcher.Constants
{
    enum BinaryTypes : uint
    {
        None   = 0000000000,
        Pe32   = 0x0000014C,
        Pe64   = 0x00008664,
        Mach32 = 0xFEEDFACE,
        Mach64 = 0xFEEDFACF
    }
}
