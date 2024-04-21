#include "Renderer.hpp"

Renderer& Renderer::instance()
{
    static Renderer INSTANCE;
    return INSTANCE;
}

Renderer::Renderer()
    : m_rectBatch()
{

}

void Renderer::drawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float dx, float dy, float dWidth, float dHeight)
{
    m_rectBatch.setImage(image);

    float vertices[] = {
        dx,          dy + dHeight, 0, 1, 1, 1,  sx / image->width,               (sy + sHeight) / image->height,
        dx + dWidth, dy + dHeight, 0, 1, 1, 1, (sx + sWidth) / image->width,     (sy + sHeight) / image->height,
        dx + dWidth, dy,           0, 1, 1, 1, (sx + sWidth) / image-> width,    sy / image->height,
        dx,          dy,           0, 1, 1, 1,  sx / image-> width,              sy / image->height 
    };

    m_rectBatch.add(&vertices[0], nullptr, 4, 0);
}

void Renderer::drawRect(float x, float y, float width, float height, int color)
{
    float rgb[3];
    Batch::hexToRGB(color, &rgb[0]);

    float vertices[] = {
        x,          y + height, 0.0f, rgb[0], rgb[1], rgb[2], -1, -1,
        x + width,  y + height, 0.0f, rgb[0], rgb[1], rgb[2], -1, -1,
        x + width,  y,          0.0f, rgb[0], rgb[1], rgb[2], -1, -1,
        x,          y,          0.0f, rgb[0], rgb[1], rgb[2], -1, -1
    };
    
    m_rectBatch.add(&vertices[0], nullptr, 4, 0);
}

void Renderer::fillBackground(int color)
{
    float rgb[3];
    Batch::hexToRGB(color, &rgb[0]);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(rgb[0], rgb[1], rgb[2], 1.0f);
}

void Renderer::render()
{
    m_rectBatch.render();
}