#pragma once
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Input.hpp"

class Player
{
public:
    Player(Vector pos);
    ~Player();

    void update(float deltaTime);
    void draw();

    Polygon m_polygon;
private:
    Image m_sprite;

};