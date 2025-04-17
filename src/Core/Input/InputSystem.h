
#pragma once

#include "Fwd.h"
#include "Input/InputAction.h"
#include "Input/InputContext.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace ShapeGame
{
    class InputSystem
    {
    public:
        void Initialize();
        void Update(float DeltaTime);

        id_type RegisterAction(
            const std::string& name,
            std::function<void(float)> callback,
            std::unique_ptr<class IInputTrigger> trigger,
            std::vector<std::unique_ptr<class IInputModifier>> modifiers = {});

        void BindKey(id_type ActionID, id_type KeyID);

        void PushContext(const class InputContext& Context);
        void PopContext();

    private:
        std::unordered_map<id_type, struct InputAction> actions;
        std::vector<InputContext> context_stack;

        std::unordered_set<id_type> pressed_keys;
        std::unordered_set<id_type> held_keys;
        std::unordered_set<id_type> released_keys;

        void ProcessInputAction(InputAction& Action, float DeltaTime);
    };
} // namespace ShapeGame