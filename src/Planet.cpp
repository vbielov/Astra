#include "Planet.hpp"

Planet::Planet(int size)
    : m_grid(nullptr), m_size(size), m_lines()
{
    // Generate noise map
    const int SEED = 1273418;
    const float FREQUENCY = 0.05f;

    PerlinNoise noise = PerlinNoise(SEED);
    m_grid = new float[m_size * m_size];

    for(int x = 0; x < m_size; x++)
    {
        for(int y = 0; y < m_size; y++)
        {
            float noiseValue = noise.noise(x * FREQUENCY, y * FREQUENCY, SEED * FREQUENCY);
            m_grid[x * m_size + y] = noiseValue;
        }
    }

    // Marching Squares
    const float ISO = 0.5f;
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
            
            Vector pos = Vector(x, y);

            if(m_indexTable[shapeIndex][0] != -1)
            {
                int edgeIndexFromA = m_edgeTable[m_indexTable[shapeIndex][0]][1];
                int edgeIndexFromB = m_edgeTable[m_indexTable[shapeIndex][0]][0];
                float fromInterpolate = getT(values[edgeIndexFromA], values[edgeIndexFromB], ISO);
                Vector from = edges[edgeIndexFromA].lerp(edges[edgeIndexFromB], fromInterpolate);
                from += pos;
                from *= SCALE;

                int edgeIndexToA = m_edgeTable[m_indexTable[shapeIndex][1]][1];
                int edgeIndexToB = m_edgeTable[m_indexTable[shapeIndex][1]][0];
                float toInterpolate = getT(values[edgeIndexToA], values[edgeIndexToB], ISO);
                Vector to = edges[edgeIndexToA].lerp(edges[edgeIndexToB], toInterpolate);
                to += pos;
                to *= SCALE;

                m_lines.emplace_back(from, to);

                if(m_indexTable[shapeIndex][2] != -1)
                {
                    edgeIndexFromA = m_edgeTable[m_indexTable[shapeIndex][2]][1];
                    edgeIndexFromB = m_edgeTable[m_indexTable[shapeIndex][2]][0];
                    fromInterpolate = getT(values[edgeIndexFromA], values[edgeIndexFromB], ISO);
                    from = edges[edgeIndexFromA].lerp(edges[edgeIndexFromB], fromInterpolate);
                    from += pos;
                    from *= SCALE;

                    edgeIndexToA = m_edgeTable[m_indexTable[shapeIndex][3]][1];
                    edgeIndexToB = m_edgeTable[m_indexTable[shapeIndex][3]][0];
                    toInterpolate = getT(values[edgeIndexToA], values[edgeIndexToB], ISO);
                    to = edges[edgeIndexToA].lerp(edges[edgeIndexToB], toInterpolate);
                    to += pos;
                    to *= SCALE;

                    m_lines.emplace_back(from, to);
                }
            }

            // Debug noise map
            // int color = 255 * values[0];
            // int hex = (color << 16) | (color << 8) | (color);
            // renderer->drawRect(x / 10.0f, y / 10.0f, 0.1f, 0.1f, hex);
        }
    }

    int linesCount = m_lines.size();
}

Planet::~Planet()
{
    delete[] m_grid;
}

void Planet::draw() const
{
    Renderer* renderer = &Renderer::instance();
    for(Line const& line : m_lines)
    {
        renderer->drawLine(line.a.x, line.a.y, line.b.x, line.b.y, 0xFFFFFF);
    }
}

float Planet::getT(float a, float b, float iso) const
{
    float v2 = std::max(b, a);
    float v1 = std::min(b, a);
    return (iso - v1) / (v2 - v1);
}

Planet::Line::Line(Vector a, Vector b)
    : a(a), b(b)
{
}
