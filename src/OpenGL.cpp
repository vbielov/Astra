#include "OpenGL.hpp"

OpenGL::OpenGL()
    : maxVerticies(64000), usedVerticies(0), m_VBO(), m_EBO(), 
    m_VAO(), m_shaderProgram()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
   // Shaders
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aUv;
        out vec3 vColor;
        out vec2 vUv;
        void main()
        {
            vColor = aColor;
            vUv = aUv;
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        };
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        in vec3 vColor;
        in vec2 vUv;
        uniform sampler2D u_Texture;
        void main()
        {
            if(vUv.x == -1.0f && vUv.y == -1.0f)
            {
                FragColor = vec4(vColor, 1.0f);
                return;
            }
            
            FragColor = texture(u_Texture, vUv) * vec4(vColor, 1.0f);
        }
    )";

    unsigned int vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    int success;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(m_shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    SendUniformSampler2D("u_Texture", 0);

    // Buffers
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, maxVerticies * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    int maxIndices = maxVerticies / 4 * 6;
    unsigned int indices[maxIndices];
    for(int i = 0, v = 0; (i + 5) < maxIndices; i += 6)
    {
        // 0 1 2 | 2 3 0
        indices[i + 0] = 0 + v;
        indices[i + 1] = 1 + v;
        indices[i + 2] = 2 + v;

        indices[i + 3] = 2 + v;
        indices[i + 4] = 3 + v;
        indices[i + 5] = 0 + v;

        v += 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices, indices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // rgb
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

OpenGL::~OpenGL()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteProgram(m_shaderProgram);
}

void OpenGL::Render()
{
    glDrawElements(GL_TRIANGLES, usedVerticies / 4 * 6, GL_UNSIGNED_INT, 0);
    usedVerticies = 0;
}

void OpenGL::DrawRect(float x, float y, float width, float height, int color)
{
    std::unique_ptr<float[]> rgb = HexToRGB(color);

    Vertex vertices[] = {
        Vertex(x,          y + height, 0.0f, rgb[0], rgb[1], rgb[2], -1, -1),
        Vertex(x + width,  y + height, 0.0f, rgb[0], rgb[1], rgb[2], -1, -1),
        Vertex(x + width,  y,          0.0f, rgb[0], rgb[1], rgb[2], -1, -1),
        Vertex(x,          y,          0.0f, rgb[0], rgb[1], rgb[2], -1, -1)
    };
    
    Add(vertices, 4);
}

void OpenGL::FillBackground(int color)
{
    std::unique_ptr<float[]> rgb = HexToRGB(color);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(rgb[0], rgb[1], rgb[2], 1.0f);
}

void OpenGL::DrawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float dx, float dy, float dWidth, float dHeight)
{
    SetImage(image);

    Vertex vertices[] = {
        Vertex(dx,          dy + dHeight, 0, 1, 1, 1,  sx / image->width,               (sy + sHeight) / image->height),
        Vertex(dx + dWidth, dy + dHeight, 0, 1, 1, 1, (sx + sWidth) / image->width,     (sy + sHeight) / image->height),
        Vertex(dx + dWidth, dy,           0, 1, 1, 1, (sx + sWidth) / image-> width,    sy / image->height),
        Vertex(dx,          dy,           0, 1, 1, 1,  sx / image-> width,              sy / image->height)
    };

    Add(vertices, 4);
}

void OpenGL::Add(const Vertex* vertices, int size)
{
    if(IsEnoughRoom(size) == false)
        Render();

    glBufferSubData(GL_ARRAY_BUFFER, usedVerticies * sizeof(Vertex), size * sizeof(Vertex), vertices);
    usedVerticies += size;
}

bool OpenGL::IsEnoughRoom(unsigned int uNumVertices) const
{
    return (usedVerticies + uNumVertices) <= maxVerticies;
}

void OpenGL::SetImage(Image *image)
{
    if(m_lastImage == image || image == nullptr)
        return;

    Render();
    image->Bind(0);
    m_lastImage = image;
}

unsigned int OpenGL::CompileShader(const char* source, int type) const
{
    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        const char* typeStr = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
        std::cout << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    return shader;
}

void OpenGL::SendUniformSampler2D(const char* uniformName, int unit) const
{
    // TODO: optimize with hash table
    glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName), unit);
}

std::unique_ptr<float[]> OpenGL::HexToRGB(int hex)
{
    std::unique_ptr<float[]> rgb(new float[3]);
    int i = 3;
    while(hex)
    {
        rgb[--i] = (float)(hex & 0xFF) / 255.0f;
        hex >>= 8; 
    }
    return rgb;
}
