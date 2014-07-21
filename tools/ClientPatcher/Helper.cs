using System;
using System.IO;
using System.Security.Cryptography;
using ClientPatcher.Constants;

namespace ClientPatcher
{
    class Helper
    {
        public static BinaryTypes GetBinaryType(byte[] p_Data)
        {
            BinaryTypes l_Type = 0u;

            using (var l_Reader = new BinaryReader(new MemoryStream(p_Data)))
            {
                var l_Magic = (uint)l_Reader.ReadUInt16();

                // Check MS-DOS magic
                if (l_Magic == 0x5A4D)
                {
                    l_Reader.BaseStream.Seek(0x3C, SeekOrigin.Begin);

                    var l_PEOffset = l_Reader.ReadUInt32();

                    l_Reader.BaseStream.Seek(l_PEOffset, SeekOrigin.Begin);

                    var l_PEMagic = l_Reader.ReadUInt32();

                    if (l_PEMagic != 0x4550)
                        throw new NotSupportedException("Not a PE file!");

                    l_Type = (BinaryTypes)l_Reader.ReadUInt16();
                }
                else
                {
                    l_Reader.BaseStream.Seek(0, SeekOrigin.Begin);

                    l_Type = (BinaryTypes)l_Reader.ReadUInt32();
                }
            }

            return l_Type;
        }

        public static string GetFileChecksum(byte[] p_Data)
        {
            using (var l_Stream = new BufferedStream(new MemoryStream(p_Data), 1200000))
            {
                var l_Sha256    = new SHA256Managed();
                var l_CheckSum  = l_Sha256.ComputeHash(l_Stream);

                return BitConverter.ToString(l_CheckSum).Replace("-", "").ToLower();
            }
        }
    }
}
