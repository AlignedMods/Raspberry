#pragma once

#include "application/renderer.hpp"

#include <vector>

class Renderer_OpenGL3 : public Renderer {
public:
    explicit Renderer_OpenGL3(Vector2 viewport);

    virtual void UpdateViewport(Vector2 viewport) override;

    virtual void VertexV(const Vertex& v) override;

    virtual void Begin(RenderingMode mode) override;
    virtual void End() override;

    virtual void DrawPoint(const Vertex& p) override;

    virtual void DrawTriangle(const Vertex& bl, const Vertex& t, const Vertex& br) override;
    virtual void DrawTriangle(Vector2 bl, Vector2 t, Vector2 br, Color color) override;

    virtual void UseTexture(const Texture& tex) override;

private:
    virtual void Render() override;
    void RenderIfNeeded();

private:
    u32 m_VAO;
    u32 m_VBO;
    
    // shaders
    u32 m_Shader;

    f32 m_Vertices[2000];
    u64 m_VertexIndex = 0;
    u64 m_CurrentVertex = 0;

    Vector2 m_ScreenSize;

    RenderingMode m_Mode;
};