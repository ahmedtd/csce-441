
#ifndef SPHERE
#define SPHERE

#include "renderable.hpp"
#include "material.hpp"

class sphere : public renderable
{
public:
    sphere(const vec &position, double radius);

    virtual set<intersection> intersect(const ray &viewer);

    // virtual fvec propogate(const ray &viewer,
    //                        const intersection &info,
    //                        const set<light*> &lights,
    //                        const set<renderable*> &renderables);
private:
    vec mPosition;
    double mRadius;
};

#endif
