using System;

namespace WowPacketParser.Enums
{
    [Flags]
    public enum GroupTypeFlag
    {
        Normal            = 0x00,
        Battleground      = 0x01,
        Raid              = 0x02,
        Unk4              = 0x04,
        LookingForDungeon = 0x08,
        Unk10             = 0x10,
        Unk40             = 0x40
    }

    public enum MemberFlags
    {
        None        = 0x00,
        Assistant   = 0x01,
        Maintank    = 0x02,
        MainAssist  = 0x04
    }

    public enum MemberStatus
    {
        Offline = 0x00,
        Online  = 0x01,
        PvP     = 0x02,
        Dead    = 0x04,
        Ghost   = 0x08,
        PvPFFa  = 0x16,
        Unk3    = 0x32,
        Afk     = 0x64,
        Dnd     = 0x128
    }
}
