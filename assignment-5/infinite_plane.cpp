
#include <armadillo>
using arma::norm;

#include "infinite_plane.hpp"

infinite_plane::infinite_plane(const vec &point, const vec &normal)
:
    mPoint(point),
    mNormal(normal)
{
    mNormal /= norm(mNormal, 2);
}

set<intersection> infinite_plane::intersect(const ray &viewer) const
{
    intersection sol;
    
    sol.paramval = dot(mNormal, mPoint - viewer.point)
                   / dot(mNormal, viewer.slope);
    sol.generating_ray = viewer;
    sol.target         = this;
    sol.surfnorm       = mNormal;
    sol.surfpos        = viewer.evaluate(sol.paramval); 

    set<intersection> sols;
    sols.insert(sol);
    
    return sols;
}
