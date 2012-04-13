
#ifndef RENDERABLE
#define RENDERABLE

#include <set>
using std::set;

#include <armadillo>
using arma::vec;
using arma::fvec;

struct light
{
    vec position;
};

struct ray
{
    vec point;
    vec slope;
};

struct intersection
{
    vec location;
    fvec color;
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
                                   const set<renderable*> &renderables) = 0;
};

#endif
