
#ifndef CYLINDER
#define CYLINDER

#include <armadillo>
using arma::mat;
using arma::vec;

#include "renderable.hpp"

class cylinder : public renderable
{
public:
    cylinder(const vec &center,
             const vec &direction,
             double radius,
             double lengtha,
             double lengthb);
    
    virtual set<intersection> intersect(const ray &viewer) const;

private:
    vec m_center;
    vec m_direction;
    
    double m_radius;
    
    double m_length_a;
    double m_length_b;
};

#endif
