#pragma once
#include "Window.hpp"
#include <memory>
#include <vector>
#include <iostream>

class Batch
{
public:
    Batch();
    virtual ~Batch();

    void bind();
    void unBind();
    void sendMVPUniform();
    virtual void render() = 0;

    int getIndexCount() const;
    int getVertexCount() const;
    
protected:
    unsigned int m_maxVertices;
    unsigned int m_usedVertices;
    unsigned int m_maxIndices;
    unsigned int m_usedIndices;

    unsigned int m_VBO;
    unsigned int m_EBO;
    unsigned int m_VAO;
    unsigned int m_shaderProgram;

    virtual void add(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount) = 0;
    virtual bool isEnoughRoom(unsigned int numVertices, unsigned int numIndices) const = 0;

    static unsigned int compileShader(const char* source, int type);
};