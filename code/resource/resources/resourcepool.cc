//------------------------------------------------------------------------------
// resourceloader.cc
// (C)2017-2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "foundation/stdneb.h"
#include "resourcepool.h"

namespace Resources
{

__ImplementAbstractClass(Resources::ResourcePool, 'RELO', Core::RefCounted);
//------------------------------------------------------------------------------
/**
*/
ResourcePool::ResourcePool() :
    resourceInstanceIndexPool(0xFFFFFFFF)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ResourcePool::~ResourcePool()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ResourcePool::Setup()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ResourcePool::Discard()
{
    // empty
}


//------------------------------------------------------------------------------
/**
*/
void
ResourcePool::LoadFallbackResources()
{
    // empty, implement in subclass
}

//------------------------------------------------------------------------------
/**
*/
void
ResourcePool::DiscardResource(const Resources::ResourceId id)
{
    // reduce usage
    this->usage[id.poolId]--;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourcePool::DiscardByTag(const Util::StringAtom& tag)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ResourcePool::Update(IndexT frameIndex)
{
    // do nothing
}

} // namespace Resources