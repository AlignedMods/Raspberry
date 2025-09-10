#include "opengl3_renderer.hpp"
#include "util.hpp"
#include "log.hpp"

#include "glad/glad.h"

// shaders
static const char* VertexShaderSource = MULTILINE_STR(
    // we cannot use '#' since the c preprocesser gets mad at us
    \x23version 330 core\n
    layout (location = 0) in vec3 a_Pos;
    layout (location = 1) in vec4 a_Color;
    
    out vec4 col;

    uniform vec3 windowSize;
    
    void main()
    {
        vec3 ndc = (a_Pos / windowSize) * 2.0 - 1.0;
        gl_Position = vec4(ndc, 1.0);
        col = a_Color;
    }
);

static const char* FragmentShaderSource = MULTILINE_STR(
    \x23version 330 core\n

    in vec4 col;

    out vec4 FragColor;

    void main()
    {
       FragColor = col;
    }
);

Renderer_OpenGL3::Renderer_OpenGL3(Vector2 viewport) {
    UpdateViewport(viewport);

    // compile shaders
    i32 errorCode = 0;
    char buf[512];

    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &VertexShaderSource, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &errorCode);

    if (!errorCode) {
        glGetShaderInfoLog(vertex, 512, nullptr, buf);
        Log(Log_Error, "Failed to compile default vertex shader! Error: %s", buf);
    }

    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &FragmentShaderSource, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &errorCode);

    if (!errorCode) {
        glGetShaderInfoLog(fragment, 512, nullptr, buf);
        Log(Log_Error, "Failed to compile default fragment shader! Error: %s", buf);
    }

    m_Shader = glCreateProgram();
    glAttachShader(m_Shader, vertex);
    glAttachShader(m_Shader, fragment);
    glLinkProgram(m_Shader);

    glGetProgramiv(m_Shader, GL_LINK_STATUS, &errorCode);

    if (!errorCode) {
        glGetProgramInfoLog(m_Shader, 512, nullptr, buf);
        Log(Log_Error, "Failed to link default shader program! Error: %s", buf);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // generate VAOs and VBOs
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

void Renderer_OpenGL3::UpdateViewport(Vector2 viewport) {
    m_ScreenSize = viewport;
    glViewport(0, 0, viewport.x, viewport.y);
}

void Renderer_OpenGL3::VertexV(const Vertex& v) {
    m_Vertices[m_VertexIndex++] = v.pos.x;
    m_Vertices[m_VertexIndex++] = v.pos.y;
    m_Vertices[m_VertexIndex++] = 0.0f;

    m_Vertices[m_VertexIndex++] = v.color.r;
    m_Vertices[m_VertexIndex++] = v.color.g;
    m_Vertices[m_VertexIndex++] = v.color.b;
    m_Vertices[m_VertexIndex++] = v.color.a;

    m_CurrentVertex++;
}

void Renderer_OpenGL3::Begin(RenderingMode mode) {
    Render(); // to render any remaining vertices
    m_Mode = mode;

    m_VertexIndex = 0; // reset the vertex counter
    m_CurrentVertex = 0;
}

void Renderer_OpenGL3::End() {
    Render();
}

void Renderer_OpenGL3::DrawPoint(const Vertex& p) {
    Begin(RenderingMode::Points);
    VertexV(p);
    End();
}

void Renderer_OpenGL3::DrawTriangle(const Vertex& bl, const Vertex& t, const Vertex& br) {
    Begin(RenderingMode::Triangles);
    VertexV(bl);
    VertexV(t);
    VertexV(br);
    End();
}

void Renderer_OpenGL3::DrawTriangle(Vector2 bl, Vector2 t, Vector2 br, Color color) {
    
}

void Renderer_OpenGL3::UseTexture(const Texture& tex) {
}

void Renderer_OpenGL3::Render() {
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLenum glMode;

    switch (m_Mode) {
        case RenderingMode::Points: glMode = GL_POINTS; break;
        case RenderingMode::Lines: glMode = GL_LINES; break;
        case RenderingMode::Triangles: glMode = GL_TRIANGLES; break;
        default: glMode = GL_POINTS; break;
    }

    glUseProgram(m_Shader);
    u32 uni = glGetUniformLocation(m_Shader, "windowSize");
    glUniform3f(uni, m_ScreenSize.x, m_ScreenSize.y, 1.0f);

    glBindVertexArray(m_VAO);

    // upload vertex data to funny gpu
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_VertexIndex * sizeof(f32), m_Vertices, GL_STREAM_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(0));
    glEnableVertexAttribArray(0);

    // vertex color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(glMode, 0, m_CurrentVertex);

    glBindVertexArray(0);
}

void Renderer_OpenGL3::RenderIfNeeded() {
    
}