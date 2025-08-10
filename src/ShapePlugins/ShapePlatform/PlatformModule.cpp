//
// Created by yuyzc on 2025/7/27.
//

#include "PlatformModule.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL.h>

#include "App/ConfigManager.h"
#include "Core/Logger.h"

using namespace ShapeEngine;

void ShapePlatform::PlatformModule::Startup()
{
    IPlatformModule::Startup();
    Logger()->info("Starting PlatformSDL Module...");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        const std::string errorMsg = "Failed to initialize SDL: " + std::string(SDL_GetError());
        Logger()->critical(errorMsg);
        throw std::runtime_error(errorMsg);
    }

    const auto& config = ConfigManager::Get();
    const auto title = config.GetValueOrDefault<std::string>("Platform.Window.Title", "ShapeEngine");
    const int width = config.GetValueOrDefault<int>("Platform.Window.Width", 1280);
    const int height = config.GetValueOrDefault<int>("Platform.Window.Height", 720);
    const bool fullscreen = config.GetValueOrDefault<bool>("Platform.Window.Fullscreen", false);

    uint32_t windowFlags = SDL_WINDOW_RESIZABLE;
    if (fullscreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    window.reset(SDL_CreateWindow(title.c_str(), width, height, windowFlags));
    if (!window)
    {
        const std::string errorMsg = "Failed to create SDL window: " + std::string(SDL_GetError());
        Logger()->critical(errorMsg);
        SDL_Quit();
        throw std::runtime_error(errorMsg);
    }

    Logger()->info("PlatformSDL Module started successfully.");
}

void ShapePlatform::PlatformModule::Shutdown()
{
    Logger()->info("Shutting down PlatformSDL Module...");
    window.reset();
    SDL_Quit();
    Logger()->info("PlatformSDL Module shut down.");
}

void ShapePlatform::PlatformModule::PumpEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        HandleSDLEvent(event);
    }
}

void* ShapePlatform::PlatformModule::GetNativeWindowHandle()
{
    if (!window) return nullptr;

#if defined(SDL_PLATFORM_WIN32)
    return SDL_GetPointerProperty(SDL_GetWindowProperties(window.get()), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(SDL_PLATFORM_MACOS)
    return SDL_GetPointerProperty(SDL_GetWindowProperties(m_Window.get()), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
    {
        Display *xdisplay = (Display*)SDL_GetPointerProperty(SDL_GetWindowProperties(m_Window.get()), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        Window xwindow = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(m_Window.get()), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    }
    else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
    {
        struct wl_display *display = (struct wl_display *)SDL_GetPointerProperty(SDL_GetWindowProperties(m_Window.get()), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        struct wl_surface *surface = (struct wl_surface *)SDL_GetPointerProperty(SDL_GetWindowProperties(m_Window.get()), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
    }
    return nullptr;
#elif defined(SDL_PLATFORM_IOS)
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    UIWindow *uiwindow = (__bridge UIWindow *)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, nullptr);
    return nullptr;
#endif
}

void ShapePlatform::PlatformModule::GetWindowSize(int& width, int& height)
{
    if (window)
    {
        SDL_GetWindowSize(window.get(), &width, &height);
    }
    else
    {
        width = 0;
        height = 0;
    }
}

void ShapePlatform::PlatformModule::SetWindowTitle(const std::string& title)
{
    if (window)
    {
        SDL_SetWindowTitle(window.get(), title.c_str());
    }
}

void ShapePlatform::PlatformModule::HandleSDLEvent(const SDL_Event& event)
{
}