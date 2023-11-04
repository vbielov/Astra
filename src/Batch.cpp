#include "Batch.hpp"

Batch::Batch() 
    :   m_maxVertices(65536), m_usedVertices(0), m_maxIndices(65536), m_usedIndices(0), 
        m_VBO(), m_EBO(), m_VAO(), m_shaderProgram()
{
    m_shaderProgram = glCreateProgram();
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);
}

Batch::~Batch()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteProgram(m_shaderProgram);
}

void Batch::bind()
{
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void Batch::unBind()
{
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Batch::sendMVPUniform()
{
    Window* window = &Window::instance();
    float a = (float)window->width / (float)window->height; // aspect ratio
    float scale = window->scale;
    float xOffset = window->xOffset * window->scale;
    float yOffset = window->yOffset * window->scale;

    float matrix4x4[] = {
        1 * scale / a,  0,          0,          0,    //m11, m21, m31, m41,  // Column 1
        0,              1 * scale,  0,          0,    //m12, m22, m32, m42,  // Column 2
        0,              0,          1 * scale,  0,    //m13, m23, m33, m43,  // Column 3
        xOffset / a,    yOffset,    0,          1     //m14, m24, m34, m44   // Column 4
    };

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "u_MVP"), 1, GL_FALSE, matrix4x4);
}

unsigned int Batch::compileShader(const char *source, int type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        const char* types[] = {
            "VERTEX", "FRAGMENT", "GEOMETRY"
        };
        const char** typeStr = nullptr;
        switch(type)
        {
            case (GL_VERTEX_SHADER):
                typeStr = &(types[0]);
                break;
            case (GL_FRAGMENT_SHADER):
                typeStr = &(types[1]);
                break;
            case (GL_GEOMETRY_SHADER):
                typeStr = &(types[2]);
                break;
        }
        std::cerr << "OpenGL Error: " << *typeStr << " Shader COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    return shader;
}

int Batch::getIndexCount() const
{
    return m_usedIndices;
}

std::unique_ptr<float[]> Batch::hexToRGB(int hex)
{
    std::unique_ptr<float[]> rgb(new float[3]);
    int i = 3;
    // for some odd reason, the release version didn't liked while loop here
    for(; i >= 0 ;) 
    {
        rgb[--i] = (float)(hex & 0xFF) / 255.0f;
        hex >>= 8; 
    }
    return rgb;
}
