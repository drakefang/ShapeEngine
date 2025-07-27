//
// Created by yuyzc on 2025/7/1.
//

#pragma once
#include <string_view>

#include "Core/PlatformDefine.h"


namespace ShapeEngine
{
    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;
        virtual void Startup() = 0;
        virtual void Shutdown() = 0;
        virtual std::string_view GetName() = 0;
    };
}

#define IMPLEMENT_PLUGIN(PluginClass) \
extern "C" { \
SHAPE_ENGINE_API ShapeEngine::IPlugin* CreatePlugin() { return new PluginClass(); } \
}