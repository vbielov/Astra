#pragma once
#include <vector>
#include "Polygon.hpp"

class Terrain
{
public:
    std::vector<Polygon> polygons;
    
    Terrain();

    void update();
    void draw();

private:

};