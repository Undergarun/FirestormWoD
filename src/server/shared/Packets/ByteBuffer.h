////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "Common.h"
#include "Debugging/Errors.h"
#include "Log.h"
#include "Utilities/ByteConverter.h"
#include "Guid.h"
#include <G3D/Vector2.h>
#include <G3D/Vector3.h>

//! Structure to ease conversions from single 64 bit integer guid into individual bytes, for packet sending purposes
//! Nuke this out when porting ObjectGuid from MaNGOS, but preserve the per-byte storage
struct ObjectGuid
{
    public:
        ObjectGuid() { _data.u64 = 0LL; }
        ObjectGuid(uint64 guid) { _data.u64 = guid; }
        ObjectGuid(ObjectGuid const& other) { _data.u64 = other._data.u64; }

        uint8& operator[](uint32 index)
        {
            ASSERT(index < sizeof(uint64));

#if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
            return _data.byte[index];
#else
            return _data.byte[7 - index];
#endif
        }

        uint8 const& operator[](uint32 index) const
        {
            ASSERT(index < sizeof(uint64));

#if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
            return _data.byte[index];
#else
            return _data.byte[7 - index];
#endif
        }

        operator uint64()
        {
            return _data.u64;
        }

        ObjectGuid& operator=(uint64 guid)
        {
            _data.u64 = guid;
            return *this;
        }

        ObjectGuid& operator=(ObjectGuid const& other)
        {
            _data.u64 = other._data.u64;
            return *this;
        }

        void Clear()
        {
            _data.u64 = 0LL;
        }

        bool IsEmpty() const
        {
            return bool(_data.u64);
        }

    private:
        union
        {
            uint64 u64;
            uint8 byte[8];
        } _data;
};

class ByteBufferException
{
    public:
        ByteBufferException(size_t pos, size_t size, size_t valueSize)
            : Pos(pos), Size(size), ValueSize(valueSize)
        {
        }

    protected:
        size_t Pos;
        size_t Size;
        size_t ValueSize;
};

class ByteBufferPositionException : public ByteBufferException
{
    public:
        ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize)
        : ByteBufferException(pos, size, valueSize), _add(add)
        {
            PrintError();
        }

    protected:
        void PrintError() const
        {
#ifdef DEBUG
            ACE_Stack_Trace trace;

            sLog->outError(LOG_FILTER_GENERAL, "Attempted to %s value with size: " SIZEFMTD " in ByteBuffer (pos: " SIZEFMTD " size: " SIZEFMTD ")\n[Stack trace: %s]" ,
                (_add ? "put" : "get"), ValueSize, Pos, Size, trace.c_str());
#endif
        }

    private:
        bool _add;
};

class ByteBufferSourceException : public ByteBufferException
{
    public:
        ByteBufferSourceException(size_t pos, size_t size, size_t valueSize)
        : ByteBufferException(pos, size, valueSize)
        {
            PrintError();
        }

    protected:
        void PrintError() const
        {
#ifdef DEBUG
            ACE_Stack_Trace trace;

            sLog->outError(LOG_FILTER_GENERAL, "Attempted to put a %s in ByteBuffer (pos: " SIZEFMTD " size: " SIZEFMTD ")\n[Stack trace: %s]",
                (ValueSize > 0 ? "NULL-pointer" : "zero-sized value"), Pos, Size, trace.c_str());
#endif
        }
};

class ByteBuffer
{
    public:

        void readPackGUID(uint64& guid)
        {
            if (rpos() + 1 > size())
                throw ByteBufferPositionException(false, _rpos, 1, size());

            uint8 l_GuidMaskLow = read<uint8>();
            uint8 l_GuidMaskHi = read<uint8>();

            uint64 l_GuidLow = 0;
            uint64 l_GuidHi = 0;

            uint8 l_BitIT = 0;
            uint8 l_MaskIT = 0;

            do
            {
                if ((1 << l_MaskIT) & l_GuidMaskLow)
                {
                    uint8 l_CurrentByte = read<uint8>();

                    l_GuidLow |= (uint64(l_CurrentByte) << l_BitIT);
                }

                l_BitIT += 8;
                l_MaskIT++;
            } while (l_BitIT < 64);

            l_BitIT = 0;
            l_MaskIT = 0;

            do
            {
                if ((1 << l_MaskIT) & l_GuidMaskHi)
                {
                    uint8 l_CurrentByte = read<uint8>();

                    l_GuidHi |= (uint64(l_CurrentByte) << l_BitIT);
                }

                l_BitIT += 8;
                l_MaskIT++;
            } while (l_BitIT < 64);

            Guid128 l_128Guid = Guid128(l_GuidLow, l_GuidHi);

            guid = Guid128To64(l_128Guid);
        }

        void appendPackGUID(uint64 guid)
        {
            Guid128 l_128Guid = Guid64To128(guid);

            uint32 l_StartWPos = wpos();

            append<uint8>(0);
            append<uint8>(0);

            uint64 l_GuidLow = l_128Guid.GetLow();
            uint64 l_GuidHi = l_128Guid.GetHi();

            uint8 l_BitIT = 0;
            uint8 l_MaskIT = 0;

            do
            {
                uint8 l_CurrentByte = (l_GuidLow >> l_BitIT) & 0xFF;

                if (l_CurrentByte)
                {
                    _storage[l_StartWPos + 0] |= 1 << l_MaskIT;
                    append<uint8>(l_CurrentByte);
                }

                l_BitIT += 8;
                l_MaskIT++;
            } while (l_BitIT < 64);

            l_BitIT = 0;
            l_MaskIT = 0;

            do
            {
                uint8 l_CurrentByte = (l_GuidHi >> l_BitIT) & 0xFF;

                if (l_CurrentByte)
                {
                    _storage[l_StartWPos + 1] |= 1 << l_MaskIT;
                    append<uint8>(l_CurrentByte);
                }

                l_BitIT += 8;
                l_MaskIT++;
            } while (l_BitIT < 64);
        }

        void ReadVector2(G3D::Vector2& p_Vector2)
        {
            *this >> p_Vector2.x;
            *this >> p_Vector2.y;
        }

        void ReadVector3(G3D::Vector3& p_Vector3)
        {
            *this >> p_Vector3.x;
            *this >> p_Vector3.y;
            *this >> p_Vector3.z;
        }

        void WriteVector2(G3D::Vector2& p_Vector2)
        {
            *this << float(p_Vector2.x);
            *this << float(p_Vector2.y);
        }

        void WriteVector3(G3D::Vector3& p_Vector3)
        {
            *this << float(p_Vector3.x);
            *this << float(p_Vector3.y);
            *this << float(p_Vector3.z);
        }

    public:
        const static size_t DEFAULT_SIZE = 0x1000;

        // constructor
        ByteBuffer() : _rpos(0), _wpos(0), _wbitpos(8), _rbitpos(8), _curbitval(0)
        {
            _storage.reserve(DEFAULT_SIZE);
            m_BaseSize = DEFAULT_SIZE;
        }

        ByteBuffer(size_t reserve) : _rpos(0), _wpos(0), _wbitpos(8), _rbitpos(8), _curbitval(0)
        {
            _storage.reserve(reserve);
            m_BaseSize = reserve;
        }

        // copy constructor
        ByteBuffer(const ByteBuffer &buf) : _rpos(buf._rpos), _wpos(buf._wpos),
            _wbitpos(buf._wbitpos), _rbitpos(8), _curbitval(buf._curbitval), m_BaseSize(buf.m_BaseSize), _storage(buf._storage)
        {
        }

        void clear()
        {
            _storage.clear();
            _rpos = _wpos = 0;
            _curbitval = 0;
            _wbitpos = 8;
            _rbitpos = 8;
        }

        template <typename T> void append(T value)
        {
            FlushBits();
            EndianConvert(value);
            append((uint8 *)&value, sizeof(value));
        }

        void ResetBitReading()
        {
            _rbitpos = 8;
        }

        void FlushBits()
        {
            if (_wbitpos == 8)
                return;

            append((uint8 *)&_curbitval, sizeof(uint8));
            _curbitval = 0;
            _wbitpos = 8;
        }

        void WriteBitInOrder(ObjectGuid guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                WriteBit(guid[order[i]]);
        }

        bool WriteBit(uint32 bit)
        {
            --_wbitpos;
            if (bit)
                _curbitval |= (1 << (_wbitpos));

            if (_wbitpos == 0)
            {
                _wbitpos = 8;
                append((uint8 *)&_curbitval, sizeof(_curbitval));
                _curbitval = 0;
            }

            return (bit != 0);
        }

        void ReadBitInOrder(ObjectGuid& guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                guid[order[i]] = ReadBit();
        }

        bool ReadBit()
        {
            ++_rbitpos;
            if (_rbitpos > 7)
            {
                _curbitval = read<uint8>();
                _rbitpos = 0;
            }

            return ((_curbitval >> (7-_rbitpos)) & 1) != 0;
        }

        template <typename T> void WriteBits(T value, size_t bits)
        {
            for (int32 i = bits-1; i >= 0; --i)
                WriteBit((value >> i) & 1);
        }

        uint32 ReadBits(size_t bits)
        {
            uint32 value = 0;
            for (int32 i = bits-1; i >= 0; --i)
                if (ReadBit())
                    value |= (1 << (i));

            return value;
        }

        uint32 ReadPackedTime()
        {
            uint32 l_Date = read<uint32>();
            tm l_Time = tm();

            l_Time.tm_min = l_Date & 0x3F;
            l_Time.tm_hour = (l_Date >> 6) & 0x1F;
            l_Time.tm_mday = ((l_Date >> 14) & 0x3F) + 1;
            l_Time.tm_mon = (l_Date >> 20) & 0xF;
            l_Time.tm_year = ((l_Date >> 24) & 0x1F) + 100;

            /// Needed for the msvc 2013 support
#ifdef _MSC_VER
            return uint32(mktime(&l_Time) + _timezone);
#else
            return uint32(mktime(&l_Time) + timezone);
#endif
        }

        ByteBuffer& ReadPackedTime(uint32& p_Time)
        {
            p_Time = ReadPackedTime();
            return *this;
        }

        void ReadBytesSeq(ObjectGuid& guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                ReadByteSeq(guid[order[i]]);
        }

        // Reads a byte (if needed) in-place
        void ReadByteSeq(uint8& b)
        {
            if (b != 0)
                b ^= read<uint8>();
        }

        void WriteBytesSeq(ObjectGuid guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                WriteByteSeq(guid[order[i]]);
        }

        void WriteByteSeq(uint8 b)
        {
            if (b != 0)
                append<uint8>(b ^ 1);
        }

        template <typename T> void put(size_t pos, T value)
        {
            EndianConvert(value);
            put(pos, (uint8 *)&value, sizeof(value));
        }

        /**
          * @name   PutBits
          * @brief  Places specified amount of bits of value at specified position in packet.
          *         To ensure all bits are correctly written, only call this method after
          *         bit flush has been performed

          * @param  pos Position to place the value at, in bits. The entire value must fit in the packet
          *             It is advised to obtain the position using bitwpos() function.

          * @param  value Data to write.
          * @param  bitCount Number of bits to store the value on.
        */
        template <typename T> void PutBits(size_t pos, T value, uint32 bitCount)
        {
            if (!bitCount)
                throw ByteBufferSourceException((pos + bitCount) / 8, size(), 0);

            if (pos + bitCount > size() * 8)
                throw ByteBufferPositionException(false, (pos + bitCount) / 8, size(), (bitCount - 1) / 8 + 1);

            for (uint32 i = 0; i < bitCount; ++i)
            {
                size_t wp = (pos + i) / 8;
                size_t bit = (pos + i) % 8;
                if ((value >> (bitCount - i - 1)) & 1)
                    _storage[wp] |= 1 << (7 - bit);
                else
                    _storage[wp] &= ~(1 << (7 - bit));
            }
        }

        ByteBuffer &operator<<(uint8 value)
        {
            append<uint8>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint16 value)
        {
            append<uint16>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint32 value)
        {
            append<uint32>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint64 value)
        {
            append<uint64>(value);
            return *this;
        }

        // signed as in 2e complement
        ByteBuffer &operator<<(int8 value)
        {
            append<int8>(value);
            return *this;
        }

        ByteBuffer &operator<<(int16 value)
        {
            append<int16>(value);
            return *this;
        }

        ByteBuffer &operator<<(int32 value)
        {
            append<int32>(value);
            return *this;
        }

        ByteBuffer &operator<<(int64 value)
        {
            append<int64>(value);
            return *this;
        }

        // floating points
        ByteBuffer &operator<<(float value)
        {
            append<float>(value);
            return *this;
        }

        ByteBuffer &operator<<(double value)
        {
            append<double>(value);
            return *this;
        }

        ByteBuffer &operator<<(const std::string &value)
        {
            if (size_t len = value.length())
                append((uint8 const*)value.c_str(), len);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator<<(const char *str)
        {
            if (size_t len = (str ? strlen(str) : 0))
                append((uint8 const*)str, len);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator<<(bool &value)
        {
            append<uint8>(value);
            return *this;
        }

        ByteBuffer &operator>>(bool &value)
        {
            value = read<char>() > 0 ? true : false;
            return *this;
        }

        ByteBuffer &operator>>(uint8 &value)
        {
            value = read<uint8>();
            return *this;
        }

        ByteBuffer &operator>>(uint16 &value)
        {
            value = read<uint16>();
            return *this;
        }

        ByteBuffer &operator>>(uint32 &value)
        {
            value = read<uint32>();
            return *this;
        }

        ByteBuffer &operator>>(uint64 &value)
        {
            value = read<uint64>();
            return *this;
        }

        //signed as in 2e complement
        ByteBuffer &operator>>(int8 &value)
        {
            value = read<int8>();
            return *this;
        }

        ByteBuffer &operator>>(int16 &value)
        {
            value = read<int16>();
            return *this;
        }

        ByteBuffer &operator>>(int32 &value)
        {
            value = read<int32>();
            return *this;
        }

        ByteBuffer &operator>>(int64 &value)
        {
            value = read<int64>();
            return *this;
        }

        ByteBuffer &operator>>(float &value)
        {
            value = read<float>();
            return *this;
        }

        ByteBuffer &operator>>(double &value)
        {
            value = read<double>();
            return *this;
        }

        ByteBuffer &operator>>(std::string& value)
        {
            value.clear();
            while (rpos() < size())                         // prevent crash at wrong string format in packet
            {
                char c = read<char>();
                if (c == 0)
                    break;
                value += c;
            }
            return *this;
        }

        uint8& operator[](size_t const pos)
        {
            if (pos >= size())
                throw ByteBufferPositionException(false, pos, 1, size());
            return _storage[pos];
        }

        uint8 const& operator[](size_t const pos) const
        {
            if (pos >= size())
                throw ByteBufferPositionException(false, pos, 1, size());
            return _storage[pos];
        }

        size_t rpos() const { return _rpos; }

        size_t rpos(size_t rpos_)
        {
            _rpos = rpos_;
            _rbitpos = 8;
            return _rpos;
        }

        void rfinish()
        {
            _rpos = wpos();
        }

        size_t wpos() const { return _wpos; }

        size_t wpos(size_t wpos_)
        {
            _wpos = wpos_;
            return _wpos;
        }

        /// Returns position of last written bit
        size_t bitwpos() const { return _wpos * 8 + 8 - _wbitpos; }

        size_t bitwpos(size_t newPos)
        {
            _wpos = newPos / 8;
            _wbitpos = 8 - (newPos % 8);
            return _wpos * 8 + 8 - _wbitpos;
        }

        template<typename T>
        void read_skip() { read_skip(sizeof(T)); }

        void read_skip(size_t skip)
        {
            if (_rpos + skip > size())
                throw ByteBufferPositionException(false, _rpos, skip, size());
            _rpos += skip;

            _rbitpos = 8;
        }

        template <typename T> T read()
        {
            T r = read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        }

        template <typename T> T read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
                throw ByteBufferPositionException(false, pos, sizeof(T), size());
            T val = *((T const*)&_storage[pos]);
            EndianConvert(val);
            const_cast<ByteBuffer*>(this)->_rbitpos = 8;
            return val;
        }

        void read(uint8 *dest, size_t len)
        {
            if (_rpos  + len > size())
               throw ByteBufferPositionException(false, _rpos, len, size());
            memcpy(dest, &_storage[_rpos], len);
            _rpos += len;
            _rbitpos = 8;
        }

        std::string ReadString(uint32 length)
        {
            if (!length)
                return std::string();
            char* buffer = new char[length + 1];
            memset(buffer, 0, length + 1);
            read((uint8*)buffer, length);
            std::string retval = buffer;
            delete[] buffer;
            return retval;
        }

        //! Method for writing strings that have their length sent separately in packet
        //! without null-terminating the string
        void WriteString(std::string const& str)
        {
            FlushBits();
            
            if (size_t len = str.length())
                append(str.c_str(), len);
        }

        const uint8 *contents() const { return &_storage[0]; }

        size_t size() const { return _storage.size(); }
        bool empty() const { return _storage.empty(); }

        void resize(size_t newsize)
        {
            _storage.resize(newsize, 0);
            _rpos = 0;
            _wpos = size();
        }

        void reserve(size_t ressize)
        {
            if (ressize > size())
                _storage.reserve(ressize);
        }

        void append(const char *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt);
        }

        template<class T> void append(const T *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt * sizeof(T));
        }

        void append(const uint8 *src, size_t cnt)
        {
            if (!cnt)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            if (!src)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            ASSERT(size() < 10000000);

            if (_storage.size() < _wpos + cnt)
                _storage.resize(_wpos + cnt);

            memcpy(&_storage[_wpos], src, cnt);
            _wpos += cnt;
        }

        void append(const ByteBuffer& buffer)
        {
            if (buffer.wpos())
                append(buffer.contents(), buffer.wpos());
        }

        // can be used in SMSG_MONSTER_MOVE opcode
        void appendPackXYZ(float x, float y, float z)
        {
            uint32 packed = 0;
            packed |= ((int)(x / 0.25f) & 0x7FF);
            packed |= ((int)(y / 0.25f) & 0x7FF) << 11;
            packed |= ((int)(z / 0.25f) & 0x3FF) << 22;
            *this << packed;
        }

        void put(size_t pos, const uint8 *src, size_t cnt)
        {
            if (pos + cnt > size())
                throw ByteBufferPositionException(true, pos, cnt, size());

            if (!src)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            memcpy(&_storage[pos], src, cnt);
        }

        void print_storage() const
        {
            if (!sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE)) // optimize disabled debug output
                return;

            std::ostringstream o;
            o << "STORAGE_SIZE: " << size();
            for (uint32 i = 0; i < size(); ++i)
                o << read<uint8>(i) << " - ";
            o << " ";

            sLog->outTrace(LOG_FILTER_NETWORKIO, "%s", o.str().c_str());
        }

        void textlike() const
        {
            if (!sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE)) // optimize disabled debug output
                return;

            std::ostringstream o;
            o << "STORAGE_SIZE: " << size();
            for (uint32 i = 0; i < size(); ++i)
            {
                char buf[1];
                snprintf(buf, 1, "%c", read<uint8>(i));
                o << buf;
            }
            o << " ";
            sLog->outTrace(LOG_FILTER_NETWORKIO, "%s", o.str().c_str());
        }

        void hexlike() const
        {
            if (!sLog->ShouldLog(LOG_FILTER_NETWORKIO, LOG_LEVEL_TRACE)) // optimize disabled debug output
                return;

            uint32 j = 1, k = 1;

            std::ostringstream o;
            o << "STORAGE_SIZE: " << size();

            for (uint32 i = 0; i < size(); ++i)
            {
                char buf[3];
                snprintf(buf, 1, "%2X ", read<uint8>(i));
                if ((i == (j * 8)) && ((i != (k * 16))))
                {
                    o << "| ";
                    ++j;
                }
                else if (i == (k * 16))
                {
                    o << "\n";
                    ++k;
                    ++j;
                }

                o << buf;
            }
            o << " ";
            sLog->outTrace(LOG_FILTER_NETWORKIO, "%s", o.str().c_str());
        }

        size_t GetBitPos() const
        {
            return _wbitpos;
        }

    protected:
        size_t _rpos, _wpos, _wbitpos, _rbitpos;
        uint8 _curbitval;
        uint32 m_BaseSize;
        std::vector<uint8> _storage;
};

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i)
    {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while (msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

template<> inline std::string ByteBuffer::read<std::string>()
{
    std::string tmp;
    *this >> tmp;
    return tmp;
}

template<>
inline void ByteBuffer::read_skip<char*>()
{
    std::string temp;
    *this >> temp;
}

template<>
inline void ByteBuffer::read_skip<char const*>()
{
    read_skip<char*>();
}

template<>
inline void ByteBuffer::read_skip<std::string>()
{
    read_skip<char*>();
}

#endif

