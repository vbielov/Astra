#pragma once
#include "Batch.hpp"

class LineBatch : public Batch
{
private:
    struct LineVertex
    {
        float x, y;
        float r, g, b;
    };

public:
    LineBatch();

    void render();
    void add(const float* vertices, const unsigned int* indices, int vertexCount, int indexCount);
    bool isEnoughRoom(unsigned int numVertices, unsigned int numIndices) const;
private:

};
