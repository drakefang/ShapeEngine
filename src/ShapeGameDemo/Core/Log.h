//
// Created by yuyzc on 2025/8/4.
//

#pragma once
#include <memory>
#include "spdlog/spdlog.h"

namespace ShapeGame
{
    class Log
    {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    private:
        static inline std::shared_ptr<spdlog::logger> s_CoreLogger = nullptr;
    };
}

#define SG_TRACE(...)    ::ShapeGame::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SG_INFO(...)     ::ShapeGame::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SG_WARN(...)     ::ShapeGame::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SG_ERROR(...)    ::ShapeGame::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SG_CRITICAL(...) ::ShapeGame::Log::GetCoreLogger()->critical(__VA_ARGS__)