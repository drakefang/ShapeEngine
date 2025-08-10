//
// Created by yuyzc on 2025/7/27.
//

#include "PlatformPlugin.h"

#include "PlatformDefine.h"
#include "PlatformModule.h"
#include "Core/Logger.h"
#include "Module/ModuleManager.h"
#include "Service/ServiceLocator.h"


namespace ShapePlatform
{
    void PlatformPlugin::Startup()
    {
        name = "ShapePlatform";
        ShapeEngine::ModuleManager::Get().RegisterModule("PlatformSDL", []() {
            return std::make_unique<PlatformModule>();
        });

        try
        {
            auto platformModule = ShapeEngine::LoadModuleChecked<ShapeEngine::IPlatformModule>("PlatformSDL");
            ShapeEngine::ServiceLocator::Provide<ShapeEngine::IPlatformModule>(platformModule);

            ShapeEngine::Logger()->info("PlatformSDL module loaded and registered as a service.");
        }
        catch (const std::exception& e)
        {
            ShapeEngine::Logger()->critical("Failed to load PlatformSDL module: {}", e.what());
        }
    }

    void PlatformPlugin::Shutdown()
    {
    }

    const std::string PlatformPlugin::GetName() const
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