
#ifndef AFFINE_RENDERABLE
#define AFFINE_RENDERABLE

#include <set>
using std::set;

#include <armadillo>
using arma::mat;
using arma::vec;

#include "renderable.hpp"

class affine_renderable : public renderable
{
public:
    affine_renderable(const vec &axis,
                      double angle,
                      const vec &scalefactors,
                      const vec &translate,
                      const set<renderable*> children);

    set<intersection> intersect(const ray &viewer) const;
private:
    mat m_rotate;
    mat m_rotate_i;
    
    vec m_scale;
    vec m_scale_i;
    
    vec m_translate;

    set<renderable*> m_children;
};

#endif
