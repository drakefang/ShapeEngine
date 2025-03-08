
#include "GameContext.h"
#include "Renderer/RenderSystem.h"
#include "Timer/TimerSystem.h"

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
        RegisterSystem<TimerSystem>();
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