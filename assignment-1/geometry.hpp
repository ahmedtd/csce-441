
#ifndef GEOMETRY
#define GEOMETRY

#include <GL/glut.h>

/// Vector of length 2, that is implicitly interpretable as a vector of 2
/// GLdoubles.
class vec2
{
public:
    /// Component constructor.
    vec2(const double x, const double y);
    
    /// Copy constructor.
    vec2(const vec2 &other);

    /// Extract the x component of this vector.
    double x() const;

    /// Extract the y component of this vector.
    double y() const;

    /// Vector addition operator.
    vec2 operator+(const vec2 &rop) const;

    /// Vecotr subtraction operator.
    vec2 operator-(const vec2 &rop) const;

    /// Conversion operator that allows this vec2 to be interpreted as a vector
    /// of 2 GLdoubles
    operator const GLdouble*() const;

private:
    double mComponents [2];
};

#endif
