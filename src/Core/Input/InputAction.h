
#pragma once

#include "Fwd.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ShapeGame
{
    enum class InputState : uint8_t
    {
        Pressed,
        Released,
        Held,
        None
    };

    class IInputModifier
    {
    public:
        virtual ~IInputModifier() = default;
        virtual float Modify(float Value, float DeltaTime) = 0;
    };

    class IInputTrigger
    {
    public:
        virtual ~IInputTrigger() = default;
        virtual bool Check(InputState Current, InputState Previous, float Duration) = 0;
    };

    struct InputAction
    {
        std::string name;
        std::vector<id_type> bound_keys;
        std::vector<std::unique_ptr<IInputModifier>> modifiers;
        std::unique_ptr<IInputTrigger> trigger;
        std::function<void(float)> callback;
        InputState current_state = InputState::None;
        InputState previous_state = InputState::None;
        float hold_duration = 0.f;
    };
} // namespace ShapeGame