#include "RectBatch.hpp"

RectBatch::RectBatch() 
    : Batch(), m_lastImage(nullptr), m_vertices(), m_indices()
{
    this->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Shaders
    const char* VERTEX_SHADER_SOURCE = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aUv;
        uniform mat4 u_MVP;
        out vec3 vColor;
        out vec2 vUv;
        void main()
        {
            vColor = aColor;
            vUv = aUv;
            gl_Position = u_MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        };
    )";

    const char* FRAGMENT_SHADER_SOURCE = R"(
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

    sendSampler2DUniform("u_Texture", 0);

    // Buffers
    glBufferData(GL_ARRAY_BUFFER, m_maxVertices * sizeof(RectVertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxIndices * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // rgb
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    this->unBind();
}

void RectBatch::render()
{
    if(m_usedVertices == 0)
    {
        return;
    }

    this->bind();
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_usedVertices * sizeof(RectVertex), m_vertices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_usedIndices * sizeof(unsigned int), m_indices.data());

    sendMVPUniform();
    sendSampler2DUniform("u_Texture", 0);
    glDrawElements(GL_TRIANGLES, m_usedIndices, GL_UNSIGNED_INT, 0);

    m_vertices.clear();
    m_indices.clear();

    m_usedVertices = 0;
    m_usedIndices = 0;
    
    this->unBind();
}

void RectBatch::add(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount)
{
    if(isEnoughRoom(vertexCount, 0) == false)
    {
        render();
    }

    size_t existingSize = m_vertices.size();
    const int floatCountOfVertex = sizeof(RectVertex) / sizeof(float);
    m_vertices.resize(existingSize + vertexCount * floatCountOfVertex);
    std::copy(vertices, vertices + vertexCount * floatCountOfVertex, m_vertices.begin() + existingSize);

    existingSize = m_indices.size();
    m_indices.resize(existingSize + indexCount);
    std::copy(indices, indices + indexCount, m_indices.begin() + existingSize);

    m_usedVertices += vertexCount;
    m_usedIndices += indexCount;
}

bool RectBatch::isEnoughRoom(unsigned int numVertices, unsigned int numIndices) const
{
    return (m_usedVertices + numVertices) <= m_maxVertices;
}

void RectBatch::setImage(const Image* image)
{
    if(m_lastImage == image || image == nullptr)
        return;

    render();
    image->bind(0);
    m_lastImage = (Image*)image;
}

void RectBatch::sendSampler2DUniform(const char* uniformName, int unit) const
{
    // TODO: optimize with hash table, error handling when location not found
    glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName), unit);
}
