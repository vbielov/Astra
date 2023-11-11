#pragma once
#include "Polygon.hpp"
#include "Input.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <float.h>
#include <iostream>
#include "Physics.hpp"
#include "Renderer.hpp"

class TestPolygon
{
public:
    TestPolygon();

    void draw(int color);
    void update(float deltaTime);

    Polygon polygon;
private:
    bool m_selected;
    bool m_isColliding;

    void generateRandomPolygon();
};