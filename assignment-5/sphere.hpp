
#ifndef SPHERE
#define SPHERE

#include "renderable.hpp"

class sphere : public renderable
{
public:
    sphere(const vec &position, double radius);

    virtual set<double> intersect(const ray &viewer);

    virtual intersection propogate(const ray &viewer,
                                   double parameter,
                                   const set<light> &lights,
                                   const set<renderable*> &renderables);
private:
    vec mPosition;
    double mRadius;
};

#endif
