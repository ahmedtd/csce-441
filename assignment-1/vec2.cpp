
#include "geometry.hpp"

vec2::vec2(const double x, const double y)
{
    mComponents[0] = x;
    mComponents[1] = y;
}

vec2::vec2(const vec2 &other)
{
    mComponents[0] = other.x();
    mComponents[1] = other.y();
}

double vec2::x() const
{
    return mComponents[0];
}

double vec2::y() const
{
    return mComponents[1];
}

vec2 vec2::operator+(const vec2 &rop) const
{
    return (vec2(x() + rop.x(), y()+ rop.y()));
}

vec2 vec2::operator-(const vec2 &rop) const
{
    return (vec2(x() - rop.x(), y() - rop.y()));
}

vec2::operator const GLdouble*() const
{
    return static_cast<const GLdouble*>(mComponents);
}
