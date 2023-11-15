#pragma once
#include "Polygon.hpp"
#include <float.h>
#include <algorithm>

class Physics
{
private:
    struct Interval
    {
        float min, max;

        Interval(float min, float max);
    };
    
    struct Collision
    {
        Polygon* polygonA;
        Polygon* polygonB;
        CollisionHit hit;

        Collision(Polygon* polygonA, Polygon* polygonB, CollisionHit hit);
    };

public:
    static Physics& instance();

    void addPolygon(Polygon* polygon);
    void removePolygon(Polygon* polygon);
    void step(float deltaTime);

private:
    std::vector<Polygon*> m_polygons;

    Interval getProjection(const Polygon* polygon, const Vector& axis) const;
    bool overlap(const Interval& projA, const Interval& projB) const;
    Vector getCenter(const Polygon* polygon) const;
    void calculateNormals(std::vector<Vector>* dst, const Polygon* polygon) const;
    CollisionHit findSATCollision(const Polygon* a, const Polygon* b);
    void resolveCollision(Polygon* a, Polygon* b, const CollisionHit& hit) const;
    bool isInBounds(const Polygon* a, const Polygon* b) const;
};