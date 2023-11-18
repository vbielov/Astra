#pragma once
#include "Renderer.hpp"
#include "PerlinNoise.h"
#include "Physics.hpp"

class Planet
{
public:
    Planet(int size);
    ~Planet();

    void draw() const;
private:
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Polygon> m_polygons;
    Image m_surfaceTexture;

    float* m_grid;
    int m_size;

    /*
        4 - (2) - 5
        |         |
       (3)       (1)
        |         |
        7 - (0) - 6
    */
    int m_meshIndexTable[16][9] = {
        { -1, -1, -1, -1, -1, -1, -1, -1, -1 }, // 0
        {  3,  2,  4, -1, -1, -1, -1, -1, -1 }, // 1
        {  2,  1,  5, -1, -1, -1, -1, -1, -1 }, // 2
        {  3,  1,  4,  1,  5,  4, -1, -1, -1 }, // 3
        {  0,  6,  1, -1, -1, -1, -1, -1, -1 }, // 4
        {  3,  2,  4,  0,  6,  1, -1, -1, -1 }, // 5
        {  0,  6,  5,  0,  5,  2, -1, -1, -1 }, // 6
        {  0,  5,  3,  0,  6,  5,  3,  5,  4 }, // 7
        {  7,  0,  3, -1, -1, -1, -1, -1, -1 }, // 8
        {  7,  0,  2,  7,  2,  4, -1, -1, -1 }, // 9
        {  7,  0,  3,  1,  5,  2, -1, -1, -1 }, // 10
        {  0,  1,  4,  0,  4,  7,  1,  5,  4 }, // 11
        {  7,  6,  3,  6,  1,  3, -1, -1, -1 }, // 12
        {  7,  1,  2,  7,  6,  1,  7,  2,  4 }, // 13
        {  3,  6,  2,  7,  6,  3,  6,  5,  2 }, // 14
        {  7,  6,  4,  6,  5,  4, -1, -1, -1 }  // 15
    };

    int m_edgeTable[8][2] = {
        // between edges
        { 2, 3 }, // 0
        { 1, 2 }, // 1
        { 0, 1 }, // 2
        { 0, 3 }, // 3
        // corners
        { 0, 0 }, // 4
        { 1, 1 }, // 5
        { 2, 2 }, // 6
        { 3, 3 }, // 7
    };

    float getT(float a, float b, float iso) const;
};