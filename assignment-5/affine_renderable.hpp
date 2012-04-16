
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
    affine_renderable(const mat &transform, const vec &offset, const set<renderable*> children);

    set<intersection> intersect(const ray &viewer);
private:
    mat m_transform;
    mat m_inverse_transform;
    
    vec m_offset;

    set<renderable*> m_children;
};

#endif
