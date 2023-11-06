#pragma once
#include <iostream>
#include <math.h>

struct Vector
{
    float x;
    float y;

    Vector();
    Vector(float x, float y);
    
    float DotProduct(const Vector& other) const;
    float Length() const;
    void Normilize();
    static Vector RemoveComponent(Vector& vector, Vector direction);

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector& operator+=(const Vector &other);
    Vector& operator-=(const Vector &other);
    Vector operator*(float scalar) const;
    Vector operator/(float scalar) const;
    Vector& operator*=(float scalar);
    Vector& operator/=(float scalar);
    bool operator==(const Vector& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Vector& point);
};