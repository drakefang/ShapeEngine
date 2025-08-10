//
// Created by yuyzc on 2025/8/10.
//

#pragma once

#include <vector>
#include <string>

namespace ShapeEngine
{
    class ISubSystem
    {
    public:
        virtual ~ISubSystem() = default;

        virtual void Startup() {}

        virtual void Shutdown() {}

        virtual std::vector<std::string> GetDependencies() const { return {}; }

        virtual const char* GetName() const = 0;
    };

    class IEngineSubSystem : public ISubSystem
    {

    };
}