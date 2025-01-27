#pragma once
//------------------------------------------------------------------------------
/**
    @file coregraphics/shadersemantics.h
    
    Standard shader variable semantic names.
    
    @copyright
    (C) 2009 Radon Labs GmbH
    (C) 2013-2020 Individual contributors, see AUTHORS file
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#define NEBULA_SEMANTIC_CHARACTERINDEX             "CharacterIndex"
#define NEBULA_SEMANTIC_JOINTPALETTE               "JointPalette"
#define NEBULA_SEMANTIC_JOINTBUFFER                "JointBuffer"
#define NEBULA_SEMANTIC_JOINTBLOCK                 "JointBlock"
#define NEBULA_SEMANTIC_MODELVIEWPROJECTION        "ModelViewProjection"
#define NEBULA_SEMANTIC_INVVIEWPROJECTION          "InvViewProjection"
#define NEBULA_SEMANTIC_MODEL                      "Model"
#define NEBULA_SEMANTIC_VIEW                       "View"
#define NEBULA_SEMANTIC_MODELVIEW                  "ModelView"
#define NEBULA_SEMANTIC_INVMODEL                   "InvModel"
#define NEBULA_SEMANTIC_INVVIEW                    "InvView"
#define NEBULA_SEMANTIC_INVMODELVIEW               "InvModelView"
#define NEBULA_SEMANTIC_VIEWPROJECTION             "ViewProjection"
#define NEBULA_SEMANTIC_EYEPOS                     "EyePos"
#define NEBULA_SEMANTIC_FOCALLENGTHNEARFAR         "FocalLengthNearFar"
#define NEBULA_SEMANTIC_PROJECTION                 "Projection"
#define NEBULA_SEMANTIC_INVPROJECTION              "InvProjection"
#define NEBULA_SEMANTIC_VIEWMATRIXARRAY            "ViewMatrixArray"
#define NEBULA_SEMANTIC_PIXELSIZE                  "PixelSize"
#define NEBULA_SEMANTIC_HALFPIXELSIZE              "HalfPixelSize"
#define NEBULA_SEMANTIC_LIGHTPOSRANGE              "LightPosRange"
#define NEBULA_SEMANTIC_LIGHTCOLOR                 "LightColor"
#define NEBULA_SEMANTIC_LIGHTSHADOWBIAS            "LightShadowBias"
#define NEBULA_SEMANTIC_GLOBALLIGHTDIRWORLDSPACE   "GlobalLightDirWorldspace"
#define NEBULA_SEMANTIC_GLOBALLIGHTCOLOR           "GlobalLightColor"
#define NEBULA_SEMANTIC_GLOBALAMBIENTLIGHTCOLOR    "GlobalAmbientLightColor"
#define NEBULA_SEMANTIC_GLOBALLIGHTSHADOWBIAS      "GlobalLightShadowBias"
#define NEBULA_SEMANTIC_LIGHTPROJTRANSFORM         "LightProjTransform"   
#define NEBULA_SEMANTIC_LIGHTTRANSFORM             "LightTransform"   
#define NEBULA_SEMANTIC_SHADOWTRANSFORM            "ShadowTransform"
#define NEBULA_SEMANTIC_SHADOWPROJTRANSFORM        "ShadowProjTransform"
#define NEBULA_SEMANTIC_INVERSELIGHTPROJECTION     "InvLightProj"
#define NEBULA_SEMANTIC_SHADOWOFFSETSCALE          "ShadowOffsetScale"
#define NEBULA_SEMANTIC_SHADOWCONSTANTS            "ShadowConstants"
#define NEBULA_SEMANTIC_SHADOWINTENSITY            "ShadowIntensity"
#define NEBULA_SEMANTIC_CASTSHADOWS                "CastShadows"
#define NEBULA_SEMANTIC_FADEVALUE                  "FadeValue"
#define NEBULA_SEMANTIC_SATURATION                 "Saturation"
#define NEBULA_SEMANTIC_BALANCE                    "Balance"
#define NEBULA_SEMANTIC_MAXLUMINANCE               "MaxLuminance"
#define NEBULA_SEMANTIC_FOGCOLOR                   "FogColor"
#define NEBULA_SEMANTIC_FOGDISTANCES               "FogDistances"
#define NEBULA_SEMANTIC_HDRBLOOMCOLOR              "HDRBloomColor"
#define NEBULA_SEMANTIC_HDRBLOOMSCALE              "HDRBloomScale"
#define NEBULA_SEMANTIC_HDRBRIGHTPASSTHRESHOLD     "HDRBrightPassThreshold"
#define NEBULA_SEMANTIC_DOFDISTANCES               "DoFDistances"
#define NEBULA_SEMANTIC_EMITTERTRANSFORM           "EmitterTransform"
#define NEBULA_SEMANTIC_BILLBOARD                  "Billboard"
#define NEBULA_SEMANTIC_BBOXCENTER                 "BBoxCenter"
#define NEBULA_SEMANTIC_BBOXSIZE                   "BBoxSize"
#define NEBULA_SEMANTIC_TIME                       "Time"
#define NEBULA_SEMANTIC_TIMEDIFF                   "TimeDiff"
#define NEBULA_SEMANTIC_TIMEANDRANDOM              "Time_Random_Luminance_X"
#define NEBULA_SEMANTIC_RANDOM                     "Random"
#define NEBULA_SEMANTIC_OBJECTID                   "ObjectId"
#define NEBULA_SEMANTIC_ANIMPHASES                 "NumAnimPhases"
#define NEBULA_SEMANTIC_ANIMSPERSEC                "AnimFramesPerSecond"
#define NEBULA_SEMANTIC_UVTOVIEWA                  "UVToViewA"
#define NEBULA_SEMANTIC_UVTOVIEWB                  "UVToViewB"
#define NEBULA_SEMANTIC_R                          "R"
#define NEBULA_SEMANTIC_R2                         "R2"
#define NEBULA_SEMANTIC_NEGINVR2                   "NegInvR2"
#define NEBULA_SEMANTIC_PIXELFOCALLENGTH           "FocalLength"
#define NEBULA_SEMANTIC_AORESOLUTION               "AOResolution"
#define NEBULA_SEMANTIC_INVAORESOLUTION            "InvAOResolution"
#define NEBULA_SEMANTIC_MAXRADIUSPIXELS            "MaxRadiusPixels"
#define NEBULA_SEMANTIC_STRENGHT                   "Strength"
#define NEBULA_SEMANTIC_TANANGLEBIAS               "TanAngleBias"
#define NEBULA_SEMANTIC_POWEREXPONENT              "PowerExponent"
#define NEBULA_SEMANTIC_FALLOFF                    "BlurFalloff"
#define NEBULA_SEMANTIC_DEPTHTHRESHOLD             "BlurDepthThreshold"
#define NEBULA_SEMANTIC_COLORSOURCE                "ColorSource"
#define NEBULA_SEMANTIC_LIGHTPOS                   "LightPos"
#define NEBULA_SEMANTIC_DENSITY                    "Density"
#define NEBULA_SEMANTIC_DECAY                      "Decay"
#define NEBULA_SEMANTIC_WEIGHT                     "Weight"
#define NEBULA_SEMANTIC_EXPOSURE                   "Exposure"
#define NEBULA_SEMANTIC_LIGHTTEXTURE               "LightTexture"
#define NEBULA_SEMANTIC_WORLDVIEWPROJ              "WorldViewProjection"
#define NEBULA_SEMANTIC_WORLD                      "World"
#define NEBULA_SEMANTIC_WORLDVIEW                  "WorldView"
#define NEBULA_SEMANTIC_CSMSHADOWMATRIX            "CSMShadowMatrix"
#define NEBULA_SEMANTIC_CASCADEOFFSET              "CascadeOffset"
#define NEBULA_SEMANTIC_CASCADESCALE               "CascadeScale"
#define NEBULA_SEMANTIC_CASCADELEVELS              "CascadeLevels"
#define NEBULA_SEMANTIC_PCFBLURSTART               "PCFBlurForLoopStart"
#define NEBULA_SEMANTIC_PCFBLUREND                 "PCFBlurForLoopEnd"
#define NEBULA_SEMANTIC_MINBORDERPADDING           "MinBorderPadding"
#define NEBULA_SEMANTIC_MAXBORDERPADDING           "MaxBorderPadding"
#define NEBULA_SEMANTIC_SHADOWPARTITIONSIZE        "ShadowPartitionSize"
#define NEBULA_SEMANTIC_CASCADEBLENDAREA           "CascadeBlendArea"
#define NEBULA_SEMANTIC_TEXELSIZE                  "TexelSize"
#define NEBULA_SEMANTIC_NATIVETEXELSIZEINX         "NativeTexelSizeInX"
#define NEBULA_SEMANTIC_CASCADEFRUSTUMSEYE         "CascadeFrustumsEyeSpaceDepthsFloat"
#define NEBULA_SEMANTIC_CASCADEFRUSTUMSEYE4        "CascadeFrustumsEyeSpaceDepthsFloat4"
#define NEBULA_SEMANTIC_PIXELCAMERAPOSITION        "CameraPosition"
#define NEBULA_SEMANTIC_VERTEXCAMERAPOSITION       "CameraPosition"
#define NEBULA_SEMANTIC_LIGHTDIR                   "LightDir"
#define NEBULA_SEMANTIC_CONTRAST                   "Contrast"
#define NEBULA_SEMANTIC_BRIGHTNESS                 "Brightness"
#define NEBULA_SEMANTIC_SKYBLENDFACTOR             "SkyBlendFactor"
#define NEBULA_SEMANTIC_SKYROTATIONFACTOR          "SkyRotationFactor"
#define NEBULA_SEMANTIC_SKY1                       "SkyLayer1"
#define NEBULA_SEMANTIC_SKY2                       "SkyLayer2"
#define NEBULA_SEMANTIC_ENVIRONMENT                "EnvironmentMap"
#define NEBULA_SEMANTIC_IRRADIANCE                 "IrradianceMap"
#define NEBULA_SEMANTIC_DEPTHCONEMAP               "DepthConeMap"
#define NEBULA_SEMANTIC_ENVFALLOFF                 "FalloffDistance"
#define NEBULA_SEMANTIC_ENVFALLOFFDISTANCE         "FalloffDistance"
#define NEBULA_SEMANTIC_ENVFALLOFFPOWER            "FalloffPower"
#define NEBULA_SEMANTIC_NUMENVMIPS                 "NumEnvMips"
#define NEBULA_SEMANTIC_BBOXMIN                    "BBoxMin"
#define NEBULA_SEMANTIC_BBOXMAX                    "BBoxMax"
#define NEBULA_SEMANTIC_PEROBJECT                  "PerObject"

// instancing
#define NEBULA_SEMANTIC_MODELARRAY                 "ModelArray"
#define NEBULA_SEMANTIC_MODELVIEWARRAY             "ModelViewArray"
#define NEBULA_SEMANTIC_MODELVIEWPROJECTIONARRAY   "ModelViewProjectionArray"
#define NEBULA_SEMANTIC_OBJECTIDARRAY              "ObjectIdArray"

#define NEBULA_SEMANTIC_MATDIFFUSE                 "MatDiffuse"
#define NEBULA_SEMANTIC_DEBUGSHADERLAYER           "DebugShaderLayer"
#define NEBULA_SEMANTIC_DIFFMAP0                   "DiffMap0"
#define NEBULA_SEMANTIC_DIFFMAP1                   "DiffMap1"
#define NEBULA_SEMANTIC_INTENSITY0                 "Intensity0"
#define NEBULA_SEMANTIC_INTENSITY1                 "Intensity1"
#define NEBULA_SEMANTIC_INTENSITY2                 "Intensity2"
#define NEBULA_SEMANTIC_NORMALBUFFER               "NormalBuffer"
#define NEBULA_SEMANTIC_DEPTHBUFFER                "DepthBuffer"
#define NEBULA_SEMANTIC_LIGHTBUFFER                "LightBuffer"
#define NEBULA_SEMANTIC_LIGHTPROJMAP               "LightProjMap" 
#define NEBULA_SEMANTIC_LIGHTPROJCUBE              "LightProjCube" 
#define NEBULA_SEMANTIC_SHADOWPROJMAP              "ShadowProjMap"
#define NEBULA_SEMANTIC_SHADOWPROJCUBE             "ShadowProjCube"
#define NEBULA_SEMANTIC_OBJECTID                   "ObjectId"
#define NEBULA_SEMANTIC_REPEATINDEX                "RepeatIndex"
#define NEBULA_SEMANTIC_OCCLUSIONCONSTANTS         "OcclusionConstants"
#define NEBULA_SEMANTIC_SHADOWBUFFERSIZE           "ShadowBufferSize"
#define NEBULA_SEMANTIC_RENDERTARGETDIMENSIONS     "RenderTargetDimensions"
#define NEBULA_SEMANTIC_RENDERCUBEFACE             "RenderCubeFace"

//------------------------------------------------------------------------------
