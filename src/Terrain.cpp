#include "Terrain.hpp"

Terrain::Terrain()
    : polygons(), m_heightPoints(), m_transform(Vector(0, 0), Vector(1, 1), 0.0f)
{
    int chunkPosition = 0;
    // noise settings
    const int SEED = 49;
    PerlinNoise pn(SEED);

    const int CHUNK_SIZE = 8;
    const float CHUNK_DETAIL = 0.5f;
    const float AMPLITUDE = 1.0f;
    const float FREQUENCY = 0.20f;
    const float SEED_Y = 1245.2f;
    const float SEED_Z = 253.6;

    // surface generation
    float chunkOffset = chunkPosition * (CHUNK_SIZE * CHUNK_DETAIL);
    for(int x = 0; x < (CHUNK_SIZE + 1) * CHUNK_DETAIL; x++)
    {
        float noiseValue = AMPLITUDE * pn.noise(
            (x + chunkOffset) * FREQUENCY, SEED_Y, SEED_Z
        );
        m_heightPoints.push_back(Vector(
            (x + chunkOffset) / (float)CHUNK_DETAIL, 
            noiseValue
        ));
    }

    // colliders generation
    std::vector<Vector>::iterator pointsIt;
    for(pointsIt = m_heightPoints.begin(); pointsIt != m_heightPoints.end() - 1; ++pointsIt)
    {
        Vector points[] = {
            Vector(pointsIt->x, 0.1f),
            Vector((pointsIt + 1)->x, 0.1f),
            *(pointsIt + 1),
            *pointsIt,
        };
        polygons.emplace_back(points, 4, true);
    }

    Physics* physics = &Physics::instance();
    for(Polygon& polygon : polygons)
    {
        physics->addPolygon(&polygon);
    }
}

Terrain::~Terrain()
{
    Physics* physics = &Physics::instance();
    for(Polygon& polygon : polygons)
    {
        physics->removePolygon(&polygon);
    }  
}

void Terrain::update(float deltaTime)
{

}

void Terrain::draw()
{
    for(Polygon& polygon : polygons)
    {
        polygon.draw(0xFFFFFF);
    }
}
