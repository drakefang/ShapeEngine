

#include "SDL3/SDL_main.h"
#include "SDL3/SDL.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using EngineMainFunc = int (*)(int, char**);

void* LoadEngineLibrary(const char* libName)
{
#if defined(_WIN32)
    return LoadLibraryA(libName);
#else
    return dlopen(libName, RTLD_LAZY | RTLD_GLOBAL);
#endif
}

EngineMainFunc GetEngineEntryPoint(void* libraryHandle)
{
#if defined(_WIN32)
    return reinterpret_cast<EngineMainFunc>(GetProcAddress(static_cast<HMODULE>(libraryHandle), "EngineMain"));
#else
    return (EngineMainFunc)dlsym(libraryHandle, "EngineMain");
#endif
}

void FreeEngineLibrary(void* libraryHandle)
{
#if defined(_WIN32)
    FreeLibrary(static_cast<HMODULE>(libraryHandle));
#else
    dlclose(libraryHandle);
#endif
}

int main(int argc, char* argv[])
{
#if defined(_WIN32)
    const char* engineLibName = "ShapeEngine.dll";
#elif defined(__APPLE__)
    const char* engineLibName = "libShapeEngine.dylib";
#else
    const char* engineLibName = "libShapeEngine.so";
#endif

    void* engineDll = LoadEngineLibrary(engineLibName);
    if (!engineDll)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Fatal Error",
                                 "Failed to load ShapeEngine library!",
                                 nullptr);
        return -1;
    }

    const EngineMainFunc engineMain = GetEngineEntryPoint(engineDll);
    if (!engineMain)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Fatal Error",
                                 "Failed to find EngineMain entry point in ShapeEngine library!",
                                 nullptr);
        FreeEngineLibrary(engineDll);
        return -1;
    }

    const int result = engineMain(argc, argv);

    FreeEngineLibrary(engineDll);

    return result;
}