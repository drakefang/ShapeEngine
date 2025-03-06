

#include "TimerSystem.h"
#include "Core/GameContext.h"
#include "Core/Timer.h"

namespace ShapeGame
{
    TimerSystem::TimerSystem() {}

    void TimerSystem::Update(float DeltaTime)
    {
        auto view = GameContext::Get().Registry().view<Timer>();
        view.each(
            [DeltaTime](auto entity, Timer& timer)
            {
                if (timer.paused)
                {
                    return;
                }
                timer.elapsed += DeltaTime;
                if (timer.elapsed >= timer.interval)
                {
                    if (auto callback = TimerSystem::GetCallback(timer))
                    {
                        callback();
                    }
                    //
                    if (timer.looping)
                    {
                        timer.elapsed = 0.f;
                    }
                    else
                    {
                        GameContext::Get().Registry().remove<Timer>(entity);
                    }
                }
            });
    }

    TimerCallback TimerSystem::GetCallback(const Timer& timer)
    {
        return TimerCallbackRegistry::Get().GetCallback(timer.handle);
    }

    TimerCallbackRegistry& TimerCallbackRegistry::Get()
    {
        static TimerCallbackRegistry instance;
        return instance;
    }

    size_t TimerCallbackRegistry::Register(TimerCallback Callback)
    {
        size_t id = ++currentId;
        callbacks[id] = Callback;
        return id;
    }

    TimerCallback TimerCallbackRegistry::GetCallback(size_t id)
    {
        return callbacks.contains(id) ? callbacks[id] : nullptr;
    }
} // namespace ShapeGame