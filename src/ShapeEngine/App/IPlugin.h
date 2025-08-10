//
// Created by yuyzc on 2025/7/1.
//

#pragma once
#include <string>

#include "Core/PlatformDefine.h"


namespace ShapeEngine
{
    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;
        virtual void Startup() = 0;
        virtual void Shutdown() = 0;
        virtual const std::string GetName() const = 0;
        virtual uint32_t Version() const = 0;
        virtual void OnLoad() = 0;
        virtual void OnUnLoad() = 0;
    };
}

#define IMPLEMENT_PLUGIN(API_MACRO, PluginClass) \
extern "C" { \
API_MACRO ShapeEngine::IPlugin* CreatePlugin() { return new PluginClass(); } \
}