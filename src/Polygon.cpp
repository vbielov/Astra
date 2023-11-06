#include "Polygon.hpp"

Transform::Transform(Vector pos, Vector scale, float rotation)
    : pos(pos), scale(scale), rotation(rotation)
{
    
}

Vector Transform::transformVector(Vector vector) const
{
    Vector rotationVector = Vector(
        vector.x * cos(rotation) - vector.y * sin(rotation),
        vector.x * sin(rotation) + vector.y * cos(rotation)
    );
    rotationVector.x *= scale.x;
    rotationVector.y *= scale.y;
    return rotationVector + pos;
}

Polygon::Polygon(Vector* points, int count, bool isDynamic)
    : transform(Vector(0, 0), Vector(1, 1), 0.0f), edges(points, points + count), normals(), isDynamic(isDynamic), velocity(), force(), mass(1)
{
    // Calculate normals
    if(count <= 0 || points == nullptr)
        return;

    normals.reserve(count);
    for(int i = 0; i < count; i++)
    {
        Vector edge1 = edges[i];
        Vector edge2 = edges[(i + 1) % count];

        Vector dir = edge2 - edge1;
        normals.emplace_back(dir.y, -dir.x);
    }
}

void Polygon::draw(int color)
{
    if(edges.size() <= 0)
        return;

    Renderer* renderer = &Renderer::instance();

    std::vector<Vector>::iterator edgeIt;

    // Not very clean, but caching transformVector() should be better.
    Vector firstVector = transform.transformVector(edges[0]);
    Vector lastVector = firstVector;
    for(edgeIt = edges.begin() + 1; edgeIt != edges.end(); ++edgeIt)
    {
        Vector transVector = transform.transformVector(*edgeIt);
        renderer->drawLine(lastVector.x, lastVector.y, transVector.x, transVector.y, color);
        lastVector = transVector;
    }
    
    renderer->drawLine(lastVector.x, lastVector.y, firstVector.x, firstVector.y, color);
}

Vector Polygon::getCenter() const
{
    Vector sum = Vector(0, 0);
    for(Vector const& edge : edges)
    {
        sum += transform.transformVector(edge);
    }

    return sum / (float)edges.size();
}
