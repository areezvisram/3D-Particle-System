#include <cmath>
#include <mathLib3D.h>

//Point3D
Point3D::Point3D()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}
Point3D::Point3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
float Point3D::distanceTo(Point3D other)
{
    return sqrt(pow(other.x - x, 2.0) + pow(other.y - y, 2.0) + pow(other.z - z, 2.0));
}

float Point3D::fastDistanceTo(Point3D other)
{
    return pow(other.x - x, 2.0) + pow(other.y - y, 2.0) + pow(other.z - z, 2.0);
}

//Vec3D
Vec3D::Vec3D()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}
Vec3D::Vec3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
float Vec3D::length()
{
    return sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
}
Vec3D Vec3D::normalize()
{
    if (length() == 0)
    {
        return Vec3D();
    }
    return Vec3D(x / length(), y / length(), z / length());
}
Vec3D Vec3D::multiply(float scalar)
{
    return Vec3D(x * scalar, y * scalar, z * scalar);
}
Vec3D Vec3D::add(Vec3D vec)
{
    return Vec3D(x + vec.x, y + vec.y, z + vec.z);
}
Point3D Vec3D::movePoint(Point3D source)
{
    return Point3D(source.x + x, source.y + y, source.z + z);
}
Vec3D Vec3D::createVector(Point3D p1, Point3D p2)
{
    return Vec3D(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}
float Vec3D::dot(Vec3D v1, Vec3D v2)
{
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}
Vec3D Vec3D::cross(Vec3D v1, Vec3D v2)
{
    return Vec3D(
        v1.y*v2.z - v1.z*v2.y,
        v1.z*v2.x - v1.x*v2.z,
        v1.x*v2.y - v1.y*v2.x
    );
}