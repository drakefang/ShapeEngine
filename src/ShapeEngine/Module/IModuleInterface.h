//
// Created by yuyzc on 2025/7/8.
//

#pragma once
#include <string>
#include <vector>

namespace ShapeEngine
{
    class IModule
    {
    public:
        virtual ~IModule() = default;
        virtual void Startup() {}
        virtual void Shutdown() {}

        [[nodiscard]] virtual std::vector<std::string> GetDependencies() const { return {}; }
        virtual const char* GetName() const { return "IModule"; }
    };

    class IModuleInterface : public virtual IModule
    {

    };
}