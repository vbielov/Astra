#pragma once
#include <vector>
#include "PerlinNoise.h"
#include "Physics.hpp"

class Terrain
{
public:
    std::vector<Polygon> polygons;
    
    Terrain();
    ~Terrain();

    void update(float deltaTime);
    void draw();

private:
    std::vector<Vector> m_heightPoints;
    Transform m_transform;
};