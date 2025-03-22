
#include "InputContext.h"
#include "Fwd.h"


namespace ShapeGame
{
    InputContext::InputContext(int Priority) : priority(Priority)
    {

    }

    void InputContext::AddAction(id_type ActionID)
    {
        actions.emplace_back(ActionID);
    }

    const std::vector<id_type>& InputContext::GetActions() const
    {
        return actions;
    }

    int InputContext::GetPriority() const
    {
        return priority;
    }
}