#pragma once
#include "Vector.hpp"
#include "Renderer.hpp"
#include <vector>

struct Transform
{
    Vector pos;
    Vector scale;
    float rotation;

    Transform(Vector pos, Vector scale, float rotation);
    
    Vector transformVector(Vector vector) const;
};

class Polygon
{
public:
    Transform transform;
    std::vector<Vector> edges;
    std::vector<Vector> normals;
    bool isDynamic;
    
    Vector velocity;
    Vector force;
    float mass;

    Polygon(Vector* points, int count, bool isDynamic);

    void draw(int color);
    Vector getCenter() const;
};