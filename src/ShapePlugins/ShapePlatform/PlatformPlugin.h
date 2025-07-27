//
// Created by yuyzc on 2025/7/27.
//

#pragma once

#include "App/IPlugin.h"

namespace ShapePlatform
{
    class PlatformPlugin final : public ShapeEngine::IPlugin
    {
    public:
        virtual void Startup();
        virtual void Shutdown();
        virtual std::string_view GetName();
    };
}