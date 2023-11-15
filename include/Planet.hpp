#pragma once
#include "Renderer.hpp"
#include "PerlinNoise.h"

class Planet
{
public:
    Planet(int size);
    ~Planet();

    void draw() const;
private:
    struct Line
    {
        Vector a;
        Vector b;

        Line(Vector a, Vector b);
    };

    std::vector<Line> m_lines;

    float* m_grid;
    int m_size;

    /*
        0 - (2) - 1
        |         |
       (3)       (1)
        |         |
        3 - (0) - 2
    */
    int m_indexTable[16][4] = {
        { -1, -1, -1, -1 }, // 0
        {  5,  6, -1, -1 }, // 1
        {  4,  3, -1, -1 }, // 2
        {  6,  3, -1, -1 }, // 3
        {  1,  2, -1, -1 }, // 4
        {  1,  6,  2,  5 }, // 5
        {  1,  4, -1, -1 }, // 6
        {  1,  6, -1, -1 }, // 7
        {  0,  7, -1, -1 }, // 8
        {  0,  5, -1, -1 }, // 9
        {  0,  3,  7,  4 }, // 10
        {  0,  3, -1, -1 }, // 11
        {  7,  2, -1, -1 }, // 12
        {  2,  5, -1, -1 }, // 13
        {  7,  4, -1, -1 }, // 14
        { -1, -1, -1, -1 }  // 15
    };

    int m_edgeTable[8][2] = {
        { 3, 2 }, // 0
        { 2, 3 }, // 1
        { 2, 1 }, // 2
        { 1, 2 }, // 3
        { 1, 0 }, // 4
        { 0, 1 }, // 5
        { 0, 3 }, // 6
        { 3, 0 }  // 7
    };

    float getT(float a, float b, float iso) const;
};