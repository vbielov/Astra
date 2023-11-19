#pragma once
#include "RectBatch.hpp"
#include "LineBatch.hpp"
#include "Font.hpp"

class Renderer
{
public:
    static Renderer& instance();

    void drawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float dx, float dy, float dWidth, float dHeight);
    void drawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float* destVertices);
    void drawRect(float x, float y, float width, float height, int color);
    void drawLine(float fromX, float fromY, float toX, float toY, int color);
    void drawMesh(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount, const Image* image);
    void drawText(const char* text, const Font* font, float x, float y, float fontSize, int color);
    void fillBackground(int color);

    void render();

    static void hexToRGB(int hex, float* color);

private:
    RectBatch m_rectBatch;
    LineBatch m_lineBatch;

    Renderer();
};