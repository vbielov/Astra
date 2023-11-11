#include "Player.hpp"

Player::Player(Vector pos)
    : m_polygon(std::vector<Vector>({
        Vector(-0.5f, 0.5f),
        Vector(0.5f, 0.5f),
        Vector(0.5f, -0.5f),
        Vector(-0.5f, -0.5f)
    }).data(), 4, true), m_sprite("res/player.png")
{
    m_polygon.transform.pos = pos;
    m_polygon.transform.scale = Vector(0.10f, 0.10f * (22.0f / 20.0f));
    Physics* physics = &Physics::instance();
    physics->addPolygon(&m_polygon);
}

Player::~Player()
{
    Physics* physics = &Physics::instance();
    physics->removePolygon(&m_polygon);
}

void Player::update(float deltaTime)
{
    Input* input = &Input::instance();

    int rotationDir = 0;
    if(input->isKey(GLFW_KEY_A))
    {
        rotationDir +=  1;
    }
    if(input->isKey(GLFW_KEY_D))
    {
        rotationDir += -1;
    }

    const float ROTATION_SPEED = 1.5f;
    m_polygon.transform.rotation += rotationDir * ROTATION_SPEED * deltaTime;

    const float THRUST_FORCE = 4.0f;
    float theta = m_polygon.transform.rotation;
    Vector thrustDirection = Vector(-sin(theta), cos(theta));
    if(input->isKey(GLFW_KEY_SPACE))
    {
        m_polygon.force += thrustDirection * m_polygon.mass * THRUST_FORCE;
    }
}

void Player::draw()
{
    Renderer* renderer = &Renderer::instance();
    const Transform* transform = &m_polygon.transform;

    Vector vertices[4];
    int increment = 0;
    for(Vector const& edge : m_polygon.edges)
    {
        vertices[increment++] = m_polygon.transform.transformVector(edge);
    }

    renderer->drawImage(&m_sprite, 0, 0, 20, 22, (float*)&vertices[0]);
}
