//
// Created by yuyzc on 2025/8/4.
//

#pragma once

struct SDL_Window;
// Forward declare bgfx::PlatformData
namespace bgfx { struct PlatformData; }

namespace ShapeGame {
    inline bgfx::PlatformData GetPlatformData(SDL_Window* window);
} // namespace ShapeGame