
#ifndef INFINITE_PLANE
#define INFINITE_PLANE

#include <armadillo>
using arma::vec;

#include "renderable.hpp"

class infinite_plane : public renderable
{
public:
    infinite_plane(const vec &point, const vec& normal);

    virtual set<intersection> intersect(const ray &viewer);

private:
    vec mPoint;
    vec mNormal;
};

#endif
