
#ifndef LIGHT
#define LIGHT

#include <armadillo>
using arma::vec;
using arma::fvec;

class light
{
public:
    virtual fvec intensity(const vec &surfpos) const = 0;
    virtual vec dirtolight(const vec &surfpos) const = 0;
};

#endif
