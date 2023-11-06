#include "LineBatch.hpp"

LineBatch::LineBatch()
    : Batch()
{
    this->bind();
    // Shaders
    const char* VERTEX_SHADER_SOURCE = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec3 aColor;
        uniform mat4 u_MVP;
        out vec3 vColor;
        void main()
        {
            vColor = aColor;
            gl_Position = u_MVP * vec4(aPos.x, aPos.y, 0.0, 1.0);
        };
    )";

    const char* FRAGMENT_SHADER_SOURCE = R"(
        #version 330 core
        out vec4 FragColor;
        in vec3 vColor;
        void main()
        {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    unsigned int vertexShader = Batch::compileShader(VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);
    unsigned int fragmentShader = Batch::compileShader(FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);

    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    int success;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cerr << "OpenGL Error: shaderProgram LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Buffers
    glBufferData(GL_ARRAY_BUFFER, m_maxVertices * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxIndices * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

    // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // rgb
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->unBind();
}

void LineBatch::render()
{
    if(m_usedIndices == 0)
    {
        return;
    }

    this->bind();
    sendMVPUniform();
    glDrawElements(GL_LINES, m_usedIndices, GL_UNSIGNED_INT, 0);
    m_usedVertices = 0;
    m_usedIndices = 0;
    this->unBind();
}

void LineBatch::add(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount)
{
    if(isEnoughRoom(vertexCount, 0) == false)
    {
        render();
    }

    this->bind();
    glBufferSubData(GL_ARRAY_BUFFER, m_usedVertices * sizeof(LineVertex), vertexCount * sizeof(LineVertex), vertices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_usedIndices * sizeof(unsigned int), indexCount * sizeof(unsigned int), indices);

    m_usedVertices += vertexCount;
    m_usedIndices += indexCount;
    this->unBind();
}

bool LineBatch::isEnoughRoom(unsigned int numVertices, unsigned int numIndices) const
{
    return (m_usedVertices + numVertices) <= m_maxVertices;
}
