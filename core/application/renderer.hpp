#pragma once

#include "types.hpp"

enum class RenderingMode {
    Points,
    Lines,
    Triangles,
    Quads
};

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void UpdateViewport(Vector2 viewport) = 0;

    virtual void Begin(RenderingMode mode) = 0;
    virtual void End() = 0;

    virtual void VertexV(const Vertex& v) = 0;
    virtual void Vertex2f(f32 x, f32 y) { VertexV({{x, y}, m_Color}); }
    virtual void Vertex2i(i32 x, i32 y) { Vertex2f((f32)x, (f32)y); }

    void SetColor(Color color) { m_Color = color; };

    virtual void DrawPoint(const Vertex& p) = 0;

    virtual void DrawTriangle(const Vertex& bl, const Vertex& t, const Vertex& br) = 0;
    virtual void DrawTriangle(Vector2 bl, Vector2 t, Vector2 br, Color color) = 0; // it's faster to copy these types
    virtual void DrawTriangle(Vector2 bl, Vector2 t, Vector2 br) { DrawTriangle(bl, t, br, m_Color); }; // uses the currently set color

    virtual void UseTexture(const Texture& texture) = 0;

protected:
    virtual void Render() = 0;

protected:
    Color m_Color;
};