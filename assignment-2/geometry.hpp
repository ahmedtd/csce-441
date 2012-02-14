
#ifndef GEOMETRY
#define GEOMETRY

#include <ostream>
using std::ostream;

#include <GL/glut.h>

/// Vector of length 2, that is implicitly interpretable as a vector of 2
/// GLdoubles.
class vec2
{
public:
    /// Component constructor.
    vec2(const double x = 1.0, const double y = 0.0);
    
    /// Copy constructor.
    vec2(const vec2 &other);

    void x(double x);
    
    void y(double y);

    /// Extract the x component of this vector.
    double x() const;

    /// Extract the y component of this vector.
    double y() const;

    /// Vector addition operator.
    vec2 operator+(const vec2 &rop) const;

    /// Scalar multiplication operator.
    vec2 operator*(const double rop) const;

    /// Scalar division operator.
    vec2 operator/(const double rop) const;

    /// Vecotr subtraction operator.
    vec2 operator-(const vec2 &rop) const;

    bool operator==(const vec2 &rop) const;

    double norm() const;

    void normalize();

    /// Conversion operator that allows this vec2 to be interpreted as a vector
    /// of 2 GLdoubles
    operator const GLdouble*() const;

private:
    double mComponents [2];
};

vec2 operator*(double lop, const vec2 &rop);

/// Stream insertion operator for vec2 objects
ostream& operator<<(ostream &out, const vec2 &rop);

#endif
