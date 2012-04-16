
#include <iostream>
using std::cout;
using std::endl;

#include <armadillo>
using arma::inv;

#include "affine_renderable.hpp"

affine_renderable::affine_renderable(const mat &transform,
                                     const vec &offset,
                                     const set<renderable*> children)
:
    m_transform(transform),
    m_inverse_transform(inv(transform)),
    m_offset(offset),
    m_children(children)
{

    //cout << m_inverse_transform << endl;
    
}

set<intersection> affine_renderable::intersect(const ray &viewer)
{
    // Put viewer into the transformed space
    ray trans_viewer(m_inverse_transform * viewer.point - m_offset,
                     m_inverse_transform * viewer.slope);

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

        //cout << untrans.paramval << endl;
        
        untrans.paramval = trans_it->paramval;

        // untrans.paramval = norm(trans_viewer.slope, 2)
        //                     / norm(viewer.slope, 2)
        //                     * trans_it->paramval;

        // untrans.paramval = norm(viewer.slope, 2)
        //                    / norm(trans_viewer.slope, 2)
        //                    * trans_it->paramval;


        untrans.generating_ray = viewer;
        
        untrans.target = trans_it->target;

        //untrans.surfnorm = trans_it->surfnorm;
        untrans.surfnorm = m_transform.t() * trans_it->surfnorm;
        untrans.surfnorm /= norm(untrans.surfnorm, 2);

        child_intersec.insert(untrans);
    }

    return child_intersec;
}
