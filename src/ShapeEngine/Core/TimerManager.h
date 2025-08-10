//
// Created by yuyzc on 2025/7/26.
//
#pragma once

#include "PlatformDefine.h"
#include "GameTime.h"
#include <functional>
#include <vector>
#include <cstdint>
#include <variant>
#include <coroutine>

#include "ISubsystem.h"

namespace ShapeEngine
{
    class TimerManager;

    struct TimerHandle
    {
        uint64_t ID = 0;

        [[nodiscard]] bool IsValid() const
        {
            return ID != 0;
        }

        void Validate()
        {
            ID = 0;
        }

        bool operator==(const TimerHandle& other) const
        {
            return ID == other.ID;
        }

        struct Hasher
        {
            std::size_t operator()(const TimerHandle& h) const
            {
                return std::hash<uint64_t>()(h.ID);
            }
        };
    };

    using TimerDelegate = std::function<void()>;

    struct [[nodiscard]] TimerAwaitable
    {
        float Duration;
        TimerManager* Manager;

        explicit TimerAwaitable(const float seconds, TimerManager* manager)
            : Duration(seconds), Manager(manager)
        {
        }

        [[nodiscard]] bool await_ready() const noexcept;

        void await_suspend(std::coroutine_handle<> handle) const noexcept;

        void await_resume() const noexcept
        {
        }
    };


    class SHAPE_ENGINE_API TimerManager final : public IEngineSubSystem
    {
    public:
        TimerManager() = default;

        void Tick(const GameTime& time);

        TimerHandle SetTimer(TimerDelegate delegate, float rate, bool bLoop = false, float firstDelay = -1.0f);

        TimerAwaitable WaitFor(float seconds);

        void ClearTimer(TimerHandle& handle);

        void PauseTimer(const TimerHandle& handle);

        void UnpauseTimer(const TimerHandle& handle);

        [[nodiscard]] bool IsTimerActive(const TimerHandle& handle) const;

        [[nodiscard]] float GetTimerRemaining(const TimerHandle& handle) const;

        void ClearAllTimers();

        const char* GetName() const noexcept override;

    private:
        friend struct TimerAwaitable;

        struct TimerData
        {
            TimerHandle Handle;
            std::variant<TimerDelegate, std::coroutine_handle<> > Callback;
            float Rate = 0.0f;
            float TimeRemaining = 0.0f;
            bool bIsLooping = false;
            bool bIsPaused = false;
        };

        TimerHandle SetTimer(std::coroutine_handle<> handle, float duration);

        TimerData* FindTimer(const TimerHandle& handle);

        [[nodiscard]] const TimerData* FindTimer(const TimerHandle& handle) const;

        std::vector<TimerData> Timers;
        uint64_t NextTimerID = 1;
    };
}