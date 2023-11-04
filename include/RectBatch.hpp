#pragma once
#include "Batch.hpp"
#include "Image.hpp"

class RectBatch : public Batch
{
private:
    struct RectVertex
    {
        float x, y, z;
        float r, g, b;
        float uvX, uvY;
    };

public:
    RectBatch();

    void render();
    void add(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount);
    bool isEnoughRoom(unsigned int numVertices, unsigned int numIndices) const;
    void setImage(Image* image);

private:
    Image* m_lastImage;

    void sendSampler2DUniform(const char* uniformName, int unit) const;

};