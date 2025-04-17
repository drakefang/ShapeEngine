

#include "Input/InputSystem.h"
#include "Input/InputAction.h"

namespace ShapeGame
{
    void InputSystem::Initialize() {}
    void InputSystem::Update(float DeltaTime) {}
    id_type InputSystem::RegisterAction(
        const std::string& name,
        std::function<void(float)> callback,
        std::unique_ptr<class IInputTrigger> trigger,
        std::vector<std::unique_ptr<class IInputModifier>> modifiers)
    {
        return {};
    }
    void InputSystem::BindKey(id_type ActionID, id_type KeyID) {}
    void InputSystem::PushContext(const class InputContext& Context) {}
    void InputSystem::PopContext() {}
    void InputSystem::ProcessInputAction(InputAction& Action, float DeltaTime) {}
} // namespace ShapeGame