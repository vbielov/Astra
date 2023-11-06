#include "Vector.hpp"

Vector::Vector() 
    : x(0), y(0) {}

Vector::Vector(float x, float y)
    : x(x), y(y) {}

float Vector::DotProduct(const Vector &other) const
{
    return this->x * other.x + this->y * other.y;
}

float Vector::Length() const
{
    return sqrt(x * x + y * y);
}

void Vector::Normilize()
{
    float length = Length();
    if(length == 0.0f)
        return;

    x /= length;
    y /= length;
}

Vector Vector::RemoveComponent(Vector &vector, Vector direction)
{
    direction.Normilize();
    return vector - direction * vector.DotProduct(direction);
}

Vector Vector::operator+(const Vector &other) const
{
    return Vector(this->x + other.x, this->y + other.y);
}

Vector Vector::operator-(const Vector &other) const
{
    return Vector(this->x - other.x, this->y - other.y);
}

Vector& Vector::operator+=(const Vector &other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector &other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Vector Vector::operator*(float scalar) const
{
    return Vector(this->x * scalar, this->y * scalar);
}

Vector Vector::operator/(float scalar) const
{
    return Vector(this->x / scalar, this->y / scalar);
}

Vector& Vector::operator*=(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

Vector& Vector::operator/=(float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

bool Vector::operator==(const Vector &other) const
{
    return this->x == other.x && this->y == other.y;
}

std::ostream &operator<<(std::ostream &out, const Vector &point)
{
    out << "(" << point.x << ", " << point.y << ")";
    return out;
}
