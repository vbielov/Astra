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

void Renderer::drawImage(Image* image, float sx, float sy, float sWidth, float sHeight, float* destVertices)
{
    m_rectBatch.setImage(image);

    float vertices[] = {
        destVertices[0], destVertices[1], 0, 1, 1, 1,  sx / image->width,               (sy + sHeight) / image->height,
        destVertices[2], destVertices[3], 0, 1, 1, 1, (sx + sWidth) / image->width,     (sy + sHeight) / image->height,
        destVertices[4], destVertices[5], 0, 1, 1, 1, (sx + sWidth) / image-> width,    sy / image->height,
        destVertices[6], destVertices[7], 0, 1, 1, 1,  sx / image-> width,              sy / image->height
    };

    m_rectBatch.add(&vertices[0], nullptr, 4, 0);
}

void Renderer::drawRect(float x, float y, float width, float height, int color)
{
    float rgb[3];
    hexToRGB(color, rgb);

    float vertices[] = {
        x,          y + height, 0.0f, rgb[0], rgb[1], rgb[2], -1, -1,
        x + width,  y + height, 0.0f, rgb[0], rgb[1], rgb[2], -1, -1,
        x + width,  y,          0.0f, rgb[0], rgb[1], rgb[2], -1, -1,
        x,          y,          0.0f, rgb[0], rgb[1], rgb[2], -1, -1
    };
    
    m_rectBatch.add(&vertices[0], nullptr, 4, 0);
}

void Renderer::drawLine(float fromX, float fromY, float toX, float toY, int color)
{
    float rgb[3];
    hexToRGB(color, rgb);

    float vertices[] = {
        fromX,  fromY,  rgb[0], rgb[1], rgb[2],
        toX,    toY,    rgb[0], rgb[1], rgb[2]
    };

    unsigned int usedIndices = m_lineBatch.getIndexCount();

    unsigned int indices[] = {
        usedIndices + 0,
        usedIndices + 1
    };
    
    m_lineBatch.add(&vertices[0], &indices[0], 2, 2);
}

void Renderer::fillBackground(int color)
{
    float rgb[3];
    hexToRGB(color, rgb);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(rgb[0], rgb[1], rgb[2], 1.0f);
}

void Renderer::render()
{
    m_rectBatch.render();
    m_lineBatch.render();
}

// color needs float[3]
void Renderer::hexToRGB(int hex, float* color)
{
    // NOTE: I just don't understand why I can't use unique pointers here. 
    for(int i = 2; i >= 0; --i) 
    {
        color[i] = (float)(hex & 0xFF) / 255.0f;
        hex >>= 8; 
    }
}