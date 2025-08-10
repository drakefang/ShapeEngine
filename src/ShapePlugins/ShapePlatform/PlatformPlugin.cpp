//
// Created by yuyzc on 2025/7/27.
//

#include "PlatformPlugin.h"

#include "PlatformDefine.h"
#include "PlatformModule.h"
#include "App/Application.h"
#include "Core/Logger.h"
#include "Module/ModuleManager.h"
#include "Module/PluginModuleRegistry.h"
#include "Service/ServiceLocator.h"


namespace ShapePlatform
{
    void PlatformPlugin::Shutdown()
    {
        ShapeEngine::Logger()->info("{} shutting down...");
    }

    const std::string& PlatformPlugin::GetName() const
    {
        return name;
    }

    uint32_t PlatformPlugin::Version() const
    {
        return 0;
    }

    void PlatformPlugin::OnLoad()
    {
    }

    void PlatformPlugin::OnUnLoad()
    {
    }
}

IMPLEMENT_PLUGIN(SHAPE_PLATFORM_API, ShapePlatform::PlatformPlugin);