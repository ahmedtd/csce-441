
#include <cmath>
using std::pow;
#include <iostream>
using std::cout;

#include <armadillo>
using arma::dot;

#include "sphere.hpp"

sphere::sphere(const vec &position, double radius)
:
    mPosition(position),
    mRadius(radius)
{
}

set<double> sphere::intersect(const ray &viewer)
{   
    double dot_pp = dot(viewer.point, viewer.point);
    double dot_ps = dot(viewer.point, viewer.slope);
    double dot_ss = dot(viewer.slope, viewer.slope);
    double dot_cc = dot(mPosition, mPosition);
    double dot_cs = dot(mPosition, viewer.slope);
    double dot_cp = dot(mPosition, viewer.point);

    double a = dot_ss;
    double b = 2.0*(dot_ps - dot_cs);
    double c = dot_pp - 2*dot_cp + dot_cc - pow(mRadius, 2);

    double det = pow(b,2) - 4*a*c;
    
    //cout << det << " ";

    // If we have two solutions (handle the case of one solution as two really
    // close solutions)
    if(det >= 0.0)
    {
        set<double> solutions;
        solutions.insert((-b + pow(det, 0.5))/(2.0*a));
        solutions.insert((-b - pow(det, 0.5))/(2.0*a));
        
        return solutions;
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
    info.color = fvec("1 0 0 1");

    return info;
}
