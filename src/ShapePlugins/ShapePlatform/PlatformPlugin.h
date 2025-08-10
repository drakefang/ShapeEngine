//
// Created by yuyzc on 2025/7/27.
//

#pragma once

#include "App/PluginBase.h"

namespace ShapePlatform
{
    class PlatformPlugin final : public ShapeEngine::PluginBase<PlatformPlugin>
    {
    public:
        virtual void Shutdown() override;

        const std::string& GetName() const override;
        uint32_t Version() const override;
        void OnLoad() override;
        void OnUnLoad() override;

    private:
        std::string name;
    };
}