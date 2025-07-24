//
// Created by yuyzc on 2025/7/24.
//

#pragma once

#include <filesystem>
#include <string>

#include "Core/PlatformDefine.h"

namespace ShapeEngine
{
    class SHAPE_ENGINE_API DynamicLibrary
    {
    public:
        explicit DynamicLibrary(const std::filesystem::path& path);

        ~DynamicLibrary();

        DynamicLibrary(const DynamicLibrary&) = delete;
        DynamicLibrary& operator=(const DynamicLibrary&) = delete;

        DynamicLibrary(DynamicLibrary&& other) noexcept;
        DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

        bool IsValid() const;

        void* GetSymbol(const std::string& symbolName) const;

    private:
        void* Handle = nullptr;
    };
}
