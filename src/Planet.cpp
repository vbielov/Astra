#include "Planet.hpp"

Planet::Planet(int size)
    : m_grid(nullptr), m_size(size), m_vertices(), m_indices(), m_polygons(), m_surfaceTexture("res/moonDirt.png")
{
    // Generate noise map
    const int SEED = 1273418;
    const float FREQUENCY = 0.2f;
    const float RADIUS = 20;
    const float NOISE_AMPLIFIER = 5.0f;

    PerlinNoise noise = PerlinNoise(SEED);
    m_grid = new float[m_size * m_size];

    for(int x = 0; x < m_size; x++)
    {
        for(int y = 0; y < m_size; y++)
        {
            float radialFalloff = RADIUS - std::sqrt((x - RADIUS) * (x - RADIUS) + (y - RADIUS) * (y - RADIUS));
            // if(radialFalloff > 1)
            //     radialFalloff = 1;
            if(radialFalloff < 0)
                radialFalloff = 0;

            float noiseValue = noise.noise(x * FREQUENCY, y * FREQUENCY, SEED * FREQUENCY) * NOISE_AMPLIFIER;

            m_grid[x * m_size + y] = radialFalloff - noiseValue;
        }
    }

    // Marching Squares
    const float ISO = 0.75f;
    const float SCALE = 0.1f;

    Vector edges[4] = {
        Vector(0, 1),
        Vector(1, 1),
        Vector(1, 0),
        Vector(0, 0)
    };

    for(int x = 0; x < m_size - 1; ++x)
    {
        for(int y = 0; y < m_size - 1; ++y)  
        {
            Vector pos = Vector(x, y);

            float values[4] = {
                m_grid[x * m_size + (y + 1)],
                m_grid[(x + 1) * m_size + (y + 1)],
                m_grid[(x + 1) * m_size + y],
                m_grid[x * m_size + y]
            };

            int shapeIndex = 0;
            shapeIndex |= (values[0] > ISO) ? 0b0001 : 0b0000;
            shapeIndex |= (values[1] > ISO) ? 0b0010 : 0b0000;
            shapeIndex |= (values[2] > ISO) ? 0b0100 : 0b0000;
            shapeIndex |= (values[3] > ISO) ? 0b1000 : 0b0000;

            // Mesh
            for(int i = 0; i < 9; i += 3)
            {
                if(m_meshIndexTable[shapeIndex][i] == -1)
                {
                    continue;
                }

                Vector triangleVertices[3];
                for(int j = 0; j < 3; j++)
                {
                    int indexA = m_edgeTable[m_meshIndexTable[shapeIndex][i + j]][0];
                    int indexB = m_edgeTable[m_meshIndexTable[shapeIndex][i + j]][1];
                    Vector pointA = edges[indexB];
                    Vector pointB = edges[indexA];
                    if(values[indexA] > values[indexB])
                    {
                        pointA = edges[indexA];
                        pointB = edges[indexB];
                    }
                    float interpolation = getT(values[indexA], values[indexB], ISO);
                    Vector localPoint = pointB.lerp(pointA, interpolation);
                    Vector globalPoint = (localPoint + pos) * SCALE;
                    triangleVertices[j] = globalPoint;

                    float triangleVertices[] = { 
                        // x y z r g b uv.x uv.y
                        globalPoint.x, globalPoint.y, 0.0f,     1.0f, 1.0f, 1.0f,   localPoint.x, localPoint.y
                    };

                    auto existingSize = m_vertices.size();
                    m_vertices.resize(existingSize + 8);
                    std::copy(triangleVertices, triangleVertices + 8, m_vertices.begin() + existingSize);
                }

                if(shapeIndex != 15)
                {
                    m_polygons.push_back(Polygon(&triangleVertices[0], 3, false));
                }

                int indexCount = m_indices.size();
                m_indices.push_back(0 + indexCount);
                m_indices.push_back(1 + indexCount);
                m_indices.push_back(2 + indexCount);
            }
        }
    }

    Physics* physics = &Physics::instance();
    for(Polygon& polygon : m_polygons)
    {
        physics->addPolygon(&polygon);
    }
}

Planet::~Planet()
{
    delete[] m_grid;
    Physics* physics = &Physics::instance();
    for(Polygon& polygon : m_polygons)
    {
        physics->removePolygon(&polygon);
    }
}

void Planet::draw() const
{
    Renderer* renderer = &Renderer::instance();

    renderer->drawMesh(m_vertices.data(), m_indices.data(), m_vertices.size() / 8, m_indices.size(), &m_surfaceTexture);

    // Debug colliders
    // for(Polygon const& polygon : m_polygons)
    // {
    //     ((Polygon&)polygon).draw(0x00FF00);
    // }
}

float Planet::getT(float a, float b, float iso) const
{
    float v2 = std::max(b, a);
    float v1 = std::min(b, a);
    if(v2 - v1 == 0)
    {
        return 0.0f;
    }
    return (iso - v1) / (v2 - v1);
}