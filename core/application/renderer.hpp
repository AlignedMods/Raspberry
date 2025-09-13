#pragma once

#include "types.hpp"

enum class RenderingMode {
    Points,
    Lines,
    Triangles,
    Quads
};

namespace Blackberry {

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void UpdateViewport(Vector2 viewport) = 0;

        virtual void Begin(RenderingMode mode) = 0;
        virtual void End() = 0;

        virtual void Clear() const = 0;

        virtual void VertexV(const Vertex& v) = 0;
        virtual void Vertex2f(f32 x, f32 y) { VertexV({{x, y}, m_Color}); }
        virtual void Vertex2i(i32 x, i32 y) { Vertex2f((f32)x, (f32)y); }

        void SetColor(Color color) { m_Color = color; };

        virtual void TexCoord2f(f32 x, f32 y) = 0;
        virtual void UseTexture(const Texture& texture) = 0;

        virtual Texture GenTexture(const Image& image) = 0;

    protected:
        virtual void Render() = 0;

    protected:
        Color m_Color;
    };

} // namespace Blackberry