#pragma once
//------------------------------------------------------------------------------
/**
    A model resource consists of nodes, each of which inhibit some information
    read from an .n3 file. 
    
    @copyright
    (C) 2017-2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "ids/id.h"
#include "ids/idallocator.h"
#include "resources/resourceid.h"
#include "core/rttimacros.h"
#include "memory/rangeallocator.h"
#include <functional>
namespace Models
{

class ModelNode;

RESOURCE_ID_TYPE(ModelId);

enum NodeType
{
    CharacterNodeType,
    TransformNodeType,
    ShaderStateNodeType,
    PrimitiveNodeType,
    ParticleSystemNodeType,
    CharacterSkinNodeType,

    NumNodeTypes
};

enum NodeBits
{
    NoBits = N_BIT(0),
    HasTransformBit = N_BIT(1),
    HasStateBit = N_BIT(2)
};
__ImplementEnumBitOperators(NodeBits);

enum class LoadBits
{
    NoBits = N_BIT(0),
    MeshBit = N_BIT(1),
    AnimBit = N_BIT(2),
    MaterialBit = N_BIT(3)
};
__ImplementEnumBitOperators(LoadBits);

struct ModelStreamingData
{
    LoadBits requiredBits;
    LoadBits loadedBits;
};

struct NodeInstanceRange
{
    Memory::RangeAllocation allocation;
    SizeT begin, end;
};

struct ModelCreateInfo
{
    Math::bbox boundingBox;
    Util::Array<Models::ModelNode*> nodes;
};

/// create model (resource)
const ModelId CreateModel(const ModelCreateInfo& info);
/// discard model (resource)
void DestroyModel(const ModelId id);

/// Get model nodes
const Util::Array<Models::ModelNode*>& ModelGetNodes(const ModelId id);
/// Get model bounding box
const Math::bbox& ModelGetBoundingBox(const ModelId id);

enum
{
    Model_BoundingBox,
    Model_Nodes
};

typedef Ids::IdAllocator<
    Math::bbox,
    Util::Array<Models::ModelNode*>
> ModelAllocator;
extern ModelAllocator modelAllocator;
} // namespace Models
