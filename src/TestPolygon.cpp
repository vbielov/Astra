#include "TestPolygon.hpp"

TestPolygon::TestPolygon()
    : polygon(nullptr, 0, false), m_selected(false), m_isColliding(false)
{
    generateRandomPolygon();
}

void TestPolygon::draw(int color)
{
    polygon.draw(m_isColliding ? 0xFF0000 : 0xFFFFFF);
}

void TestPolygon::update(float deltaTime)
{
    m_isColliding = false;

    const float GRAVITY = 0.98f;
    polygon.velocity.y -= GRAVITY * deltaTime;

    Input* input = &Input::instance();

    if(input->isKeyDown(GLFW_KEY_R))
    {
        generateRandomPolygon();
    }

    Window* window = &Window::instance();
    Vector mousePos = window->mouseToWorldPosition(input->getMousePosition());

    // Find boundries of polygon
    Vector minPoint = Vector(10000000, 10000000);
    Vector maxPoint = Vector(-10000000, -10000000);
    for(Vector const& edge : polygon.edges)
    {
        Vector point = polygon.transform.transformVector(edge);
        minPoint.x = std::min(point.x, minPoint.x);
        minPoint.y = std::min(point.y, minPoint.y);

        maxPoint.x = std::max(point.x, maxPoint.x);
        maxPoint.y = std::max(point.y, maxPoint.y);
    }

    Renderer* renderer = &Renderer::instance();

    renderer->drawLine(minPoint.x, minPoint.y, maxPoint.x, minPoint.y, 0x00FF00);
    renderer->drawLine(maxPoint.x, minPoint.y, maxPoint.x, maxPoint.y, 0x00FF00);
    renderer->drawLine(maxPoint.x, maxPoint.y, minPoint.x, maxPoint.y, 0x00FF00);
    renderer->drawLine(minPoint.x, maxPoint.y, minPoint.x, minPoint.y, 0x00FF00);

    if(m_selected == false)
    {

        // If mouse is in bounds and presses down, select the polygon
        if( mousePos.x >= minPoint.x &&
            mousePos.x <= maxPoint.x &&
            mousePos.y >= minPoint.y &&
            mousePos.y <= maxPoint.y &&
            input->isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
        {
            m_selected = true;
        }

        return;
    }

    polygon.transform.pos = mousePos;

    if(input->isMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT))
    {
        m_selected = false;
    }
}

void TestPolygon::generateRandomPolygon()
{
    const float SCALE = 1.0f / 100.0f;
    const unsigned int N_MAX = 3;
    const int LOW_DIST = -50;
    const int HIGH_DIST = 50;
    const float PI = 3.14159265359f;

    // Generate random points
    Vector points[N_MAX];
    std::srand((unsigned int)std::time(nullptr) + (size_t)(this));
    for(int n = 0; n < N_MAX; n++)
    {
        int rndX = LOW_DIST + std::rand() % (HIGH_DIST - LOW_DIST);
        int rndY = LOW_DIST + std::rand() % (HIGH_DIST - LOW_DIST);

        points[n] = Vector((float)rndX * SCALE, (float)rndY * SCALE);
    }

    //Compute the angle from center and sort
    Vector centerPoint = Vector(
        (LOW_DIST + HIGH_DIST) / 2.0f * SCALE, 
        (LOW_DIST + HIGH_DIST) / 2.0f * SCALE
    );
    
    std::sort(points, points + N_MAX, [&](Vector& a, Vector& b) {
        return  atan2(a.y - centerPoint.y, a.x - centerPoint.x) * 180 / PI -
                atan2(b.y - centerPoint.y, b.x - centerPoint.x) * 180 / PI;
    });

    // That's it
    polygon = Polygon(points, N_MAX, true);

    polygon.onCollision = [&](CollisionHit& hit, float someValue) {
        m_isColliding = true;
        std::cout << "Normal: " << hit.normal << "Depth" << hit.depth << std::endl;
    };
}
