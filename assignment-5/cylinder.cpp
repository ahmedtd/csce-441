
#include <cmath>
using std::pow;
#include <iostream>
using std::cout;
using std::endl;

#include <armadillo>
using arma::norm;

#include "cylinder.hpp"

cylinder::cylinder(const vec &center,
                   const vec &direction,
                   double radius,
                   double lengtha,
                   double lengthb)
:
    m_center(center),
    m_direction(direction/norm(direction,2)),
    m_radius(radius),
    m_length_a(lengtha),
    m_length_b(lengthb)
{
}

set<intersection> cylinder::intersect(const ray &viewer) const
{
    vec coef_a = viewer.slope - dot(viewer.slope, m_direction)*m_direction;
    vec coef_b = dot((m_center - viewer.point), m_direction)*m_direction
        + viewer.point
        - m_center;

    double a = dot(coef_a, coef_a);
    double b = 2 * dot(coef_a, coef_b);
    double c = dot(coef_b, coef_b) - pow(m_radius, 2);
    
    double det = pow(b,2) - 4*a*c;
    
    set<intersection> intersecs;
    if(det >= 0.0)
    {
        // There are two solutions
        intersection intersec;
        
        intersec.paramval = (-b + pow(det, 0.5)) / (2*a);
        intersec.generating_ray = viewer;
        intersec.target = this;
        intersec.surfpos = viewer.evaluate(intersec.paramval);
        intersec.surfnorm =
            (intersec.surfpos-m_center)
            - dot(intersec.surfpos - m_center, m_direction)*m_direction;
        intersec.surfnorm /= norm(intersec.surfnorm, 2);

        intersecs.insert(intersec);
        
        intersec.paramval = (-b - pow(det,0.5)) / (2*a);
        intersec.generating_ray = viewer;
        intersec.target = this;
        intersec.surfpos = viewer.evaluate(intersec.paramval);
        intersec.surfnorm =
            (intersec.surfpos-m_center)
            - dot(intersec.surfpos - m_center, m_direction)*m_direction;
        intersec.surfnorm /= norm(intersec.surfnorm, 2);

        intersecs.insert(intersec);
    }
    // else if(det < 0.05 && det >= 0.0)
    // {
    //     // There is one solution
    //     intersection intersec;
        
    //     intersec.paramval = - b / (2*a);
    //     intersec.generating_ray = viewer;
    //     intersec.target = this;
    //     intersec.surfpos = viewer.evaluate(intersec.paramval);
    //     intersec.surfnorm =
    //         (intersec.surfpos-m_center)
    //         - dot(intersec.surfpos - m_center, m_direction)*m_direction;
    //     intersec.surfnorm /= norm(intersec.surfnorm, 2);

    //     intersecs.insert(intersec);
    // }

    // post-process the solutions to account for finite length
    set<intersection> corrected_sols;
    set<intersection>::iterator sol_it = intersecs.begin();
    for(; sol_it != intersecs.end(); sol_it++)
    {
        intersection corrected = *sol_it;

        double length = dot(sol_it->surfpos-m_center, m_direction);
        
        double newrad = 0.0;
        if(length < m_length_a)
        {
            vec cap_center = m_center + m_direction * m_length_a;
            
            corrected.surfnorm = -m_direction;

            corrected.paramval =
                dot(corrected.surfnorm, cap_center - viewer.point)
                / dot(corrected.surfnorm, viewer.slope);

            corrected.surfpos = viewer.evaluate(corrected.paramval);
            
            newrad = norm(corrected.surfpos - cap_center, 2);

        }
        
        if(length > m_length_b)
        {
            vec cap_center = m_center + m_direction * m_length_b;

            corrected.surfnorm = m_direction;

            corrected.paramval =
                dot(corrected.surfnorm, cap_center - viewer.point)
                / dot(corrected.surfnorm, viewer.slope);

            corrected.surfpos = viewer.evaluate(corrected.paramval);
            
            newrad = norm(corrected.surfpos - cap_center, 2);
        }

        if(newrad < m_radius)
            corrected_sols.insert(corrected);
    }
    
    return corrected_sols;
}

