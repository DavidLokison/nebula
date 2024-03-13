//------------------------------------------------------------------------------
//  charactermaterialskinnode.cc
//  (C) 2011-2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------

#include "characterskinnode.h"
#include "coregraphics/shaderserver.h"

#include "system_shaders/objects_shared.h"

namespace Models
{

using namespace Util;
using namespace Models;
using namespace IO;
using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
CharacterSkinNode::CharacterSkinNode()
{
    this->type = CharacterSkinNodeType;
    this->bits = HasTransformBit | HasStateBit;
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkinNode::~CharacterSkinNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CharacterSkinNode::Load(const Util::FourCC& fourcc, const Util::StringAtom& tag, const Ptr<IO::BinaryReader>& reader, bool immediate)
{
    bool retval = true;
    if (FourCC('NSKF') == fourcc)
    {
        // NumSkinFragments
        this->ReserveFragments(reader->ReadInt());
    }
    else if (FourCC('SFRG') == fourcc)
    {
        // SkinFragment
        this->loadContext.primIndex = reader->ReadInt();
        Array<IndexT> jointPalette;
        SizeT numJoints = reader->ReadInt();
        jointPalette.Reserve(numJoints);
        IndexT i;
        for (i = 0; i < numJoints; i++)
        {
            jointPalette.Append(reader->ReadInt());
        }
        this->AddFragment(this->loadContext.primIndex, jointPalette);
    }
    else
    {
        retval = PrimitiveNode::Load(fourcc, tag, reader, immediate);
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
*/
void 
CharacterSkinNode::OnFinishedLoading(ModelStreamingData* streamingData)
{
    PrimitiveNode::OnFinishedLoading(streamingData);
    SizeT numFrames = CoreGraphics::GetNumBufferedFrames();
    for (IndexT i = 0; i < numFrames; i++)
    {
        CoreGraphics::ResourceTableSetConstantBuffer(this->resourceTables[i], { CoreGraphics::GetConstantBuffer(i), ObjectsShared::Table_DynamicOffset::JointBlock_SLOT, 0, (SizeT)(sizeof(Math::mat4) * 256), 0, false, true });
        CoreGraphics::ResourceTableCommitChanges(this->resourceTables[i]);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNode::ReserveFragments(SizeT numFragments)
{
    this->skinFragments.Clear();
    this->skinFragments.Reserve(numFragments);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNode::AddFragment(IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette)
{
    Fragment fragment;
    this->skinFragments.Append(fragment);
    this->skinFragments.Back().primGroupIndex = primGroupIndex;
    this->skinFragments.Back().jointPalette = jointPalette;
}

} // namespace Characters
