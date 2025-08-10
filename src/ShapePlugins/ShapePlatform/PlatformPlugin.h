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
        virtual void Startup() override;
        virtual void Shutdown() override;

        const std::string GetName() const override;
        uint32_t Version() const override;
        void OnLoad() override;
        void OnUnLoad() override;

    private:
        std::string name;
    };
}