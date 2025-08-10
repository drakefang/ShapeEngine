//
// Created by yuyzc on 2025/7/8.
//

#pragma once
#include "Module/IModuleInterface.h"

namespace ShapeEngine
{
    struct GameTime;

    class IPrimaryGameModule : public IModuleInterface
    {
    public:
        void Startup() override
        {
        }

        void Shutdown() override
        {
        }

        virtual void Tick(const GameTime& gameTime) = 0;

        [[nodiscard]] virtual std::string GetPrimaryGameModuleName() const = 0;

        [[nodiscard]] virtual std::vector<std::string> GetPrimaryGameModuleDependencies() const
        {
            return {};
        }
    };
}