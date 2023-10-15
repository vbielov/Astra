#pragma once
#include <GL/glew.h>
#include "Image.hpp"
#include "Vertex.hpp"
#include <vector>
#include <memory>

class OpenGL
{
private:
    unsigned int maxVerticies;
    unsigned int usedVerticies;
    unsigned int m_VBO;
    unsigned int m_EBO;
    unsigned int m_VAO;
    unsigned int m_shaderProgram;
    Image* m_lastImage;
public:
    OpenGL();
    ~OpenGL();

    void DrawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float dx, float dy, float dWidth, float dHeight);
    void DrawRect(float x, float y, float width, float height, int color);
    void FillBackground(int color);
    void Render();

private:
    void Add(const Vertex* vertices, int size);
    void SetImage(Image* image);
    bool IsEnoughRoom(unsigned int uNumVertices) const;
    unsigned int CompileShader(const char* source, int type) const;
    void SendUniformSampler2D(const char* uniformName, int unit) const;
    static std::unique_ptr<float[]> HexToRGB(int hex);
};