#pragma once

// 8 floats
// 8 * 4 byte/float = 32 bytes
struct Vertex
{
    float x, y, z;
    float r, g, b;
    float uvX, uvY;

    Vertex(float x, float y, float z, float r, float g, float b, float uvX, float uvY);
};