
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::cos;
using std::sin;
using std::floor;

#include <armadillo>
using arma::mat;
using arma::norm;

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
    mHangle(hangle),
    mVangle(vangle)
{
    //cout << "Viewport constructor" << endl;

    mEyeright = cross(eyeforward, eyeup);

    //cout << rotmat(mEyeup, hangle/2.0) << endl;

    mEyel = rotmat(mEyeup, hangle/2.0) * mEyeforward;
    mEyer = rotmat(mEyeup, -hangle/2.0) * mEyeforward;

    mEyet = rotmat(mEyeright, vangle/2.0) * mEyeforward;
    mEyeb = rotmat(mEyeright, -vangle/2.0) * mEyeforward;
}

ray viewport::genray(double row, double col, double width, double height)
{
    vec hor_comp = -(col - width/2.0) / width * (mEyer - mEyel);
    //cout << "sub 1" << endl;
    vec ver_comp = -(row - height/2.0) / height * (mEyeb - mEyet);
    //cout << "sub 2" << endl;

    // if((int)floor(row)%100 == 0 && (int)floor(col)%100==0)
    // {
    //     cout << hor_comp << endl;
    //     cout << ver_comp << endl;
    //     cout << endl;
    // }

    ray generated(mEyepos, mEyeforward + hor_comp + ver_comp);
    generated.slope /= norm(generated.slope, 2);
    
    return generated;
}
