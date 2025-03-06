
#pragma once

#include <functional>
#include <unordered_map>
#include "Core/SystemBase.h"
#include "Core/Timer.h"

namespace ShapeGame
{
    using TimerCallback = std::function<void()>;

    class TimerCallbackRegistry
    {
    public:
        static TimerCallbackRegistry& Get();

        size_t Register(TimerCallback Callback);

        template <typename... Args>
        size_t RegisterWithParams(std::function<void(Args...)> cb, Args... args)
        {
            return Register([=]() { cb(args...); });
        }

        TimerCallback GetCallback(size_t id);

        std::unordered_map<size_t, TimerCallback> callbacks;

    private:
        size_t currentId;
    };

    class TimerSystem : public SystemBase<TimerSystem>
    {
    public:
        TimerSystem();

        virtual void Update(float DeltaTime) override;

        template <typename Func>
        static void BindCallback(Timer& timer, Func&& func)
        {
            timer.handle = TimerCallbackRegistry::Get().Register(std::forward<Func>(func));
        }

        template <typename Func, typename... Args>
        static void BindCallback(Timer& timer, Func&& func, Args... args)
        {
            using FunctionType = std::function<void(Args...)>;
            FunctionType wrapper = std::forward<Func>(func);
            timer.handle = TimerCallbackRegistry::Get().RegisterWithParams(wrapper, std::forward<Args>(args)...);
        }

        static TimerCallback GetCallback(const Timer& timer);
    };
} // namespace ShapeGame