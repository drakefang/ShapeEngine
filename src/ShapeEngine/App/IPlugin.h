//
// Created by yuyzc on 2025/7/1.
//

#pragma once
#include <string_view>


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