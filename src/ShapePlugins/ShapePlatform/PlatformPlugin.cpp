//
// Created by yuyzc on 2025/7/27.
//

#include "PlatformPlugin.h"

#include "PlatformDefine.h"
#include "PlatformModule.h"
#include "Module/ModuleManager.h"


namespace ShapePlatform
{
    void PlatformPlugin::Startup()
    {
        ShapeEngine::ModuleManager::Get().RegisterModule("PlatformSDL", []() {
            return std::make_unique<PlatformModule>();
        });
    }

    void PlatformPlugin::Shutdown()
    {
    }

    std::string_view PlatformPlugin::GetName()
    {
        return "ShapePlatform";
    }
}

IMPLEMENT_PLUGIN(SHAPE_PLATFORM_API, ShapePlatform::PlatformPlugin);