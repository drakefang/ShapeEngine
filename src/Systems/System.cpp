
#include "System.h"
#include <memory>
#include <vector>

namespace ShapeGame
{
    std::vector<std::unique_ptr<System>> System::SubSystems;
}