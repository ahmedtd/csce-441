
#include "geometry.hpp"

#include <cmath>
using std::sqrt;

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

void vec2::x(double x)
{
    mComponents[0] = x;
}

void vec2::y(double y)
{
    mComponents[1] = y;
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

vec2 vec2::operator*(const double rop) const
{
    return (vec2(x()*rop, y()*rop));
}

vec2 vec2::operator/(const double rop) const
{
    return (vec2(x()/rop, y()/rop));
}

bool vec2::operator==(const vec2 &rop) const
{
    return x() == rop.x() && y() == rop.y();
}

double vec2::norm() const
{
    return sqrt(x() * x() + y() * y());
}

void vec2::normalize()
{
    x(x() / norm());
    y(y() / norm());
}

vec2::operator const GLdouble*() const
{
    return static_cast<const GLdouble*>(mComponents);
}

vec2 operator*(double lop, const vec2 &rop)
{
    return rop * lop;
}

ostream& operator<<(ostream &out, const vec2 &rop)
{
    out << "(" << rop.x() << ", " << rop.y() << ")";
    return out;
}
