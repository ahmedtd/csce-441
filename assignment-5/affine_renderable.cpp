
#include <iostream>
using std::cout;
using std::endl;

#include <armadillo>
using arma::inv;
using arma::eye;
using arma::norm;

#include "vecops.hpp"
#include "affine_renderable.hpp"

affine_renderable::affine_renderable(const vec &axis,
                                     double angle,
                                     const vec &scalefactors,
                                     const vec &translate,
                                     const set<renderable*> children)
:
    m_rotate(rotmat(axis, angle)),
    m_rotate_i(inv(m_rotate)),
    m_scale(scalefactors),
    m_scale_i(3),
    m_translate(translate),
    m_children(children)
{
    m_scale_i(0) = 1.0 / m_scale(0);
    m_scale_i(1) = 1.0 / m_scale(1);
    m_scale_i(2) = 1.0 / m_scale(2);
}

set<intersection> affine_renderable::intersect(const ray &viewer) const
{
    // Put viewer into the transformed space
    ray trans_viewer(m_scale_i % (m_rotate_i * (viewer.point - m_translate)),
                     m_scale_i % (m_rotate_i * viewer.slope));

    set<intersection> child_intersec_transformed;

    // Intersect the transformed ray with each of the children
    set<renderable*>::const_iterator child_it = m_children.begin();
    for(; child_it != m_children.end(); child_it++)
    {
        set<intersection> cur_intersec = (*child_it)->intersect(trans_viewer);
        
        child_intersec_transformed.insert(cur_intersec.begin(),
                                          cur_intersec.end());
    }

    set<intersection> child_intersec;
    
    // Untransform the transformed intersections
    set<intersection>::const_iterator trans_it
        = child_intersec_transformed.begin();
    for(; trans_it != child_intersec_transformed.end(); trans_it++)
    {
        intersection untrans;

        untrans.paramval = trans_it->paramval;
        
        untrans.generating_ray = viewer;
        
        untrans.target = trans_it->target;

        untrans.surfnorm = m_rotate * (m_scale_i % trans_it->surfnorm);
        untrans.surfnorm /= norm(untrans.surfnorm, 2);

        // Perturb the transformed point outwards to combat a loss of numeric
        // precision that occurs in the transform
        vec trans_point = trans_viewer.evaluate(trans_it->paramval);
        untrans.surfpos = m_translate + (m_rotate * (m_scale % trans_point))
            + untrans.surfnorm * 0.05;

        child_intersec.insert(untrans);
    }

    return child_intersec;
}
