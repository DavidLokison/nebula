#pragma once
//------------------------------------------------------------------------------
/**
    @file bit.h
    
    Implements helper functions for checking bits
    
    @copyright
    (C) 2009 Radon Labs GmbH
    (C) 2013-2020 Individual contributors, see AUTHORS file
*/
#include "core/types.h"

namespace Util
{

//------------------------------------------------------------------------------
/**
*/
constexpr uint64 
SetBit(uint64 mask, uint8 bit)
{
    return mask | (1ULL << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
HasBit(uint64 mask, uint8 bit)
{
    return mask & (1ULL << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr uint32
SetBit(uint32 mask, uint8 bit)
{
    return mask | (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
HasBit(uint32 mask, uint8 bit)
{
    return mask & (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr uint16
SetBit(uint16 mask, uint8 bit)
{
    return mask | (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
HasBit(uint16 mask, uint8 bit)
{
    return mask & (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr int64
SetBit(int64 mask, uint8 bit)
{
    return mask | (1LL << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
HasBit(int64 mask, uint8 bit)
{
    return mask & (1LL << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr int32
SetBit(int32 mask, uint8 bit)
{
    return mask | (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
HasBit(int32 mask, uint8 bit)
{
    return mask & (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr int16
SetBit(int16 mask, uint8 bit)
{
    return mask | (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
HasBit(int16 mask, uint8 bit)
{
    return mask & (1 << bit);
}

//------------------------------------------------------------------------------
/**
*/
constexpr uint32
CountBits(uint32 i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

//------------------------------------------------------------------------------
/**
    Combine hashes
*/
template<typename T>
inline void HashCombine(uint32_t& s, const T& v)
{
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

//------------------------------------------------------------------------------
/**
*/
inline uint
PopCnt(uint value)
{
	return _mm_popcnt_u32(value);
}

//------------------------------------------------------------------------------
/**
*/
inline uint64
PopCnt(uint64 value)
{
	return _mm_popcnt_u64(value);
}

//------------------------------------------------------------------------------
/**
*/
inline uint
FirstOne(uint value)
{
#if __WIN32__
    DWORD count = 0;
    _BitScanForward(&count, value);
#else
    int count = __builtin_ctz(value);
#endif
    return count;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
FirstOne(uint64 value)
{
#if __WIN32__
    DWORD count = 0;
    _BitScanForward64(&count, value);
#else
    int count = __builtin_ctz(value);
#endif
    return count;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
LastOne(uint value)
{
#if __WIN32__
    DWORD count = 0;
    _BitScanReverse(&count, value);
#else
    int count = __builtin_clz(value);
#endif
    return count;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
LastOne(uint64 value)
{
#if __WIN32__
    DWORD count = 0;
    _BitScanReverse64(&count, value);
#else
    int count = __builtin_clz(value);
#endif
    return count;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
Lsb(uint value, byte bit)
{
    uint mask = value & ~((1 << bit) - 1);
#if __WIN32__
    DWORD count = 0;
    _BitScanForward(&count, mask);
#else
    int count = __builtin_ctz(mask);
#endif
    return mask ? count : 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/
inline uint 
BitmaskConvert(uint mask, const uint* table, const uint numEntries = 0xFFFFFFFF)
{
    uint ret = 0x0;
    uint usageFlags = mask;
    uint flagIndex = 0;
    while (usageFlags != 0x0 && flagIndex < numEntries)
    {
        if (usageFlags & (1 << flagIndex))
        {
            ret |= table[flagIndex];
        }
        usageFlags &= ~(1 << flagIndex);
        flagIndex++;
    }
    return ret;
}

} // namespace Util

