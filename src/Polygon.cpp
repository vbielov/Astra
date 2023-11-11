#include "Polygon.hpp"

Transform::Transform(Vector pos, Vector scale, float rotation)
    : pos(pos), scale(scale), rotation(rotation)
{
    
}

Vector Transform::transformVector(Vector vector) const
{
    // Vector rotationVector = Vector(
    //     vector.x * cos(rotation) - vector.y * sin(rotation),
    //     vector.x * sin(rotation) + vector.y * cos(rotation)
    // );
    // rotationVector.x *= scale.x;
    // rotationVector.y *= scale.y;
    return vector + pos;
}

Polygon::Polygon(Vector* points, int count, bool isDynamic)
    :   transform(Vector(0, 0), Vector(1, 1), 0.0f), edges(points, points + count), 
        isDynamic(isDynamic), velocity(), force(), mass(1), onCollision(nullptr)
{
    // Calculate normals
    if(count <= 0 || points == nullptr)
        return;
}

void Polygon::draw(int color)
{
    if(edges.size() <= 0)
        return;

    Renderer* renderer = &Renderer::instance();

    for(int i = 0; i < edges.size(); i++)
    {
        Vector va = transform.transformVector(edges[i]);
        Vector vb = transform.transformVector(edges[(i + 1) % edges.size()]);
        renderer->drawLine(va.x, va.y, vb.x, vb.y, color);
    }
}

CollisionHit::CollisionHit(Vector normal, float depth, bool hasCollision)
    : normal(normal), depth(depth), hasCollision(hasCollision)
{

}