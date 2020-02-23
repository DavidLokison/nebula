//------------------------------------------------------------------------------
// viewerapp.cc
// (C) 2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/refcounted.h"
#include "timing/timer.h"
#include "io/console.h"
#include "visibility/visibilitycontext.h"
#include "models/streammodelpool.h"
#include "models/modelcontext.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "viewerapp.h"
#include "math/vector.h"
#include "math/point.h"
#include "dynui/imguicontext.h"
#include "lighting/lightcontext.h"
#include "characters/charactercontext.h"
#include "imgui.h"
#include "dynui/im3d/im3dcontext.h"
#include "dynui/im3d/im3d.h"
#include "graphics/environmentcontext.h"
#include "clustering/clustercontext.h"
#include "scenes/scenes.h"
#include "debug/framescriptinspector.h"
#include "io/logfileconsolehandler.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;

namespace Tests
{

//------------------------------------------------------------------------------
/**
*/
const char* stateToString(Resources::Resource::State state)
{
    switch (state)
    {
    case Resources::Resource::State::Pending: return "Pending";
    case Resources::Resource::State::Loaded: return "Loaded";
    case Resources::Resource::State::Failed: return "Failed";
    case Resources::Resource::State::Unloaded: return "Unloaded";
    }
    return "Unknown";
}

//------------------------------------------------------------------------------
/**
*/
SimpleViewerApplication::SimpleViewerApplication()
    : pauseProfiling(false)
    , profileFixedFps(false)
    , fixedFps(60)
{
    this->SetAppTitle("Viewer App");
    this->SetCompanyName("Nebula");
}

//------------------------------------------------------------------------------
/**
*/
SimpleViewerApplication::~SimpleViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
SimpleViewerApplication::Open()
{
    if (Application::Open())
    {
#if __NEBULA_HTTP__

		// setup debug subsystem
		this->debugInterface = Debug::DebugInterface::Create();
		this->debugInterface->Open();
#endif
        this->gfxServer = GraphicsServer::Create();
        this->resMgr = Resources::ResourceManager::Create();
        this->inputServer = Input::InputServer::Create();
        this->ioServer = IO::IoServer::Create();

#if NEBULA_ENABLE_PROFILING
        Profiling::ProfilingRegisterThread();
#endif
        
#if __WIN32__
        //Ptr<IO::LogFileConsoleHandler> logFileHandler = IO::LogFileConsoleHandler::Create();
        //IO::Console::Instance()->AttachHandler(logFileHandler.upcast<IO::ConsoleHandler>());
#endif

        this->resMgr->Open();
        this->inputServer->Open();
        this->gfxServer->Open();

        SizeT width = this->GetCmdLineArgs().GetInt("-w", 1680);
        SizeT height = this->GetCmdLineArgs().GetInt("-h", 1050);

        CoreGraphics::WindowCreateInfo wndInfo =
        {
            CoreGraphics::DisplayMode{ 100, 100, width, height },
            this->GetAppTitle(), "", CoreGraphics::AntiAliasQuality::None, true, true, false
        };
        this->wnd = CreateWindow(wndInfo);
		this->cam = Graphics::CreateEntity();

        // create contexts, this could and should be bundled together
        CameraContext::Create();
        ModelContext::Create();
        Characters::CharacterContext::Create();

		Particles::ParticleContext::Create();

		// make sure all bounding box modifying contexts are created before the observer contexts
        ObserverContext::Create();
        ObservableContext::Create();

		Graphics::RegisterEntity<CameraContext, ObserverContext>(this->cam);
		CameraContext::SetupProjectionFov(this->cam, width / (float)height, Math::n_deg2rad(60.f), 0.1f, 1000.0f);

		Clustering::ClusterContext::Create(0.1f, 1000.0f, this->wnd);
		Lighting::LightContext::Create();
		Im3d::Im3dContext::Create();
		Dynui::ImguiContext::Create();


		this->view = gfxServer->CreateView("mainview", "frame:vkdefault.json"_uri);
		this->stage = gfxServer->CreateStage("stage1", true);

        Im3d::Im3dContext::SetGridStatus(true);
        Im3d::Im3dContext::SetGridSize(1.0f, 25);
        Im3d::Im3dContext::SetGridColor(Math::float4(0.2f, 0.2f, 0.2f, 0.8f));

		this->globalLight = Graphics::CreateEntity();
		Lighting::LightContext::RegisterEntity(this->globalLight);
		Lighting::LightContext::SetupGlobalLight(this->globalLight, Math::float4(1, 1, 1, 0), 1.0f, Math::float4(0, 0, 0, 0), Math::float4(0, 0, 0, 0), 0.0f, -Math::vector(1, 1, 1), true);

        this->ResetCamera();
        CameraContext::SetTransform(this->cam, this->mayaCameraUtil.GetCameraTransform());

        this->view->SetCamera(this->cam);
        this->view->SetStage(this->stage);

        // register visibility system
        ObserverContext::CreateBruteforceSystem({});

        ObserverContext::Setup(this->cam, VisibilityEntityType::Camera);

        // create environment context for the atmosphere effects
		EnvironmentContext::Create(this->globalLight);

        this->UpdateCamera();

        this->frametimeHistory.Fill(0, 120, 0.0f);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::Close()
{
	App::Application::Close();
    DestroyWindow(this->wnd);
    this->gfxServer->DiscardStage(this->stage);
    this->gfxServer->DiscardView(this->view);
    ObserverContext::Discard();
    Lighting::LightContext::Discard();

    this->gfxServer->Close();
    this->inputServer->Close();
    this->resMgr->Close();
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::Run()
{    
    bool run = true;

    const Ptr<Input::Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    const Ptr<Input::Mouse>& mouse = inputServer->GetDefaultMouse();
    
    scenes[currentScene]->Open();

    while (run && !inputServer->IsQuitRequested())
    {

        /*
        Math::matrix44 globalLightTransform = Lighting::LightContext::GetTransform(globalLight);
        Math::matrix44 rotY = Math::matrix44::rotationy(Math::n_deg2rad(0.1f));
        Math::matrix44 rotX = Math::matrix44::rotationz(Math::n_deg2rad(0.05f));
        globalLightTransform = globalLightTransform * rotX * rotY;
        Lighting::LightContext::SetTransform(globalLight, globalLightTransform);
        */

#if NEBULA_ENABLE_PROFILING
        if (!this->pauseProfiling)
            this->profilingContexts = Profiling::ProfilingGetContexts();
        Profiling::ProfilingNewFrame();
#endif

        N_MARKER_BEGIN(Input, App);
        this->inputServer->BeginFrame();
        this->inputServer->OnFrame();
        N_MARKER_END();

        N_MARKER_BEGIN(Resources, App);
        this->resMgr->Update(this->frameIndex);
        N_MARKER_END();

#if NEBULA_ENABLE_PROFILING
        // copy because the information has been discarded when we render UI
        if (!this->pauseProfiling)
            this->frameProfilingMarkers = CoreGraphics::GetProfilingMarkers();
#endif NEBULA_ENABLE_PROFILING

        N_MARKER_BEGIN(BeginFrame, App);
		this->gfxServer->BeginFrame();
        N_MARKER_END();

        scenes[currentScene]->Run();

        // put game code which doesn't need visibility data or animation here
        N_MARKER_BEGIN(BeforeViews, App);
        this->gfxServer->BeforeViews();
        N_MARKER_END();
        this->RenderUI();              

        N_MARKER_BEGIN(Draw, App);
        if (this->renderDebug)
        {
            this->gfxServer->RenderDebug(0);
        }
        
        // put game code which need visibility data here
        this->gfxServer->RenderViews();
        N_MARKER_END();

        N_MARKER_BEGIN(AfterDraw, App);

        // put game code which needs rendering to be done (animation etc) here
        this->gfxServer->EndViews();

        // do stuff after rendering is done
        this->gfxServer->EndFrame();
        N_MARKER_END();

        // force wait immediately
        N_MARKER_BEGIN(Present, App);
        WindowPresent(wnd, frameIndex);
        N_MARKER_END();

        if (keyboard->KeyPressed(Input::Key::Escape)) run = false;
                
        if (keyboard->KeyPressed(Input::Key::LeftMenu))
            this->UpdateCamera();
        
		if (keyboard->KeyPressed(Input::Key::F8))
			Resources::ResourceManager::Instance()->ReloadResource("shd:imgui.fxb");

        if (keyboard->KeyDown(Input::Key::F3))
            this->pauseProfiling = !this->pauseProfiling;

        frameIndex++;             
        this->inputServer->EndFrame();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RecursiveDrawScope(const Profiling::ProfilingScope& scope, ImDrawList* drawList, const ImVec2 start, const ImVec2 fullSize, ImVec2 pos, const ImVec2 canvas, const float frameTime, const int level)
{
    static const ImU32 colors[] =
    {
        IM_COL32(200, 50, 50, 255),
        IM_COL32(50, 200, 50, 255),
        IM_COL32(50, 50, 200, 255),
        IM_COL32(200, 50, 200, 255),
        IM_COL32(50, 200, 200, 255),
        IM_COL32(200, 200, 50, 255),
    };
    static const float YPad = 20.0f;
    static const float TextPad = 5.0f;

    // convert to milliseconds
    float startX = pos.x + scope.start / frameTime * canvas.x;
    float stopX = startX + Math::n_max(scope.duration / frameTime * canvas.x, 1.0);
    float startY = pos.y;
    float stopY = startY + YPad;

    ImVec2 bbMin = ImVec2(startX, startY);
    ImVec2 bbMax = ImVec2(Math::n_min(stopX, pos.x + fullSize.x), Math::n_min(stopY, startY + fullSize.y));

    // draw a filled rect for background, and normal rect for outline
    drawList->PushClipRect(bbMin, bbMax, true);
    drawList->AddRectFilled(bbMin, bbMax, colors[level], 0.0f);
    drawList->AddRect(bbMin, bbMax, IM_COL32(128, 128, 128, 128), 0.0f);

    // make sure text appears inside the box
    Util::String text = Util::String::Sprintf("%s (%4.4f ms)", scope.name, scope.duration * 1000);
    drawList->AddText(ImVec2(startX + TextPad, pos.y), IM_COL32_BLACK, text.AsCharPtr());
    drawList->PopClipRect();

    if (ImGui::IsMouseHoveringRect(bbMin, bbMax))
    {
        ImGui::BeginTooltip();
        Util::String text = Util::String::Sprintf("%s [%s] (%4.4f ms) in %s (%d)", scope.name, scope.category.Value(), scope.duration * 1000, scope.file, scope.line);
        ImGui::Text(text.AsCharPtr());
        ImGui::EndTooltip();
        ImVec2 l1 = bbMax;
        l1.y = start.y;
        ImVec2 l2 = bbMax;
        l2.y = fullSize.y;
        drawList->PushClipRect(start, fullSize);
        drawList->AddLine(l1, l2, IM_COL32(255, 0, 0, 255), 1.0f);
        drawList->PopClipRect();
    }

    // move next element down one level
    pos.y += YPad;
    for (IndexT i = 0; i < scope.children.Size(); i++)
    {
        RecursiveDrawScope(scope.children[i], drawList, start, fullSize, pos, canvas, frameTime, level + 1);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
RecursiveDrawGpuMarker(const CoreGraphics::FrameProfilingMarker& marker, ImDrawList* drawList, const ImVec2 start, const ImVec2 fullSize, ImVec2 pos, const ImVec2 canvas, const float frameTime, const int level)
{
    static const ImU32 colors[] =
    {
        IM_COL32(200, 50, 50, 255),
        IM_COL32(50, 200, 50, 255),
        IM_COL32(50, 50, 200, 255),
        IM_COL32(200, 50, 200, 255),
        IM_COL32(50, 200, 200, 255),
        IM_COL32(200, 200, 50, 255),
    };
    static const float YPad = 20.0f;
    static const float TextPad = 5.0f;

    // convert to milliseconds
    float begin = marker.start / 1000000000.0f;
    float duration = marker.duration / 1000000000.0f;
    float startX = pos.x + begin / frameTime * canvas.x;
    float stopX = startX + Math::n_max(duration / frameTime * canvas.x, 1.0f);
    float startY = pos.y + 125.0f * marker.queue;
    float stopY = startY + YPad;

    ImVec2 bbMin = ImVec2(startX, startY);
    ImVec2 bbMax = ImVec2(Math::n_min(stopX, pos.x + fullSize.x), Math::n_min(stopY, startY + fullSize.y));

    // draw a filled rect for background, and normal rect for outline
    drawList->PushClipRect(bbMin, bbMax, true);
    drawList->AddRectFilled(bbMin, bbMax, colors[level], 0.0f);
    drawList->AddRect(bbMin, bbMax, IM_COL32(128, 128, 128, 128), 0.0f);

    // make sure text appears inside the box
    Util::String text = Util::String::Sprintf("%s (%4.4f ms)", marker.name, duration * 1000);
    drawList->AddText(ImVec2(startX + TextPad, startY), IM_COL32_BLACK, text.AsCharPtr());
    drawList->PopClipRect();

    if (ImGui::IsMouseHoveringRect(bbMin, bbMax))
    {
        ImGui::BeginTooltip();
        Util::String text = Util::String::Sprintf("%s (%4.4f ms)", marker.name, duration * 1000);
        ImGui::Text(text.AsCharPtr());
        ImGui::EndTooltip();
    }

    // move next element down one level
    pos.y += YPad;
    for (IndexT i = 0; i < marker.children.Size(); i++)
    {
        RecursiveDrawGpuMarker(marker.children[i], drawList, start, fullSize, pos, canvas, frameTime, level + 1);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::RenderUI()
{
    N_SCOPE(UpdateUI, App);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, { 0,0,0,0.15f });
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Scenes"))
        {
            int i = 0;
            for (auto scene : scenes)
            {
                bool isSelected = (i == currentScene);
                if (ImGui::MenuItem(scene->name, nullptr, &isSelected))
                {
                    if (i != currentScene)
                    {
                        scenes[currentScene]->Close();
                        currentScene = i;
                        scenes[currentScene]->Open();
                    }
                }
                i++;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            ImGui::MenuItem("Camera Window", nullptr, &this->showCameraWindow);
            ImGui::MenuItem("Frame Profiler", nullptr, &this->showFrameProfiler);
            ImGui::MenuItem("Scene UI", nullptr, &this->showSceneUI);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor();

    if (!this->pauseProfiling)
    {
        this->currentFrameTime = (float)this->gfxServer->GetFrameTime();
        this->averageFrameTime += this->currentFrameTime;
        this->frametimeHistory.Append(this->currentFrameTime);
        if (this->frametimeHistory.Size() > 120)
            this->frametimeHistory.EraseFront();

        if (this->gfxServer->GetFrameIndex() % 35 == 0)
        {
            this->prevAverageFrameTime = this->averageFrameTime / 35.0f;
            this->averageFrameTime = 0.0f;
        }
    }

    if (this->showCameraWindow)
    {
        ImGui::Begin("Viewer", &showCameraWindow, 0);

        ImGui::SetWindowSize(ImVec2(240, 400));
        if (ImGui::CollapsingHeader("Camera mode", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::RadioButton("Maya", &this->cameraMode, 0))this->ToMaya();
            ImGui::SameLine();
            if (ImGui::RadioButton("Free", &this->cameraMode, 1))this->ToFree();
            ImGui::SameLine();
            if (ImGui::Button("Reset")) this->ResetCamera();
        }
        ImGui::Checkbox("Debug Rendering", &this->renderDebug);

        ImGui::End();
    }

    if (this->showSceneUI)
    {
        scenes[currentScene]->RenderUI();
    }

    if (this->showFrameProfiler)
    {
        Debug::FrameScriptInspector::Run(this->view->GetFrameScript());
        ImGui::Begin("Performance Profiler", &this->showFrameProfiler);
        {
            ImGui::Text("ms - %.2f\nFPS - %.2f", this->prevAverageFrameTime * 1000, 1 / this->prevAverageFrameTime);
            ImGui::PlotLines("Frame Times", &this->frametimeHistory[0], this->frametimeHistory.Size(), 0, 0, FLT_MIN, FLT_MAX, { ImGui::GetContentRegionAvail().x, 90 });
            ImGui::Separator();
            ImGui::Checkbox("Fixed FPS", &this->profileFixedFps);
            if (this->profileFixedFps)
            {
                ImGui::InputInt("FPS", &this->fixedFps);
                this->currentFrameTime = 1 / float(this->fixedFps);
            }

#if NEBULA_ENABLE_PROFILING
            if (ImGui::CollapsingHeader("Profiler"))
            {

                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 start = ImGui::GetCursorScreenPos();
                ImVec2 fullSize = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y);
                for (const Profiling::ProfilingContext& ctx : this->profilingContexts)
                {
                    if (ImGui::CollapsingHeader(ctx.threadName.Value()))
                    {
                        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
                        ImVec2 pos = ImGui::GetCursorScreenPos();
                        ImGui::InvisibleButton("canvas", ImVec2(canvasSize.x, 100.0f));
                        for (IndexT i = 0; i < ctx.topLevelScopes.Size(); i++)
                        {
                            const Profiling::ProfilingScope& scope = ctx.topLevelScopes[i];
                            RecursiveDrawScope(scope, drawList, start, fullSize, pos, canvasSize, this->currentFrameTime, 0);
                        }
                    }
                }
                if (ImGui::CollapsingHeader("GPU"))
                {
                    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    ImGui::InvisibleButton("canvas", ImVec2(canvasSize.x, 250.0f));
                    drawList->AddText(ImVec2(pos.x, pos.y), IM_COL32_WHITE, "Graphics Queue");
                    drawList->AddText(ImVec2(pos.x, pos.y + 125), IM_COL32_WHITE, "Compute Queue");
                    pos.y += 25.0f;
                    const Util::Array<CoreGraphics::FrameProfilingMarker>& frameMarkers = this->frameProfilingMarkers;
                    for (int i = 0; i < frameMarkers.Size(); i++)
                    {
                        const CoreGraphics::FrameProfilingMarker& marker = frameMarkers[i];
                        RecursiveDrawGpuMarker(marker, drawList, start, fullSize, pos, canvasSize, this->currentFrameTime, 0);
                    }
                }
            }
#endif NEBULA_ENABLE_PROFILING
        }
        ImGui::End();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::UpdateCamera()
{
    const Ptr<Input::Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    const Ptr<Input::Mouse>& mouse = inputServer->GetDefaultMouse();

    this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(Input::MouseButton::LeftButton));
    this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(Input::MouseButton::MiddleButton));
    this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(Input::MouseButton::RightButton));
    this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
    this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
    this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());

    // process keyboard input
    Math::float4 pos(0.0f);
    if (keyboard->KeyDown(Input::Key::Space))
    {
        this->mayaCameraUtil.Reset();
    }
    if (keyboard->KeyPressed(Input::Key::Left))
    {
        panning.x() -= 0.1f;
        pos.x() -= 0.1f;
    }
    if (keyboard->KeyPressed(Input::Key::Right))
    {
        panning.x() += 0.1f;
        pos.x() += 0.1f;
    }
    if (keyboard->KeyPressed(Input::Key::Up))
    {
        panning.y() -= 0.1f;
        if (keyboard->KeyPressed(Input::Key::LeftShift))
        {
            pos.y() -= 0.1f;
        }
        else
        {
            pos.z() -= 0.1f;
        }
    }
    if (keyboard->KeyPressed(Input::Key::Down))
    {
        panning.y() += 0.1f;
        if (keyboard->KeyPressed(Input::Key::LeftShift))
        {
            pos.y() += 0.1f;
        }
        else
        {
            pos.z() += 0.1f;
        }
    }


    this->mayaCameraUtil.SetPanning(panning);
    this->mayaCameraUtil.SetOrbiting(orbiting);
    this->mayaCameraUtil.SetZoomIn(zoomIn);
    this->mayaCameraUtil.SetZoomOut(zoomOut);
    this->mayaCameraUtil.Update();

    
    this->freeCamUtil.SetForwardsKey(keyboard->KeyPressed(Input::Key::W));
    this->freeCamUtil.SetBackwardsKey(keyboard->KeyPressed(Input::Key::S));
    this->freeCamUtil.SetRightStrafeKey(keyboard->KeyPressed(Input::Key::D));
    this->freeCamUtil.SetLeftStrafeKey(keyboard->KeyPressed(Input::Key::A));
    this->freeCamUtil.SetUpKey(keyboard->KeyPressed(Input::Key::Q));
    this->freeCamUtil.SetDownKey(keyboard->KeyPressed(Input::Key::E));

    this->freeCamUtil.SetMouseMovement(mouse->GetMovement());
    this->freeCamUtil.SetAccelerateButton(keyboard->KeyPressed(Input::Key::LeftShift));

    this->freeCamUtil.SetRotateButton(mouse->ButtonPressed(Input::MouseButton::LeftButton));
    this->freeCamUtil.Update();
    
    switch (this->cameraMode)
    {
    case 0:
        CameraContext::SetTransform(this->cam, Math::matrix44::inverse(this->mayaCameraUtil.GetCameraTransform()));
        break;
    case 1:
        CameraContext::SetTransform(this->cam, Math::matrix44::inverse(this->freeCamUtil.GetTransform()));
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::ResetCamera()
{
    this->freeCamUtil.Setup(this->defaultViewPoint, Math::float4::normalize(this->defaultViewPoint));
    this->freeCamUtil.Update();
    this->mayaCameraUtil.Setup(Math::point(0.0f, 0.0f, 0.0f), this->defaultViewPoint, Math::vector(0.0f, 1.0f, 0.0f));
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::ToMaya()
{
    this->mayaCameraUtil.Setup(this->mayaCameraUtil.GetCenterOfInterest(), this->freeCamUtil.GetTransform().get_position(), Math::vector(0, 1, 0));
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::ToFree()
{
    Math::float4 pos = this->mayaCameraUtil.GetCameraTransform().get_position();
    this->freeCamUtil.Setup(pos, Math::float4::normalize(pos - this->mayaCameraUtil.GetCenterOfInterest()));
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleViewerApplication::Browse()
{
    this->folders = IO::IoServer::Instance()->ListDirectories("mdl:", "*");    
    this->files = IO::IoServer::Instance()->ListFiles("mdl:" + this->folders[this->selectedFolder], "*");
}
}
