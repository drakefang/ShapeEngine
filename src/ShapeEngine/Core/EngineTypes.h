//
// Created by yuyzc on 2025/8/10.
//

#pragma once

#include <functional>
#include <memory>

namespace ShapeEngine
{
    class IModule;

    using ModuleFactoryFunc = std::function<std::shared_ptr<IModule>()>;

    enum class EModuleLoadingPhase
    {
        None,
        PreDefault,
        Default,
        PostConfig
    };
}