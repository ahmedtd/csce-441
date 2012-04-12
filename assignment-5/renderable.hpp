
#ifndef RENDERABLE
#define RENDERABLE

#include <armadillo>
using std::vec;

struct light
{
    vec position;
}

struct ray
{
    vec point;
    vec slope;
};

struct intersection
{
    vec location;
    vec color;
    vec normal;
    vec reflected;
    vec transmitted;
};

class renderable
{
public:
    virtual set<double> intersect(const ray &viewer) = 0;

    virtual intersection propogate(const ray &viewer,
                                   double parameterval,
                                   const set<light> &lights,
                                   const set<renderable*> renderables) = 0;
};

#endif
