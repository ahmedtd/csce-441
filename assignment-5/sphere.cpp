
#include <cmath>
using std::pow;
using std::sqrt;
#include <iostream>
using std::cout;
using std::endl;

#include <armadillo>
using arma::dot;
using arma::norm;

#include "sphere.hpp"

sphere::sphere(const vec &position, double radius)
:
    mPosition(position),
    mRadius(radius)
    //mMat(mat)
{
}

set<intersection> sphere::intersect(const ray &viewer)
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
    set<intersection> solset;
    if(det >= 0.075)
    {
        intersection solution;
        solution.target = this;

        solution.paramval = (-b + pow(det, 0.5))/(2.0*a);
        solution.generating_ray = viewer;
        solution.surfnorm = 2*viewer.evaluate(solution.paramval) - 2*mPosition;
        solution.surfnorm /= norm(solution.surfnorm, 2);
        solset.insert(solution);

        
        solution.paramval = (-b - pow(det, 0.5))/(2.0*a);
        solution.generating_ray = viewer;
        solution.surfnorm = 2*viewer.evaluate(solution.paramval) - 2*mPosition;
        solution.surfnorm /= norm(solution.surfnorm, 2);
        solset.insert(solution);
    }
    else if(det < 0.075 && det >= 0.0)
    {
        intersection solution;
        solution.target = this;

        solution.paramval = -b/(2.0*a);
        solution.generating_ray = viewer;
        solution.surfnorm = 2*viewer.evaluate(solution.paramval) - 2*mPosition;
        solution.surfnorm /= norm(solution.surfnorm, 2);
        solset.insert(solution);
    }
    
    return solset;

    // vec ray_min_approach = dot((mPosition - viewer.point), viewer.slope)
    //     * viewer.slope
    //     / (dot(viewer.slope, viewer.slope));

    // double ray_min_radius = norm(ray_min_approach,2);
    
    // set<intersection> solset;
    // if(ray_min_radius < mRadius)
    // {
    //     double alpha = sqrt(pow(mRadius,2) - pow(ray_min_radius,2));
        
    //     solution.paramval
    // }
    
    // return solset;
        
}

// fvec sphere::propogate(const ray &viewer,
//                        const intersection &info,
//                        const set<light*> &lights,
//                        const set<renderable*> &renderables)
// {
//     // Get intersection position
//     vec surfpos = viewer.evaluate(info.paramval);

//     // Get surface normal
//     vec surfnorm = 2*surfpos - 2*mPosition;
//     surfnorm /= norm(surfnorm, 2);

//     return mMat.get_color(-viewer.slope,
//                           surfpos,
//                           surfnorm,
//                           lights,
//                           renderables);
// }
