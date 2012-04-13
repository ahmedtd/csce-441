
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::cos;
using std::sin;

#include <armadillo>
using arma::eye;
using arma::norm;
using arma::zeros;

#include "vecops.hpp"

mat crosspmatrix(vec in)
{
    mat out(3,3);
    out.zeros(3,3);
    
    out(0,1) = -in(2);
    out(0,2) = in(1);

    out(1,0) = in(2);
    out(1,2) = -in(0);
    
    out(2,0) = -in(1);
    out(2,1) = in(0);

    return out;
}

mat rotmat(const vec &axis, double angle)
{
    vec naxis = axis / norm(axis, 2);

    mat result(3,3);
    result = (1-cos(angle))*naxis*naxis.t()
        + sin(angle)*crosspmatrix(naxis)
        + eye(3,3)*cos(angle);

    return result;
}
