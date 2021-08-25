//------------------------------------------------------------------------------
//  bruteforcesystem.cc
//  (C) 2018-2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "render/stdneb.h"
#include "bruteforcesystem.h"
#include "visibility/visibilitycontext.h"
#include "graphics/graphicsserver.h"
#include "math/mat4.h"
namespace Visibility
{

//------------------------------------------------------------------------------
/**
*/
void
BruteforceSystem::Setup(const BruteforceSystemLoadInfo& info)
{
}

//------------------------------------------------------------------------------
/**
*/
void
BruteforceSystem::Run()
{
    IndexT i;
    for (i = 0; i < this->obs.count; i++)
    {
        Jobs::JobContext ctx;

        // uniform data is the observer transform
        ctx.uniform.numBuffers = 1;
        ctx.uniform.data[0] = (unsigned char*)&this->obs.transforms[i];
        ctx.uniform.dataSize[0] = sizeof(Math::mat4);
        ctx.uniform.scratchSize = 0;

        // just one input buffer of all the transforms
        ctx.input.numBuffers = 2;
        ctx.input.data[0] = (unsigned char*)this->ent.boxes;
        ctx.input.dataSize[0] = sizeof(Math::bbox) * this->ent.count;
        ctx.input.sliceSize[0] = sizeof(Math::bbox);

        ctx.input.data[1] = (unsigned char*)this->ent.entityFlags;
        ctx.input.dataSize[1] = sizeof(uint32_t) * this->ent.count;
        ctx.input.sliceSize[1] = sizeof(uint32_t);

        // the output is the visibility result
        ctx.output.numBuffers = 1;
        ctx.output.data[0] = (unsigned char*)this->obs.vis[i];
        ctx.output.dataSize[0] = sizeof(Math::ClipStatus::Type) * this->ent.count;
        ctx.output.sliceSize[0] = sizeof(Math::ClipStatus::Type);

        // create and run job
        Jobs::JobId job = Jobs::CreateJob({ BruteforceSystemJobFunc });
        Jobs::JobSchedule(job, Graphics::GraphicsServer::renderSystemsJobPort, ctx);

        // enqueue here, but don't dequeue as VisibilityContext will do it for us
        ObserverContext::runningJobs.Enqueue(job);
    }
}
} // namespace Visibility
