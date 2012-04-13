
#include <cmath>
using std::cos;
using std::sin;

#include <armadillo>
using arma::mat;

#include "viewport.hpp"
#include "vecops.hpp"

viewport::viewport(const vec &eyepos,
                   const vec &eyeforward,
                   const vec &eyeup,
                   double hangle,
                   double vangle)
 :
    mEyepos(eyepos),
    mEyeforward(eyeforward),
    mEyeup(eyeup),
    mEyeRight(cross(eyeforward, eyeup)),
    mHangle(hangle),
    mVangle(vangle)
{
    eyel = rotmat(mEyeup, hangle) * mEyeforward;
    eyer = rotmat(mEyeup, -hangle) * mEyeforward;

    eyet = rotmat(mEyeup, vangle) * mEyeforward;
    eyeb = rotmat(mEyeup, -vangle) * mEyeforward;
}

vec viewport::ray(double row, double col, double width, double height)
{
    vec hor_comp = (col - width/2.0) / width * (eyer - eyel);
    vec ver_comp = (row - height/2.0) / height * (eyeb - eyet);

    return mEyepos + mEyeForward + hor_comp + ver_comp;
}
