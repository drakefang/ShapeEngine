//
// Created by yuyzc on 2025/7/27.
//

#pragma once
#include <memory>
#include <SDL3/SDL_events.h>

#include "Module/IPlatformModule.h"

namespace ShapePlatform
{
    struct WindowDeleter
    {
        void operator()(SDL_Window* window) const
        {
            if (window)
            {
                SDL_DestroyWindow(window);
            }
        }
    };

    class PlatformModule final : public ShapeEngine::IPlatformModule
    {
    public:
        void Startup() override;
        void Shutdown() override;

        void PumpEvents() override;
        void* GetNativeWindowHandle() override;
        void GetWindowSize(int& width, int& height) override;
        void SetWindowTitle(const std::string& title) override;

        const char* GetName() const override;

    private:
        void HandleSDLEvent(const SDL_Event& event);

        std::unique_ptr<SDL_Window, WindowDeleter> window;
    };
}