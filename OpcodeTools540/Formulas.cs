using System;
using System.Collections.Generic;
using System.Text;

namespace OpcodeTools
{
    /*public class Windows406 : FormulasBase
    {
        public override string ToString()
        {
            return "4.0.6 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x3FFD) == 8217;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0xB2AD) == 12;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x2093) == 8320 && opcode != 60096 && opcode != 44964;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            uint a1 = opcode;
            return (a1 & 0xC | ((a1 & 0x60 | ((a1 & 0x1F00 | (a1 >> 1) & 0x6000) >> 1)) >> 1)) >> 2;
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return (a4 & 2 | ((a4 & 0x10 | ((a4 & 0x40 | ((a4 & 0x100 | ((a4 & 0xC00 | (a4 >> 2) & 0x1000) >> 1)) >> 1)) >> 1)) >> 2)) >> 1;
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            uint a1 = opcode;
            return (a1 & 2 | (a1 >> 12) & 0xC) >> 1;
        }
    }

    public class Mac406 : FormulasBase
    {
        public override string ToString()
        {
            return "4.0.6.13623 Mac";
        }

        protected override uint BaseOffset { get { return 1380; } }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x3FFD) == 8217;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0xB2AD) == 12;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x2093) == 8320 && opcode != 60096 && opcode != 44964;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return ((opcode & 0xC000u) >> 5) | ((opcode & 0x1F00u) >> 4) | ((opcode & 0x60u) >> 3) | ((opcode & 0xCu) >> 2);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return ((opcode & 0x4000) >> 8) | ((opcode & 0xC00) >> 6) | ((opcode & 0x100) >> 5) | ((opcode & 0x40) >> 4) | ((opcode & 0x10) >> 3) | ((opcode & 2) >> 1);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return ((opcode & 0xC000) >> 13) | ((opcode & 2) >> 1);
        }
    }

    public class Mac13860 : FormulasBase
    {
        public override string ToString()
        {
            return "0.4.1.0.13860 Mac";
        }

        protected override uint BaseOffset { get { return 1380; } }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0xFCFB) == 1216;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x467C) == 1544;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x4C9) == 1088 && opcode != 24176 && opcode != 23636;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            uint v3 = opcode;
            return (((v3 & 0xF800u) >> 5) | ((v3 & 0x300u) >> 4) | ((v3 & 6u) >> 1) | ((v3 & 0x30u) >> 2));
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return ((a4 & 0x8000) >> 8) | a4 & 3 | ((a4 & 0x3800) >> 7) | ((a4 & 0x180) >> 5);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return ((a4 & 4) >> 2) | ((a4 & 0x300) >> 7);
        }
    }

    public class Windows13875 : FormulasBase
    {
        public override string ToString()
        {
            return "0.4.1.0.13875 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0xDFF6) == 546;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0xD57) == 1346;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x8AA) == 2088 && opcode != 52776 && opcode != 7784;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            uint v6 = opcode;
            return v6 & 1 | ((v6 & 4 | ((v6 & 0x10 | ((v6 & 0x40 | ((v6 & 0x700 | (v6 >> 1) & 0x7800) >> 1)) >> 1)) >> 1)) >> 1);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return (a4 & 8 | ((a4 & 0x20 | (((a4 & 0x80) | ((a4 & 0x200 | (a4 >> 2) & 0x3C00) >> 1)) >> 1)) >> 1)) >> 3;
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return a4 & 1 | ((a4 & 8 | (a4 >> 9) & 0x10) >> 2);
        }
    }

    public class Mac13875 : FormulasBase
    {
        public override string ToString()
        {
            return "0.4.1.0.13875 Mac";
        }

        protected override uint BaseOffset { get { return 1380; } }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0xDFF6) == 546;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0xD57) == 1346;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x8AA) == 2088 && opcode != 52776 && opcode != 7784;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            uint v3 = opcode;
            return ((v3 & 0xF000u) >> 5) | ((v3 & 0x700u) >> 4) | ((v3 & 0x40u) >> 3) | v3 & 1 | ((v3 & 0x10u) >> 2) | ((v3 & 4u) >> 1);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return ((a4 & 0xF000) >> 8) | ((a4 & 0x200) >> 6) | ((a4 & 0x80) >> 5) | ((a4 & 8) >> 3) | ((a4 & 0x20) >> 4);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return (a4 & 1 | ((a4 & 0x2000) >> 11) | ((a4 & 8) >> 2));
        }
    }

    public class Mac410 : FormulasBase
    {
        public override string ToString()
        {
            return "4.1.0.13914 Mac";
        }

        protected override uint BaseOffset { get { return 1380; } }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x7F7D) == 2568;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x942F) == 1037;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x8AC) == 2092 && opcode != 44607 && opcode != 18796;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            uint v3 = opcode;
            return ((v3 & 0xF000u) >> 5) | ((v3 & 0x700u) >> 4) | v3 & 3 | ((v3 & 0x40u) >> 3) | ((v3 & 0x10u) >> 2);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return ((a4 & 0x6000) >> 7) | ((a4 & 0x800) >> 6) | ((a4 & 0x10) >> 4) | ((a4 & 0x3C0) >> 5);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return ((a4 & 0x8000) >> 13) | ((a4 & 2) >> 1) | ((a4 & 0x80) >> 6);
        }
    }

    public class Windows410 : FormulasBase
    {
        public override string ToString()
        {
            return "4.1.0.13914 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x7F7D) == 2568;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x942F) == 1037;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x8AC) == 2092 && opcode != 44607 && opcode != 18796;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            uint v6 = opcode;
            return v6 & 3 | ((v6 & 0x10 | ((v6 & 0x40 | ((v6 & 0x700 | (v6 >> 1) & 0x7800) >> 1)) >> 1)) >> 2);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return (a4 & 0x10 | ((a4 & 0x3C0 | ((a4 & 0x800 | (a4 >> 1) & 0x3000) >> 1)) >> 1)) >> 4;
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            uint a4 = opcode;
            return (a4 & 2 | (((a4 & 0x80) | (a4 >> 7) & 0x100) >> 5)) >> 1;
        }
    }

    public class Windows420 : FormulasBase
    {
        public override string ToString()
        {
            return "4.2.0.14333 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x777F) == 1040;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x2399) == 769;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x2322) == 8738 && opcode != 57919 && opcode != 26159;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return opcode & 1 | ((opcode & 0x1C | (((opcode & 0xC0) | ((opcode & 0x1C00 | (opcode >> 1) & 0x6000) >> 2)) >> 1)) >> 1);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return (opcode & 6 | ((opcode & 0x60 | ((opcode & 0x1C00 | (opcode >> 1) & 0x6000) >> 3)) >> 2)) >> 1;
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return (opcode & 0x80 | ((opcode & 0x800 | (opcode >> 3) & 0x1000) >> 3)) >> 7;
        }
    }

    public class Mac420 : FormulasBase
    {
        public override string ToString()
        {
            return "4.2.0.14333 Mac";
        }

        protected override uint BaseOffset { get { return 1380; } }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x777F) == 1040;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x2399) == 769;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x2322) == 8738 && opcode != 57919 && opcode != 26159;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return ((opcode & 0xC000u) >> 5) | ((opcode & 0x1C00u) >> 4) | opcode & 1 | ((opcode & 0xC0u) >> 2) | ((opcode & 0x1Cu) >> 1);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return ((opcode & 0xC000) >> 7) | ((opcode & 0x1C00) >> 6) | ((opcode & 6) >> 1) | ((opcode & 0x60) >> 3);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return ((opcode & 0x8000) >> 13) | ((opcode & 0x80) >> 7) | ((opcode & 0x800) >> 10);
        }
    }

    public class Windows430 : FormulasBase
    {
        public override string ToString()
        {
            return "4.3.0.15005 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0xEBDE) == 770;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x9549) == 1032;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x8159) == 0;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return (opcode & 6 | ((opcode & 0x20 | (((opcode & 0x80) | (opcode >> 1) & 0x3F00) >> 1)) >> 2)) >> 1;
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return (opcode & 6 | ((opcode & 0x30 | (((opcode & 0x80) | ((opcode & 0x200 | ((opcode & 0x800 | (opcode >> 1) & 0x3000) >> 1)) >> 1)) >> 1)) >> 1)) >> 1;
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return opcode & 1 | ((opcode & 0x20 | ((opcode & 0x400 | (opcode >> 1) & 0x800) >> 4)) >> 4);
        }
    }

    public class Mac430 : FormulasBase
    {
        public override string ToString()
        {
            return "4.3.0.15005 Mac";
        }

        protected override uint BaseOffset { get { return 1380; } }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0xEBDE) == 770;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x9549) == 1032;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x8159) == 0;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return ((opcode & 0x7E00u) >> 5) | ((opcode & 0x80u) >> 4) | ((opcode & 6u) >> 1) | ((opcode & 0x20u) >> 3);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return ((opcode & 0x6000) >> 6) | ((opcode & 0x800) >> 5) | ((opcode & 0x200) >> 4) | ((opcode & 0x80) >> 3) | ((opcode & 6) >> 1) | ((opcode & 0x30) >> 2);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return ((opcode & 0x1000) >> 9) | opcode & 1 | ((opcode & 0x400) >> 8) | ((opcode & 0x20) >> 4);
        }
    }*/

    public class Windows540 : FormulasBase
    {
        public override string ToString()
        {
            return "5.4.0 17399 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0x16D6) == 66;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x1240) == 0;
        }

        public override bool IsSpecialSpellOpcode(uint opcode)
        {
            return (opcode & 0x1240) != 0 && (opcode & 0x1248) == 4096;
        }

        public override bool IsSpecialGuildOpcode(uint opcode)
        {
            return (opcode & 0x1240) != 0 && (opcode & 0x16C2) == 1090;
        }

        public override bool IsSpecialMovementOpcode(uint opcode)
        {
            return (opcode & 0x1240) != 0 && (opcode & 0x1202) == 4610;
        }

        public override bool IsSpecialQuestOpcode(uint opcode)
        {
            return (opcode & 0x1240) != 0 && (opcode & 0x1248) == 4104;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x90CC) == 4;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return opcode & 1 | ((opcode & 0x1FC | ((opcode & 0xC00 | (opcode >> 1) & 0x7000) >> 1)) >> 1);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return (opcode & 0x180 | ((opcode & 0xC00 | (opcode >> 1) & 0x7000) >> 1)) >> 1 | opcode & 0x3F;
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return opcode & 1 | ((opcode & 8 | ((opcode & 0x20 | ((opcode & 0x100 | ((opcode & 0x800 | (opcode >> 1) & 0x7000) >> 2)) >> 2)) >> 1)) >> 2);
        }

        public override uint CalcSpellFromOpcode(uint opcode)
        {
            return opcode & 7 | ((opcode & 0x30 | ((opcode & 0x180 | ((opcode & 0xC00 | (opcode >> 1) & 0x7000) >> 1)) >> 1)) >> 1);
        }

        public override uint CalcGuildFromOpcode(uint opcode)
        {
            return opcode & 1 | ((opcode & 0x3C | ((opcode & 0x100 | ((opcode & 0x800 | (opcode >> 1) & 0x7000) >> 2)) >> 2)) >> 1);
        }

        public override uint CalcMovementFromOpcode(uint opcode)
        {
            return opcode & 1 | ((opcode & 0x1FC | ((opcode & 0xC00 | (opcode >> 1) & 0x7000) >> 1)) >> 1);
        }

        public override uint CalcQuestFromOpcode(uint opcode)
        {
            return opcode & 7 | ((opcode & 0x30 | ((opcode & 0x180 | ((opcode & 0xC00 | (opcode >> 1) & 0x7000) >> 1)) >> 1)) >> 1);
        }
    }

    /*public class Windows434 : FormulasBase
    {
        public override string ToString()
        {
            return "4.3.4.15595 Windows";
        }

        protected override bool AuthCheck(uint opcode)
        {
            return (opcode & 0xB3FD) == 320;
        }

        protected override bool SpecialCheck(uint opcode)
        {
            return (opcode & 0x92E8) == 4256;
        }

        protected override bool NormalCheck(uint opcode)
        {
            return (opcode & 0x90CC) == 4;
        }

        public override uint CalcCryptedFromOpcode(uint opcode)
        {
            return (opcode & 3) | ((opcode & 0x30 | ((opcode & 0xF00 | (opcode >> 1) & 0x3000) >> 2)) >> 2);
        }

        public override uint CalcSpecialFromOpcode(uint opcode)
        {
            return (opcode & 7) | ((opcode & 0x10 | ((opcode & 0x100 | ((opcode & 0xC00 | (opcode >> 1) & 0x3000) >> 1)) >> 3)) >> 1);
        }

        public override uint CalcAuthFromOpcode(uint opcode)
        {
            return (opcode & 2 | ((opcode & 0xC00 | (opcode >> 2) & 0x1000) >> 8)) >> 1;
        }
    }*/
}
