
#ifndef VECOPS
#define VECOPS

#include <armadillo>
using arma::vec;
using arma::mat;

bool epsilon_compare(const vec &a, const vec &b, double epsilon = 0.05);

mat crosspmatrix(const vec &in);

mat rotmat(const vec &axis, double angle);

#endif
