
#include "GameContext.h"
#include "Core/Renderer/RenderSystem.h"

namespace ShapeGame
{
    GameContext& GameContext::Get()
    {
        static GameContext instance;
        return instance;
    }

    void GameContext::Initialize() 
    {
        RegisterSystem<RenderSystem>();
    }

    void GameContext::Update(float DeltaTime)
    {
        for (auto& sys : systems)
        {
            sys->Update(DeltaTime);
        }
    }
    entt::entity GameContext::CreateEntity()
    {
        return Registry().create();
    }
} // namespace ShapeGame