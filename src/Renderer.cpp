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

    unsigned int vertexCount = m_rectBatch.getVertexCount();
    unsigned int indices[] = {
        0 + vertexCount,
        1 + vertexCount,
        2 + vertexCount,
        2 + vertexCount,
        3 + vertexCount,
        0 + vertexCount
    };

    m_rectBatch.add(&vertices[0], &indices[0], 4, 6);
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

    unsigned int vertexCount = m_rectBatch.getVertexCount();
    unsigned int indices[] = {
        0 + vertexCount,
        1 + vertexCount,
        2 + vertexCount,
        2 + vertexCount,
        3 + vertexCount,
        0 + vertexCount
    };

    m_rectBatch.add(&vertices[0], &indices[0], 4, 6);
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

    unsigned int vertexCount = m_rectBatch.getVertexCount();
    unsigned int indices[] = {
        0 + vertexCount,
        1 + vertexCount,
        2 + vertexCount,
        2 + vertexCount,
        3 + vertexCount,
        0 + vertexCount
    };
    
    m_rectBatch.add(&vertices[0], &indices[0], 4, 6);
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

void Renderer::drawMesh(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount, const Image* image)
{
    if(image != nullptr)
    {
        m_rectBatch.setImage(image);
    }
    m_rectBatch.add(&vertices[0], &indices[0], vertexCount, indexCount);
}

void Renderer::drawText(const char* text, const Font* font, float x, float y, float fontSize, int color)
{
    m_rectBatch.setImage((Image*)font);

    float rgb[3];
    hexToRGB(color, rgb);

    int vertexCount = m_rectBatch.getVertexCount();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int charCounter = 0;
    float xOffset = 0;
    float yOffset = 0;

    while(text[charCounter] != '\0') 
    {
        int letterIndex = (int)text[charCounter++];

        int cellSize = font->getCellSize();
        float textureSize = font->width;
        int cellGridWidth = std::floor(textureSize / cellSize);

        Vector cellPos = Vector(
            (letterIndex % cellGridWidth) * cellSize, 
            std::floor((float)letterIndex / (float)cellGridWidth) * (float)cellSize
        );
        cellPos.y += cellSize;
        cellPos.y = font->height - cellPos.y;
        cellPos /= (float)textureSize;

        float vertexQuad[] = {
            x + xOffset,            y + yOffset + fontSize, 0.0f, rgb[0], rgb[1], rgb[2], cellPos.x,                          cellPos.y + cellSize / textureSize,
            x + xOffset + fontSize, y + yOffset + fontSize, 0.0f, rgb[0], rgb[1], rgb[2], cellPos.x + cellSize / textureSize, cellPos.y + cellSize / textureSize,
            x + xOffset + fontSize, y + yOffset,            0.0f, rgb[0], rgb[1], rgb[2], cellPos.x + cellSize / textureSize, cellPos.y,
            x + xOffset,            y + yOffset,            0.0f, rgb[0], rgb[1], rgb[2], cellPos.x,                          cellPos.y
        };

        unsigned int letterVertexCount = vertices.size() / 8;
        unsigned int indexQuad[] = {
            0 + vertexCount + letterVertexCount,
            1 + vertexCount + letterVertexCount,
            2 + vertexCount + letterVertexCount,
            2 + vertexCount + letterVertexCount,
            3 + vertexCount + letterVertexCount,
            0 + vertexCount + letterVertexCount
        };

        size_t existingSize = vertices.size();
        vertices.resize(existingSize + 32);
        std::copy(&vertexQuad[0], &vertexQuad[0] + 32, vertices.begin() + existingSize);

        existingSize = indices.size();
        indices.resize(existingSize + 6);
        std::copy(&indexQuad[0], &indexQuad[0] + 6, indices.begin() + existingSize);

        if((char)letterIndex == '\n')
        {
            yOffset -= fontSize;
            xOffset = 0;
            continue;
        }
        xOffset += fontSize;
    }

    m_rectBatch.add(vertices.data(), indices.data(), vertices.size() / 8, indices.size());
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