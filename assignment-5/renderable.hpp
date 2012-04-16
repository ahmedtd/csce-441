
#ifndef RENDERABLE
#define RENDERABLE

#include <set>
using std::set;

#include <armadillo>
using arma::vec;
using arma::fvec;

#include "ray.hpp"
#include "light.hpp"

class renderable;

struct intersection
{
    double paramval;
    ray generating_ray;
    renderable *target;
    vec surfnorm;

    bool operator<(const intersection &rop) const;
};

class renderable
{
public:
    virtual set<intersection> intersect(const ray &viewer) = 0;

    // virtual fvec propogate(const ray &viewer,
    //                        const intersection &info,
    //                        const set<light*> &lights,
    //                        const set<renderable*> &renderables) = 0;
};

#endif
