
#pragma once

#include "Fwd.h"
#include <vector>

namespace ShapeGame
{
    class InputContext
    {
    public:
        explicit InputContext(int Priority = 0);

        void AddAction(id_type ActionID);
        const std::vector<id_type>& GetActions() const;

        int GetPriority() const;

    private:
        std::vector<id_type> actions;
        int priority;
    };
} // namespace ShapeGame