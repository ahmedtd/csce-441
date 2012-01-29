
#ifndef GEOMETRY
#define GEOMETRY

#include <GL/gl.h>

class vec2
{
public:
    vec2(const double x, const double y);
    vec2(const vec2 &other);

    double x() const;
    double y() const;

    vec2 operator+(const vec2 &rop) const;
    vec2 operator-(const vec2 &rop) const;

    operator const GLdouble*() const;

private:
    double mComponents [2];
};

#endif
