//
// Created by yuyzc on 2025/8/4.
//

#include "Renderer2D.h"
#include "Core/Log.h"
#include <bx/string.h>
#include <bgfx/embedded_shader.h>

// --- 内嵌着色器 (Embedded Shaders) ---
// 为了简化，我们直接将着色器代码嵌入到 C++ 中。
// 这是一个非常简单的顶点着色器和片段着色器。
#include "shaders/vs_simple.bin.h"
#include "shaders/fs_simple.bin.h"

static const bgfx::EmbeddedShader s_EmbeddedShaders[] = {
    BGFX_EMBEDDED_SHADER(vs_simple),
    BGFX_EMBEDDED_SHADER(fs_simple),
    BGFX_EMBEDDED_SHADER_END()
};

namespace ShapeGame
{

    bgfx::VertexLayout Renderer2D::PosColorVertex::s_Layout;

    void Renderer2D::Init()
    {
        SG_INFO("Renderer2D::Init");
        PosColorVertex::Init();

        // 创建着色器程序
        bgfx::ShaderHandle vsh = bgfx::createEmbeddedShader(s_EmbeddedShaders, bgfx::getRendererType(), "vs_simple");
        bgfx::ShaderHandle fsh = bgfx::createEmbeddedShader(s_EmbeddedShaders, bgfx::getRendererType(), "fs_simple");
        m_Program = bgfx::createProgram(vsh, fsh, true);

        if (!bgfx::isValid(m_Program))
        {
            throw std::runtime_error("Failed to create shader program.");
        }

        // 创建 Uniform
        u_ViewProj = bgfx::createUniform("u_viewProj", bgfx::UniformType::Mat4);

        m_BatchedVertices.reserve(MaxVertices);
        m_BatchedIndices.reserve(MaxIndices);
    }

    void Renderer2D::Shutdown()
    {
        SG_INFO("Renderer2D::Shutdown");
        bgfx::destroy(m_Program);
        bgfx::destroy(u_ViewProj);
    }

    void Renderer2D::BeginScene(const glm::mat4& viewProjection)
    {
        // 重置批处理数据
        m_BatchedVertices.clear();
        m_BatchedIndices.clear();
        m_IndexCount = 0;

        // 设置视图变换矩阵 Uniform
        bgfx::setUniform(u_ViewProj, &viewProjection[0][0]);
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::Flush()
    {
        if (m_BatchedVertices.empty())
        {
            return;
        }

        // 分配 bgfx 的临时顶点和索引缓冲区
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        if (bgfx::allocTransientVertexBuffer(&tvb, m_BatchedVertices.size(), PosColorVertex::s_Layout) &&
            bgfx::allocTransientIndexBuffer(&tib, m_IndexCount))
        {

            // 拷贝我们的数据到 bgfx 缓冲区
            memcpy(tvb.data, m_BatchedVertices.data(), m_BatchedVertices.size() * sizeof(PosColorVertex));
            memcpy(tib.data, m_BatchedIndices.data(), m_IndexCount * sizeof(uint16_t));

            // 设置渲染状态
            bgfx::setVertexBuffer(0, &tvb);
            bgfx::setIndexBuffer(&tib);
            bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);

            // 提交绘制调用
            bgfx::submit(0, m_Program);
        }
    }

    void Renderer2D::DrawShape(const Geometry& geometry, const glm::mat4& transform)
    {
        if (geometry.vertices.empty())
            return;

        // 如果当前批次放不下这个形状，先刷新旧的批次
        if (m_BatchedVertices.size() + geometry.vertices.size() > MaxVertices ||
            m_IndexCount + (geometry.vertices.size() * 2) > MaxIndices)
        {
            Flush();
            m_BatchedVertices.clear();
            m_BatchedIndices.clear();
            m_IndexCount = 0;
        }

        uint16_t startIndex = static_cast<uint16_t>(m_BatchedVertices.size());

        // 将顶点转换到世界空间并添加到批处理中
        for (const auto& localPos : geometry.vertices)
        {
            glm::vec4 worldPos = transform * glm::vec4(localPos.x, localPos.y, 0.0f, 1.0f);
            m_BatchedVertices.push_back({{worldPos.x, worldPos.y}, geometry.color});
        }

        // 为 LINE_STRIP 创建索引
        for (size_t i = 0; i < geometry.vertices.size(); ++i)
        {
            m_BatchedIndices.push_back(startIndex + i);
        }
        // 闭合形状
        m_BatchedIndices.push_back(startIndex);
        m_IndexCount += geometry.vertices.size() + 1;
    }
}