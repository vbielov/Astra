#pragma once
#include "RectBatch.hpp"

class Renderer
{
public:
    static Renderer& instance();

    void drawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float dx, float dy, float dWidth, float dHeight);
    void drawRect(float x, float y, float width, float height, int color);

    void fillBackground(int color);

    void render();

private:
    Renderer();
    RectBatch m_rectBatch;
};