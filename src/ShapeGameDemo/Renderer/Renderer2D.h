//
// Created by yuyzc on 2025/8/4.
//

#pragma once
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <vector>
#include "Components/RenderComponent.h"

namespace ShapeGame
{
    class Renderer2D
    {
    public:
        Renderer2D() = default;
        ~Renderer2D() = default;

        void Init();
        void Shutdown();

        void BeginScene(const glm::mat4& viewProjection);
        void EndScene();

        void DrawShape(const Geometry& geometry, const glm::mat4& transform);

    private:
        void Flush();

        struct PosColorVertex
        {
            glm::vec2 pos;
            uint32_t color;

            static void Init()
            {
                s_Layout
                    .begin()
                    .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
                    .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                    .end();
            }

            static bgfx::VertexLayout s_Layout;
        };

        bgfx::ProgramHandle m_Program = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle u_ViewProj;

        static const uint32_t MaxVertices = 10000;
        static const uint32_t MaxIndices = MaxVertices * 3;

        std::vector<PosColorVertex> m_BatchedVertices;
        std::vector<uint16_t> m_BatchedIndices;
        uint32_t m_IndexCount = 0;
    };
}
