#pragma once

#include "application/renderer.hpp"

#include "glm/glm.hpp"

#include <vector>

namespace Blackberry {

    class Renderer_OpenGL3 : public Renderer {
    public:
        explicit Renderer_OpenGL3(Vector2 viewport);

        virtual void UpdateViewport(Vector2 viewport) override;

        virtual void VertexV(const Vertex& v) override;

        virtual void Begin(RenderingMode mode) override;
        virtual void End() override;

        virtual void Clear() const override;

        virtual void TexCoord2f(f32 x, f32 y) override;
        virtual void UseTexture(const Texture& tex) override;

        virtual Texture GenTexture(const Image& image) override;

    private:
        virtual void Render() override;

    private:
        u32 m_VAO;
        u32 m_VBO;
    
        // shaders
        u32 m_Shader;

        f32 m_Vertices[2000];
        u64 m_VertexIndex = 0;
        u64 m_CurrentVertex = 0;

        glm::mat4 m_Projection;

        RenderingMode m_Mode;

        Vector2 m_CurrentTexCoord;

        bool m_UsingTexture = false;
        const Texture* m_CurrentTexture{};
    };

} // namespace Blackberry