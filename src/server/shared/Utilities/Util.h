////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _UTIL_H
#define _UTIL_H

#include "Define.h"
#include "Common.h"

#include <cctype>
#include <iomanip>

 void init_sfmt();

// Searcher for map of structs
template<typename T, class S> struct Finder
{
    T val_;
    T S::* idMember_;
    Finder(T val, T S::* idMember) : val_(val), idMember_(idMember) {}
    bool operator()(const std::pair<int, S> &obj) { return obj.second.*idMember_ == val_; }
};

class Tokenizer
{
public:
    typedef std::vector<char const *> StorageType;

    typedef StorageType::size_type size_type;

    typedef StorageType::const_iterator const_iterator;
    typedef StorageType::reference reference;
    typedef StorageType::const_reference const_reference;

public:
    Tokenizer(const std::string &src, char const sep, uint32 vectorReserve = 0);
    ~Tokenizer() { delete[] m_str; }

    const_iterator begin() const { return m_storage.begin(); }
    const_iterator end() const { return m_storage.end(); }

    size_type size() const { return m_storage.size(); }

    reference operator [] (size_type i) { return m_storage[i]; }
    const_reference operator [] (size_type i) const { return m_storage[i]; }

    private:
    char* m_str;
    StorageType m_storage;
};

struct nullable_string
{
    char const* const ptr;
    uint32 const length;

    nullable_string()
        : ptr(""), length(0)
    {
    }

    nullable_string(const char* ptr_, uint32 len)
        : ptr(ptr_), length(len)
    {
    }
};

void UnpackDBBinary(void* unpackedData, uint32 unpackedCount, void const* packedData, uint32 packedCount);

nullable_string PackDBBinary(void const* unpackedData, uint32 unpackedCount);

void stripLineInvisibleChars(std::string &src);

std::string secsToTimeString(uint64 timeInSecs, bool shortText = false, bool hoursOnly = false);
uint32 TimeStringToSecs(const std::string& timestring);
std::string TimeToTimestampStr(time_t t);

/* Return a random number in the range min..max; (max-min) must be smaller than 32768. */
int32 irand(int32 min, int32 max);

/* Return a random number in the range min..max (inclusive). For reliable results, the difference
* between max and min should be less than RAND32_MAX. */
uint32 urand(uint32 min, uint32 max);

/* Return a random number in the range 0 .. RAND32_MAX. */
int32 rand32();

/* Return a random number in the range min..max */
float frand(float min, float max);

/* Return a random double from 0.0 to 1.0 (exclusive). Floats support only 7 valid decimal digits.
 * A double supports up to 15 valid decimal digits and is used internally (RAND32_MAX has 10 digits).
 * With an FPU, there is usually no difference in performance between float and double. */
 double rand_norm(void);

/* Return a random double from 0.0 to 99.9999999999999. Floats support only 7 valid decimal digits.
 * A double supports up to 15 valid decimal digits and is used internally (RAND32_MAX has 10 digits).
 * With an FPU, there is usually no difference in performance between float and double. */
 double rand_chance(void);

/* Return true if a random roll fits in the specified chance (range 0-100). */
inline bool roll_chance_f(float chance)
{
    return chance > rand_chance();
}

/* Return true if a random roll fits in the specified chance (range 0-100). */
inline bool roll_chance_i(int chance)
{
    return chance > irand(0, 99);
}

inline void ApplyPercentModFloatVar(float& var, float val, bool apply)
{
    if (val == -100.0f)     // prevent set var to zero
        val = -99.99f;
    var *= (apply ? (100.0f + val) / 100.0f : 100.0f / (100.0f + val));
}

// Percentage calculation
template <class T, class U>
inline T CalculatePct(T base, U pct)
{
    return T(base * static_cast<float>(pct) / 100.0f);
}

template <class T, class U>
inline T AddPct(T &base, U pct)
{
    return base += CalculatePct(base, pct);
}

template <class T, class U>
inline T ApplyPct(T &base, U pct)
{
    return base = CalculatePct(base, pct);
}

template <class T>
inline T RoundToInterval(T& num, T floor, T ceil)
{
    return num = std::min(std::max(num, floor), ceil);
}

// UTF8 handling
bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr);
// in wsize==max size of buffer, out wsize==real string size
bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize);
inline bool Utf8toWStr(const std::string& utf8str, wchar_t* wstr, size_t& wsize)
{
    return Utf8toWStr(utf8str.c_str(), utf8str.size(), wstr, wsize);
}

bool WStrToUtf8(std::wstring const& wstr, std::string& utf8str);
// size==real string size
bool WStrToUtf8(wchar_t* wstr, size_t size, std::string& utf8str);

size_t utf8length(std::string& utf8str);                    // set string to "" if invalid utf8 sequence
void utf8truncate(std::string& utf8str, size_t len);

inline bool isBasicLatinCharacter(wchar_t wchar)
{
    if (wchar >= L'a' && wchar <= L'z')                      // LATIN SMALL LETTER A - LATIN SMALL LETTER Z
        return true;
    if (wchar >= L'A' && wchar <= L'Z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
        return true;
    return false;
}

inline bool isExtendedLatinCharacter(wchar_t wchar)
{
    if (isBasicLatinCharacter(wchar))
        return true;
    if (wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
        return true;
    if (wchar >= 0x00D8 && wchar <= 0x00DF)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
        return true;
    if (wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
        return true;
    if (wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
        return true;
    if (wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
        return true;
    if (wchar >= 0x0100 && wchar <= 0x012F)                  // LATIN CAPITAL LETTER A WITH MACRON - LATIN SMALL LETTER I WITH OGONEK
        return true;
    if (wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
        return true;
    return false;
}

inline bool isCyrillicCharacter(wchar_t wchar)
{
    if (wchar >= 0x0410 && wchar <= 0x044F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC SMALL LETTER YA
        return true;
    if (wchar == 0x0401 || wchar == 0x0451)                  // CYRILLIC CAPITAL LETTER IO, CYRILLIC SMALL LETTER IO
        return true;
    return false;
}

inline bool isEastAsianCharacter(wchar_t wchar)
{
    if (wchar >= 0x1100 && wchar <= 0x11F9)                  // Hangul Jamo
        return true;
    if (wchar >= 0x3041 && wchar <= 0x30FF)                  // Hiragana + Katakana
        return true;
    if (wchar >= 0x3131 && wchar <= 0x318E)                  // Hangul Compatibility Jamo
        return true;
    if (wchar >= 0x31F0 && wchar <= 0x31FF)                  // Katakana Phonetic Ext.
        return true;
    if (wchar >= 0x3400 && wchar <= 0x4DB5)                  // CJK Ideographs Ext. A
        return true;
    if (wchar >= 0x4E00 && wchar <= 0x9FC3)                  // Unified CJK Ideographs
        return true;
    if (wchar >= 0xAC00 && wchar <= 0xD7A3)                  // Hangul Syllables
        return true;
    if (wchar >= 0xFF01 && wchar <= 0xFFEE)                  // Halfwidth forms
        return true;
    return false;
}

inline bool isNumeric(wchar_t wchar)
{
    return (wchar >= L'0' && wchar <=L'9');
}

inline bool isNumeric(char c)
{
    return (c >= '0' && c <='9');
}

inline bool isNumeric(char const* str)
{
    for (char const* c = str; *c; ++c)
        if (!isNumeric(*c))
            return false;

    return true;
}

inline bool isNumericOrSpace(wchar_t wchar)
{
    return isNumeric(wchar) || wchar == L' ';
}

inline bool isBasicLatinString(std::wstring const& wstr, bool numericOrSpace)
{
    for (size_t i = 0; i < wstr.size(); ++i)
        if (!isBasicLatinCharacter(wstr[i]) && (!numericOrSpace || !isNumericOrSpace(wstr[i])))
            return false;
    return true;
}

inline bool isExtendedLatinString(std::wstring const& wstr, bool numericOrSpace)
{
    for (size_t i = 0; i < wstr.size(); ++i)
        if (!isExtendedLatinCharacter(wstr[i]) && (!numericOrSpace || !isNumericOrSpace(wstr[i])))
            return false;
    return true;
}

inline bool isCyrillicString(std::wstring const& wstr, bool numericOrSpace)
{
    for (size_t i = 0; i < wstr.size(); ++i)
        if (!isCyrillicCharacter(wstr[i]) && (!numericOrSpace || !isNumericOrSpace(wstr[i])))
            return false;
    return true;
}

inline bool isEastAsianString(std::wstring const& wstr, bool numericOrSpace)
{
    for (size_t i = 0; i < wstr.size(); ++i)
        if (!isEastAsianCharacter(wstr[i]) && (!numericOrSpace || !isNumericOrSpace(wstr[i])))
            return false;
    return true;
}

inline wchar_t wcharToUpper(wchar_t wchar)
{
    if (wchar >= L'a' && wchar <= L'z')                      // LATIN SMALL LETTER A - LATIN SMALL LETTER Z
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
        return wchar_t(0x1E9E);
    if (wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar >= 0x0101 && wchar <= 0x012F)                  // LATIN SMALL LETTER A WITH MACRON - LATIN SMALL LETTER I WITH OGONEK (only %2=1)
    {
        if (wchar % 2 == 1)
            return wchar_t(uint16(wchar)-0x0001);
    }
    if (wchar >= 0x0430 && wchar <= 0x044F)                  // CYRILLIC SMALL LETTER A - CYRILLIC SMALL LETTER YA
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar == 0x0451)                                     // CYRILLIC SMALL LETTER IO
        return wchar_t(0x0401);

    return wchar;
}

inline wchar_t wcharToUpperOnlyLatin(wchar_t wchar)
{
    return isBasicLatinCharacter(wchar) ? wcharToUpper(wchar) : wchar;
}

inline wchar_t wcharToLower(wchar_t wchar)
{
    if (wchar >= L'A' && wchar <= L'Z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
        return wchar_t(uint16(wchar)+0x0020);
    if (wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
        return wchar_t(uint16(wchar)+0x0020);
    if (wchar >= 0x00D8 && wchar <= 0x00DE)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
        return wchar_t(uint16(wchar)+0x0020);
    if (wchar >= 0x0100 && wchar <= 0x012E)                  // LATIN CAPITAL LETTER A WITH MACRON - LATIN CAPITAL LETTER I WITH OGONEK (only %2=0)
    {
        if (wchar % 2 == 0)
            return wchar_t(uint16(wchar)+0x0001);
    }
    if (wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
        return wchar_t(0x00DF);
    if (wchar == 0x0401)                                     // CYRILLIC CAPITAL LETTER IO
        return wchar_t(0x0451);
    if (wchar >= 0x0410 && wchar <= 0x042F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC CAPITAL LETTER YA
        return wchar_t(uint16(wchar)+0x0020);

    return wchar;
}

inline void wstrToUpper(std::wstring& str)
{
    std::transform( str.begin(), str.end(), str.begin(), wcharToUpper );
}

inline void wstrToLower(std::wstring& str)
{
    std::transform( str.begin(), str.end(), str.begin(), wcharToLower );
}

std::wstring GetMainPartOfName(std::wstring const& wname, uint32 declension);

bool utf8ToConsole(const std::string& utf8str, std::string& conStr);
bool consoleToUtf8(const std::string& conStr, std::string& utf8str);
bool Utf8FitTo(const std::string& str, std::wstring search);
void utf8printf(FILE* out, const char *str, ...);
void vutf8printf(FILE* out, const char *str, va_list* ap);

bool IsIPAddress(char const* ipaddress);
uint32 CreatePIDFile(const std::string& filename);

std::string ByteArrayToHexStr(uint8 const* bytes, uint32 length, bool reverse = false);

inline std::string UrlEncode(const std::string & p_Value)
{
    std::ostringstream l_Espaced;
    l_Espaced.fill('0');
    l_Espaced << std::hex;

    for (std::string::const_iterator l_I = p_Value.begin(); l_I != p_Value.end(); ++l_I)
    {
        std::string::value_type l_Char = (*l_I);

        if (isalnum(l_Char) || l_Char == '-' || l_Char == '_' || l_Char == '.' || l_Char == '~')
        {
            l_Espaced << l_Char;
            continue;
        }

        l_Espaced << '%' << std::setw(2) << int((unsigned char)l_Char);
    }

    return l_Espaced.str();
}
#endif

//handler for operations on large flags
#ifndef _FLAG96
#define _FLAG96

#ifndef PAIR64_HIPART
#define PAIR64_HIPART(x)   (uint32)((uint64(x) >> 32) & UI64LIT(0x00000000FFFFFFFF))
#define PAIR64_LOPART(x)   (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))
#endif

// simple class for not-modifiable list
template <typename T>
class HookList
{
    typedef typename std::list<T>::iterator ListIterator;
    private:
        typename std::list<T> m_list;
    public:
        HookList<T> & operator+=(T t)
        {
            m_list.push_back(t);
            return *this;
        }
        HookList<T> & operator-=(T t)
        {
            m_list.remove(t);
            return *this;
        }
        size_t size()
        {
            return m_list.size();
        }
        ListIterator begin()
        {
            return m_list.begin();
        }
        ListIterator end()
        {
            return m_list.end();
        }
};

class flag96
{
private:
    uint32 part[3];
public:
    flag96(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0)
    {
        part[0] =p1;
        part[1] =p2;
        part[2] =p3;
    }

    flag96(uint64 p1, uint32 p2)
    {
        part[0] =PAIR64_LOPART(p1);
        part[1] =PAIR64_HIPART(p1);
        part[2] =p2;
    }

    inline bool IsEqual(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0) const
    {
        return (part[0] == p1 && part[1] == p2 && part[2] == p3);
    }

    inline bool HasFlag(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0) const
    {
        return (part[0] & p1 || part[1] & p2 || part[2] & p3);
    };

    inline void Set(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0)
    {
        part[0] = p1;
        part[1] = p2;
        part[2] = p3;
    };

    inline bool operator <(const flag96 &right) const
    {
        for (uint8 i = 3; i > 0; --i)
        {
            if (part[i - 1] < right.part[i - 1])
                return true;
            else if (part[i - 1] > right.part[i - 1])
                return false;
        }
        return false;
    }

    inline bool operator ==(const flag96 &right) const
    {
        return
        (
            part[0] == right.part[0] &&
            part[1] == right.part[1] &&
            part[2] == right.part[2]
        ); 
    }

    inline bool operator !=(const flag96 &right) const
    {
        return !this->operator ==(right);
    };

    inline flag96 & operator =(const flag96 &right)
    {
        part[0] = right.part[0];
        part[1] = right.part[1];
        part[2] = right.part[2];
        return *this;
    };

    inline flag96 operator &(const flag96 &right) const
    {
        return flag96(part[0] & right.part[0], part[1] & right.part[1],
            part[2] & right.part[2]);
    };

    inline flag96 & operator &=(const flag96 &right)
    {
        part[0] &= right.part[0];
        part[1] &= right.part[1];
        part[2] &= right.part[2];
        return *this;
    };

    inline flag96 operator |(const flag96 &right) const
    {
        return flag96(part[0] | right.part[0], part[1] | right.part[1],
            part[2] | right.part[2]);
    };

    inline flag96 & operator |=(const flag96 &right)
    {
        part[0] |= right.part[0];
        part[1] |= right.part[1];
        part[2] |= right.part[2];
        return *this;
    };

    inline flag96 operator ~() const
    {
        return flag96(~part[0], ~part[1], ~part[2]);
    };

    inline flag96 operator ^(const flag96 &right) const
    {
        return flag96(part[0] ^ right.part[0], part[1] ^ right.part[1],
            part[2] ^ right.part[2]);
    };

    inline flag96 & operator ^=(const flag96 &right)
    {
        part[0] ^= right.part[0];
        part[1] ^= right.part[1];
        part[2] ^= right.part[2];
        return *this;
    };

    inline operator bool() const
    {
        return (part[0] != 0 || part[1] != 0 || part[2] != 0);
    };

    inline bool operator !() const
    {
        return !this->operator bool();
    };

    inline uint32 & operator [](uint8 el)
    {
        return part[el];
    };

    inline const uint32 & operator [](uint8 el) const
    {
        return part[el];
    }
};

class flag128
{
private:
    uint32 part[4];

public:
    flag128(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0, uint32 p4 = 0)
    {
        part[0] = p1;
        part[1] = p2;
        part[2] = p3;
        part[3] = p4;
    }

    flag128(uint64 p1, uint64 p2)
    {
        part[0] = (uint32)(p1 & UI64LIT(0x00000000FFFFFFFF));
        part[1] = (uint32)((p1 >> 32) & UI64LIT(0x00000000FFFFFFFF));
        part[2] = (uint32)(p2 & UI64LIT(0x00000000FFFFFFFF));
        part[3] = (uint32)((p2 >> 32) & UI64LIT(0x00000000FFFFFFFF));
    }

    inline bool IsEqual(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0, uint32 p4 = 0) const
    {
        return (part[0] == p1 && part[1] == p2 && part[2] == p3 && part[3] == p4);
    }

    inline bool HasFlag(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0, uint32 p4 = 0) const
    {
        return (part[0] & p1 || part[1] & p2 || part[2] & p3 || part[3] & p4);
    }

    inline bool HasFlag(flag128 const& other) const
    {
        return HasFlag(other[0], other[1], other[2], other[3]);
    }

    inline void Set(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0, uint32 p4 = 0)
    {
        part[0] = p1;
        part[1] = p2;
        part[2] = p3;
        part[3] = p4;
    }

    inline bool operator <(const flag128 &right) const
    {
        for (uint8 i = 3; i + 1 > 0; --i)
        {
            if (part[i] < right.part[i])
                return true;
            else if (part[i] > right.part[i])
                return false;
        }
        return false;
    }

    inline bool operator ==(const flag128 &right) const
    {
        return
        (
            part[0] == right.part[0] &&
            part[1] == right.part[1] &&
            part[2] == right.part[2] &&
            part[3] == right.part[3]
        );
    }

    inline bool operator !=(const flag128 &right) const
    {
        return !this->operator ==(right);
    }

    inline flag128 & operator =(const flag128 &right)
    {
        part[0] = right.part[0];
        part[1] = right.part[1];
        part[2] = right.part[2];
        part[3] = right.part[3];
        return *this;
    }

    inline flag128 operator &(const flag128 &right) const
    {
        return flag128(part[0] & right.part[0], part[1] & right.part[1],
            part[2] & right.part[2], part[3] & right.part[3]);
    }

    inline flag128 & operator &=(const flag128 &right)
    {
        part[0] &= right.part[0];
        part[1] &= right.part[1];
        part[2] &= right.part[2];
        part[3] &= right.part[3];
        return *this;
    }

    inline flag128 operator |(const flag128 &right) const
    {
        return flag128(part[0] | right.part[0], part[1] | right.part[1],
            part[2] | right.part[2], part[3] | right.part[3]);
    }

    inline flag128 & operator |=(const flag128 &right)
    {
        part[0] |= right.part[0];
        part[1] |= right.part[1];
        part[2] |= right.part[2];
        part[3] |= right.part[3];
        return *this;
    }

    inline flag128 operator ~() const
    {
        return flag128(~part[0], ~part[1], ~part[2], ~part[3]);
    }

    inline flag128 operator ^(const flag128 &right) const
    {
        return flag128(part[0] ^ right.part[0], part[1] ^ right.part[1],
            part[2] ^ right.part[2], part[3] ^ right.part[3]);
    }

    inline flag128 & operator ^=(const flag128 &right)
    {
        part[0] ^= right.part[0];
        part[1] ^= right.part[1];
        part[2] ^= right.part[2];
        part[3] ^= right.part[3];
        return *this;
    }

    inline operator bool() const
    {
        return (part[0] != 0 || part[1] != 0 || part[2] != 0 || part[3] != 0);
    }

    inline bool operator !() const
    {
        return !this->operator bool();
    }

    inline uint32 & operator [](uint8 el)
    {
        return part[el];
    }

    inline const uint32 & operator [](uint8 el) const
    {
        return part[el];
    }
};

inline uint32 ExtractBitMaskBitCount(uint32 p_Value)
{
    uint32 l_MaskPart = (p_Value - ((p_Value >> 1) & 0x55555555));

    return 0x1010101 * (((l_MaskPart & 0x33333333) + ((l_MaskPart >> 2) & 0x33333333)
        + (((l_MaskPart & 0x33333333) + ((l_MaskPart >> 2) & 0x33333333)) >> 4)) & 0xF0F0F0F) >> 24;
}

#endif
