#pragma once
//------------------------------------------------------------------------------
/**
    @class Posix::PosixInterlocked
 
    Provides simple atomic operations on shared variables.
     
    (C) 2013-2018 Individual contributors, see AUTHORS file
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Posix
{
class PosixInterlocked
{
public:
    /// interlocked increment
    static int Increment(int volatile* var);
    /// interlocked decrement
    static int Decrement(int volatile* var);
    /// interlocked add
    static int Add(int volatile* var, int add);
    /// interlocked exchange
    static int Exchange(int volatile* dest, int value);
    /// interlocked compare-exchange
    static int CompareExchange(int volatile* dest, int exchange, int comparand);
};

//------------------------------------------------------------------------------
/**
*/
inline int
PosixInterlocked::Increment(int volatile* var)
{
    return __sync_add_and_fetch(var, 1);
}
    
//------------------------------------------------------------------------------
/**
*/
inline int
PosixInterlocked::Decrement(int volatile* var)
{
    return __sync_sub_and_fetch(var, 1);
}
    
//------------------------------------------------------------------------------
/**
*/
inline int
PosixInterlocked::Add(int volatile* var, int add)
{
    return __sync_add_and_fetch(var, add);
}
   
//------------------------------------------------------------------------------
/**
*/
inline int
PosixInterlocked::Exchange(int volatile* dest, int value)
{
    return __sync_lock_test_and_set(dest, value);
}
    
//------------------------------------------------------------------------------
/**
 */
inline int
PosixInterlocked::CompareExchange(int volatile* dest, int exchange, int comparand)
{
    return __sync_val_compare_and_swap(dest, comparand, exchange);
}

} // namespace Posix
//------------------------------------------------------------------------------
