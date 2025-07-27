//
// Created by yuyzc on 2025/6/29.
//

#include "GameInstance.h"

#include "World.h"
#include "Core/Logger.h"
#include "Core/MetaRegistry.h"

namespace ShapeGame
{
    void GameInstance::RegisterMeta()
    {
        using namespace entt::literals;
        entt::meta_factory<GameInstance>()
            .type("GameInstance"_hs)
            //.ctor<std::shared_ptr<GameInstance>(*)()>(entt::invoker_v<&std::make_shared<GameInstance>>)
            .base<Object>();
    }

    static AutoRegistrar registrar_GameInstance(&GameInstance::RegisterMeta);

    void GameInstance::Initialize()
    {
        ShapeGame::Logger()->info("game instance initialize...");
    }

    void GameInstance::Shutdown()
    {
        CurrentWorld = nullptr;
        ShapeGame::Logger()->info("game instance shutdown...");
    }

    void GameInstance::LoadWorld(const std::string& worldAssetPath)
    {
        CurrentWorld = std::make_shared<World>();
        ShapeGame::Logger()->info("world {0} loaded...", worldAssetPath);
    }

    World* GameInstance::GetCurrentWorld() const
    {
        return CurrentWorld.get();
    }

    void GameInstance::Tick(float deltaTime)
    {
        if (CurrentWorld)
        {
            CurrentWorld->Tick(deltaTime);
        }
    }
}