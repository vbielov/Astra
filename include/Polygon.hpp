#pragma once
#include "Vector.hpp"
#include "Renderer.hpp"
#include <vector>
#include <functional>

struct Transform
{
    Vector pos;
    Vector scale;
    float rotation;

    Transform(Vector pos, Vector scale, float rotation);
    
    Vector transformVector(Vector vector) const;
};

struct CollisionHit
{
    Vector normal;
    float depth;
    bool hasCollision;

    CollisionHit(Vector normal, float depth, bool hasCollision);
};

class Polygon
{
public:
    Transform transform;
    std::vector<Vector> edges;

    bool isDynamic;
    Vector velocity;
    Vector force;
    float mass;

    std::function<void(CollisionHit&, float)> onCollision;

    Polygon(Vector* points, int count, bool isDynamic);

    void draw(int color);
};