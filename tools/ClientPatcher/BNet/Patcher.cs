using System;
using System.IO;
using ClientPatcher.Constants;

namespace ClientPatcher
{
    class Patcher : IDisposable
    {
        public string Binary { get; set; }
        public bool Initialized { get; private set; }
        public BinaryTypes Type { get; private set; }

        public byte[] binary;
        bool success;

        public Patcher(string p_File)
        {
            Initialized = false;
            success = false;

            using (var stream = new MemoryStream(File.ReadAllBytes(p_File)))
            {
                Binary = p_File;
                binary = stream.ToArray();

                if (binary != null)
                {
                    Type = Helper.GetBinaryType(binary);

                    Initialized = true;
                }
            }
        }

        public void Patch(byte[] p_Bytes, byte[] p_Pattern, long p_Address = 0)
        {
            if (Initialized && (p_Address != 0 || binary.Length >= p_Pattern.Length))
            {
                var p_Offset = p_Pattern == null ? p_Address : SearchOffset(p_Pattern);

                if (p_Offset != 0 && binary.Length >= p_Bytes.Length)
                {
                    try
                    {
                        for (int i = 0; i < p_Bytes.Length; i++)
                            binary[p_Offset + i] = p_Bytes[i];
                    }
                    catch (Exception ex)
                    {
                        throw new NotSupportedException(ex.Message);
                    }
                }
            }
        }

        long SearchOffset(byte[] pattern)
        {
            var matches = 0;

            for (long i = 0; i < binary.Length; i++)
            {
                matches = 0;

                for (int j = 0; j < pattern.Length; j++)
                {
                    if (pattern.Length > (binary.Length - i))
                        return 0;

                    if (pattern[j] == 0)
                    {
                        matches++;
                        continue;
                    }

                    if (binary[i + j] != pattern[j])
                        break;

                    matches++;
                }

                if (matches == pattern.Length)
                    return i;
            }

            return 0;
        }

        public void Finish()
        {
            success = true;
        }

        public void Dispose()
        {
            if (File.Exists(Binary))
                File.Delete(Binary);

            if (success)
                File.WriteAllBytes(Binary, binary);

            binary = null;
        }
    }
}
