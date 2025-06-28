
#include "Core/MetaRegistry.h"
#include "Core/Object.h"


int main(int argc, char** argv)
{
    ShapeGame::MetaRegistry::RegisterAll();
    ShapeGame::Object Obj;

    return 0;
}