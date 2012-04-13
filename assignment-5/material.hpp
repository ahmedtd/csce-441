
#ifndef MATERIAL
#define MATERIAL

#include <set>
using std::set;

#include <armadillo>
using arma::vec;
using arma::fvec;

class material;

#include "renderable.hpp"
#include "light.hpp"
#include "scene.hpp"

class material
{
public:
    
    virtual fvec get_color(const vec &viewdir,
                           const vec &position,
                           const vec &surfnorm,
                           const scene &inscene,
                           int depth) const = 0;
};

#endif
