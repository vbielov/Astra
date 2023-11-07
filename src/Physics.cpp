#include "Physics.hpp"

Physics& Physics::instance()
{
    static Physics INSTANCE;
    return INSTANCE;
}

void Physics::addPolygon(Polygon* polygon)
{
    m_polygons.push_back(polygon);
}

void Physics::removePolygon(Polygon* polygon)
{
    if(polygon == nullptr)
        return;

    std::vector<Polygon*>::iterator itr = std::find(m_polygons.begin(), m_polygons.end(), polygon);
    if(itr == m_polygons.end())
        return;

    m_polygons.erase(itr);
}

void Physics::step(float deltaTime)
{
    // detect collisions
    std::vector<Collision> collisions;
    for(Polygon* const& a : m_polygons)
    {
        for(Polygon* const& b : m_polygons)
        {
            if(a == b)
                break;

            if(!a->isDynamic && !b->isDynamic)
                continue;

            CollisionHit hit = findSATCollision(a, b);

            if(hit.hasCollision)
            {
                collisions.emplace_back(a, b, hit);
            }
        }
    }
    // resolve collisions
    for(Collision const& col : collisions)
    {
        if(col.polygonA->isDynamic)
        {
            col.polygonA->transform.pos.x -= col.hit.normal.x * col.hit.depth;
            col.polygonA->transform.pos.y -= col.hit.normal.y * col.hit.depth;
            col.polygonA->velocity = Vector::removeComponent(
                col.polygonA->velocity, 
                col.hit.normal * col.hit.depth
            );
        }
            
        if(col.polygonB->isDynamic)
        {
            col.polygonB->transform.pos.x += col.hit.normal.x * col.hit.depth;
            col.polygonB->transform.pos.y += col.hit.normal.y * col.hit.depth;
            col.polygonB->velocity = Vector::removeComponent(
                col.polygonB->velocity, 
                col.hit.normal * col.hit.depth
            );
        }
    }

    // apply dynamics
    for(Polygon* const& polygon : m_polygons)
    {
        if(!polygon->isDynamic)
            continue;

        const float GRAVITY = 1.0f;
        polygon->velocity += Vector(0.0f, -1.0f) * GRAVITY * deltaTime;

        polygon->velocity += polygon->force / polygon->mass * deltaTime;
        polygon->transform.pos += polygon->velocity * deltaTime;

        const float DAMPING_FACTOR = 0.995f;
        polygon->velocity *= DAMPING_FACTOR;

        polygon->force = Vector(0, 0); // reseting force
    }
}

Physics::Interval Physics::getProjection(const Polygon* polygon, Vector axis) const
{
    float min = FLT_MAX;
    float max = FLT_MIN;
    for(Vector const& edge : polygon->edges)
    {
        float dotProduct = axis.dotProduct(polygon->transform.transformVector(edge));
        min = std::min(min, dotProduct);
        max = std::max(max, dotProduct);
    }
    return Interval(min, max);
}

bool Physics::overlap(const Interval& projA, const Interval& projB) const
{
    return  projA.min <= projB.max && 
            projA.max >= projB.min;
}

Physics::CollisionHit Physics::findSATCollision(const Polygon* a, const Polygon* b)
{
    CollisionHit hit = CollisionHit(Vector(0, 0), FLT_MAX, false);

    // Getting axis
    const std::vector<Vector>* axis1 = &(a->normals);
    const std::vector<Vector>* axis2 = &(b->normals);

    // Projecting on axis
    for(Vector const& axis : *axis1)
    {
        Interval p1 = getProjection(a, axis);
        Interval p2 = getProjection(b, axis);

        if(overlap(p1, p2) == false)
            return hit;

        float axisDepth = std::min(p2.max - p1.min, p1.max - p2.min);
        if(axisDepth < hit.depth)
        {
            hit.depth = axisDepth;
            hit.normal = axis;
        }
    }

    for(Vector const& axis : *axis2)
    {
        Interval p1 = getProjection(a, axis);
        Interval p2 = getProjection(b, axis);

        if(overlap(p1, p2) == false)
            return hit;

        float axisDepth = std::min(p2.max - p1.min, p1.max - p2.min);
        if(axisDepth < hit.depth)
        {
            hit.depth = axisDepth;
            hit.normal = axis;
        }
    }

    // Normal & Depth calculation
    // https://www.youtube.com/watch?v=SUyG3aV_vpM
    float length = hit.normal.length();
    hit.normal.normilize();

    if(length != 0.0f)
        hit.depth /= length;
    else
        hit.depth = 0.0f;

    Vector centerA = a->getCenter();
    Vector centerB = b->getCenter();
    Vector direction = centerB - centerA;

    if(direction.dotProduct(hit.normal) < 0.0f)
        hit.normal = hit.normal * -1.0f;

    hit.hasCollision = true;
    return hit;
}

Physics::Interval::Interval(float min, float max)
    : min(min), max(max)
{

}

Physics::CollisionHit::CollisionHit(Vector normal, float depth, bool hasCollision)
    : normal(normal), depth(depth), hasCollision(hasCollision)
{

}

Physics::Collision::Collision(Polygon* polygonA, Polygon* polygonB, CollisionHit hit)
    : polygonA(polygonA), polygonB(polygonB), hit(hit)
{

}
