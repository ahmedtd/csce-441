
#ifndef VIEWPORT
#define VIEWPORT

#include <armadillo>
using arma::vec;

class viewport
{
public:
    viewport(const vec &eyepos,
             const vec &eyeforward,
             const vec &eyeup,
             double hangle,
             double vangle);

    vec ray(double row, double col, double width, double height);

private:
    vec &mEyepos;
    vec &mEyeforward;
    vec &mEyeup;
    vec &mEyeRight;

    double mHangle;
    double mVangle;

    vec eyel;
    vec eyer;
    vec eyet;
    vec eyeb;
};

#endif
