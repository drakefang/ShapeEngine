//
// Created by yuyzc on 2025/7/27.
//

#pragma once
#include "IModuleInterface.h"

namespace ShapeEngine
{
    class IPlatformModule : public IModuleInterface
    {
    public:
        virtual void PumpEvents() = 0;
        virtual void* GetNativeWindowHandle() = 0;
        virtual void GetWindowSize(int& width, int& height) = 0;
        virtual void SetWindowTitle(const std::string& title) = 0;
    };
}