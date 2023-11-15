#include "Vector.hpp"

Vector::Vector() 
    : x(0), y(0) {}

Vector::Vector(float x, float y)
    : x(x), y(y) {}

float Vector::dotProduct(const Vector& other) const
{
    return this->x * other.x + this->y * other.y;
}

float Vector::length() const
{
    return sqrt(x * x + y * y);
}

void Vector::normilize()
{
    float len = length();
    if(len == 0.0f)
        return;

    x /= len;
    y /= len;
}

Vector Vector::removeComponent(Vector &vector, Vector direction)
{
    direction.normilize();
    return vector - direction * vector.dotProduct(direction);
}

Vector Vector::lerp(const Vector &other, float t) const
{
    return Vector(
        this->x + (other.x - this->x) * t,
        this->y + (other.y - this->y) * t
    );
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
