//
// Created by yuyzc on 2025/7/24.
//

#include "DynamicLibrary.h"
#include "Core/Logger.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace ShapeEngine
{
    DynamicLibrary::DynamicLibrary(const std::filesystem::path& path)
    {
#if defined(_WIN32)
        // 在Windows上，使用LoadLibraryW以支持Unicode路径
        Handle = LoadLibraryW(path.c_str());
        if (!Handle)
        {
            Logger()->error("Failed to load DLL: {}. Win32 Error Code: {}", path.string(), GetLastError());
        }
#else
        m_Handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!m_Handle)
        {
            Logger()->error("Failed to load shared library: {}. Error: {}", path.string(), dlerror());
        }
#endif
    }

    DynamicLibrary::~DynamicLibrary()
    {
        if (Handle)
        {
#if defined(_WIN32)
            FreeLibrary(static_cast<HMODULE>(Handle));
#else
            dlclose(m_Handle);
#endif
        }
    }

    DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept
        : Handle(other.Handle)
    {
        other.Handle = nullptr;
    }

    DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept
    {
        if (this != &other)
        {
            if (Handle)
            {
#if defined(_WIN32)
                FreeLibrary(static_cast<HMODULE>(Handle));
#else
                dlclose(m_Handle);
#endif
            }
            Handle = other.Handle;
            other.Handle = nullptr;
        }
        return *this;
    }

    bool DynamicLibrary::IsValid() const
    {
        return Handle != nullptr;
    }

    void* DynamicLibrary::GetSymbol(const std::string& symbolName) const
    {
        if (!Handle)
        {
            return nullptr;
        }
#if defined(_WIN32)
        return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(Handle), symbolName.c_str()));
#else
        return dlsym(m_Handle, symbolName.c_str());
#endif
    }
}