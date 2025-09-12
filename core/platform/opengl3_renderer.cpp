#include "opengl3_renderer.hpp"
#include "util.hpp"
#include "log.hpp"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr f32 DEG2RAD = 3.14159f/180.0f;

namespace Blackberry {

    // shaders
    static const char* VertexShaderSource = MULTILINE_STR(
        // we cannot use '#' since the c preprocesser gets mad at us
        \x23version 330 core\n
        layout (location = 0) in vec3 a_Pos;
        layout (location = 1) in vec4 a_Color;
        layout (location = 2) in vec2 a_TexCoord;
    
        out vec4 col;
        out vec2 texCoord;

        uniform mat4 projection;
    
        void main()
        {
            gl_Position = projection * vec4(a_Pos, 1.0);
            col = a_Color;
            texCoord = a_TexCoord;
        }
    );

    static const char* FragmentShaderSource = MULTILINE_STR(
        \x23version 330 core\n

        in vec4 col;
        in vec2 texCoord;

        out vec4 FragColor;

        uniform sampler2D sampler;
        uniform int useTexture;

        void main()
        {
            if (useTexture == 1) {
                vec4 texelColor = texture(sampler, texCoord);
                FragColor = col * texelColor;
            } else {
                FragColor = col;
            }
        }
    );

    static u32 tex;

    Renderer_OpenGL3::Renderer_OpenGL3(Vector2 viewport) {
        UpdateViewport(viewport);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);

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

        stbi_set_flip_vertically_on_load(true);

        // try load tex

        i32 width;
        i32 height;
        i32 channels;

        u8* data = stbi_load("fox.jpg", & width, &height, &channels, 4);

        if (!data) {
            Log(Log_Error, "Failed to find image!");
        }

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }

    void Renderer_OpenGL3::UpdateViewport(Vector2 viewport) {
        glViewport(0, 0, viewport.x, viewport.y);

        m_Projection = glm::ortho(
            0.0f,            // left
            (f32)viewport.x, // right
            (f32)viewport.y, // bottom
            0.0f,            // top
            -1.0f, 1.0f      // near-far
        );
    }

    void Renderer_OpenGL3::VertexV(const Vertex& v) {
        m_Vertices[m_VertexIndex++] = v.pos.x;
        m_Vertices[m_VertexIndex++] = v.pos.y;
        m_Vertices[m_VertexIndex++] = 0.0f;

        m_Vertices[m_VertexIndex++] = v.color.r;
        m_Vertices[m_VertexIndex++] = v.color.g;
        m_Vertices[m_VertexIndex++] = v.color.b;
        m_Vertices[m_VertexIndex++] = v.color.a;

        m_Vertices[m_VertexIndex++] = m_CurrentTexCoord.x;
        m_Vertices[m_VertexIndex++] = m_CurrentTexCoord.y;

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

    void Renderer_OpenGL3::Clear() const {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
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

    void Renderer_OpenGL3::DrawTriangle(Vector2 bl, Vector2 t, Vector2 br, const Color& color) {
        Begin(RenderingMode::Triangles);

        SetColor(color);
        Vertex2f(bl.x, bl.y);
        Vertex2f(t.x, t.y);
        Vertex2f(br.x, br.y);

        End();
    }

    void Renderer_OpenGL3::DrawRectangle(const Vertex& bl, const Vertex& br, const Vertex& tr, const Vertex& tl) {
        Begin(RenderingMode::Triangles);

        // first triangle
        VertexV(bl);
        VertexV(tr);
        VertexV(br);

        // second triangle
        VertexV(bl);
        VertexV(tl);
        VertexV(tr);

        End();
    }

    void Renderer_OpenGL3::DrawRectangle(Vector2 position, Vector2 dimensions, const Color& color) {
        Vertex bl;
        Vertex br;
        Vertex tr;
        Vertex tl;

        bl = {{position.x, position.y + dimensions.y}, color};
        br = {{position.x + dimensions.x, position.y + dimensions.y}, color};
        tr = {{position.x + dimensions.x, position.y}, color};
        tl = {position, color};
    
        DrawRectangle(bl, br, tr, tl);
    }

    void Renderer_OpenGL3::DrawCircle(const Vertex& center, f32 radius, u32 segments) {
        f32 angle = 0.0f;
        f32 stepLength = 360.0f / (f32)segments;

        Begin(RenderingMode::Triangles);

        for (u32 i = 0; i < segments; i++) {
            SetColor(center.color);

            Vertex2f(center.pos.x + cosf(DEG2RAD*angle)*radius, center.pos.y + sinf(DEG2RAD*angle)*radius);
            Vertex2f(center.pos.x + cosf(DEG2RAD*(angle + stepLength*2.0f))*radius, center.pos.y + sinf(DEG2RAD*(angle + stepLength*2.0f))*radius);
            Vertex2f(center.pos.x, center.pos.y);
        
            angle += stepLength;
        }

        End();
    }

    void Renderer_OpenGL3::TexCoord2f(f32 x, f32 y) {
        m_CurrentTexCoord.x = x;
        m_CurrentTexCoord.y = y;
    }

    void Renderer_OpenGL3::UseTexture(const Texture& teex) {
        m_UsingTexture = true;

        glBindTexture(GL_TEXTURE_2D, tex);
    }

    void Renderer_OpenGL3::StopUseTexture() {
        m_UsingTexture = false;
    }

    void Renderer_OpenGL3::Render() {
        GLenum glMode;

        switch (m_Mode) {
            case RenderingMode::Points: glMode = GL_POINTS; break;
            case RenderingMode::Lines: glMode = GL_LINES; break;
            case RenderingMode::Triangles: glMode = GL_TRIANGLES; break;
            default: glMode = GL_POINTS; break;
        }

        glUseProgram(m_Shader);
        u32 uni = glGetUniformLocation(m_Shader, "projection");
        glUniformMatrix4fv(uni, 1, GL_FALSE, glm::value_ptr(m_Projection));

        if (m_UsingTexture) {
            u32 useTex = glGetUniformLocation(m_Shader, "useTexture");
            glUniform1i(useTex, 1);
        } else {
            u32 useTex = glGetUniformLocation(m_Shader, "useTexture");
            glUniform1i(useTex, 0);
        }

        glBindVertexArray(m_VAO);

        // upload vertex data to funny gpu
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_VertexIndex * sizeof(f32), m_Vertices, GL_STREAM_DRAW);

        // vertex position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(0));
        glEnableVertexAttribArray(0);

        // vertex color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(3 * sizeof(f32)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(7 * sizeof(f32)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(glMode, 0, m_CurrentVertex);

        glBindVertexArray(0);
    }

} // namespace Blackberry