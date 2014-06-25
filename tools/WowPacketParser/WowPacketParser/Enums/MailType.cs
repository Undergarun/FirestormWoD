namespace WowPacketParser.Enums
{
    public enum MailType
    {
        Normal     = 0,
        Auction    = 2,
        Creature   = 3,
        GameObject = 4,
        Item       = 5
    }

    public enum MailFlags
    {
        None        = 0,
        Read        = 1,
        Returned    = 2,
        Copied      = 4,
        CodPayment  = 8,
        HasBody     = 16
    }
}
