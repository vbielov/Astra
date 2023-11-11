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
                if(a->onCollision != nullptr) 
                    a->onCollision(hit, 0);
                if(b->onCollision != nullptr) 
                    b->onCollision(hit, 0);
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
                col.hit.normal * -col.hit.depth
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
        // polygon->velocity += Vector(0.0f, -1.0f) * GRAVITY * deltaTime;

        polygon->velocity += polygon->force / polygon->mass * deltaTime;
        polygon->transform.pos += polygon->velocity * deltaTime;

        const float DAMPING_FACTOR = 0.995f;
        polygon->velocity *= DAMPING_FACTOR;

        polygon->force = Vector(0, 0); // reseting force
    }
}

Physics::Interval Physics::getProjection(const Polygon* polygon, const Vector& axis) const
{
    // Avoid using FLT_MAX, FLT_MIN
    float min = axis.dotProduct(polygon->transform.transformVector(polygon->edges[0]));
    float max = axis.dotProduct(polygon->transform.transformVector(polygon->edges[0]));

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

Vector Physics::getCenter(const Polygon* polygon) const
{     
    Vector sum = Vector(0, 0);
    for(Vector const& edge : polygon->edges)
    {
        sum += edge;
    }

    return sum / (float)polygon->edges.size();
}

void Physics::calculateNormals(std::vector<Vector>* dst, const Polygon* polygon) const
{
    for(int i = 0; i < polygon->edges.size(); ++i)
    {
        // find edge normal, not very efficient to be honest.
        Vector va = polygon->transform.transformVector(polygon->edges[i]);
        Vector vb = polygon->transform.transformVector(polygon->edges[(i + 1) % polygon->edges.size()]);
        Vector edge = vb - va;
        dst->emplace_back(-edge.y, edge.x);
    } 
}

CollisionHit Physics::findSATCollision(const Polygon* a, const Polygon* b)
{
    
    // Normal & Depth calculation
    // https://www.youtube.com/watch?v=SUyG3aV_vpM

    // Avoid using FLT_MAX, FLT_MIN
    CollisionHit hit = CollisionHit(Vector(0, 0), 0, false);

    std::vector<Vector> normalsA;
    calculateNormals(&normalsA, a);

    std::vector<Vector> normalsB;
    calculateNormals(&normalsB, b);

    for(Vector const& axis : normalsA)
    {
        Interval projA = getProjection(a, axis);
        Interval projB = getProjection(b, axis);

        if(overlap(projA, projB) == false)
        {
            return hit;
        }

        float axisDepth = std::min(projB.max - projA.min, projA.max - projB.min);
        if(axisDepth < hit.depth || hit.depth == 0)
        {
            hit.depth = axisDepth;
            hit.normal = axis;
        }
    }

    for(Vector const& axis : normalsB)
    {
        Interval projA = getProjection(a, axis);
        Interval projB = getProjection(b, axis);

        if(overlap(projA, projB) == false)
        {
            return hit;
        }

        float axisDepth = std::min(projB.max - projA.min, projA.max - projB.min);
        if(axisDepth < hit.depth || hit.depth == 0)
        {
            hit.depth = axisDepth;
            hit.normal = axis;
        }
    }

    float length = hit.normal.length();
    hit.normal.normilize();

    if(length != 0.0f)
    {
        hit.depth /= length;
    }
    else
    {
        hit.depth = 0.0f;
    }

    Vector centerA = a->transform.transformVector(getCenter(a));
    Vector centerB = b->transform.transformVector(getCenter(b));
    Vector direction = centerB - centerA;

    if(direction.dotProduct(hit.normal) < 0.0f)
    {
        hit.normal = hit.normal * -1.0f;
    }

    hit.hasCollision = true;
    return hit;
}

Physics::Interval::Interval(float min, float max)
    : min(min), max(max)
{

}

Physics::Collision::Collision(Polygon* polygonA, Polygon* polygonB, CollisionHit hit)
    : polygonA(polygonA), polygonB(polygonB), hit(hit)
{

}
