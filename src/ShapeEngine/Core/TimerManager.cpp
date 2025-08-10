//
// Created by yuyzc on 2025/7/26.
//

#include "TimerManager.h"
#include <algorithm>

namespace ShapeEngine
{
    bool TimerAwaitable::await_ready() const noexcept
    {
        return Duration <= 0.0f;
    }

    void TimerAwaitable::await_suspend(const std::coroutine_handle<> handle) const noexcept
    {
        if (Manager)
        {
            Manager->SetTimer(handle, Duration);
        }
    }

    void TimerManager::Tick(const GameTime& time)
    {
        if (time.DeltaTime <= 0.0f)
        {
            return;
        }

        std::vector<size_t> timers_to_trigger_indices;
        for (size_t i = 0; i < Timers.size(); ++i)
        {
            if (TimerData& timer = Timers[i]; !timer.bIsPaused)
            {
                timer.TimeRemaining -= time.DeltaTime;
                if (timer.TimeRemaining <= 0.0f)
                {
                    timers_to_trigger_indices.push_back(i);
                }
            }
        }

        for (const size_t index : timers_to_trigger_indices)
        {
            if (index >= Timers.size() || Timers[index].Handle.ID == 0)
            {
                continue;
            }

            TimerData& timer = Timers[index];

            std::visit(
                [&](auto&& arg)
                {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, TimerDelegate>)
                    {
                        if (arg)
                        {
                            arg();
                        }
                    }
                    else if constexpr (std::is_same_v<T, std::coroutine_handle<> >)
                    {
                        if (arg && !arg.done())
                        {
                            arg.resume();
                        }
                    }
                },
                timer.Callback);

            if (index >= Timers.size() || Timers[index].Handle.ID == 0)
            {
                continue;
            }

            if (timer.bIsLooping)
            {
                timer.TimeRemaining += timer.Rate;
            }
            else
            {
                timer.Handle.ID = 0;
            }
        }

        std::erase_if(
            Timers,
            [](const TimerData& timer)
            {
                return !timer.Handle.IsValid();
            });
    }

    TimerHandle TimerManager::SetTimer(TimerDelegate delegate, const float rate, const bool bLoop, const float firstDelay)
    {
        const TimerHandle newHandle{NextTimerID++};
        const float initialTime = (firstDelay >= 0.0f) ? firstDelay : rate;

        Timers.push_back(
        {
            newHandle,
            std::move(delegate),
            rate,
            initialTime,
            bLoop,
            false
        });
        return newHandle;
    }

    TimerAwaitable TimerManager::WaitFor(const float seconds)
    {
        return TimerAwaitable(seconds, this);
    }

    TimerHandle TimerManager::SetTimer(std::coroutine_handle<> handle, const float duration)
    {
        const TimerHandle newHandle{NextTimerID++};
        Timers.push_back(
        {
            newHandle,
            handle,
            0.0f,
            duration,
            false,
            false
        });
        return newHandle;
    }

    void TimerManager::ClearTimer(TimerHandle& handle)
    {
        if (auto* timer = FindTimer(handle))
        {
            timer->Handle.ID = 0;
            handle.Validate();
        }
    }

    void TimerManager::PauseTimer(const TimerHandle& handle)
    {
        if (auto* timer = FindTimer(handle))
        {
            timer->bIsPaused = true;
        }
    }

    void TimerManager::UnpauseTimer(const TimerHandle& handle)
    {
        if (auto* timer = FindTimer(handle))
        {
            timer->bIsPaused = false;
        }
    }

    bool TimerManager::IsTimerActive(const TimerHandle& handle) const
    {
        if (const auto* timer = FindTimer(handle))
        {
            return !timer->bIsPaused;
        }
        return false;
    }

    float TimerManager::GetTimerRemaining(const TimerHandle& handle) const
    {
        if (const auto* timer = FindTimer(handle))
        {
            return timer->bIsPaused ? -1.0f : timer->TimeRemaining;
        }
        return -1.0f;
    }

    void TimerManager::ClearAllTimers()
    {
        Timers.clear();
    }

    const char* TimerManager::GetName() const noexcept
    {
        return "TimerManager";
    }

    TimerManager::TimerData* TimerManager::FindTimer(const TimerHandle& handle)
    {
        if (!handle.IsValid())
        {
            return nullptr;
        }
        const auto it = std::ranges::find(Timers, handle, &TimerData::Handle);
        return (it != Timers.end()) ? &(*it) : nullptr;
    }

    const TimerManager::TimerData* TimerManager::FindTimer(const TimerHandle& handle) const
    {
        if (!handle.IsValid())
        {
            return nullptr;
        }
        const auto it = std::ranges::find(Timers, handle, &TimerData::Handle);
        return (it != Timers.end()) ? &(*it) : nullptr;
    }
}