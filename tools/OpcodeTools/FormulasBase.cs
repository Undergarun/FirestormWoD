using System;
namespace OpcodeTools
{
    public abstract class FormulasBase
    {
        public abstract uint CalcCryptedFromOpcode(uint opcode);
        public abstract uint CalcSpecialFromOpcode(uint opcode);
        public abstract uint CalcAuthFromOpcode(uint opcode);
        public abstract uint CalcGuildFromOpcode(uint opcode);
        public abstract uint CalcMovementFromOpcode(uint opcode);
        public abstract uint CalcQuestFromOpcode(uint opcode);
        protected abstract bool NormalCheck(uint opcode);
        protected abstract bool SpecialCheck(uint opcode);
        public abstract bool IsSpecialSpellOpcode(uint opcode);
        public abstract bool IsSpecialGuildOpcode(uint opcode);
        public abstract bool IsSpecialMovementOpcode(uint opcode);
        public abstract bool IsSpecialQuestOpcode(uint opcode);
        protected abstract bool AuthCheck(uint opcode);
        protected virtual uint BaseOffset { get { return 1376; } }

        public abstract uint CalcSpellFromOpcode(uint opcode);

        public bool IsAuthOpcode(uint opcode)
        {
            return AuthCheck(opcode);
        }

        public bool IsNormalOpcode(uint opcode)
        {
            return !IsSpecialOpcode(opcode) && !IsAuthOpcode(opcode) && NormalCheck(opcode);
        }
        
        public bool IsSpecialOpcode(uint opcode)
        {
            return !IsAuthOpcode(opcode) && SpecialCheck(opcode);
        }

        public uint CalcOffsetFromOpcode(uint opcode)
        {
            uint crypted = CalcCryptedFromOpcode(opcode);
            return (crypted * 4) + BaseOffset;
        }

        public uint CalcOpcodeFromSpecial(uint offset)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsSpecialOpcode(i))
                {
                    if (CalcSpecialFromOpcode(i) == offset)
                        return i;
                }
            }
            return 0;
        }

        public uint CalcOpcodeFromSpell(uint spell)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsSpecialSpellOpcode(i))
                {
                    if (CalcSpellFromOpcode(i) == spell)
                        return i;
                }
            }

            return 0;
        }

        public uint CalcOpcodeFromMovement(uint mouvement)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsSpecialMovementOpcode(i))
                {
                    if (CalcMovementFromOpcode(i) == mouvement)
                        return i;
                }
            }
            return 0;
        }

        public uint CalcOpcodeFromQuest(uint quest)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsSpecialQuestOpcode(i))
                {
                    if (CalcQuestFromOpcode(i) == quest)
                        return i;
                }
            }
            return 0;
        }

        /*
        public uint CalcOpcodeFromQuest(uint mouvement)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsSpecialQuestOpcode(i))
                {
                    if (CalcQuestFromOpcode(i) == mouvement)
                        return i;
                }
            }
            return 0;
        }*/

        public uint CalcOpcodeFromGuild(uint guild)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsSpecialGuildOpcode(i))
                {
                    if (CalcGuildFromOpcode(i) == guild)
                        return i;
                }
            }

            return 0;
        }

        public uint CalcOpcodeFromOffset(uint offset)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsNormalOpcode(i))
                {
                    if (CalcOffsetFromOpcode(i) == offset)
                        return i;
                }
            }
            return 0;
        }

        public uint CalcOpcodeFromAuth(uint auth)
        {
            for (uint i = 1; i < 0xFFFF; ++i)
            {
                if (IsAuthOpcode(i) &&
                    CalcAuthFromOpcode(i) == auth)
                {
                    return i;
                }
            }
            return 0;
        }
    }
}
