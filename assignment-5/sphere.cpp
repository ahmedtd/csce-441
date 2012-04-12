
#include <armadillo>
using arma::dot;

#include "sphere.hpp"

sphere::sphere(const vec &position, double radius)
:
    mPosition(position),
    mRadius(radius)
{
}

set<double> sphere::intersect(const ray viewer)
{
    // (ray.point + t*ray.slope)*(ray.point + t*ray.slope)  = r^2
    // ray.point*(ray.point+t*ray.slope) + t*ray.slope*(ray.point+t*ray.slope)
    // ray.point^2 + 2t*ray.point*ray.slope + t^2*ray.slope^2
    
    double dot_pp = dot(ray.point, ray.point);
    double dot_ps = dot(ray.point, ray.slope);
    double dot_ss = dot(ray.slope, ray.slope);

    double det = pow(2*dot_ps,2) - 4*dot_ss*dot_pp;
    
    // If we have two solutions (handle the case of one solution as two really
    // close solutions)
    if(det >= 0.0)
    {
        set<double> solutions;
        solutions.insert((-2*dot_ps + det)/(2.0*dot_ss));
        solutions.insert((-2*dot_ps - det)/(2.0*dot_ss));
    }
    else
    {
        return (set<double>());
    }
}

intersection sphere::propogate(const ray &viewer,
                               double parameter,
                               const set<light> &lights,
                               const set<renderable*> &renderables)
{
    // Return an ambient color
    intersection info;
    info.color = vec(1.0, 0.0, 0.0);
}
