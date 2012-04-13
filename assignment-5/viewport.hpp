
#ifndef VIEWPORT
#define VIEWPORT

#include <armadillo>
using arma::vec;

#include "ray.hpp"

class viewport
{
public:
    viewport(const vec &eyepos,
             const vec &eyeforward,
             const vec &eyeup,
             double hangle,
             double vangle);

    ray genray(double row, double col, double width, double height);

private:
    vec mEyepos;
    vec mEyeforward;
    vec mEyeup;
    vec mEyeright;

    double mHangle;
    double mVangle;

    vec mEyel;
    vec mEyer;
    vec mEyet;
    vec mEyeb;
};

#endif
