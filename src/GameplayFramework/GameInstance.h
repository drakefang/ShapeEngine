//
// Created by yuyzc on 2025/6/29.
//

#pragma once
#include "Core/Object.h"

namespace ShapeGame
{
    class World;

    class GameInstance : public Object
    {
    public:
        GameInstance() = default;
        ~GameInstance() override = default;
        static void RegisterMeta();

        void Initialize();
        void Shutdown();

        void LoadWorld(const std::string& worldAssetPath);
        World* GetCurrentWorld() const;

        void Tick(float deltaTime);

    private:
        std::shared_ptr<World> CurrentWorld;
    };
}
